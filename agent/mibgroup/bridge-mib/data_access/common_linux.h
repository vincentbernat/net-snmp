/*
 * Methods to access bridge data for Linux (using sysfs)
 */

FILE *bridge_sysfs_open(const char *bridge, const char *path);
int   bridge_sysfs_read(const char *bridge, const char *path, u_long *value);
FILE *bridge_sysfs_open_port(const char *bridge, const char *path, const char *port);
int   bridge_sysfs_read_port(const char *bridge, const char *path, const char *port, u_long *value);
int   bridge_sysfs_foreachport(const char *bridge,
                               int(*callback)(const char *, const char *, void *),
                               void*);
int   bridge_sysfs_root_id(const char *bridge, char *root_bridge_id);
