#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net/if.h>
#include "ports.h"
#include "common_linux.h"
#include "bridge-mib/dot1dBasePortTable/dot1dBasePortTable.h"
#include "bridge-mib/dot1dStpPortTable/dot1dStpPortTable.h"
#include "bridge-mib/dot1dTpPortTable/dot1dTpPortTable.h"

#if defined(HAVE_LINUX_RTNETLINK_H)
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif /* HAVE_LINUX_RTNETLINK_H */

/* Stolen from <linux/if_bridge.h> */
#define BR_STATE_DISABLED   0
#define BR_STATE_LISTENING  1
#define BR_STATE_LEARNING   2
#define BR_STATE_FORWARDING 3
#define BR_STATE_BLOCKING   4

static int
populateBasePortTable(const char *bridge_name, const char *port, void *arg)
{
    netsnmp_container *container = arg;
    dot1dBasePortTable_rowreq_ctx *rowreq_ctx;
    int rc;
    u_long dot1dBasePort;
    u_long dot1dBasePortIfIndex;

    rc = bridge_sysfs_read_port(bridge_name, port, "port_no", &dot1dBasePort);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port number for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }
    dot1dBasePortIfIndex = if_nametoindex(port);
    if (dot1dBasePortIfIndex == 0) {
        snmp_log(LOG_ERR, "unable to grab index for port %s\n",
                 port);
        return -1;
    }

    rowreq_ctx = dot1dBasePortTable_allocate_rowreq_ctx();
    if (NULL == rowreq_ctx) {
        snmp_log(LOG_ERR, "memory allocation failed\n");
        return -1;
    }
    if (MFD_SUCCESS != dot1dBasePortTable_indexes_set(rowreq_ctx, dot1dBasePort)) {
        snmp_log(LOG_ERR, "error setting index while loading dot1dBasePortTable data.\n");
        dot1dBasePortTable_release_rowreq_ctx(rowreq_ctx);
        return -1;
    }

    /* No circuit ID, use {0,0} as OID. */
    rowreq_ctx->data.dot1dBasePortCircuit_len = 2;
    memset(rowreq_ctx->data.dot1dBasePortCircuit, 0, 2*sizeof(oid));

    rowreq_ctx->data.dot1dBasePortIfIndex = dot1dBasePortIfIndex;
    rowreq_ctx->data.dot1dBasePortDelayExceededDiscards = 0; /* not available? */
    rowreq_ctx->data.dot1dBasePortMtuExceededDiscards = 0;   /* not available? */

    rowreq_ctx->column_exists_flags |= (COLUMN_DOT1DBASEPORTIFINDEX_FLAG |
                                        COLUMN_DOT1DBASEPORTCIRCUIT_FLAG |
                                        COLUMN_DOT1DBASEPORTDELAYEXCEEDEDDISCARDS_FLAG |
                                        COLUMN_DOT1DBASEPORTMTUEXCEEDEDDISCARDS_FLAG);

    CONTAINER_INSERT(container, rowreq_ctx);
    return 0;
}

