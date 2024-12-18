

#ifndef __DMAC_VAP_H__
#define __DMAC_VAP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_ext_if.h"
#include "hal_ext_if.h"
#include "mac_vap.h"
#include "dmac_user.h"
#include "dmac_ext_if.h"
#include "dmac_ap_pm.h"
#ifdef _PRE_WLAN_FEATURE_BTCOEX
#include "dmac_btcoex.h"
#endif

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_DMAC_VAP_H

/*****************************************************************************
  2 ??????
*****************************************************************************/
#define DMAC_MAX_SW_RETRIES         10  /* ???????????????? */
#define DMAC_MGMT_MAX_SW_RETRIES    3   /* ???????????????? */
#define DMAC_MAX_AMPDU_LENGTH_PERFOMANCE_COUNT    32   /* ????????????????????????*/
#ifdef _PRE_WLAN_FEATURE_BTCOEX
#define BTCOEX_LINKLOSS_OCCUPIED_NUMBER (8)
#endif
#if(_PRE_WLAN_FEATURE_PMF == _PRE_PMF_HW_CCMP_BIP)

#define IS_OPEN_PMF_REG(_pst_dmac_vap)  (0 != ((_pst_dmac_vap)->ul_user_pmf_status))
#endif

#define DMAC_IS_LINKLOSS(_pst_dmac_vap) ((_pst_dmac_vap->st_linkloss_info.us_link_loss) > (_pst_dmac_vap->st_linkloss_info.us_linkloss_threshold))

#define DMAC_MAX_TX_SUCCESSIVE_FAIL_PRINT_THRESHOLD_BTCOEX    (40)   /* ??????????????????RF??????????*/
#define DMAC_MAX_TX_SUCCESSIVE_FAIL_PRINT_THRESHOLD    (20)   /* ??????????????????RF??????????*/

/*****************************************************************************
  3 ????????
*****************************************************************************/
/* beacon?????????? */
typedef enum
{
    DMAC_VAP_BEACON_BUFFER1,
    DMAC_VAP_BEACON_BUFFER2,

    DMAC_VAP_BEACON_BUFFER_BUTT
}dmac_vap_beacon_buffer_enum;
/* ??????????????????????????????????*/
/*0:1~14 */
/*1:15~17 */
/*2:18~30 */
/*3:31~32 */
typedef enum
{
    DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_0,
    DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_1,
    DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_2,
    DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_3,
    DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_BUTT
}dmac_count_by_ampdu_length_enum;
/* ??????AMPDU??????????????*/
typedef enum
{
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_1 = 1,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_14 = 14,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_15 = 15,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_17 = 17,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_18 = 18,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_30 = 30,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_31 = 31,
    DMAC_AMPDU_LENGTH_COUNT_LEVEL_32 = 32
}dmac_count_by_ampdu_length_level_enum;

typedef oal_uint8 dmac_vap_beacon_buffer_enum_uint8;
#ifdef _PRE_WLAN_DFT_STAT
#define   DMAC_VAP_DFT_STATS_PKT_INCR(_member, _cnt)        ((_member) += (_cnt))
#define   DMAC_VAP_DFT_STATS_PKT_SET_ZERO(_member)        ((_member) = (0))
#else
#define   DMAC_VAP_DFT_STATS_PKT_INCR(_member, _cnt)
#define   DMAC_VAP_DFT_STATS_PKT_SET_ZERO(_member)
#endif
#define   DMAC_VAP_STATS_PKT_INCR(_member, _cnt)            ((_member) += (_cnt))


/*****************************************************************************
  4 ????????????
*****************************************************************************/


/*****************************************************************************
  5 ??????????
*****************************************************************************/


/*****************************************************************************
  6 ????????
*****************************************************************************/


/*****************************************************************************
  7 STRUCT????
*****************************************************************************/
/* vap tx???????????????????????????? */
typedef struct
{
    oal_uint16  us_rts_threshold;       /* rts???? */
    oal_uint8   uc_mcast_rate;          /* ???????? */
    oal_uint8   auc_resv[1];            /* ???????? */
}dmac_vap_tx_param_stru;

