#pragma once
#include <cstring>
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"

namespace esphome { namespace daikin_cm1106 {
class CM1106 : public uart::UARTDevice {
 public:
  explicit CM1106(uart::UARTComponent *parent) : uart::UARTDevice(parent) {}
  bool set_co2_calibration(uint16_t ppm = 400) {
    uint8_t cmd[6] = {0x11, 0x03, 0x03, uint8_t(ppm >> 8), uint8_t(ppm), 0};
    cmd[5] = checksum(cmd, 6);
    uint8_t response[4] = {};
    while (available()) read();
    write_array(cmd, sizeof(cmd)); flush();
    if (!read_array(response, sizeof(response))) return false;
    const uint8_t expected[4] = {0x16, 0x01, 0x03, 0xE6};
    return memcmp(response, expected, sizeof(expected)) == 0;
  }
  int16_t read_co2() {
    const uint8_t cmd[4] = {0x11, 0x01, 0x01, 0xED};
    uint8_t response[8] = {};
    while (available()) read();
    write_array(cmd, sizeof(cmd)); flush();
    if (!read_array(response, sizeof(response))) return -1;
    if (response[0] != 0x16 || response[1] != 0x05 || response[2] != 0x01 ||
        response[7] != checksum(response, sizeof(response))) return -1;
    return (int16_t(response[3]) << 8) | response[4];
  }
 private:
  static uint8_t checksum(const uint8_t *data, size_t len) {
    uint8_t sum = 0;
    for (size_t i = 0; i + 1 < len; i++) sum -= data[i];
    return sum;
  }
};

class CM1106Sensor : public PollingComponent, public sensor::Sensor {
 public:
  CM1106Sensor(uart::UARTComponent *parent, uint32_t interval)
      : PollingComponent(interval), cm1106_(parent) {}
  void update() override {
    const int16_t ppm = cm1106_.read_co2();
    if (ppm >= 0) publish_state(ppm);
    else ESP_LOGW("cm1106", "Failed to read CO2 sensor");
  }
 private:
  CM1106 cm1106_;
};

class CM1106CalibrateSwitch : public Component, public switch_::Switch {
 public:
  explicit CM1106CalibrateSwitch(uart::UARTComponent *parent) : cm1106_(parent) {}
 protected:
  void write_state(bool state) override {
    if (state) {
      const bool ok = cm1106_.set_co2_calibration();
      if (ok) ESP_LOGI("cm1106", "CO2 sensor calibrated to 400 ppm");
      else ESP_LOGW("cm1106", "CO2 calibration command failed");
      publish_state(false);
    } else {
      publish_state(false);
    }
  }
 private:
  CM1106 cm1106_;
};
} }
