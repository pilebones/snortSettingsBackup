/*
 * DOES NOT USE BUILT-IN DETECTION FUNCTION!
alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Apple QuickTime color table atom movie file handling heap corruption attempt"; flowbits:isset,file.quicktime; flow:to_client,established; content:"ctab"; metadata:policy balanced-ips drop, policy security-ips drop, service http; reference:bugtraq,26338; reference:cve,2007-4677; classtype:attempted-user;)
*/
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

#include "so-util.h"

//#define DEBUG 1
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif

/* declare detection functions */
int rule17608eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flowbits:isset "file.quicktime"; */
static FlowBitsInfo rule17608flowbits0 =
{
    "file.quicktime",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule17608option0 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule17608flowbits0
    }
};
/* flow:established, to_client; */
static FlowFlags rule17608flow1 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule17608option1 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule17608flow1
    }
};
// content:"ctab", depth 0; 
static ContentInfo rule17608content2 = 
{
    (uint8_t *) "ctab", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED|CONTENT_RELATIVE, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule17608option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule17608content2
    }
};

/* references for sid 17608 */
/* reference: bugtraq "26338"; */
static RuleReference rule17608ref1 = 
{
    "bugtraq", /* type */
    "26338" /* value */
};

/* reference: cve "2007-4677"; */
static RuleReference rule17608ref2 = 
{
    "cve", /* type */
    "2007-4677" /* value */
};

static RuleReference *rule17608refs[] =
{
    &rule17608ref1,
    &rule17608ref2,
    NULL
};
/* metadata for sid 17608 */
/* metadata:service http, policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule17608service1 = 
{
    "service http"
};
//
//
//static RuleMetaData rule17608policy1 = 
//{
//    "policy balanced-ips drop"
//};
//
//static RuleMetaData rule17608policy2 = 
//{
//    "policy security-ips drop"
//};
//

static RuleMetaData *rule17608metadata[] =
{
    &rule17608service1,
//    &rule17608policy1,
//    &rule17608policy2,
    NULL
};

RuleOption *rule17608options[] =
{
    &rule17608option0,
    &rule17608option1,
    &rule17608option2,
    NULL
};

Rule rule17608 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
   
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       17608, /* sigid */
       6, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "FILE-MULTIMEDIA Apple QuickTime color table atom movie file handling heap corruption attempt",     /* message */
       rule17608refs /* ptr to references */
       ,rule17608metadata
   },
   rule17608options, /* ptr to rule options */
   &rule17608eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule17608eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    const uint8_t *beg_of_payload;
    const uint8_t *end_of_payload;
    const uint8_t *cursor_detect01;
    const uint8_t *cursor_detect02;
    uint32_t       atom_size;
    uint32_t       color_table_size;  // Note that color_table_size is 16 bit field but we are using 32 bit variable
                                       // so that (color_table_size + 1) won't overflow

    DEBUG_SO(const char *name="rule17608eval";)
    DEBUG_SO(printf("%s: enter\n", name);)

    if(sp == NULL)
        return RULE_NOMATCH;

    // flowbits:isset "file.quicktime";
    if (processFlowbits(p, rule17608options[0]->option_u.flowBit) > 0) {
            // flow:established, to_client;
        if (checkFlow(p, rule17608options[1]->option_u.flowFlags) > 0 ) {

            if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
                return RULE_NOMATCH;

            // We need to initialize cursor_normal because "ctab" content match is a relative match
            cursor_normal = beg_of_payload;

            // content:"ctab", depth 0; CONTENT_BUF_NORMALIZED|CONTENT_RELATIVE
            while (contentMatch(p, rule17608options[2]->option_u.content, &cursor_normal) > 0)
            {
                cursor_detect01 = cursor_normal - 8;
                cursor_detect02 = cursor_normal + 6;
                
                if ((cursor_detect01 < beg_of_payload) || (cursor_detect02 + 2 > end_of_payload))
                    return RULE_NOMATCH;

                atom_size = (uint32_t)read_big_32(cursor_detect01);  // read atom size                
                color_table_size = (uint16_t)read_big_16(cursor_detect02);  // read color table size

                DEBUG_SO(printf("%s : atom size 0x%08x\n", name, atom_size);)
                DEBUG_SO(printf("%s : color_table_size 0x%08x\n", name, color_table_size);)
                DEBUG_SO(printf("%s : available size 0x%08x\n", name, (atom_size-16)/8 - 1);)                

                // malicious if color table size is bigger than the available size 
                if ( (atom_size < 16) || (color_table_size + 1 > (atom_size-16) / 8) )
                    return RULE_MATCH;
            }
        }
    }
    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule17608,
    NULL
};
*/