/*
 *
 * All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses. You may choose this file to be licensed under the terms
 * of the GNU General Public License (GPL) Version 2 or the 2-clause
 * BSD license listed below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */



/******************************************************************************
   1 ??????????
******************************************************************************/
#include "PPP/Inc/ppp_public.h"
#include "PPP/Inc/ppp_init.h"
#include "PPP/Inc/hdlc_interface.h"

/*****************************************************************************
    ??????????????????????.C??????????
*****************************************************************************/
/*lint -e767  ??????????????ID???? */
#define    THIS_FILE_ID                 PS_FILE_ID_PPP_PUBLIC_C
/*lint +e767   */


/******************************************************************************
   2 ????????????????
******************************************************************************/



/******************************************************************************
   3 ????????
******************************************************************************/


/******************************************************************************
   4 ????????????
******************************************************************************/
extern PPP_DATA_Q_CTRL_ST               g_PppDataQCtrl;

/******************************************************************************
   5 ????????
******************************************************************************/

PPP_ZC_STRU * PPP_MemAlloc(VOS_UINT16 usLen, VOS_UINT16 usReserveLen)
{
    /* ????????????????????????MAC????????
      ??????ADS????????IP????????NDIS??E5??????????????????????????MAC????
      ????????????C??????????????????????????????????????????????
    */
    /*
       ????????????????????0????????USB??????????????????????PPP??????MAC??
    */
    PPP_ZC_STRU *pstMem = PPP_ZC_MEM_ALLOC(usLen + usReserveLen);


    if (VOS_NULL_PTR != pstMem)
    {
        if ( usReserveLen > 0)
        {
            /* ????????????????PPP??????????????????usLen???????????????????????????? */
            PPP_ZC_RESERVE(pstMem, usReserveLen);

            /* ?????????????????? */
            g_PppDataQCtrl.stStat.ulMemAllocUplinkCnt++;

            /* ?????????????????????????????? */
            PPP_ZC_SET_DATA_APP(pstMem, (VOS_UINT16)(1 << 8) | (VOS_UINT16)PPP_PULL_PACKET_TYPE);
        }
        else
        {
            /* ?????????????????? */
            g_PppDataQCtrl.stStat.ulMemAllocDownlinkCnt++;

            /* ?????????????????????????????? */
            PPP_ZC_SET_DATA_APP(pstMem, (VOS_UINT16)(1 << 8) | (VOS_UINT16)PPP_PUSH_PACKET_TYPE);
        }
    }
    else
    {
        if ( usReserveLen > 0)
        {
            /* ???????????????????? */
            g_PppDataQCtrl.stStat.ulMemAllocUplinkFailCnt++;
        }
        else
        {
            /* ???????????????????? */
            g_PppDataQCtrl.stStat.ulMemAllocDownlinkFailCnt++;
        }
    }

    return pstMem;
}


VOS_VOID PPP_MemWriteData(PPP_ZC_STRU *pstMem, VOS_UINT16 usDstLen, VOS_UINT8 *pucSrc, VOS_UINT16 usLen)
{
    /* ???????????????????????????????????? */
    PPP_ZC_SET_DATA_LEN(pstMem, usLen);

    /* ?????????????????? */
    PPP_MemSingleCopy(PPP_ZC_GET_DATA_PTR(pstMem), usDstLen, pucSrc, usLen);

    return;
}


PPP_ZC_STRU * PPP_MemCopyAlloc(VOS_UINT8 *pSrc, VOS_UINT16 usLen, VOS_UINT16 usReserveLen)
{
    PPP_ZC_STRU                        *pstMem = VOS_NULL_PTR;


    pstMem = PPP_MemAlloc(usLen, usReserveLen);

    if ( VOS_NULL_PTR != pstMem )
    {
        /* ?????????????????? */
        PPP_MemWriteData(pstMem, usLen, pSrc, usLen);
    }

    return pstMem;
}


