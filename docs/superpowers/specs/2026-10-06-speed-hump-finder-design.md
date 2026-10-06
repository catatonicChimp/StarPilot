# Speed hump finder: design

Date: 2026-10-06
Status: design agreed, not built

## Goal

An offline tool that scans openpilot drive logs, finds places that look like speed humps, and builds a local review page. Confirmed humps export as OpenStreetMap data (`traffic_calming=hump` etc.) for adding to OSM. Longer term, the labelled set could help comma (or the model) learn about humps.

Version 1 runs on a computer only. It never runs on the car and never changes driving behaviour.

It must work on logs from any openpilot fork: the core uses only standard openpilot messages.

## Location

- Code: `tools/speed_humps/` (standard openpilot tools folder, so it can be offered upstream unchanged).
- Output: `.analysis/humps/` (git-excluded). Drive locations, frames, the review page and labels never get committed or published.
- The code holds no real locations. Tests use synthetic traces.

## Parts

| File | Job | In → out |
|---|---|---|
| `extract.py` | Pull needed signals from one rlog segment | rlog → cached per-segment arrays: accelerometer + gyroscope (104 Hz), vEgo, brake/gas, GPS, calibration, wheelbase |
| `detect.py` | Find bump events in one segment | signals → events (time, lat/lon, speed, jolt gap, pitch, roll, slow-down, score 0-1, reasons) |
| `cluster.py` | Group events across drives into places | events + GPS tracks → places (position, passes, flagged passes, confidence; multi-hump groups) |
| `frames.py` | Camera frames for each place | qcamera (fcamera if uploaded) → frame ~3 s before + frame at the hump, best few passes |
| `review.py` | Build review page, read answers back | places → `review.html`; exported answers → `labels.json` |
| `export.py` | Confirmed places to OSM | `labels.json` → `.osm` + GeoJSON, one node per hump |
| `run.py` | One command | route list or rlog folder → finished review page |

Logs and qcamera download via the comma API and are cached. One worker by default, max 2-3 (Mac memory limit).

## Signals and frame

- Accelerometer and gyroscope come from rlogs at 104 Hz. qlogs only have 1 Hz, so they're not usable.
- Device readings rotate into the car frame using `liveCalibration` (rpyCalib) and the gravity direction: vertical accel, pitch rate, roll rate.
- Wheelbase comes from `carParams`, falling back to 2.7 m.
- Speed is `carState.vEgo`; pedals are `carState.brakePressed` / `gasPressed`; position is `gpsLocationExternal` (10 Hz), falling back to `gpsLocation`.
- Fork differences in message names (e.g. `liveLocationKalman` vs `livePose`) are handled in `extract.py` only.
- Car-specific CAN extras (e.g. VW wheel speeds) are optional plug-ins, not required.

## Detection (per segment)

Only considered at 3-60 km/h with GPS accuracy ≤15 m.

1. **Jolts:** band-pass vertical accel (about 1-15 Hz) and find peaks above a threshold (start 1.5 m/s²).
2. **Front/rear pairing:** a front jolt pairs with a rear jolt at wheelbase / v (±25%). Jolts are processed in time order, and a jolt used in a pair cannot pair again. This keeps back-to-back humps (front, rear, front, rear) as two events instead of cross-pairing them.
3. **Pitch:** nose-up then nose-down on the front hit, the reverse on the rear.
4. **Low roll:** a hump lifts both wheels on an axle at once; a pothole or cover usually catches one wheel and makes the car roll. High roll lowers the score.
5. **Driver behaviour:** slowed in the 8 s before (drop and minimum speed), then sped up again after.

The score is a 0-1 weighted combination with plain-English reasons (e.g. "double jolt 0.37 s apart at 25 km/h (expected 0.38), low roll, slowed 52→24"). Weights start hand-set and get tuned on known humps.

Event position is GPS interpolated to the front-wheel hit.

## Grouping into places

- An event joins the nearest existing hump within about 15 m. Direction of travel is ignored.
- Events from the **same pass** never merge into one hump: they are physically separate.
- For each hump, every pass over it is counted (from GPS tracks), flagged or not, giving "flagged on X of Y passes".
- Confidence combines the best event score with the hit rate.
- Position is the mean over flagged passes.
- Humps repeatedly found close together on the same passes (e.g. a double hump) form a **group**: one review card, but each hump exports as its own OSM node.

## Review page

`.analysis/humps/review.html`, opened locally, never published as an artifact.

- **Overview map** with every place as a pin coloured by confidence. Tiles come from OSM, which sends the map area to OSM's tile servers but no list of places. A plain-plot option avoids tiles entirely.
- **One card per place or group,** sorted by confidence:
  - frame ~3 s before and frame at the hump;
  - jolt/pitch/roll chart;
  - flagged X of Y passes, score reasons, comma connect link.
- **Controls:** Yes / No / Unsure, a type (hump, table, cushion, other), and notes.
- **Saving:** answers save in the page, and Export writes `labels.json`. Re-runs keep earlier answers and only add new places.

## Edge cases

- Segments with missing or corrupt logs, no calibration, or no GPS are skipped and listed in an end-of-run summary.
- Crawls under 3 km/h are ignored.
- False alarms from rail crossings, grates and rough roads are expected. Review and pass counting handle them.

## Testing

- **Unit tests (synthetic traces):**
  - clean double jolt;
  - **back-to-back double hump** (must give two events, not a cross-pair);
  - one-wheel pothole (high roll);
  - single jolt without a rear hit;
  - noise only.
- **Grouping:** fixed examples, including a same-pass double hump staying as two humps in one group.
- **Export:** a fixed `labels.json` produces the expected `.osm` / GeoJSON.
- **Real-world check:** the user's known humps near work (Tenerry Cres, Noone St, South Terrace, including the South Terrace double humps).

## Success for version 1

- Known humps appear as places flagged on most passes, near the top of the list. The South Terrace double comes out as two humps in one group.
- After grouping, no more than about 2 rejected places per confirmed place.

## Not in version 1

- A learned classifier trained on the review labels (planned next step).
- On-car detection, or slowing the car for known humps.
- Proposing the tool to comma (after it's proven).
