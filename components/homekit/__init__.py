import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, PLATFORM_ESP32
from esphome.core import ID
from . import homekit_base

DEPENDENCIES = ["esp32", "network", "homekit_base"]
AUTO_LOAD = ["homekit_base"]

homekit_ns = homekit_base.homekit_ns
HAPAccessory = homekit_ns.class_("HAPAccessory", cg.Component)
SensorEntity = homekit_ns.class_("SensorEntity")
TemperatureUnits = homekit_ns.enum("TemperatureUnits")

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(HAPAccessory),
        cv.Required("sensor"): cv.ensure_list(cv.Schema({
            cv.Required(CONF_ID): cv.use_id(sensor.Sensor),
            cv.Optional("temp_units", default="CELSIUS"): cv.enum({
                "CELSIUS": TemperatureUnits.CELSIUS,
                "FAHRENHEIT": TemperatureUnits.FAHRENHEIT,
            }),
        })),
    }).extend(cv.COMPONENT_SCHEMA),
    cv.only_on([PLATFORM_ESP32]),
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    for item in config["sensor"]:
        sensor_var = await cg.get_variable(item[CONF_ID])
        cg.Pvariable(
            ID(f"{item[CONF_ID].id}_hk_sensor_entity", type=SensorEntity),
            var.add_sensor(sensor_var, item["temp_units"]),
        )
