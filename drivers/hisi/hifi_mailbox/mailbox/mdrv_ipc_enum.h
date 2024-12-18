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

#ifndef __MDRV_IPC_ENUM_H__
#define __MDRV_IPC_ENUM_H__
#ifdef __cplusplus
extern "C"
{
#endif
/*lint -e488*/
/* ??????????*/
typedef enum tagIPC_INT_CORE_E
{
    IPC_CORE_ARM11 = 0x0,   /* P500????IPC */
    IPC_CORE_A9,
    IPC_CORE_CEVA,
    IPC_CORE_TENS0,
    IPC_CORE_TENS1,
    IPC_CORE_DSP,
    IPC_CORE_APPARM = 0x0,  /* V7R1/V3R2????IPC */
    IPC_CORE_COMARM,
    IPC_CORE_LTEDSP,
    IPC_CORE_VDSP,
    IPC_CORE_ZSP,
    IPC_CORE_DSP_GU,
    IPC_CORE_ACPU = 0x0,    /* V9R1 SFT????IPC */
    IPC_CORE_CCPU,
    IPC_CORE_MCU,
    IPC_CORE_HIFI,
    IPC_CORE_BBE16,
    IPC_CORE_ACORE = 0x0,   /*P531,V7R2*/
    IPC_CORE_CCORE,
    IPC_CORE_MCORE,
    IPC_CORE_LDSP,
    IPC_CORE_HiFi,
    IPC_CORE_BBE,           /*..5..P531\V7R2...........P531..*/
    IPC_CORE0_A15,
    IPC_CORE1_A15,
    /* !!!!????????????????????  */
    IPC_CORE_BUTTOM
}IPC_INT_CORE_E;

/*********************************************************
*  ??????IPC??????????????????:
*  IPC_<??????????>_INT_SRC_<????????>_<????/????>
*  ??????????:ACPU??CCPU??MCU??HIFI??BBE16
*  ????????  :ACPU??CCPU??MCU??HIFI??BBE16
*  ????/???? :
*********************************************************/
#if defined(CHIP_BB_HI6210)
typedef enum tagIPC_INT_LEV_E
{
   /* ????CCPU IPC??????????????bit???? */
    IPC_INT_DSP_MODEM                   = 0,
    IPC_CCPU_INT_SRC_HIFI_MSG           = 1,    /* bit1, HIFI???????????? */
    IPC_INT_DSP_MSP                     = 2,
    IPC_INT_DSP_PS 						= 3,
    IPC_CCPU_INT_SRC_MCU_MSG            = 5,    /* bit5, MCU???????????? */
    IPC_CCPU_INT_SRC_ACPU_MSG           = 6,    /* bit6, ACPU???????????? */
    IPC_CCPU_INT_SRC_ACPU_IFC           = 7,    /* bit7, ACPU???????????????? */
    IPC_INT_DSP_HALT                    = 8,    /* bit8, DSP????ARM????*/
    IPC_INT_DSP_RESUME                  = 9,    /* bit9, DSP????ARM????????????????????*/
    IPC_CCPU_INT_SRC_MCU_IFC            = 10,   /* bit10, MCU????????????????*/
    IPC_INT_WAKE_GU                     = 11,   /* bit11, ????????????????*/
    IPC_INT_SLEEP_GU                    = 12,   /* bit12, ????????????*/
    IPC_INT_DICC_USRDATA_ACPU           = 13,   /* bit13, ????,??????????IPC_INT_DICC_USRDATA????*/
    IPC_INT_DICC_RELDATA_ACPU           = 14,   /* bit14, ????,??????????IPC_INT_DICC_USRDATA????*/
    IPC_INT_ARM_SLEEP                   = 15,   /* bit15, ??8??????15*/
    /* ????TDS??????????IPC????,begin */
    IPC_INT_WAKE_GSM                    = 16,   /* bit16, BBE16????CCPU,????GSM???? */
    IPC_INT_WAKE_WCDMA                  = 17,   /* bit17, BBE16????CCPU,????WCDMA???? */
    IPC_INT_DSP_PS_PUB_MBX              = 18,   /* bit18, ????DSP->ARM,????????IPC???????? */
    IPC_INT_DSP_PS_MAC_MBX              = 19,   /* bit19, ????DSP->ARM,????????IPC???????? */
    IPC_INT_DSP_MBX_RSD                 = 20,   /* bit20, ????DSP->ARM,????????IPC???????? */
    IPC_CCPU_INT_SRC_DSP_MNTN           = 21,   /* bit21??BBE16????????????????mailbox???????? */
    IPC_CCPU_INT_SRC_DSP_RCM_PUB_MBX    = 22,   /* bit22*/
    /* ????TDS??????????IPC????,end */
    IPC_CCPU_INT_SRC_ACPU_ICC           = 30,   /* bit30, ACPU ICC????????????*/
    IPC_CCPU_INT_SDR_CCPU_BBP_MASTER_ERROR = 31, /* BBE16????C???????????????? */
    IPC_CCPU_INT_SRC_BUTT               = 32,

   /* ????MCU IPC??????????????bit???? */
    IPC_MCU_INT_SRC_ACPU_MSG            = 4,    /* bit4, ACPU???????????? */
    IPC_MCU_INT_SRC_CCPU_MSG            = 5,    /* bit5, CCPU???????????? */
    IPC_MCU_INT_SRC_HIFI_MSG            = 6,    /* bit6, HIFI???????????? */
    IPC_MCU_INT_SRC_CCPU_IFC            = 7,    /* bit7, HIFI???????????? */
    IPC_MCU_INT_SRC_CCPU_IPF            = 8,    /* bit8, IPF?????? */
    IPC_MCU_INT_SRC_ACPU_IFC            = 9,    /* bit9, ACPU???????????????? */
    IPC_MCU_INT_SRC_ACPU0_PD            = 10,   /* bit10, acpu0 power down */
    IPC_MCU_INT_SRC_ACPU1_PD            = 11,   /* bit11, acpu1 power down */
    IPC_MCU_INT_SRC_ACPU2_PD            = 12,   /* bit12, acpu2 power down */
    IPC_MCU_INT_SRC_ACPU3_PD            = 13,   /* bit13, acpu3 power down */
    IPC_MCU_INT_SRC_ACPU_HOTPLUG        = 14,   /* bit14, acpu hotplug--no use */
    IPC_MCU_INT_SRC_ACPU_DFS            = 15,   /* bit15, ACPU DFS*/
    IPC_MCU_INT_SRC_ACPU_PD             = 16,   /* bit16, acpu power down */
    IPC_MCU_INT_SRC_CCPU_PD             = 17,   /* bit17, ccpu power down */
    IPC_MCU_INT_SRC_HIFI_PD             = 18,   /* bit18, hifi power down */
    IPC_MCU_INT_SRC_MCU_AGT             = 19,   /* bit19, mcu agent */
    IPC_MCU_INT_SRC_HIFI_DDR_VOTE       = 20,   /* bit20, HIFI DDR?????????? */
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLOW    = 21,   /* bit21, ACPU ??????SLOW?????????? */
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLEEP   = 22,   /* bit22, ACPU ??????TCXO???????????????????? */
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_INVALID = 23,   /* bit23, ACPU ???????????????? */
    IPC_MCU_INT_SRC_HIFI_MEMSHARE_DDR_VOTE = 24,    /* bit24, MP3????????????????HARQMEM,HIFI DDR??????????*/
    IPC_MCU_INT_SRC_HIFI_MEMSHARE_DDR_EXIT_VOTE = 25, /* bit25, MP3????????????????HARQMEM,HIFI????DDR??????????*/
    IPC_MCU_INT_SRC_ACPU4_PD            = 26,   /* bit26, acpu4:cluster1 core0 power down */
    IPC_MCU_INT_SRC_ACPU5_PD            = 27,   /* bit27, acpu5:cluster1 core1 power down */
    IPC_MCU_INT_SRC_ACPU6_PD            = 28,   /* bit28, acpu6:cluster1 core2 power down */
    IPC_MCU_INT_SRC_ACPU7_PD            = 29,   /* bit29, acpu7:cluster1 core3 power down */
    IPC_MCU_INT_SRC_HIFI_IFC            = 31,   /* bit31, HIFI????????????????*/
    IPC_MCU_INT_SRC_BUTT                = 32,

    /* ????ACPU IPC??????????????bit???? */
    IPC_ACPU_INT_SRC_CCPU_MSG           = 1,    /* bit1, CCPU???????????? */
    IPC_ACPU_INT_SRC_HIFI_MSG           = 2,    /* bit2, HIFI???????????? */
    IPC_ACPU_INT_SRC_MCU_MSG            = 3,    /* bit3, ACPU???????????? */
    IPC_ACPU_INT_SRC_CCPU_NVIM          = 4,    /* bit4, ????NVIM????C????A????????????*/
    IPC_ACPU_INT_SRC_CCPU_IFC           = 5,    /* bit5, CCPU????????????????  */
    IPC_ACPU_INT_SRC_MCU_IFC            = 6,    /* bit6, MCU????????????????  */
    IPC_ACPU_INT_SRC_MCU_THERMAL_HIGH   = 7,    /* bit7, MCU????????????????IPC????????ACPU*/
    IPC_ACPU_INT_SRC_MCU_THERMAL_LOW    = 8,    /* bit8, MCU????????????????IPC????????ACPU*/
    IPC_INT_DSP_APP                     = 9,    /* bit9, LDSP???????? */
    IPC_ACPU_INT_SRC_HIFI_PC_VOICE_RX_DATA = 10,  /* hifi->acore pc voice */
    IPC_INT_DICC_USRDATA                = 13,   /* bit13, TTF????CCPU????????????????IPC_INT_DICC_USRDATA_ACPU????????*/
    IPC_INT_DICC_RELDATA                = 14,   /* bit14, TTF????CCPU????????????????IPC_INT_DICC_RELDATA_ACPU????????*/
    IPC_ACPU_INT_SRC_CCPU_LOG = 25,   /* bit25,   CCPU??????ACPU??LOG????????*/
    IPC_ACPU_INI_SRC_MCU_TELE_MNTN_NOTIFY    = 26,   /* bit26, TELE_MNTN????ACPU???? */
    IPC_ACPU_INI_SRC_MCU_EXC_REBOOT     = 27,   /* bit27, MCU???????????????? */
    IPC_ACPU_INT_SRC_CCPU_EXC_REBOOT    = 28,   /* bit28, CCPU???????????????? */
    IPC_ACPU_INT_SRC_CCPU_NORMAL_REBOOT = 29,   /* bit29, CCPU???????????????? */
    IPC_ACPU_INT_SRC_MCU_DDR_EXC        = 30,   /* bit30, MCU DDR???????????? */
    IPC_ACPU_INT_SRC_CCPU_ICC           = 31,   /* bit31, CCPU ICC???????????? */
    IPC_ACPU_INT_SRC_BUTT               = 32,


    /* ????HIFI IPC??????????????bit???? */
    IPC_HIFI_INT_SRC_ACPU_MSG           = 0,    /* bit0, ACPU???????????? */
    IPC_HIFI_INT_SRC_CCPU_MSG           = 1,    /* bit1, CCPU???????????? */
    IPC_HIFI_INT_SRC_BBE_MSG            = 4,    /* bit4, TDDSP???????????? */
    IPC_HIFI_INT_SRC_MCU_MSG            = 6,    /* bit6, MCU???????????? */
    IPC_HIFI_INT_SRC_MCU_WAKE_DDR       = 7,    /* bit7, MCU????HIFI????DDR */
    IPC_HIFI_INT_SRC_MCU_IFC            = 8,    /* bit8, MCU???????????????? */
    IPC_HIFI_INT_SRC_BUTT               = 32,

    /* ????BBE16 IPC??????????????bit???? */
    IPC_INT_MSP_DSP_OM_MBX              = 0,   /* bit0, ????ARM->DSP */
    IPC_INT_PS_DSP_PUB_MBX              = 1,   /* bit1, ????ARM->DSP */
    IPC_INT_PS_DSP_MAC_MBX              = 2,   /* bit2, ????ARM->DSP */
    IPC_INT_HIFI_DSP_MBX                = 3,   /* bit3, HIFI->DSP */
    IPC_BBE16_INT_SRC_HIFI_MSG          = 3,   /* bit3, ??????????v7r2??v9r1??????????bit3?????????????? */
    IPC_BBE16_INT_SRC_BUTT              = 32,
    /* ?????????????? */
    IPC_INT_BUTTOM                      = 32
}IPC_INT_LEV_E;

typedef enum tagIPC_SEM_ID_E
{
    IPC_SEM_ICC = 0,
    IPC_SEM_NAND = 1,
    IPC_SEM_MEM = 2,
    IPC_SEM_DICC = 3,
    IPC_SEM_RFILE_LOG = 4,          /*????flashless????????*/
    IPC_SEM_EMMC = 5,
    IPC_SEM_NVIM = 6,
    IPC_SEM_TELE_MNTN = 7,
    IPC_SEM_MEDPLL_STATE = 8,
	IPC_SEM_EFUSE = 9,
    IPC_SEM_BBPMASTER_0 = 10,
    IPC_SEM_BBPMASTER_1 = 11,
    IPC_SEM_BBPMASTER_2 = 12,
    IPC_SEM_BBPMASTER_3 = 13,
    IPC_SEM_GU_SLEEP    = 14,
    IPC_SEM_BBPMASTER_5 = 15,
    IPC_SEM_BBPMASTER_6 = 16,
    IPC_SEM_BBPMASTER_7 = 17,
    IPC_SEM_DPDT_CTRL_ANT = 19, /*just for compile in portland*/
    IPC_SEM_SMP_CPU0 = 21,
    IPC_SEM_SMP_CPU1 = 22,
    IPC_SEM_SMP_CPU2 = 23,
    IPC_SEM_SMP_CPU3 = 24,
    IPC_SEM_SYNC = 25,
    IPC_SEM_BBP = 26,           /*??????BBE16????????????????????BBE16??????????*/
    IPC_SEM_CPUIDLE = 27,   /*CPUIDLE A/M????????????*/
	IPC_SEM_BBPPS = 28,         /*BBP PS??????????*/
	IPC_SEM_HKADC = 29,     /*A??M??????HKADC????????*/
    IPC_SEM_SYSCTRL = 30,
    IPC_SEM_ZSP_HALT = 31,
    IPC_SEM_BUTTOM
}IPC_SEM_ID_E;

#else
typedef enum tagIPC_INT_LEV_E
{
    IPC_CCPU_INT_SRC_HIFI_MSG              = 0, /* HIFI???????????? */
    IPC_CCPU_INT_SRC_MCU_MSG               = 1, /* MCU???????????? */
    IPC_INT_DSP_HALT                       = 2, /* DSP????ARM????*/
    IPC_INT_DSP_RESUME                     = 3, /* DSP????ARM????????????????????*/
                                                /* 4, ????????????????????,??????????IPC_INT_DICC_USRDATA????*/
                                                /* 5, ????????????????????,??????????IPC_INT_DICC_USRDATA????*/
    IPC_INT_WAKE_GU                        = 6, /* ????????????????*/
    IPC_INT_SLEEP_GU                          , /* ????????????*/
    /* ????TDS??????????IPC????,begin */
    IPC_INT_WAKE_GSM                       , /* BBE16????CCPU,????GSM???? */
    IPC_INT_WAKE_WCDMA                     , /* BBE16????CCPU,????WCDMA???? */
    IPC_INT_DSP_PS_PUB_MBX                 ,/* ????DSP->ARM,????????IPC???????? */
    IPC_INT_DSP_PS_MAC_MBX                 ,/* ????DSP->ARM,????????IPC???????? */
    IPC_INT_DSP_MBX_RSD                    ,/* ????DSP->ARM,????????IPC???????? */
    IPC_CCPU_INT_SRC_DSP_MNTN              ,/* BBE16????????????????mailbox???????? */
    IPC_CCPU_INT_SRC_DSP_RCM_PUB_MBX       ,
    /* ????TDS??????????IPC????,end */
    IPC_CCPU_INT_SDR_CCPU_BBP_MASTER_ERROR , /* BBE16????C???????????????? */
    IPC_CCPU_INT_SRC_COMMON_END            , /* V7??V8??????C???????????????? */

    /* C????????v7  ?????? ipc ???? */
    IPC_CCPU_INT_SRC_ACPU_RESET            = IPC_CCPU_INT_SRC_COMMON_END,    /* A/C????C??????????????????????(????C??), v8r2?????????????? */
    IPC_CCPU_SRC_ACPU_DUMP                 ,   /* v7r2 ??????????ACPU ??????CCPU ??IPC???? */
    IPC_CCPU_INT_SRC_ICC_PRIVATE           ,   /* v7r2 ICC??????icc????????????????,ACPU??????CCPU??IPC????       */
    IPC_CCPU_INT_SRC_MCPU                  ,   /* ICC????, MCU??????CCPU??  IPC???? */
    IPC_CCPU_INT_SRC_MCPU_WDT              ,   /* ????trace????wdt */
    IPC_CCPU_INT_SRC_XDSP_1X_HALT          ,   /* 1X Halt???? */
    IPC_CCPU_INT_SRC_XDSP_HRPD_HALT        ,   /* HRPD Halt???? */
    IPC_CCPU_INT_SRC_XDSP_1X_RESUME        ,   /* 1X Resume???? */
	IPC_CCPU_INT_SRC_XDSP_HRPD_RESUME      ,   /* HRPD Resume???? */
	IPC_CCPU_INT_SRC_XDSP_MNTN             ,   /* XDSP???????????????? */
	IPC_CCPU_INT_SRC_XDSP_PS_MBX           ,   /* PS??X???????????????????? */
    IPC_CCPU_INT_SRC_XDSP_RCM_MBX          ,   /* RCM??X???????????????????? */

	/*  austin?????? mcu ???????? icc ??????, ???????? */
    IPC_CCPU_INT_SRC_ACPU_ICC			   = 31, /* acpu????ccpu????*/

	/* C????????v8 ??????ipc ???? */
    IPC_CCPU_INT_SRC_ACPU_MSG              = IPC_CCPU_INT_SRC_COMMON_END,      /* ????mailbox,????????????*/
    IPC_CCPU_INT_SRC_ACPU_IFC              ,      /* ????????????,????????????*/
    IPC_CCPU_INT_SRC_MCU_IFC               ,      /* ????????????,????????????*/
    IPC_INT_ARM_SLEEP                      ,      /* ????????????*/

    /* ????MCU IPC??????????????bit???? */
    IPC_MCU_INT_SRC_ACPU_MSG             = 0,    /* ACPU???????????? */
    IPC_MCU_INT_SRC_CCPU_MSG                ,    /* CCPU???????????? */
    IPC_MCU_INT_SRC_HIFI_MSG                ,    /* HIFI???????????? */
    IPC_MCU_INT_SRC_CCPU_IPF                ,    /* IPF?????? */
    IPC_MCU_INT_SRC_ACPU_PD                 ,    /* acpu power down */
    IPC_MCU_INT_SRC_HIFI_PD                 ,    /* hifi power down */
    IPC_MCU_INT_SRC_HIFI_DDR_VOTE           ,    /* HIFI DDR?????????? */
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLOW    ,
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_SLEEP   ,
    IPC_MCU_INT_SRC_ACPU_I2S_REMOTE_INVALID ,
    IPC_MCU_INT_SRC_COMMON_END              ,
    /* v7  ?????? ipc ???? */
    /* ACPU??CCPU????IPC??????????????????????M3 */
    IPC_MCU_INT_SRC_ACPU_DRX                = IPC_MCU_INT_SRC_COMMON_END,
    IPC_MCU_INT_SRC_CCPU_DRX                ,   /* CCPU??ACPU????IPC??????????????????????M3 */
    IPC_MCU_INT_SRC_ICC_PRIVATE             ,   /* m3 icc??????ipc?????????????????????????????? */
    IPC_MCU_INT_SRC_DUMP                    ,   /* m3 dump??????ACORE??????MCU??IPC????*/
    IPC_MCU_INT_SRC_HIFI_PU                 ,   /* POWERUP,  HIFI??????MCU??IPC???? */
    IPC_MCU_INT_SRC_HIFI_DDR_DFS_QOS        , /* HIFI??DDR???????? */
    IPC_MCU_INT_SRC_TEST                    ,   /* for test a\c interact with m3 */
    IPC_MCPU_INT_SRC_ACPU_USB_PME_EN        ,  /* acore??M3????USB???????????? */
    /* ????3??austin????????AP?????????????? */
	IPC_MCU_INT_SRC_ICC 				= 29,	/* m3 icc??????ipc???? */
	IPC_MCU_INT_SRC_CCPU_PD 			= 30,	 /* ccpu power down */
	IPC_MCU_INT_SRC_CCPU_START			= 31,	/* ????MCU????CCPU */


    /* v8 ??????ipc ???? */
    /* HIFI????????????,????????????,???????????? */
    IPC_MCU_INT_SRC_CCPU_IFC                    = IPC_MCU_INT_SRC_COMMON_END,
    IPC_MCU_INT_SRC_ACPU_IFC                    ,   /* ACPU????????????????,???????????? */
    IPC_MCU_INT_SRC_HIFI_IFC                    ,   /* HIFI????????????????*/
    IPC_MCU_INT_SRC_ACPU0_PD                    ,   /* acpu0 power down, V8???? */
    IPC_MCU_INT_SRC_ACPU1_PD                    ,   /* acpu1 power down, V8???? */
    IPC_MCU_INT_SRC_ACPU2_PD                    ,   /* acpu2 power down, V8???? */
    IPC_MCU_INT_SRC_ACPU3_PD                    ,   /* acpu3 power down , V8????*/
    IPC_MCU_INT_SRC_ACPU4_PD                    ,   /* acpu4:cluster1 core0 power down */
    IPC_MCU_INT_SRC_ACPU5_PD                    ,   /* acpu5:cluster1 core1 power down */
    IPC_MCU_INT_SRC_ACPU6_PD                    ,   /* acpu6:cluster1 core2 power down */
    IPC_MCU_INT_SRC_ACPU7_PD                    ,   /* acpu7:cluster1 core3 power down */
    IPC_MCU_INT_SRC_ACPU_HOTPLUG                ,   /* acpu hotplug--no use, V8???? */
    IPC_MCU_INT_SRC_MCU_AGT                     ,   /* mcu agent */
    IPC_MCU_INT_SRC_HIFI_MEMSHARE_DDR_VOTE      ,   /* MP3????????????????HARQMEM,HIFI DDR??????????*/
    IPC_MCU_INT_SRC_HIFI_MEMSHARE_DDR_EXIT_VOTE ,   /* MP3????????????????HARQMEM,HIFI????DDR??????????*/
    IPC_MCU_INT_SRC_ACPU_DFS                    ,   /*ACPU DFS*/

    IPC_MCU_INT_SRC_END                         ,

    /* ????ACPU IPC??????????????bit???? */
    IPC_ACPU_INT_SRC_CCPU_MSG             = 0,   /* CCPU???????????? */
    IPC_ACPU_INT_SRC_HIFI_MSG             = 1,   /* HIFI???????????? */
    IPC_ACPU_INT_SRC_MCU_MSG              = 2,   /* ACPU???????????? */
    IPC_ACPU_INT_SRC_CCPU_NVIM            = 3,   /* ????NVIM????C????A????????????*/
    IPC_INT_DICC_USRDATA                  = 4,   /* ??????CCPU??????????????????*/
    IPC_INT_DICC_RELDATA                  = 5,   /* ??????CCPU??????????????????*/
    IPC_ACPU_INT_SRC_CCPU_ICC                ,
    IPC_ACPU_INT_SRC_COMMON_END              ,
    /* v7  ?????? ipc ???? */
    IPC_ACPU_INT_SRC_ICC_PRIVATE          = IPC_ACPU_INT_SRC_COMMON_END,   /* v7r2 icc ??????CCPU??????ACPU??IPC???? */
    IPC_ACPU_SRC_CCPU_DUMP                ,   /* v7r2 ??????????CCPU ??????ACPU ??IPC???? */
    IPC_ACPU_INT_SRC_MCPU                 ,   /* ICC????, MCU??????ACPU?? IPC???? */
    IPC_ACPU_INT_SRC_MCPU_WDT             ,  /* ????trace????wdt */
    IPC_ACPU_INT_MCU_SRC_DUMP             ,  /* dump?????MCU??????ACPU??IPC???? */
    IPC_ACPU_INT_SRC_CCPU_RESET_IDLE      ,  /* A/C????C??????????????????????(master in idle)???????? */
    IPC_ACPU_INT_SRC_CCPU_RESET_SUCC      ,  /* A/C????C??????????????????????(ccore reset ok)???????? */

    IPC_ACPU_INT_SRC_CCPU_LOG             ,  /* CCPU??????ACPU??LOG???????? */
    IPC_ACPU_INT_SRC_MCU_FOR_TEST         ,  /* test, m core to acore */
    IPC_ACPU_INT_SRC_CCPU_TEST_ENABLE     ,  /* IPC??????????????????????????????IPC?????????????????????? */
    IPC_ACPU_INT_SRC_MCPU_USB_PME         ,  /* M3??acore????USB???????? */
    IPC_ACPU_INT_SRC_HIFI_PC_VOICE_RX_DATA,  /* hifi->acore pc voice */
    IPC_ACPU_INT_SRC_CCPU_PM_OM           ,  /* CCPU????ACPU??PMOM????,??????????, ?????? */

    /* v8 ??????ipc ???? */
    IPC_ACPU_INT_SRC_CCPU_IFC             = IPC_ACPU_INT_SRC_COMMON_END,       /* ????????????*/
    IPC_ACPU_INT_SRC_MCU_IFC              ,   /* ????????????*/
    IPC_ACPU_INT_SRC_MCU_THERMAL_HIGH     ,   /* MCU????????????????IPC????????ACPU, ????????????*/
    IPC_ACPU_INT_SRC_MCU_THERMAL_LOW      ,  /* MCU????????????????IPC????????ACPU*/
    IPC_ACPU_INI_SRC_MCU_TELE_MNTN_NOTIFY ,  /* ????????????,??????????*/
    IPC_ACPU_INI_SRC_MCU_EXC_REBOOT       ,  /* ????????????,??????????*/
    IPC_ACPU_INT_SRC_CCPU_EXC_REBOOT      ,  /* ????????CCPU ????????????ACPUfuwei*/
    IPC_ACPU_INT_SRC_CCPU_NORMAL_REBOOT   ,  /* ????????;*RIL????Cfuwei, C????????????????A??????????*/
    IPC_ACPU_INT_SRC_MCU_DDR_EXC          ,  /* ????????,MCU*/

    IPC_ACPU_INT_SRC_END              ,

    /* ????HIFI IPC??????????????bit???? */
    IPC_HIFI_INT_SRC_ACPU_MSG = 0 ,  /* ACPU???????????? */
    IPC_HIFI_INT_SRC_CCPU_MSG     ,  /* CCPU???????????? */
    IPC_HIFI_INT_SRC_BBE_MSG      ,  /* TDDSP???????????? */
    IPC_HIFI_INT_SRC_MCU_MSG      ,
    IPC_HIFI_INT_SRC_COMMON_END   ,

    /* v8 ??????ipc ???? */
    IPC_HIFI_INT_SRC_MCU_WAKE_DDR = IPC_HIFI_INT_SRC_COMMON_END,  /* MCU????HIFI????DDR */
    IPC_HIFI_INT_SRC_MCU_IFC      ,

    IPC_HIFI_INT_SRC_END          ,

    /* ????BBE16 IPC??????????????bit???? */
    IPC_INT_MSP_DSP_OM_MBX = 0 ,   /* ????ARM->DSP */
    IPC_INT_PS_DSP_PUB_MBX     ,   /* ????ARM->DSP */
    IPC_INT_PS_DSP_MAC_MBX     ,   /* ????ARM->DSP */
    IPC_INT_HIFI_DSP_MBX       ,   /* HIFI->DSP */
    IPC_BBE16_INT_SRC_HIFI_MSG ,   /* ??????????HIFI??????BBE16??IPC???? */

    IPC_BBE16_INT_SRC_END  ,


    /* ????XDSP IPC??????????????bit???? */
    IPC_XDSP_INT_SRC_CCPU_1X_WAKE = 0 ,   /* 1X Wake???? */
    IPC_XDSP_INT_SRC_CCPU_HRPD_WAKE   ,   /* HRPD Wake???? */
    IPC_XDSP_INT_SRC_CCPU_OM_MBX      ,   /* C??->XDSP */
    IPC_XDSP_INT_SRC_CCPU_PUB_MBX     ,   /* C??->XDSP */

	IPC_XDSP_INT_SRC_END ,


    /* ?????????????? */
    IPC_INT_BUTTOM             = 32,
}IPC_INT_LEV_E;

typedef enum tagIPC_SEM_ID_E
{
    IPC_SEM_MEM          ,
    IPC_SEM_DICC         ,
    IPC_SEM_EMMC         ,
    IPC_SEM_SYNC         ,
    IPC_SEM_SYSCTRL      ,
    IPC_SEM_BBP          ,  /*??????BBE16????????????????????BBE16??????????*/
    IPC_SEM_RFILE_LOG    ,  /*????flashless????????*/
    IPC_SEM_NVIM         ,
    IPC_SEM_EFUSE        ,
    IPC_SEM_DPDT_CTRL_ANT,  /* ????mdrv_ipc_spin_trylock?????? */
    IPC_SEM_BBPMASTER_0  ,
    IPC_SEM_BBPMASTER_1  ,
    IPC_SEM_BBPMASTER_2  ,
    IPC_SEM_BBPMASTER_3  ,
    IPC_SEM_BBPMASTER_5  ,
    IPC_SEM_BBPMASTER_6  ,
    IPC_SEM_BBPMASTER_7  ,
    IPC_SEM_COMMON_END   ,

    /* v7 ??????IPC SEM */
    IPC_SEM_SPI0         = IPC_SEM_COMMON_END,
    IPC_SEM_NV           ,
    IPC_SEM_GPIO         ,
    IPC_SEM_CLK          ,
    IPC_SEM_PMU          ,
    IPC_SEM_MTCMOS       ,
    IPC_SEM_IPF_PWCTRL   ,
    IPC_SEM_PMU_FPGA     , /*????pmu??????????,????????????*/
    IPC_SEM_NV_CRC       ,
    IPC_SEM_PM_OM_LOG    ,
    IPC_SEM_MDRV_LOCK    ,
    IPC_SEM_CDMA_DRX     , /* C????XDSP???? */
    IPC_SEM_GU_SLEEP     ,
    IPC_SEM2_IPC_TEST    , /*IPC??????????????*/

    /* v8 ??????IPC SEM */
    IPC_SEM_ICC          = IPC_SEM_COMMON_END,
    IPC_SEM_NAND         ,
    IPC_SEM_TELE_MNTN    ,
    IPC_SEM_MEDPLL_STATE ,
    IPC_SEM_SMP_CPU0     ,
    IPC_SEM_SMP_CPU1     ,
    IPC_SEM_SMP_CPU2     ,
    IPC_SEM_SMP_CPU3     ,
    IPC_SEM_CPUIDLE      ,   /*CPUIDLE A/M????????????*/
    IPC_SEM_BBPPS        ,   /*BBP PS??????????*/
    IPC_SEM_HKADC        ,   /*A??M??????HKADC????????*/

    IPC_SEM_END          ,

    IPC_SEM_BUTTOM       = 32
} IPC_SEM_ID_E;
/*lint +e488*/
#endif


#ifdef __cplusplus
}
#endif

#endif
