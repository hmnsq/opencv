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

#ضمن "precomp.hpp"
#ضمن "gl_core_3_1.hpp"

#جيڪڏهن_وصف_ٿيل HAVE_OPENGL

    #جيڪڏهن_وصف_ٿيل __APPLE__
        #ضمن <dlfcn.h>

        ساكن فراغ* Apple_GLGet_Proc_پتو (ثابت حرف* name)
        {
            ساكن منطقي initialized = خطأ;
            ساكن فراغ * handle = عدم;
            إذا (!handle)
            {
                إذا (!initialized)
                {
                    initialized = سچ;
                    ثابت حرف * ثابت رستو = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(رستو, RTLD_LAZY | RTLD_GLOBAL);
                }
                إذا (!handle)
                    أرجع عدم;
            }
            أرجع dlsym(handle, name);
        }
    #جيڪڏهن_پورو // __APPLE__

    #إذا defined(__sgi) || defined (__sun)
        #ضمن <dlfcn.h>
        #ضمن <stdio.h>

        ساكن فراغ* Sun_Get_Proc_پتو (ثابت حرف* name)
        {
            تعريفنوع فراغ* (func_t*)(ثابت GLubyte*);

            ساكن فراغ* h = 0;
            ساكن func_t gpa = 0;

            إذا (!h)
            {
                h = dlopen(عدم, RTLD_LAZY | RTLD_LOCAL);
                إذا (!h)
                    أرجع 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            أرجع gpa ? gpa((ثابت GLubyte*) name) : dlsym(h, name);
        }
    #جيڪڏهن_پورو // __sgi || __sun

    #إذا defined(_WIN32)
        #جيڪڏهن_وصف_ٿيل _MSC_VER
            #هدايت warning(disable: 4055)
            #هدايت warning(disable: 4054)
        #جيڪڏهن_پورو

        ساكن صحيح Test_اشارو(ثابت PROC pTest)
        {
            إذا(!pTest)
                أرجع 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            إذا (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                أرجع 0;

            أرجع 1;
        }

        ساكن PROC Win_Get_Proc_پتو(ثابت حرف* name)
        {
            PROC pFunc = wgl_Get_Proc_پتو((LPCSTR) name);
            إذا (Test_اشارو(pFunc))
                أرجع pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            أرجع (PROC) Get_Proc_پتو(glMod, (LPCSTR) name);
        }
    #جيڪڏهن_پورو // _WIN32

    #إذا defined(_WIN32)
        #وصف_ڪريو CV_GL_GET_PROC_پتو(name) Win_Get_Proc_پتو(name)
    #وإلاإذا defined(__APPLE__)
        #وصف_ڪريو CV_GL_GET_PROC_پتو(name) Apple_GLGet_Proc_پتو(name)
    #وإلاإذا defined(__sgi) || defined(__sun)
        #وصف_ڪريو CV_GL_GET_PROC_پتو(name) Sun_Get_Proc_پتو(name)
    #وإلا // GLX
        #ضمن <GL/glx.h>

        #وصف_ڪريو CV_GL_GET_PROC_پتو(name) gl_XGet_Proc_پتو_ARB((ثابت GLubyte*) name)
    #جيڪڏهن_پورو

    ساكن فراغ* Int_Get_Proc_پتو(ثابت حرف* name)
    {
        فراغ* func =  (فراغ*) CV_GL_GET_PROC_پتو(name);
        إذا (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        أرجع func;
    }
#وإلا
#إذا defined(_MSC_VER)
    #هدايت warning(disable : 4702)  // unreachable code
#جيڪڏهن_پورو
    ساكن فراغ* Int_Get_Proc_پتو(ثابت حرف*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#جيڪڏهن_پورو

نطاق gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    تعريفنوع GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    تعريفنوع ثابت GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(ثابت GLubyte *);

    // Extension: 1.2
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , ثابت GLint *, ثابت GLsizei *, GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , ثابت GLsizei *, GLenum , ثابت GLvoid* ثابت *, GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , ثابت GLint *);

    // Extension: 1.5
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , ثابت GLuint *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , ثابت GLvoid *, GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    تعريفنوع GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , ثابت GLenum *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , ثابت GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    تعريفنوع GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    تعريفنوع GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    تعريفنوع GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , ثابت GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    تعريفنوع GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , ثابت GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , ثابت GLchar* ثابت *, ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , ثابت GLvoid *);

    // Extension: 2.1
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , ثابت GLfloat *);

    // Extension: ARB_vertex_array_object
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    تعريفنوع GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    تعريفنوع GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    تعريفنوع GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , ثابت GLchar* ثابت *, GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , ثابت GLbyte *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , ثابت GLshort *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , ثابت GLubyte *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , ثابت GLushort *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , ثابت GLchar *);
    تعريفنوع GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , ثابت GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , ثابت GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , ثابت GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , ثابت GLfloat *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    تعريفنوع ثابت GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , ثابت GLchar* ثابت *, GLuint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , ثابت GLuint *, GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    تعريفنوع GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , ثابت GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , ثابت GLvoid *, GLsizei );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , ثابت GLvoid *);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(فراغ);
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    تعريفنوع فراغ (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC سٽ_ويڪر;
    PFNPOINTSIZEPROC نقطو_سائيز;
    PFNPOLYGONMODEPROC Polygon_گهڻائي;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_بفر;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC Clear_اونهائي;
    PFNSTENCILMASKPROC Stencil_نقاب;
    PFNCOLORMASKPROC Color_نقاب;
    PFNDEPTHMASKPROC اونهائي_نقاب;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC پڄاڻي;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC اونهائي_Func;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC Read_بفر;
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
    PFNDEPTHRANGEPROC اونهائي_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_وٿي;
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
    PFNACTIVETEXTUREPROC سرگرم_Texture;
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
    PFNPOINTPARAMETERFPROC نقطو_Parameterf;
    PFNPOINTPARAMETERFVPROC نقطو_Parameterfv;
    PFNPOINTPARAMETERIPROC نقطو_Parameteri;
    PFNPOINTPARAMETERIVPROC نقطو_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC پڇاڙي_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_بفر;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_بفر;
    PFNBUFFERDATAPROC بفر_معلومات;
    PFNBUFFERSUBDATAPROC بفر_Sub_معلومات;
    PFNGETBUFFERSUBDATAPROC Get_بفر_Sub_معلومات;
    PFNMAPBUFFERPROC Map_بفر;
    PFNUNMAPBUFFERPROC Unmap_بفر;
    PFNGETBUFFERPARAMETERIVPROC Get_بفر_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_بفر_Pointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC Stencil_نقاب_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_پروگرام;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_پروگرام;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC Disable_راس_Attrib_Array;
    PFNENABLEVERTEXATTRIBARRAYPROC Enable_راس_Attrib_Array;
    PFNGETACTIVEATTRIBPROC Get_سرگرم_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_سرگرم_هڪجهڙو;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_پروگرام_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_ذريعو;
    PFNGETUNIFORMLOCATIONPROC Get_هڪجهڙو_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_راس_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_راس_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_راس_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_راس_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_پروگرام;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_پروگرام;
    PFNSHADERSOURCEPROC Shader_ذريعو;
    PFNUSEPROGRAMPROC Use_پروگرام;
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
    PFNUNIFORMMATRIX2FVPROC هڪجهڙو_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC هڪجهڙو_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC هڪجهڙو_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_پروگرام;
    PFNVERTEXATTRIBPOINTERPROC راس_Attrib_اشارو;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC هڪجهڙو_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC هڪجهڙو_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC هڪجهڙو_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC هڪجهڙو_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC هڪجهڙو_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC هڪجهڙو_Matrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC Bind_راس_Array;
    PFNDELETEVERTEXARRAYSPROC Delete_راس_Arrays;
    PFNGENVERTEXARRAYSPROC Gen_راس_Arrays;
    PFNISVERTEXARRAYPROC Is_راس_Array;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC Map_بفر_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_بفر_Range;

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
    PFNCHECKFRAMEBUFFERSTATUSPROC Check_Framebuffer_حالت;
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
    PFNENDTRANSFORMFEEDBACKPROC پڇاڙي_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_بفر_Range;
    PFNBINDBUFFERBASEPROC Bind_بفر_بنياد;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC پڇاڙي_Conditional_Render;
    PFNVERTEXATTRIBIPOINTERPROC راس_Attrib_IPointer;
    PFNGETVERTEXATTRIBIIVPROC Get_راس_Attrib_Iiv;
    PFNGETVERTEXATTRIBIUIVPROC Get_راس_Attrib_Iuiv;
    PFNVERTEXATTRIBI1IPROC راس_Attrib_I1i;
    PFNVERTEXATTRIBI2IPROC راس_Attrib_I2i;
    PFNVERTEXATTRIBI3IPROC راس_Attrib_I3i;
    PFNVERTEXATTRIBI4IPROC راس_Attrib_I4i;
    PFNVERTEXATTRIBI1UIPROC راس_Attrib_I1ui;
    PFNVERTEXATTRIBI2UIPROC راس_Attrib_I2ui;
    PFNVERTEXATTRIBI3UIPROC راس_Attrib_I3ui;
    PFNVERTEXATTRIBI4UIPROC راس_Attrib_I4ui;
    PFNVERTEXATTRIBI1IVPROC راس_Attrib_I1iv;
    PFNVERTEXATTRIBI2IVPROC راس_Attrib_I2iv;
    PFNVERTEXATTRIBI3IVPROC راس_Attrib_I3iv;
    PFNVERTEXATTRIBI4IVPROC راس_Attrib_I4iv;
    PFNVERTEXATTRIBI1UIVPROC راس_Attrib_I1uiv;
    PFNVERTEXATTRIBI2UIVPROC راس_Attrib_I2uiv;
    PFNVERTEXATTRIBI3UIVPROC راس_Attrib_I3uiv;
    PFNVERTEXATTRIBI4UIVPROC راس_Attrib_I4uiv;
    PFNVERTEXATTRIBI4BVPROC راس_Attrib_I4bv;
    PFNVERTEXATTRIBI4SVPROC راس_Attrib_I4sv;
    PFNVERTEXATTRIBI4UBVPROC راس_Attrib_I4ubv;
    PFNVERTEXATTRIBI4USVPROC راس_Attrib_I4usv;
    PFNGETUNIFORMUIVPROC GetUniformuiv;
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_معلومات_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_معلومات_Location;
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
    PFNGETUNIFORMINDICESPROC Get_هڪجهڙو_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_سرگرم_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_سرگرم_هڪجهڙو_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_هڪجهڙو_بلاڪ_اشاريو;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_سرگرم_هڪجهڙو_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_سرگرم_هڪجهڙو_بلاڪ_Name;
    PFNUNIFORMBLOCKBINDINGPROC هڪجهڙو_بلاڪ_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_بفر_Sub_معلومات;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_بفر;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_اشاريو;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC راس_اشارو;
    PFNNORMALPOINTERPROC عام_اشارو;
    PFNCOLORPOINTERPROC Color_اشارو;
    PFNTEXCOORDPOINTERPROC Tex_Coord_اشارو;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC ميٽرڪس_گهڻائي;
    PFNLOADIDENTITYPROC ڀريو_سڃاڻپ_عنصر;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_CullFace(GLenum گهڻائي)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_پتو("glCullFace");
        CullFace(گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FrontFace(GLenum گهڻائي)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_پتو("glFrontFace");
        FrontFace(گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum گهڻائي)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_پتو("glHint");
        Hint(target, گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_سٽ_ويڪر(GLfloat ويڪر)
    {
        سٽ_ويڪر = (PFNLINEWIDTHPROC)Int_Get_Proc_پتو("glLineWidth");
        سٽ_ويڪر(ويڪر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطو_سائيز(GLfloat size)
    {
        نقطو_سائيز = (PFNPOINTSIZEPROC)Int_Get_Proc_پتو("glPointSize");
        نقطو_سائيز(size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_گهڻائي(GLenum face, GLenum گهڻائي)
    {
        Polygon_گهڻائي = (PFNPOLYGONMODEPROC)Int_Get_Proc_پتو("glPolygonMode");
        Polygon_گهڻائي(face, گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei ويڪر, GLsizei اوچائي)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_پتو("glScissor");
        Scissor(x, y, ويڪر, اوچائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_پتو("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ثابت GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_پتو("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_پتو("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_پتو("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei ويڪر, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_پتو("glTexImage1D");
        TexImage1D(target, level, internalformat, ويڪر, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei ويڪر, GLsizei اوچائي, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_پتو("glTexImage2D");
        TexImage2D(target, level, internalformat, ويڪر, اوچائي, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Draw_بفر(GLenum گهڻائي)
    {
        Draw_بفر = (PFNDRAWBUFFERPROC)Int_Get_Proc_پتو("glDrawBuffer");
        Draw_بفر(گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear(GLbitfield نقاب)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_پتو("glClear");
        Clear(نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_پتو("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_پتو("glClearStencil");
        ClearStencil(s);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear_اونهائي(GLdouble اونهائي)
    {
        Clear_اونهائي = (PFNCLEARDEPTHPROC)Int_Get_Proc_پتو("glClearDepth");
        Clear_اونهائي(اونهائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_نقاب(GLuint نقاب)
    {
        Stencil_نقاب = (PFNSTENCILMASKPROC)Int_Get_Proc_پتو("glStencilMask");
        Stencil_نقاب(نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_نقاب(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_نقاب = (PFNCOLORMASKPROC)Int_Get_Proc_پتو("glColorMask");
        Color_نقاب(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اونهائي_نقاب(GLboolean flag)
    {
        اونهائي_نقاب = (PFNDEPTHMASKPROC)Int_Get_Proc_پتو("glDepthMask");
        اونهائي_نقاب(flag);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_پتو("glDisable");
        Disable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_پتو("glEnable");
        Enable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پڄاڻي()
    {
        پڄاڻي = (PFNFINISHPROC)Int_Get_Proc_پتو("glFinish");
        پڄاڻي();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_پتو("glFlush");
        Flush();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_پتو("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_پتو("glLogicOp");
        LogicOp(opcode);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint نقاب)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_پتو("glStencilFunc");
        StencilFunc(func, ref, نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_پتو("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اونهائي_Func(GLenum func)
    {
        اونهائي_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_پتو("glDepthFunc");
        اونهائي_Func(func);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_پتو("glPixelStoref");
        PixelStoref(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_پتو("glPixelStorei");
        PixelStorei(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Read_بفر(GLenum گهڻائي)
    {
        Read_بفر = (PFNREADBUFFERPROC)Int_Get_Proc_پتو("glReadBuffer");
        Read_بفر(گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei ويڪر, GLsizei اوچائي, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_پتو("glReadPixels");
        ReadPixels(x, y, ويڪر, اوچائي, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_پتو("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_پتو("glGetDoublev");
        GetDoublev(pname, params);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_پتو("glGetError");
        أرجع GetError();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_پتو("glGetFloatv");
        GetFloatv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_پتو("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_پتو("glGetString");
        أرجع GetString(name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_پتو("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_پتو("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_پتو("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_پتو("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_پتو("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_پتو("glIsEnabled");
        أرجع IsEnabled(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اونهائي_Range(GLdouble ren_near, GLdouble ren_far)
    {
        اونهائي_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_پتو("glDepthRange");
        اونهائي_Range(ren_near, ren_far);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei ويڪر, GLsizei اوچائي)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_پتو("glViewport");
        Viewport(x, y, ويڪر, اوچائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArrays(GLenum گهڻائي, GLint first, GLsizei ڳڻپ_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_پتو("glDrawArrays");
        DrawArrays(گهڻائي, first, ڳڻپ_2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElements(GLenum گهڻائي, GLsizei ڳڻپ_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_پتو("glDrawElements");
        DrawElements(گهڻائي, ڳڻپ_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_پتو("glGetPointerv");
        GetPointerv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_وٿي(GLfloat ضرب_ڪندڙ, GLfloat units)
    {
        Polygon_وٿي = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_پتو("glPolygonOffset");
        Polygon_وٿي(ضرب_ڪندڙ, units);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ويڪر, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_پتو("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, ويڪر, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei ويڪر, GLsizei اوچائي, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_پتو("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, ويڪر, اوچائي, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei ويڪر)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_پتو("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, ويڪر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei ويڪر, GLsizei اوچائي)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_پتو("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, ويڪر, اوچائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei ويڪر, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_پتو("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, ويڪر, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ويڪر, GLsizei اوچائي, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_پتو("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, ويڪر, اوچائي, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_پتو("glBindTexture");
        BindTexture(target, texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ثابت GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_پتو("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_پتو("glGenTextures");
        GenTextures(n, textures);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_پتو("glIsTexture");
        أرجع IsTexture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_پتو("glIndexub");
        Indexub(c);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexubv(ثابت GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_پتو("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_پتو("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquation(GLenum گهڻائي)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_پتو("glBlendEquation");
        BlendEquation(گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum گهڻائي, GLuint شروعات, GLuint end, GLsizei ڳڻپ_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_پتو("glDrawRangeElements");
        DrawRangeElements(گهڻائي, شروعات, end, ڳڻپ_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ويڪر, GLsizei اوچائي, GLsizei اونهائي, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_پتو("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, ويڪر, اوچائي, اونهائي, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei ويڪر, GLsizei اوچائي)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_پتو("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, ويڪر, اوچائي);
    }

    // Extension: 1.3

    ساكن فراغ CODEGEN_FUNCPTR Switch_سرگرم_Texture(GLenum texture)
    {
        سرگرم_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_پتو("glActiveTexture");
        سرگرم_Texture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_پتو("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei ويڪر, GLsizei اوچائي, GLsizei اونهائي, GLint border, GLsizei image_سائيز, ثابت GLvoid *معلومات)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_پتو("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, ويڪر, اوچائي, اونهائي, border, image_سائيز, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei ويڪر, GLsizei اوچائي, GLint border, GLsizei image_سائيز, ثابت GLvoid *معلومات)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_پتو("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, ويڪر, اوچائي, border, image_سائيز, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei ويڪر, GLint border, GLsizei image_سائيز, ثابت GLvoid *معلومات)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_پتو("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, ويڪر, border, image_سائيز, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei ويڪر, GLsizei اوچائي, GLsizei اونهائي, GLenum format, GLsizei image_سائيز, ثابت GLvoid *معلومات)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_پتو("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, ويڪر, اوچائي, اونهائي, format, image_سائيز, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei ويڪر, GLsizei اوچائي, GLenum format, GLsizei image_سائيز, ثابت GLvoid *معلومات)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_پتو("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, ويڪر, اوچائي, format, image_سائيز, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei ويڪر, GLenum format, GLsizei image_سائيز, ثابت GLvoid *معلومات)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_پتو("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, ويڪر, format, image_سائيز, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_پتو("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_پتو("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum گهڻائي, ثابت GLint *first, ثابت GLsizei *ڳڻپ_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_پتو("glMultiDrawArrays");
        MultiDrawArrays(گهڻائي, first, ڳڻپ_2, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum گهڻائي, ثابت GLsizei *ڳڻپ_2, GLenum type, ثابت GLvoid* ثابت *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_پتو("glMultiDrawElements");
        MultiDrawElements(گهڻائي, ڳڻپ_2, type, indices, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطو_Parameterf(GLenum pname, GLfloat param)
    {
        نقطو_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_پتو("glPointParameterf");
        نقطو_Parameterf(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطو_Parameterfv(GLenum pname, ثابت GLfloat *params)
    {
        نقطو_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_پتو("glPointParameterfv");
        نقطو_Parameterfv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطو_Parameteri(GLenum pname, GLint param)
    {
        نقطو_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_پتو("glPointParameteri");
        نقطو_Parameteri(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطو_Parameteriv(GLenum pname, ثابت GLint *params)
    {
        نقطو_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_پتو("glPointParameteriv");
        نقطو_Parameteriv(pname, params);
    }

    // Extension: 1.5

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_پتو("glGenQueries");
        GenQueries(n, ids);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ثابت GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_پتو("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_پتو("glIsQuery");
        أرجع IsQuery(id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_پتو("glBeginQuery");
        BeginQuery(target, id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پڇاڙي_Query(GLenum target)
    {
        پڇاڙي_Query = (PFNENDQUERYPROC)Int_Get_Proc_پتو("glEndQuery");
        پڇاڙي_Query(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_پتو("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_پتو("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_پتو("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر(GLenum target, GLuint بفر)
    {
        Bind_بفر = (PFNBINDBUFFERPROC)Int_Get_Proc_پتو("glBindBuffer");
        Bind_بفر(target, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ثابت GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_پتو("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_پتو("glGenBuffers");
        GenBuffers(n, buffers);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_بفر(GLuint بفر)
    {
        Is_بفر = (PFNISBUFFERPROC)Int_Get_Proc_پتو("glIsBuffer");
        أرجع Is_بفر(بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بفر_معلومات(GLenum target, GLsizeiptr size, ثابت GLvoid *معلومات, GLenum usage)
    {
        بفر_معلومات = (PFNBUFFERDATAPROC)Int_Get_Proc_پتو("glBufferData");
        بفر_معلومات(target, size, معلومات, usage);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بفر_Sub_معلومات(GLenum target, GLintptr offset, GLsizeiptr size, ثابت GLvoid *معلومات)
    {
        بفر_Sub_معلومات = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_پتو("glBufferSubData");
        بفر_Sub_معلومات(target, offset, size, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Sub_معلومات(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *معلومات)
    {
        Get_بفر_Sub_معلومات = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_پتو("glGetBufferSubData");
        Get_بفر_Sub_معلومات(target, offset, size, معلومات);
    }

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_بفر(GLenum target, GLenum access)
    {
        Map_بفر = (PFNMAPBUFFERPROC)Int_Get_Proc_پتو("glMapBuffer");
        أرجع Map_بفر(target, access);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Unmap_بفر(GLenum target)
    {
        Unmap_بفر = (PFNUNMAPBUFFERPROC)Int_Get_Proc_پتو("glUnmapBuffer");
        أرجع Unmap_بفر(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_بفر_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_پتو("glGetBufferParameteriv");
        Get_بفر_Parameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_بفر_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_پتو("glGetBufferPointerv");
        Get_بفر_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum گهڻائي_RGB, GLenum گهڻائي_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_پتو("glBlendEquationSeparate");
        BlendEquationSeparate(گهڻائي_RGB, گهڻائي_Alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ثابت GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_پتو("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_پتو("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint نقاب)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_پتو("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_نقاب_Separate(GLenum face, GLuint نقاب)
    {
        Stencil_نقاب_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_پتو("glStencilMaskSeparate");
        Stencil_نقاب_Separate(face, نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_AttachShader(GLuint پروگرام, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_پتو("glAttachShader");
        AttachShader(پروگرام, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint پروگرام, GLuint اشاريو, ثابت GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_پتو("glBindAttribLocation");
        BindAttribLocation(پروگرام, اشاريو, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_پتو("glCompileShader");
        CompileShader(shader);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Create_پروگرام()
    {
        Create_پروگرام = (PFNCREATEPROGRAMPROC)Int_Get_Proc_پتو("glCreateProgram");
        أرجع Create_پروگرام();
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_پتو("glCreateShader");
        أرجع CreateShader(type);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_پروگرام(GLuint پروگرام)
    {
        Delete_پروگرام = (PFNDELETEPROGRAMPROC)Int_Get_Proc_پتو("glDeleteProgram");
        Delete_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_پتو("glDeleteShader");
        DeleteShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DetachShader(GLuint پروگرام, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_پتو("glDetachShader");
        DetachShader(پروگرام, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable_راس_Attrib_Array(GLuint اشاريو)
    {
        Disable_راس_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_پتو("glDisableVertexAttribArray");
        Disable_راس_Attrib_Array(اشاريو);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable_راس_Attrib_Array(GLuint اشاريو)
    {
        Enable_راس_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_پتو("glEnableVertexAttribArray");
        Enable_راس_Attrib_Array(اشاريو);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_Attrib(GLuint پروگرام, GLuint اشاريو, GLsizei buf_سائيز, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_سرگرم_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_پتو("glGetActiveAttrib");
        Get_سرگرم_Attrib(پروگرام, اشاريو, buf_سائيز, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_هڪجهڙو(GLuint پروگرام, GLuint اشاريو, GLsizei buf_سائيز, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_سرگرم_هڪجهڙو = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_پتو("glGetActiveUniform");
        Get_سرگرم_هڪجهڙو(پروگرام, اشاريو, buf_سائيز, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint پروگرام, GLsizei max_ڳڻپ, GLsizei *ڳڻپ_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_پتو("glGetAttachedShaders");
        GetAttachedShaders(پروگرام, max_ڳڻپ, ڳڻپ_2, obj);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint پروگرام, ثابت GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_پتو("glGetAttribLocation");
        أرجع GetAttribLocation(پروگرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint پروگرام, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_پتو("glGetProgramiv");
        GetProgramiv(پروگرام, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_پروگرام_Info_Log(GLuint پروگرام, GLsizei buf_سائيز, GLsizei *length, GLchar *infoLog)
    {
        Get_پروگرام_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_پتو("glGetProgramInfoLog");
        Get_پروگرام_Info_Log(پروگرام, buf_سائيز, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_پتو("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_سائيز, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_پتو("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_سائيز, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_Shader_ذريعو(GLuint shader, GLsizei buf_سائيز, GLsizei *length, GLchar *ذريعو)
    {
        Get_Shader_ذريعو = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_پتو("glGetShaderSource");
        Get_Shader_ذريعو(shader, buf_سائيز, length, ذريعو);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_هڪجهڙو_Location(GLuint پروگرام, ثابت GLchar *name)
    {
        Get_هڪجهڙو_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_پتو("glGetUniformLocation");
        أرجع Get_هڪجهڙو_Location(پروگرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint پروگرام, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_پتو("glGetUniformfv");
        GetUniformfv(پروگرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint پروگرام, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_پتو("glGetUniformiv");
        GetUniformiv(پروگرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribdv(GLuint اشاريو, GLenum pname, GLdouble *params)
    {
        Get_راس_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_پتو("glGetVertexAttribdv");
        Get_راس_Attribdv(اشاريو, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribfv(GLuint اشاريو, GLenum pname, GLfloat *params)
    {
        Get_راس_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_پتو("glGetVertexAttribfv");
        Get_راس_Attribfv(اشاريو, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribiv(GLuint اشاريو, GLenum pname, GLint *params)
    {
        Get_راس_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_پتو("glGetVertexAttribiv");
        Get_راس_Attribiv(اشاريو, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Pointerv(GLuint اشاريو, GLenum pname, GLvoid* *pointer)
    {
        Get_راس_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_پتو("glGetVertexAttribPointerv");
        Get_راس_Attrib_Pointerv(اشاريو, pname, pointer);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_پروگرام(GLuint پروگرام)
    {
        Is_پروگرام = (PFNISPROGRAMPROC)Int_Get_Proc_پتو("glIsProgram");
        أرجع Is_پروگرام(پروگرام);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_پتو("glIsShader");
        أرجع IsShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Link_پروگرام(GLuint پروگرام)
    {
        Link_پروگرام = (PFNLINKPROGRAMPROC)Int_Get_Proc_پتو("glLinkProgram");
        Link_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Shader_ذريعو(GLuint shader, GLsizei ڳڻپ_2, ثابت GLchar* ثابت *string, ثابت GLint *length)
    {
        Shader_ذريعو = (PFNSHADERSOURCEPROC)Int_Get_Proc_پتو("glShaderSource");
        Shader_ذريعو(shader, ڳڻپ_2, string, length);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Use_پروگرام(GLuint پروگرام)
    {
        Use_پروگرام = (PFNUSEPROGRAMPROC)Int_Get_Proc_پتو("glUseProgram");
        Use_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_پتو("glUniform1f");
        Uniform1f(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_پتو("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_پتو("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_پتو("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_پتو("glUniform1i");
        Uniform1i(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_پتو("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_پتو("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_پتو("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei ڳڻپ_2, ثابت GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_پتو("glUniform1fv");
        Uniform1fv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei ڳڻپ_2, ثابت GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_پتو("glUniform2fv");
        Uniform2fv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei ڳڻپ_2, ثابت GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_پتو("glUniform3fv");
        Uniform3fv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei ڳڻپ_2, ثابت GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_پتو("glUniform4fv");
        Uniform4fv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei ڳڻپ_2, ثابت GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_پتو("glUniform1iv");
        Uniform1iv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei ڳڻپ_2, ثابت GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_پتو("glUniform2iv");
        Uniform2iv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei ڳڻپ_2, ثابت GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_پتو("glUniform3iv");
        Uniform3iv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei ڳڻپ_2, ثابت GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_پتو("glUniform4iv");
        Uniform4iv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix2fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_پتو("glUniformMatrix2fv");
        هڪجهڙو_Matrix2fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix3fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_پتو("glUniformMatrix3fv");
        هڪجهڙو_Matrix3fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix4fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_پتو("glUniformMatrix4fv");
        هڪجهڙو_Matrix4fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Validate_پروگرام(GLuint پروگرام)
    {
        Validate_پروگرام = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_پتو("glValidateProgram");
        Validate_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_اشارو(GLuint اشاريو, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ثابت GLvoid *pointer)
    {
        راس_Attrib_اشارو = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_پتو("glVertexAttribPointer");
        راس_Attrib_اشارو(اشاريو, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix2x3fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_پتو("glUniformMatrix2x3fv");
        هڪجهڙو_Matrix2x3fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix3x2fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_پتو("glUniformMatrix3x2fv");
        هڪجهڙو_Matrix3x2fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix2x4fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_پتو("glUniformMatrix2x4fv");
        هڪجهڙو_Matrix2x4fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix4x2fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_پتو("glUniformMatrix4x2fv");
        هڪجهڙو_Matrix4x2fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix3x4fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_پتو("glUniformMatrix3x4fv");
        هڪجهڙو_Matrix3x4fv(location, ڳڻپ_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_Matrix4x3fv(GLint location, GLsizei ڳڻپ_2, GLboolean transpose, ثابت GLfloat *value)
    {
        هڪجهڙو_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_پتو("glUniformMatrix4x3fv");
        هڪجهڙو_Matrix4x3fv(location, ڳڻپ_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_راس_Array(GLuint ren_array)
    {
        Bind_راس_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_پتو("glBindVertexArray");
        Bind_راس_Array(ren_array);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_راس_Arrays(GLsizei n, ثابت GLuint *arrays)
    {
        Delete_راس_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_پتو("glDeleteVertexArrays");
        Delete_راس_Arrays(n, arrays);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Gen_راس_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_راس_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_پتو("glGenVertexArrays");
        Gen_راس_Arrays(n, arrays);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_راس_Array(GLuint ren_array)
    {
        Is_راس_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_پتو("glIsVertexArray");
        أرجع Is_راس_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_بفر_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_بفر_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_پتو("glMapBufferRange");
        أرجع Map_بفر_Range(target, offset, length, access);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush_Mapped_بفر_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_بفر_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_پتو("glFlushMappedBufferRange");
        Flush_Mapped_بفر_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_پتو("glIsRenderbuffer");
        أرجع IsRenderbuffer(renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_پتو("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ثابت GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_پتو("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_پتو("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei ويڪر, GLsizei اوچائي)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_پتو("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, ويڪر, اوچائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_پتو("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_پتو("glIsFramebuffer");
        أرجع IsFramebuffer(framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_پتو("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ثابت GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_پتو("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_پتو("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_حالت(GLenum target)
    {
        Check_Framebuffer_حالت = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_پتو("glCheckFramebufferStatus");
        أرجع Check_Framebuffer_حالت(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_پتو("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_پتو("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_پتو("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_پتو("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_پتو("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_پتو("glGenerateMipmap");
        GenerateMipmap(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield نقاب, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_پتو("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, نقاب, filter);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei ويڪر, GLsizei اوچائي)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_پتو("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, ويڪر, اوچائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_پتو("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_ColorMaski(GLuint اشاريو, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_پتو("glColorMaski");
        ColorMaski(اشاريو, r, g, b, a);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint اشاريو, GLboolean *معلومات)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_پتو("glGetBooleani_v");
        GetBooleani_v(target, اشاريو, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint اشاريو, GLint *معلومات)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_پتو("glGetIntegeri_v");
        GetIntegeri_v(target, اشاريو, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint اشاريو)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_پتو("glEnablei");
        Enablei(target, اشاريو);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint اشاريو)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_پتو("glDisablei");
        Disablei(target, اشاريو);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint اشاريو)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_پتو("glIsEnabledi");
        أرجع IsEnabledi(target, اشاريو);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_گهڻائي)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_پتو("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پڇاڙي_Transform_Feedback()
    {
        پڇاڙي_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_پتو("glEndTransformFeedback");
        پڇاڙي_Transform_Feedback();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر_Range(GLenum target, GLuint اشاريو, GLuint بفر, GLintptr offset, GLsizeiptr size)
    {
        Bind_بفر_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_پتو("glBindBufferRange");
        Bind_بفر_Range(target, اشاريو, بفر, offset, size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر_بنياد(GLenum target, GLuint اشاريو, GLuint بفر)
    {
        Bind_بفر_بنياد = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_پتو("glBindBufferBase");
        Bind_بفر_بنياد(target, اشاريو, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint پروگرام, GLsizei ڳڻپ_2, ثابت GLchar* ثابت *varyings, GLenum بفر_گهڻائي)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_پتو("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(پروگرام, ڳڻپ_2, varyings, بفر_گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint پروگرام, GLuint اشاريو, GLsizei buf_سائيز, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_پتو("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(پروگرام, اشاريو, buf_سائيز, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_پتو("glClampColor");
        ClampColor(target, clamp);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum گهڻائي)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_پتو("glBeginConditionalRender");
        BeginConditionalRender(id, گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پڇاڙي_Conditional_Render()
    {
        پڇاڙي_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_پتو("glEndConditionalRender");
        پڇاڙي_Conditional_Render();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_IPointer(GLuint اشاريو, GLint size, GLenum type, GLsizei stride, ثابت GLvoid *pointer)
    {
        راس_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_پتو("glVertexAttribIPointer");
        راس_Attrib_IPointer(اشاريو, size, type, stride, pointer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Iiv(GLuint اشاريو, GLenum pname, GLint *params)
    {
        Get_راس_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_پتو("glGetVertexAttribIiv");
        Get_راس_Attrib_Iiv(اشاريو, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Iuiv(GLuint اشاريو, GLenum pname, GLuint *params)
    {
        Get_راس_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_پتو("glGetVertexAttribIuiv");
        Get_راس_Attrib_Iuiv(اشاريو, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1i(GLuint اشاريو, GLint x)
    {
        راس_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_پتو("glVertexAttribI1i");
        راس_Attrib_I1i(اشاريو, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2i(GLuint اشاريو, GLint x, GLint y)
    {
        راس_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_پتو("glVertexAttribI2i");
        راس_Attrib_I2i(اشاريو, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3i(GLuint اشاريو, GLint x, GLint y, GLint z)
    {
        راس_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_پتو("glVertexAttribI3i");
        راس_Attrib_I3i(اشاريو, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4i(GLuint اشاريو, GLint x, GLint y, GLint z, GLint w)
    {
        راس_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_پتو("glVertexAttribI4i");
        راس_Attrib_I4i(اشاريو, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1ui(GLuint اشاريو, GLuint x)
    {
        راس_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_پتو("glVertexAttribI1ui");
        راس_Attrib_I1ui(اشاريو, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2ui(GLuint اشاريو, GLuint x, GLuint y)
    {
        راس_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_پتو("glVertexAttribI2ui");
        راس_Attrib_I2ui(اشاريو, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3ui(GLuint اشاريو, GLuint x, GLuint y, GLuint z)
    {
        راس_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_پتو("glVertexAttribI3ui");
        راس_Attrib_I3ui(اشاريو, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4ui(GLuint اشاريو, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        راس_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_پتو("glVertexAttribI4ui");
        راس_Attrib_I4ui(اشاريو, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1iv(GLuint اشاريو, ثابت GLint *v)
    {
        راس_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_پتو("glVertexAttribI1iv");
        راس_Attrib_I1iv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2iv(GLuint اشاريو, ثابت GLint *v)
    {
        راس_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_پتو("glVertexAttribI2iv");
        راس_Attrib_I2iv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3iv(GLuint اشاريو, ثابت GLint *v)
    {
        راس_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_پتو("glVertexAttribI3iv");
        راس_Attrib_I3iv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4iv(GLuint اشاريو, ثابت GLint *v)
    {
        راس_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_پتو("glVertexAttribI4iv");
        راس_Attrib_I4iv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1uiv(GLuint اشاريو, ثابت GLuint *v)
    {
        راس_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_پتو("glVertexAttribI1uiv");
        راس_Attrib_I1uiv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2uiv(GLuint اشاريو, ثابت GLuint *v)
    {
        راس_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_پتو("glVertexAttribI2uiv");
        راس_Attrib_I2uiv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3uiv(GLuint اشاريو, ثابت GLuint *v)
    {
        راس_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_پتو("glVertexAttribI3uiv");
        راس_Attrib_I3uiv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4uiv(GLuint اشاريو, ثابت GLuint *v)
    {
        راس_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_پتو("glVertexAttribI4uiv");
        راس_Attrib_I4uiv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4bv(GLuint اشاريو, ثابت GLbyte *v)
    {
        راس_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_پتو("glVertexAttribI4bv");
        راس_Attrib_I4bv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4sv(GLuint اشاريو, ثابت GLshort *v)
    {
        راس_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_پتو("glVertexAttribI4sv");
        راس_Attrib_I4sv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4ubv(GLuint اشاريو, ثابت GLubyte *v)
    {
        راس_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_پتو("glVertexAttribI4ubv");
        راس_Attrib_I4ubv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4usv(GLuint اشاريو, ثابت GLushort *v)
    {
        راس_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_پتو("glVertexAttribI4usv");
        راس_Attrib_I4usv(اشاريو, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint پروگرام, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_پتو("glGetUniformuiv");
        GetUniformuiv(پروگرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_Frag_معلومات_Location(GLuint پروگرام, GLuint color, ثابت GLchar *name)
    {
        Bind_Frag_معلومات_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_پتو("glBindFragDataLocation");
        Bind_Frag_معلومات_Location(پروگرام, color, name);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_Frag_معلومات_Location(GLuint پروگرام, ثابت GLchar *name)
    {
        Get_Frag_معلومات_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_پتو("glGetFragDataLocation");
        أرجع Get_Frag_معلومات_Location(پروگرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_پتو("glUniform1ui");
        Uniform1ui(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_پتو("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_پتو("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_پتو("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei ڳڻپ_2, ثابت GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_پتو("glUniform1uiv");
        Uniform1uiv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei ڳڻپ_2, ثابت GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_پتو("glUniform2uiv");
        Uniform2uiv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei ڳڻپ_2, ثابت GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_پتو("glUniform3uiv");
        Uniform3uiv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei ڳڻپ_2, ثابت GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_پتو("glUniform4uiv");
        Uniform4uiv(location, ڳڻپ_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_پتو("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ثابت GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_پتو("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_پتو("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_پتو("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum بفر, GLint drawbuffer, ثابت GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_پتو("glClearBufferiv");
        ClearBufferiv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum بفر, GLint drawbuffer, ثابت GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_پتو("glClearBufferuiv");
        ClearBufferuiv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum بفر, GLint drawbuffer, ثابت GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_پتو("glClearBufferfv");
        ClearBufferfv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum بفر, GLint drawbuffer, GLfloat اونهائي, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_پتو("glClearBufferfi");
        ClearBufferfi(بفر, drawbuffer, اونهائي, stencil);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint اشاريو)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_پتو("glGetStringi");
        أرجع GetStringi(name, اشاريو);
    }

    // Extension: ARB_uniform_buffer_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_هڪجهڙو_Indices(GLuint پروگرام, GLsizei هڪجهڙو_ڳڻپ, ثابت GLchar* ثابت *هڪجهڙو_نالا, GLuint *هڪجهڙو_Indices)
    {
        Get_هڪجهڙو_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_پتو("glGetUniformIndices");
        Get_هڪجهڙو_Indices(پروگرام, هڪجهڙو_ڳڻپ, هڪجهڙو_نالا, هڪجهڙو_Indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_Uniformsiv(GLuint پروگرام, GLsizei هڪجهڙو_ڳڻپ, ثابت GLuint *هڪجهڙو_Indices, GLenum pname, GLint *params)
    {
        Get_سرگرم_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_پتو("glGetActiveUniformsiv");
        Get_سرگرم_Uniformsiv(پروگرام, هڪجهڙو_ڳڻپ, هڪجهڙو_Indices, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_هڪجهڙو_Name(GLuint پروگرام, GLuint هڪجهڙو_اشاريو, GLsizei buf_سائيز, GLsizei *length, GLchar *هڪجهڙو_Name)
    {
        Get_سرگرم_هڪجهڙو_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_پتو("glGetActiveUniformName");
        Get_سرگرم_هڪجهڙو_Name(پروگرام, هڪجهڙو_اشاريو, buf_سائيز, length, هڪجهڙو_Name);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Get_هڪجهڙو_بلاڪ_اشاريو(GLuint پروگرام, ثابت GLchar *هڪجهڙو_بلاڪ_Name)
    {
        Get_هڪجهڙو_بلاڪ_اشاريو = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_پتو("glGetUniformBlockIndex");
        أرجع Get_هڪجهڙو_بلاڪ_اشاريو(پروگرام, هڪجهڙو_بلاڪ_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_هڪجهڙو_Blockiv(GLuint پروگرام, GLuint هڪجهڙو_بلاڪ_اشاريو, GLenum pname, GLint *params)
    {
        Get_سرگرم_هڪجهڙو_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_پتو("glGetActiveUniformBlockiv");
        Get_سرگرم_هڪجهڙو_Blockiv(پروگرام, هڪجهڙو_بلاڪ_اشاريو, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_هڪجهڙو_بلاڪ_Name(GLuint پروگرام, GLuint هڪجهڙو_بلاڪ_اشاريو, GLsizei buf_سائيز, GLsizei *length, GLchar *هڪجهڙو_بلاڪ_Name)
    {
        Get_سرگرم_هڪجهڙو_بلاڪ_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_پتو("glGetActiveUniformBlockName");
        Get_سرگرم_هڪجهڙو_بلاڪ_Name(پروگرام, هڪجهڙو_بلاڪ_اشاريو, buf_سائيز, length, هڪجهڙو_بلاڪ_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_هڪجهڙو_بلاڪ_Binding(GLuint پروگرام, GLuint هڪجهڙو_بلاڪ_اشاريو, GLuint هڪجهڙو_بلاڪ_Binding_2)
    {
        هڪجهڙو_بلاڪ_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_پتو("glUniformBlockBinding");
        هڪجهڙو_بلاڪ_Binding(پروگرام, هڪجهڙو_بلاڪ_اشاريو, هڪجهڙو_بلاڪ_Binding_2);
    }

    // Extension: ARB_copy_buffer

    ساكن فراغ CODEGEN_FUNCPTR Switch_Copy_بفر_Sub_معلومات(GLenum read_هدف, GLenum write_هدف, GLintptr read_وٿي, GLintptr write_وٿي, GLsizeiptr size)
    {
        Copy_بفر_Sub_معلومات = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_پتو("glCopyBufferSubData");
        Copy_بفر_Sub_معلومات(read_هدف, write_هدف, read_وٿي, write_وٿي, size);
    }

    // Extension: 3.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum گهڻائي, GLint first, GLsizei ڳڻپ_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_پتو("glDrawArraysInstanced");
        DrawArraysInstanced(گهڻائي, first, ڳڻپ_2, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum گهڻائي, GLsizei ڳڻپ_2, GLenum type, ثابت GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_پتو("glDrawElementsInstanced");
        DrawElementsInstanced(گهڻائي, ڳڻپ_2, type, indices, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_بفر(GLenum target, GLenum internalformat, GLuint بفر)
    {
        Tex_بفر = (PFNTEXBUFFERPROC)Int_Get_Proc_پتو("glTexBuffer");
        Tex_بفر(target, internalformat, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Primitive_Restart_اشاريو(GLuint اشاريو)
    {
        Primitive_Restart_اشاريو = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_پتو("glPrimitiveRestartIndex");
        Primitive_Restart_اشاريو(اشاريو);
    }

    // Legacy

    ساكن فراغ CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_پتو("glEnableClientState");
        EnableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_پتو("glDisableClientState");
        DisableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_اشارو(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        راس_اشارو = (PFNVERTEXPOINTERPROC)Int_Get_Proc_پتو("glVertexPointer");
        راس_اشارو(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_عام_اشارو(GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        عام_اشارو = (PFNNORMALPOINTERPROC)Int_Get_Proc_پتو("glNormalPointer");
        عام_اشارو(type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_اشارو(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Color_اشارو = (PFNCOLORPOINTERPROC)Int_Get_Proc_پتو("glColorPointer");
        Color_اشارو(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_Coord_اشارو(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Tex_Coord_اشارو = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_پتو("glTexCoordPointer");
        Tex_Coord_اشارو(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_پتو("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ميٽرڪس_گهڻائي(GLenum گهڻائي)
    {
        ميٽرڪس_گهڻائي = (PFNMATRIXMODEPROC)Int_Get_Proc_پتو("glMatrixMode");
        ميٽرڪس_گهڻائي(گهڻائي);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ڀريو_سڃاڻپ_عنصر(فراغ)
    {
        ڀريو_سڃاڻپ_عنصر = (PFNLOADIDENTITYPROC)Int_Get_Proc_پتو("glLoadIdentity");
        ڀريو_سڃاڻپ_عنصر();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Ortho(GLdouble کاٻي, GLdouble ساڄي, GLdouble bottom, GLdouble چوٽي_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_پتو("glOrtho");
        Ortho(کاٻي, ساڄي, bottom, چوٽي_2, near_val, far_val);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_پتو("glColor3d");
        Color3d(red, green, blue);
    }

    بنية InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            سٽ_ويڪر = Switch_سٽ_ويڪر;
            نقطو_سائيز = Switch_نقطو_سائيز;
            Polygon_گهڻائي = Switch_Polygon_گهڻائي;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_بفر = Switch_Draw_بفر;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            Clear_اونهائي = Switch_Clear_اونهائي;
            Stencil_نقاب = Switch_Stencil_نقاب;
            Color_نقاب = Switch_Color_نقاب;
            اونهائي_نقاب = Switch_اونهائي_نقاب;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            پڄاڻي = Switch_پڄاڻي;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            اونهائي_Func = Switch_اونهائي_Func;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            Read_بفر = Switch_Read_بفر;
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
            اونهائي_Range = Switch_اونهائي_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_وٿي = Switch_Polygon_وٿي;
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
            سرگرم_Texture = Switch_سرگرم_Texture;
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
            نقطو_Parameterf = Switch_نقطو_Parameterf;
            نقطو_Parameterfv = Switch_نقطو_Parameterfv;
            نقطو_Parameteri = Switch_نقطو_Parameteri;
            نقطو_Parameteriv = Switch_نقطو_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            پڇاڙي_Query = Switch_پڇاڙي_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_بفر = Switch_Bind_بفر;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_بفر = Switch_Is_بفر;
            بفر_معلومات = Switch_بفر_معلومات;
            بفر_Sub_معلومات = Switch_بفر_Sub_معلومات;
            Get_بفر_Sub_معلومات = Switch_Get_بفر_Sub_معلومات;
            Map_بفر = Switch_Map_بفر;
            Unmap_بفر = Switch_Unmap_بفر;
            Get_بفر_Parameteriv = Switch_Get_بفر_Parameteriv;
            Get_بفر_Pointerv = Switch_Get_بفر_Pointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            Stencil_نقاب_Separate = Switch_Stencil_نقاب_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_پروگرام = Switch_Create_پروگرام;
            CreateShader = Switch_CreateShader;
            Delete_پروگرام = Switch_Delete_پروگرام;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            Disable_راس_Attrib_Array = Switch_Disable_راس_Attrib_Array;
            Enable_راس_Attrib_Array = Switch_Enable_راس_Attrib_Array;
            Get_سرگرم_Attrib = Switch_Get_سرگرم_Attrib;
            Get_سرگرم_هڪجهڙو = Switch_Get_سرگرم_هڪجهڙو;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_پروگرام_Info_Log = Switch_Get_پروگرام_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_ذريعو = Switch_Get_Shader_ذريعو;
            Get_هڪجهڙو_Location = Switch_Get_هڪجهڙو_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_راس_Attribdv = Switch_Get_راس_Attribdv;
            Get_راس_Attribfv = Switch_Get_راس_Attribfv;
            Get_راس_Attribiv = Switch_Get_راس_Attribiv;
            Get_راس_Attrib_Pointerv = Switch_Get_راس_Attrib_Pointerv;
            Is_پروگرام = Switch_Is_پروگرام;
            IsShader = Switch_IsShader;
            Link_پروگرام = Switch_Link_پروگرام;
            Shader_ذريعو = Switch_Shader_ذريعو;
            Use_پروگرام = Switch_Use_پروگرام;
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
            هڪجهڙو_Matrix2fv = Switch_هڪجهڙو_Matrix2fv;
            هڪجهڙو_Matrix3fv = Switch_هڪجهڙو_Matrix3fv;
            هڪجهڙو_Matrix4fv = Switch_هڪجهڙو_Matrix4fv;
            Validate_پروگرام = Switch_Validate_پروگرام;
            راس_Attrib_اشارو = Switch_راس_Attrib_اشارو;

            // Extension: 2.1
            هڪجهڙو_Matrix2x3fv = Switch_هڪجهڙو_Matrix2x3fv;
            هڪجهڙو_Matrix3x2fv = Switch_هڪجهڙو_Matrix3x2fv;
            هڪجهڙو_Matrix2x4fv = Switch_هڪجهڙو_Matrix2x4fv;
            هڪجهڙو_Matrix4x2fv = Switch_هڪجهڙو_Matrix4x2fv;
            هڪجهڙو_Matrix3x4fv = Switch_هڪجهڙو_Matrix3x4fv;
            هڪجهڙو_Matrix4x3fv = Switch_هڪجهڙو_Matrix4x3fv;

            // Extension: ARB_vertex_array_object
            Bind_راس_Array = Switch_Bind_راس_Array;
            Delete_راس_Arrays = Switch_Delete_راس_Arrays;
            Gen_راس_Arrays = Switch_Gen_راس_Arrays;
            Is_راس_Array = Switch_Is_راس_Array;

            // Extension: ARB_map_buffer_range
            Map_بفر_Range = Switch_Map_بفر_Range;
            Flush_Mapped_بفر_Range = Switch_Flush_Mapped_بفر_Range;

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
            Check_Framebuffer_حالت = Switch_Check_Framebuffer_حالت;
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
            پڇاڙي_Transform_Feedback = Switch_پڇاڙي_Transform_Feedback;
            Bind_بفر_Range = Switch_Bind_بفر_Range;
            Bind_بفر_بنياد = Switch_Bind_بفر_بنياد;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            پڇاڙي_Conditional_Render = Switch_پڇاڙي_Conditional_Render;
            راس_Attrib_IPointer = Switch_راس_Attrib_IPointer;
            Get_راس_Attrib_Iiv = Switch_Get_راس_Attrib_Iiv;
            Get_راس_Attrib_Iuiv = Switch_Get_راس_Attrib_Iuiv;
            راس_Attrib_I1i = Switch_راس_Attrib_I1i;
            راس_Attrib_I2i = Switch_راس_Attrib_I2i;
            راس_Attrib_I3i = Switch_راس_Attrib_I3i;
            راس_Attrib_I4i = Switch_راس_Attrib_I4i;
            راس_Attrib_I1ui = Switch_راس_Attrib_I1ui;
            راس_Attrib_I2ui = Switch_راس_Attrib_I2ui;
            راس_Attrib_I3ui = Switch_راس_Attrib_I3ui;
            راس_Attrib_I4ui = Switch_راس_Attrib_I4ui;
            راس_Attrib_I1iv = Switch_راس_Attrib_I1iv;
            راس_Attrib_I2iv = Switch_راس_Attrib_I2iv;
            راس_Attrib_I3iv = Switch_راس_Attrib_I3iv;
            راس_Attrib_I4iv = Switch_راس_Attrib_I4iv;
            راس_Attrib_I1uiv = Switch_راس_Attrib_I1uiv;
            راس_Attrib_I2uiv = Switch_راس_Attrib_I2uiv;
            راس_Attrib_I3uiv = Switch_راس_Attrib_I3uiv;
            راس_Attrib_I4uiv = Switch_راس_Attrib_I4uiv;
            راس_Attrib_I4bv = Switch_راس_Attrib_I4bv;
            راس_Attrib_I4sv = Switch_راس_Attrib_I4sv;
            راس_Attrib_I4ubv = Switch_راس_Attrib_I4ubv;
            راس_Attrib_I4usv = Switch_راس_Attrib_I4usv;
            GetUniformuiv = Switch_GetUniformuiv;
            Bind_Frag_معلومات_Location = Switch_Bind_Frag_معلومات_Location;
            Get_Frag_معلومات_Location = Switch_Get_Frag_معلومات_Location;
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
            Get_هڪجهڙو_Indices = Switch_Get_هڪجهڙو_Indices;
            Get_سرگرم_Uniformsiv = Switch_Get_سرگرم_Uniformsiv;
            Get_سرگرم_هڪجهڙو_Name = Switch_Get_سرگرم_هڪجهڙو_Name;
            Get_هڪجهڙو_بلاڪ_اشاريو = Switch_Get_هڪجهڙو_بلاڪ_اشاريو;
            Get_سرگرم_هڪجهڙو_Blockiv = Switch_Get_سرگرم_هڪجهڙو_Blockiv;
            Get_سرگرم_هڪجهڙو_بلاڪ_Name = Switch_Get_سرگرم_هڪجهڙو_بلاڪ_Name;
            هڪجهڙو_بلاڪ_Binding = Switch_هڪجهڙو_بلاڪ_Binding;

            // Extension: ARB_copy_buffer
            Copy_بفر_Sub_معلومات = Switch_Copy_بفر_Sub_معلومات;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_بفر = Switch_Tex_بفر;
            Primitive_Restart_اشاريو = Switch_Primitive_Restart_اشاريو;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            راس_اشارو = Switch_راس_اشارو;
            عام_اشارو = Switch_عام_اشارو;
            Color_اشارو = Switch_Color_اشارو;
            Tex_Coord_اشارو = Switch_Tex_Coord_اشارو;
            TexEnvi = Switch_TexEnvi;
            ميٽرڪس_گهڻائي = Switch_ميٽرڪس_گهڻائي;
            ڀريو_سڃاڻپ_عنصر = Switch_ڀريو_سڃاڻپ_عنصر;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
