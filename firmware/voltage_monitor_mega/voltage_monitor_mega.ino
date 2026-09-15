/*
  Rig 1: Voltage Monitor Proto Franken-Rig

  Wiring:
    Supply + ---- R1 30k ---- A2 ---- R2 10k ---- GND
    Supply - ------------------------------- Mega GND
    Recommended: 100 nF ceramic from A2 to GND.

  Calibrate ADC_REFERENCE_VOLTS and the two resistor values with a multimeter.
  Prototype limit: 16 V DC. This input is not protected against vehicle transients.
*/

const byte VOLTAGE_SENSE_PIN = A2;

// Bench calibration values: replace with measured values before final use.
const float ADC_REFERENCE_VOLTS = 5.000;
const float DIVIDER_R1_OHMS = 30000.0;
const float DIVIDER_R2_OHMS = 10000.0;
const float ADC_MAX_COUNT = 1023.0;
const float DIVIDER_RATIO =
    (DIVIDER_R1_OHMS + DIVIDER_R2_OHMS) / DIVIDER_R2_OHMS;

const float DOCUMENTED_MAX_INPUT_VOLTS = 16.0;
const int ADC_OVER_RANGE_COUNT = 820;  // About 4.0 V at A2 with a 5 V reference.
const unsigned long SAMPLE_INTERVAL_MS = 250;
const byte SAMPLES_PER_READING = 16;
const float SMOOTHING_ALPHA = 0.20;

struct VoltageReading {
  uint16_t rawAverage;
  float inputVolts;
};

float smoothedSupplyVolts = 0.0;
unsigned long lastSampleMs = 0;

VoltageReading readSupplyVoltage() {
  unsigned long rawTotal = 0;

  // Discard one conversion after startup/mux settling, then average 16 samples.
  analogRead(VOLTAGE_SENSE_PIN);
  for (byte sample = 0; sample < SAMPLES_PER_READING; sample++) {
    rawTotal += analogRead(VOLTAGE_SENSE_PIN);
    delay(2);
  }

  VoltageReading reading;
  reading.rawAverage =
      (rawTotal + (SAMPLES_PER_READING / 2)) / SAMPLES_PER_READING;
  const float sensedVolts =
      (reading.rawAverage * ADC_REFERENCE_VOLTS) / ADC_MAX_COUNT;
  reading.inputVolts = sensedVolts * DIVIDER_RATIO;
  return reading;
}

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);

  const VoltageReading initial = readSupplyVoltage();
  smoothedSupplyVolts = initial.inputVolts;

  Serial.println(F("Rig 1 Voltage Monitor Proto Franken-Rig"));
  Serial.println(F("Calibrate ADC reference and resistor values before final use."));
  Serial.println(F("time_ms,adc_raw,input_v,filtered_v,status"));
}

void loop() {
  const unsigned long nowMs = millis();
  if (nowMs - lastSampleMs < SAMPLE_INTERVAL_MS) {
    return;
  }
  lastSampleMs = nowMs;

  const VoltageReading reading = readSupplyVoltage();
  smoothedSupplyVolts =
      (SMOOTHING_ALPHA * reading.inputVolts) +
      ((1.0 - SMOOTHING_ALPHA) * smoothedSupplyVolts);

  const bool overRange =
      reading.rawAverage >= ADC_OVER_RANGE_COUNT ||
      reading.inputVolts > DOCUMENTED_MAX_INPUT_VOLTS;

  Serial.print(nowMs);
  Serial.print(F(","));
  Serial.print(reading.rawAverage);
  Serial.print(F(","));
  Serial.print(reading.inputVolts, 3);
  Serial.print(F(","));
  Serial.print(smoothedSupplyVolts, 3);
  Serial.print(F(","));
  Serial.println(overRange ? F("OVER_RANGE") : F("OK"));
}
