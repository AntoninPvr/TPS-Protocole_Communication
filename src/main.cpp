#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

// Timing
// =================
#define DELAY_TEMP 2000

uint32_t previous_millis_temp = 0;

// Config
// =================
union AlertConfig {
  uint16_t config;
  uint8_t bytes[2];
  struct {
    uint8_t alert_mode : 1;
    uint8_t alert_polarity : 1;
    uint8_t alert_select : 1;
    uint8_t alert_control : 1;
    uint8_t alert_status : 1;
    uint8_t interrupt_clear : 1;
    uint8_t window_locked : 1;
    uint8_t critical_locked : 1;
    uint8_t shutdown : 1;
    uint8_t t_hyst : 2;
    uint8_t reserved : 5;
  };
};

// Global variables
// =================

// Prototypes
// =================
void enable_alert(uint32_t temp);
uint16_t temp_2_bit(uint16_t temp);

void setup() {
  Serial.begin(115200);
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
  Serial.println("Found MCP9808!");
  tempsensor.setResolution(3);
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
  enable_alert(temp_2_bit(23));
}

void loop() {
  // put your main code here, to run repeatedly:
  tempsensor.wake();
  uint32_t current_millis = millis();
  if (current_millis - previous_millis_temp >= DELAY_TEMP) {
    previous_millis_temp  = current_millis;
    float c = tempsensor.readTempC();
    Serial.print(c);
    Serial.print("C\n");
  }
}

void enable_alert(uint32_t temp) {
  // Set the high and low temp
  union AlertConfig config;
  config.alert_mode = 0;
  config.alert_polarity = 0;
  config.alert_select = 0;
  config.alert_control = 1;
  config.alert_status = 0;
  config.interrupt_clear = 0;
  config.window_locked = 0;
  config.critical_locked = 0;
  config.shutdown = 0;
  config.t_hyst = 0;

  Wire.beginTransmission(0x18);
  Wire.write(0x01);
  Wire.write(config.bytes[1]);
  Wire.write(config.bytes[0]);
  Wire.endTransmission();
  Serial.println("enable altert");

  Wire.beginTransmission(0x18);
  Wire.write(0x02);
  Wire.write(temp);
  Wire.endTransmission();
}

uint16_t temp_2_bit(uint16_t temp) {
  return temp << 4;
}