#ifdef _PRE_WLAN_FEATURE_ROAM

#define ROAM_TRIGGER_COUNT_THRESHOLD           (5)
#define ROAM_TRIGGER_RSSI_NE80_DB              (-80)
#define ROAM_TRIGGER_RSSI_NE75_DB              (-75)
#define ROAM_TRIGGER_RSSI_NE70_DB              (-70)
#define ROAM_TRIGGER_INTERVAL_10S              (10 * 1000)
#define ROAM_TRIGGER_INTERVAL_15S              (15 * 1000)
#define ROAM_TRIGGER_INTERVAL_20S              (20 * 1000)
#define ROAM_WPA_CONNECT_INTERVAL_TIME         (5 * 1000)    /* ??????????????????????????????WIFI+ ???????????? */

typedef struct
{
    oal_int8    c_trigger_2G;           /* 2G????????????   */
    oal_int8    c_trigger_5G;           /* 5G????????????   */
    oal_uint8   auc_recv[2];
    oal_uint32  ul_cnt;                 /* ??????????????       */
    oal_uint32  ul_time_stamp;          /* ????????????????     */
    oal_uint32  ul_ip_obtain_stamp;     /* ????????IP?????????? */
    oal_uint32  ul_ip_addr_obtained;    /* IP?????????????? */
}dmac_vap_roam_trigger_stru;
#endif //_PRE_WLAN_FEATURE_ROAM

/*????????????????????????SDT??????????????????*/
typedef struct
{
    oal_int32    ul_signal;

    oal_uint32   ul_drv_rx_pkts;     /* ?????????????????? */
    oal_uint32   ul_hw_tx_pkts;      /* ?????????????????????????????? */
    oal_uint32   ul_drv_rx_bytes;    /* ??????????????????????80211???? */
    oal_uint32   ul_hw_tx_bytes;     /* ?????????????????????????????? */
    oal_uint32   ul_tx_retries;      /*????????????*/
    oal_uint32   ul_rx_dropped_misc; /*????????????*/
    oal_uint32   ul_tx_failed;     /* ???????????????????????????? */
//    oal_rate_info_stru st_txrate; /*vap????????*/

    oal_int16    s_free_power;  /*????*/
    /*????????????????????????????????????????????*/
#ifdef _PRE_WLAN_DFT_STAT
    /***************************************************************************
                                ??????????
    ***************************************************************************/

    /* ????????????????????(????????????)??????????MPDU?????? */
    oal_uint32  ul_rx_ppdu_dropped;                             /* ??????????vap_id????????????MPDU???? */

    /* ????????????????MPDU??????????????????????MAC?????????????? */
    oal_uint32  ul_rx_mpdu_total_num;                           /* ????????????????????????????MPDU?????? */

    /* MPDU??????????????????????????MPDU???????? */
    oal_uint32  ul_rx_ta_check_dropped;                         /* ????????????????????????????MPDU */
    oal_uint32  ul_rx_key_search_fail_dropped;                  /*  */
    oal_uint32  ul_rx_tkip_mic_fail_dropped;                    /* ??????????status?? tkip mic fail????MPDU */
    oal_uint32  ul_rx_replay_fail_dropped;                      /* ??????????????????MPDU */
    oal_uint32  ul_rx_security_check_fail_dropped;              /* ????????????*/
    oal_uint32  ul_rx_alg_process_dropped;                      /* ???????????????? */
    oal_uint32  ul_rx_null_frame_dropped;                       /* ??????????????(????????????????????????????) */
    oal_uint32  ul_rx_abnormal_dropped;                         /* ????????????????MPDU */
    oal_uint32  ul_rx_mgmt_mpdu_num_cnt;                         /* ??????????????????????????*/
    oal_uint32  ul_rx_mgmt_abnormal_dropped;                    /* ??????????????????????????vap????dev?????? */

    /***************************************************************************
                                ??????????
    ***************************************************************************/
    oal_uint32  ul_drv_tx_pkts;     /* ???????????????????????????? */
    oal_uint32  ul_drv_tx_bytes;    /* ????????????????????80211???? */
    /* ??????????????????????????????????????MSDU???? */
    oal_uint32  ul_tx_abnormal_mpdu_dropped;                    /* ????????????MPDU????????vap????user?????????? */
    /* ??????????????????????????????????????MPDU???? */
    oal_uint32  ul_tx_mpdu_succ_num;                            /* ????MPDU?????? */
    oal_uint32  ul_tx_mpdu_fail_num;                            /* ????MPDU???????? */
    oal_uint32  ul_tx_ampdu_succ_num;                           /* ??????????AMPDU?????? */
    oal_uint32  ul_tx_mpdu_in_ampdu;                            /* ????AMPDU??MPDU?????????? */
    oal_uint32  ul_tx_ampdu_fail_num;                           /* ????AMPDU???????? */
    oal_uint32  ul_tx_mpdu_fail_in_ampdu;                       /* ????AMPDU??MPDU???????????? */
    oal_uint32  aul_tx_count_per_apmpdu_length[DMAC_COUNT_BY_AMPDU_LENGTH_INDEX_BUTT];/*??????????????????????????????????*/
    oal_uint32  ul_tx_cts_fail;                                  /*????rts??????????*/
    oal_uint8  uc_tx_successive_mpdu_fail_num;                  /*??????????????????*/
    oal_uint8  uc_reserve[3];                                   /*????????*/
#endif
}dmac_vap_query_stats_stru;

