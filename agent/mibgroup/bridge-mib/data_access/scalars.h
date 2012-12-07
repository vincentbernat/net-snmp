#if defined( linux )
config_require(bridge-mib/data_access/scalars_linux)
config_require(bridge-mib/data_access/common_linux)
#else
config_error(the bridge-mib data access library is not available in this environment.)
#endif

int netsnmp_arch_bridge_scalars_BaseBridgeAddress_get(const char *bridge_name, u_char **value, size_t *varlen);
int netsnmp_arch_bridge_scalars_BaseNumPorts_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpPriority_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpTimeSinceTopologyChange_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpTopChanges_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpDesignatedRoot_get(const char *bridge_name, u_char **value, size_t *varlen);
int netsnmp_arch_bridge_scalars_StpRootCost_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpRootPort_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpMaxAge_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpHelloTime_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpHoldTime_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpForwardDelay_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpBridgeMaxAge_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpBridgeHelloTime_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_StpBridgeForwardDelay_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_TpLearnedEntryDiscards_get(const char *bridge_name, u_long *value);
int netsnmp_arch_bridge_scalars_TpAgingTime_get(const char *bridge_name, u_long *value);
