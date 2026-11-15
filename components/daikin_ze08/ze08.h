#pragma once
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome { namespace daikin_ze08 {
class ZE08Sensor : public PollingComponent, public uart::UARTDevice, public sensor::Sensor {
 public:
  explicit ZE08Sensor(uart::UARTComponent *parent, uint32_t interval)
      : PollingComponent(interval), uart::UARTDevice(parent) {}
  void setup() override {
    const uint8_t set_mode[] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};
    write_array(set_mode, sizeof(set_mode));
    flush();
  }
  void update() override {
    const uint8_t query[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    uint8_t buf[9] = {};
    while (available()) read();
    write_array(query, sizeof(query));
    flush();
    if (!read_array(buf, sizeof(buf))) return;
    if (buf[0] != 0xFF || buf[1] != 0x86) return;
    const uint8_t checksum = uint8_t(~(buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7]) + 1);
    if (buf[8] != checksum) {
      ESP_LOGW("ze08", "Invalid checksum");
      return;
    }
    publish_state((uint16_t(buf[2]) << 8 | buf[3]));
  }
};
} }
