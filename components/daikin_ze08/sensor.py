import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_UART_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]
ze_ns = cg.esphome_ns.namespace("daikin_ze08")
ZE08Sensor = ze_ns.class_("ZE08Sensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(ZE08Sensor, unit_of_measurement="µg/m³", accuracy_decimals=0,
                         device_class="volatile_organic_compounds", state_class="measurement")
    .extend(cv.polling_component_schema("5s"))
    .extend({
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    }),
)

async def to_code(config):
    cg.add_global(cg.RawStatement('#include "esphome/components/daikin_ze08/ze08.h"'))
    parent = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], parent, config[CONF_UPDATE_INTERVAL])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
