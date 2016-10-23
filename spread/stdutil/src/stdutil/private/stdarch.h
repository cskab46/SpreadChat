/* Copyright (c) 2000-2009, The Johns Hopkins University
 * All rights reserved.
 *
 * The contents of this file are subject to a license (the ``License'').
 * You may not use this file except in compliance with the License. The
 * specific language governing the rights and limitations of the License
 * can be found in the file ``STDUTIL_LICENSE'' found in this
 * distribution.
 *
 * Software distributed under the License is distributed on an AS IS
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *
 * The Original Software is:
 *     The Stdutil Library
 *
 * Contributors:
 *     Creator - John Lane Schultz (jschultz@cnds.jhu.edu)
 *     The Center for Networking and Distributed Systems
 *         (CNDS - http://www.cnds.jhu.edu)
 */

#ifndef stdarch_h_2005_07_12_00_51_28_jschultz_at_cnds_jhu_edu
#define stdarch_h_2005_07_12_00_51_28_jschultz_at_cnds_jhu_edu

/* Modified to use standard int types */
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STDARCH_MAX_BYTE_ALIGNMENT sizeof(max_align_t)
#define STDARCH_NULL_IS_ZERO 1

typedef int32_t stdarch_int32;
typedef uint32_t stdarch_uint32;
typedef int64_t stdarch_int64;
typedef uint64_t stdarch_uint64;
typedef size_t stdarch_ssize;

/* Little-endian (x86) support only */
#define STDENDIAN16_SWAP 1

#define STDENDIAN32_NET0_FROM_HOST 3
#define STDENDIAN32_NET1_FROM_HOST 2
#define STDENDIAN32_NET2_FROM_HOST 1
#define STDENDIAN32_NET3_FROM_HOST 0

#define STDENDIAN32_HOST0_FROM_NET 3
#define STDENDIAN32_HOST1_FROM_NET 2
#define STDENDIAN32_HOST2_FROM_NET 1
#define STDENDIAN32_HOST3_FROM_NET 0

#define STDENDIAN64_NET0_FROM_HOST 7
#define STDENDIAN64_NET1_FROM_HOST 6
#define STDENDIAN64_NET2_FROM_HOST 5
#define STDENDIAN64_NET3_FROM_HOST 4
#define STDENDIAN64_NET4_FROM_HOST 3
#define STDENDIAN64_NET5_FROM_HOST 2
#define STDENDIAN64_NET6_FROM_HOST 1
#define STDENDIAN64_NET7_FROM_HOST 0

#define STDENDIAN64_HOST0_FROM_NET 7
#define STDENDIAN64_HOST1_FROM_NET 6
#define STDENDIAN64_HOST2_FROM_NET 5
#define STDENDIAN64_HOST3_FROM_NET 4
#define STDENDIAN64_HOST4_FROM_NET 3
#define STDENDIAN64_HOST5_FROM_NET 2
#define STDENDIAN64_HOST6_FROM_NET 1
#define STDENDIAN64_HOST7_FROM_NET 0

#ifdef __cplusplus
}
#endif

#endif
