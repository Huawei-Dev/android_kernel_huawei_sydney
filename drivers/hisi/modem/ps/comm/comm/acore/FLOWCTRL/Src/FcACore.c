/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*****************************************************************************
  1 ??????????
*****************************************************************************/
#include "product_config.h"
#include "Fc.h"
#include "FcInterface.h"
#include "FcCdsInterface.h"
#include "FcCstInterface.h"
#include "AtFcInterface.h"
#include "AdsFcInterface.h"
#include "ACpuLoadInterface.h"
#include "ImmInterface.h"
#include "NFExtInterface.h"
#include "FcIntraMsg.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "PsLib.h"
#include "NVIM_Interface.h"
#include "FcACoreCReset.h"
#include "TTFUtil.h"
#include "FcACore.h"

/*****************************************************************************
    ??????????????????????.C??????????
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_ACORE_FLOW_CTRL_C


/*****************************************************************************
  2 ????????????
*****************************************************************************/
/* A?????????????? */
ACORE_FC_CFG_NV_STRU                    g_stFcAcoreCfg;

/* ?????????????????????? */
FC_BRIDGE_RATE_STRU                     g_stFcBridgeRate;

/* CDS????????????????????FC IdRAB???????? */

FC_RAB_MAPPING_INFO_SET_STRU            g_astFcRabMappingInfoSet[MODEM_ID_BUTT];

/* CPU?????????????????????????????? */
FC_CPU_CTRL_STRU                        g_stFcCpuACtrl;

FC_CPU_DRV_ASSEM_CONFIG_PARA_STRU       g_stCpuDriverAssePara =
{
    6,
    10,
    5,
    0,
    {
    /* cpuload, PC??????????????UE????TX????????, UE????TX??????UE????RX??????????UE????RX???????????????? */
        {
            85,{20, 40, 10, 10, 10, 56, {0, 0}}
        },
        {
            70,{15, 20, 10, 10, 10, 20, {0, 0}}
        },
        {
            60,{10, 10, 10, 10, 10, 5, {0, 0}}
        },
        {
            40,{5,  5,  10, 10, 10, 1, {0, 0}}
        },
        {
            0, {0,  1,  1,  1,  1, 1, {0, 0}}
        }
    }
};

/* ???????????????????? */
FC_CPU_DRV_ASSEM_PARA_ENTITY_STRU  g_stDrvAssemParaEntity = {0, 0, 0, 0, 0, {0, 0, 0, 0}, {100, {0, 0, 0, 0, 0, 0, {0, 0}}}, VOS_NULL_PTR, VOS_NULL_PTR};

VOS_SPINLOCK                       g_stFcMemSpinLock;

STATIC VOS_VOID  FC_BRIDGE_ResetRate( VOS_VOID );

STATIC VOS_UINT32  FC_BRIDGE_GetRate( VOS_VOID );
STATIC VOS_UINT32  FC_RmRateJudge( VOS_VOID );

STATIC VOS_VOID FC_DrvAssemInit(VOS_VOID);
STATIC VOS_VOID FC_JudgeAssemSmoothFactor(FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara);
STATIC FC_CPU_DRV_ASSEM_PARA_STRU *FC_GetCpuDrvAssemPara(VOS_UINT32 ulLev);
STATIC FC_CPU_DRV_ASSEM_PARA_STRU* FC_GetCurrentAssemPara(VOS_UINT32 ulAssemLev);
STATIC VOS_VOID FC_JudgeDrvAssemAction(VOS_UINT32 ulAssemLev);

STATIC FC_CPU_DRV_ASSEM_PARA_STRU* FC_SelectDrvAssemParaRule(VOS_UINT32 ulCpuLoad, VOS_UINT32 *pulAssemLev);
STATIC VOS_VOID FC_JudgeDrvAssemPara(VOS_UINT32 ulCpuLoad);

STATIC VOS_UINT32  FC_PsRateJudge( VOS_VOID );
STATIC VOS_VOID  FC_GetPsRate( VOS_UINT32 *pulUlRate, VOS_UINT32 *pulDlRate );
STATIC VOS_UINT32 FC_CPUA_DownJudge
(
    VOS_UINT32       ulCpuLoad,
    FC_CFG_CPU_STRU *pstFcCfgCpu,
    FC_POLICY_STRU  *pstFcPolicy
);

VOS_UINT32  FC_CPUA_StopFcAttempt( VOS_UINT32 ulParam1, VOS_UINT32 ulParam2 );

STATIC VOS_UINT32  FC_CPUA_UpProcess( VOS_VOID );
STATIC VOS_UINT32  FC_CPUA_DownProcess( VOS_VOID );
STATIC VOS_UINT32  FC_CPUA_StopFlowCtrl( VOS_VOID );

STATIC VOS_UINT32  FC_CPUA_Init( VOS_VOID );

STATIC VOS_UINT32  FC_MEM_AdjustPriForDown
(
    FC_PRI_ENUM_UINT8                  enPointPri,
    FC_ID_ENUM_UINT8                   enFcId
);

STATIC VOS_UINT32  FC_MEM_SndUpToTargetPriIndMsg(FC_PRI_ENUM_UINT8 enTargetPri, VOS_UINT16 usMemFreeCnt);
STATIC VOS_UINT32  FC_MEM_SndDownToTargetPriIndMsg( FC_PRI_ENUM_UINT8 enTargetPri, VOS_UINT16 usMemFreeCnt);

STATIC VOS_UINT32  FC_CST_UpProcess( VOS_UINT8 ucRabId );
STATIC VOS_UINT32  FC_CST_DownProcess( VOS_UINT8 ucRabId );
STATIC FC_RAB_MAPPING_INFO_STRU  *FC_CDS_GetFcInfo( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId );

STATIC VOS_UINT32  FC_CDS_Init( VOS_VOID );

STATIC VOS_VOID  FC_UpProcess(VOS_RATMODE_ENUM_UINT32 enRateMode);
STATIC VOS_VOID  FC_DownProcess(VOS_RATMODE_ENUM_UINT32 enRateMode);

STATIC VOS_UINT32  FC_RcvCstMsg( MsgBlock * pMsg );
STATIC VOS_UINT32  FC_RcvCdsMsg( MsgBlock * pMsg );
STATIC VOS_UINT32  FC_ACORE_RcvTimerMsg(REL_TIMER_MSG *pTimerMsg);
STATIC VOS_UINT32  FC_CDMA_DownProcess( VOS_VOID );
STATIC VOS_UINT32  FC_CDMA_UpProcess( VOS_VOID );
STATIC VOS_UINT32  FC_GPRS_DownProcess( VOS_VOID );

STATIC VOS_UINT32  FC_GPRS_UpProcess( VOS_VOID );

/*****************************************************************************
  3 ????????
*****************************************************************************/


STATIC VOS_VOID  FC_BRIDGE_ResetRate( VOS_VOID )
{
    g_stFcBridgeRate.ulLastByteCnt      = NFExt_GetBrBytesCnt();
    g_stFcBridgeRate.ulRate             = 0;
}



STATIC VOS_UINT32  FC_BRIDGE_GetRate( VOS_VOID )
{
    return(g_stFcBridgeRate.ulRate);
}



