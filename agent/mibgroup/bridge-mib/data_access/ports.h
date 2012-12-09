#if defined( linux )
config_require(bridge-mib/data_access/ports_linux)
config_require(bridge-mib/data_access/common_linux)
#else
config_error(the bridge-mib data access library is not available in this environment.)
#endif

int netsnmp_access_bridge_BasePortTable_container_load(const char *, netsnmp_container *);
int netsnmp_access_bridge_StpPortTable_container_load(const char *, netsnmp_container *, char *);
int netsnmp_access_bridge_TpPortTable_container_load(const char *, netsnmp_container *);
void netsnmp_access_bridge_StpPortTable_init(void);
