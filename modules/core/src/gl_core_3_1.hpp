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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENGL_NOLOAD_STYLE_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENGL_NOLOAD_STYLE_HPP

#ਜੇ defined(__gl_h_) || defined(__GL_H__)
#ਗਲਤੀ Attempt ਮੰਜ਼ਿਲ include ਸਵੈਚਾਲਿਤ-generated ਸਿਰਲੇਖ after including gl.h
#ਜੇ_ਅੰਤ
#ਜੇ defined(__glext_h_) || defined(__GLEXT_H_)
#ਗਲਤੀ Attempt ਮੰਜ਼ਿਲ include ਸਵੈਚਾਲਿਤ-generated ਸਿਰਲੇਖ after including glext.h
#ਜੇ_ਅੰਤ
#ਜੇ defined(__gl_ATI_h_)
#ਗਲਤੀ Attempt ਮੰਜ਼ਿਲ include ਸਵੈਚਾਲਿਤ-generated ਸਿਰਲੇਖ after including glATI.h
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __gl_h_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GL_H__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __glext_h_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __GLEXT_H_
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __gl_ATI_h_

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ APIENTRY
    #ਜੇ defined(__MINGW32__)
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WIN32_LEAN_AND_ਔਸਤ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WIN32_LEAN_AND_ਔਸਤ 1
        #ਜੇ_ਅੰਤ
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NOMINMAX
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOMINMAX
        #ਜੇ_ਅੰਤ
        #ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
    #ਜਾਂਜੇ (defined(_MSC_VER) && _MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WIN32_LEAN_AND_ਔਸਤ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WIN32_LEAN_AND_ਔਸਤ 1
        #ਜੇ_ਅੰਤ
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NOMINMAX
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOMINMAX
        #ਜੇ_ਅੰਤ
        #ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ APIENTRY
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ // APIENTRY

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CODEGEN_FUNCPTR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CODEGEN_REMOVE_FUNCPTR
    #ਜੇ defined(_WIN32)
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CODEGEN_FUNCPTR APIENTRY
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CODEGEN_FUNCPTR
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ // CODEGEN_FUNCPTR

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ਲੱਦੋ_GEN_BASIC_OPENGL_TYPEDEFS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GL_ਲੱਦੋ_GEN_BASIC_OPENGL_TYPEDEFS
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ GLenum;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ GLboolean;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ GLbitfield;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ GLbyte;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਛੋਟਾ GLshort;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ GLint;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ GLsizei;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ GLubyte;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ GLushort;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ GLuint;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ GLfloat;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦਸ਼ਮਲਵ GLclampf;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦੁੱਗਣਾ GLdouble;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਦੁੱਗਣਾ GLclampd;
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GLvoid ਖਾਲੀ
#ਜੇ_ਅੰਤ // GL_LOAD_GEN_BASIC_OPENGL_TYPEDEFS

#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ਸੰਸਕਰਣ_2_0
    // GL type for program/shader text
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ GLchar;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ਸੰਸਕਰਣ_1_5
    // GL types for handling large vertex buffer objects
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ptrdiff_t GLintptr;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ptrdiff_t GLsizeiptr;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ARB_ਸ਼ੀਰਸ਼_ਬਫਰ_object
    // GL types for handling large vertex buffer objects
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ptrdiff_t GLintptrARB;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ptrdiff_t GLsizeiptrARB;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ARB_shader_objects
    // GL types for program/shader text and shader object handles
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ GLcharARB;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ GLhandleARB;
#ਜੇ_ਅੰਤ

