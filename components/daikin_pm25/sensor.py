import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ID, CONF_I2C_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor"]
pm_ns = cg.esphome_ns.namespace("daikin_pm25")
PM25Sensor = pm_ns.class_("PM25Sensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(PM25Sensor, unit_of_measurement="µg/m³", accuracy_decimals=0,
                         device_class="pm25", state_class="measurement")
    .extend(cv.polling_component_schema("1s"))
    .extend({
        cv.Required(CONF_I2C_ID): cv.use_id(i2c.I2CBus),
    }),
)

async def to_code(config):
    cg.add_global(cg.RawStatement('#include "esphome/components/daikin_pm25/pm25.h"'))
    bus = await cg.get_variable(config[CONF_I2C_ID])
    var = cg.new_Pvariable(config[CONF_ID], bus, config[CONF_UPDATE_INTERVAL])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
