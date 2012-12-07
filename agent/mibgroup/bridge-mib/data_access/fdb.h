#if defined( linux )
config_require(bridge-mib/data_access/fdb_linux)
config_require(bridge-mib/data_access/common_linux)
#else
config_error(the bridge-mib FDB data access library is not available in this environment.)
#endif

int netsnmp_access_bridge_TpFdbPortTable_container_load(const char *, netsnmp_container *);
