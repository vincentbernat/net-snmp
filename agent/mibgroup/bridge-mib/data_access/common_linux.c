#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "common_linux.h"

#include <sys/types.h>
#include <dirent.h>

FILE *
bridge_sysfs_open(const char *bridge, const char *path)
{
    FILE *filep;
    char fpath[SNMP_MAXPATH];

    if (snprintf(fpath, SNMP_MAXPATH, "/sys/class/net/%s/%s",
                 bridge, path) >= SNMP_MAXPATH - 1) {
        DEBUGMSGTL(("access:bridge:sysfs_open", "path too long for %s/%s\n",
                    bridge, path));
        return NULL;
    }

    filep = fopen(fpath, "r");
    if (filep == NULL) {
        DEBUGMSGTL(("access:bridge:sysfs_open", "unable to open %s\n",
                    fpath));
        return NULL;
    }
    return filep;
}

FILE *
bridge_sysfs_open_port(const char *bridge, const char *port, const char *path)
{
    FILE *filep;
    char fpath[SNMP_MAXPATH];

    if (snprintf(fpath, SNMP_MAXPATH, "/sys/class/net/%s/brif/%s/%s",
                 bridge, port, path) >= SNMP_MAXPATH - 1) {
        DEBUGMSGTL(("access:bridge:sysfs_open", "path too long for %s/brif/%s/%s\n",
                    bridge, port, path));
        return NULL;
    }

    filep = fopen(fpath, "r");
    if (filep == NULL) {
        DEBUGMSGTL(("access:bridge:sysfs_open", "unable to open %s\n",
                    fpath));
        return NULL;
    }
    return filep;
}

int
bridge_sysfs_read(const char *bridge, const char *path, u_long *value)
{
    FILE *filep;
    char line[48];
    char *end;

    filep = bridge_sysfs_open(bridge, path);
    if (filep == NULL) {
        DEBUGMSGTL(("access:bridge:sysfs_read", "unable to read value of %s/%s\n",
                    bridge, path));
        return -1;
    }

    end = fgets(line, sizeof(line), filep);
    fclose(filep);
    if (end)
        *value = strtoll(line, &end, 0);
    if (NULL == end || (*end != '\0' && *end != '\n')) {
        DEBUGMSGTL(("access:bridge:sysfs_read", "unable to read value for %s/%s\n",
                    bridge, path));
        return -1;
    }

    return 0;
}

int
bridge_sysfs_read_port(const char *bridge, const char *port, const char *path, u_long *value)
{
    FILE *filep;
    char line[48];
    char *end;

    filep = bridge_sysfs_open_port(bridge, port, path);
    if (filep == NULL) {
        DEBUGMSGTL(("access:bridge:sysfs_read", "unable to read value of %s/brif/%s/%s\n",
                    bridge, port, path));
        return -1;
    }

    end = fgets(line, sizeof(line), filep);
    fclose(filep);
    if (end)
        *value = strtoll(line, &end, 0);
    if (NULL == end || (*end != '\0' && *end != '\n')) {
        DEBUGMSGTL(("access:bridge:sysfs_read", "unable to read value for %s/brif/%s/%s\n",
                    bridge, port, path));
        return -1;
    }

    return 0;
}

int
bridge_sysfs_foreachport(const char *bridge, int(*callback)(const char *, const char *, void *),
    void *arg)
{
    int count = 0;
    char fpath[SNMP_MAXPATH];
    DIR            *brif_dir;
    struct dirent  *brif_entry;

    if (snprintf(fpath, SNMP_MAXPATH,
                 "/sys/class/net/%s/brif", bridge) >= SNMP_MAXPATH - 1) {
        DEBUGMSGTL(("access:bridge:sysfs_read", "path too long for %s\n",
                    bridge));
        return -1;
    }

    if ((brif_dir = opendir(fpath)) == NULL) {
        DEBUGMSGTL(("access:bridge:sysfs_read",
                    "Failed to browse brif directory for %s\n", bridge));
        return -1;
    }

    while ((brif_entry = readdir(brif_dir)) != NULL) {
        if (brif_entry->d_name[0] == '.')
            continue;

        if (callback(bridge, brif_entry->d_name, arg) != 0) {
            DEBUGMSGTL(("access:bridge:sysfs_read",
                        "Failed callback for %s on port %s\n",
                        bridge, brif_entry->d_name));
            return -1;
        }
        count++;
    }

    closedir(brif_dir);
    return count;
}

int
bridge_sysfs_root_id(const char *bridge, char *bridgeid)
{
    int rc;
    FILE *filep;
    filep = bridge_sysfs_open(bridge, "bridge/root_id");

    if (NULL == filep) return 1;

    rc = fscanf(filep, "%02hhx%02hhx.%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
                &bridgeid[0], &bridgeid[1], &bridgeid[2], &bridgeid[3],
                &bridgeid[4], &bridgeid[5], &bridgeid[6], &bridgeid[7]);
    fclose(filep);

    if (8 != rc) {
        DEBUGMSGTL(("access:bridge:bridge_address", "Unable to parse root bridge identifier for %s\n",
                    bridge));
        return 1;
    }

    return 0;
}
