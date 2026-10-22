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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "gl_core_3_1.hpp"

#ifdef HAVE_OPENGL

    #ifdef __APPLE__
        #அந்தர்பாவ <dlfcn.h>

        நிலையான வெறுமை* AppleGLGetProcAddress (மாறிலி எழுத்து* name)
        {
            நிலையான தருக்க initialized = பொய்;
            நிலையான வெறுமை * handle = வெற்று;
            எனில் (!handle)
            {
                எனில் (!initialized)
                {
                    initialized = உண்மை;
                    மாறிலி எழுத்து * மாறிலி path = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
                }
                எனில் (!handle)
                    திரும்பு வெற்று;
            }
            திரும்பு dlsym(handle, name);
        }
    #endif // __APPLE__

    #எனில் defined(__sgi) || defined (__sun)
        #அந்தர்பாவ <dlfcn.h>
        #அந்தர்பாவ <stdio.h>

        நிலையான வெறுமை* SunGetProcAddress (மாறிலி எழுத்து* name)
        {
            வகைவரை வெறுமை* (func_t*)(மாறிலி GLubyte*);

            நிலையான வெறுமை* h = 0;
            நிலையான func_t gpa = 0;

            எனில் (!h)
            {
                h = dlopen(வெற்று, RTLD_LAZY | RTLD_LOCAL);
                எனில் (!h)
                    திரும்பு 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            திரும்பு gpa ? gpa((மாறிலி GLubyte*) name) : dlsym(h, name);
        }
    #endif // __sgi || __sun

    #எனில் defined(_WIN32)
        #ifdef _MSC_VER
            #pragma warning(disable: 4055)
            #pragma warning(disable: 4054)
        #endif

        நிலையான முழு TestPointer(மாறிலி PROC pTest)
        {
            எனில்(!pTest)
                திரும்பு 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            எனில் (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                திரும்பு 0;

            திரும்பு 1;
        }

        நிலையான PROC WinGetProcAddress(மாறிலி எழுத்து* name)
        {
            PROC pFunc = wglGetProcAddress((LPCSTR) name);
            எனில் (TestPointer(pFunc))
                திரும்பு pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            திரும்பு (PROC) GetProcAddress(glMod, (LPCSTR) name);
        }
    #endif // _WIN32

    #எனில் defined(_WIN32)
        #define CV_GL_GET_PROC_ADDRESS(name) WinGetProcAddress(name)
    #அல்லதெனில் defined(__APPLE__)
        #define CV_GL_GET_PROC_ADDRESS(name) AppleGLGetProcAddress(name)
    #அல்லதெனில் defined(__sgi) || defined(__sun)
        #define CV_GL_GET_PROC_ADDRESS(name) SunGetProcAddress(name)
    #இல்லையெனில் // GLX
        #அந்தர்பாவ <GL/glx.h>

        #define CV_GL_GET_PROC_ADDRESS(name) glXGetProcAddressARB((மாறிலி GLubyte*) name)
    #endif

    நிலையான வெறுமை* IntGetProcAddress(மாறிலி எழுத்து* name)
    {
        வெறுமை* func =  (வெறுமை*) CV_GL_GET_PROC_ADDRESS(name);
        எனில் (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        திரும்பு func;
    }
#இல்லையெனில்
#எனில் defined(_MSC_VER)
    #pragma warning(disable : 4702)  // unreachable code
#endif
    நிலையான வெறுமை* IntGetProcAddress(மாறிலி எழுத்து*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#endif

நாமாகாஶ gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    வகைவரை GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    வகைவரை மாறிலி GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(மாறிலி GLubyte *);

    // Extension: 1.2
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , மாறிலி GLint *, மாறிலி GLsizei *, GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , மாறிலி GLsizei *, GLenum , மாறிலி GLvoid* மாறிலி *, GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , மாறிலி GLint *);

    // Extension: 1.5
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , மாறிலி GLuint *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , மாறிலி GLvoid *, GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    வகைவரை GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , மாறிலி GLenum *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , மாறிலி GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    வகைவரை GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    வகைவரை GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    வகைவரை GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , மாறிலி GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    வகைவரை GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , மாறிலி GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , மாறிலி GLchar* மாறிலி *, மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , மாறிலி GLvoid *);

    // Extension: 2.1
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , மாறிலி GLfloat *);

    // Extension: ARB_vertex_array_object
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    வகைவரை GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    வகைவரை GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    வகைவரை GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , மாறிலி GLchar* மாறிலி *, GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , மாறிலி GLbyte *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , மாறிலி GLshort *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , மாறிலி GLubyte *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , மாறிலி GLushort *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , மாறிலி GLchar *);
    வகைவரை GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , மாறிலி GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , மாறிலி GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , மாறிலி GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , மாறிலி GLfloat *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    வகைவரை மாறிலி GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , மாறிலி GLchar* மாறிலி *, GLuint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , மாறிலி GLuint *, GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    வகைவரை GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , மாறிலி GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , மாறிலி GLvoid *, GLsizei );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , மாறிலி GLvoid *);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(வெறுமை);
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    வகைவரை வெறுமை (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC வரி_Width;
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

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CullFace(GLenum mode)
    {
        CullFace = (PFNCULLFACEPROC)IntGetProcAddress("glCullFace");
        CullFace(mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FrontFace(GLenum mode)
    {
        FrontFace = (PFNFRONTFACEPROC)IntGetProcAddress("glFrontFace");
        FrontFace(mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum mode)
    {
        Hint = (PFNHINTPROC)IntGetProcAddress("glHint");
        Hint(target, mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_வரி_Width(GLfloat width)
    {
        வரி_Width = (PFNLINEWIDTHPROC)IntGetProcAddress("glLineWidth");
        வரி_Width(width);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PointSize(GLfloat size)
    {
        PointSize = (PFNPOINTSIZEPROC)IntGetProcAddress("glPointSize");
        PointSize(size);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PolygonMode(GLenum face, GLenum mode)
    {
        PolygonMode = (PFNPOLYGONMODEPROC)IntGetProcAddress("glPolygonMode");
        PolygonMode(face, mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Scissor = (PFNSCISSORPROC)IntGetProcAddress("glScissor");
        Scissor(x, y, width, height);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)IntGetProcAddress("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, மாறிலி GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)IntGetProcAddress("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)IntGetProcAddress("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, மாறிலி GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)IntGetProcAddress("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, மாறிலி GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)IntGetProcAddress("glTexImage1D");
        TexImage1D(target, level, internalformat, width, border, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, மாறிலி GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)IntGetProcAddress("glTexImage2D");
        TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawBuffer(GLenum mode)
    {
        DrawBuffer = (PFNDRAWBUFFERPROC)IntGetProcAddress("glDrawBuffer");
        DrawBuffer(mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Clear(GLbitfield mask)
    {
        Clear = (PFNCLEARPROC)IntGetProcAddress("glClear");
        Clear(mask);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)IntGetProcAddress("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)IntGetProcAddress("glClearStencil");
        ClearStencil(s);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearDepth(GLdouble depth)
    {
        ClearDepth = (PFNCLEARDEPTHPROC)IntGetProcAddress("glClearDepth");
        ClearDepth(depth);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_StencilMask(GLuint mask)
    {
        StencilMask = (PFNSTENCILMASKPROC)IntGetProcAddress("glStencilMask");
        StencilMask(mask);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        ColorMask = (PFNCOLORMASKPROC)IntGetProcAddress("glColorMask");
        ColorMask(red, green, blue, alpha);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DepthMask(GLboolean flag)
    {
        DepthMask = (PFNDEPTHMASKPROC)IntGetProcAddress("glDepthMask");
        DepthMask(flag);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)IntGetProcAddress("glDisable");
        Disable(cap);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)IntGetProcAddress("glEnable");
        Enable(cap);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Finish()
    {
        Finish = (PFNFINISHPROC)IntGetProcAddress("glFinish");
        Finish();
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)IntGetProcAddress("glFlush");
        Flush();
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)IntGetProcAddress("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)IntGetProcAddress("glLogicOp");
        LogicOp(opcode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)IntGetProcAddress("glStencilFunc");
        StencilFunc(func, ref, mask);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)IntGetProcAddress("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DepthFunc(GLenum func)
    {
        DepthFunc = (PFNDEPTHFUNCPROC)IntGetProcAddress("glDepthFunc");
        DepthFunc(func);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)IntGetProcAddress("glPixelStoref");
        PixelStoref(pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)IntGetProcAddress("glPixelStorei");
        PixelStorei(pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ReadBuffer(GLenum mode)
    {
        ReadBuffer = (PFNREADBUFFERPROC)IntGetProcAddress("glReadBuffer");
        ReadBuffer(mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)IntGetProcAddress("glReadPixels");
        ReadPixels(x, y, width, height, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)IntGetProcAddress("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)IntGetProcAddress("glGetDoublev");
        GetDoublev(pname, params);
    }

    நிலையான GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)IntGetProcAddress("glGetError");
        திரும்பு GetError();
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)IntGetProcAddress("glGetFloatv");
        GetFloatv(pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)IntGetProcAddress("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    நிலையான மாறிலி GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)IntGetProcAddress("glGetString");
        திரும்பு GetString(name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)IntGetProcAddress("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)IntGetProcAddress("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)IntGetProcAddress("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)IntGetProcAddress("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)IntGetProcAddress("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)IntGetProcAddress("glIsEnabled");
        திரும்பு IsEnabled(cap);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DepthRange(GLdouble ren_near, GLdouble ren_far)
    {
        DepthRange = (PFNDEPTHRANGEPROC)IntGetProcAddress("glDepthRange");
        DepthRange(ren_near, ren_far);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Viewport = (PFNVIEWPORTPROC)IntGetProcAddress("glViewport");
        Viewport(x, y, width, height);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawArrays(GLenum mode, GLint first, GLsizei மொத்தம்_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)IntGetProcAddress("glDrawArrays");
        DrawArrays(mode, first, மொத்தம்_2);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawElements(GLenum mode, GLsizei மொத்தம்_2, GLenum type, மாறிலி GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)IntGetProcAddress("glDrawElements");
        DrawElements(mode, மொத்தம்_2, type, indices);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)IntGetProcAddress("glGetPointerv");
        GetPointerv(pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PolygonOffset(GLfloat factor, GLfloat units)
    {
        PolygonOffset = (PFNPOLYGONOFFSETPROC)IntGetProcAddress("glPolygonOffset");
        PolygonOffset(factor, units);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)IntGetProcAddress("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, width, border);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)IntGetProcAddress("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)IntGetProcAddress("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, width);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)IntGetProcAddress("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, மாறிலி GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)IntGetProcAddress("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, width, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, மாறிலி GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)IntGetProcAddress("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)IntGetProcAddress("glBindTexture");
        BindTexture(target, texture);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, மாறிலி GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)IntGetProcAddress("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)IntGetProcAddress("glGenTextures");
        GenTextures(n, textures);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)IntGetProcAddress("glIsTexture");
        திரும்பு IsTexture(texture);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)IntGetProcAddress("glIndexub");
        Indexub(c);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Indexubv(மாறிலி GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)IntGetProcAddress("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)IntGetProcAddress("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BlendEquation(GLenum mode)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)IntGetProcAddress("glBlendEquation");
        BlendEquation(mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei மொத்தம்_2, GLenum type, மாறிலி GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)IntGetProcAddress("glDrawRangeElements");
        DrawRangeElements(mode, start, end, மொத்தம்_2, type, indices);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, மாறிலி GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)IntGetProcAddress("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)IntGetProcAddress("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    }

    // Extension: 1.3

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ActiveTexture(GLenum texture)
    {
        ActiveTexture = (PFNACTIVETEXTUREPROC)IntGetProcAddress("glActiveTexture");
        ActiveTexture(texture);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)IntGetProcAddress("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, மாறிலி GLvoid *data)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)IntGetProcAddress("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, width, height, depth, border, imageSize, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, மாறிலி GLvoid *data)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)IntGetProcAddress("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, மாறிலி GLvoid *data)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)IntGetProcAddress("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, width, border, imageSize, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, மாறிலி GLvoid *data)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)IntGetProcAddress("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, imageSize, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, மாறிலி GLvoid *data)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)IntGetProcAddress("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, மாறிலி GLvoid *data)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)IntGetProcAddress("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, width, format, imageSize, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)IntGetProcAddress("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)IntGetProcAddress("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum mode, மாறிலி GLint *first, மாறிலி GLsizei *மொத்தம்_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)IntGetProcAddress("glMultiDrawArrays");
        MultiDrawArrays(mode, first, மொத்தம்_2, drawcount);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum mode, மாறிலி GLsizei *மொத்தம்_2, GLenum type, மாறிலி GLvoid* மாறிலி *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)IntGetProcAddress("glMultiDrawElements");
        MultiDrawElements(mode, மொத்தம்_2, type, indices, drawcount);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PointParameterf(GLenum pname, GLfloat param)
    {
        PointParameterf = (PFNPOINTPARAMETERFPROC)IntGetProcAddress("glPointParameterf");
        PointParameterf(pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PointParameterfv(GLenum pname, மாறிலி GLfloat *params)
    {
        PointParameterfv = (PFNPOINTPARAMETERFVPROC)IntGetProcAddress("glPointParameterfv");
        PointParameterfv(pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PointParameteri(GLenum pname, GLint param)
    {
        PointParameteri = (PFNPOINTPARAMETERIPROC)IntGetProcAddress("glPointParameteri");
        PointParameteri(pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PointParameteriv(GLenum pname, மாறிலி GLint *params)
    {
        PointParameteriv = (PFNPOINTPARAMETERIVPROC)IntGetProcAddress("glPointParameteriv");
        PointParameteriv(pname, params);
    }

    // Extension: 1.5

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)IntGetProcAddress("glGenQueries");
        GenQueries(n, ids);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, மாறிலி GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)IntGetProcAddress("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)IntGetProcAddress("glIsQuery");
        திரும்பு IsQuery(id);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)IntGetProcAddress("glBeginQuery");
        BeginQuery(target, id);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_EndQuery(GLenum target)
    {
        EndQuery = (PFNENDQUERYPROC)IntGetProcAddress("glEndQuery");
        EndQuery(target);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)IntGetProcAddress("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)IntGetProcAddress("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)IntGetProcAddress("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindBuffer(GLenum target, GLuint buffer)
    {
        BindBuffer = (PFNBINDBUFFERPROC)IntGetProcAddress("glBindBuffer");
        BindBuffer(target, buffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, மாறிலி GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)IntGetProcAddress("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)IntGetProcAddress("glGenBuffers");
        GenBuffers(n, buffers);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsBuffer(GLuint buffer)
    {
        IsBuffer = (PFNISBUFFERPROC)IntGetProcAddress("glIsBuffer");
        திரும்பு IsBuffer(buffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BufferData(GLenum target, GLsizeiptr size, மாறிலி GLvoid *data, GLenum usage)
    {
        BufferData = (PFNBUFFERDATAPROC)IntGetProcAddress("glBufferData");
        BufferData(target, size, data, usage);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, மாறிலி GLvoid *data)
    {
        BufferSubData = (PFNBUFFERSUBDATAPROC)IntGetProcAddress("glBufferSubData");
        BufferSubData(target, offset, size, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data)
    {
        GetBufferSubData = (PFNGETBUFFERSUBDATAPROC)IntGetProcAddress("glGetBufferSubData");
        GetBufferSubData(target, offset, size, data);
    }

    நிலையான GLvoid* CODEGEN_FUNCPTR Switch_MapBuffer(GLenum target, GLenum access)
    {
        MapBuffer = (PFNMAPBUFFERPROC)IntGetProcAddress("glMapBuffer");
        திரும்பு MapBuffer(target, access);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_UnmapBuffer(GLenum target)
    {
        UnmapBuffer = (PFNUNMAPBUFFERPROC)IntGetProcAddress("glUnmapBuffer");
        திரும்பு UnmapBuffer(target);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetBufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetBufferParameteriv = (PFNGETBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetBufferParameteriv");
        GetBufferParameteriv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetBufferPointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        GetBufferPointerv = (PFNGETBUFFERPOINTERVPROC)IntGetProcAddress("glGetBufferPointerv");
        GetBufferPointerv(target, pname, params);
    }

    // Extension: 2.0

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)IntGetProcAddress("glBlendEquationSeparate");
        BlendEquationSeparate(modeRGB, modeAlpha);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, மாறிலி GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)IntGetProcAddress("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)IntGetProcAddress("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)IntGetProcAddress("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, mask);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_StencilMaskSeparate(GLenum face, GLuint mask)
    {
        StencilMaskSeparate = (PFNSTENCILMASKSEPARATEPROC)IntGetProcAddress("glStencilMaskSeparate");
        StencilMaskSeparate(face, mask);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_AttachShader(GLuint program, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)IntGetProcAddress("glAttachShader");
        AttachShader(program, shader);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint program, GLuint index, மாறிலி GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)IntGetProcAddress("glBindAttribLocation");
        BindAttribLocation(program, index, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)IntGetProcAddress("glCompileShader");
        CompileShader(shader);
    }

    நிலையான GLuint CODEGEN_FUNCPTR Switch_CreateProgram()
    {
        CreateProgram = (PFNCREATEPROGRAMPROC)IntGetProcAddress("glCreateProgram");
        திரும்பு CreateProgram();
    }

    நிலையான GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)IntGetProcAddress("glCreateShader");
        திரும்பு CreateShader(type);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteProgram(GLuint program)
    {
        DeleteProgram = (PFNDELETEPROGRAMPROC)IntGetProcAddress("glDeleteProgram");
        DeleteProgram(program);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)IntGetProcAddress("glDeleteShader");
        DeleteShader(shader);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DetachShader(GLuint program, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)IntGetProcAddress("glDetachShader");
        DetachShader(program, shader);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DisableVertexAttribArray(GLuint index)
    {
        DisableVertexAttribArray = (PFNDISABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glDisableVertexAttribArray");
        DisableVertexAttribArray(index);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_EnableVertexAttribArray(GLuint index)
    {
        EnableVertexAttribArray = (PFNENABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glEnableVertexAttribArray");
        EnableVertexAttribArray(index);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveAttrib = (PFNGETACTIVEATTRIBPROC)IntGetProcAddress("glGetActiveAttrib");
        GetActiveAttrib(program, index, bufSize, length, size, type, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveUniform = (PFNGETACTIVEUNIFORMPROC)IntGetProcAddress("glGetActiveUniform");
        GetActiveUniform(program, index, bufSize, length, size, type, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint program, GLsizei max_மொத்தம், GLsizei *மொத்தம்_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)IntGetProcAddress("glGetAttachedShaders");
        GetAttachedShaders(program, max_மொத்தம், மொத்தம்_2, obj);
    }

    நிலையான GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint program, மாறிலி GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)IntGetProcAddress("glGetAttribLocation");
        திரும்பு GetAttribLocation(program, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint program, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)IntGetProcAddress("glGetProgramiv");
        GetProgramiv(program, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
    {
        GetProgramInfoLog = (PFNGETPROGRAMINFOLOGPROC)IntGetProcAddress("glGetProgramInfoLog");
        GetProgramInfoLog(program, bufSize, length, infoLog);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)IntGetProcAddress("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)IntGetProcAddress("glGetShaderInfoLog");
        GetShaderInfoLog(shader, bufSize, length, infoLog);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
    {
        GetShaderSource = (PFNGETSHADERSOURCEPROC)IntGetProcAddress("glGetShaderSource");
        GetShaderSource(shader, bufSize, length, source);
    }

    நிலையான GLint CODEGEN_FUNCPTR Switch_GetUniformLocation(GLuint program, மாறிலி GLchar *name)
    {
        GetUniformLocation = (PFNGETUNIFORMLOCATIONPROC)IntGetProcAddress("glGetUniformLocation");
        திரும்பு GetUniformLocation(program, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint program, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)IntGetProcAddress("glGetUniformfv");
        GetUniformfv(program, location, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint program, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)IntGetProcAddress("glGetUniformiv");
        GetUniformiv(program, location, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params)
    {
        GetVertexAttribdv = (PFNGETVERTEXATTRIBDVPROC)IntGetProcAddress("glGetVertexAttribdv");
        GetVertexAttribdv(index, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params)
    {
        GetVertexAttribfv = (PFNGETVERTEXATTRIBFVPROC)IntGetProcAddress("glGetVertexAttribfv");
        GetVertexAttribfv(index, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetVertexAttribiv(GLuint index, GLenum pname, GLint *params)
    {
        GetVertexAttribiv = (PFNGETVERTEXATTRIBIVPROC)IntGetProcAddress("glGetVertexAttribiv");
        GetVertexAttribiv(index, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid* *pointer)
    {
        GetVertexAttribPointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)IntGetProcAddress("glGetVertexAttribPointerv");
        GetVertexAttribPointerv(index, pname, pointer);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsProgram(GLuint program)
    {
        IsProgram = (PFNISPROGRAMPROC)IntGetProcAddress("glIsProgram");
        திரும்பு IsProgram(program);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)IntGetProcAddress("glIsShader");
        திரும்பு IsShader(shader);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_LinkProgram(GLuint program)
    {
        LinkProgram = (PFNLINKPROGRAMPROC)IntGetProcAddress("glLinkProgram");
        LinkProgram(program);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ShaderSource(GLuint shader, GLsizei மொத்தம்_2, மாறிலி GLchar* மாறிலி *string, மாறிலி GLint *length)
    {
        ShaderSource = (PFNSHADERSOURCEPROC)IntGetProcAddress("glShaderSource");
        ShaderSource(shader, மொத்தம்_2, string, length);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UseProgram(GLuint program)
    {
        UseProgram = (PFNUSEPROGRAMPROC)IntGetProcAddress("glUseProgram");
        UseProgram(program);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)IntGetProcAddress("glUniform1f");
        Uniform1f(location, v0);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)IntGetProcAddress("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)IntGetProcAddress("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)IntGetProcAddress("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)IntGetProcAddress("glUniform1i");
        Uniform1i(location, v0);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)IntGetProcAddress("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)IntGetProcAddress("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)IntGetProcAddress("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)IntGetProcAddress("glUniform1fv");
        Uniform1fv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)IntGetProcAddress("glUniform2fv");
        Uniform2fv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)IntGetProcAddress("glUniform3fv");
        Uniform3fv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)IntGetProcAddress("glUniform4fv");
        Uniform4fv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)IntGetProcAddress("glUniform1iv");
        Uniform1iv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)IntGetProcAddress("glUniform2iv");
        Uniform2iv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)IntGetProcAddress("glUniform3iv");
        Uniform3iv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)IntGetProcAddress("glUniform4iv");
        Uniform4iv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix2fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix2fv = (PFNUNIFORMMATRIX2FVPROC)IntGetProcAddress("glUniformMatrix2fv");
        UniformMatrix2fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix3fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix3fv = (PFNUNIFORMMATRIX3FVPROC)IntGetProcAddress("glUniformMatrix3fv");
        UniformMatrix3fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix4fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix4fv = (PFNUNIFORMMATRIX4FVPROC)IntGetProcAddress("glUniformMatrix4fv");
        UniformMatrix4fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ValidateProgram(GLuint program)
    {
        ValidateProgram = (PFNVALIDATEPROGRAMPROC)IntGetProcAddress("glValidateProgram");
        ValidateProgram(program);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, மாறிலி GLvoid *pointer)
    {
        VertexAttribPointer = (PFNVERTEXATTRIBPOINTERPROC)IntGetProcAddress("glVertexAttribPointer");
        VertexAttribPointer(index, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix2x3fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)IntGetProcAddress("glUniformMatrix2x3fv");
        UniformMatrix2x3fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix3x2fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)IntGetProcAddress("glUniformMatrix3x2fv");
        UniformMatrix3x2fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix2x4fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)IntGetProcAddress("glUniformMatrix2x4fv");
        UniformMatrix2x4fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix4x2fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)IntGetProcAddress("glUniformMatrix4x2fv");
        UniformMatrix4x2fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix3x4fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)IntGetProcAddress("glUniformMatrix3x4fv");
        UniformMatrix3x4fv(location, மொத்தம்_2, transpose, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformMatrix4x3fv(GLint location, GLsizei மொத்தம்_2, GLboolean transpose, மாறிலி GLfloat *value)
    {
        UniformMatrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)IntGetProcAddress("glUniformMatrix4x3fv");
        UniformMatrix4x3fv(location, மொத்தம்_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindVertexArray(GLuint ren_array)
    {
        BindVertexArray = (PFNBINDVERTEXARRAYPROC)IntGetProcAddress("glBindVertexArray");
        BindVertexArray(ren_array);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteVertexArrays(GLsizei n, மாறிலி GLuint *arrays)
    {
        DeleteVertexArrays = (PFNDELETEVERTEXARRAYSPROC)IntGetProcAddress("glDeleteVertexArrays");
        DeleteVertexArrays(n, arrays);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenVertexArrays(GLsizei n, GLuint *arrays)
    {
        GenVertexArrays = (PFNGENVERTEXARRAYSPROC)IntGetProcAddress("glGenVertexArrays");
        GenVertexArrays(n, arrays);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsVertexArray(GLuint ren_array)
    {
        IsVertexArray = (PFNISVERTEXARRAYPROC)IntGetProcAddress("glIsVertexArray");
        திரும்பு IsVertexArray(ren_array);
    }

    // Extension: ARB_map_buffer_range

    நிலையான GLvoid* CODEGEN_FUNCPTR Switch_MapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        MapBufferRange = (PFNMAPBUFFERRANGEPROC)IntGetProcAddress("glMapBufferRange");
        திரும்பு MapBufferRange(target, offset, length, access);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        FlushMappedBufferRange = (PFNFLUSHMAPPEDBUFFERRANGEPROC)IntGetProcAddress("glFlushMappedBufferRange");
        FlushMappedBufferRange(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)IntGetProcAddress("glIsRenderbuffer");
        திரும்பு IsRenderbuffer(renderbuffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)IntGetProcAddress("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, மாறிலி GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)IntGetProcAddress("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)IntGetProcAddress("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)IntGetProcAddress("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, width, height);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)IntGetProcAddress("glIsFramebuffer");
        திரும்பு IsFramebuffer(framebuffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)IntGetProcAddress("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, மாறிலி GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)IntGetProcAddress("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)IntGetProcAddress("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    நிலையான GLenum CODEGEN_FUNCPTR Switch_CheckFramebufferStatus(GLenum target)
    {
        CheckFramebufferStatus = (PFNCHECKFRAMEBUFFERSTATUSPROC)IntGetProcAddress("glCheckFramebufferStatus");
        திரும்பு CheckFramebufferStatus(target);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)IntGetProcAddress("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)IntGetProcAddress("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)IntGetProcAddress("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)IntGetProcAddress("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)IntGetProcAddress("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)IntGetProcAddress("glGenerateMipmap");
        GenerateMipmap(target);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)IntGetProcAddress("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)IntGetProcAddress("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, width, height);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)IntGetProcAddress("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)IntGetProcAddress("glColorMaski");
        ColorMaski(index, r, g, b, a);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint index, GLboolean *data)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)IntGetProcAddress("glGetBooleani_v");
        GetBooleani_v(target, index, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint index, GLint *data)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)IntGetProcAddress("glGetIntegeri_v");
        GetIntegeri_v(target, index, data);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint index)
    {
        Enablei = (PFNENABLEIPROC)IntGetProcAddress("glEnablei");
        Enablei(target, index);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint index)
    {
        Disablei = (PFNDISABLEIPROC)IntGetProcAddress("glDisablei");
        Disablei(target, index);
    }

    நிலையான GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint index)
    {
        IsEnabledi = (PFNISENABLEDIPROC)IntGetProcAddress("glIsEnabledi");
        திரும்பு IsEnabledi(target, index);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitiveMode)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)IntGetProcAddress("glBeginTransformFeedback");
        BeginTransformFeedback(primitiveMode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_EndTransformFeedback()
    {
        EndTransformFeedback = (PFNENDTRANSFORMFEEDBACKPROC)IntGetProcAddress("glEndTransformFeedback");
        EndTransformFeedback();
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)
    {
        BindBufferRange = (PFNBINDBUFFERRANGEPROC)IntGetProcAddress("glBindBufferRange");
        BindBufferRange(target, index, buffer, offset, size);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindBufferBase(GLenum target, GLuint index, GLuint buffer)
    {
        BindBufferBase = (PFNBINDBUFFERBASEPROC)IntGetProcAddress("glBindBufferBase");
        BindBufferBase(target, index, buffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint program, GLsizei மொத்தம்_2, மாறிலி GLchar* மாறிலி *varyings, GLenum bufferMode)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)IntGetProcAddress("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(program, மொத்தம்_2, varyings, bufferMode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)IntGetProcAddress("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(program, index, bufSize, length, size, type, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)IntGetProcAddress("glClampColor");
        ClampColor(target, clamp);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum mode)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)IntGetProcAddress("glBeginConditionalRender");
        BeginConditionalRender(id, mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_EndConditionalRender()
    {
        EndConditionalRender = (PFNENDCONDITIONALRENDERPROC)IntGetProcAddress("glEndConditionalRender");
        EndConditionalRender();
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, மாறிலி GLvoid *pointer)
    {
        VertexAttribIPointer = (PFNVERTEXATTRIBIPOINTERPROC)IntGetProcAddress("glVertexAttribIPointer");
        VertexAttribIPointer(index, size, type, stride, pointer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetVertexAttribIiv(GLuint index, GLenum pname, GLint *params)
    {
        GetVertexAttribIiv = (PFNGETVERTEXATTRIBIIVPROC)IntGetProcAddress("glGetVertexAttribIiv");
        GetVertexAttribIiv(index, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params)
    {
        GetVertexAttribIuiv = (PFNGETVERTEXATTRIBIUIVPROC)IntGetProcAddress("glGetVertexAttribIuiv");
        GetVertexAttribIuiv(index, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI1i(GLuint index, GLint x)
    {
        VertexAttribI1i = (PFNVERTEXATTRIBI1IPROC)IntGetProcAddress("glVertexAttribI1i");
        VertexAttribI1i(index, x);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI2i(GLuint index, GLint x, GLint y)
    {
        VertexAttribI2i = (PFNVERTEXATTRIBI2IPROC)IntGetProcAddress("glVertexAttribI2i");
        VertexAttribI2i(index, x, y);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI3i(GLuint index, GLint x, GLint y, GLint z)
    {
        VertexAttribI3i = (PFNVERTEXATTRIBI3IPROC)IntGetProcAddress("glVertexAttribI3i");
        VertexAttribI3i(index, x, y, z);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w)
    {
        VertexAttribI4i = (PFNVERTEXATTRIBI4IPROC)IntGetProcAddress("glVertexAttribI4i");
        VertexAttribI4i(index, x, y, z, w);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI1ui(GLuint index, GLuint x)
    {
        VertexAttribI1ui = (PFNVERTEXATTRIBI1UIPROC)IntGetProcAddress("glVertexAttribI1ui");
        VertexAttribI1ui(index, x);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI2ui(GLuint index, GLuint x, GLuint y)
    {
        VertexAttribI2ui = (PFNVERTEXATTRIBI2UIPROC)IntGetProcAddress("glVertexAttribI2ui");
        VertexAttribI2ui(index, x, y);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z)
    {
        VertexAttribI3ui = (PFNVERTEXATTRIBI3UIPROC)IntGetProcAddress("glVertexAttribI3ui");
        VertexAttribI3ui(index, x, y, z);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        VertexAttribI4ui = (PFNVERTEXATTRIBI4UIPROC)IntGetProcAddress("glVertexAttribI4ui");
        VertexAttribI4ui(index, x, y, z, w);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI1iv(GLuint index, மாறிலி GLint *v)
    {
        VertexAttribI1iv = (PFNVERTEXATTRIBI1IVPROC)IntGetProcAddress("glVertexAttribI1iv");
        VertexAttribI1iv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI2iv(GLuint index, மாறிலி GLint *v)
    {
        VertexAttribI2iv = (PFNVERTEXATTRIBI2IVPROC)IntGetProcAddress("glVertexAttribI2iv");
        VertexAttribI2iv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI3iv(GLuint index, மாறிலி GLint *v)
    {
        VertexAttribI3iv = (PFNVERTEXATTRIBI3IVPROC)IntGetProcAddress("glVertexAttribI3iv");
        VertexAttribI3iv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4iv(GLuint index, மாறிலி GLint *v)
    {
        VertexAttribI4iv = (PFNVERTEXATTRIBI4IVPROC)IntGetProcAddress("glVertexAttribI4iv");
        VertexAttribI4iv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI1uiv(GLuint index, மாறிலி GLuint *v)
    {
        VertexAttribI1uiv = (PFNVERTEXATTRIBI1UIVPROC)IntGetProcAddress("glVertexAttribI1uiv");
        VertexAttribI1uiv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI2uiv(GLuint index, மாறிலி GLuint *v)
    {
        VertexAttribI2uiv = (PFNVERTEXATTRIBI2UIVPROC)IntGetProcAddress("glVertexAttribI2uiv");
        VertexAttribI2uiv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI3uiv(GLuint index, மாறிலி GLuint *v)
    {
        VertexAttribI3uiv = (PFNVERTEXATTRIBI3UIVPROC)IntGetProcAddress("glVertexAttribI3uiv");
        VertexAttribI3uiv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4uiv(GLuint index, மாறிலி GLuint *v)
    {
        VertexAttribI4uiv = (PFNVERTEXATTRIBI4UIVPROC)IntGetProcAddress("glVertexAttribI4uiv");
        VertexAttribI4uiv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4bv(GLuint index, மாறிலி GLbyte *v)
    {
        VertexAttribI4bv = (PFNVERTEXATTRIBI4BVPROC)IntGetProcAddress("glVertexAttribI4bv");
        VertexAttribI4bv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4sv(GLuint index, மாறிலி GLshort *v)
    {
        VertexAttribI4sv = (PFNVERTEXATTRIBI4SVPROC)IntGetProcAddress("glVertexAttribI4sv");
        VertexAttribI4sv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4ubv(GLuint index, மாறிலி GLubyte *v)
    {
        VertexAttribI4ubv = (PFNVERTEXATTRIBI4UBVPROC)IntGetProcAddress("glVertexAttribI4ubv");
        VertexAttribI4ubv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexAttribI4usv(GLuint index, மாறிலி GLushort *v)
    {
        VertexAttribI4usv = (PFNVERTEXATTRIBI4USVPROC)IntGetProcAddress("glVertexAttribI4usv");
        VertexAttribI4usv(index, v);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint program, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)IntGetProcAddress("glGetUniformuiv");
        GetUniformuiv(program, location, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_BindFragDataLocation(GLuint program, GLuint color, மாறிலி GLchar *name)
    {
        BindFragDataLocation = (PFNBINDFRAGDATALOCATIONPROC)IntGetProcAddress("glBindFragDataLocation");
        BindFragDataLocation(program, color, name);
    }

    நிலையான GLint CODEGEN_FUNCPTR Switch_GetFragDataLocation(GLuint program, மாறிலி GLchar *name)
    {
        GetFragDataLocation = (PFNGETFRAGDATALOCATIONPROC)IntGetProcAddress("glGetFragDataLocation");
        திரும்பு GetFragDataLocation(program, name);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)IntGetProcAddress("glUniform1ui");
        Uniform1ui(location, v0);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)IntGetProcAddress("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)IntGetProcAddress("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)IntGetProcAddress("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)IntGetProcAddress("glUniform1uiv");
        Uniform1uiv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)IntGetProcAddress("glUniform2uiv");
        Uniform2uiv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)IntGetProcAddress("glUniform3uiv");
        Uniform3uiv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei மொத்தம்_2, மாறிலி GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)IntGetProcAddress("glUniform4uiv");
        Uniform4uiv(location, மொத்தம்_2, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, மாறிலி GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)IntGetProcAddress("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, மாறிலி GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)IntGetProcAddress("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)IntGetProcAddress("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)IntGetProcAddress("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum buffer, GLint drawbuffer, மாறிலி GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)IntGetProcAddress("glClearBufferiv");
        ClearBufferiv(buffer, drawbuffer, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum buffer, GLint drawbuffer, மாறிலி GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)IntGetProcAddress("glClearBufferuiv");
        ClearBufferuiv(buffer, drawbuffer, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum buffer, GLint drawbuffer, மாறிலி GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)IntGetProcAddress("glClearBufferfv");
        ClearBufferfv(buffer, drawbuffer, value);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)IntGetProcAddress("glClearBufferfi");
        ClearBufferfi(buffer, drawbuffer, depth, stencil);
    }

    நிலையான மாறிலி GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint index)
    {
        GetStringi = (PFNGETSTRINGIPROC)IntGetProcAddress("glGetStringi");
        திரும்பு GetStringi(name, index);
    }

    // Extension: ARB_uniform_buffer_object

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetUniformIndices(GLuint program, GLsizei uniform_மொத்தம், மாறிலி GLchar* மாறிலி *uniformNames, GLuint *uniformIndices)
    {
        GetUniformIndices = (PFNGETUNIFORMINDICESPROC)IntGetProcAddress("glGetUniformIndices");
        GetUniformIndices(program, uniform_மொத்தம், uniformNames, uniformIndices);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetActiveUniformsiv(GLuint program, GLsizei uniform_மொத்தம், மாறிலி GLuint *uniformIndices, GLenum pname, GLint *params)
    {
        GetActiveUniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)IntGetProcAddress("glGetActiveUniformsiv");
        GetActiveUniformsiv(program, uniform_மொத்தம், uniformIndices, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName)
    {
        GetActiveUniformName = (PFNGETACTIVEUNIFORMNAMEPROC)IntGetProcAddress("glGetActiveUniformName");
        GetActiveUniformName(program, uniformIndex, bufSize, length, uniformName);
    }

    நிலையான GLuint CODEGEN_FUNCPTR Switch_GetUniformBlockIndex(GLuint program, மாறிலி GLchar *uniformBlockName)
    {
        GetUniformBlockIndex = (PFNGETUNIFORMBLOCKINDEXPROC)IntGetProcAddress("glGetUniformBlockIndex");
        திரும்பு GetUniformBlockIndex(program, uniformBlockName);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params)
    {
        GetActiveUniformBlockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)IntGetProcAddress("glGetActiveUniformBlockiv");
        GetActiveUniformBlockiv(program, uniformBlockIndex, pname, params);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_GetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName)
    {
        GetActiveUniformBlockName = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)IntGetProcAddress("glGetActiveUniformBlockName");
        GetActiveUniformBlockName(program, uniformBlockIndex, bufSize, length, uniformBlockName);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)
    {
        UniformBlockBinding = (PFNUNIFORMBLOCKBINDINGPROC)IntGetProcAddress("glUniformBlockBinding");
        UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
    }

    // Extension: ARB_copy_buffer

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_CopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
    {
        CopyBufferSubData = (PFNCOPYBUFFERSUBDATAPROC)IntGetProcAddress("glCopyBufferSubData");
        CopyBufferSubData(readTarget, writeTarget, readOffset, writeOffset, size);
    }

    // Extension: 3.1

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum mode, GLint first, GLsizei மொத்தம்_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)IntGetProcAddress("glDrawArraysInstanced");
        DrawArraysInstanced(mode, first, மொத்தம்_2, instancecount);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum mode, GLsizei மொத்தம்_2, GLenum type, மாறிலி GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)IntGetProcAddress("glDrawElementsInstanced");
        DrawElementsInstanced(mode, மொத்தம்_2, type, indices, instancecount);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexBuffer(GLenum target, GLenum internalformat, GLuint buffer)
    {
        TexBuffer = (PFNTEXBUFFERPROC)IntGetProcAddress("glTexBuffer");
        TexBuffer(target, internalformat, buffer);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_PrimitiveRestartIndex(GLuint index)
    {
        PrimitiveRestartIndex = (PFNPRIMITIVERESTARTINDEXPROC)IntGetProcAddress("glPrimitiveRestartIndex");
        PrimitiveRestartIndex(index);
    }

    // Legacy

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)IntGetProcAddress("glEnableClientState");
        EnableClientState(cap);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)IntGetProcAddress("glDisableClientState");
        DisableClientState(cap);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_VertexPointer(GLint size, GLenum type, GLsizei stride, மாறிலி GLvoid *ptr)
    {
        VertexPointer = (PFNVERTEXPOINTERPROC)IntGetProcAddress("glVertexPointer");
        VertexPointer(size, type, stride, ptr);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_NormalPointer(GLenum type, GLsizei stride, மாறிலி GLvoid *ptr)
    {
        NormalPointer = (PFNNORMALPOINTERPROC)IntGetProcAddress("glNormalPointer");
        NormalPointer(type, stride, ptr);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_ColorPointer(GLint size, GLenum type, GLsizei stride, மாறிலி GLvoid *ptr)
    {
        ColorPointer = (PFNCOLORPOINTERPROC)IntGetProcAddress("glColorPointer");
        ColorPointer(size, type, stride, ptr);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexCoordPointer(GLint size, GLenum type, GLsizei stride, மாறிலி GLvoid *ptr)
    {
        TexCoordPointer = (PFNTEXCOORDPOINTERPROC)IntGetProcAddress("glTexCoordPointer");
        TexCoordPointer(size, type, stride, ptr);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)IntGetProcAddress("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_MatrixMode(GLenum mode)
    {
        MatrixMode = (PFNMATRIXMODEPROC)IntGetProcAddress("glMatrixMode");
        MatrixMode(mode);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_LoadIdentity(வெறுமை)
    {
        LoadIdentity = (PFNLOADIDENTITYPROC)IntGetProcAddress("glLoadIdentity");
        LoadIdentity();
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Ortho(GLdouble இடது, GLdouble வலது, GLdouble bottom, GLdouble உச்சி_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)IntGetProcAddress("glOrtho");
        Ortho(இடது, வலது, bottom, உச்சி_2, near_val, far_val);
    }

    நிலையான வெறுமை CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)IntGetProcAddress("glColor3d");
        Color3d(red, green, blue);
    }

    கட்டமைப்பு InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            வரி_Width = Switch_வரி_Width;
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
