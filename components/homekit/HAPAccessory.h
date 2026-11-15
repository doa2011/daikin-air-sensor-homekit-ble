#pragma once
#include <vector>
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "const.h"
namespace esphome { namespace homekit {
class SensorEntity;
class HAPAccessory : public Component {
 public:
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI + 1; }
  void setup() override;
  void dump_config() override;
  SensorEntity *add_sensor(sensor::Sensor *sensor, TemperatureUnits units);
 protected:
  std::vector<SensorEntity *> sensors_;
};
} }
