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

#ifndef dae_brep_surface_h
#define dae_brep_surface_h

#include "../common.h"

AkSurface* _assetkit_hide
dae_surface(DAEState * __restrict dst,
            xml_t    * __restrict xml,
            void     * __restrict memp);

AkSurfaces* _assetkit_hide
dae_surfaces(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp);

#endif /* dae_brep_surface_h */
