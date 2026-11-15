#pragma once
#include <map>
#include <vector>
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <hap.h>
#include "esphome/components/homekit/const.h"
namespace esphome { namespace homekit {
class HAPRootComponent : public Component {
 private:
  static constexpr const char *TAG = "HAPRootComponent";
  std::map<AInfo, const char *> accessory_info = {
    {NAME, "ESPHome Bridge"}, {MODEL, "HAP-BRIDGE"}, {SN, "16161616"},
    {MANUFACTURER, "DAIKIN"}, {FW_REV, "1.0"}
  };
 public:
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  void factory_reset();
  HAPRootComponent(const char *setup_code = "159-35-728", const char *setup_id = "ES32",
                   std::map<AInfo, const char *> info = {});
  void setup() override;
  void loop() override;
  void dump_config() override;
};
} }
