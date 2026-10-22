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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "gl_core_3_1.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
        #ਅਨ੍ਤਰ੍ਭਾਵ <dlfcn.h>

        ਸਥਿਰ ਖਾਲੀ* Apple_GLGet_Proc_ਪਤਾ (ਅਚਲ ਅੱਖਰ* name)
        {
            ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
            ਸਥਿਰ ਖਾਲੀ * handle = ਨਿਰਾ;
            ਜੇ (!handle)
            {
                ਜੇ (!initialized)
                {
                    initialized = ਸੱਚ;
                    ਅਚਲ ਅੱਖਰ * ਅਚਲ ਰਸਤਾ = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(ਰਸਤਾ, RTLD_LAZY | RTLD_GLOBAL);
                }
                ਜੇ (!handle)
                    ਵਾਪਸ ਨਿਰਾ;
            }
            ਵਾਪਸ dlsym(handle, name);
        }
    #ਜੇ_ਅੰਤ // __APPLE__

    #ਜੇ defined(__sgi) || defined (__sun)
        #ਅਨ੍ਤਰ੍ਭਾਵ <dlfcn.h>
        #ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

        ਸਥਿਰ ਖਾਲੀ* Sun_Get_Proc_ਪਤਾ (ਅਚਲ ਅੱਖਰ* name)
        {
            ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ* (func_t*)(ਅਚਲ GLubyte*);

            ਸਥਿਰ ਖਾਲੀ* h = 0;
            ਸਥਿਰ func_t gpa = 0;

            ਜੇ (!h)
            {
                h = dlopen(ਨਿਰਾ, RTLD_LAZY | RTLD_LOCAL);
                ਜੇ (!h)
                    ਵਾਪਸ 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            ਵਾਪਸ gpa ? gpa((ਅਚਲ GLubyte*) name) : dlsym(h, name);
        }
    #ਜੇ_ਅੰਤ // __sgi || __sun

    #ਜੇ defined(_WIN32)
        #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
            #ਹਦਾਇਤ warning(disable: 4055)
            #ਹਦਾਇਤ warning(disable: 4054)
        #ਜੇ_ਅੰਤ

        ਸਥਿਰ ਪੂਰਨ Test_ਸੂਚਕ(ਅਚਲ PROC pTest)
        {
            ਜੇ(!pTest)
                ਵਾਪਸ 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            ਜੇ (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                ਵਾਪਸ 0;

            ਵਾਪਸ 1;
        }

        ਸਥਿਰ PROC Win_Get_Proc_ਪਤਾ(ਅਚਲ ਅੱਖਰ* name)
        {
            PROC pFunc = wgl_Get_Proc_ਪਤਾ((LPCSTR) name);
            ਜੇ (Test_ਸੂਚਕ(pFunc))
                ਵਾਪਸ pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            ਵਾਪਸ (PROC) Get_Proc_ਪਤਾ(glMod, (LPCSTR) name);
        }
    #ਜੇ_ਅੰਤ // _WIN32

    #ਜੇ defined(_WIN32)
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GL_GET_PROC_ਪਤਾ(name) Win_Get_Proc_ਪਤਾ(name)
    #ਜਾਂਜੇ defined(__APPLE__)
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GL_GET_PROC_ਪਤਾ(name) Apple_GLGet_Proc_ਪਤਾ(name)
    #ਜਾਂਜੇ defined(__sgi) || defined(__sun)
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GL_GET_PROC_ਪਤਾ(name) Sun_Get_Proc_ਪਤਾ(name)
    #ਨਹੀਂਤਾਂ // GLX
        #ਅਨ੍ਤਰ੍ਭਾਵ <GL/glx.h>

        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_GL_GET_PROC_ਪਤਾ(name) gl_XGet_Proc_ਪਤਾ_ARB((ਅਚਲ GLubyte*) name)
    #ਜੇ_ਅੰਤ

    ਸਥਿਰ ਖਾਲੀ* Int_Get_Proc_ਪਤਾ(ਅਚਲ ਅੱਖਰ* name)
    {
        ਖਾਲੀ* func =  (ਖਾਲੀ*) CV_GL_GET_PROC_ਪਤਾ(name);
        ਜੇ (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        ਵਾਪਸ func;
    }
#ਨਹੀਂਤਾਂ
#ਜੇ defined(_MSC_VER)
    #ਹਦਾਇਤ warning(disable : 4702)  // unreachable code
#ਜੇ_ਅੰਤ
    ਸਥਿਰ ਖਾਲੀ* Int_Get_Proc_ਪਤਾ(ਅਚਲ ਅੱਖਰ*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(ਅਚਲ GLubyte *);

    // Extension: 1.2
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , ਅਚਲ GLint *, ਅਚਲ GLsizei *, GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , ਅਚਲ GLsizei *, GLenum , ਅਚਲ GLvoid* ਅਚਲ *, GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , ਅਚਲ GLint *);

    // Extension: 1.5
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , ਅਚਲ GLvoid *, GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , ਅਚਲ GLenum *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , ਅਚਲ GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , ਅਚਲ GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , ਅਚਲ GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , ਅਚਲ GLchar* ਅਚਲ *, ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , ਅਚਲ GLvoid *);

    // Extension: 2.1
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , ਅਚਲ GLfloat *);

    // Extension: ARB_vertex_array_object
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , ਅਚਲ GLchar* ਅਚਲ *, GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , ਅਚਲ GLbyte *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , ਅਚਲ GLshort *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , ਅਚਲ GLubyte *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , ਅਚਲ GLushort *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , ਅਚਲ GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , ਅਚਲ GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , ਅਚਲ GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , ਅਚਲ GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , ਅਚਲ GLfloat *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , ਅਚਲ GLchar* ਅਚਲ *, GLuint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , ਅਚਲ GLuint *, GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , ਅਚਲ GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , ਅਚਲ GLvoid *, GLsizei );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , ਅਚਲ GLvoid *);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(ਖਾਲੀ);
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC ਸਤਰ_ਚੌੜਾਈ;
    PFNPOINTSIZEPROC ਬਿੰਦੂ_ਮਾਪ;
    PFNPOLYGONMODEPROC Polygon_ਬਹੁਲਕ;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_ਬਫਰ;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC Clear_ਡੂੰਘਾਈ;
    PFNSTENCILMASKPROC Stencil_ਨਕਾਬ;
    PFNCOLORMASKPROC Color_ਨਕਾਬ;
    PFNDEPTHMASKPROC ਡੂੰਘਾਈ_ਨਕਾਬ;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC ਅੰਤ;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC ਡੂੰਘਾਈ_Func;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC Read_ਬਫਰ;
    PFNREADPIXELSPROC ReadPixels;
    PFNGETBOOLEANVPROC GetBooleanv;
    PFNGETDOUBLEVPROC GetDoublev;
    PFNGETERRORPROC GetError;
    PFNGETFLOATVPROC GetFloatv;
    PFNGETINTEGERVPROC GetIntegerv;
    PFNGETSTRINGPROC GetString;
    PFNGETTEXIMAGEPROC GetTexImage;
    PFNGETTEXPARAMETERFVPROC GetTexParameterfv;
    PFNGETTEXPARAMETERIVPROC GetTexParameteriv;
    PFNGETTEXLEVELPARAMETERFVPROC GetTexLevelParameterfv;
    PFNGETTEXLEVELPARAMETERIVPROC GetTexLevelParameteriv;
    PFNISENABLEDPROC IsEnabled;
    PFNDEPTHRANGEPROC ਡੂੰਘਾਈ_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_ਵਿੱਥ;
    PFNCOPYTEXIMAGE1DPROC CopyTexImage1D;
    PFNCOPYTEXIMAGE2DPROC CopyTexImage2D;
    PFNCOPYTEXSUBIMAGE1DPROC CopyTexSubImage1D;
    PFNCOPYTEXSUBIMAGE2DPROC CopyTexSubImage2D;
    PFNTEXSUBIMAGE1DPROC TexSubImage1D;
    PFNTEXSUBIMAGE2DPROC TexSubImage2D;
    PFNBINDTEXTUREPROC BindTexture;
    PFNDELETETEXTURESPROC DeleteTextures;
    PFNGENTEXTURESPROC GenTextures;
    PFNISTEXTUREPROC IsTexture;
    PFNINDEXUBPROC Indexub;
    PFNINDEXUBVPROC Indexubv;

    // Extension: 1.2
    PFNBLENDCOLORPROC BlendColor;
    PFNBLENDEQUATIONPROC BlendEquation;
    PFNDRAWRANGEELEMENTSPROC DrawRangeElements;
    PFNTEXSUBIMAGE3DPROC TexSubImage3D;
    PFNCOPYTEXSUBIMAGE3DPROC CopyTexSubImage3D;

    // Extension: 1.3
    PFNACTIVETEXTUREPROC ਸਰਗਰਮ_Texture;
    PFNSAMPLECOVERAGEPROC SampleCoverage;
    PFNCOMPRESSEDTEXIMAGE3DPROC CompressedTexImage3D;
    PFNCOMPRESSEDTEXIMAGE2DPROC CompressedTexImage2D;
    PFNCOMPRESSEDTEXIMAGE1DPROC CompressedTexImage1D;
    PFNCOMPRESSEDTEXSUBIMAGE3DPROC CompressedTexSubImage3D;
    PFNCOMPRESSEDTEXSUBIMAGE2DPROC CompressedTexSubImage2D;
    PFNCOMPRESSEDTEXSUBIMAGE1DPROC CompressedTexSubImage1D;
    PFNGETCOMPRESSEDTEXIMAGEPROC GetCompressedTexImage;

    // Extension: 1.4
    PFNBLENDFUNCSEPARATEPROC BlendFuncSeparate;
    PFNMULTIDRAWARRAYSPROC MultiDrawArrays;
    PFNMULTIDRAWELEMENTSPROC MultiDrawElements;
    PFNPOINTPARAMETERFPROC ਬਿੰਦੂ_Parameterf;
    PFNPOINTPARAMETERFVPROC ਬਿੰਦੂ_Parameterfv;
    PFNPOINTPARAMETERIPROC ਬਿੰਦੂ_Parameteri;
    PFNPOINTPARAMETERIVPROC ਬਿੰਦੂ_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC ਸਿਰਾ_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_ਬਫਰ;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_ਬਫਰ;
    PFNBUFFERDATAPROC ਬਫਰ_ਅੰਕੜੇ;
    PFNBUFFERSUBDATAPROC ਬਫਰ_Sub_ਅੰਕੜੇ;
    PFNGETBUFFERSUBDATAPROC Get_ਬਫਰ_Sub_ਅੰਕੜੇ;
    PFNMAPBUFFERPROC Map_ਬਫਰ;
    PFNUNMAPBUFFERPROC Unmap_ਬਫਰ;
    PFNGETBUFFERPARAMETERIVPROC Get_ਬਫਰ_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_ਬਫਰ_Pointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC Stencil_ਨਕਾਬ_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_ਪ੍ਰੋਗਰਾਮ;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_ਪ੍ਰੋਗਰਾਮ;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC Disable_ਸ਼ੀਰਸ਼_Attrib_Array;
    PFNENABLEVERTEXATTRIBARRAYPROC Enable_ਸ਼ੀਰਸ਼_Attrib_Array;
    PFNGETACTIVEATTRIBPROC Get_ਸਰਗਰਮ_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_ਸਰਗਰਮ_ਇਕਸਾਰ;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_ਪ੍ਰੋਗਰਾਮ_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_ਸਰੋਤ;
    PFNGETUNIFORMLOCATIONPROC Get_ਇਕਸਾਰ_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_ਸ਼ੀਰਸ਼_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_ਸ਼ੀਰਸ਼_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_ਸ਼ੀਰਸ਼_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_ਪ੍ਰੋਗਰਾਮ;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_ਪ੍ਰੋਗਰਾਮ;
    PFNSHADERSOURCEPROC Shader_ਸਰੋਤ;
    PFNUSEPROGRAMPROC Use_ਪ੍ਰੋਗਰਾਮ;
    PFNUNIFORM1FPROC Uniform1f;
    PFNUNIFORM2FPROC Uniform2f;
    PFNUNIFORM3FPROC Uniform3f;
    PFNUNIFORM4FPROC Uniform4f;
    PFNUNIFORM1IPROC Uniform1i;
    PFNUNIFORM2IPROC Uniform2i;
    PFNUNIFORM3IPROC Uniform3i;
    PFNUNIFORM4IPROC Uniform4i;
    PFNUNIFORM1FVPROC Uniform1fv;
    PFNUNIFORM2FVPROC Uniform2fv;
    PFNUNIFORM3FVPROC Uniform3fv;
    PFNUNIFORM4FVPROC Uniform4fv;
    PFNUNIFORM1IVPROC Uniform1iv;
    PFNUNIFORM2IVPROC Uniform2iv;
    PFNUNIFORM3IVPROC Uniform3iv;
    PFNUNIFORM4IVPROC Uniform4iv;
    PFNUNIFORMMATRIX2FVPROC ਇਕਸਾਰ_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC ਇਕਸਾਰ_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC ਇਕਸਾਰ_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_ਪ੍ਰੋਗਰਾਮ;
    PFNVERTEXATTRIBPOINTERPROC ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC ਇਕਸਾਰ_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC ਇਕਸਾਰ_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC ਇਕਸਾਰ_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC ਇਕਸਾਰ_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC ਇਕਸਾਰ_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC ਇਕਸਾਰ_Matrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC Bind_ਸ਼ੀਰਸ਼_Array;
    PFNDELETEVERTEXARRAYSPROC Delete_ਸ਼ੀਰਸ਼_Arrays;
    PFNGENVERTEXARRAYSPROC Gen_ਸ਼ੀਰਸ਼_Arrays;
    PFNISVERTEXARRAYPROC Is_ਸ਼ੀਰਸ਼_Array;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC Map_ਬਫਰ_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_ਬਫਰ_Range;

    // Extension: ARB_framebuffer_object
    PFNISRENDERBUFFERPROC IsRenderbuffer;
    PFNBINDRENDERBUFFERPROC BindRenderbuffer;
    PFNDELETERENDERBUFFERSPROC DeleteRenderbuffers;
    PFNGENRENDERBUFFERSPROC GenRenderbuffers;
    PFNRENDERBUFFERSTORAGEPROC RenderbufferStorage;
    PFNGETRENDERBUFFERPARAMETERIVPROC GetRenderbufferParameteriv;
    PFNISFRAMEBUFFERPROC IsFramebuffer;
    PFNBINDFRAMEBUFFERPROC BindFramebuffer;
    PFNDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
    PFNGENFRAMEBUFFERSPROC GenFramebuffers;
    PFNCHECKFRAMEBUFFERSTATUSPROC Check_Framebuffer_ਸਥਿਤੀ;
    PFNFRAMEBUFFERTEXTURE1DPROC FramebufferTexture1D;
    PFNFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
    PFNFRAMEBUFFERTEXTURE3DPROC FramebufferTexture3D;
    PFNFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;
    PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC GetFramebufferAttachmentParameteriv;
    PFNGENERATEMIPMAPPROC GenerateMipmap;
    PFNBLITFRAMEBUFFERPROC BlitFramebuffer;
    PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC RenderbufferStorageMultisample;
    PFNFRAMEBUFFERTEXTURELAYERPROC FramebufferTextureLayer;

    // Extension: 3.0
    PFNCOLORMASKIPROC ColorMaski;
    PFNGETBOOLEANI_VPROC GetBooleani_v;
    PFNGETINTEGERI_VPROC GetIntegeri_v;
    PFNENABLEIPROC Enablei;
    PFNDISABLEIPROC Disablei;
    PFNISENABLEDIPROC IsEnabledi;
    PFNBEGINTRANSFORMFEEDBACKPROC BeginTransformFeedback;
    PFNENDTRANSFORMFEEDBACKPROC ਸਿਰਾ_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_ਬਫਰ_Range;
    PFNBINDBUFFERBASEPROC Bind_ਬਫਰ_ਅਧਾਰ;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC ਸਿਰਾ_Conditional_Render;
    PFNVERTEXATTRIBIPOINTERPROC ਸ਼ੀਰਸ਼_Attrib_IPointer;
    PFNGETVERTEXATTRIBIIVPROC Get_ਸ਼ੀਰਸ਼_Attrib_Iiv;
    PFNGETVERTEXATTRIBIUIVPROC Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv;
    PFNVERTEXATTRIBI1IPROC ਸ਼ੀਰਸ਼_Attrib_I1i;
    PFNVERTEXATTRIBI2IPROC ਸ਼ੀਰਸ਼_Attrib_I2i;
    PFNVERTEXATTRIBI3IPROC ਸ਼ੀਰਸ਼_Attrib_I3i;
    PFNVERTEXATTRIBI4IPROC ਸ਼ੀਰਸ਼_Attrib_I4i;
    PFNVERTEXATTRIBI1UIPROC ਸ਼ੀਰਸ਼_Attrib_I1ui;
    PFNVERTEXATTRIBI2UIPROC ਸ਼ੀਰਸ਼_Attrib_I2ui;
    PFNVERTEXATTRIBI3UIPROC ਸ਼ੀਰਸ਼_Attrib_I3ui;
    PFNVERTEXATTRIBI4UIPROC ਸ਼ੀਰਸ਼_Attrib_I4ui;
    PFNVERTEXATTRIBI1IVPROC ਸ਼ੀਰਸ਼_Attrib_I1iv;
    PFNVERTEXATTRIBI2IVPROC ਸ਼ੀਰਸ਼_Attrib_I2iv;
    PFNVERTEXATTRIBI3IVPROC ਸ਼ੀਰਸ਼_Attrib_I3iv;
    PFNVERTEXATTRIBI4IVPROC ਸ਼ੀਰਸ਼_Attrib_I4iv;
    PFNVERTEXATTRIBI1UIVPROC ਸ਼ੀਰਸ਼_Attrib_I1uiv;
    PFNVERTEXATTRIBI2UIVPROC ਸ਼ੀਰਸ਼_Attrib_I2uiv;
    PFNVERTEXATTRIBI3UIVPROC ਸ਼ੀਰਸ਼_Attrib_I3uiv;
    PFNVERTEXATTRIBI4UIVPROC ਸ਼ੀਰਸ਼_Attrib_I4uiv;
    PFNVERTEXATTRIBI4BVPROC ਸ਼ੀਰਸ਼_Attrib_I4bv;
    PFNVERTEXATTRIBI4SVPROC ਸ਼ੀਰਸ਼_Attrib_I4sv;
    PFNVERTEXATTRIBI4UBVPROC ਸ਼ੀਰਸ਼_Attrib_I4ubv;
    PFNVERTEXATTRIBI4USVPROC ਸ਼ੀਰਸ਼_Attrib_I4usv;
    PFNGETUNIFORMUIVPROC GetUniformuiv;
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_ਅੰਕੜੇ_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_ਅੰਕੜੇ_Location;
    PFNUNIFORM1UIPROC Uniform1ui;
    PFNUNIFORM2UIPROC Uniform2ui;
    PFNUNIFORM3UIPROC Uniform3ui;
    PFNUNIFORM4UIPROC Uniform4ui;
    PFNUNIFORM1UIVPROC Uniform1uiv;
    PFNUNIFORM2UIVPROC Uniform2uiv;
    PFNUNIFORM3UIVPROC Uniform3uiv;
    PFNUNIFORM4UIVPROC Uniform4uiv;
    PFNTEXPARAMETERIIVPROC TexParameterIiv;
    PFNTEXPARAMETERIUIVPROC TexParameterIuiv;
    PFNGETTEXPARAMETERIIVPROC GetTexParameterIiv;
    PFNGETTEXPARAMETERIUIVPROC GetTexParameterIuiv;
    PFNCLEARBUFFERIVPROC ClearBufferiv;
    PFNCLEARBUFFERUIVPROC ClearBufferuiv;
    PFNCLEARBUFFERFVPROC ClearBufferfv;
    PFNCLEARBUFFERFIPROC ClearBufferfi;
    PFNGETSTRINGIPROC GetStringi;

    // Extension: ARB_uniform_buffer_object
    PFNGETUNIFORMINDICESPROC Get_ਇਕਸਾਰ_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_ਸਰਗਰਮ_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name;
    PFNUNIFORMBLOCKBINDINGPROC ਇਕਸਾਰ_ਖੰਡ_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_ਬਫਰ_Sub_ਅੰਕੜੇ;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_ਬਫਰ;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_ਸੂਚਕਾਂਕ;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC ਸ਼ੀਰਸ਼_ਸੂਚਕ;
    PFNNORMALPOINTERPROC ਸਾਧਾਰਨ_ਸੂਚਕ;
    PFNCOLORPOINTERPROC Color_ਸੂਚਕ;
    PFNTEXCOORDPOINTERPROC Tex_Coord_ਸੂਚਕ;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ;
    PFNLOADIDENTITYPROC ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CullFace(GLenum ਬਹੁਲਕ)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_ਪਤਾ("glCullFace");
        CullFace(ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_FrontFace(GLenum ਬਹੁਲਕ)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_ਪਤਾ("glFrontFace");
        FrontFace(ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum ਬਹੁਲਕ)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_ਪਤਾ("glHint");
        Hint(target, ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸਤਰ_ਚੌੜਾਈ(GLfloat ਚੌੜਾਈ)
    {
        ਸਤਰ_ਚੌੜਾਈ = (PFNLINEWIDTHPROC)Int_Get_Proc_ਪਤਾ("glLineWidth");
        ਸਤਰ_ਚੌੜਾਈ(ਚੌੜਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਿੰਦੂ_ਮਾਪ(GLfloat size)
    {
        ਬਿੰਦੂ_ਮਾਪ = (PFNPOINTSIZEPROC)Int_Get_Proc_ਪਤਾ("glPointSize");
        ਬਿੰਦੂ_ਮਾਪ(size);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Polygon_ਬਹੁਲਕ(GLenum face, GLenum ਬਹੁਲਕ)
    {
        Polygon_ਬਹੁਲਕ = (PFNPOLYGONMODEPROC)Int_Get_Proc_ਪਤਾ("glPolygonMode");
        Polygon_ਬਹੁਲਕ(face, ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_ਪਤਾ("glScissor");
        Scissor(x, y, ਚੌੜਾਈ, ਉਚਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_ਪਤਾ("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ਅਚਲ GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_ਪਤਾ("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_ਪਤਾ("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ਅਚਲ GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei ਚੌੜਾਈ, GLint border, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_ਪਤਾ("glTexImage1D");
        TexImage1D(target, level, internalformat, ਚੌੜਾਈ, border, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLint border, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_ਪਤਾ("glTexImage2D");
        TexImage2D(target, level, internalformat, ਚੌੜਾਈ, ਉਚਾਈ, border, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Draw_ਬਫਰ(GLenum ਬਹੁਲਕ)
    {
        Draw_ਬਫਰ = (PFNDRAWBUFFERPROC)Int_Get_Proc_ਪਤਾ("glDrawBuffer");
        Draw_ਬਫਰ(ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Clear(GLbitfield ਨਕਾਬ)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_ਪਤਾ("glClear");
        Clear(ਨਕਾਬ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_ਪਤਾ("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_ਪਤਾ("glClearStencil");
        ClearStencil(s);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Clear_ਡੂੰਘਾਈ(GLdouble ਡੂੰਘਾਈ)
    {
        Clear_ਡੂੰਘਾਈ = (PFNCLEARDEPTHPROC)Int_Get_Proc_ਪਤਾ("glClearDepth");
        Clear_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Stencil_ਨਕਾਬ(GLuint ਨਕਾਬ)
    {
        Stencil_ਨਕਾਬ = (PFNSTENCILMASKPROC)Int_Get_Proc_ਪਤਾ("glStencilMask");
        Stencil_ਨਕਾਬ(ਨਕਾਬ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Color_ਨਕਾਬ(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_ਨਕਾਬ = (PFNCOLORMASKPROC)Int_Get_Proc_ਪਤਾ("glColorMask");
        Color_ਨਕਾਬ(red, green, blue, alpha);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਡੂੰਘਾਈ_ਨਕਾਬ(GLboolean flag)
    {
        ਡੂੰਘਾਈ_ਨਕਾਬ = (PFNDEPTHMASKPROC)Int_Get_Proc_ਪਤਾ("glDepthMask");
        ਡੂੰਘਾਈ_ਨਕਾਬ(flag);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_ਪਤਾ("glDisable");
        Disable(cap);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_ਪਤਾ("glEnable");
        Enable(cap);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਅੰਤ()
    {
        ਅੰਤ = (PFNFINISHPROC)Int_Get_Proc_ਪਤਾ("glFinish");
        ਅੰਤ();
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_ਪਤਾ("glFlush");
        Flush();
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_ਪਤਾ("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_ਪਤਾ("glLogicOp");
        LogicOp(opcode);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint ਨਕਾਬ)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_ਪਤਾ("glStencilFunc");
        StencilFunc(func, ref, ਨਕਾਬ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_ਪਤਾ("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਡੂੰਘਾਈ_Func(GLenum func)
    {
        ਡੂੰਘਾਈ_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_ਪਤਾ("glDepthFunc");
        ਡੂੰਘਾਈ_Func(func);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_ਪਤਾ("glPixelStoref");
        PixelStoref(pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_ਪਤਾ("glPixelStorei");
        PixelStorei(pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Read_ਬਫਰ(GLenum ਬਹੁਲਕ)
    {
        Read_ਬਫਰ = (PFNREADBUFFERPROC)Int_Get_Proc_ਪਤਾ("glReadBuffer");
        Read_ਬਫਰ(ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_ਪਤਾ("glReadPixels");
        ReadPixels(x, y, ਚੌੜਾਈ, ਉਚਾਈ, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_ਪਤਾ("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_ਪਤਾ("glGetDoublev");
        GetDoublev(pname, params);
    }

    ਸਥਿਰ GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_ਪਤਾ("glGetError");
        ਵਾਪਸ GetError();
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_ਪਤਾ("glGetFloatv");
        GetFloatv(pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_ਪਤਾ("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    ਸਥਿਰ ਅਚਲ GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_ਪਤਾ("glGetString");
        ਵਾਪਸ GetString(name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_ਪਤਾ("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_ਪਤਾ("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_ਪਤਾ("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_ਪਤਾ("glIsEnabled");
        ਵਾਪਸ IsEnabled(cap);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਡੂੰਘਾਈ_Range(GLdouble ren_near, GLdouble ren_far)
    {
        ਡੂੰਘਾਈ_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_ਪਤਾ("glDepthRange");
        ਡੂੰਘਾਈ_Range(ren_near, ren_far);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_ਪਤਾ("glViewport");
        Viewport(x, y, ਚੌੜਾਈ, ਉਚਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DrawArrays(GLenum ਬਹੁਲਕ, GLint first, GLsizei ਗਿਣਤੀ_3)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_ਪਤਾ("glDrawArrays");
        DrawArrays(ਬਹੁਲਕ, first, ਗਿਣਤੀ_3);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DrawElements(GLenum ਬਹੁਲਕ, GLsizei ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_ਪਤਾ("glDrawElements");
        DrawElements(ਬਹੁਲਕ, ਗਿਣਤੀ_3, type, indices);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_ਪਤਾ("glGetPointerv");
        GetPointerv(pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Polygon_ਵਿੱਥ(GLfloat ਗੁਣਕ, GLfloat units)
    {
        Polygon_ਵਿੱਥ = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_ਪਤਾ("glPolygonOffset");
        Polygon_ਵਿੱਥ(ਗੁਣਕ, units);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_ਪਤਾ("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, ਚੌੜਾਈ, border);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_ਪਤਾ("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, ਚੌੜਾਈ, ਉਚਾਈ, border);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei ਚੌੜਾਈ)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_ਪਤਾ("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, ਚੌੜਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_ਪਤਾ("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, ਚੌੜਾਈ, ਉਚਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei ਚੌੜਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_ਪਤਾ("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, ਚੌੜਾਈ, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_ਪਤਾ("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, ਚੌੜਾਈ, ਉਚਾਈ, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_ਪਤਾ("glBindTexture");
        BindTexture(target, texture);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ਅਚਲ GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_ਪਤਾ("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_ਪਤਾ("glGenTextures");
        GenTextures(n, textures);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_ਪਤਾ("glIsTexture");
        ਵਾਪਸ IsTexture(texture);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_ਪਤਾ("glIndexub");
        Indexub(c);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Indexubv(ਅਚਲ GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_ਪਤਾ("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_ਪਤਾ("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BlendEquation(GLenum ਬਹੁਲਕ)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_ਪਤਾ("glBlendEquation");
        BlendEquation(ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum ਬਹੁਲਕ, GLuint ਸ਼ੁਰੂ, GLuint end, GLsizei ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_ਪਤਾ("glDrawRangeElements");
        DrawRangeElements(ਬਹੁਲਕ, ਸ਼ੁਰੂ, end, ਗਿਣਤੀ_3, type, indices);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLsizei ਡੂੰਘਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_ਪਤਾ("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, format, type, pixels);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_ਪਤਾ("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, ਚੌੜਾਈ, ਉਚਾਈ);
    }

    // Extension: 1.3

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸਰਗਰਮ_Texture(GLenum texture)
    {
        ਸਰਗਰਮ_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_ਪਤਾ("glActiveTexture");
        ਸਰਗਰਮ_Texture(texture);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_ਪਤਾ("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLsizei ਡੂੰਘਾਈ, GLint border, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_ਪਤਾ("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, border, image_ਮਾਪ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLint border, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_ਪਤਾ("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, ਚੌੜਾਈ, ਉਚਾਈ, border, image_ਮਾਪ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLint border, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_ਪਤਾ("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, ਚੌੜਾਈ, border, image_ਮਾਪ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLsizei ਡੂੰਘਾਈ, GLenum format, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_ਪਤਾ("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, ਚੌੜਾਈ, ਉਚਾਈ, ਡੂੰਘਾਈ, format, image_ਮਾਪ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_ਪਤਾ("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, ਚੌੜਾਈ, ਉਚਾਈ, format, image_ਮਾਪ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei ਚੌੜਾਈ, GLenum format, GLsizei image_ਮਾਪ, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_ਪਤਾ("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, ਚੌੜਾਈ, format, image_ਮਾਪ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_ਪਤਾ("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_ਪਤਾ("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum ਬਹੁਲਕ, ਅਚਲ GLint *first, ਅਚਲ GLsizei *ਗਿਣਤੀ_3, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_ਪਤਾ("glMultiDrawArrays");
        MultiDrawArrays(ਬਹੁਲਕ, first, ਗਿਣਤੀ_3, drawcount);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum ਬਹੁਲਕ, ਅਚਲ GLsizei *ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid* ਅਚਲ *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_ਪਤਾ("glMultiDrawElements");
        MultiDrawElements(ਬਹੁਲਕ, ਗਿਣਤੀ_3, type, indices, drawcount);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਿੰਦੂ_Parameterf(GLenum pname, GLfloat param)
    {
        ਬਿੰਦੂ_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_ਪਤਾ("glPointParameterf");
        ਬਿੰਦੂ_Parameterf(pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਿੰਦੂ_Parameterfv(GLenum pname, ਅਚਲ GLfloat *params)
    {
        ਬਿੰਦੂ_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_ਪਤਾ("glPointParameterfv");
        ਬਿੰਦੂ_Parameterfv(pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਿੰਦੂ_Parameteri(GLenum pname, GLint param)
    {
        ਬਿੰਦੂ_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_ਪਤਾ("glPointParameteri");
        ਬਿੰਦੂ_Parameteri(pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਿੰਦੂ_Parameteriv(GLenum pname, ਅਚਲ GLint *params)
    {
        ਬਿੰਦੂ_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glPointParameteriv");
        ਬਿੰਦੂ_Parameteriv(pname, params);
    }

    // Extension: 1.5

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_ਪਤਾ("glGenQueries");
        GenQueries(n, ids);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ਅਚਲ GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_ਪਤਾ("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_ਪਤਾ("glIsQuery");
        ਵਾਪਸ IsQuery(id);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_ਪਤਾ("glBeginQuery");
        BeginQuery(target, id);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸਿਰਾ_Query(GLenum target)
    {
        ਸਿਰਾ_Query = (PFNENDQUERYPROC)Int_Get_Proc_ਪਤਾ("glEndQuery");
        ਸਿਰਾ_Query(target);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_ਪਤਾ("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_ਪਤਾ("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_ਪਤਾ("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Bind_ਬਫਰ(GLenum target, GLuint ਬਫਰ)
    {
        Bind_ਬਫਰ = (PFNBINDBUFFERPROC)Int_Get_Proc_ਪਤਾ("glBindBuffer");
        Bind_ਬਫਰ(target, ਬਫਰ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ਅਚਲ GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glGenBuffers");
        GenBuffers(n, buffers);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_Is_ਬਫਰ(GLuint ਬਫਰ)
    {
        Is_ਬਫਰ = (PFNISBUFFERPROC)Int_Get_Proc_ਪਤਾ("glIsBuffer");
        ਵਾਪਸ Is_ਬਫਰ(ਬਫਰ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਫਰ_ਅੰਕੜੇ(GLenum target, GLsizeiptr size, ਅਚਲ GLvoid *ਅੰਕੜੇ, GLenum usage)
    {
        ਬਫਰ_ਅੰਕੜੇ = (PFNBUFFERDATAPROC)Int_Get_Proc_ਪਤਾ("glBufferData");
        ਬਫਰ_ਅੰਕੜੇ(target, size, ਅੰਕੜੇ, usage);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਬਫਰ_Sub_ਅੰਕੜੇ(GLenum target, GLintptr offset, GLsizeiptr size, ਅਚਲ GLvoid *ਅੰਕੜੇ)
    {
        ਬਫਰ_Sub_ਅੰਕੜੇ = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_ਪਤਾ("glBufferSubData");
        ਬਫਰ_Sub_ਅੰਕੜੇ(target, offset, size, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਬਫਰ_Sub_ਅੰਕੜੇ(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *ਅੰਕੜੇ)
    {
        Get_ਬਫਰ_Sub_ਅੰਕੜੇ = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_ਪਤਾ("glGetBufferSubData");
        Get_ਬਫਰ_Sub_ਅੰਕੜੇ(target, offset, size, ਅੰਕੜੇ);
    }

    ਸਥਿਰ GLvoid* CODEGEN_FUNCPTR Switch_Map_ਬਫਰ(GLenum target, GLenum access)
    {
        Map_ਬਫਰ = (PFNMAPBUFFERPROC)Int_Get_Proc_ਪਤਾ("glMapBuffer");
        ਵਾਪਸ Map_ਬਫਰ(target, access);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_Unmap_ਬਫਰ(GLenum target)
    {
        Unmap_ਬਫਰ = (PFNUNMAPBUFFERPROC)Int_Get_Proc_ਪਤਾ("glUnmapBuffer");
        ਵਾਪਸ Unmap_ਬਫਰ(target);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਬਫਰ_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_ਬਫਰ_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glGetBufferParameteriv");
        Get_ਬਫਰ_Parameteriv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਬਫਰ_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_ਬਫਰ_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_ਪਤਾ("glGetBufferPointerv");
        Get_ਬਫਰ_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum ਬਹੁਲਕ_RGB, GLenum ਬਹੁਲਕ_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_ਪਤਾ("glBlendEquationSeparate");
        BlendEquationSeparate(ਬਹੁਲਕ_RGB, ਬਹੁਲਕ_Alpha);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ਅਚਲ GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_ਪਤਾ("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint ਨਕਾਬ)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_ਪਤਾ("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, ਨਕਾਬ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Stencil_ਨਕਾਬ_Separate(GLenum face, GLuint ਨਕਾਬ)
    {
        Stencil_ਨਕਾਬ_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_ਪਤਾ("glStencilMaskSeparate");
        Stencil_ਨਕਾਬ_Separate(face, ਨਕਾਬ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_AttachShader(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_ਪਤਾ("glAttachShader");
        AttachShader(ਪ੍ਰੋਗਰਾਮ, shader);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_ਪਤਾ("glBindAttribLocation");
        BindAttribLocation(ਪ੍ਰੋਗਰਾਮ, ਸੂਚਕਾਂਕ, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_ਪਤਾ("glCompileShader");
        CompileShader(shader);
    }

    ਸਥਿਰ GLuint CODEGEN_FUNCPTR Switch_Create_ਪ੍ਰੋਗਰਾਮ()
    {
        Create_ਪ੍ਰੋਗਰਾਮ = (PFNCREATEPROGRAMPROC)Int_Get_Proc_ਪਤਾ("glCreateProgram");
        ਵਾਪਸ Create_ਪ੍ਰੋਗਰਾਮ();
    }

    ਸਥਿਰ GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_ਪਤਾ("glCreateShader");
        ਵਾਪਸ CreateShader(type);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Delete_ਪ੍ਰੋਗਰਾਮ(GLuint ਪ੍ਰੋਗਰਾਮ)
    {
        Delete_ਪ੍ਰੋਗਰਾਮ = (PFNDELETEPROGRAMPROC)Int_Get_Proc_ਪਤਾ("glDeleteProgram");
        Delete_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_ਪਤਾ("glDeleteShader");
        DeleteShader(shader);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DetachShader(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_ਪਤਾ("glDetachShader");
        DetachShader(ਪ੍ਰੋਗਰਾਮ, shader);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Disable_ਸ਼ੀਰਸ਼_Attrib_Array(GLuint ਸੂਚਕਾਂਕ)
    {
        Disable_ਸ਼ੀਰਸ਼_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_ਪਤਾ("glDisableVertexAttribArray");
        Disable_ਸ਼ੀਰਸ਼_Attrib_Array(ਸੂਚਕਾਂਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Enable_ਸ਼ੀਰਸ਼_Attrib_Array(GLuint ਸੂਚਕਾਂਕ)
    {
        Enable_ਸ਼ੀਰਸ਼_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_ਪਤਾ("glEnableVertexAttribArray");
        Enable_ਸ਼ੀਰਸ਼_Attrib_Array(ਸੂਚਕਾਂਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸਰਗਰਮ_Attrib(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_ਸਰਗਰਮ_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_ਪਤਾ("glGetActiveAttrib");
        Get_ਸਰਗਰਮ_Attrib(ਪ੍ਰੋਗਰਾਮ, ਸੂਚਕਾਂਕ, buf_ਮਾਪ, length, size, type, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_ਪਤਾ("glGetActiveUniform");
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ(ਪ੍ਰੋਗਰਾਮ, ਸੂਚਕਾਂਕ, buf_ਮਾਪ, length, size, type, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei max_ਗਿਣਤੀ, GLsizei *ਗਿਣਤੀ_3, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_ਪਤਾ("glGetAttachedShaders");
        GetAttachedShaders(ਪ੍ਰੋਗਰਾਮ, max_ਗਿਣਤੀ, ਗਿਣਤੀ_3, obj);
    }

    ਸਥਿਰ GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_ਪਤਾ("glGetAttribLocation");
        ਵਾਪਸ GetAttribLocation(ਪ੍ਰੋਗਰਾਮ, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint ਪ੍ਰੋਗਰਾਮ, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_ਪਤਾ("glGetProgramiv");
        GetProgramiv(ਪ੍ਰੋਗਰਾਮ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਪ੍ਰੋਗਰਾਮ_Info_Log(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *infoLog)
    {
        Get_ਪ੍ਰੋਗਰਾਮ_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_ਪਤਾ("glGetProgramInfoLog");
        Get_ਪ੍ਰੋਗਰਾਮ_Info_Log(ਪ੍ਰੋਗਰਾਮ, buf_ਮਾਪ, length, infoLog);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_ਪਤਾ("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_ਪਤਾ("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_ਮਾਪ, length, infoLog);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_Shader_ਸਰੋਤ(GLuint shader, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *ਸਰੋਤ)
    {
        Get_Shader_ਸਰੋਤ = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_ਪਤਾ("glGetShaderSource");
        Get_Shader_ਸਰੋਤ(shader, buf_ਮਾਪ, length, ਸਰੋਤ);
    }

    ਸਥਿਰ GLint CODEGEN_FUNCPTR Switch_Get_ਇਕਸਾਰ_Location(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *name)
    {
        Get_ਇਕਸਾਰ_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_ਪਤਾ("glGetUniformLocation");
        ਵਾਪਸ Get_ਇਕਸਾਰ_Location(ਪ੍ਰੋਗਰਾਮ, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint ਪ੍ਰੋਗਰਾਮ, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_ਪਤਾ("glGetUniformfv");
        GetUniformfv(ਪ੍ਰੋਗਰਾਮ, location, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint ਪ੍ਰੋਗਰਾਮ, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_ਪਤਾ("glGetUniformiv");
        GetUniformiv(ਪ੍ਰੋਗਰਾਮ, location, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸ਼ੀਰਸ਼_Attribdv(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLdouble *params)
    {
        Get_ਸ਼ੀਰਸ਼_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_ਪਤਾ("glGetVertexAttribdv");
        Get_ਸ਼ੀਰਸ਼_Attribdv(ਸੂਚਕਾਂਕ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸ਼ੀਰਸ਼_Attribfv(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLfloat *params)
    {
        Get_ਸ਼ੀਰਸ਼_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_ਪਤਾ("glGetVertexAttribfv");
        Get_ਸ਼ੀਰਸ਼_Attribfv(ਸੂਚਕਾਂਕ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸ਼ੀਰਸ਼_Attribiv(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLint *params)
    {
        Get_ਸ਼ੀਰਸ਼_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_ਪਤਾ("glGetVertexAttribiv");
        Get_ਸ਼ੀਰਸ਼_Attribiv(ਸੂਚਕਾਂਕ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLvoid* *pointer)
    {
        Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_ਪਤਾ("glGetVertexAttribPointerv");
        Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv(ਸੂਚਕਾਂਕ, pname, pointer);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_Is_ਪ੍ਰੋਗਰਾਮ(GLuint ਪ੍ਰੋਗਰਾਮ)
    {
        Is_ਪ੍ਰੋਗਰਾਮ = (PFNISPROGRAMPROC)Int_Get_Proc_ਪਤਾ("glIsProgram");
        ਵਾਪਸ Is_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_ਪਤਾ("glIsShader");
        ਵਾਪਸ IsShader(shader);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Link_ਪ੍ਰੋਗਰਾਮ(GLuint ਪ੍ਰੋਗਰਾਮ)
    {
        Link_ਪ੍ਰੋਗਰਾਮ = (PFNLINKPROGRAMPROC)Int_Get_Proc_ਪਤਾ("glLinkProgram");
        Link_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Shader_ਸਰੋਤ(GLuint shader, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLchar* ਅਚਲ *string, ਅਚਲ GLint *length)
    {
        Shader_ਸਰੋਤ = (PFNSHADERSOURCEPROC)Int_Get_Proc_ਪਤਾ("glShaderSource");
        Shader_ਸਰੋਤ(shader, ਗਿਣਤੀ_3, string, length);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Use_ਪ੍ਰੋਗਰਾਮ(GLuint ਪ੍ਰੋਗਰਾਮ)
    {
        Use_ਪ੍ਰੋਗਰਾਮ = (PFNUSEPROGRAMPROC)Int_Get_Proc_ਪਤਾ("glUseProgram");
        Use_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_ਪਤਾ("glUniform1f");
        Uniform1f(location, v0);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_ਪਤਾ("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_ਪਤਾ("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_ਪਤਾ("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_ਪਤਾ("glUniform1i");
        Uniform1i(location, v0);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_ਪਤਾ("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_ਪਤਾ("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_ਪਤਾ("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_ਪਤਾ("glUniform1fv");
        Uniform1fv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_ਪਤਾ("glUniform2fv");
        Uniform2fv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_ਪਤਾ("glUniform3fv");
        Uniform3fv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_ਪਤਾ("glUniform4fv");
        Uniform4fv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_ਪਤਾ("glUniform1iv");
        Uniform1iv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_ਪਤਾ("glUniform2iv");
        Uniform2iv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_ਪਤਾ("glUniform3iv");
        Uniform3iv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_ਪਤਾ("glUniform4iv");
        Uniform4iv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix2fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix2fv");
        ਇਕਸਾਰ_Matrix2fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix3fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix3fv");
        ਇਕਸਾਰ_Matrix3fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix4fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix4fv");
        ਇਕਸਾਰ_Matrix4fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Validate_ਪ੍ਰੋਗਰਾਮ(GLuint ਪ੍ਰੋਗਰਾਮ)
    {
        Validate_ਪ੍ਰੋਗਰਾਮ = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_ਪਤਾ("glValidateProgram");
        Validate_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ(GLuint ਸੂਚਕਾਂਕ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ਅਚਲ GLvoid *pointer)
    {
        ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribPointer");
        ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ(ਸੂਚਕਾਂਕ, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix2x3fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix2x3fv");
        ਇਕਸਾਰ_Matrix2x3fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix3x2fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix3x2fv");
        ਇਕਸਾਰ_Matrix3x2fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix2x4fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix2x4fv");
        ਇਕਸਾਰ_Matrix2x4fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix4x2fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix4x2fv");
        ਇਕਸਾਰ_Matrix4x2fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix3x4fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix3x4fv");
        ਇਕਸਾਰ_Matrix3x4fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_Matrix4x3fv(GLint location, GLsizei ਗਿਣਤੀ_3, GLboolean transpose, ਅਚਲ GLfloat *value)
    {
        ਇਕਸਾਰ_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_ਪਤਾ("glUniformMatrix4x3fv");
        ਇਕਸਾਰ_Matrix4x3fv(location, ਗਿਣਤੀ_3, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Bind_ਸ਼ੀਰਸ਼_Array(GLuint ren_array)
    {
        Bind_ਸ਼ੀਰਸ਼_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_ਪਤਾ("glBindVertexArray");
        Bind_ਸ਼ੀਰਸ਼_Array(ren_array);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Delete_ਸ਼ੀਰਸ਼_Arrays(GLsizei n, ਅਚਲ GLuint *arrays)
    {
        Delete_ਸ਼ੀਰਸ਼_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_ਪਤਾ("glDeleteVertexArrays");
        Delete_ਸ਼ੀਰਸ਼_Arrays(n, arrays);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Gen_ਸ਼ੀਰਸ਼_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_ਸ਼ੀਰਸ਼_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_ਪਤਾ("glGenVertexArrays");
        Gen_ਸ਼ੀਰਸ਼_Arrays(n, arrays);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_Is_ਸ਼ੀਰਸ਼_Array(GLuint ren_array)
    {
        Is_ਸ਼ੀਰਸ਼_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_ਪਤਾ("glIsVertexArray");
        ਵਾਪਸ Is_ਸ਼ੀਰਸ਼_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    ਸਥਿਰ GLvoid* CODEGEN_FUNCPTR Switch_Map_ਬਫਰ_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_ਬਫਰ_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_ਪਤਾ("glMapBufferRange");
        ਵਾਪਸ Map_ਬਫਰ_Range(target, offset, length, access);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Flush_Mapped_ਬਫਰ_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_ਬਫਰ_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_ਪਤਾ("glFlushMappedBufferRange");
        Flush_Mapped_ਬਫਰ_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_ਪਤਾ("glIsRenderbuffer");
        ਵਾਪਸ IsRenderbuffer(renderbuffer);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_ਪਤਾ("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ਅਚਲ GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_ਪਤਾ("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, ਚੌੜਾਈ, ਉਚਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_ਪਤਾ("glIsFramebuffer");
        ਵਾਪਸ IsFramebuffer(framebuffer);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_ਪਤਾ("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ਅਚਲ GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_ਪਤਾ("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    ਸਥਿਰ GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_ਸਥਿਤੀ(GLenum target)
    {
        Check_Framebuffer_ਸਥਿਤੀ = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_ਪਤਾ("glCheckFramebufferStatus");
        ਵਾਪਸ Check_Framebuffer_ਸਥਿਤੀ(target);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_ਪਤਾ("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_ਪਤਾ("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_ਪਤਾ("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_ਪਤਾ("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_ਪਤਾ("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_ਪਤਾ("glGenerateMipmap");
        GenerateMipmap(target);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ਨਕਾਬ, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_ਪਤਾ("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, ਨਕਾਬ, filter);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_ਪਤਾ("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, ਚੌੜਾਈ, ਉਚਾਈ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_ਪਤਾ("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ColorMaski(GLuint ਸੂਚਕਾਂਕ, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_ਪਤਾ("glColorMaski");
        ColorMaski(ਸੂਚਕਾਂਕ, r, g, b, a);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLboolean *ਅੰਕੜੇ)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_ਪਤਾ("glGetBooleani_v");
        GetBooleani_v(target, ਸੂਚਕਾਂਕ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLint *ਅੰਕੜੇ)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_ਪਤਾ("glGetIntegeri_v");
        GetIntegeri_v(target, ਸੂਚਕਾਂਕ, ਅੰਕੜੇ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint ਸੂਚਕਾਂਕ)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_ਪਤਾ("glEnablei");
        Enablei(target, ਸੂਚਕਾਂਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint ਸੂਚਕਾਂਕ)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_ਪਤਾ("glDisablei");
        Disablei(target, ਸੂਚਕਾਂਕ);
    }

    ਸਥਿਰ GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint ਸੂਚਕਾਂਕ)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_ਪਤਾ("glIsEnabledi");
        ਵਾਪਸ IsEnabledi(target, ਸੂਚਕਾਂਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_ਬਹੁਲਕ)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_ਪਤਾ("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸਿਰਾ_Transform_Feedback()
    {
        ਸਿਰਾ_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_ਪਤਾ("glEndTransformFeedback");
        ਸਿਰਾ_Transform_Feedback();
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Bind_ਬਫਰ_Range(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLuint ਬਫਰ, GLintptr offset, GLsizeiptr size)
    {
        Bind_ਬਫਰ_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_ਪਤਾ("glBindBufferRange");
        Bind_ਬਫਰ_Range(target, ਸੂਚਕਾਂਕ, ਬਫਰ, offset, size);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Bind_ਬਫਰ_ਅਧਾਰ(GLenum target, GLuint ਸੂਚਕਾਂਕ, GLuint ਬਫਰ)
    {
        Bind_ਬਫਰ_ਅਧਾਰ = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_ਪਤਾ("glBindBufferBase");
        Bind_ਬਫਰ_ਅਧਾਰ(target, ਸੂਚਕਾਂਕ, ਬਫਰ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLchar* ਅਚਲ *varyings, GLenum ਬਫਰ_ਬਹੁਲਕ)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_ਪਤਾ("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(ਪ੍ਰੋਗਰਾਮ, ਗਿਣਤੀ_3, varyings, ਬਫਰ_ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_ਪਤਾ("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(ਪ੍ਰੋਗਰਾਮ, ਸੂਚਕਾਂਕ, buf_ਮਾਪ, length, size, type, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_ਪਤਾ("glClampColor");
        ClampColor(target, clamp);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum ਬਹੁਲਕ)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_ਪਤਾ("glBeginConditionalRender");
        BeginConditionalRender(id, ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸਿਰਾ_Conditional_Render()
    {
        ਸਿਰਾ_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_ਪਤਾ("glEndConditionalRender");
        ਸਿਰਾ_Conditional_Render();
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_IPointer(GLuint ਸੂਚਕਾਂਕ, GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *pointer)
    {
        ਸ਼ੀਰਸ਼_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribIPointer");
        ਸ਼ੀਰਸ਼_Attrib_IPointer(ਸੂਚਕਾਂਕ, size, type, stride, pointer);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸ਼ੀਰਸ਼_Attrib_Iiv(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLint *params)
    {
        Get_ਸ਼ੀਰਸ਼_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_ਪਤਾ("glGetVertexAttribIiv");
        Get_ਸ਼ੀਰਸ਼_Attrib_Iiv(ਸੂਚਕਾਂਕ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv(GLuint ਸੂਚਕਾਂਕ, GLenum pname, GLuint *params)
    {
        Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_ਪਤਾ("glGetVertexAttribIuiv");
        Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv(ਸੂਚਕਾਂਕ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I1i(GLuint ਸੂਚਕਾਂਕ, GLint x)
    {
        ਸ਼ੀਰਸ਼_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI1i");
        ਸ਼ੀਰਸ਼_Attrib_I1i(ਸੂਚਕਾਂਕ, x);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I2i(GLuint ਸੂਚਕਾਂਕ, GLint x, GLint y)
    {
        ਸ਼ੀਰਸ਼_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI2i");
        ਸ਼ੀਰਸ਼_Attrib_I2i(ਸੂਚਕਾਂਕ, x, y);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I3i(GLuint ਸੂਚਕਾਂਕ, GLint x, GLint y, GLint z)
    {
        ਸ਼ੀਰਸ਼_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI3i");
        ਸ਼ੀਰਸ਼_Attrib_I3i(ਸੂਚਕਾਂਕ, x, y, z);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4i(GLuint ਸੂਚਕਾਂਕ, GLint x, GLint y, GLint z, GLint w)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4i");
        ਸ਼ੀਰਸ਼_Attrib_I4i(ਸੂਚਕਾਂਕ, x, y, z, w);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I1ui(GLuint ਸੂਚਕਾਂਕ, GLuint x)
    {
        ਸ਼ੀਰਸ਼_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI1ui");
        ਸ਼ੀਰਸ਼_Attrib_I1ui(ਸੂਚਕਾਂਕ, x);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I2ui(GLuint ਸੂਚਕਾਂਕ, GLuint x, GLuint y)
    {
        ਸ਼ੀਰਸ਼_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI2ui");
        ਸ਼ੀਰਸ਼_Attrib_I2ui(ਸੂਚਕਾਂਕ, x, y);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I3ui(GLuint ਸੂਚਕਾਂਕ, GLuint x, GLuint y, GLuint z)
    {
        ਸ਼ੀਰਸ਼_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI3ui");
        ਸ਼ੀਰਸ਼_Attrib_I3ui(ਸੂਚਕਾਂਕ, x, y, z);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4ui(GLuint ਸੂਚਕਾਂਕ, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4ui");
        ਸ਼ੀਰਸ਼_Attrib_I4ui(ਸੂਚਕਾਂਕ, x, y, z, w);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I1iv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI1iv");
        ਸ਼ੀਰਸ਼_Attrib_I1iv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I2iv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI2iv");
        ਸ਼ੀਰਸ਼_Attrib_I2iv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I3iv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI3iv");
        ਸ਼ੀਰਸ਼_Attrib_I3iv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4iv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4iv");
        ਸ਼ੀਰਸ਼_Attrib_I4iv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I1uiv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI1uiv");
        ਸ਼ੀਰਸ਼_Attrib_I1uiv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I2uiv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI2uiv");
        ਸ਼ੀਰਸ਼_Attrib_I2uiv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I3uiv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI3uiv");
        ਸ਼ੀਰਸ਼_Attrib_I3uiv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4uiv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLuint *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4uiv");
        ਸ਼ੀਰਸ਼_Attrib_I4uiv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4bv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLbyte *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4bv");
        ਸ਼ੀਰਸ਼_Attrib_I4bv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4sv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLshort *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4sv");
        ਸ਼ੀਰਸ਼_Attrib_I4sv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4ubv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLubyte *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4ubv");
        ਸ਼ੀਰਸ਼_Attrib_I4ubv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_Attrib_I4usv(GLuint ਸੂਚਕਾਂਕ, ਅਚਲ GLushort *v)
    {
        ਸ਼ੀਰਸ਼_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_ਪਤਾ("glVertexAttribI4usv");
        ਸ਼ੀਰਸ਼_Attrib_I4usv(ਸੂਚਕਾਂਕ, v);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint ਪ੍ਰੋਗਰਾਮ, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_ਪਤਾ("glGetUniformuiv");
        GetUniformuiv(ਪ੍ਰੋਗਰਾਮ, location, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Bind_Frag_ਅੰਕੜੇ_Location(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint color, ਅਚਲ GLchar *name)
    {
        Bind_Frag_ਅੰਕੜੇ_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_ਪਤਾ("glBindFragDataLocation");
        Bind_Frag_ਅੰਕੜੇ_Location(ਪ੍ਰੋਗਰਾਮ, color, name);
    }

    ਸਥਿਰ GLint CODEGEN_FUNCPTR Switch_Get_Frag_ਅੰਕੜੇ_Location(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *name)
    {
        Get_Frag_ਅੰਕੜੇ_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_ਪਤਾ("glGetFragDataLocation");
        ਵਾਪਸ Get_Frag_ਅੰਕੜੇ_Location(ਪ੍ਰੋਗਰਾਮ, name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_ਪਤਾ("glUniform1ui");
        Uniform1ui(location, v0);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_ਪਤਾ("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_ਪਤਾ("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_ਪਤਾ("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_ਪਤਾ("glUniform1uiv");
        Uniform1uiv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_ਪਤਾ("glUniform2uiv");
        Uniform2uiv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_ਪਤਾ("glUniform3uiv");
        Uniform3uiv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei ਗਿਣਤੀ_3, ਅਚਲ GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_ਪਤਾ("glUniform4uiv");
        Uniform4uiv(location, ਗਿਣਤੀ_3, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ਅਚਲ GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_ਪਤਾ("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ਅਚਲ GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_ਪਤਾ("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_ਪਤਾ("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_ਪਤਾ("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum ਬਫਰ, GLint drawbuffer, ਅਚਲ GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_ਪਤਾ("glClearBufferiv");
        ClearBufferiv(ਬਫਰ, drawbuffer, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum ਬਫਰ, GLint drawbuffer, ਅਚਲ GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_ਪਤਾ("glClearBufferuiv");
        ClearBufferuiv(ਬਫਰ, drawbuffer, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum ਬਫਰ, GLint drawbuffer, ਅਚਲ GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_ਪਤਾ("glClearBufferfv");
        ClearBufferfv(ਬਫਰ, drawbuffer, value);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum ਬਫਰ, GLint drawbuffer, GLfloat ਡੂੰਘਾਈ, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_ਪਤਾ("glClearBufferfi");
        ClearBufferfi(ਬਫਰ, drawbuffer, ਡੂੰਘਾਈ, stencil);
    }

    ਸਥਿਰ ਅਚਲ GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint ਸੂਚਕਾਂਕ)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_ਪਤਾ("glGetStringi");
        ਵਾਪਸ GetStringi(name, ਸੂਚਕਾਂਕ);
    }

    // Extension: ARB_uniform_buffer_object

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਇਕਸਾਰ_Indices(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei ਇਕਸਾਰ_ਗਿਣਤੀ, ਅਚਲ GLchar* ਅਚਲ *ਇਕਸਾਰ_ਨਾਂ, GLuint *ਇਕਸਾਰ_Indices)
    {
        Get_ਇਕਸਾਰ_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_ਪਤਾ("glGetUniformIndices");
        Get_ਇਕਸਾਰ_Indices(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਗਿਣਤੀ, ਇਕਸਾਰ_ਨਾਂ, ਇਕਸਾਰ_Indices);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸਰਗਰਮ_Uniformsiv(GLuint ਪ੍ਰੋਗਰਾਮ, GLsizei ਇਕਸਾਰ_ਗਿਣਤੀ, ਅਚਲ GLuint *ਇਕਸਾਰ_Indices, GLenum pname, GLint *params)
    {
        Get_ਸਰਗਰਮ_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_ਪਤਾ("glGetActiveUniformsiv");
        Get_ਸਰਗਰਮ_Uniformsiv(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਗਿਣਤੀ, ਇਕਸਾਰ_Indices, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *ਇਕਸਾਰ_Name)
    {
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_ਪਤਾ("glGetActiveUniformName");
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਸੂਚਕਾਂਕ, buf_ਮਾਪ, length, ਇਕਸਾਰ_Name);
    }

    ਸਥਿਰ GLuint CODEGEN_FUNCPTR Switch_Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ(GLuint ਪ੍ਰੋਗਰਾਮ, ਅਚਲ GLchar *ਇਕਸਾਰ_ਖੰਡ_Name)
    {
        Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_ਪਤਾ("glGetUniformBlockIndex");
        ਵਾਪਸ Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਖੰਡ_Name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, GLenum pname, GLint *params)
    {
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_ਪਤਾ("glGetActiveUniformBlockiv");
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, pname, params);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, GLsizei buf_ਮਾਪ, GLsizei *length, GLchar *ਇਕਸਾਰ_ਖੰਡ_Name)
    {
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_ਪਤਾ("glGetActiveUniformBlockName");
        Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, buf_ਮਾਪ, length, ਇਕਸਾਰ_ਖੰਡ_Name);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਇਕਸਾਰ_ਖੰਡ_Binding(GLuint ਪ੍ਰੋਗਰਾਮ, GLuint ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, GLuint ਇਕਸਾਰ_ਖੰਡ_Binding_2)
    {
        ਇਕਸਾਰ_ਖੰਡ_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_ਪਤਾ("glUniformBlockBinding");
        ਇਕਸਾਰ_ਖੰਡ_Binding(ਪ੍ਰੋਗਰਾਮ, ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ, ਇਕਸਾਰ_ਖੰਡ_Binding_2);
    }

    // Extension: ARB_copy_buffer

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Copy_ਬਫਰ_Sub_ਅੰਕੜੇ(GLenum read_ਨਿਸ਼ਾਨਾ, GLenum write_ਨਿਸ਼ਾਨਾ, GLintptr read_ਵਿੱਥ, GLintptr write_ਵਿੱਥ, GLsizeiptr size)
    {
        Copy_ਬਫਰ_Sub_ਅੰਕੜੇ = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_ਪਤਾ("glCopyBufferSubData");
        Copy_ਬਫਰ_Sub_ਅੰਕੜੇ(read_ਨਿਸ਼ਾਨਾ, write_ਨਿਸ਼ਾਨਾ, read_ਵਿੱਥ, write_ਵਿੱਥ, size);
    }

    // Extension: 3.1

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum ਬਹੁਲਕ, GLint first, GLsizei ਗਿਣਤੀ_3, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_ਪਤਾ("glDrawArraysInstanced");
        DrawArraysInstanced(ਬਹੁਲਕ, first, ਗਿਣਤੀ_3, instancecount);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum ਬਹੁਲਕ, GLsizei ਗਿਣਤੀ_3, GLenum type, ਅਚਲ GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_ਪਤਾ("glDrawElementsInstanced");
        DrawElementsInstanced(ਬਹੁਲਕ, ਗਿਣਤੀ_3, type, indices, instancecount);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Tex_ਬਫਰ(GLenum target, GLenum internalformat, GLuint ਬਫਰ)
    {
        Tex_ਬਫਰ = (PFNTEXBUFFERPROC)Int_Get_Proc_ਪਤਾ("glTexBuffer");
        Tex_ਬਫਰ(target, internalformat, ਬਫਰ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Primitive_Restart_ਸੂਚਕਾਂਕ(GLuint ਸੂਚਕਾਂਕ)
    {
        Primitive_Restart_ਸੂਚਕਾਂਕ = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_ਪਤਾ("glPrimitiveRestartIndex");
        Primitive_Restart_ਸੂਚਕਾਂਕ(ਸੂਚਕਾਂਕ);
    }

    // Legacy

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_ਪਤਾ("glEnableClientState");
        EnableClientState(cap);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_ਪਤਾ("glDisableClientState");
        DisableClientState(cap);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸ਼ੀਰਸ਼_ਸੂਚਕ(GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr)
    {
        ਸ਼ੀਰਸ਼_ਸੂਚਕ = (PFNVERTEXPOINTERPROC)Int_Get_Proc_ਪਤਾ("glVertexPointer");
        ਸ਼ੀਰਸ਼_ਸੂਚਕ(size, type, stride, ptr);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਸਾਧਾਰਨ_ਸੂਚਕ(GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr)
    {
        ਸਾਧਾਰਨ_ਸੂਚਕ = (PFNNORMALPOINTERPROC)Int_Get_Proc_ਪਤਾ("glNormalPointer");
        ਸਾਧਾਰਨ_ਸੂਚਕ(type, stride, ptr);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Color_ਸੂਚਕ(GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr)
    {
        Color_ਸੂਚਕ = (PFNCOLORPOINTERPROC)Int_Get_Proc_ਪਤਾ("glColorPointer");
        Color_ਸੂਚਕ(size, type, stride, ptr);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Tex_Coord_ਸੂਚਕ(GLint size, GLenum type, GLsizei stride, ਅਚਲ GLvoid *ptr)
    {
        Tex_Coord_ਸੂਚਕ = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_ਪਤਾ("glTexCoordPointer");
        Tex_Coord_ਸੂਚਕ(size, type, stride, ptr);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_ਪਤਾ("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ(GLenum ਬਹੁਲਕ)
    {
        ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ = (PFNMATRIXMODEPROC)Int_Get_Proc_ਪਤਾ("glMatrixMode");
        ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ(ਬਹੁਲਕ);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ(ਖਾਲੀ)
    {
        ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ = (PFNLOADIDENTITYPROC)Int_Get_Proc_ਪਤਾ("glLoadIdentity");
        ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ();
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Ortho(GLdouble ਖੱਬੇ, GLdouble ਸੱਜੇ, GLdouble bottom, GLdouble ਸਿਖਰ_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_ਪਤਾ("glOrtho");
        Ortho(ਖੱਬੇ, ਸੱਜੇ, bottom, ਸਿਖਰ_2, near_val, far_val);
    }

    ਸਥਿਰ ਖਾਲੀ CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_ਪਤਾ("glColor3d");
        Color3d(red, green, blue);
    }

    ਬਣਤਰ InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            ਸਤਰ_ਚੌੜਾਈ = Switch_ਸਤਰ_ਚੌੜਾਈ;
            ਬਿੰਦੂ_ਮਾਪ = Switch_ਬਿੰਦੂ_ਮਾਪ;
            Polygon_ਬਹੁਲਕ = Switch_Polygon_ਬਹੁਲਕ;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_ਬਫਰ = Switch_Draw_ਬਫਰ;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            Clear_ਡੂੰਘਾਈ = Switch_Clear_ਡੂੰਘਾਈ;
            Stencil_ਨਕਾਬ = Switch_Stencil_ਨਕਾਬ;
            Color_ਨਕਾਬ = Switch_Color_ਨਕਾਬ;
            ਡੂੰਘਾਈ_ਨਕਾਬ = Switch_ਡੂੰਘਾਈ_ਨਕਾਬ;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            ਅੰਤ = Switch_ਅੰਤ;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            ਡੂੰਘਾਈ_Func = Switch_ਡੂੰਘਾਈ_Func;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            Read_ਬਫਰ = Switch_Read_ਬਫਰ;
            ReadPixels = Switch_ReadPixels;
            GetBooleanv = Switch_GetBooleanv;
            GetDoublev = Switch_GetDoublev;
            GetError = Switch_GetError;
            GetFloatv = Switch_GetFloatv;
            GetIntegerv = Switch_GetIntegerv;
            GetString = Switch_GetString;
            GetTexImage = Switch_GetTexImage;
            GetTexParameterfv = Switch_GetTexParameterfv;
            GetTexParameteriv = Switch_GetTexParameteriv;
            GetTexLevelParameterfv = Switch_GetTexLevelParameterfv;
            GetTexLevelParameteriv = Switch_GetTexLevelParameteriv;
            IsEnabled = Switch_IsEnabled;
            ਡੂੰਘਾਈ_Range = Switch_ਡੂੰਘਾਈ_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_ਵਿੱਥ = Switch_Polygon_ਵਿੱਥ;
            CopyTexImage1D = Switch_CopyTexImage1D;
            CopyTexImage2D = Switch_CopyTexImage2D;
            CopyTexSubImage1D = Switch_CopyTexSubImage1D;
            CopyTexSubImage2D = Switch_CopyTexSubImage2D;
            TexSubImage1D = Switch_TexSubImage1D;
            TexSubImage2D = Switch_TexSubImage2D;
            BindTexture = Switch_BindTexture;
            DeleteTextures = Switch_DeleteTextures;
            GenTextures = Switch_GenTextures;
            IsTexture = Switch_IsTexture;
            Indexub = Switch_Indexub;
            Indexubv = Switch_Indexubv;

            // Extension: 1.2
            BlendColor = Switch_BlendColor;
            BlendEquation = Switch_BlendEquation;
            DrawRangeElements = Switch_DrawRangeElements;
            TexSubImage3D = Switch_TexSubImage3D;
            CopyTexSubImage3D = Switch_CopyTexSubImage3D;

            // Extension: 1.3
            ਸਰਗਰਮ_Texture = Switch_ਸਰਗਰਮ_Texture;
            SampleCoverage = Switch_SampleCoverage;
            CompressedTexImage3D = Switch_CompressedTexImage3D;
            CompressedTexImage2D = Switch_CompressedTexImage2D;
            CompressedTexImage1D = Switch_CompressedTexImage1D;
            CompressedTexSubImage3D = Switch_CompressedTexSubImage3D;
            CompressedTexSubImage2D = Switch_CompressedTexSubImage2D;
            CompressedTexSubImage1D = Switch_CompressedTexSubImage1D;
            GetCompressedTexImage = Switch_GetCompressedTexImage;

            // Extension: 1.4
            BlendFuncSeparate = Switch_BlendFuncSeparate;
            MultiDrawArrays = Switch_MultiDrawArrays;
            MultiDrawElements = Switch_MultiDrawElements;
            ਬਿੰਦੂ_Parameterf = Switch_ਬਿੰਦੂ_Parameterf;
            ਬਿੰਦੂ_Parameterfv = Switch_ਬਿੰਦੂ_Parameterfv;
            ਬਿੰਦੂ_Parameteri = Switch_ਬਿੰਦੂ_Parameteri;
            ਬਿੰਦੂ_Parameteriv = Switch_ਬਿੰਦੂ_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            ਸਿਰਾ_Query = Switch_ਸਿਰਾ_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_ਬਫਰ = Switch_Bind_ਬਫਰ;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_ਬਫਰ = Switch_Is_ਬਫਰ;
            ਬਫਰ_ਅੰਕੜੇ = Switch_ਬਫਰ_ਅੰਕੜੇ;
            ਬਫਰ_Sub_ਅੰਕੜੇ = Switch_ਬਫਰ_Sub_ਅੰਕੜੇ;
            Get_ਬਫਰ_Sub_ਅੰਕੜੇ = Switch_Get_ਬਫਰ_Sub_ਅੰਕੜੇ;
            Map_ਬਫਰ = Switch_Map_ਬਫਰ;
            Unmap_ਬਫਰ = Switch_Unmap_ਬਫਰ;
            Get_ਬਫਰ_Parameteriv = Switch_Get_ਬਫਰ_Parameteriv;
            Get_ਬਫਰ_Pointerv = Switch_Get_ਬਫਰ_Pointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            Stencil_ਨਕਾਬ_Separate = Switch_Stencil_ਨਕਾਬ_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_ਪ੍ਰੋਗਰਾਮ = Switch_Create_ਪ੍ਰੋਗਰਾਮ;
            CreateShader = Switch_CreateShader;
            Delete_ਪ੍ਰੋਗਰਾਮ = Switch_Delete_ਪ੍ਰੋਗਰਾਮ;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            Disable_ਸ਼ੀਰਸ਼_Attrib_Array = Switch_Disable_ਸ਼ੀਰਸ਼_Attrib_Array;
            Enable_ਸ਼ੀਰਸ਼_Attrib_Array = Switch_Enable_ਸ਼ੀਰਸ਼_Attrib_Array;
            Get_ਸਰਗਰਮ_Attrib = Switch_Get_ਸਰਗਰਮ_Attrib;
            Get_ਸਰਗਰਮ_ਇਕਸਾਰ = Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_ਪ੍ਰੋਗਰਾਮ_Info_Log = Switch_Get_ਪ੍ਰੋਗਰਾਮ_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_ਸਰੋਤ = Switch_Get_Shader_ਸਰੋਤ;
            Get_ਇਕਸਾਰ_Location = Switch_Get_ਇਕਸਾਰ_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_ਸ਼ੀਰਸ਼_Attribdv = Switch_Get_ਸ਼ੀਰਸ਼_Attribdv;
            Get_ਸ਼ੀਰਸ਼_Attribfv = Switch_Get_ਸ਼ੀਰਸ਼_Attribfv;
            Get_ਸ਼ੀਰਸ਼_Attribiv = Switch_Get_ਸ਼ੀਰਸ਼_Attribiv;
            Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv = Switch_Get_ਸ਼ੀਰਸ਼_Attrib_Pointerv;
            Is_ਪ੍ਰੋਗਰਾਮ = Switch_Is_ਪ੍ਰੋਗਰਾਮ;
            IsShader = Switch_IsShader;
            Link_ਪ੍ਰੋਗਰਾਮ = Switch_Link_ਪ੍ਰੋਗਰਾਮ;
            Shader_ਸਰੋਤ = Switch_Shader_ਸਰੋਤ;
            Use_ਪ੍ਰੋਗਰਾਮ = Switch_Use_ਪ੍ਰੋਗਰਾਮ;
            Uniform1f = Switch_Uniform1f;
            Uniform2f = Switch_Uniform2f;
            Uniform3f = Switch_Uniform3f;
            Uniform4f = Switch_Uniform4f;
            Uniform1i = Switch_Uniform1i;
            Uniform2i = Switch_Uniform2i;
            Uniform3i = Switch_Uniform3i;
            Uniform4i = Switch_Uniform4i;
            Uniform1fv = Switch_Uniform1fv;
            Uniform2fv = Switch_Uniform2fv;
            Uniform3fv = Switch_Uniform3fv;
            Uniform4fv = Switch_Uniform4fv;
            Uniform1iv = Switch_Uniform1iv;
            Uniform2iv = Switch_Uniform2iv;
            Uniform3iv = Switch_Uniform3iv;
            Uniform4iv = Switch_Uniform4iv;
            ਇਕਸਾਰ_Matrix2fv = Switch_ਇਕਸਾਰ_Matrix2fv;
            ਇਕਸਾਰ_Matrix3fv = Switch_ਇਕਸਾਰ_Matrix3fv;
            ਇਕਸਾਰ_Matrix4fv = Switch_ਇਕਸਾਰ_Matrix4fv;
            Validate_ਪ੍ਰੋਗਰਾਮ = Switch_Validate_ਪ੍ਰੋਗਰਾਮ;
            ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ = Switch_ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ;

            // Extension: 2.1
            ਇਕਸਾਰ_Matrix2x3fv = Switch_ਇਕਸਾਰ_Matrix2x3fv;
            ਇਕਸਾਰ_Matrix3x2fv = Switch_ਇਕਸਾਰ_Matrix3x2fv;
            ਇਕਸਾਰ_Matrix2x4fv = Switch_ਇਕਸਾਰ_Matrix2x4fv;
            ਇਕਸਾਰ_Matrix4x2fv = Switch_ਇਕਸਾਰ_Matrix4x2fv;
            ਇਕਸਾਰ_Matrix3x4fv = Switch_ਇਕਸਾਰ_Matrix3x4fv;
            ਇਕਸਾਰ_Matrix4x3fv = Switch_ਇਕਸਾਰ_Matrix4x3fv;

            // Extension: ARB_vertex_array_object
            Bind_ਸ਼ੀਰਸ਼_Array = Switch_Bind_ਸ਼ੀਰਸ਼_Array;
            Delete_ਸ਼ੀਰਸ਼_Arrays = Switch_Delete_ਸ਼ੀਰਸ਼_Arrays;
            Gen_ਸ਼ੀਰਸ਼_Arrays = Switch_Gen_ਸ਼ੀਰਸ਼_Arrays;
            Is_ਸ਼ੀਰਸ਼_Array = Switch_Is_ਸ਼ੀਰਸ਼_Array;

            // Extension: ARB_map_buffer_range
            Map_ਬਫਰ_Range = Switch_Map_ਬਫਰ_Range;
            Flush_Mapped_ਬਫਰ_Range = Switch_Flush_Mapped_ਬਫਰ_Range;

            // Extension: ARB_framebuffer_object
            IsRenderbuffer = Switch_IsRenderbuffer;
            BindRenderbuffer = Switch_BindRenderbuffer;
            DeleteRenderbuffers = Switch_DeleteRenderbuffers;
            GenRenderbuffers = Switch_GenRenderbuffers;
            RenderbufferStorage = Switch_RenderbufferStorage;
            GetRenderbufferParameteriv = Switch_GetRenderbufferParameteriv;
            IsFramebuffer = Switch_IsFramebuffer;
            BindFramebuffer = Switch_BindFramebuffer;
            DeleteFramebuffers = Switch_DeleteFramebuffers;
            GenFramebuffers = Switch_GenFramebuffers;
            Check_Framebuffer_ਸਥਿਤੀ = Switch_Check_Framebuffer_ਸਥਿਤੀ;
            FramebufferTexture1D = Switch_FramebufferTexture1D;
            FramebufferTexture2D = Switch_FramebufferTexture2D;
            FramebufferTexture3D = Switch_FramebufferTexture3D;
            FramebufferRenderbuffer = Switch_FramebufferRenderbuffer;
            GetFramebufferAttachmentParameteriv = Switch_GetFramebufferAttachmentParameteriv;
            GenerateMipmap = Switch_GenerateMipmap;
            BlitFramebuffer = Switch_BlitFramebuffer;
            RenderbufferStorageMultisample = Switch_RenderbufferStorageMultisample;
            FramebufferTextureLayer = Switch_FramebufferTextureLayer;

            // Extension: 3.0
            ColorMaski = Switch_ColorMaski;
            GetBooleani_v = Switch_GetBooleani_v;
            GetIntegeri_v = Switch_GetIntegeri_v;
            Enablei = Switch_Enablei;
            Disablei = Switch_Disablei;
            IsEnabledi = Switch_IsEnabledi;
            BeginTransformFeedback = Switch_BeginTransformFeedback;
            ਸਿਰਾ_Transform_Feedback = Switch_ਸਿਰਾ_Transform_Feedback;
            Bind_ਬਫਰ_Range = Switch_Bind_ਬਫਰ_Range;
            Bind_ਬਫਰ_ਅਧਾਰ = Switch_Bind_ਬਫਰ_ਅਧਾਰ;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            ਸਿਰਾ_Conditional_Render = Switch_ਸਿਰਾ_Conditional_Render;
            ਸ਼ੀਰਸ਼_Attrib_IPointer = Switch_ਸ਼ੀਰਸ਼_Attrib_IPointer;
            Get_ਸ਼ੀਰਸ਼_Attrib_Iiv = Switch_Get_ਸ਼ੀਰਸ਼_Attrib_Iiv;
            Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv = Switch_Get_ਸ਼ੀਰਸ਼_Attrib_Iuiv;
            ਸ਼ੀਰਸ਼_Attrib_I1i = Switch_ਸ਼ੀਰਸ਼_Attrib_I1i;
            ਸ਼ੀਰਸ਼_Attrib_I2i = Switch_ਸ਼ੀਰਸ਼_Attrib_I2i;
            ਸ਼ੀਰਸ਼_Attrib_I3i = Switch_ਸ਼ੀਰਸ਼_Attrib_I3i;
            ਸ਼ੀਰਸ਼_Attrib_I4i = Switch_ਸ਼ੀਰਸ਼_Attrib_I4i;
            ਸ਼ੀਰਸ਼_Attrib_I1ui = Switch_ਸ਼ੀਰਸ਼_Attrib_I1ui;
            ਸ਼ੀਰਸ਼_Attrib_I2ui = Switch_ਸ਼ੀਰਸ਼_Attrib_I2ui;
            ਸ਼ੀਰਸ਼_Attrib_I3ui = Switch_ਸ਼ੀਰਸ਼_Attrib_I3ui;
            ਸ਼ੀਰਸ਼_Attrib_I4ui = Switch_ਸ਼ੀਰਸ਼_Attrib_I4ui;
            ਸ਼ੀਰਸ਼_Attrib_I1iv = Switch_ਸ਼ੀਰਸ਼_Attrib_I1iv;
            ਸ਼ੀਰਸ਼_Attrib_I2iv = Switch_ਸ਼ੀਰਸ਼_Attrib_I2iv;
            ਸ਼ੀਰਸ਼_Attrib_I3iv = Switch_ਸ਼ੀਰਸ਼_Attrib_I3iv;
            ਸ਼ੀਰਸ਼_Attrib_I4iv = Switch_ਸ਼ੀਰਸ਼_Attrib_I4iv;
            ਸ਼ੀਰਸ਼_Attrib_I1uiv = Switch_ਸ਼ੀਰਸ਼_Attrib_I1uiv;
            ਸ਼ੀਰਸ਼_Attrib_I2uiv = Switch_ਸ਼ੀਰਸ਼_Attrib_I2uiv;
            ਸ਼ੀਰਸ਼_Attrib_I3uiv = Switch_ਸ਼ੀਰਸ਼_Attrib_I3uiv;
            ਸ਼ੀਰਸ਼_Attrib_I4uiv = Switch_ਸ਼ੀਰਸ਼_Attrib_I4uiv;
            ਸ਼ੀਰਸ਼_Attrib_I4bv = Switch_ਸ਼ੀਰਸ਼_Attrib_I4bv;
            ਸ਼ੀਰਸ਼_Attrib_I4sv = Switch_ਸ਼ੀਰਸ਼_Attrib_I4sv;
            ਸ਼ੀਰਸ਼_Attrib_I4ubv = Switch_ਸ਼ੀਰਸ਼_Attrib_I4ubv;
            ਸ਼ੀਰਸ਼_Attrib_I4usv = Switch_ਸ਼ੀਰਸ਼_Attrib_I4usv;
            GetUniformuiv = Switch_GetUniformuiv;
            Bind_Frag_ਅੰਕੜੇ_Location = Switch_Bind_Frag_ਅੰਕੜੇ_Location;
            Get_Frag_ਅੰਕੜੇ_Location = Switch_Get_Frag_ਅੰਕੜੇ_Location;
            Uniform1ui = Switch_Uniform1ui;
            Uniform2ui = Switch_Uniform2ui;
            Uniform3ui = Switch_Uniform3ui;
            Uniform4ui = Switch_Uniform4ui;
            Uniform1uiv = Switch_Uniform1uiv;
            Uniform2uiv = Switch_Uniform2uiv;
            Uniform3uiv = Switch_Uniform3uiv;
            Uniform4uiv = Switch_Uniform4uiv;
            TexParameterIiv = Switch_TexParameterIiv;
            TexParameterIuiv = Switch_TexParameterIuiv;
            GetTexParameterIiv = Switch_GetTexParameterIiv;
            GetTexParameterIuiv = Switch_GetTexParameterIuiv;
            ClearBufferiv = Switch_ClearBufferiv;
            ClearBufferuiv = Switch_ClearBufferuiv;
            ClearBufferfv = Switch_ClearBufferfv;
            ClearBufferfi = Switch_ClearBufferfi;
            GetStringi = Switch_GetStringi;

            // Extension: ARB_uniform_buffer_object
            Get_ਇਕਸਾਰ_Indices = Switch_Get_ਇਕਸਾਰ_Indices;
            Get_ਸਰਗਰਮ_Uniformsiv = Switch_Get_ਸਰਗਰਮ_Uniformsiv;
            Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name = Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Name;
            Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ = Switch_Get_ਇਕਸਾਰ_ਖੰਡ_ਸੂਚਕਾਂਕ;
            Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv = Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ_Blockiv;
            Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name = Switch_Get_ਸਰਗਰਮ_ਇਕਸਾਰ_ਖੰਡ_Name;
            ਇਕਸਾਰ_ਖੰਡ_Binding = Switch_ਇਕਸਾਰ_ਖੰਡ_Binding;

            // Extension: ARB_copy_buffer
            Copy_ਬਫਰ_Sub_ਅੰਕੜੇ = Switch_Copy_ਬਫਰ_Sub_ਅੰਕੜੇ;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_ਬਫਰ = Switch_Tex_ਬਫਰ;
            Primitive_Restart_ਸੂਚਕਾਂਕ = Switch_Primitive_Restart_ਸੂਚਕਾਂਕ;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            ਸ਼ੀਰਸ਼_ਸੂਚਕ = Switch_ਸ਼ੀਰਸ਼_ਸੂਚਕ;
            ਸਾਧਾਰਨ_ਸੂਚਕ = Switch_ਸਾਧਾਰਨ_ਸੂਚਕ;
            Color_ਸੂਚਕ = Switch_Color_ਸੂਚਕ;
            Tex_Coord_ਸੂਚਕ = Switch_Tex_Coord_ਸੂਚਕ;
            TexEnvi = Switch_TexEnvi;
            ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ = Switch_ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ;
            ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ = Switch_ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
