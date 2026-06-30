# Rolling Dev Station Build Notes

## Current Rig

Rig 1 is the voltage-monitoring proof rig:

1. Stacked Arduino Mega in the 3D-printed case.
2. Proto board mounted on top like the kit photo.
3. 9 V native barrel-jack rail monitored through a resistor divider.
4. `A2` reads the divided voltage.
5. Serial output reports the real supply voltage live.
6. A carbon-fibre gauge image becomes the target visual style for a later voltage needle driven by real OBD2-style voltage data.

## Cabinet and Bench Layout

- The 1.4 m fold-up bench is the main work surface.
- The raised laptop barricade section protects the laptop and keeps it separated from loose wiring.
- The wheeled speaker cabinet rolls under or beside the bench.
- The cabinet carries electronics that should stay wired together:
  - Arduino Mega voltage monitor stack
  - Nakamichi DSP
  - Power distribution
  - Future CAN and ESP32 modules
  - Optional TFT/LVGL or HUD display test hardware

## Quick-Release Connections

Use detachable leads between the bench and cabinet so they can split quickly:

- One labelled USB lead for programming/Serial.
- One fused DC feed for the electronics stack.
- Optional separate audio/DSP cable bundle.
- Strain relief at both ends so rolling the cabinet never tugs directly on boards.

## Expansion Path

1. Confirm stable 9 V rail using the Mega Serial monitor.
2. Add the gauge UI and map the live voltage value to the needle angle.
3. Add CAN/OBD2 input once the power rail is proven.
4. Move from Serial-only output to a TFT/LVGL or RealDash-style HUD screen.
5. Add more boards only after the distro, fusing, and ground layout are documented.
