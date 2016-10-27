/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../ak_common.h"
#include "../ak_memory_common.h"

AK_EXPORT
void
ak_changeCoordSys(AkDoc * __restrict doc,
                  AkCoordSys * newCoordSys) {
  AkLibGeometry *libGeom;
  AkGeometry    *geom;

  libGeom = doc->lib.geometries;

  while (libGeom) {
    geom = libGeom->chld;

    while (geom) {
      ak_changeCoordSysGeom(geom, newCoordSys);
      geom = geom->next;
    }

    libGeom = libGeom->next;
  }

  doc->coordSys = newCoordSys;
}