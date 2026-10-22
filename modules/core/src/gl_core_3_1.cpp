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

#اگر_تعریف_شده HAVE_OPENGL

    #اگر_تعریف_شده __APPLE__
        #ضمن <dlfcn.h>

        ساكن فراغ* Apple_GLGet_Proc_آدرس (ثابت حرف* name)
        {
            ساكن منطقي initialized = خطأ;
            ساكن فراغ * handle = عدم;
            إذا (!handle)
            {
                إذا (!initialized)
                {
                    initialized = درست;
                    ثابت حرف * ثابت مسیر = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(مسیر, RTLD_LAZY | RTLD_GLOBAL);
                }
                إذا (!handle)
                    أرجع عدم;
            }
            أرجع dlsym(handle, name);
        }
    #پایان_اگر // __APPLE__

    #إذا defined(__sgi) || defined (__sun)
        #ضمن <dlfcn.h>
        #ضمن <stdio.h>

        ساكن فراغ* Sun_Get_Proc_آدرس (ثابت حرف* name)
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
    #پایان_اگر // __sgi || __sun

    #إذا defined(_WIN32)
        #اگر_تعریف_شده _MSC_VER
            #رهنمود warning(disable: 4055)
            #رهنمود warning(disable: 4054)
        #پایان_اگر

        ساكن صحيح Test_اشاره_گر(ثابت PROC pTest)
        {
            إذا(!pTest)
                أرجع 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            إذا (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                أرجع 0;

            أرجع 1;
        }

        ساكن PROC Win_Get_Proc_آدرس(ثابت حرف* name)
        {
            PROC pFunc = wgl_Get_Proc_آدرس((LPCSTR) name);
            إذا (Test_اشاره_گر(pFunc))
                أرجع pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            أرجع (PROC) Get_Proc_آدرس(glMod, (LPCSTR) name);
        }
    #پایان_اگر // _WIN32

    #إذا defined(_WIN32)
        #تعریف_کن CV_GL_GET_PROC_آدرس(name) Win_Get_Proc_آدرس(name)
    #وإلاإذا defined(__APPLE__)
        #تعریف_کن CV_GL_GET_PROC_آدرس(name) Apple_GLGet_Proc_آدرس(name)
    #وإلاإذا defined(__sgi) || defined(__sun)
        #تعریف_کن CV_GL_GET_PROC_آدرس(name) Sun_Get_Proc_آدرس(name)
    #وإلا // GLX
        #ضمن <GL/glx.h>

        #تعریف_کن CV_GL_GET_PROC_آدرس(name) gl_XGet_Proc_آدرس_ARB((ثابت GLubyte*) name)
    #پایان_اگر

    ساكن فراغ* Int_Get_Proc_آدرس(ثابت حرف* name)
    {
        فراغ* func =  (فراغ*) CV_GL_GET_PROC_آدرس(name);
        إذا (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        أرجع func;
    }
#وإلا
#إذا defined(_MSC_VER)
    #رهنمود warning(disable : 4702)  // unreachable code
#پایان_اگر
    ساكن فراغ* Int_Get_Proc_آدرس(ثابت حرف*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#پایان_اگر

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
    PFNLINEWIDTHPROC سطر_عرض;
    PFNPOINTSIZEPROC نقطه_اندازه;
    PFNPOLYGONMODEPROC Polygon_نما;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_میانگیر;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC Clear_عمق;
    PFNSTENCILMASKPROC Stencil_ماسک;
    PFNCOLORMASKPROC Color_ماسک;
    PFNDEPTHMASKPROC عمق_ماسک;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC پایان;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC عمق_Func;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC Read_میانگیر;
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
    PFNDEPTHRANGEPROC عمق_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_جابه_جایی;
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
    PFNACTIVETEXTUREPROC فعال_Texture;
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
    PFNPOINTPARAMETERFPROC نقطه_Parameterf;
    PFNPOINTPARAMETERFVPROC نقطه_Parameterfv;
    PFNPOINTPARAMETERIPROC نقطه_Parameteri;
    PFNPOINTPARAMETERIVPROC نقطه_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC انتها_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_میانگیر;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_میانگیر;
    PFNBUFFERDATAPROC میانگیر_داده;
    PFNBUFFERSUBDATAPROC میانگیر_Sub_داده;
    PFNGETBUFFERSUBDATAPROC Get_میانگیر_Sub_داده;
    PFNMAPBUFFERPROC Map_میانگیر;
    PFNUNMAPBUFFERPROC Unmap_میانگیر;
    PFNGETBUFFERPARAMETERIVPROC Get_میانگیر_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_میانگیر_Pointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC Stencil_ماسک_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_برنامه;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_برنامه;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC Disable_رأس_Attrib_Array;
    PFNENABLEVERTEXATTRIBARRAYPROC Enable_رأس_Attrib_Array;
    PFNGETACTIVEATTRIBPROC Get_فعال_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_فعال_یکنواخت;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_برنامه_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_منبع;
    PFNGETUNIFORMLOCATIONPROC Get_یکنواخت_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_رأس_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_رأس_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_رأس_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_رأس_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_برنامه;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_برنامه;
    PFNSHADERSOURCEPROC Shader_منبع;
    PFNUSEPROGRAMPROC Use_برنامه;
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
    PFNUNIFORMMATRIX2FVPROC یکنواخت_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC یکنواخت_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC یکنواخت_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_برنامه;
    PFNVERTEXATTRIBPOINTERPROC رأس_Attrib_اشاره_گر;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC یکنواخت_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC یکنواخت_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC یکنواخت_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC یکنواخت_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC یکنواخت_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC یکنواخت_Matrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC Bind_رأس_Array;
    PFNDELETEVERTEXARRAYSPROC Delete_رأس_Arrays;
    PFNGENVERTEXARRAYSPROC Gen_رأس_Arrays;
    PFNISVERTEXARRAYPROC Is_رأس_Array;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC Map_میانگیر_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_میانگیر_Range;

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
    PFNCHECKFRAMEBUFFERSTATUSPROC Check_Framebuffer_وضعیت;
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
    PFNENDTRANSFORMFEEDBACKPROC انتها_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_میانگیر_Range;
    PFNBINDBUFFERBASEPROC Bind_میانگیر_پایه;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC انتها_Conditional_Render;
    PFNVERTEXATTRIBIPOINTERPROC رأس_Attrib_IPointer;
    PFNGETVERTEXATTRIBIIVPROC Get_رأس_Attrib_Iiv;
    PFNGETVERTEXATTRIBIUIVPROC Get_رأس_Attrib_Iuiv;
    PFNVERTEXATTRIBI1IPROC رأس_Attrib_I1i;
    PFNVERTEXATTRIBI2IPROC رأس_Attrib_I2i;
    PFNVERTEXATTRIBI3IPROC رأس_Attrib_I3i;
    PFNVERTEXATTRIBI4IPROC رأس_Attrib_I4i;
    PFNVERTEXATTRIBI1UIPROC رأس_Attrib_I1ui;
    PFNVERTEXATTRIBI2UIPROC رأس_Attrib_I2ui;
    PFNVERTEXATTRIBI3UIPROC رأس_Attrib_I3ui;
    PFNVERTEXATTRIBI4UIPROC رأس_Attrib_I4ui;
    PFNVERTEXATTRIBI1IVPROC رأس_Attrib_I1iv;
    PFNVERTEXATTRIBI2IVPROC رأس_Attrib_I2iv;
    PFNVERTEXATTRIBI3IVPROC رأس_Attrib_I3iv;
    PFNVERTEXATTRIBI4IVPROC رأس_Attrib_I4iv;
    PFNVERTEXATTRIBI1UIVPROC رأس_Attrib_I1uiv;
    PFNVERTEXATTRIBI2UIVPROC رأس_Attrib_I2uiv;
    PFNVERTEXATTRIBI3UIVPROC رأس_Attrib_I3uiv;
    PFNVERTEXATTRIBI4UIVPROC رأس_Attrib_I4uiv;
    PFNVERTEXATTRIBI4BVPROC رأس_Attrib_I4bv;
    PFNVERTEXATTRIBI4SVPROC رأس_Attrib_I4sv;
    PFNVERTEXATTRIBI4UBVPROC رأس_Attrib_I4ubv;
    PFNVERTEXATTRIBI4USVPROC رأس_Attrib_I4usv;
    PFNGETUNIFORMUIVPROC GetUniformuiv;
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_داده_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_داده_Location;
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
    PFNGETUNIFORMINDICESPROC Get_یکنواخت_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_فعال_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_فعال_یکنواخت_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_یکنواخت_بلوک_شاخص;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_فعال_یکنواخت_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_فعال_یکنواخت_بلوک_Name;
    PFNUNIFORMBLOCKBINDINGPROC یکنواخت_بلوک_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_میانگیر_Sub_داده;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_میانگیر;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_شاخص;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC رأس_اشاره_گر;
    PFNNORMALPOINTERPROC نرمال_اشاره_گر;
    PFNCOLORPOINTERPROC Color_اشاره_گر;
    PFNTEXCOORDPOINTERPROC Tex_Coord_اشاره_گر;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC ماتریس_نما;
    PFNLOADIDENTITYPROC بارگیری_عنصر_خنثی;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_CullFace(GLenum نما)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_آدرس("glCullFace");
        CullFace(نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FrontFace(GLenum نما)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_آدرس("glFrontFace");
        FrontFace(نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum نما)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_آدرس("glHint");
        Hint(target, نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_سطر_عرض(GLfloat عرض)
    {
        سطر_عرض = (PFNLINEWIDTHPROC)Int_Get_Proc_آدرس("glLineWidth");
        سطر_عرض(عرض);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطه_اندازه(GLfloat size)
    {
        نقطه_اندازه = (PFNPOINTSIZEPROC)Int_Get_Proc_آدرس("glPointSize");
        نقطه_اندازه(size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_نما(GLenum face, GLenum نما)
    {
        Polygon_نما = (PFNPOLYGONMODEPROC)Int_Get_Proc_آدرس("glPolygonMode");
        Polygon_نما(face, نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei عرض, GLsizei ارتفاع)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_آدرس("glScissor");
        Scissor(x, y, عرض, ارتفاع);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_آدرس("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ثابت GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_آدرس("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_آدرس("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_آدرس("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei عرض, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_آدرس("glTexImage1D");
        TexImage1D(target, level, internalformat, عرض, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei عرض, GLsizei ارتفاع, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_آدرس("glTexImage2D");
        TexImage2D(target, level, internalformat, عرض, ارتفاع, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Draw_میانگیر(GLenum نما)
    {
        Draw_میانگیر = (PFNDRAWBUFFERPROC)Int_Get_Proc_آدرس("glDrawBuffer");
        Draw_میانگیر(نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear(GLbitfield ماسک)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_آدرس("glClear");
        Clear(ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_آدرس("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_آدرس("glClearStencil");
        ClearStencil(s);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear_عمق(GLdouble عمق)
    {
        Clear_عمق = (PFNCLEARDEPTHPROC)Int_Get_Proc_آدرس("glClearDepth");
        Clear_عمق(عمق);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_ماسک(GLuint ماسک)
    {
        Stencil_ماسک = (PFNSTENCILMASKPROC)Int_Get_Proc_آدرس("glStencilMask");
        Stencil_ماسک(ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_ماسک(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_ماسک = (PFNCOLORMASKPROC)Int_Get_Proc_آدرس("glColorMask");
        Color_ماسک(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_عمق_ماسک(GLboolean flag)
    {
        عمق_ماسک = (PFNDEPTHMASKPROC)Int_Get_Proc_آدرس("glDepthMask");
        عمق_ماسک(flag);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_آدرس("glDisable");
        Disable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_آدرس("glEnable");
        Enable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پایان()
    {
        پایان = (PFNFINISHPROC)Int_Get_Proc_آدرس("glFinish");
        پایان();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_آدرس("glFlush");
        Flush();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_آدرس("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_آدرس("glLogicOp");
        LogicOp(opcode);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint ماسک)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_آدرس("glStencilFunc");
        StencilFunc(func, ref, ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_آدرس("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_عمق_Func(GLenum func)
    {
        عمق_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_آدرس("glDepthFunc");
        عمق_Func(func);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_آدرس("glPixelStoref");
        PixelStoref(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_آدرس("glPixelStorei");
        PixelStorei(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Read_میانگیر(GLenum نما)
    {
        Read_میانگیر = (PFNREADBUFFERPROC)Int_Get_Proc_آدرس("glReadBuffer");
        Read_میانگیر(نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei عرض, GLsizei ارتفاع, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_آدرس("glReadPixels");
        ReadPixels(x, y, عرض, ارتفاع, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_آدرس("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_آدرس("glGetDoublev");
        GetDoublev(pname, params);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_آدرس("glGetError");
        أرجع GetError();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_آدرس("glGetFloatv");
        GetFloatv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_آدرس("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_آدرس("glGetString");
        أرجع GetString(name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_آدرس("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_آدرس("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_آدرس("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_آدرس("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_آدرس("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_آدرس("glIsEnabled");
        أرجع IsEnabled(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_عمق_Range(GLdouble ren_near, GLdouble ren_far)
    {
        عمق_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_آدرس("glDepthRange");
        عمق_Range(ren_near, ren_far);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei عرض, GLsizei ارتفاع)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_آدرس("glViewport");
        Viewport(x, y, عرض, ارتفاع);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArrays(GLenum نما, GLint first, GLsizei شمار_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_آدرس("glDrawArrays");
        DrawArrays(نما, first, شمار_2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElements(GLenum نما, GLsizei شمار_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_آدرس("glDrawElements");
        DrawElements(نما, شمار_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_آدرس("glGetPointerv");
        GetPointerv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_جابه_جایی(GLfloat ضریب, GLfloat units)
    {
        Polygon_جابه_جایی = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_آدرس("glPolygonOffset");
        Polygon_جابه_جایی(ضریب, units);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei عرض, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_آدرس("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, عرض, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei عرض, GLsizei ارتفاع, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_آدرس("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, عرض, ارتفاع, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei عرض)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_آدرس("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, عرض);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei عرض, GLsizei ارتفاع)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_آدرس("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, عرض, ارتفاع);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei عرض, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_آدرس("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, عرض, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei عرض, GLsizei ارتفاع, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_آدرس("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, عرض, ارتفاع, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_آدرس("glBindTexture");
        BindTexture(target, texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ثابت GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_آدرس("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_آدرس("glGenTextures");
        GenTextures(n, textures);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_آدرس("glIsTexture");
        أرجع IsTexture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_آدرس("glIndexub");
        Indexub(c);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexubv(ثابت GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_آدرس("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_آدرس("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquation(GLenum نما)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_آدرس("glBlendEquation");
        BlendEquation(نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum نما, GLuint شروع, GLuint end, GLsizei شمار_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_آدرس("glDrawRangeElements");
        DrawRangeElements(نما, شروع, end, شمار_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei عرض, GLsizei ارتفاع, GLsizei عمق, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_آدرس("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, عرض, ارتفاع, عمق, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei عرض, GLsizei ارتفاع)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_آدرس("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, عرض, ارتفاع);
    }

    // Extension: 1.3

    ساكن فراغ CODEGEN_FUNCPTR Switch_فعال_Texture(GLenum texture)
    {
        فعال_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_آدرس("glActiveTexture");
        فعال_Texture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_آدرس("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei عرض, GLsizei ارتفاع, GLsizei عمق, GLint border, GLsizei image_اندازه, ثابت GLvoid *داده)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_آدرس("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, عرض, ارتفاع, عمق, border, image_اندازه, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei عرض, GLsizei ارتفاع, GLint border, GLsizei image_اندازه, ثابت GLvoid *داده)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_آدرس("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, عرض, ارتفاع, border, image_اندازه, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei عرض, GLint border, GLsizei image_اندازه, ثابت GLvoid *داده)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_آدرس("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, عرض, border, image_اندازه, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei عرض, GLsizei ارتفاع, GLsizei عمق, GLenum format, GLsizei image_اندازه, ثابت GLvoid *داده)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_آدرس("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, عرض, ارتفاع, عمق, format, image_اندازه, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei عرض, GLsizei ارتفاع, GLenum format, GLsizei image_اندازه, ثابت GLvoid *داده)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_آدرس("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, عرض, ارتفاع, format, image_اندازه, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei عرض, GLenum format, GLsizei image_اندازه, ثابت GLvoid *داده)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_آدرس("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, عرض, format, image_اندازه, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_آدرس("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_آدرس("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum نما, ثابت GLint *first, ثابت GLsizei *شمار_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_آدرس("glMultiDrawArrays");
        MultiDrawArrays(نما, first, شمار_2, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum نما, ثابت GLsizei *شمار_2, GLenum type, ثابت GLvoid* ثابت *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_آدرس("glMultiDrawElements");
        MultiDrawElements(نما, شمار_2, type, indices, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطه_Parameterf(GLenum pname, GLfloat param)
    {
        نقطه_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_آدرس("glPointParameterf");
        نقطه_Parameterf(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطه_Parameterfv(GLenum pname, ثابت GLfloat *params)
    {
        نقطه_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_آدرس("glPointParameterfv");
        نقطه_Parameterfv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطه_Parameteri(GLenum pname, GLint param)
    {
        نقطه_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_آدرس("glPointParameteri");
        نقطه_Parameteri(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطه_Parameteriv(GLenum pname, ثابت GLint *params)
    {
        نقطه_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_آدرس("glPointParameteriv");
        نقطه_Parameteriv(pname, params);
    }

    // Extension: 1.5

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_آدرس("glGenQueries");
        GenQueries(n, ids);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ثابت GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_آدرس("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_آدرس("glIsQuery");
        أرجع IsQuery(id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_آدرس("glBeginQuery");
        BeginQuery(target, id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_انتها_Query(GLenum target)
    {
        انتها_Query = (PFNENDQUERYPROC)Int_Get_Proc_آدرس("glEndQuery");
        انتها_Query(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_آدرس("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_آدرس("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_آدرس("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_میانگیر(GLenum target, GLuint میانگیر)
    {
        Bind_میانگیر = (PFNBINDBUFFERPROC)Int_Get_Proc_آدرس("glBindBuffer");
        Bind_میانگیر(target, میانگیر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ثابت GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_آدرس("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_آدرس("glGenBuffers");
        GenBuffers(n, buffers);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_میانگیر(GLuint میانگیر)
    {
        Is_میانگیر = (PFNISBUFFERPROC)Int_Get_Proc_آدرس("glIsBuffer");
        أرجع Is_میانگیر(میانگیر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_میانگیر_داده(GLenum target, GLsizeiptr size, ثابت GLvoid *داده, GLenum usage)
    {
        میانگیر_داده = (PFNBUFFERDATAPROC)Int_Get_Proc_آدرس("glBufferData");
        میانگیر_داده(target, size, داده, usage);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_میانگیر_Sub_داده(GLenum target, GLintptr offset, GLsizeiptr size, ثابت GLvoid *داده)
    {
        میانگیر_Sub_داده = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_آدرس("glBufferSubData");
        میانگیر_Sub_داده(target, offset, size, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_میانگیر_Sub_داده(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *داده)
    {
        Get_میانگیر_Sub_داده = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_آدرس("glGetBufferSubData");
        Get_میانگیر_Sub_داده(target, offset, size, داده);
    }

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_میانگیر(GLenum target, GLenum access)
    {
        Map_میانگیر = (PFNMAPBUFFERPROC)Int_Get_Proc_آدرس("glMapBuffer");
        أرجع Map_میانگیر(target, access);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Unmap_میانگیر(GLenum target)
    {
        Unmap_میانگیر = (PFNUNMAPBUFFERPROC)Int_Get_Proc_آدرس("glUnmapBuffer");
        أرجع Unmap_میانگیر(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_میانگیر_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_میانگیر_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_آدرس("glGetBufferParameteriv");
        Get_میانگیر_Parameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_میانگیر_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_میانگیر_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_آدرس("glGetBufferPointerv");
        Get_میانگیر_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum نما_RGB, GLenum نما_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_آدرس("glBlendEquationSeparate");
        BlendEquationSeparate(نما_RGB, نما_Alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ثابت GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_آدرس("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_آدرس("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint ماسک)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_آدرس("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_ماسک_Separate(GLenum face, GLuint ماسک)
    {
        Stencil_ماسک_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_آدرس("glStencilMaskSeparate");
        Stencil_ماسک_Separate(face, ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_AttachShader(GLuint برنامه, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_آدرس("glAttachShader");
        AttachShader(برنامه, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint برنامه, GLuint شاخص, ثابت GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_آدرس("glBindAttribLocation");
        BindAttribLocation(برنامه, شاخص, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_آدرس("glCompileShader");
        CompileShader(shader);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Create_برنامه()
    {
        Create_برنامه = (PFNCREATEPROGRAMPROC)Int_Get_Proc_آدرس("glCreateProgram");
        أرجع Create_برنامه();
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_آدرس("glCreateShader");
        أرجع CreateShader(type);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_برنامه(GLuint برنامه)
    {
        Delete_برنامه = (PFNDELETEPROGRAMPROC)Int_Get_Proc_آدرس("glDeleteProgram");
        Delete_برنامه(برنامه);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_آدرس("glDeleteShader");
        DeleteShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DetachShader(GLuint برنامه, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_آدرس("glDetachShader");
        DetachShader(برنامه, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable_رأس_Attrib_Array(GLuint شاخص)
    {
        Disable_رأس_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_آدرس("glDisableVertexAttribArray");
        Disable_رأس_Attrib_Array(شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable_رأس_Attrib_Array(GLuint شاخص)
    {
        Enable_رأس_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_آدرس("glEnableVertexAttribArray");
        Enable_رأس_Attrib_Array(شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_Attrib(GLuint برنامه, GLuint شاخص, GLsizei buf_اندازه, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_فعال_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_آدرس("glGetActiveAttrib");
        Get_فعال_Attrib(برنامه, شاخص, buf_اندازه, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یکنواخت(GLuint برنامه, GLuint شاخص, GLsizei buf_اندازه, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_فعال_یکنواخت = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_آدرس("glGetActiveUniform");
        Get_فعال_یکنواخت(برنامه, شاخص, buf_اندازه, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint برنامه, GLsizei max_شمار, GLsizei *شمار_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_آدرس("glGetAttachedShaders");
        GetAttachedShaders(برنامه, max_شمار, شمار_2, obj);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint برنامه, ثابت GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_آدرس("glGetAttribLocation");
        أرجع GetAttribLocation(برنامه, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint برنامه, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_آدرس("glGetProgramiv");
        GetProgramiv(برنامه, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_برنامه_Info_Log(GLuint برنامه, GLsizei buf_اندازه, GLsizei *length, GLchar *infoLog)
    {
        Get_برنامه_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_آدرس("glGetProgramInfoLog");
        Get_برنامه_Info_Log(برنامه, buf_اندازه, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_آدرس("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_اندازه, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_آدرس("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_اندازه, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_Shader_منبع(GLuint shader, GLsizei buf_اندازه, GLsizei *length, GLchar *منبع)
    {
        Get_Shader_منبع = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_آدرس("glGetShaderSource");
        Get_Shader_منبع(shader, buf_اندازه, length, منبع);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_یکنواخت_Location(GLuint برنامه, ثابت GLchar *name)
    {
        Get_یکنواخت_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_آدرس("glGetUniformLocation");
        أرجع Get_یکنواخت_Location(برنامه, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint برنامه, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_آدرس("glGetUniformfv");
        GetUniformfv(برنامه, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint برنامه, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_آدرس("glGetUniformiv");
        GetUniformiv(برنامه, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_رأس_Attribdv(GLuint شاخص, GLenum pname, GLdouble *params)
    {
        Get_رأس_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_آدرس("glGetVertexAttribdv");
        Get_رأس_Attribdv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_رأس_Attribfv(GLuint شاخص, GLenum pname, GLfloat *params)
    {
        Get_رأس_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_آدرس("glGetVertexAttribfv");
        Get_رأس_Attribfv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_رأس_Attribiv(GLuint شاخص, GLenum pname, GLint *params)
    {
        Get_رأس_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_آدرس("glGetVertexAttribiv");
        Get_رأس_Attribiv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_رأس_Attrib_Pointerv(GLuint شاخص, GLenum pname, GLvoid* *pointer)
    {
        Get_رأس_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_آدرس("glGetVertexAttribPointerv");
        Get_رأس_Attrib_Pointerv(شاخص, pname, pointer);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_برنامه(GLuint برنامه)
    {
        Is_برنامه = (PFNISPROGRAMPROC)Int_Get_Proc_آدرس("glIsProgram");
        أرجع Is_برنامه(برنامه);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_آدرس("glIsShader");
        أرجع IsShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Link_برنامه(GLuint برنامه)
    {
        Link_برنامه = (PFNLINKPROGRAMPROC)Int_Get_Proc_آدرس("glLinkProgram");
        Link_برنامه(برنامه);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Shader_منبع(GLuint shader, GLsizei شمار_2, ثابت GLchar* ثابت *string, ثابت GLint *length)
    {
        Shader_منبع = (PFNSHADERSOURCEPROC)Int_Get_Proc_آدرس("glShaderSource");
        Shader_منبع(shader, شمار_2, string, length);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Use_برنامه(GLuint برنامه)
    {
        Use_برنامه = (PFNUSEPROGRAMPROC)Int_Get_Proc_آدرس("glUseProgram");
        Use_برنامه(برنامه);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_آدرس("glUniform1f");
        Uniform1f(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_آدرس("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_آدرس("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_آدرس("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_آدرس("glUniform1i");
        Uniform1i(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_آدرس("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_آدرس("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_آدرس("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei شمار_2, ثابت GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_آدرس("glUniform1fv");
        Uniform1fv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei شمار_2, ثابت GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_آدرس("glUniform2fv");
        Uniform2fv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei شمار_2, ثابت GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_آدرس("glUniform3fv");
        Uniform3fv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei شمار_2, ثابت GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_آدرس("glUniform4fv");
        Uniform4fv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei شمار_2, ثابت GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_آدرس("glUniform1iv");
        Uniform1iv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei شمار_2, ثابت GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_آدرس("glUniform2iv");
        Uniform2iv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei شمار_2, ثابت GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_آدرس("glUniform3iv");
        Uniform3iv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei شمار_2, ثابت GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_آدرس("glUniform4iv");
        Uniform4iv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix2fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_آدرس("glUniformMatrix2fv");
        یکنواخت_Matrix2fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix3fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_آدرس("glUniformMatrix3fv");
        یکنواخت_Matrix3fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix4fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_آدرس("glUniformMatrix4fv");
        یکنواخت_Matrix4fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Validate_برنامه(GLuint برنامه)
    {
        Validate_برنامه = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_آدرس("glValidateProgram");
        Validate_برنامه(برنامه);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_اشاره_گر(GLuint شاخص, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ثابت GLvoid *pointer)
    {
        رأس_Attrib_اشاره_گر = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_آدرس("glVertexAttribPointer");
        رأس_Attrib_اشاره_گر(شاخص, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix2x3fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_آدرس("glUniformMatrix2x3fv");
        یکنواخت_Matrix2x3fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix3x2fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_آدرس("glUniformMatrix3x2fv");
        یکنواخت_Matrix3x2fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix2x4fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_آدرس("glUniformMatrix2x4fv");
        یکنواخت_Matrix2x4fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix4x2fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_آدرس("glUniformMatrix4x2fv");
        یکنواخت_Matrix4x2fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix3x4fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_آدرس("glUniformMatrix3x4fv");
        یکنواخت_Matrix3x4fv(location, شمار_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_Matrix4x3fv(GLint location, GLsizei شمار_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یکنواخت_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_آدرس("glUniformMatrix4x3fv");
        یکنواخت_Matrix4x3fv(location, شمار_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_رأس_Array(GLuint ren_array)
    {
        Bind_رأس_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_آدرس("glBindVertexArray");
        Bind_رأس_Array(ren_array);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_رأس_Arrays(GLsizei n, ثابت GLuint *arrays)
    {
        Delete_رأس_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_آدرس("glDeleteVertexArrays");
        Delete_رأس_Arrays(n, arrays);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Gen_رأس_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_رأس_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_آدرس("glGenVertexArrays");
        Gen_رأس_Arrays(n, arrays);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_رأس_Array(GLuint ren_array)
    {
        Is_رأس_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_آدرس("glIsVertexArray");
        أرجع Is_رأس_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_میانگیر_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_میانگیر_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_آدرس("glMapBufferRange");
        أرجع Map_میانگیر_Range(target, offset, length, access);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush_Mapped_میانگیر_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_میانگیر_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_آدرس("glFlushMappedBufferRange");
        Flush_Mapped_میانگیر_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_آدرس("glIsRenderbuffer");
        أرجع IsRenderbuffer(renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_آدرس("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ثابت GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_آدرس("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_آدرس("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei عرض, GLsizei ارتفاع)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_آدرس("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, عرض, ارتفاع);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_آدرس("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_آدرس("glIsFramebuffer");
        أرجع IsFramebuffer(framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_آدرس("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ثابت GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_آدرس("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_آدرس("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_وضعیت(GLenum target)
    {
        Check_Framebuffer_وضعیت = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_آدرس("glCheckFramebufferStatus");
        أرجع Check_Framebuffer_وضعیت(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_آدرس("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_آدرس("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_آدرس("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_آدرس("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_آدرس("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_آدرس("glGenerateMipmap");
        GenerateMipmap(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ماسک, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_آدرس("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, ماسک, filter);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei عرض, GLsizei ارتفاع)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_آدرس("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, عرض, ارتفاع);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_آدرس("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_ColorMaski(GLuint شاخص, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_آدرس("glColorMaski");
        ColorMaski(شاخص, r, g, b, a);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint شاخص, GLboolean *داده)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_آدرس("glGetBooleani_v");
        GetBooleani_v(target, شاخص, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint شاخص, GLint *داده)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_آدرس("glGetIntegeri_v");
        GetIntegeri_v(target, شاخص, داده);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint شاخص)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_آدرس("glEnablei");
        Enablei(target, شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint شاخص)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_آدرس("glDisablei");
        Disablei(target, شاخص);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint شاخص)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_آدرس("glIsEnabledi");
        أرجع IsEnabledi(target, شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_نما)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_آدرس("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_انتها_Transform_Feedback()
    {
        انتها_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_آدرس("glEndTransformFeedback");
        انتها_Transform_Feedback();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_میانگیر_Range(GLenum target, GLuint شاخص, GLuint میانگیر, GLintptr offset, GLsizeiptr size)
    {
        Bind_میانگیر_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_آدرس("glBindBufferRange");
        Bind_میانگیر_Range(target, شاخص, میانگیر, offset, size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_میانگیر_پایه(GLenum target, GLuint شاخص, GLuint میانگیر)
    {
        Bind_میانگیر_پایه = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_آدرس("glBindBufferBase");
        Bind_میانگیر_پایه(target, شاخص, میانگیر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint برنامه, GLsizei شمار_2, ثابت GLchar* ثابت *varyings, GLenum میانگیر_نما)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_آدرس("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(برنامه, شمار_2, varyings, میانگیر_نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint برنامه, GLuint شاخص, GLsizei buf_اندازه, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_آدرس("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(برنامه, شاخص, buf_اندازه, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_آدرس("glClampColor");
        ClampColor(target, clamp);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum نما)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_آدرس("glBeginConditionalRender");
        BeginConditionalRender(id, نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_انتها_Conditional_Render()
    {
        انتها_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_آدرس("glEndConditionalRender");
        انتها_Conditional_Render();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_IPointer(GLuint شاخص, GLint size, GLenum type, GLsizei stride, ثابت GLvoid *pointer)
    {
        رأس_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_آدرس("glVertexAttribIPointer");
        رأس_Attrib_IPointer(شاخص, size, type, stride, pointer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_رأس_Attrib_Iiv(GLuint شاخص, GLenum pname, GLint *params)
    {
        Get_رأس_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_آدرس("glGetVertexAttribIiv");
        Get_رأس_Attrib_Iiv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_رأس_Attrib_Iuiv(GLuint شاخص, GLenum pname, GLuint *params)
    {
        Get_رأس_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_آدرس("glGetVertexAttribIuiv");
        Get_رأس_Attrib_Iuiv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I1i(GLuint شاخص, GLint x)
    {
        رأس_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_آدرس("glVertexAttribI1i");
        رأس_Attrib_I1i(شاخص, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I2i(GLuint شاخص, GLint x, GLint y)
    {
        رأس_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_آدرس("glVertexAttribI2i");
        رأس_Attrib_I2i(شاخص, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I3i(GLuint شاخص, GLint x, GLint y, GLint z)
    {
        رأس_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_آدرس("glVertexAttribI3i");
        رأس_Attrib_I3i(شاخص, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4i(GLuint شاخص, GLint x, GLint y, GLint z, GLint w)
    {
        رأس_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_آدرس("glVertexAttribI4i");
        رأس_Attrib_I4i(شاخص, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I1ui(GLuint شاخص, GLuint x)
    {
        رأس_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_آدرس("glVertexAttribI1ui");
        رأس_Attrib_I1ui(شاخص, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I2ui(GLuint شاخص, GLuint x, GLuint y)
    {
        رأس_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_آدرس("glVertexAttribI2ui");
        رأس_Attrib_I2ui(شاخص, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I3ui(GLuint شاخص, GLuint x, GLuint y, GLuint z)
    {
        رأس_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_آدرس("glVertexAttribI3ui");
        رأس_Attrib_I3ui(شاخص, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4ui(GLuint شاخص, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        رأس_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_آدرس("glVertexAttribI4ui");
        رأس_Attrib_I4ui(شاخص, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I1iv(GLuint شاخص, ثابت GLint *v)
    {
        رأس_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_آدرس("glVertexAttribI1iv");
        رأس_Attrib_I1iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I2iv(GLuint شاخص, ثابت GLint *v)
    {
        رأس_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_آدرس("glVertexAttribI2iv");
        رأس_Attrib_I2iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I3iv(GLuint شاخص, ثابت GLint *v)
    {
        رأس_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_آدرس("glVertexAttribI3iv");
        رأس_Attrib_I3iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4iv(GLuint شاخص, ثابت GLint *v)
    {
        رأس_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_آدرس("glVertexAttribI4iv");
        رأس_Attrib_I4iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I1uiv(GLuint شاخص, ثابت GLuint *v)
    {
        رأس_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_آدرس("glVertexAttribI1uiv");
        رأس_Attrib_I1uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I2uiv(GLuint شاخص, ثابت GLuint *v)
    {
        رأس_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_آدرس("glVertexAttribI2uiv");
        رأس_Attrib_I2uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I3uiv(GLuint شاخص, ثابت GLuint *v)
    {
        رأس_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_آدرس("glVertexAttribI3uiv");
        رأس_Attrib_I3uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4uiv(GLuint شاخص, ثابت GLuint *v)
    {
        رأس_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_آدرس("glVertexAttribI4uiv");
        رأس_Attrib_I4uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4bv(GLuint شاخص, ثابت GLbyte *v)
    {
        رأس_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_آدرس("glVertexAttribI4bv");
        رأس_Attrib_I4bv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4sv(GLuint شاخص, ثابت GLshort *v)
    {
        رأس_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_آدرس("glVertexAttribI4sv");
        رأس_Attrib_I4sv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4ubv(GLuint شاخص, ثابت GLubyte *v)
    {
        رأس_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_آدرس("glVertexAttribI4ubv");
        رأس_Attrib_I4ubv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_Attrib_I4usv(GLuint شاخص, ثابت GLushort *v)
    {
        رأس_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_آدرس("glVertexAttribI4usv");
        رأس_Attrib_I4usv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint برنامه, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_آدرس("glGetUniformuiv");
        GetUniformuiv(برنامه, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_Frag_داده_Location(GLuint برنامه, GLuint color, ثابت GLchar *name)
    {
        Bind_Frag_داده_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_آدرس("glBindFragDataLocation");
        Bind_Frag_داده_Location(برنامه, color, name);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_Frag_داده_Location(GLuint برنامه, ثابت GLchar *name)
    {
        Get_Frag_داده_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_آدرس("glGetFragDataLocation");
        أرجع Get_Frag_داده_Location(برنامه, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_آدرس("glUniform1ui");
        Uniform1ui(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_آدرس("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_آدرس("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_آدرس("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei شمار_2, ثابت GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_آدرس("glUniform1uiv");
        Uniform1uiv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei شمار_2, ثابت GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_آدرس("glUniform2uiv");
        Uniform2uiv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei شمار_2, ثابت GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_آدرس("glUniform3uiv");
        Uniform3uiv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei شمار_2, ثابت GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_آدرس("glUniform4uiv");
        Uniform4uiv(location, شمار_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_آدرس("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ثابت GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_آدرس("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_آدرس("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_آدرس("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum میانگیر, GLint drawbuffer, ثابت GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_آدرس("glClearBufferiv");
        ClearBufferiv(میانگیر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum میانگیر, GLint drawbuffer, ثابت GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_آدرس("glClearBufferuiv");
        ClearBufferuiv(میانگیر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum میانگیر, GLint drawbuffer, ثابت GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_آدرس("glClearBufferfv");
        ClearBufferfv(میانگیر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum میانگیر, GLint drawbuffer, GLfloat عمق, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_آدرس("glClearBufferfi");
        ClearBufferfi(میانگیر, drawbuffer, عمق, stencil);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint شاخص)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_آدرس("glGetStringi");
        أرجع GetStringi(name, شاخص);
    }

    // Extension: ARB_uniform_buffer_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_یکنواخت_Indices(GLuint برنامه, GLsizei یکنواخت_شمار, ثابت GLchar* ثابت *یکنواخت_نامها, GLuint *یکنواخت_Indices)
    {
        Get_یکنواخت_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_آدرس("glGetUniformIndices");
        Get_یکنواخت_Indices(برنامه, یکنواخت_شمار, یکنواخت_نامها, یکنواخت_Indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_Uniformsiv(GLuint برنامه, GLsizei یکنواخت_شمار, ثابت GLuint *یکنواخت_Indices, GLenum pname, GLint *params)
    {
        Get_فعال_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_آدرس("glGetActiveUniformsiv");
        Get_فعال_Uniformsiv(برنامه, یکنواخت_شمار, یکنواخت_Indices, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یکنواخت_Name(GLuint برنامه, GLuint یکنواخت_شاخص, GLsizei buf_اندازه, GLsizei *length, GLchar *یکنواخت_Name)
    {
        Get_فعال_یکنواخت_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_آدرس("glGetActiveUniformName");
        Get_فعال_یکنواخت_Name(برنامه, یکنواخت_شاخص, buf_اندازه, length, یکنواخت_Name);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Get_یکنواخت_بلوک_شاخص(GLuint برنامه, ثابت GLchar *یکنواخت_بلوک_Name)
    {
        Get_یکنواخت_بلوک_شاخص = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_آدرس("glGetUniformBlockIndex");
        أرجع Get_یکنواخت_بلوک_شاخص(برنامه, یکنواخت_بلوک_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یکنواخت_Blockiv(GLuint برنامه, GLuint یکنواخت_بلوک_شاخص, GLenum pname, GLint *params)
    {
        Get_فعال_یکنواخت_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_آدرس("glGetActiveUniformBlockiv");
        Get_فعال_یکنواخت_Blockiv(برنامه, یکنواخت_بلوک_شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یکنواخت_بلوک_Name(GLuint برنامه, GLuint یکنواخت_بلوک_شاخص, GLsizei buf_اندازه, GLsizei *length, GLchar *یکنواخت_بلوک_Name)
    {
        Get_فعال_یکنواخت_بلوک_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_آدرس("glGetActiveUniformBlockName");
        Get_فعال_یکنواخت_بلوک_Name(برنامه, یکنواخت_بلوک_شاخص, buf_اندازه, length, یکنواخت_بلوک_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یکنواخت_بلوک_Binding(GLuint برنامه, GLuint یکنواخت_بلوک_شاخص, GLuint یکنواخت_بلوک_Binding_2)
    {
        یکنواخت_بلوک_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_آدرس("glUniformBlockBinding");
        یکنواخت_بلوک_Binding(برنامه, یکنواخت_بلوک_شاخص, یکنواخت_بلوک_Binding_2);
    }

    // Extension: ARB_copy_buffer

    ساكن فراغ CODEGEN_FUNCPTR Switch_Copy_میانگیر_Sub_داده(GLenum read_هدف, GLenum write_هدف, GLintptr read_جابه_جایی, GLintptr write_جابه_جایی, GLsizeiptr size)
    {
        Copy_میانگیر_Sub_داده = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_آدرس("glCopyBufferSubData");
        Copy_میانگیر_Sub_داده(read_هدف, write_هدف, read_جابه_جایی, write_جابه_جایی, size);
    }

    // Extension: 3.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum نما, GLint first, GLsizei شمار_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_آدرس("glDrawArraysInstanced");
        DrawArraysInstanced(نما, first, شمار_2, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum نما, GLsizei شمار_2, GLenum type, ثابت GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_آدرس("glDrawElementsInstanced");
        DrawElementsInstanced(نما, شمار_2, type, indices, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_میانگیر(GLenum target, GLenum internalformat, GLuint میانگیر)
    {
        Tex_میانگیر = (PFNTEXBUFFERPROC)Int_Get_Proc_آدرس("glTexBuffer");
        Tex_میانگیر(target, internalformat, میانگیر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Primitive_Restart_شاخص(GLuint شاخص)
    {
        Primitive_Restart_شاخص = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_آدرس("glPrimitiveRestartIndex");
        Primitive_Restart_شاخص(شاخص);
    }

    // Legacy

    ساكن فراغ CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_آدرس("glEnableClientState");
        EnableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_آدرس("glDisableClientState");
        DisableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_رأس_اشاره_گر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        رأس_اشاره_گر = (PFNVERTEXPOINTERPROC)Int_Get_Proc_آدرس("glVertexPointer");
        رأس_اشاره_گر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نرمال_اشاره_گر(GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        نرمال_اشاره_گر = (PFNNORMALPOINTERPROC)Int_Get_Proc_آدرس("glNormalPointer");
        نرمال_اشاره_گر(type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_اشاره_گر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Color_اشاره_گر = (PFNCOLORPOINTERPROC)Int_Get_Proc_آدرس("glColorPointer");
        Color_اشاره_گر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_Coord_اشاره_گر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Tex_Coord_اشاره_گر = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_آدرس("glTexCoordPointer");
        Tex_Coord_اشاره_گر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_آدرس("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ماتریس_نما(GLenum نما)
    {
        ماتریس_نما = (PFNMATRIXMODEPROC)Int_Get_Proc_آدرس("glMatrixMode");
        ماتریس_نما(نما);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بارگیری_عنصر_خنثی(فراغ)
    {
        بارگیری_عنصر_خنثی = (PFNLOADIDENTITYPROC)Int_Get_Proc_آدرس("glLoadIdentity");
        بارگیری_عنصر_خنثی();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Ortho(GLdouble چپ, GLdouble راست, GLdouble bottom, GLdouble بالاترین_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_آدرس("glOrtho");
        Ortho(چپ, راست, bottom, بالاترین_2, near_val, far_val);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_آدرس("glColor3d");
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
            سطر_عرض = Switch_سطر_عرض;
            نقطه_اندازه = Switch_نقطه_اندازه;
            Polygon_نما = Switch_Polygon_نما;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_میانگیر = Switch_Draw_میانگیر;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            Clear_عمق = Switch_Clear_عمق;
            Stencil_ماسک = Switch_Stencil_ماسک;
            Color_ماسک = Switch_Color_ماسک;
            عمق_ماسک = Switch_عمق_ماسک;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            پایان = Switch_پایان;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            عمق_Func = Switch_عمق_Func;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            Read_میانگیر = Switch_Read_میانگیر;
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
            عمق_Range = Switch_عمق_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_جابه_جایی = Switch_Polygon_جابه_جایی;
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
            فعال_Texture = Switch_فعال_Texture;
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
            نقطه_Parameterf = Switch_نقطه_Parameterf;
            نقطه_Parameterfv = Switch_نقطه_Parameterfv;
            نقطه_Parameteri = Switch_نقطه_Parameteri;
            نقطه_Parameteriv = Switch_نقطه_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            انتها_Query = Switch_انتها_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_میانگیر = Switch_Bind_میانگیر;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_میانگیر = Switch_Is_میانگیر;
            میانگیر_داده = Switch_میانگیر_داده;
            میانگیر_Sub_داده = Switch_میانگیر_Sub_داده;
            Get_میانگیر_Sub_داده = Switch_Get_میانگیر_Sub_داده;
            Map_میانگیر = Switch_Map_میانگیر;
            Unmap_میانگیر = Switch_Unmap_میانگیر;
            Get_میانگیر_Parameteriv = Switch_Get_میانگیر_Parameteriv;
            Get_میانگیر_Pointerv = Switch_Get_میانگیر_Pointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            Stencil_ماسک_Separate = Switch_Stencil_ماسک_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_برنامه = Switch_Create_برنامه;
            CreateShader = Switch_CreateShader;
            Delete_برنامه = Switch_Delete_برنامه;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            Disable_رأس_Attrib_Array = Switch_Disable_رأس_Attrib_Array;
            Enable_رأس_Attrib_Array = Switch_Enable_رأس_Attrib_Array;
            Get_فعال_Attrib = Switch_Get_فعال_Attrib;
            Get_فعال_یکنواخت = Switch_Get_فعال_یکنواخت;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_برنامه_Info_Log = Switch_Get_برنامه_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_منبع = Switch_Get_Shader_منبع;
            Get_یکنواخت_Location = Switch_Get_یکنواخت_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_رأس_Attribdv = Switch_Get_رأس_Attribdv;
            Get_رأس_Attribfv = Switch_Get_رأس_Attribfv;
            Get_رأس_Attribiv = Switch_Get_رأس_Attribiv;
            Get_رأس_Attrib_Pointerv = Switch_Get_رأس_Attrib_Pointerv;
            Is_برنامه = Switch_Is_برنامه;
            IsShader = Switch_IsShader;
            Link_برنامه = Switch_Link_برنامه;
            Shader_منبع = Switch_Shader_منبع;
            Use_برنامه = Switch_Use_برنامه;
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
            یکنواخت_Matrix2fv = Switch_یکنواخت_Matrix2fv;
            یکنواخت_Matrix3fv = Switch_یکنواخت_Matrix3fv;
            یکنواخت_Matrix4fv = Switch_یکنواخت_Matrix4fv;
            Validate_برنامه = Switch_Validate_برنامه;
            رأس_Attrib_اشاره_گر = Switch_رأس_Attrib_اشاره_گر;

            // Extension: 2.1
            یکنواخت_Matrix2x3fv = Switch_یکنواخت_Matrix2x3fv;
            یکنواخت_Matrix3x2fv = Switch_یکنواخت_Matrix3x2fv;
            یکنواخت_Matrix2x4fv = Switch_یکنواخت_Matrix2x4fv;
            یکنواخت_Matrix4x2fv = Switch_یکنواخت_Matrix4x2fv;
            یکنواخت_Matrix3x4fv = Switch_یکنواخت_Matrix3x4fv;
            یکنواخت_Matrix4x3fv = Switch_یکنواخت_Matrix4x3fv;

            // Extension: ARB_vertex_array_object
            Bind_رأس_Array = Switch_Bind_رأس_Array;
            Delete_رأس_Arrays = Switch_Delete_رأس_Arrays;
            Gen_رأس_Arrays = Switch_Gen_رأس_Arrays;
            Is_رأس_Array = Switch_Is_رأس_Array;

            // Extension: ARB_map_buffer_range
            Map_میانگیر_Range = Switch_Map_میانگیر_Range;
            Flush_Mapped_میانگیر_Range = Switch_Flush_Mapped_میانگیر_Range;

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
            Check_Framebuffer_وضعیت = Switch_Check_Framebuffer_وضعیت;
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
            انتها_Transform_Feedback = Switch_انتها_Transform_Feedback;
            Bind_میانگیر_Range = Switch_Bind_میانگیر_Range;
            Bind_میانگیر_پایه = Switch_Bind_میانگیر_پایه;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            انتها_Conditional_Render = Switch_انتها_Conditional_Render;
            رأس_Attrib_IPointer = Switch_رأس_Attrib_IPointer;
            Get_رأس_Attrib_Iiv = Switch_Get_رأس_Attrib_Iiv;
            Get_رأس_Attrib_Iuiv = Switch_Get_رأس_Attrib_Iuiv;
            رأس_Attrib_I1i = Switch_رأس_Attrib_I1i;
            رأس_Attrib_I2i = Switch_رأس_Attrib_I2i;
            رأس_Attrib_I3i = Switch_رأس_Attrib_I3i;
            رأس_Attrib_I4i = Switch_رأس_Attrib_I4i;
            رأس_Attrib_I1ui = Switch_رأس_Attrib_I1ui;
            رأس_Attrib_I2ui = Switch_رأس_Attrib_I2ui;
            رأس_Attrib_I3ui = Switch_رأس_Attrib_I3ui;
            رأس_Attrib_I4ui = Switch_رأس_Attrib_I4ui;
            رأس_Attrib_I1iv = Switch_رأس_Attrib_I1iv;
            رأس_Attrib_I2iv = Switch_رأس_Attrib_I2iv;
            رأس_Attrib_I3iv = Switch_رأس_Attrib_I3iv;
            رأس_Attrib_I4iv = Switch_رأس_Attrib_I4iv;
            رأس_Attrib_I1uiv = Switch_رأس_Attrib_I1uiv;
            رأس_Attrib_I2uiv = Switch_رأس_Attrib_I2uiv;
            رأس_Attrib_I3uiv = Switch_رأس_Attrib_I3uiv;
            رأس_Attrib_I4uiv = Switch_رأس_Attrib_I4uiv;
            رأس_Attrib_I4bv = Switch_رأس_Attrib_I4bv;
            رأس_Attrib_I4sv = Switch_رأس_Attrib_I4sv;
            رأس_Attrib_I4ubv = Switch_رأس_Attrib_I4ubv;
            رأس_Attrib_I4usv = Switch_رأس_Attrib_I4usv;
            GetUniformuiv = Switch_GetUniformuiv;
            Bind_Frag_داده_Location = Switch_Bind_Frag_داده_Location;
            Get_Frag_داده_Location = Switch_Get_Frag_داده_Location;
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
            Get_یکنواخت_Indices = Switch_Get_یکنواخت_Indices;
            Get_فعال_Uniformsiv = Switch_Get_فعال_Uniformsiv;
            Get_فعال_یکنواخت_Name = Switch_Get_فعال_یکنواخت_Name;
            Get_یکنواخت_بلوک_شاخص = Switch_Get_یکنواخت_بلوک_شاخص;
            Get_فعال_یکنواخت_Blockiv = Switch_Get_فعال_یکنواخت_Blockiv;
            Get_فعال_یکنواخت_بلوک_Name = Switch_Get_فعال_یکنواخت_بلوک_Name;
            یکنواخت_بلوک_Binding = Switch_یکنواخت_بلوک_Binding;

            // Extension: ARB_copy_buffer
            Copy_میانگیر_Sub_داده = Switch_Copy_میانگیر_Sub_داده;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_میانگیر = Switch_Tex_میانگیر;
            Primitive_Restart_شاخص = Switch_Primitive_Restart_شاخص;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            رأس_اشاره_گر = Switch_رأس_اشاره_گر;
            نرمال_اشاره_گر = Switch_نرمال_اشاره_گر;
            Color_اشاره_گر = Switch_Color_اشاره_گر;
            Tex_Coord_اشاره_گر = Switch_Tex_Coord_اشاره_گر;
            TexEnvi = Switch_TexEnvi;
            ماتریس_نما = Switch_ماتریس_نما;
            بارگیری_عنصر_خنثی = Switch_بارگیری_عنصر_خنثی;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
