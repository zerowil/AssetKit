/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../ak_common.h"
#include "../ak_memory_common.h"

AK_EXPORT
AkGeometry *
ak_libFirstGeom(AkDoc * __restrict doc) {
  if (!doc->lib.geometries)
    return NULL;

  return doc->lib.geometries->chld;
}
