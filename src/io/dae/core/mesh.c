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

#include "mesh.h"
#include "source.h"
#include "vert.h"
#include "triangle.h"
#include "poly.h"
#include "line.h"

AkObject* _assetkit_hide
dae_mesh(DAEState   * __restrict dst,
         xml_t      * __restrict xml,
         AkGeometry * __restrict geom) {
  AkVertices *vert;
  AkObject   *obj;
  AkMesh     *mesh;
  AkHeap     *heap;
  uint32_t    m;

  heap = dst->heap;
  xml  = xml->val;

  obj  = ak_objAlloc(heap, geom, sizeof(*mesh), AK_GEOMETRY_MESH, true);
  mesh = ak_objGet(obj);

  mesh->geom         = geom;
  mesh->convexHullOf = xmla_strdup_by(xml, heap, _s_dae_convex_hull_of, obj);

  vert = NULL;
  
  while (xml) {
    if (xml_tag_eq(xml, _s_dae_source)) {
      (void)dae_source(dst, xml, NULL, 0);
    } else if (xml_tag_eq(xml, _s_dae_vertices)) {
      vert = dae_vert(dst, xml, obj);
    } else if ((xml_tag_eq(xml, _s_dae_triangles) & (m = AK_TRIANGLES))
            || (xml_tag_eq(xml, _s_dae_trifans)   & (m = AK_TRIANGLE_FAN))
            || (xml_tag_eq(xml, _s_dae_tristrips) & (m = AK_TRIANGLE_STRIP))) {
      AkTriangles *tri;
      
      if ((tri = dae_triangles(dst, xml, obj, m))) {
        tri->base.next  = mesh->primitive;
        mesh->primitive = &tri->base;

        tri->base.mesh  = mesh;
        if (tri->base.bindmaterial)
          ak_meshSetMaterial(&tri->base, tri->base.bindmaterial);
        
        mesh->primitiveCount++;
      }
    } else if ((xml_tag_eq(xml, _s_dae_polygons) & (m = AK_POLY_POLYGONS))
            || (xml_tag_eq(xml, _s_dae_polylist) & (m = AK_POLY_POLYLIST))) {
      AkPolygon *poly;

      if ((poly = dae_poly(dst, xml, obj, m))) {
        poly->base.next = mesh->primitive;
        mesh->primitive = &poly->base;
      
        poly->base.mesh = mesh;
        if (poly->base.bindmaterial)
          ak_meshSetMaterial(&poly->base, poly->base.bindmaterial);
        
        mesh->primitiveCount++;
      }
      
    } else if (xml_tag_eq(xml, _s_dae_lines)      & (m = AK_LINES)
           || (xml_tag_eq(xml, _s_dae_linestrips) & (m = AK_LINE_STRIP))) {
      AkLines *lines;
      
      if ((lines = dae_lines(dst, xml, obj, m))) {
        lines->base.next = mesh->primitive;
        mesh->primitive  = &lines->base;

        lines->base.mesh = mesh;
        if (lines->base.bindmaterial)
          ak_meshSetMaterial(&lines->base, lines->base.bindmaterial);
        
        mesh->primitiveCount++;
      }
    } else if (xml_tag_eq(xml, _s_dae_extra)) {
      mesh->extra = tree_fromxml(heap, obj, xml);
    }

    xml = xml->next;
  }

  /* copy <vertices> to all primitives */
  if (vert) {
    AkMeshPrimitive *prim;
    AkInput         *inp;
    AkInput         *inpv;
    bool             setMeshInfo;
    
    prim        = mesh->primitive;
    setMeshInfo = false;
    
    while (prim) {
      inpv = vert->input;
      while (inpv) {
        AkURL *url;
        
        inp  = ak_heap_calloc(heap, prim, sizeof(*inp));
        inp->semantic = inpv->semantic;
        if (inpv->semanticRaw)
          inp->semanticRaw = ak_heap_strdup(heap, inp, inpv->semanticRaw);
        
        inp->offset = prim->reserved1;
        inp->set    = prim->reserved2;
        inp->next   = prim->input;
        prim->input = inp;
        
        if (inp->semantic == AK_INPUT_SEMANTIC_POSITION) {
          prim->pos = inp;
          if (!setMeshInfo) {
            AkDaeMeshInfo *mi;
            
            mi      = ak_heap_calloc(heap, NULL, sizeof(*mi));
            mi->pos = inp;
            
            rb_insert(dst->meshInfo, mesh, mi);
            
            setMeshInfo = true;
          }
        }
        
        if ((url = rb_find(dst->inputmap, inpv))) {
          ak_url_dup(url, inp, url);
          rb_insert(dst->inputmap, inp, url);
        }

        prim->inputCount++;
        inpv = inpv->next;
      }
      prim = prim->next;
    }
    
    /* dont keep vertices */
    inpv = vert->input;
    while (inpv) {
      inp = inpv;

      rb_remove(dst->inputmap, inpv);

      inpv = inpv->next;
      ak_free(inp);
    }
    ak_free(vert);
  }

  return obj;
}
