#ifndef __BASE_TYPE__
#define __BASE_TYPE__
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

/*----- constant and type define -----------------------------------------*/

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef true
#define true    1
#endif

#ifndef false
#define false   0
#endif

#ifndef NULL
#define NULL    0
#endif


#ifndef BOOL
typedef bool                             BOOL;
#endif
#ifndef UINT8
typedef unsigned char                   UINT8;
#endif
#ifndef INT8
typedef signed char                     INT8;
#endif

#ifndef UINT16
typedef unsigned short                  UINT16;
#endif
#ifndef INT16
typedef signed short                    INT16;
#endif

#ifndef UINT32
typedef unsigned long                   UINT32;
#endif
#ifndef INT32
typedef signed long                     INT32;
#endif

#ifndef UINT8D
typedef unsigned char  __data             UINT8D;
#endif
#ifndef UINT16D
typedef unsigned short __data             UINT16D;
#endif
#ifndef UINT32D
typedef unsigned long  __data             UINT32D;
#endif
#ifndef UINT8I
typedef unsigned char  __idata            UINT8I;
#endif
#ifndef UINT16I
typedef unsigned short __idata            UINT16I;
#endif
#ifndef UINT32I
typedef unsigned long  __idata            UINT32I;
#endif
#ifndef UINT8X
typedef unsigned char  __xdata            UINT8X;
#endif
#ifndef UINT16X
typedef unsigned short __xdata            UINT16X;
#endif
#ifndef UINT32X
typedef unsigned long  __xdata            UINT32X;
#endif
#ifndef UINT8V
typedef unsigned char volatile          UINT8V;
#endif
#ifndef UINT8DV
typedef unsigned char volatile __data     UINT8DV;
#endif
#ifndef UINT8XV
typedef unsigned char volatile __xdata    UINT8XV;
#endif
#ifndef UINT8PV
typedef unsigned char volatile __pdata    UINT8PV;
#endif
#ifndef UINT8C
typedef const unsigned char __code        UINT8C;
#endif
#ifndef PUINT8
typedef unsigned char                 *PUINT8;
#endif
#ifndef PUINT16
typedef unsigned short                *PUINT16;
#endif
#ifndef PUINT32
typedef unsigned long                 *PUINT32;
#endif
#ifndef PUINT8I
typedef unsigned char  __idata          *PUINT8I;
#endif
#ifndef PUINT16I
typedef unsigned short __idata          *PUINT16I;
#endif
#ifndef PUINT32I
typedef unsigned long  __idata          *PUINT32I;
#endif
#ifndef PUINT8X
typedef unsigned char  __xdata          *PUINT8X;
#endif
#ifndef PUINT16X
typedef unsigned short __xdata          *PUINT16X;
#endif
#ifndef PUINT32X
typedef unsigned long  __xdata          *PUINT32X;
#endif
#ifndef PUINT8V
typedef unsigned char volatile        *PUINT8V;
#endif
#ifndef PUINT8DV
typedef unsigned char volatile __data   *PUINT8DV;
#endif
#ifndef PUINT8XV
typedef unsigned char volatile __xdata  *PUINT8XV;
#endif
#ifndef PUINT8PV
typedef unsigned char volatile __pdata  *PUINT8PV;
#endif
#ifndef PUINT8C
typedef const unsigned char __code      *PUINT8C;
#endif

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;
typedef unsigned long   ulong;

#ifndef STRUCT_OFFSET
#define STRUCT_OFFSET( s, m )       ( (UINT8)( & (((s) *)0) -> (m) ) )  /* get the offset address for a member of a structure */
#endif

#ifdef __cplusplus
}
#endif

#endif  // __BASE_TYPE__
