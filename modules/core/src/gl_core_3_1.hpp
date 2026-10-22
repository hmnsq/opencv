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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENGL_NOLOAD_STYLE_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENGL_NOLOAD_STYLE_HPP

#ಆದರೆ defined(__gl_h_) || defined(__GL_H__)
#ದೋಷ Attempt ಗಮ್ಯ include ಸ್ವಯಂಚಾಲಿತ-generated ಶೀರ್ಷಿಕೆ after including gl.h
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined(__glext_h_) || defined(__GLEXT_H_)
#ದೋಷ Attempt ಗಮ್ಯ include ಸ್ವಯಂಚಾಲಿತ-generated ಶೀರ್ಷಿಕೆ after including glext.h
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined(__gl_ATI_h_)
#ದೋಷ Attempt ಗಮ್ಯ include ಸ್ವಯಂಚಾಲಿತ-generated ಶೀರ್ಷಿಕೆ after including glATI.h
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು __gl_h_
#ವ್ಯಾಖ್ಯಾನಿಸು __GL_H__
#ವ್ಯಾಖ್ಯಾನಿಸು __glext_h_
#ವ್ಯಾಖ್ಯಾನಿಸು __GLEXT_H_
#ವ್ಯಾಖ್ಯಾನಿಸು __gl_ATI_h_

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ APIENTRY
    #ಆದರೆ defined(__MINGW32__)
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WIN32_LEAN_AND_ಸರಾಸರಿ
            #ವ್ಯಾಖ್ಯಾನಿಸು WIN32_LEAN_AND_ಸರಾಸರಿ 1
        #ಆದರೆ_ಅಂತ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NOMINMAX
            #ವ್ಯಾಖ್ಯಾನಿಸು NOMINMAX
        #ಆದರೆ_ಅಂತ್ಯ
        #ಅನ್ತರ್ಭಾವ <windows.h>
    #ಅಥವಾಆದರೆ (defined(_MSC_VER) && _MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WIN32_LEAN_AND_ಸರಾಸರಿ
            #ವ್ಯಾಖ್ಯಾನಿಸು WIN32_LEAN_AND_ಸರಾಸರಿ 1
        #ಆದರೆ_ಅಂತ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NOMINMAX
            #ವ್ಯಾಖ್ಯಾನಿಸು NOMINMAX
        #ಆದರೆ_ಅಂತ್ಯ
        #ಅನ್ತರ್ಭಾವ <windows.h>
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು APIENTRY
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ // APIENTRY

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CODEGEN_FUNCPTR
    #ವ್ಯಾಖ್ಯಾನಿಸು CODEGEN_REMOVE_FUNCPTR
    #ಆದರೆ defined(_WIN32)
        #ವ್ಯಾಖ್ಯಾನಿಸು CODEGEN_FUNCPTR APIENTRY
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು CODEGEN_FUNCPTR
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ // CODEGEN_FUNCPTR

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ತುಂಬು_GEN_BASIC_OPENGL_TYPEDEFS
#ವ್ಯಾಖ್ಯಾನಿಸು GL_ತುಂಬು_GEN_BASIC_OPENGL_TYPEDEFS
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ GLenum;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ GLboolean;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ GLbitfield;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ GLbyte;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಗಿಡ್ಡ GLshort;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ GLint;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ GLsizei;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ GLubyte;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ GLushort;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ GLuint;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ GLfloat;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ GLclampf;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ GLdouble;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ GLclampd;
    #ವ್ಯಾಖ್ಯಾನಿಸು GLvoid ಶೂನ್ಯ
#ಆದರೆ_ಅಂತ್ಯ // GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS

#ಅನ್ತರ್ಭಾವ <stddef.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ಆವೃತ್ತಿ_2_0
    // GL type for program/shader text
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ GLchar;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ಆವೃತ್ತಿ_1_5
    // GL types for handling large vertex buffer objects
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ptrdiff_t GLintptr;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ptrdiff_t GLsizeiptr;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ARB_ಶೃಂಗ_ಬಫರ್_object
    // GL types for handling large vertex buffer objects
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ptrdiff_t GLintptrARB;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ptrdiff_t GLsizeiptrARB;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ARB_shader_objects
    // GL types for program/shader text and shader object handles
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಅಕ್ಷರ GLcharARB;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ GLhandleARB;
#ಆದರೆ_ಅಂತ್ಯ

