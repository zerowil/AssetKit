/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef ak_options_h
#define ak_options_h
#ifdef __cplusplus
extern "C" {
#endif

/* TODO: */
typedef enum AkOption {
  AK_OPT_INDICES_DEFAULT            = 0,  /* false    */
  AK_OPT_INDICES_SINGLE_INTERLEAVED = 1,  /* false    */
  AK_OPT_INDICES_SINGLE_SEPARATE    = 2,  /* false    */
  AK_OPT_INDICES_SINGLE             = 3,  /* false    */
  AK_OPT_NOINDEX_INTERLEAVED        = 4,  /* true     */
  AK_OPT_NOINDEX_SEPARATE           = 5,  /* true     */
  AK_OPT_COORD                      = 6,  /* Y_UP     */
  AK_OPT_USE_DOC_COORD              = 7,  /* true     */
  AK_OPT_DEFAULT_ID_PREFIX          = 8,  /* id-      */
  AK_OPT_COMPUTE_BBOX               = 9,  /* false    */
  AK_OPT_TRIANGULATE                = 10, /* true     */
  AK_OPT_GEN_NORMALS_IF_NEEDED      = 11, /* true     */
  AK_OPT_DEFAULT_PROFILE            = 12, /* COMMON   */
  AK_OPT_EFFECT_PROFILE             = 13, /* true     */
  AK_OPT_TECHNIQUE                  = 14, /* "common" */
  AK_OPT_TECHNIQUE_FX               = 15, /* "common" */
  AK_OPT_ZERO_INDEXED_INPUT         = 16, /* false    */
  AK_OPT_IMAGE_LOAD_FLIP_VERTICALLY = 17, /* true     */
  AK_OPT_ADD_DEFAULT_CAMERA         = 18, /* true     */
  AK_OPT_ADD_DEFAULT_LIGHT          = 19, /* true     */
  AK_OPT_COORD_CONVERT_TYPE         = 20  /* FIX_ROTATION */
} AkOption;

AK_EXPORT
void
ak_opt_set(AkOption option, uintptr_t value);

AK_EXPORT
uintptr_t
ak_opt_get(AkOption option);

AK_EXPORT
void
ak_opt_set_str(AkOption option, const char *value);

#ifdef __cplusplus
}
#endif
#endif /* ak_options_h */
