/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_common.h"
#include "ak_memory_common.h"
#include "ak_memory_rb.h"
#include "ak_memory_lt.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#if !defined(_WIN32) && defined(USE_JEMALLOC)
#  include <jemalloc/jemalloc.h>
#else
# ifndef JEMALLOC_VERSION
#   define je_malloc(size)          malloc(size)
#   define je_calloc(size, count)   calloc(size, count)
#   define je_realloc(ptr, newsize) realloc(ptr, newsize)
#   define je_free(ptr)             free(ptr)
# endif
#endif

void
ak_heap_mk_srchnode(AkHeap * __restrict heap,
                    AkHeapNode * __restrict heapNode,
                    void * __restrict memId);

void
ak_heap_rm_srchnode(AkHeap * __restrict heap,
                    AkHeapNode * __restrict heapNode);

static
int
ak__heap_srch_cmp(void * __restrict key1,
                  void * __restrict key2);

static
void
ak__heap_srch_print(void * __restrict key);

static
char*
ak__heap_strdup_def(const char * str);

static char * ak__emptystr = "";

AkHeapAllocator ak__allocator = {
#ifdef JEMALLOC_VERSION
  .malloc   = je_malloc,
  .calloc   = je_calloc,
  .valloc   = je_valloc,
  .realloc  = je_realloc,
  .memalign = je_posix_memalign,
  .free     = je_free,
#else
  .malloc   = malloc,
  .calloc   = calloc,
  .realloc  = realloc,
  .free     = free,
#ifndef _WIN32
  .valloc   = valloc,
  .memalign = posix_memalign,
#endif
#endif
  .strdup   = ak__heap_strdup_def
};

static AkHeap ak__heap = {
  .allocator  = &ak__allocator,
  .srchctx    = NULL,
  .root       = NULL,
  .trash      = NULL,
  .flags      = 0
};

static
int
ak__heap_srch_cmp(void * __restrict key1,
                  void * __restrict key2) {
  return strcmp((char *)key1, (char *)key2);
}

static
void
ak__heap_srch_print(void * __restrict key) {
  printf("\t%s\n", (const char *)key);
}

static
char*
ak__heap_strdup_def(const char * str) {
  void  *memptr;
  size_t memsize;

  memsize = strlen(str);
  memptr  = ak__heap.allocator->malloc(memsize + 1);
  memcpy(memptr, str, memsize);

  /* NULL */
  memset((char *)memptr + memsize, '\0', 1);

  return memptr;
}

AK_EXPORT
char*
ak_heap_strdup(AkHeap * __restrict heap,
               void * __restrict parent,
               const char * str) {
  void  *memptr;
  size_t memsize;

  memsize = strlen(str);
  memptr  = ak_heap_alloc(heap, parent, memsize + 1, false);
  memcpy(memptr, str, memsize);

  /* NULL */
  memset((char *)memptr + memsize, '\0', 1);

  return memptr;
}

AK_EXPORT
char*
ak_heap_strndup(AkHeap * __restrict heap,
                void * __restrict parent,
                const char * str,
                size_t size) {
  void  *memptr;

  memptr  = ak_heap_alloc(heap, parent, size + 1, false);
  memcpy(memptr, str, size);

  /* NULL */
  memset((char *)memptr + size, '\0', 1);

  return memptr;
}

AK_EXPORT
AkHeapAllocator *
ak_heap_allocator(AkHeap * __restrict heap) {
  return heap->allocator;
}

AK_EXPORT
AkHeap *
ak_heap_getheap(void * __restrict memptr) {
  AkHeapNode   *heapNode;
  heapNode = ak__alignof(memptr);
  return ak_heap_lt_find(heapNode->heapid);
}

AK_EXPORT
AkHeap *
ak_heap_new(AkHeapAllocator *allocator,
            AkHeapSrchCmpFn cmp,
            AkHeapSrchPrintFn print) {
  AkHeapAllocator *alc;
  AkHeap          *heap;

  alc = allocator ? allocator : &ak__allocator;

  heap = alc->malloc(sizeof(*heap));
  assert(heap && "malloc failed");

  heap->flags = AK_HEAP_FLAGS_DYNAMIC;
  ak_heap_init(heap, allocator, cmp, print);

  return heap;
}

