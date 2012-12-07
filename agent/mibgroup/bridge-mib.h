/*
 * module to include the modules
 */

config_require(bridge-mib/dot1dBasePortTable);
config_require(bridge-mib/dot1dStpPortTable);
config_require(bridge-mib/dot1dTpFdbTable);
config_require(bridge-mib/dot1dTpPortTable);
config_require(bridge-mib/dot1dBridge_scalars);
config_add_mib(BRIDGE-MIB)
