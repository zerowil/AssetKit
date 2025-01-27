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

#include "../common.h"
//
//AK_EXPORT
//AkBuffer*
//ak_sourceDetachArray(AkAccessor * __restrict acc) {
//  AkHeap      *heap;
//  AkBuffer    *buff, *newbuff;
//  AkDataParam *dp;
//  size_t       size, icount, i;
//
//  heap = ak_heap_getheap(acc);
//  buff = ak_getObjectByUrl(&acc->source);
//  if (!buff)
//    return NULL;
//
//  if (acc->componentType == AKT_STRING) {
//    /* TODO: */
//    return NULL;
//  }
//
//  size = acc->count * acc->bound;
//
//  newbuff = ak_heap_calloc(heap, NULL, sizeof(*newbuff));
//  newbuff->length = size;
//
//  newbuff->data = ak_heap_alloc(heap, newbuff, size);
//
//
//  switch (acc->componentType) {
//    case AKT_FLOAT: {
//      AkFloat *olditms, *newitms;
//      AkUInt   j;
//
//      newitms = newbuff->data;
//      olditms = buff->data;
//
//      icount = newbuff->length / sizeof(float);
//      for (i = 0; i < icount; i++) {
//        j  = 0;
//        dp = acc->param;
//
//        while (dp) {
//          if (!dp->name)
//            continue;
//
//          newitms[i + j++] = olditms[i + acc->offset + dp->offset];
//          dp = dp->next;
//        }
//      }
//      break;
//    }
//    case AKT_INT: {
//      AkInt *olditms, *newitms;
//      AkUInt j;
//
//      newitms = newbuff->data;
//      olditms = buff->data;
//
//      icount = newbuff->length / sizeof(int32_t);
//      for (i = 0; i < icount; i++) {
//        j  = 0;
//        dp = acc->param;
//
//        while (dp) {
//          if (!dp->name)
//            continue;
//
//          newitms[i + j++] = olditms[i + acc->offset + dp->offset];
//          dp = dp->next;
//        }
//      }
//      break;
//    }
//    case AKT_STRING: {
//      /* TODO: */
//      break;
//    }
//    case AKT_BOOL: {
//      bool  *olditms, *newitms;
//      AkUInt j;
//
//      newitms = newbuff->data;
//      olditms = buff->data;
//
//      icount = newbuff->length / sizeof(bool);
//      for (i = 0; i < icount; i++) {
//        j  = 0;
//        dp = acc->param;
//
//        while (dp) {
//          if (!dp->name)
//            continue;
//
//          newitms[i + j++] = olditms[i + acc->offset + dp->offset];
//          dp = dp->next;
//        }
//      }
//      break;
//    }
//    default:
//      return NULL;
//  }
//
//  return newbuff;
//}
