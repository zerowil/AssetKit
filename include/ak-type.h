/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_types_h
#define ak_types_h

#include <stdbool.h>

typedef enum AkTypeId {
  AKT_NONE = 0,
  AKT_EFFECT,
  AKT_PROFILE,
  AKT_PARAM,
  AKT_NEWPARAM,
  AKT_SETPARAM,
  AKT_TECHNIQUE_FX,
  AKT_TECHNIQUE,
  AKT_SAMPLER,
  AKT_TEXTURE,
  AKT_TEXTURE_NAME,
  AKT_TEXCOORD
} AkTypeId;

AkTypeId
ak_typeid(void * __restrict mem);

AkTypeId
ak_typeidh(AkHeapNode * __restrict hnode);

void
ak_setypeid(void * __restrict mem,
            AkTypeId tid);

bool
ak_isKindOf(void * __restrict mem,
            void * __restrict other);

#endif /* ak_types_h */