AK_EXPORT
void
ak_heap_attach(AkHeap * __restrict parent,
               AkHeap * __restrict chld) {
  if (!parent->chld) {
    parent->chld = chld;
    return;
  }

  chld->next   = parent->chld;
  parent->chld = chld;
}

AK_EXPORT
void
ak_heap_dettach(AkHeap * __restrict parent,
                AkHeap * __restrict chld) {
  AkHeap *heap;

  heap = parent->chld;
  if (heap == chld) {
    parent->chld = chld->next;
    return;
  }

  while (heap) {
    if (heap->next == chld) {
      heap->next = chld->next;
      break;
    }

    heap = heap->next;
  }
}

AK_EXPORT
AkResult
ak_heap_setdata(AkHeap * __restrict heap,
                void * __restrict memptr) {

  heap->data = memptr;
  return AK_OK;
}

AK_EXPORT
void*
ak_heap_data(AkHeap * __restrict heap) {
  return heap->data;
}

AK_EXPORT
void
ak_heap_init(AkHeap * __restrict heap,
             AkHeapAllocator *allocator,
             AkHeapSrchCmpFn cmp,
             AkHeapSrchPrintFn print) {
  AkHeapAllocator *alc;
  AkHeapSrchCtx   *srchctx;
  AkHeapNode      *rootNode,     *nullNode;
  AkHeapNodeExt   *rootNodeExt,  *nullNodeExt;
  AkHeapSrchNode  *srchRootNode, *srchNullNode;
  size_t           srchNodeSize;

  if (heap->flags & AK_HEAP_FLAGS_INITIALIZED)
    return;

  alc = allocator ? allocator : &ak__allocator;

  srchctx = alc->malloc(sizeof(*srchctx));
  assert(srchctx && "malloc failed");

  srchctx->cmp   = cmp ? cmp : ak__heap_srch_cmp;
  srchctx->print = print ? print : ak__heap_srch_print;

  srchNodeSize = sizeof(AkHeapSrchNode);

  rootNode    = alc->calloc(ak__heapnd_sz, 1);
  nullNode    = alc->calloc(ak__heapnd_sz, 1);
  rootNodeExt = alc->calloc(sizeof(*rootNodeExt) + srchNodeSize, 1);
  nullNodeExt = alc->calloc(sizeof(*nullNodeExt) + srchNodeSize, 1);

  assert(rootNode
         && nullNode
         && rootNodeExt
         && nullNodeExt
         && "malloc failed");

  rootNode->chld    = rootNodeExt;
  nullNode->chld    = nullNodeExt;
  rootNodeExt->node = rootNode;
  nullNodeExt->node = nullNode;
  srchRootNode      = (AkHeapSrchNode *)rootNodeExt->data;
  srchNullNode      = (AkHeapSrchNode *)nullNodeExt->data;

  srchNullNode->key = ak__emptystr;
  srchNullNode->chld[AK__BST_LEFT]  = srchNullNode;
  srchNullNode->chld[AK__BST_RIGHT] = srchNullNode;

  srchRootNode->key = ak__emptystr;
  srchRootNode->chld[AK__BST_LEFT]  = srchNullNode;
  srchRootNode->chld[AK__BST_RIGHT] = srchNullNode;

  rootNode->flags   = (AK_HEAP_NODE_FLAGS_EXT | AK_HEAP_NODE_FLAGS_SRCH);
  nullNode->flags   = (AK_HEAP_NODE_FLAGS_EXT | AK_HEAP_NODE_FLAGS_SRCH);

  /* Real Root is srchRoot-right node */
  srchctx->root     = srchRootNode;
  srchctx->nullNode = srchNullNode;

  heap->chld      = NULL;
  heap->next      = NULL;
  heap->root      = NULL;
  heap->trash     = NULL;
  heap->allocator = alc;
  heap->srchctx   = srchctx;
  heap->flags    |= AK_HEAP_FLAGS_INITIALIZED;

  if (heap != &ak__heap)
    ak_heap_lt_insert(heap);
}

