/*
 *
 * !!! XXX DOES NOT USE BUILT-IN DETECTION FUNCTION XXX !!!
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */

#include <string.h>
#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"


/* declare detection functions */
int rule15700eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
// content:"|02|", depth 1; 
static ContentInfo rule15700content0 = 
{
    (uint8_t *) "|02|", /* pattern (now in snort content format) */
    1, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule15700option0 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule15700content0
    }
};
#ifndef CONTENT_FAST_PATTERN
#define CONTENT_FAST_PATTERN 0
#endif
// content:"c|82|Sc", offset 236, depth 4, fast_pattern; 
static ContentInfo rule15700content1 = 
{
    (uint8_t *) "c|82|Sc", /* pattern (now in snort content format) */
    4, /* depth */
    236, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule15700option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule15700content1
    }
};

/* references for sid 15700 */
/* reference: cve "2009-0692"; */
static RuleReference rule15700ref1 = 
{
    "cve", /* type */
    "2009-0692" /* value */
};

/* reference: bugtraq "35668"; */
static RuleReference rule15700ref2 =
{
    "bugtraq", /* type */
    "35668" /* value */
};


static RuleReference *rule15700refs[] =
{
    &rule15700ref1,
    &rule15700ref2,
    NULL
};

/* metadata for sid 15700 */
/* metadata:policy balanced-ips drop, policy security-ips drop; */

static RuleMetaData rule15700policy1 = 
{
    "policy balanced-ips drop"
};

static RuleMetaData rule15700policy2 = 
{
    "policy security-ips drop"
};


static RuleMetaData *rule15700metadata[] =
{
    &rule15700policy1,
    &rule15700policy2,
    NULL
};

RuleOption *rule15700options[] =
{
    &rule15700option0,
    &rule15700option1,
    NULL
};

Rule rule15700 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_UDP, /* proto */
       "$HOME_NET", /* SRCIP     */
       "[67,68]", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "[67,68]", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       15700, /* sigid */
       3, /* revision */
   
       "attempted-admin", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "SERVER-OTHER dhclient subnet mask option buffer overflow attempt",     /* message */
       rule15700refs /* ptr to references */
       ,rule15700metadata
   },
   rule15700options, /* ptr to rule options */
   &rule15700eval, /* DOES NOT use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule15700eval(void *p) {
   const uint8_t *cursor_normal = 0, *beg_of_payload, *end_of_payload;
   SFSnortPacket *sp = (SFSnortPacket *) p;

   if(sp == NULL)
      return RULE_NOMATCH;

   if(sp->payload == NULL)
      return RULE_NOMATCH;
    
   // content:"|02|", depth 1;
   if(contentMatch(p, rule15700options[0]->option_u.content, &cursor_normal) <= 0)
      return RULE_NOMATCH;

   // content:"c|82|Sc", offset 236, depth 4, fast_pattern;
   if(contentMatch(p, rule15700options[1]->option_u.content, &cursor_normal) <= 0)
      return RULE_NOMATCH;

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   // "c|82|Sc" puts us at the beginning of the dhcp options
   // 0xFF is the end of DHCP options
   while((cursor_normal + 1 < end_of_payload) && (*cursor_normal != 0xFF)) {

      if(*cursor_normal == 0x01) { // subnet mask option

         // Alert if the subnet mask is more than four bytes long
         if(*(cursor_normal + 1) > 4)
            return RULE_MATCH;

         // Do not return RULE_NOMATCH in the negative case in case it's
         // possible to specify this option more than once and still hit
         // the vulnerable code
           
      }

      cursor_normal += *(cursor_normal + 1) + 2; // + option size + option type and size bytes
   }

   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule15700,
    NULL
};
*/