static int
populateStpPortTable(const char *bridge_name, const char *port, void *arg)
{
    netsnmp_container *container = arg;
    dot1dStpPortTable_rowreq_ctx *rowreq_ctx;
    int rc, newrow = 0;

    dot1dStpPortTable_mib_index dot1dStpIndex;
    u_long dot1dStpPort;
    u_long dot1dStpPortPriority;
    u_long dot1dStpPortState;
    u_long dot1dStpPortEnable;
    u_long dot1dStpPortPathCost;
    u_char dot1dStpPortDesignatedRoot[8];
    u_long dot1dStpPortDesignatedCost;
    u_char dot1dStpPortDesignatedBridge[8];
    u_char dot1dStpPortDesignatedPort[2];
    u_long dot1dStpPortPathCost32;
    u_long tmp;

    rc = bridge_sysfs_read_port(bridge_name, port, "port_no", &dot1dStpPort);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port number for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }
    dot1dStpIndex.dot1dStpPort = dot1dStpPort;

    rc = bridge_sysfs_read_port(bridge_name, port, "priority", &dot1dStpPortPriority);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port priority for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rc = bridge_sysfs_read_port(bridge_name, port, "state", &dot1dStpPortState);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port state for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }
    switch (dot1dStpPortState) {
    case BR_STATE_DISABLED:   dot1dStpPortState = DOT1DSTPPORTSTATE_DISABLED; break;
    case BR_STATE_LISTENING:  dot1dStpPortState = DOT1DSTPPORTSTATE_LISTENING; break;
    case BR_STATE_LEARNING:   dot1dStpPortState = DOT1DSTPPORTSTATE_LEARNING; break;
    case BR_STATE_FORWARDING: dot1dStpPortState = DOT1DSTPPORTSTATE_FORWARDING; break;
    case BR_STATE_BLOCKING:   dot1dStpPortState = DOT1DSTPPORTSTATE_BLOCKING; break;
    default:
        snmp_log(LOG_ERR, "unknown value for port state of port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rc = bridge_sysfs_read_port(bridge_name, port, "state", &dot1dStpPortEnable);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port status for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }
    switch (dot1dStpPortEnable) {
    case 0:  dot1dStpPortEnable = DOT1DSTPPORTENABLE_DISABLED; break;
    default: dot1dStpPortEnable = DOT1DSTPPORTENABLE_ENABLED; break;
    }

    rc = bridge_sysfs_read_port(bridge_name, port, "path_cost", &dot1dStpPortPathCost32);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port cost for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }
    dot1dStpPortPathCost = (dot1dStpPortPathCost32 > 65535)?65535:dot1dStpPortPathCost32;

    rc = bridge_sysfs_read_port(bridge_name, port, "designated_cost", &dot1dStpPortDesignatedCost);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve designated cost for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    {
        FILE *filep;
        filep = bridge_sysfs_open_port(bridge_name, port, "designated_root");
        if (NULL == filep) return -1;
        rc = fscanf(filep, "%02hhx%02hhx.%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
                    &dot1dStpPortDesignatedRoot[0], &dot1dStpPortDesignatedRoot[1],
                    &dot1dStpPortDesignatedRoot[2], &dot1dStpPortDesignatedRoot[3],
                    &dot1dStpPortDesignatedRoot[4], &dot1dStpPortDesignatedRoot[5],
                    &dot1dStpPortDesignatedRoot[6], &dot1dStpPortDesignatedRoot[7]);
        fclose(filep);
        if (8 != rc) {
            snmp_log(LOG_ERR, "unable to retrieve designated root for port %s on bridge %s\n",
                     port, bridge_name);
            return -1;
        }

        filep = bridge_sysfs_open_port(bridge_name, port, "designated_bridge");
        if (NULL == filep) return -1;
        rc = fscanf(filep, "%02hhx%02hhx.%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
                    &dot1dStpPortDesignatedBridge[0], &dot1dStpPortDesignatedBridge[1],
                    &dot1dStpPortDesignatedBridge[2], &dot1dStpPortDesignatedBridge[3],
                    &dot1dStpPortDesignatedBridge[4], &dot1dStpPortDesignatedBridge[5],
                    &dot1dStpPortDesignatedBridge[6], &dot1dStpPortDesignatedBridge[7]);
        fclose(filep);
        if (8 != rc) {
            snmp_log(LOG_ERR, "unable to retrieve designated bridge for port %s on bridge %s\n",
                     port, bridge_name);
            return -1;
        }
    }

    rc = bridge_sysfs_read_port(bridge_name, port, "designated_port", &tmp);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve designated port for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }
    dot1dStpPortDesignatedPort[0] = (tmp & 0xff00) >> 8;
    dot1dStpPortDesignatedPort[1] = (tmp & 0xff);

    /* We need to check if we have to allocate a new row or if we have an existing one. */
    rowreq_ctx = dot1dStpPortTable_row_find_by_mib_index(&dot1dStpIndex);

    if (NULL == rowreq_ctx) {
        newrow = 1;
        rowreq_ctx = dot1dStpPortTable_allocate_rowreq_ctx();
        DEBUGMSGTL(("access:bridge:bridge_stp", "New port %s for bridge %s\n",
                    port, bridge_name));
    }
    if (NULL == rowreq_ctx) {
        snmp_log(LOG_ERR, "memory allocation failed\n");
        return -1;
    }
    if(MFD_SUCCESS != dot1dStpPortTable_indexes_set(rowreq_ctx, dot1dStpPort)) {
            snmp_log(LOG_ERR,"error setting index while loading dot1dStpPortTable data.\n");
            dot1dStpPortTable_release_rowreq_ctx(rowreq_ctx);
            return -1;;
    }

    rowreq_ctx->data.dot1dStpPortPriority = dot1dStpPortPriority;
    if (!newrow) {
        if ((rowreq_ctx->data.dot1dStpPortState == DOT1DSTPPORTSTATE_LEARNING &&
             dot1dStpPortState == DOT1DSTPPORTSTATE_FORWARDING) ||
            (rowreq_ctx->data.dot1dStpPortState == DOT1DSTPPORTSTATE_FORWARDING &&
             dot1dStpPortState == DOT1DSTPPORTSTATE_BLOCKING))
            rowreq_ctx->rowreq_flags |= DOT1DSTPPORTTABLE_ROW_TRAP;
    }
    rowreq_ctx->data.dot1dStpPortState = dot1dStpPortState;
    rowreq_ctx->data.dot1dStpPortEnable = dot1dStpPortEnable;
    rowreq_ctx->data.dot1dStpPortPathCost = dot1dStpPortPathCost;
    rowreq_ctx->data.dot1dStpPortDesignatedRoot_len = 8;
    memcpy(rowreq_ctx->data.dot1dStpPortDesignatedRoot, dot1dStpPortDesignatedRoot, 8);
    rowreq_ctx->data.dot1dStpPortDesignatedCost = dot1dStpPortDesignatedCost;
    rowreq_ctx->data.dot1dStpPortDesignatedBridge_len = 8;
    memcpy(rowreq_ctx->data.dot1dStpPortDesignatedBridge, dot1dStpPortDesignatedBridge, 8);
    rowreq_ctx->data.dot1dStpPortDesignatedPort_len = 2;
    memcpy(rowreq_ctx->data.dot1dStpPortDesignatedPort, dot1dStpPortDesignatedPort, 2);
    rowreq_ctx->data.dot1dStpPortForwardTransitions = 0; /* not available */
    rowreq_ctx->data.dot1dStpPortPathCost32 = dot1dStpPortPathCost32;

    rowreq_ctx->rowreq_flags &= ~DOT1DSTPPORTTABLE_ROW_DELETED;
    rowreq_ctx->column_exists_flags |= (COLUMN_DOT1DSTPPORT_FLAG |
                                        COLUMN_DOT1DSTPPORTPRIORITY_FLAG |
                                        COLUMN_DOT1DSTPPORTSTATE_FLAG |
                                        COLUMN_DOT1DSTPPORTENABLE_FLAG |
                                        COLUMN_DOT1DSTPPORTPATHCOST_FLAG |
                                        COLUMN_DOT1DSTPPORTDESIGNATEDROOT_FLAG |
                                        COLUMN_DOT1DSTPPORTDESIGNATEDCOST_FLAG |
                                        COLUMN_DOT1DSTPPORTDESIGNATEDBRIDGE_FLAG |
                                        COLUMN_DOT1DSTPPORTDESIGNATEDPORT_FLAG |
/*                                        COLUMN_DOT1DSTPPORTFORWARDTRANSITIONS_FLAG | */
                                        COLUMN_DOT1DSTPPORTPATHCOST32_FLAG);

    if (newrow) CONTAINER_INSERT(container, rowreq_ctx);
    return 0;
}