AK_EXPORT
void
ak_heap_destroy(AkHeap * __restrict heap) {
  AkHeapAllocator *alc;

  if (!(heap->flags & AK_HEAP_FLAGS_INITIALIZED))
    return;

  alc = heap->allocator;

  /* first destroy all attached heaps */
  if (heap->chld) {
    AkHeap *aheap;
    aheap = heap->chld;

    do {
      ak_heap_destroy(aheap);
      aheap = aheap->next;
    } while (aheap);
  }

  ak_heap_cleanup(heap);

  alc->free(AK__HEAPNODE(heap->srchctx->root));
  alc->free(AK__HEAPNODE(heap->srchctx->nullNode));
  alc->free(AK__HEAPEXTNODE(heap->srchctx->root));
  alc->free(AK__HEAPEXTNODE(heap->srchctx->nullNode));
  alc->free(heap->srchctx);

  heap->data = NULL;
  ak_heap_lt_remove(heap->heapid);

  if (heap->flags & AK_HEAP_FLAGS_DYNAMIC
      && heap != &ak__heap)
    alc->free(heap);
}

AK_EXPORT
void*
ak_heap_alloc(AkHeap * __restrict heap,
              void * __restrict parent,
              size_t size,
              bool srch) {
  AkHeapNode *currNode;
  AkHeapNode *parentNode;
  size_t      memsize;

  assert((!parent || heap->heapid == ak__alignof(parent)->heapid)
         && "parent and child mem must use same heap");

  memsize = ak__heapnd_sz + size;
  memsize = ak__align(memsize);
  currNode = heap->allocator->malloc(memsize);
  assert(currNode && "malloc failed");

  currNode->flags  = 0;
  currNode->heapid = heap->heapid;
  currNode->chld   = NULL;

  if (parent) {
    AkHeapNode *chldNode;

    parentNode = ak__alignof(parent);
    chldNode   = ak_heap_chld(parentNode);

    ak_heap_chld_set(parentNode, currNode);
    if (chldNode) {
      chldNode->prev = currNode;
      currNode->next = chldNode;
    } else {
      currNode->next = NULL;
    }

    currNode->prev = parentNode;
  } else {
    if (heap->root) {
      heap->root->prev = currNode;
      currNode->next   = heap->root;
    } else {
      currNode->next = NULL;
    }

    heap->root = currNode;
    currNode->prev = NULL;
  }

  return ak__alignas(currNode);
}

AK_EXPORT
void*
ak_heap_calloc(AkHeap * __restrict heap,
               void * __restrict parent,
               size_t size,
               bool srch) {
  void  *memptr;

  memptr = ak_heap_alloc(heap,
                         parent,
                         size,
                         srch);
  memset(memptr, '\0', size);

  return memptr;
}

AK_EXPORT
void*
ak_heap_realloc(AkHeap * __restrict heap,
                void * __restrict parent,
                void * __restrict memptr,
                size_t newsize) {
  AkHeapNode *oldNode;
  AkHeapNode *newNode;
  AkHeapNode *chld;

  if (!memptr)
    return ak_heap_alloc(heap,
                         parent,
                         newsize,
                         false);

  oldNode = ak__alignof(memptr);

  if (newsize == 0) {
    ak_heap_free(heap, oldNode);
    return NULL;
  }

  newNode = heap->allocator->realloc(oldNode,
                                     ak__heapnd_sz + newsize);
  assert(newNode && "realloc failed");

  if (heap->root == oldNode)
    heap->root = newNode;

  if (heap->trash == oldNode)
    heap->trash = newNode;

  chld = ak_heap_chld(newNode);
  if (chld)
    chld->prev = newNode;

  if (newNode->next)
    newNode->next->prev = newNode;

  if (newNode->prev) {
    chld = ak_heap_chld(newNode->prev);

    if (chld == oldNode)
      ak_heap_chld_set(newNode->prev, newNode);

    if (newNode->prev->next == oldNode)
      newNode->prev->next = newNode;
  }

  return ak__alignas(newNode);
}

void *
ak_heap_chld(AkHeapNode *heapNode) {
  if (heapNode->flags & AK_HEAP_NODE_FLAGS_EXT)
    return ((AkHeapNodeExt *)heapNode->chld)->chld;

  return heapNode->chld;
}

