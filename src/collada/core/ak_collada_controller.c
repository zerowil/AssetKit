/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "ak_collada_controller.h"
#include "../ak_collada_asset.h"
#include "ak_collada_skin.h"
#include "ak_collada_morph.h"

AkResult _assetkit_hide
ak_dae_controller(void * __restrict memParent,
                  xmlTextReaderPtr reader,
                  AkController ** __restrict dest) {
  AkController   *controller;
  const xmlChar  *nodeName;
  int             nodeType;
  int             nodeRet;

  controller = ak_calloc(memParent, sizeof(*controller), 1);

  _xml_readAttr(controller, controller->id, _s_dae_id);
  _xml_readAttr(controller, controller->name, _s_dae_name);


  do {
    _xml_beginElement(_s_dae_controller);

    if (_xml_eqElm(_s_dae_asset)) {
      ak_assetinf *assetInf;
      AkResult ret;

      assetInf = NULL;
      ret = ak_dae_assetInf(controller, reader, &assetInf);
      if (ret == AK_OK)
        controller->inf = assetInf;

    } else if (_xml_eqElm(_s_dae_skin)) {
      AkSkin  *skin;
      AkResult ret;

      ret = ak_dae_skin(controller, reader, true, &skin);
      if (ret == AK_OK)
        controller->data = ak_objFrom(skin);

    } else if (_xml_eqElm(_s_dae_morph)) {
      AkMorph *morph;
      AkResult ret;

      ret = ak_dae_morph(controller, reader, true, &morph);
      if (ret == AK_OK)
        controller->data = ak_objFrom(morph);

    } else if (_xml_eqElm(_s_dae_extra)) {
      xmlNodePtr nodePtr;
      AkTree   *tree;

      nodePtr = xmlTextReaderExpand(reader);
      tree = NULL;

      ak_tree_fromXmlNode(controller, nodePtr, &tree, NULL);
      controller->extra = tree;

      _xml_skipElement;
    } else {
      _xml_skipElement;
    }
    
    /* end element */
    _xml_endElement;
  } while (nodeRet);
  
  *dest = controller;
  
  return AK_OK;
}