typedef oal_uint8 dmac_linkloss_status_enum_uint8;


typedef struct
{
    oal_uint32                        ul_old_dot11BeaconPeriod;     /* ????beacon??????????????????linkloss?????????? */
    oal_uint16                        us_linkloss_threshold;       /*  LinkLoss????  */
    oal_uint16                        us_link_loss;                /*  LinkLoss?????? */
    oal_bool_enum_uint8               en_old_bt_on;                 /* ????bt??????????????????bt?????????????? */
    oal_bool_enum_uint8               en_old_dbac_runing;         /* ????dbac??????????????????dbac?????????????? */
    oal_bool_enum_uint8               en_old_far_distance;        /* ??????????????????????????true*/
    oal_uint8                         uc_linkloss_times;            /* ????linkloss????????????????????????????????beacon?????????? */
}dmac_vap_linkloss_stru;


#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
#define DMAC_MAX_IPV4_ENTRIES         8
#define DMAC_MAX_IPV6_ENTRIES         8

typedef union
{
    oal_uint32                    ul_value;
    oal_uint8                     auc_value[OAL_IPV4_ADDR_SIZE];
}un_ipv4_addr;

typedef struct
{
    un_ipv4_addr        un_local_ip;
    un_ipv4_addr        un_mask;
}dmac_vap_ipv4_addr_stru;

typedef struct
{
    oal_uint8                         auc_ip_addr[OAL_IPV6_ADDR_SIZE];
}dmac_vap_ipv6_addr_stru;

typedef struct
{
    dmac_vap_ipv4_addr_stru           ast_ipv4_entry[DMAC_MAX_IPV4_ENTRIES];
    dmac_vap_ipv6_addr_stru           ast_ipv6_entry[DMAC_MAX_IPV6_ENTRIES];
}dmac_vap_ip_entries_stru;
#endif

#ifdef _PRE_WLAN_FEATURE_PROXYSTA
typedef struct
{
    oal_uint16  aus_last_qos_seq_num[WLAN_TID_MAX_NUM];         /* ??????Proxy STA??????????????Root Ap????????QoS????seq num */
    oal_uint16  us_non_qos_seq_num;                             /* ??????Proxy STA??????????????Root Ap??????????QOS????seq num */
    oal_uint8   uc_lut_idx;
    oal_uint8   auc_resv[1];
} dmac_vap_psta_stru;
#define dmac_vap_psta_lut_idx(vap)   ((vap)->st_psta.uc_lut_idx)
#endif