void
ak_heap_chld_set(AkHeapNode * __restrict heapNode,
                 AkHeapNode * __restrict chldNode) {
  if (heapNode->flags & AK_HEAP_NODE_FLAGS_EXT)
    ((AkHeapNodeExt *)heapNode->chld)->chld = chldNode;
  else
    heapNode->chld = chldNode;
}

AK_EXPORT
void
ak_heap_setp(AkHeap * __restrict heap,
             AkHeapNode * __restrict heapNode,
             AkHeapNode * __restrict newParent) {
  AkHeapNode *parentChld;

  if (heapNode->prev) {
    if (heapNode->prev->next == heapNode)
      heapNode->prev->next = heapNode->next;
    else if (ak_heap_chld(heapNode->prev) == heapNode)
      ak_heap_chld_set(heapNode->prev, heapNode->next);

    if (heapNode->next)
      heapNode->next->prev = heapNode->prev;
  }

  if (heapNode == heap->root)
    heap->root = heapNode->next;

  parentChld = ak_heap_chld(newParent);
  if (parentChld) {
    parentChld->prev = heapNode;
    heapNode->next = parentChld;
  }

  ak_heap_chld_set(newParent, heapNode);

  /* move all ids to new heap (if it is different) */
  if (newParent->heapid != heapNode->heapid)
    ak_heap_moveh(heap,
                  ak_heap_getheap(newParent),
                  heapNode);
}

AK_EXPORT
void
ak_heap_moveh(AkHeap * __restrict heap,
              AkHeap * __restrict newheap,
              AkHeapNode * __restrict heapNode) {
  do {
    if (heapNode->flags &
        (AK_HEAP_NODE_FLAGS_EXT | AK_HEAP_NODE_FLAGS_SRCH)) {
      AkHeapSrchNode *srchNode;
      srchNode = (AkHeapSrchNode *)((AkHeapNodeExt *)heapNode->chld)->data;

      ak_heap_rb_remove(heap->srchctx, srchNode);
      ak_heap_rb_insert(newheap->srchctx, srchNode);
    }

    heapNode->heapid = newheap->heapid;
    heapNode         = ak_heap_chld(heapNode);

    /* TODO: get rid of recursive call like free */
    if (heapNode) {
      ak_heap_moveh(heap, newheap, heapNode);
      heapNode = heapNode->next;
    }
  } while (heapNode);
}

AK_EXPORT
void
ak_heap_setpm(AkHeap * __restrict heap,
              void * __restrict memptr,
              void * __restrict newparent) {
  ak_heap_setp(heap,
               ak__alignof(memptr),
               ak__alignof(newparent));
}

void
ak_heap_free_ext(AkHeap * __restrict heap,
                 AkHeapNode * __restrict heapNode) {
  AkHeapNodeExt *extNode;

  if (!(heapNode->flags & AK_HEAP_NODE_FLAGS_EXT))
    return;

  extNode = heapNode->chld;

  if (heapNode->flags & AK_HEAP_NODE_FLAGS_SRCH) {
    AkHeapSrchNode *srchNode;
    srchNode = (AkHeapSrchNode *)extNode->data;

    /* remove it from rb tree */
    if (srchNode->key != ak__emptystr)
      ak_heap_rb_remove(heap->srchctx, srchNode);

    heapNode->flags &= ~(AK_HEAP_NODE_FLAGS_SRCH | AK_HEAP_NODE_FLAGS_RED);
  }

  if (heapNode->flags & AK_HEAP_NODE_FLAGS_SID) {
    /* TODO: */
  }

  heapNode->chld = extNode->chld;
  heap->allocator->free(extNode);
  heapNode->flags &= ~AK_HEAP_NODE_FLAGS_EXT;
}

