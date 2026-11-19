"""Compatibility shim for HAP-ESPHome's optional PN532 dependency.

This project uses HomeKit sensor accessories only and does not enable HomeKey.
HAP-ESPHome imports the PN532 component while loading its HomeKit schema; the
upstream PN532 module uses newer ESPHome automation APIs than the legacy
custom-sensor components in this project. Keep only the class declaration
needed by HAP's schema. Do not configure pn532 or HomeKey with this shim.
"""
import esphome.codegen as cg

pn532_ns = cg.esphome_ns.namespace("pn532")
PN532 = pn532_ns.class_("PN532", cg.PollingComponent)