#ifdef _PRE_WLAN_FEATURE_11K
typedef struct mac_rrm_info_tag
{
    mac_action_rm_rpt_stru              *pst_rm_rpt_action;
    mac_meas_rpt_ie_stru                *pst_meas_rpt_ie;           /* Measurement Report IE Addr */
    mac_bcn_rpt_stru                    *pst_bcn_rpt_item;          /* Beacon Report Addr */
    oal_netbuf_stru                     *pst_rm_rpt_mgmt_buf;       /* Report Frame Addr for Transfer */
    mac_scan_req_stru                   *pst_scan_req;

    oal_uint8                            uc_quiet_count;
    oal_uint8                            uc_quiet_period;
    oal_mac_quiet_state_uint8            en_quiet_state;
    oal_uint8                            uc_link_dialog_token;

    oal_uint8                            uc_ori_max_reg_pwr;
    oal_uint8                            uc_local_pwr_constraint;
    oal_uint8                            uc_ori_max_pwr_flush_flag;
    oal_uint8                            uc_rsv;

    oal_int8                             c_link_tx_pwr_used;
    oal_int8                             c_link_max_tx_pwr;
    oal_uint8                            auc_act_meas_start_time[8];
    oal_uint16                           us_quiet_duration;

    oal_uint16                           us_quiet_offset;
    oal_uint16                           us_rm_rpt_action_len;      /* Report Frame Length for Transfer */

    oal_dlist_head_stru                  st_meas_rpt_list;
    mac_bcn_req_info_stru                st_bcn_req_info;
    frw_timeout_stru                     st_quiet_timer;    /* quiet????????????????quiet????????quiet duration??????????????quiet */
    frw_timeout_stru                     st_offset_timer;   /* ????????tbtt??????????????offset????????offset??????????????????quiet????????quiet?????? */
}mac_rrm_info_stru;

#endif //_PRE_WLAN_FEATURE_11K

#ifdef _PRE_WLAN_FEATURE_VOWIFI
typedef struct
{
    oal_uint8           uc_rssi_trigger_cnt;        /* ?????????????????????????????? ??1??100??*/
    oal_uint8           auc_resv[3];
    oal_uint64          ull_rssi_timestamp_ms;     /* ????????ms???? */
    oal_uint64          ull_arp_timestamp_ms;      /* arp req device??????????(tx 5s????????????????) */

    oal_uint32   ul_arp_rx_succ_pkts;  /* TX arp_req??????????????rx_succ_pkts?? */
    oal_uint32   ul_tx_failed;         /* ??????????????????????????????????????  */
    oal_uint32   ul_tx_total;          /* ????????????????????????  */

}mac_vowifi_status_stru;

#endif /* _PRE_WLAN_FEATURE_VOWIFI */

