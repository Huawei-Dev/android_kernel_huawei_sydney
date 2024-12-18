

#ifndef __PLAT_EFUSE_H__
#define __PLAT_EFUSE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ??????????????
*****************************************************************************/

#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include "plat_debug.h"
#include "plat_firmware.h"

/*****************************************************************************
  2 ??????
*****************************************************************************/
#define GET_EFUSE_CMD          "0x50000744,64"
#define GET_EFUSE_EC_VERSION   "0x50000770,4"
#ifndef HISI_WIFI_MISC_TOP_DIR
#define EFUSE_FILE_PATH        "/data/misc/wifi/efuse"
#else
#define EFUSE_FILE_PATH        HISI_WIFI_MISC_TOP_DIR"/wifi/efuse"
#endif
#define EFUSE_REG_NUM          (16)
#define EFUSE_VALUE_WIDTH      (16)
#define EFUSE_REG_WIDTH        (4)
#define EFUSE_REG_ADDR_BASE    (0x50000744)
#define EFUSE_BUFF_LEN         (128)
#define EFUSE_RETRY            (3)
#define DIEID_BIT_4            (4)
#define DIEID_BIT_21           (21)
#define DIEID_BIT_45           (45)
#define DIEID_BIT_53           (53)
#define DIEID_BIT_79           (79)
#define DIEID_BIT_95           (95)

/*ec version define*/
#define V100                   (0x00)
#define V120                   (0x01)

#define STR_REG_NFC_EN_KEEP    ("0x60001044")

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
extern int32 number_type_cmd_send(uint8 *Key, uint8 *Value);
extern int32 read_msg(uint8 *data, int32 len);
extern void  store_efuse_info(void);
extern uint8 get_ec_version(void);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of plat_efuse.h */
