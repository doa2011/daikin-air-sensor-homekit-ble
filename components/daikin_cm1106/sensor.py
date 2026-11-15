import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, CONF_UART_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor"]
cm_ns = cg.esphome_ns.namespace("daikin_cm1106")
CM1106Sensor = cm_ns.class_("CM1106Sensor", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(CM1106Sensor, unit_of_measurement="ppm", accuracy_decimals=0,
                         device_class="carbon_dioxide", state_class="measurement")
    .extend(cv.polling_component_schema("10s"))
    .extend({
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    }),
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], parent, config[CONF_UPDATE_INTERVAL])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
