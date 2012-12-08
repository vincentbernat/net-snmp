#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "scalars.h"
#include "common_linux.h"

int
netsnmp_arch_bridge_scalars_BaseBridgeAddress_get(const char *bridge_name, u_char **value, size_t *varlen)
{
    static u_char mac[6];
    u_long priority;
    int rc;
    FILE *filep;
    filep = bridge_sysfs_open(bridge_name, "bridge/bridge_id");

    if (NULL == filep) return 1;

    rc = fscanf(filep, "%lx.%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
                &priority, &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
    fclose(filep);

    if (7 != rc) {
        DEBUGMSGTL(("access:bridge:bridge_address", "Unable to parse bridge address for %s\n",
                    bridge_name));
        return 1;
    }

    *value = mac;
    *varlen = 6;
    return 0;
}

static int
increment(const char *bridge_name, const char *port, void *arg)
{
    u_long *value = arg;
    DEBUGMSGTL(("access:bridge:bridge_ports", "Found port %s in bridge %s\n",
                port, bridge_name));
    (*value)++;
    return 0;
}

int
netsnmp_arch_bridge_scalars_BaseNumPorts_get(const char *bridge_name, u_long *value)
{
    int rc;
    *value = 0;
    rc = bridge_sysfs_foreachport(bridge_name, increment, value);
    return (rc < 0)?1:0;
}

int
netsnmp_arch_bridge_scalars_StpPriority_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/priority", value);
}

int
netsnmp_arch_bridge_scalars_StpTimeSinceTopologyChange_get(const char *bridge_name, u_long *value)
{
    return 1;                   /* not available? */
}

int
netsnmp_arch_bridge_scalars_StpTopChanges_get(const char *bridge_name, u_long *value)
{
    return 1;                   /* not available? */
}

int
netsnmp_arch_bridge_scalars_StpDesignatedRoot_get(const char *bridge_name, u_char **value, size_t *varlen)
{
    static u_char bridgeid[8];
    if (0 != bridge_sysfs_root_id(bridge_name, bridgeid))
        return 1;

    *value = bridgeid;
    *varlen = 8;
    return 0;
}

int
netsnmp_arch_bridge_scalars_StpRootCost_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/root_path_cost", value);
}

int
netsnmp_arch_bridge_scalars_StpRootPort_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/root_port", value);
}

int
netsnmp_arch_bridge_scalars_StpMaxAge_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/max_age", value);
}

int
netsnmp_arch_bridge_scalars_StpHelloTime_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/hello_time", value);
}

int
netsnmp_arch_bridge_scalars_StpHoldTime_get(const char *bridge_name, u_long *value)
{
    return 1;                    /* not available? */
}

int
netsnmp_arch_bridge_scalars_StpForwardDelay_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/forward_delay", value);
}

int
netsnmp_arch_bridge_scalars_StpBridgeMaxAge_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/max_age", value);
}

int
netsnmp_arch_bridge_scalars_StpBridgeHelloTime_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/hello_time", value);
}

int
netsnmp_arch_bridge_scalars_StpBridgeForwardDelay_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/forward_delay", value);
}

int
netsnmp_arch_bridge_scalars_TpLearnedEntryDiscards_get(const char *bridge_name, u_long *value)
{
    return 1;                   /* not available? */
}

int
netsnmp_arch_bridge_scalars_TpAgingTime_get(const char *bridge_name, u_long *value)
{
    return bridge_sysfs_read(bridge_name, "bridge/ageing_time", value);
}
