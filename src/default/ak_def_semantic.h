/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_def_semantic_h
#define ak_def_semantic_h

#include "../ak_common.h"

typedef struct AkInputSemanticPair {
  uint32_t        count;
  AkDataType     *type;
  char           **params;
} AkInputSemanticPair;

const AkInputSemanticPair**
ak_def_semantic();

uint32_t
ak_def_semanticc();

#endif /* ak_def_semantic_h */