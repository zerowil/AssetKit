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

#ifndef wobj_commoh_h
#define wobj_commoh_h

#include "../../../include/ak/assetkit.h"
#include "../../common.h"
#include "../../utils.h"
#include "../../tree.h"
#include "../../json.h"

#include <string.h>
#include <stdlib.h>

typedef struct AkWOBJState {
  AkHeap       *heap;
  AkDoc        *doc;
  void         *tmpParent;
} AkWOBJState;

#endif /* wobj_commoh_h */