STATIC VOS_UINT32  FC_RmRateJudge( VOS_VOID )
{
    /* ????????????????????????????????????????CPU?? */
    if (g_stFcBridgeRate.ulRate > g_stFcAcoreCfg.stFcCfgCpuA.ulRmRateThreshold)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 FC_ACORE_RegDrvAssemFunc(FC_ACORE_DRV_ASSEMBLE_PARA_FUNC pFcDrvSetAssemParaFuncUe, FC_ACORE_DRV_ASSEMBLE_PARA_FUNC pFcDrvSetAssemParaFuncPc)
{
    /* ADS???????????????????????????????????????????? */
    
    /*FC_ADS_DRV_ASSEM_STRU stFcAdsAssemEntity;

    g_stDrvAssemParaEntity.pDrvSetAssemParaFuncUe = pFcDrvSetAssemParaFuncUe;
    g_stDrvAssemParaEntity.pDrvSetAssemParaFuncPc = pFcDrvSetAssemParaFuncPc;
    
    
    stFcAdsAssemEntity.ucEnableMask         = g_stCpuDriverAssePara.ucEnableMask;
    stFcAdsAssemEntity.ulDlRateUpLev        = FC_ADS_DL_RATE_UP_LEV;
    stFcAdsAssemEntity.ulDlRateDownLev      = FC_ADS_DL_RATE_DOWN_LEV;
    stFcAdsAssemEntity.ulExpireTmrLen       = FC_ADS_TIMER_LEN;
    stFcAdsAssemEntity.pDrvAssemFunc        = FC_JudgeDrvToMaxPara;
    
    ADS_DL_RegDrvAssemFunc(&stFcAdsAssemEntity);*/

    return VOS_OK;
}


VOS_VOID FC_ShowDrvAssemPara(VOS_VOID)
{
    FC_CPU_DRV_ASSEM_PARA_STRU *pstCpuDrvAssemPara  = VOS_NULL_PTR;
    VOS_INT i;

    (VOS_VOID)vos_printf("g_stCpuDriverAssePara.ucEnableMask = %d\n", g_stCpuDriverAssePara.ucEnableMask);

    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.stCurAssemLevPara.ulLev = %d\n", (VOS_INT32)(g_stDrvAssemParaEntity.ulCurLev));
    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.stCurAssemPara.ucEthTxMinNum = %d\n", g_stDrvAssemParaEntity.stCurAssemPara.stDrvAssemPara.ucEthTxMinNum);
    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.stCurAssemPara.ucCdsGuDlThres = %d\n", g_stDrvAssemParaEntity.stCurAssemPara.stDrvAssemPara.ucCdsGuDlThres);
    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.ucSmoothCntUp = %d\n", g_stDrvAssemParaEntity.ucSmoothCntUp);
    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.ucSmoothCntDown = %d\n", g_stDrvAssemParaEntity.ucSmoothCntDown);
    (VOS_VOID)vos_printf("g_stCpuDriverAssePara.ucSmoothCntUpLev = %d\n", g_stCpuDriverAssePara.ucSmoothCntUpLev);
    (VOS_VOID)vos_printf("g_stCpuDriverAssePara.ucSmoothCntDownLev = %d\n", g_stCpuDriverAssePara.ucSmoothCntDownLev);
    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.pDrvSetAssemParaFunc = %d\n", ((VOS_NULL_PTR == g_stDrvAssemParaEntity.pDrvSetAssemParaFuncUe) ? 1 : 0));
    (VOS_VOID)vos_printf("g_stDrvAssemParaEntity.ucSetDrvFailCnt = 0x%x\n", g_stDrvAssemParaEntity.ucSetDrvFailCnt);

    for (i = 0; i < FC_ACPU_DRV_ASSEM_LEV_BUTT; i++)
    {
        pstCpuDrvAssemPara = &g_stCpuDriverAssePara.stCpuDrvAssemPara[i];

        (VOS_VOID)vos_printf("pstCpuDrvAssemPara->ulCpuLoad = %d\n", (VOS_INT32)(pstCpuDrvAssemPara->ulCpuLoad));
        (VOS_VOID)vos_printf("pstCpuDrvAssemPara->stDrvAssemPara.ucEthRxMinNum = %d\n", pstCpuDrvAssemPara->stDrvAssemPara.ucEthRxMinNum);
        (VOS_VOID)vos_printf("pstCpuDrvAssemPara->stDrvAssemPara.ucEthRxTimeout = %d\n", pstCpuDrvAssemPara->stDrvAssemPara.ucEthRxTimeout);
        (VOS_VOID)vos_printf("pstCpuDrvAssemPara->stDrvAssemPara.ucEthTxMinNum = %d\n", pstCpuDrvAssemPara->stDrvAssemPara.ucEthTxMinNum);
        (VOS_VOID)vos_printf("pstCpuDrvAssemPara->stDrvAssemPara.ucEthTxTimeout = %d\n", pstCpuDrvAssemPara->stDrvAssemPara.ucEthTxTimeout);
        (VOS_VOID)vos_printf("pstCpuDrvAssemPara->stDrvAssemPara.ucHostOutTimeout = %d\n", pstCpuDrvAssemPara->stDrvAssemPara.ucHostOutTimeout);
    }
}


STATIC VOS_VOID FC_DrvAssemInit(VOS_VOID)
{
    FC_CPU_DRV_ASSEM_PARA_NV_STRU   stCpuDrvAssemPara;
    VOS_UINT32                      ulRst;

    ulRst = GUCTTF_NV_Read(MODEM_ID_0, en_NV_Item_FC_ACPU_DRV_ASSEMBLE_PARA, &stCpuDrvAssemPara, (VOS_UINT32)sizeof(FC_CPU_DRV_ASSEM_PARA_NV_STRU));

    /* NV?????????????????????? */
    if (NV_OK != ulRst)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_DrvAssemInit Read NV fail!\n");

        return;
    }

    g_stDrvAssemParaEntity.stCurAssemPara.ulCpuLoad = 100;
    g_stDrvAssemParaEntity.ulCurLev                 = FC_ACPU_DRV_ASSEM_LEV_1;
    g_stDrvAssemParaEntity.ucSetDrvFailCnt          = 0;

    /* NV??????????????????????????????????NV?????? */
    PSACORE_MEM_CPY(&g_stCpuDriverAssePara, (VOS_UINT32)sizeof(FC_CPU_DRV_ASSEM_PARA_NV_STRU), &stCpuDrvAssemPara, (VOS_UINT32)sizeof(FC_CPU_DRV_ASSEM_PARA_NV_STRU));
}


STATIC VOS_VOID FC_JudgeAssemSmoothFactor(FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara)
{
    /* ???????????????????????????????????? */
    if (g_stDrvAssemParaEntity.stCurAssemPara.ulCpuLoad > pstDrvAssemPara->ulCpuLoad)
    {
        /* ???????????? */
        g_stDrvAssemParaEntity.ucSmoothCntDown++;
        g_stDrvAssemParaEntity.ucSmoothCntUp = 0;
    }
    else if (g_stDrvAssemParaEntity.stCurAssemPara.ulCpuLoad < pstDrvAssemPara->ulCpuLoad)
    {
        /* ???????????? */
        g_stDrvAssemParaEntity.ucSmoothCntUp++;
        g_stDrvAssemParaEntity.ucSmoothCntDown = 0;
    }
    else
    {
        /* ?????????????????????????????????????????? */
        g_stDrvAssemParaEntity.ucSmoothCntUp    = 0;
        g_stDrvAssemParaEntity.ucSmoothCntDown  = 0;
    }
}



STATIC FC_CPU_DRV_ASSEM_PARA_STRU *FC_GetCpuDrvAssemPara(VOS_UINT32 ulLev)
{
    if (ulLev >= FC_ACPU_DRV_ASSEM_LEV_BUTT)
    {
        return VOS_NULL_PTR;
    }

    return &g_stCpuDriverAssePara.stCpuDrvAssemPara[ulLev];
}



STATIC FC_CPU_DRV_ASSEM_PARA_STRU* FC_GetCurrentAssemPara(VOS_UINT32 ulAssemLev)
{
    FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara;

    /* ?????????????????? */
    if (g_stDrvAssemParaEntity.ucSmoothCntUp >= g_stCpuDriverAssePara.ucSmoothCntUpLev)
    {
        g_stDrvAssemParaEntity.ulCurLev = ulAssemLev;
    }
    /* ???????????????????????????? */
    else if (g_stDrvAssemParaEntity.ucSmoothCntDown >= g_stCpuDriverAssePara.ucSmoothCntDownLev)
    {
        if (g_stDrvAssemParaEntity.ulCurLev < (FC_ACPU_DRV_ASSEM_LEV_BUTT - 1))
        {
             g_stDrvAssemParaEntity.ulCurLev++;
        }
    }
    else
    {
        return VOS_NULL_PTR;
    }

    pstDrvAssemPara = FC_GetCpuDrvAssemPara(g_stDrvAssemParaEntity.ulCurLev);

    return pstDrvAssemPara;
}


VOS_UINT32  FC_MNTN_TraceDrvAssemPara(FC_DRV_ASSEM_PARA_STRU *pstDrvAssenPara)
{
    FC_MNTN_DRV_ASSEM_PARA_STRU     stFcMntnDrvAssemPara;

    if (VOS_NULL_PTR == pstDrvAssenPara)
    {
        return VOS_ERR;
    }

    PSACORE_MEM_SET(&stFcMntnDrvAssemPara, sizeof(FC_MNTN_DRV_ASSEM_PARA_STRU), 0, sizeof(FC_MNTN_DRV_ASSEM_PARA_STRU));

    stFcMntnDrvAssemPara.ulSenderCpuId   = VOS_LOCAL_CPUID;
    stFcMntnDrvAssemPara.ulSenderPid     = UEPS_PID_FLOWCTRL;
    stFcMntnDrvAssemPara.ulReceiverCpuId = VOS_LOCAL_CPUID;
    stFcMntnDrvAssemPara.ulReceiverPid   = UEPS_PID_FLOWCTRL;
    stFcMntnDrvAssemPara.ulLength        = (sizeof(FC_MNTN_DRV_ASSEM_PARA_STRU) - VOS_MSG_HEAD_LENGTH);
    stFcMntnDrvAssemPara.enMsgName       = ID_FC_MNTN_DRV_ASSEM_PARA;
    PSACORE_MEM_CPY(&stFcMntnDrvAssemPara.stDrvAssemPara, sizeof(FC_DRV_ASSEM_PARA_STRU), pstDrvAssenPara, sizeof(FC_DRV_ASSEM_PARA_STRU));

    FC_MNTN_TraceEvent(&stFcMntnDrvAssemPara);

    return VOS_OK;
}


STATIC VOS_VOID FC_DoJudgeDrvAssem(FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara)
{
    /* ???????????? */
    if (FC_ACPU_DRV_ASSEM_PC_ON_MASK == (FC_ACPU_DRV_ASSEM_PC_ON_MASK & g_stCpuDriverAssePara.ucEnableMask))
    {
        if (VOS_NULL_PTR != g_stDrvAssemParaEntity.pDrvSetAssemParaFuncPc)
        {
            /* ??????????????????????????????????????PC?????????? */
            if (VOS_OK != g_stDrvAssemParaEntity.pDrvSetAssemParaFuncPc(&(pstDrvAssemPara->stDrvAssemPara)))
            {
                FC_LOG(PS_PRINT_ERROR, "Set Driver Assemble parameter fail!\n");
                /* ???????????? */
                g_stDrvAssemParaEntity.ucSetDrvFailCnt++;

                return;
            }
        }
    }

    /* ???????????? */
    g_stDrvAssemParaEntity.ucSmoothCntUp    = 0;
    g_stDrvAssemParaEntity.ucSmoothCntDown  = 0;

    /* ???????????????? */
    g_stDrvAssemParaEntity.ucSetDrvFailCnt  = 0;

    /* ???????????????? */
    PSACORE_MEM_CPY(&g_stDrvAssemParaEntity.stCurAssemPara, (VOS_UINT32)sizeof(FC_CPU_DRV_ASSEM_PARA_STRU), pstDrvAssemPara, (VOS_UINT32)sizeof(FC_CPU_DRV_ASSEM_PARA_STRU));

    /* ???????? */
    FC_MNTN_TraceDrvAssemPara(&(pstDrvAssemPara->stDrvAssemPara));

}


