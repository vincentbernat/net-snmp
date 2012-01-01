/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 14170 $ of $ 
 *
 * $Id:$
 */
/*
 * standard Net-SNMP includes 
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "dot3StatsTable.h"
#include "dot3StatsTable_data_access.h"

#if defined(linux)
#include "ioctl_imp_common.h"
#endif

/** @ingroup interface
 * @addtogroup data_access data_access: Routines to access data
 *
 * These routines are used to locate the data used to satisfy
 * requests.
 * 
 * @{
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table dot3StatsTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * EtherLike-MIB::dot3StatsTable is subid 2 of dot3.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.10.7.2, length: 9
 */

/**
 * initialization for dot3StatsTable data access
 *
 * This function is called during startup to allow you to
 * allocate any resources you need for the data table.
 *
 * @param dot3StatsTable_reg
 *        Pointer to dot3StatsTable_registration
 *
 * @retval MFD_SUCCESS : success.
 * @retval MFD_ERROR   : unrecoverable error.
 */
int
dot3StatsTable_init_data(dot3StatsTable_registration * dot3StatsTable_reg)
{
    DEBUGMSGTL(("verbose:dot3StatsTable:dot3StatsTable_init_data",
                "called\n"));

    /*
     * TODO:303:o: Initialize dot3StatsTable data.
     */

    return MFD_SUCCESS;
}                               /* dot3StatsTable_init_data */

/**
 * container overview
 *
 */

/**
 * container initialization
 *
 * @param container_ptr_ptr A pointer to a container pointer. If you
 *        create a custom container, use this parameter to return it
 *        to the MFD helper. If set to NULL, the MFD helper will
 *        allocate a container for you.
 * @param  cache A pointer to a cache structure. You can set the timeout
 *         and other cache flags using this pointer.
 *
 *  This function is called at startup to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases. If no custom
 *  container is allocated, the MFD code will create one for your.
 *
 *  This is also the place to set up cache behavior. The default, to
 *  simply set the cache timeout, will work well with the default
 *  container. If you are using a custom container, you may want to
 *  look at the cache helper documentation to see if there are any
 *  flags you want to set.
 *
 * @remark
 *  This would also be a good place to do any initialization needed
 *  for you data source. For example, opening a connection to another
 *  process that will supply the data, opening a database, etc.
 */
void
dot3StatsTable_container_init(netsnmp_container ** container_ptr_ptr,
                              netsnmp_cache * cache)
{
    DEBUGMSGTL(("verbose:dot3StatsTable:dot3StatsTable_container_init",
                "called\n"));

    if (NULL == container_ptr_ptr) {
        snmp_log(LOG_ERR,
                 "bad container param to dot3StatsTable_container_init\n");
        return;
    }

    /*
     * For advanced users, you can use a custom container. If you
     * do not create one, one will be created for you.
     */
    *container_ptr_ptr = NULL;

    if (NULL == cache) {
        snmp_log(LOG_ERR,
                 "bad cache param to dot3StatsTable_container_init\n");
        return;
    }

    /*
     * TODO:345:A: Set up dot3StatsTable cache properties.
     *
     * Also for advanced users, you can set parameters for the
     * cache. Do not change the magic pointer, as it is used
     * by the MFD helper. To completely disable caching, set
     * cache->enabled to 0.
     */
    cache->timeout = DOT3STATSTABLE_CACHE_TIMEOUT;      /* seconds */
}                               /* dot3StatsTable_container_init */

/**
 * container shutdown
 *
 * @param container_ptr A pointer to the container.
 *
 *  This function is called at shutdown to allow you to customize certain
 *  aspects of the access method. For the most part, it is for advanced
 *  users. The default code should suffice for most cases.
 *
 *  This function is called before dot3StatsTable_container_free().
 *
 * @remark
 *  This would also be a good place to do any cleanup needed
 *  for you data source. For example, closing a connection to another
 *  process that supplied the data, closing a database, etc.
 */
void
dot3StatsTable_container_shutdown(netsnmp_container * container_ptr)
{
    DEBUGMSGTL(("verbose:dot3StatsTable:dot3StatsTable_container_shutdown",
                "called\n"));

    if (NULL == container_ptr) {
        snmp_log(LOG_ERR,
                 "bad params to dot3StatsTable_container_shutdown\n");
        return;
    }

}                               /* dot3StatsTable_container_shutdown */

/**
 * load initial data
 *
 * TODO:350:M: Implement dot3StatsTable data load
 * This function will also be called by the cache helper to load
 * the container again (after the container free function has been
 * called to free the previous contents).
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  dot3StatsTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */

/*
 *
 * @retval  MFD_SUCCESS success
 * @retval  MFD_ERROR could not get the interface names 
 * @retval  MFD_RESOURCE_UNAVAILABLE failed to allocate memory
 * @retval  -2 could not open a socket
 */


int
dot3StatsTable_container_load(netsnmp_container * container)
{
    dot3StatsTable_rowreq_ctx *rowreq_ctx;
    size_t          count = 0;
    
    DEBUGMSGTL(("verbose:dot3StatsTable:dot3StatsTable_container_load",
                "called\n"));
      
    /*
     * TODO:352:M: |   |-> set indexes in new dot3StatsTable rowreq context.
     * data context will be set from the param (unless NULL,
     * in which case a new data context will be allocated)
     */
    
    /*
     * temporary storage for index values
     */

    /*
     * dot3StatsIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/A/w/e/R/d/H
     */

    long            dot3StatsIndex;
    int             fd;
    int             rc = 0, retval = 0;

#if defined(linux)
    struct ifname *list_head = NULL, *p = NULL;
#endif

    /*
     * create socket for ioctls
     */
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0) {
        snmp_log(LOG_ERR, "could not create socket\n");
        return -2;
    }

    /*
     * get the interface names of the devices present in the system, in case of failure retval suggests the reson for failure
     * and list_head contains null
     */

