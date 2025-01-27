/*
 * Copyright (C) 2020 Recep Aslantas
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fix.h"

_assetkit_hide
void
ak_skinFixWeights(AkMesh * __restrict mesh) {
  AkHeap          *heap;
  AkDoc           *doc;
  AkMeshPrimitive *prim;
  AkSkin          *skin;
  FListItem       *skinItem;
  AkBoneWeights   *wl;
  AkBoneWeight    *w, *iw, *old, *oiw;
  AkDuplicator    *dupl;
  AkUIntArray     *dupc, *dupcsum;
  AkAccessor      *acci;
  size_t          *pOldIndex, *wi;
  size_t           vc, d, s, pno, poo, nwsum, newidx, next, tmp, count;
  uint32_t        *nj, i, j, k, vcount, primIndex;

  if (!(skinItem = mesh->skins))
    return;

  heap      = ak_heap_getheap(mesh->geom);
  doc       = ak_heap_data(heap);
  pOldIndex = NULL;

  /* fix every skin that attached to the mesh */
  do {
    skin      = skinItem->data;
    prim      = mesh->primitive;
    primIndex = 0;

    while (prim) {
      if (!(dupl = rb_find(doc->reserved, prim))
          || dupl->dupCount < 1
          || !(acci = prim->pos->accessor))
        continue;

      wl          = skin->weights[primIndex];

      old         = wl->weights;
      pOldIndex   = wl->indexes;
      vc          = acci->count;
      dupc        = dupl->range->dupc;
      dupcsum     = dupl->range->dupcsum;
      nwsum       = 0;

      wl->nVertex = count = dupl->bufCount + dupl->dupCount;
      nj          = ak_heap_alloc(heap, wl, count * sizeof(uint32_t));
      wi          = ak_heap_alloc(heap, wl, count * sizeof(size_t));

      /* copy to new location and duplicate if needed */
      for (i = 0; i < vc; i++) {
        if ((poo = dupc->items[3 * i + 2]) == 0)
          continue;

        pno    = dupc->items[3 * i];
        d      = dupc->items[3 * i + 1];
        s      = dupcsum->items[pno];
        vcount = wl->counts[poo - 1];

        for (j = 0; j <= d; j++) {
          newidx     = pno + j + s;
          wi[newidx] = vcount; /* weight index     */
          nj[newidx] = vcount; /* number of joints */
        }

        nwsum += vcount * (d + 1);
      }

      /* prepare weight index */
      for (next = j = 0; j < wl->nVertex; j++) {
        tmp   = wi[j];
        wi[j] = next;
        next  = tmp + next;
      }

      /* now we know the size of arrays: weights, pCount, pIndex */
      w     = ak_heap_alloc(heap, wl, sizeof(*w) * nwsum);
      nwsum = 0;

      for (i = 0; i < vc; i++) {
        if ((poo = dupc->items[3 * i + 2]) == 0)
          continue;

        pno    = dupc->items[3 * i];
        d      = dupc->items[3 * i + 1];
        s      = dupcsum->items[pno];
        vcount = wl->counts[poo - 1];

        for (j = 0; j <= d; j++) {
          newidx = wi[pno + j + s];

          for (k = 0; k < vcount; k++) {
            iw         = &w[newidx + k];
            oiw        = &old[pOldIndex[poo - 1] + k];
            iw->joint  = oiw->joint;
            iw->weight = oiw->weight;
          }
        }

        nwsum += vcount * (d + 1);
      }

      if (pOldIndex)
        ak_free(pOldIndex);

      if (old)
        ak_free(old);

      if (wl->counts)
        ak_free(wl->counts);

      wl->counts  = nj;
      wl->indexes  = wi;
      wl->weights = w;

      primIndex++;
      prim = prim->next;
    }

    skinItem = skinItem->next;
  } while (skinItem);
}