/* dmac vap */
typedef struct dmac_vap_tag
{
    mac_vap_stru                     st_vap_base_info;                                  /* ????VAP???? */

    oal_uint32                       ul_active_id_bitmap;                               /* ????user??bitmap */

    oal_uint8                       *pauc_beacon_buffer[DMAC_VAP_BEACON_BUFFER_BUTT];   /* VAP????????beacon?? */
    oal_uint8                        uc_beacon_idx;                                     /* ????????????beacon???????? */
    oal_uint8                        uc_tim_offset_in_beacon;
    oal_uint16                       us_beacon_len;                                     /* beacon???????? */

    hal_to_dmac_vap_stru            *pst_hal_vap;                                       /* hal vap???? */
    hal_to_dmac_device_stru         *pst_hal_device;                                    /* hal device?????????????????? */

    dmac_vap_linkloss_stru           st_linkloss_info;                                  /* linkloss???????????? */
    oal_bool_enum_uint8              en_is_host_vap;                                    /* TRUE:??VAP??FALSE:??VAP */
    oal_uint8                        uc_default_ant_bitmap;                             /* ????????????bitmap, ?????????????????? */

    oal_uint8                        uc_sw_retry_limit;
    oal_uint8                        en_multi_user_multi_ac_flag;			    /* ????????????????????????????????*/

    oal_traffic_type_enum_uint8      uc_traffic_type;                           /* ?????????????????????????????? */

    oal_uint8                        uc_sta_pm_open_by_host;                /* sta ??????????: HOST?????????????????? */
    oal_uint8                        uc_cfg_pm_mode;                            /* ?????????????????????? */
    oal_uint8                        uc_sta_pm_close_status;                    /* sta ??????????, ???????????????????????????? */

    /* ???????????? */
    oal_uint8                        uc_vap_tx_chain;                                   /* ????????????????????????????????????????????????????TXCS???????????????????????????????????????? */
    oal_uint16                       us_del_timeout;                                    /* ????????????????ba???? ??????0???????? */
    oal_uint8                        bit_bw_cmd:1;                                      /* ?????????????????????????? 0:No  1:Yes */
    oal_uint8                        bit_beacon_timeout_times:7;                           /* sta????beacon???????? */
    hal_tx_txop_alg_stru             st_tx_alg;                                         /* ?????????????????? */

#ifdef _PRE_WLAN_FEATURE_WEB_CFG_FIXED_RATE
    hal_tx_txop_alg_stru             st_tx_alg_vht;                                     /* VHT?????????????????? */
    hal_tx_txop_alg_stru             st_tx_alg_ht;                                      /* HT?????????????????? */
    hal_tx_txop_alg_stru             st_tx_alg_11ag;                                    /* 11a/g?????????????????? */
    hal_tx_txop_alg_stru             st_tx_alg_11b;                                     /* 11b?????????????????? */

    union
    {
        oal_uint8                    uc_mode_param_valid;                               /* ??????????????????????????????????????????(0=??, ????0=??) */
        struct{
            oal_uint8                bit_vht_param_vaild  : 1;                          /* VHT??????????????????????????(0=??????, 1=????) */
            oal_uint8                bit_ht_param_vaild   : 1;                          /* HT??????????????????????????(0=??????, 1=????) */
            oal_uint8                bit_11ag_param_vaild : 1;                          /* 11a/g??????????????????????????(0=??????, 1=????) */
            oal_uint8                bit_11b_param_vaild  : 1;                          /* 11b??????????????????????????(0=??????, 1=????) */
            oal_uint8                bit_reserve          : 4;
        }st_spec_mode;
    }un_mode_valid;
    oal_uint8                        auc_resv1[3];
#endif

    hal_tx_txop_alg_stru             st_tx_data_mcast;                                  /* ?????????????? */
    hal_tx_txop_alg_stru             st_tx_data_bcast;                                  /* ??????????????*/
    hal_tx_txop_alg_stru             ast_tx_mgmt_ucast[WLAN_BAND_BUTT];                  /* ??????????????*/
    hal_tx_txop_alg_stru             ast_tx_mgmt_bmcast[WLAN_BAND_BUTT];                 /* ????????????????????*/

    oal_void                        *p_alg_priv;                                        /* VAP?????????????????? */

    oal_uint8                       *puc_tim_bitmap;                                    /* ??????????tim_bitmap??AP???????? */
    oal_uint8                        uc_tim_bitmap_len;
    oal_uint8                        uc_ps_user_num;                                    /* ??????????????????????????AP???????? */
    oal_uint8                        uc_dtim_count;
    oal_uint8                        uc_uapsd_max_depth;                                /* U-APSD??????????????????*/
    oal_bool_enum_uint8              en_beacon_chain_active;                            /* beacon????????????????, 0-????, 1-???? */
    oal_bool_enum_uint8              en_dtim_ctrl_bit0;                                 /* ????????DTIM Control??????????0??????1?? */
#if defined(_PRE_WLAN_FEATURE_PM) || defined(_PRE_WLAN_FEATURE_STA_PM)
    oal_netbuf_stru                 *pst_wow_probe_resp;                                /* wow??????,??????probe response??*/
    oal_netbuf_stru                 *pst_wow_null_data;                                 /* wow??????,??????null data??,STA??????????*/
    oal_uint16                       us_wow_probe_resp_len;

#ifdef _PRE_WLAN_FEATURE_11K
    oal_uint8                        bit_bcn_table_switch: 1;
    oal_uint8                        bit_11k_enable      : 1;
    oal_uint8                        bit_11v_enable      : 1;
    oal_uint8                        bit_rsv1            : 5;
#else
    oal_uint8                        auc_resv2[1];
#endif

#ifdef _PRE_WLAN_FEATURE_11R
    oal_uint8                        bit_11r_enable      : 1;
    oal_uint8                        bit_rsv2            : 7;
#else
    oal_uint8                        auc_resv3[1];
#endif

#else

#ifdef _PRE_WLAN_FEATURE_11K
    oal_uint8                        bit_bcn_table_switch: 1;
    oal_uint8                        bit_11k_enable      : 1;
    oal_uint8                        bit_11v_enable      : 1;
    oal_uint8                        bit_rsv3            : 5;

#else
    oal_uint8                        auc_resv4[11];
#endif
#ifdef _PRE_WLAN_FEATURE_11R
    oal_uint8                        bit_11r_enable      : 1;
    oal_uint8                        bit_rsv4            : 7;
#else
    oal_uint8                        auc_resv5[1];
#endif

#endif
#ifdef _PRE_WLAN_FEATURE_PROXYSTA
    dmac_vap_psta_stru               st_psta;
#endif

    oal_uint32                       ul_obss_scan_timer_remain;                  /* 02??????????????65s, OBSS????????????????????????????????????????????????*/
    oal_uint8                        uc_obss_scan_timer_started;
    oal_uint8                        uc_bcn_tout_max_cnt;                      /* beacon?????????????????????? */

#ifdef _PRE_WLAN_FEATURE_STA_PM
    oal_uint8                       uc_null_frm_ofdm_succ_cnt;
    oal_uint8                       uc_null_frm_cnt;
#else
    oal_uint8                       uac_resv5[2];
#endif  /* _PRE_WLAN_FEATURE_STA_PM */

    oal_bool_enum_uint8              en_bfee_actived;
    oal_bool_enum_uint8              en_bfer_actived;
    oal_bool_enum_uint8              en_mu_bfee_actived;
    oal_bool_enum_uint8              en_txstbc_actived;
    oal_uint8                        uc_ps_poll_pending;
    #ifdef _PRE_WLAN_FEATURE_TSF_SYNC
    oal_uint8                        uc_beacon_miss_cnt;                /* beacon miss ???? */
    #else
    oal_uint8                        uac_resv4[1];
    #endif
#if(_PRE_WLAN_FEATURE_PMF == _PRE_PMF_HW_CCMP_BIP)
    oal_uint32                       ul_user_pmf_status;                 /* ??????vap??user pmf??????????????????????vap?????????????????? */
#endif

#if defined (_PRE_WLAN_FEATURE_PM) || defined(_PRE_WLAN_FEATURE_STA_PM)
    oal_void*                        pst_pm_handler;  //ap??sta????????????????????????????
#endif
    /*????????+????????????????????????????????????????SDT????????????*/
    dmac_vap_query_stats_stru        st_query_stats;
    /* ???????????? */
#ifdef _PRE_WLAN_FEATURE_ALWAYS_TX
    oal_uint8                        uc_protocol_rate_dscr;                          /* ?????????????????????????????????????????????????????? */
    oal_uint8                        uc_bw_flag;                                     /* ????????????40M???? */
    oal_uint8                        uc_short_gi;                                    /* short gi???????? */
#endif
    hal_to_dmac_vap_stru            *pst_p2p0_hal_vap;                               /* p2p0 hal vap???? */
#ifdef _PRE_WLAN_FEATURE_P2P
    mac_cfg_p2p_noa_param_stru         st_p2p_noa_param;
    mac_cfg_p2p_ops_param_stru         st_p2p_ops_param;
#endif

#ifdef _PRE_WLAN_FEATURE_ARP_OFFLOAD
    oal_switch_enum_uint8             en_arp_offload_switch;         /* ARP offload?????? */
    oal_uint8                         auc_resv9[3];
    dmac_vap_ip_entries_stru         *pst_ip_addr_info;              /* Host??IPv4??IPv6???? */
#endif

#ifdef _PRE_WLAN_FEATURE_TSF_SYNC
    oal_uint16                          us_sync_tsf_value;
    oal_uint64                          ul_old_beacon_timestamp;
#endif
#ifdef _PRE_WLAN_FEATURE_ROAM
    dmac_vap_roam_trigger_stru          st_roam_trigger;
#endif  //_PRE_WLAN_FEATURE_ROAM
#ifdef _PRE_WLAN_FEATURE_11K
    mac_rrm_info_stru                  *pst_rrm_info;
#endif //_PRE_WLAN_FEATURE_11K
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_vap_btcoex_stru            st_dmac_vap_btcoex;
#endif


#ifdef _PRE_WLAN_FEATURE_VOWIFI
    //mac_vowifi_status_stru              st_vowifi_status;
    mac_vowifi_status_stru              *pst_vowifi_status;
#endif /* _PRE_WLAN_FEATURE_VOWIFI */

    oal_uint16                       us_ext_tbtt_offset;            /* ????tbtt offset??????*/
    oal_uint16                       us_in_tbtt_offset;             /* ????tbtt offset??????*/
    oal_uint16                       us_beacon_timeout;             /* beacon timeout??????*/
	oal_uint8                  	     uc_psm_dtim_period;		    /* ??????????dtim period*/
	oal_uint8                  	     uc_psm_auto_dtim_cnt;		    /* ????dtim????????*/
	oal_uint16                  	 us_psm_listen_interval;		/* ??????????listen_interval*/

	oal_bool_enum_uint8              en_non_erp_exist;              /* sta??????????????non_erp station */
}dmac_vap_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/

