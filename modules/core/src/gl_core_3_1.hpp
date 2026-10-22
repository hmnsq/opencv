/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENGL_NOLOAD_STYLE_HPP
#define OPENGL_NOLOAD_STYLE_HPP

#إذا defined(__gl_h_) || defined(__GL_H__)
#error Attempt to include auto-generated header after including gl.h
#endif
#إذا defined(__glext_h_) || defined(__GLEXT_H_)
#error Attempt to include auto-generated header after including glext.h
#endif
#إذا defined(__gl_ATI_h_)
#error Attempt to include auto-generated header after including glATI.h
#endif

#define __gl_h_
#define __GL_H__
#define __glext_h_
#define __GLEXT_H_
#define __gl_ATI_h_

#ifndef APIENTRY
    #إذا defined(__MINGW32__)
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #ضمن <windows.h>
    #وإلاإذا (defined(_MSC_VER) && _MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN 1
        #endif
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #ضمن <windows.h>
    #وإلا
        #define APIENTRY
    #endif
#endif // APIENTRY

#ifndef CODEGEN_FUNCPTR
    #define CODEGEN_REMOVE_FUNCPTR
    #إذا defined(_WIN32)
        #define CODEGEN_FUNCPTR APIENTRY
    #وإلا
        #define CODEGEN_FUNCPTR
    #endif
#endif // CODEGEN_FUNCPTR

#ifndef GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS
#define GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS
    تعريفنوع unsigned صحيح GLenum;
    تعريفنوع unsigned حرف GLboolean;
    تعريفنوع unsigned صحيح GLbitfield;
    تعريفنوع signed حرف GLbyte;
    تعريفنوع short GLshort;
    تعريفنوع صحيح GLint;
    تعريفنوع صحيح GLsizei;
    تعريفنوع unsigned حرف GLubyte;
    تعريفنوع unsigned short GLushort;
    تعريفنوع unsigned صحيح GLuint;
    تعريفنوع عائم GLfloat;
    تعريفنوع عائم GLclampf;
    تعريفنوع مزدوج GLdouble;
    تعريفنوع مزدوج GLclampd;
    #define GLvoid فراغ
#endif // GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS

#ضمن <stddef.h>

#ifndef GL_VERSION_2_0
    // GL type for program/shader text
    تعريفنوع حرف GLchar;
#endif

#ifndef GL_VERSION_1_5
    // GL types for handling large vertex buffer objects
    تعريفنوع ptrdiff_t GLintptr;
    تعريفنوع ptrdiff_t GLsizeiptr;
#endif

#ifndef GL_ARB_vertex_buffer_object
    // GL types for handling large vertex buffer objects
    تعريفنوع ptrdiff_t GLintptrARB;
    تعريفنوع ptrdiff_t GLsizeiptrARB;
#endif

#ifndef GL_ARB_shader_objects
    // GL types for program/shader text and shader object handles
    تعريفنوع حرف GLcharARB;
    تعريفنوع unsigned صحيح GLhandleARB;
#endif

// GL type for "half" precision (s10e5) float data in host memory
#ifndef GL_ARB_half_float_pixel
    تعريفنوع unsigned short GLhalfARB;
#endif
#ifndef GL_NV_half_float
    تعريفنوع unsigned short GLhalfNV;
#endif

#ifndef GLEXT_64_TYPES_DEFINED
    // This code block is duplicated in glxext.h, so must be protected
    #define GLEXT_64_TYPES_DEFINED

    // Define int32_t, int64_t, and uint64_t types for UST/MSC
    // (as used in the GL_EXT_timer_query extension)
    #إذا defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #ضمن <inttypes.h>
    #وإلاإذا defined(__sun__) || defined(__digital__)
        #ضمن <inttypes.h>
        #إذا defined(__STDC__)
            #إذا defined(__arch64__) || defined(_LP64)
                تعريفنوع long صحيح int64_t;
                تعريفنوع unsigned long صحيح uint64_t;
            #وإلا
                تعريفنوع long long صحيح int64_t;
                تعريفنوع unsigned long long صحيح uint64_t;
            #endif // __arch64__
        #endif // __STDC__
    #وإلاإذا defined( __VMS ) || defined(__sgi)
        #ضمن <inttypes.h>
    #وإلاإذا defined(__SCO__) || defined(__USLC__)
        #ضمن <stdint.h>
    #وإلاإذا defined(__UNIXOS2__) || defined(__SOL64__)
        تعريفنوع long صحيح int32_t;
        تعريفنوع long long صحيح int64_t;
        تعريفنوع unsigned long long صحيح uint64_t;
    #وإلاإذا defined(_WIN32) && defined(__GNUC__)
        #ضمن <stdint.h>
    #وإلاإذا defined(_WIN32)
        تعريفنوع __int32 int32_t;
        تعريفنوع __int64 int64_t;
        تعريفنوع unsigned __int64 uint64_t;
    #وإلا
        // Fallback if nothing above works
        #ضمن <inttypes.h>
    #endif
#endif

#ifndef GL_EXT_timer_query
    تعريفنوع int64_t GLint64EXT;
    تعريفنوع uint64_t GLuint64EXT;
#endif

#ifndef GL_ARB_sync
    تعريفنوع int64_t GLint64;
    تعريفنوع uint64_t GLuint64;
    تعريفنوع بنية __GLsync *GLsync;
#endif

#ifndef GL_ARB_cl_event
    // These incomplete types let us declare types compatible with OpenCL's cl_context and cl_event
    بنية _cl_context;
    بنية _cl_event;
#endif

#ifndef GL_ARB_debug_output
    تعريفنوع فراغ (APIENTRY *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,ثابت GLchar *message,GLvoid *userParam);
#endif

#ifndef GL_AMD_debug_output
    تعريفنوع فراغ (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,ثابت GLchar *message,GLvoid *userParam);
#endif

#ifndef GL_KHR_debug
    تعريفنوع فراغ (APIENTRY *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,ثابت GLchar *message,GLvoid *userParam);
#endif

#ifndef GL_NV_vdpau_interop
    تعريفنوع GLintptr GLvdpauSurfaceNV;
#endif

