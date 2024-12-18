



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "wlan_spec.h"
#include "mac_board.h"

#undef  THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_MAC_BOARD_C


/*****************************************************************************
  2 ????????????
*****************************************************************************/
/* HOST CRX???? */
frw_event_sub_table_item_stru g_ast_dmac_host_crx_table_etc[HMAC_TO_DMAC_SYN_BUTT];

/* DMAC??????HOST_DRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tx_host_drx_etc[DMAC_TX_HOST_DRX_BUTT];

/* DMAC??????WLAN_DTX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tx_wlan_dtx_etc[DMAC_TX_WLAN_DTX_BUTT];

#ifndef _PRE_WLAN_PROFLING_MIPS
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)  && defined (__CC_ARM)
#pragma arm section rwdata = "BTCM", code ="ATCM", zidata = "BTCM", rodata = "ATCM"
#endif
#endif
/* DMAC??????WLAN_CTX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_wlan_ctx_event_sub_table_etc[DMAC_WLAN_CTX_EVENT_SUB_TYPE_BUTT];

/* DMAC????,WLAN_DRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_wlan_drx_event_sub_table_etc[HAL_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST))
/* DMAC??????high prio?????????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table_etc[HAL_EVENT_DMAC_HIGH_PRIO_SUB_TYPE_BUTT];
#else
/* DMAC??????ERROR_IRQ?????????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_high_prio_event_sub_table_etc[HAL_EVENT_ERROR_IRQ_SUB_TYPE_BUTT];
#endif

/* DMAC????,WLAN_CRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_wlan_crx_event_sub_table_etc[HAL_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* DMAC??????TX_COMP???????????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tx_comp_event_sub_table_etc[HAL_TX_COMP_SUB_TYPE_BUTT];

/* DMAC????, TBTT?????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_tbtt_event_sub_table_etc[HAL_EVENT_TBTT_SUB_TYPE_BUTT];

/*DMAC????, MISC?????????????????? */
frw_event_sub_table_item_stru g_ast_dmac_misc_event_sub_table_etc[HAL_EVENT_DMAC_MISC_SUB_TYPE_BUTT];

/* WLAN_DTX ???????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_dtx_event_sub_table_etc[DMAC_TX_WLAN_DTX_BUTT];

/* HMAC???? WLAN_DRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_drx_event_sub_table_etc[DMAC_WLAN_DRX_EVENT_SUB_TYPE_BUTT];

/* HMAC???? WLAN_CRX???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_crx_event_sub_table_etc[DMAC_WLAN_CRX_EVENT_SUB_TYPE_BUTT];

/* HMAC???? TBTT???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_tbtt_event_sub_table_etc[DMAC_TBTT_EVENT_SUB_TYPE_BUTT];

/* HMAC???? ????HOST???????????????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_ctx_event_sub_table_etc[DMAC_TO_HMAC_SYN_BUTT];

/* HMAC???? MISC???????????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_wlan_misc_event_sub_table_etc[DMAC_MISC_SUB_TYPE_BUTT];

#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
/* HMAC???? IPC???????????????????????? */
frw_event_sub_table_item_stru g_ast_hmac_host_drx_event_sub_table[DMAC_TX_HOST_DRX_BUTT];
#endif


#ifndef _PRE_WLAN_PROFLING_MIPS
#if (_PRE_OS_VERSION_RAW == _PRE_OS_VERSION)  && defined (__CC_ARM)
#pragma arm section rodata, code, rwdata, zidata  // return to default placement
#endif
#endif



/*****************************************************************************
  3 ????????
*****************************************************************************/


oal_void  event_fsm_table_register_etc(oal_void)
{
    /* Part1: ??????Dmac????????*/

    /* ????DMAC????HOST_CRX???? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_HOST_CRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_host_crx_table_etc);

    /* ????DMAC????HOST_DRX?????????????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_HOST_DRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_tx_host_drx_etc);

    /* ????DMAC????WLAN_DTX?????????????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_tx_wlan_dtx_etc);

    /* ????DMAC????WLAN_CTX?????????????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_CTX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_dmac_wlan_ctx_event_sub_table_etc);

    /* ????DMAC????WLAN_DRX???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_wlan_drx_event_sub_table_etc);

    /* ????DMAC????WLAN_CRX????pipeline 0???? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_wlan_crx_event_sub_table_etc);

    /* ????DMAC????TX_COMP???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_TX_COMP, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_tx_comp_event_sub_table_etc);

    /* ????DMAC????TBTT???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_TBTT, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_tbtt_event_sub_table_etc);

#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST))
    /* ????DMAC???????????????? */
#else
    /* ????DMAC????ERR???????? */
#endif
    frw_event_table_register_etc(FRW_EVENT_TYPE_HIGH_PRIO, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_high_prio_event_sub_table_etc);

    /* ????DMAC????MISC???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_0, g_ast_dmac_misc_event_sub_table_etc);
    /* Part2: ??????Hmac????????*/

    /* ????WLAN_DTX???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_DTX, FRW_EVENT_PIPELINE_STAGE_0, g_ast_hmac_wlan_dtx_event_sub_table_etc);

    /* ????WLAN_DRX???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_DRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_drx_event_sub_table_etc);

    /* ????HMAC????WLAN_CRX???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_WLAN_CRX, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_crx_event_sub_table_etc);

     /* ????DMAC????MISC???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_DMAC_MISC, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_misc_event_sub_table_etc);

    /* ????TBTT???????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_TBTT, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_tbtt_event_sub_table_etc);

    /* ???????????????????????? */
    frw_event_table_register_etc(FRW_EVENT_TYPE_HOST_SDT_REG, FRW_EVENT_PIPELINE_STAGE_1, g_ast_hmac_wlan_ctx_event_sub_table_etc);
}