OAL_STATIC OAL_INLINE wlan_vap_mode_enum_uint8  dmac_vap_get_bss_type(mac_vap_stru *pst_vap)
{
    return pst_vap->en_vap_mode;
}


OAL_STATIC OAL_INLINE oal_void dmac_vap_linkloss_clean(dmac_vap_stru *pst_dmac_vap)
{
#ifdef _PRE_WLAN_FEATURE_BTCOEX
    dmac_vap_btcoex_occupied_stru *pst_dmac_vap_btcoex_occupied;
#endif

    pst_dmac_vap->st_linkloss_info.us_link_loss = 0;

#ifdef _PRE_WLAN_FEATURE_BTCOEX
    pst_dmac_vap_btcoex_occupied = &(pst_dmac_vap->st_dmac_vap_btcoex.st_dmac_vap_btcoex_occupied);
    pst_dmac_vap_btcoex_occupied->uc_linkloss_index = 1;
    pst_dmac_vap_btcoex_occupied->uc_linkloss_occupied_times = 0;
#endif

}
#ifdef _PRE_WLAN_FEATURE_PROXYSTA

OAL_STATIC OAL_INLINE oal_void dmac_psta_init_vap(dmac_vap_stru *pst_dmac_vap)
{
    OAL_MEMZERO(&pst_dmac_vap->st_psta, OAL_SIZEOF(pst_dmac_vap->st_psta));
}

