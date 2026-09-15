# Rolling Dev Station Build Notes

## Rig 1 Signal Path

1. Nominal 9 V centre-positive supply enters through the barrel jack.
2. A fused logic branch powers the Arduino Mega through its barrel jack/VIN path.
3. A 30 kΩ / 10 kΩ divider samples that rail at `A2`.
4. A 100 nF ceramic capacitor from `A2` to GND filters local wiring noise.
5. USB Serial reports raw, calculated, and filtered voltage for comparison with a multimeter.

## Power and Ground Layout

- Put the fuse close to the DC source, before the cable crosses the cabinet or hinge area.
- For the first Mega-only test, use a 500 mA fuse or a current-limited bench supply set near 250 mA. Increase the branch rating only after measuring the real load.
- Give the Mega/logic and Nakamichi DSP separate fused branches from one documented distribution point.
- Return both branches to the same distribution ground point; do not make the ADC sense return share a long DSP/speaker-current path.
- Keep the `A2` sense pair short and routed away from USB, speaker, motor, relay, and switching-regulator wiring.
- Do not connect an external 9 V rail to the Mega `5V` pin.

## Quick-Release Connections

- Use keyed connectors so positive and ground cannot be reversed.
- Label both ends with voltage, polarity, source, destination, and fuse rating.
- Provide strain relief at the bench and cabinet; rolling or folding must not pull on headers or proto-board joints.
- Use a connector/contact rated above the branch fuse and expected current.
- Prefer a connector arrangement that cannot leave signal wires connected while their ground is disconnected.
- Keep USB/programming separate from DSP/audio cabling and the high-current power bundle.

## Minimum Pre-Power Checks

With all power disconnected:

1. Confirm the supply polarity and actual open-circuit voltage.
2. Confirm no short between supply positive and ground.
3. Measure R1 and R2 in isolation; update the firmware constants if they are not nominal.
4. Confirm continuity: supply negative → Mega GND → R2 ground.
5. Confirm continuity: divider midpoint → `A2`, and no continuity from supply positive directly to `A2`.
6. Inspect that the 100 nF capacitor is from `A2` to GND, not across the incoming supply by mistake.

## Staged Power-Up

1. **USB only:** upload the sketch, open Serial at 115200 baud, and measure the Mega `5V` pin. Update `ADC_REFERENCE_VOLTS` if required.
2. **Divider test only:** use a current-limited 5 V source, common the grounds, and verify about 1.25 V at `A2` before connecting the divider midpoint to the Mega.
3. **A2 connected:** confirm Serial reads within the greater of ±0.10 V or ±2% of the multimeter reading.
4. **9 V barrel input:** current-limit first power-up where possible. Stop if `A2` exceeds 4.0 V, Serial reports `OVER_RANGE`, polarity is wrong, or current is unexpectedly high.
5. **Move/fold test:** power down, operate the bench and cabinet through their full travel, then reinspect strain relief and exposed conductors before re-energising.

## Expansion Gate

Add the gauge UI, CAN/OBD2 interface, TFT/LVGL hardware, ESP32 modules, or DSP only after the voltage reading is calibrated and the fused distribution/ground map is documented. This prototype is limited to 16 V DC and is not protected for direct vehicle transients or load dump.