STATIC VOS_VOID FC_JudgeDrvAssemAction(VOS_UINT32 ulAssemLev)
{

    FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara;

    pstDrvAssemPara = FC_GetCurrentAssemPara(ulAssemLev);

    /* ???????????????????????????????? */
    if ( VOS_NULL_PTR != pstDrvAssemPara)
    {
        FC_DoJudgeDrvAssem(pstDrvAssemPara);
    }
}


VOS_VOID FC_JudgeDrvToMaxPara(VOS_VOID)
{
    FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara;

    /* ?????????????????? */
    g_stDrvAssemParaEntity.ulCurLev = FC_ACPU_DRV_ASSEM_LEV_1;

    pstDrvAssemPara =  FC_GetCpuDrvAssemPara(FC_ACPU_DRV_ASSEM_LEV_1);

    FC_DoJudgeDrvAssem(pstDrvAssemPara);
}


STATIC FC_CPU_DRV_ASSEM_PARA_STRU* FC_SelectDrvAssemParaRule(VOS_UINT32 ulCpuLoad, VOS_UINT32 *pulAssemLev)
{
    FC_CPU_DRV_ASSEM_PARA_STRU      *pstCpuDrvAssemParaRst  = VOS_NULL_PTR;
    FC_CPU_DRV_ASSEM_PARA_STRU      *pstCpuDrvAssemPara     = VOS_NULL_PTR;
    VOS_INT                          i;

    /* ???????????????????? */
    for (i = 0; i < FC_ACPU_DRV_ASSEM_LEV_BUTT; i++)
    {
        pstCpuDrvAssemPara = &g_stCpuDriverAssePara.stCpuDrvAssemPara[i];
        if (ulCpuLoad >= pstCpuDrvAssemPara->ulCpuLoad)
        {
            pstCpuDrvAssemParaRst   = pstCpuDrvAssemPara;
           *pulAssemLev             = (VOS_UINT32)i;

            break;
        }
    }

    return pstCpuDrvAssemParaRst;
}



STATIC VOS_VOID FC_JudgeDrvAssemPara(VOS_UINT32 ulCpuLoad)
{
    FC_CPU_DRV_ASSEM_PARA_STRU *pstDrvAssemPara;
    /* ?????????? */
    VOS_UINT32                  ulAssemLev       = FC_ACPU_DRV_ASSEM_LEV_1;

    /* ?????????????????????????????????? */
    if (0 == g_stCpuDriverAssePara.ucEnableMask)
    {
        return;
    }

    if (0 != (g_stDrvAssemParaEntity.ucSetDrvFailCnt % CPU_MAX_SET_DRV_FAIL_SMOOTH_CNT))
    {
        g_stDrvAssemParaEntity.ucSetDrvFailCnt++;

        return;
    }

    /* ????CPU???????????????????? */
    pstDrvAssemPara = FC_SelectDrvAssemParaRule(ulCpuLoad, &ulAssemLev);

    if (VOS_NULL_PTR == pstDrvAssemPara)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_JudgeDrvAssemPara pstDrvAssemPara is NULL!\n");

        return;
    }

    /* ???????????????????????? */
    FC_JudgeAssemSmoothFactor(pstDrvAssemPara);

    /* ?????????????? */
    FC_JudgeDrvAssemAction(ulAssemLev);
}


STATIC VOS_UINT32  FC_PsRateJudge( VOS_VOID )
{
    /*
    STICK????????????????
    E5????????????????
    */
    return (FC_RmRateJudge());
}



STATIC VOS_VOID  FC_GetPsRate( VOS_UINT32 *pulUlRate, VOS_UINT32 *pulDlRate )
{
    /* E5???????????????????? */
   /* ?????????????????????????????????? */
   *pulUlRate   = FC_BRIDGE_GetRate();
   *pulDlRate   = *pulUlRate;
}



VOS_UINT32 FC_CPUA_UpJudge
(
    VOS_UINT32       ulCpuLoad,
    FC_CFG_CPU_STRU *pstFcCfgCpu,
    FC_POLICY_STRU  *pstFcPolicy
)
{
    VOS_UINT32                          ulResult;


    if ( ulCpuLoad < pstFcCfgCpu->ulCpuOverLoadVal )
    {
        g_stFcCpuACtrl.ulSmoothTimerLen = 0;
        return VOS_FALSE;
    }

    /* ????????CPU???????????????????????????????? */
    if (pstFcPolicy->enHighestPri <= pstFcPolicy->enDonePri)
    {
        return VOS_FALSE;
    }


    /* ??????????????????????  */
    /* ????????????????CPU?????????????????????????????????????????????? */
    if ( FC_PRI_NULL != pstFcPolicy->enDonePri )
    {
        return VOS_TRUE;
    }

    if ( 0 == g_stFcCpuACtrl.ulSmoothTimerLen )
    {
        FC_BRIDGE_ResetRate();
    }

    g_stFcCpuACtrl.ulSmoothTimerLen++;

    if (g_stFcCpuACtrl.ulSmoothTimerLen < pstFcCfgCpu->ulSmoothTimerLen)
    {
        return VOS_FALSE;
    }


    g_stFcCpuACtrl.ulSmoothTimerLen  = 0;


    /* ??????????????CPU???????? */
    ulResult    = FC_PsRateJudge();
    if (VOS_FALSE == ulResult)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



STATIC VOS_UINT32 FC_CPUA_DownJudge
(
    VOS_UINT32       ulCpuLoad,
    FC_CFG_CPU_STRU *pstFcCfgCpu,
    FC_POLICY_STRU  *pstFcPolicy
)
{
    if ( (ulCpuLoad < pstFcCfgCpu->ulCpuUnderLoadVal)
         && (FC_PRI_NULL < pstFcPolicy->enDonePri) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID FC_CPUA_RcvCpuLoad(VOS_UINT32  ulCpuLoad)
{
    FC_CFG_CPU_STRU                    *pstFcCfgCpu;
    FC_POLICY_STRU                     *pstFcPolicy;
    VOS_UINT32                          ulStartCtrl;
    VOS_UINT32                          ulUlRate;
    VOS_UINT32                          ulDlRate;


    /* ????CPU LOAD??????????????????????CPU???????????? */
    FC_JudgeDrvAssemPara(ulCpuLoad);


    /* ???????? */
    if ( FC_POLICY_MASK_CPU_A != (FC_POLICY_MASK_CPU_A & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* CPU?????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CPU_ProcessLoad, INFO, CPU FlowCtrl is disabled %d\n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return;
    }

    /* ???????????? */
    FC_MNTN_TraceCpuLoad(ID_FC_MNTN_CPU_A_CPULOAD, ulCpuLoad);

    if ( 100 < ulCpuLoad )
    {
        /* ???????? */
        FC_LOG1(PS_PRINT_WARNING, "FC_CPU_ProcessLoad, WARNING, Invalid Cpu Load %d\n",
                (VOS_INT32)ulCpuLoad);
        return;
    }


    pstFcCfgCpu = &(g_stFcAcoreCfg.stFcCfgCpuA);
    pstFcPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);

    /* ??????????CPU???? */
    ulStartCtrl = FC_CPUA_UpJudge(ulCpuLoad, pstFcCfgCpu, pstFcPolicy);
    if (VOS_TRUE == ulStartCtrl)
    {
        FC_GetPsRate(&ulUlRate, &ulDlRate);
        FC_SndCpuMsg(ID_FC_CPU_A_OVERLOAD_IND, ulCpuLoad, ulUlRate, ulDlRate);
        return;
    }


    /* ????CPU?????????? */
    ulStartCtrl = FC_CPUA_DownJudge(ulCpuLoad, pstFcCfgCpu, pstFcPolicy);
    if ( VOS_TRUE ==  ulStartCtrl )
    {
        FC_GetPsRate(&ulUlRate, &ulDlRate);
        FC_SndCpuMsg(ID_FC_CPU_A_UNDERLOAD_IND, ulCpuLoad, ulUlRate, ulDlRate);
        return;
    }

    return;
}



VOS_UINT32  FC_CPUA_StopFcAttempt( VOS_UINT32 ulParam1, VOS_UINT32 ulParam2 )
{
    FC_CFG_CPU_STRU                     *pstCfgCpu;


    pstCfgCpu   = &(g_stFcAcoreCfg.stFcCfgCpuA);
    if (0 == pstCfgCpu->ulStopAttemptTimerLen)
    {
        return VOS_OK;
    }


    /*????????????????????????????????????*/
    if (VOS_NULL_PTR == g_stFcCpuACtrl.pstStopAttemptTHandle)
    {
        if ( VOS_OK != VOS_StartRelTimer(&g_stFcCpuACtrl.pstStopAttemptTHandle,
                                            UEPS_PID_FLOWCTRL,
                                            pstCfgCpu->ulStopAttemptTimerLen,
                                            FC_TIMER_STOP_CPU_ATTEMPT,
                                            0,
                                            VOS_RELTIMER_NOLOOP,
                                            VOS_TIMER_PRECISION_0))
        {
            FC_LOG(PS_PRINT_WARNING, "FC_CPUA_StopFcAttempt, VOS_StartRelTimer Fail!\n");
            return VOS_ERR;
        }
    }

    /* for lint 715 */
    (VOS_VOID)ulParam1;
    (VOS_VOID)ulParam2;

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CPUA_UpProcess( VOS_VOID )
{
    FC_POLICY_STRU                     *pFcPolicy;


    /* ????CPU???????????????????????????????? */
    pFcPolicy   = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);
    FC_POLICY_Up(pFcPolicy);


    /* ???????????????????????????????????????? */
    if (pFcPolicy->enDonePri == pFcPolicy->enHighestPri)
    {
        if (VOS_NULL_PTR != pFcPolicy->pPostFunc)
        {
            (VOS_VOID)pFcPolicy->pPostFunc(FC_POLICY_ID_CPU_A, 0);
        }
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CPUA_DownProcess( VOS_VOID )
{
    FC_POLICY_STRU                      *pPolicy;


    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);
    FC_POLICY_Down(pPolicy);

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CPUA_StopFlowCtrl( VOS_VOID )
{
    FC_CFG_CPU_STRU                    *pstFcCfgCpu;
    FC_POLICY_STRU                     *pstFcPolicy;
    VOS_UINT32                          ulCpuLoad = 0;

    pstFcCfgCpu = &(g_stFcAcoreCfg.stFcCfgCpuA);
    pstFcPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CPU_A_MODEM_0);


    /* ????CPU????????????????,?????? */
    /*lint --e(685) FEATURE_ACPU_STAT??????ulCpuLoad??0???????????????????? */
    if ( (ulCpuLoad <= pstFcCfgCpu->ulCpuUnderLoadVal)
         && (FC_PRI_NULL < pstFcPolicy->enDonePri) )
    {
        FC_CPUA_DownProcess();
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CPUA_Init( VOS_VOID )
{
    /* ?????????????????????????????????? */

    PSACORE_MEM_SET(&g_stFcBridgeRate, (VOS_UINT32)sizeof(FC_BRIDGE_RATE_STRU), 0, (VOS_UINT32)sizeof(FC_BRIDGE_RATE_STRU));
    PSACORE_MEM_SET(&g_stFcCpuACtrl, (VOS_UINT32)sizeof(g_stFcCpuACtrl), 0, (VOS_UINT32)sizeof(g_stFcCpuACtrl));

    return VOS_OK;
}



FC_PRI_ENUM_UINT8  FC_MEM_CalcUpTargetFcPri
(
    FC_POLICY_STRU                     *pPolicy,
    VOS_UINT32                          ulMemValue
)
{
    FC_PRI_ENUM_UINT8                  enTargetPri;
    FC_CFG_MEM_STRU                    *pstMemCfg;


    pstMemCfg   = &(g_stFcAcoreCfg.stFcCfgMem);
    enTargetPri = pPolicy->enDonePri;

    /* ????????????????????????????????Lev3???? */
    if (1 == pPolicy->ucPriCnt)
    {
        /* ???????????????????? */
        if (ulMemValue <= pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_3].ulSetThreshold)
        {
            enTargetPri     = pPolicy->enHighestPri;
        }
    }
    /* ?????????????????????????? */
    else
    {
        /* ???????????????????? */
        if (ulMemValue <= pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_4].ulSetThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_4;
        }
        else if (ulMemValue <= pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_3].ulSetThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_3;
        }
        else if (ulMemValue <= pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_2].ulSetThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_2;
        }
        else if (ulMemValue <= pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_1].ulSetThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_1;
        }
        else
        {
            ;
        }
    }

    return enTargetPri;
} /* FC_MEM_CalcUpTargetFcPri */



FC_PRI_ENUM_UINT8  FC_MEM_CalcDownTargetFcPri
(
    FC_POLICY_STRU                     *pPolicy,
    VOS_UINT32                          ulMemValue
)
{
    FC_PRI_ENUM_UINT8                  enTargetPri;
    FC_CFG_MEM_STRU                    *pstMemCfg;

    pstMemCfg   = &(g_stFcAcoreCfg.stFcCfgMem);
    enTargetPri = pPolicy->enDonePri;

    /* ?????????????????????????? */
    if (1 == pPolicy->ucPriCnt)
    {
        /* ???????????????????? */
        if (ulMemValue > pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_3].ulStopThreshold)
        {
            enTargetPri     = FC_PRI_NULL;
        }
    }
    /* ?????????????????????????? */
    else
    {
        /* ???????????????????? */
        if (ulMemValue > pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_1].ulStopThreshold)
        {
            enTargetPri     = FC_PRI_NULL;
        }
        else if (ulMemValue > pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_2].ulStopThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_1;
        }
        else if (ulMemValue > pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_3].ulStopThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_2;
        }
        else if (ulMemValue > pstMemCfg->astThreshold[FC_MEM_THRESHOLD_LEV_4].ulStopThreshold)
        {
            enTargetPri     = FC_PRI_FOR_MEM_LEV_3;
        }
        else
        {
            ;
        }
    }

    return enTargetPri;
} /* FC_MEM_CalcUpTargetFcPri */