VOS_UINT32 PPP_MemCutTailData
(
    PPP_ZC_STRU **ppMemSrc,
    VOS_UINT8 *pucDest,
    VOS_UINT16 usLen,
    VOS_UINT16 usReserveLen
)
{
    PPP_ZC_STRU                        *pCurrMem;
    VOS_UINT16                          usCurrLen;
    VOS_UINT16                          usCurrOffset;


    /* ???????? */
    if ( (VOS_NULL_PTR == ppMemSrc) ||
         (VOS_NULL_PTR == *ppMemSrc) ||
         (VOS_NULL_PTR == pucDest))
    {
        PPP_MNTN_LOG2(PS_PID_APP_PPP, 0, PS_PRINT_WARNING,
                      "PPP_MemCutTailData input parameters error, \
                      src addr'addr: 0X%p, dest addr: 0X%p\r\n",
                      (VOS_UINT_PTR)ppMemSrc, (VOS_UINT_PTR)pucDest);

        return PS_FAIL;
    }

    pCurrMem    = (PPP_ZC_STRU *)(*ppMemSrc);
    usCurrLen   = PPP_ZC_GET_DATA_LEN(pCurrMem);

    if ( ( 0 == usLen) || (usCurrLen < usLen) )
    {
        PPP_MNTN_LOG2(PS_PID_APP_PPP, 0, PS_PRINT_WARNING,
                      "PPP_MemCutTailData, Warning, usCurrLen %d Less Than usLen %d!\r\n",
                      usCurrLen, usLen);

        return PS_FAIL;
    }

    /* ?????????????????????????????????? */
    usCurrOffset = usCurrLen - usLen;

    PSACORE_MEM_CPY(pucDest, usLen, &(PPP_ZC_GET_DATA_PTR(pCurrMem)[usCurrOffset]), (VOS_ULONG)usLen);

    if ( usCurrOffset > 0 )
    {
        /* ??????????????????????????????????Tail???????????????????????? */
        (*ppMemSrc) = PPP_MemCopyAlloc(PPP_ZC_GET_DATA_PTR(pCurrMem), usCurrOffset, usReserveLen);
    }
    else
    {
        (*ppMemSrc) = VOS_NULL_PTR;
    }

    /* ???????? */
    PPP_MemFree(pCurrMem);

    return PS_SUCC;
}


VOS_UINT32 PPP_MemCutHeadData
(
    PPP_ZC_STRU **ppMemSrc,
    VOS_UINT8 *pucDest,
    VOS_UINT16 usDataLen
)
{
    PPP_ZC_STRU                        *pCurrMem;
    VOS_UINT16                          usMemSrcLen;


    if ( (VOS_NULL_PTR == ppMemSrc) ||
         (VOS_NULL_PTR == *ppMemSrc) ||
         (VOS_NULL_PTR == pucDest) )
    {
        PPP_MNTN_LOG(PS_PID_APP_PPP, 0, LOG_LEVEL_WARNING, "Para Err");

        return PS_FAIL;
    }

    /* ????TTF???????????????????????? */
    pCurrMem        = (PPP_ZC_STRU *)(*ppMemSrc);
    usMemSrcLen     = PPP_ZC_GET_DATA_LEN(pCurrMem);

    if ( ( 0 == usDataLen) || (usMemSrcLen < usDataLen) )
    {
        PPP_MNTN_LOG2(PS_PID_APP_PPP, 0, LOG_LEVEL_WARNING,
                      "PPP_MemCutHeadData, Warning: usMemSrcLen: %d Less Than usDataLen: %d!\r\n",
                      usMemSrcLen, usDataLen);

        return PS_FAIL;
    }

    /* ?????????????????????????????????? */
    PSACORE_MEM_CPY(pucDest, usDataLen, PPP_ZC_GET_DATA_PTR(pCurrMem), (VOS_ULONG)usDataLen);

    if ( usMemSrcLen >  usDataLen)
    {
        /* ???????????????????????????????? */
        PPP_ZC_REMOVE_HDR(pCurrMem, usDataLen);
    }
    else
    {
        /* ???????????? */
        PPP_MemFree(pCurrMem);
        (*ppMemSrc) = VOS_NULL_PTR;
    }

    return PS_SUCC;
}


