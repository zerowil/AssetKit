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

#include "img.h"
#include "../core/asset.h"
#include "../1.4/image.h"
#include "../core/enum.h"

static
AkInitFrom*
dae_initFrom(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp);

static
AkImageFormat*
dae_imageFormat(DAEState * __restrict dst,
                xml_t    * __restrict xml,
                void     * __restrict memp);

static
AkImage2d*
dae_create2d(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp);

static
AkImage3d*
dae_create3d(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp);

static
AkImageCube*
dae_createCube(DAEState * __restrict dst,
               xml_t    * __restrict xml,
               void     * __restrict memp);

_assetkit_hide
void*
dae_image(DAEState * __restrict dst,
          xml_t    * __restrict xml,
          void     * __restrict memp) {
  AkHeap     *heap;
  AkImage    *img;
  xml_attr_t *att;

  if (dst->version < AK_COLLADA_VERSION_150) {
    dae14_fxMigrateImg(dst, xml, memp);
    return NULL;
  }

  heap = dst->heap;
  img  = ak_heap_calloc(heap, memp, sizeof(*img));

  xmla_setid(xml, heap, img);
  sid_set(xml, heap, img);
  
  img->name = xmla_strdup_by(xml, heap, _s_dae_name, img);

  xml = xml->val;
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_asset)) {
      (void)dae_asset(dst, xml, img, NULL);
    } else if (xml_tag_eq(xml, _s_dae_renderable)) {
      img->renderable = (att = xmla(xml, _s_dae_share))
                          && att->val
                          && strcasecmp(att->val, _s_dae_true) == 0;
    } else if (xml_tag_eq(xml, _s_dae_init_from)) {
      img->initFrom = dae_initFrom(dst, xml, img);
    } else if (xml_tag_eq(xml, _s_dae_create_2d)) {
      AkImage2d *image2d;
      if ((image2d = dae_create2d(dst, xml, img)))
          img->image = &image2d->base;
    } else if (xml_tag_eq(xml, _s_dae_create_3d)) {
      AkImage3d *image3d;
      if ((image3d = dae_create3d(dst, xml, img)))
          img->image = &image3d->base;
    } else if (xml_tag_eq(xml, _s_dae_create_cube)) {
      AkImageCube *imageCube;
      if ((imageCube = dae_createCube(dst, xml, img)))
          img->image = &imageCube->base;
    } else if (xml_tag_eq(xml, _s_dae_extra)) {
      img->extra = tree_fromxml(heap, img, xml);
    }
    xml = xml->next;
  }

  return img;
}

_assetkit_hide
AkInstanceBase*
dae_instImage(DAEState * __restrict dst,
              xml_t    * __restrict xml,
              void     * __restrict memp) {
  AkHeap         *heap;
  AkInstanceBase *instImg;

  heap          = dst->heap;
  instImg       = ak_heap_calloc(heap, memp, sizeof(*instImg));
  instImg->name = xmla_strdup_by(xml, heap, _s_dae_name, instImg);
  
  sid_set(xml, heap, instImg);
  url_set(dst, xml, _s_dae_url, instImg, &instImg->url);

  return instImg;
}

static
AkInitFrom*
dae_initFrom(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp) {
  AkHeap     *heap;
  AkInitFrom *initFrom;
  AkHexData  *hex;
  xml_attr_t *att;
  
  heap     = dst->heap;
  initFrom = ak_heap_calloc(heap, memp, sizeof(*initFrom));
  
  initFrom->mipsGenerate = xmla_u32(xmla(xml, _s_dae_mips_generate), 0);
  initFrom->arrayIndex   = xmla_u32(xmla(xml, _s_dae_array_index), 0);
  initFrom->mipIndex     = xmla_u32(xmla(xml, _s_dae_mip_index), 0);
  initFrom->depth        = xmla_u32(xmla(xml, _s_dae_depth), 0);

  if ((att = xmla(xml, _s_dae_face)) && att->val)
    initFrom->face = dae_face(att);

  xml = xml->val;
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_ref)) {
      initFrom->ref = xml_strdup(xml, heap, initFrom);
    } else if (xml_tag_eq(xml, _s_dae_hex)) {
      hex         = ak_heap_calloc(heap, initFrom, sizeof(*hex));
      hex->format = xmla_strdup(xmla(xml, _s_dae_format), heap, hex);

      if (hex->format) {
        hex->hexval = xml_strdup(xml, heap, initFrom);
        initFrom->hex = hex;
      } else {
        ak_free(hex);
      }
    }
    xml = xml->next;
  }
  
  return initFrom;
}

static
AkImageFormat*
dae_imageFormat(DAEState * __restrict dst,
                xml_t    * __restrict xml,
                void     * __restrict memp) {
  AkHeap        *heap;
  AkImageFormat *format;
  xml_attr_t    *att;

  heap   = dst->heap;
  format = ak_heap_calloc(heap, memp, sizeof(*format));

  xml = xml->val;
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_hint)) {
      if ((att = xmla(xml, _s_dae_channels)) && att->val)
        format->channel = dae_enumChannel(att->val, att->valsize);
      
      if ((att = xmla(xml, _s_dae_range)) && att->val)
        format->range = dae_range(att->val, att->valsize);
      
      if ((att = xmla(xml, _s_dae_precision)) && att->val)
        format->precision = dae_precision(att->val, att->valsize);
      
      if ((att = xmla(xml, _s_dae_space)) && att->val)
        format->space = xmla_strdup(att, heap, format);
    } else if (xml_tag_eq(xml, _s_dae_exact)) {
      format->exact = xml_strdup(xml, heap, format);
    }
    xml = xml->next;
  }

  return format;
}

