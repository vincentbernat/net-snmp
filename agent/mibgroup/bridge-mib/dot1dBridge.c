#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "dot1dBridge.h"

char *bridge_name = NULL;

static void
free_bridge_name(void)
{
    free(bridge_name); bridge_name = NULL;
}

static void
parse_bridge_name(const char *token, char *line)
{
    free_bridge_name();
    bridge_name = strdup(line);
}

void
init_dot1dBridge(void)
{
  DEBUGMSGTL(("dot1dBridge", "Initializing\n"));

  snmpd_register_config_handler("bridge", parse_bridge_name, free_bridge_name,
                                "bridge [bridgename]");
}
