#include "HAPAccessory.h"
#include "sensor.hpp"
namespace esphome { namespace homekit {
void HAPAccessory::setup() {
  for (auto *sensor : sensors_) sensor->setup();
}
void HAPAccessory::dump_config() {
  ESP_LOGCONFIG("homekit", "HomeKit sensor accessories: %u", (unsigned) sensors_.size());
}
SensorEntity *HAPAccessory::add_sensor(sensor::Sensor *sensor, TemperatureUnits units) {
  auto *entity = new SensorEntity(sensor);
  sensors_.push_back(entity);
  return entity;
}
} }
