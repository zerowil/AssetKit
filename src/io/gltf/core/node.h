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

#ifndef gltf_node_h
#define gltf_node_h

#include "../common.h"

void _assetkit_hide
gltf_nodes(json_t * __restrict jnode,
           void   * __restrict userdata);

AkNode* _assetkit_hide
gltf_node(AkGLTFState * __restrict gst,
          void        * __restrict memParent,
          json_t      * __restrict jnode,
          AkNode     ** __restrict nodechld);

void _assetkit_hide
gltf_bindMaterials(AkGLTFState        * __restrict gst,
                   AkInstanceGeometry * __restrict instGeom,
                   int32_t                         meshIndex);

#endif /* gltf_node_h */
