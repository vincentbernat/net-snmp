/*
 * Note: this file originally auto-generated by mib2c using
 *  : generic-table-constants.m2c,v 1.4 2004/06/07 18:20:28 rstory Exp $
 *
 * $Id$
 */
#ifndef INETNETTOMEDIATABLE_CONSTANTS_H
#define INETNETTOMEDIATABLE_CONSTANTS_H

#ifdef __cplusplus
extern          "C" {
#endif


    /*
     * column number definitions for table inetNetToMediaTable 
     */
#define INETNETTOMEDIATABLE_OID              1,3,6,1,2,1,4,35
#define COLUMN_INETNETTOMEDIAIFINDEX		1
#define COLUMN_INETNETTOMEDIANETADDRESSTYPE		2
#define COLUMN_INETNETTOMEDIANETADDRESS		3
#define COLUMN_INETNETTOMEDIAPHYSADDRESS		4
#define COLUMN_INETNETTOMEDIALASTUPDATED		5
#define COLUMN_INETNETTOMEDIATYPE		6
#define COLUMN_INETNETTOMEDIASTATE		7
#define COLUMN_INETNETTOMEDIAROWSTATUS		8

#define INETNETTOMEDIATABLE_MIN_COL		COLUMN_INETNETTOMEDIAPHYSADDRESS
#define INETNETTOMEDIATABLE_MAX_COL		COLUMN_INETNETTOMEDIAROWSTATUS

    /*
     * change flags for writable columns 
     */
#define FLAG_INETNETTOMEDIAPHYSADDRESS       (0x1 << 0)
#define FLAG_INETNETTOMEDIATYPE       (0x1 << 1)
#define FLAG_INETNETTOMEDIAROWSTATUS       (0x1 << 2)

#define FLAG_MAX_INETNETTOMEDIATABLE 3


    /*
     * NOTES on enums
     * ==============
     *
     * Value Mapping
     * -------------
     * If the values for your data type don't exactly match the
     * possible values defined by the mib, you should map them
     * below. For example, a boolean flag (1/0) is usually represented
     * as a TruthValue in a MIB, which maps to the values (1/2).
     *
     */
/*************************************************************************
 *************************************************************************
 *
 * enum definitions for table inetNetToMediaTable
 *
 *************************************************************************
 *************************************************************************/

/*************************************************************
 * constants for enums for the MIB node
 * inetNetToMediaNetAddressType (InetAddressType / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef INETADDRESSTYPE_ENUMS
#define INETADDRESSTYPE_ENUMS

#define INETADDRESSTYPE_UNKNOWN  0
#define INETADDRESSTYPE_IPV4  1
#define INETADDRESSTYPE_IPV6  2
#define INETADDRESSTYPE_IPV4Z  3
#define INETADDRESSTYPE_IPV6Z  4
#define INETADDRESSTYPE_DNS  16


#endif                          /* INETADDRESSTYPE_ENUMS */

    /*
     * TODO:
     * value mapping (see notes at top of file)
     */
#define INTERNAL_INETNETTOMEDIANETADDRESSTYPE_UNKNOWN  0
#define INTERNAL_INETNETTOMEDIANETADDRESSTYPE_IPV4  1
#define INTERNAL_INETNETTOMEDIANETADDRESSTYPE_IPV6  2
#define INTERNAL_INETNETTOMEDIANETADDRESSTYPE_IPV4Z  3
#define INTERNAL_INETNETTOMEDIANETADDRESSTYPE_IPV6Z  4
#define INTERNAL_INETNETTOMEDIANETADDRESSTYPE_DNS  16

/*************************************************************
 * constants for enums for the MIB node
 * inetNetToMediaType (INTEGER / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef INETNETTOMEDIATYPE_ENUMS
#define INETNETTOMEDIATYPE_ENUMS

#define INETNETTOMEDIATYPE_OTHER  1
#define INETNETTOMEDIATYPE_INVALID  2
#define INETNETTOMEDIATYPE_DYNAMIC  3
#define INETNETTOMEDIATYPE_STATIC  4
#define INETNETTOMEDIATYPE_LOCAL  5


#endif                          /* INETNETTOMEDIATYPE_ENUMS */

    /*
     * TODO:
     * value mapping (see notes at top of file)
     */
#define INTERNAL_INETNETTOMEDIATYPE_OTHER  1
#define INTERNAL_INETNETTOMEDIATYPE_INVALID  2
#define INTERNAL_INETNETTOMEDIATYPE_DYNAMIC  3
#define INTERNAL_INETNETTOMEDIATYPE_STATIC  4
#define INTERNAL_INETNETTOMEDIATYPE_LOCAL  5

/*************************************************************
 * constants for enums for the MIB node
 * inetNetToMediaState (INTEGER / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef INETNETTOMEDIASTATE_ENUMS
#define INETNETTOMEDIASTATE_ENUMS

#define INETNETTOMEDIASTATE_REACHABLE  1
#define INETNETTOMEDIASTATE_STALE  2
#define INETNETTOMEDIASTATE_DELAY  3
#define INETNETTOMEDIASTATE_PROBE  4
#define INETNETTOMEDIASTATE_INVALID  5
#define INETNETTOMEDIASTATE_UNKNOWN  6
#define INETNETTOMEDIASTATE_INCOMPLETE  7


#endif                          /* INETNETTOMEDIASTATE_ENUMS */

    /*
     * TODO:
     * value mapping (see notes at top of file)
     */
#define INTERNAL_INETNETTOMEDIASTATE_REACHABLE  1
#define INTERNAL_INETNETTOMEDIASTATE_STALE  2
#define INTERNAL_INETNETTOMEDIASTATE_DELAY  3
#define INTERNAL_INETNETTOMEDIASTATE_PROBE  4
#define INTERNAL_INETNETTOMEDIASTATE_INVALID  5
#define INTERNAL_INETNETTOMEDIASTATE_UNKNOWN  6
#define INTERNAL_INETNETTOMEDIASTATE_INCOMPLETE  7

/*************************************************************
 * constants for enums for the MIB node
 * inetNetToMediaRowStatus (RowStatus / ASN_INTEGER)
 *
 * since a Textual Convention may be referenced more than once in a
 * MIB, protect againt redefinitions of the enum values.
 */
#ifndef ROWSTATUS_ENUMS
#define ROWSTATUS_ENUMS

#define ROWSTATUS_ACTIVE  1
#define ROWSTATUS_NOTINSERVICE  2
#define ROWSTATUS_NOTREADY  3
#define ROWSTATUS_CREATEANDGO  4
#define ROWSTATUS_CREATEANDWAIT  5
#define ROWSTATUS_DESTROY  6


#endif                          /* ROWSTATUS_ENUMS */

    /*
     * TODO:
     * value mapping (see notes at top of file)
     */
#define INTERNAL_INETNETTOMEDIAROWSTATUS_ACTIVE  1
#define INTERNAL_INETNETTOMEDIAROWSTATUS_NOTINSERVICE  2
#define INTERNAL_INETNETTOMEDIAROWSTATUS_NOTREADY  3
#define INTERNAL_INETNETTOMEDIAROWSTATUS_CREATEANDGO  4
#define INTERNAL_INETNETTOMEDIAROWSTATUS_CREATEANDWAIT  5
#define INTERNAL_INETNETTOMEDIAROWSTATUS_DESTROY  6



#ifdef __cplusplus
}
#endif

#endif                          /* INETNETTOMEDIATABLE_OIDS_H */
