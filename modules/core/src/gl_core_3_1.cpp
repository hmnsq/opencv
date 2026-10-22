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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "gl_core_3_1.hpp"

#ifdef HAVE_OPENGL

    #ifdef __APPLE__
        #अन्तर्भाव <dlfcn.h>

        स्थिर शून्य* AppleGLGetProcAddress (ध्रुव वर्ण* name)
        {
            स्थिर तार्किक initialized = मिच्छ;
            स्थिर शून्य * handle = रिक्त;
            जइ (!handle)
            {
                जइ (!initialized)
                {
                    initialized = सच्च;
                    ध्रुव वर्ण * ध्रुव path = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
                }
                जइ (!handle)
                    प्रत्यागम रिक्त;
            }
            प्रत्यागम dlsym(handle, name);
        }
    #endif // __APPLE__

    #जइ defined(__sgi) || defined (__sun)
        #अन्तर्भाव <dlfcn.h>
        #अन्तर्भाव <stdio.h>

        स्थिर शून्य* SunGetProcAddress (ध्रुव वर्ण* name)
        {
            प्रकारनिर्देश शून्य* (func_t*)(ध्रुव GLubyte*);

            स्थिर शून्य* h = 0;
            स्थिर func_t gpa = 0;

            जइ (!h)
            {
                h = dlopen(रिक्त, RTLD_LAZY | RTLD_LOCAL);
                जइ (!h)
                    प्रत्यागम 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            प्रत्यागम gpa ? gpa((ध्रुव GLubyte*) name) : dlsym(h, name);
        }
    #endif // __sgi || __sun

    #जइ defined(_WIN32)
        #ifdef _MSC_VER
            #pragma warning(disable: 4055)
            #pragma warning(disable: 4054)
        #endif

        स्थिर पूर्णाङ्क TestPointer(ध्रुव PROC pTest)
        {
            जइ(!pTest)
                प्रत्यागम 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            जइ (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                प्रत्यागम 0;

            प्रत्यागम 1;
        }

        स्थिर PROC WinGetProcAddress(ध्रुव वर्ण* name)
        {
            PROC pFunc = wglGetProcAddress((LPCSTR) name);
            जइ (TestPointer(pFunc))
                प्रत्यागम pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            प्रत्यागम (PROC) GetProcAddress(glMod, (LPCSTR) name);
        }
    #endif // _WIN32

    #जइ defined(_WIN32)
        #define CV_GL_GET_PROC_ADDRESS(name) WinGetProcAddress(name)
    #नोचेद्यदि defined(__APPLE__)
        #define CV_GL_GET_PROC_ADDRESS(name) AppleGLGetProcAddress(name)
    #नोचेद्यदि defined(__sgi) || defined(__sun)
        #define CV_GL_GET_PROC_ADDRESS(name) SunGetProcAddress(name)
    #अण्णहा // GLX
        #अन्तर्भाव <GL/glx.h>

        #define CV_GL_GET_PROC_ADDRESS(name) glXGetProcAddressARB((ध्रुव GLubyte*) name)
    #endif

    स्थिर शून्य* IntGetProcAddress(ध्रुव वर्ण* name)
    {
        शून्य* func =  (शून्य*) CV_GL_GET_PROC_ADDRESS(name);
        जइ (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        प्रत्यागम func;
    }
#अण्णहा
#जइ defined(_MSC_VER)
    #pragma warning(disable : 4702)  // unreachable code
#endif
    स्थिर शून्य* IntGetProcAddress(ध्रुव वर्ण*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#endif

नामाकाश gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    प्रकारनिर्देश GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    प्रकारनिर्देश ध्रुव GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(ध्रुव GLubyte *);

    // Extension: 1.2
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , ध्रुव GLint *, ध्रुव GLsizei *, GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , ध्रुव GLsizei *, GLenum , ध्रुव GLvoid* ध्रुव *, GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , ध्रुव GLint *);

    // Extension: 1.5
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , ध्रुव GLvoid *, GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    प्रकारनिर्देश GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , ध्रुव GLenum *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , ध्रुव GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    प्रकारनिर्देश GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    प्रकारनिर्देश GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    प्रकारनिर्देश GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , ध्रुव GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकारनिर्देश GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , ध्रुव GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , ध्रुव GLchar* ध्रुव *, ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , ध्रुव GLvoid *);

    // Extension: 2.1
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , ध्रुव GLfloat *);

    // Extension: ARB_vertex_array_object
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    प्रकारनिर्देश GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    प्रकारनिर्देश GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    प्रकारनिर्देश GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , ध्रुव GLchar* ध्रुव *, GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , ध्रुव GLbyte *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , ध्रुव GLshort *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , ध्रुव GLubyte *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , ध्रुव GLushort *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , ध्रुव GLchar *);
    प्रकारनिर्देश GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , ध्रुव GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , ध्रुव GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , ध्रुव GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , ध्रुव GLfloat *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    प्रकारनिर्देश ध्रुव GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , ध्रुव GLchar* ध्रुव *, GLuint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , ध्रुव GLuint *, GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकारनिर्देश GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , ध्रुव GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , ध्रुव GLvoid *, GLsizei );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , ध्रुव GLvoid *);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(शून्य);
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    प्रकारनिर्देश शून्य (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC पङ्क्तिः_Width;
    PFNPOINTSIZEPROC PointSize;
    PFNPOLYGONMODEPROC PolygonMode;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC DrawBuffer;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC ClearDepth;
    PFNSTENCILMASKPROC StencilMask;
    PFNCOLORMASKPROC ColorMask;
    PFNDEPTHMASKPROC DepthMask;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC Finish;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC DepthFunc;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC ReadBuffer;
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
    PFNDEPTHRANGEPROC DepthRange;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC PolygonOffset;
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
    PFNACTIVETEXTUREPROC ActiveTexture;
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
    PFNPOINTPARAMETERFPROC PointParameterf;
    PFNPOINTPARAMETERFVPROC PointParameterfv;
    PFNPOINTPARAMETERIPROC PointParameteri;
    PFNPOINTPARAMETERIVPROC PointParameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC EndQuery;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC BindBuffer;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC IsBuffer;
    PFNBUFFERDATAPROC BufferData;
    PFNBUFFERSUBDATAPROC BufferSubData;
    PFNGETBUFFERSUBDATAPROC GetBufferSubData;
    PFNMAPBUFFERPROC MapBuffer;
    PFNUNMAPBUFFERPROC UnmapBuffer;
    PFNGETBUFFERPARAMETERIVPROC GetBufferParameteriv;
    PFNGETBUFFERPOINTERVPROC GetBufferPointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC StencilMaskSeparate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC CreateProgram;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC DeleteProgram;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    PFNENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    PFNGETACTIVEATTRIBPROC GetActiveAttrib;
    PFNGETACTIVEUNIFORMPROC GetActiveUniform;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC GetProgramInfoLog;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC GetShaderSource;
    PFNGETUNIFORMLOCATIONPROC GetUniformLocation;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC GetVertexAttribdv;
    PFNGETVERTEXATTRIBFVPROC GetVertexAttribfv;
    PFNGETVERTEXATTRIBIVPROC GetVertexAttribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerv;
    PFNISPROGRAMPROC IsProgram;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC LinkProgram;
    PFNSHADERSOURCEPROC ShaderSource;
    PFNUSEPROGRAMPROC UseProgram;
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
    PFNUNIFORMMATRIX2FVPROC UniformMatrix2fv;
    PFNUNIFORMMATRIX3FVPROC UniformMatrix3fv;
    PFNUNIFORMMATRIX4FVPROC UniformMatrix4fv;
    PFNVALIDATEPROGRAMPROC ValidateProgram;
    PFNVERTEXATTRIBPOINTERPROC VertexAttribPointer;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC UniformMatrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC UniformMatrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC UniformMatrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC UniformMatrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC UniformMatrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC UniformMatrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC BindVertexArray;
    PFNDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    PFNGENVERTEXARRAYSPROC GenVertexArrays;
    PFNISVERTEXARRAYPROC IsVertexArray;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC MapBufferRange;
    PFNFLUSHMAPPEDBUFFERRANGEPROC FlushMappedBufferRange;

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
    PFNCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
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
    PFNENDTRANSFORMFEEDBACKPROC EndTransformFeedback;
    PFNBINDBUFFERRANGEPROC BindBufferRange;
    PFNBINDBUFFERBASEPROC BindBufferBase;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC EndConditionalRender;
    PFNVERTEXATTRIBIPOINTERPROC VertexAttribIPointer;
    PFNGETVERTEXATTRIBIIVPROC GetVertexAttribIiv;
    PFNGETVERTEXATTRIBIUIVPROC GetVertexAttribIuiv;
    PFNVERTEXATTRIBI1IPROC VertexAttribI1i;
    PFNVERTEXATTRIBI2IPROC VertexAttribI2i;
    PFNVERTEXATTRIBI3IPROC VertexAttribI3i;
    PFNVERTEXATTRIBI4IPROC VertexAttribI4i;
    PFNVERTEXATTRIBI1UIPROC VertexAttribI1ui;
    PFNVERTEXATTRIBI2UIPROC VertexAttribI2ui;
    PFNVERTEXATTRIBI3UIPROC VertexAttribI3ui;
    PFNVERTEXATTRIBI4UIPROC VertexAttribI4ui;
    PFNVERTEXATTRIBI1IVPROC VertexAttribI1iv;
    PFNVERTEXATTRIBI2IVPROC VertexAttribI2iv;
    PFNVERTEXATTRIBI3IVPROC VertexAttribI3iv;
    PFNVERTEXATTRIBI4IVPROC VertexAttribI4iv;
    PFNVERTEXATTRIBI1UIVPROC VertexAttribI1uiv;
    PFNVERTEXATTRIBI2UIVPROC VertexAttribI2uiv;
    PFNVERTEXATTRIBI3UIVPROC VertexAttribI3uiv;
    PFNVERTEXATTRIBI4UIVPROC VertexAttribI4uiv;
    PFNVERTEXATTRIBI4BVPROC VertexAttribI4bv;
    PFNVERTEXATTRIBI4SVPROC VertexAttribI4sv;
    PFNVERTEXATTRIBI4UBVPROC VertexAttribI4ubv;
    PFNVERTEXATTRIBI4USVPROC VertexAttribI4usv;
    PFNGETUNIFORMUIVPROC GetUniformuiv;
    PFNBINDFRAGDATALOCATIONPROC BindFragDataLocation;
    PFNGETFRAGDATALOCATIONPROC GetFragDataLocation;
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
    PFNGETUNIFORMINDICESPROC GetUniformIndices;
    PFNGETACTIVEUNIFORMSIVPROC GetActiveUniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC GetActiveUniformName;
    PFNGETUNIFORMBLOCKINDEXPROC GetUniformBlockIndex;
    PFNGETACTIVEUNIFORMBLOCKIVPROC GetActiveUniformBlockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC GetActiveUniformBlockName;
    PFNUNIFORMBLOCKBINDINGPROC UniformBlockBinding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC CopyBufferSubData;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC TexBuffer;
    PFNPRIMITIVERESTARTINDEXPROC PrimitiveRestartIndex;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC VertexPointer;
    PFNNORMALPOINTERPROC NormalPointer;
    PFNCOLORPOINTERPROC ColorPointer;
    PFNTEXCOORDPOINTERPROC TexCoordPointer;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC MatrixMode;
    PFNLOADIDENTITYPROC LoadIdentity;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CullFace(GLenum mode)
    {
        CullFace = (PFNCULLFACEPROC)IntGetProcAddress("glCullFace");
        CullFace(mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FrontFace(GLenum mode)
    {
        FrontFace = (PFNFRONTFACEPROC)IntGetProcAddress("glFrontFace");
        FrontFace(mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum mode)
    {
        Hint = (PFNHINTPROC)IntGetProcAddress("glHint");
        Hint(target, mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_पङ्क्तिः_Width(GLfloat width)
    {
        पङ्क्तिः_Width = (PFNLINEWIDTHPROC)IntGetProcAddress("glLineWidth");
        पङ्क्तिः_Width(width);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointSize(GLfloat size)
    {
        PointSize = (PFNPOINTSIZEPROC)IntGetProcAddress("glPointSize");
        PointSize(size);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PolygonMode(GLenum face, GLenum mode)
    {
        PolygonMode = (PFNPOLYGONMODEPROC)IntGetProcAddress("glPolygonMode");
        PolygonMode(face, mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Scissor = (PFNSCISSORPROC)IntGetProcAddress("glScissor");
        Scissor(x, y, width, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)IntGetProcAddress("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ध्रुव GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)IntGetProcAddress("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)IntGetProcAddress("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ध्रुव GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)IntGetProcAddress("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)IntGetProcAddress("glTexImage1D");
        TexImage1D(target, level, internalformat, width, border, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)IntGetProcAddress("glTexImage2D");
        TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawBuffer(GLenum mode)
    {
        DrawBuffer = (PFNDRAWBUFFERPROC)IntGetProcAddress("glDrawBuffer");
        DrawBuffer(mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Clear(GLbitfield mask)
    {
        Clear = (PFNCLEARPROC)IntGetProcAddress("glClear");
        Clear(mask);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)IntGetProcAddress("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)IntGetProcAddress("glClearStencil");
        ClearStencil(s);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearDepth(GLdouble depth)
    {
        ClearDepth = (PFNCLEARDEPTHPROC)IntGetProcAddress("glClearDepth");
        ClearDepth(depth);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilMask(GLuint mask)
    {
        StencilMask = (PFNSTENCILMASKPROC)IntGetProcAddress("glStencilMask");
        StencilMask(mask);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        ColorMask = (PFNCOLORMASKPROC)IntGetProcAddress("glColorMask");
        ColorMask(red, green, blue, alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DepthMask(GLboolean flag)
    {
        DepthMask = (PFNDEPTHMASKPROC)IntGetProcAddress("glDepthMask");
        DepthMask(flag);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)IntGetProcAddress("glDisable");
        Disable(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)IntGetProcAddress("glEnable");
        Enable(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Finish()
    {
        Finish = (PFNFINISHPROC)IntGetProcAddress("glFinish");
        Finish();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)IntGetProcAddress("glFlush");
        Flush();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)IntGetProcAddress("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)IntGetProcAddress("glLogicOp");
        LogicOp(opcode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)IntGetProcAddress("glStencilFunc");
        StencilFunc(func, ref, mask);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)IntGetProcAddress("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DepthFunc(GLenum func)
    {
        DepthFunc = (PFNDEPTHFUNCPROC)IntGetProcAddress("glDepthFunc");
        DepthFunc(func);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)IntGetProcAddress("glPixelStoref");
        PixelStoref(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)IntGetProcAddress("glPixelStorei");
        PixelStorei(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ReadBuffer(GLenum mode)
    {
        ReadBuffer = (PFNREADBUFFERPROC)IntGetProcAddress("glReadBuffer");
        ReadBuffer(mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)IntGetProcAddress("glReadPixels");
        ReadPixels(x, y, width, height, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)IntGetProcAddress("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)IntGetProcAddress("glGetDoublev");
        GetDoublev(pname, params);
    }

    स्थिर GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)IntGetProcAddress("glGetError");
        प्रत्यागम GetError();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)IntGetProcAddress("glGetFloatv");
        GetFloatv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)IntGetProcAddress("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    स्थिर ध्रुव GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)IntGetProcAddress("glGetString");
        प्रत्यागम GetString(name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)IntGetProcAddress("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)IntGetProcAddress("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)IntGetProcAddress("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)IntGetProcAddress("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)IntGetProcAddress("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)IntGetProcAddress("glIsEnabled");
        प्रत्यागम IsEnabled(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DepthRange(GLdouble ren_near, GLdouble ren_far)
    {
        DepthRange = (PFNDEPTHRANGEPROC)IntGetProcAddress("glDepthRange");
        DepthRange(ren_near, ren_far);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Viewport = (PFNVIEWPORTPROC)IntGetProcAddress("glViewport");
        Viewport(x, y, width, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawArrays(GLenum mode, GLint first, GLsizei गणनम्_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)IntGetProcAddress("glDrawArrays");
        DrawArrays(mode, first, गणनम्_2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawElements(GLenum mode, GLsizei गणनम्_2, GLenum type, ध्रुव GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)IntGetProcAddress("glDrawElements");
        DrawElements(mode, गणनम्_2, type, indices);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)IntGetProcAddress("glGetPointerv");
        GetPointerv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PolygonOffset(GLfloat factor, GLfloat units)
    {
        PolygonOffset = (PFNPOLYGONOFFSETPROC)IntGetProcAddress("glPolygonOffset");
        PolygonOffset(factor, units);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)IntGetProcAddress("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, width, border);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)IntGetProcAddress("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)IntGetProcAddress("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, width);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)IntGetProcAddress("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)IntGetProcAddress("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, width, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)IntGetProcAddress("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)IntGetProcAddress("glBindTexture");
        BindTexture(target, texture);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ध्रुव GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)IntGetProcAddress("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)IntGetProcAddress("glGenTextures");
        GenTextures(n, textures);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)IntGetProcAddress("glIsTexture");
        प्रत्यागम IsTexture(texture);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)IntGetProcAddress("glIndexub");
        Indexub(c);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Indexubv(ध्रुव GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)IntGetProcAddress("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)IntGetProcAddress("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendEquation(GLenum mode)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)IntGetProcAddress("glBlendEquation");
        BlendEquation(mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei गणनम्_2, GLenum type, ध्रुव GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)IntGetProcAddress("glDrawRangeElements");
        DrawRangeElements(mode, start, end, गणनम्_2, type, indices);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)IntGetProcAddress("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)IntGetProcAddress("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    }

    // Extension: 1.3

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ActiveTexture(GLenum texture)
    {
        ActiveTexture = (PFNACTIVETEXTUREPROC)IntGetProcAddress("glActiveTexture");
        ActiveTexture(texture);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)IntGetProcAddress("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, ध्रुव GLvoid *data)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)IntGetProcAddress("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, ध्रुव GLvoid *data)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)IntGetProcAddress("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, ध्रुव GLvoid *data)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)IntGetProcAddress("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, ध्रुव GLvoid *data)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)IntGetProcAddress("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, ध्रुव GLvoid *data)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)IntGetProcAddress("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, ध्रुव GLvoid *data)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)IntGetProcAddress("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)IntGetProcAddress("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)IntGetProcAddress("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum mode, ध्रुव GLint *first, ध्रुव GLsizei *गणनम्_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)IntGetProcAddress("glMultiDrawArrays");
        MultiDrawArrays(mode, first, गणनम्_2, drawcount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum mode, ध्रुव GLsizei *गणनम्_2, GLenum type, ध्रुव GLvoid* ध्रुव *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)IntGetProcAddress("glMultiDrawElements");
        MultiDrawElements(mode, गणनम्_2, type, indices, drawcount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameterf(GLenum pname, GLfloat param)
    {
        PointParameterf = (PFNPOINTPARAMETERFPROC)IntGetProcAddress("glPointParameterf");
        PointParameterf(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameterfv(GLenum pname, ध्रुव GLfloat *params)
    {
        PointParameterfv = (PFNPOINTPARAMETERFVPROC)IntGetProcAddress("glPointParameterfv");
        PointParameterfv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameteri(GLenum pname, GLint param)
    {
        PointParameteri = (PFNPOINTPARAMETERIPROC)IntGetProcAddress("glPointParameteri");
        PointParameteri(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameteriv(GLenum pname, ध्रुव GLint *params)
    {
        PointParameteriv = (PFNPOINTPARAMETERIVPROC)IntGetProcAddress("glPointParameteriv");
        PointParameteriv(pname, params);
    }

    // Extension: 1.5

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)IntGetProcAddress("glGenQueries");
        GenQueries(n, ids);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ध्रुव GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)IntGetProcAddress("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)IntGetProcAddress("glIsQuery");
        प्रत्यागम IsQuery(id);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)IntGetProcAddress("glBeginQuery");
        BeginQuery(target, id);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EndQuery(GLenum target)
    {
        EndQuery = (PFNENDQUERYPROC)IntGetProcAddress("glEndQuery");
        EndQuery(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)IntGetProcAddress("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)IntGetProcAddress("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)IntGetProcAddress("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindBuffer(GLenum target, GLuint buffer)
    {
        BindBuffer = (PFNBINDBUFFERPROC)IntGetProcAddress("glBindBuffer");
        BindBuffer(target, buffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ध्रुव GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)IntGetProcAddress("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)IntGetProcAddress("glGenBuffers");
        GenBuffers(n, buffers);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsBuffer(GLuint buffer)
    {
        IsBuffer = (PFNISBUFFERPROC)IntGetProcAddress("glIsBuffer");
        प्रत्यागम IsBuffer(buffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BufferData(GLenum target, GLsizeiptr size, ध्रुव GLvoid *data, GLenum usage)
    {
        BufferData = (PFNBUFFERDATAPROC)IntGetProcAddress("glBufferData");
        BufferData(target, size, data, usage);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, ध्रुव GLvoid *data)
    {
        BufferSubData = (PFNBUFFERSUBDATAPROC)IntGetProcAddress("glBufferSubData");
        BufferSubData(target, offset, size, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data)
    {
        GetBufferSubData = (PFNGETBUFFERSUBDATAPROC)IntGetProcAddress("glGetBufferSubData");
        GetBufferSubData(target, offset, size, data);
    }

    स्थिर GLvoid* CODEGEN_FUNCPTR Switch_MapBuffer(GLenum target, GLenum access)
    {
        MapBuffer = (PFNMAPBUFFERPROC)IntGetProcAddress("glMapBuffer");
        प्रत्यागम MapBuffer(target, access);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_UnmapBuffer(GLenum target)
    {
        UnmapBuffer = (PFNUNMAPBUFFERPROC)IntGetProcAddress("glUnmapBuffer");
        प्रत्यागम UnmapBuffer(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetBufferParameteriv = (PFNGETBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetBufferParameteriv");
        GetBufferParameteriv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBufferPointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        GetBufferPointerv = (PFNGETBUFFERPOINTERVPROC)IntGetProcAddress("glGetBufferPointerv");
        GetBufferPointerv(target, pname, params);
    }

    // Extension: 2.0

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)IntGetProcAddress("glBlendEquationSeparate");
        BlendEquationSeparate(modeRGB, modeAlpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ध्रुव GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)IntGetProcAddress("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)IntGetProcAddress("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)IntGetProcAddress("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, mask);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilMaskSeparate(GLenum face, GLuint mask)
    {
        StencilMaskSeparate = (PFNSTENCILMASKSEPARATEPROC)IntGetProcAddress("glStencilMaskSeparate");
        StencilMaskSeparate(face, mask);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_AttachShader(GLuint program, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)IntGetProcAddress("glAttachShader");
        AttachShader(program, shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint program, GLuint index, ध्रुव GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)IntGetProcAddress("glBindAttribLocation");
        BindAttribLocation(program, index, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)IntGetProcAddress("glCompileShader");
        CompileShader(shader);
    }

    स्थिर GLuint CODEGEN_FUNCPTR Switch_CreateProgram()
    {
        CreateProgram = (PFNCREATEPROGRAMPROC)IntGetProcAddress("glCreateProgram");
        प्रत्यागम CreateProgram();
    }

    स्थिर GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)IntGetProcAddress("glCreateShader");
        प्रत्यागम CreateShader(type);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteProgram(GLuint program)
    {
        DeleteProgram = (PFNDELETEPROGRAMPROC)IntGetProcAddress("glDeleteProgram");
        DeleteProgram(program);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)IntGetProcAddress("glDeleteShader");
        DeleteShader(shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DetachShader(GLuint program, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)IntGetProcAddress("glDetachShader");
        DetachShader(program, shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DisableVertexAttribArray(GLuint index)
    {
        DisableVertexAttribArray = (PFNDISABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glDisableVertexAttribArray");
        DisableVertexAttribArray(index);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EnableVertexAttribArray(GLuint index)
    {
        EnableVertexAttribArray = (PFNENABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glEnableVertexAttribArray");
        EnableVertexAttribArray(index);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveAttrib = (PFNGETACTIVEATTRIBPROC)IntGetProcAddress("glGetActiveAttrib");
        GetActiveAttrib(program, index, bufSize, length, size, type, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveUniform = (PFNGETACTIVEUNIFORMPROC)IntGetProcAddress("glGetActiveUniform");
        GetActiveUniform(program, index, bufSize, length, size, type, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint program, GLsizei max_गणनम्, GLsizei *गणनम्_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)IntGetProcAddress("glGetAttachedShaders");
        GetAttachedShaders(program, max_गणनम्, गणनम्_2, obj);
    }

    स्थिर GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint program, ध्रुव GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)IntGetProcAddress("glGetAttribLocation");
        प्रत्यागम GetAttribLocation(program, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint program, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)IntGetProcAddress("glGetProgramiv");
        GetProgramiv(program, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
    {
        GetProgramInfoLog = (PFNGETPROGRAMINFOLOGPROC)IntGetProcAddress("glGetProgramInfoLog");
        GetProgramInfoLog(program, bufSize, length, infoLog);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)IntGetProcAddress("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)IntGetProcAddress("glGetShaderInfoLog");
        GetShaderInfoLog(shader, bufSize, length, infoLog);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
    {
        GetShaderSource = (PFNGETSHADERSOURCEPROC)IntGetProcAddress("glGetShaderSource");
        GetShaderSource(shader, bufSize, length, source);
    }

    स्थिर GLint CODEGEN_FUNCPTR Switch_GetUniformLocation(GLuint program, ध्रुव GLchar *name)
    {
        GetUniformLocation = (PFNGETUNIFORMLOCATIONPROC)IntGetProcAddress("glGetUniformLocation");
        प्रत्यागम GetUniformLocation(program, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint program, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)IntGetProcAddress("glGetUniformfv");
        GetUniformfv(program, location, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint program, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)IntGetProcAddress("glGetUniformiv");
        GetUniformiv(program, location, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params)
    {
        GetVertexAttribdv = (PFNGETVERTEXATTRIBDVPROC)IntGetProcAddress("glGetVertexAttribdv");
        GetVertexAttribdv(index, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
    {
        GetVertexAttribfv = (PFNGETVERTEXATTRIBFVPROC)IntGetProcAddress("glGetVertexAttribfv");
        GetVertexAttribfv(index, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
    {
        GetVertexAttribiv = (PFNGETVERTEXATTRIBIVPROC)IntGetProcAddress("glGetVertexAttribiv");
        GetVertexAttribiv(index, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid* *pointer)
    {
        GetVertexAttribPointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)IntGetProcAddress("glGetVertexAttribPointerv");
        GetVertexAttribPointerv(index, pname, pointer);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsProgram(GLuint program)
    {
        IsProgram = (PFNISPROGRAMPROC)IntGetProcAddress("glIsProgram");
        प्रत्यागम IsProgram(program);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)IntGetProcAddress("glIsShader");
        प्रत्यागम IsShader(shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_LinkProgram(GLuint program)
    {
        LinkProgram = (PFNLINKPROGRAMPROC)IntGetProcAddress("glLinkProgram");
        LinkProgram(program);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ShaderSource(GLuint shader, GLsizei गणनम्_2, ध्रुव GLchar* ध्रुव *string, ध्रुव GLint *length)
    {
        ShaderSource = (PFNSHADERSOURCEPROC)IntGetProcAddress("glShaderSource");
        ShaderSource(shader, गणनम्_2, string, length);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UseProgram(GLuint program)
    {
        UseProgram = (PFNUSEPROGRAMPROC)IntGetProcAddress("glUseProgram");
        UseProgram(program);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)IntGetProcAddress("glUniform1f");
        Uniform1f(location, v0);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)IntGetProcAddress("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)IntGetProcAddress("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)IntGetProcAddress("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)IntGetProcAddress("glUniform1i");
        Uniform1i(location, v0);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)IntGetProcAddress("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)IntGetProcAddress("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)IntGetProcAddress("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)IntGetProcAddress("glUniform1fv");
        Uniform1fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)IntGetProcAddress("glUniform2fv");
        Uniform2fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)IntGetProcAddress("glUniform3fv");
        Uniform3fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)IntGetProcAddress("glUniform4fv");
        Uniform4fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)IntGetProcAddress("glUniform1iv");
        Uniform1iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)IntGetProcAddress("glUniform2iv");
        Uniform2iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)IntGetProcAddress("glUniform3iv");
        Uniform3iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)IntGetProcAddress("glUniform4iv");
        Uniform4iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix2fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix2fv = (PFNUNIFORMMATRIX2FVPROC)IntGetProcAddress("glUniformMatrix2fv");
        UniformMatrix2fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix3fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix3fv = (PFNUNIFORMMATRIX3FVPROC)IntGetProcAddress("glUniformMatrix3fv");
        UniformMatrix3fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix4fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix4fv = (PFNUNIFORMMATRIX4FVPROC)IntGetProcAddress("glUniformMatrix4fv");
        UniformMatrix4fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ValidateProgram(GLuint program)
    {
        ValidateProgram = (PFNVALIDATEPROGRAMPROC)IntGetProcAddress("glValidateProgram");
        ValidateProgram(program);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ध्रुव GLvoid *pointer)
    {
        VertexAttribPointer = (PFNVERTEXATTRIBPOINTERPROC)IntGetProcAddress("glVertexAttribPointer");
        VertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix2x3fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)IntGetProcAddress("glUniformMatrix2x3fv");
        UniformMatrix2x3fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix3x2fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)IntGetProcAddress("glUniformMatrix3x2fv");
        UniformMatrix3x2fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix2x4fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)IntGetProcAddress("glUniformMatrix2x4fv");
        UniformMatrix2x4fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix4x2fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)IntGetProcAddress("glUniformMatrix4x2fv");
        UniformMatrix4x2fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix3x4fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)IntGetProcAddress("glUniformMatrix3x4fv");
        UniformMatrix3x4fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformMatrix4x3fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        UniformMatrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)IntGetProcAddress("glUniformMatrix4x3fv");
        UniformMatrix4x3fv(location, गणनम्_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindVertexArray(GLuint ren_array)
    {
        BindVertexArray = (PFNBINDVERTEXARRAYPROC)IntGetProcAddress("glBindVertexArray");
        BindVertexArray(ren_array);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteVertexArrays(GLsizei n, ध्रुव GLuint *arrays)
    {
        DeleteVertexArrays = (PFNDELETEVERTEXARRAYSPROC)IntGetProcAddress("glDeleteVertexArrays");
        DeleteVertexArrays(n, arrays);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenVertexArrays(GLsizei n, GLuint *arrays)
    {
        GenVertexArrays = (PFNGENVERTEXARRAYSPROC)IntGetProcAddress("glGenVertexArrays");
        GenVertexArrays(n, arrays);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsVertexArray(GLuint ren_array)
    {
        IsVertexArray = (PFNISVERTEXARRAYPROC)IntGetProcAddress("glIsVertexArray");
        प्रत्यागम IsVertexArray(ren_array);
    }

    // Extension: ARB_map_buffer_range

    स्थिर GLvoid* CODEGEN_FUNCPTR Switch_MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        MapBufferRange = (PFNMAPBUFFERRANGEPROC)IntGetProcAddress("glMapBufferRange");
        प्रत्यागम MapBufferRange(target, offset, length, access);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        FlushMappedBufferRange = (PFNFLUSHMAPPEDBUFFERRANGEPROC)IntGetProcAddress("glFlushMappedBufferRange");
        FlushMappedBufferRange(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)IntGetProcAddress("glIsRenderbuffer");
        प्रत्यागम IsRenderbuffer(renderbuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)IntGetProcAddress("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ध्रुव GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)IntGetProcAddress("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)IntGetProcAddress("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)IntGetProcAddress("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, width, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)IntGetProcAddress("glIsFramebuffer");
        प्रत्यागम IsFramebuffer(framebuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)IntGetProcAddress("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ध्रुव GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)IntGetProcAddress("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)IntGetProcAddress("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    स्थिर GLenum CODEGEN_FUNCPTR Switch_CheckFramebufferStatus(GLenum target)
    {
        CheckFramebufferStatus = (PFNCHECKFRAMEBUFFERSTATUSPROC)IntGetProcAddress("glCheckFramebufferStatus");
        प्रत्यागम CheckFramebufferStatus(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)IntGetProcAddress("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)IntGetProcAddress("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)IntGetProcAddress("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)IntGetProcAddress("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)IntGetProcAddress("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)IntGetProcAddress("glGenerateMipmap");
        GenerateMipmap(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)IntGetProcAddress("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)IntGetProcAddress("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, width, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)IntGetProcAddress("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)IntGetProcAddress("glColorMaski");
        ColorMaski(index, r, g, b, a);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint index, GLboolean *data)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)IntGetProcAddress("glGetBooleani_v");
        GetBooleani_v(target, index, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint index, GLint *data)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)IntGetProcAddress("glGetIntegeri_v");
        GetIntegeri_v(target, index, data);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint index)
    {
        Enablei = (PFNENABLEIPROC)IntGetProcAddress("glEnablei");
        Enablei(target, index);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint index)
    {
        Disablei = (PFNDISABLEIPROC)IntGetProcAddress("glDisablei");
        Disablei(target, index);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint index)
    {
        IsEnabledi = (PFNISENABLEDIPROC)IntGetProcAddress("glIsEnabledi");
        प्रत्यागम IsEnabledi(target, index);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitiveMode)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)IntGetProcAddress("glBeginTransformFeedback");
        BeginTransformFeedback(primitiveMode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EndTransformFeedback()
    {
        EndTransformFeedback = (PFNENDTRANSFORMFEEDBACKPROC)IntGetProcAddress("glEndTransformFeedback");
        EndTransformFeedback();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
    {
        BindBufferRange = (PFNBINDBUFFERRANGEPROC)IntGetProcAddress("glBindBufferRange");
        BindBufferRange(target, index, buffer, offset, size);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindBufferBase(GLenum target, GLuint index, GLuint buffer)
    {
        BindBufferBase = (PFNBINDBUFFERBASEPROC)IntGetProcAddress("glBindBufferBase");
        BindBufferBase(target, index, buffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint program, GLsizei गणनम्_2, ध्रुव GLchar* ध्रुव *varyings, GLenum bufferMode)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)IntGetProcAddress("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(program, गणनम्_2, varyings, bufferMode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)IntGetProcAddress("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)IntGetProcAddress("glClampColor");
        ClampColor(target, clamp);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum mode)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)IntGetProcAddress("glBeginConditionalRender");
        BeginConditionalRender(id, mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EndConditionalRender()
    {
        EndConditionalRender = (PFNENDCONDITIONALRENDERPROC)IntGetProcAddress("glEndConditionalRender");
        EndConditionalRender();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *pointer)
    {
        VertexAttribIPointer = (PFNVERTEXATTRIBIPOINTERPROC)IntGetProcAddress("glVertexAttribIPointer");
        VertexAttribIPointer(index, size, type, stride, pointer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribIiv(GLuint index, GLenum pname, GLint *params)
    {
        GetVertexAttribIiv = (PFNGETVERTEXATTRIBIIVPROC)IntGetProcAddress("glGetVertexAttribIiv");
        GetVertexAttribIiv(index, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params)
    {
        GetVertexAttribIuiv = (PFNGETVERTEXATTRIBIUIVPROC)IntGetProcAddress("glGetVertexAttribIuiv");
        GetVertexAttribIuiv(index, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1i(GLuint index, GLint x)
    {
        VertexAttribI1i = (PFNVERTEXATTRIBI1IPROC)IntGetProcAddress("glVertexAttribI1i");
        VertexAttribI1i(index, x);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2i(GLuint index, GLint x, GLint y)
    {
        VertexAttribI2i = (PFNVERTEXATTRIBI2IPROC)IntGetProcAddress("glVertexAttribI2i");
        VertexAttribI2i(index, x, y);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3i(GLuint index, GLint x, GLint y, GLint z)
    {
        VertexAttribI3i = (PFNVERTEXATTRIBI3IPROC)IntGetProcAddress("glVertexAttribI3i");
        VertexAttribI3i(index, x, y, z);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
    {
        VertexAttribI4i = (PFNVERTEXATTRIBI4IPROC)IntGetProcAddress("glVertexAttribI4i");
        VertexAttribI4i(index, x, y, z, w);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1ui(GLuint index, GLuint x)
    {
        VertexAttribI1ui = (PFNVERTEXATTRIBI1UIPROC)IntGetProcAddress("glVertexAttribI1ui");
        VertexAttribI1ui(index, x);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2ui(GLuint index, GLuint x, GLuint y)
    {
        VertexAttribI2ui = (PFNVERTEXATTRIBI2UIPROC)IntGetProcAddress("glVertexAttribI2ui");
        VertexAttribI2ui(index, x, y);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z)
    {
        VertexAttribI3ui = (PFNVERTEXATTRIBI3UIPROC)IntGetProcAddress("glVertexAttribI3ui");
        VertexAttribI3ui(index, x, y, z);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        VertexAttribI4ui = (PFNVERTEXATTRIBI4UIPROC)IntGetProcAddress("glVertexAttribI4ui");
        VertexAttribI4ui(index, x, y, z, w);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1iv(GLuint index, ध्रुव GLint *v)
    {
        VertexAttribI1iv = (PFNVERTEXATTRIBI1IVPROC)IntGetProcAddress("glVertexAttribI1iv");
        VertexAttribI1iv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2iv(GLuint index, ध्रुव GLint *v)
    {
        VertexAttribI2iv = (PFNVERTEXATTRIBI2IVPROC)IntGetProcAddress("glVertexAttribI2iv");
        VertexAttribI2iv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3iv(GLuint index, ध्रुव GLint *v)
    {
        VertexAttribI3iv = (PFNVERTEXATTRIBI3IVPROC)IntGetProcAddress("glVertexAttribI3iv");
        VertexAttribI3iv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4iv(GLuint index, ध्रुव GLint *v)
    {
        VertexAttribI4iv = (PFNVERTEXATTRIBI4IVPROC)IntGetProcAddress("glVertexAttribI4iv");
        VertexAttribI4iv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1uiv(GLuint index, ध्रुव GLuint *v)
    {
        VertexAttribI1uiv = (PFNVERTEXATTRIBI1UIVPROC)IntGetProcAddress("glVertexAttribI1uiv");
        VertexAttribI1uiv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2uiv(GLuint index, ध्रुव GLuint *v)
    {
        VertexAttribI2uiv = (PFNVERTEXATTRIBI2UIVPROC)IntGetProcAddress("glVertexAttribI2uiv");
        VertexAttribI2uiv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3uiv(GLuint index, ध्रुव GLuint *v)
    {
        VertexAttribI3uiv = (PFNVERTEXATTRIBI3UIVPROC)IntGetProcAddress("glVertexAttribI3uiv");
        VertexAttribI3uiv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4uiv(GLuint index, ध्रुव GLuint *v)
    {
        VertexAttribI4uiv = (PFNVERTEXATTRIBI4UIVPROC)IntGetProcAddress("glVertexAttribI4uiv");
        VertexAttribI4uiv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4bv(GLuint index, ध्रुव GLbyte *v)
    {
        VertexAttribI4bv = (PFNVERTEXATTRIBI4BVPROC)IntGetProcAddress("glVertexAttribI4bv");
        VertexAttribI4bv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4sv(GLuint index, ध्रुव GLshort *v)
    {
        VertexAttribI4sv = (PFNVERTEXATTRIBI4SVPROC)IntGetProcAddress("glVertexAttribI4sv");
        VertexAttribI4sv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4ubv(GLuint index, ध्रुव GLubyte *v)
    {
        VertexAttribI4ubv = (PFNVERTEXATTRIBI4UBVPROC)IntGetProcAddress("glVertexAttribI4ubv");
        VertexAttribI4ubv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4usv(GLuint index, ध्रुव GLushort *v)
    {
        VertexAttribI4usv = (PFNVERTEXATTRIBI4USVPROC)IntGetProcAddress("glVertexAttribI4usv");
        VertexAttribI4usv(index, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint program, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)IntGetProcAddress("glGetUniformuiv");
        GetUniformuiv(program, location, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindFragDataLocation(GLuint program, GLuint color, ध्रुव GLchar *name)
    {
        BindFragDataLocation = (PFNBINDFRAGDATALOCATIONPROC)IntGetProcAddress("glBindFragDataLocation");
        BindFragDataLocation(program, color, name);
    }

    स्थिर GLint CODEGEN_FUNCPTR Switch_GetFragDataLocation(GLuint program, ध्रुव GLchar *name)
    {
        GetFragDataLocation = (PFNGETFRAGDATALOCATIONPROC)IntGetProcAddress("glGetFragDataLocation");
        प्रत्यागम GetFragDataLocation(program, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)IntGetProcAddress("glUniform1ui");
        Uniform1ui(location, v0);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)IntGetProcAddress("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)IntGetProcAddress("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)IntGetProcAddress("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)IntGetProcAddress("glUniform1uiv");
        Uniform1uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)IntGetProcAddress("glUniform2uiv");
        Uniform2uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)IntGetProcAddress("glUniform3uiv");
        Uniform3uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)IntGetProcAddress("glUniform4uiv");
        Uniform4uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ध्रुव GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)IntGetProcAddress("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ध्रुव GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)IntGetProcAddress("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)IntGetProcAddress("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)IntGetProcAddress("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum buffer, GLint drawbuffer, ध्रुव GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)IntGetProcAddress("glClearBufferiv");
        ClearBufferiv(buffer, drawbuffer, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum buffer, GLint drawbuffer, ध्रुव GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)IntGetProcAddress("glClearBufferuiv");
        ClearBufferuiv(buffer, drawbuffer, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum buffer, GLint drawbuffer, ध्रुव GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)IntGetProcAddress("glClearBufferfv");
        ClearBufferfv(buffer, drawbuffer, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)IntGetProcAddress("glClearBufferfi");
        ClearBufferfi(buffer, drawbuffer, depth, stencil);
    }

    स्थिर ध्रुव GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint index)
    {
        GetStringi = (PFNGETSTRINGIPROC)IntGetProcAddress("glGetStringi");
        प्रत्यागम GetStringi(name, index);
    }

    // Extension: ARB_uniform_buffer_object

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformIndices(GLuint program, GLsizei uniform_गणनम्, ध्रुव GLchar* ध्रुव *uniformNames, GLuint *uniformIndices)
    {
        GetUniformIndices = (PFNGETUNIFORMINDICESPROC)IntGetProcAddress("glGetUniformIndices");
        GetUniformIndices(program, uniform_गणनम्, uniformNames, uniformIndices);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetActiveUniformsiv(GLuint program, GLsizei uniform_गणनम्, ध्रुव GLuint *uniformIndices, GLenum pname, GLint *params)
    {
        GetActiveUniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)IntGetProcAddress("glGetActiveUniformsiv");
        GetActiveUniformsiv(program, uniform_गणनम्, uniformIndices, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName)
    {
        GetActiveUniformName = (PFNGETACTIVEUNIFORMNAMEPROC)IntGetProcAddress("glGetActiveUniformName");
        GetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
    }

    स्थिर GLuint CODEGEN_FUNCPTR Switch_GetUniformBlockIndex(GLuint program, ध्रुव GLchar *uniformBlockName)
    {
        GetUniformBlockIndex = (PFNGETUNIFORMBLOCKINDEXPROC)IntGetProcAddress("glGetUniformBlockIndex");
        प्रत्यागम GetUniformBlockIndex(program, uniformBlockName);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
    {
        GetActiveUniformBlockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)IntGetProcAddress("glGetActiveUniformBlockiv");
        GetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
    {
        GetActiveUniformBlockName = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)IntGetProcAddress("glGetActiveUniformBlockName");
        GetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
    {
        UniformBlockBinding = (PFNUNIFORMBLOCKBINDINGPROC)IntGetProcAddress("glUniformBlockBinding");
        UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    }

    // Extension: ARB_copy_buffer

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
    {
        CopyBufferSubData = (PFNCOPYBUFFERSUBDATAPROC)IntGetProcAddress("glCopyBufferSubData");
        CopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    }

    // Extension: 3.1

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum mode, GLint first, GLsizei गणनम्_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)IntGetProcAddress("glDrawArraysInstanced");
        DrawArraysInstanced(mode, first, गणनम्_2, instancecount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum mode, GLsizei गणनम्_2, GLenum type, ध्रुव GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)IntGetProcAddress("glDrawElementsInstanced");
        DrawElementsInstanced(mode, गणनम्_2, type, indices, instancecount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexBuffer(GLenum target, GLenum internalformat, GLuint buffer)
    {
        TexBuffer = (PFNTEXBUFFERPROC)IntGetProcAddress("glTexBuffer");
        TexBuffer(target, internalformat, buffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PrimitiveRestartIndex(GLuint index)
    {
        PrimitiveRestartIndex = (PFNPRIMITIVERESTARTINDEXPROC)IntGetProcAddress("glPrimitiveRestartIndex");
        PrimitiveRestartIndex(index);
    }

    // Legacy

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)IntGetProcAddress("glEnableClientState");
        EnableClientState(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)IntGetProcAddress("glDisableClientState");
        DisableClientState(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexPointer(GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        VertexPointer = (PFNVERTEXPOINTERPROC)IntGetProcAddress("glVertexPointer");
        VertexPointer(size, type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_NormalPointer(GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        NormalPointer = (PFNNORMALPOINTERPROC)IntGetProcAddress("glNormalPointer");
        NormalPointer(type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ColorPointer(GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        ColorPointer = (PFNCOLORPOINTERPROC)IntGetProcAddress("glColorPointer");
        ColorPointer(size, type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexCoordPointer(GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        TexCoordPointer = (PFNTEXCOORDPOINTERPROC)IntGetProcAddress("glTexCoordPointer");
        TexCoordPointer(size, type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)IntGetProcAddress("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_MatrixMode(GLenum mode)
    {
        MatrixMode = (PFNMATRIXMODEPROC)IntGetProcAddress("glMatrixMode");
        MatrixMode(mode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_LoadIdentity(शून्य)
    {
        LoadIdentity = (PFNLOADIDENTITYPROC)IntGetProcAddress("glLoadIdentity");
        LoadIdentity();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Ortho(GLdouble वामः, GLdouble दक्षिणः, GLdouble bottom, GLdouble शिखरम्_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)IntGetProcAddress("glOrtho");
        Ortho(वामः, दक्षिणः, bottom, शिखरम्_2, near_val, far_val);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)IntGetProcAddress("glColor3d");
        Color3d(red, green, blue);
    }

    संरचना InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            पङ्क्तिः_Width = Switch_पङ्क्तिः_Width;
            PointSize = Switch_PointSize;
            PolygonMode = Switch_PolygonMode;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            DrawBuffer = Switch_DrawBuffer;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            ClearDepth = Switch_ClearDepth;
            StencilMask = Switch_StencilMask;
            ColorMask = Switch_ColorMask;
            DepthMask = Switch_DepthMask;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            Finish = Switch_Finish;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            DepthFunc = Switch_DepthFunc;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            ReadBuffer = Switch_ReadBuffer;
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
            DepthRange = Switch_DepthRange;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            PolygonOffset = Switch_PolygonOffset;
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
            ActiveTexture = Switch_ActiveTexture;
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
            PointParameterf = Switch_PointParameterf;
            PointParameterfv = Switch_PointParameterfv;
            PointParameteri = Switch_PointParameteri;
            PointParameteriv = Switch_PointParameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            EndQuery = Switch_EndQuery;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            BindBuffer = Switch_BindBuffer;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            IsBuffer = Switch_IsBuffer;
            BufferData = Switch_BufferData;
            BufferSubData = Switch_BufferSubData;
            GetBufferSubData = Switch_GetBufferSubData;
            MapBuffer = Switch_MapBuffer;
            UnmapBuffer = Switch_UnmapBuffer;
            GetBufferParameteriv = Switch_GetBufferParameteriv;
            GetBufferPointerv = Switch_GetBufferPointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            StencilMaskSeparate = Switch_StencilMaskSeparate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            CreateProgram = Switch_CreateProgram;
            CreateShader = Switch_CreateShader;
            DeleteProgram = Switch_DeleteProgram;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            DisableVertexAttribArray = Switch_DisableVertexAttribArray;
            EnableVertexAttribArray = Switch_EnableVertexAttribArray;
            GetActiveAttrib = Switch_GetActiveAttrib;
            GetActiveUniform = Switch_GetActiveUniform;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            GetProgramInfoLog = Switch_GetProgramInfoLog;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            GetShaderSource = Switch_GetShaderSource;
            GetUniformLocation = Switch_GetUniformLocation;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            GetVertexAttribdv = Switch_GetVertexAttribdv;
            GetVertexAttribfv = Switch_GetVertexAttribfv;
            GetVertexAttribiv = Switch_GetVertexAttribiv;
            GetVertexAttribPointerv = Switch_GetVertexAttribPointerv;
            IsProgram = Switch_IsProgram;
            IsShader = Switch_IsShader;
            LinkProgram = Switch_LinkProgram;
            ShaderSource = Switch_ShaderSource;
            UseProgram = Switch_UseProgram;
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
            UniformMatrix2fv = Switch_UniformMatrix2fv;
            UniformMatrix3fv = Switch_UniformMatrix3fv;
            UniformMatrix4fv = Switch_UniformMatrix4fv;
            ValidateProgram = Switch_ValidateProgram;
            VertexAttribPointer = Switch_VertexAttribPointer;

            // Extension: 2.1
            UniformMatrix2x3fv = Switch_UniformMatrix2x3fv;
            UniformMatrix3x2fv = Switch_UniformMatrix3x2fv;
            UniformMatrix2x4fv = Switch_UniformMatrix2x4fv;
            UniformMatrix4x2fv = Switch_UniformMatrix4x2fv;
            UniformMatrix3x4fv = Switch_UniformMatrix3x4fv;
            UniformMatrix4x3fv = Switch_UniformMatrix4x3fv;

            // Extension: ARB_vertex_array_object
            BindVertexArray = Switch_BindVertexArray;
            DeleteVertexArrays = Switch_DeleteVertexArrays;
            GenVertexArrays = Switch_GenVertexArrays;
            IsVertexArray = Switch_IsVertexArray;

            // Extension: ARB_map_buffer_range
            MapBufferRange = Switch_MapBufferRange;
            FlushMappedBufferRange = Switch_FlushMappedBufferRange;

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
            CheckFramebufferStatus = Switch_CheckFramebufferStatus;
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
            EndTransformFeedback = Switch_EndTransformFeedback;
            BindBufferRange = Switch_BindBufferRange;
            BindBufferBase = Switch_BindBufferBase;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            EndConditionalRender = Switch_EndConditionalRender;
            VertexAttribIPointer = Switch_VertexAttribIPointer;
            GetVertexAttribIiv = Switch_GetVertexAttribIiv;
            GetVertexAttribIuiv = Switch_GetVertexAttribIuiv;
            VertexAttribI1i = Switch_VertexAttribI1i;
            VertexAttribI2i = Switch_VertexAttribI2i;
            VertexAttribI3i = Switch_VertexAttribI3i;
            VertexAttribI4i = Switch_VertexAttribI4i;
            VertexAttribI1ui = Switch_VertexAttribI1ui;
            VertexAttribI2ui = Switch_VertexAttribI2ui;
            VertexAttribI3ui = Switch_VertexAttribI3ui;
            VertexAttribI4ui = Switch_VertexAttribI4ui;
            VertexAttribI1iv = Switch_VertexAttribI1iv;
            VertexAttribI2iv = Switch_VertexAttribI2iv;
            VertexAttribI3iv = Switch_VertexAttribI3iv;
            VertexAttribI4iv = Switch_VertexAttribI4iv;
            VertexAttribI1uiv = Switch_VertexAttribI1uiv;
            VertexAttribI2uiv = Switch_VertexAttribI2uiv;
            VertexAttribI3uiv = Switch_VertexAttribI3uiv;
            VertexAttribI4uiv = Switch_VertexAttribI4uiv;
            VertexAttribI4bv = Switch_VertexAttribI4bv;
            VertexAttribI4sv = Switch_VertexAttribI4sv;
            VertexAttribI4ubv = Switch_VertexAttribI4ubv;
            VertexAttribI4usv = Switch_VertexAttribI4usv;
            GetUniformuiv = Switch_GetUniformuiv;
            BindFragDataLocation = Switch_BindFragDataLocation;
            GetFragDataLocation = Switch_GetFragDataLocation;
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
            GetUniformIndices = Switch_GetUniformIndices;
            GetActiveUniformsiv = Switch_GetActiveUniformsiv;
            GetActiveUniformName = Switch_GetActiveUniformName;
            GetUniformBlockIndex = Switch_GetUniformBlockIndex;
            GetActiveUniformBlockiv = Switch_GetActiveUniformBlockiv;
            GetActiveUniformBlockName = Switch_GetActiveUniformBlockName;
            UniformBlockBinding = Switch_UniformBlockBinding;

            // Extension: ARB_copy_buffer
            CopyBufferSubData = Switch_CopyBufferSubData;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            TexBuffer = Switch_TexBuffer;
            PrimitiveRestartIndex = Switch_PrimitiveRestartIndex;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            VertexPointer = Switch_VertexPointer;
            NormalPointer = Switch_NormalPointer;
            ColorPointer = Switch_ColorPointer;
            TexCoordPointer = Switch_TexCoordPointer;
            TexEnvi = Switch_TexEnvi;
            MatrixMode = Switch_MatrixMode;
            LoadIdentity = Switch_LoadIdentity;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