STATIC VOS_UINT32  FC_MEM_AdjustPriForUp
(
    FC_PRI_ENUM_UINT8                  enPointPri,
    FC_ID_ENUM_UINT8                   enFcId
)
{
    FC_POLICY_STRU                     *pPolicy;
    FC_PRI_ENUM_UINT8                   enTargetPri;

    /* ?????????????????????????????? */
    pPolicy         = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);

    /* ???????????????????????????????????????? */
    enTargetPri     = FC_MEM_CalcUpTargetFcPri(pPolicy, 0);

    /* ?????????????????????????????????????????????????????? */
    if (pPolicy->enDonePri < enTargetPri)
    {
        FC_POLICY_UpToTargetPri(pPolicy, enTargetPri);
    }
    else if (pPolicy->enDonePri > enTargetPri)
    {
        FC_POLICY_DownToTargetPri(pPolicy, enTargetPri);
    }
    else
    {
        ;
    }

    /* for lint 715 */
    (VOS_VOID)enPointPri;
    (VOS_VOID)enFcId;

    return VOS_OK;
}



STATIC VOS_UINT32  FC_MEM_AdjustPriForDown
(
    FC_PRI_ENUM_UINT8                   enPointPri,
    FC_ID_ENUM_UINT8                    enFcId
)
{
    FC_POLICY_STRU                     *pPolicy;
    FC_PRI_ENUM_UINT8                   enTargetPri;

    /* ?????????????????????????????? */
    pPolicy         = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);

    /* ???????????????????????????????????????? */
    enTargetPri     = FC_MEM_CalcDownTargetFcPri(pPolicy, 0);

    /* ?????????????????????????????????????????????????????? */
    if (pPolicy->enDonePri < enTargetPri)
    {
        FC_POLICY_UpToTargetPri(pPolicy, enTargetPri);
    }
    else if (pPolicy->enDonePri > enTargetPri)
    {
        FC_POLICY_DownToTargetPri(pPolicy, enTargetPri);
    }
    else
    {
        ;
    }

    /* for lint 715 */
    (VOS_VOID)enPointPri;
    (VOS_VOID)enFcId;

    return VOS_OK;
}



