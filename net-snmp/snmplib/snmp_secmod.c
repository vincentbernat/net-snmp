/*
 * security service wrapper to support pluggable security models 
 */

#include <net-snmp/net-snmp-config.h>
#include <stdio.h>
#include <ctype.h>
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_DMALLOC_H
#include <dmalloc.h>
#endif

#include <net-snmp/types.h>
#include <net-snmp/output_api.h>
#include <net-snmp/config_api.h>
#include <net-snmp/utilities.h>

#include <net-snmp/library/snmp_api.h>
#include <net-snmp/library/snmp_enum.h>
#include <net-snmp/library/callback.h>
#include <net-snmp/library/snmp_secmod.h>
#include <net-snmp/library/snmpusm.h>

static struct snmp_secmod_list *registered_services = NULL;

static SNMPCallback set_default_secmod;

void
init_secmod(void)
{
    snmp_register_callback(SNMP_CALLBACK_LIBRARY,
                           SNMP_CALLBACK_SESSION_INIT, set_default_secmod,
                           NULL);

    netsnmp_ds_register_config(ASN_OCTET_STR, "snmp", "defSecurityModel",
			       NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_SECMODEL);
    /*
     * this file is generated by configure for all the stuff we're using 
     */
#include "snmpsm_init.h"
}


int
register_sec_mod(int secmod, const char *modname,
                 struct snmp_secmod_def *newdef)
{
    int             result;
    struct snmp_secmod_list *sptr;
    char           *othername;

    for (sptr = registered_services; sptr; sptr = sptr->next) {
        if (sptr->securityModel == secmod) {
            return SNMPERR_GENERR;
        }
    }
    sptr = SNMP_MALLOC_STRUCT(snmp_secmod_list);
    if (sptr == NULL)
        return SNMPERR_MALLOC;
    sptr->secDef = newdef;
    sptr->securityModel = secmod;
    sptr->next = registered_services;
    registered_services = sptr;
    if ((result =
         se_add_pair_to_slist("snmp_secmods", strdup(modname), secmod))
        != SE_OK) {
        switch (result) {
        case SE_NOMEM:
            snmp_log(LOG_CRIT, "snmp_secmod: no memory\n");
            break;

        case SE_ALREADY_THERE:
            othername = se_find_label_in_slist("snmp_secmods", secmod);
            if (strcmp(othername, modname) != 0) {
                snmp_log(LOG_ERR,
                         "snmp_secmod: two security modules %s and %s registered with the same security number\n",
                         secmod, othername);
            }
            break;

        default:
            snmp_log(LOG_ERR,
                     "snmp_secmod: unknown error trying to register a new security module\n");
            break;
        }
        return SNMPERR_GENERR;
    }
    return SNMPERR_SUCCESS;
}

int
unregister_sec_mod(int secmod)
{
    struct snmp_secmod_list *sptr, *lptr;

    for (sptr = registered_services, lptr = NULL; sptr;
         lptr = sptr, sptr = sptr->next) {
        if (sptr->securityModel == secmod) {
            lptr->next = sptr->next;
	    SNMP_FREE(sptr->secDef);
            SNMP_FREE(sptr);
            return SNMPERR_SUCCESS;
        }
    }
    /*
     * not registered 
     */
    return SNMPERR_GENERR;
}

void            
clear_sec_mod(void)
{
    struct snmp_secmod_list *tmp = registered_services, *next = NULL;

    while (tmp != NULL) {
	next = tmp->next;
	SNMP_FREE(tmp->secDef);
	SNMP_FREE(tmp);
	tmp = next;
    }
    registered_services = NULL;
}


struct snmp_secmod_def *
find_sec_mod(int secmod)
{
    struct snmp_secmod_list *sptr;

    for (sptr = registered_services; sptr; sptr = sptr->next) {
        if (sptr->securityModel == secmod) {
            return sptr->secDef;
        }
    }
    /*
     * not registered 
     */
    return NULL;
}

static int
set_default_secmod(int major, int minor, void *serverarg, void *clientarg)
{
    netsnmp_session *sess = (netsnmp_session *) serverarg;
    char           *cptr;
    int             model;

    if (!sess)
        return SNMPERR_GENERR;
    if (sess->securityModel == SNMP_DEFAULT_SECMODEL) {
        if ((cptr = netsnmp_ds_get_string(NETSNMP_DS_LIBRARY_ID, 
					  NETSNMP_DS_LIB_SECMODEL)) != NULL) {
            if ((model = se_find_value_in_slist("snmp_secmods", cptr))
		!= SE_DNE) {
                sess->securityModel = model;
            } else {
                snmp_log(LOG_ERR,
                         "unknown security model name: %s.  Forcing USM instead.\n",
                         cptr);
                sess->securityModel = USM_SEC_MODEL_NUMBER;
                return SNMPERR_GENERR;
            }
        } else {
            sess->securityModel = USM_SEC_MODEL_NUMBER;
        }
    }
    return SNMPERR_SUCCESS;
}
