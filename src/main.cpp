#include <Arduino.h>
#include <math.h>
#include <Adafruit_BNO08x.h>

#define BNO08X_RESET -1

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

const int pinRx = A0;
const int pinTx = A1;

void setReports();

void setup() {
  Serial.begin(115200);
  delay(1000);  // brief wait instead of blocking on USB

  Serial1.setPins(pinRx, pinTx);
  Serial1.begin(9600);

  Serial.println("Adafruit BNO08x test!");

  if (!bno08x.begin_I2C()) {
    Serial.println("Failed to find BNO08x chip");
    while (1) delay(10);
  }
  Serial.println("BNO08x Found!");
  setReports();
}

void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.println("sensor was reset");
    setReports();
  }
  if (!bno08x.getSensorEvent(&sensorValue)) return;
  if (sensorValue.sensorId != SH2_ACCELEROMETER) return;

  float x = sensorValue.un.accelerometer.x;
  float y = sensorValue.un.accelerometer.y;
  float z = sensorValue.un.accelerometer.z;

  float alpha = atan2(x, sqrt(y * y + z * z)) * RAD_TO_DEG;
  float beta  = atan2(y, z) * RAD_TO_DEG;

  // Angles /100 so values stay small, 5 decimals
  Serial.print(alpha / 100.0, 5);
  Serial.print(",");
  Serial.println(beta / 100.0, 5);

  Serial1.print(alpha / 100.0, 5);
  Serial1.print(",");
  Serial1.println(beta / 100.0, 5);
}

void setReports() {
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) {
    Serial.println("Could not enable accelerometer");
  } else {
    Serial.println("Set accelerometer report... success!");
  }
}