OAL_STATIC  OAL_INLINE  oal_void  dmac_psta_update_lut_range(mac_device_stru *pst_dev, dmac_vap_stru *pst_dmac_vap, oal_uint16 *us_start, oal_uint16 *us_stop)
{
    mac_vap_stru *pst_vap = &pst_dmac_vap->st_vap_base_info;

    if (mac_is_proxysta_enabled(pst_dev))
    {
        if(mac_vap_is_vsta(pst_vap))
        {
            *us_start = dmac_vap_psta_lut_idx(pst_dmac_vap);
            *us_stop  = dmac_vap_psta_lut_idx(pst_dmac_vap) + 1; // 32 is not valid for proxysta
        }
        else
        {
            *us_start = 0;
            *us_stop  = HAL_PROXYSTA_MAX_BA_LUT_SIZE; // hardware spec
        }
    }
    // else do nothing
}

#endif


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint32  dmac_vap_init(
                       dmac_vap_stru              *pst_vap,
                       oal_uint8                   uc_chip_id,
                       oal_uint8                   uc_device_id,
                       oal_uint8                   uc_vap_id,
                       mac_cfg_add_vap_param_stru *pst_param);
extern oal_uint32  dmac_vap_init_tx_frame_params(dmac_vap_stru *pst_dmac_vap, oal_bool_enum_uint8  en_mgmt_rate_init_flag);
extern oal_uint32  dmac_vap_init_tx_ucast_data_frame(dmac_vap_stru *pst_dmac_vap);
extern oal_uint32  dmac_vap_sta_reset(dmac_vap_stru *pst_dmac_vap);
extern oal_uint32  mac_vap_pause_tx(mac_vap_stru *pst_vap);
extern oal_uint32  mac_vap_resume_tx(mac_vap_stru *pst_vap);
extern oal_void  dmac_vap_pause_tx(mac_vap_stru *pst_mac_vap);

