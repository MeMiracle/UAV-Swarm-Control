#include <net/if.h>
#include <errno.h>
#include <string.h>

#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include "wifistatus/nl80211.h"
#include "wifistatus/iw.h"

SECTION(station);

enum plink_state {
	LISTEN,
	OPN_SNT,
	OPN_RCVD,
	CNF_RCVD,
	ESTAB,
	HOLDING,
	BLOCKED
};

extern struct s_wifi_status __wifi;

void parse_tid_stats(struct nlattr *tid_stats_attr)
{
	struct nlattr *stats_info[NL80211_TID_STATS_MAX + 1], *tidattr, *info;
	static struct nla_policy stats_policy[NL80211_TID_STATS_MAX + 1] = {
		[NL80211_TID_STATS_RX_MSDU] = { .type = NLA_U64 },
		[NL80211_TID_STATS_TX_MSDU] = { .type = NLA_U64 },
		[NL80211_TID_STATS_TX_MSDU_RETRIES] = { .type = NLA_U64 },
		[NL80211_TID_STATS_TX_MSDU_FAILED] = { .type = NLA_U64 },
	};
	int rem, i = 0;

	printf("\n\tMSDU:\n\t\tTID\trx\ttx\ttx retries\ttx failed");
	nla_for_each_nested(tidattr, tid_stats_attr, rem) {
		if (nla_parse_nested(stats_info, NL80211_TID_STATS_MAX,
				     tidattr, stats_policy)) {
			printf("failed to parse nested stats attributes!");
			return;
		}
		printf("\n\t\t%d", i++);
		info = stats_info[NL80211_TID_STATS_RX_MSDU];
		if (info)
			printf("\t%llu", (unsigned long long)nla_get_u64(info));
		info = stats_info[NL80211_TID_STATS_TX_MSDU];
		if (info)
			printf("\t%llu", (unsigned long long)nla_get_u64(info));
		info = stats_info[NL80211_TID_STATS_TX_MSDU_RETRIES];
		if (info)
			printf("\t%llu", (unsigned long long)nla_get_u64(info));
		info = stats_info[NL80211_TID_STATS_TX_MSDU_FAILED];
		if (info)
			printf("\t\t%llu", (unsigned long long)nla_get_u64(info));
	}
}

void parse_bss_param(struct nlattr *bss_param_attr)
{
	struct nlattr *bss_param_info[NL80211_STA_BSS_PARAM_MAX + 1], *info;
	static struct nla_policy bss_poilcy[NL80211_STA_BSS_PARAM_MAX + 1] = {
		[NL80211_STA_BSS_PARAM_CTS_PROT] = { .type = NLA_FLAG },
		[NL80211_STA_BSS_PARAM_SHORT_PREAMBLE] = { .type = NLA_FLAG },
		[NL80211_STA_BSS_PARAM_SHORT_SLOT_TIME] = { .type = NLA_FLAG },
		[NL80211_STA_BSS_PARAM_DTIM_PERIOD] = { .type = NLA_U8 },
		[NL80211_STA_BSS_PARAM_BEACON_INTERVAL] = { .type = NLA_U16 },
	};

	if (nla_parse_nested(bss_param_info, NL80211_STA_BSS_PARAM_MAX,
			     bss_param_attr, bss_poilcy)) {
		printf("failed to parse nested bss param attributes!");
	}

	info = bss_param_info[NL80211_STA_BSS_PARAM_DTIM_PERIOD];
	if (info)
		printf("\n\tDTIM period:\t%u", nla_get_u8(info));
	info = bss_param_info[NL80211_STA_BSS_PARAM_BEACON_INTERVAL];
	if (info)
		printf("\n\tbeacon interval:%u", nla_get_u16(info));
	info = bss_param_info[NL80211_STA_BSS_PARAM_CTS_PROT];
	if (info) {
		printf("\n\tCTS protection:");
		if (nla_get_u16(info))
			printf("\tyes");
		else
			printf("\tno");
	}
	info = bss_param_info[NL80211_STA_BSS_PARAM_SHORT_PREAMBLE];
	if (info) {
		printf("\n\tshort preamble:");
		if (nla_get_u16(info))
			printf("\tyes");
		else
			printf("\tno");
	}
	info = bss_param_info[NL80211_STA_BSS_PARAM_SHORT_SLOT_TIME];
	if (info) {
		printf("\n\tshort slot time:");
		if (nla_get_u16(info))
			printf("yes");
		else
			printf("no");
	}
}

