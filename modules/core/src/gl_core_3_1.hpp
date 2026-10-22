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

#నిర్వచితం_కాకపోతే OPENGL_NOLOAD_STYLE_HPP
#నిర్వచించు OPENGL_NOLOAD_STYLE_HPP

#ఒకవేళ defined(__gl_h_) || defined(__GL_H__)
#దోషం Attempt గమ్యం include స్వయంచాలక-generated శీర్షిక after including gl.h
#అయితే_ముగింపు
#ఒకవేళ defined(__glext_h_) || defined(__GLEXT_H_)
#దోషం Attempt గమ్యం include స్వయంచాలక-generated శీర్షిక after including glext.h
#అయితే_ముగింపు
#ఒకవేళ defined(__gl_ATI_h_)
#దోషం Attempt గమ్యం include స్వయంచాలక-generated శీర్షిక after including glATI.h
#అయితే_ముగింపు

#నిర్వచించు __gl_h_
#నిర్వచించు __GL_H__
#నిర్వచించు __glext_h_
#నిర్వచించు __GLEXT_H_
#నిర్వచించు __gl_ATI_h_

#నిర్వచితం_కాకపోతే APIENTRY
    #ఒకవేళ defined(__MINGW32__)
        #నిర్వచితం_కాకపోతే WIN32_LEAN_AND_సగటు
            #నిర్వచించు WIN32_LEAN_AND_సగటు 1
        #అయితే_ముగింపు
        #నిర్వచితం_కాకపోతే NOMINMAX
            #నిర్వచించు NOMINMAX
        #అయితే_ముగింపు
        #అన్తర్భావ <windows.h>
    #లేదావేళ (defined(_MSC_VER) && _MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #నిర్వచితం_కాకపోతే WIN32_LEAN_AND_సగటు
            #నిర్వచించు WIN32_LEAN_AND_సగటు 1
        #అయితే_ముగింపు
        #నిర్వచితం_కాకపోతే NOMINMAX
            #నిర్వచించు NOMINMAX
        #అయితే_ముగింపు
        #అన్తర్భావ <windows.h>
    #లేకపోతే
        #నిర్వచించు APIENTRY
    #అయితే_ముగింపు
#అయితే_ముగింపు // APIENTRY

#నిర్వచితం_కాకపోతే CODEGEN_FUNCPTR
    #నిర్వచించు CODEGEN_REMOVE_FUNCPTR
    #ఒకవేళ defined(_WIN32)
        #నిర్వచించు CODEGEN_FUNCPTR APIENTRY
    #లేకపోతే
        #నిర్వచించు CODEGEN_FUNCPTR
    #అయితే_ముగింపు
#అయితే_ముగింపు // CODEGEN_FUNCPTR

#నిర్వచితం_కాకపోతే GL_ఎక్కించు_GEN_BASIC_OPENGL_TYPEDEFS
#నిర్వచించు GL_ఎక్కించు_GEN_BASIC_OPENGL_TYPEDEFS
    రకనిర్వచనం గుర్తులేని పూర్ణాంకం GLenum;
    రకనిర్వచనం గుర్తులేని అక్షరం GLboolean;
    రకనిర్వచనం గుర్తులేని పూర్ణాంకం GLbitfield;
    రకనిర్వచనం గుర్తుగల అక్షరం GLbyte;
    రకనిర్వచనం పొట్టి GLshort;
    రకనిర్వచనం పూర్ణాంకం GLint;
    రకనిర్వచనం పూర్ణాంకం GLsizei;
    రకనిర్వచనం గుర్తులేని అక్షరం GLubyte;
    రకనిర్వచనం గుర్తులేని పొట్టి GLushort;
    రకనిర్వచనం గుర్తులేని పూర్ణాంకం GLuint;
    రకనిర్వచనం దశాంశం GLfloat;
    రకనిర్వచనం దశాంశం GLclampf;
    రకనిర్వచనం ద్విగుణ GLdouble;
    రకనిర్వచనం ద్విగుణ GLclampd;
    #నిర్వచించు GLvoid శూన్య
#అయితే_ముగింపు // GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS

#అన్తర్భావ <stddef.h>

#నిర్వచితం_కాకపోతే GL_సంస్కరణ_2_0
    // GL type for program/shader text
    రకనిర్వచనం అక్షరం GLchar;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_సంస్కరణ_1_5
    // GL types for handling large vertex buffer objects
    రకనిర్వచనం ptrdiff_t GLintptr;
    రకనిర్వచనం ptrdiff_t GLsizeiptr;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_ARB_శీర్షం_బఫరు_object
    // GL types for handling large vertex buffer objects
    రకనిర్వచనం ptrdiff_t GLintptrARB;
    రకనిర్వచనం ptrdiff_t GLsizeiptrARB;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_ARB_shader_objects
    // GL types for program/shader text and shader object handles
    రకనిర్వచనం అక్షరం GLcharARB;
    రకనిర్వచనం గుర్తులేని పూర్ణాంకం GLhandleARB;
#అయితే_ముగింపు

// GL type for "half" precision (s10e5) float data in host memory
#నిర్వచితం_కాకపోతే GL_ARB_half_float_pixel
    రకనిర్వచనం గుర్తులేని పొట్టి GLhalfARB;
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే GL_NV_half_float
    రకనిర్వచనం గుర్తులేని పొట్టి GLhalfNV;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GLEXT_64_TYPES_DEFINED
    // This code block is duplicated in glxext.h, so must be protected
    #నిర్వచించు GLEXT_64_TYPES_DEFINED

    // Define int32_t, int64_t, and uint64_t types for UST/MSC
    // (as used in the GL_EXT_timer_query extension)
    #ఒకవేళ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #అన్తర్భావ <inttypes.h>
    #లేదావేళ defined(__sun__) || defined(__digital__)
        #అన్తర్భావ <inttypes.h>
        #ఒకవేళ defined(__STDC__)
            #ఒకవేళ defined(__arch64__) || defined(_LP64)
                రకనిర్వచనం దీర్ఘ పూర్ణాంకం int64_t;
                రకనిర్వచనం గుర్తులేని దీర్ఘ పూర్ణాంకం uint64_t;
            #లేకపోతే
                రకనిర్వచనం దీర్ఘ దీర్ఘ పూర్ణాంకం int64_t;
                రకనిర్వచనం గుర్తులేని దీర్ఘ దీర్ఘ పూర్ణాంకం uint64_t;
            #అయితే_ముగింపు // __arch64__
        #అయితే_ముగింపు // __STDC__
    #లేదావేళ defined( __VMS ) || defined(__sgi)
        #అన్తర్భావ <inttypes.h>
    #లేదావేళ defined(__SCO__) || defined(__USLC__)
        #అన్తర్భావ <stdint.h>
    #లేదావేళ defined(__UNIXOS2__) || defined(__SOL64__)
        రకనిర్వచనం దీర్ఘ పూర్ణాంకం int32_t;
        రకనిర్వచనం దీర్ఘ దీర్ఘ పూర్ణాంకం int64_t;
        రకనిర్వచనం గుర్తులేని దీర్ఘ దీర్ఘ పూర్ణాంకం uint64_t;
    #లేదావేళ defined(_WIN32) && defined(__GNUC__)
        #అన్తర్భావ <stdint.h>
    #లేదావేళ defined(_WIN32)
        రకనిర్వచనం __int32 int32_t;
        రకనిర్వచనం __int64 int64_t;
        రకనిర్వచనం గుర్తులేని __int64 uint64_t;
    #లేకపోతే
        // Fallback if nothing above works
        #అన్తర్భావ <inttypes.h>
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_EXT_timer_query
    రకనిర్వచనం int64_t GLint64EXT;
    రకనిర్వచనం uint64_t GLuint64EXT;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_ARB_sync
    రకనిర్వచనం int64_t GLint64;
    రకనిర్వచనం uint64_t GLuint64;
    రకనిర్వచనం నిర్మాణం __GLsync *GLsync;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_ARB_cl_event
    // These incomplete types let us declare types compatible with OpenCL's cl_context and cl_event
    నిర్మాణం _cl_context;
    నిర్మాణం _cl_event;
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_ARB_debug_output
    రకనిర్వచనం శూన్య (APIENTRY *GLDEBUGPROCARB)(GLenum ఆధారం,GLenum type,GLuint id,GLenum severity,GLsizei length,స్థిరరాశి GLchar *message,GLvoid *userParam);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_AMD_debug_output
    రకనిర్వచనం శూన్య (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,స్థిరరాశి GLchar *message,GLvoid *userParam);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_KHR_debug
    రకనిర్వచనం శూన్య (APIENTRY *GLDEBUGPROC)(GLenum ఆధారం,GLenum type,GLuint id,GLenum severity,GLsizei length,స్థిరరాశి GLchar *message,GLvoid *userParam);
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే GL_NV_vdpau_interop
    రకనిర్వచనం GLintptr GLvdpauSurfaceNV;
