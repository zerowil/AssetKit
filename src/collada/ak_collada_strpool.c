/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef _ak_DAE_STRPOOL_
#define _ak_DAE_STRPOOL_
#endif

#include "ak_collada_strpool.h"
#include <string.h>

const char _s_dae_pool[] =
" \0"
"COLLADA\0"
"asset\0"
"contributor\0"
"author\0"
"author_email\0"
"author_website\0"
"authoring_tool\0"
"comments\0"
"copyright\0"
"source_data\0"
"created\0"
"modified\0"
"keywords\0"
"revision\0"
"subject\0"
"title\0"
"unit\0"
"name\0"
"meter\0"
"up_axis\0"
"X_UP\0"
"Z_UP\0"
"Y_UP\0"
"extra\0"
"library_cameras\0"
"id\0"
"camera\0"
"optics\0"
"technique\0"
"technique_common\0"
"profile\0"
"xmlns\0"
"perspective\0"
"xfov\0"
"yfov\0"
"sid\0"
"aspect_ratio\0"
"znear\0"
"zfar\0"
"orthographic\0"
"xmag\0"
"ymag\0"
"ambient\0"
"color\0"
"directional\0"
"point\0"
"constant_attenuation\0"
"linear_attenuation\0"
"quadratic_attenuation\0"
"spot\0"
"falloff_angle\0"
"falloff_exponent\0"
"imager\0"
"light\0"
"library_lights\0"
"library_effects\0"
"effect\0"
"annotate\0"
"newparam\0"
"profile_COMMON\0"
"profile_GLSL\0"
"profile_GLES2\0"
"profile_GLES\0"
"profile_CG\0"
"profile_BRIDGE\0"
"string\0"
"bool\0"
"bool2\0"
"bool3\0"
"bool4\0"
"int\0"
"int2\0"
"int3\0"
"int4\0"
"float\0"
"float2\0"
"float3\0"
"float4\0"
"float2x2\0"
"float3x3\0"
"float4x4\0"
"semantic\0"
"modifier\0"
"ref\0"
"type\0"
"CONST\0"
"UNIFORM\0"
"VARYING\0"
"STATIC\0"
"VOLATILE\0"
"EXTERN\0"
"SHARED\0"
"platform\0"
"platforms\0"
"language\0"
"url\0"
"code\0"
"include\0"
"pass\0"
"blinn\0"
"constant\0"
"lambert\0"
"phong\0"
"emission\0"
"diffuse\0"
"specular\0"
"shininess\0"
"reflective\0"
"reflectivity\0"
"transparent\0"
"transparency\0"
"index_of_refraction\0"
"opaque\0"
"texture\0"
"texcoord\0"
"param\0"
"states\0"
"program\0"
"evaluate\0"
"alpha_func\0"
"blend_func\0"
"blend_func_separate\0"
"blend_equation\0"
"blend_equation_separate\0"
"color_material\0"
"cull_face\0"
"depth_func\0"
"fog_mode\0"
"fog_coord_src\0"
"front_face\0"
"light_model_color_control\0"
"logic_op\0"
"polygon_mode\0"
"shade_model\0"
"stencil_func\0"
"stencil_op\0"
"stencil_func_separate\0"
"stencil_op_separate\0"
"stencil_mask_separate\0"
"light_enable\0"
"light_ambient\0"
"light_diffuse\0"
"light_specular\0"
"light_position\0"
"light_constant_attenuation\0"
"light_linear_attenuation\0"
"light_quadratic_attenuation\0"
"light_spot_cutoff\0"
"light_spot_direction\0"
"light_spot_exponent\0"
"texture1D\0"
"texture2D\0"
"texture3D\0"
"textureCUBE\0"
"textureRECT\0"
"textureDEPTH\0"
"texture1D_enable\0"
"texture2D_enable\0"
"texture3D_enable\0"
"textureCUBE_enable\0"
"textureRECT_enable\0"
"textureDEPTH_enable\0"
"texture_env_color\0"
"texture_env_mode\0"
"clip_plane\0"
"clip_plane_enable\0"
"blend_color\0"
"color_mask\0"
"depth_bounds\0"
"depth_mask\0"
"depth_range\0"
"fog_density\0"
"fog_start\0"
"fog_end\0"
"fog_color\0"
"light_model_ambient\0"
"lighting_enable\0"
"line_stipple\0"
"line_width\0"
"material_ambient\0"
"material_diffuse\0"
"material_emission\0"
"material_shininess\0"
"material_specular\0"
"model_view_matrix\0"
"point_distance_attenuation\0"
"point_fade_threshold_size\0"
"point_size\0"
"point_size_min\0"
"point_size_max\0"
"polygon_offset\0"
"projection_matrix\0"
"scissor\0"
"stencil_mask\0"
"alpha_test_enable\0"
"blend_enable\0"
"color_logic_op_enable\0"
"color_material_enable\0"
"cull_face_enable\0"
"depth_bounds_enable\0"
"depth_clamp_enable\0"
"depth_test_enable\0"
"dither_enable\0"
"fog_enable\0"
"light_model_local_viewer_enable\0"
"light_model_two_side_enable\0"
"line_smooth_enable\0"
"line_stipple_enable\0"
"logic_op_enable\0"
"multisample_enable\0"
"normalize_enable\0"
"point_smooth_enable\0"
"polygon_offset_fill_enable\0"
"polygon_offset_line_enable\0"
"polygon_offset_point_enable\0"
"polygon_smooth_enable\0"
"polygon_stipple_enable\0"
"rescale_normal_enable\0"
"sample_alpha_to_coverage_enable\0"
"sample_alpha_to_one_enable\0"
"sample_coverage_enable\0"
"scissor_test_enable\0"
"stencil_test_enable\0"
"func\0"
"value\0"
"src\0"
"dest\0"
"src_rgb\0"
"dest_rgb\0"
"src_alpha\0"
"dest_alpha\0"
"rgb\0"
"alpha\0"
"face\0"
"mode\0"
"mask\0"
"fail\0"
"zfail\0"
"zpass\0"
"front\0"
"back\0"
"index\0"
"instance_image\0"
"renderable\0"
"share\0"
"true\0"
"false\0"
"init_from\0"
"create_2d\0"
"create_3d\0"
"create_cube\0"
"image\0"
"mips_generate\0"
"array_index\0"
"mip_index\0"
"depth\0"
"hex\0"
"format\0"
"size_exact\0"
"width\0"
"height\0"
"size_ratio\0"
"mips\0"
"levels\0"
"auto_generate\0"
"unnormalized\0"
"array\0"
"length\0"
"hint\0"
"channels\0"
"range\0"
"precision\0"
"space\0"
"exact\0"
"size\0"
"wrap_s\0"
"wrap_t\0"
"wrap_p\0"
"minfilter\0"
"magfilter\0"
"mipfilter\0"
"border_color\0"
"mip_max_level\0"
"mip_min_level\0"
"mip_bias\0"
"max_anisotropy\0"
"shader\0"
"linker\0"
"target\0"
"options\0"
"binary\0"
"bind_attribute\0"
"bind_uniform\0"
"symbol\0"
"stage\0"
"sources\0"
"entry\0"
"inline\0"
"import\0"
"compiler\0"
"color_target\0"
"slice\0"
"mip\0"
"depth_target\0"
"stencil_target\0"
"color_clear\0"
"depth_clear\0"
"stencil_clear\0"
"draw\0"
"library_images\0"
"library_materials\0"
"instance_effect\0"
"technique_hint\0"
"setparam\0"
"material\0"
"library_geometries\0"
"geometry\0"
"mesh\0"
"source\0"
"count\0"
"accessor\0"
"stride\0"
"X\0"
"Y\0"
"Z\0"
"vertices\0"
"input\0"
"offset\0"
"polylist\0"
"vcount\0"
"p\0"
"convex_mesh\0"
"spline\0"
"brep\0"
"lines\0"
"linestrips\0"
"polygons\0"
"triangles\0"
"trifans\0"
"tristrips\0"
"float_array\0"
"bool_array\0"
"IDREF_array\0"
"int_array\0"
"Name_array\0"
"SIDREF_array\0"
"token_array\0"
"digits\0"
"magnitude\0"
"minInclusive\0"
"maxInclusive\0"
"ph\0"
"h\0"
"convex_hull_of\0"
"control_vertices\0"
"closed\0"
"set\0"
"line\0"
"circle\0"
"ellipse\0"
"parabola\0"
"hyperbola\0"
"nurbs\0"
"orient\0"
"origin\0"
"curve\0"
"direction\0"
"radius\0"
"degree\0"
"degree_u\0"
"closed_u\0"
"degree_v\0"
"closed_v\0"
"cone\0"
"plane\0"
"cylinder\0"
"nurbs_surface\0"
"sphere\0"
"torus\0"
"swept_surface\0"
"surface\0"
"angle\0"
"axis\0"
"curves\0"
"surface_curves\0"
"surfaces\0"
"edges\0"
"wires\0"
"faces\0"
"pcurves\0"
"shells\0"
"solids\0"
"library_controllers\0"
"controller\0"
"skin\0"
"morph\0"
"bind_shape_matrix\0"
"joints\0"
"vertex_weights\0"
"targets\0"
"v\0"
"method\0"
"library_visual_scenes\0"
"visual_scene\0"
"node\0"
"evaluate_scene\0"
"layer\0"
"lookat\0"
"matrix\0"
"rotate\0"
"scale\0"
"skew\0"
"translate\0"
"instance_camera\0"
"instance_controller\0"
"skeleton\0"
"bind_material\0"
"instance_material\0"
"bind\0"
"bind_vertex_input\0"
"input_semantic\0"
"input_set\0"
"instance_geometry\0"
"instance_light\0"
"instance_node\0"
"proxy\0"
"render\0"
"camera_node\0"
"technique_override\0"
"enable\0"
"library_nodes\0"
"instance_visual_scene\0"
"scene\0"
;

#undef _ak_DAE_STRPOOL_
