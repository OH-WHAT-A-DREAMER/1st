# Rig 1: Voltage Monitor Proto Franken-Rig

This repo captures the first safe proof-of-life build for the rolling dev station: an Arduino Mega stack in a 3D-printed case, a proto board mounted on top, and a live voltage monitor on the native 9 V barrel-jack input.

## Goal

Prove the incoming power rail before adding CAN, ESP32 modules, TFT/LVGL screens, RealDash-style HUD work, or more complicated power distribution.

## Hardware Concept

- **Controller:** Arduino Mega stack in the photographed 3D-printed case.
- **Top board:** Proto board mounted on top, matching the kit-photo layout.
- **Input being monitored:** Barrel-jack supply input, nominally 9 V.
- **Analog sense pin:** `A2`.
- **Display path:** Serial output first; later the same value can drive an OBD2-style voltage gauge needle.
- **Target look:** Use one of the carbon-fibre gauge images as the visual reference for the final voltage HUD.

## Voltage Divider

Use a resistor divider so the Mega never sees more than 5 V on `A2`.

Recommended divider for a 9 V nominal input:

- `R1 = 30 kΩ` from barrel-jack positive to `A2`
- `R2 = 10 kΩ` from `A2` to GND
- Common ground between barrel jack, Mega, and proto board

This creates a 4:1 divider, so 12 V at the input becomes about 3 V at `A2`, leaving safe headroom.

> Do not connect the barrel-jack positive directly to an analog pin.

## Firmware

The Arduino sketch lives in [`firmware/voltage_monitor_mega/voltage_monitor_mega.ino`](firmware/voltage_monitor_mega/voltage_monitor_mega.ino).

It reads `A2`, converts the ADC value back to real supply voltage using the divider ratio, smooths the reading, and prints live voltage over Serial.

## Rolling Dev Station Plan

The full Franken-Rig combines the fold-up bench with the rolling cabinet:

- Fold-up 1.4 m bench becomes the main work surface when unfolded.
- Raised laptop barricade section protects the laptop area.
- Wheeled speaker cabinet rolls underneath or beside the bench.
- Cabinet carries the Arduino Mega stack, Nakamichi DSP, and power distribution.
- Quick-release power and USB leads let the bench and cabinet separate quickly.
- Future expansion space is reserved for more Arduino/ESP32 boards, CAN interfaces, TFT/LVGL screens, and RealDash-style HUD testing.