static
AkImage2d*
dae_create2d(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp) {
  AkHeap    *heap;
  AkImage2d *img;

  heap = dst->heap;
  img  = ak_heap_calloc(heap, memp, sizeof(*img));

  img->base.type = AK_IMAGE_TYPE_2D;

  xml = xml->val;
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_size_exact)) {
      AkSizeExact *sizeExact;
      
      sizeExact = ak_heap_calloc(heap, img, sizeof(*sizeExact));
      
      sizeExact->width  = xmla_u32(xmla(xml, _s_dae_width), 0);
      sizeExact->height = xmla_u32(xmla(xml, _s_dae_height), 0);
      img->sizeExact    = sizeExact;
    } else if (xml_tag_eq(xml, _s_dae_size_ratio)) {
      AkSizeRatio *sizeRatio;
      
      sizeRatio = ak_heap_calloc(heap, img, sizeof(*sizeRatio));
      
      sizeRatio->width  = xmla_float(xmla(xml, _s_dae_width), 0);
      sizeRatio->height = xmla_float(xmla(xml, _s_dae_height), 0);
      img->sizeRatio    = sizeRatio;
    } else if (xml_tag_eq(xml, _s_dae_mips)) {
      AkMips *mips;
      
      mips = ak_heap_calloc(heap, img, sizeof(*mips));
      
      mips->levels       = xmla_u32(xmla(xml, _s_dae_levels), 0);
      mips->autoGenerate = xmla_u32(xmla(xml, _s_dae_auto_generate), 0);
      img->mips          = mips;
    } else if (xml_tag_eq(xml, _s_dae_unnormalized)) {
      img->unnormalized = xml_strdup(xml, heap, img);
    } else if (xml_tag_eq(xml, _s_dae_array)) {
      img->base.arrayLen = xmla_u32(xmla(xml, _s_dae_length), 0);
    } else if (xml_tag_eq(xml, _s_dae_format)) {
      img->base.format = dae_imageFormat(dst, xml, img);
    } else if (xml_tag_eq(xml, _s_dae_size_exact)) {
      img->base.initFrom = dae_initFrom(dst, xml, img);
    }
    xml = xml->next;
  }

  return img;
}

static
AkImage3d*
dae_create3d(DAEState * __restrict dst,
             xml_t    * __restrict xml,
             void     * __restrict memp) {
  AkHeap    *heap;
  AkImage3d *img;

  heap = dst->heap;
  img  = ak_heap_calloc(heap, memp, sizeof(*img));
  
  img->base.type = AK_IMAGE_TYPE_3D;

  xml = xml->val;
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_size)) {
      img->size.width  = xmla_u32(xmla(xml, _s_dae_width),  0);
      img->size.height = xmla_u32(xmla(xml, _s_dae_height), 0);
      img->size.depth  = xmla_u32(xmla(xml, _s_dae_depth),  0);
    } else if (xml_tag_eq(xml, _s_dae_mips)) {
      img->mips.levels       = xmla_u32(xmla(xml, _s_dae_levels), 0);
      img->mips.autoGenerate = xmla_u32(xmla(xml, _s_dae_auto_generate), 0);
    } else if (xml_tag_eq(xml, _s_dae_array)) {
      img->base.arrayLen = xmla_u32(xmla(xml, _s_dae_length), 0);
    } else if (xml_tag_eq(xml, _s_dae_format)) {
      img->base.format = dae_imageFormat(dst, xml, img);
    } else if (xml_tag_eq(xml, _s_dae_size_exact)) {
      img->base.initFrom = dae_initFrom(dst, xml, img);
    }
    xml = xml->next;
  }

  return img;
}

static
AkImageCube*
dae_createCube(DAEState * __restrict dst,
               xml_t    * __restrict xml,
               void     * __restrict memp) {
  AkHeap      *heap;
  AkImageCube *img;
  
  heap = dst->heap;
  img  = ak_heap_calloc(heap, memp, sizeof(*img));
  
  img->base.type = AK_IMAGE_TYPE_CUBE;
  
  xml = xml->val;
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_size)) {
      img->width = xmla_u32(xmla(xml, _s_dae_width),  0);
    } else if (xml_tag_eq(xml, _s_dae_mips)) {
      img->mips.levels       = xmla_u32(xmla(xml, _s_dae_levels), 0);
      img->mips.autoGenerate = xmla_u32(xmla(xml, _s_dae_auto_generate), 0);
    } else if (xml_tag_eq(xml, _s_dae_array)) {
      img->base.arrayLen = xmla_u32(xmla(xml, _s_dae_length), 0);
    } else if (xml_tag_eq(xml, _s_dae_format)) {
      img->base.format = dae_imageFormat(dst, xml, img);
    } else if (xml_tag_eq(xml, _s_dae_size_exact)) {
      img->base.initFrom = dae_initFrom(dst, xml, img);
    }
    xml = xml->next;
  }

  return img;
}