void parse_bitrate(struct nlattr *bitrate_attr, char *buf, int buflen)
{
	int rate = 0;
	char *pos = buf;
	struct nlattr *rinfo[NL80211_RATE_INFO_MAX + 1];
	static struct nla_policy rate_policy[NL80211_RATE_INFO_MAX + 1] = {
		[NL80211_RATE_INFO_BITRATE] = { .type = NLA_U16 },
		[NL80211_RATE_INFO_BITRATE32] = { .type = NLA_U32 },
		[NL80211_RATE_INFO_MCS] = { .type = NLA_U8 },
		[NL80211_RATE_INFO_40_MHZ_WIDTH] = { .type = NLA_FLAG },
		[NL80211_RATE_INFO_SHORT_GI] = { .type = NLA_FLAG },
	};

	if (nla_parse_nested(rinfo, NL80211_RATE_INFO_MAX,
			     bitrate_attr, rate_policy)) {
		snprintf(buf, buflen, "failed to parse nested rate attributes!");
		return;
	}

	if (rinfo[NL80211_RATE_INFO_BITRATE32])
		rate = nla_get_u32(rinfo[NL80211_RATE_INFO_BITRATE32]);
	else if (rinfo[NL80211_RATE_INFO_BITRATE])
		rate = nla_get_u16(rinfo[NL80211_RATE_INFO_BITRATE]);
	if (rate > 0)
		pos += snprintf(pos, buflen - (pos - buf),
				"%d.%d MBit/s", rate / 10, rate % 10);
	else
		pos += snprintf(pos, buflen - (pos - buf), "(unknown)");
}

static char *get_chain_signal(struct nlattr *attr_list)
{
	struct nlattr *attr;
	static char buf[64];
	char *cur = buf;
	int i = 0, rem;
	const char *prefix;

	if (!attr_list)
		return "";

	nla_for_each_nested(attr, attr_list, rem) {
		if (i++ > 0)
			prefix = ", ";
		else
			prefix = "[";

		cur += snprintf(cur, sizeof(buf) - (cur - buf), "%s%d", prefix,
				(int8_t) nla_get_u8(attr));
	}

	if (i)
		snprintf(cur, sizeof(buf) - (cur - buf), "] ");

	return buf;
}