extern oal_void  dmac_vap_pause_tx_by_chl(mac_device_stru *pst_device, mac_channel_stru *pst_src_chl);
extern oal_void  dmac_vap_resume_tx_by_chl(mac_device_stru *pst_device, mac_channel_stru *pst_dst_channel);
extern oal_void  dmac_vap_update_bi_from_hw(mac_vap_stru *pst_mac_vap);
extern oal_void  dmac_vap_init_tx_data_ucast(dmac_vap_stru *pst_dmac_vap,oal_uint8 uc_protocol_mode, oal_uint8 uc_legacy_rate);
extern oal_uint32  dmac_vap_is_in_p2p_listen(mac_vap_stru *pst_mac_vap);
extern oal_void  dmac_vap_linkloss_init(dmac_vap_stru *pst_dmac_vap);
#if defined(_PRE_PRODUCT_ID_HI110X_DEV)

#ifdef _PRE_WLAN_FEATURE_VOWIFI
extern void dmac_vap_vowifi_init(dmac_vap_stru *pst_dmac_vap);
extern void dmac_vap_vowifi_exit(dmac_vap_stru *pst_dmac_vap);
#endif /* _PRE_WLAN_FEATURE_VOWIFI */

extern oal_void  dmac_one_packet_send_null_data(mac_device_stru *pst_mac_device, mac_vap_stru *pst_mac_vap, oal_bool_enum_uint8 en_ps);
#endif

#ifdef _PRE_WLAN_SW_CTRL_RSP
extern oal_void dmac_vap_update_rsp_frm_rate(oal_uint8 uc_vap_id, oal_uint8 uc_protocol_mode, oal_uint8 uc_bandwidth, oal_uint8 uc_ref_rate);
#endif /* */

#ifdef _PRE_WLAN_FEATURE_OPMODE_NOTIFY
extern oal_uint32 dmac_ie_proc_opmode_notify(mac_user_stru *pst_mac_user, mac_vap_stru *pst_mac_vap, mac_opmode_notify_stru *pst_opmode_notify);
extern oal_uint32 dmac_check_opmode_notify(mac_vap_stru *pst_mac_vap,oal_uint8 *puc_payload,oal_uint32 ul_msg_len,
                                                        mac_user_stru  *pst_mac_user);
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of dmac_vap.h */