STATIC VOS_UINT32  FC_MEM_SndUpToTargetPriIndMsg(FC_PRI_ENUM_UINT8 enTargetPri, VOS_UINT16 usMemFreeCnt)
{
    FC_MEM_UP_TO_TARGET_PRI_IND_STRU   *pstMsg;
    VOS_UINT32                          ulResult;


    pstMsg = (FC_MEM_UP_TO_TARGET_PRI_IND_STRU *)(VOS_UINT_PTR)VOS_AllocMsg(UEPS_PID_FLOWCTRL, \
                                         (VOS_UINT32)sizeof(FC_MEM_UP_TO_TARGET_PRI_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemUpToTargetPriIndMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL;
    pstMsg->usMsgName       = ID_FC_MEM_UP_TO_TARGET_PRI_IND;
    pstMsg->enTargetPri     = enTargetPri;
    pstMsg->usMemFreeCnt    = usMemFreeCnt;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemUpToTargetPriIndMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;

}



STATIC VOS_UINT32  FC_MEM_SndDownToTargetPriIndMsg( FC_PRI_ENUM_UINT8 enTargetPri, VOS_UINT16 usMemFreeCnt)
{
    FC_MEM_DOWN_TO_TARGET_PRI_IND_STRU *pstMsg;
    VOS_UINT32                          ulResult;


    pstMsg = (FC_MEM_DOWN_TO_TARGET_PRI_IND_STRU *)(VOS_UINT_PTR)VOS_AllocMsg(UEPS_PID_FLOWCTRL, \
                                         (VOS_UINT32)sizeof(FC_MEM_DOWN_TO_TARGET_PRI_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemDownToTargetPriIndMsg, Alloc Msg Fail\n");
        return VOS_ERR;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL;
    pstMsg->usMsgName       = ID_FC_MEM_DOWN_TO_TARGET_PRI_IND;
    pstMsg->enTargetPri     = enTargetPri;
    pstMsg->usMemFreeCnt    = usMemFreeCnt;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_SndMemDownToTargetPriIndMsg, Send Msg Fail\n");
        return VOS_ERR;
    }

    return VOS_OK;

}



VOS_VOID FC_MEM_UpProcess( VOS_UINT32 ulMemValue  )
{
    FC_POLICY_STRU                     *pPolicy;
    FC_PRI_ENUM_UINT8                  enTargetPri;
    VOS_ULONG                           ulFlags = 0UL;

    /* ???????? */
    if ( FC_POLICY_MASK_MEM != (FC_POLICY_MASK_MEM & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_MEM_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return;
    }

    pPolicy         = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);
    enTargetPri     = FC_MEM_CalcUpTargetFcPri(pPolicy, ulMemValue);

    if ((pPolicy->enDonePri < enTargetPri) && (pPolicy->enToPri < enTargetPri))
    {
        /* ???????????????????????????????????????????????? */
        /*lint --e(506,774,550,730) ????dummy??????????lint???? */
        /*lint --e{550,1072} ????dummy????????lint???? */
        if (likely(preemptible()))
        {
            VOS_SpinLockIntLock(&g_stFcMemSpinLock, ulFlags);
            pPolicy->enToPri = enTargetPri;
            VOS_SpinUnlockIntUnlock(&g_stFcMemSpinLock , ulFlags);

            /* ????????????????????????????????enToPri?????? */
            FC_MEM_SndUpToTargetPriIndMsg(enTargetPri, (VOS_UINT16)ulMemValue);

        }
    }

    return;
}


VOS_VOID  FC_MEM_DownProcess( VOS_UINT32 ulMemValue )
{
    FC_POLICY_STRU                     *pPolicy;
    FC_PRI_ENUM_UINT8                  enTargetPri;
    VOS_ULONG                           ulFlags = 0UL;

    /* ???????? */
    if ( FC_POLICY_MASK_MEM != (FC_POLICY_MASK_MEM & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_MEM_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return;
    }

    pPolicy         = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_MEM_MODEM_0);
    enTargetPri     = FC_MEM_CalcDownTargetFcPri(pPolicy, ulMemValue);

    if ((pPolicy->enDonePri > enTargetPri) && (pPolicy->enToPri > enTargetPri))
    {
        /* ???????????????????????????????????????????????? */
        /*lint --e(506,774,550,730) ????dummy??????????lint???? */
        /*lint --e{550,1072} ????dummy????????lint???? */
        if (likely(preemptible()))
        {
            VOS_SpinLockIntLock(&g_stFcMemSpinLock, ulFlags);
            pPolicy->enToPri = enTargetPri;
            VOS_SpinUnlockIntUnlock(&g_stFcMemSpinLock, ulFlags);

            /* ????????????????????????????????enToPri?????? */
            FC_MEM_SndDownToTargetPriIndMsg(enTargetPri, (VOS_UINT16)ulMemValue);
        }
    }

    return;
}



STATIC VOS_UINT32  FC_MEM_Init( VOS_VOID )
{
    VOS_SpinLockInit(&g_stFcMemSpinLock);

    /* V9R1??????????ACPU?????????????????????????????????????? */
    /* ???????????????? */
    if ( (FC_POLICY_MASK(FC_POLICY_ID_MEM) == (FC_POLICY_MASK(FC_POLICY_ID_MEM) & g_stFcAcoreCfg.ulFcEnableMask) ))
    {
        /* A???????????????????? */
        g_astFcPolicy[FC_POLICY_ID_MEM].pAdjustForUpFunc    = FC_MEM_AdjustPriForUp;
        g_astFcPolicy[FC_POLICY_ID_MEM].pAdjustForDownFunc  = FC_MEM_AdjustPriForDown;
    }
    else
    {
         /* A???????????????????? */
        g_astFcPolicy[FC_POLICY_ID_MEM].pAdjustForUpFunc    = VOS_NULL_PTR;
        g_astFcPolicy[FC_POLICY_ID_MEM].pAdjustForDownFunc  = VOS_NULL_PTR;
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CST_UpProcess( VOS_UINT8 ucRabId )
{
    FC_POLICY_STRU                     *pPolicy;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_CST != (FC_POLICY_MASK_CST & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CST_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }


    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CST_MODEM_0);
    FC_POLICY_UpToTargetPri(pPolicy, pPolicy->enHighestPri);

    /* for lint 715 */
    (VOS_VOID)ucRabId;

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CST_DownProcess( VOS_UINT8 ucRabId )
{
    FC_POLICY_STRU                     *pPolicy;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_CST != (FC_POLICY_MASK_CST & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CST_DownProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }


    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CST_MODEM_0);
    FC_POLICY_DownToTargetPri(pPolicy, FC_PRI_NULL);

    /* for lint 715 */
    (VOS_VOID)ucRabId;

    return VOS_OK;
}



STATIC FC_RAB_MAPPING_INFO_STRU  *FC_CDS_GetFcInfo( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId )
{
    FC_RAB_MAPPING_INFO_SET_STRU       *pstFcRabMappingInfoSet;
    FC_RAB_MAPPING_INFO_STRU           *pstFcRabMappingInfo;
    FC_ID_ENUM_UINT8                    enFcId;
    VOS_UINT32                          ulRabMask;


    pstFcRabMappingInfoSet  = &g_astFcRabMappingInfoSet[enModemId];
    ulRabMask               = (VOS_UINT32)(1UL << ucRabId);

    /*====================================*//* ??????????????????????????????????RAB_ID?????????????????? */
    for (enFcId = 0; enFcId < pstFcRabMappingInfoSet->enFcIdCnt; enFcId++)
    {
        pstFcRabMappingInfo = &(pstFcRabMappingInfoSet->astFcRabMappingInfo[enFcId]);
        if ( 0 != (ulRabMask & pstFcRabMappingInfo->ulIncludeRabMask) )
        {
            return pstFcRabMappingInfo;
        }
    }

    return VOS_NULL_PTR;
}



VOS_UINT32  FC_CDS_DelFcId( FC_ID_ENUM_UINT8 enFcId, MODEM_ID_ENUM_UINT16 enModemId )
{
    FC_RAB_MAPPING_INFO_SET_STRU       *pstFcRabMappingInfoSet;
    FC_RAB_MAPPING_INFO_STRU           *pstFcRabMappingInfo;
    FC_ID_ENUM_UINT8                    enFcIdNum;
    FC_ID_ENUM_UINT8                    enShiftFcId;

    pstFcRabMappingInfoSet    = &g_astFcRabMappingInfoSet[enModemId];

    for (enFcIdNum = 0; enFcIdNum < pstFcRabMappingInfoSet->enFcIdCnt; enFcIdNum++)
    {
        if (enFcId == pstFcRabMappingInfoSet->astFcRabMappingInfo[enFcIdNum].enFcId)
        {
            break;
        }
    }

    if (enFcIdNum >= pstFcRabMappingInfoSet->enFcIdCnt)
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_CDS_DelFcId, can not find the Fc Id <1>!\n",
            (VOS_INT32)enFcId);
        return VOS_ERR;
    }

    /* ???????????????????????????????????????? */
    for (enShiftFcId = (FC_PRI_ENUM_UINT8)(enFcIdNum + 1); enShiftFcId < pstFcRabMappingInfoSet->enFcIdCnt; enShiftFcId++)
    {
        pstFcRabMappingInfo   = &(pstFcRabMappingInfoSet->astFcRabMappingInfo[enShiftFcId]);
        PSACORE_MEM_CPY((pstFcRabMappingInfo - 1), (VOS_UINT32)sizeof(FC_RAB_MAPPING_INFO_STRU), pstFcRabMappingInfo, (VOS_UINT32)sizeof(FC_RAB_MAPPING_INFO_STRU));
    }

    /* ???????????????????????????????????????????????? */
    PSACORE_MEM_SET(&(pstFcRabMappingInfoSet->astFcRabMappingInfo[enShiftFcId - 1]), (VOS_UINT32)sizeof(FC_RAB_MAPPING_INFO_STRU), 0, (VOS_UINT32)sizeof(FC_RAB_MAPPING_INFO_STRU));
    pstFcRabMappingInfoSet->enFcIdCnt--;

    return VOS_OK;
}



VOS_UINT32  FC_CDS_AddRab(FC_ID_ENUM_UINT8 enFcId, VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId )
{
    FC_RAB_MAPPING_INFO_SET_STRU       *pstFcRabMappingInfoSet;
    FC_RAB_MAPPING_INFO_STRU           *pstFcRabMappingInfo;
    FC_ID_ENUM_UINT8                    enFcIdNum;
    VOS_UINT32                          ulRabMask;

    pstFcRabMappingInfoSet  = &g_astFcRabMappingInfoSet[enModemId];
    ulRabMask               = (VOS_UINT32)(1UL << ucRabId);

    if ( FC_MAX_NUM < pstFcRabMappingInfoSet->enFcIdCnt )
    {
        FC_LOG1(PS_PRINT_ERROR, "FC_CDS_AddRab, g_astFcRabMappingInfoSet is exceed the ranger!\n",
            (VOS_INT32)pstFcRabMappingInfoSet->enFcIdCnt);
        return VOS_ERR;
    }

    /*====================================*//* ??????FC????????????????????????????RAB_ID??Fc Id???????? */
    for (enFcIdNum = 0; enFcIdNum < pstFcRabMappingInfoSet->enFcIdCnt; enFcIdNum++)
    {
        pstFcRabMappingInfo = &(pstFcRabMappingInfoSet->astFcRabMappingInfo[enFcIdNum]);
        if ( enFcId == pstFcRabMappingInfo->enFcId )
        {
            if ( 0 == (pstFcRabMappingInfo->ulIncludeRabMask & ulRabMask) )
            {
                pstFcRabMappingInfo->ulIncludeRabMask   |= ulRabMask;
                pstFcRabMappingInfo->ulNoFcRabMask      |= ulRabMask;
                (VOS_VOID)FC_POINT_ClrFc(FC_POLICY_MASK_CDS, enFcId);
            }

            return VOS_OK;
        }
    }

    if ( FC_MAX_NUM == pstFcRabMappingInfoSet->enFcIdCnt )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_CDS_AddRab, AtClientCnt reaches the max num!\n");
        return VOS_ERR;
    }

    /*====================================*//* ??????????FC Id??????????Fc Id??RAB ID */
    pstFcRabMappingInfo = &(pstFcRabMappingInfoSet->astFcRabMappingInfo[pstFcRabMappingInfoSet->enFcIdCnt]);
    pstFcRabMappingInfo->enFcId             = enFcId;
    pstFcRabMappingInfo->ulIncludeRabMask   = ulRabMask;
    pstFcRabMappingInfo->ulNoFcRabMask      = ulRabMask;

    pstFcRabMappingInfoSet->enFcIdCnt++;

    return VOS_OK;
}



VOS_UINT32  FC_CDS_DelRab( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId )
{
    FC_RAB_MAPPING_INFO_STRU           *pstFcRabMappingInfo;
    VOS_UINT32                          ulRabMask;


    /*====================================*//* ????Rab Id????????Rab Id??????FC???? */
    pstFcRabMappingInfo                 = FC_CDS_GetFcInfo(ucRabId, enModemId);
    if (VOS_NULL_PTR == pstFcRabMappingInfo)
    {
        return VOS_ERR;
    }

    /* ??????RabId??????????????Fc????????????????????????RabId???????????? */
    ulRabMask       = (VOS_UINT32)(1UL << ucRabId);

    pstFcRabMappingInfo->ulIncludeRabMask  &= (~ulRabMask);

    if (0 != pstFcRabMappingInfo->ulNoFcRabMask)
    {
        pstFcRabMappingInfo->ulNoFcRabMask       &= (~ulRabMask);
        if ((0 == pstFcRabMappingInfo->ulNoFcRabMask) && (0 != pstFcRabMappingInfo->ulIncludeRabMask))
        {
            (VOS_VOID)FC_POINT_SetFc(FC_POLICY_MASK_CDS, pstFcRabMappingInfo->enFcId);
        }
    }

    /* ??????????RabId??????FC????????????????????????FC ID?????????? */
    if (0 == pstFcRabMappingInfo->ulIncludeRabMask)
    {
        FC_CDS_DelFcId(pstFcRabMappingInfo->enFcId, enModemId);
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_CDS_Init( VOS_VOID )
{
    PSACORE_MEM_SET(g_astFcRabMappingInfoSet, (VOS_UINT32)sizeof(g_astFcRabMappingInfoSet), 0, (VOS_UINT32)sizeof(g_astFcRabMappingInfoSet));

    return VOS_OK;
}



VOS_UINT32  FC_CDS_UpProcess( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId )
{
    FC_RAB_MAPPING_INFO_STRU               *pstFcRabMappingInfo;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_CDS != (FC_POLICY_MASK_CDS & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CST_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }

    if ( MODEM_ID_BUTT <= enModemId )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_CDS_UpProcess ModemId Is Invalid %d\n", enModemId);
        return VOS_ERR;
    }

    /*====================================*//* ????RAB id?????? Client???????? */
    pstFcRabMappingInfo         = FC_CDS_GetFcInfo(ucRabId, enModemId);
    if (VOS_NULL_PTR == pstFcRabMappingInfo)
    {
        return VOS_ERR;
    }


    /*====================================*//* ??????Fc?????????????????????????????????????? */
    if (0 == pstFcRabMappingInfo->ulNoFcRabMask)
    {
        return VOS_OK;
    }


    /*====================================*/
    /* ??????FC????????????????RAB????????????0??????????RAB??????????????FC???????? */
    pstFcRabMappingInfo->ulNoFcRabMask   &= (~((VOS_UINT32)1 << ucRabId));
    if (0 == pstFcRabMappingInfo->ulNoFcRabMask)
    {
        (VOS_VOID)FC_POINT_SetFc(FC_POLICY_MASK_CDS, pstFcRabMappingInfo->enFcId);
    }

    return VOS_OK;
}



VOS_UINT32  FC_CDS_DownProcess( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16 enModemId )
{
    FC_RAB_MAPPING_INFO_STRU                  *pstFcRabMappingInfo;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_CDS != (FC_POLICY_MASK_CDS & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CDS_DownProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }

    /* add by wangrong */
    if ( MODEM_ID_BUTT <= enModemId )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_CDS_DownProcess ModemId Is Invalid %d\n", enModemId);
        return VOS_ERR;
    }

    /*====================================*//* ????RAB id?????? FC???? */
    pstFcRabMappingInfo                 = FC_CDS_GetFcInfo(ucRabId, enModemId);
    if (VOS_NULL_PTR == pstFcRabMappingInfo)
    {
        return VOS_ERR;
    }


    /*====================================*/
    /* ??????FC?????????????????????????????????????? */
    if (0 != pstFcRabMappingInfo->ulNoFcRabMask)
    {
        pstFcRabMappingInfo->ulNoFcRabMask   |= ((VOS_UINT32)1 << ucRabId);
        return VOS_OK;
    }


    /*====================================*/
    /* ????v????????????????RAB????????????0??????FC????s?????????????? */
    pstFcRabMappingInfo->ulNoFcRabMask   |= ((VOS_UINT32)1 << ucRabId);
    (VOS_VOID)FC_POINT_ClrFc(FC_POLICY_MASK_CDS, pstFcRabMappingInfo->enFcId);

    return VOS_OK;
}


STATIC VOS_VOID  FC_UpProcess(VOS_RATMODE_ENUM_UINT32 enRateMode)
{

    switch (enRateMode)
    {
        case VOS_RATMODE_GSM:

            FC_GPRS_UpProcess();

            break;
        case VOS_RATMODE_1X:
        case VOS_RATMODE_HRPD:

            FC_CDMA_UpProcess();

            break;
        default:

            FC_LOG1(PS_PRINT_WARNING, "FC_UpProcess RateMode Is Invalid %d\n", (VOS_INT32)enRateMode);

            break;
    }

    return;
}



STATIC VOS_UINT32  FC_GPRS_UpProcess( VOS_VOID )
{
    FC_POLICY_STRU                      *pPolicy;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_GPRS != (FC_POLICY_MASK_GPRS & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_GPRS_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }


    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_GPRS_MODEM_0);
    FC_POLICY_UpToTargetPri(pPolicy, pPolicy->enHighestPri);

    return VOS_OK;
}


