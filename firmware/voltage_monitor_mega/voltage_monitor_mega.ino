/*
  Rig 1: Voltage Monitor Proto Franken-Rig

  Arduino Mega sketch for proving the barrel-jack supply rail before adding
  CAN, ESP32, TFT/LVGL, or RealDash-style HUD hardware.

  Wiring:
    Barrel jack + ---- R1 30k ---- A2 ---- R2 10k ---- GND
    Barrel jack - ------------------------------------ GND

  The divider ratio is 4:1, so a 9 V input appears as about 2.25 V on A2.
*/

const byte VOLTAGE_SENSE_PIN = A2;

const float ADC_REFERENCE_VOLTS = 5.0;
const float ADC_MAX_COUNT = 1023.0;
const float DIVIDER_R1_OHMS = 30000.0;
const float DIVIDER_R2_OHMS = 10000.0;
const float DIVIDER_RATIO = (DIVIDER_R1_OHMS + DIVIDER_R2_OHMS) / DIVIDER_R2_OHMS;

const unsigned long SAMPLE_INTERVAL_MS = 250;
const byte SAMPLES_PER_READING = 16;
const float SMOOTHING_ALPHA = 0.20;

float smoothedSupplyVolts = 0.0;
unsigned long lastSampleMs = 0;

float readSupplyVoltage() {
  unsigned long rawTotal = 0;

  for (byte sample = 0; sample < SAMPLES_PER_READING; sample++) {
    rawTotal += analogRead(VOLTAGE_SENSE_PIN);
    delay(2);
  }

  const float rawAverage = rawTotal / float(SAMPLES_PER_READING);
  const float sensedVolts = (rawAverage * ADC_REFERENCE_VOLTS) / ADC_MAX_COUNT;
  return sensedVolts * DIVIDER_RATIO;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  smoothedSupplyVolts = readSupplyVoltage();

  Serial.println(F("Rig 1 Voltage Monitor Proto Franken-Rig"));
  Serial.println(F("Pin: A2 | Divider: 30k over 10k | Serial: 115200 baud"));
  Serial.println(F("voltage_v,obd2_pid_42_style_v"));
}

void loop() {
  const unsigned long nowMs = millis();

  if (nowMs - lastSampleMs < SAMPLE_INTERVAL_MS) {
    return;
  }

  lastSampleMs = nowMs;

  const float supplyVolts = readSupplyVoltage();
  smoothedSupplyVolts = (SMOOTHING_ALPHA * supplyVolts) + ((1.0 - SMOOTHING_ALPHA) * smoothedSupplyVolts);

  Serial.print(smoothedSupplyVolts, 2);
  Serial.print(F(","));
  Serial.println(smoothedSupplyVolts, 2);
}