#if defined(linux)
    list_head = dot3stats_interface_name_list_get (list_head, &retval);

    if (!list_head) {
        snmp_log (LOG_ERR, "access:dot3StatsTable, error getting the interface names present in the system\n");
        DEBUGMSGTL(("access:dot3StatsTable", "error getting the interface names present in the system"));
        close(fd);
        return MFD_ERROR;
    }
    
    /*
     * Walk over the list of interface names present in the system and retreive the statistics 
     */

    for (p = list_head; p; p = p->ifn_next) {
        DEBUGMSGTL(("access:dot3StatsTable", "processing '%s'\n", p->name));

        /*
         * get index via ioctl.
         */

        dot3StatsIndex = (long) dot3stats_interface_ioctl_ifindex_get(-1, p->name);

        /* 
         *    get the dot3stats contents populated, if the device is not an ethernet device
         *    the operation will not be supported and an error message will be logged
         */
        
        rowreq_ctx = dot3StatsTable_allocate_rowreq_ctx(NULL);
        if (NULL == rowreq_ctx) {
            snmp_log(LOG_ERR, "memory allocation for dot3StatsTable failed\n");
            close(fd);
            return MFD_RESOURCE_UNAVAILABLE;
        }
        
        if (MFD_SUCCESS !=
            dot3StatsTable_indexes_set(rowreq_ctx, dot3StatsIndex)) {
            snmp_log(LOG_ERR,
                     "error setting index while loading "
                     "dot3StatsTable data.\n");
            dot3StatsTable_release_rowreq_ctx(rowreq_ctx);
            continue;
        }

        /*
         * TODO:352:r: |   |-> populate dot3StatsTable data context.
         * Populate data context here. (optionally, delay until row prep)
         */
        /*
         * non-TRANSIENT data: no need to copy. set pointer to data 
         */

        memset (&rowreq_ctx->data, 0, sizeof (rowreq_ctx->data));

	interface_sysclassnet_dot3stats_get(rowreq_ctx, p->name);

	interface_dot3stats_get_errorcounters(rowreq_ctx, p->name);

        rc = interface_ioctl_dot3stats_get (rowreq_ctx, fd, p->name);

        if (rc < 0) {
            DEBUGMSGTL(("access:dot3StatsTable", "error getting the statistics for interface |%s| "
                        "dot3StatsTable data, operation might not be supported\n", p->name));
            dot3StatsTable_release_rowreq_ctx(rowreq_ctx);
            continue;
        }

        rc = interface_ioctl_dot3stats_duplex_get(rowreq_ctx, fd, p->name);
        if (rc < 0) {
            DEBUGMSGTL(("access:dot3StatsTable", "error getting the duplex status for |%s| "
                        "dot3StatsTable data, operation might not be supported\n", p->name));
            dot3StatsTable_release_rowreq_ctx(rowreq_ctx);
            continue;
        }

        /*
         * insert into table container
         */
        rc = CONTAINER_INSERT(container, rowreq_ctx);
        if (rc < 0) {
            DEBUGMSGTL(("access:dot3StatsTable", "error inserting |%s|", p->name));
            dot3StatsTable_release_rowreq_ctx(rowreq_ctx);
            continue;
        }

        ++count;
    }

    close(fd);

    /*
     * free the interface names list 
     */

    if ( (dot3stats_interface_name_list_free(list_head)) < 0) {
        snmp_log(LOG_ERR, "access:dot3StatsTable, error freeing the interface name list \n");
        DEBUGMSGTL(("access:dot3StatsTable", "error freeing the interface name list\n"));
        return MFD_ERROR;
    }
#endif

    DEBUGMSGT(("verbose:dot3StatsTable:dot3StatsTable_container_load",
               "inserted %" NETSNMP_PRIz "d records\n", count));

    return MFD_SUCCESS;
}                               /* dot3StatsTable_container_load */

/**
 * container clean up
 *
 * @param container container with all current items
 *
 *  This optional callback is called prior to all
 *  item's being removed from the container. If you
 *  need to do any processing before that, do it here.
 *
 * @note
 *  The MFD helper will take care of releasing all the row contexts.
 *
 */
void
dot3StatsTable_container_free(netsnmp_container * container)
{
    DEBUGMSGTL(("verbose:dot3StatsTable:dot3StatsTable_container_free",
                "called\n"));

    /*
     * TODO:380:M: Free dot3StatsTable container data.
     */
}                               /* dot3StatsTable_container_free */

/**
 * prepare row for processing.
 *
 *  When the agent has located the row for a request, this function is
 *  called to prepare the row for processing. If you fully populated
 *  the data context during the index setup phase, you may not need to
 *  do anything.
 *
 * @param rowreq_ctx pointer to a context.
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 */
int
dot3StatsTable_row_prep(dot3StatsTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("verbose:dot3StatsTable:dot3StatsTable_row_prep",
                "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    /*
     * TODO:390:o: Prepare row for request.
     * If populating row data was delayed, this is the place to
     * fill in the row for this request.
     */

    return MFD_SUCCESS;
}                               /* dot3StatsTable_row_prep */

/** @} */
