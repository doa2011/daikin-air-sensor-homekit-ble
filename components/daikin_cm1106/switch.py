import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch, uart
from esphome.const import CONF_ID, CONF_UART_ID

DEPENDENCIES = ["uart"]
cm_ns = cg.esphome_ns.namespace("daikin_cm1106")
CM1106CalibrateSwitch = cm_ns.class_("CM1106CalibrateSwitch", cg.Component, switch.Switch)

CONFIG_SCHEMA = switch.switch_schema(CM1106CalibrateSwitch).extend({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    cg.add_global(cg.RawStatement('#include "esphome/components/daikin_cm1106/cm1106.h"'))
    parent = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], parent)
    await cg.register_component(var, config)
    await switch.register_switch(var, config)
