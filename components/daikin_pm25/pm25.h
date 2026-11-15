#pragma once
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome { namespace daikin_pm25 {
class PM25Sensor : public PollingComponent, public sensor::Sensor, public i2c::I2CDevice {
 public:
  PM25Sensor(i2c::I2CBus *bus, uint32_t interval) : PollingComponent(interval) {
    set_i2c_bus(bus);
    set_i2c_address(0x28);
  }
  void update() override {
    uint8_t data[12] = {};
    if (!read_bytes_raw(data, sizeof(data))) {
      ESP_LOGW("pm25", "I2C read from PM sensor failed");
      return;
    }
    const uint8_t status = data[2];
    if (status != 0x80) return;
    const uint16_t value = (uint16_t(data[5]) << 8) | data[6];
    publish_state(value);
  }
};
} }