AK_EXPORT
void
ak_heap_free(AkHeap * __restrict heap,
             AkHeapNode * __restrict heapNode) {
  AkHeapAllocator * __restrict allocator;
  allocator = heap->allocator;

  if (heapNode->flags & AK_HEAP_NODE_FLAGS_EXT)
    ak_heap_free_ext(heap, heapNode);

  /* free all child nodes */
  if (heapNode->chld) {
    AkHeapNode *toFree;
    AkHeapNode *nextFree;

    toFree = heapNode->chld;

    do {
      nextFree = toFree->next;
      if (nextFree)
        nextFree->prev = NULL;

      if (heap->trash == toFree)
        heap->trash = nextFree;

      if (heapNode->flags & AK_HEAP_NODE_FLAGS_EXT)
        ak_heap_free_ext(heap, toFree);

      if (toFree->chld) {
        if (heap->trash) {
          AkHeapNode *lastNode;

          lastNode = toFree->chld;
          while (lastNode->next)
            lastNode = lastNode->next;

          heap->trash->prev = lastNode;
          lastNode->next = heap->trash;
        }

        heap->trash = toFree->chld;
        ((AkHeapNode *)toFree->chld)->prev = NULL;
      }

      /* connect prev and next */
      if (toFree->prev) {
        toFree->prev->next = nextFree;

        if (nextFree)
          nextFree->prev = toFree->prev;
      }

      allocator->free(toFree);
      toFree = nextFree;

      /* empty trash */
      if (!toFree && heap->trash) {
        toFree = heap->trash;
        heap->trash = nextFree;
      }

    } while (toFree);

    heapNode->chld = NULL;
  }

  if (heapNode->prev)
    heapNode->prev->next = heapNode->next;

  if (heap->root == heapNode)
    heap->root = heapNode->next;

  heapNode->next = NULL;
  heapNode->prev = NULL;

  allocator->free(heapNode);
}

AK_EXPORT
void
ak_heap_cleanup(AkHeap * __restrict heap) {
  while (heap->root)
    ak_heap_free(heap, heap->root);
}

AK_EXPORT
void *
ak_heap_getId(AkHeap * __restrict heap,
              AkHeapNode * __restrict heapNode) {
  AkHeapSrchNode *snode;

  if (!(heapNode->flags & AK_HEAP_NODE_FLAGS_EXT)
      || !(heapNode->flags & AK_HEAP_NODE_FLAGS_SRCH))
    return NULL;

  snode = (AkHeapSrchNode *)((AkHeapNodeExt *)heapNode->chld)->data;
  return snode->key;

  AK__UNUSED(heap);
}

void
ak_heap_mk_srchnode(AkHeap * __restrict heap,
                    AkHeapNode * __restrict heapNode,
                    void * __restrict memId) {
  AkHeapAllocator *alc;
  AkHeapNodeExt   *extNode;
  AkHeapSrchNode  *snode;
  AkHeapNodeExt   *extNodeR;
  AkSIDNode       *sidNode;
  size_t           snodeSize;

  alc       = heap->allocator;
  snodeSize = sizeof(AkHeapSrchNode);

  if (!(heapNode->flags & AK_HEAP_NODE_FLAGS_EXT)) {
    extNode        = alc->malloc(sizeof(*extNode) + snodeSize);
    extNode->node  = heapNode;
    extNode->chld  = heapNode->chld;
    heapNode->chld = extNode;

    snode = (AkHeapSrchNode *)extNode->data;
    goto done;
  }

  extNode = heapNode->chld;

  if (heapNode->flags & AK_HEAP_NODE_FLAGS_SRCH) {
    snode = (AkHeapSrchNode *)extNode->data;
    ak_heap_rb_remove(heap->srchctx, snode);
    goto done;
  }

  sidNode  = (AkSIDNode *)(extNode->data + 1);
  extNodeR = alc->realloc(extNode,
                          sizeof(*extNodeR)
                          + snodeSize
                          + sizeof(AkSIDNode)
                          + 1);

  if (heapNode->flags & AK_HEAP_NODE_FLAGS_SID) {
    if (sidNode->prev) { sidNode->prev       += snodeSize; }
    if (sidNode->next) { sidNode->next->prev += snodeSize; }
    if (sidNode->chld) { sidNode->chld->prev += snodeSize; }

    memmove(extNodeR + snodeSize,
            extNodeR,
            sizeof(AkSIDNode) + 1);
    extNodeR->data[snodeSize] = 1;

    /* TODO: update refs */
    heapNode->chld = extNode;
  }

  snode = (AkHeapSrchNode *)extNodeR->data;

done:
  heapNode->flags |= (AK_HEAP_NODE_FLAGS_EXT
                      | AK_HEAP_NODE_FLAGS_SRCH
                      | AK_HEAP_NODE_FLAGS_RED);

  snode->chld[AK__BST_LEFT]  = heap->srchctx->nullNode;
  snode->chld[AK__BST_RIGHT] = heap->srchctx->nullNode;
  snode->key                 = memId;

  ak_heap_rb_insert(heap->srchctx, snode);
}

