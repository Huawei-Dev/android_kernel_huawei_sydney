

#ifndef __HMAC_DATA_ACQ_H__
#define __HMAC_DATA_ACQ_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _PRE_WLAN_FEATURE_DAQ

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
#include "oal_types.h"
#include "mac_vap.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_HMAC_DATA_ACQ_H
/*****************************************************************************
  2 ??????
*****************************************************************************/


/*****************************************************************************
  3 ????????
*****************************************************************************/


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


/*****************************************************************************
  8 UNION????
*****************************************************************************/


/*****************************************************************************
  9 OTHERS????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/
extern oal_uint8 g_uc_data_acq_used;
extern oal_void  hmac_data_acq_init(oal_void);
extern oal_void  hmac_data_acq_exit(oal_void);
extern oal_void  hmac_data_acq_down_vap(mac_vap_stru *pst_mac_vap);

#endif  /* end of _PRE_WLAN_FEATURE_DAQ */


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of hmac_data_acq.h */
