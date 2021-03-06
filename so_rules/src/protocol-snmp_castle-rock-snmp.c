/* Does not use built-in detection function!
alert udp $EXTERNAL_NET any -> $HOME_NET 162 (msg:"SNMP Castle Rock Computing SNMPc Network Manager community string attempted stack overflow"; flow:to_server; content:"|04|"; depth:10; metadata:policy balanced-ips drop, policy security-ips drop, service snmp; reference:bugtraq,28990; reference:cve,2008-2214; classtype:attempted-admin; sid:17632;)
*/
/*
 * Vuln Title: Castle Rock Computing SNMPc Network Manager community string attempted stack overflow
 *
 * Copyright (C) 2005-2010 Sourcefire, Inc. All Rights Reserved
 *
 * Written by Patrick Mullen, Sourcefire VRT <pmullen@sourcefire.com>
 *
 * Auto-generated by Patrick Mullen
 *
 * This file may contain proprietary rules that were created, tested and
 * certified by Sourcefire, Inc. (the "VRT Certified Rules") as well as
 * rules that were created by Sourcefire and other third parties and
 * distributed under the GNU General Public License (the "GPL Rules").  The
 * VRT Certified Rules contained in this file are the property of
 * Sourcefire, Inc. Copyright 2005 Sourcefire, Inc. All Rights Reserved.
 * The GPL Rules created by Sourcefire, Inc. are the property of
 * Sourcefire, Inc. Copyright 2002-2005 Sourcefire, Inc. All Rights
 * Reserved.  All other GPL Rules are owned and copyrighted by their
 * respective owners (please see www.snort.org/contributors for a list of
 * owners and their respective copyrights).  In order to determine what
 * rules are VRT Certified Rules or GPL Rules, please refer to the VRT
 * Certified Rules License Agreement.
 */

#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

#include "so-util_ber.h"

/* declare detection functions */
int rule17632eval(void *p);

/* declare rule data structures */
/* flow:to_server; */
static FlowFlags rule17632flow0 = 
{
    FLOW_TO_SERVER
};

static RuleOption rule17632option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule17632flow0
    }
};
// content:"|04|", depth 10, fast_pattern; 
static ContentInfo rule17632content1 = 
{
    (uint8_t *) "|04|", /* pattern (now in snort content format) */
    10, /* depth */
    0, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule17632option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule17632content1
    }
};

/* references for sid 17632 */
/* reference: bugtraq "28990"; */
static RuleReference rule17632ref1 = 
{
    "bugtraq", /* type */
    "28990" /* value */
};

/* reference: cve "2008-2214"; */
static RuleReference rule17632ref2 = 
{
    "cve", /* type */
    "2008-2214" /* value */
};

static RuleReference *rule17632refs[] =
{
    &rule17632ref1,
    &rule17632ref2,
    NULL
};
/* metadata for sid 17632 */
/* metadata:service snmp, policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule17632service1 = 
{
    "service snmp"
};


static RuleMetaData rule17632policy1 = 
{
    "policy balanced-ips drop"
};

static RuleMetaData rule17632policy2 = 
{
    "policy security-ips drop"
};


static RuleMetaData *rule17632metadata[] =
{
    &rule17632service1,
    &rule17632policy1,
    &rule17632policy2,
    NULL
};

RuleOption *rule17632options[] =
{
    &rule17632option0,
    &rule17632option1,
    NULL
};

Rule rule17632 = {
   /* rule header, akin to => tcp any any -> any any */
   {
       IPPROTO_UDP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "any", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "162", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       17632, /* sigid */
       2, /* revision */
       "attempted-admin", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "PROTOCOL-SNMP Castle Rock Computing SNMPc Network Manager community string attempted stack overflow",     /* message */
       rule17632refs /* ptr to references */
       ,rule17632metadata
   },
   rule17632options, /* ptr to rule options */
   &rule17632eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule17632eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    const uint8_t *end_of_payload;

    BER_ELEMENT ber_element;

    if(sp == NULL)
        return RULE_NOMATCH;

    // flow:to_server;
    if (checkFlow(p, rule17632options[0]->option_u.flowFlags) > 0 ) {
        // content:"|04|", depth 10, fast_pattern;
        if (contentMatch(p, rule17632options[1]->option_u.content, &cursor_normal) > 0) {

           if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &cursor_normal, &end_of_payload) != CURSOR_IN_BOUNDS)
              return RULE_NOMATCH;
   
           BER_DATA(0x30);
           BER_SKIP(0x02);
   
           if(ber_get_element(sp, cursor_normal, &ber_element) < 0)
              return RULE_NOMATCH;
   
           // Vuln condition is a string (type 0x04) greater than 64 bytes long
           if(ber_element.type == 0x04 && ber_element.data_len >= 64)
              return RULE_MATCH;

        }
    }

    return RULE_NOMATCH;
}
/*
Rule *rules[] = {
    &rule17632,
    NULL
};
*/
