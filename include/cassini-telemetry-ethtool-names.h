// SPDX-License-Identifier: GPL-2.0-only or BSD-2-Clause
/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
 */

#ifndef __CASSINI_ETHTOOL_NAMES_H
#define __CASSINI_ETHTOOL_NAMES_H

#include "cassini-telemetry-items.h"

/* stats returned via .get_ethtool_stats()  */
#define CXI_GLOBAL_STATS_LEN    ARRAY_SIZE(cxi_get_ethtool_stats_set)
static const int cxi_get_ethtool_stats_set[] = {
        C_TELEM_HNI_RX_OK_IEEE,
        C_TELEM_HNI_RX_BAD_IEEE,
        C_TELEM_HNI_TX_OK_IEEE,
        C_TELEM_HNI_TX_POISONED_IEEE,
        C_TELEM_HNI_RX_OK_OPT,
        C_TELEM_HNI_RX_BAD_OPT,
        C_TELEM_HNI_TX_OK_OPT,
        C_TELEM_HNI_TX_POISONED_OPT,
        C_TELEM_HNI_PCS_CORRECTED_CW,
        C_TELEM_HNI_PCS_UNCORRECTED_CW,
        C_TELEM_HNI_PCS_GOOD_CW,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_0,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_1,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_2,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_3,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_4,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_5,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_6,
        C_TELEM_HNI_PFC_FIFO_OFLW_CNTR_7,
        C_TELEM_IXE_RX_UDP_PKT,
        C_TELEM_IXE_RX_TCP_PKT,
        C_TELEM_IXE_RX_IPV4_PKT,
        C_TELEM_IXE_RX_IPV6_PKT,
        C_TELEM_IXE_RX_ROCE_PKT,
        C_TELEM_IXE_RX_PTL_GEN_PKT,
        C_TELEM_IXE_RX_PTL_SML_PKT,
        C_TELEM_IXE_RX_PTL_UNRESTRICTED_PKT,
        C_TELEM_IXE_RX_PTL_SMALLMSG_PKT,
        C_TELEM_IXE_RX_PTL_CONTINUATION_PKT,
        C_TELEM_IXE_RX_PTL_RESTRICTED_PKT,
        C_TELEM_IXE_RX_PTL_CONNMGMT_PKT,
        C_TELEM_IXE_RX_PTL_RESPONSE_PKT,
        C_TELEM_IXE_RX_UNRECOGNIZED_PKT,
        C_TELEM_IXE_RX_PTL_SML_AMO_PKT,
        C_TELEM_IXE_RX_PTL_MSGS,
        C_TELEM_IXE_RX_PTL_MULTI_MSGS,
        C_TELEM_IXE_RX_PTL_MR_MSGS,
        C_TELEM_IXE_RX_PKT_DROP_PCT,
        C_TELEM_IXE_RX_PKT_DROP_RMU_NORSP,
        C_TELEM_IXE_RX_PKT_DROP_RMU_WRSP,
        C_TELEM_IXE_RX_PKT_DROP_IXE_PARSER,
        C_TELEM_IXE_RX_PKT_IPV4_OPTIONS,
        C_TELEM_IXE_RX_PKT_IPV6_OPTIONS,
        C_TELEM_IXE_RX_ETH_SEG,
        C_TELEM_IXE_RX_ROCE_SEG,
        C_TELEM_IXE_RX_ROCE_SPSEG,
};

static const char cxi_get_ethtool_stats_name[CXI_GLOBAL_STATS_LEN][ETH_GSTRING_LEN] = {
        "rx_ok_ieee",
        "rx_bad_ieee",
        "tx_ok_ieee",
        "tx_poisoned_ieee",
        "rx_ok_opt",
        "rx_bad_opt",
        "tx_ok_opt",
        "tx_poisoned_opt",
        "corrected_cw",
        "uncorrected_cw",
        "good_cw",
        "fifo_oflw_cntr_0",
        "fifo_oflw_cntr_1",
        "fifo_oflw_cntr_2",
        "fifo_oflw_cntr_3",
        "fifo_oflw_cntr_4",
        "fifo_oflw_cntr_5",
        "fifo_oflw_cntr_6",
        "fifo_oflw_cntr_7",
        "rx_udp_pkt",
        "rx_tcp_pkt",
        "rx_ipv4_pkt",
        "rx_ipv6_pkt",
        "rx_roce_pkt",
        "rx_ptl_gen_pkt",
        "rx_ptl_sml_pkt",
        "rx_ptl_unrestricted_pkt",
        "rx_ptl_smallmsg_pkt",
        "rx_ptl_continuation_pkt",
        "rx_ptl_restricted_pkt",
        "rx_ptl_connmgmt_pkt",
        "rx_ptl_response_pkt",
        "rx_unrecognized_pkt",
        "rx_ptl_sml_amo_pkt",
        "rx_ptl_msgs",
        "rx_ptl_multi_msgs",
        "rx_ptl_mr_msgs",
        "rx_pkt_drop_pct",
        "rx_pkt_drop_rmu_norsp",
        "rx_pkt_drop_rmu_wrsp",
        "rx_pkt_drop_ixe_parser",
        "rx_pkt_ipv4_options",
        "rx_pkt_ipv6_options",
        "rx_eth_seg",
        "rx_roce_seg",
        "rx_roce_spseg",
};

#endif