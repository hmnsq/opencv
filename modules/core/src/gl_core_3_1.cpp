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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "gl_core_3_1.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL

    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __APPLE__
        #ಅನ್ತರ್ಭಾವ <dlfcn.h>

        ಸ್ಥಿರ ಶೂನ್ಯ* Apple_GLGet_Proc_ವಿಳಾಸ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name)
        {
            ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
            ಸ್ಥಿರ ಶೂನ್ಯ * handle = ಖಾಲಿ;
            ಆದರೆ (!handle)
            {
                ಆದರೆ (!initialized)
                {
                    initialized = ನಿಜ;
                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ ಮಾರ್ಗ = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(ಮಾರ್ಗ, RTLD_LAZY | RTLD_GLOBAL);
                }
                ಆದರೆ (!handle)
                    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
            }
            ಹಿಂತಿರುಗಿ dlsym(handle, name);
        }
    #ಆದರೆ_ಅಂತ್ಯ // __APPLE__

    #ಆದರೆ defined(__sgi) || defined (__sun)
        #ಅನ್ತರ್ಭಾವ <dlfcn.h>
        #ಅನ್ತರ್ಭಾವ <stdio.h>

        ಸ್ಥಿರ ಶೂನ್ಯ* Sun_Get_Proc_ವಿಳಾಸ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name)
        {
            ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ* (func_t*)(ಸ್ಥಿರಾಂಕ GLubyte*);

            ಸ್ಥಿರ ಶೂನ್ಯ* h = 0;
            ಸ್ಥಿರ func_t gpa = 0;

            ಆದರೆ (!h)
            {
                h = dlopen(ಖಾಲಿ, RTLD_LAZY | RTLD_LOCAL);
                ಆದರೆ (!h)
                    ಹಿಂತಿರುಗಿ 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            ಹಿಂತಿರುಗಿ gpa ? gpa((ಸ್ಥಿರಾಂಕ GLubyte*) name) : dlsym(h, name);
        }
    #ಆದರೆ_ಅಂತ್ಯ // __sgi || __sun

    #ಆದರೆ defined(_WIN32)
        #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
            #ಸೂಚನೆ warning(disable: 4055)
            #ಸೂಚನೆ warning(disable: 4054)
        #ಆದರೆ_ಅಂತ್ಯ

        ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ Test_ಸೂಚಕ(ಸ್ಥಿರಾಂಕ PROC pTest)
        {
            ಆದರೆ(!pTest)
                ಹಿಂತಿರುಗಿ 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            ಆದರೆ (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                ಹಿಂತಿರುಗಿ 0;

            ಹಿಂತಿರುಗಿ 1;
        }

        ಸ್ಥಿರ PROC Win_Get_Proc_ವಿಳಾಸ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name)
        {
            PROC pFunc = wgl_Get_Proc_ವಿಳಾಸ((LPCSTR) name);
            ಆದರೆ (Test_ಸೂಚಕ(pFunc))
                ಹಿಂತಿರುಗಿ pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            ಹಿಂತಿರುಗಿ (PROC) Get_Proc_ವಿಳಾಸ(glMod, (LPCSTR) name);
        }
    #ಆದರೆ_ಅಂತ್ಯ // _WIN32

    #ಆದರೆ defined(_WIN32)
        #ವ್ಯಾಖ್ಯಾನಿಸು CV_GL_GET_PROC_ವಿಳಾಸ(name) Win_Get_Proc_ವಿಳಾಸ(name)
    #ಅಥವಾಆದರೆ defined(__APPLE__)
        #ವ್ಯಾಖ್ಯಾನಿಸು CV_GL_GET_PROC_ವಿಳಾಸ(name) Apple_GLGet_Proc_ವಿಳಾಸ(name)
    #ಅಥವಾಆದರೆ defined(__sgi) || defined(__sun)
        #ವ್ಯಾಖ್ಯಾನಿಸು CV_GL_GET_PROC_ವಿಳಾಸ(name) Sun_Get_Proc_ವಿಳಾಸ(name)
    #ಇಲ್ಲದಿದ್ದರೆ // GLX
        #ಅನ್ತರ್ಭಾವ <GL/glx.h>

        #ವ್ಯಾಖ್ಯಾನಿಸು CV_GL_GET_PROC_ವಿಳಾಸ(name) gl_XGet_Proc_ವಿಳಾಸ_ARB((ಸ್ಥಿರಾಂಕ GLubyte*) name)
    #ಆದರೆ_ಅಂತ್ಯ

    ಸ್ಥಿರ ಶೂನ್ಯ* Int_Get_Proc_ವಿಳಾಸ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name)
    {
        ಶೂನ್ಯ* func =  (ಶೂನ್ಯ*) CV_GL_GET_PROC_ವಿಳಾಸ(name);
        ಆದರೆ (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        ಹಿಂತಿರುಗಿ func;
    }
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ defined(_MSC_VER)
    #ಸೂಚನೆ warning(disable : 4702)  // unreachable code
#ಆದರೆ_ಅಂತ್ಯ
    ಸ್ಥಿರ ಶೂನ್ಯ* Int_Get_Proc_ವಿಳಾಸ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸ್ಥಿರಾಂಕ GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(ಸ್ಥಿರಾಂಕ GLubyte *);

    // Extension: 1.2
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , ಸ್ಥಿರಾಂಕ GLint *, ಸ್ಥಿರಾಂಕ GLsizei *, GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , ಸ್ಥಿರಾಂಕ GLsizei *, GLenum , ಸ್ಥಿರಾಂಕ GLvoid* ಸ್ಥಿರಾಂಕ *, GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , ಸ್ಥಿರಾಂಕ GLint *);

    // Extension: 1.5
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , ಸ್ಥಿರಾಂಕ GLvoid *, GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLenum *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , ಸ್ಥಿರಾಂಕ GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *, ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);

    // Extension: 2.1
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , ಸ್ಥಿರಾಂಕ GLfloat *);

    // Extension: ARB_vertex_array_object
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *, GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLbyte *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLshort *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLubyte *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLushort *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , ಸ್ಥಿರಾಂಕ GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , ಸ್ಥಿರಾಂಕ GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , ಸ್ಥಿರಾಂಕ GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , ಸ್ಥಿರಾಂಕ GLfloat *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸ್ಥಿರಾಂಕ GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *, GLuint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , ಸ್ಥಿರಾಂಕ GLuint *, GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , ಸ್ಥಿರಾಂಕ GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , ಸ್ಥಿರಾಂಕ GLvoid *, GLsizei );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , ಸ್ಥಿರಾಂಕ GLvoid *);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(ಶೂನ್ಯ);
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC ಸಾಲು_ಅಗಲ;
    PFNPOINTSIZEPROC ಬಿಂದು_ಗಾತ್ರ;
    PFNPOLYGONMODEPROC Polygon_ಬಹುಲಕ;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_ಬಫರ್;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC Clear_ಆಳ;
    PFNSTENCILMASKPROC Stencil_ಮುಖವಾಡ;
    PFNCOLORMASKPROC Color_ಮುಖವಾಡ;
    PFNDEPTHMASKPROC ಆಳ_ಮುಖವಾಡ;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC ಮುಕ್ತಾಯ;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC ಆಳ_Func;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC Read_ಬಫರ್;
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
    PFNDEPTHRANGEPROC ಆಳ_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_ಸ್ಥಾನಾಂತರ;
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
    PFNACTIVETEXTUREPROC ಸಕ್ರಿಯ_Texture;
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
    PFNPOINTPARAMETERFPROC ಬಿಂದು_Parameterf;
    PFNPOINTPARAMETERFVPROC ಬಿಂದು_Parameterfv;
    PFNPOINTPARAMETERIPROC ಬಿಂದು_Parameteri;
    PFNPOINTPARAMETERIVPROC ಬಿಂದು_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC ಕೊನೆ_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_ಬಫರ್;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_ಬಫರ್;
    PFNBUFFERDATAPROC ಬಫರ್_ದತ್ತಾಂಶ;
    PFNBUFFERSUBDATAPROC ಬಫರ್_Sub_ದತ್ತಾಂಶ;
    PFNGETBUFFERSUBDATAPROC Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ;
    PFNMAPBUFFERPROC Map_ಬಫರ್;
    PFNUNMAPBUFFERPROC Unmap_ಬಫರ್;
    PFNGETBUFFERPARAMETERIVPROC Get_ಬಫರ್_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_ಬಫರ್_Pointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC Stencil_ಮುಖವಾಡ_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_ಕಾರ್ಯಕ್ರಮ;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_ಕಾರ್ಯಕ್ರಮ;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC Disable_ಶೃಂಗ_Attrib_Array;
    PFNENABLEVERTEXATTRIBARRAYPROC Enable_ಶೃಂಗ_Attrib_Array;
    PFNGETACTIVEATTRIBPROC Get_ಸಕ್ರಿಯ_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_ಸಕ್ರಿಯ_ಏಕರೂಪ;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_ಕಾರ್ಯಕ್ರಮ_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_ಮೂಲ;
    PFNGETUNIFORMLOCATIONPROC Get_ಏಕರೂಪ_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_ಶೃಂಗ_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_ಶೃಂಗ_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_ಶೃಂಗ_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_ಶೃಂಗ_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_ಕಾರ್ಯಕ್ರಮ;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_ಕಾರ್ಯಕ್ರಮ;
    PFNSHADERSOURCEPROC Shader_ಮೂಲ;
    PFNUSEPROGRAMPROC Use_ಕಾರ್ಯಕ್ರಮ;
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
    PFNUNIFORMMATRIX2FVPROC ಏಕರೂಪ_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC ಏಕರೂಪ_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC ಏಕರೂಪ_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_ಕಾರ್ಯಕ್ರಮ;
    PFNVERTEXATTRIBPOINTERPROC ಶೃಂಗ_Attrib_ಸೂಚಕ;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC ಏಕರೂಪ_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC ಏಕರೂಪ_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC ಏಕರೂಪ_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC ಏಕರೂಪ_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC ಏಕರೂಪ_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC ಏಕರೂಪ_Matrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC Bind_ಶೃಂಗ_Array;
    PFNDELETEVERTEXARRAYSPROC Delete_ಶೃಂಗ_Arrays;
    PFNGENVERTEXARRAYSPROC Gen_ಶೃಂಗ_Arrays;
    PFNISVERTEXARRAYPROC Is_ಶೃಂಗ_Array;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC Map_ಬಫರ್_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_ಬಫರ್_Range;

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
    PFNCHECKFRAMEBUFFERSTATUSPROC Check_Framebuffer_ಸ್ಥಿತಿ;
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
    PFNENDTRANSFORMFEEDBACKPROC ಕೊನೆ_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_ಬಫರ್_Range;
    PFNBINDBUFFERBASEPROC Bind_ಬಫರ್_ತಳ;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC ಕೊನೆ_Conditional_Render;
    PFNVERTEXATTRIBIPOINTERPROC ಶೃಂಗ_Attrib_IPointer;
    PFNGETVERTEXATTRIBIIVPROC Get_ಶೃಂಗ_Attrib_Iiv;
    PFNGETVERTEXATTRIBIUIVPROC Get_ಶೃಂಗ_Attrib_Iuiv;
    PFNVERTEXATTRIBI1IPROC ಶೃಂಗ_Attrib_I1i;
    PFNVERTEXATTRIBI2IPROC ಶೃಂಗ_Attrib_I2i;
    PFNVERTEXATTRIBI3IPROC ಶೃಂಗ_Attrib_I3i;
    PFNVERTEXATTRIBI4IPROC ಶೃಂಗ_Attrib_I4i;
    PFNVERTEXATTRIBI1UIPROC ಶೃಂಗ_Attrib_I1ui;
    PFNVERTEXATTRIBI2UIPROC ಶೃಂಗ_Attrib_I2ui;
    PFNVERTEXATTRIBI3UIPROC ಶೃಂಗ_Attrib_I3ui;
    PFNVERTEXATTRIBI4UIPROC ಶೃಂಗ_Attrib_I4ui;
    PFNVERTEXATTRIBI1IVPROC ಶೃಂಗ_Attrib_I1iv;
    PFNVERTEXATTRIBI2IVPROC ಶೃಂಗ_Attrib_I2iv;
    PFNVERTEXATTRIBI3IVPROC ಶೃಂಗ_Attrib_I3iv;
    PFNVERTEXATTRIBI4IVPROC ಶೃಂಗ_Attrib_I4iv;
    PFNVERTEXATTRIBI1UIVPROC ಶೃಂಗ_Attrib_I1uiv;
    PFNVERTEXATTRIBI2UIVPROC ಶೃಂಗ_Attrib_I2uiv;
    PFNVERTEXATTRIBI3UIVPROC ಶೃಂಗ_Attrib_I3uiv;
    PFNVERTEXATTRIBI4UIVPROC ಶೃಂಗ_Attrib_I4uiv;
    PFNVERTEXATTRIBI4BVPROC ಶೃಂಗ_Attrib_I4bv;
    PFNVERTEXATTRIBI4SVPROC ಶೃಂಗ_Attrib_I4sv;
    PFNVERTEXATTRIBI4UBVPROC ಶೃಂಗ_Attrib_I4ubv;
    PFNVERTEXATTRIBI4USVPROC ಶೃಂಗ_Attrib_I4usv;
    PFNGETUNIFORMUIVPROC GetUniformuiv;
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_ದತ್ತಾಂಶ_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_ದತ್ತಾಂಶ_Location;
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
    PFNGETUNIFORMINDICESPROC Get_ಏಕರೂಪ_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_ಸಕ್ರಿಯ_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name;
    PFNUNIFORMBLOCKBINDINGPROC ಏಕರೂಪ_ಖಂಡ_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_ಬಫರ್;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_ಸೂಚ್ಯಂಕ;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC ಶೃಂಗ_ಸೂಚಕ;
    PFNNORMALPOINTERPROC ಸಾಮಾನ್ಯ_ಸೂಚಕ;
    PFNCOLORPOINTERPROC Color_ಸೂಚಕ;
    PFNTEXCOORDPOINTERPROC Tex_Coord_ಸೂಚಕ;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC ಮಾತೃಕೆ_ಬಹುಲಕ;
    PFNLOADIDENTITYPROC ತುಂಬು_ತಟಸ್ಥ;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CullFace(GLenum ಬಹುಲಕ)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_ವಿಳಾಸ("glCullFace");
        CullFace(ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_FrontFace(GLenum ಬಹುಲಕ)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_ವಿಳಾಸ("glFrontFace");
        FrontFace(ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum ಬಹುಲಕ)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_ವಿಳಾಸ("glHint");
        Hint(target, ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಸಾಲು_ಅಗಲ(GLfloat ಅಗಲ)
    {
        ಸಾಲು_ಅಗಲ = (PFNLINEWIDTHPROC)Int_Get_Proc_ವಿಳಾಸ("glLineWidth");
        ಸಾಲು_ಅಗಲ(ಅಗಲ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಿಂದು_ಗಾತ್ರ(GLfloat size)
    {
        ಬಿಂದು_ಗಾತ್ರ = (PFNPOINTSIZEPROC)Int_Get_Proc_ವಿಳಾಸ("glPointSize");
        ಬಿಂದು_ಗಾತ್ರ(size);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Polygon_ಬಹುಲಕ(GLenum face, GLenum ಬಹುಲಕ)
    {
        Polygon_ಬಹುಲಕ = (PFNPOLYGONMODEPROC)Int_Get_Proc_ವಿಳಾಸ("glPolygonMode");
        Polygon_ಬಹುಲಕ(face, ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_ವಿಳಾಸ("glScissor");
        Scissor(x, y, ಅಗಲ, ಎತ್ತರ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_ವಿಳಾಸ("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_ವಿಳಾಸ("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_ವಿಳಾಸ("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei ಅಗಲ, GLint border, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glTexImage1D");
        TexImage1D(target, level, internalformat, ಅಗಲ, border, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLint border, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glTexImage2D");
        TexImage2D(target, level, internalformat, ಅಗಲ, ಎತ್ತರ, border, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Draw_ಬಫರ್(GLenum ಬಹುಲಕ)
    {
        Draw_ಬಫರ್ = (PFNDRAWBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawBuffer");
        Draw_ಬಫರ್(ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Clear(GLbitfield ಮುಖವಾಡ)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_ವಿಳಾಸ("glClear");
        Clear(ಮುಖವಾಡ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_ವಿಳಾಸ("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_ವಿಳಾಸ("glClearStencil");
        ClearStencil(s);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Clear_ಆಳ(GLdouble ಆಳ)
    {
        Clear_ಆಳ = (PFNCLEARDEPTHPROC)Int_Get_Proc_ವಿಳಾಸ("glClearDepth");
        Clear_ಆಳ(ಆಳ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Stencil_ಮುಖವಾಡ(GLuint ಮುಖವಾಡ)
    {
        Stencil_ಮುಖವಾಡ = (PFNSTENCILMASKPROC)Int_Get_Proc_ವಿಳಾಸ("glStencilMask");
        Stencil_ಮುಖವಾಡ(ಮುಖವಾಡ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Color_ಮುಖವಾಡ(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_ಮುಖವಾಡ = (PFNCOLORMASKPROC)Int_Get_Proc_ವಿಳಾಸ("glColorMask");
        Color_ಮುಖವಾಡ(red, green, blue, alpha);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಆಳ_ಮುಖವಾಡ(GLboolean flag)
    {
        ಆಳ_ಮುಖವಾಡ = (PFNDEPTHMASKPROC)Int_Get_Proc_ವಿಳಾಸ("glDepthMask");
        ಆಳ_ಮುಖವಾಡ(flag);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_ವಿಳಾಸ("glDisable");
        Disable(cap);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_ವಿಳಾಸ("glEnable");
        Enable(cap);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಮುಕ್ತಾಯ()
    {
        ಮುಕ್ತಾಯ = (PFNFINISHPROC)Int_Get_Proc_ವಿಳಾಸ("glFinish");
        ಮುಕ್ತಾಯ();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_ವಿಳಾಸ("glFlush");
        Flush();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_ವಿಳಾಸ("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_ವಿಳಾಸ("glLogicOp");
        LogicOp(opcode);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint ಮುಖವಾಡ)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_ವಿಳಾಸ("glStencilFunc");
        StencilFunc(func, ref, ಮುಖವಾಡ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_ವಿಳಾಸ("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಆಳ_Func(GLenum func)
    {
        ಆಳ_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_ವಿಳಾಸ("glDepthFunc");
        ಆಳ_Func(func);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_ವಿಳಾಸ("glPixelStoref");
        PixelStoref(pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_ವಿಳಾಸ("glPixelStorei");
        PixelStorei(pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Read_ಬಫರ್(GLenum ಬಹುಲಕ)
    {
        Read_ಬಫರ್ = (PFNREADBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glReadBuffer");
        Read_ಬಫರ್(ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_ವಿಳಾಸ("glReadPixels");
        ReadPixels(x, y, ಅಗಲ, ಎತ್ತರ, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetDoublev");
        GetDoublev(pname, params);
    }

    ಸ್ಥಿರ GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_ವಿಳಾಸ("glGetError");
        ಹಿಂತಿರುಗಿ GetError();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetFloatv");
        GetFloatv(pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_ವಿಳಾಸ("glGetString");
        ಹಿಂತಿರುಗಿ GetString(name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_ವಿಳಾಸ("glIsEnabled");
        ಹಿಂತಿರುಗಿ IsEnabled(cap);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಆಳ_Range(GLdouble ren_near, GLdouble ren_far)
    {
        ಆಳ_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_ವಿಳಾಸ("glDepthRange");
        ಆಳ_Range(ren_near, ren_far);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_ವಿಳಾಸ("glViewport");
        Viewport(x, y, ಅಗಲ, ಎತ್ತರ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DrawArrays(GLenum ಬಹುಲಕ, GLint first, GLsizei ಎಣಿಕೆ_3)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawArrays");
        DrawArrays(ಬಹುಲಕ, first, ಎಣಿಕೆ_3);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DrawElements(GLenum ಬಹುಲಕ, GLsizei ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawElements");
        DrawElements(ಬಹುಲಕ, ಎಣಿಕೆ_3, type, indices);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetPointerv");
        GetPointerv(pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Polygon_ಸ್ಥಾನಾಂತರ(GLfloat ಗುಣಕ, GLfloat units)
    {
        Polygon_ಸ್ಥಾನಾಂತರ = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_ವಿಳಾಸ("glPolygonOffset");
        Polygon_ಸ್ಥಾನಾಂತರ(ಗುಣಕ, units);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ಅಗಲ, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, ಅಗಲ, border);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, ಅಗಲ, ಎತ್ತರ, border);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei ಅಗಲ)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, ಅಗಲ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, ಅಗಲ, ಎತ್ತರ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei ಅಗಲ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, ಅಗಲ, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, ಅಗಲ, ಎತ್ತರ, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_ವಿಳಾಸ("glBindTexture");
        BindTexture(target, texture);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_ವಿಳಾಸ("glGenTextures");
        GenTextures(n, textures);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_ವಿಳಾಸ("glIsTexture");
        ಹಿಂತಿರುಗಿ IsTexture(texture);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_ವಿಳಾಸ("glIndexub");
        Indexub(c);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Indexubv(ಸ್ಥಿರಾಂಕ GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_ವಿಳಾಸ("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_ವಿಳಾಸ("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BlendEquation(GLenum ಬಹುಲಕ)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_ವಿಳಾಸ("glBlendEquation");
        BlendEquation(ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum ಬಹುಲಕ, GLuint ಆರಂಭ, GLuint end, GLsizei ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawRangeElements");
        DrawRangeElements(ಬಹುಲಕ, ಆರಂಭ, end, ಎಣಿಕೆ_3, type, indices);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLsizei ಆಳ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_ವಿಳಾಸ("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, ಅಗಲ, ಎತ್ತರ, ಆಳ, format, type, pixels);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_ವಿಳಾಸ("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, ಅಗಲ, ಎತ್ತರ);
    }

    // Extension: 1.3

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಸಕ್ರಿಯ_Texture(GLenum texture)
    {
        ಸಕ್ರಿಯ_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_ವಿಳಾಸ("glActiveTexture");
        ಸಕ್ರಿಯ_Texture(texture);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_ವಿಳಾಸ("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLsizei ಆಳ, GLint border, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_ವಿಳಾಸ("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, ಅಗಲ, ಎತ್ತರ, ಆಳ, border, image_ಗಾತ್ರ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLint border, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, ಅಗಲ, ಎತ್ತರ, border, image_ಗಾತ್ರ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei ಅಗಲ, GLint border, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, ಅಗಲ, border, image_ಗಾತ್ರ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLsizei ಆಳ, GLenum format, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_ವಿಳಾಸ("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, ಅಗಲ, ಎತ್ತರ, ಆಳ, format, image_ಗಾತ್ರ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, ಅಗಲ, ಎತ್ತರ, format, image_ಗಾತ್ರ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei ಅಗಲ, GLenum format, GLsizei image_ಗಾತ್ರ, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, ಅಗಲ, format, image_ಗಾತ್ರ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_ವಿಳಾಸ("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_ವಿಳಾಸ("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ GLint *first, ಸ್ಥಿರಾಂಕ GLsizei *ಎಣಿಕೆ_3, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_ವಿಳಾಸ("glMultiDrawArrays");
        MultiDrawArrays(ಬಹುಲಕ, first, ಎಣಿಕೆ_3, drawcount);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum ಬಹುಲಕ, ಸ್ಥಿರಾಂಕ GLsizei *ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid* ಸ್ಥಿರಾಂಕ *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_ವಿಳಾಸ("glMultiDrawElements");
        MultiDrawElements(ಬಹುಲಕ, ಎಣಿಕೆ_3, type, indices, drawcount);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಿಂದು_Parameterf(GLenum pname, GLfloat param)
    {
        ಬಿಂದು_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_ವಿಳಾಸ("glPointParameterf");
        ಬಿಂದು_Parameterf(pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಿಂದು_Parameterfv(GLenum pname, ಸ್ಥಿರಾಂಕ GLfloat *params)
    {
        ಬಿಂದು_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_ವಿಳಾಸ("glPointParameterfv");
        ಬಿಂದು_Parameterfv(pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಿಂದು_Parameteri(GLenum pname, GLint param)
    {
        ಬಿಂದು_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_ವಿಳಾಸ("glPointParameteri");
        ಬಿಂದು_Parameteri(pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಿಂದು_Parameteriv(GLenum pname, ಸ್ಥಿರಾಂಕ GLint *params)
    {
        ಬಿಂದು_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glPointParameteriv");
        ಬಿಂದು_Parameteriv(pname, params);
    }

    // Extension: 1.5

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_ವಿಳಾಸ("glGenQueries");
        GenQueries(n, ids);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_ವಿಳಾಸ("glIsQuery");
        ಹಿಂತಿರುಗಿ IsQuery(id);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_ವಿಳಾಸ("glBeginQuery");
        BeginQuery(target, id);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಕೊನೆ_Query(GLenum target)
    {
        ಕೊನೆ_Query = (PFNENDQUERYPROC)Int_Get_Proc_ವಿಳಾಸ("glEndQuery");
        ಕೊನೆ_Query(target);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Bind_ಬಫರ್(GLenum target, GLuint ಬಫರ್)
    {
        Bind_ಬಫರ್ = (PFNBINDBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glBindBuffer");
        Bind_ಬಫರ್(target, ಬಫರ್);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glGenBuffers");
        GenBuffers(n, buffers);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_Is_ಬಫರ್(GLuint ಬಫರ್)
    {
        Is_ಬಫರ್ = (PFNISBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glIsBuffer");
        ಹಿಂತಿರುಗಿ Is_ಬಫರ್(ಬಫರ್);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಫರ್_ದತ್ತಾಂಶ(GLenum target, GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ, GLenum usage)
    {
        ಬಫರ್_ದತ್ತಾಂಶ = (PFNBUFFERDATAPROC)Int_Get_Proc_ವಿಳಾಸ("glBufferData");
        ಬಫರ್_ದತ್ತಾಂಶ(target, size, ದತ್ತಾಂಶ, usage);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಬಫರ್_Sub_ದತ್ತಾಂಶ(GLenum target, GLintptr offset, GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid *ದತ್ತಾಂಶ)
    {
        ಬಫರ್_Sub_ದತ್ತಾಂಶ = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_ವಿಳಾಸ("glBufferSubData");
        ಬಫರ್_Sub_ದತ್ತಾಂಶ(target, offset, size, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *ದತ್ತಾಂಶ)
    {
        Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_ವಿಳಾಸ("glGetBufferSubData");
        Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ(target, offset, size, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ GLvoid* CODEGEN_FUNCPTR Switch_Map_ಬಫರ್(GLenum target, GLenum access)
    {
        Map_ಬಫರ್ = (PFNMAPBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glMapBuffer");
        ಹಿಂತಿರುಗಿ Map_ಬಫರ್(target, access);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_Unmap_ಬಫರ್(GLenum target)
    {
        Unmap_ಬಫರ್ = (PFNUNMAPBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glUnmapBuffer");
        ಹಿಂತಿರುಗಿ Unmap_ಬಫರ್(target);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಬಫರ್_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_ಬಫರ್_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetBufferParameteriv");
        Get_ಬಫರ್_Parameteriv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಬಫರ್_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_ಬಫರ್_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetBufferPointerv");
        Get_ಬಫರ್_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum ಬಹುಲಕ_RGB, GLenum ಬಹುಲಕ_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_ವಿಳಾಸ("glBlendEquationSeparate");
        BlendEquationSeparate(ಬಹುಲಕ_RGB, ಬಹುಲಕ_Alpha);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ಸ್ಥಿರಾಂಕ GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_ವಿಳಾಸ("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint ಮುಖವಾಡ)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_ವಿಳಾಸ("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, ಮುಖವಾಡ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Stencil_ಮುಖವಾಡ_Separate(GLenum face, GLuint ಮುಖವಾಡ)
    {
        Stencil_ಮುಖವಾಡ_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_ವಿಳಾಸ("glStencilMaskSeparate");
        Stencil_ಮುಖವಾಡ_Separate(face, ಮುಖವಾಡ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_AttachShader(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_ವಿಳಾಸ("glAttachShader");
        AttachShader(ಕಾರ್ಯಕ್ರಮ, shader);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_ವಿಳಾಸ("glBindAttribLocation");
        BindAttribLocation(ಕಾರ್ಯಕ್ರಮ, ಸೂಚ್ಯಂಕ, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_ವಿಳಾಸ("glCompileShader");
        CompileShader(shader);
    }

    ಸ್ಥಿರ GLuint CODEGEN_FUNCPTR Switch_Create_ಕಾರ್ಯಕ್ರಮ()
    {
        Create_ಕಾರ್ಯಕ್ರಮ = (PFNCREATEPROGRAMPROC)Int_Get_Proc_ವಿಳಾಸ("glCreateProgram");
        ಹಿಂತಿರುಗಿ Create_ಕಾರ್ಯಕ್ರಮ();
    }

    ಸ್ಥಿರ GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_ವಿಳಾಸ("glCreateShader");
        ಹಿಂತಿರುಗಿ CreateShader(type);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Delete_ಕಾರ್ಯಕ್ರಮ(GLuint ಕಾರ್ಯಕ್ರಮ)
    {
        Delete_ಕಾರ್ಯಕ್ರಮ = (PFNDELETEPROGRAMPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteProgram");
        Delete_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteShader");
        DeleteShader(shader);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DetachShader(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_ವಿಳಾಸ("glDetachShader");
        DetachShader(ಕಾರ್ಯಕ್ರಮ, shader);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Disable_ಶೃಂಗ_Attrib_Array(GLuint ಸೂಚ್ಯಂಕ)
    {
        Disable_ಶೃಂಗ_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_ವಿಳಾಸ("glDisableVertexAttribArray");
        Disable_ಶೃಂಗ_Attrib_Array(ಸೂಚ್ಯಂಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Enable_ಶೃಂಗ_Attrib_Array(GLuint ಸೂಚ್ಯಂಕ)
    {
        Enable_ಶೃಂಗ_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_ವಿಳಾಸ("glEnableVertexAttribArray");
        Enable_ಶೃಂಗ_Attrib_Array(ಸೂಚ್ಯಂಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಸಕ್ರಿಯ_Attrib(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_ಸಕ್ರಿಯ_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_ವಿಳಾಸ("glGetActiveAttrib");
        Get_ಸಕ್ರಿಯ_Attrib(ಕಾರ್ಯಕ್ರಮ, ಸೂಚ್ಯಂಕ, buf_ಗಾತ್ರ, length, size, type, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_ವಿಳಾಸ("glGetActiveUniform");
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ(ಕಾರ್ಯಕ್ರಮ, ಸೂಚ್ಯಂಕ, buf_ಗಾತ್ರ, length, size, type, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei max_ಎಣಿಕೆ, GLsizei *ಎಣಿಕೆ_3, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_ವಿಳಾಸ("glGetAttachedShaders");
        GetAttachedShaders(ಕಾರ್ಯಕ್ರಮ, max_ಎಣಿಕೆ, ಎಣಿಕೆ_3, obj);
    }

    ಸ್ಥಿರ GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_ವಿಳಾಸ("glGetAttribLocation");
        ಹಿಂತಿರುಗಿ GetAttribLocation(ಕಾರ್ಯಕ್ರಮ, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint ಕಾರ್ಯಕ್ರಮ, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetProgramiv");
        GetProgramiv(ಕಾರ್ಯಕ್ರಮ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಕಾರ್ಯಕ್ರಮ_Info_Log(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *infoLog)
    {
        Get_ಕಾರ್ಯಕ್ರಮ_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_ವಿಳಾಸ("glGetProgramInfoLog");
        Get_ಕಾರ್ಯಕ್ರಮ_Info_Log(ಕಾರ್ಯಕ್ರಮ, buf_ಗಾತ್ರ, length, infoLog);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_ವಿಳಾಸ("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_ಗಾತ್ರ, length, infoLog);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_Shader_ಮೂಲ(GLuint shader, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *ಮೂಲ)
    {
        Get_Shader_ಮೂಲ = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_ವಿಳಾಸ("glGetShaderSource");
        Get_Shader_ಮೂಲ(shader, buf_ಗಾತ್ರ, length, ಮೂಲ);
    }

    ಸ್ಥಿರ GLint CODEGEN_FUNCPTR Switch_Get_ಏಕರೂಪ_Location(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *name)
    {
        Get_ಏಕರೂಪ_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_ವಿಳಾಸ("glGetUniformLocation");
        ಹಿಂತಿರುಗಿ Get_ಏಕರೂಪ_Location(ಕಾರ್ಯಕ್ರಮ, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint ಕಾರ್ಯಕ್ರಮ, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetUniformfv");
        GetUniformfv(ಕಾರ್ಯಕ್ರಮ, location, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint ಕಾರ್ಯಕ್ರಮ, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetUniformiv");
        GetUniformiv(ಕಾರ್ಯಕ್ರಮ, location, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಶೃಂಗ_Attribdv(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLdouble *params)
    {
        Get_ಶೃಂಗ_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetVertexAttribdv");
        Get_ಶೃಂಗ_Attribdv(ಸೂಚ್ಯಂಕ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಶೃಂಗ_Attribfv(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLfloat *params)
    {
        Get_ಶೃಂಗ_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetVertexAttribfv");
        Get_ಶೃಂಗ_Attribfv(ಸೂಚ್ಯಂಕ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಶೃಂಗ_Attribiv(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLint *params)
    {
        Get_ಶೃಂಗ_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetVertexAttribiv");
        Get_ಶೃಂಗ_Attribiv(ಸೂಚ್ಯಂಕ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಶೃಂಗ_Attrib_Pointerv(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLvoid* *pointer)
    {
        Get_ಶೃಂಗ_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetVertexAttribPointerv");
        Get_ಶೃಂಗ_Attrib_Pointerv(ಸೂಚ್ಯಂಕ, pname, pointer);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_Is_ಕಾರ್ಯಕ್ರಮ(GLuint ಕಾರ್ಯಕ್ರಮ)
    {
        Is_ಕಾರ್ಯಕ್ರಮ = (PFNISPROGRAMPROC)Int_Get_Proc_ವಿಳಾಸ("glIsProgram");
        ಹಿಂತಿರುಗಿ Is_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_ವಿಳಾಸ("glIsShader");
        ಹಿಂತಿರುಗಿ IsShader(shader);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Link_ಕಾರ್ಯಕ್ರಮ(GLuint ಕಾರ್ಯಕ್ರಮ)
    {
        Link_ಕಾರ್ಯಕ್ರಮ = (PFNLINKPROGRAMPROC)Int_Get_Proc_ವಿಳಾಸ("glLinkProgram");
        Link_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Shader_ಮೂಲ(GLuint shader, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *string, ಸ್ಥಿರಾಂಕ GLint *length)
    {
        Shader_ಮೂಲ = (PFNSHADERSOURCEPROC)Int_Get_Proc_ವಿಳಾಸ("glShaderSource");
        Shader_ಮೂಲ(shader, ಎಣಿಕೆ_3, string, length);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Use_ಕಾರ್ಯಕ್ರಮ(GLuint ಕಾರ್ಯಕ್ರಮ)
    {
        Use_ಕಾರ್ಯಕ್ರಮ = (PFNUSEPROGRAMPROC)Int_Get_Proc_ವಿಳಾಸ("glUseProgram");
        Use_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform1f");
        Uniform1f(location, v0);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform1i");
        Uniform1i(location, v0);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform1fv");
        Uniform1fv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform2fv");
        Uniform2fv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform3fv");
        Uniform3fv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform4fv");
        Uniform4fv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform1iv");
        Uniform1iv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform2iv");
        Uniform2iv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform3iv");
        Uniform3iv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform4iv");
        Uniform4iv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix2fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix2fv");
        ಏಕರೂಪ_Matrix2fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix3fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix3fv");
        ಏಕರೂಪ_Matrix3fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix4fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix4fv");
        ಏಕರೂಪ_Matrix4fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Validate_ಕಾರ್ಯಕ್ರಮ(GLuint ಕಾರ್ಯಕ್ರಮ)
    {
        Validate_ಕಾರ್ಯಕ್ರಮ = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_ವಿಳಾಸ("glValidateProgram");
        Validate_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_ಸೂಚಕ(GLuint ಸೂಚ್ಯಂಕ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *pointer)
    {
        ಶೃಂಗ_Attrib_ಸೂಚಕ = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribPointer");
        ಶೃಂಗ_Attrib_ಸೂಚಕ(ಸೂಚ್ಯಂಕ, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix2x3fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix2x3fv");
        ಏಕರೂಪ_Matrix2x3fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix3x2fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix3x2fv");
        ಏಕರೂಪ_Matrix3x2fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix2x4fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix2x4fv");
        ಏಕರೂಪ_Matrix2x4fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix4x2fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix4x2fv");
        ಏಕರೂಪ_Matrix4x2fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix3x4fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix3x4fv");
        ಏಕರೂಪ_Matrix3x4fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_Matrix4x3fv(GLint location, GLsizei ಎಣಿಕೆ_3, GLboolean transpose, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ಏಕರೂಪ_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformMatrix4x3fv");
        ಏಕರೂಪ_Matrix4x3fv(location, ಎಣಿಕೆ_3, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Bind_ಶೃಂಗ_Array(GLuint ren_array)
    {
        Bind_ಶೃಂಗ_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_ವಿಳಾಸ("glBindVertexArray");
        Bind_ಶೃಂಗ_Array(ren_array);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Delete_ಶೃಂಗ_Arrays(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *arrays)
    {
        Delete_ಶೃಂಗ_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteVertexArrays");
        Delete_ಶೃಂಗ_Arrays(n, arrays);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Gen_ಶೃಂಗ_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_ಶೃಂಗ_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_ವಿಳಾಸ("glGenVertexArrays");
        Gen_ಶೃಂಗ_Arrays(n, arrays);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_Is_ಶೃಂಗ_Array(GLuint ren_array)
    {
        Is_ಶೃಂಗ_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_ವಿಳಾಸ("glIsVertexArray");
        ಹಿಂತಿರುಗಿ Is_ಶೃಂಗ_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    ಸ್ಥಿರ GLvoid* CODEGEN_FUNCPTR Switch_Map_ಬಫರ್_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_ಬಫರ್_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_ವಿಳಾಸ("glMapBufferRange");
        ಹಿಂತಿರುಗಿ Map_ಬಫರ್_Range(target, offset, length, access);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Flush_Mapped_ಬಫರ್_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_ಬಫರ್_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_ವಿಳಾಸ("glFlushMappedBufferRange");
        Flush_Mapped_ಬಫರ್_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glIsRenderbuffer");
        ಹಿಂತಿರುಗಿ IsRenderbuffer(renderbuffer);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_ವಿಳಾಸ("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, ಅಗಲ, ಎತ್ತರ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glIsFramebuffer");
        ಹಿಂತಿರುಗಿ IsFramebuffer(framebuffer);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ಸ್ಥಿರಾಂಕ GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_ವಿಳಾಸ("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    ಸ್ಥಿರ GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_ಸ್ಥಿತಿ(GLenum target)
    {
        Check_Framebuffer_ಸ್ಥಿತಿ = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_ವಿಳಾಸ("glCheckFramebufferStatus");
        ಹಿಂತಿರುಗಿ Check_Framebuffer_ಸ್ಥಿತಿ(target);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_ವಿಳಾಸ("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_ವಿಳಾಸ("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_ವಿಳಾಸ("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_ವಿಳಾಸ("glGenerateMipmap");
        GenerateMipmap(target);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ಮುಖವಾಡ, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, ಮುಖವಾಡ, filter);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_ವಿಳಾಸ("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, ಅಗಲ, ಎತ್ತರ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_ವಿಳಾಸ("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ColorMaski(GLuint ಸೂಚ್ಯಂಕ, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_ವಿಳಾಸ("glColorMaski");
        ColorMaski(ಸೂಚ್ಯಂಕ, r, g, b, a);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLboolean *ದತ್ತಾಂಶ)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_ವಿಳಾಸ("glGetBooleani_v");
        GetBooleani_v(target, ಸೂಚ್ಯಂಕ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLint *ದತ್ತಾಂಶ)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_ವಿಳಾಸ("glGetIntegeri_v");
        GetIntegeri_v(target, ಸೂಚ್ಯಂಕ, ದತ್ತಾಂಶ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint ಸೂಚ್ಯಂಕ)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_ವಿಳಾಸ("glEnablei");
        Enablei(target, ಸೂಚ್ಯಂಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint ಸೂಚ್ಯಂಕ)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_ವಿಳಾಸ("glDisablei");
        Disablei(target, ಸೂಚ್ಯಂಕ);
    }

    ಸ್ಥಿರ GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint ಸೂಚ್ಯಂಕ)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_ವಿಳಾಸ("glIsEnabledi");
        ಹಿಂತಿರುಗಿ IsEnabledi(target, ಸೂಚ್ಯಂಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_ಬಹುಲಕ)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_ವಿಳಾಸ("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಕೊನೆ_Transform_Feedback()
    {
        ಕೊನೆ_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_ವಿಳಾಸ("glEndTransformFeedback");
        ಕೊನೆ_Transform_Feedback();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Bind_ಬಫರ್_Range(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLuint ಬಫರ್, GLintptr offset, GLsizeiptr size)
    {
        Bind_ಬಫರ್_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_ವಿಳಾಸ("glBindBufferRange");
        Bind_ಬಫರ್_Range(target, ಸೂಚ್ಯಂಕ, ಬಫರ್, offset, size);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Bind_ಬಫರ್_ತಳ(GLenum target, GLuint ಸೂಚ್ಯಂಕ, GLuint ಬಫರ್)
    {
        Bind_ಬಫರ್_ತಳ = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_ವಿಳಾಸ("glBindBufferBase");
        Bind_ಬಫರ್_ತಳ(target, ಸೂಚ್ಯಂಕ, ಬಫರ್);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *varyings, GLenum ಬಫರ್_ಬಹುಲಕ)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_ವಿಳಾಸ("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(ಕಾರ್ಯಕ್ರಮ, ಎಣಿಕೆ_3, varyings, ಬಫರ್_ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(ಕಾರ್ಯಕ್ರಮ, ಸೂಚ್ಯಂಕ, buf_ಗಾತ್ರ, length, size, type, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_ವಿಳಾಸ("glClampColor");
        ClampColor(target, clamp);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum ಬಹುಲಕ)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_ವಿಳಾಸ("glBeginConditionalRender");
        BeginConditionalRender(id, ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಕೊನೆ_Conditional_Render()
    {
        ಕೊನೆ_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_ವಿಳಾಸ("glEndConditionalRender");
        ಕೊನೆ_Conditional_Render();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_IPointer(GLuint ಸೂಚ್ಯಂಕ, GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *pointer)
    {
        ಶೃಂಗ_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribIPointer");
        ಶೃಂಗ_Attrib_IPointer(ಸೂಚ್ಯಂಕ, size, type, stride, pointer);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಶೃಂಗ_Attrib_Iiv(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLint *params)
    {
        Get_ಶೃಂಗ_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetVertexAttribIiv");
        Get_ಶೃಂಗ_Attrib_Iiv(ಸೂಚ್ಯಂಕ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಶೃಂಗ_Attrib_Iuiv(GLuint ಸೂಚ್ಯಂಕ, GLenum pname, GLuint *params)
    {
        Get_ಶೃಂಗ_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetVertexAttribIuiv");
        Get_ಶೃಂಗ_Attrib_Iuiv(ಸೂಚ್ಯಂಕ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I1i(GLuint ಸೂಚ್ಯಂಕ, GLint x)
    {
        ಶೃಂಗ_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI1i");
        ಶೃಂಗ_Attrib_I1i(ಸೂಚ್ಯಂಕ, x);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I2i(GLuint ಸೂಚ್ಯಂಕ, GLint x, GLint y)
    {
        ಶೃಂಗ_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI2i");
        ಶೃಂಗ_Attrib_I2i(ಸೂಚ್ಯಂಕ, x, y);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I3i(GLuint ಸೂಚ್ಯಂಕ, GLint x, GLint y, GLint z)
    {
        ಶೃಂಗ_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI3i");
        ಶೃಂಗ_Attrib_I3i(ಸೂಚ್ಯಂಕ, x, y, z);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4i(GLuint ಸೂಚ್ಯಂಕ, GLint x, GLint y, GLint z, GLint w)
    {
        ಶೃಂಗ_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4i");
        ಶೃಂಗ_Attrib_I4i(ಸೂಚ್ಯಂಕ, x, y, z, w);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I1ui(GLuint ಸೂಚ್ಯಂಕ, GLuint x)
    {
        ಶೃಂಗ_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI1ui");
        ಶೃಂಗ_Attrib_I1ui(ಸೂಚ್ಯಂಕ, x);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I2ui(GLuint ಸೂಚ್ಯಂಕ, GLuint x, GLuint y)
    {
        ಶೃಂಗ_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI2ui");
        ಶೃಂಗ_Attrib_I2ui(ಸೂಚ್ಯಂಕ, x, y);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I3ui(GLuint ಸೂಚ್ಯಂಕ, GLuint x, GLuint y, GLuint z)
    {
        ಶೃಂಗ_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI3ui");
        ಶೃಂಗ_Attrib_I3ui(ಸೂಚ್ಯಂಕ, x, y, z);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4ui(GLuint ಸೂಚ್ಯಂಕ, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        ಶೃಂಗ_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4ui");
        ಶೃಂಗ_Attrib_I4ui(ಸೂಚ್ಯಂಕ, x, y, z, w);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I1iv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v)
    {
        ಶೃಂಗ_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI1iv");
        ಶೃಂಗ_Attrib_I1iv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I2iv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v)
    {
        ಶೃಂಗ_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI2iv");
        ಶೃಂಗ_Attrib_I2iv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I3iv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v)
    {
        ಶೃಂಗ_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI3iv");
        ಶೃಂಗ_Attrib_I3iv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4iv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLint *v)
    {
        ಶೃಂಗ_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4iv");
        ಶೃಂಗ_Attrib_I4iv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I1uiv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v)
    {
        ಶೃಂಗ_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI1uiv");
        ಶೃಂಗ_Attrib_I1uiv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I2uiv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v)
    {
        ಶೃಂಗ_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI2uiv");
        ಶೃಂಗ_Attrib_I2uiv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I3uiv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v)
    {
        ಶೃಂಗ_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI3uiv");
        ಶೃಂಗ_Attrib_I3uiv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4uiv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLuint *v)
    {
        ಶೃಂಗ_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4uiv");
        ಶೃಂಗ_Attrib_I4uiv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4bv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLbyte *v)
    {
        ಶೃಂಗ_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4bv");
        ಶೃಂಗ_Attrib_I4bv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4sv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLshort *v)
    {
        ಶೃಂಗ_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4sv");
        ಶೃಂಗ_Attrib_I4sv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4ubv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLubyte *v)
    {
        ಶೃಂಗ_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4ubv");
        ಶೃಂಗ_Attrib_I4ubv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_Attrib_I4usv(GLuint ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ GLushort *v)
    {
        ಶೃಂಗ_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexAttribI4usv");
        ಶೃಂಗ_Attrib_I4usv(ಸೂಚ್ಯಂಕ, v);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint ಕಾರ್ಯಕ್ರಮ, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetUniformuiv");
        GetUniformuiv(ಕಾರ್ಯಕ್ರಮ, location, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Bind_Frag_ದತ್ತಾಂಶ_Location(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint color, ಸ್ಥಿರಾಂಕ GLchar *name)
    {
        Bind_Frag_ದತ್ತಾಂಶ_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_ವಿಳಾಸ("glBindFragDataLocation");
        Bind_Frag_ದತ್ತಾಂಶ_Location(ಕಾರ್ಯಕ್ರಮ, color, name);
    }

    ಸ್ಥಿರ GLint CODEGEN_FUNCPTR Switch_Get_Frag_ದತ್ತಾಂಶ_Location(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *name)
    {
        Get_Frag_ದತ್ತಾಂಶ_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_ವಿಳಾಸ("glGetFragDataLocation");
        ಹಿಂತಿರುಗಿ Get_Frag_ದತ್ತಾಂಶ_Location(ಕಾರ್ಯಕ್ರಮ, name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform1ui");
        Uniform1ui(location, v0);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform1uiv");
        Uniform1uiv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform2uiv");
        Uniform2uiv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform3uiv");
        Uniform3uiv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei ಎಣಿಕೆ_3, ಸ್ಥಿರಾಂಕ GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_ವಿಳಾಸ("glUniform4uiv");
        Uniform4uiv(location, ಎಣಿಕೆ_3, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_ವಿಳಾಸ("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ಸ್ಥಿರಾಂಕ GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_ವಿಳಾಸ("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum ಬಫರ್, GLint drawbuffer, ಸ್ಥಿರಾಂಕ GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_ವಿಳಾಸ("glClearBufferiv");
        ClearBufferiv(ಬಫರ್, drawbuffer, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum ಬಫರ್, GLint drawbuffer, ಸ್ಥಿರಾಂಕ GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_ವಿಳಾಸ("glClearBufferuiv");
        ClearBufferuiv(ಬಫರ್, drawbuffer, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum ಬಫರ್, GLint drawbuffer, ಸ್ಥಿರಾಂಕ GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_ವಿಳಾಸ("glClearBufferfv");
        ClearBufferfv(ಬಫರ್, drawbuffer, value);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum ಬಫರ್, GLint drawbuffer, GLfloat ಆಳ, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_ವಿಳಾಸ("glClearBufferfi");
        ClearBufferfi(ಬಫರ್, drawbuffer, ಆಳ, stencil);
    }

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint ಸೂಚ್ಯಂಕ)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_ವಿಳಾಸ("glGetStringi");
        ಹಿಂತಿರುಗಿ GetStringi(name, ಸೂಚ್ಯಂಕ);
    }

    // Extension: ARB_uniform_buffer_object

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಏಕರೂಪ_Indices(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei ಏಕರೂಪ_ಎಣಿಕೆ, ಸ್ಥಿರಾಂಕ GLchar* ಸ್ಥಿರಾಂಕ *ಏಕರೂಪ_ಹೆಸರುಗಳು, GLuint *ಏಕರೂಪ_Indices)
    {
        Get_ಏಕರೂಪ_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_ವಿಳಾಸ("glGetUniformIndices");
        Get_ಏಕರೂಪ_Indices(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಎಣಿಕೆ, ಏಕರೂಪ_ಹೆಸರುಗಳು, ಏಕರೂಪ_Indices);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಸಕ್ರಿಯ_Uniformsiv(GLuint ಕಾರ್ಯಕ್ರಮ, GLsizei ಏಕರೂಪ_ಎಣಿಕೆ, ಸ್ಥಿರಾಂಕ GLuint *ಏಕರೂಪ_Indices, GLenum pname, GLint *params)
    {
        Get_ಸಕ್ರಿಯ_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetActiveUniformsiv");
        Get_ಸಕ್ರಿಯ_Uniformsiv(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಎಣಿಕೆ, ಏಕರೂಪ_Indices, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *ಏಕರೂಪ_Name)
    {
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_ವಿಳಾಸ("glGetActiveUniformName");
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಸೂಚ್ಯಂಕ, buf_ಗಾತ್ರ, length, ಏಕರೂಪ_Name);
    }

    ಸ್ಥಿರ GLuint CODEGEN_FUNCPTR Switch_Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ(GLuint ಕಾರ್ಯಕ್ರಮ, ಸ್ಥಿರಾಂಕ GLchar *ಏಕರೂಪ_ಖಂಡ_Name)
    {
        Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_ವಿಳಾಸ("glGetUniformBlockIndex");
        ಹಿಂತಿರುಗಿ Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಖಂಡ_Name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, GLenum pname, GLint *params)
    {
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_ವಿಳಾಸ("glGetActiveUniformBlockiv");
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, pname, params);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, GLsizei buf_ಗಾತ್ರ, GLsizei *length, GLchar *ಏಕರೂಪ_ಖಂಡ_Name)
    {
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_ವಿಳಾಸ("glGetActiveUniformBlockName");
        Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, buf_ಗಾತ್ರ, length, ಏಕರೂಪ_ಖಂಡ_Name);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಏಕರೂಪ_ಖಂಡ_Binding(GLuint ಕಾರ್ಯಕ್ರಮ, GLuint ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, GLuint ಏಕರೂಪ_ಖಂಡ_Binding_2)
    {
        ಏಕರೂಪ_ಖಂಡ_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_ವಿಳಾಸ("glUniformBlockBinding");
        ಏಕರೂಪ_ಖಂಡ_Binding(ಕಾರ್ಯಕ್ರಮ, ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ, ಏಕರೂಪ_ಖಂಡ_Binding_2);
    }

    // Extension: ARB_copy_buffer

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ(GLenum read_ಗುರಿ, GLenum write_ಗುರಿ, GLintptr read_ಸ್ಥಾನಾಂತರ, GLintptr write_ಸ್ಥಾನಾಂತರ, GLsizeiptr size)
    {
        Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_ವಿಳಾಸ("glCopyBufferSubData");
        Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ(read_ಗುರಿ, write_ಗುರಿ, read_ಸ್ಥಾನಾಂತರ, write_ಸ್ಥಾನಾಂತರ, size);
    }

    // Extension: 3.1

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum ಬಹುಲಕ, GLint first, GLsizei ಎಣಿಕೆ_3, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawArraysInstanced");
        DrawArraysInstanced(ಬಹುಲಕ, first, ಎಣಿಕೆ_3, instancecount);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum ಬಹುಲಕ, GLsizei ಎಣಿಕೆ_3, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_ವಿಳಾಸ("glDrawElementsInstanced");
        DrawElementsInstanced(ಬಹುಲಕ, ಎಣಿಕೆ_3, type, indices, instancecount);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Tex_ಬಫರ್(GLenum target, GLenum internalformat, GLuint ಬಫರ್)
    {
        Tex_ಬಫರ್ = (PFNTEXBUFFERPROC)Int_Get_Proc_ವಿಳಾಸ("glTexBuffer");
        Tex_ಬಫರ್(target, internalformat, ಬಫರ್);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Primitive_Restart_ಸೂಚ್ಯಂಕ(GLuint ಸೂಚ್ಯಂಕ)
    {
        Primitive_Restart_ಸೂಚ್ಯಂಕ = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_ವಿಳಾಸ("glPrimitiveRestartIndex");
        Primitive_Restart_ಸೂಚ್ಯಂಕ(ಸೂಚ್ಯಂಕ);
    }

    // Legacy

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_ವಿಳಾಸ("glEnableClientState");
        EnableClientState(cap);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_ವಿಳಾಸ("glDisableClientState");
        DisableClientState(cap);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಶೃಂಗ_ಸೂಚಕ(GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr)
    {
        ಶೃಂಗ_ಸೂಚಕ = (PFNVERTEXPOINTERPROC)Int_Get_Proc_ವಿಳಾಸ("glVertexPointer");
        ಶೃಂಗ_ಸೂಚಕ(size, type, stride, ptr);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಸಾಮಾನ್ಯ_ಸೂಚಕ(GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr)
    {
        ಸಾಮಾನ್ಯ_ಸೂಚಕ = (PFNNORMALPOINTERPROC)Int_Get_Proc_ವಿಳಾಸ("glNormalPointer");
        ಸಾಮಾನ್ಯ_ಸೂಚಕ(type, stride, ptr);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Color_ಸೂಚಕ(GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr)
    {
        Color_ಸೂಚಕ = (PFNCOLORPOINTERPROC)Int_Get_Proc_ವಿಳಾಸ("glColorPointer");
        Color_ಸೂಚಕ(size, type, stride, ptr);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Tex_Coord_ಸೂಚಕ(GLint size, GLenum type, GLsizei stride, ಸ್ಥಿರಾಂಕ GLvoid *ptr)
    {
        Tex_Coord_ಸೂಚಕ = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_ವಿಳಾಸ("glTexCoordPointer");
        Tex_Coord_ಸೂಚಕ(size, type, stride, ptr);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_ವಿಳಾಸ("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ಮಾತೃಕೆ_ಬಹುಲಕ(GLenum ಬಹುಲಕ)
    {
        ಮಾತೃಕೆ_ಬಹುಲಕ = (PFNMATRIXMODEPROC)Int_Get_Proc_ವಿಳಾಸ("glMatrixMode");
        ಮಾತೃಕೆ_ಬಹುಲಕ(ಬಹುಲಕ);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_ತುಂಬು_ತಟಸ್ಥ(ಶೂನ್ಯ)
    {
        ತುಂಬು_ತಟಸ್ಥ = (PFNLOADIDENTITYPROC)Int_Get_Proc_ವಿಳಾಸ("glLoadIdentity");
        ತುಂಬು_ತಟಸ್ಥ();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Ortho(GLdouble ಎಡ, GLdouble ಬಲ, GLdouble bottom, GLdouble ತುದಿ_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_ವಿಳಾಸ("glOrtho");
        Ortho(ಎಡ, ಬಲ, bottom, ತುದಿ_2, near_val, far_val);
    }

    ಸ್ಥಿರ ಶೂನ್ಯ CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_ವಿಳಾಸ("glColor3d");
        Color3d(red, green, blue);
    }

    ರಚನೆ InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            ಸಾಲು_ಅಗಲ = Switch_ಸಾಲು_ಅಗಲ;
            ಬಿಂದು_ಗಾತ್ರ = Switch_ಬಿಂದು_ಗಾತ್ರ;
            Polygon_ಬಹುಲಕ = Switch_Polygon_ಬಹುಲಕ;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_ಬಫರ್ = Switch_Draw_ಬಫರ್;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            Clear_ಆಳ = Switch_Clear_ಆಳ;
            Stencil_ಮುಖವಾಡ = Switch_Stencil_ಮುಖವಾಡ;
            Color_ಮುಖವಾಡ = Switch_Color_ಮುಖವಾಡ;
            ಆಳ_ಮುಖವಾಡ = Switch_ಆಳ_ಮುಖವಾಡ;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            ಮುಕ್ತಾಯ = Switch_ಮುಕ್ತಾಯ;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            ಆಳ_Func = Switch_ಆಳ_Func;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            Read_ಬಫರ್ = Switch_Read_ಬಫರ್;
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
            ಆಳ_Range = Switch_ಆಳ_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_ಸ್ಥಾನಾಂತರ = Switch_Polygon_ಸ್ಥಾನಾಂತರ;
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
            ಸಕ್ರಿಯ_Texture = Switch_ಸಕ್ರಿಯ_Texture;
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
            ಬಿಂದು_Parameterf = Switch_ಬಿಂದು_Parameterf;
            ಬಿಂದು_Parameterfv = Switch_ಬಿಂದು_Parameterfv;
            ಬಿಂದು_Parameteri = Switch_ಬಿಂದು_Parameteri;
            ಬಿಂದು_Parameteriv = Switch_ಬಿಂದು_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            ಕೊನೆ_Query = Switch_ಕೊನೆ_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_ಬಫರ್ = Switch_Bind_ಬಫರ್;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_ಬಫರ್ = Switch_Is_ಬಫರ್;
            ಬಫರ್_ದತ್ತಾಂಶ = Switch_ಬಫರ್_ದತ್ತಾಂಶ;
            ಬಫರ್_Sub_ದತ್ತಾಂಶ = Switch_ಬಫರ್_Sub_ದತ್ತಾಂಶ;
            Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ = Switch_Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ;
            Map_ಬಫರ್ = Switch_Map_ಬಫರ್;
            Unmap_ಬಫರ್ = Switch_Unmap_ಬಫರ್;
            Get_ಬಫರ್_Parameteriv = Switch_Get_ಬಫರ್_Parameteriv;
            Get_ಬಫರ್_Pointerv = Switch_Get_ಬಫರ್_Pointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            Stencil_ಮುಖವಾಡ_Separate = Switch_Stencil_ಮುಖವಾಡ_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_ಕಾರ್ಯಕ್ರಮ = Switch_Create_ಕಾರ್ಯಕ್ರಮ;
            CreateShader = Switch_CreateShader;
            Delete_ಕಾರ್ಯಕ್ರಮ = Switch_Delete_ಕಾರ್ಯಕ್ರಮ;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            Disable_ಶೃಂಗ_Attrib_Array = Switch_Disable_ಶೃಂಗ_Attrib_Array;
            Enable_ಶೃಂಗ_Attrib_Array = Switch_Enable_ಶೃಂಗ_Attrib_Array;
            Get_ಸಕ್ರಿಯ_Attrib = Switch_Get_ಸಕ್ರಿಯ_Attrib;
            Get_ಸಕ್ರಿಯ_ಏಕರೂಪ = Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_ಕಾರ್ಯಕ್ರಮ_Info_Log = Switch_Get_ಕಾರ್ಯಕ್ರಮ_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_ಮೂಲ = Switch_Get_Shader_ಮೂಲ;
            Get_ಏಕರೂಪ_Location = Switch_Get_ಏಕರೂಪ_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_ಶೃಂಗ_Attribdv = Switch_Get_ಶೃಂಗ_Attribdv;
            Get_ಶೃಂಗ_Attribfv = Switch_Get_ಶೃಂಗ_Attribfv;
            Get_ಶೃಂಗ_Attribiv = Switch_Get_ಶೃಂಗ_Attribiv;
            Get_ಶೃಂಗ_Attrib_Pointerv = Switch_Get_ಶೃಂಗ_Attrib_Pointerv;
            Is_ಕಾರ್ಯಕ್ರಮ = Switch_Is_ಕಾರ್ಯಕ್ರಮ;
            IsShader = Switch_IsShader;
            Link_ಕಾರ್ಯಕ್ರಮ = Switch_Link_ಕಾರ್ಯಕ್ರಮ;
            Shader_ಮೂಲ = Switch_Shader_ಮೂಲ;
            Use_ಕಾರ್ಯಕ್ರಮ = Switch_Use_ಕಾರ್ಯಕ್ರಮ;
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
            ಏಕರೂಪ_Matrix2fv = Switch_ಏಕರೂಪ_Matrix2fv;
            ಏಕರೂಪ_Matrix3fv = Switch_ಏಕರೂಪ_Matrix3fv;
            ಏಕರೂಪ_Matrix4fv = Switch_ಏಕರೂಪ_Matrix4fv;
            Validate_ಕಾರ್ಯಕ್ರಮ = Switch_Validate_ಕಾರ್ಯಕ್ರಮ;
            ಶೃಂಗ_Attrib_ಸೂಚಕ = Switch_ಶೃಂಗ_Attrib_ಸೂಚಕ;

            // Extension: 2.1
            ಏಕರೂಪ_Matrix2x3fv = Switch_ಏಕರೂಪ_Matrix2x3fv;
            ಏಕರೂಪ_Matrix3x2fv = Switch_ಏಕರೂಪ_Matrix3x2fv;
            ಏಕರೂಪ_Matrix2x4fv = Switch_ಏಕರೂಪ_Matrix2x4fv;
            ಏಕರೂಪ_Matrix4x2fv = Switch_ಏಕರೂಪ_Matrix4x2fv;
            ಏಕರೂಪ_Matrix3x4fv = Switch_ಏಕರೂಪ_Matrix3x4fv;
            ಏಕರೂಪ_Matrix4x3fv = Switch_ಏಕರೂಪ_Matrix4x3fv;

            // Extension: ARB_vertex_array_object
            Bind_ಶೃಂಗ_Array = Switch_Bind_ಶೃಂಗ_Array;
            Delete_ಶೃಂಗ_Arrays = Switch_Delete_ಶೃಂಗ_Arrays;
            Gen_ಶೃಂಗ_Arrays = Switch_Gen_ಶೃಂಗ_Arrays;
            Is_ಶೃಂಗ_Array = Switch_Is_ಶೃಂಗ_Array;

            // Extension: ARB_map_buffer_range
            Map_ಬಫರ್_Range = Switch_Map_ಬಫರ್_Range;
            Flush_Mapped_ಬಫರ್_Range = Switch_Flush_Mapped_ಬಫರ್_Range;

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
            Check_Framebuffer_ಸ್ಥಿತಿ = Switch_Check_Framebuffer_ಸ್ಥಿತಿ;
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
            ಕೊನೆ_Transform_Feedback = Switch_ಕೊನೆ_Transform_Feedback;
            Bind_ಬಫರ್_Range = Switch_Bind_ಬಫರ್_Range;
            Bind_ಬಫರ್_ತಳ = Switch_Bind_ಬಫರ್_ತಳ;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            ಕೊನೆ_Conditional_Render = Switch_ಕೊನೆ_Conditional_Render;
            ಶೃಂಗ_Attrib_IPointer = Switch_ಶೃಂಗ_Attrib_IPointer;
            Get_ಶೃಂಗ_Attrib_Iiv = Switch_Get_ಶೃಂಗ_Attrib_Iiv;
            Get_ಶೃಂಗ_Attrib_Iuiv = Switch_Get_ಶೃಂಗ_Attrib_Iuiv;
            ಶೃಂಗ_Attrib_I1i = Switch_ಶೃಂಗ_Attrib_I1i;
            ಶೃಂಗ_Attrib_I2i = Switch_ಶೃಂಗ_Attrib_I2i;
            ಶೃಂಗ_Attrib_I3i = Switch_ಶೃಂಗ_Attrib_I3i;
            ಶೃಂಗ_Attrib_I4i = Switch_ಶೃಂಗ_Attrib_I4i;
            ಶೃಂಗ_Attrib_I1ui = Switch_ಶೃಂಗ_Attrib_I1ui;
            ಶೃಂಗ_Attrib_I2ui = Switch_ಶೃಂಗ_Attrib_I2ui;
            ಶೃಂಗ_Attrib_I3ui = Switch_ಶೃಂಗ_Attrib_I3ui;
            ಶೃಂಗ_Attrib_I4ui = Switch_ಶೃಂಗ_Attrib_I4ui;
            ಶೃಂಗ_Attrib_I1iv = Switch_ಶೃಂಗ_Attrib_I1iv;
            ಶೃಂಗ_Attrib_I2iv = Switch_ಶೃಂಗ_Attrib_I2iv;
            ಶೃಂಗ_Attrib_I3iv = Switch_ಶೃಂಗ_Attrib_I3iv;
            ಶೃಂಗ_Attrib_I4iv = Switch_ಶೃಂಗ_Attrib_I4iv;
            ಶೃಂಗ_Attrib_I1uiv = Switch_ಶೃಂಗ_Attrib_I1uiv;
            ಶೃಂಗ_Attrib_I2uiv = Switch_ಶೃಂಗ_Attrib_I2uiv;
            ಶೃಂಗ_Attrib_I3uiv = Switch_ಶೃಂಗ_Attrib_I3uiv;
            ಶೃಂಗ_Attrib_I4uiv = Switch_ಶೃಂಗ_Attrib_I4uiv;
            ಶೃಂಗ_Attrib_I4bv = Switch_ಶೃಂಗ_Attrib_I4bv;
            ಶೃಂಗ_Attrib_I4sv = Switch_ಶೃಂಗ_Attrib_I4sv;
            ಶೃಂಗ_Attrib_I4ubv = Switch_ಶೃಂಗ_Attrib_I4ubv;
            ಶೃಂಗ_Attrib_I4usv = Switch_ಶೃಂಗ_Attrib_I4usv;
            GetUniformuiv = Switch_GetUniformuiv;
            Bind_Frag_ದತ್ತಾಂಶ_Location = Switch_Bind_Frag_ದತ್ತಾಂಶ_Location;
            Get_Frag_ದತ್ತಾಂಶ_Location = Switch_Get_Frag_ದತ್ತಾಂಶ_Location;
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
            Get_ಏಕರೂಪ_Indices = Switch_Get_ಏಕರೂಪ_Indices;
            Get_ಸಕ್ರಿಯ_Uniformsiv = Switch_Get_ಸಕ್ರಿಯ_Uniformsiv;
            Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name = Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Name;
            Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ = Switch_Get_ಏಕರೂಪ_ಖಂಡ_ಸೂಚ್ಯಂಕ;
            Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv = Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_Blockiv;
            Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name = Switch_Get_ಸಕ್ರಿಯ_ಏಕರೂಪ_ಖಂಡ_Name;
            ಏಕರೂಪ_ಖಂಡ_Binding = Switch_ಏಕರೂಪ_ಖಂಡ_Binding;

            // Extension: ARB_copy_buffer
            Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ = Switch_Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_ಬಫರ್ = Switch_Tex_ಬಫರ್;
            Primitive_Restart_ಸೂಚ್ಯಂಕ = Switch_Primitive_Restart_ಸೂಚ್ಯಂಕ;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            ಶೃಂಗ_ಸೂಚಕ = Switch_ಶೃಂಗ_ಸೂಚಕ;
            ಸಾಮಾನ್ಯ_ಸೂಚಕ = Switch_ಸಾಮಾನ್ಯ_ಸೂಚಕ;
            Color_ಸೂಚಕ = Switch_Color_ಸೂಚಕ;
            Tex_Coord_ಸೂಚಕ = Switch_Tex_Coord_ಸೂಚಕ;
            TexEnvi = Switch_TexEnvi;
            ಮಾತೃಕೆ_ಬಹುಲಕ = Switch_ಮಾತೃಕೆ_ಬಹುಲಕ;
            ತುಂಬು_ತಟಸ್ಥ = Switch_ತುಂಬು_ತಟಸ್ಥ;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
