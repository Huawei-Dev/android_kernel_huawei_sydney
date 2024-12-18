

#ifndef __OAL_LINUX_TYPES_H__
#define __OAL_LINUX_TYPES_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ??????????????
*****************************************************************************/
/*****************************************************************************
  2 ????????????????
*****************************************************************************/
typedef signed char             oal_int8;           /* ????????:c */
typedef signed short            oal_int16;          /* ????????:s */
typedef signed int              oal_int32;          /* ????????:l */
typedef signed long long        oal_int64;          /* ????????:ll */
typedef unsigned char           oal_uint8;          /* ????????:uc */
typedef unsigned short          oal_uint16;         /* ????????:us */
typedef unsigned int            oal_uint32;         /* ????????:ul */
typedef unsigned long long      oal_uint64;         /* ????????:ull */
typedef void                    oal_void;

/*****************************************************************************
  3 ??????
*****************************************************************************/
#ifdef INLINE_TO_FORCEINLINE
#define OAL_INLINE      __forceinline
#else
#define OAL_INLINE          inline
#endif

#define OAL_STATIC          static

#if defined(_PRE_PC_LINT)
#define OAL_VOLATILE
#else
#define OAL_VOLATILE        volatile
#endif

/*****************************************************************************
  4 ????????
*****************************************************************************/


/*****************************************************************************
  5 ????????????
*****************************************************************************/


/*****************************************************************************
  6 ??????????
*****************************************************************************/


/*****************************************************************************
  7 ????????
*****************************************************************************/


/*****************************************************************************
  8 STRUCT????
*****************************************************************************/


/*****************************************************************************
  9 UNION????
*****************************************************************************/


/*****************************************************************************
  10 ????????
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of oal_types.h */