نطاق gl
{
    تعداد
    {
        // Version: 1.1
        DEPTH_BUFFER_BIT                 = 0x00000100,
        STENCIL_BUFFER_BIT               = 0x00000400,
        COLOR_BUFFER_BIT                 = 0x00004000,
        FALSE_                           = 0,
        TRUE_                            = 1,
        POINTS                           = 0x0000,
        أسطر                            = 0x0001,
        سطر_LOOP                        = 0x0002,
        سطر_STRIP                       = 0x0003,
        TRIANGLES                        = 0x0004,
        TRIANGLE_STRIP                   = 0x0005,
        TRIANGLE_FAN                     = 0x0006,
        QUADS                            = 0x0007,
        NEVER                            = 0x0200,
        LESS                             = 0x0201,
        EQUAL                            = 0x0202,
        LEQUAL                           = 0x0203,
        GREATER                          = 0x0204,
        NOTEQUAL                         = 0x0205,
        GEQUAL                           = 0x0206,
        ALWAYS                           = 0x0207,
        ZERO                             = 0,
        ONE                              = 1,
        SRC_COLOR                        = 0x0300,
        ONE_MINUS_SRC_COLOR              = 0x0301,
        SRC_ALPHA                        = 0x0302,
        ONE_MINUS_SRC_ALPHA              = 0x0303,
        DST_ALPHA                        = 0x0304,
        ONE_MINUS_DST_ALPHA              = 0x0305,
        DST_COLOR                        = 0x0306,
        ONE_MINUS_DST_COLOR              = 0x0307,
        SRC_ALPHA_SATURATE               = 0x0308,
        NONE                             = 0,
        FRONT_يسار                       = 0x0400,
        FRONT_يمين                      = 0x0401,
        BACK_يسار                        = 0x0402,
        BACK_يمين                       = 0x0403,
        FRONT                            = 0x0404,
        BACK                             = 0x0405,
        يسار_2                             = 0x0406,
        يمين_2                            = 0x0407,
        FRONT_AND_BACK                   = 0x0408,
        NO_ERROR_                        = 0,
        INVALID_ENUM                     = 0x0500,
        INVALID_قيمة                    = 0x0501,
        INVALID_OPERATION                = 0x0502,
        OUT_OF_MEMORY                    = 0x0505,
        CW                               = 0x0900,
        CCW                              = 0x0901,
        POINT_SIZE                       = 0x0B11,
        POINT_SIZE_RANGE                 = 0x0B12,
        POINT_SIZE_GRANULARITY           = 0x0B13,
        سطر_SMOOTH                      = 0x0B20,
        سطر_WIDTH                       = 0x0B21,
        سطر_WIDTH_RANGE                 = 0x0B22,
        سطر_WIDTH_GRANULARITY           = 0x0B23,
        POLYGON_MODE                     = 0x0B40,
        POLYGON_SMOOTH                   = 0x0B41,
        CULL_FACE                        = 0x0B44,
        CULL_FACE_MODE                   = 0x0B45,
        FRONT_FACE                       = 0x0B46,
        DEPTH_RANGE                      = 0x0B70,
        DEPTH_TEST                       = 0x0B71,
        DEPTH_WRITEMASK                  = 0x0B72,
        DEPTH_CLEAR_قيمة                = 0x0B73,
        DEPTH_FUNC                       = 0x0B74,
        STENCIL_TEST                     = 0x0B90,
        STENCIL_CLEAR_قيمة              = 0x0B91,
        STENCIL_FUNC                     = 0x0B92,
        STENCIL_قيمة_MASK               = 0x0B93,
        STENCIL_FAIL                     = 0x0B94,
        STENCIL_PASS_DEPTH_FAIL          = 0x0B95,
        STENCIL_PASS_DEPTH_PASS          = 0x0B96,
        STENCIL_REF                      = 0x0B97,
        STENCIL_WRITEMASK                = 0x0B98,
        VIEWPORT                         = 0x0BA2,
        DITHER                           = 0x0BD0,
        BLEND_DST                        = 0x0BE0,
        BLEND_SRC                        = 0x0BE1,
        BLEND                            = 0x0BE2,
        LOGIC_OP_MODE                    = 0x0BF0,
        COLOR_LOGIC_OP                   = 0x0BF2,
        DRAW_BUFFER                      = 0x0C01,
        READ_BUFFER                      = 0x0C02,
        SCISSOR_BOX                      = 0x0C10,
        SCISSOR_TEST                     = 0x0C11,
        COLOR_CLEAR_قيمة                = 0x0C22,
        COLOR_WRITEMASK                  = 0x0C23,
        DOUBLEBUFFER                     = 0x0C32,
        STEREO                           = 0x0C33,
        سطر_SMOOTH_HINT                 = 0x0C52,
        POLYGON_SMOOTH_HINT              = 0x0C53,
        UNPACK_SWAP_BYTES                = 0x0CF0,
        UNPACK_LSB_FIRST                 = 0x0CF1,
        UNPACK_ROW_LENGTH                = 0x0CF2,
        UNPACK_SKIP_ROWS                 = 0x0CF3,
        UNPACK_SKIP_PIXELS               = 0x0CF4,
        UNPACK_ALIGNMENT                 = 0x0CF5,
        PACK_SWAP_BYTES                  = 0x0D00,
        PACK_LSB_FIRST                   = 0x0D01,
        PACK_ROW_LENGTH                  = 0x0D02,
        PACK_SKIP_ROWS                   = 0x0D03,
        PACK_SKIP_PIXELS                 = 0x0D04,
        PACK_ALIGNMENT                   = 0x0D05,
        MAX_TEXTURE_SIZE                 = 0x0D33,
        MAX_VIEWPORT_DIMS                = 0x0D3A,
        SUBPIXEL_BITS                    = 0x0D50,
        TEXTURE_1D                       = 0x0DE0,
        TEXTURE_2D                       = 0x0DE1,
        POLYGON_OFFSET_UNITS             = 0x2A00,
        POLYGON_OFFSET_POINT             = 0x2A01,
        POLYGON_OFFSET_سطر              = 0x2A02,
        POLYGON_OFFSET_FILL              = 0x8037,
        POLYGON_OFFSET_FACTOR            = 0x8038,
        TEXTURE_BINDING_1D               = 0x8068,
        TEXTURE_BINDING_2D               = 0x8069,
        TEXTURE_WIDTH                    = 0x1000,
        TEXTURE_HEIGHT                   = 0x1001,
        TEXTURE_INTERNAL_FORMAT          = 0x1003,
        TEXTURE_BORDER_COLOR             = 0x1004,
        TEXTURE_RED_SIZE                 = 0x805C,
        TEXTURE_GREEN_SIZE               = 0x805D,
        TEXTURE_BLUE_SIZE                = 0x805E,
        TEXTURE_ALPHA_SIZE               = 0x805F,
        DONT_CARE                        = 0x1100,
        FASTEST                          = 0x1101,
        NICEST                           = 0x1102,
        BYTE                             = 0x1400,
        UNSIGNED_BYTE                    = 0x1401,
        SHORT                            = 0x1402,
        UNSIGNED_SHORT                   = 0x1403,
        INT                              = 0x1404,
        UNSIGNED_INT                     = 0x1405,
        FLOAT                            = 0x1406,
        DOUBLE                           = 0x140A,
        CLEAR                            = 0x1500,
        AND                              = 0x1501,
        AND_REVERSE                      = 0x1502,
        COPY                             = 0x1503,
        AND_INVERTED                     = 0x1504,
        NOOP                             = 0x1505,
        XOR                              = 0x1506,
        OR                               = 0x1507,
        NOR                              = 0x1508,
        EQUIV                            = 0x1509,
        INVERT                           = 0x150A,
        OR_REVERSE                       = 0x150B,
        COPY_INVERTED                    = 0x150C,
        OR_INVERTED                      = 0x150D,
        NAND                             = 0x150E,
        SET                              = 0x150F,
        TEXTURE                          = 0x1702,
        COLOR                            = 0x1800,
        DEPTH                            = 0x1801,
        STENCIL                          = 0x1802,
        STENCIL_INDEX                    = 0x1901,
        DEPTH_COMPONENT                  = 0x1902,
        RED                              = 0x1903,
        GREEN                            = 0x1904,
        BLUE                             = 0x1905,
        ALPHA                            = 0x1906,
        RGB                              = 0x1907,
        RGBA                             = 0x1908,
        POINT                            = 0x1B00,
        سطر_2                             = 0x1B01,
        FILL                             = 0x1B02,
        KEEP                             = 0x1E00,
        REPLACE                          = 0x1E01,
        INCR                             = 0x1E02,
        DECR                             = 0x1E03,
        VENDOR                           = 0x1F00,
        RENDERER                         = 0x1F01,
        VERSION_                         = 0x1F02,
        EXTENSIONS                       = 0x1F03,
        NEAREST                          = 0x2600,
        LINEAR                           = 0x2601,
        NEAREST_MIPMAP_NEAREST           = 0x2700,
        LINEAR_MIPMAP_NEAREST            = 0x2701,
        NEAREST_MIPMAP_LINEAR            = 0x2702,
        LINEAR_MIPMAP_LINEAR             = 0x2703,
        TEXTURE_MAG_FILTER               = 0x2800,
        TEXTURE_MIN_FILTER               = 0x2801,
        TEXTURE_WRAP_S                   = 0x2802,
        TEXTURE_WRAP_T                   = 0x2803,
        PROXY_TEXTURE_1D                 = 0x8063,
        PROXY_TEXTURE_2D                 = 0x8064,
        REPEAT                           = 0x2901,
        R3_G3_B2                         = 0x2A10,
        RGB4                             = 0x804F,
        RGB5                             = 0x8050,
        RGB8                             = 0x8051,
        RGB10                            = 0x8052,
        RGB12                            = 0x8053,
        RGB16                            = 0x8054,
        RGBA2                            = 0x8055,
        RGBA4                            = 0x8056,
        RGB5_A1                          = 0x8057,
        RGBA8                            = 0x8058,
        RGB10_A2                         = 0x8059,
        RGBA12                           = 0x805A,
        RGBA16                           = 0x805B,

        // Core Extension: ARB_imaging
        CONSTANT_COLOR                   = 0x8001,
        ONE_MINUS_CONSTANT_COLOR         = 0x8002,
        CONSTANT_ALPHA                   = 0x8003,
        ONE_MINUS_CONSTANT_ALPHA         = 0x8004,
        BLEND_COLOR                      = 0x8005,
        FUNC_ADD                         = 0x8006,
        MIN                              = 0x8007,
        MAX                              = 0x8008,
        BLEND_EQUATION                   = 0x8009,
        FUNC_SUBTRACT                    = 0x800A,
        FUNC_REVERSE_SUBTRACT            = 0x800B,
        CONVOLUTION_1D                   = 0x8010,
        CONVOLUTION_2D                   = 0x8011,
        SEPARABLE_2D                     = 0x8012,
        CONVOLUTION_BORDER_MODE          = 0x8013,
        CONVOLUTION_FILTER_SCALE         = 0x8014,
        CONVOLUTION_FILTER_BIAS          = 0x8015,
        REDUCE                           = 0x8016,
        CONVOLUTION_FORMAT               = 0x8017,
        CONVOLUTION_WIDTH                = 0x8018,
        CONVOLUTION_HEIGHT               = 0x8019,
        MAX_CONVOLUTION_WIDTH            = 0x801A,
        MAX_CONVOLUTION_HEIGHT           = 0x801B,
        POST_CONVOLUTION_RED_SCALE       = 0x801C,
        POST_CONVOLUTION_GREEN_SCALE     = 0x801D,
        POST_CONVOLUTION_BLUE_SCALE      = 0x801E,
        POST_CONVOLUTION_ALPHA_SCALE     = 0x801F,
        POST_CONVOLUTION_RED_BIAS        = 0x8020,
        POST_CONVOLUTION_GREEN_BIAS      = 0x8021,
        POST_CONVOLUTION_BLUE_BIAS       = 0x8022,
        POST_CONVOLUTION_ALPHA_BIAS      = 0x8023,
        HISTOGRAM                        = 0x8024,
        PROXY_HISTOGRAM                  = 0x8025,
        HISTOGRAM_WIDTH                  = 0x8026,
        HISTOGRAM_FORMAT                 = 0x8027,
        HISTOGRAM_RED_SIZE               = 0x8028,
        HISTOGRAM_GREEN_SIZE             = 0x8029,
        HISTOGRAM_BLUE_SIZE              = 0x802A,
        HISTOGRAM_ALPHA_SIZE             = 0x802B,
        HISTOGRAM_LUMINANCE_SIZE         = 0x802C,
        HISTOGRAM_SINK                   = 0x802D,
        MINMAX                           = 0x802E,
        MINMAX_FORMAT                    = 0x802F,
        MINMAX_SINK                      = 0x8030,
        TABLE_TOO_LARGE                  = 0x8031,
        COLOR_MATRIX                     = 0x80B1,
        COLOR_MATRIX_مكدس_DEPTH         = 0x80B2,
        MAX_COLOR_MATRIX_مكدس_DEPTH     = 0x80B3,
        POST_COLOR_MATRIX_RED_SCALE      = 0x80B4,
        POST_COLOR_MATRIX_GREEN_SCALE    = 0x80B5,
        POST_COLOR_MATRIX_BLUE_SCALE     = 0x80B6,
        POST_COLOR_MATRIX_ALPHA_SCALE    = 0x80B7,
        POST_COLOR_MATRIX_RED_BIAS       = 0x80B8,
        POST_COLOR_MATRIX_GREEN_BIAS     = 0x80B9,
        POST_COLOR_MATRIX_BLUE_BIAS      = 0x80BA,
        POST_COLOR_MATRIX_ALPHA_BIAS     = 0x80BB,
        COLOR_TABLE                      = 0x80D0,
        POST_CONVOLUTION_COLOR_TABLE     = 0x80D1,
        POST_COLOR_MATRIX_COLOR_TABLE    = 0x80D2,
        PROXY_COLOR_TABLE                = 0x80D3,
        PROXY_POST_CONVOLUTION_COLOR_TABLE = 0x80D4,
        PROXY_POST_COLOR_MATRIX_COLOR_TABLE = 0x80D5,
        COLOR_TABLE_SCALE                = 0x80D6,
        COLOR_TABLE_BIAS                 = 0x80D7,
        COLOR_TABLE_FORMAT               = 0x80D8,
        COLOR_TABLE_WIDTH                = 0x80D9,
        COLOR_TABLE_RED_SIZE             = 0x80DA,
        COLOR_TABLE_GREEN_SIZE           = 0x80DB,
        COLOR_TABLE_BLUE_SIZE            = 0x80DC,
        COLOR_TABLE_ALPHA_SIZE           = 0x80DD,
        COLOR_TABLE_LUMINANCE_SIZE       = 0x80DE,
        COLOR_TABLE_INTENSITY_SIZE       = 0x80DF,
        CONSTANT_BORDER                  = 0x8151,
        REPLICATE_BORDER                 = 0x8153,
        CONVOLUTION_BORDER_COLOR         = 0x8154,

        // Version: 1.2
        UNSIGNED_BYTE_3_3_2              = 0x8032,
        UNSIGNED_SHORT_4_4_4_4           = 0x8033,
        UNSIGNED_SHORT_5_5_5_1           = 0x8034,
        UNSIGNED_INT_8_8_8_8             = 0x8035,
        UNSIGNED_INT_10_10_10_2          = 0x8036,
        TEXTURE_BINDING_3D               = 0x806A,
        PACK_SKIP_IMAGES                 = 0x806B,
        PACK_IMAGE_HEIGHT                = 0x806C,
        UNPACK_SKIP_IMAGES               = 0x806D,
        UNPACK_IMAGE_HEIGHT              = 0x806E,
        TEXTURE_3D                       = 0x806F,
        PROXY_TEXTURE_3D                 = 0x8070,
        TEXTURE_DEPTH                    = 0x8071,
        TEXTURE_WRAP_R                   = 0x8072,
        MAX_3D_TEXTURE_SIZE              = 0x8073,
        UNSIGNED_BYTE_2_3_3_REV          = 0x8362,
        UNSIGNED_SHORT_5_6_5             = 0x8363,
        UNSIGNED_SHORT_5_6_5_REV         = 0x8364,
        UNSIGNED_SHORT_4_4_4_4_REV       = 0x8365,
        UNSIGNED_SHORT_1_5_5_5_REV       = 0x8366,
        UNSIGNED_INT_8_8_8_8_REV         = 0x8367,
        UNSIGNED_INT_2_10_10_10_REV      = 0x8368,
        BGR                              = 0x80E0,
        BGRA                             = 0x80E1,
        MAX_ELEMENTS_VERTICES            = 0x80E8,
        MAX_ELEMENTS_INDICES             = 0x80E9,
        CLAMP_TO_EDGE                    = 0x812F,
        TEXTURE_MIN_LOD                  = 0x813A,
        TEXTURE_MAX_LOD                  = 0x813B,
        TEXTURE_BASE_LEVEL               = 0x813C,
        TEXTURE_MAX_LEVEL                = 0x813D,
        SMOOTH_POINT_SIZE_RANGE          = 0x0B12,
        SMOOTH_POINT_SIZE_GRANULARITY    = 0x0B13,
        SMOOTH_سطر_WIDTH_RANGE          = 0x0B22,
        SMOOTH_سطر_WIDTH_GRANULARITY    = 0x0B23,
        ALIASED_سطر_WIDTH_RANGE         = 0x846E,

        // Version: 1.3
        TEXTURE0                         = 0x84C0,
        TEXTURE1                         = 0x84C1,
        TEXTURE2                         = 0x84C2,
        TEXTURE3                         = 0x84C3,
        TEXTURE4                         = 0x84C4,
        TEXTURE5                         = 0x84C5,
        TEXTURE6                         = 0x84C6,
        TEXTURE7                         = 0x84C7,
        TEXTURE8                         = 0x84C8,
        TEXTURE9                         = 0x84C9,
        TEXTURE10                        = 0x84CA,
        TEXTURE11                        = 0x84CB,
        TEXTURE12                        = 0x84CC,
        TEXTURE13                        = 0x84CD,
        TEXTURE14                        = 0x84CE,
        TEXTURE15                        = 0x84CF,
        TEXTURE16                        = 0x84D0,
        TEXTURE17                        = 0x84D1,
        TEXTURE18                        = 0x84D2,
        TEXTURE19                        = 0x84D3,
        TEXTURE20                        = 0x84D4,
        TEXTURE21                        = 0x84D5,
        TEXTURE22                        = 0x84D6,
        TEXTURE23                        = 0x84D7,
        TEXTURE24                        = 0x84D8,
        TEXTURE25                        = 0x84D9,
        TEXTURE26                        = 0x84DA,
        TEXTURE27                        = 0x84DB,
        TEXTURE28                        = 0x84DC,
        TEXTURE29                        = 0x84DD,
        TEXTURE30                        = 0x84DE,
        TEXTURE31                        = 0x84DF,
        ACTIVE_TEXTURE                   = 0x84E0,
        MULTISAMPLE                      = 0x809D,
        SAMPLE_ALPHA_TO_COVERAGE         = 0x809E,
        SAMPLE_ALPHA_TO_ONE              = 0x809F,
        SAMPLE_COVERAGE                  = 0x80A0,
        SAMPLE_BUFFERS                   = 0x80A8,
        SAMPLES                          = 0x80A9,
        SAMPLE_COVERAGE_قيمة            = 0x80AA,
        SAMPLE_COVERAGE_INVERT           = 0x80AB,
        TEXTURE_CUBE_MAP                 = 0x8513,
        TEXTURE_BINDING_CUBE_MAP         = 0x8514,
        TEXTURE_CUBE_MAP_POSITIVE_X      = 0x8515,
        TEXTURE_CUBE_MAP_NEGATIVE_X      = 0x8516,
        TEXTURE_CUBE_MAP_POSITIVE_Y      = 0x8517,
        TEXTURE_CUBE_MAP_NEGATIVE_Y      = 0x8518,
        TEXTURE_CUBE_MAP_POSITIVE_Z      = 0x8519,
        TEXTURE_CUBE_MAP_NEGATIVE_Z      = 0x851A,
        PROXY_TEXTURE_CUBE_MAP           = 0x851B,
        MAX_CUBE_MAP_TEXTURE_SIZE        = 0x851C,
        COMPRESSED_RGB                   = 0x84ED,
        COMPRESSED_RGBA                  = 0x84EE,
        TEXTURE_COMPRESSION_HINT         = 0x84EF,
        TEXTURE_COMPRESSED_IMAGE_SIZE    = 0x86A0,
        TEXTURE_COMPRESSED               = 0x86A1,
        رقم_COMPRESSED_TEXTURE_FORMATS   = 0x86A2,
        COMPRESSED_TEXTURE_FORMATS       = 0x86A3,
        CLAMP_TO_BORDER                  = 0x812D,

        // Version: 1.4
        BLEND_DST_RGB                    = 0x80C8,
        BLEND_SRC_RGB                    = 0x80C9,
        BLEND_DST_ALPHA                  = 0x80CA,
        BLEND_SRC_ALPHA                  = 0x80CB,
        POINT_FADE_THRESHOLD_SIZE        = 0x8128,
        DEPTH_COMPONENT16                = 0x81A5,
        DEPTH_COMPONENT24                = 0x81A6,
        DEPTH_COMPONENT32                = 0x81A7,
        MIRRORED_REPEAT                  = 0x8370,
        MAX_TEXTURE_LOD_BIAS             = 0x84FD,
        TEXTURE_LOD_BIAS                 = 0x8501,
        INCR_WRAP                        = 0x8507,
        DECR_WRAP                        = 0x8508,
        TEXTURE_DEPTH_SIZE               = 0x884A,
        TEXTURE_COMPARE_MODE             = 0x884C,
        TEXTURE_COMPARE_FUNC             = 0x884D,

        // Version: 1.5
        BUFFER_SIZE                      = 0x8764,
        BUFFER_USAGE                     = 0x8765,
        QUERY_COUNTER_BITS               = 0x8864,
        CURRENT_QUERY                    = 0x8865,
        QUERY_RESULT                     = 0x8866,
        QUERY_RESULT_AVAILABLE           = 0x8867,
        ARRAY_BUFFER                     = 0x8892,
        ELEMENT_ARRAY_BUFFER             = 0x8893,
        ARRAY_BUFFER_BINDING             = 0x8894,
        ELEMENT_ARRAY_BUFFER_BINDING     = 0x8895,
        VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F,
        READ_ONLY                        = 0x88B8,
        WRITE_ONLY                       = 0x88B9,
        READ_WRITE                       = 0x88BA,
        BUFFER_ACCESS                    = 0x88BB,
        BUFFER_MAPPED                    = 0x88BC,
        BUFFER_MAP_POINTER               = 0x88BD,
        STREAM_DRAW                      = 0x88E0,
        STREAM_READ                      = 0x88E1,
        STREAM_COPY                      = 0x88E2,
        STATIC_DRAW                      = 0x88E4,
        STATIC_READ                      = 0x88E5,
        STATIC_COPY                      = 0x88E6,
        DYNAMIC_DRAW                     = 0x88E8,
        DYNAMIC_READ                     = 0x88E9,
        DYNAMIC_COPY                     = 0x88EA,
        SAMPLES_PASSED                   = 0x8914,
        SRC1_ALPHA                       = 0x8589,

        // Version: 2.0
        BLEND_EQUATION_RGB               = 0x8009,
        VERTEX_ATTRIB_ARRAY_ENABLED      = 0x8622,
        VERTEX_ATTRIB_ARRAY_SIZE         = 0x8623,
        VERTEX_ATTRIB_ARRAY_STRIDE       = 0x8624,
        VERTEX_ATTRIB_ARRAY_TYPE         = 0x8625,
        CURRENT_VERTEX_ATTRIB            = 0x8626,
        VERTEX_PROGRAM_POINT_SIZE        = 0x8642,
        VERTEX_ATTRIB_ARRAY_POINTER      = 0x8645,
        STENCIL_BACK_FUNC                = 0x8800,
        STENCIL_BACK_FAIL                = 0x8801,
        STENCIL_BACK_PASS_DEPTH_FAIL     = 0x8802,
        STENCIL_BACK_PASS_DEPTH_PASS     = 0x8803,
        MAX_DRAW_BUFFERS                 = 0x8824,
        DRAW_BUFFER0                     = 0x8825,
        DRAW_BUFFER1                     = 0x8826,
        DRAW_BUFFER2                     = 0x8827,
        DRAW_BUFFER3                     = 0x8828,
        DRAW_BUFFER4                     = 0x8829,
        DRAW_BUFFER5                     = 0x882A,
        DRAW_BUFFER6                     = 0x882B,
        DRAW_BUFFER7                     = 0x882C,
        DRAW_BUFFER8                     = 0x882D,
        DRAW_BUFFER9                     = 0x882E,
        DRAW_BUFFER10                    = 0x882F,
        DRAW_BUFFER11                    = 0x8830,
        DRAW_BUFFER12                    = 0x8831,
        DRAW_BUFFER13                    = 0x8832,
        DRAW_BUFFER14                    = 0x8833,
        DRAW_BUFFER15                    = 0x8834,
        BLEND_EQUATION_ALPHA             = 0x883D,
        MAX_VERTEX_ATTRIBS               = 0x8869,
        VERTEX_ATTRIB_ARRAY_NORMALIZED   = 0x886A,
        MAX_TEXTURE_IMAGE_UNITS          = 0x8872,
        FRAGMENT_SHADER                  = 0x8B30,
        VERTEX_SHADER                    = 0x8B31,
        MAX_FRAGMENT_UNIFORM_COMPONENTS  = 0x8B49,
        MAX_VERTEX_UNIFORM_COMPONENTS    = 0x8B4A,
        MAX_VARYING_FLOATS               = 0x8B4B,
        MAX_VERTEX_TEXTURE_IMAGE_UNITS   = 0x8B4C,
        MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D,
        SHADER_TYPE                      = 0x8B4F,
        FLOAT_VEC2                       = 0x8B50,
        FLOAT_VEC3                       = 0x8B51,
        FLOAT_VEC4                       = 0x8B52,
        INT_VEC2                         = 0x8B53,
        INT_VEC3                         = 0x8B54,
        INT_VEC4                         = 0x8B55,
        BOOL                             = 0x8B56,
        BOOL_VEC2                        = 0x8B57,
        BOOL_VEC3                        = 0x8B58,
        BOOL_VEC4                        = 0x8B59,
        FLOAT_MAT2                       = 0x8B5A,
        FLOAT_MAT3                       = 0x8B5B,
        FLOAT_MAT4                       = 0x8B5C,
        SAMPLER_1D                       = 0x8B5D,
        SAMPLER_2D                       = 0x8B5E,
        SAMPLER_3D                       = 0x8B5F,
        SAMPLER_CUBE                     = 0x8B60,
        SAMPLER_1D_SHADOW                = 0x8B61,
        SAMPLER_2D_SHADOW                = 0x8B62,
        DELETE_STATUS                    = 0x8B80,
        COMPILE_STATUS                   = 0x8B81,
        LINK_STATUS                      = 0x8B82,
        VALIDATE_STATUS                  = 0x8B83,
        INFO_LOG_LENGTH                  = 0x8B84,
        ATTACHED_SHADERS                 = 0x8B85,
        ACTIVE_UNIFORMS                  = 0x8B86,
        ACTIVE_UNIFORM_MAX_LENGTH        = 0x8B87,
        SHADER_SOURCE_LENGTH             = 0x8B88,
        ACTIVE_ATTRIBUTES                = 0x8B89,
        ACTIVE_ATTRIBUTE_MAX_LENGTH      = 0x8B8A,
        FRAGMENT_SHADER_DERIVATIVE_HINT  = 0x8B8B,
        SHADING_LANGUAGE_VERSION         = 0x8B8C,
        CURRENT_PROGRAM                  = 0x8B8D,
        POINT_SPRITE_COORD_ORIGIN        = 0x8CA0,
        LOWER_يسار                       = 0x8CA1,
        UPPER_يسار                       = 0x8CA2,
        STENCIL_BACK_REF                 = 0x8CA3,
        STENCIL_BACK_قيمة_MASK          = 0x8CA4,
        STENCIL_BACK_WRITEMASK           = 0x8CA5,

        // Version: 2.1
        PIXEL_PACK_BUFFER                = 0x88EB,
        PIXEL_UNPACK_BUFFER              = 0x88EC,
        PIXEL_PACK_BUFFER_BINDING        = 0x88ED,
        PIXEL_UNPACK_BUFFER_BINDING      = 0x88EF,
        FLOAT_MAT2x3                     = 0x8B65,
        FLOAT_MAT2x4                     = 0x8B66,
        FLOAT_MAT3x2                     = 0x8B67,
        FLOAT_MAT3x4                     = 0x8B68,
        FLOAT_MAT4x2                     = 0x8B69,
        FLOAT_MAT4x3                     = 0x8B6A,
        SRGB                             = 0x8C40,
        SRGB8                            = 0x8C41,
        SRGB_ALPHA                       = 0x8C42,
        SRGB8_ALPHA8                     = 0x8C43,
        COMPRESSED_SRGB                  = 0x8C48,
        COMPRESSED_SRGB_ALPHA            = 0x8C49,

        // Core Extension: ARB_vertex_array_object
        VERTEX_ARRAY_BINDING             = 0x85B5,

        // Core Extension: ARB_texture_rg
        RG                               = 0x8227,
        RG_INTEGER                       = 0x8228,
        R8                               = 0x8229,
        R16                              = 0x822A,
        RG8                              = 0x822B,
        RG16                             = 0x822C,
        R16F                             = 0x822D,
        R32F                             = 0x822E,
        RG16F                            = 0x822F,
        RG32F                            = 0x8230,
        R8I                              = 0x8231,
        R8UI                             = 0x8232,
        R16I                             = 0x8233,
        R16UI                            = 0x8234,
        R32I                             = 0x8235,
        R32UI                            = 0x8236,
        RG8I                             = 0x8237,
        RG8UI                            = 0x8238,
        RG16I                            = 0x8239,
        RG16UI                           = 0x823A,
        RG32I                            = 0x823B,
        RG32UI                           = 0x823C,

        // Core Extension: ARB_texture_compression_rgtc
        COMPRESSED_RED_RGTC1             = 0x8DBB,
        COMPRESSED_SIGNED_RED_RGTC1      = 0x8DBC,
        COMPRESSED_RG_RGTC2              = 0x8DBD,
        COMPRESSED_SIGNED_RG_RGTC2       = 0x8DBE,

        // Core Extension: ARB_map_buffer_range
        MAP_READ_BIT                     = 0x0001,
        MAP_WRITE_BIT                    = 0x0002,
        MAP_INVALIDATE_RANGE_BIT         = 0x0004,
        MAP_INVALIDATE_BUFFER_BIT        = 0x0008,
        MAP_FLUSH_EXPLICIT_BIT           = 0x0010,
        MAP_UNSYNCHRONIZED_BIT           = 0x0020,

        // Core Extension: ARB_half_float_vertex
        HALF_FLOAT                       = 0x140B,

        // Core Extension: ARB_framebuffer_sRGB
        FRAMEBUFFER_SRGB                 = 0x8DB9,

        // Core Extension: ARB_framebuffer_object
        INVALID_FRAMEBUFFER_OPERATION    = 0x0506,
        FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = 0x8210,
        FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = 0x8211,
        FRAMEBUFFER_ATTACHMENT_RED_SIZE  = 0x8212,
        FRAMEBUFFER_ATTACHMENT_GREEN_SIZE = 0x8213,
        FRAMEBUFFER_ATTACHMENT_BLUE_SIZE = 0x8214,
        FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE = 0x8215,
        FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE = 0x8216,
        FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE = 0x8217,
        FRAMEBUFFER_DEFAULT              = 0x8218,
        FRAMEBUFFER_UNDEFINED            = 0x8219,
        DEPTH_STENCIL_ATTACHMENT         = 0x821A,
        INDEX                            = 0x8222,
        MAX_RENDERBUFFER_SIZE            = 0x84E8,
        DEPTH_STENCIL                    = 0x84F9,
        UNSIGNED_INT_24_8                = 0x84FA,
        DEPTH24_STENCIL8                 = 0x88F0,
        TEXTURE_STENCIL_SIZE             = 0x88F1,
        TEXTURE_RED_TYPE                 = 0x8C10,
        TEXTURE_GREEN_TYPE               = 0x8C11,
        TEXTURE_BLUE_TYPE                = 0x8C12,
        TEXTURE_ALPHA_TYPE               = 0x8C13,
        TEXTURE_DEPTH_TYPE               = 0x8C16,
        UNSIGNED_NORMALIZED              = 0x8C17,
        FRAMEBUFFER_BINDING              = 0x8CA6,
        DRAW_FRAMEBUFFER_BINDING         = 0x8CA6,
        RENDERBUFFER_BINDING             = 0x8CA7,
        READ_FRAMEBUFFER                 = 0x8CA8,
        DRAW_FRAMEBUFFER                 = 0x8CA9,
        READ_FRAMEBUFFER_BINDING         = 0x8CAA,
        RENDERBUFFER_SAMPLES             = 0x8CAB,
        FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0,
        FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1,
        FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2,
        FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3,
        FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER = 0x8CD4,
        FRAMEBUFFER_COMPLETE             = 0x8CD5,
        FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6,
        FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7,
        FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER = 0x8CDB,
        FRAMEBUFFER_INCOMPLETE_READ_BUFFER = 0x8CDC,
        FRAMEBUFFER_UNSUPPORTED          = 0x8CDD,
        MAX_COLOR_ATTACHMENTS            = 0x8CDF,
        COLOR_ATTACHMENT0                = 0x8CE0,
        COLOR_ATTACHMENT1                = 0x8CE1,
        COLOR_ATTACHMENT2                = 0x8CE2,
        COLOR_ATTACHMENT3                = 0x8CE3,
        COLOR_ATTACHMENT4                = 0x8CE4,
        COLOR_ATTACHMENT5                = 0x8CE5,
        COLOR_ATTACHMENT6                = 0x8CE6,
        COLOR_ATTACHMENT7                = 0x8CE7,
        COLOR_ATTACHMENT8                = 0x8CE8,
        COLOR_ATTACHMENT9                = 0x8CE9,
        COLOR_ATTACHMENT10               = 0x8CEA,
        COLOR_ATTACHMENT11               = 0x8CEB,
        COLOR_ATTACHMENT12               = 0x8CEC,
        COLOR_ATTACHMENT13               = 0x8CED,
        COLOR_ATTACHMENT14               = 0x8CEE,
        COLOR_ATTACHMENT15               = 0x8CEF,
        DEPTH_ATTACHMENT                 = 0x8D00,
        STENCIL_ATTACHMENT               = 0x8D20,
        FRAMEBUFFER                      = 0x8D40,
        RENDERBUFFER                     = 0x8D41,
        RENDERBUFFER_WIDTH               = 0x8D42,
        RENDERBUFFER_HEIGHT              = 0x8D43,
        RENDERBUFFER_INTERNAL_FORMAT     = 0x8D44,
        STENCIL_INDEX1                   = 0x8D46,
        STENCIL_INDEX4                   = 0x8D47,
        STENCIL_INDEX8                   = 0x8D48,
        STENCIL_INDEX16                  = 0x8D49,
        RENDERBUFFER_RED_SIZE            = 0x8D50,
        RENDERBUFFER_GREEN_SIZE          = 0x8D51,
        RENDERBUFFER_BLUE_SIZE           = 0x8D52,
        RENDERBUFFER_ALPHA_SIZE          = 0x8D53,
        RENDERBUFFER_DEPTH_SIZE          = 0x8D54,
        RENDERBUFFER_STENCIL_SIZE        = 0x8D55,
        FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = 0x8D56,
        MAX_SAMPLES                      = 0x8D57,
        TEXTURE_LUMINANCE_TYPE           = 0x8C14,
        TEXTURE_INTENSITY_TYPE           = 0x8C15,

        // Core Extension: ARB_depth_buffer_float
        DEPTH_COMPONENT32F               = 0x8CAC,
        DEPTH32F_STENCIL8                = 0x8CAD,
        FLOAT_32_UNSIGNED_INT_24_8_REV   = 0x8DAD,

        // Version: 3.0
        COMPARE_REF_TO_TEXTURE           = 0x884E,
        CLIP_DISTANCE0                   = 0x3000,
        CLIP_DISTANCE1                   = 0x3001,
        CLIP_DISTANCE2                   = 0x3002,
        CLIP_DISTANCE3                   = 0x3003,
        CLIP_DISTANCE4                   = 0x3004,
        CLIP_DISTANCE5                   = 0x3005,
        CLIP_DISTANCE6                   = 0x3006,
        CLIP_DISTANCE7                   = 0x3007,
        MAX_CLIP_DISTANCES               = 0x0D32,
        MAJOR_VERSION                    = 0x821B,
        MINOR_VERSION                    = 0x821C,
        رقم_EXTENSIONS                   = 0x821D,
        CONTEXT_FLAGS                    = 0x821E,
        COMPRESSED_RED                   = 0x8225,
        COMPRESSED_RG                    = 0x8226,
        CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = 0x0001,
        RGBA32F                          = 0x8814,
        RGB32F                           = 0x8815,
        RGBA16F                          = 0x881A,
        RGB16F                           = 0x881B,
        VERTEX_ATTRIB_ARRAY_INTEGER      = 0x88FD,
        MAX_ARRAY_TEXTURE_LAYERS         = 0x88FF,
        MIN_PROGRAM_TEXEL_OFFSET         = 0x8904,
        MAX_PROGRAM_TEXEL_OFFSET         = 0x8905,
        CLAMP_READ_COLOR                 = 0x891C,
        FIXED_ONLY                       = 0x891D,
        TEXTURE_1D_ARRAY                 = 0x8C18,
        PROXY_TEXTURE_1D_ARRAY           = 0x8C19,
        TEXTURE_2D_ARRAY                 = 0x8C1A,
        PROXY_TEXTURE_2D_ARRAY           = 0x8C1B,
        TEXTURE_BINDING_1D_ARRAY         = 0x8C1C,
        TEXTURE_BINDING_2D_ARRAY         = 0x8C1D,
        R11F_G11F_B10F                   = 0x8C3A,
        UNSIGNED_INT_10F_11F_11F_REV     = 0x8C3B,
        RGB9_E5                          = 0x8C3D,
        UNSIGNED_INT_5_9_9_9_REV         = 0x8C3E,
        TEXTURE_SHARED_SIZE              = 0x8C3F,
        TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH = 0x8C76,
        TRANSFORM_FEEDBACK_BUFFER_MODE   = 0x8C7F,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = 0x8C80,
        TRANSFORM_FEEDBACK_VARYINGS      = 0x8C83,
        TRANSFORM_FEEDBACK_BUFFER_START  = 0x8C84,
        TRANSFORM_FEEDBACK_BUFFER_SIZE   = 0x8C85,
        PRIMITIVES_GENERATED             = 0x8C87,
        TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88,
        RASTERIZER_DISCARD               = 0x8C89,
        MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = 0x8C8B,
        INTERLEAVED_ATTRIBS              = 0x8C8C,
        SEPARATE_ATTRIBS                 = 0x8C8D,
        TRANSFORM_FEEDBACK_BUFFER        = 0x8C8E,
        TRANSFORM_FEEDBACK_BUFFER_BINDING = 0x8C8F,
        RGBA32UI                         = 0x8D70,
        RGB32UI                          = 0x8D71,
        RGBA16UI                         = 0x8D76,
        RGB16UI                          = 0x8D77,
        RGBA8UI                          = 0x8D7C,
        RGB8UI                           = 0x8D7D,
        RGBA32I                          = 0x8D82,
        RGB32I                           = 0x8D83,
        RGBA16I                          = 0x8D88,
        RGB16I                           = 0x8D89,
        RGBA8I                           = 0x8D8E,
        RGB8I                            = 0x8D8F,
        RED_INTEGER                      = 0x8D94,
        GREEN_INTEGER                    = 0x8D95,
        BLUE_INTEGER                     = 0x8D96,
        RGB_INTEGER                      = 0x8D98,
        RGBA_INTEGER                     = 0x8D99,
        BGR_INTEGER                      = 0x8D9A,
        BGRA_INTEGER                     = 0x8D9B,
        SAMPLER_1D_ARRAY                 = 0x8DC0,
        SAMPLER_2D_ARRAY                 = 0x8DC1,
        SAMPLER_1D_ARRAY_SHADOW          = 0x8DC3,
        SAMPLER_2D_ARRAY_SHADOW          = 0x8DC4,
        SAMPLER_CUBE_SHADOW              = 0x8DC5,
        UNSIGNED_INT_VEC2                = 0x8DC6,
        UNSIGNED_INT_VEC3                = 0x8DC7,
        UNSIGNED_INT_VEC4                = 0x8DC8,
        INT_SAMPLER_1D                   = 0x8DC9,
        INT_SAMPLER_2D                   = 0x8DCA,
        INT_SAMPLER_3D                   = 0x8DCB,
        INT_SAMPLER_CUBE                 = 0x8DCC,
        INT_SAMPLER_1D_ARRAY             = 0x8DCE,
        INT_SAMPLER_2D_ARRAY             = 0x8DCF,
        UNSIGNED_INT_SAMPLER_1D          = 0x8DD1,
        UNSIGNED_INT_SAMPLER_2D          = 0x8DD2,
        UNSIGNED_INT_SAMPLER_3D          = 0x8DD3,
        UNSIGNED_INT_SAMPLER_CUBE        = 0x8DD4,
        UNSIGNED_INT_SAMPLER_1D_ARRAY    = 0x8DD6,
        UNSIGNED_INT_SAMPLER_2D_ARRAY    = 0x8DD7,
        QUERY_WAIT                       = 0x8E13,
        QUERY_NO_WAIT                    = 0x8E14,
        QUERY_BY_REGION_WAIT             = 0x8E15,
        QUERY_BY_REGION_NO_WAIT          = 0x8E16,
        BUFFER_ACCESS_FLAGS              = 0x911F,
        BUFFER_MAP_LENGTH                = 0x9120,
        BUFFER_MAP_OFFSET                = 0x9121,

        // Core Extension: ARB_uniform_buffer_object
        UNIFORM_BUFFER                   = 0x8A11,
        UNIFORM_BUFFER_BINDING           = 0x8A28,
        UNIFORM_BUFFER_START             = 0x8A29,
        UNIFORM_BUFFER_SIZE              = 0x8A2A,
        MAX_VERTEX_UNIFORM_BLOCKS        = 0x8A2B,
        MAX_FRAGMENT_UNIFORM_BLOCKS      = 0x8A2D,
        MAX_COMBINED_UNIFORM_BLOCKS      = 0x8A2E,
        MAX_UNIFORM_BUFFER_BINDINGS      = 0x8A2F,
        MAX_UNIFORM_BLOCK_SIZE           = 0x8A30,
        MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = 0x8A31,
        MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = 0x8A33,
        UNIFORM_BUFFER_OFFSET_ALIGNMENT  = 0x8A34,
        ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH = 0x8A35,
        ACTIVE_UNIFORM_BLOCKS            = 0x8A36,
        UNIFORM_TYPE                     = 0x8A37,
        UNIFORM_SIZE                     = 0x8A38,
        UNIFORM_NAME_LENGTH              = 0x8A39,
        UNIFORM_BLOCK_INDEX              = 0x8A3A,
        UNIFORM_OFFSET                   = 0x8A3B,
        UNIFORM_ARRAY_STRIDE             = 0x8A3C,
        UNIFORM_MATRIX_STRIDE            = 0x8A3D,
        UNIFORM_IS_ROW_MAJOR             = 0x8A3E,
        UNIFORM_BLOCK_BINDING            = 0x8A3F,
        UNIFORM_BLOCK_DATA_SIZE          = 0x8A40,
        UNIFORM_BLOCK_NAME_LENGTH        = 0x8A41,
        UNIFORM_BLOCK_ACTIVE_UNIFORMS    = 0x8A42,
        UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES = 0x8A43,
        UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER = 0x8A44,
        UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46,
        INVALID_INDEX                    = 0xFFFFFFFF,
        MAX_GEOMETRY_UNIFORM_BLOCKS      = 0x8A2C,
        MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS = 0x8A32,
        UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER = 0x8A45,

        // Core Extension: ARB_copy_buffer
        COPY_READ_BUFFER                 = 0x8F36,
        COPY_WRITE_BUFFER                = 0x8F37,
        COPY_READ_BUFFER_BINDING         = 0x8F36,
        COPY_WRITE_BUFFER_BINDING        = 0x8F37,

        // Version: 3.1
        SAMPLER_2D_RECT                  = 0x8B63,
        SAMPLER_2D_RECT_SHADOW           = 0x8B64,
        SAMPLER_BUFFER                   = 0x8DC2,
        INT_SAMPLER_2D_RECT              = 0x8DCD,
        INT_SAMPLER_BUFFER               = 0x8DD0,
        UNSIGNED_INT_SAMPLER_2D_RECT     = 0x8DD5,
        UNSIGNED_INT_SAMPLER_BUFFER      = 0x8DD8,
        TEXTURE_BUFFER                   = 0x8C2A,
        MAX_TEXTURE_BUFFER_SIZE          = 0x8C2B,
        TEXTURE_BINDING_BUFFER           = 0x8C2C,
        TEXTURE_BUFFER_DATA_STORE_BINDING = 0x8C2D,
        TEXTURE_RECTANGLE                = 0x84F5,
        TEXTURE_BINDING_RECTANGLE        = 0x84F6,
        PROXY_TEXTURE_RECTANGLE          = 0x84F7,
        MAX_RECTANGLE_TEXTURE_SIZE       = 0x84F8,
        RED_SNORM                        = 0x8F90,
        RG_SNORM                         = 0x8F91,
        RGB_SNORM                        = 0x8F92,
        RGBA_SNORM                       = 0x8F93,
        R8_SNORM                         = 0x8F94,
        RG8_SNORM                        = 0x8F95,
        RGB8_SNORM                       = 0x8F96,
        RGBA8_SNORM                      = 0x8F97,
        R16_SNORM                        = 0x8F98,
        RG16_SNORM                       = 0x8F99,
        RGB16_SNORM                      = 0x8F9A,
        RGBA16_SNORM                     = 0x8F9B,
        SIGNED_NORMALIZED                = 0x8F9C,
        PRIMITIVE_RESTART                = 0x8F9D,
        PRIMITIVE_RESTART_INDEX          = 0x8F9E,

        // Legacy
        VERTEX_ARRAY = 0x8074,
        NORMAL_ARRAY = 0x8075,
        COLOR_ARRAY = 0x8076,
        TEXTURE_COORD_ARRAY = 0x8078,
        TEXTURE_ENV = 0x2300,
        TEXTURE_ENV_MODE = 0x2200,
        MODELVIEW = 0x1700,
        PROJECTION = 0x1701,
        LIGHTING = 0x0B50
    };

    // Extension: 1.1
    خارجي فراغ (CODEGEN_FUNCPTR *CullFace)(GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *FrontFace)(GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *Hint)(GLenum target, GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *سطر_Width)(GLfloat width);
    خارجي فراغ (CODEGEN_FUNCPTR *PointSize)(GLfloat size);
    خارجي فراغ (CODEGEN_FUNCPTR *PolygonMode)(GLenum face, GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *Scissor)(GLint x, GLint y, GLsizei width, GLsizei height);
    خارجي فراغ (CODEGEN_FUNCPTR *TexParameterf)(GLenum target, GLenum pname, GLfloat param);
    خارجي فراغ (CODEGEN_FUNCPTR *TexParameterfv)(GLenum target, GLenum pname, ثابت GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *TexParameteri)(GLenum target, GLenum pname, GLint param);
    خارجي فراغ (CODEGEN_FUNCPTR *TexParameteriv)(GLenum target, GLenum pname, ثابت GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *TexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *DrawBuffer)(GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *Clear)(GLbitfield mask);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearStencil)(GLint s);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearDepth)(GLdouble depth);
    خارجي فراغ (CODEGEN_FUNCPTR *StencilMask)(GLuint mask);
    خارجي فراغ (CODEGEN_FUNCPTR *ColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    خارجي فراغ (CODEGEN_FUNCPTR *DepthMask)(GLboolean flag);
    خارجي فراغ (CODEGEN_FUNCPTR *Disable)(GLenum cap);
    خارجي فراغ (CODEGEN_FUNCPTR *Enable)(GLenum cap);
    خارجي فراغ (CODEGEN_FUNCPTR *Finish)();
    خارجي فراغ (CODEGEN_FUNCPTR *Flush)();
    خارجي فراغ (CODEGEN_FUNCPTR *BlendFunc)(GLenum sfactor, GLenum dfactor);
    خارجي فراغ (CODEGEN_FUNCPTR *LogicOp)(GLenum opcode);
    خارجي فراغ (CODEGEN_FUNCPTR *StencilFunc)(GLenum func, GLint ref, GLuint mask);
    خارجي فراغ (CODEGEN_FUNCPTR *StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    خارجي فراغ (CODEGEN_FUNCPTR *DepthFunc)(GLenum func);
    خارجي فراغ (CODEGEN_FUNCPTR *PixelStoref)(GLenum pname, GLfloat param);
    خارجي فراغ (CODEGEN_FUNCPTR *PixelStorei)(GLenum pname, GLint param);
    خارجي فراغ (CODEGEN_FUNCPTR *ReadBuffer)(GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *ReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *GetBooleanv)(GLenum pname, GLboolean *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetDoublev)(GLenum pname, GLdouble *params);
    خارجي GLenum (CODEGEN_FUNCPTR *GetError)();
    خارجي فراغ (CODEGEN_FUNCPTR *GetFloatv)(GLenum pname, GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetIntegerv)(GLenum pname, GLint *params);
    خارجي ثابت GLubyte * (CODEGEN_FUNCPTR *GetString)(GLenum name);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsEnabled)(GLenum cap);
    خارجي فراغ (CODEGEN_FUNCPTR *DepthRange)(GLdouble ren_near, GLdouble ren_far);
    خارجي فراغ (CODEGEN_FUNCPTR *Viewport)(GLint x, GLint y, GLsizei width, GLsizei height);
    خارجي فراغ (CODEGEN_FUNCPTR *DrawArrays)(GLenum mode, GLint first, GLsizei عدد_2);
    خارجي فراغ (CODEGEN_FUNCPTR *DrawElements)(GLenum mode, GLsizei عدد_2, GLenum type, ثابت GLvoid *indices);
    خارجي فراغ (CODEGEN_FUNCPTR *GetPointerv)(GLenum pname, GLvoid* *params);
    خارجي فراغ (CODEGEN_FUNCPTR *PolygonOffset)(GLfloat factor, GLfloat units);
    خارجي فراغ (CODEGEN_FUNCPTR *CopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
    خارجي فراغ (CODEGEN_FUNCPTR *CopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
    خارجي فراغ (CODEGEN_FUNCPTR *CopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
    خارجي فراغ (CODEGEN_FUNCPTR *CopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
    خارجي فراغ (CODEGEN_FUNCPTR *TexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, ثابت GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *TexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *BindTexture)(GLenum target, GLuint texture);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteTextures)(GLsizei n, ثابت GLuint *textures);
    خارجي فراغ (CODEGEN_FUNCPTR *GenTextures)(GLsizei n, GLuint *textures);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsTexture)(GLuint texture);
    خارجي فراغ (CODEGEN_FUNCPTR *Indexub)(GLubyte c);
    خارجي فراغ (CODEGEN_FUNCPTR *Indexubv)(ثابت GLubyte *c);

    // Extension: 1.2
    خارجي فراغ (CODEGEN_FUNCPTR *BlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    خارجي فراغ (CODEGEN_FUNCPTR *BlendEquation)(GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *DrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei عدد_2, GLenum type, ثابت GLvoid *indices);
    خارجي فراغ (CODEGEN_FUNCPTR *TexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, ثابت GLvoid *pixels);
    خارجي فراغ (CODEGEN_FUNCPTR *CopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

    // Extension: 1.3
    خارجي فراغ (CODEGEN_FUNCPTR *ActiveTexture)(GLenum texture);
    خارجي فراغ (CODEGEN_FUNCPTR *SampleCoverage)(GLfloat value, GLboolean invert);
    خارجي فراغ (CODEGEN_FUNCPTR *CompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *CompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *CompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *CompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *CompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *CompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *GetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);

    // Extension: 1.4
    خارجي فراغ (CODEGEN_FUNCPTR *BlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    خارجي فراغ (CODEGEN_FUNCPTR *MultiDrawArrays)(GLenum mode, ثابت GLint *first, ثابت GLsizei *عدد_2, GLsizei drawcount);
    خارجي فراغ (CODEGEN_FUNCPTR *MultiDrawElements)(GLenum mode, ثابت GLsizei *عدد_2, GLenum type, ثابت GLvoid* ثابت *indices, GLsizei drawcount);
    خارجي فراغ (CODEGEN_FUNCPTR *PointParameterf)(GLenum pname, GLfloat param);
    خارجي فراغ (CODEGEN_FUNCPTR *PointParameterfv)(GLenum pname, ثابت GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *PointParameteri)(GLenum pname, GLint param);
    خارجي فراغ (CODEGEN_FUNCPTR *PointParameteriv)(GLenum pname, ثابت GLint *params);

    // Extension: 1.5
    خارجي فراغ (CODEGEN_FUNCPTR *GenQueries)(GLsizei n, GLuint *ids);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteQueries)(GLsizei n, ثابت GLuint *ids);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsQuery)(GLuint id);
    خارجي فراغ (CODEGEN_FUNCPTR *BeginQuery)(GLenum target, GLuint id);
    خارجي فراغ (CODEGEN_FUNCPTR *EndQuery)(GLenum target);
    خارجي فراغ (CODEGEN_FUNCPTR *GetQueryiv)(GLenum target, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *BindBuffer)(GLenum target, GLuint buffer);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteBuffers)(GLsizei n, ثابت GLuint *buffers);
    خارجي فراغ (CODEGEN_FUNCPTR *GenBuffers)(GLsizei n, GLuint *buffers);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsBuffer)(GLuint buffer);
    خارجي فراغ (CODEGEN_FUNCPTR *BufferData)(GLenum target, GLsizeiptr size, ثابت GLvoid *data, GLenum usage);
    خارجي فراغ (CODEGEN_FUNCPTR *BufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, ثابت GLvoid *data);
    خارجي فراغ (CODEGEN_FUNCPTR *GetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
    خارجي GLvoid* (CODEGEN_FUNCPTR *MapBuffer)(GLenum target, GLenum access);
    خارجي GLboolean (CODEGEN_FUNCPTR *UnmapBuffer)(GLenum target);
    خارجي فراغ (CODEGEN_FUNCPTR *GetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetBufferPointerv)(GLenum target, GLenum pname, GLvoid* *params);

    // Extension: 2.0
    خارجي فراغ (CODEGEN_FUNCPTR *BlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
    خارجي فراغ (CODEGEN_FUNCPTR *DrawBuffers)(GLsizei n, ثابت GLenum *bufs);
    خارجي فراغ (CODEGEN_FUNCPTR *StencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    خارجي فراغ (CODEGEN_FUNCPTR *StencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
    خارجي فراغ (CODEGEN_FUNCPTR *StencilMaskSeparate)(GLenum face, GLuint mask);
    خارجي فراغ (CODEGEN_FUNCPTR *AttachShader)(GLuint program, GLuint shader);
    خارجي فراغ (CODEGEN_FUNCPTR *BindAttribLocation)(GLuint program, GLuint index, ثابت GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *CompileShader)(GLuint shader);
    خارجي GLuint (CODEGEN_FUNCPTR *CreateProgram)();
    خارجي GLuint (CODEGEN_FUNCPTR *CreateShader)(GLenum type);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteProgram)(GLuint program);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteShader)(GLuint shader);
    خارجي فراغ (CODEGEN_FUNCPTR *DetachShader)(GLuint program, GLuint shader);
    خارجي فراغ (CODEGEN_FUNCPTR *DisableVertexAttribArray)(GLuint index);
    خارجي فراغ (CODEGEN_FUNCPTR *EnableVertexAttribArray)(GLuint index);
    خارجي فراغ (CODEGEN_FUNCPTR *GetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *GetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *GetAttachedShaders)(GLuint program, GLsizei max_عدد, GLsizei *عدد_2, GLuint *obj);
    خارجي GLint (CODEGEN_FUNCPTR *GetAttribLocation)(GLuint program, ثابت GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *GetProgramiv)(GLuint program, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    خارجي فراغ (CODEGEN_FUNCPTR *GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    خارجي فراغ (CODEGEN_FUNCPTR *GetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
    خارجي GLint (CODEGEN_FUNCPTR *GetUniformLocation)(GLuint program, ثابت GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *GetUniformfv)(GLuint program, GLint location, GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetUniformiv)(GLuint program, GLint location, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsProgram)(GLuint program);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsShader)(GLuint shader);
    خارجي فراغ (CODEGEN_FUNCPTR *LinkProgram)(GLuint program);
    خارجي فراغ (CODEGEN_FUNCPTR *ShaderSource)(GLuint shader, GLsizei عدد_2, ثابت GLchar* ثابت *string, ثابت GLint *length);
    خارجي فراغ (CODEGEN_FUNCPTR *UseProgram)(GLuint program);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform1f)(GLint location, GLfloat v0);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform2f)(GLint location, GLfloat v0, GLfloat v1);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform1i)(GLint location, GLint v0);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform2i)(GLint location, GLint v0, GLint v1);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform1fv)(GLint location, GLsizei عدد_2, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform2fv)(GLint location, GLsizei عدد_2, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform3fv)(GLint location, GLsizei عدد_2, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform4fv)(GLint location, GLsizei عدد_2, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform1iv)(GLint location, GLsizei عدد_2, ثابت GLint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform2iv)(GLint location, GLsizei عدد_2, ثابت GLint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform3iv)(GLint location, GLsizei عدد_2, ثابت GLint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform4iv)(GLint location, GLsizei عدد_2, ثابت GLint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix2fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix3fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix4fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *ValidateProgram)(GLuint program);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ثابت GLvoid *pointer);

    // Extension: 2.1
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix2x3fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix3x2fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix2x4fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix4x2fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix3x4fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformMatrix4x3fv)(GLint location, GLsizei عدد_2, GLboolean transpose, ثابت GLfloat *value);

    // Extension: ARB_vertex_array_object
    خارجي فراغ (CODEGEN_FUNCPTR *BindVertexArray)(GLuint ren_array);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteVertexArrays)(GLsizei n, ثابت GLuint *arrays);
    خارجي فراغ (CODEGEN_FUNCPTR *GenVertexArrays)(GLsizei n, GLuint *arrays);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsVertexArray)(GLuint ren_array);

    // Extension: ARB_map_buffer_range
    خارجي GLvoid* (CODEGEN_FUNCPTR *MapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    خارجي فراغ (CODEGEN_FUNCPTR *FlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);

    // Extension: ARB_framebuffer_object
    خارجي GLboolean (CODEGEN_FUNCPTR *IsRenderbuffer)(GLuint renderbuffer);
    خارجي فراغ (CODEGEN_FUNCPTR *BindRenderbuffer)(GLenum target, GLuint renderbuffer);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteRenderbuffers)(GLsizei n, ثابت GLuint *renderbuffers);
    خارجي فراغ (CODEGEN_FUNCPTR *GenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
    خارجي فراغ (CODEGEN_FUNCPTR *RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
    خارجي فراغ (CODEGEN_FUNCPTR *GetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsFramebuffer)(GLuint framebuffer);
    خارجي فراغ (CODEGEN_FUNCPTR *BindFramebuffer)(GLenum target, GLuint framebuffer);
    خارجي فراغ (CODEGEN_FUNCPTR *DeleteFramebuffers)(GLsizei n, ثابت GLuint *framebuffers);
    خارجي فراغ (CODEGEN_FUNCPTR *GenFramebuffers)(GLsizei n, GLuint *framebuffers);
    خارجي GLenum (CODEGEN_FUNCPTR *CheckFramebufferStatus)(GLenum target);
    خارجي فراغ (CODEGEN_FUNCPTR *FramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    خارجي فراغ (CODEGEN_FUNCPTR *FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    خارجي فراغ (CODEGEN_FUNCPTR *FramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
    خارجي فراغ (CODEGEN_FUNCPTR *FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    خارجي فراغ (CODEGEN_FUNCPTR *GetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GenerateMipmap)(GLenum target);
    خارجي فراغ (CODEGEN_FUNCPTR *BlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
    خارجي فراغ (CODEGEN_FUNCPTR *RenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
    خارجي فراغ (CODEGEN_FUNCPTR *FramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

    // Extension: 3.0
    خارجي فراغ (CODEGEN_FUNCPTR *ColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
    خارجي فراغ (CODEGEN_FUNCPTR *GetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
    خارجي فراغ (CODEGEN_FUNCPTR *GetIntegeri_v)(GLenum target, GLuint index, GLint *data);
    خارجي فراغ (CODEGEN_FUNCPTR *Enablei)(GLenum target, GLuint index);
    خارجي فراغ (CODEGEN_FUNCPTR *Disablei)(GLenum target, GLuint index);
    خارجي GLboolean (CODEGEN_FUNCPTR *IsEnabledi)(GLenum target, GLuint index);
    خارجي فراغ (CODEGEN_FUNCPTR *BeginTransformFeedback)(GLenum primitiveMode);
    خارجي فراغ (CODEGEN_FUNCPTR *EndTransformFeedback)();
    خارجي فراغ (CODEGEN_FUNCPTR *BindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
    خارجي فراغ (CODEGEN_FUNCPTR *BindBufferBase)(GLenum target, GLuint index, GLuint buffer);
    خارجي فراغ (CODEGEN_FUNCPTR *TransformFeedbackVaryings)(GLuint program, GLsizei عدد_2, ثابت GLchar* ثابت *varyings, GLenum bufferMode);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *ClampColor)(GLenum target, GLenum clamp);
    خارجي فراغ (CODEGEN_FUNCPTR *BeginConditionalRender)(GLuint id, GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *EndConditionalRender)();
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, ثابت GLvoid *pointer);
    خارجي فراغ (CODEGEN_FUNCPTR *GetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI1i)(GLuint index, GLint x);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI2i)(GLuint index, GLint x, GLint y);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI1ui)(GLuint index, GLuint x);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI1iv)(GLuint index, ثابت GLint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI2iv)(GLuint index, ثابت GLint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI3iv)(GLuint index, ثابت GLint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4iv)(GLuint index, ثابت GLint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI1uiv)(GLuint index, ثابت GLuint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI2uiv)(GLuint index, ثابت GLuint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI3uiv)(GLuint index, ثابت GLuint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4uiv)(GLuint index, ثابت GLuint *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4bv)(GLuint index, ثابت GLbyte *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4sv)(GLuint index, ثابت GLshort *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4ubv)(GLuint index, ثابت GLubyte *v);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexAttribI4usv)(GLuint index, ثابت GLushort *v);
    خارجي فراغ (CODEGEN_FUNCPTR *GetUniformuiv)(GLuint program, GLint location, GLuint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *BindFragDataLocation)(GLuint program, GLuint color, ثابت GLchar *name);
    خارجي GLint (CODEGEN_FUNCPTR *GetFragDataLocation)(GLuint program, ثابت GLchar *name);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform1ui)(GLint location, GLuint v0);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform2ui)(GLint location, GLuint v0, GLuint v1);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform1uiv)(GLint location, GLsizei عدد_2, ثابت GLuint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform2uiv)(GLint location, GLsizei عدد_2, ثابت GLuint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform3uiv)(GLint location, GLsizei عدد_2, ثابت GLuint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *Uniform4uiv)(GLint location, GLsizei عدد_2, ثابت GLuint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *TexParameterIiv)(GLenum target, GLenum pname, ثابت GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *TexParameterIuiv)(GLenum target, GLenum pname, ثابت GLuint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearBufferiv)(GLenum buffer, GLint drawbuffer, ثابت GLint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearBufferuiv)(GLenum buffer, GLint drawbuffer, ثابت GLuint *value);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearBufferfv)(GLenum buffer, GLint drawbuffer, ثابت GLfloat *value);
    خارجي فراغ (CODEGEN_FUNCPTR *ClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
    خارجي ثابت GLubyte * (CODEGEN_FUNCPTR *GetStringi)(GLenum name, GLuint index);

    // Extension: ARB_uniform_buffer_object
    خارجي فراغ (CODEGEN_FUNCPTR *GetUniformIndices)(GLuint program, GLsizei uniform_عدد, ثابت GLchar* ثابت *uniformNames, GLuint *uniformIndices);
    خارجي فراغ (CODEGEN_FUNCPTR *GetActiveUniformsiv)(GLuint program, GLsizei uniform_عدد, ثابت GLuint *uniformIndices, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
    خارجي GLuint (CODEGEN_FUNCPTR *GetUniformBlockIndex)(GLuint program, ثابت GLchar *uniformBlockName);
    خارجي فراغ (CODEGEN_FUNCPTR *GetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
    خارجي فراغ (CODEGEN_FUNCPTR *GetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
    خارجي فراغ (CODEGEN_FUNCPTR *UniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

    // Extension: ARB_copy_buffer
    خارجي فراغ (CODEGEN_FUNCPTR *CopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

    // Extension: 3.1
    خارجي فراغ (CODEGEN_FUNCPTR *DrawArraysInstanced)(GLenum mode, GLint first, GLsizei عدد_2, GLsizei instancecount);
    خارجي فراغ (CODEGEN_FUNCPTR *DrawElementsInstanced)(GLenum mode, GLsizei عدد_2, GLenum type, ثابت GLvoid *indices, GLsizei instancecount);
    خارجي فراغ (CODEGEN_FUNCPTR *TexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
    خارجي فراغ (CODEGEN_FUNCPTR *PrimitiveRestartIndex)(GLuint index);

    // Legacy
    خارجي فراغ (CODEGEN_FUNCPTR *EnableClientState)(GLenum cap);
    خارجي فراغ (CODEGEN_FUNCPTR *DisableClientState)(GLenum cap);
    خارجي فراغ (CODEGEN_FUNCPTR *VertexPointer)(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr);
    خارجي فراغ (CODEGEN_FUNCPTR *NormalPointer)(GLenum type, GLsizei stride, ثابت GLvoid *ptr);
    خارجي فراغ (CODEGEN_FUNCPTR *ColorPointer)(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr);
    خارجي فراغ (CODEGEN_FUNCPTR *TexCoordPointer)(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr);
    خارجي فراغ (CODEGEN_FUNCPTR *TexEnvi)(GLenum target, GLenum pname, GLint param);
    خارجي فراغ (CODEGEN_FUNCPTR *MatrixMode)(GLenum mode);
    خارجي فراغ (CODEGEN_FUNCPTR *LoadIdentity)(فراغ);
    خارجي فراغ (CODEGEN_FUNCPTR *Ortho)(GLdouble يسار, GLdouble يمين, GLdouble bottom, GLdouble قمة_2, GLdouble near_val, GLdouble far_val);
    خارجي فراغ (CODEGEN_FUNCPTR *Color3d)(GLdouble red, GLdouble green, GLdouble blue);
}

#endif // OPENGL_NOLOAD_STYLE_HPP
