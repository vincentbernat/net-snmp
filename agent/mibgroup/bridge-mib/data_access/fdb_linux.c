#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <net/if.h>
#include "fdb.h"
#include "common_linux.h"
#include "bridge-mib/dot1dTpFdbTable/dot1dTpFdbTable.h"

/* Stolen from <linux/if_bridge.h> */
struct fdb_entry {
	uint8_t mac_addr[6];
	uint8_t port_no;
	uint8_t is_local;
	uint32_t ageing_timer_value;
	uint8_t port_hi;
	uint8_t pad0;
	uint16_t unused;
};

int
netsnmp_access_bridge_TpFdbTable_container_load(const char *bridge_name,
                                                    netsnmp_container *container)
{
    int count = 0;
    dot1dTpFdbTable_rowreq_ctx *rowreq_ctx = NULL;
    FILE *filep = NULL;
#define CHUNK 10
    struct fdb_entry fe[CHUNK];
    size_t n;

    filep = bridge_sysfs_open(bridge_name, "brforward");
    if (filep == NULL) {
        snmp_log(LOG_ERR, "unable to open FDB for bridge %s\n",
                 bridge_name);
        goto error;
    }

    for (;;) {
        n = fread(fe, sizeof(struct fdb_entry), CHUNK, filep);
        if (n == 0) break;

        while (n--) {
            rowreq_ctx = dot1dTpFdbTable_allocate_rowreq_ctx();
            if (NULL == rowreq_ctx) {
                snmp_log(LOG_ERR, "memory allocation failed\n");
                goto error;
            }
            if (MFD_SUCCESS != dot1dTpFdbTable_indexes_set(rowreq_ctx, (char*)fe[n].mac_addr, 6)) {
                snmp_log(LOG_ERR, "error setting index while loading dot1dTpFdbTable data.\n");
                goto error;
            }

            rowreq_ctx->data.dot1dTpFdbPort = fe[n].port_no;
            rowreq_ctx->data.dot1dTpFdbStatus = fe[n].is_local ?
                DOT1DTPFDBSTATUS_SELF : DOT1DTPFDBSTATUS_LEARNED;

            CONTAINER_INSERT(container, rowreq_ctx);
            count++;
        }

        rowreq_ctx = NULL;
    }

    fclose(filep);

    return count;

error:
    if (rowreq_ctx) dot1dTpFdbTable_release_rowreq_ctx(rowreq_ctx);
    if (filep) fclose(filep);
    return -1;
}
