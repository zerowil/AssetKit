/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef __libassetkit__assetkit_string__h_
#define __libassetkit__assetkit_string__h_

AkResult
ak_strtof(char ** __restrict src,
           AkFloat * __restrict dest,
           unsigned long n);

AkResult
ak_strtomb(char ** __restrict src,
            AkBool * __restrict dest,
            unsigned long m,
            unsigned long n);

AkResult
ak_strtomi(char ** __restrict src,
            AkInt * __restrict dest,
            unsigned long m,
            unsigned long n);

AkResult
ak_strtomf(char ** __restrict src,
            AkFloat * __restrict dest,
            unsigned long m,
            unsigned long n);

AkResult
ak_strtof_s(const char * __restrict src,
             AkFloat * __restrict dest,
             unsigned long n);

extern
AkResult
ak_strtof4(char ** __restrict src,
            AkFloat4 * __restrict dest);

extern
AkResult
ak_strtof4_s(const char * __restrict src,
              AkFloat4 * __restrict dest);

#endif /* __libassetkit__assetkit_string__h_ */