static int print_sta_handler(struct nl_msg *msg, void *arg)
{
	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *sinfo[NL80211_STA_INFO_MAX + 1];
	char mac_addr[20], state_name[10], dev[20];
	static struct nla_policy stats_policy[NL80211_STA_INFO_MAX + 1] = {
		[NL80211_STA_INFO_INACTIVE_TIME] = { .type = NLA_U32 },
		[NL80211_STA_INFO_RX_BYTES] = { .type = NLA_U32 },
		[NL80211_STA_INFO_TX_BYTES] = { .type = NLA_U32 },
		[NL80211_STA_INFO_RX_BYTES64] = { .type = NLA_U64 },
		[NL80211_STA_INFO_TX_BYTES64] = { .type = NLA_U64 },
		[NL80211_STA_INFO_RX_PACKETS] = { .type = NLA_U32 },
		[NL80211_STA_INFO_TX_PACKETS] = { .type = NLA_U32 },
		[NL80211_STA_INFO_BEACON_RX] = { .type = NLA_U64},
		[NL80211_STA_INFO_SIGNAL] = { .type = NLA_U8 },
		[NL80211_STA_INFO_T_OFFSET] = { .type = NLA_U64 },
		[NL80211_STA_INFO_TX_BITRATE] = { .type = NLA_NESTED },
		[NL80211_STA_INFO_RX_BITRATE] = { .type = NLA_NESTED },
		[NL80211_STA_INFO_LLID] = { .type = NLA_U16 },
		[NL80211_STA_INFO_PLID] = { .type = NLA_U16 },
		[NL80211_STA_INFO_PLINK_STATE] = { .type = NLA_U8 },
		[NL80211_STA_INFO_TX_RETRIES] = { .type = NLA_U32 },
		[NL80211_STA_INFO_TX_FAILED] = { .type = NLA_U32 },
		[NL80211_STA_INFO_BEACON_LOSS] = { .type = NLA_U32},
		[NL80211_STA_INFO_RX_DROP_MISC] = { .type = NLA_U64},
		[NL80211_STA_INFO_STA_FLAGS] =
			{ .minlen = sizeof(struct nl80211_sta_flag_update) },
		[NL80211_STA_INFO_LOCAL_PM] = { .type = NLA_U32},
		[NL80211_STA_INFO_PEER_PM] = { .type = NLA_U32},
		[NL80211_STA_INFO_NONPEER_PM] = { .type = NLA_U32},
		[NL80211_STA_INFO_CHAIN_SIGNAL] = { .type = NLA_NESTED },
		[NL80211_STA_INFO_CHAIN_SIGNAL_AVG] = { .type = NLA_NESTED },
		[NL80211_STA_INFO_TID_STATS] = { .type = NLA_NESTED },
		[NL80211_STA_INFO_BSS_PARAM] = { .type = NLA_NESTED },
		[NL80211_STA_INFO_RX_DURATION] = { .type = NLA_U64 },
	};

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		  genlmsg_attrlen(gnlh, 0), NULL);

	/*
	 * TODO: validate the interface and mac address!
	 * Otherwise, there's a race condition as soon as
	 * the kernel starts sending station notifications.
	 */

	if (!tb[NL80211_ATTR_STA_INFO]) {
		fprintf(stderr, "sta stats missing!\n");
		return NL_SKIP;
	}
	if (nla_parse_nested(sinfo, NL80211_STA_INFO_MAX,
			     tb[NL80211_ATTR_STA_INFO],
			     stats_policy)) {
		fprintf(stderr, "failed to parse nested attributes!\n");
		return NL_SKIP;
	}

	mac_addr_n2a(mac_addr, nla_data(tb[NL80211_ATTR_MAC]));
	if_indextoname(nla_get_u32(tb[NL80211_ATTR_IFINDEX]), dev);

	if (sinfo[NL80211_STA_INFO_RX_BYTES64])
	{
		__wifi.rx_bytes_64 = nla_get_u64(sinfo[NL80211_STA_INFO_RX_BYTES64]);
	}
	else if (sinfo[NL80211_STA_INFO_RX_BYTES])
	{
		__wifi.rx_bytes_32 = nla_get_u32(sinfo[NL80211_STA_INFO_RX_BYTES]);
	}
	if (sinfo[NL80211_STA_INFO_RX_PACKETS])
	{
		__wifi.rx_packages = nla_get_u32(sinfo[NL80211_STA_INFO_RX_PACKETS]);
	}
	if (sinfo[NL80211_STA_INFO_TX_BYTES64])
	{
		__wifi.tx_bytes_64 = nla_get_u64(sinfo[NL80211_STA_INFO_TX_BYTES64]);
	}
	else if (sinfo[NL80211_STA_INFO_TX_BYTES])
	{
		__wifi.tx_bytes_32 = nla_get_u32(sinfo[NL80211_STA_INFO_TX_BYTES]);
	}
	if (sinfo[NL80211_STA_INFO_TX_PACKETS])
	{
		__wifi.tx_packages = nla_get_u32(sinfo[NL80211_STA_INFO_TX_PACKETS]);
	}
	if (sinfo[NL80211_STA_INFO_TX_RETRIES])
	{
		__wifi.tx_retries = nla_get_u32(sinfo[NL80211_STA_INFO_TX_RETRIES]);
	}
	if (sinfo[NL80211_STA_INFO_TX_FAILED])
	{
		__wifi.tx_fails = nla_get_u32(sinfo[NL80211_STA_INFO_TX_FAILED]);
	}
	if (sinfo[NL80211_STA_INFO_BEACON_LOSS])
	{
		__wifi.beacon_loss = nla_get_u32(sinfo[NL80211_STA_INFO_BEACON_LOSS]);
	}

	if (sinfo[NL80211_STA_INFO_SIGNAL])
	{
		__wifi.signal = (int8_t)nla_get_u8(sinfo[NL80211_STA_INFO_SIGNAL]);
	}


	if (sinfo[NL80211_STA_INFO_TX_BITRATE]) {
		char buf[100];
		parse_bitrate(sinfo[NL80211_STA_INFO_TX_BITRATE], buf, sizeof(buf));
		char *pos = strstr(buf, "MBit/s");
		if(NULL != pos)
		{
			uint32_t bitrate_size = pos - buf;
			strncpy(__wifi.tx_bitrate, buf, bitrate_size);
			__wifi.tx_bitrate[bitrate_size] = '\0';
		}

	}

	if (sinfo[NL80211_STA_INFO_RX_BITRATE]) {
		char buf[100];

		parse_bitrate(sinfo[NL80211_STA_INFO_RX_BITRATE], buf, sizeof(buf));
		char *pos = strstr(buf, "MBit/s");
		if(NULL != pos)
		{
			uint32_t bitrate_size = pos - buf;
			strncpy(__wifi.rx_bitrate, buf, bitrate_size);
			__wifi.tx_bitrate[bitrate_size] = '\0';
		}
	}


	if (sinfo[NL80211_STA_INFO_EXPECTED_THROUGHPUT]) {
		uint32_t thr;

		thr = nla_get_u32(sinfo[NL80211_STA_INFO_EXPECTED_THROUGHPUT]);
		/* convert in Mbps but scale by 1000 to save kbps units */
		__wifi.expect_throughput = thr * 1000 / 1024;
	}
	return NL_SKIP;
}