#అయితే_ముగింపు

నామాకాశ gl
{
    గణన
    {
        // Version: 1.1
        లోతు_బఫరు_బిట్టు                 = 0x00000100,
        STENCIL_బఫరు_బిట్టు               = 0x00000400,
        COLOR_బఫరు_బిట్టు                 = 0x00004000,
        FALSE_                           = 0,
        TRUE_                            = 1,
        POINTS                           = 0x0000,
        పంక్తులు                            = 0x0001,
        పంక్తి_LOOP                        = 0x0002,
        పంక్తి_STRIP                       = 0x0003,
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
        FRONT_ఎడమ                       = 0x0400,
        FRONT_కుడి                      = 0x0401,
        వెనుక_ఎడమ                        = 0x0402,
        వెనుక_కుడి                       = 0x0403,
        FRONT                            = 0x0404,
        వెనుక                             = 0x0405,
        ఎడమ_2                             = 0x0406,
        కుడి_2                            = 0x0407,
        FRONT_AND_వెనుక                   = 0x0408,
        NO_ERROR_                        = 0,
        INVALID_ENUM                     = 0x0500,
        INVALID_విలువ                    = 0x0501,
        INVALID_OPERATION                = 0x0502,
        బహిర్గతం_యొక్క_జ్ఞాపకం                    = 0x0505,
        CW                               = 0x0900,
        CCW                              = 0x0901,
        బిందువు_కొలత_2                       = 0x0B11,
        బిందువు_కొలత_RANGE                 = 0x0B12,
        బిందువు_కొలత_GRANULARITY           = 0x0B13,
        పంక్తి_SMOOTH                      = 0x0B20,
        పంక్తి_వెడల్పు_2                       = 0x0B21,
        పంక్తి_వెడల్పు_RANGE                 = 0x0B22,
        పంక్తి_వెడల్పు_GRANULARITY           = 0x0B23,
        POLYGON_బహుళకం                     = 0x0B40,
        POLYGON_SMOOTH                   = 0x0B41,
        CULL_FACE                        = 0x0B44,
        CULL_FACE_బహుళకం                   = 0x0B45,
        FRONT_FACE                       = 0x0B46,
        లోతు_RANGE                      = 0x0B70,
        లోతు_TEST                       = 0x0B71,
        లోతు_WRITEMASK                  = 0x0B72,
        లోతు_CLEAR_విలువ                = 0x0B73,
        లోతు_FUNC                       = 0x0B74,
        STENCIL_TEST                     = 0x0B90,
        STENCIL_CLEAR_విలువ              = 0x0B91,
        STENCIL_FUNC                     = 0x0B92,
        STENCIL_విలువ_ముసుగు               = 0x0B93,
        STENCIL_FAIL                     = 0x0B94,
        STENCIL_PASS_లోతు_FAIL          = 0x0B95,
        STENCIL_PASS_లోతు_PASS          = 0x0B96,
        STENCIL_REF                      = 0x0B97,
        STENCIL_WRITEMASK                = 0x0B98,
        VIEWPORT                         = 0x0BA2,
        DITHER                           = 0x0BD0,
        BLEND_DST                        = 0x0BE0,
        BLEND_SRC                        = 0x0BE1,
        BLEND                            = 0x0BE2,
        LOGIC_OP_బహుళకం                    = 0x0BF0,
        COLOR_LOGIC_OP                   = 0x0BF2,
        DRAW_బఫరు                      = 0x0C01,
        READ_బఫరు                      = 0x0C02,
        SCISSOR_BOX                      = 0x0C10,
        SCISSOR_TEST                     = 0x0C11,
        COLOR_CLEAR_విలువ                = 0x0C22,
        COLOR_WRITEMASK                  = 0x0C23,
        DOUBLEBUFFER                     = 0x0C32,
        STEREO                           = 0x0C33,
        పంక్తి_SMOOTH_HINT                 = 0x0C52,
        POLYGON_SMOOTH_HINT              = 0x0C53,
        UNPACK_మార్పిడి_బైట్లు                = 0x0CF0,
        UNPACK_LSB_మొదటి                 = 0x0CF1,
        UNPACK_అడ్డువరుస_నిడివి                = 0x0CF2,
        UNPACK_SKIP_అడ్డువరుసలు                 = 0x0CF3,
        UNPACK_SKIP_PIXELS               = 0x0CF4,
        UNPACK_ALIGNMENT                 = 0x0CF5,
        PACK_మార్పిడి_బైట్లు                  = 0x0D00,
        PACK_LSB_మొదటి                   = 0x0D01,
        PACK_అడ్డువరుస_నిడివి                  = 0x0D02,
        PACK_SKIP_అడ్డువరుసలు                   = 0x0D03,
        PACK_SKIP_PIXELS                 = 0x0D04,
        PACK_ALIGNMENT                   = 0x0D05,
        MAX_TEXTURE_కొలత                 = 0x0D33,
        MAX_VIEWPORT_DIMS                = 0x0D3A,
        SUBPIXEL_బిట్లు                    = 0x0D50,
        TEXTURE_1D                       = 0x0DE0,
        TEXTURE_2D                       = 0x0DE1,
        POLYGON_స్థానభ్రంశం_UNITS             = 0x2A00,
        POLYGON_స్థానభ్రంశం_బిందువు             = 0x2A01,
        POLYGON_స్థానభ్రంశం_పంక్తి              = 0x2A02,
        POLYGON_స్థానభ్రంశం_FILL              = 0x8037,
        POLYGON_స్థానభ్రంశం_గుణకం            = 0x8038,
        TEXTURE_BINDING_1D               = 0x8068,
        TEXTURE_BINDING_2D               = 0x8069,
        TEXTURE_వెడల్పు                    = 0x1000,
        TEXTURE_ఎత్తు                   = 0x1001,
        TEXTURE_అంతర్గత_FORMAT          = 0x1003,
        TEXTURE_BORDER_COLOR             = 0x1004,
        TEXTURE_RED_కొలత                 = 0x805C,
        TEXTURE_GREEN_కొలత               = 0x805D,
        TEXTURE_BLUE_కొలత                = 0x805E,
        TEXTURE_ALPHA_కొలత               = 0x805F,
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
        AND_తిరగేయి                      = 0x1502,
        COPY                             = 0x1503,
        AND_INVERTED                     = 0x1504,
        NOOP                             = 0x1505,
        XOR                              = 0x1506,
        OR                               = 0x1507,
        NOR                              = 0x1508,
        EQUIV                            = 0x1509,
        INVERT                           = 0x150A,
        OR_తిరగేయి                       = 0x150B,
        COPY_INVERTED                    = 0x150C,
        OR_INVERTED                      = 0x150D,
        NAND                             = 0x150E,
        సమితి_2                              = 0x150F,
        TEXTURE                          = 0x1702,
        COLOR                            = 0x1800,
        లోతు_3                            = 0x1801,
        STENCIL                          = 0x1802,
        STENCIL_సూచీ                    = 0x1901,
        లోతు_COMPONENT                  = 0x1902,
        RED                              = 0x1903,
        GREEN                            = 0x1904,
        BLUE                             = 0x1905,
        ALPHA                            = 0x1906,
        RGB                              = 0x1907,
        RGBA                             = 0x1908,
        బిందువు_2                            = 0x1B00,
        పంక్తి_2                             = 0x1B01,
        FILL                             = 0x1B02,
        KEEP                             = 0x1E00,
        REPLACE                          = 0x1E01,
        INCR                             = 0x1E02,
        DECR                             = 0x1E03,
        VENDOR                           = 0x1F00,
        RENDERER                         = 0x1F01,
        సంస్కరణ_2                         = 0x1F02,
        EXTENSIONS                       = 0x1F03,
        సమీప                          = 0x2600,
        రేఖీయ                           = 0x2601,
        సమీప_MIPMAP_సమీప           = 0x2700,
        రేఖీయ_MIPMAP_సమీప            = 0x2701,
        సమీప_MIPMAP_రేఖీయ            = 0x2702,
        రేఖీయ_MIPMAP_రేఖీయ             = 0x2703,
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
        FUNC_కూడు                         = 0x8006,
        MIN                              = 0x8007,
        MAX                              = 0x8008,
        BLEND_EQUATION                   = 0x8009,
        FUNC_వ్యవకలనం                    = 0x800A,
        FUNC_తిరగేయి_వ్యవకలనం            = 0x800B,
        CONVOLUTION_1D                   = 0x8010,
        CONVOLUTION_2D                   = 0x8011,
        SEPARABLE_2D                     = 0x8012,
        CONVOLUTION_BORDER_బహుళకం          = 0x8013,
        CONVOLUTION_FILTER_SCALE         = 0x8014,
        CONVOLUTION_FILTER_BIAS          = 0x8015,
        REDUCE                           = 0x8016,
        CONVOLUTION_FORMAT               = 0x8017,
        CONVOLUTION_వెడల్పు                = 0x8018,
        CONVOLUTION_ఎత్తు               = 0x8019,
        MAX_CONVOLUTION_వెడల్పు            = 0x801A,
        MAX_CONVOLUTION_ఎత్తు           = 0x801B,
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
        HISTOGRAM_వెడల్పు                  = 0x8026,
        HISTOGRAM_FORMAT                 = 0x8027,
        HISTOGRAM_RED_కొలత               = 0x8028,
        HISTOGRAM_GREEN_కొలత             = 0x8029,
        HISTOGRAM_BLUE_కొలత              = 0x802A,
        HISTOGRAM_ALPHA_కొలత             = 0x802B,
        HISTOGRAM_LUMINANCE_కొలత         = 0x802C,
        HISTOGRAM_SINK                   = 0x802D,
        MINMAX                           = 0x802E,
        MINMAX_FORMAT                    = 0x802F,
        MINMAX_SINK                      = 0x8030,
        పట్టిక_TOO_LARGE                  = 0x8031,
        COLOR_మాతృక                     = 0x80B1,
        COLOR_మాతృక_దొంతర_లోతు         = 0x80B2,
        MAX_COLOR_మాతృక_దొంతర_లోతు     = 0x80B3,
        POST_COLOR_మాతృక_RED_SCALE      = 0x80B4,
        POST_COLOR_మాతృక_GREEN_SCALE    = 0x80B5,
        POST_COLOR_మాతృక_BLUE_SCALE     = 0x80B6,
        POST_COLOR_మాతృక_ALPHA_SCALE    = 0x80B7,
        POST_COLOR_మాతృక_RED_BIAS       = 0x80B8,
        POST_COLOR_మాతృక_GREEN_BIAS     = 0x80B9,
        POST_COLOR_మాతృక_BLUE_BIAS      = 0x80BA,
        POST_COLOR_మాతృక_ALPHA_BIAS     = 0x80BB,
        COLOR_పట్టిక                      = 0x80D0,
        POST_CONVOLUTION_COLOR_పట్టిక     = 0x80D1,
        POST_COLOR_మాతృక_COLOR_పట్టిక    = 0x80D2,
        PROXY_COLOR_పట్టిక                = 0x80D3,
        PROXY_POST_CONVOLUTION_COLOR_పట్టిక = 0x80D4,
        PROXY_POST_COLOR_మాతృక_COLOR_పట్టిక = 0x80D5,
        COLOR_పట్టిక_SCALE                = 0x80D6,
        COLOR_పట్టిక_BIAS                 = 0x80D7,
        COLOR_పట్టిక_FORMAT               = 0x80D8,
        COLOR_పట్టిక_వెడల్పు                = 0x80D9,
        COLOR_పట్టిక_RED_కొలత             = 0x80DA,
        COLOR_పట్టిక_GREEN_కొలత           = 0x80DB,
        COLOR_పట్టిక_BLUE_కొలత            = 0x80DC,
        COLOR_పట్టిక_ALPHA_కొలత           = 0x80DD,
        COLOR_పట్టిక_LUMINANCE_కొలత       = 0x80DE,
        COLOR_పట్టిక_INTENSITY_కొలత       = 0x80DF,
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
        PACK_IMAGE_ఎత్తు                = 0x806C,
        UNPACK_SKIP_IMAGES               = 0x806D,
        UNPACK_IMAGE_ఎత్తు              = 0x806E,
        TEXTURE_3D                       = 0x806F,
        PROXY_TEXTURE_3D                 = 0x8070,
        TEXTURE_లోతు                    = 0x8071,
        TEXTURE_WRAP_R                   = 0x8072,
        MAX_3_D_TEXTURE_కొలత              = 0x8073,
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
        CLAMP_గమ్యం_అంచు                    = 0x812F,
        TEXTURE_MIN_LOD                  = 0x813A,
        TEXTURE_MAX_LOD                  = 0x813B,
        TEXTURE_పునాది_LEVEL               = 0x813C,
        TEXTURE_MAX_LEVEL                = 0x813D,
        SMOOTH_బిందువు_కొలత_RANGE          = 0x0B12,
        SMOOTH_బిందువు_కొలత_GRANULARITY    = 0x0B13,
        SMOOTH_పంక్తి_వెడల్పు_RANGE          = 0x0B22,
        SMOOTH_పంక్తి_వెడల్పు_GRANULARITY    = 0x0B23,
        ALIASED_పంక్తి_వెడల్పు_RANGE         = 0x846E,

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
        చురుకైన_TEXTURE                   = 0x84E0,
        MULTISAMPLE                      = 0x809D,
        SAMPLE_ALPHA_గమ్యం_COVERAGE         = 0x809E,
        SAMPLE_ALPHA_గమ్యం_ONE              = 0x809F,
        SAMPLE_COVERAGE                  = 0x80A0,
        SAMPLE_BUFFERS                   = 0x80A8,
        SAMPLES                          = 0x80A9,
        SAMPLE_COVERAGE_విలువ            = 0x80AA,
        SAMPLE_COVERAGE_INVERT           = 0x80AB,
        TEXTURE_CUBE_MAP                 = 0x8513,
        TEXTURE_BINDING_CUBE_MAP         = 0x8514,
        TEXTURE_CUBE_MAP_ధనాత్మక_X      = 0x8515,
        TEXTURE_CUBE_MAP_NEGATIVE_X      = 0x8516,
        TEXTURE_CUBE_MAP_ధనాత్మక_Y      = 0x8517,
        TEXTURE_CUBE_MAP_NEGATIVE_Y      = 0x8518,
        TEXTURE_CUBE_MAP_ధనాత్మక_Z      = 0x8519,
        TEXTURE_CUBE_MAP_NEGATIVE_Z      = 0x851A,
        PROXY_TEXTURE_CUBE_MAP           = 0x851B,
        MAX_CUBE_MAP_TEXTURE_కొలత        = 0x851C,
        COMPRESSED_RGB                   = 0x84ED,
        COMPRESSED_RGBA                  = 0x84EE,
        TEXTURE_COMPRESSION_HINT         = 0x84EF,
        TEXTURE_COMPRESSED_IMAGE_కొలత    = 0x86A0,
        TEXTURE_COMPRESSED               = 0x86A1,
        అంకె_COMPRESSED_TEXTURE_FORMATS   = 0x86A2,
        COMPRESSED_TEXTURE_FORMATS       = 0x86A3,
        CLAMP_గమ్యం_BORDER                  = 0x812D,

        // Version: 1.4
        BLEND_DST_RGB                    = 0x80C8,
        BLEND_SRC_RGB                    = 0x80C9,
        BLEND_DST_ALPHA                  = 0x80CA,
        BLEND_SRC_ALPHA                  = 0x80CB,
        బిందువు_FADE_పరిమితి_కొలత        = 0x8128,
        లోతు_COMPONENT16                = 0x81A5,
        లోతు_COMPONENT24                = 0x81A6,
        లోతు_COMPONENT32                = 0x81A7,
        MIRRORED_REPEAT                  = 0x8370,
        MAX_TEXTURE_LOD_BIAS             = 0x84FD,
        TEXTURE_LOD_BIAS                 = 0x8501,
        INCR_WRAP                        = 0x8507,
        DECR_WRAP                        = 0x8508,
        TEXTURE_లోతు_కొలత               = 0x884A,
        TEXTURE_COMPARE_బహుళకం             = 0x884C,
        TEXTURE_COMPARE_FUNC             = 0x884D,

        // Version: 1.5
        బఫరు_కొలత_2                      = 0x8764,
        బఫరు_USAGE                     = 0x8765,
        QUERY_COUNTER_బిట్లు               = 0x8864,
        CURRENT_QUERY                    = 0x8865,
        QUERY_ఫలితం                     = 0x8866,
        QUERY_ఫలితం_అందుబాటు           = 0x8867,
        ARRAY_బఫరు                     = 0x8892,
        ELEMENT_ARRAY_బఫరు             = 0x8893,
        ARRAY_బఫరు_BINDING             = 0x8894,
        ELEMENT_ARRAY_బఫరు_BINDING     = 0x8895,
        శీర్షం_ATTRIB_ARRAY_బఫరు_BINDING = 0x889F,
        READ_ONLY                        = 0x88B8,
        WRITE_ONLY                       = 0x88B9,
        READ_WRITE                       = 0x88BA,
        బఫరు_ACCESS                    = 0x88BB,
        బఫరు_MAPPED                    = 0x88BC,
        బఫరు_MAP_సూచిక               = 0x88BD,
        ప్రవాహం_DRAW                      = 0x88E0,
        ప్రవాహం_READ                      = 0x88E1,
        ప్రవాహం_COPY                      = 0x88E2,
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
        శీర్షం_ATTRIB_ARRAY_ENABLED      = 0x8622,
        శీర్షం_ATTRIB_ARRAY_కొలత         = 0x8623,
        శీర్షం_ATTRIB_ARRAY_STRIDE       = 0x8624,
        శీర్షం_ATTRIB_ARRAY_TYPE         = 0x8625,
        CURRENT_శీర్షం_ATTRIB            = 0x8626,
        శీర్షం_కార్యక్రమం_బిందువు_కొలత        = 0x8642,
        శీర్షం_ATTRIB_ARRAY_సూచిక      = 0x8645,
        STENCIL_వెనుక_FUNC                = 0x8800,
        STENCIL_వెనుక_FAIL                = 0x8801,
        STENCIL_వెనుక_PASS_లోతు_FAIL     = 0x8802,
        STENCIL_వెనుక_PASS_లోతు_PASS     = 0x8803,
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
        MAX_శీర్షం_ATTRIBS               = 0x8869,
        శీర్షం_ATTRIB_ARRAY_NORMALIZED   = 0x886A,
        MAX_TEXTURE_IMAGE_UNITS          = 0x8872,
        FRAGMENT_SHADER                  = 0x8B30,
        శీర్షం_SHADER                    = 0x8B31,
        MAX_FRAGMENT_ఏకరీతి_COMPONENTS  = 0x8B49,
        MAX_శీర్షం_ఏకరీతి_COMPONENTS    = 0x8B4A,
        MAX_VARYING_FLOATS               = 0x8B4B,
        MAX_శీర్షం_TEXTURE_IMAGE_UNITS   = 0x8B4C,
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
        DELETE_స్థితి                    = 0x8B80,
        COMPILE_స్థితి                   = 0x8B81,
        LINK_స్థితి                      = 0x8B82,
        VALIDATE_స్థితి                  = 0x8B83,
        INFO_LOG_నిడివి                  = 0x8B84,
        ATTACHED_SHADERS                 = 0x8B85,
        చురుకైన_UNIFORMS                  = 0x8B86,
        చురుకైన_ఏకరీతి_MAX_నిడివి        = 0x8B87,
        SHADER_ఆధారం_నిడివి             = 0x8B88,
        చురుకైన_లక్షణాలు                = 0x8B89,
        చురుకైన_ATTRIBUTE_MAX_నిడివి      = 0x8B8A,
        FRAGMENT_SHADER_DERIVATIVE_HINT  = 0x8B8B,
        SHADING_LANGUAGE_సంస్కరణ         = 0x8B8C,
        CURRENT_కార్యక్రమం                  = 0x8B8D,
        బిందువు_SPRITE_COORD_ORIGIN        = 0x8CA0,
        క్రింది_ఎడమ                       = 0x8CA1,
        పై_ఎడమ                       = 0x8CA2,
        STENCIL_వెనుక_REF                 = 0x8CA3,
        STENCIL_వెనుక_విలువ_ముసుగు          = 0x8CA4,
        STENCIL_వెనుక_WRITEMASK           = 0x8CA5,

        // Version: 2.1
        PIXEL_PACK_బఫరు                = 0x88EB,
        PIXEL_UNPACK_బఫరు              = 0x88EC,
        PIXEL_PACK_బఫరు_BINDING        = 0x88ED,
        PIXEL_UNPACK_బఫరు_BINDING      = 0x88EF,
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
        శీర్షం_ARRAY_BINDING             = 0x85B5,

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
        MAP_READ_బిట్టు                     = 0x0001,
        MAP_WRITE_బిట్టు                    = 0x0002,
        MAP_INVALIDATE_RANGE_బిట్టు         = 0x0004,
        MAP_INVALIDATE_బఫరు_బిట్టు        = 0x0008,
        MAP_FLUSH_EXPLICIT_బిట్టు           = 0x0010,
        MAP_UNSYNCHRONIZED_బిట్టు           = 0x0020,

        // Core Extension: ARB_half_float_vertex
        HALF_FLOAT                       = 0x140B,

        // Core Extension: ARB_framebuffer_sRGB
        FRAMEBUFFER_SRGB                 = 0x8DB9,

        // Core Extension: ARB_framebuffer_object
        INVALID_FRAMEBUFFER_OPERATION    = 0x0506,
        FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = 0x8210,
        FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = 0x8211,
        FRAMEBUFFER_ATTACHMENT_RED_కొలత  = 0x8212,
        FRAMEBUFFER_ATTACHMENT_GREEN_కొలత = 0x8213,
        FRAMEBUFFER_ATTACHMENT_BLUE_కొలత = 0x8214,
        FRAMEBUFFER_ATTACHMENT_ALPHA_కొలత = 0x8215,
        FRAMEBUFFER_ATTACHMENT_లోతు_కొలత = 0x8216,
        FRAMEBUFFER_ATTACHMENT_STENCIL_కొలత = 0x8217,
        FRAMEBUFFER_DEFAULT              = 0x8218,
        FRAMEBUFFER_UNDEFINED            = 0x8219,
        లోతు_STENCIL_ATTACHMENT         = 0x821A,
        సూచీ_2                            = 0x8222,
        MAX_RENDERBUFFER_కొలత            = 0x84E8,
        లోతు_STENCIL                    = 0x84F9,
        UNSIGNED_INT_24_8                = 0x84FA,
        DEPTH24_STENCIL8                 = 0x88F0,
        TEXTURE_STENCIL_కొలత             = 0x88F1,
        TEXTURE_RED_TYPE                 = 0x8C10,
        TEXTURE_GREEN_TYPE               = 0x8C11,
        TEXTURE_BLUE_TYPE                = 0x8C12,
        TEXTURE_ALPHA_TYPE               = 0x8C13,
        TEXTURE_లోతు_TYPE               = 0x8C16,
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
        FRAMEBUFFER_INCOMPLETE_DRAW_బఫరు = 0x8CDB,
        FRAMEBUFFER_INCOMPLETE_READ_బఫరు = 0x8CDC,
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
        లోతు_ATTACHMENT                 = 0x8D00,
        STENCIL_ATTACHMENT               = 0x8D20,
        FRAMEBUFFER                      = 0x8D40,
        RENDERBUFFER                     = 0x8D41,
        RENDERBUFFER_వెడల్పు               = 0x8D42,
        RENDERBUFFER_ఎత్తు              = 0x8D43,
        RENDERBUFFER_అంతర్గత_FORMAT     = 0x8D44,
        STENCIL_INDEX1                   = 0x8D46,
        STENCIL_INDEX4                   = 0x8D47,
        STENCIL_INDEX8                   = 0x8D48,
        STENCIL_INDEX16                  = 0x8D49,
        RENDERBUFFER_RED_కొలత            = 0x8D50,
        RENDERBUFFER_GREEN_కొలత          = 0x8D51,
        RENDERBUFFER_BLUE_కొలత           = 0x8D52,
        RENDERBUFFER_ALPHA_కొలత          = 0x8D53,
        RENDERBUFFER_లోతు_కొలత          = 0x8D54,
        RENDERBUFFER_STENCIL_కొలత        = 0x8D55,
        FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = 0x8D56,
        MAX_SAMPLES                      = 0x8D57,
        TEXTURE_LUMINANCE_TYPE           = 0x8C14,
        TEXTURE_INTENSITY_TYPE           = 0x8C15,

        // Core Extension: ARB_depth_buffer_float
        లోతు_COMPONENT32_F               = 0x8CAC,
        DEPTH32F_STENCIL8                = 0x8CAD,
        FLOAT_32_UNSIGNED_INT_24_8_REV   = 0x8DAD,

        // Version: 3.0
        COMPARE_REF_గమ్యం_TEXTURE           = 0x884E,
        CLIP_DISTANCE0                   = 0x3000,
        CLIP_DISTANCE1                   = 0x3001,
        CLIP_DISTANCE2                   = 0x3002,
        CLIP_DISTANCE3                   = 0x3003,
        CLIP_DISTANCE4                   = 0x3004,
        CLIP_DISTANCE5                   = 0x3005,
        CLIP_DISTANCE6                   = 0x3006,
        CLIP_DISTANCE7                   = 0x3007,
        MAX_CLIP_DISTANCES               = 0x0D32,
        MAJOR_సంస్కరణ                    = 0x821B,
        MINOR_సంస్కరణ                    = 0x821C,
        అంకె_EXTENSIONS                   = 0x821D,
        CONTEXT_FLAGS                    = 0x821E,
        COMPRESSED_RED                   = 0x8225,
        COMPRESSED_RG                    = 0x8226,
        CONTEXT_FLAG_FORWARD_COMPATIBLE_బిట్టు = 0x0001,
        RGBA32F                          = 0x8814,
        RGB32F                           = 0x8815,
        RGBA16F                          = 0x881A,
        RGB16F                           = 0x881B,
        శీర్షం_ATTRIB_ARRAY_INTEGER      = 0x88FD,
        MAX_ARRAY_TEXTURE_LAYERS         = 0x88FF,
        MIN_కార్యక్రమం_TEXEL_స్థానభ్రంశం         = 0x8904,
        MAX_కార్యక్రమం_TEXEL_స్థానభ్రంశం         = 0x8905,
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
        TEXTURE_SHARED_కొలత              = 0x8C3F,
        TRANSFORM_FEEDBACK_VARYING_MAX_నిడివి = 0x8C76,
        TRANSFORM_FEEDBACK_బఫరు_బహుళకం   = 0x8C7F,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = 0x8C80,
        TRANSFORM_FEEDBACK_VARYINGS      = 0x8C83,
        TRANSFORM_FEEDBACK_బఫరు_ప్రారంభం  = 0x8C84,
        TRANSFORM_FEEDBACK_బఫరు_కొలత   = 0x8C85,
        PRIMITIVES_GENERATED             = 0x8C87,
        TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88,
        RASTERIZER_DISCARD               = 0x8C89,
        MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = 0x8C8B,
        INTERLEAVED_ATTRIBS              = 0x8C8C,
        SEPARATE_ATTRIBS                 = 0x8C8D,
        TRANSFORM_FEEDBACK_బఫరు        = 0x8C8E,
        TRANSFORM_FEEDBACK_బఫరు_BINDING = 0x8C8F,
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
        బఫరు_ACCESS_FLAGS              = 0x911F,
        బఫరు_MAP_నిడివి                = 0x9120,
        బఫరు_MAP_స్థానభ్రంశం                = 0x9121,

        // Core Extension: ARB_uniform_buffer_object
        ఏకరీతి_బఫరు                   = 0x8A11,
        ఏకరీతి_బఫరు_BINDING           = 0x8A28,
        ఏకరీతి_బఫరు_ప్రారంభం             = 0x8A29,
        ఏకరీతి_బఫరు_కొలత              = 0x8A2A,
        MAX_శీర్షం_ఏకరీతి_ఖండాలు        = 0x8A2B,
        MAX_FRAGMENT_ఏకరీతి_ఖండాలు      = 0x8A2D,
        MAX_COMBINED_ఏకరీతి_ఖండాలు      = 0x8A2E,
        MAX_ఏకరీతి_బఫరు_BINDINGS      = 0x8A2F,
        MAX_ఏకరీతి_ఖండం_కొలత           = 0x8A30,
        MAX_COMBINED_శీర్షం_ఏకరీతి_COMPONENTS = 0x8A31,
        MAX_COMBINED_FRAGMENT_ఏకరీతి_COMPONENTS = 0x8A33,
        ఏకరీతి_బఫరు_స్థానభ్రంశం_ALIGNMENT  = 0x8A34,
        చురుకైన_ఏకరీతి_ఖండం_MAX_NAME_నిడివి = 0x8A35,
        చురుకైన_ఏకరీతి_ఖండాలు            = 0x8A36,
        ఏకరీతి_TYPE                     = 0x8A37,
        ఏకరీతి_కొలత                     = 0x8A38,
        ఏకరీతి_NAME_నిడివి              = 0x8A39,
        ఏకరీతి_ఖండం_సూచీ_2              = 0x8A3A,
        ఏకరీతి_స్థానభ్రంశం                   = 0x8A3B,
        ఏకరీతి_ARRAY_STRIDE             = 0x8A3C,
        ఏకరీతి_మాతృక_STRIDE            = 0x8A3D,
        ఏకరీతి_IS_అడ్డువరుస_MAJOR             = 0x8A3E,
        ఏకరీతి_ఖండం_BINDING            = 0x8A3F,
        ఏకరీతి_ఖండం_దత్తాంశం_కొలత          = 0x8A40,
        ఏకరీతి_ఖండం_NAME_నిడివి        = 0x8A41,
        ఏకరీతి_ఖండం_చురుకైన_UNIFORMS    = 0x8A42,
        ఏకరీతి_ఖండం_చురుకైన_ఏకరీతి_INDICES = 0x8A43,
        ఏకరీతి_ఖండం_REFERENCED_BY_శీర్షం_SHADER = 0x8A44,
        ఏకరీతి_ఖండం_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46,
        INVALID_సూచీ                    = 0xFFFFFFFF,
        MAX_GEOMETRY_ఏకరీతి_ఖండాలు      = 0x8A2C,
        MAX_COMBINED_GEOMETRY_ఏకరీతి_COMPONENTS = 0x8A32,
        ఏకరీతి_ఖండం_REFERENCED_BY_GEOMETRY_SHADER = 0x8A45,

        // Core Extension: ARB_copy_buffer
        COPY_READ_బఫరు                 = 0x8F36,
        COPY_WRITE_బఫరు                = 0x8F37,
        COPY_READ_బఫరు_BINDING         = 0x8F36,
        COPY_WRITE_బఫరు_BINDING        = 0x8F37,

        // Version: 3.1
        SAMPLER_2D_RECT                  = 0x8B63,
        SAMPLER_2D_RECT_SHADOW           = 0x8B64,
        SAMPLER_బఫరు                   = 0x8DC2,
        INT_SAMPLER_2D_RECT              = 0x8DCD,
        INT_SAMPLER_బఫరు               = 0x8DD0,
        UNSIGNED_INT_SAMPLER_2D_RECT     = 0x8DD5,
        UNSIGNED_INT_SAMPLER_బఫరు      = 0x8DD8,
        TEXTURE_బఫరు                   = 0x8C2A,
        MAX_TEXTURE_బఫరు_కొలత          = 0x8C2B,
        TEXTURE_BINDING_బఫరు           = 0x8C2C,
        TEXTURE_బఫరు_దత్తాంశం_STORE_BINDING = 0x8C2D,
        TEXTURE_RECTANGLE                = 0x84F5,
        TEXTURE_BINDING_RECTANGLE        = 0x84F6,
        PROXY_TEXTURE_RECTANGLE          = 0x84F7,
        MAX_RECTANGLE_TEXTURE_కొలత       = 0x84F8,
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
        PRIMITIVE_RESTART_సూచీ          = 0x8F9E,

        // Legacy
        శీర్షం_ARRAY = 0x8074,
        సాధారణ_ARRAY = 0x8075,
        COLOR_ARRAY = 0x8076,
        TEXTURE_COORD_ARRAY = 0x8078,
        TEXTURE_ENV = 0x2300,
        TEXTURE_ENV_బహుళకం = 0x2200,
        MODELVIEW = 0x1700,
        PROJECTION = 0x1701,
        LIGHTING = 0x0B50
    };

    // Extension: 1.1
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CullFace)(GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *FrontFace)(GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Hint)(GLenum target, GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *పంక్తి_వెడల్పు)(GLfloat వెడల్పు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బిందువు_కొలత)(GLfloat size);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Polygon_బహుళకం)(GLenum face, GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Scissor)(GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexParameterf)(GLenum target, GLenum pname, GLfloat param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexParameterfv)(GLenum target, GLenum pname, స్థిరరాశి GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexParameteri)(GLenum target, GLenum pname, GLint param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexParameteriv)(GLenum target, GLenum pname, స్థిరరాశి GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei వెడల్పు, GLint border, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు, GLint border, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Draw_బఫరు)(GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Clear)(GLbitfield ముసుగు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClearStencil)(GLint s);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Clear_లోతు)(GLdouble లోతు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Stencil_ముసుగు)(GLuint ముసుగు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Color_ముసుగు)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *లోతు_ముసుగు)(GLboolean flag);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Disable)(GLenum cap);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Enable)(GLenum cap);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ముగింపు)();
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Flush)();
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BlendFunc)(GLenum sfactor, GLenum dfactor);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *LogicOp)(GLenum opcode);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *StencilFunc)(GLenum func, GLint ref, GLuint ముసుగు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *లోతు_Func)(GLenum func);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *PixelStoref)(GLenum pname, GLfloat param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *PixelStorei)(GLenum pname, GLint param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Read_బఫరు)(GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ReadPixels)(GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetBooleanv)(GLenum pname, GLboolean *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetDoublev)(GLenum pname, GLdouble *params);
    బాహ్య GLenum (CODEGEN_FUNCPTR *GetError)();
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetFloatv)(GLenum pname, GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetIntegerv)(GLenum pname, GLint *params);
    బాహ్య స్థిరరాశి GLubyte * (CODEGEN_FUNCPTR *GetString)(GLenum name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsEnabled)(GLenum cap);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *లోతు_Range)(GLdouble ren_near, GLdouble ren_far);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Viewport)(GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DrawArrays)(GLenum బహుళకం, GLint first, GLsizei లెక్క_2);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DrawElements)(GLenum బహుళకం, GLsizei లెక్క_2, GLenum type, స్థిరరాశి GLvoid *indices);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetPointerv)(GLenum pname, GLvoid* *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Polygon_స్థానభ్రంశం)(GLfloat గుణకం, GLfloat units);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei వెడల్పు, GLint border);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు, GLint border);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei వెడల్పు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei వెడల్పు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BindTexture)(GLenum target, GLuint texture);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DeleteTextures)(GLsizei n, స్థిరరాశి GLuint *textures);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GenTextures)(GLsizei n, GLuint *textures);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsTexture)(GLuint texture);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Indexub)(GLubyte c);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Indexubv)(స్థిరరాశి GLubyte *c);

    // Extension: 1.2
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BlendEquation)(GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DrawRangeElements)(GLenum బహుళకం, GLuint ప్రారంభం, GLuint end, GLsizei లెక్క_2, GLenum type, స్థిరరాశి GLvoid *indices);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLsizei లోతు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు);

    // Extension: 1.3
    బాహ్య శూన్య (CODEGEN_FUNCPTR *చురుకైన_Texture)(GLenum texture);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *SampleCoverage)(GLfloat value, GLboolean invert);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు, GLsizei లోతు, GLint border, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు, GLint border, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei వెడల్పు, GLint border, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLsizei లోతు, GLenum format, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei వెడల్పు, GLenum format, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);

    // Extension: 1.4
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *MultiDrawArrays)(GLenum బహుళకం, స్థిరరాశి GLint *first, స్థిరరాశి GLsizei *లెక్క_2, GLsizei drawcount);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *MultiDrawElements)(GLenum బహుళకం, స్థిరరాశి GLsizei *లెక్క_2, GLenum type, స్థిరరాశి GLvoid* స్థిరరాశి *indices, GLsizei drawcount);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బిందువు_Parameterf)(GLenum pname, GLfloat param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బిందువు_Parameterfv)(GLenum pname, స్థిరరాశి GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బిందువు_Parameteri)(GLenum pname, GLint param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బిందువు_Parameteriv)(GLenum pname, స్థిరరాశి GLint *params);

    // Extension: 1.5
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GenQueries)(GLsizei n, GLuint *ids);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DeleteQueries)(GLsizei n, స్థిరరాశి GLuint *ids);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsQuery)(GLuint id);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BeginQuery)(GLenum target, GLuint id);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *చివర_Query)(GLenum target);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetQueryiv)(GLenum target, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Bind_బఫరు)(GLenum target, GLuint బఫరు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DeleteBuffers)(GLsizei n, స్థిరరాశి GLuint *buffers);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GenBuffers)(GLsizei n, GLuint *buffers);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *Is_బఫరు)(GLuint బఫరు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బఫరు_దత్తాంశం)(GLenum target, GLsizeiptr size, స్థిరరాశి GLvoid *దత్తాంశం, GLenum usage);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *బఫరు_Sub_దత్తాంశం)(GLenum target, GLintptr offset, GLsizeiptr size, స్థిరరాశి GLvoid *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_బఫరు_Sub_దత్తాంశం)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *దత్తాంశం);
    బాహ్య GLvoid* (CODEGEN_FUNCPTR *Map_బఫరు)(GLenum target, GLenum access);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *Unmap_బఫరు)(GLenum target);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_బఫరు_Parameteriv)(GLenum target, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_బఫరు_Pointerv)(GLenum target, GLenum pname, GLvoid* *params);

    // Extension: 2.0
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BlendEquationSeparate)(GLenum బహుళకం_RGB, GLenum బహుళకం_Alpha);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DrawBuffers)(GLsizei n, స్థిరరాశి GLenum *bufs);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *StencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *StencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint ముసుగు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Stencil_ముసుగు_Separate)(GLenum face, GLuint ముసుగు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *AttachShader)(GLuint కార్యక్రమం, GLuint shader);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BindAttribLocation)(GLuint కార్యక్రమం, GLuint సూచీ, స్థిరరాశి GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *CompileShader)(GLuint shader);
    బాహ్య GLuint (CODEGEN_FUNCPTR *Create_కార్యక్రమం)();
    బాహ్య GLuint (CODEGEN_FUNCPTR *CreateShader)(GLenum type);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Delete_కార్యక్రమం)(GLuint కార్యక్రమం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DeleteShader)(GLuint shader);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DetachShader)(GLuint కార్యక్రమం, GLuint shader);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Disable_శీర్షం_Attrib_Array)(GLuint సూచీ);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Enable_శీర్షం_Attrib_Array)(GLuint సూచీ);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_చురుకైన_Attrib)(GLuint కార్యక్రమం, GLuint సూచీ, GLsizei buf_కొలత, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_చురుకైన_ఏకరీతి)(GLuint కార్యక్రమం, GLuint సూచీ, GLsizei buf_కొలత, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetAttachedShaders)(GLuint కార్యక్రమం, GLsizei max_లెక్క, GLsizei *లెక్క_2, GLuint *obj);
    బాహ్య GLint (CODEGEN_FUNCPTR *GetAttribLocation)(GLuint కార్యక్రమం, స్థిరరాశి GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetProgramiv)(GLuint కార్యక్రమం, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_కార్యక్రమం_Info_Log)(GLuint కార్యక్రమం, GLsizei buf_కొలత, GLsizei *length, GLchar *infoLog);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetShaderInfoLog)(GLuint shader, GLsizei buf_కొలత, GLsizei *length, GLchar *infoLog);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_Shader_ఆధారం)(GLuint shader, GLsizei buf_కొలత, GLsizei *length, GLchar *ఆధారం);
    బాహ్య GLint (CODEGEN_FUNCPTR *Get_ఏకరీతి_Location)(GLuint కార్యక్రమం, స్థిరరాశి GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetUniformfv)(GLuint కార్యక్రమం, GLint location, GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetUniformiv)(GLuint కార్యక్రమం, GLint location, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_శీర్షం_Attribdv)(GLuint సూచీ, GLenum pname, GLdouble *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_శీర్షం_Attribfv)(GLuint సూచీ, GLenum pname, GLfloat *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_శీర్షం_Attribiv)(GLuint సూచీ, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_శీర్షం_Attrib_Pointerv)(GLuint సూచీ, GLenum pname, GLvoid* *pointer);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *Is_కార్యక్రమం)(GLuint కార్యక్రమం);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsShader)(GLuint shader);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Link_కార్యక్రమం)(GLuint కార్యక్రమం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Shader_ఆధారం)(GLuint shader, GLsizei లెక్క_2, స్థిరరాశి GLchar* స్థిరరాశి *string, స్థిరరాశి GLint *length);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Use_కార్యక్రమం)(GLuint కార్యక్రమం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform1f)(GLint location, GLfloat v0);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform2f)(GLint location, GLfloat v0, GLfloat v1);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform1i)(GLint location, GLint v0);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform2i)(GLint location, GLint v0, GLint v1);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform1fv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform2fv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform3fv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform4fv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform1iv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform2iv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform3iv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform4iv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix2fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix3fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix4fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Validate_కార్యక్రమం)(GLuint కార్యక్రమం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_సూచిక)(GLuint సూచీ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, స్థిరరాశి GLvoid *pointer);

    // Extension: 2.1
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix2x3fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix3x2fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix2x4fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix4x2fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix3x4fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_Matrix4x3fv)(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value);

    // Extension: ARB_vertex_array_object
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Bind_శీర్షం_Array)(GLuint ren_array);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Delete_శీర్షం_Arrays)(GLsizei n, స్థిరరాశి GLuint *arrays);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Gen_శీర్షం_Arrays)(GLsizei n, GLuint *arrays);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *Is_శీర్షం_Array)(GLuint ren_array);

    // Extension: ARB_map_buffer_range
    బాహ్య GLvoid* (CODEGEN_FUNCPTR *Map_బఫరు_Range)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Flush_Mapped_బఫరు_Range)(GLenum target, GLintptr offset, GLsizeiptr length);

    // Extension: ARB_framebuffer_object
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsRenderbuffer)(GLuint renderbuffer);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BindRenderbuffer)(GLenum target, GLuint renderbuffer);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DeleteRenderbuffers)(GLsizei n, స్థిరరాశి GLuint *renderbuffers);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsFramebuffer)(GLuint framebuffer);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BindFramebuffer)(GLenum target, GLuint framebuffer);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DeleteFramebuffers)(GLsizei n, స్థిరరాశి GLuint *framebuffers);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GenFramebuffers)(GLsizei n, GLuint *framebuffers);
    బాహ్య GLenum (CODEGEN_FUNCPTR *Check_Framebuffer_స్థితి)(GLenum target);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *FramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *FramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GenerateMipmap)(GLenum target);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ముసుగు, GLenum filter);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *RenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *FramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

    // Extension: 3.0
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ColorMaski)(GLuint సూచీ, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetBooleani_v)(GLenum target, GLuint సూచీ, GLboolean *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetIntegeri_v)(GLenum target, GLuint సూచీ, GLint *దత్తాంశం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Enablei)(GLenum target, GLuint సూచీ);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Disablei)(GLenum target, GLuint సూచీ);
    బాహ్య GLboolean (CODEGEN_FUNCPTR *IsEnabledi)(GLenum target, GLuint సూచీ);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BeginTransformFeedback)(GLenum primitive_బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *చివర_Transform_Feedback)();
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Bind_బఫరు_Range)(GLenum target, GLuint సూచీ, GLuint బఫరు, GLintptr offset, GLsizeiptr size);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Bind_బఫరు_పునాది)(GLenum target, GLuint సూచీ, GLuint బఫరు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TransformFeedbackVaryings)(GLuint కార్యక్రమం, GLsizei లెక్క_2, స్థిరరాశి GLchar* స్థిరరాశి *varyings, GLenum బఫరు_బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTransformFeedbackVarying)(GLuint కార్యక్రమం, GLuint సూచీ, GLsizei buf_కొలత, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClampColor)(GLenum target, GLenum clamp);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *BeginConditionalRender)(GLuint id, GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *చివర_Conditional_Render)();
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_IPointer)(GLuint సూచీ, GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *pointer);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_శీర్షం_Attrib_Iiv)(GLuint సూచీ, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_శీర్షం_Attrib_Iuiv)(GLuint సూచీ, GLenum pname, GLuint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I1i)(GLuint సూచీ, GLint x);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I2i)(GLuint సూచీ, GLint x, GLint y);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I3i)(GLuint సూచీ, GLint x, GLint y, GLint z);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4i)(GLuint సూచీ, GLint x, GLint y, GLint z, GLint w);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I1ui)(GLuint సూచీ, GLuint x);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I2ui)(GLuint సూచీ, GLuint x, GLuint y);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I3ui)(GLuint సూచీ, GLuint x, GLuint y, GLuint z);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4ui)(GLuint సూచీ, GLuint x, GLuint y, GLuint z, GLuint w);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I1iv)(GLuint సూచీ, స్థిరరాశి GLint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I2iv)(GLuint సూచీ, స్థిరరాశి GLint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I3iv)(GLuint సూచీ, స్థిరరాశి GLint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4iv)(GLuint సూచీ, స్థిరరాశి GLint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I1uiv)(GLuint సూచీ, స్థిరరాశి GLuint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I2uiv)(GLuint సూచీ, స్థిరరాశి GLuint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I3uiv)(GLuint సూచీ, స్థిరరాశి GLuint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4uiv)(GLuint సూచీ, స్థిరరాశి GLuint *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4bv)(GLuint సూచీ, స్థిరరాశి GLbyte *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4sv)(GLuint సూచీ, స్థిరరాశి GLshort *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4ubv)(GLuint సూచీ, స్థిరరాశి GLubyte *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_Attrib_I4usv)(GLuint సూచీ, స్థిరరాశి GLushort *v);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetUniformuiv)(GLuint కార్యక్రమం, GLint location, GLuint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Bind_Frag_దత్తాంశం_Location)(GLuint కార్యక్రమం, GLuint color, స్థిరరాశి GLchar *name);
    బాహ్య GLint (CODEGEN_FUNCPTR *Get_Frag_దత్తాంశం_Location)(GLuint కార్యక్రమం, స్థిరరాశి GLchar *name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform1ui)(GLint location, GLuint v0);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform2ui)(GLint location, GLuint v0, GLuint v1);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform1uiv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform2uiv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform3uiv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Uniform4uiv)(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexParameterIiv)(GLenum target, GLenum pname, స్థిరరాశి GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexParameterIuiv)(GLenum target, GLenum pname, స్థిరరాశి GLuint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *GetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClearBufferiv)(GLenum బఫరు, GLint drawbuffer, స్థిరరాశి GLint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClearBufferuiv)(GLenum బఫరు, GLint drawbuffer, స్థిరరాశి GLuint *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClearBufferfv)(GLenum బఫరు, GLint drawbuffer, స్థిరరాశి GLfloat *value);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ClearBufferfi)(GLenum బఫరు, GLint drawbuffer, GLfloat లోతు, GLint stencil);
    బాహ్య స్థిరరాశి GLubyte * (CODEGEN_FUNCPTR *GetStringi)(GLenum name, GLuint సూచీ);

    // Extension: ARB_uniform_buffer_object
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_ఏకరీతి_Indices)(GLuint కార్యక్రమం, GLsizei ఏకరీతి_లెక్క, స్థిరరాశి GLchar* స్థిరరాశి *ఏకరీతి_పేర్లు, GLuint *ఏకరీతి_Indices);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_చురుకైన_Uniformsiv)(GLuint కార్యక్రమం, GLsizei ఏకరీతి_లెక్క, స్థిరరాశి GLuint *ఏకరీతి_Indices, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_చురుకైన_ఏకరీతి_Name)(GLuint కార్యక్రమం, GLuint ఏకరీతి_సూచీ, GLsizei buf_కొలత, GLsizei *length, GLchar *ఏకరీతి_Name);
    బాహ్య GLuint (CODEGEN_FUNCPTR *Get_ఏకరీతి_ఖండం_సూచీ)(GLuint కార్యక్రమం, స్థిరరాశి GLchar *ఏకరీతి_ఖండం_Name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_చురుకైన_ఏకరీతి_Blockiv)(GLuint కార్యక్రమం, GLuint ఏకరీతి_ఖండం_సూచీ, GLenum pname, GLint *params);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Get_చురుకైన_ఏకరీతి_ఖండం_Name)(GLuint కార్యక్రమం, GLuint ఏకరీతి_ఖండం_సూచీ, GLsizei buf_కొలత, GLsizei *length, GLchar *ఏకరీతి_ఖండం_Name);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఏకరీతి_ఖండం_Binding)(GLuint కార్యక్రమం, GLuint ఏకరీతి_ఖండం_సూచీ, GLuint ఏకరీతి_ఖండం_Binding_2);

    // Extension: ARB_copy_buffer
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Copy_బఫరు_Sub_దత్తాంశం)(GLenum read_లక్ష్యం, GLenum write_లక్ష్యం, GLintptr read_స్థానభ్రంశం, GLintptr write_స్థానభ్రంశం, GLsizeiptr size);

    // Extension: 3.1
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DrawArraysInstanced)(GLenum బహుళకం, GLint first, GLsizei లెక్క_2, GLsizei instancecount);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DrawElementsInstanced)(GLenum బహుళకం, GLsizei లెక్క_2, GLenum type, స్థిరరాశి GLvoid *indices, GLsizei instancecount);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Tex_బఫరు)(GLenum target, GLenum internalformat, GLuint బఫరు);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Primitive_Restart_సూచీ)(GLuint సూచీ);

    // Legacy
    బాహ్య శూన్య (CODEGEN_FUNCPTR *EnableClientState)(GLenum cap);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *DisableClientState)(GLenum cap);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *శీర్షం_సూచిక)(GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *సాధారణ_సూచిక)(GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Color_సూచిక)(GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Tex_Coord_సూచిక)(GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *TexEnvi)(GLenum target, GLenum pname, GLint param);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *మాతృక_బహుళకం)(GLenum బహుళకం);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *ఎక్కించు_తటస్థం)(శూన్య);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Ortho)(GLdouble ఎడమ, GLdouble కుడి, GLdouble bottom, GLdouble శిఖరం_2, GLdouble near_val, GLdouble far_val);
    బాహ్య శూన్య (CODEGEN_FUNCPTR *Color3d)(GLdouble red, GLdouble green, GLdouble blue);
}

#అయితే_ముగింపు // OPENGL_NOLOAD_STYLE_HPP