void
ak_heap_rm_srchnode(AkHeap * __restrict heap,
                    AkHeapNode * __restrict heapNode) {
  AkHeapAllocator *alc;
  AkHeapNodeExt   *extNode;
  AkHeapNodeExt   *extNodeR;
  AkSIDNode       *sidNode;
  size_t           snodeSize;

  alc       = heap->allocator;
  snodeSize = sizeof(AkHeapSrchNode);

  if (!(heapNode->flags
        & (AK_HEAP_NODE_FLAGS_EXT | AK_HEAP_NODE_FLAGS_SRCH))) {
    heapNode->flags &= ~AK_HEAP_NODE_FLAGS_RED;
    return;
  }

  extNode = heapNode->chld;
  ak_heap_rb_remove(heap->srchctx,
                    (AkHeapSrchNode *)extNode->data);

  if (!(heapNode->flags & AK_HEAP_NODE_FLAGS_SID)) {
    heapNode->chld = extNode->chld;
    heapNode->flags &= ~(AK_HEAP_NODE_FLAGS_SRCH
                         | AK_HEAP_NODE_FLAGS_EXT
                         | AK_HEAP_NODE_FLAGS_RED);
    alc->free(extNode);
    return;
  }

  sidNode  = (AkSIDNode *)(extNode->data + snodeSize + 1);
  extNodeR = alc->realloc(extNode,
                          sizeof(*extNodeR)
                          + sizeof(AkSIDNode)
                          + 1);

  if (heapNode->flags & AK_HEAP_NODE_FLAGS_SID) {
    if (sidNode->prev) { sidNode->prev       -= snodeSize; }
    if (sidNode->next) { sidNode->next->prev -= snodeSize; }
    if (sidNode->chld) { sidNode->chld->prev -= snodeSize; }

    memmove(extNodeR - snodeSize,
            extNodeR,
            sizeof(AkSIDNode) + 1);

    extNodeR->data[0] = 0;

    /* TODO: update refs */
    heapNode->chld = extNodeR;
  }

  heapNode->flags &= ~(AK_HEAP_NODE_FLAGS_SRCH | AK_HEAP_NODE_FLAGS_RED);
}

AK_EXPORT
void
ak_heap_setId(AkHeap * __restrict heap,
              AkHeapNode * __restrict heapNode,
              void * __restrict memId) {
  if (!memId) {
    ak_heap_rm_srchnode(heap, heapNode);
    return;
  }

  ak_heap_mk_srchnode(heap, heapNode, memId);
}

AK_EXPORT
AkResult
ak_heap_getMemById(AkHeap * __restrict heap,
                   void * __restrict memId,
                   void ** __restrict dest) {
  AkHeapNode   *heapNode;
  AkHeapSrchNode *srchNode;

  srchNode = ak_heap_rb_find(heap->srchctx, memId);
  if (!srchNode || srchNode == heap->srchctx->nullNode) {
    *dest = NULL;
    return AK_EFOUND;
  }

  heapNode = AK__HEAPNODE(srchNode);
  *dest = ak__alignas(heapNode);

  if (!*dest)
    return AK_EFOUND;

  return AK_OK;
}

AK_EXPORT
void
ak_heap_printKeys(AkHeap * __restrict heap) {
  ak_heap_rb_print(heap->srchctx);
}

AK_EXPORT
AkHeapAllocator *
ak_mem_allocator() {
  return ak__heap.allocator;
}

AK_EXPORT
void
ak_mem_printKeys() {
  ak_heap_rb_print(ak__heap.srchctx);
}

AK_EXPORT
void *
ak_mem_getId(void * __restrict memptr) {
  AkHeap     *heap;
  AkHeapNode *heapNode;

  heapNode = ak__alignof(memptr);
  if (heapNode->heapid == 0)
    heap = &ak__heap;
  else
    heap = ak_heap_lt_find(heapNode->heapid);

  return ak_heap_getId(heap, heapNode);
}