static int handle_station_get(struct nl80211_state *state,
			      struct nl_msg *msg,
			      int argc, char **argv,
			      enum id_input id)
{
	unsigned char mac_addr[ETH_ALEN];

	if (argc < 1)
		return 1;

	if (mac_addr_a2n(mac_addr, argv[0])) {
		fprintf(stderr, "invalid mac address\n");
		return 2;
	}

	argc--;
	argv++;

	if (argc)
		return 1;

	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);

	register_handler(print_sta_handler, NULL);

	return 0;
 nla_put_failure:
	return -ENOBUFS;
}
COMMAND(station, get, "<MAC address>",
	NL80211_CMD_GET_STATION, 0, CIB_NETDEV, handle_station_get,
	"Get information for a specific station.");

static int handle_station_del(struct nl80211_state *state,
			      struct nl_msg *msg,
			      int argc, char **argv,
			      enum id_input id)
{
	char *end;
	unsigned char mac_addr[ETH_ALEN];
	int subtype;
	int reason_code;

	if (argc < 1)
		return 1;

	if (mac_addr_a2n(mac_addr, argv[0])) {
		fprintf(stderr, "invalid mac address\n");
		return 2;
	}

	argc--;
	argv++;
	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);

	if (argc > 1 && strcmp(argv[0], "subtype") == 0) {
		argv++;
		argc--;

		subtype = strtod(argv[0], &end);
		if (*end != '\0')
			return 1;

		NLA_PUT_U8(msg, NL80211_ATTR_MGMT_SUBTYPE, subtype);
		argv++;
		argc--;
	}

	if (argc > 1 && strcmp(argv[0], "reason-code") == 0) {
		argv++;
		argc--;

		reason_code = strtod(argv[0], &end);
		if (*end != '\0')
			return 1;

		NLA_PUT_U16(msg, NL80211_ATTR_REASON_CODE, reason_code);
		argv++;
		argc--;
	}

	if (argc)
		return 1;

	register_handler(print_sta_handler, NULL);

	return 0;
 nla_put_failure:
	return -ENOBUFS;
}
COMMAND(station, del, "<MAC address> [subtype <subtype>] [reason-code <code>]",
	NL80211_CMD_DEL_STATION, 0, CIB_NETDEV, handle_station_del,
	"Remove the given station entry (use with caution!)\n"
	"Example subtype values: 0xA (disassociation), 0xC (deauthentication)");

static const struct cmd *station_set_plink;
static const struct cmd *station_set_vlan;
static const struct cmd *station_set_mesh_power_mode;

static const struct cmd *select_station_cmd(int argc, char **argv)
{
	if (argc < 2)
		return NULL;
	if (strcmp(argv[1], "plink_action") == 0)
		return station_set_plink;
	if (strcmp(argv[1], "vlan") == 0)
		return station_set_vlan;
	if (strcmp(argv[1], "mesh_power_mode") == 0)
		return station_set_mesh_power_mode;
	return NULL;
}

static int handle_station_set_plink(struct nl80211_state *state,
			      struct nl_msg *msg,
			      int argc, char **argv,
			      enum id_input id)
{
	unsigned char plink_action;
	unsigned char mac_addr[ETH_ALEN];

	if (argc < 3)
		return 1;

	if (mac_addr_a2n(mac_addr, argv[0])) {
		fprintf(stderr, "invalid mac address\n");
		return 2;
	}
	argc--;
	argv++;

	if (strcmp("plink_action", argv[0]) != 0)
		return 1;
	argc--;
	argv++;

