/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "aio_collada_camera.h"
#include "aio_collada_common.h"
#include "aio_collada_asset.h"
#include "aio_collada_technique.h"

int _assetio_hide
aio_dae_camera(xmlTextReaderPtr __restrict reader,
               aio_camera ** __restrict  dest) {
  aio_camera    *camera;
  const xmlChar *nodeName;
  int            nodeType;
  int            nodeRet;

  camera = aio_calloc(sizeof(*camera), 1);

  _xml_readAttr(camera->id, _s_dae_id);
  _xml_readAttr(camera->name, _s_dae_name);

  do {
    _xml_beginElement(_s_dae_camera);

    if (_xml_eqElm(_s_dae_asset)) {
      aio_assetinf *assetInf;
      int ret;

      assetInf = NULL;
      ret = aio_dae_assetInf(reader, &assetInf);
      if (ret == 0)
        camera->inf = assetInf;

    } else if (_xml_eqElm(_s_dae_optics)) {
      aio_optics           *optics;
      aio_technique        *last_tq;
      aio_technique_common *last_tc;

      optics = aio_calloc(sizeof(*optics), 1);

      last_tq = optics->technique;
      last_tc = optics->technique_common;

      do {
        _xml_beginElement(_s_dae_optics);

        if (_xml_eqElm(_s_dae_techniquec)) {
          aio_technique_common *tc;
          int                   ret;

          tc = NULL;
          ret = aio_dae_techniquec(reader, &tc);
          if (ret == 0) {
            optics->technique_common = tc;

            if (last_tc)
              last_tc->next = tc;
            else
              optics->technique_common = tc;

            last_tc = tc;
          }

        } else if (_xml_eqElm(_s_dae_technique)) {
          aio_technique *tq;
          int            ret;

          tq = NULL;
          ret = aio_dae_technique(reader, &tq);
          if (ret == 0) {
            optics->technique = tq;

            if (last_tq)
              last_tq->next = tq;
            else
              optics->technique = tq;

            last_tq = tq;
          }
        } else {
          _xml_skipElement;
        }

        /* end element */
        _xml_endElement;
      } while (nodeRet);

      camera->optics = optics;
    } else if (_xml_eqElm(_s_dae_imager)) {
      aio_imager    *imager;
      aio_technique *last_tq;

      imager  = aio_calloc(sizeof(*imager), 1);
      last_tq = imager->technique;

      do {
        _xml_beginElement(_s_dae_imager);

        if (_xml_eqElm(_s_dae_technique)) {
          aio_technique *tq;
          int            ret;

          tq = NULL;
          ret = aio_dae_technique(reader, &tq);
          if (ret == 0) {
            imager->technique = tq;

            if (last_tq)
              last_tq->next = tq;
            else
              imager->technique = tq;

            last_tq = tq;
          }
        } else if (_xml_eqElm(_s_dae_extra)) {
          xmlNodePtr nodePtr;
          aio_tree  *tree;

          nodePtr = xmlTextReaderExpand(reader);
          tree = NULL;

          aio_tree_fromXmlNode(nodePtr, &tree, NULL);
          imager->extra = tree;

          _xml_skipElement;
        } else {
          _xml_skipElement;
        }
        
        /* end element */
        _xml_endElement;
      } while (nodeRet);

      camera->imager = imager;
    } else if (_xml_eqElm(_s_dae_extra)) {
      xmlNodePtr nodePtr;
      aio_tree  *tree;

      nodePtr = xmlTextReaderExpand(reader);
      tree = NULL;

      aio_tree_fromXmlNode(nodePtr, &tree, NULL);
      camera->extra = tree;

      _xml_skipElement;
    }

    /* end element */
    _xml_endElement;
  } while (nodeRet);

  *dest = camera;

  return 0;
}