VOS_UINT32 PPP_MemGet(PPP_ZC_STRU *pMemSrc, VOS_UINT16 usOffset, VOS_UINT8 *pDest, VOS_UINT16 usLen)
{
    VOS_UINT16                          usMemSrcLen;


    /* ???????? */
    if ( (VOS_NULL_PTR == pMemSrc)||(VOS_NULL_PTR == pDest) )
    {
        PPP_MNTN_LOG(PS_PID_APP_PPP, 0, PS_PRINT_WARNING,
                     "PPP_MemGet, Warning, Input Par pMemSrc Or pDest is Null!\r\n");

        return PS_FAIL;
    }

    if ( 0 == usLen )
    {
        PPP_MNTN_LOG(PS_PID_APP_PPP, 0, PS_PRINT_WARNING,
                     "PPP_MemGet, Warning, Input Par usLen is 0!\r\n");

        return PS_FAIL;
    }

    /* ????TTF???????????????????????? */
    usMemSrcLen = PPP_ZC_GET_DATA_LEN(pMemSrc);

    if ( usMemSrcLen < (usOffset + usLen) )
    {
        PPP_MNTN_LOG2(PS_PID_APP_PPP, 0, PS_PRINT_WARNING,
                      "PPP_MemGet, Warning, MemSrcLen %d Less Than (Offset + Len) %d!\r\n",
                      usMemSrcLen, (usOffset + usLen));

        return PS_FAIL;
    }

    PSACORE_MEM_CPY(pDest, usLen, PPP_ZC_GET_DATA_PTR(pMemSrc) + usOffset, (VOS_ULONG)usLen);

    return PS_SUCC;
}


VOS_VOID PPP_MemFree(PPP_ZC_STRU *pstMem)
{
    /* ?????????????? */
    PPP_ZC_MEM_FREE(pstMem);

    g_PppDataQCtrl.stStat.ulMemFreeCnt++;

    return;
}


VOS_VOID PPP_MemSingleCopy(VOS_UINT8 *pucDest, VOS_UINT32 ulDstLen, VOS_UINT8 *pucSrc, VOS_UINT32 ulLen)
{
    /* ????????EDMA???? */
    PSACORE_MEM_CPY(pucDest, ulDstLen, pucSrc, (VOS_ULONG)ulLen);

    return;
}


VOS_UINT32 PPP_GenerateSeed
(
    VOS_UINT8                           aucSerial[],
    VOS_UINT8                           ucSerialLen,
    VOS_UINT32                          ulStartIndex,
    VOS_UINT32                          ulDisturbaceFactor
)
{

    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulSeed;
    VOS_UINT8                          *pucSeed;
    VOS_UINT8                           ucTempValue = 0xFF;


    /* ?????????????????????????????? */
    pucSeed = (VOS_UINT8 *)&ulSeed;
    for ( ulLoop = 0; ulLoop < sizeof(ulSeed); ulLoop++ )
    {
        *pucSeed++ = aucSerial[(VOS_UINT8)((ulStartIndex + ulLoop) % ucSerialLen)];
    }

    /* ???? */
    ulSeed += ulDisturbaceFactor;

    /* ?????????? */
    pucSeed = (VOS_UINT8 *)&ulSeed;
    for ( ulLoop = 0; ulLoop < sizeof(ulSeed); ulLoop++ )
    {
        if (0 == *pucSeed)
        {
            *pucSeed = ucTempValue--;
        }
        pucSeed++;
    }

    return ulSeed;
}


VOS_VOID PPP_Get16ByteSerial
(
    VOS_UINT8                           aucSerial[]
)
{
    VOS_UINT8                           aucSrcSerial[4][4];
    VOS_UINT32                          ulTick      = VOS_GetTick();
    VOS_UINT32                          ulSlice     = VOS_GetSlice();
    VOS_UINT32                          ulTaskId    = VOS_GetCurrentTaskID();
    VOS_UINT32                          ulStatSum;

    ulStatSum   = 0xECA8642F;

    /* ??????????????0???????????????? */
    ulTick = (0x00UL != ulTick) ? ulTick : 0x12345678;
    ulSlice = (0x00UL != ulSlice) ? ulSlice : 0x9ABCDEF1;
    ulTaskId = (0x00UL != ulTaskId) ? ulTaskId : 0xFDB97531;

    /* ???????????????????????? */
    PSACORE_MEM_CPY(&(aucSrcSerial[0][0]), sizeof(ulTick), &ulTick, sizeof(ulTick));
    PSACORE_MEM_CPY(&(aucSrcSerial[1][0]), sizeof(ulSlice), &ulSlice, sizeof(ulSlice));
    PSACORE_MEM_CPY(&(aucSrcSerial[2][0]), sizeof(ulTaskId), &ulTaskId, sizeof(ulTaskId));
    PSACORE_MEM_CPY(&(aucSrcSerial[3][0]), sizeof(ulStatSum), &ulStatSum, sizeof(ulStatSum));

    /* ????????????????????????????*/
    PPP_MATRIX_TRANSFORM(aucSerial, aucSrcSerial);

    return;
}


