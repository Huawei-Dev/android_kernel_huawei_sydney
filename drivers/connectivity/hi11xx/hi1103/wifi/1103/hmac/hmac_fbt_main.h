

#ifndef __HMAC_FBT_MAIN_H__
#define __HMAC_FBT_MAIN_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_types.h"
#include "oam_ext_if.h"
#include "hmac_vap.h"
#include "hmac_fsm.h"
#include "frw_ext_if.h"
#include "hmac_device.h"
#include "oal_cfg80211.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_FBT_H


/*****************************************************************************
  2 ??????
*****************************************************************************/
#define HMAC_FBT_DEFAULT_DISABLE_INTERVAL  0

/*****************************************************************************
  3 ????????
*****************************************************************************/
typedef enum
{
    HMAC_FBT_LINK_WEAK       = 0,       /* ????????????????????????STA??AP?????????????????????? */
    HMAC_FBT_LINK_STRONG     = 1,       /* ????????????????????????STA??AP?????????????????????? */
    HMAC_FBT_STA_ONLINE      = 2,       /* ?????????????????????? */
    HMAC_FBT_STA_OFFLINE     = 3,       /* ???????????????????? */
    HMAC_FBT_STA_FOUND       = 4,       /* ???????????????????????? */
    HMAC_FBT_LINK_BUTT
}hmac_fbt_notify_type_enum;
typedef oal_uint8  hmac_fbt_notify_type_enum_uint8;

/* ??????????????????????????:??????AC???? */
typedef enum
{
    HMAC_FBT_MODE_CLOSE     = 0,
    HMAC_FBT_MODE_AC        = 1,

    HMAC_FBT_MODE_BUTT
}hmac_fbt_mode_enum;
typedef oal_uint8 hmac_fbt_mode_enum_uint8;

/* ?????????????????????? */
typedef enum
{
    HMAC_FBT_SCAN_SCOPE_ALL     = 0,     /* ?????????? */
    HMAC_FBT_SCAN_SCOPE_SELECT  = 1,  /* ???????????? */

    HMAC_FBT_SCAN_SCOPE_BUTT,
}hmac_fbt_scan_scope_enum;
typedef oal_uint8 hmac_fbt_scan_scope_enum_uint8;

/* ???????????????? */
typedef enum
{
    HMAC_FBT_ENABLE_STATE       = 0,       /* ?????????? */
    HMAC_FBT_DISASSC_STATE      = 1,       /* ???????????????????????????? */
    HMAC_FBT_FORBIDDEN_STATE    = 2,       /* ?????????????????? */

    HMAC_FBT_STATE_BUTT
}hmac_fbt_user_state_enum;
typedef oal_uint8  hmac_fbt_user_state_enum_uint8;


typedef oal_uint8 hmac_fbt_disable_state_enum_uint8;





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
/* ?????????????????? */
typedef struct
{
    oal_uint8                       auc_user_bssid[WLAN_MAC_ADDR_LEN];  /* ??????????????AP??bssid */
    oal_uint8                       uc_user_channel;                    /* ?????????????????? */
    oal_uint8                       uc_user_rssi;                       /* ?????????????????? */
    hmac_fbt_notify_type_enum       en_fbt_notify_type;                 /* ?????????????????? */
    oal_uint8                       auc_user_mac_addr[WLAN_MAC_ADDR_LEN];   /* ??????????MAC???? */
    oal_uint32                      ul_rssi_timestamp;                  /* ?????????????????? */
}hmac_fbt_notify_stru;

typedef struct {
     oal_uint32   timestamp;
     oal_uint8    ver;
     oal_uint8    rssi;
     oal_uint8    sta[WLAN_MAC_ADDR_LEN];
} hmac_fbt_detect_notify_stru;

/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
oal_uint32  hmac_fbt_notify(hmac_vap_stru *pst_hmac_vap, hmac_fbt_notify_stru *pst_notify);
#ifdef _PRE_WLAN_FEATURE_HILINK_HERA_PRODUCT
void  hmac_fbt_detect_notify(hmac_vap_stru *pst_hmac_vap, hmac_fbt_notify_stru *pst_hmac_fbt_notify);
#endif
extern oal_uint32  hmac_fbt_init(hmac_vap_stru *pst_hmac_vap);
extern oal_uint32  hmac_config_fbt_set_mode(mac_vap_stru *pst_mac_vap, oal_uint16 us_len, oal_uint8 *puc_param);
extern oal_uint32  hmac_fbt_start_scan(mac_vap_stru *pst_mac_vap);
extern oal_uint32  hmac_fbt_stop_scan(mac_vap_stru *pst_mac_vap);
extern oal_uint32  hmac_fbt_stop_scan_timer(mac_vap_stru *pst_mac_vap);
extern oal_uint32  hmac_fbt_stop_scan_report_timer(mac_vap_stru *pst_mac_vap);
#ifdef _PRE_WLAN_FEATURE_11K_EXTERN
extern oal_uint32  hmac_hilink_bcn_rpt_notify_hook(hmac_user_stru *pst_hmac_user, mac_rrm_state_enum en_rpt_state);
#endif
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_fbt_main.h */