STATIC VOS_VOID  FC_DownProcess(VOS_RATMODE_ENUM_UINT32 enRateMode)
{

    switch (enRateMode)
    {
        case VOS_RATMODE_GSM:

            FC_GPRS_DownProcess();

            break;
        case VOS_RATMODE_1X:
        case VOS_RATMODE_HRPD:

            FC_CDMA_DownProcess();

            break;
        default:

            FC_LOG1(PS_PRINT_WARNING, "FC_DownProcess RateMode Is Invalid %d\n", (VOS_INT32)enRateMode);

            break;
    }

    return;
}


STATIC VOS_UINT32  FC_GPRS_DownProcess( VOS_VOID )
{
    FC_POLICY_STRU                      *pPolicy;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_GPRS != (FC_POLICY_MASK_GPRS & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_GPRS_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }

    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_GPRS_MODEM_0);
    FC_POLICY_DownToTargetPri(pPolicy, FC_PRI_NULL);

    return VOS_OK;
}


STATIC VOS_UINT32  FC_CDMA_UpProcess( VOS_VOID )
{
    FC_POLICY_STRU                      *pPolicy;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_CDMA != (FC_POLICY_MASK_CDMA & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CDMA_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }


    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CDMA_MODEM_0);
    FC_POLICY_UpToTargetPri(pPolicy, pPolicy->enHighestPri);

    return VOS_OK;
}


STATIC VOS_UINT32  FC_CDMA_DownProcess( VOS_VOID )
{
    FC_POLICY_STRU                      *pPolicy;


    /*====================================*//* ???????? */
    if ( FC_POLICY_MASK_CDMA != (FC_POLICY_MASK_CDMA & g_stFcAcoreCfg.ulFcEnableMask) )
    {
        /* ?????????????? */
        FC_LOG1(PS_PRINT_INFO, "FC_CDMA_UpProcess, INFO, MEM FlowCtrl is disabled %d \n",
                (VOS_INT32)g_stFcAcoreCfg.ulFcEnableMask);
        return VOS_OK;
    }

    /* ????CPU???????????????????????????????? */
    pPolicy = FC_POLICY_Get(FC_PRIVATE_POLICY_ID_CDMA_MODEM_0);
    FC_POLICY_DownToTargetPri(pPolicy, FC_PRI_NULL);

    return VOS_OK;
}

VOS_VOID  FC_ChannelMapCreate(FC_ID_ENUM_UINT8 enFcId, VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16  enModemId)
{
    FC_ADD_RAB_FCID_MAP_IND_STRU       *pstMsg;
    VOS_UINT32                          ulResult;


    /* ??????????RabId??????[5,15] */
    if ( (FC_UE_MIN_RAB_ID > ucRabId) || (FC_UE_MAX_RAB_ID < ucRabId) )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapCreate RabId Is Invalid %d\n", ucRabId);
        return;
    }

    if ( MODEM_ID_BUTT <= enModemId )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapCreate ModemId Is Invalid %d\n", enModemId);
        return;
    }

    /* ???????? */
    pstMsg = (FC_ADD_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)VOS_AllocMsg(UEPS_PID_FLOWCTRL_A, \
                                   (VOS_UINT32)sizeof(FC_ADD_RAB_FCID_MAP_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR,"FC_ChannelMapCreate, Alloc Msg Fail\n");
        return;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL_A;
    pstMsg->usMsgName       = ID_FC_ADD_RAB_FCID_MAP_IND;
    pstMsg->enFcId          = enFcId;
    pstMsg->ucRabId         = ucRabId;
    pstMsg->enModemId       = enModemId;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL_A, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapCreate, Send Msg Fail\n");
        return;
    }
}