VOS_VOID PPP_GetSecurityRand
(
    VOS_UINT8                           ucRandByteLen,
    VOS_UINT8                          *pucRand
)
{
    VOS_UINT8                           aucDictionary[256] = {0};
    VOS_UINT8                           aucSerial[16] = {0};
    VOS_UINT8                           ucValue = 0;
    VOS_UINT8                           ucTempValue;
    VOS_UINT32                          ulSeed;
    VOS_UINT32                          ulRandIndex;
    VOS_UINT32                          ulSwapIndex;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulStart;
    VOS_UINT32                          ulRandSum;
    const VOS_UINT32                    ulMax = 0x100;/* ??????????????, ????????????????[0x00..0xFF] */


    /***************************************************************************
     ??????????????????????????????????????????????????????????????????????????
     ??????????????????????????????????????????????????????????????????????????
     ??????????????????????????????????????????????????????????????????????????
     ??????????????????????????????????????????????????????????????????????????
     ????????????????????TICK??????????????????????????????????/????ID????????
     ??????????????????????????????????????????????????????????????????????????
     ????????????????????
    ****************************************************************************/

    /***************************************************************************
     ??????????????????????:
     1??????????
     2??????????????
    ****************************************************************************/

    /* ????16byte???????? */
    PPP_Get16ByteSerial(aucSerial);

    /* ???????? */
    ulSeed = PPP_GenerateSeed(aucSerial, (VOS_UINT8)sizeof(aucSerial), 0, 0);
    VOS_SetSeed(ulSeed);

    /***************************************************************************
     1??????????
    ****************************************************************************/
    /* a??????????????????, ??????????:[0..0xFF] */
    ulStart = VOS_Rand(ulMax);
    ulRandSum = ulStart;

    /* b????????????????????: [ulStart??0xFF] */
    for (ulLoop = ulStart; ulLoop < ulMax; ulLoop++)
    {
        aucDictionary[ulLoop] = ucValue;
        ucValue++;
    }

    /* c????????????????????: [0, ulStart) */
    for (ulLoop = 0; ulLoop < ulStart; ulLoop++)
    {
        aucDictionary[ulLoop] = ucValue;
        ucValue++;
    }

    /***************************************************************************
     2??????????????
       ????ucRandByteLen??????????????????????????ulLoop = 0??????
       ????????????????(ulLoop??[ulLoop, 0xFF]????)????????????????
    ****************************************************************************/
    for (ulLoop = 0; ulLoop < ucRandByteLen; ulLoop++)
    {
        /* ???????? */
        ulSeed = PPP_GenerateSeed(aucSerial, (VOS_UINT8)sizeof(aucSerial), ulLoop, ulRandSum);
        VOS_SetSeed(ulSeed);

        /* ??????????, ??????????:[0..0xFF] */
        ulRandIndex                 = VOS_Rand(ulMax);
        ulRandSum                  += ulRandIndex;

        /* ????????????????????:[ulLoop..0xFF] */
        ulSwapIndex                 = (ulRandIndex % (ulMax - ulLoop)) + ulLoop;

        /* ????aucDictionary[ulLoop]??aucDictionary[ulSwapIndex] */
        ucTempValue                 = aucDictionary[ulLoop];
        aucDictionary[ulLoop]       = aucDictionary[ulSwapIndex];
        aucDictionary[ulSwapIndex]  = ucTempValue;
    }

    /* ????ucRandByteLen???????????? */
    PSACORE_MEM_CPY(pucRand, ucRandByteLen, &(aucDictionary[0]), ucRandByteLen);

    return;
}