AK_EXPORT
void
ak_mem_setId(void * __restrict memptr,
             void * __restrict memId) {
  AkHeap     *heap;
  AkHeapNode *heapNode;
  
  heapNode = ak__alignof(memptr);
  if (heapNode->heapid == 0)
    heap = &ak__heap;
  else
    heap = ak_heap_lt_find(heapNode->heapid);

  ak_heap_setId(heap,
                heapNode,
                memId);
}

AK_EXPORT
AkResult
ak_mem_getMemById(void * __restrict ctx,
                  void * __restrict memId,
                  void ** __restrict dest) {
  AkHeap     *heap;
  AkHeapNode *heapNode;

  heapNode = ak__alignof(ctx);
  if (heapNode->heapid == 0)
    heap = &ak__heap;
  else
    heap = ak_heap_lt_find(heapNode->heapid);

  return ak_heap_getMemById(heap,
                            memId,
                            dest);
}

AK_EXPORT
void
ak_mem_setp(void * __restrict memptr,
            void * __restrict newparent) {
  ak_heap_setp(&ak__heap,
                ak__alignof(memptr),
                ak__alignof(newparent));
}

AK_EXPORT
void*
ak_malloc(void * __restrict parent,
          size_t size,
          bool srch) {
  return ak_heap_alloc(&ak__heap,
                       parent,
                       size,
                       srch);
}

AK_EXPORT
void*
ak_calloc(void * __restrict parent,
          size_t size,
          bool srch) {
  void  *memptr;

  memptr = ak_heap_alloc(&ak__heap,
                         parent,
                         size,
                         srch);
  memset(memptr, '\0', size);

  return memptr;
}

AK_EXPORT
void*
ak_realloc(void * __restrict parent,
           void * __restrict memptr,
           size_t newsize) {
  return ak_heap_realloc(&ak__heap,
                          parent,
                          memptr,
                          newsize);
}

AK_EXPORT
char*
ak_strdup(void * __restrict parent,
          const char * __restrict str) {
  void  *memptr;
  size_t memsize;

  memsize = strlen(str);
  memptr  = ak_heap_alloc(&ak__heap,
                          parent,
                          memsize + 1,
                          false);

  memcpy(memptr, str, memsize);

  /* NULL */
  memset((char *)memptr + memsize, '\0', 1);

  return memptr;
}

AK_EXPORT
void
ak_free(void * __restrict memptr) {
  AkHeap     *heap;
  AkHeapNode *heapNode;

  heap     = &ak__heap;
  heapNode = ak__alignof(memptr);

  if (heapNode->heapid != heap->heapid)
    heap = ak_heap_lt_find(heapNode->heapid);

  if (!heap)
    return;

  if (heap->data == memptr)
    ak_heap_destroy(heap);
  else
    ak_heap_free(heap, heapNode);
}

AK_EXPORT
AkObject*
ak_objAlloc(AkHeap * __restrict heap,
void * __restrict memParent,
            size_t typeSize,
            AkEnum typeEnum,
            bool zeroed,
            bool srch) {
  AkObject * obj;

  assert(typeSize > 0 && "invalid parameter value");

  obj = ak_heap_alloc(heap,
                      memParent,
                      sizeof(*obj) + typeSize,
                      srch);

  obj->size  = typeSize;
  obj->type  = typeEnum;
  obj->next  = NULL;
  obj->pData = (void *)((char *)obj + offsetof(AkObject, data));

  if (zeroed)
    memset(obj->pData, '\0', typeSize);

  return obj;
}

AK_EXPORT
AkObject*
ak_objFrom(void * __restrict memptr) {
  AkObject *obj;
  obj = (void *)((char *)memptr - offsetof(AkObject, data));
  assert(obj->pData == memptr && "invalid cas to AkObject");
  return obj;
}

void
ak_mem_init() {
  ak_heap_init(&ak__heap, NULL, NULL, NULL);
  ak_heap_lt_init(&ak__heap);
}

void
ak_mem_deinit() {
  ak_heap_destroy(&ak__heap);
  ak_heap_lt_cleanup();
}