VOS_VOID  FC_ChannelMapDelete( VOS_UINT8 ucRabId, MODEM_ID_ENUM_UINT16  enModemId )
{
    FC_DEL_RAB_FCID_MAP_IND_STRU       *pstMsg;
    VOS_UINT32                          ulResult;


    /* ??????????RabId??????[5,15] */
    if ( (FC_UE_MIN_RAB_ID > ucRabId) || (FC_UE_MAX_RAB_ID < ucRabId) )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapDelete RabId Is Invalid %d\n", ucRabId);
        return;
    }

    if ( MODEM_ID_BUTT <= enModemId )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_ChannelMapDelete ModemId Is Invalid %d\n", enModemId);
        return;
    }

    /* ???????? */
    pstMsg = (FC_DEL_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)VOS_AllocMsg(UEPS_PID_FLOWCTRL_A, \
                                    (VOS_UINT32)sizeof(FC_DEL_RAB_FCID_MAP_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pstMsg)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapDelete, Alloc Msg Fail\n");
        return;
    }

    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = UEPS_PID_FLOWCTRL_A;
    pstMsg->usMsgName       = ID_FC_DEL_RAB_FCID_MAP_IND;
    pstMsg->ucRabId         = ucRabId;
    pstMsg->enModemId       = enModemId;

    ulResult = VOS_SendMsg(UEPS_PID_FLOWCTRL_A, pstMsg);

    if (VOS_OK != ulResult)
    {
        FC_LOG(PS_PRINT_ERROR, "FC_ChannelMapDelete, Send Msg Fail\n");
        return;
    }
}



