#include <net/if.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <netlink/genl/genl.h>
#include <netlink/genl/family.h>
#include <netlink/genl/ctrl.h>
#include <netlink/msg.h>
#include <netlink/attr.h>

#include "wifistatus/nl80211.h"
#include "wifistatus/iw.h"

struct link_result {
	uint8_t bssid[8];
	bool link_found;
	bool anything_found;
};

static struct link_result lr = { .link_found = false };

extern struct s_wifi_status __wifi;
/*
 * Use Command "iw dev_name link" to get wifi link freq
 */
static int get_link_freq(struct nl_msg *msg, void *arg) {
	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *bss[NL80211_BSS_MAX + 1];
	static struct nla_policy bss_policy[NL80211_BSS_MAX + 1] =
			{ [NL80211_BSS_TSF] = { .type = NLA_U64 }, [NL80211_BSS_FREQUENCY
					] = { .type = NLA_U32 }, [NL80211_BSS_BSSID] = { },
					[NL80211_BSS_BEACON_INTERVAL] = { .type = NLA_U16 },
					[NL80211_BSS_CAPABILITY] = { .type = NLA_U16 },
					[NL80211_BSS_INFORMATION_ELEMENTS] = { },
					[NL80211_BSS_SIGNAL_MBM] = { .type = NLA_U32 },
					[NL80211_BSS_SIGNAL_UNSPEC] = { .type = NLA_U8 },
					[NL80211_BSS_STATUS] = { .type = NLA_U32 }, };
	struct link_result *result = arg;

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
			genlmsg_attrlen(gnlh, 0), NULL);

	if (!tb[NL80211_ATTR_BSS])
		return NL_SKIP;

	if (nla_parse_nested(bss, NL80211_BSS_MAX, tb[NL80211_ATTR_BSS],
			bss_policy))
		return NL_SKIP;

	if (!bss[NL80211_BSS_BSSID])
		return NL_SKIP;

	if (!bss[NL80211_BSS_STATUS])
		return NL_SKIP;

	mac_addr_n2a(__wifi.mac_addr, nla_data(bss[NL80211_BSS_BSSID]));
	if_indextoname(nla_get_u32(tb[NL80211_ATTR_IFINDEX]), __wifi.dev);

	result->anything_found = true;

	if (bss[NL80211_BSS_FREQUENCY])
		__wifi.freq = nla_get_u32(bss[NL80211_BSS_FREQUENCY]);

	if (nla_get_u32(bss[NL80211_BSS_STATUS]) != NL80211_BSS_STATUS_ASSOCIATED)
		return NL_SKIP;

	/* only in the assoc case do we want more info from station get */
	result->link_found = true;
	memcpy(result->bssid, nla_data(bss[NL80211_BSS_BSSID]), 6);
	return NL_SKIP;
}

static int handle_scan_for_link(struct nl80211_state *state,
				struct nl_msg *msg,
				int argc, char **argv,
				enum id_input id)
{
	if (argc > 0)
		return 1;

	//register_handler(link_bss_handler, &lr);
	register_handler(get_link_freq, &lr);
	return 0;
}

static int handle_link_sta(struct nl80211_state *state,
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

//	register_handler(print_link_sta, NULL);

	return 0;
 nla_put_failure:
	return -ENOBUFS;
}

static int handle_link(struct nl80211_state *state,
		       struct nl_msg *msg, int argc, char **argv,
		       enum id_input id)
{
	char *link_argv[] = {
		NULL,
		"link",
		"get_bss",
		NULL,
	};
	char *station_argv[] = {
		NULL,
		"link",
		"get_sta",
		NULL,
		NULL,
	};
	char bssid_buf[3*6];
	int err;

	link_argv[0] = argv[0];
	err = handle_cmd(state, id, 3, link_argv);
	if (err)
		return err;

	if (!lr.link_found) {
		if (!lr.anything_found)
			printf("Not connected.\n");
		return 0;
	}

	mac_addr_n2a(bssid_buf, lr.bssid);
	bssid_buf[17] = '\0';

	station_argv[0] = argv[0];
	station_argv[3] = bssid_buf;
	return handle_cmd(state, id, 4, station_argv);
}
TOPLEVEL(link, NULL, 0, 0, CIB_NETDEV, handle_link,
	 "Print information about the current link, if any.");
HIDDEN(link, get_sta, "", NL80211_CMD_GET_STATION, 0,
	CIB_NETDEV, handle_link_sta);
HIDDEN(link, get_bss, NULL, NL80211_CMD_GET_SCAN, NLM_F_DUMP,
	CIB_NETDEV, handle_scan_for_link);