// GL type for "half" precision (s10e5) float data in host memory
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ARB_half_float_pixel
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ GLhalfARB;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_NV_half_float
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ GLhalfNV;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GLEXT_64_TYPES_DEFINED
    // This code block is duplicated in glxext.h, so must be protected
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GLEXT_64_TYPES_DEFINED

    // Define int32_t, int64_t, and uint64_t types for UST/MSC
    // (as used in the GL_EXT_timer_query extension)
    #ਜੇ defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        #ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
    #ਜਾਂਜੇ defined(__sun__) || defined(__digital__)
        #ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
        #ਜੇ defined(__STDC__)
            #ਜੇ defined(__arch64__) || defined(_LP64)
                ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਪੂਰਨ int64_t;
                ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ uint64_t;
            #ਨਹੀਂਤਾਂ
                ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ int64_t;
                ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ uint64_t;
            #ਜੇ_ਅੰਤ // __arch64__
        #ਜੇ_ਅੰਤ // __STDC__
    #ਜਾਂਜੇ defined( __VMS ) || defined(__sgi)
        #ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
    #ਜਾਂਜੇ defined(__SCO__) || defined(__USLC__)
        #ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
    #ਜਾਂਜੇ defined(__UNIXOS2__) || defined(__SOL64__)
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਪੂਰਨ int32_t;
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ int64_t;
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ uint64_t;
    #ਜਾਂਜੇ defined(_WIN32) && defined(__GNUC__)
        #ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
    #ਜਾਂਜੇ defined(_WIN32)
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ __int32 int32_t;
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ __int64 int64_t;
        ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ __int64 uint64_t;
    #ਨਹੀਂਤਾਂ
        // Fallback if nothing above works
        #ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_EXT_timer_query
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ int64_t GLint64EXT;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint64_t GLuint64EXT;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ARB_sync
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ int64_t GLint64;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint64_t GLuint64;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ __GLsync *GLsync;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ARB_cl_event
    // These incomplete types let us declare types compatible with OpenCL's cl_context and cl_event
    ਬਣਤਰ _cl_context;
    ਬਣਤਰ _cl_event;
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_ARB_debug_output
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (APIENTRY *GLDEBUGPROCARB)(GLenum ਸਰੋਤ,GLenum type,GLuint id,GLenum severity,GLsizei length,ਅਚਲ GLchar *message,GLvoid *userParam);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_AMD_debug_output
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (APIENTRY *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,ਅਚਲ GLchar *message,GLvoid *userParam);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_KHR_debug
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (APIENTRY *GLDEBUGPROC)(GLenum ਸਰੋਤ,GLenum type,GLuint id,GLenum severity,GLsizei length,ਅਚਲ GLchar *message,GLvoid *userParam);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GL_NV_vdpau_interop
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLintptr GLvdpauSurfaceNV;
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ gl
{
    ਗਿਣਤੀ
    {
        // Version: 1.1
        ਡੂੰਘਾਈ_ਬਫਰ_ਦੋਆਧਾਰੀ_ਅੰਕ                 = 0x00000100,
        STENCIL_ਬਫਰ_ਦੋਆਧਾਰੀ_ਅੰਕ               = 0x00000400,
        COLOR_ਬਫਰ_ਦੋਆਧਾਰੀ_ਅੰਕ                 = 0x00004000,
        FALSE_                           = 0,
        TRUE_                            = 1,
        POINTS                           = 0x0000,
        ਸਤਰਾਂ                            = 0x0001,
        ਸਤਰ_LOOP                        = 0x0002,
        ਸਤਰ_STRIP                       = 0x0003,
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
        FRONT_ਖੱਬਾ                       = 0x0400,
        FRONT_ਸੱਜਾ                      = 0x0401,
        ਪਿੱਠ_ਖੱਬਾ                        = 0x0402,
        ਪਿੱਠ_ਸੱਜਾ                       = 0x0403,
        FRONT                            = 0x0404,
        ਪਿੱਠ                             = 0x0405,
        ਖੱਬਾ                             = 0x0406,
        ਸੱਜਾ                            = 0x0407,
        FRONT_AND_ਪਿੱਠ                   = 0x0408,
        NO_ERROR_                        = 0,
        INVALID_ENUM                     = 0x0500,
        INVALID_ਮੁੱਲ                    = 0x0501,
        INVALID_OPERATION                = 0x0502,
        ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ                    = 0x0505,
        CW                               = 0x0900,
        CCW                              = 0x0901,
        ਬਿੰਦੂ_ਮਾਪ_2                       = 0x0B11,
        ਬਿੰਦੂ_ਮਾਪ_RANGE                 = 0x0B12,
        ਬਿੰਦੂ_ਮਾਪ_GRANULARITY           = 0x0B13,
        ਸਤਰ_SMOOTH                      = 0x0B20,
        ਸਤਰ_ਚੌੜਾਈ_2                       = 0x0B21,
        ਸਤਰ_ਚੌੜਾਈ_RANGE                 = 0x0B22,
        ਸਤਰ_ਚੌੜਾਈ_GRANULARITY           = 0x0B23,
        POLYGON_ਬਹੁਲਕ                     = 0x0B40,
        POLYGON_SMOOTH                   = 0x0B41,
        CULL_FACE                        = 0x0B44,
        CULL_FACE_ਬਹੁਲਕ                   = 0x0B45,
        FRONT_FACE                       = 0x0B46,
        ਡੂੰਘਾਈ_RANGE                      = 0x0B70,
        ਡੂੰਘਾਈ_TEST                       = 0x0B71,
        ਡੂੰਘਾਈ_WRITEMASK                  = 0x0B72,
        ਡੂੰਘਾਈ_CLEAR_ਮੁੱਲ                = 0x0B73,
        ਡੂੰਘਾਈ_FUNC                       = 0x0B74,
        STENCIL_TEST                     = 0x0B90,
        STENCIL_CLEAR_ਮੁੱਲ              = 0x0B91,
        STENCIL_FUNC                     = 0x0B92,
        STENCIL_ਮੁੱਲ_ਨਕਾਬ               = 0x0B93,
        STENCIL_FAIL                     = 0x0B94,
        STENCIL_PASS_ਡੂੰਘਾਈ_FAIL          = 0x0B95,
        STENCIL_PASS_ਡੂੰਘਾਈ_PASS          = 0x0B96,
        STENCIL_REF                      = 0x0B97,
        STENCIL_WRITEMASK                = 0x0B98,
        VIEWPORT                         = 0x0BA2,
        DITHER                           = 0x0BD0,
        BLEND_DST                        = 0x0BE0,
        BLEND_SRC                        = 0x0BE1,
        BLEND                            = 0x0BE2,
        LOGIC_OP_ਬਹੁਲਕ                    = 0x0BF0,
        COLOR_LOGIC_OP                   = 0x0BF2,
        DRAW_ਬਫਰ                      = 0x0C01,
        READ_ਬਫਰ                      = 0x0C02,
        SCISSOR_BOX                      = 0x0C10,
        SCISSOR_TEST                     = 0x0C11,
        COLOR_CLEAR_ਮੁੱਲ                = 0x0C22,
        COLOR_WRITEMASK                  = 0x0C23,
        DOUBLEBUFFER                     = 0x0C32,
        STEREO                           = 0x0C33,
        ਸਤਰ_SMOOTH_HINT                 = 0x0C52,
        POLYGON_SMOOTH_HINT              = 0x0C53,
        UNPACK_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ                = 0x0CF0,
        UNPACK_LSB_ਪਹਿਲਾ                 = 0x0CF1,
        UNPACK_ਪੰਕਤੀ_ਲੰਮਾਈ                = 0x0CF2,
        UNPACK_SKIP_ਪੰਕਤੀਆਂ                 = 0x0CF3,
        UNPACK_SKIP_PIXELS               = 0x0CF4,
        UNPACK_ALIGNMENT                 = 0x0CF5,
        PACK_ਅਦਲਾ_ਬਦਲੀ_ਬਾਈਟਾਂ                  = 0x0D00,
        PACK_LSB_ਪਹਿਲਾ                   = 0x0D01,
        PACK_ਪੰਕਤੀ_ਲੰਮਾਈ                  = 0x0D02,
        PACK_SKIP_ਪੰਕਤੀਆਂ                   = 0x0D03,
        PACK_SKIP_PIXELS                 = 0x0D04,
        PACK_ALIGNMENT                   = 0x0D05,
        MAX_TEXTURE_ਮਾਪ                 = 0x0D33,
        MAX_VIEWPORT_DIMS                = 0x0D3A,
        SUBPIXEL_ਬਿੱਟ                    = 0x0D50,
        TEXTURE_1D                       = 0x0DE0,
        TEXTURE_2D                       = 0x0DE1,
        POLYGON_ਵਿੱਥ_UNITS             = 0x2A00,
        POLYGON_ਵਿੱਥ_ਬਿੰਦੂ             = 0x2A01,
        POLYGON_ਵਿੱਥ_ਸਤਰ              = 0x2A02,
        POLYGON_ਵਿੱਥ_FILL              = 0x8037,
        POLYGON_ਵਿੱਥ_ਗੁਣਕ            = 0x8038,
        TEXTURE_BINDING_1D               = 0x8068,
        TEXTURE_BINDING_2D               = 0x8069,
        TEXTURE_ਚੌੜਾਈ                    = 0x1000,
        TEXTURE_ਉਚਾਈ                   = 0x1001,
        TEXTURE_ਅੰਦਰੂਨੀ_FORMAT          = 0x1003,
        TEXTURE_BORDER_COLOR             = 0x1004,
        TEXTURE_RED_ਮਾਪ                 = 0x805C,
        TEXTURE_GREEN_ਮਾਪ               = 0x805D,
        TEXTURE_BLUE_ਮਾਪ                = 0x805E,
        TEXTURE_ALPHA_ਮਾਪ               = 0x805F,
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
        AND_ਉਲਟ                      = 0x1502,
        COPY                             = 0x1503,
        AND_INVERTED                     = 0x1504,
        NOOP                             = 0x1505,
        XOR                              = 0x1506,
        OR                               = 0x1507,
        NOR                              = 0x1508,
        EQUIV                            = 0x1509,
        INVERT                           = 0x150A,
        OR_ਉਲਟ                       = 0x150B,
        COPY_INVERTED                    = 0x150C,
        OR_INVERTED                      = 0x150D,
        NAND                             = 0x150E,
        ਸੈੱਟ_2                              = 0x150F,
        TEXTURE                          = 0x1702,
        COLOR                            = 0x1800,
        ਡੂੰਘਾਈ_3                            = 0x1801,
        STENCIL                          = 0x1802,
        STENCIL_ਸੂਚਕਾਂਕ                    = 0x1901,
        ਡੂੰਘਾਈ_COMPONENT                  = 0x1902,
        RED                              = 0x1903,
        GREEN                            = 0x1904,
        BLUE                             = 0x1905,
        ALPHA                            = 0x1906,
        RGB                              = 0x1907,
        RGBA                             = 0x1908,
        ਬਿੰਦੂ_2                            = 0x1B00,
        ਸਤਰ_2                             = 0x1B01,
        FILL                             = 0x1B02,
        KEEP                             = 0x1E00,
        REPLACE                          = 0x1E01,
        INCR                             = 0x1E02,
        DECR                             = 0x1E03,
        VENDOR                           = 0x1F00,
        RENDERER                         = 0x1F01,
        ਸੰਸਕਰਣ_2                         = 0x1F02,
        EXTENSIONS                       = 0x1F03,
        ਸਭ_ਤੋਂ_ਨੇੜੇ                          = 0x2600,
        ਰੇਖਿਕ                           = 0x2601,
        ਸਭ_ਤੋਂ_ਨੇੜੇ_MIPMAP_ਸਭ_ਤੋਂ_ਨੇੜੇ           = 0x2700,
        ਰੇਖਿਕ_MIPMAP_ਸਭ_ਤੋਂ_ਨੇੜੇ            = 0x2701,
        ਸਭ_ਤੋਂ_ਨੇੜੇ_MIPMAP_ਰੇਖਿਕ            = 0x2702,
        ਰੇਖਿਕ_MIPMAP_ਰੇਖਿਕ             = 0x2703,
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
        FUNC_ਜੋੜੋ                         = 0x8006,
        MIN                              = 0x8007,
        MAX                              = 0x8008,
        BLEND_EQUATION                   = 0x8009,
        FUNC_ਘਟਾਓ                    = 0x800A,
        FUNC_ਉਲਟ_ਘਟਾਓ            = 0x800B,
        CONVOLUTION_1D                   = 0x8010,
        CONVOLUTION_2D                   = 0x8011,
        SEPARABLE_2D                     = 0x8012,
        CONVOLUTION_BORDER_ਬਹੁਲਕ          = 0x8013,
        CONVOLUTION_FILTER_SCALE         = 0x8014,
        CONVOLUTION_FILTER_BIAS          = 0x8015,
        REDUCE                           = 0x8016,
        CONVOLUTION_FORMAT               = 0x8017,
        CONVOLUTION_ਚੌੜਾਈ                = 0x8018,
        CONVOLUTION_ਉਚਾਈ               = 0x8019,
        MAX_CONVOLUTION_ਚੌੜਾਈ            = 0x801A,
        MAX_CONVOLUTION_ਉਚਾਈ           = 0x801B,
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
        HISTOGRAM_ਚੌੜਾਈ                  = 0x8026,
        HISTOGRAM_FORMAT                 = 0x8027,
        HISTOGRAM_RED_ਮਾਪ               = 0x8028,
        HISTOGRAM_GREEN_ਮਾਪ             = 0x8029,
        HISTOGRAM_BLUE_ਮਾਪ              = 0x802A,
        HISTOGRAM_ALPHA_ਮਾਪ             = 0x802B,
        HISTOGRAM_LUMINANCE_ਮਾਪ         = 0x802C,
        HISTOGRAM_SINK                   = 0x802D,
        MINMAX                           = 0x802E,
        MINMAX_FORMAT                    = 0x802F,
        MINMAX_SINK                      = 0x8030,
        ਸਾਰਣੀ_TOO_LARGE                  = 0x8031,
        COLOR_ਮੈਟ੍ਰਿਕਸ                     = 0x80B1,
        COLOR_ਮੈਟ੍ਰਿਕਸ_ਢੇਰ_ਡੂੰਘਾਈ         = 0x80B2,
        MAX_COLOR_ਮੈਟ੍ਰਿਕਸ_ਢੇਰ_ਡੂੰਘਾਈ     = 0x80B3,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_RED_SCALE      = 0x80B4,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_GREEN_SCALE    = 0x80B5,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_BLUE_SCALE     = 0x80B6,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_ALPHA_SCALE    = 0x80B7,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_RED_BIAS       = 0x80B8,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_GREEN_BIAS     = 0x80B9,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_BLUE_BIAS      = 0x80BA,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_ALPHA_BIAS     = 0x80BB,
        COLOR_ਸਾਰਣੀ                      = 0x80D0,
        POST_CONVOLUTION_COLOR_ਸਾਰਣੀ     = 0x80D1,
        POST_COLOR_ਮੈਟ੍ਰਿਕਸ_COLOR_ਸਾਰਣੀ    = 0x80D2,
        PROXY_COLOR_ਸਾਰਣੀ                = 0x80D3,
        PROXY_POST_CONVOLUTION_COLOR_ਸਾਰਣੀ = 0x80D4,
        PROXY_POST_COLOR_ਮੈਟ੍ਰਿਕਸ_COLOR_ਸਾਰਣੀ = 0x80D5,
        COLOR_ਸਾਰਣੀ_SCALE                = 0x80D6,
        COLOR_ਸਾਰਣੀ_BIAS                 = 0x80D7,
        COLOR_ਸਾਰਣੀ_FORMAT               = 0x80D8,
        COLOR_ਸਾਰਣੀ_ਚੌੜਾਈ                = 0x80D9,
        COLOR_ਸਾਰਣੀ_RED_ਮਾਪ             = 0x80DA,
        COLOR_ਸਾਰਣੀ_GREEN_ਮਾਪ           = 0x80DB,
        COLOR_ਸਾਰਣੀ_BLUE_ਮਾਪ            = 0x80DC,
        COLOR_ਸਾਰਣੀ_ALPHA_ਮਾਪ           = 0x80DD,
        COLOR_ਸਾਰਣੀ_LUMINANCE_ਮਾਪ       = 0x80DE,
        COLOR_ਸਾਰਣੀ_INTENSITY_ਮਾਪ       = 0x80DF,
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
        PACK_IMAGE_ਉਚਾਈ                = 0x806C,
        UNPACK_SKIP_IMAGES               = 0x806D,
        UNPACK_IMAGE_ਉਚਾਈ              = 0x806E,
        TEXTURE_3D                       = 0x806F,
        PROXY_TEXTURE_3D                 = 0x8070,
        TEXTURE_ਡੂੰਘਾਈ                    = 0x8071,
        TEXTURE_WRAP_R                   = 0x8072,
        MAX_3_D_TEXTURE_ਮਾਪ              = 0x8073,
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
        CLAMP_ਮੰਜ਼ਿਲ_ਕਿਨਾਰਾ                    = 0x812F,
        TEXTURE_MIN_LOD                  = 0x813A,
        TEXTURE_MAX_LOD                  = 0x813B,
        TEXTURE_ਅਧਾਰ_LEVEL               = 0x813C,
        TEXTURE_MAX_LEVEL                = 0x813D,
        SMOOTH_ਬਿੰਦੂ_ਮਾਪ_RANGE          = 0x0B12,
        SMOOTH_ਬਿੰਦੂ_ਮਾਪ_GRANULARITY    = 0x0B13,
        SMOOTH_ਸਤਰ_ਚੌੜਾਈ_RANGE          = 0x0B22,
        SMOOTH_ਸਤਰ_ਚੌੜਾਈ_GRANULARITY    = 0x0B23,
        ALIASED_ਸਤਰ_ਚੌੜਾਈ_RANGE         = 0x846E,

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
        ਸਰਗਰਮ_TEXTURE                   = 0x84E0,
        MULTISAMPLE                      = 0x809D,
        SAMPLE_ALPHA_ਮੰਜ਼ਿਲ_COVERAGE         = 0x809E,
        SAMPLE_ALPHA_ਮੰਜ਼ਿਲ_ONE              = 0x809F,
        SAMPLE_COVERAGE                  = 0x80A0,
        SAMPLE_BUFFERS                   = 0x80A8,
        SAMPLES                          = 0x80A9,
        SAMPLE_COVERAGE_ਮੁੱਲ            = 0x80AA,
        SAMPLE_COVERAGE_INVERT           = 0x80AB,
        TEXTURE_CUBE_MAP                 = 0x8513,
        TEXTURE_BINDING_CUBE_MAP         = 0x8514,
        TEXTURE_CUBE_MAP_ਸਕਾਰਾਤਮਕ_X      = 0x8515,
        TEXTURE_CUBE_MAP_NEGATIVE_X      = 0x8516,
        TEXTURE_CUBE_MAP_ਸਕਾਰਾਤਮਕ_Y      = 0x8517,
        TEXTURE_CUBE_MAP_NEGATIVE_Y      = 0x8518,
        TEXTURE_CUBE_MAP_ਸਕਾਰਾਤਮਕ_Z      = 0x8519,
        TEXTURE_CUBE_MAP_NEGATIVE_Z      = 0x851A,
        PROXY_TEXTURE_CUBE_MAP           = 0x851B,
        MAX_CUBE_MAP_TEXTURE_ਮਾਪ        = 0x851C,
        COMPRESSED_RGB                   = 0x84ED,
        COMPRESSED_RGBA                  = 0x84EE,
        TEXTURE_COMPRESSION_HINT         = 0x84EF,
        TEXTURE_COMPRESSED_IMAGE_ਮਾਪ    = 0x86A0,
        TEXTURE_COMPRESSED               = 0x86A1,
        ਅੰਕ_COMPRESSED_TEXTURE_FORMATS   = 0x86A2,
        COMPRESSED_TEXTURE_FORMATS       = 0x86A3,
        CLAMP_ਮੰਜ਼ਿਲ_BORDER                  = 0x812D,

        // Version: 1.4
        BLEND_DST_RGB                    = 0x80C8,
        BLEND_SRC_RGB                    = 0x80C9,
        BLEND_DST_ALPHA                  = 0x80CA,
        BLEND_SRC_ALPHA                  = 0x80CB,
        ਬਿੰਦੂ_FADE_ਹੱਦ_ਮਾਪ        = 0x8128,
        ਡੂੰਘਾਈ_COMPONENT16                = 0x81A5,
        ਡੂੰਘਾਈ_COMPONENT24                = 0x81A6,
        ਡੂੰਘਾਈ_COMPONENT32                = 0x81A7,
        MIRRORED_REPEAT                  = 0x8370,
        MAX_TEXTURE_LOD_BIAS             = 0x84FD,
        TEXTURE_LOD_BIAS                 = 0x8501,
        INCR_WRAP                        = 0x8507,
        DECR_WRAP                        = 0x8508,
        TEXTURE_ਡੂੰਘਾਈ_ਮਾਪ               = 0x884A,
        TEXTURE_COMPARE_ਬਹੁਲਕ             = 0x884C,
        TEXTURE_COMPARE_FUNC             = 0x884D,

        // Version: 1.5
        ਬਫਰ_ਮਾਪ_2                      = 0x8764,
        ਬਫਰ_USAGE                     = 0x8765,
        QUERY_COUNTER_ਬਿੱਟ               = 0x8864,
        CURRENT_QUERY                    = 0x8865,
        QUERY_ਨਤੀਜਾ                     = 0x8866,
        QUERY_ਨਤੀਜਾ_ਉਪਲਬਧ           = 0x8867,
        ARRAY_ਬਫਰ                     = 0x8892,
        ELEMENT_ARRAY_ਬਫਰ             = 0x8893,
        ARRAY_ਬਫਰ_BINDING             = 0x8894,
        ELEMENT_ARRAY_ਬਫਰ_BINDING     = 0x8895,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_ਬਫਰ_BINDING = 0x889F,
        READ_ONLY                        = 0x88B8,
        WRITE_ONLY                       = 0x88B9,
        READ_WRITE                       = 0x88BA,
        ਬਫਰ_ACCESS                    = 0x88BB,
        ਬਫਰ_MAPPED                    = 0x88BC,
        ਬਫਰ_MAP_ਸੂਚਕ               = 0x88BD,
        ਧਾਰਾ_DRAW                      = 0x88E0,
        ਧਾਰਾ_READ                      = 0x88E1,
        ਧਾਰਾ_COPY                      = 0x88E2,
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
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_ENABLED      = 0x8622,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_ਮਾਪ         = 0x8623,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_STRIDE       = 0x8624,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_TYPE         = 0x8625,
        CURRENT_ਸ਼ੀਰਸ਼_ATTRIB            = 0x8626,
        ਸ਼ੀਰਸ਼_ਪ੍ਰੋਗਰਾਮ_ਬਿੰਦੂ_ਮਾਪ        = 0x8642,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_ਸੂਚਕ      = 0x8645,
        STENCIL_ਪਿੱਠ_FUNC                = 0x8800,
        STENCIL_ਪਿੱਠ_FAIL                = 0x8801,
        STENCIL_ਪਿੱਠ_PASS_ਡੂੰਘਾਈ_FAIL     = 0x8802,
        STENCIL_ਪਿੱਠ_PASS_ਡੂੰਘਾਈ_PASS     = 0x8803,
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
        MAX_ਸ਼ੀਰਸ਼_ATTRIBS               = 0x8869,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_NORMALIZED   = 0x886A,
        MAX_TEXTURE_IMAGE_UNITS          = 0x8872,
        FRAGMENT_SHADER                  = 0x8B30,
        ਸ਼ੀਰਸ਼_SHADER                    = 0x8B31,
        MAX_FRAGMENT_ਇਕਸਾਰ_COMPONENTS  = 0x8B49,
        MAX_ਸ਼ੀਰਸ਼_ਇਕਸਾਰ_COMPONENTS    = 0x8B4A,
        MAX_VARYING_FLOATS               = 0x8B4B,
        MAX_ਸ਼ੀਰਸ਼_TEXTURE_IMAGE_UNITS   = 0x8B4C,
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
        DELETE_ਸਥਿਤੀ                    = 0x8B80,
        COMPILE_ਸਥਿਤੀ                   = 0x8B81,
        LINK_ਸਥਿਤੀ                      = 0x8B82,
        VALIDATE_ਸਥਿਤੀ                  = 0x8B83,
        INFO_LOG_ਲੰਮਾਈ                  = 0x8B84,
        ATTACHED_SHADERS                 = 0x8B85,
        ਸਰਗਰਮ_UNIFORMS                  = 0x8B86,
        ਸਰਗਰਮ_ਇਕਸਾਰ_MAX_ਲੰਮਾਈ        = 0x8B87,
        SHADER_ਸਰੋਤ_ਲੰਮਾਈ             = 0x8B88,
        ਸਰਗਰਮ_ਗੁਣ                = 0x8B89,
        ਸਰਗਰਮ_ATTRIBUTE_MAX_ਲੰਮਾਈ      = 0x8B8A,
        FRAGMENT_SHADER_DERIVATIVE_HINT  = 0x8B8B,
        SHADING_LANGUAGE_ਸੰਸਕਰਣ         = 0x8B8C,
        CURRENT_ਪ੍ਰੋਗਰਾਮ                  = 0x8B8D,
        ਬਿੰਦੂ_SPRITE_COORD_ORIGIN        = 0x8CA0,
        ਹੇਠਲਾ_ਖੱਬਾ                       = 0x8CA1,
        ਉੱਤਲਾ_ਖੱਬਾ                       = 0x8CA2,
        STENCIL_ਪਿੱਠ_REF                 = 0x8CA3,
        STENCIL_ਪਿੱਠ_ਮੁੱਲ_ਨਕਾਬ          = 0x8CA4,
        STENCIL_ਪਿੱਠ_WRITEMASK           = 0x8CA5,

        // Version: 2.1
        PIXEL_PACK_ਬਫਰ                = 0x88EB,
        PIXEL_UNPACK_ਬਫਰ              = 0x88EC,
        PIXEL_PACK_ਬਫਰ_BINDING        = 0x88ED,
        PIXEL_UNPACK_ਬਫਰ_BINDING      = 0x88EF,
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
        ਸ਼ੀਰਸ਼_ARRAY_BINDING             = 0x85B5,

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
        MAP_READ_ਦੋਆਧਾਰੀ_ਅੰਕ                     = 0x0001,
        MAP_WRITE_ਦੋਆਧਾਰੀ_ਅੰਕ                    = 0x0002,
        MAP_INVALIDATE_RANGE_ਦੋਆਧਾਰੀ_ਅੰਕ         = 0x0004,
        MAP_INVALIDATE_ਬਫਰ_ਦੋਆਧਾਰੀ_ਅੰਕ        = 0x0008,
        MAP_FLUSH_EXPLICIT_ਦੋਆਧਾਰੀ_ਅੰਕ           = 0x0010,
        MAP_UNSYNCHRONIZED_ਦੋਆਧਾਰੀ_ਅੰਕ           = 0x0020,

        // Core Extension: ARB_half_float_vertex
        HALF_FLOAT                       = 0x140B,

        // Core Extension: ARB_framebuffer_sRGB
        FRAMEBUFFER_SRGB                 = 0x8DB9,

        // Core Extension: ARB_framebuffer_object
        INVALID_FRAMEBUFFER_OPERATION    = 0x0506,
        FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = 0x8210,
        FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = 0x8211,
        FRAMEBUFFER_ATTACHMENT_RED_ਮਾਪ  = 0x8212,
        FRAMEBUFFER_ATTACHMENT_GREEN_ਮਾਪ = 0x8213,
        FRAMEBUFFER_ATTACHMENT_BLUE_ਮਾਪ = 0x8214,
        FRAMEBUFFER_ATTACHMENT_ALPHA_ਮਾਪ = 0x8215,
        FRAMEBUFFER_ATTACHMENT_ਡੂੰਘਾਈ_ਮਾਪ = 0x8216,
        FRAMEBUFFER_ATTACHMENT_STENCIL_ਮਾਪ = 0x8217,
        FRAMEBUFFER_DEFAULT              = 0x8218,
        FRAMEBUFFER_UNDEFINED            = 0x8219,
        ਡੂੰਘਾਈ_STENCIL_ATTACHMENT         = 0x821A,
        ਸੂਚਕਾਂਕ_2                            = 0x8222,
        MAX_RENDERBUFFER_ਮਾਪ            = 0x84E8,
        ਡੂੰਘਾਈ_STENCIL                    = 0x84F9,
        UNSIGNED_INT_24_8                = 0x84FA,
        DEPTH24_STENCIL8                 = 0x88F0,
        TEXTURE_STENCIL_ਮਾਪ             = 0x88F1,
        TEXTURE_RED_TYPE                 = 0x8C10,
        TEXTURE_GREEN_TYPE               = 0x8C11,
        TEXTURE_BLUE_TYPE                = 0x8C12,
        TEXTURE_ALPHA_TYPE               = 0x8C13,
        TEXTURE_ਡੂੰਘਾਈ_TYPE               = 0x8C16,
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
        FRAMEBUFFER_INCOMPLETE_DRAW_ਬਫਰ = 0x8CDB,
        FRAMEBUFFER_INCOMPLETE_READ_ਬਫਰ = 0x8CDC,
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
        ਡੂੰਘਾਈ_ATTACHMENT                 = 0x8D00,
        STENCIL_ATTACHMENT               = 0x8D20,
        FRAMEBUFFER                      = 0x8D40,
        RENDERBUFFER                     = 0x8D41,
        RENDERBUFFER_ਚੌੜਾਈ               = 0x8D42,
        RENDERBUFFER_ਉਚਾਈ              = 0x8D43,
        RENDERBUFFER_ਅੰਦਰੂਨੀ_FORMAT     = 0x8D44,
        STENCIL_INDEX1                   = 0x8D46,
        STENCIL_INDEX4                   = 0x8D47,
        STENCIL_INDEX8                   = 0x8D48,
        STENCIL_INDEX16                  = 0x8D49,
        RENDERBUFFER_RED_ਮਾਪ            = 0x8D50,
        RENDERBUFFER_GREEN_ਮਾਪ          = 0x8D51,
        RENDERBUFFER_BLUE_ਮਾਪ           = 0x8D52,
        RENDERBUFFER_ALPHA_ਮਾਪ          = 0x8D53,
        RENDERBUFFER_ਡੂੰਘਾਈ_ਮਾਪ          = 0x8D54,
        RENDERBUFFER_STENCIL_ਮਾਪ        = 0x8D55,
        FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = 0x8D56,
        MAX_SAMPLES                      = 0x8D57,
        TEXTURE_LUMINANCE_TYPE           = 0x8C14,
        TEXTURE_INTENSITY_TYPE           = 0x8C15,

        // Core Extension: ARB_depth_buffer_float
        ਡੂੰਘਾਈ_COMPONENT32_F               = 0x8CAC,
        DEPTH32F_STENCIL8                = 0x8CAD,
        FLOAT_32_UNSIGNED_INT_24_8_REV   = 0x8DAD,

        // Version: 3.0
        COMPARE_REF_ਮੰਜ਼ਿਲ_TEXTURE           = 0x884E,
        CLIP_DISTANCE0                   = 0x3000,
        CLIP_DISTANCE1                   = 0x3001,
        CLIP_DISTANCE2                   = 0x3002,
        CLIP_DISTANCE3                   = 0x3003,
        CLIP_DISTANCE4                   = 0x3004,
        CLIP_DISTANCE5                   = 0x3005,
        CLIP_DISTANCE6                   = 0x3006,
        CLIP_DISTANCE7                   = 0x3007,
        MAX_CLIP_DISTANCES               = 0x0D32,
        MAJOR_ਸੰਸਕਰਣ                    = 0x821B,
        MINOR_ਸੰਸਕਰਣ                    = 0x821C,
        ਅੰਕ_EXTENSIONS                   = 0x821D,
        CONTEXT_FLAGS                    = 0x821E,
        COMPRESSED_RED                   = 0x8225,
        COMPRESSED_RG                    = 0x8226,
        CONTEXT_FLAG_FORWARD_COMPATIBLE_ਦੋਆਧਾਰੀ_ਅੰਕ = 0x0001,
        RGBA32F                          = 0x8814,
        RGB32F                           = 0x8815,
        RGBA16F                          = 0x881A,
        RGB16F                           = 0x881B,
        ਸ਼ੀਰਸ਼_ATTRIB_ARRAY_INTEGER      = 0x88FD,
        MAX_ARRAY_TEXTURE_LAYERS         = 0x88FF,
        MIN_ਪ੍ਰੋਗਰਾਮ_TEXEL_ਵਿੱਥ         = 0x8904,
        MAX_ਪ੍ਰੋਗਰਾਮ_TEXEL_ਵਿੱਥ         = 0x8905,
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
        TEXTURE_SHARED_ਮਾਪ              = 0x8C3F,
        TRANSFORM_FEEDBACK_VARYING_MAX_ਲੰਮਾਈ = 0x8C76,
        TRANSFORM_FEEDBACK_ਬਫਰ_ਬਹੁਲਕ   = 0x8C7F,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = 0x8C80,
        TRANSFORM_FEEDBACK_VARYINGS      = 0x8C83,
        TRANSFORM_FEEDBACK_ਬਫਰ_ਸ਼ੁਰੂ  = 0x8C84,
        TRANSFORM_FEEDBACK_ਬਫਰ_ਮਾਪ   = 0x8C85,
        PRIMITIVES_GENERATED             = 0x8C87,
        TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88,
        RASTERIZER_DISCARD               = 0x8C89,
        MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A,
        MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = 0x8C8B,
        INTERLEAVED_ATTRIBS              = 0x8C8C,
        SEPARATE_ATTRIBS                 = 0x8C8D,
        TRANSFORM_FEEDBACK_ਬਫਰ        = 0x8C8E,
        TRANSFORM_FEEDBACK_ਬਫਰ_BINDING = 0x8C8F,
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
        ਬਫਰ_ACCESS_FLAGS              = 0x911F,
        ਬਫਰ_MAP_ਲੰਮਾਈ                = 0x9120,
        ਬਫਰ_MAP_ਵਿੱਥ                = 0x9121,

        // Core Extension: ARB_uniform_buffer_object
        ਇਕਸਾਰ_ਬਫਰ                   = 0x8A11,
        ਇਕਸਾਰ_ਬਫਰ_BINDING           = 0x8A28,
        ਇਕਸਾਰ_ਬਫਰ_ਸ਼ੁਰੂ             = 0x8A29,
        ਇਕਸਾਰ_ਬਫਰ_ਮਾਪ              = 0x8A2A,
        MAX_ਸ਼ੀਰਸ਼_ਇਕਸਾਰ_ਖੰਡਾਂ        = 0x8A2B,
        MAX_FRAGMENT_ਇਕਸਾਰ_ਖੰਡਾਂ      = 0x8A2D,
        MAX_COMBINED_ਇਕਸਾਰ_ਖੰਡਾਂ      = 0x8A2E,
        MAX_ਇਕਸਾਰ_ਬਫਰ_BINDINGS      = 0x8A2F,
        MAX_ਇਕਸਾਰ_ਖੰਡ_ਮਾਪ           = 0x8A30,
        MAX_COMBINED_ਸ਼ੀਰਸ਼_ਇਕਸਾਰ_COMPONENTS = 0x8A31,
        MAX_COMBINED_FRAGMENT_ਇਕਸਾਰ_COMPONENTS = 0x8A33,
        ਇਕਸਾਰ_ਬਫਰ_ਵਿੱਥ_ALIGNMENT  = 0x8A34,
        ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_MAX_NAME_ਲੰਮਾਈ = 0x8A35,
        ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡਾਂ            = 0x8A36,
        ਇਕਸਾਰ_TYPE                     = 0x8A37,
        ਇਕਸਾਰ_ਮਾਪ                     = 0x8A38,
        ਇਕਸਾਰ_NAME_ਲੰਮਾਈ              = 0x8A39,
        ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ_2              = 0x8A3A,
        ਇਕਸਾਰ_ਵਿੱਥ                   = 0x8A3B,
        ਇਕਸਾਰ_ARRAY_STRIDE             = 0x8A3C,
        ਇਕਸਾਰ_ਮੈਟ੍ਰਿਕਸ_STRIDE            = 0x8A3D,
        ਇਕਸਾਰ_IS_ਪੰਕਤੀ_MAJOR             = 0x8A3E,
        ਇਕਸਾਰ_ਖੰਡ_BINDING            = 0x8A3F,
        ਇਕਸਾਰ_ਖੰਡ_ਅੰਕੜੇ_ਮਾਪ          = 0x8A40,
        ਇਕਸਾਰ_ਖੰਡ_NAME_ਲੰਮਾਈ        = 0x8A41,
        ਇਕਸਾਰ_ਖੰਡ_ਸਰਗਰਮ_UNIFORMS    = 0x8A42,
        ਇਕਸਾਰ_ਖੰਡ_ਸਰਗਰਮ_ਇਕਸਾਰ_INDICES = 0x8A43,
        ਇਕਸਾਰ_ਖੰਡ_REFERENCED_BY_ਸ਼ੀਰਸ਼_SHADER = 0x8A44,
        ਇਕਸਾਰ_ਖੰਡ_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46,
        INVALID_ਸੂਚਕਾਂਕ                    = 0xFFFFFFFF,
        MAX_GEOMETRY_ਇਕਸਾਰ_ਖੰਡਾਂ      = 0x8A2C,
        MAX_COMBINED_GEOMETRY_ਇਕਸਾਰ_COMPONENTS = 0x8A32,
        ਇਕਸਾਰ_ਖੰਡ_REFERENCED_BY_GEOMETRY_SHADER = 0x8A45,

        // Core Extension: ARB_copy_buffer
        COPY_READ_ਬਫਰ                 = 0x8F36,
        COPY_WRITE_ਬਫਰ                = 0x8F37,
        COPY_READ_ਬਫਰ_BINDING         = 0x8F36,
        COPY_WRITE_ਬਫਰ_BINDING        = 0x8F37,

        // Version: 3.1
        SAMPLER_2D_RECT                  = 0x8B63,
        SAMPLER_2D_RECT_SHADOW           = 0x8B64,
        SAMPLER_ਬਫਰ                   = 0x8DC2,
        INT_SAMPLER_2D_RECT              = 0x8DCD,
        INT_SAMPLER_ਬਫਰ               = 0x8DD0,
        UNSIGNED_INT_SAMPLER_2D_RECT     = 0x8DD5,
        UNSIGNED_INT_SAMPLER_ਬਫਰ      = 0x8DD8,
        TEXTURE_ਬਫਰ                   = 0x8C2A,
        MAX_TEXTURE_ਬਫਰ_ਮਾਪ          = 0x8C2B,
        TEXTURE_BINDING_ਬਫਰ           = 0x8C2C,
        TEXTURE_ਬਫਰ_ਅੰਕੜੇ_STORE_BINDING = 0x8C2D,
        TEXTURE_RECTANGLE                = 0x84F5,
        TEXTURE_BINDING_RECTANGLE        = 0x84F6,
        PROXY_TEXTURE_RECTANGLE          = 0x84F7,
        MAX_RECTANGLE_TEXTURE_ਮਾਪ       = 0x84F8,
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
        PRIMITIVE_RESTART_ਸੂਚਕਾਂਕ          = 0x8F9E,

        // Legacy
        ਸ਼ੀਰਸ਼_ARRAY = 0x8074,
        ਸਾਧਾਰਨ_ARRAY = 0x8075,
        COLOR_ARRAY = 0x8076,
        TEXTURE_COORD_ARRAY = 0x8078,
        TEXTURE_ENV = 0x2300,
        TEXTURE_ENV_ਬਹੁਲਕ = 0x2200,
        MODELVIEW = 0x1700,
        PROJECTION = 0x1701,
        LIGHTING = 0x0B50
    };

    // Extension: 1.1
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CullFace)(GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *FrontFace)(GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Hint)(GLenum target, GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸਤਰ_ਚੌੜਾਈ)(GLfloat ਚੌੜਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਿੰਦੂ_ਮਾਪ)(GLfloat size);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Polygon_ਬਹੁਲਕ)(GLenum face, GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Scissor)(GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexParameterf)(GLenum target, GLenum pname, GLfloat param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexParameterfv)(GLenum target, GLenum pname, ਅਚਲ GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexParameteri)(GLenum target, GLenum pname, GLint param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexParameteriv)(GLenum target, GLenum pname, ਅਚਲ GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei ਚੌੜਾਈ, GLint border, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLint border, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Draw_ਬਫਰ)(GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Clear)(GLbitfield ਨਕਾਬ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClearStencil)(GLint s);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Clear_ਡੂੰਘਾਈ)(GLdouble ਡੂੰਘਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Stencil_ਨਕਾਬ)(GLuint ਨਕਾਬ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Color_ਨਕਾਬ)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਡੂੰਘਾਈ_ਨਕਾਬ)(GLboolean flag);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Disable)(GLenum cap);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Enable)(GLenum cap);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਅੰਤ)();
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Flush)();
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BlendFunc)(GLenum sfactor, GLenum dfactor);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *LogicOp)(GLenum opcode);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *StencilFunc)(GLenum func, GLint ref, GLuint ਨਕਾਬ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *StencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਡੂੰਘਾਈ_Func)(GLenum func);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *PixelStoref)(GLenum pname, GLfloat param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *PixelStorei)(GLenum pname, GLint param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Read_ਬਫਰ)(GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ReadPixels)(GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetBooleanv)(GLenum pname, GLboolean *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetDoublev)(GLenum pname, GLdouble *params);
    ਬਾਹਰੀ GLenum (CODEGEN_FUNCPTR *GetError)();
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetFloatv)(GLenum pname, GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetIntegerv)(GLenum pname, GLint *params);
    ਬਾਹਰੀ ਅਚਲ GLubyte * (CODEGEN_FUNCPTR *GetString)(GLenum name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsEnabled)(GLenum cap);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਡੂੰਘਾਈ_Range)(GLdouble ren_near, GLdouble ren_far);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Viewport)(GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DrawArrays)(GLenum ਬਹੁਲਕ, GLint first, GLsizei ਗਿਣਤੀ_3);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DrawElements)(GLenum ਬਹੁਲਕ, GLsizei ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid *indices);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetPointerv)(GLenum pname, GLvoid* *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Polygon_ਵਿੱਥ)(GLfloat ਗੁਣਕ, GLfloat units);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLint border);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLint border);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei ਚੌੜਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei ਚੌੜਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BindTexture)(GLenum target, GLuint texture);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DeleteTextures)(GLsizei n, ਅਚਲ GLuint *textures);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GenTextures)(GLsizei n, GLuint *textures);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsTexture)(GLuint texture);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Indexub)(GLubyte c);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Indexubv)(ਅਚਲ GLubyte *c);

    // Extension: 1.2
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BlendEquation)(GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DrawRangeElements)(GLenum ਬਹੁਲਕ, GLuint ਸ਼ੁਰੂ, GLuint end, GLsizei ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid *indices);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLsizei ਡੂੰਘਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ);

    // Extension: 1.3
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸਰਗਰਮ_Texture)(GLenum texture);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *SampleCoverage)(GLfloat value, GLboolean invert);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLsizei ਡੂੰਘਾਈ, GLint border, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLint border, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLint border, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLsizei ਡੂੰਘਾਈ, GLenum format, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei ਚੌੜਾਈ, GLenum format, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);

    // Extension: 1.4
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *MultiDrawArrays)(GLenum ਬਹੁਲਕ, ਅਚਲ GLint *first, ਅਚਲ GLsizei *ਗਿਣਤੀ_3, GLsizei drawcount);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *MultiDrawElements)(GLenum ਬਹੁਲਕ, ਅਚਲ GLsizei *ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid* ਅਚਲ *indices, GLsizei drawcount);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਿੰਦੂ_Parameterf)(GLenum pname, GLfloat param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਿੰਦੂ_Parameterfv)(GLenum pname, ਅਚਲ GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਿੰਦੂ_Parameteri)(GLenum pname, GLint param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਿੰਦੂ_Parameteriv)(GLenum pname, ਅਚਲ GLint *params);

    // Extension: 1.5
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GenQueries)(GLsizei n, GLuint *ids);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DeleteQueries)(GLsizei n, ਅਚਲ GLuint *ids);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsQuery)(GLuint id);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BeginQuery)(GLenum target, GLuint id);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸਿਰਾ_Query)(GLenum target);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetQueryiv)(GLenum target, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Bind_ਬਫਰ)(GLenum target, GLuint ਬਫਰ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DeleteBuffers)(GLsizei n, ਅਚਲ GLuint *buffers);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GenBuffers)(GLsizei n, GLuint *buffers);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *Is_ਬਫਰ)(GLuint ਬਫਰ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਫਰ_ਅੰਕੜੇ)(GLenum target, GLsizeiptr size, ਅਚਲ GLvoid *ਅੰਕੜੇ, GLenum usage);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਬਫਰ_Sub_ਅੰਕੜੇ)(GLenum target, GLintptr offset, GLsizeiptr size, ਅਚਲ GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਬਫਰ_Sub_ਅੰਕੜੇ)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *ਅੰਕੜੇ);
    ਬਾਹਰੀ GLvoid* (CODEGEN_FUNCPTR *Map_ਬਫਰ)(GLenum target, GLenum access);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *Unmap_ਬਫਰ)(GLenum target);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਬਫਰ_Parameteriv)(GLenum target, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਬਫਰ_Pointerv)(GLenum target, GLenum pname, GLvoid* *params);

    // Extension: 2.0
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BlendEquationSeparate)(GLenum ਬਹੁਲਕ_RGB, GLenum ਬਹੁਲਕ_Alpha);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DrawBuffers)(GLsizei n, ਅਚਲ GLenum *bufs);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *StencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *StencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint ਨਕਾਬ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Stencil_ਨਕਾਬ_Separate)(GLenum face, GLuint ਨਕਾਬ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *AttachShader)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint shader);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BindAttribLocation)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *CompileShader)(GLuint shader);
    ਬਾਹਰੀ GLuint (CODEGEN_FUNCPTR *Create_ਪ੍ਰੋਗਰਾਮ)();
    ਬਾਹਰੀ GLuint (CODEGEN_FUNCPTR *CreateShader)(GLenum type);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Delete_ਪ੍ਰੋਗਰਾਮ)(GLuint ਪ੍ਰੋਗਰਾਮ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DeleteShader)(GLuint shader);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DetachShader)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint shader);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Disable_ਸ਼ੀਰਸ਼_Attrib_Array)(GLuint ਸੂਚਕਾਂਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Enable_ਸ਼ੀਰਸ਼_Attrib_Array)(GLuint ਸੂਚਕਾਂਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸਰਗਰਮ_Attrib)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸਰਗਰਮ_ਇਕਸਾਰ)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetAttachedShaders)(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei max_ਗਿਣਤੀ, GLsizei *ਗਿਣਤੀ_3, GLuint *obj);
    ਬਾਹਰੀ GLint (CODEGEN_FUNCPTR *GetAttribLocation)(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetProgramiv)(GLuint ਪ੍ਰੋਗਰਾਮ, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਪ੍ਰੋਗਰਾਮ_Info_Log)(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *infoLog);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetShaderiv)(GLuint shader, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetShaderInfoLog)(GLuint shader, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *infoLog);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_Shader_ਸਰੋਤ)(GLuint shader, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *ਸਰੋਤ);
    ਬਾਹਰੀ GLint (CODEGEN_FUNCPTR *Get_ਇਕਸਾਰ_Location)(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetUniformfv)(GLuint ਪ੍ਰੋਗਰਾਮ, GLint location, GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetUniformiv)(GLuint ਪ੍ਰੋਗਰਾਮ, GLint location, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸ਼ੀਰਸ਼_Attribdv)(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLdouble *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸ਼ੀਰਸ਼_Attribfv)(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLfloat *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸ਼ੀਰਸ਼_Attribiv)(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv)(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLvoid* *pointer);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *Is_ਪ੍ਰੋਗਰਾਮ)(GLuint ਪ੍ਰੋਗਰਾਮ);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsShader)(GLuint shader);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Link_ਪ੍ਰੋਗਰਾਮ)(GLuint ਪ੍ਰੋਗਰਾਮ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Shader_ਸਰੋਤ)(GLuint shader, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLchar* ਅਚਲ *string, ਅਚਲ GLint *length);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Use_ਪ੍ਰੋਗਰਾਮ)(GLuint ਪ੍ਰੋਗਰਾਮ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform1f)(GLint location, GLfloat v0);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform2f)(GLint location, GLfloat v0, GLfloat v1);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform1i)(GLint location, GLint v0);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform2i)(GLint location, GLint v0, GLint v1);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform1fv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform2fv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform3fv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform4fv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform1iv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform2iv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform3iv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform4iv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix2fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix3fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix4fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Validate_ਪ੍ਰੋਗਰਾਮ)(GLuint ਪ੍ਰੋਗਰਾਮ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ)(GLuint ਸੂਚਕਾਂਕ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ਅਚਲ GLvoid *pointer);

    // Extension: 2.1
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix2x3fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix3x2fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix2x4fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix4x2fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix3x4fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_Matrix4x3fv)(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value);

    // Extension: ARB_vertex_array_object
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Bind_ਸ਼ੀਰਸ਼_Array)(GLuint ren_array);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Delete_ਸ਼ੀਰਸ਼_Arrays)(GLsizei n, ਅਚਲ GLuint *arrays);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Gen_ਸ਼ੀਰਸ਼_Arrays)(GLsizei n, GLuint *arrays);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *Is_ਸ਼ੀਰਸ਼_Array)(GLuint ren_array);

    // Extension: ARB_map_buffer_range
    ਬਾਹਰੀ GLvoid* (CODEGEN_FUNCPTR *Map_ਬਫਰ_Range)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Flush_Mapped_ਬਫਰ_Range)(GLenum target, GLintptr offset, GLsizeiptr length);

    // Extension: ARB_framebuffer_object
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsRenderbuffer)(GLuint renderbuffer);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BindRenderbuffer)(GLenum target, GLuint renderbuffer);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DeleteRenderbuffers)(GLsizei n, ਅਚਲ GLuint *renderbuffers);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *RenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsFramebuffer)(GLuint framebuffer);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BindFramebuffer)(GLenum target, GLuint framebuffer);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DeleteFramebuffers)(GLsizei n, ਅਚਲ GLuint *framebuffers);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GenFramebuffers)(GLsizei n, GLuint *framebuffers);
    ਬਾਹਰੀ GLenum (CODEGEN_FUNCPTR *Check_Framebuffer_ਸਥਿਤੀ)(GLenum target);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *FramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *FramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *FramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *FramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GenerateMipmap)(GLenum target);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ਨਕਾਬ, GLenum filter);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *RenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *FramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

    // Extension: 3.0
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ColorMaski)(GLuint ਸੂਚਕਾਂਕ, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetBooleani_v)(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLboolean *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetIntegeri_v)(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLint *ਅੰਕੜੇ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Enablei)(GLenum target, GLuint ਸੂਚਕਾਂਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Disablei)(GLenum target, GLuint ਸੂਚਕਾਂਕ);
    ਬਾਹਰੀ GLboolean (CODEGEN_FUNCPTR *IsEnabledi)(GLenum target, GLuint ਸੂਚਕਾਂਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BeginTransformFeedback)(GLenum primitive_ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸਿਰਾ_Transform_Feedback)();
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Bind_ਬਫਰ_Range)(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLuint ਬਫਰ, GLintptr offset, GLsizeiptr size);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Bind_ਬਫਰ_ਅਧਾਰ)(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLuint ਬਫਰ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TransformFeedbackVaryings)(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLchar* ਅਚਲ *varyings, GLenum ਬਫਰ_ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTransformFeedbackVarying)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClampColor)(GLenum target, GLenum clamp);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *BeginConditionalRender)(GLuint id, GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸਿਰਾ_Conditional_Render)();
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_IPointer)(GLuint ਸੂਚਕਾਂਕ, GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *pointer);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸ਼ੀਰਸ਼_Attrib_Iiv)(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv)(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLuint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I1i)(GLuint ਸੂਚਕਾਂਕ, GLint x);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I2i)(GLuint ਸੂਚਕਾਂਕ, GLint x, GLint y);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I3i)(GLuint ਸੂਚਕਾਂਕ, GLint x, GLint y, GLint z);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4i)(GLuint ਸੂਚਕਾਂਕ, GLint x, GLint y, GLint z, GLint w);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I1ui)(GLuint ਸੂਚਕਾਂਕ, GLuint x);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I2ui)(GLuint ਸੂਚਕਾਂਕ, GLuint x, GLuint y);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I3ui)(GLuint ਸੂਚਕਾਂਕ, GLuint x, GLuint y, GLuint z);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4ui)(GLuint ਸੂਚਕਾਂਕ, GLuint x, GLuint y, GLuint z, GLuint w);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I1iv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I2iv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I3iv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4iv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I1uiv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I2uiv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I3uiv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4uiv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4bv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLbyte *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4sv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLshort *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4ubv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLubyte *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_Attrib_I4usv)(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLushort *v);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetUniformuiv)(GLuint ਪ੍ਰੋਗਰਾਮ, GLint location, GLuint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Bind_Frag_ਅੰਕੜੇ_Location)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint color, ਅਚਲ GLchar *name);
    ਬਾਹਰੀ GLint (CODEGEN_FUNCPTR *Get_Frag_ਅੰਕੜੇ_Location)(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform1ui)(GLint location, GLuint v0);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform2ui)(GLint location, GLuint v0, GLuint v1);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform1uiv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform2uiv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform3uiv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Uniform4uiv)(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexParameterIiv)(GLenum target, GLenum pname, ਅਚਲ GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexParameterIuiv)(GLenum target, GLenum pname, ਅਚਲ GLuint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *GetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClearBufferiv)(GLenum ਬਫਰ, GLint drawbuffer, ਅਚਲ GLint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClearBufferuiv)(GLenum ਬਫਰ, GLint drawbuffer, ਅਚਲ GLuint *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClearBufferfv)(GLenum ਬਫਰ, GLint drawbuffer, ਅਚਲ GLfloat *value);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ClearBufferfi)(GLenum ਬਫਰ, GLint drawbuffer, GLfloat ਡੂੰਘਾਈ, GLint stencil);
    ਬਾਹਰੀ ਅਚਲ GLubyte * (CODEGEN_FUNCPTR *GetStringi)(GLenum name, GLuint ਸੂਚਕਾਂਕ);

    // Extension: ARB_uniform_buffer_object
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਇਕਸਾਰ_Indices)(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei ਇਕਸਾਰ_ਗਿਣਤੀ, ਅਚਲ GLchar* ਅਚਲ *ਇਕਸਾਰ_ਨਾਂ, GLuint *ਇਕਸਾਰ_Indices);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸਰਗਰਮ_Uniformsiv)(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei ਇਕਸਾਰ_ਗਿਣਤੀ, ਅਚਲ GLuint *ਇਕਸਾਰ_Indices, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *ਇਕਸਾਰ_Name);
    ਬਾਹਰੀ GLuint (CODEGEN_FUNCPTR *Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ)(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *ਇਕਸਾਰ_ਖੰਡ_Name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, GLenum pname, GLint *params);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *ਇਕਸਾਰ_ਖੰਡ_Name);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਇਕਸਾਰ_ਖੰਡ_Binding)(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, GLuint ਇਕਸਾਰ_ਖੰਡ_Binding_2);

    // Extension: ARB_copy_buffer
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Copy_ਬਫਰ_Sub_ਅੰਕੜੇ)(GLenum read_ਨਿਸ਼ਾਨਾ, GLenum write_ਨਿਸ਼ਾਨਾ, GLintptr read_ਵਿੱਥ, GLintptr write_ਵਿੱਥ, GLsizeiptr size);

    // Extension: 3.1
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DrawArraysInstanced)(GLenum ਬਹੁਲਕ, GLint first, GLsizei ਗਿਣਤੀ_3, GLsizei instancecount);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DrawElementsInstanced)(GLenum ਬਹੁਲਕ, GLsizei ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid *indices, GLsizei instancecount);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Tex_ਬਫਰ)(GLenum target, GLenum internalformat, GLuint ਬਫਰ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Primitive_Restart_ਸੂਚਕਾਂਕ)(GLuint ਸੂਚਕਾਂਕ);

    // Legacy
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *EnableClientState)(GLenum cap);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *DisableClientState)(GLenum cap);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸ਼ੀਰਸ਼_ਸੂਚਕ)(GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਸਾਧਾਰਨ_ਸੂਚਕ)(GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Color_ਸੂਚਕ)(GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Tex_Coord_ਸੂਚਕ)(GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *TexEnvi)(GLenum target, GLenum pname, GLint param);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ)(GLenum ਬਹੁਲਕ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ)(ਖਾਲੀ);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Ortho)(GLdouble ਖੱਬੇ, GLdouble ਸੱਜੇ, GLdouble bottom, GLdouble ਸਿਖਰ_2, GLdouble near_val, GLdouble far_val);
    ਬਾਹਰੀ ਖਾਲੀ (CODEGEN_FUNCPTR *Color3d)(GLdouble red, GLdouble green, GLdouble blue);
}

#ਜੇ_ਅੰਤ // OPENGL_NOLOAD_STYLE_HPP