STATIC VOS_UINT32  FC_RcvCstMsg( MsgBlock * pMsg )
{
    /*lint --e(826) ????????????UINT8????????UINT32???????????????? */
    switch ( FC_GET_MSG_NAME32(pMsg) )
    {
        case ID_CST_FC_SET_FLOWCTRL_REQ:
            FC_CST_UpProcess( ((CST_FC_SET_FLOWCTRL_REQ_STRU *)(VOS_UINT_PTR)pMsg)->ucRabId );
            break;

        case ID_CST_FC_STOP_FLOWCTRL_REQ:
            FC_CST_DownProcess( ((CST_FC_STOP_FLOWCTRL_REQ_STRU *)(VOS_UINT_PTR)pMsg)->ucRabId );
            break;

        default:
            break;
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_RcvCdsMsg( MsgBlock * pMsg )
{
    /*lint --e(826) ????????????UINT8????????UINT32???????????????? */
    switch ( FC_GET_MSG_NAME32(pMsg) )
    {
        case ID_CDS_FC_STOP_CHANNEL_IND:
            FC_CDS_UpProcess( ((CDS_FC_STOP_CHANNEL_IND_STRU *)(VOS_UINT_PTR)pMsg)->ucRabId,
                               ((CDS_FC_STOP_CHANNEL_IND_STRU *)(VOS_UINT_PTR)pMsg)->enModemId );
            break;

        case ID_CDS_FC_START_CHANNEL_IND:
            FC_CDS_DownProcess( ((CDS_FC_START_CHANNEL_IND_STRU *)(VOS_UINT_PTR)pMsg)->ucRabId,
                                 ((CDS_FC_START_CHANNEL_IND_STRU *)(VOS_UINT_PTR)pMsg)->enModemId );
            break;

        default:
            break;
    }

    return VOS_OK;
}



STATIC VOS_UINT32  FC_ACORE_RegPoint( VOS_VOID )
{

    return VOS_OK;
}



STATIC VOS_UINT32  FC_ACORE_RcvTimerMsg(REL_TIMER_MSG *pTimerMsg)
{
    switch (pTimerMsg->ulName)
    {
        case FC_TIMER_STOP_CPU_ATTEMPT:
            FC_CPUA_StopFlowCtrl();
            break;

        default:
            break;
    }

    return VOS_OK;

}



VOS_UINT32  FC_ACORE_RcvIntraMsg( MsgBlock * pMsg )
{
    /*lint --e(826) ????????????UINT8????????UINT16???????????????? */
    switch( FC_GET_MSG_NAME16(pMsg) )
    {
        case ID_FC_REG_POINT_IND:
            FC_POINT_Reg( &(((FC_REG_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->stFcPoint) );
            break;

        case ID_FC_DEREG_POINT_IND:
            FC_POINT_DeReg( ((FC_DEREG_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->enFcId,
                            ((FC_DEREG_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->enModemId );
            break;

        case ID_FC_CHANGE_POINT_IND:
            FC_POINT_Change( ((FC_CHANGE_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->enFcId,
                             ((FC_CHANGE_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->enPolicyId,
                             ((FC_CHANGE_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->enPri,
                             ((FC_CHANGE_POINT_IND_STRU *)(VOS_UINT_PTR)pMsg)->enModemId);
            break;

        case ID_FC_CPU_A_OVERLOAD_IND:
            FC_CPUA_UpProcess();
            break;

        case ID_FC_CPU_A_UNDERLOAD_IND:
            FC_CPUA_DownProcess();
            break;

        case ID_FC_SET_FLOWCTRL_IND:
            FC_UpProcess(((FC_SET_FLOWCTRL_IND_STRU *)(VOS_UINT_PTR)pMsg)->enRateMode);
            break;

        case ID_FC_STOP_FLOWCTRL_IND:
            FC_DownProcess(((FC_SET_FLOWCTRL_IND_STRU *)(VOS_UINT_PTR)pMsg)->enRateMode);
            break;

        case ID_FC_ADD_RAB_FCID_MAP_IND:
            FC_CDS_AddRab(((FC_ADD_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)pMsg)->enFcId,
                ((FC_ADD_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)pMsg)->ucRabId,
                ((FC_ADD_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)pMsg)->enModemId);
            break;

        case ID_FC_DEL_RAB_FCID_MAP_IND:
            FC_CDS_DelRab(( (FC_DEL_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)pMsg)->ucRabId,
                          ( (FC_DEL_RAB_FCID_MAP_IND_STRU *)(VOS_UINT_PTR)pMsg)->enModemId);
            break;

        case ID_FC_MEM_UP_TO_TARGET_PRI_IND:
            FC_POLICY_UpToTargetPri( &(g_astFcPolicy[FC_POLICY_ID_MEM]),
                ((FC_MEM_UP_TO_TARGET_PRI_IND_STRU *)(VOS_UINT_PTR)pMsg)->enTargetPri );
            break;

        case ID_FC_MEM_DOWN_TO_TARGET_PRI_IND:
            FC_POLICY_DownToTargetPri( &(g_astFcPolicy[FC_POLICY_ID_MEM]),
                ((FC_MEM_DOWN_TO_TARGET_PRI_IND_STRU *)(VOS_UINT_PTR)pMsg)->enTargetPri );
            break;

        case ID_FC_ACORE_CRESET_START_IND:
            FC_ACORE_CResetProc(FC_ACORE_CRESET_BEFORE_RESET);
            break;

        case ID_FC_ACORE_CRESET_END_IND:
            FC_ACORE_CResetProc(FC_ACORE_CRESET_AFTER_RESET);
            break;

        case ID_FC_ACORE_CRESET_START_RSP:
            FC_ACORE_CResetRcvStartRsp();
            break;

        default:
            break;
    }

    return VOS_OK;
}



VOS_VOID FC_ACORE_MsgProc(MsgBlock * pMsg)
{
    switch (pMsg->ulSenderPid)
    {
        case UEPS_PID_FLOWCTRL_A:
        case UEPS_PID_FLOWCTRL_C:
            FC_ACORE_RcvIntraMsg(pMsg);
            break;

        case UEPS_PID_CST:
            FC_RcvCstMsg(pMsg);
            break;

        case UEPS_PID_CDS:
            FC_RcvCdsMsg(pMsg);
            break;

        case VOS_PID_TIMER:
            FC_ACORE_RcvTimerMsg( (REL_TIMER_MSG *)(VOS_UINT_PTR)pMsg );
            break;

        default:
            break;
    }

} /* FC_MsgProc */


STATIC VOS_UINT32  FC_ACORE_CFG_SetDefaultValue( ACORE_FC_CFG_NV_STRU *pstAcoreFcCfg )
{
    FC_LOG(PS_PRINT_WARNING, "FC_CFG_SetDefaultValue, Set Default NV Value.\n");

    PSACORE_MEM_SET(pstAcoreFcCfg, sizeof(ACORE_FC_CFG_NV_STRU), 0x0, sizeof(ACORE_FC_CFG_NV_STRU));

    pstAcoreFcCfg->ulFcEnableMask = 0xFF;
    FC_SetFcEnableMask(pstAcoreFcCfg->ulFcEnableMask);

    pstAcoreFcCfg->stFcCfgCpuA.ulCpuOverLoadVal         = 95;
    pstAcoreFcCfg->stFcCfgCpuA.ulCpuUnderLoadVal        = 70;
    pstAcoreFcCfg->stFcCfgCpuA.ulSmoothTimerLen         = 8;
    pstAcoreFcCfg->stFcCfgCpuA.ulStopAttemptTimerLen    = 100;
    pstAcoreFcCfg->stFcCfgCpuA.ulUmUlRateThreshold      = 1048576;
    pstAcoreFcCfg->stFcCfgCpuA.ulUmDlRateThreshold      = 10485760;
    pstAcoreFcCfg->stFcCfgCpuA.ulRmRateThreshold        = 10485760;

    pstAcoreFcCfg->stFcCfgMem.ulThresholdCnt                    = 4;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[0].ulSetThreshold    = 300;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[0].ulStopThreshold   = 350;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[1].ulSetThreshold    = 250;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[1].ulStopThreshold   = 300;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[2].ulSetThreshold    = 150;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[2].ulStopThreshold   = 200;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[3].ulSetThreshold    = 0;
    pstAcoreFcCfg->stFcCfgMem.astThreshold[3].ulStopThreshold   = 20;

    pstAcoreFcCfg->stFcCfgCst.stThreshold.ulSetThreshold        = 3072;
    pstAcoreFcCfg->stFcCfgCst.stThreshold.ulStopThreshold       = 1024;

    return VOS_OK;
}


STATIC VOS_UINT32  FC_ACORE_CFG_CheckMemParam( FC_CFG_MEM_STRU *pstFcCfgMem )
{
    VOS_UINT32                          ulThresholdLoop;

    if ( FC_MEM_THRESHOLD_LEV_BUTT < pstFcCfgMem->ulThresholdCnt )
    {
        FC_LOG1(PS_PRINT_WARNING,
                "FC, FC_ACORE_CFG_CheckMemParam, WARNING, ulThresholdCnt %d\n",
                (VOS_INT32)pstFcCfgMem->ulThresholdCnt);

        return VOS_ERR;
    }

    for ( ulThresholdLoop = 0 ; ulThresholdLoop < pstFcCfgMem->ulThresholdCnt ; ulThresholdLoop++ )
    {
        /* ?????????????????????? */
        if ( pstFcCfgMem->astThreshold[ulThresholdLoop].ulSetThreshold
             > pstFcCfgMem->astThreshold[ulThresholdLoop].ulStopThreshold )
        {
            FC_LOG3(PS_PRINT_WARNING,
                    "FC, FC_ACORE_CFG_CheckMemParam, WARNING, ulThresholdLoop %d ulSetThreshold %D less than ulStopThreshold %d\n",
                    (VOS_INT32)ulThresholdLoop,
                    (VOS_INT32)pstFcCfgMem->astThreshold[ulThresholdLoop].ulSetThreshold,
                    (VOS_INT32)pstFcCfgMem->astThreshold[ulThresholdLoop].ulStopThreshold);
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


VOS_UINT32  FC_ACORE_CFG_CheckParam( ACORE_FC_CFG_NV_STRU *pstAcoreFcCfg )
{
    /* ??????????A??????????????????????????CPU????????????????CPU??????
       ??????????????????????????????????????2?? */
    if (2 > pstAcoreFcCfg->stFcCfgCpuA.ulSmoothTimerLen)
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_CFG_CheckParam, WARNING, CPUA flow ctrl ulSmoothTimerLen is %d!\n",
                                  (VOS_INT32)pstAcoreFcCfg->stFcCfgCpuA.ulSmoothTimerLen);
        return VOS_ERR;
    }

    if ( VOS_OK != FC_CFG_CheckCpuParam(&(pstAcoreFcCfg->stFcCfgCpuA)) )
    {
        FC_LOG(PS_PRINT_WARNING, "FC_CFG_CheckParam, WARNING, Check CPUA flow ctrl param failed!\n");
        return VOS_ERR;
    }

    if ( VOS_OK != FC_ACORE_CFG_CheckMemParam(&(pstAcoreFcCfg->stFcCfgMem)) )
    {
        FC_LOG(PS_PRINT_WARNING, "FC_CFG_CheckParam, WARNING, Check mem flow ctrl param failed!\n");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID FC_ACORE_CFG_Init(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulFcEnableMask;

    PSACORE_MEM_SET(&g_stFcAcoreCfg, sizeof(ACORE_FC_CFG_NV_STRU), 0, sizeof(ACORE_FC_CFG_NV_STRU));

    ulResult = GUCTTF_NV_Read(MODEM_ID_0, en_NV_Item_Acore_Flow_Ctrl_Config,\
                   &g_stFcAcoreCfg, sizeof(ACORE_FC_CFG_NV_STRU));

    /* ??A??FC NV??????????????????FC???????????? */
    ulFcEnableMask  = g_stFcAcoreCfg.ulFcEnableMask;
    g_stFcAcoreCfg.ulFcEnableMask   = 0;
    g_stFcAcoreCfg.ulFcEnableMask   |= ((1 == FC_ACORE_GetEnableMask(ulFcEnableMask, FC_ACORE_MEM_ENABLE_MASK))?(FC_POLICY_MASK(FC_POLICY_ID_MEM)):(0));
    g_stFcAcoreCfg.ulFcEnableMask   |= ((1 == FC_ACORE_GetEnableMask(ulFcEnableMask, FC_ACORE_CPUA_ENABLE_MASK))?(FC_POLICY_MASK(FC_POLICY_ID_CPU_A)):(0));
    g_stFcAcoreCfg.ulFcEnableMask   |= ((1 == FC_ACORE_GetEnableMask(ulFcEnableMask, FC_ACORE_CDS_ENABLE_MASK))?(FC_POLICY_MASK(FC_POLICY_ID_CDS)):(0));
    g_stFcAcoreCfg.ulFcEnableMask   |= ((1 == FC_ACORE_GetEnableMask(ulFcEnableMask, FC_ACORE_CST_ENABLE_MASK))?(FC_POLICY_MASK(FC_POLICY_ID_CST)):(0));

    /* C???????????????????????????? */
    g_stFcAcoreCfg.ulFcEnableMask |= FC_POLICY_MASK_GPRS;
    g_stFcAcoreCfg.ulFcEnableMask |= FC_POLICY_MASK_TMP;
    g_stFcAcoreCfg.ulFcEnableMask |= FC_POLICY_MASK_CPU_C;
    g_stFcAcoreCfg.ulFcEnableMask |= FC_POLICY_MASK_CDMA;
    FC_SetFcEnableMask(g_stFcAcoreCfg.ulFcEnableMask);

    if ( NV_OK != ulResult )
    {
        FC_LOG1(PS_PRINT_WARNING, "FC_ACORE_CFG_Init, WARNING, Fail to read NV, result %d\n",
                (VOS_INT32)ulResult);

        (VOS_VOID)FC_ACORE_CFG_SetDefaultValue(&g_stFcAcoreCfg);

        return;
    }

    ulResult = FC_ACORE_CFG_CheckParam(&g_stFcAcoreCfg);

    if ( VOS_OK != ulResult )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_CommInit, ERROR, Check NV parameter fail!\n");

        /* ??????NV?????????????????????????? */
        (VOS_VOID)FC_ACORE_CFG_SetDefaultValue(&g_stFcAcoreCfg);
    }

    return;
}


VOS_UINT32  FC_ACORE_Init( VOS_VOID )
{
    VOS_UINT32                          ulResult;
    VOS_INT                             iRet;
    VOS_CHAR                            ucSmName[] = "FcACoreCResetDoneSem";

    ulResult = FC_CommInit();

    if ( VOS_OK != ulResult )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_Init, ERROR, FC_CommInit return error!\n");
        return VOS_ERR;
    }

    /* ???????????????? */
    FC_ACORE_CFG_Init();

    ulResult    = FC_CPUA_Init();
    if ( VOS_OK != ulResult )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_ACORE_Init, ERROR, FC_CPUA_Init return error!\n");
        return VOS_ERR;
    }

    FC_CDS_Init();

    FC_MEM_Init();

    FC_ACORE_RegPoint();

    FC_DrvAssemInit();

    /* ????????????????C??????????????????????FcACore???????????????? */
    if ( VOS_OK != VOS_SmBCreate(ucSmName, 0, VOS_SEMA4_FIFO, (VOS_SEM *)&g_ulFcACoreCResetDoneSem) )
    {
        FC_LOG(PS_PRINT_ERROR,"FC_ACORE_Init, Create Sem Fail\n");
        return VOS_ERR;
    }

    /* ??????????????????C???????????? */
    iRet    = mdrv_sysboot_register_reset_notify ("TTF_FcACore", FC_ACORE_CResetCallback, 0, FC_ACORE_CRESET_CALLBACK_PRIOR);

    if ( VOS_OK != iRet )
    {
        FC_LOG(PS_PRINT_ERROR, "FC_ACORE_Init, ERROR, DRV_CCORERESET_REGCBFUNC fail!\n");
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 FC_ACORE_FidInit(enum VOS_INIT_PHASE_DEFINE enPhase)
{
    VOS_UINT32  ulResult = VOS_ERR;


    switch (enPhase)
    {
        case   VOS_IP_LOAD_CONFIG:
            ulResult = VOS_RegisterPIDInfo(UEPS_PID_FLOWCTRL,
                                           (Init_Fun_Type)VOS_NULL_PTR,
                                           (Msg_Fun_Type)FC_ACORE_MsgProc);
            if (VOS_OK != ulResult)
            {
                FC_LOG(PS_PRINT_ERROR, "FC_FidInit, VOS_RegisterPIDInfo Fail\n");
                return VOS_ERR;
            }
            ulResult = VOS_RegisterTaskPrio(UEPS_FID_FLOWCTRL, TTF_FLOW_CTRL_TASK_PRIO);
            if (VOS_OK != ulResult)
            {
                FC_LOG(PS_PRINT_ERROR, "FC_FidInit, OS_RegisterTaskPrio Fail\n");
                return VOS_ERR;
            }

            ulResult = FC_ACORE_Init();

            if (VOS_OK != ulResult)
            {
                FC_LOG(PS_PRINT_ERROR, "FC_FidInit, Call FC_Init return error!\n");
                return VOS_ERR;
            }

            break;
        case   VOS_IP_FARMALLOC:
        case   VOS_IP_INITIAL:
        case   VOS_IP_ENROLLMENT:
        case   VOS_IP_LOAD_DATA:
        case   VOS_IP_FETCH_DATA:
        case   VOS_IP_STARTUP:
        case   VOS_IP_RIVAL:
        case   VOS_IP_KICKOFF:
        case   VOS_IP_STANDBY:
        case   VOS_IP_BROADCAST_STATE:
        case   VOS_IP_RESTART:
            break;
        default:
            break;
    }

    return VOS_OK;
}










