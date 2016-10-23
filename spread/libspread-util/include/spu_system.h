/* This system header contains those constants that change upon autoconf compilation and are
 * required for the Spread Util Library API.  These should not conflict with any other 
 * definitions in other software and should be safe to include in other software.  This file 
 * contains static definitions of the normally autoconf generated constants for a static
 * build on Windows.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#define SPU_HAVE_STDLIB_H 1
#define SPU_HAVE_LIMITS_H 1
#define SPU_SIZEOF_CHAR sizeof(char)
#define SPU_SIZEOF_SHORT_INT sizeof(short)
#define SPU_SIZEOF_INT sizeof(int)
#define SPU_SIZEOF_LONG_INT sizeof(long)
#define SPU_SIZEOF_LONG_LONG_INT sizeof(long long)

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#ifndef ARCH_PC_WIN95
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/param.h>
#endif

typedef unsigned int u_int;
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;

#ifndef byte
#define byte u_int8_t
#endif

#ifndef int16
#define int16 int16_t
#endif

#ifndef int16u
#define int16u u_int16_t
#endif

#ifndef int32
#define int32 int32_t
#endif

#ifndef int32u
#define int32u u_int32_t
#endif

#ifdef UIO_MAXIOV
#define         SPU_ARCH_SCATTER_SIZE       UIO_MAXIOV
#else
#define         SPU_ARCH_SCATTER_SIZE       1024
#endif

#endif /* SYSTEM_H */