static int
populateTpPortTable(const char *bridge_name, const char *port, void *arg)
{
    netsnmp_container *container = arg;
    dot1dTpPortTable_rowreq_ctx *rowreq_ctx;
    int rc;
    u_long dot1dTpPort;
    u_long dot1dTpPortMaxInfo;
    u_long dot1dTpPortInFrames;
    u_long dot1dTpPortOutFrames;
    u_long dot1dTpPortInDiscards;

    rc = bridge_sysfs_read_port(bridge_name, port, "port_no", &dot1dTpPort);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port number for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rc = bridge_sysfs_read(port, "mtu", &dot1dTpPortMaxInfo);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve port MTU for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rc = bridge_sysfs_read(port, "statistics/rx_packets", &dot1dTpPortInFrames);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve incoming frames for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rc = bridge_sysfs_read(port, "statistics/tx_packets", &dot1dTpPortOutFrames);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve outgoing frames for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rc = bridge_sysfs_read(port, "statistics/rx_dropped", &dot1dTpPortInDiscards);
    if (0 != rc) {
        snmp_log(LOG_ERR, "unable to retrieve dropped incoming frames for port %s on bridge %s\n",
                 port, bridge_name);
        return -1;
    }

    rowreq_ctx = dot1dTpPortTable_allocate_rowreq_ctx();
    if (NULL == rowreq_ctx) {
        snmp_log(LOG_ERR, "memory allocation failed\n");
        return -1;
    }
    if (MFD_SUCCESS != dot1dTpPortTable_indexes_set(rowreq_ctx, dot1dTpPort)) {
        snmp_log(LOG_ERR, "error setting index while loading dot1dTpPortTable data.\n");
        dot1dTpPortTable_release_rowreq_ctx(rowreq_ctx);
        return -1;
    }

    rowreq_ctx->data.dot1dTpPortMaxInfo = dot1dTpPortMaxInfo;
    rowreq_ctx->data.dot1dTpPortInFrames = dot1dTpPortInFrames;
    rowreq_ctx->data.dot1dTpPortOutFrames = dot1dTpPortOutFrames;
    rowreq_ctx->data.dot1dTpPortInDiscards = dot1dTpPortInDiscards;

    rowreq_ctx->column_exists_flags |= (COLUMN_DOT1DTPPORTMAXINFO_FLAG |
                                        COLUMN_DOT1DTPPORTINFRAMES_FLAG |
                                        COLUMN_DOT1DTPPORTOUTFRAMES_FLAG |
                                        COLUMN_DOT1DTPPORTINDISCARDS_FLAG);

    CONTAINER_INSERT(container, rowreq_ctx);
    return 0;
}

