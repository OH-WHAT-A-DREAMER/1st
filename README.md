# Rig 1: Voltage Monitor Proto Franken-Rig

This repo captures the first bench-safe proof-of-life build for the rolling dev station: an Arduino Mega stack in a 3D-printed case, a proto board mounted on top, and a live voltage monitor on the nominal 9 V barrel-jack input.

## Goal

Prove and calibrate the incoming power rail before adding CAN, ESP32 modules, TFT/LVGL screens, RealDash-style dashboard work, or more complicated power distribution.

## Hardware Concept

- **Controller:** Arduino Mega stack in the 3D-printed case.
- **Top board:** Proto board mounted on top, matching the kit-photo layout.
- **Input being monitored:** Centre-positive barrel-jack supply input, nominally 9 V.
- **Analog sense pin:** `A2`.
- **Display path:** CSV Serial output first; later the filtered value can drive a voltage gauge.

## Voltage Divider

Wire the divider exactly as follows:

```text
Supply + ---- R1 30 kΩ ----+---- A2
                           |
                         R2 10 kΩ
                           |
Supply - / Mega GND -------+---- GND
```

The nominal ratio is 4:1: 9 V becomes 2.25 V at `A2`, and 12 V becomes 3.00 V. The divider's Thevenin resistance is 7.5 kΩ, suitable for the Mega ADC. A 100 nF ceramic capacitor from `A2` to GND is recommended at the Mega/proto board to reject wiring noise.

The maths alone gives a theoretical 20 V full-scale input with a true 5.000 V ADC reference. Treat **16 V as the documented maximum for this prototype**, because the Mega's real 5 V rail varies and the circuit has no surge-rated input protection. This is a rail monitor, not an automotive load-dump input.

Before connecting `A2`:

1. Measure the fitted R1 and R2 values and enter them in the sketch.
2. Power the Mega by USB and measure its `5V` pin to GND; enter that value as `ADC_REFERENCE_VOLTS`.
3. Apply a current-limited 5 V test input to the divider and confirm approximately 1.25 V at `A2` with a multimeter.

> Never connect supply positive directly to an analog pin, and never use the Mega `5V` pin as the 9 V input.

## Firmware

The sketch lives in [`firmware/voltage_monitor_mega/voltage_monitor_mega.ino`](firmware/voltage_monitor_mega/voltage_monitor_mega.ino). It prints:

```text
time_ms,adc_raw,input_v,filtered_v,status
```

The 16-sample average reduces random ADC noise. The `0.20` exponential smoothing value updates every 250 ms and settles in roughly a second—stable enough for a gauge without hiding normal rail changes.

## Rolling Dev Station Plan

The full Franken-Rig combines the fold-up 1.4 m bench with the rolling cabinet. The cabinet carries the Mega stack, Nakamichi DSP, fused power distribution, and later CAN/ESP32/display hardware. Use keyed, labelled quick-release connectors, strain relief, and separate fused branches for logic and high-current/noisy loads. See [`docs/rolling-dev-station.md`](docs/rolling-dev-station.md) for the staged power-up procedure.