oal_void  event_fsm_unregister_etc(oal_void)
{
    /* Part1: ??????Dmac????????*/

    /* ??????DMAC????HOST_CRX???? */
    OAL_MEMZERO(g_ast_dmac_host_crx_table_etc, OAL_SIZEOF(g_ast_dmac_host_crx_table_etc));

    /* ??????DMAC????HOST_DRX?????????????? */
    OAL_MEMZERO(g_ast_dmac_tx_host_drx_etc, OAL_SIZEOF(g_ast_dmac_tx_host_drx_etc));

    /* ??????DMAC????WLAN_DTX?????????????? */
    OAL_MEMZERO(g_ast_dmac_tx_wlan_dtx_etc, OAL_SIZEOF(g_ast_dmac_tx_wlan_dtx_etc));

    /* ??????DMAC????WLAN_DRX???????? */
    OAL_MEMZERO(g_ast_dmac_wlan_drx_event_sub_table_etc, OAL_SIZEOF(g_ast_dmac_wlan_drx_event_sub_table_etc));

    /* ??????DMAC????WLAN_CRX???????? */
    OAL_MEMZERO(g_ast_dmac_wlan_crx_event_sub_table_etc, OAL_SIZEOF(g_ast_dmac_wlan_crx_event_sub_table_etc));

    /* ??????DMAC????TX_COMP???????? */
    OAL_MEMZERO(g_ast_dmac_tx_comp_event_sub_table_etc, OAL_SIZEOF(g_ast_dmac_tx_comp_event_sub_table_etc));

    /* ??????DMAC????TBTT???????? */
    OAL_MEMZERO(g_ast_dmac_tbtt_event_sub_table_etc, OAL_SIZEOF(g_ast_dmac_tbtt_event_sub_table_etc));

#if ((_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_DEV) || (_PRE_PRODUCT_ID == _PRE_PRODUCT_ID_HI1103_HOST))
    /* ??????DMAC???????????????? */
#else
    /* ??????DMAC????ERR???????? */
#endif
    OAL_MEMZERO(g_ast_dmac_high_prio_event_sub_table_etc, OAL_SIZEOF(g_ast_dmac_high_prio_event_sub_table_etc));

    /* ??????DMAC???????????????? */
    OAL_MEMZERO(g_ast_dmac_misc_event_sub_table_etc, OAL_SIZEOF(g_ast_dmac_misc_event_sub_table_etc));

    /* Part2: ??????Hmac????????*/
    OAL_MEMZERO(g_ast_hmac_wlan_dtx_event_sub_table_etc, OAL_SIZEOF(g_ast_hmac_wlan_dtx_event_sub_table_etc));

    OAL_MEMZERO(g_ast_hmac_wlan_drx_event_sub_table_etc, OAL_SIZEOF(g_ast_hmac_wlan_drx_event_sub_table_etc));

    OAL_MEMZERO(g_ast_hmac_wlan_crx_event_sub_table_etc, OAL_SIZEOF(g_ast_hmac_wlan_crx_event_sub_table_etc));

    OAL_MEMZERO(g_ast_hmac_tbtt_event_sub_table_etc, OAL_SIZEOF(g_ast_hmac_tbtt_event_sub_table_etc));

    OAL_MEMZERO(g_ast_hmac_wlan_ctx_event_sub_table_etc, OAL_SIZEOF(g_ast_hmac_wlan_ctx_event_sub_table_etc));

    OAL_MEMZERO(g_ast_hmac_wlan_misc_event_sub_table_etc, OAL_SIZEOF(g_ast_hmac_wlan_misc_event_sub_table_etc));

}



/*lint -e19*/
oal_module_symbol(g_ast_dmac_host_crx_table_etc);
oal_module_symbol(g_ast_dmac_tx_host_drx_etc);
oal_module_symbol(g_ast_dmac_tx_wlan_dtx_etc);
oal_module_symbol(g_ast_dmac_wlan_ctx_event_sub_table_etc);
oal_module_symbol(g_ast_dmac_wlan_drx_event_sub_table_etc);
oal_module_symbol(g_ast_dmac_high_prio_event_sub_table_etc);
oal_module_symbol(g_ast_dmac_wlan_crx_event_sub_table_etc);
oal_module_symbol(g_ast_dmac_tx_comp_event_sub_table_etc);
oal_module_symbol(g_ast_dmac_tbtt_event_sub_table_etc);
oal_module_symbol(g_ast_dmac_misc_event_sub_table_etc);

oal_module_symbol(g_ast_hmac_wlan_dtx_event_sub_table_etc);
oal_module_symbol(g_ast_hmac_wlan_drx_event_sub_table_etc);
oal_module_symbol(g_ast_hmac_wlan_crx_event_sub_table_etc);
oal_module_symbol(g_ast_hmac_tbtt_event_sub_table_etc);
oal_module_symbol(g_ast_hmac_wlan_ctx_event_sub_table_etc);
oal_module_symbol(g_ast_hmac_wlan_misc_event_sub_table_etc);
#if (_PRE_MULTI_CORE_MODE_OFFLOAD_DMAC != _PRE_MULTI_CORE_MODE)
oal_module_symbol(g_ast_hmac_host_drx_event_sub_table);
#endif


oal_module_symbol(event_fsm_table_register_etc);
oal_module_symbol(event_fsm_unregister_etc);

/*lint +e19*/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