int
netsnmp_access_bridge_BasePortTable_container_load(const char *bridge_name,
                                                   netsnmp_container *container)
{
    return bridge_sysfs_foreachport(bridge_name, populateBasePortTable, container);
}

int
netsnmp_access_bridge_StpPortTable_container_load(const char *bridge_name,
                                                  netsnmp_container *container,
                                                  char *root_bridge_id)
{
    if (0 != bridge_sysfs_root_id(bridge_name, root_bridge_id))
        return -1;
    return bridge_sysfs_foreachport(bridge_name, populateStpPortTable, container);
}

int
netsnmp_access_bridge_TpPortTable_container_load(const char *bridge_name,
                                                 netsnmp_container *container)
{
    return bridge_sysfs_foreachport(bridge_name, populateTpPortTable, container);
}

#ifdef HAVE_LINUX_RTNETLINK_H
static void _netlink_process(int fd, void *data)
{
    int                status;
    char               buf[16384];

    status = recv(fd, buf, sizeof(buf), 0);
    if (status < 0) {
        snmp_log(LOG_ERR,"receive failed\n");
        return;
    }

    /* Skip any complex parsing of the message, we don't really
     * care. Just request a refresh! */
    dot1dStpPortTable_cache_reload();
}

void
netsnmp_access_bridge_StpPortTable_init()
{
    struct {
        struct nlmsghdr nlh;
        struct rtgenmsg g;
    } req;
    struct sockaddr_nl localaddrinfo;
    int status;
    int fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (fd < 0) {
        snmp_log(LOG_ERR, "cannot create netlink socket.\n");
        return -1;
    }

    memset(&localaddrinfo, 0, sizeof(struct sockaddr_nl));

    localaddrinfo.nl_family = AF_NETLINK;
    localaddrinfo.nl_groups = RTNLGRP_LINK;

    if (bind(fd, (struct sockaddr*)&localaddrinfo, sizeof(localaddrinfo)) < 0) {
        snmp_log(LOG_ERR,"cannot bind to netlink socket.\n");
        close(fd);
        return;
    }

    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = sizeof(req);
    req.nlh.nlmsg_type = RTM_GETLINK;
    req.nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
    req.g.rtgen_family = AF_UNSPEC;

    status = send(fd, (void*)&req, sizeof(req), 0);
    if (status < 0) {
        snmp_log(LOG_ERR, "send failed\n");
        close(fd);
        return;
    }

    if (register_readfd(fd, _netlink_process, NULL) != 0) {
        snmp_log(LOG_ERR, "error registering netlink socket\n");
        close(fd);
        return;
    }
}
#else
void
netsnmp_access_bridge_StpPortTable_init()
{
}
#endif