	if (strcmp("open", argv[0]) == 0)
		plink_action = NL80211_PLINK_ACTION_OPEN;
	else if (strcmp("block", argv[0]) == 0)
		plink_action = NL80211_PLINK_ACTION_BLOCK;
	else {
		fprintf(stderr, "plink action not supported\n");
		return 2;
	}
	argc--;
	argv++;

	if (argc)
		return 1;

	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);
	NLA_PUT_U8(msg, NL80211_ATTR_STA_PLINK_ACTION, plink_action);

	return 0;
 nla_put_failure:
	return -ENOBUFS;
}
COMMAND_ALIAS(station, set, "<MAC address> plink_action <open|block>",
	NL80211_CMD_SET_STATION, 0, CIB_NETDEV, handle_station_set_plink,
	"Set mesh peer link action for this station (peer).",
	select_station_cmd, station_set_plink);

static int handle_station_set_vlan(struct nl80211_state *state,
				   struct nl_msg *msg,
				   int argc, char **argv,
				   enum id_input id)
{
	unsigned char mac_addr[ETH_ALEN];
	unsigned long sta_vlan = 0;
	char *err = NULL;

	if (argc < 3)
		return 1;

	if (mac_addr_a2n(mac_addr, argv[0])) {
		fprintf(stderr, "invalid mac address\n");
		return 2;
	}
	argc--;
	argv++;

	if (strcmp("vlan", argv[0]) != 0)
		return 1;
	argc--;
	argv++;

	sta_vlan = strtoul(argv[0], &err, 0);
	if (err && *err) {
		fprintf(stderr, "invalid vlan id\n");
		return 2;
	}
	argc--;
	argv++;

	if (argc)
		return 1;

	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);
	NLA_PUT_U32(msg, NL80211_ATTR_STA_VLAN, sta_vlan);

	return 0;
 nla_put_failure:
	return -ENOBUFS;
}
COMMAND_ALIAS(station, set, "<MAC address> vlan <ifindex>",
	NL80211_CMD_SET_STATION, 0, CIB_NETDEV, handle_station_set_vlan,
	"Set an AP VLAN for this station.",
	select_station_cmd, station_set_vlan);

static int handle_station_set_mesh_power_mode(struct nl80211_state *state,
					      struct nl_msg *msg,
					      int argc, char **argv,
					      enum id_input id)
{
	unsigned char mesh_power_mode;
	unsigned char mac_addr[ETH_ALEN];

	if (argc < 3)
		return 1;

	if (mac_addr_a2n(mac_addr, argv[0])) {
		fprintf(stderr, "invalid mac address\n");
		return 2;
	}
	argc--;
	argv++;

	if (strcmp("mesh_power_mode", argv[0]) != 0)
		return 1;
	argc--;
	argv++;

	if (strcmp("active", argv[0]) == 0)
		mesh_power_mode = NL80211_MESH_POWER_ACTIVE;
	else if (strcmp("light", argv[0]) == 0)
		mesh_power_mode = NL80211_MESH_POWER_LIGHT_SLEEP;
	else if (strcmp("deep", argv[0]) == 0)
		mesh_power_mode = NL80211_MESH_POWER_DEEP_SLEEP;
	else {
		fprintf(stderr, "unknown mesh power mode\n");
		return 2;
	}
	argc--;
	argv++;

	if (argc)
		return 1;

	NLA_PUT(msg, NL80211_ATTR_MAC, ETH_ALEN, mac_addr);
	NLA_PUT_U32(msg, NL80211_ATTR_LOCAL_MESH_POWER_MODE, mesh_power_mode);

	return 0;
nla_put_failure:
	return -ENOBUFS;
}
COMMAND_ALIAS(station, set, "<MAC address> mesh_power_mode "
	"<active|light|deep>", NL80211_CMD_SET_STATION, 0, CIB_NETDEV,
	handle_station_set_mesh_power_mode,
	"Set link-specific mesh power mode for this station",
	select_station_cmd, station_set_mesh_power_mode);

static int handle_station_dump(struct nl80211_state *state,
			       struct nl_msg *msg,
			       int argc, char **argv,
			       enum id_input id)
{
	register_handler(print_sta_handler, *argv);
	return 0;
}
COMMAND(station, dump, "[-v]",
	NL80211_CMD_GET_STATION, NLM_F_DUMP, CIB_NETDEV, handle_station_dump,
	"List all stations known, e.g. the AP on managed interfaces");
