#pragma once

#include <cstdint>
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace daikin_pm25 {

class PM25Sensor : public PollingComponent, public sensor::Sensor, public i2c::I2CDevice {
 public:
  PM25Sensor(i2c::I2CBus *bus, uint32_t interval) : PollingComponent(interval) {
    set_i2c_bus(bus);
    set_i2c_address(0x28);
  }

  void update() override {
    // Both PM2005 and PM2105 community drivers request 12 bytes and
    // locate the 0x16 frame header after one leading byte. Some I2C
    // bridges expose the frame from byte 0, so accept either alignment.
    uint8_t data[12] = {};
    if (!read_bytes_raw(data, sizeof(data))) {
      ESP_LOGW(TAG, "I2C read from PM sensor failed");
      return;
    }

    uint8_t offset;
    if (data[0] == FRAME_HEADER) {
      offset = 0;
    } else if (data[1] == FRAME_HEADER) {
      offset = 1;
    } else {
      ESP_LOGW(TAG, "Invalid PM sensor frame header: %02X %02X", data[0], data[1]);
      ESP_LOGD(TAG, "Raw frame: %s", format_hex_pretty(data, sizeof(data)).c_str());
      return;
    }

    // Layout relative to 0x16:
    // +0 header, +1 frame length, +2 sensor status,
    // +3..+4 reserved, +5..+6 PM2.5, +7..+8 PM10.
    const uint8_t status = data[offset + 2];
    if (status != STATUS_COMPLETE) {
      if (status != last_status_) {
        ESP_LOGI(TAG, "PM sensor status: 0x%02X", status);
        last_status_ = status;
      }
      return;
    }

    const uint16_t pm25 = (uint16_t(data[offset + 5]) << 8) | data[offset + 6];
    publish_state(pm25);
    ESP_LOGD(TAG, "PM2.5: %u ug/m3 (header offset %u)", pm25, offset);
    last_status_ = status;
  }

 protected:
  static constexpr const char *TAG = "daikin_pm25";
  static constexpr uint8_t FRAME_HEADER = 0x16;
  static constexpr uint8_t STATUS_COMPLETE = 0x80;
  uint8_t last_status_{0xFF};
};

}  // namespace daikin_pm25
}  // namespace esphome