// GL type for "half" precision (s10e5) float data in host memory
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ARB_half_float_pixel
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ GLhalfARB;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_NV_half_float
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ GLhalfNV;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GLEXT_64_TYPES_DEFINED
    // This code block is duplicated in glxext.h, so must be protected
    #ವ್ಯಾಖ್ಯಾನಿಸು GLEXT_64_TYPES_DEFINED

    // Define int32_t, int64_t, and uint64_t types for UST/MSC
    // (as used in the GL_EXT_timer_query extension)
    #ಆದರೆ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #ಅನ್ತರ್ಭಾವ <inttypes.h>
    #ಅಥವಾಆದರೆ defined(__sun__) || defined(__digital__)
        #ಅನ್ತರ್ಭಾವ <inttypes.h>
        #ಆದರೆ defined(__STDC__)
            #ಆದರೆ defined(__arch64__) || defined(_LP64)
                ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ int64_t;
                ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ uint64_t;
            #ಇಲ್ಲದಿದ್ದರೆ
                ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ int64_t;
                ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ uint64_t;
            #ಆದರೆ_ಅಂತ್ಯ // __arch64__
        #ಆದರೆ_ಅಂತ್ಯ // __STDC__
    #ಅಥವಾಆದರೆ defined( __VMS ) || defined(__sgi)
        #ಅನ್ತರ್ಭಾವ <inttypes.h>
    #ಅಥವಾಆದರೆ defined(__SCO__) || defined(__USLC__)
        #ಅನ್ತರ್ಭಾವ <stdint.h>
    #ಅಥವಾಆದರೆ defined(__UNIXOS2__) || defined(__SOL64__)
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ int32_t;
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ int64_t;
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ uint64_t;
    #ಅಥವಾಆದರೆ defined(_WIN32) && defined(__GNUC__)
        #ಅನ್ತರ್ಭಾವ <stdint.h>
    #ಅಥವಾಆದರೆ defined(_WIN32)
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __int32 int32_t;
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ __int64 int64_t;
        ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ __int64 uint64_t;
    #ಇಲ್ಲದಿದ್ದರೆ
        // Fallback if nothing above works
        #ಅನ್ತರ್ಭಾವ <inttypes.h>
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_EXT_timer_query
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int64_t GLint64EXT;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint64_t GLuint64EXT;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ARB_sync
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int64_t GLint64;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint64_t GLuint64;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ __GLsync *GLsync;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ARB_cl_event
    // These incomplete types let us declare types compatible with OpenCL's cl_context and cl_event
    ರಚನೆ _cl_context;
    ರಚನೆ _cl_event;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_ARB_debug_output
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (APIENTRY *GLDEBUGPROCARB)(GLenum ಮೂಲ,GLenum type,GLuint id,GLenum severity,GLsizei length,ಸ್ಥಿರಾಂಕ GLchar *message,GLvoid *userParam);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_AMD_debug_output
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,ಸ್ಥಿರಾಂಕ GLchar *message,GLvoid *userParam);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_KHR_debug
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (APIENTRY *GLDEBUGPROC)(GLenum ಮೂಲ,GLenum type,GLuint id,GLenum severity,GLsizei length,ಸ್ಥಿರಾಂಕ GLchar *message,GLvoid *userParam);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GL_NV_vdpau_interop
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLintptr GLvdpauSurfaceNV;
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ gl
{
    ಎಣಿಕೆ
    {
        // Version: 1.1
        ಆಳ_ಬಫರ್_ಬಿಟ್                 = 0x00000100,
        STENCIL_ಬಫರ್_ಬಿಟ್               = 0x00000400,
        COLOR_ಬಫರ್_ಬಿಟ್                 = 0x00004000,
        FALSE_                           = 0,
        TRUE_                            = 1,
        POINTS                           = 0x0000,
        ಸಾಲುಗಳು                            = 0x0001,
        ಸಾಲು_LOOP                        = 0x0002,
        ಸಾಲು_STRIP                       = 0x0003,
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
        FRONT_ಎಡ                       = 0x0400,
        FRONT_ಬಲ                      = 0x0401,
        ಹಿಂಭಾಗ_ಎಡ                        = 0x0402,
        ಹಿಂಭಾಗ_ಬಲ                       = 0x0403,
        FRONT                            = 0x0404,
        ಹಿಂಭಾಗ                             = 0x0405,
        ಎಡ_2                             = 0x0406,
        ಬಲ_2                            = 0x0407,
        FRONT_AND_ಹಿಂಭಾಗ                   = 0x0408,
        NO_ERROR_                        = 0,
        INVALID_ENUM                     = 0x0500,
        INVALID_ಮೌಲ್ಯ                    = 0x0501,
        INVALID_OPERATION                = 0x0502,
        ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ                    = 0x0505,
        CW                               = 0x0900,
        CCW                              = 0x0901,
        ಬಿಂದು_ಗಾತ್ರ_2                       = 0x0B11,
        ಬಿಂದು_ಗಾತ್ರ_RANGE                 = 0x0B12,
        ಬಿಂದು_ಗಾತ್ರ_GRANULARITY           = 0x0B13,
        ಸಾಲು_SMOOTH                      = 0x0B20,
        ಸಾಲು_ಅಗಲ_2                       = 0x0B21,
        ಸಾಲು_ಅಗಲ_RANGE                 = 0x0B22,
        ಸಾಲು_ಅಗಲ_GRANULARITY           = 0x0B23,
        POLYGON_ಬಹುಲಕ                     = 0x0B40,
        POLYGON_SMOOTH                   = 0x0B41,
        CULL_FACE                        = 0x0B44,
        CULL_FACE_ಬಹುಲಕ                   = 0x0B45,
        FRONT_FACE                       = 0x0B46,
        ಆಳ_RANGE                      = 0x0B70,
        ಆಳ_TEST                       = 0x0B71,
        ಆಳ_WRITEMASK                  = 0x0B72,
        ಆಳ_CLEAR_ಮೌಲ್ಯ                = 0x0B73,
        ಆಳ_FUNC                       = 0x0B74,
        STENCIL_TEST                     = 0x0B90,
        STENCIL_CLEAR_ಮೌಲ್ಯ              = 0x0B91,
        STENCIL_FUNC                     = 0x0B92,
        STENCIL_ಮೌಲ್ಯ_ಮುಖವಾಡ               = 0x0B93,
        STENCIL_FAIL                     = 0x0B94,
        STENCIL_PASS_ಆಳ_FAIL          = 0x0B95,
        STENCIL_PASS_ಆಳ_PASS          = 0x0B96,
        STENCIL_REF                      = 0x0B97,
        STENCIL_WRITEMASK                = 0x0B98,
        VIEWPORT                         = 0x0BA2,
        DITHER                           = 0x0BD0,
        BLEND_DST                        = 0x0BE0,
        BLEND_SRC                        = 0x0BE1,
        BLEND                            = 0x0BE2,
        LOGIC_OP_ಬಹುಲಕ                    = 0x0BF0,
        COLOR_LOGIC_OP                   = 0x0BF2,
        DRAW_ಬಫರ್                      = 0x0C01,
        READ_ಬಫರ್                      = 0x0C02,
        SCISSOR_BOX                      = 0x0C10,
        SCISSOR_TEST                     = 0x0C11,
        COLOR_CLEAR_ಮೌಲ್ಯ                = 0x0C22,
        COLOR_WRITEMASK                  = 0x0C23,
        DOUBLEBUFFER                     = 0x0C32,
        STEREO                           = 0x0C33,
        ಸಾಲು_SMOOTH_HINT                 = 0x0C52,
        POLYGON_SMOOTH_HINT              = 0x0C53,
        UNPACK_ಅದಲುಬದಲು_ಬೈಟುಗಳು                = 0x0CF0,
        UNPACK_LSB_ಮೊದಲ                 = 0x0CF1,
        UNPACK_ಅಡ್ಡಸಾಲು_ಉದ್ದಳತೆ                = 0x0CF2,
        UNPACK_SKIP_ಅಡ್ಡಸಾಲುಗಳು                 = 0x0CF3,
        UNPACK_SKIP_PIXELS               = 0x0CF4,
        UNPACK_ALIGNMENT                 = 0x0CF5,
        PACK_ಅದಲುಬದಲು_ಬೈಟುಗಳು                  = 0x0D00,
        PACK_LSB_ಮೊದಲ                   = 0x0D01,
        PACK_ಅಡ್ಡಸಾಲು_ಉದ್ದಳತೆ                  = 0x0D02,
        PACK_SKIP_ಅಡ್ಡಸಾಲುಗಳು                   = 0x0D03,
        PACK_SKIP_PIXELS                 = 0x0D04,
        PACK_ALIGNMENT                   = 0x0D05,
        MAX_TEXTURE_ಗಾತ್ರ                 = 0x0D33,
        MAX_VIEWPORT_DIMS                = 0x0D3A,
        SUBPIXEL_ಬಿಟ್ಟುಗಳು                    = 0x0D50,
        TEXTURE_1D                       = 0x0DE0,
        TEXTURE_2D                       = 0x0DE1,
        POLYGON_ಸ್ಥಾನಾಂತರ_UNITS             = 0x2A00,
        POLYGON_ಸ್ಥಾನಾಂತರ_ಬಿಂದು             = 0x2A01,
        POLYGON_ಸ್ಥಾನಾಂತರ_ಸಾಲು              = 0x2A02,
        POLYGON_ಸ್ಥಾನಾಂತರ_FILL              = 0x8037,
        POLYGON_ಸ್ಥಾನಾಂತರ_ಗುಣಕ            = 0x8038,
        TEXTURE_BINDING_1D               = 0x8068,
        TEXTURE_BINDING_2D               = 0x8069,
        TEXTURE_ಅಗಲ                    = 0x1000,
        TEXTURE_ಎತ್ತರ                   = 0x1001,
        TEXTURE_ಆಂತರಿಕ_FORMAT          = 0x1003,
        TEXTURE_BORDER_COLOR             = 0x1004,
        TEXTURE_RED_ಗಾತ್ರ                 = 0x805C,
        TEXTURE_GREEN_ಗಾತ್ರ               = 0x805D,
        TEXTURE_BLUE_ಗಾತ್ರ                = 0x805E,
        TEXTURE_ALPHA_ಗಾತ್ರ               = 0x805F,
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
        AND_ಹಿಮ್ಮುಖ                      = 0x1502,
        COPY                             = 0x1503,
        AND_INVERTED                     = 0x1504,
        NOOP                             = 0x1505,
        XOR                              = 0x1506,
        OR                               = 0x1507,
        NOR                              = 0x1508,
        EQUIV                            = 0x1509,
        INVERT                           = 0x150A,
        OR_ಹಿಮ್ಮುಖ                       = 0x150B,
        COPY_INVERTED                    = 0x150C,
        OR_INVERTED                      = 0x150D,
        NAND                             = 0x150E,
        ಗಣ_2                              = 0x150F,
        TEXTURE                          = 0x1702,
        COLOR                            = 0x1800,
        ಆಳ_3                            = 0x1801,
        STENCIL                          = 0x1802,
        STENCIL_ಸೂಚ್ಯಂಕ                    = 0x1901,
        ಆಳ_COMPONENT                  = 0x1902,
        RED                              = 0x1903,
        GREEN                            = 0x1904,
        BLUE                             = 0x1905,
        ALPHA                            = 0x1906,
        RGB                              = 0x1907,
        RGBA                             = 0x1908,
        ಬಿಂದು_2                            = 0x1B00,
        ಸಾಲು_2                             = 0x1B01,
        FILL                             = 0x1B02,
        KEEP                             = 0x1E00,
        REPLACE                          = 0x1E01,
        INCR                             = 0x1E02,
        DECR                             = 0x1E03,
        VENDOR                           = 0x1F00,
        RENDERER                         = 0x1F01,
        ಆವೃತ್ತಿ_2                         = 0x1F02,
        EXTENSIONS                       = 0x1F03,
        ಹತ್ತಿರದ                          = 0x2600,
        ರೇಖೀಯ                           = 0x2601,
        ಹತ್ತಿರದ_MIPMAP_ಹತ್ತಿರದ           = 0x2700,
        ರೇಖೀಯ_MIPMAP_ಹತ್ತಿರದ            = 0x2701,
        ಹತ್ತಿರದ_MIPMAP_ರೇಖೀಯ            = 0x2702,
        ರೇಖೀಯ_MIPMAP_ರೇಖೀಯ             = 0x2703,
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
        FUNC_ಕೂಡಿಸು                         = 0x8006,
        MIN                              = 0x8007,
        MAX                              = 0x8008,
        BLEND_EQUATION                   = 0x8009,
        FUNC_ಕಳೆ                    = 0x800A,
        FUNC_ಹಿಮ್ಮುಖ_ಕಳೆ            = 0x800B,
        CONVOLUTION_1D                   = 0x8010,
        CONVOLUTION_2D                   = 0x8011,
        SEPARABLE_2D                     = 0x8012,
        CONVOLUTION_BORDER_ಬಹುಲಕ          = 0x8013,
        CONVOLUTION_FILTER_SCALE         = 0x8014,
        CONVOLUTION_FILTER_BIAS          = 0x8015,
        REDUCE                           = 0x8016,
        CONVOLUTION_FORMAT               = 0x8017,
        CONVOLUTION_ಅಗಲ                = 0x8018,
        CONVOLUTION_ಎತ್ತರ               = 0x8019,
        MAX_CONVOLUTION_ಅಗಲ            = 0x801A,
        MAX_CONVOLUTION_ಎತ್ತರ           = 0x801B,
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
        HISTOGRAM_ಅಗಲ                  = 0x8026,
        HISTOGRAM_FORMAT                 = 0x8027,
        HISTOGRAM_RED_ಗಾತ್ರ               = 0x8028,
        HISTOGRAM_GREEN_ಗಾತ್ರ             = 0x8029,
        HISTOGRAM_BLUE_ಗಾತ್ರ              = 0x802A,
        HISTOGRAM_ALPHA_ಗಾತ್ರ             = 0x802B,
        HISTOGRAM_LUMINANCE_ಗಾತ್ರ         = 0x802C,
        HISTOGRAM_SINK                   = 0x802D,
        MINMAX                           = 0x802E,
        MINMAX_FORMAT                    = 0x802F,
        MINMAX_SINK                      = 0x8030,
        ಕೋಷ್ಟಕ_TOO_LARGE                  = 0x8031,
        COLOR_ಮಾತೃಕೆ                     = 0x80B1,
        COLOR_ಮಾತೃಕೆ_ರಾಶಿ_ಆಳ         = 0x80B2,
        MAX_COLOR_ಮಾತೃಕೆ_ರಾಶಿ_ಆಳ     = 0x80B3,
        POST_COLOR_ಮಾತೃಕೆ_RED_SCALE      = 0x80B4,
        POST_COLOR_ಮಾತೃಕೆ_GREEN_SCALE    = 0x80B5,
        POST_COLOR_ಮಾತೃಕೆ_BLUE_SCALE     = 0x80B6,
        POST_COLOR_ಮಾತೃಕೆ_ALPHA_SCALE    = 0x80B7,
        POST_COLOR_ಮಾತೃಕೆ_RED_BIAS       = 0x80B8,
        POST_COLOR_ಮಾತೃಕೆ_GREEN_BIAS     = 0x80B9,
        POST_COLOR_ಮಾತೃಕೆ_BLUE_BIAS      = 0x80BA,
        POST_COLOR_ಮಾತೃಕೆ_ALPHA_BIAS     = 0x80BB,
        COLOR_ಕೋಷ್ಟಕ                      = 0x80D0,
        POST_CONVOLUTION_COLOR_ಕೋಷ್ಟಕ     = 0x80D1,
        POST_COLOR_ಮಾತೃಕೆ_COLOR_ಕೋಷ್ಟಕ    = 0x80D2,
        PROXY_COLOR_ಕೋಷ್ಟಕ                = 0x80D3,
        PROXY_POST_CONVOLUTION_COLOR_ಕೋಷ್ಟಕ = 0x80D4,
        PROXY_POST_COLOR_ಮಾತೃಕೆ_COLOR_ಕೋಷ್ಟಕ = 0x80D5,
        COLOR_ಕೋಷ್ಟಕ_SCALE                = 0x80D6,
        COLOR_ಕೋಷ್ಟಕ_BIAS                 = 0x80D7,
        COLOR_ಕೋಷ್ಟಕ_FORMAT               = 0x80D8,
        COLOR_ಕೋಷ್ಟಕ_ಅಗಲ                = 0x80D9,
        COLOR_ಕೋಷ್ಟಕ_RED_ಗಾತ್ರ             = 0x80DA,
        COLOR_ಕೋಷ್ಟಕ_GREEN_ಗಾತ್ರ           = 0x80DB,
        COLOR_ಕೋಷ್ಟಕ_BLUE_ಗಾತ್ರ            = 0x80DC,
        COLOR_ಕೋಷ್ಟಕ_ALPHA_ಗಾತ್ರ           = 0x80DD,
        COLOR_ಕೋಷ್ಟಕ_LUMINANCE_ಗಾತ್ರ       = 0x80DE,
        COLOR_ಕೋಷ್ಟಕ_INTENSITY_ಗಾತ್ರ       = 0x80DF,
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
        PACK_IMAGE_ಎತ್ತರ                = 0x806C,
        UNPACK_SKIP_IMAGES               = 0x806D,
        UNPACK_IMAGE_ಎತ್ತರ              = 0x806E,
        TEXTURE_3D                       = 0x806F,
        PROXY_TEXTURE_3D                 = 0x8070,
        TEXTURE_ಆಳ                    = 0x8071,
        TEXTURE_WRAP_R                   = 0x8072,
        MAX_3_D_TEXTURE_ಗಾತ್ರ              = 0x8073,
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
        CLAMP_ಗಮ್ಯ_ಅಂಚು                    = 0x812F,
        TEXTURE_MIN_LOD                  = 0x813A,
        TEXTURE_MAX_LOD                  = 0x813B,
        TEXTURE_ತಳ_LEVEL               = 0x813C,
        TEXTURE_MAX_LEVEL                = 0x813D,
        SMOOTH_ಬಿಂದು_ಗಾತ್ರ_RANGE          = 0x0B12,
        SMOOTH_ಬಿಂದು_ಗಾತ್ರ_GRANULARITY    = 0x0B13,
        SMOOTH_ಸಾಲು_ಅಗಲ_RANGE          = 0x0B22,
        SMOOTH_ಸಾಲು_ಅಗಲ_GRANULARITY    = 0x0B23,
        ALIASED_ಸಾಲು_ಅಗಲ_RANGE         = 0x846E,

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
        ಸಕ್ರಿಯ_TEXTURE                   = 0x84E0,
        MULTISAMPLE                      = 0x809D,
        SAMPLE_ALPHA_ಗಮ್ಯ_COVERAGE         = 0x809E,
        SAMPLE_ALPHA_ಗಮ್ಯ_ONE              = 0x809F,
        SAMPLE_COVERAGE                  = 0x80A0,
        SAMPLE_BUFFERS                   = 0x80A8,
        SAMPLES                          = 0x80A9,
        SAMPLE_COVERAGE_ಮೌಲ್ಯ            = 0x80AA,
        SAMPLE_COVERAGE_INVERT           = 0x80AB,
        TEXTURE_CUBE_MAP                 = 0x8513,
        TEXTURE_BINDING_CUBE_MAP         = 0x8514,
        TEXTURE_CUBE_MAP_ಧನಾತ್ಮಕ_X      = 0x8515,
        TEXTURE_CUBE_MAP_NEGATIVE_X      = 0x8516,
        TEXTURE_CUBE_MAP_ಧನಾತ್ಮಕ_Y      = 0x8517,
        TEXTURE_CUBE_MAP_NEGATIVE_Y      = 0x8518,
        TEXTURE_CUBE_MAP_ಧನಾತ್ಮಕ_Z      = 0x8519,
        TEXTURE_CUBE_MAP_NEGATIVE_Z      = 0x851A,
        PROXY_TEXTURE_CUBE_MAP           = 0x851B,
        MAX_CUBE_MAP_TEXTURE_ಗಾತ್ರ        = 0x851C,
        COMPRESSED_RGB                   = 0x84ED,
        COMPRESSED_RGBA                  = 0x84EE,
        TEXTURE_COMPRESSION_HINT         = 0x84EF,
        TEXTURE_COMPRESSED_IMAGE_ಗಾತ್ರ    = 0x86A0,
        TEXTURE_COMPRESSED               = 0x86A1,
        ಅಂಕೆ_COMPRESSED_TEXTURE_FORMATS   = 0x86A2,
        COMPRESSED_TEXTURE_FORMATS       = 0x86A3,
        CLAMP_ಗಮ್ಯ_BORDER                  = 0x812D,

        // Version: 1.4
        BLEND_DST_RGB                    = 0x80C8,
        BLEND_SRC_RGB                    = 0x80C9,
        BLEND_DST_ALPHA                  = 0x80CA,
        BLEND_SRC_ALPHA                  = 0x80CB,
        ಬಿಂದು_FADE_ಮಿತಿ_ಗಾತ್ರ        = 0x8128,
        ಆಳ_COMPONENT16                = 0x81A5,
        ಆಳ_COMPONENT24                = 0x81A6,
        ಆಳ_COMPONENT32                = 0x81A7,
        MIRRORED_REPEAT                  = 0x8370,
        MAX_TEXTURE_LOD_BIAS             = 0x84FD,
        TEXTURE_LOD_BIAS                 = 0x8501,
        INCR_WRAP                        = 0x8507,
        DECR_WRAP                        = 0x8508,
        TEXTURE_ಆಳ_ಗಾತ್ರ               = 0x884A,
        TEXTURE_COMPARE_ಬಹುಲಕ             = 0x884C,
        TEXTURE_COMPARE_FUNC             = 0x884D,

        // Version: 1.5
        ಬಫರ್_ಗಾತ್ರ_2                      = 0x8764,
        ಬಫರ್_USAGE                     = 0x8765,
        QUERY_COUNTER_ಬಿಟ್ಟುಗಳು               = 0x8864,
        CURRENT_QUERY                    = 0x8865,
        QUERY_ಫಲಿತಾಂಶ                     = 0x8866,
        QUERY_ಫಲಿತಾಂಶ_ಲಭ್ಯ           = 0x8867,
        ARRAY_ಬಫರ್                     = 0x8892,
        ELEMENT_ARRAY_ಬಫರ್             = 0x8893,
        ARRAY_ಬಫರ್_BINDING             = 0x8894,
        ELEMENT_ARRAY_ಬಫರ್_BINDING     = 0x8895,
        ಶೃಂಗ_ATTRIB_ARRAY_ಬಫರ್_BINDING = 0x889F,
        READ_ONLY                        = 0x88B8,
        WRITE_ONLY                       = 0x88B9,
        READ_WRITE                       = 0x88BA,
        ಬಫರ್_ACCESS                    = 0x88BB,
        ಬಫರ್_MAPPED                    = 0x88BC,
        ಬಫರ್_MAP_ಸೂಚಕ               = 0x88BD,
        ಹರಿವು_DRAW                      = 0x88E0,
        ಹರಿವು_READ                      = 0x88E1,
        ಹರಿವು_COPY                      = 0x88E2,
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
        ಶೃಂಗ_ATTRIB_ARRAY_ENABLED      = 0x8622,
        ಶೃಂಗ_ATTRIB_ARRAY_ಗಾತ್ರ         = 0x8623,
        ಶೃಂಗ_ATTRIB_ARRAY_STRIDE       = 0x8624,
        ಶೃಂಗ_ATTRIB_ARRAY_TYPE         = 0x8625,
        CURRENT_ಶೃಂಗ_ATTRIB            = 0x8626,
        ಶೃಂಗ_ಕಾರ್ಯಕ್ರಮ_ಬಿಂದು_ಗಾತ್ರ        = 0x8642,
        ಶೃಂಗ_ATTRIB_ARRAY_ಸೂಚಕ      = 0x8645,
        STENCIL_ಹಿಂಭಾಗ_FUNC                = 0x8800,
        STENCIL_ಹಿಂಭಾಗ_FAIL                = 0x8801,
        STENCIL_ಹಿಂಭಾಗ_PASS_ಆಳ_FAIL     = 0x8802,
        STENCIL_ಹಿಂಭಾಗ_PASS_ಆಳ_PASS     = 0x8803,
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
        MAX_ಶೃಂಗ_ATTRIBS               = 0x8869,
        ಶೃಂಗ_ATTRIB_ARRAY_NORMALIZED   = 0x886A,
        MAX_TEXTURE_IMAGE_UNITS          = 0x8872,
        FRAGMENT_SHADER                  = 0x8B30,
        ಶೃಂಗ_SHADER                    = 0x8B31,
        MAX_FRAGMENT_ಏಕರೂಪ_COMPONENTS  = 0x8B49,
        MAX_ಶೃಂಗ_ಏಕರೂಪ_COMPONENTS    = 0x8B4A,
        MAX_VARYING_FLOATS               = 0x8B4B,
        MAX_ಶೃಂಗ_TEXTURE_IMAGE_UNITS   = 0x8B4C,
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
        DELETE_ಸ್ಥಿತಿ                    = 0x8B80,
        COMPILE_ಸ್ಥಿತಿ                   = 0x8B81,
        LINK_ಸ್ಥಿತಿ                      = 0x8B82,
        VALIDATE_ಸ್ಥಿತಿ                  = 0x8B83,
        INFO_LOG_ಉದ್ದಳತೆ                  = 0x8B84,
        ATTACHED_SHADERS                 = 0x8B85,
        ಸಕ್ರಿಯ_UNIFORMS                  = 0x8B86,
        ಸಕ್ರಿಯ_ಏಕರೂಪ_MAX_ಉದ್ದಳತೆ        = 0x8B87,
        SHADER_ಮೂಲ_ಉದ್ದಳತೆ             = 0x8B88,
        ಸಕ್ರಿಯ_ಗುಣಲಕ್ಷಣಗಳು                = 0x8B89,
        ಸಕ್ರಿಯ_ATTRIBUTE_MAX_ಉದ್ದಳತೆ      = 0x8B8A,
        FRAGMENT_SHADER_DERIVATIVE_HINT  = 0x8B8B,
        SHADING_LANGUAGE_ಆವೃತ್ತಿ         = 0x8B8C,
        CURRENT_ಕಾರ್ಯಕ್ರಮ                  = 0x8B8D,
        ಬಿಂದು_SPRITE_COORD_ORIGIN        = 0x8CA0,
        ಅಧೋ_ಎಡ                       = 0x8CA1,
        ಊರ್ಧ್ವ_ಎಡ                       = 0x8CA2,
        STENCIL_ಹಿಂಭಾಗ_REF                 = 0x8CA3,
        STENCIL_ಹಿಂಭಾಗ_ಮೌಲ್ಯ_ಮುಖವಾಡ          = 0x8CA4,
        STENCIL_ಹಿಂಭಾಗ_WRITEMASK           = 0x8CA5,

        // Version: 2.1
        PIXEL_PACK_ಬಫರ್                = 0x88EB,
        PIXEL_UNPACK_ಬಫರ್              = 0x88EC,
        PIXEL_PACK_ಬಫರ್_BINDING        = 0x88ED,
        PIXEL_UNPACK_ಬಫರ್_BINDING      = 0x88EF,
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
        ಶೃಂಗ_ARRAY_BINDING             = 0x85B5,

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
        MAP_READ_ಬಿಟ್                     = 0x0001,
        MAP_WRITE_ಬಿಟ್                    = 0x0002,
        MAP_INVALIDATE_RANGE_ಬಿಟ್         = 0x0004,
        MAP_INVALIDATE_ಬಫರ್_ಬಿಟ್        = 0x0008,
        MAP_FLUSH_EXPLICIT_ಬಿಟ್           = 0x0010,
        MAP_UNSYNCHRONIZED_ಬಿಟ್           = 0x0020,

        // Core Extension: ARB_half_float_vertex
        HALF_FLOAT                       = 0x140B,

        // Core Extension: ARB_framebuffer_sRGB
        FRAMEBUFFER_SRGB                 = 0x8DB9,

        // Core Extension: ARB_framebuffer_object
        INVALID_FRAMEBUFFER_OPERATION    = 0x0506,
        FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = 0x8210,
        FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = 0x8211,
        FRAMEBUFFER_ATTACHMENT_RED_ಗಾತ್ರ  = 0x8212,
        FRAMEBUFFER_ATTACHMENT_GREEN_ಗಾತ್ರ = 0x8213,
        FRAMEBUFFER_ATTACHMENT_BLUE_ಗಾತ್ರ = 0x8214,
        FRAMEBUFFER_ATTACHMENT_ALPHA_ಗಾತ್ರ = 0x8215,
        FRAMEBUFFER_ATTACHMENT_ಆಳ_ಗಾತ್ರ = 0x8216,
        FRAMEBUFFER_ATTACHMENT_STENCIL_ಗಾತ್ರ = 0x8217,
        FRAMEBUFFER_DEFAULT              = 0x8218,
        FRAMEBUFFER_UNDEFINED            = 0x8219,
        ಆಳ_STENCIL_ATTACHMENT         = 0x821A,
        ಸೂಚ್ಯಂಕ_2                            = 0x8222,
        MAX_RENDERBUFFER_ಗಾತ್ರ            = 0x84E8,
        ಆಳ_STENCIL                    = 0x84F9,
        UNSIGNED_INT_24_8                = 0x84FA,
        DEPTH24_STENCIL8                 = 0x88F0,
        TEXTURE_STENCIL_ಗಾತ್ರ             = 0x88F1,
        TEXTURE_RED_TYPE                 = 0x8C10,
        TEXTURE_GREEN_TYPE               = 0x8C11,
        TEXTURE_BLUE_TYPE                = 0x8C12,
        TEXTURE_ALPHA_TYPE               = 0x8C13,
        TEXTURE_ಆಳ_TYPE               = 0x8C16,
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
        FRAMEBUFFER_INCOMPLETE_DRAW_ಬಫರ್ = 0x8CDB,
        FRAMEBUFFER_INCOMPLETE_READ_ಬಫರ್ = 0x8CDC,
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
        ಆಳ_ATTACHMENT                 = 0x8D00,
        STENCIL_ATTACHMENT               = 0x8D20,
        FRAMEBUFFER                      = 0x8D40,
        RENDERBUFFER                     = 0x8D41,
        RENDERBUFFER_ಅಗಲ               = 0x8D42,
        RENDERBUFFER_ಎತ್ತರ              = 0x8D43,
        RENDERBUFFER_ಆಂತರಿಕ_FORMAT     = 0x8D44,
        STENCIL_INDEX1                   = 0x8D46,
        STENCIL_INDEX4                   = 0x8D47,
        STENCIL_INDEX8                   = 0x8D48,
        STENCIL_INDEX16                  = 0x8D49,
        RENDERBUFFER_RED_ಗಾತ್ರ            = 0x8D50,
        RENDERBUFFER_GREEN_ಗಾತ್ರ          = 0x8D51,
        RENDERBUFFER_BLUE_ಗಾತ್ರ           = 0x8D52,
        RENDERBUFFER_ALPHA_ಗಾತ್ರ          = 0x8D53,
        RENDERBUFFER_ಆಳ_ಗಾತ್ರ          = 0x8D54,
        RENDERBUFFER_STENCIL_ಗಾತ್ರ        = 0x8D55,
        FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = 0x8D56,
        MAX_SAMPLES                      = 0x8D57,
        TEXTURE_LUMINANCE_TYPE           = 0x8C14,
        TEXTURE_INTENSITY_TYPE           = 0x8C15,

        // Core Extension: ARB_depth_buffer_float
        ಆಳ_COMPONENT32_F               = 0x8CAC,
        DEPTH32F_STENCIL8                = 0x8CAD,
        FLOAT_32_UNSIGNED_INT_24_8_REV   = 0x8DAD,

        // Version: 3.0
        COMPARE_REF_ಗಮ್ಯ_TEXTURE           = 0x884E,
        CLIP_DISTANCE0                   = 0x3000,
        CLIP_DISTANCE1                   = 0x3001,
        CLIP_DISTANCE2                   = 0x3002,
        CLIP_DISTANCE3                   = 0x3003,
        CLIP_DISTANCE4                   = 0x3004,
        CLIP_DISTANCE5                   = 0x3005,
        CLIP_DISTANCE6                   = 0x3006,
        CLIP_DISTANCE7                   = 0x3007,
        MAX_CLIP_DISTANCES               = 0x0D32,
        MAJOR_ಆವೃತ್ತಿ                    = 0x821B,
        MINOR_ಆವೃತ್ತಿ                    = 0x821C,
        ಅಂಕೆ_EXTENSIONS                   = 0x821D,
        CONTEXT_FLAGS                    = 0x821E,
        COMPRESSED_RED                   = 0x8225,
        COMPRESSED_RG                    = 0x8226,
        CONTEXT_FLAG_FORWARD_COMPATIBLE_ಬಿಟ್ = 0x0001,
        RGBA32F                          = 0x8814,
        RGB32F                           = 0x8815,
        RGBA16F                          = 0x881A,
        RGB16F                           = 0x881B,
        ಶೃಂಗ_ATTRIB_ARRAY_INTEGER      = 0x88FD,
        MAX_ARRAY_TEXTURE_LAYERS         = 0x88FF,
        MIN_ಕಾರ್ಯಕ್ರಮ_TEXEL_ಸ್ಥಾನಾಂತರ         = 0x8904,
        MAX_ಕಾರ್ಯಕ್ರಮ_TEXEL_ಸ್ಥಾನಾಂತರ         = 0x8905,
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
        TEXTURE_SHARED_ಗಾತ್ರ              = 0x8C3F,
        TRANSFORM_FEEDBACK_VARYING_MAX_ಉದ್ದಳತೆ = 0x8C76,
        TRANSFORM_FEEDBACK_ಬಫರ್_ಬಹುಲಕ   = 0x8C7F,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = 0x8C80,
        TRANSFORM_FEEDBACK_VARYINGS      = 0x8C83,
        TRANSFORM_FEEDBACK_ಬಫರ್_ಆರಂಭ  = 0x8C84,
        TRANSFORM_FEEDBACK_ಬಫರ್_ಗಾತ್ರ   = 0x8C85,
        PRIMITIVES_GENERATED             = 0x8C87,
        TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88,
        RASTERIZER_DISCARD               = 0x8C89,
        MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = 0x8C8B,
        INTERLEAVED_ATTRIBS              = 0x8C8C,
        SEPARATE_ATTRIBS                 = 0x8C8D,
        TRANSFORM_FEEDBACK_ಬಫರ್        = 0x8C8E,
        TRANSFORM_FEEDBACK_ಬಫರ್_BINDING = 0x8C8F,
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
        ಬಫರ್_ACCESS_FLAGS              = 0x911F,
        ಬಫರ್_MAP_ಉದ್ದಳತೆ                = 0x9120,
        ಬಫರ್_MAP_ಸ್ಥಾನಾಂತರ                = 0x9121,

        // Core Extension: ARB_uniform_buffer_object
        ಏಕರೂಪ_ಬಫರ್                   = 0x8A11,
        ಏಕರೂಪ_ಬಫರ್_BINDING           = 0x8A28,
        ಏಕರೂಪ_ಬಫರ್_ಆರಂಭ             = 0x8A29,
        ಏಕರೂಪ_ಬಫರ್_ಗಾತ್ರ              = 0x8A2A,
        MAX_ಶೃಂಗ_ಏಕರೂಪ_ಖಂಡಗಳು        = 0x8A2B,
        MAX_FRAGMENT_ಏಕರೂಪ_ಖಂಡಗಳು      = 0x8A2D,
        MAX_COMBINED_ಏಕರೂಪ_ಖಂಡಗಳು      = 0x8A2E,
        MAX_ಏಕರೂಪ_ಬಫರ್_BINDINGS      = 0x8A2F,
        MAX_ಏಕರೂಪ_ಖಂಡ_ಗಾತ್ರ           = 0x8A30,
        MAX_COMBINED_ಶೃಂಗ_ಏಕರೂಪ_COMPONENTS = 0x8A31,
        MAX_COMBINED_FRAGMENT_ಏಕರೂಪ_COMPONENTS = 0x8A33,
        ಏಕರೂಪ_ಬಫರ್_ಸ್ಥಾನಾಂತರ_ALIGNMENT  = 0x8A34,
        ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_MAX_NAME_ಉದ್ದಳತೆ = 0x8A35,
        ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡಗಳು            = 0x8A36,
        ಏಕರೂಪ_TYPE                     = 0x8A37,
        ಏಕರೂಪ_ಗಾತ್ರ                     = 0x8A38,
        ಏಕರೂಪ_NAME_ಉದ್ದಳತೆ              = 0x8A39,
        ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ_2              = 0x8A3A,
        ಏಕರೂಪ_ಸ್ಥಾನಾಂತರ                   = 0x8A3B,
        ಏಕರೂಪ_ARRAY_STRIDE             = 0x8A3C,
        ಏಕರೂಪ_ಮಾತೃಕೆ_STRIDE            = 0x8A3D,
        ಏಕರೂಪ_IS_ಅಡ್ಡಸಾಲು_MAJOR             = 0x8A3E,
        ಏಕರೂಪ_ಖಂಡ_BINDING            = 0x8A3F,
        ಏಕರೂಪ_ಖಂಡ_ದತ್ತಾಂಶ_ಗಾತ್ರ          = 0x8A40,
        ಏಕರೂಪ_ಖಂಡ_NAME_ಉದ್ದಳತೆ        = 0x8A41,
        ಏಕರೂಪ_ಖಂಡ_ಸಕ್ರಿಯ_UNIFORMS    = 0x8A42,
        ಏಕರೂಪ_ಖಂಡ_ಸಕ್ರಿಯ_ಏಕರೂಪ_INDICES = 0x8A43,
        ಏಕರೂಪ_ಖಂಡ_REFERENCED_BY_ಶೃಂಗ_SHADER = 0x8A44,
        ಏಕರೂಪ_ಖಂಡ_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46,
        INVALID_ಸೂಚ್ಯಂಕ                    = 0xFFFFFFFF,
        MAX_GEOMETRY_ಏಕರೂಪ_ಖಂಡಗಳು      = 0x8A2C,
        MAX_COMBINED_GEOMETRY_ಏಕರೂಪ_COMPONENTS = 0x8A32,
        ಏಕರೂಪ_ಖಂಡ_REFERENCED_BY_GEOMETRY_SHADER = 0x8A45,

        // Core Extension: ARB_copy_buffer
        COPY_READ_ಬಫರ್                 = 0x8F36,
        COPY_WRITE_ಬಫರ್                = 0x8F37,
        COPY_READ_ಬಫರ್_BINDING         = 0x8F36,
        COPY_WRITE_ಬಫರ್_BINDING        = 0x8F37,

        // Version: 3.1
        SAMPLER_2D_RECT                  = 0x8B63,
        SAMPLER_2D_RECT_SHADOW           = 0x8B64,
        SAMPLER_ಬಫರ್                   = 0x8DC2,
        INT_SAMPLER_2D_RECT              = 0x8DCD,
        INT_SAMPLER_ಬಫರ್               = 0x8DD0,
        UNSIGNED_INT_SAMPLER_2D_RECT     = 0x8DD5,
        UNSIGNED_INT_SAMPLER_ಬಫರ್      = 0x8DD8,
        TEXTURE_ಬಫರ್                   = 0x8C2A,
        MAX_TEXTURE_ಬಫರ್_ಗಾತ್ರ          = 0x8C2B,
        TEXTURE_BINDING_ಬಫರ್           = 0x8C2C,
        TEXTURE_ಬಫರ್_ದತ್ತಾಂಶ_STORE_BINDING = 0x8C2D,
        TEXTURE_RECTANGLE                = 0x84F5,
        TEXTURE_BINDING_RECTANGLE        = 0x84F6,
        PROXY_TEXTURE_RECTANGLE          = 0x84F7,
        MAX_RECTANGLE_TEXTURE_ಗಾತ್ರ       = 0x84F8,
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
        PRIMITIVE_RESTART_ಸೂಚ್ಯಂಕ          = 0x8F9E,

        // Legacy
        ಶೃಂಗ_ARRAY = 0x8074,
        ಸಾಮಾನ್ಯ_ARRAY = 0x8075,
        COLOR_ARRAY = 0x8076,
        TEXTURE_COORD_ARRAY = 0x8078,
        TEXTURE_ENV = 0x2300,
        TEXTURE_ENV_ಬಹುಲಕ = 0x2200,
        MODELVIEW = 0x1700,
        PROJECTION = 0x1701,
        LIGHTING = 0x0B50
    };

    // Extension: 1.1
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CullFace)(GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *FrontFace)(GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Hint)(GLenum target, GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಸಾಲು_ಅಗಲ)(GLfloat ಅಗಲ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಿಂದು_ಗಾತ್ರ)(GLfloat size);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Polygon_ಬಹುಲಕ)(GLenum face, GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Scissor)(GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexParameterf)(GLenum target, GLenum pname, GLfloat param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexParameterfv)(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexParameteri)(GLenum target, GLenum pname, GLint param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexParameteriv)(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei ಅಗಲ, GLint border, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLint border, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Draw_ಬಫರ್)(GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Clear)(GLbitfield ಮುಖವಾಡ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClearStencil)(GLint s);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Clear_ಆಳ)(GLdouble ಆಳ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Stencil_ಮುಖವಾಡ)(GLuint ಮುಖವಾಡ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Color_ಮುಖವಾಡ)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಆಳ_ಮುಖವಾಡ)(GLboolean flag);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Disable)(GLenum cap);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Enable)(GLenum cap);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಮುಕ್ತಾಯ)();
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Flush)();
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BlendFunc)(GLenum sfactor, GLenum dfactor);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *LogicOp)(GLenum opcode);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *StencilFunc)(GLenum func, GLint ref, GLuint ಮುಖವಾಡ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಆಳ_Func)(GLenum func);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PixelStoref)(GLenum pname, GLfloat param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PixelStorei)(GLenum pname, GLint param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Read_ಬಫರ್)(GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ReadPixels)(GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetBooleanv)(GLenum pname, GLboolean *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetDoublev)(GLenum pname, GLdouble *params);
    ಬಾಹ್ಯ GLenum (CODEGEN_FUNCPTR *GetError)();
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetFloatv)(GLenum pname, GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetIntegerv)(GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ GLubyte * (CODEGEN_FUNCPTR *GetString)(GLenum name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsEnabled)(GLenum cap);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಆಳ_Range)(GLdouble ren_near, GLdouble ren_far);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Viewport)(GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DrawArrays)(GLenum ಬಹುಲಕ, GLint first, GLsizei ಎಣಿಕೆ_3);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DrawElements)(GLenum ಬಹುಲಕ, GLsizei ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *indices);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetPointerv)(GLenum pname, GLvoid* *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Polygon_ಸ್ಥಾನಾಂತರ)(GLfloat ಗುಣಕ, GLfloat units);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ಅಗಲ, GLint border);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLint border);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei ಅಗಲ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei ಅಗಲ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BindTexture)(GLenum target, GLuint texture);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DeleteTextures)(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *textures);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GenTextures)(GLsizei n, GLuint *textures);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsTexture)(GLuint texture);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Indexub)(GLubyte c);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Indexubv)(ಸ್ಥಿರಾಂಕ GLubyte *c);

    // Extension: 1.2
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BlendEquation)(GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DrawRangeElements)(GLenum ಬಹುಲಕ, GLuint ಆರಂಭ, GLuint end, GLsizei ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *indices);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLsizei ಆಳ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ);

    // Extension: 1.3
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಸಕ್ರಿಯ_Texture)(GLenum texture);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *SampleCoverage)(GLfloat value, GLboolean invert);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLsizei ಆಳ, GLint border, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLint border, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei ಅಗಲ, GLint border, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLsizei ಆಳ, GLenum format, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei ಅಗಲ, GLenum format, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);

    // Extension: 1.4
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *MultiDrawArrays)(GLenum ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ GLint *first, ಸ್ಥಿರಾಂಕ GLsizei *ಎಣಿಕೆ_3, GLsizei drawcount);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *MultiDrawElements)(GLenum ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ GLsizei *ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid* ಸ್ಥಿರಾಂಕ *indices, GLsizei drawcount);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಿಂದು_Parameterf)(GLenum pname, GLfloat param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಿಂದು_Parameterfv)(GLenum pname, ಸ್ಥಿರಾಂಕ GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಿಂದು_Parameteri)(GLenum pname, GLint param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಿಂದು_Parameteriv)(GLenum pname, ಸ್ಥಿರಾಂಕ GLint *params);

    // Extension: 1.5
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GenQueries)(GLsizei n, GLuint *ids);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DeleteQueries)(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *ids);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsQuery)(GLuint id);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BeginQuery)(GLenum target, GLuint id);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಕೊನೆ_Query)(GLenum target);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetQueryiv)(GLenum target, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Bind_ಬಫರ್)(GLenum target, GLuint ಬಫರ್);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DeleteBuffers)(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *buffers);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GenBuffers)(GLsizei n, GLuint *buffers);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *Is_ಬಫರ್)(GLuint ಬಫರ್);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಫರ್_ದತ್ತಾಂಶ)(GLenum target, GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ, GLenum usage);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಬಫರ್_Sub_ದತ್ತಾಂಶ)(GLenum target, GLintptr offset, GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ GLvoid* (CODEGEN_FUNCPTR *Map_ಬಫರ್)(GLenum target, GLenum access);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *Unmap_ಬಫರ್)(GLenum target);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಬಫರ್_Parameteriv)(GLenum target, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಬಫರ್_Pointerv)(GLenum target, GLenum pname, GLvoid* *params);

    // Extension: 2.0
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BlendEquationSeparate)(GLenum ಬಹುಲಕ_RGB, GLenum ಬಹುಲಕ_Alpha);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DrawBuffers)(GLsizei n, ಸ್ಥಿರಾಂಕ GLenum *bufs);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *StencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *StencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint ಮುಖವಾಡ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Stencil_ಮುಖವಾಡ_Separate)(GLenum face, GLuint ಮುಖವಾಡ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *AttachShader)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint shader);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BindAttribLocation)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *CompileShader)(GLuint shader);
    ಬಾಹ್ಯ GLuint (CODEGEN_FUNCPTR *Create_ಕಾರ್ಯಕ್ರಮ)();
    ಬಾಹ್ಯ GLuint (CODEGEN_FUNCPTR *CreateShader)(GLenum type);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Delete_ಕಾರ್ಯಕ್ರಮ)(GLuint ಕಾರ್ಯಕ್ರಮ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DeleteShader)(GLuint shader);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DetachShader)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint shader);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Disable_ಶೃಂಗ_Attrib_Array)(GLuint ಸೂಚ್ಯಂಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Enable_ಶೃಂಗ_Attrib_Array)(GLuint ಸೂಚ್ಯಂಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಸಕ್ರಿಯ_Attrib)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಸಕ್ರಿಯ_ಏಕರೂಪ)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetAttachedShaders)(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei max_ಎಣಿಕೆ, GLsizei *ಎಣಿಕೆ_3, GLuint *obj);
    ಬಾಹ್ಯ GLint (CODEGEN_FUNCPTR *GetAttribLocation)(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetProgramiv)(GLuint ಕಾರ್ಯಕ್ರಮ, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಕಾರ್ಯಕ್ರಮ_Info_Log)(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *infoLog);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetShaderInfoLog)(GLuint shader, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *infoLog);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_Shader_ಮೂಲ)(GLuint shader, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *ಮೂಲ);
    ಬಾಹ್ಯ GLint (CODEGEN_FUNCPTR *Get_ಏಕರೂಪ_Location)(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetUniformfv)(GLuint ಕಾರ್ಯಕ್ರಮ, GLint location, GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetUniformiv)(GLuint ಕಾರ್ಯಕ್ರಮ, GLint location, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಶೃಂಗ_Attribdv)(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLdouble *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಶೃಂಗ_Attribfv)(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLfloat *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಶೃಂಗ_Attribiv)(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಶೃಂಗ_Attrib_Pointerv)(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLvoid* *pointer);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *Is_ಕಾರ್ಯಕ್ರಮ)(GLuint ಕಾರ್ಯಕ್ರಮ);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsShader)(GLuint shader);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Link_ಕಾರ್ಯಕ್ರಮ)(GLuint ಕಾರ್ಯಕ್ರಮ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Shader_ಮೂಲ)(GLuint shader, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *string, ಸ್ಥಿರಾಂಕ GLint *length);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Use_ಕಾರ್ಯಕ್ರಮ)(GLuint ಕಾರ್ಯಕ್ರಮ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform1f)(GLint location, GLfloat v0);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform2f)(GLint location, GLfloat v0, GLfloat v1);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform1i)(GLint location, GLint v0);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform2i)(GLint location, GLint v0, GLint v1);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform1fv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform2fv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform3fv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform4fv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform1iv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform2iv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform3iv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform4iv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix2fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix3fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix4fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Validate_ಕಾರ್ಯಕ್ರಮ)(GLuint ಕಾರ್ಯಕ್ರಮ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_ಸೂಚಕ)(GLuint ಸೂಚ್ಯಂಕ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *pointer);

    // Extension: 2.1
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix2x3fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix3x2fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix2x4fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix4x2fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix3x4fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_Matrix4x3fv)(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value);

    // Extension: ARB_vertex_array_object
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Bind_ಶೃಂಗ_Array)(GLuint ren_array);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Delete_ಶೃಂಗ_Arrays)(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *arrays);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Gen_ಶೃಂಗ_Arrays)(GLsizei n, GLuint *arrays);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *Is_ಶೃಂಗ_Array)(GLuint ren_array);

    // Extension: ARB_map_buffer_range
    ಬಾಹ್ಯ GLvoid* (CODEGEN_FUNCPTR *Map_ಬಫರ್_Range)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Flush_Mapped_ಬಫರ್_Range)(GLenum target, GLintptr offset, GLsizeiptr length);

    // Extension: ARB_framebuffer_object
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsRenderbuffer)(GLuint renderbuffer);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BindRenderbuffer)(GLenum target, GLuint renderbuffer);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DeleteRenderbuffers)(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *renderbuffers);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsFramebuffer)(GLuint framebuffer);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BindFramebuffer)(GLenum target, GLuint framebuffer);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DeleteFramebuffers)(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *framebuffers);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GenFramebuffers)(GLsizei n, GLuint *framebuffers);
    ಬಾಹ್ಯ GLenum (CODEGEN_FUNCPTR *Check_Framebuffer_ಸ್ಥಿತಿ)(GLenum target);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *FramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *FramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GenerateMipmap)(GLenum target);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ಮುಖವಾಡ, GLenum filter);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *RenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *FramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

    // Extension: 3.0
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ColorMaski)(GLuint ಸೂಚ್ಯಂಕ, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetBooleani_v)(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLboolean *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetIntegeri_v)(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLint *ದತ್ತಾಂಶ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Enablei)(GLenum target, GLuint ಸೂಚ್ಯಂಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Disablei)(GLenum target, GLuint ಸೂಚ್ಯಂಕ);
    ಬಾಹ್ಯ GLboolean (CODEGEN_FUNCPTR *IsEnabledi)(GLenum target, GLuint ಸೂಚ್ಯಂಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BeginTransformFeedback)(GLenum primitive_ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಕೊನೆ_Transform_Feedback)();
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Bind_ಬಫರ್_Range)(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLuint ಬಫರ್, GLintptr offset, GLsizeiptr size);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Bind_ಬಫರ್_ತಳ)(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLuint ಬಫರ್);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TransformFeedbackVaryings)(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *varyings, GLenum ಬಫರ್_ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTransformFeedbackVarying)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClampColor)(GLenum target, GLenum clamp);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *BeginConditionalRender)(GLuint id, GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಕೊನೆ_Conditional_Render)();
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_IPointer)(GLuint ಸೂಚ್ಯಂಕ, GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *pointer);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಶೃಂಗ_Attrib_Iiv)(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಶೃಂಗ_Attrib_Iuiv)(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLuint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I1i)(GLuint ಸೂಚ್ಯಂಕ, GLint x);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I2i)(GLuint ಸೂಚ್ಯಂಕ, GLint x, GLint y);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I3i)(GLuint ಸೂಚ್ಯಂಕ, GLint x, GLint y, GLint z);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4i)(GLuint ಸೂಚ್ಯಂಕ, GLint x, GLint y, GLint z, GLint w);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I1ui)(GLuint ಸೂಚ್ಯಂಕ, GLuint x);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I2ui)(GLuint ಸೂಚ್ಯಂಕ, GLuint x, GLuint y);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I3ui)(GLuint ಸೂಚ್ಯಂಕ, GLuint x, GLuint y, GLuint z);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4ui)(GLuint ಸೂಚ್ಯಂಕ, GLuint x, GLuint y, GLuint z, GLuint w);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I1iv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I2iv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I3iv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4iv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I1uiv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I2uiv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I3uiv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4uiv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4bv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLbyte *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4sv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLshort *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4ubv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLubyte *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_Attrib_I4usv)(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLushort *v);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetUniformuiv)(GLuint ಕಾರ್ಯಕ್ರಮ, GLint location, GLuint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Bind_Frag_ದತ್ತಾಂಶ_Location)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint color, ಸ್ಥಿರಾಂಕ GLchar *name);
    ಬಾಹ್ಯ GLint (CODEGEN_FUNCPTR *Get_Frag_ದತ್ತಾಂಶ_Location)(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform1ui)(GLint location, GLuint v0);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform2ui)(GLint location, GLuint v0, GLuint v1);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform1uiv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform2uiv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform3uiv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Uniform4uiv)(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexParameterIiv)(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexParameterIuiv)(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLuint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *GetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClearBufferiv)(GLenum ಬಫರ್, GLint drawbuffer, ಸ್ಥಿರಾಂಕ GLint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClearBufferuiv)(GLenum ಬಫರ್, GLint drawbuffer, ಸ್ಥಿರಾಂಕ GLuint *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClearBufferfv)(GLenum ಬಫರ್, GLint drawbuffer, ಸ್ಥಿರಾಂಕ GLfloat *value);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ClearBufferfi)(GLenum ಬಫರ್, GLint drawbuffer, GLfloat ಆಳ, GLint stencil);
    ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ GLubyte * (CODEGEN_FUNCPTR *GetStringi)(GLenum name, GLuint ಸೂಚ್ಯಂಕ);

    // Extension: ARB_uniform_buffer_object
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಏಕರೂಪ_Indices)(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei ಏಕರೂಪ_ಎಣಿಕೆ, ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *ಏಕರೂಪ_ಹೆಸರುಗಳು, GLuint *ಏಕರೂಪ_Indices);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಸಕ್ರಿಯ_Uniformsiv)(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei ಏಕರೂಪ_ಎಣಿಕೆ, ಸ್ಥಿರಾಂಕ GLuint *ಏಕರೂಪ_Indices, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *ಏಕರೂಪ_Name);
    ಬಾಹ್ಯ GLuint (CODEGEN_FUNCPTR *Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ)(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *ಏಕರೂಪ_ಖಂಡ_Name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, GLenum pname, GLint *params);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *ಏಕರೂಪ_ಖಂಡ_Name);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಏಕರೂಪ_ಖಂಡ_Binding)(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, GLuint ಏಕರೂಪ_ಖಂಡ_Binding_2);

    // Extension: ARB_copy_buffer
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ)(GLenum read_ಗುರಿ, GLenum write_ಗುರಿ, GLintptr read_ಸ್ಥಾನಾಂತರ, GLintptr write_ಸ್ಥಾನಾಂತರ, GLsizeiptr size);

    // Extension: 3.1
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DrawArraysInstanced)(GLenum ಬಹುಲಕ, GLint first, GLsizei ಎಣಿಕೆ_3, GLsizei instancecount);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DrawElementsInstanced)(GLenum ಬಹುಲಕ, GLsizei ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *indices, GLsizei instancecount);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Tex_ಬಫರ್)(GLenum target, GLenum internalformat, GLuint ಬಫರ್);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Primitive_Restart_ಸೂಚ್ಯಂಕ)(GLuint ಸೂಚ್ಯಂಕ);

    // Legacy
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *EnableClientState)(GLenum cap);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *DisableClientState)(GLenum cap);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಶೃಂಗ_ಸೂಚಕ)(GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಸಾಮಾನ್ಯ_ಸೂಚಕ)(GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Color_ಸೂಚಕ)(GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Tex_Coord_ಸೂಚಕ)(GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *TexEnvi)(GLenum target, GLenum pname, GLint param);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ಮಾತೃಕೆ_ಬಹುಲಕ)(GLenum ಬಹುಲಕ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *ತುಂಬು_ತಟಸ್ಥ)(ಶೂನ್ಯ);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Ortho)(GLdouble ಎಡ, GLdouble ಬಲ, GLdouble bottom, GLdouble ತುದಿ_2, GLdouble near_val, GLdouble far_val);
    ಬಾಹ್ಯ ಶೂನ್ಯ (CODEGEN_FUNCPTR *Color3d)(GLdouble red, GLdouble green, GLdouble blue);
}

#ಆದರೆ_ಅಂತ್ಯ // OPENGL_NOLOAD_STYLE_HPP
