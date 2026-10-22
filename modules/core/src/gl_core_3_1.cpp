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

#جے_تعریف_شدہ HAVE_OPENGL

    #جے_تعریف_شدہ __APPLE__
        #ضمن <dlfcn.h>

        ساكن فراغ* Apple_GLGet_Proc_پتہ (ثابت حرف* name)
        {
            ساكن منطقي initialized = خطأ;
            ساكن فراغ * handle = عدم;
            إذا (!handle)
            {
                إذا (!initialized)
                {
                    initialized = سچ;
                    ثابت حرف * ثابت رستہ = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(رستہ, RTLD_LAZY | RTLD_GLOBAL);
                }
                إذا (!handle)
                    أرجع عدم;
            }
            أرجع dlsym(handle, name);
        }
    #جے_انت // __APPLE__

    #إذا defined(__sgi) || defined (__sun)
        #ضمن <dlfcn.h>
        #ضمن <stdio.h>

        ساكن فراغ* Sun_Get_Proc_پتہ (ثابت حرف* name)
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
    #جے_انت // __sgi || __sun

    #إذا defined(_WIN32)
        #جے_تعریف_شدہ _MSC_VER
            #ہدایت warning(disable: 4055)
            #ہدایت warning(disable: 4054)
        #جے_انت

        ساكن صحيح Test_اشارہ_گر(ثابت PROC pTest)
        {
            إذا(!pTest)
                أرجع 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            إذا (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                أرجع 0;

            أرجع 1;
        }

        ساكن PROC Win_Get_Proc_پتہ(ثابت حرف* name)
        {
            PROC pFunc = wgl_Get_Proc_پتہ((LPCSTR) name);
            إذا (Test_اشارہ_گر(pFunc))
                أرجع pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            أرجع (PROC) Get_Proc_پتہ(glMod, (LPCSTR) name);
        }
    #جے_انت // _WIN32

    #إذا defined(_WIN32)
        #تعریف_کرو CV_GL_GET_PROC_پتہ(name) Win_Get_Proc_پتہ(name)
    #وإلاإذا defined(__APPLE__)
        #تعریف_کرو CV_GL_GET_PROC_پتہ(name) Apple_GLGet_Proc_پتہ(name)
    #وإلاإذا defined(__sgi) || defined(__sun)
        #تعریف_کرو CV_GL_GET_PROC_پتہ(name) Sun_Get_Proc_پتہ(name)
    #وإلا // GLX
        #ضمن <GL/glx.h>

        #تعریف_کرو CV_GL_GET_PROC_پتہ(name) gl_XGet_Proc_پتہ_ARB((ثابت GLubyte*) name)
    #جے_انت

    ساكن فراغ* Int_Get_Proc_پتہ(ثابت حرف* name)
    {
        فراغ* func =  (فراغ*) CV_GL_GET_PROC_پتہ(name);
        إذا (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        أرجع func;
    }
#وإلا
#إذا defined(_MSC_VER)
    #ہدایت warning(disable : 4702)  // unreachable code
#جے_انت
    ساكن فراغ* Int_Get_Proc_پتہ(ثابت حرف*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#جے_انت

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
    PFNLINEWIDTHPROC سطر_چوڑائی;
    PFNPOINTSIZEPROC نقطہ_ناپ;
    PFNPOLYGONMODEPROC Polygon_منوال;
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
    PFNCLEARDEPTHPROC Clear_ڈونگھائی;
    PFNSTENCILMASKPROC Stencil_نقاب;
    PFNCOLORMASKPROC Color_نقاب;
    PFNDEPTHMASKPROC ڈونگھائی_نقاب;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC انت;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC ڈونگھائی_Func;
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
    PFNDEPTHRANGEPROC ڈونگھائی_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_وتھ;
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
    PFNPOINTPARAMETERFPROC نقطہ_Parameterf;
    PFNPOINTPARAMETERFVPROC نقطہ_Parameterfv;
    PFNPOINTPARAMETERIPROC نقطہ_Parameteri;
    PFNPOINTPARAMETERIVPROC نقطہ_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC سرا_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_بفر;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_بفر;
    PFNBUFFERDATAPROC بفر_انکڑے;
    PFNBUFFERSUBDATAPROC بفر_Sub_انکڑے;
    PFNGETBUFFERSUBDATAPROC Get_بفر_Sub_انکڑے;
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
    PFNGETACTIVEUNIFORMPROC Get_سرگرم_اکسار;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_پروگرام_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_سروت;
    PFNGETUNIFORMLOCATIONPROC Get_اکسار_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_راس_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_راس_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_راس_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_راس_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_پروگرام;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_پروگرام;
    PFNSHADERSOURCEPROC Shader_سروت;
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
    PFNUNIFORMMATRIX2FVPROC اکسار_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC اکسار_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC اکسار_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_پروگرام;
    PFNVERTEXATTRIBPOINTERPROC راس_Attrib_اشارہ_گر;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC اکسار_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC اکسار_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC اکسار_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC اکسار_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC اکسار_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC اکسار_Matrix4x3fv;

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
    PFNENDTRANSFORMFEEDBACKPROC سرا_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_بفر_Range;
    PFNBINDBUFFERBASEPROC Bind_بفر_ادھار;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC سرا_Conditional_Render;
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
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_انکڑے_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_انکڑے_Location;
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
    PFNGETUNIFORMINDICESPROC Get_اکسار_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_سرگرم_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_سرگرم_اکسار_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_اکسار_بلاک_اشاریہ;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_سرگرم_اکسار_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_سرگرم_اکسار_بلاک_Name;
    PFNUNIFORMBLOCKBINDINGPROC اکسار_بلاک_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_بفر_Sub_انکڑے;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_بفر;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_اشاریہ;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC راس_اشارہ_گر;
    PFNNORMALPOINTERPROC عام_اشارہ_گر;
    PFNCOLORPOINTERPROC Color_اشارہ_گر;
    PFNTEXCOORDPOINTERPROC Tex_Coord_اشارہ_گر;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC میٹرکس_منوال;
    PFNLOADIDENTITYPROC لدو_پچھان_تت;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_CullFace(GLenum منوال)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_پتہ("glCullFace");
        CullFace(منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FrontFace(GLenum منوال)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_پتہ("glFrontFace");
        FrontFace(منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum منوال)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_پتہ("glHint");
        Hint(target, منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_سطر_چوڑائی(GLfloat چوڑائی)
    {
        سطر_چوڑائی = (PFNLINEWIDTHPROC)Int_Get_Proc_پتہ("glLineWidth");
        سطر_چوڑائی(چوڑائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطہ_ناپ(GLfloat size)
    {
        نقطہ_ناپ = (PFNPOINTSIZEPROC)Int_Get_Proc_پتہ("glPointSize");
        نقطہ_ناپ(size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_منوال(GLenum face, GLenum منوال)
    {
        Polygon_منوال = (PFNPOLYGONMODEPROC)Int_Get_Proc_پتہ("glPolygonMode");
        Polygon_منوال(face, منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei چوڑائی, GLsizei اچائی)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_پتہ("glScissor");
        Scissor(x, y, چوڑائی, اچائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_پتہ("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ثابت GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_پتہ("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_پتہ("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_پتہ("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei چوڑائی, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_پتہ("glTexImage1D");
        TexImage1D(target, level, internalformat, چوڑائی, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei چوڑائی, GLsizei اچائی, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_پتہ("glTexImage2D");
        TexImage2D(target, level, internalformat, چوڑائی, اچائی, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Draw_بفر(GLenum منوال)
    {
        Draw_بفر = (PFNDRAWBUFFERPROC)Int_Get_Proc_پتہ("glDrawBuffer");
        Draw_بفر(منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear(GLbitfield نقاب)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_پتہ("glClear");
        Clear(نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_پتہ("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_پتہ("glClearStencil");
        ClearStencil(s);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear_ڈونگھائی(GLdouble ڈونگھائی)
    {
        Clear_ڈونگھائی = (PFNCLEARDEPTHPROC)Int_Get_Proc_پتہ("glClearDepth");
        Clear_ڈونگھائی(ڈونگھائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_نقاب(GLuint نقاب)
    {
        Stencil_نقاب = (PFNSTENCILMASKPROC)Int_Get_Proc_پتہ("glStencilMask");
        Stencil_نقاب(نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_نقاب(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_نقاب = (PFNCOLORMASKPROC)Int_Get_Proc_پتہ("glColorMask");
        Color_نقاب(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ڈونگھائی_نقاب(GLboolean flag)
    {
        ڈونگھائی_نقاب = (PFNDEPTHMASKPROC)Int_Get_Proc_پتہ("glDepthMask");
        ڈونگھائی_نقاب(flag);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_پتہ("glDisable");
        Disable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_پتہ("glEnable");
        Enable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_انت()
    {
        انت = (PFNFINISHPROC)Int_Get_Proc_پتہ("glFinish");
        انت();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_پتہ("glFlush");
        Flush();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_پتہ("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_پتہ("glLogicOp");
        LogicOp(opcode);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint نقاب)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_پتہ("glStencilFunc");
        StencilFunc(func, ref, نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_پتہ("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ڈونگھائی_Func(GLenum func)
    {
        ڈونگھائی_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_پتہ("glDepthFunc");
        ڈونگھائی_Func(func);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_پتہ("glPixelStoref");
        PixelStoref(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_پتہ("glPixelStorei");
        PixelStorei(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Read_بفر(GLenum منوال)
    {
        Read_بفر = (PFNREADBUFFERPROC)Int_Get_Proc_پتہ("glReadBuffer");
        Read_بفر(منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_پتہ("glReadPixels");
        ReadPixels(x, y, چوڑائی, اچائی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_پتہ("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_پتہ("glGetDoublev");
        GetDoublev(pname, params);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_پتہ("glGetError");
        أرجع GetError();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_پتہ("glGetFloatv");
        GetFloatv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_پتہ("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_پتہ("glGetString");
        أرجع GetString(name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_پتہ("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_پتہ("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_پتہ("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_پتہ("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_پتہ("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_پتہ("glIsEnabled");
        أرجع IsEnabled(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ڈونگھائی_Range(GLdouble ren_near, GLdouble ren_far)
    {
        ڈونگھائی_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_پتہ("glDepthRange");
        ڈونگھائی_Range(ren_near, ren_far);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei چوڑائی, GLsizei اچائی)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_پتہ("glViewport");
        Viewport(x, y, چوڑائی, اچائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArrays(GLenum منوال, GLint first, GLsizei گنتی_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_پتہ("glDrawArrays");
        DrawArrays(منوال, first, گنتی_2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElements(GLenum منوال, GLsizei گنتی_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_پتہ("glDrawElements");
        DrawElements(منوال, گنتی_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_پتہ("glGetPointerv");
        GetPointerv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_وتھ(GLfloat ضارب, GLfloat units)
    {
        Polygon_وتھ = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_پتہ("glPolygonOffset");
        Polygon_وتھ(ضارب, units);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei چوڑائی, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_پتہ("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, چوڑائی, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei چوڑائی, GLsizei اچائی, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_پتہ("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, چوڑائی, اچائی, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei چوڑائی)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_پتہ("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, چوڑائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei چوڑائی, GLsizei اچائی)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_پتہ("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, چوڑائی, اچائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei چوڑائی, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_پتہ("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, چوڑائی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_پتہ("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, چوڑائی, اچائی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_پتہ("glBindTexture");
        BindTexture(target, texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ثابت GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_پتہ("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_پتہ("glGenTextures");
        GenTextures(n, textures);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_پتہ("glIsTexture");
        أرجع IsTexture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_پتہ("glIndexub");
        Indexub(c);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexubv(ثابت GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_پتہ("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_پتہ("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquation(GLenum منوال)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_پتہ("glBlendEquation");
        BlendEquation(منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum منوال, GLuint شروع, GLuint end, GLsizei گنتی_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_پتہ("glDrawRangeElements");
        DrawRangeElements(منوال, شروع, end, گنتی_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei چوڑائی, GLsizei اچائی, GLsizei ڈونگھائی, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_پتہ("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, چوڑائی, اچائی, ڈونگھائی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei چوڑائی, GLsizei اچائی)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_پتہ("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, چوڑائی, اچائی);
    }

    // Extension: 1.3

    ساكن فراغ CODEGEN_FUNCPTR Switch_سرگرم_Texture(GLenum texture)
    {
        سرگرم_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_پتہ("glActiveTexture");
        سرگرم_Texture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_پتہ("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei چوڑائی, GLsizei اچائی, GLsizei ڈونگھائی, GLint border, GLsizei image_ناپ, ثابت GLvoid *انکڑے)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_پتہ("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, چوڑائی, اچائی, ڈونگھائی, border, image_ناپ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei چوڑائی, GLsizei اچائی, GLint border, GLsizei image_ناپ, ثابت GLvoid *انکڑے)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_پتہ("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, چوڑائی, اچائی, border, image_ناپ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei چوڑائی, GLint border, GLsizei image_ناپ, ثابت GLvoid *انکڑے)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_پتہ("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, چوڑائی, border, image_ناپ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei چوڑائی, GLsizei اچائی, GLsizei ڈونگھائی, GLenum format, GLsizei image_ناپ, ثابت GLvoid *انکڑے)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_پتہ("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, چوڑائی, اچائی, ڈونگھائی, format, image_ناپ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLsizei image_ناپ, ثابت GLvoid *انکڑے)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_پتہ("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, چوڑائی, اچائی, format, image_ناپ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei چوڑائی, GLenum format, GLsizei image_ناپ, ثابت GLvoid *انکڑے)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_پتہ("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, چوڑائی, format, image_ناپ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_پتہ("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_پتہ("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum منوال, ثابت GLint *first, ثابت GLsizei *گنتی_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_پتہ("glMultiDrawArrays");
        MultiDrawArrays(منوال, first, گنتی_2, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum منوال, ثابت GLsizei *گنتی_2, GLenum type, ثابت GLvoid* ثابت *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_پتہ("glMultiDrawElements");
        MultiDrawElements(منوال, گنتی_2, type, indices, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطہ_Parameterf(GLenum pname, GLfloat param)
    {
        نقطہ_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_پتہ("glPointParameterf");
        نقطہ_Parameterf(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطہ_Parameterfv(GLenum pname, ثابت GLfloat *params)
    {
        نقطہ_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_پتہ("glPointParameterfv");
        نقطہ_Parameterfv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطہ_Parameteri(GLenum pname, GLint param)
    {
        نقطہ_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_پتہ("glPointParameteri");
        نقطہ_Parameteri(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نقطہ_Parameteriv(GLenum pname, ثابت GLint *params)
    {
        نقطہ_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_پتہ("glPointParameteriv");
        نقطہ_Parameteriv(pname, params);
    }

    // Extension: 1.5

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_پتہ("glGenQueries");
        GenQueries(n, ids);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ثابت GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_پتہ("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_پتہ("glIsQuery");
        أرجع IsQuery(id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_پتہ("glBeginQuery");
        BeginQuery(target, id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_سرا_Query(GLenum target)
    {
        سرا_Query = (PFNENDQUERYPROC)Int_Get_Proc_پتہ("glEndQuery");
        سرا_Query(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_پتہ("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_پتہ("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_پتہ("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر(GLenum target, GLuint بفر)
    {
        Bind_بفر = (PFNBINDBUFFERPROC)Int_Get_Proc_پتہ("glBindBuffer");
        Bind_بفر(target, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ثابت GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_پتہ("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_پتہ("glGenBuffers");
        GenBuffers(n, buffers);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_بفر(GLuint بفر)
    {
        Is_بفر = (PFNISBUFFERPROC)Int_Get_Proc_پتہ("glIsBuffer");
        أرجع Is_بفر(بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بفر_انکڑے(GLenum target, GLsizeiptr size, ثابت GLvoid *انکڑے, GLenum usage)
    {
        بفر_انکڑے = (PFNBUFFERDATAPROC)Int_Get_Proc_پتہ("glBufferData");
        بفر_انکڑے(target, size, انکڑے, usage);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بفر_Sub_انکڑے(GLenum target, GLintptr offset, GLsizeiptr size, ثابت GLvoid *انکڑے)
    {
        بفر_Sub_انکڑے = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_پتہ("glBufferSubData");
        بفر_Sub_انکڑے(target, offset, size, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Sub_انکڑے(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *انکڑے)
    {
        Get_بفر_Sub_انکڑے = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_پتہ("glGetBufferSubData");
        Get_بفر_Sub_انکڑے(target, offset, size, انکڑے);
    }

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_بفر(GLenum target, GLenum access)
    {
        Map_بفر = (PFNMAPBUFFERPROC)Int_Get_Proc_پتہ("glMapBuffer");
        أرجع Map_بفر(target, access);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Unmap_بفر(GLenum target)
    {
        Unmap_بفر = (PFNUNMAPBUFFERPROC)Int_Get_Proc_پتہ("glUnmapBuffer");
        أرجع Unmap_بفر(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_بفر_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_پتہ("glGetBufferParameteriv");
        Get_بفر_Parameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_بفر_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_پتہ("glGetBufferPointerv");
        Get_بفر_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum منوال_RGB, GLenum منوال_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_پتہ("glBlendEquationSeparate");
        BlendEquationSeparate(منوال_RGB, منوال_Alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ثابت GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_پتہ("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_پتہ("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint نقاب)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_پتہ("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_نقاب_Separate(GLenum face, GLuint نقاب)
    {
        Stencil_نقاب_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_پتہ("glStencilMaskSeparate");
        Stencil_نقاب_Separate(face, نقاب);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_AttachShader(GLuint پروگرام, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_پتہ("glAttachShader");
        AttachShader(پروگرام, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint پروگرام, GLuint اشاریہ, ثابت GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_پتہ("glBindAttribLocation");
        BindAttribLocation(پروگرام, اشاریہ, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_پتہ("glCompileShader");
        CompileShader(shader);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Create_پروگرام()
    {
        Create_پروگرام = (PFNCREATEPROGRAMPROC)Int_Get_Proc_پتہ("glCreateProgram");
        أرجع Create_پروگرام();
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_پتہ("glCreateShader");
        أرجع CreateShader(type);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_پروگرام(GLuint پروگرام)
    {
        Delete_پروگرام = (PFNDELETEPROGRAMPROC)Int_Get_Proc_پتہ("glDeleteProgram");
        Delete_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_پتہ("glDeleteShader");
        DeleteShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DetachShader(GLuint پروگرام, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_پتہ("glDetachShader");
        DetachShader(پروگرام, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable_راس_Attrib_Array(GLuint اشاریہ)
    {
        Disable_راس_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_پتہ("glDisableVertexAttribArray");
        Disable_راس_Attrib_Array(اشاریہ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable_راس_Attrib_Array(GLuint اشاریہ)
    {
        Enable_راس_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_پتہ("glEnableVertexAttribArray");
        Enable_راس_Attrib_Array(اشاریہ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_Attrib(GLuint پروگرام, GLuint اشاریہ, GLsizei buf_ناپ, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_سرگرم_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_پتہ("glGetActiveAttrib");
        Get_سرگرم_Attrib(پروگرام, اشاریہ, buf_ناپ, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_اکسار(GLuint پروگرام, GLuint اشاریہ, GLsizei buf_ناپ, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_سرگرم_اکسار = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_پتہ("glGetActiveUniform");
        Get_سرگرم_اکسار(پروگرام, اشاریہ, buf_ناپ, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint پروگرام, GLsizei max_گنتی, GLsizei *گنتی_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_پتہ("glGetAttachedShaders");
        GetAttachedShaders(پروگرام, max_گنتی, گنتی_2, obj);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint پروگرام, ثابت GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_پتہ("glGetAttribLocation");
        أرجع GetAttribLocation(پروگرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint پروگرام, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_پتہ("glGetProgramiv");
        GetProgramiv(پروگرام, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_پروگرام_Info_Log(GLuint پروگرام, GLsizei buf_ناپ, GLsizei *length, GLchar *infoLog)
    {
        Get_پروگرام_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_پتہ("glGetProgramInfoLog");
        Get_پروگرام_Info_Log(پروگرام, buf_ناپ, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_پتہ("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_ناپ, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_پتہ("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_ناپ, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_Shader_سروت(GLuint shader, GLsizei buf_ناپ, GLsizei *length, GLchar *سروت)
    {
        Get_Shader_سروت = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_پتہ("glGetShaderSource");
        Get_Shader_سروت(shader, buf_ناپ, length, سروت);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_اکسار_Location(GLuint پروگرام, ثابت GLchar *name)
    {
        Get_اکسار_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_پتہ("glGetUniformLocation");
        أرجع Get_اکسار_Location(پروگرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint پروگرام, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_پتہ("glGetUniformfv");
        GetUniformfv(پروگرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint پروگرام, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_پتہ("glGetUniformiv");
        GetUniformiv(پروگرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribdv(GLuint اشاریہ, GLenum pname, GLdouble *params)
    {
        Get_راس_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_پتہ("glGetVertexAttribdv");
        Get_راس_Attribdv(اشاریہ, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribfv(GLuint اشاریہ, GLenum pname, GLfloat *params)
    {
        Get_راس_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_پتہ("glGetVertexAttribfv");
        Get_راس_Attribfv(اشاریہ, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribiv(GLuint اشاریہ, GLenum pname, GLint *params)
    {
        Get_راس_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_پتہ("glGetVertexAttribiv");
        Get_راس_Attribiv(اشاریہ, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Pointerv(GLuint اشاریہ, GLenum pname, GLvoid* *pointer)
    {
        Get_راس_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_پتہ("glGetVertexAttribPointerv");
        Get_راس_Attrib_Pointerv(اشاریہ, pname, pointer);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_پروگرام(GLuint پروگرام)
    {
        Is_پروگرام = (PFNISPROGRAMPROC)Int_Get_Proc_پتہ("glIsProgram");
        أرجع Is_پروگرام(پروگرام);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_پتہ("glIsShader");
        أرجع IsShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Link_پروگرام(GLuint پروگرام)
    {
        Link_پروگرام = (PFNLINKPROGRAMPROC)Int_Get_Proc_پتہ("glLinkProgram");
        Link_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Shader_سروت(GLuint shader, GLsizei گنتی_2, ثابت GLchar* ثابت *string, ثابت GLint *length)
    {
        Shader_سروت = (PFNSHADERSOURCEPROC)Int_Get_Proc_پتہ("glShaderSource");
        Shader_سروت(shader, گنتی_2, string, length);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Use_پروگرام(GLuint پروگرام)
    {
        Use_پروگرام = (PFNUSEPROGRAMPROC)Int_Get_Proc_پتہ("glUseProgram");
        Use_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_پتہ("glUniform1f");
        Uniform1f(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_پتہ("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_پتہ("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_پتہ("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_پتہ("glUniform1i");
        Uniform1i(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_پتہ("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_پتہ("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_پتہ("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei گنتی_2, ثابت GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_پتہ("glUniform1fv");
        Uniform1fv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei گنتی_2, ثابت GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_پتہ("glUniform2fv");
        Uniform2fv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei گنتی_2, ثابت GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_پتہ("glUniform3fv");
        Uniform3fv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei گنتی_2, ثابت GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_پتہ("glUniform4fv");
        Uniform4fv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei گنتی_2, ثابت GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_پتہ("glUniform1iv");
        Uniform1iv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei گنتی_2, ثابت GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_پتہ("glUniform2iv");
        Uniform2iv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei گنتی_2, ثابت GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_پتہ("glUniform3iv");
        Uniform3iv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei گنتی_2, ثابت GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_پتہ("glUniform4iv");
        Uniform4iv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix2fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_پتہ("glUniformMatrix2fv");
        اکسار_Matrix2fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix3fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_پتہ("glUniformMatrix3fv");
        اکسار_Matrix3fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix4fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_پتہ("glUniformMatrix4fv");
        اکسار_Matrix4fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Validate_پروگرام(GLuint پروگرام)
    {
        Validate_پروگرام = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_پتہ("glValidateProgram");
        Validate_پروگرام(پروگرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_اشارہ_گر(GLuint اشاریہ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ثابت GLvoid *pointer)
    {
        راس_Attrib_اشارہ_گر = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_پتہ("glVertexAttribPointer");
        راس_Attrib_اشارہ_گر(اشاریہ, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix2x3fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_پتہ("glUniformMatrix2x3fv");
        اکسار_Matrix2x3fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix3x2fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_پتہ("glUniformMatrix3x2fv");
        اکسار_Matrix3x2fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix2x4fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_پتہ("glUniformMatrix2x4fv");
        اکسار_Matrix2x4fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix4x2fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_پتہ("glUniformMatrix4x2fv");
        اکسار_Matrix4x2fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix3x4fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_پتہ("glUniformMatrix3x4fv");
        اکسار_Matrix3x4fv(location, گنتی_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_Matrix4x3fv(GLint location, GLsizei گنتی_2, GLboolean transpose, ثابت GLfloat *value)
    {
        اکسار_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_پتہ("glUniformMatrix4x3fv");
        اکسار_Matrix4x3fv(location, گنتی_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_راس_Array(GLuint ren_array)
    {
        Bind_راس_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_پتہ("glBindVertexArray");
        Bind_راس_Array(ren_array);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_راس_Arrays(GLsizei n, ثابت GLuint *arrays)
    {
        Delete_راس_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_پتہ("glDeleteVertexArrays");
        Delete_راس_Arrays(n, arrays);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Gen_راس_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_راس_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_پتہ("glGenVertexArrays");
        Gen_راس_Arrays(n, arrays);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_راس_Array(GLuint ren_array)
    {
        Is_راس_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_پتہ("glIsVertexArray");
        أرجع Is_راس_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_بفر_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_بفر_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_پتہ("glMapBufferRange");
        أرجع Map_بفر_Range(target, offset, length, access);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush_Mapped_بفر_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_بفر_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_پتہ("glFlushMappedBufferRange");
        Flush_Mapped_بفر_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_پتہ("glIsRenderbuffer");
        أرجع IsRenderbuffer(renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_پتہ("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ثابت GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_پتہ("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_پتہ("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei چوڑائی, GLsizei اچائی)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_پتہ("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, چوڑائی, اچائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_پتہ("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_پتہ("glIsFramebuffer");
        أرجع IsFramebuffer(framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_پتہ("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ثابت GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_پتہ("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_پتہ("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_حالت(GLenum target)
    {
        Check_Framebuffer_حالت = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_پتہ("glCheckFramebufferStatus");
        أرجع Check_Framebuffer_حالت(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_پتہ("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_پتہ("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_پتہ("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_پتہ("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_پتہ("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_پتہ("glGenerateMipmap");
        GenerateMipmap(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield نقاب, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_پتہ("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, نقاب, filter);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei چوڑائی, GLsizei اچائی)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_پتہ("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, چوڑائی, اچائی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_پتہ("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_ColorMaski(GLuint اشاریہ, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_پتہ("glColorMaski");
        ColorMaski(اشاریہ, r, g, b, a);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint اشاریہ, GLboolean *انکڑے)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_پتہ("glGetBooleani_v");
        GetBooleani_v(target, اشاریہ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint اشاریہ, GLint *انکڑے)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_پتہ("glGetIntegeri_v");
        GetIntegeri_v(target, اشاریہ, انکڑے);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint اشاریہ)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_پتہ("glEnablei");
        Enablei(target, اشاریہ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint اشاریہ)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_پتہ("glDisablei");
        Disablei(target, اشاریہ);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint اشاریہ)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_پتہ("glIsEnabledi");
        أرجع IsEnabledi(target, اشاریہ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_منوال)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_پتہ("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_سرا_Transform_Feedback()
    {
        سرا_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_پتہ("glEndTransformFeedback");
        سرا_Transform_Feedback();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر_Range(GLenum target, GLuint اشاریہ, GLuint بفر, GLintptr offset, GLsizeiptr size)
    {
        Bind_بفر_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_پتہ("glBindBufferRange");
        Bind_بفر_Range(target, اشاریہ, بفر, offset, size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر_ادھار(GLenum target, GLuint اشاریہ, GLuint بفر)
    {
        Bind_بفر_ادھار = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_پتہ("glBindBufferBase");
        Bind_بفر_ادھار(target, اشاریہ, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint پروگرام, GLsizei گنتی_2, ثابت GLchar* ثابت *varyings, GLenum بفر_منوال)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_پتہ("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(پروگرام, گنتی_2, varyings, بفر_منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint پروگرام, GLuint اشاریہ, GLsizei buf_ناپ, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_پتہ("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(پروگرام, اشاریہ, buf_ناپ, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_پتہ("glClampColor");
        ClampColor(target, clamp);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum منوال)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_پتہ("glBeginConditionalRender");
        BeginConditionalRender(id, منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_سرا_Conditional_Render()
    {
        سرا_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_پتہ("glEndConditionalRender");
        سرا_Conditional_Render();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_IPointer(GLuint اشاریہ, GLint size, GLenum type, GLsizei stride, ثابت GLvoid *pointer)
    {
        راس_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_پتہ("glVertexAttribIPointer");
        راس_Attrib_IPointer(اشاریہ, size, type, stride, pointer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Iiv(GLuint اشاریہ, GLenum pname, GLint *params)
    {
        Get_راس_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_پتہ("glGetVertexAttribIiv");
        Get_راس_Attrib_Iiv(اشاریہ, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Iuiv(GLuint اشاریہ, GLenum pname, GLuint *params)
    {
        Get_راس_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_پتہ("glGetVertexAttribIuiv");
        Get_راس_Attrib_Iuiv(اشاریہ, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1i(GLuint اشاریہ, GLint x)
    {
        راس_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_پتہ("glVertexAttribI1i");
        راس_Attrib_I1i(اشاریہ, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2i(GLuint اشاریہ, GLint x, GLint y)
    {
        راس_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_پتہ("glVertexAttribI2i");
        راس_Attrib_I2i(اشاریہ, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3i(GLuint اشاریہ, GLint x, GLint y, GLint z)
    {
        راس_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_پتہ("glVertexAttribI3i");
        راس_Attrib_I3i(اشاریہ, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4i(GLuint اشاریہ, GLint x, GLint y, GLint z, GLint w)
    {
        راس_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_پتہ("glVertexAttribI4i");
        راس_Attrib_I4i(اشاریہ, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1ui(GLuint اشاریہ, GLuint x)
    {
        راس_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_پتہ("glVertexAttribI1ui");
        راس_Attrib_I1ui(اشاریہ, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2ui(GLuint اشاریہ, GLuint x, GLuint y)
    {
        راس_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_پتہ("glVertexAttribI2ui");
        راس_Attrib_I2ui(اشاریہ, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3ui(GLuint اشاریہ, GLuint x, GLuint y, GLuint z)
    {
        راس_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_پتہ("glVertexAttribI3ui");
        راس_Attrib_I3ui(اشاریہ, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4ui(GLuint اشاریہ, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        راس_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_پتہ("glVertexAttribI4ui");
        راس_Attrib_I4ui(اشاریہ, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1iv(GLuint اشاریہ, ثابت GLint *v)
    {
        راس_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_پتہ("glVertexAttribI1iv");
        راس_Attrib_I1iv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2iv(GLuint اشاریہ, ثابت GLint *v)
    {
        راس_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_پتہ("glVertexAttribI2iv");
        راس_Attrib_I2iv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3iv(GLuint اشاریہ, ثابت GLint *v)
    {
        راس_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_پتہ("glVertexAttribI3iv");
        راس_Attrib_I3iv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4iv(GLuint اشاریہ, ثابت GLint *v)
    {
        راس_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_پتہ("glVertexAttribI4iv");
        راس_Attrib_I4iv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1uiv(GLuint اشاریہ, ثابت GLuint *v)
    {
        راس_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_پتہ("glVertexAttribI1uiv");
        راس_Attrib_I1uiv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2uiv(GLuint اشاریہ, ثابت GLuint *v)
    {
        راس_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_پتہ("glVertexAttribI2uiv");
        راس_Attrib_I2uiv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3uiv(GLuint اشاریہ, ثابت GLuint *v)
    {
        راس_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_پتہ("glVertexAttribI3uiv");
        راس_Attrib_I3uiv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4uiv(GLuint اشاریہ, ثابت GLuint *v)
    {
        راس_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_پتہ("glVertexAttribI4uiv");
        راس_Attrib_I4uiv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4bv(GLuint اشاریہ, ثابت GLbyte *v)
    {
        راس_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_پتہ("glVertexAttribI4bv");
        راس_Attrib_I4bv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4sv(GLuint اشاریہ, ثابت GLshort *v)
    {
        راس_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_پتہ("glVertexAttribI4sv");
        راس_Attrib_I4sv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4ubv(GLuint اشاریہ, ثابت GLubyte *v)
    {
        راس_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_پتہ("glVertexAttribI4ubv");
        راس_Attrib_I4ubv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4usv(GLuint اشاریہ, ثابت GLushort *v)
    {
        راس_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_پتہ("glVertexAttribI4usv");
        راس_Attrib_I4usv(اشاریہ, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint پروگرام, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_پتہ("glGetUniformuiv");
        GetUniformuiv(پروگرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_Frag_انکڑے_Location(GLuint پروگرام, GLuint color, ثابت GLchar *name)
    {
        Bind_Frag_انکڑے_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_پتہ("glBindFragDataLocation");
        Bind_Frag_انکڑے_Location(پروگرام, color, name);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_Frag_انکڑے_Location(GLuint پروگرام, ثابت GLchar *name)
    {
        Get_Frag_انکڑے_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_پتہ("glGetFragDataLocation");
        أرجع Get_Frag_انکڑے_Location(پروگرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_پتہ("glUniform1ui");
        Uniform1ui(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_پتہ("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_پتہ("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_پتہ("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei گنتی_2, ثابت GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_پتہ("glUniform1uiv");
        Uniform1uiv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei گنتی_2, ثابت GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_پتہ("glUniform2uiv");
        Uniform2uiv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei گنتی_2, ثابت GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_پتہ("glUniform3uiv");
        Uniform3uiv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei گنتی_2, ثابت GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_پتہ("glUniform4uiv");
        Uniform4uiv(location, گنتی_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_پتہ("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ثابت GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_پتہ("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_پتہ("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_پتہ("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum بفر, GLint drawbuffer, ثابت GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_پتہ("glClearBufferiv");
        ClearBufferiv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum بفر, GLint drawbuffer, ثابت GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_پتہ("glClearBufferuiv");
        ClearBufferuiv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum بفر, GLint drawbuffer, ثابت GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_پتہ("glClearBufferfv");
        ClearBufferfv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum بفر, GLint drawbuffer, GLfloat ڈونگھائی, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_پتہ("glClearBufferfi");
        ClearBufferfi(بفر, drawbuffer, ڈونگھائی, stencil);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint اشاریہ)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_پتہ("glGetStringi");
        أرجع GetStringi(name, اشاریہ);
    }

    // Extension: ARB_uniform_buffer_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_اکسار_Indices(GLuint پروگرام, GLsizei اکسار_گنتی, ثابت GLchar* ثابت *اکسار_ناں, GLuint *اکسار_Indices)
    {
        Get_اکسار_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_پتہ("glGetUniformIndices");
        Get_اکسار_Indices(پروگرام, اکسار_گنتی, اکسار_ناں, اکسار_Indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_Uniformsiv(GLuint پروگرام, GLsizei اکسار_گنتی, ثابت GLuint *اکسار_Indices, GLenum pname, GLint *params)
    {
        Get_سرگرم_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_پتہ("glGetActiveUniformsiv");
        Get_سرگرم_Uniformsiv(پروگرام, اکسار_گنتی, اکسار_Indices, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_اکسار_Name(GLuint پروگرام, GLuint اکسار_اشاریہ, GLsizei buf_ناپ, GLsizei *length, GLchar *اکسار_Name)
    {
        Get_سرگرم_اکسار_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_پتہ("glGetActiveUniformName");
        Get_سرگرم_اکسار_Name(پروگرام, اکسار_اشاریہ, buf_ناپ, length, اکسار_Name);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Get_اکسار_بلاک_اشاریہ(GLuint پروگرام, ثابت GLchar *اکسار_بلاک_Name)
    {
        Get_اکسار_بلاک_اشاریہ = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_پتہ("glGetUniformBlockIndex");
        أرجع Get_اکسار_بلاک_اشاریہ(پروگرام, اکسار_بلاک_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_اکسار_Blockiv(GLuint پروگرام, GLuint اکسار_بلاک_اشاریہ, GLenum pname, GLint *params)
    {
        Get_سرگرم_اکسار_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_پتہ("glGetActiveUniformBlockiv");
        Get_سرگرم_اکسار_Blockiv(پروگرام, اکسار_بلاک_اشاریہ, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_سرگرم_اکسار_بلاک_Name(GLuint پروگرام, GLuint اکسار_بلاک_اشاریہ, GLsizei buf_ناپ, GLsizei *length, GLchar *اکسار_بلاک_Name)
    {
        Get_سرگرم_اکسار_بلاک_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_پتہ("glGetActiveUniformBlockName");
        Get_سرگرم_اکسار_بلاک_Name(پروگرام, اکسار_بلاک_اشاریہ, buf_ناپ, length, اکسار_بلاک_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_اکسار_بلاک_Binding(GLuint پروگرام, GLuint اکسار_بلاک_اشاریہ, GLuint اکسار_بلاک_Binding_2)
    {
        اکسار_بلاک_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_پتہ("glUniformBlockBinding");
        اکسار_بلاک_Binding(پروگرام, اکسار_بلاک_اشاریہ, اکسار_بلاک_Binding_2);
    }

    // Extension: ARB_copy_buffer

    ساكن فراغ CODEGEN_FUNCPTR Switch_Copy_بفر_Sub_انکڑے(GLenum read_نشانہ, GLenum write_نشانہ, GLintptr read_وتھ, GLintptr write_وتھ, GLsizeiptr size)
    {
        Copy_بفر_Sub_انکڑے = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_پتہ("glCopyBufferSubData");
        Copy_بفر_Sub_انکڑے(read_نشانہ, write_نشانہ, read_وتھ, write_وتھ, size);
    }

    // Extension: 3.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum منوال, GLint first, GLsizei گنتی_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_پتہ("glDrawArraysInstanced");
        DrawArraysInstanced(منوال, first, گنتی_2, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum منوال, GLsizei گنتی_2, GLenum type, ثابت GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_پتہ("glDrawElementsInstanced");
        DrawElementsInstanced(منوال, گنتی_2, type, indices, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_بفر(GLenum target, GLenum internalformat, GLuint بفر)
    {
        Tex_بفر = (PFNTEXBUFFERPROC)Int_Get_Proc_پتہ("glTexBuffer");
        Tex_بفر(target, internalformat, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Primitive_Restart_اشاریہ(GLuint اشاریہ)
    {
        Primitive_Restart_اشاریہ = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_پتہ("glPrimitiveRestartIndex");
        Primitive_Restart_اشاریہ(اشاریہ);
    }

    // Legacy

    ساكن فراغ CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_پتہ("glEnableClientState");
        EnableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_پتہ("glDisableClientState");
        DisableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_اشارہ_گر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        راس_اشارہ_گر = (PFNVERTEXPOINTERPROC)Int_Get_Proc_پتہ("glVertexPointer");
        راس_اشارہ_گر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_عام_اشارہ_گر(GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        عام_اشارہ_گر = (PFNNORMALPOINTERPROC)Int_Get_Proc_پتہ("glNormalPointer");
        عام_اشارہ_گر(type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_اشارہ_گر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Color_اشارہ_گر = (PFNCOLORPOINTERPROC)Int_Get_Proc_پتہ("glColorPointer");
        Color_اشارہ_گر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_Coord_اشارہ_گر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Tex_Coord_اشارہ_گر = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_پتہ("glTexCoordPointer");
        Tex_Coord_اشارہ_گر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_پتہ("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_میٹرکس_منوال(GLenum منوال)
    {
        میٹرکس_منوال = (PFNMATRIXMODEPROC)Int_Get_Proc_پتہ("glMatrixMode");
        میٹرکس_منوال(منوال);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_لدو_پچھان_تت(فراغ)
    {
        لدو_پچھان_تت = (PFNLOADIDENTITYPROC)Int_Get_Proc_پتہ("glLoadIdentity");
        لدو_پچھان_تت();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Ortho(GLdouble کھبے, GLdouble سجے, GLdouble bottom, GLdouble چوٹی_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_پتہ("glOrtho");
        Ortho(کھبے, سجے, bottom, چوٹی_2, near_val, far_val);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_پتہ("glColor3d");
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
            سطر_چوڑائی = Switch_سطر_چوڑائی;
            نقطہ_ناپ = Switch_نقطہ_ناپ;
            Polygon_منوال = Switch_Polygon_منوال;
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
            Clear_ڈونگھائی = Switch_Clear_ڈونگھائی;
            Stencil_نقاب = Switch_Stencil_نقاب;
            Color_نقاب = Switch_Color_نقاب;
            ڈونگھائی_نقاب = Switch_ڈونگھائی_نقاب;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            انت = Switch_انت;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            ڈونگھائی_Func = Switch_ڈونگھائی_Func;
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
            ڈونگھائی_Range = Switch_ڈونگھائی_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_وتھ = Switch_Polygon_وتھ;
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
            نقطہ_Parameterf = Switch_نقطہ_Parameterf;
            نقطہ_Parameterfv = Switch_نقطہ_Parameterfv;
            نقطہ_Parameteri = Switch_نقطہ_Parameteri;
            نقطہ_Parameteriv = Switch_نقطہ_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            سرا_Query = Switch_سرا_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_بفر = Switch_Bind_بفر;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_بفر = Switch_Is_بفر;
            بفر_انکڑے = Switch_بفر_انکڑے;
            بفر_Sub_انکڑے = Switch_بفر_Sub_انکڑے;
            Get_بفر_Sub_انکڑے = Switch_Get_بفر_Sub_انکڑے;
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
            Get_سرگرم_اکسار = Switch_Get_سرگرم_اکسار;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_پروگرام_Info_Log = Switch_Get_پروگرام_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_سروت = Switch_Get_Shader_سروت;
            Get_اکسار_Location = Switch_Get_اکسار_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_راس_Attribdv = Switch_Get_راس_Attribdv;
            Get_راس_Attribfv = Switch_Get_راس_Attribfv;
            Get_راس_Attribiv = Switch_Get_راس_Attribiv;
            Get_راس_Attrib_Pointerv = Switch_Get_راس_Attrib_Pointerv;
            Is_پروگرام = Switch_Is_پروگرام;
            IsShader = Switch_IsShader;
            Link_پروگرام = Switch_Link_پروگرام;
            Shader_سروت = Switch_Shader_سروت;
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
            اکسار_Matrix2fv = Switch_اکسار_Matrix2fv;
            اکسار_Matrix3fv = Switch_اکسار_Matrix3fv;
            اکسار_Matrix4fv = Switch_اکسار_Matrix4fv;
            Validate_پروگرام = Switch_Validate_پروگرام;
            راس_Attrib_اشارہ_گر = Switch_راس_Attrib_اشارہ_گر;

            // Extension: 2.1
            اکسار_Matrix2x3fv = Switch_اکسار_Matrix2x3fv;
            اکسار_Matrix3x2fv = Switch_اکسار_Matrix3x2fv;
            اکسار_Matrix2x4fv = Switch_اکسار_Matrix2x4fv;
            اکسار_Matrix4x2fv = Switch_اکسار_Matrix4x2fv;
            اکسار_Matrix3x4fv = Switch_اکسار_Matrix3x4fv;
            اکسار_Matrix4x3fv = Switch_اکسار_Matrix4x3fv;

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
            سرا_Transform_Feedback = Switch_سرا_Transform_Feedback;
            Bind_بفر_Range = Switch_Bind_بفر_Range;
            Bind_بفر_ادھار = Switch_Bind_بفر_ادھار;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            سرا_Conditional_Render = Switch_سرا_Conditional_Render;
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
            Bind_Frag_انکڑے_Location = Switch_Bind_Frag_انکڑے_Location;
            Get_Frag_انکڑے_Location = Switch_Get_Frag_انکڑے_Location;
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
            Get_اکسار_Indices = Switch_Get_اکسار_Indices;
            Get_سرگرم_Uniformsiv = Switch_Get_سرگرم_Uniformsiv;
            Get_سرگرم_اکسار_Name = Switch_Get_سرگرم_اکسار_Name;
            Get_اکسار_بلاک_اشاریہ = Switch_Get_اکسار_بلاک_اشاریہ;
            Get_سرگرم_اکسار_Blockiv = Switch_Get_سرگرم_اکسار_Blockiv;
            Get_سرگرم_اکسار_بلاک_Name = Switch_Get_سرگرم_اکسار_بلاک_Name;
            اکسار_بلاک_Binding = Switch_اکسار_بلاک_Binding;

            // Extension: ARB_copy_buffer
            Copy_بفر_Sub_انکڑے = Switch_Copy_بفر_Sub_انکڑے;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_بفر = Switch_Tex_بفر;
            Primitive_Restart_اشاریہ = Switch_Primitive_Restart_اشاریہ;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            راس_اشارہ_گر = Switch_راس_اشارہ_گر;
            عام_اشارہ_گر = Switch_عام_اشارہ_گر;
            Color_اشارہ_گر = Switch_Color_اشارہ_گر;
            Tex_Coord_اشارہ_گر = Switch_Tex_Coord_اشارہ_گر;
            TexEnvi = Switch_TexEnvi;
            میٹرکس_منوال = Switch_میٹرکس_منوال;
            لدو_پچھان_تت = Switch_لدو_پچھان_تت;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
