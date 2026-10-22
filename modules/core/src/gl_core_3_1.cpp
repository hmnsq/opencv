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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "gl_core_3_1.hpp"

#ifdef HAVE_OPENGL

    #ifdef __APPLE__
        #অন্তর্ভাব <dlfcn.h>

        স্থির শূন্য* AppleGLGetProcAddress (ধ্রুবক অক্ষর* name)
        {
            স্থির যৌক্তিক initialized = মিথ্যা;
            স্থির শূন্য * handle = ফাঁকা;
            যদি (!handle)
            {
                যদি (!initialized)
                {
                    initialized = সত্য;
                    ধ্রুবক অক্ষর * ধ্রুবক path = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
                }
                যদি (!handle)
                    ফেরত ফাঁকা;
            }
            ফেরত dlsym(handle, name);
        }
    #endif // __APPLE__

    #যদি defined(__sgi) || defined (__sun)
        #অন্তর্ভাব <dlfcn.h>
        #অন্তর্ভাব <stdio.h>

        স্থির শূন্য* SunGetProcAddress (ধ্রুবক অক্ষর* name)
        {
            প্রকারলেখ শূন্য* (func_t*)(ধ্রুবক GLubyte*);

            স্থির শূন্য* h = 0;
            স্থির func_t gpa = 0;

            যদি (!h)
            {
                h = dlopen(ফাঁকা, RTLD_LAZY | RTLD_LOCAL);
                যদি (!h)
                    ফেরত 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            ফেরত gpa ? gpa((ধ্রুবক GLubyte*) name) : dlsym(h, name);
        }
    #endif // __sgi || __sun

    #যদি defined(_WIN32)
        #ifdef _MSC_VER
            #pragma warning(disable: 4055)
            #pragma warning(disable: 4054)
        #endif

        স্থির পূর্ণসংখ্যা TestPointer(ধ্রুবক PROC pTest)
        {
            যদি(!pTest)
                ফেরত 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            যদি (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                ফেরত 0;

            ফেরত 1;
        }

        স্থির PROC WinGetProcAddress(ধ্রুবক অক্ষর* name)
        {
            PROC pFunc = wglGetProcAddress((LPCSTR) name);
            যদি (TestPointer(pFunc))
                ফেরত pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            ফেরত (PROC) GetProcAddress(glMod, (LPCSTR) name);
        }
    #endif // _WIN32

    #যদি defined(_WIN32)
        #define CV_GL_GET_PROC_ADDRESS(name) WinGetProcAddress(name)
    #নতুবা_যদি defined(__APPLE__)
        #define CV_GL_GET_PROC_ADDRESS(name) AppleGLGetProcAddress(name)
    #নতুবা_যদি defined(__sgi) || defined(__sun)
        #define CV_GL_GET_PROC_ADDRESS(name) SunGetProcAddress(name)
    #অন্যথা // GLX
        #অন্তর্ভাব <GL/glx.h>

        #define CV_GL_GET_PROC_ADDRESS(name) glXGetProcAddressARB((ধ্রুবক GLubyte*) name)
    #endif

    স্থির শূন্য* IntGetProcAddress(ধ্রুবক অক্ষর* name)
    {
        শূন্য* func =  (শূন্য*) CV_GL_GET_PROC_ADDRESS(name);
        যদি (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        ফেরত func;
    }
#অন্যথা
#যদি defined(_MSC_VER)
    #pragma warning(disable : 4702)  // unreachable code
#endif
    স্থির শূন্য* IntGetProcAddress(ধ্রুবক অক্ষর*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#endif

নামাকাশ gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    প্রকারলেখ GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    প্রকারলেখ ধ্রুবক GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(ধ্রুবক GLubyte *);

    // Extension: 1.2
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , ধ্রুবক GLint *, ধ্রুবক GLsizei *, GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , ধ্রুবক GLsizei *, GLenum , ধ্রুবক GLvoid* ধ্রুবক *, GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , ধ্রুবক GLint *);

    // Extension: 1.5
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , ধ্রুবক GLvoid *, GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    প্রকারলেখ GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , ধ্রুবক GLenum *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , ধ্রুবক GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    প্রকারলেখ GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    প্রকারলেখ GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    প্রকারলেখ GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , ধ্রুবক GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    প্রকারলেখ GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , ধ্রুবক GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , ধ্রুবক GLchar* ধ্রুবক *, ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , ধ্রুবক GLvoid *);

    // Extension: 2.1
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , ধ্রুবক GLfloat *);

    // Extension: ARB_vertex_array_object
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    প্রকারলেখ GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    প্রকারলেখ GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    প্রকারলেখ GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , ধ্রুবক GLchar* ধ্রুবক *, GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , ধ্রুবক GLbyte *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , ধ্রুবক GLshort *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , ধ্রুবক GLubyte *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , ধ্রুবক GLushort *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , ধ্রুবক GLchar *);
    প্রকারলেখ GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , ধ্রুবক GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , ধ্রুবক GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , ধ্রুবক GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , ধ্রুবক GLfloat *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    প্রকারলেখ ধ্রুবক GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , ধ্রুবক GLchar* ধ্রুবক *, GLuint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , ধ্রুবক GLuint *, GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    প্রকারলেখ GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , ধ্রুবক GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , ধ্রুবক GLvoid *, GLsizei );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , ধ্রুবক GLvoid *);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(শূন্য);
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    প্রকারলেখ শূন্য (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC সারি_Width;
    PFNPOINTSIZEPROC Point_আকার;
    PFNPOLYGONMODEPROC PolygonMode;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_বাফার;
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
    PFNREADBUFFERPROC Read_বাফার;
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
    PFNBINDBUFFERPROC Bind_বাফার;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_বাফার;
    PFNBUFFERDATAPROC বাফার_তথ্য;
    PFNBUFFERSUBDATAPROC বাফার_Sub_তথ্য;
    PFNGETBUFFERSUBDATAPROC Get_বাফার_Sub_তথ্য;
    PFNMAPBUFFERPROC Map_বাফার;
    PFNUNMAPBUFFERPROC Unmap_বাফার;
    PFNGETBUFFERPARAMETERIVPROC Get_বাফার_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_বাফার_Pointerv;

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
    PFNMAPBUFFERRANGEPROC Map_বাফার_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_বাফার_Range;

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
    PFNBINDBUFFERRANGEPROC Bind_বাফার_Range;
    PFNBINDBUFFERBASEPROC Bind_বাফার_Base;
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
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_তথ্য_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_তথ্য_Location;
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
    PFNGETUNIFORMBLOCKINDEXPROC Get_Uniform_Block_সূচক;
    PFNGETACTIVEUNIFORMBLOCKIVPROC GetActiveUniformBlockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC GetActiveUniformBlockName;
    PFNUNIFORMBLOCKBINDINGPROC UniformBlockBinding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_বাফার_Sub_তথ্য;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_বাফার;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_সূচক;

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

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CullFace(GLenum mode)
    {
        CullFace = (PFNCULLFACEPROC)IntGetProcAddress("glCullFace");
        CullFace(mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_FrontFace(GLenum mode)
    {
        FrontFace = (PFNFRONTFACEPROC)IntGetProcAddress("glFrontFace");
        FrontFace(mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum mode)
    {
        Hint = (PFNHINTPROC)IntGetProcAddress("glHint");
        Hint(target, mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_সারি_Width(GLfloat width)
    {
        সারি_Width = (PFNLINEWIDTHPROC)IntGetProcAddress("glLineWidth");
        সারি_Width(width);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Point_আকার(GLfloat size)
    {
        Point_আকার = (PFNPOINTSIZEPROC)IntGetProcAddress("glPointSize");
        Point_আকার(size);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PolygonMode(GLenum face, GLenum mode)
    {
        PolygonMode = (PFNPOLYGONMODEPROC)IntGetProcAddress("glPolygonMode");
        PolygonMode(face, mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Scissor = (PFNSCISSORPROC)IntGetProcAddress("glScissor");
        Scissor(x, y, width, height);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)IntGetProcAddress("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ধ্রুবক GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)IntGetProcAddress("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)IntGetProcAddress("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ধ্রুবক GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)IntGetProcAddress("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, ধ্রুবক GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)IntGetProcAddress("glTexImage1D");
        TexImage1D(target, level, internalformat, width, border, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, ধ্রুবক GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)IntGetProcAddress("glTexImage2D");
        TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Draw_বাফার(GLenum mode)
    {
        Draw_বাফার = (PFNDRAWBUFFERPROC)IntGetProcAddress("glDrawBuffer");
        Draw_বাফার(mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Clear(GLbitfield mask)
    {
        Clear = (PFNCLEARPROC)IntGetProcAddress("glClear");
        Clear(mask);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)IntGetProcAddress("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)IntGetProcAddress("glClearStencil");
        ClearStencil(s);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearDepth(GLdouble depth)
    {
        ClearDepth = (PFNCLEARDEPTHPROC)IntGetProcAddress("glClearDepth");
        ClearDepth(depth);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_StencilMask(GLuint mask)
    {
        StencilMask = (PFNSTENCILMASKPROC)IntGetProcAddress("glStencilMask");
        StencilMask(mask);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        ColorMask = (PFNCOLORMASKPROC)IntGetProcAddress("glColorMask");
        ColorMask(red, green, blue, alpha);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DepthMask(GLboolean flag)
    {
        DepthMask = (PFNDEPTHMASKPROC)IntGetProcAddress("glDepthMask");
        DepthMask(flag);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)IntGetProcAddress("glDisable");
        Disable(cap);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)IntGetProcAddress("glEnable");
        Enable(cap);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Finish()
    {
        Finish = (PFNFINISHPROC)IntGetProcAddress("glFinish");
        Finish();
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)IntGetProcAddress("glFlush");
        Flush();
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)IntGetProcAddress("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)IntGetProcAddress("glLogicOp");
        LogicOp(opcode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)IntGetProcAddress("glStencilFunc");
        StencilFunc(func, ref, mask);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)IntGetProcAddress("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DepthFunc(GLenum func)
    {
        DepthFunc = (PFNDEPTHFUNCPROC)IntGetProcAddress("glDepthFunc");
        DepthFunc(func);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)IntGetProcAddress("glPixelStoref");
        PixelStoref(pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)IntGetProcAddress("glPixelStorei");
        PixelStorei(pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Read_বাফার(GLenum mode)
    {
        Read_বাফার = (PFNREADBUFFERPROC)IntGetProcAddress("glReadBuffer");
        Read_বাফার(mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)IntGetProcAddress("glReadPixels");
        ReadPixels(x, y, width, height, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)IntGetProcAddress("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)IntGetProcAddress("glGetDoublev");
        GetDoublev(pname, params);
    }

    স্থির GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)IntGetProcAddress("glGetError");
        ফেরত GetError();
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)IntGetProcAddress("glGetFloatv");
        GetFloatv(pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)IntGetProcAddress("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    স্থির ধ্রুবক GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)IntGetProcAddress("glGetString");
        ফেরত GetString(name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)IntGetProcAddress("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)IntGetProcAddress("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)IntGetProcAddress("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)IntGetProcAddress("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)IntGetProcAddress("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)IntGetProcAddress("glIsEnabled");
        ফেরত IsEnabled(cap);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DepthRange(GLdouble ren_near, GLdouble ren_far)
    {
        DepthRange = (PFNDEPTHRANGEPROC)IntGetProcAddress("glDepthRange");
        DepthRange(ren_near, ren_far);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Viewport = (PFNVIEWPORTPROC)IntGetProcAddress("glViewport");
        Viewport(x, y, width, height);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DrawArrays(GLenum mode, GLint first, GLsizei গুনতি_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)IntGetProcAddress("glDrawArrays");
        DrawArrays(mode, first, গুনতি_2);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DrawElements(GLenum mode, GLsizei গুনতি_2, GLenum type, ধ্রুবক GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)IntGetProcAddress("glDrawElements");
        DrawElements(mode, গুনতি_2, type, indices);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)IntGetProcAddress("glGetPointerv");
        GetPointerv(pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PolygonOffset(GLfloat factor, GLfloat units)
    {
        PolygonOffset = (PFNPOLYGONOFFSETPROC)IntGetProcAddress("glPolygonOffset");
        PolygonOffset(factor, units);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)IntGetProcAddress("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, width, border);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)IntGetProcAddress("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)IntGetProcAddress("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, width);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)IntGetProcAddress("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, ধ্রুবক GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)IntGetProcAddress("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, width, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, ধ্রুবক GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)IntGetProcAddress("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)IntGetProcAddress("glBindTexture");
        BindTexture(target, texture);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ধ্রুবক GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)IntGetProcAddress("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)IntGetProcAddress("glGenTextures");
        GenTextures(n, textures);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)IntGetProcAddress("glIsTexture");
        ফেরত IsTexture(texture);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)IntGetProcAddress("glIndexub");
        Indexub(c);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Indexubv(ধ্রুবক GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)IntGetProcAddress("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)IntGetProcAddress("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BlendEquation(GLenum mode)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)IntGetProcAddress("glBlendEquation");
        BlendEquation(mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei গুনতি_2, GLenum type, ধ্রুবক GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)IntGetProcAddress("glDrawRangeElements");
        DrawRangeElements(mode, start, end, গুনতি_2, type, indices);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, ধ্রুবক GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)IntGetProcAddress("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)IntGetProcAddress("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    }

    // Extension: 1.3

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ActiveTexture(GLenum texture)
    {
        ActiveTexture = (PFNACTIVETEXTUREPROC)IntGetProcAddress("glActiveTexture");
        ActiveTexture(texture);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)IntGetProcAddress("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei image_আকার, ধ্রুবক GLvoid *তথ্য)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)IntGetProcAddress("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, width, height, depth, border, image_আকার, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei image_আকার, ধ্রুবক GLvoid *তথ্য)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)IntGetProcAddress("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, width, height, border, image_আকার, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei image_আকার, ধ্রুবক GLvoid *তথ্য)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)IntGetProcAddress("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, width, border, image_আকার, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei image_আকার, ধ্রুবক GLvoid *তথ্য)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)IntGetProcAddress("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, image_আকার, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei image_আকার, ধ্রুবক GLvoid *তথ্য)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)IntGetProcAddress("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, image_আকার, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei image_আকার, ধ্রুবক GLvoid *তথ্য)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)IntGetProcAddress("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, width, format, image_আকার, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)IntGetProcAddress("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)IntGetProcAddress("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum mode, ধ্রুবক GLint *first, ধ্রুবক GLsizei *গুনতি_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)IntGetProcAddress("glMultiDrawArrays");
        MultiDrawArrays(mode, first, গুনতি_2, drawcount);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum mode, ধ্রুবক GLsizei *গুনতি_2, GLenum type, ধ্রুবক GLvoid* ধ্রুবক *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)IntGetProcAddress("glMultiDrawElements");
        MultiDrawElements(mode, গুনতি_2, type, indices, drawcount);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PointParameterf(GLenum pname, GLfloat param)
    {
        PointParameterf = (PFNPOINTPARAMETERFPROC)IntGetProcAddress("glPointParameterf");
        PointParameterf(pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PointParameterfv(GLenum pname, ধ্রুবক GLfloat *params)
    {
        PointParameterfv = (PFNPOINTPARAMETERFVPROC)IntGetProcAddress("glPointParameterfv");
        PointParameterfv(pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PointParameteri(GLenum pname, GLint param)
    {
        PointParameteri = (PFNPOINTPARAMETERIPROC)IntGetProcAddress("glPointParameteri");
        PointParameteri(pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_PointParameteriv(GLenum pname, ধ্রুবক GLint *params)
    {
        PointParameteriv = (PFNPOINTPARAMETERIVPROC)IntGetProcAddress("glPointParameteriv");
        PointParameteriv(pname, params);
    }

    // Extension: 1.5

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)IntGetProcAddress("glGenQueries");
        GenQueries(n, ids);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ধ্রুবক GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)IntGetProcAddress("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)IntGetProcAddress("glIsQuery");
        ফেরত IsQuery(id);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)IntGetProcAddress("glBeginQuery");
        BeginQuery(target, id);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_EndQuery(GLenum target)
    {
        EndQuery = (PFNENDQUERYPROC)IntGetProcAddress("glEndQuery");
        EndQuery(target);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)IntGetProcAddress("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)IntGetProcAddress("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)IntGetProcAddress("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Bind_বাফার(GLenum target, GLuint বাফার)
    {
        Bind_বাফার = (PFNBINDBUFFERPROC)IntGetProcAddress("glBindBuffer");
        Bind_বাফার(target, বাফার);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ধ্রুবক GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)IntGetProcAddress("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)IntGetProcAddress("glGenBuffers");
        GenBuffers(n, buffers);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_Is_বাফার(GLuint বাফার)
    {
        Is_বাফার = (PFNISBUFFERPROC)IntGetProcAddress("glIsBuffer");
        ফেরত Is_বাফার(বাফার);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_বাফার_তথ্য(GLenum target, GLsizeiptr size, ধ্রুবক GLvoid *তথ্য, GLenum usage)
    {
        বাফার_তথ্য = (PFNBUFFERDATAPROC)IntGetProcAddress("glBufferData");
        বাফার_তথ্য(target, size, তথ্য, usage);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_বাফার_Sub_তথ্য(GLenum target, GLintptr offset, GLsizeiptr size, ধ্রুবক GLvoid *তথ্য)
    {
        বাফার_Sub_তথ্য = (PFNBUFFERSUBDATAPROC)IntGetProcAddress("glBufferSubData");
        বাফার_Sub_তথ্য(target, offset, size, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Get_বাফার_Sub_তথ্য(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *তথ্য)
    {
        Get_বাফার_Sub_তথ্য = (PFNGETBUFFERSUBDATAPROC)IntGetProcAddress("glGetBufferSubData");
        Get_বাফার_Sub_তথ্য(target, offset, size, তথ্য);
    }

    স্থির GLvoid* CODEGEN_FUNCPTR Switch_Map_বাফার(GLenum target, GLenum access)
    {
        Map_বাফার = (PFNMAPBUFFERPROC)IntGetProcAddress("glMapBuffer");
        ফেরত Map_বাফার(target, access);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_Unmap_বাফার(GLenum target)
    {
        Unmap_বাফার = (PFNUNMAPBUFFERPROC)IntGetProcAddress("glUnmapBuffer");
        ফেরত Unmap_বাফার(target);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Get_বাফার_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_বাফার_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetBufferParameteriv");
        Get_বাফার_Parameteriv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Get_বাফার_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_বাফার_Pointerv = (PFNGETBUFFERPOINTERVPROC)IntGetProcAddress("glGetBufferPointerv");
        Get_বাফার_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)IntGetProcAddress("glBlendEquationSeparate");
        BlendEquationSeparate(modeRGB, modeAlpha);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ধ্রুবক GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)IntGetProcAddress("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)IntGetProcAddress("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)IntGetProcAddress("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, mask);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_StencilMaskSeparate(GLenum face, GLuint mask)
    {
        StencilMaskSeparate = (PFNSTENCILMASKSEPARATEPROC)IntGetProcAddress("glStencilMaskSeparate");
        StencilMaskSeparate(face, mask);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_AttachShader(GLuint program, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)IntGetProcAddress("glAttachShader");
        AttachShader(program, shader);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint program, GLuint সূচক, ধ্রুবক GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)IntGetProcAddress("glBindAttribLocation");
        BindAttribLocation(program, সূচক, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)IntGetProcAddress("glCompileShader");
        CompileShader(shader);
    }

    স্থির GLuint CODEGEN_FUNCPTR Switch_CreateProgram()
    {
        CreateProgram = (PFNCREATEPROGRAMPROC)IntGetProcAddress("glCreateProgram");
        ফেরত CreateProgram();
    }

    স্থির GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)IntGetProcAddress("glCreateShader");
        ফেরত CreateShader(type);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteProgram(GLuint program)
    {
        DeleteProgram = (PFNDELETEPROGRAMPROC)IntGetProcAddress("glDeleteProgram");
        DeleteProgram(program);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)IntGetProcAddress("glDeleteShader");
        DeleteShader(shader);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DetachShader(GLuint program, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)IntGetProcAddress("glDetachShader");
        DetachShader(program, shader);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DisableVertexAttribArray(GLuint সূচক)
    {
        DisableVertexAttribArray = (PFNDISABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glDisableVertexAttribArray");
        DisableVertexAttribArray(সূচক);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_EnableVertexAttribArray(GLuint সূচক)
    {
        EnableVertexAttribArray = (PFNENABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glEnableVertexAttribArray");
        EnableVertexAttribArray(সূচক);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetActiveAttrib(GLuint program, GLuint সূচক, GLsizei buf_আকার, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveAttrib = (PFNGETACTIVEATTRIBPROC)IntGetProcAddress("glGetActiveAttrib");
        GetActiveAttrib(program, সূচক, buf_আকার, length, size, type, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetActiveUniform(GLuint program, GLuint সূচক, GLsizei buf_আকার, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveUniform = (PFNGETACTIVEUNIFORMPROC)IntGetProcAddress("glGetActiveUniform");
        GetActiveUniform(program, সূচক, buf_আকার, length, size, type, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint program, GLsizei max_গুনতি, GLsizei *গুনতি_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)IntGetProcAddress("glGetAttachedShaders");
        GetAttachedShaders(program, max_গুনতি, গুনতি_2, obj);
    }

    স্থির GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint program, ধ্রুবক GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)IntGetProcAddress("glGetAttribLocation");
        ফেরত GetAttribLocation(program, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint program, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)IntGetProcAddress("glGetProgramiv");
        GetProgramiv(program, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetProgramInfoLog(GLuint program, GLsizei buf_আকার, GLsizei *length, GLchar *infoLog)
    {
        GetProgramInfoLog = (PFNGETPROGRAMINFOLOGPROC)IntGetProcAddress("glGetProgramInfoLog");
        GetProgramInfoLog(program, buf_আকার, length, infoLog);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)IntGetProcAddress("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_আকার, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)IntGetProcAddress("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_আকার, length, infoLog);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetShaderSource(GLuint shader, GLsizei buf_আকার, GLsizei *length, GLchar *source)
    {
        GetShaderSource = (PFNGETSHADERSOURCEPROC)IntGetProcAddress("glGetShaderSource");
        GetShaderSource(shader, buf_আকার, length, source);
    }

    স্থির GLint CODEGEN_FUNCPTR Switch_GetUniformLocation(GLuint program, ধ্রুবক GLchar *name)
    {
        GetUniformLocation = (PFNGETUNIFORMLOCATIONPROC)IntGetProcAddress("glGetUniformLocation");
        ফেরত GetUniformLocation(program, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint program, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)IntGetProcAddress("glGetUniformfv");
        GetUniformfv(program, location, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint program, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)IntGetProcAddress("glGetUniformiv");
        GetUniformiv(program, location, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetVertexAttribdv(GLuint সূচক, GLenum pname, GLdouble *params)
    {
        GetVertexAttribdv = (PFNGETVERTEXATTRIBDVPROC)IntGetProcAddress("glGetVertexAttribdv");
        GetVertexAttribdv(সূচক, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetVertexAttribfv(GLuint সূচক, GLenum pname, GLfloat *params)
    {
        GetVertexAttribfv = (PFNGETVERTEXATTRIBFVPROC)IntGetProcAddress("glGetVertexAttribfv");
        GetVertexAttribfv(সূচক, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetVertexAttribiv(GLuint সূচক, GLenum pname, GLint *params)
    {
        GetVertexAttribiv = (PFNGETVERTEXATTRIBIVPROC)IntGetProcAddress("glGetVertexAttribiv");
        GetVertexAttribiv(সূচক, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetVertexAttribPointerv(GLuint সূচক, GLenum pname, GLvoid* *pointer)
    {
        GetVertexAttribPointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)IntGetProcAddress("glGetVertexAttribPointerv");
        GetVertexAttribPointerv(সূচক, pname, pointer);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsProgram(GLuint program)
    {
        IsProgram = (PFNISPROGRAMPROC)IntGetProcAddress("glIsProgram");
        ফেরত IsProgram(program);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)IntGetProcAddress("glIsShader");
        ফেরত IsShader(shader);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_LinkProgram(GLuint program)
    {
        LinkProgram = (PFNLINKPROGRAMPROC)IntGetProcAddress("glLinkProgram");
        LinkProgram(program);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ShaderSource(GLuint shader, GLsizei গুনতি_2, ধ্রুবক GLchar* ধ্রুবক *string, ধ্রুবক GLint *length)
    {
        ShaderSource = (PFNSHADERSOURCEPROC)IntGetProcAddress("glShaderSource");
        ShaderSource(shader, গুনতি_2, string, length);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UseProgram(GLuint program)
    {
        UseProgram = (PFNUSEPROGRAMPROC)IntGetProcAddress("glUseProgram");
        UseProgram(program);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)IntGetProcAddress("glUniform1f");
        Uniform1f(location, v0);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)IntGetProcAddress("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)IntGetProcAddress("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)IntGetProcAddress("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)IntGetProcAddress("glUniform1i");
        Uniform1i(location, v0);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)IntGetProcAddress("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)IntGetProcAddress("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)IntGetProcAddress("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)IntGetProcAddress("glUniform1fv");
        Uniform1fv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)IntGetProcAddress("glUniform2fv");
        Uniform2fv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)IntGetProcAddress("glUniform3fv");
        Uniform3fv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)IntGetProcAddress("glUniform4fv");
        Uniform4fv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)IntGetProcAddress("glUniform1iv");
        Uniform1iv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)IntGetProcAddress("glUniform2iv");
        Uniform2iv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)IntGetProcAddress("glUniform3iv");
        Uniform3iv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)IntGetProcAddress("glUniform4iv");
        Uniform4iv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix2fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix2fv = (PFNUNIFORMMATRIX2FVPROC)IntGetProcAddress("glUniformMatrix2fv");
        UniformMatrix2fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix3fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix3fv = (PFNUNIFORMMATRIX3FVPROC)IntGetProcAddress("glUniformMatrix3fv");
        UniformMatrix3fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix4fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix4fv = (PFNUNIFORMMATRIX4FVPROC)IntGetProcAddress("glUniformMatrix4fv");
        UniformMatrix4fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ValidateProgram(GLuint program)
    {
        ValidateProgram = (PFNVALIDATEPROGRAMPROC)IntGetProcAddress("glValidateProgram");
        ValidateProgram(program);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribPointer(GLuint সূচক, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ধ্রুবক GLvoid *pointer)
    {
        VertexAttribPointer = (PFNVERTEXATTRIBPOINTERPROC)IntGetProcAddress("glVertexAttribPointer");
        VertexAttribPointer(সূচক, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix2x3fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)IntGetProcAddress("glUniformMatrix2x3fv");
        UniformMatrix2x3fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix3x2fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)IntGetProcAddress("glUniformMatrix3x2fv");
        UniformMatrix3x2fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix2x4fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)IntGetProcAddress("glUniformMatrix2x4fv");
        UniformMatrix2x4fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix4x2fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)IntGetProcAddress("glUniformMatrix4x2fv");
        UniformMatrix4x2fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix3x4fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)IntGetProcAddress("glUniformMatrix3x4fv");
        UniformMatrix3x4fv(location, গুনতি_2, transpose, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformMatrix4x3fv(GLint location, GLsizei গুনতি_2, GLboolean transpose, ধ্রুবক GLfloat *value)
    {
        UniformMatrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)IntGetProcAddress("glUniformMatrix4x3fv");
        UniformMatrix4x3fv(location, গুনতি_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BindVertexArray(GLuint ren_array)
    {
        BindVertexArray = (PFNBINDVERTEXARRAYPROC)IntGetProcAddress("glBindVertexArray");
        BindVertexArray(ren_array);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteVertexArrays(GLsizei n, ধ্রুবক GLuint *arrays)
    {
        DeleteVertexArrays = (PFNDELETEVERTEXARRAYSPROC)IntGetProcAddress("glDeleteVertexArrays");
        DeleteVertexArrays(n, arrays);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenVertexArrays(GLsizei n, GLuint *arrays)
    {
        GenVertexArrays = (PFNGENVERTEXARRAYSPROC)IntGetProcAddress("glGenVertexArrays");
        GenVertexArrays(n, arrays);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsVertexArray(GLuint ren_array)
    {
        IsVertexArray = (PFNISVERTEXARRAYPROC)IntGetProcAddress("glIsVertexArray");
        ফেরত IsVertexArray(ren_array);
    }

    // Extension: ARB_map_buffer_range

    স্থির GLvoid* CODEGEN_FUNCPTR Switch_Map_বাফার_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_বাফার_Range = (PFNMAPBUFFERRANGEPROC)IntGetProcAddress("glMapBufferRange");
        ফেরত Map_বাফার_Range(target, offset, length, access);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Flush_Mapped_বাফার_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_বাফার_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)IntGetProcAddress("glFlushMappedBufferRange");
        Flush_Mapped_বাফার_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)IntGetProcAddress("glIsRenderbuffer");
        ফেরত IsRenderbuffer(renderbuffer);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)IntGetProcAddress("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ধ্রুবক GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)IntGetProcAddress("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)IntGetProcAddress("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)IntGetProcAddress("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, width, height);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)IntGetProcAddress("glIsFramebuffer");
        ফেরত IsFramebuffer(framebuffer);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)IntGetProcAddress("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ধ্রুবক GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)IntGetProcAddress("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)IntGetProcAddress("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    স্থির GLenum CODEGEN_FUNCPTR Switch_CheckFramebufferStatus(GLenum target)
    {
        CheckFramebufferStatus = (PFNCHECKFRAMEBUFFERSTATUSPROC)IntGetProcAddress("glCheckFramebufferStatus");
        ফেরত CheckFramebufferStatus(target);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)IntGetProcAddress("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)IntGetProcAddress("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)IntGetProcAddress("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)IntGetProcAddress("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)IntGetProcAddress("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)IntGetProcAddress("glGenerateMipmap");
        GenerateMipmap(target);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)IntGetProcAddress("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)IntGetProcAddress("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, width, height);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)IntGetProcAddress("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ColorMaski(GLuint সূচক, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)IntGetProcAddress("glColorMaski");
        ColorMaski(সূচক, r, g, b, a);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint সূচক, GLboolean *তথ্য)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)IntGetProcAddress("glGetBooleani_v");
        GetBooleani_v(target, সূচক, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint সূচক, GLint *তথ্য)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)IntGetProcAddress("glGetIntegeri_v");
        GetIntegeri_v(target, সূচক, তথ্য);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint সূচক)
    {
        Enablei = (PFNENABLEIPROC)IntGetProcAddress("glEnablei");
        Enablei(target, সূচক);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint সূচক)
    {
        Disablei = (PFNDISABLEIPROC)IntGetProcAddress("glDisablei");
        Disablei(target, সূচক);
    }

    স্থির GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint সূচক)
    {
        IsEnabledi = (PFNISENABLEDIPROC)IntGetProcAddress("glIsEnabledi");
        ফেরত IsEnabledi(target, সূচক);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitiveMode)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)IntGetProcAddress("glBeginTransformFeedback");
        BeginTransformFeedback(primitiveMode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_EndTransformFeedback()
    {
        EndTransformFeedback = (PFNENDTRANSFORMFEEDBACKPROC)IntGetProcAddress("glEndTransformFeedback");
        EndTransformFeedback();
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Bind_বাফার_Range(GLenum target, GLuint সূচক, GLuint বাফার, GLintptr offset, GLsizeiptr size)
    {
        Bind_বাফার_Range = (PFNBINDBUFFERRANGEPROC)IntGetProcAddress("glBindBufferRange");
        Bind_বাফার_Range(target, সূচক, বাফার, offset, size);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Bind_বাফার_Base(GLenum target, GLuint সূচক, GLuint বাফার)
    {
        Bind_বাফার_Base = (PFNBINDBUFFERBASEPROC)IntGetProcAddress("glBindBufferBase");
        Bind_বাফার_Base(target, সূচক, বাফার);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint program, GLsizei গুনতি_2, ধ্রুবক GLchar* ধ্রুবক *varyings, GLenum বাফার_Mode)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)IntGetProcAddress("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(program, গুনতি_2, varyings, বাফার_Mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint program, GLuint সূচক, GLsizei buf_আকার, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)IntGetProcAddress("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(program, সূচক, buf_আকার, length, size, type, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)IntGetProcAddress("glClampColor");
        ClampColor(target, clamp);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum mode)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)IntGetProcAddress("glBeginConditionalRender");
        BeginConditionalRender(id, mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_EndConditionalRender()
    {
        EndConditionalRender = (PFNENDCONDITIONALRENDERPROC)IntGetProcAddress("glEndConditionalRender");
        EndConditionalRender();
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribIPointer(GLuint সূচক, GLint size, GLenum type, GLsizei stride, ধ্রুবক GLvoid *pointer)
    {
        VertexAttribIPointer = (PFNVERTEXATTRIBIPOINTERPROC)IntGetProcAddress("glVertexAttribIPointer");
        VertexAttribIPointer(সূচক, size, type, stride, pointer);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetVertexAttribIiv(GLuint সূচক, GLenum pname, GLint *params)
    {
        GetVertexAttribIiv = (PFNGETVERTEXATTRIBIIVPROC)IntGetProcAddress("glGetVertexAttribIiv");
        GetVertexAttribIiv(সূচক, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetVertexAttribIuiv(GLuint সূচক, GLenum pname, GLuint *params)
    {
        GetVertexAttribIuiv = (PFNGETVERTEXATTRIBIUIVPROC)IntGetProcAddress("glGetVertexAttribIuiv");
        GetVertexAttribIuiv(সূচক, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI1i(GLuint সূচক, GLint x)
    {
        VertexAttribI1i = (PFNVERTEXATTRIBI1IPROC)IntGetProcAddress("glVertexAttribI1i");
        VertexAttribI1i(সূচক, x);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI2i(GLuint সূচক, GLint x, GLint y)
    {
        VertexAttribI2i = (PFNVERTEXATTRIBI2IPROC)IntGetProcAddress("glVertexAttribI2i");
        VertexAttribI2i(সূচক, x, y);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI3i(GLuint সূচক, GLint x, GLint y, GLint z)
    {
        VertexAttribI3i = (PFNVERTEXATTRIBI3IPROC)IntGetProcAddress("glVertexAttribI3i");
        VertexAttribI3i(সূচক, x, y, z);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4i(GLuint সূচক, GLint x, GLint y, GLint z, GLint w)
    {
        VertexAttribI4i = (PFNVERTEXATTRIBI4IPROC)IntGetProcAddress("glVertexAttribI4i");
        VertexAttribI4i(সূচক, x, y, z, w);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI1ui(GLuint সূচক, GLuint x)
    {
        VertexAttribI1ui = (PFNVERTEXATTRIBI1UIPROC)IntGetProcAddress("glVertexAttribI1ui");
        VertexAttribI1ui(সূচক, x);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI2ui(GLuint সূচক, GLuint x, GLuint y)
    {
        VertexAttribI2ui = (PFNVERTEXATTRIBI2UIPROC)IntGetProcAddress("glVertexAttribI2ui");
        VertexAttribI2ui(সূচক, x, y);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI3ui(GLuint সূচক, GLuint x, GLuint y, GLuint z)
    {
        VertexAttribI3ui = (PFNVERTEXATTRIBI3UIPROC)IntGetProcAddress("glVertexAttribI3ui");
        VertexAttribI3ui(সূচক, x, y, z);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4ui(GLuint সূচক, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        VertexAttribI4ui = (PFNVERTEXATTRIBI4UIPROC)IntGetProcAddress("glVertexAttribI4ui");
        VertexAttribI4ui(সূচক, x, y, z, w);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI1iv(GLuint সূচক, ধ্রুবক GLint *v)
    {
        VertexAttribI1iv = (PFNVERTEXATTRIBI1IVPROC)IntGetProcAddress("glVertexAttribI1iv");
        VertexAttribI1iv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI2iv(GLuint সূচক, ধ্রুবক GLint *v)
    {
        VertexAttribI2iv = (PFNVERTEXATTRIBI2IVPROC)IntGetProcAddress("glVertexAttribI2iv");
        VertexAttribI2iv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI3iv(GLuint সূচক, ধ্রুবক GLint *v)
    {
        VertexAttribI3iv = (PFNVERTEXATTRIBI3IVPROC)IntGetProcAddress("glVertexAttribI3iv");
        VertexAttribI3iv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4iv(GLuint সূচক, ধ্রুবক GLint *v)
    {
        VertexAttribI4iv = (PFNVERTEXATTRIBI4IVPROC)IntGetProcAddress("glVertexAttribI4iv");
        VertexAttribI4iv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI1uiv(GLuint সূচক, ধ্রুবক GLuint *v)
    {
        VertexAttribI1uiv = (PFNVERTEXATTRIBI1UIVPROC)IntGetProcAddress("glVertexAttribI1uiv");
        VertexAttribI1uiv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI2uiv(GLuint সূচক, ধ্রুবক GLuint *v)
    {
        VertexAttribI2uiv = (PFNVERTEXATTRIBI2UIVPROC)IntGetProcAddress("glVertexAttribI2uiv");
        VertexAttribI2uiv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI3uiv(GLuint সূচক, ধ্রুবক GLuint *v)
    {
        VertexAttribI3uiv = (PFNVERTEXATTRIBI3UIVPROC)IntGetProcAddress("glVertexAttribI3uiv");
        VertexAttribI3uiv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4uiv(GLuint সূচক, ধ্রুবক GLuint *v)
    {
        VertexAttribI4uiv = (PFNVERTEXATTRIBI4UIVPROC)IntGetProcAddress("glVertexAttribI4uiv");
        VertexAttribI4uiv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4bv(GLuint সূচক, ধ্রুবক GLbyte *v)
    {
        VertexAttribI4bv = (PFNVERTEXATTRIBI4BVPROC)IntGetProcAddress("glVertexAttribI4bv");
        VertexAttribI4bv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4sv(GLuint সূচক, ধ্রুবক GLshort *v)
    {
        VertexAttribI4sv = (PFNVERTEXATTRIBI4SVPROC)IntGetProcAddress("glVertexAttribI4sv");
        VertexAttribI4sv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4ubv(GLuint সূচক, ধ্রুবক GLubyte *v)
    {
        VertexAttribI4ubv = (PFNVERTEXATTRIBI4UBVPROC)IntGetProcAddress("glVertexAttribI4ubv");
        VertexAttribI4ubv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexAttribI4usv(GLuint সূচক, ধ্রুবক GLushort *v)
    {
        VertexAttribI4usv = (PFNVERTEXATTRIBI4USVPROC)IntGetProcAddress("glVertexAttribI4usv");
        VertexAttribI4usv(সূচক, v);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint program, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)IntGetProcAddress("glGetUniformuiv");
        GetUniformuiv(program, location, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Bind_Frag_তথ্য_Location(GLuint program, GLuint color, ধ্রুবক GLchar *name)
    {
        Bind_Frag_তথ্য_Location = (PFNBINDFRAGDATALOCATIONPROC)IntGetProcAddress("glBindFragDataLocation");
        Bind_Frag_তথ্য_Location(program, color, name);
    }

    স্থির GLint CODEGEN_FUNCPTR Switch_Get_Frag_তথ্য_Location(GLuint program, ধ্রুবক GLchar *name)
    {
        Get_Frag_তথ্য_Location = (PFNGETFRAGDATALOCATIONPROC)IntGetProcAddress("glGetFragDataLocation");
        ফেরত Get_Frag_তথ্য_Location(program, name);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)IntGetProcAddress("glUniform1ui");
        Uniform1ui(location, v0);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)IntGetProcAddress("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)IntGetProcAddress("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)IntGetProcAddress("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)IntGetProcAddress("glUniform1uiv");
        Uniform1uiv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)IntGetProcAddress("glUniform2uiv");
        Uniform2uiv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)IntGetProcAddress("glUniform3uiv");
        Uniform3uiv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei গুনতি_2, ধ্রুবক GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)IntGetProcAddress("glUniform4uiv");
        Uniform4uiv(location, গুনতি_2, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ধ্রুবক GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)IntGetProcAddress("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ধ্রুবক GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)IntGetProcAddress("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)IntGetProcAddress("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)IntGetProcAddress("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum বাফার, GLint drawbuffer, ধ্রুবক GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)IntGetProcAddress("glClearBufferiv");
        ClearBufferiv(বাফার, drawbuffer, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum বাফার, GLint drawbuffer, ধ্রুবক GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)IntGetProcAddress("glClearBufferuiv");
        ClearBufferuiv(বাফার, drawbuffer, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum বাফার, GLint drawbuffer, ধ্রুবক GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)IntGetProcAddress("glClearBufferfv");
        ClearBufferfv(বাফার, drawbuffer, value);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum বাফার, GLint drawbuffer, GLfloat depth, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)IntGetProcAddress("glClearBufferfi");
        ClearBufferfi(বাফার, drawbuffer, depth, stencil);
    }

    স্থির ধ্রুবক GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint সূচক)
    {
        GetStringi = (PFNGETSTRINGIPROC)IntGetProcAddress("glGetStringi");
        ফেরত GetStringi(name, সূচক);
    }

    // Extension: ARB_uniform_buffer_object

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetUniformIndices(GLuint program, GLsizei uniform_গুনতি, ধ্রুবক GLchar* ধ্রুবক *uniformNames, GLuint *uniformIndices)
    {
        GetUniformIndices = (PFNGETUNIFORMINDICESPROC)IntGetProcAddress("glGetUniformIndices");
        GetUniformIndices(program, uniform_গুনতি, uniformNames, uniformIndices);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetActiveUniformsiv(GLuint program, GLsizei uniform_গুনতি, ধ্রুবক GLuint *uniformIndices, GLenum pname, GLint *params)
    {
        GetActiveUniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)IntGetProcAddress("glGetActiveUniformsiv");
        GetActiveUniformsiv(program, uniform_গুনতি, uniformIndices, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetActiveUniformName(GLuint program, GLuint uniform_সূচক, GLsizei buf_আকার, GLsizei *length, GLchar *uniformName)
    {
        GetActiveUniformName = (PFNGETACTIVEUNIFORMNAMEPROC)IntGetProcAddress("glGetActiveUniformName");
        GetActiveUniformName(program, uniform_সূচক, buf_আকার, length, uniformName);
    }

    স্থির GLuint CODEGEN_FUNCPTR Switch_Get_Uniform_Block_সূচক(GLuint program, ধ্রুবক GLchar *uniformBlockName)
    {
        Get_Uniform_Block_সূচক = (PFNGETUNIFORMBLOCKINDEXPROC)IntGetProcAddress("glGetUniformBlockIndex");
        ফেরত Get_Uniform_Block_সূচক(program, uniformBlockName);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetActiveUniformBlockiv(GLuint program, GLuint uniform_Block_সূচক, GLenum pname, GLint *params)
    {
        GetActiveUniformBlockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)IntGetProcAddress("glGetActiveUniformBlockiv");
        GetActiveUniformBlockiv(program, uniform_Block_সূচক, pname, params);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_GetActiveUniformBlockName(GLuint program, GLuint uniform_Block_সূচক, GLsizei buf_আকার, GLsizei *length, GLchar *uniformBlockName)
    {
        GetActiveUniformBlockName = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)IntGetProcAddress("glGetActiveUniformBlockName");
        GetActiveUniformBlockName(program, uniform_Block_সূচক, buf_আকার, length, uniformBlockName);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_UniformBlockBinding(GLuint program, GLuint uniform_Block_সূচক, GLuint uniformBlockBinding)
    {
        UniformBlockBinding = (PFNUNIFORMBLOCKBINDINGPROC)IntGetProcAddress("glUniformBlockBinding");
        UniformBlockBinding(program, uniform_Block_সূচক, uniformBlockBinding);
    }

    // Extension: ARB_copy_buffer

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Copy_বাফার_Sub_তথ্য(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
    {
        Copy_বাফার_Sub_তথ্য = (PFNCOPYBUFFERSUBDATAPROC)IntGetProcAddress("glCopyBufferSubData");
        Copy_বাফার_Sub_তথ্য(readTarget, writeTarget, readOffset, writeOffset, size);
    }

    // Extension: 3.1

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum mode, GLint first, GLsizei গুনতি_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)IntGetProcAddress("glDrawArraysInstanced");
        DrawArraysInstanced(mode, first, গুনতি_2, instancecount);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum mode, GLsizei গুনতি_2, GLenum type, ধ্রুবক GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)IntGetProcAddress("glDrawElementsInstanced");
        DrawElementsInstanced(mode, গুনতি_2, type, indices, instancecount);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Tex_বাফার(GLenum target, GLenum internalformat, GLuint বাফার)
    {
        Tex_বাফার = (PFNTEXBUFFERPROC)IntGetProcAddress("glTexBuffer");
        Tex_বাফার(target, internalformat, বাফার);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Primitive_Restart_সূচক(GLuint সূচক)
    {
        Primitive_Restart_সূচক = (PFNPRIMITIVERESTARTINDEXPROC)IntGetProcAddress("glPrimitiveRestartIndex");
        Primitive_Restart_সূচক(সূচক);
    }

    // Legacy

    স্থির শূন্য CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)IntGetProcAddress("glEnableClientState");
        EnableClientState(cap);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)IntGetProcAddress("glDisableClientState");
        DisableClientState(cap);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_VertexPointer(GLint size, GLenum type, GLsizei stride, ধ্রুবক GLvoid *ptr)
    {
        VertexPointer = (PFNVERTEXPOINTERPROC)IntGetProcAddress("glVertexPointer");
        VertexPointer(size, type, stride, ptr);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_NormalPointer(GLenum type, GLsizei stride, ধ্রুবক GLvoid *ptr)
    {
        NormalPointer = (PFNNORMALPOINTERPROC)IntGetProcAddress("glNormalPointer");
        NormalPointer(type, stride, ptr);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_ColorPointer(GLint size, GLenum type, GLsizei stride, ধ্রুবক GLvoid *ptr)
    {
        ColorPointer = (PFNCOLORPOINTERPROC)IntGetProcAddress("glColorPointer");
        ColorPointer(size, type, stride, ptr);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexCoordPointer(GLint size, GLenum type, GLsizei stride, ধ্রুবক GLvoid *ptr)
    {
        TexCoordPointer = (PFNTEXCOORDPOINTERPROC)IntGetProcAddress("glTexCoordPointer");
        TexCoordPointer(size, type, stride, ptr);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)IntGetProcAddress("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_MatrixMode(GLenum mode)
    {
        MatrixMode = (PFNMATRIXMODEPROC)IntGetProcAddress("glMatrixMode");
        MatrixMode(mode);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_LoadIdentity(শূন্য)
    {
        LoadIdentity = (PFNLOADIDENTITYPROC)IntGetProcAddress("glLoadIdentity");
        LoadIdentity();
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Ortho(GLdouble বাম, GLdouble ডান, GLdouble bottom, GLdouble শীর্ষ_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)IntGetProcAddress("glOrtho");
        Ortho(বাম, ডান, bottom, শীর্ষ_2, near_val, far_val);
    }

    স্থির শূন্য CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)IntGetProcAddress("glColor3d");
        Color3d(red, green, blue);
    }

    কাঠামো InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            সারি_Width = Switch_সারি_Width;
            Point_আকার = Switch_Point_আকার;
            PolygonMode = Switch_PolygonMode;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_বাফার = Switch_Draw_বাফার;
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
            Read_বাফার = Switch_Read_বাফার;
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
            Bind_বাফার = Switch_Bind_বাফার;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_বাফার = Switch_Is_বাফার;
            বাফার_তথ্য = Switch_বাফার_তথ্য;
            বাফার_Sub_তথ্য = Switch_বাফার_Sub_তথ্য;
            Get_বাফার_Sub_তথ্য = Switch_Get_বাফার_Sub_তথ্য;
            Map_বাফার = Switch_Map_বাফার;
            Unmap_বাফার = Switch_Unmap_বাফার;
            Get_বাফার_Parameteriv = Switch_Get_বাফার_Parameteriv;
            Get_বাফার_Pointerv = Switch_Get_বাফার_Pointerv;

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
            Map_বাফার_Range = Switch_Map_বাফার_Range;
            Flush_Mapped_বাফার_Range = Switch_Flush_Mapped_বাফার_Range;

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
            Bind_বাফার_Range = Switch_Bind_বাফার_Range;
            Bind_বাফার_Base = Switch_Bind_বাফার_Base;
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
            Bind_Frag_তথ্য_Location = Switch_Bind_Frag_তথ্য_Location;
            Get_Frag_তথ্য_Location = Switch_Get_Frag_তথ্য_Location;
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
            Get_Uniform_Block_সূচক = Switch_Get_Uniform_Block_সূচক;
            GetActiveUniformBlockiv = Switch_GetActiveUniformBlockiv;
            GetActiveUniformBlockName = Switch_GetActiveUniformBlockName;
            UniformBlockBinding = Switch_UniformBlockBinding;

            // Extension: ARB_copy_buffer
            Copy_বাফার_Sub_তথ্য = Switch_Copy_বাফার_Sub_তথ্য;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_বাফার = Switch_Tex_বাফার;
            Primitive_Restart_সূচক = Switch_Primitive_Restart_সূচক;

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
