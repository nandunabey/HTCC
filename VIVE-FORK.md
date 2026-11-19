# HTCC: VIVE Focus Vision / VIVE Streaming fork

This fork adds what HTCC needs to work with hand tracking on runtimes that don't provide
`XR_FB_hand_tracking_aim`, such as VIVE Streaming through SteamVR. It's tuned for MSFS 2020
in Lock cockpit-interaction mode.

## What's added

- **Pinch detection from hand joints** (`XR_EXT_hand_tracking`). Pinches are detected with
  hysteresis on the thumb-to-fingertip distance. A finger only counts as pinching when it's
  clearly the closest one to the thumb. This is used automatically when the runtime has no
  `XR_FB_hand_tracking_aim`.
- **Remappable finger actions.** The index finger is always click/grab. VIVE's tracking reports
  a middle-finger pinch as an index pinch, so by default the middle finger does nothing and the
  **ring finger is the secondary action (push knob)**.
- **Pinch and twist to turn knobs.** While you hold an index pinch (which grabs the knob in MSFS
  Lock mode), rolling your wrist rolls the virtual controller. MSFS turns the grabbed knob to
  match.
- **Pointer lookback on pinch.** When a pinch starts, the pointer locks to where it was
  `PinchLookbackMilliseconds` earlier. That's before the pinching fingers started dragging the
  hand off the button.
- **Ignore after re-acquire.** Gestures are ignored for `HandTrackingReacquireMilliseconds`
  after tracking picks a hand back up, because the first frames are often wrong.

## Settings

These are registry values under `HKLM\SOFTWARE\Fred Emmott\HandTrackedCockpitClicking`. You
can also set them per game under `...\AppOverrides\FlightSimulator.exe`.

| Value | Type | Default | Meaning |
|---|---|---|---|
| `JointPinchDetection` | DWORD | 0 | 0 = auto (only if no FB aim extension), 1 = always, 2 = never |
| `JointPinchPressDistance` | string (float, metres) | 0.015 | Pinch starts below this thumb–fingertip distance |
| `JointPinchReleaseDistance` | string (float, metres) | 0.030 | Pinch ends above this distance |
| `JointPinchFingerMargin` | string (float, metres) | 0.008 | The pinching finger must be this much closer than the next finger |
| `JointPinchMiddleAction` | DWORD | 0 | 0 none, 1 secondary (push), 2 scroll decrease, 3 scroll increase |
| `JointPinchRingAction` | DWORD | 1 | Same values as above |
| `JointPinchLittleAction` | DWORD | 0 | Same values as above |
| `HandTrackingReacquireMilliseconds` | DWORD | 120 | Gestures are ignored for this long after a hand is re-acquired |
| `PinchLookbackMilliseconds` | DWORD | 150 | How far back the pointer locks when a pinch starts |
| `TwistToTurn` | DWORD | 1 | Pinch and twist wrist to turn knobs |
| `TwistToTurnInvert` | DWORD | 0 | Set to 1 if knobs turn the wrong way |
| `TwistToTurnGain` | string (float) | 1.5 | Virtual controller roll per unit of wrist roll |
| `TwistToTurnDeadzone` | string (float, radians) | 0.06 | Wrist roll ignored before turning starts |

The defaults come from a Phase 0 recording on a Focus Vision streaming through VIVE Hub 2.x
and SteamVR 2.18. With an index pinch, the thumb–index distance was about 0.7 cm pinched versus
about 5 cm open.

## MSFS setup

- Options > General > Accessibility > **Cockpit Interaction System: Lock**.
- In HTCC Settings, turn on **Enable HTCC**, set the hand tracking method to **OpenXR hand
  tracking**, and turn on **Pinch to click** and **Pinch and twist wrist to turn knobs**.
