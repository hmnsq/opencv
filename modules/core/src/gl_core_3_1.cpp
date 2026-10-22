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

#که_تعریف_شوی HAVE_OPENGL

    #که_تعریف_شوی __APPLE__
        #ضمن <dlfcn.h>

        ساكن فراغ* Apple_GLGet_Proc_پته (ثابت حرف* name)
        {
            ساكن منطقي initialized = خطأ;
            ساكن فراغ * handle = عدم;
            إذا (!handle)
            {
                إذا (!initialized)
                {
                    initialized = ریښتیا;
                    ثابت حرف * ثابت لار = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(لار, RTLD_LAZY | RTLD_GLOBAL);
                }
                إذا (!handle)
                    أرجع عدم;
            }
            أرجع dlsym(handle, name);
        }
    #د_که_پای // __APPLE__

    #إذا defined(__sgi) || defined (__sun)
        #ضمن <dlfcn.h>
        #ضمن <stdio.h>

        ساكن فراغ* Sun_Get_Proc_پته (ثابت حرف* name)
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
    #د_که_پای // __sgi || __sun

    #إذا defined(_WIN32)
        #که_تعریف_شوی _MSC_VER
            #لارښوونه warning(disable: 4055)
            #لارښوونه warning(disable: 4054)
        #د_که_پای

        ساكن صحيح Test_اشاره_ګر(ثابت PROC pTest)
        {
            إذا(!pTest)
                أرجع 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            إذا (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                أرجع 0;

            أرجع 1;
        }

        ساكن PROC Win_Get_Proc_پته(ثابت حرف* name)
        {
            PROC pFunc = wgl_Get_Proc_پته((LPCSTR) name);
            إذا (Test_اشاره_ګر(pFunc))
                أرجع pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            أرجع (PROC) Get_Proc_پته(glMod, (LPCSTR) name);
        }
    #د_که_پای // _WIN32

    #إذا defined(_WIN32)
        #تعریف_کړه CV_GL_GET_PROC_پته(name) Win_Get_Proc_پته(name)
    #وإلاإذا defined(__APPLE__)
        #تعریف_کړه CV_GL_GET_PROC_پته(name) Apple_GLGet_Proc_پته(name)
    #وإلاإذا defined(__sgi) || defined(__sun)
        #تعریف_کړه CV_GL_GET_PROC_پته(name) Sun_Get_Proc_پته(name)
    #وإلا // GLX
        #ضمن <GL/glx.h>

        #تعریف_کړه CV_GL_GET_PROC_پته(name) gl_XGet_Proc_پته_ARB((ثابت GLubyte*) name)
    #د_که_پای

    ساكن فراغ* Int_Get_Proc_پته(ثابت حرف* name)
    {
        فراغ* func =  (فراغ*) CV_GL_GET_PROC_پته(name);
        إذا (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        أرجع func;
    }
#وإلا
#إذا defined(_MSC_VER)
    #لارښوونه warning(disable : 4702)  // unreachable code
#د_که_پای
    ساكن فراغ* Int_Get_Proc_پته(ثابت حرف*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#د_که_پای

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
    PFNLINEWIDTHPROC کرښه_پلنوالی;
    PFNPOINTSIZEPROC ټکی_اندازه;
    PFNPOLYGONMODEPROC Polygon_ډېرپېښ;
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
    PFNCLEARDEPTHPROC Clear_ژوروالی;
    PFNSTENCILMASKPROC Stencil_ماسک;
    PFNCOLORMASKPROC Color_ماسک;
    PFNDEPTHMASKPROC ژوروالی_ماسک;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC پای;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC ژوروالی_Func;
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
    PFNDEPTHRANGEPROC ژوروالی_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_دننه_واټن;
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
    PFNPOINTPARAMETERFPROC ټکی_Parameterf;
    PFNPOINTPARAMETERFVPROC ټکی_Parameterfv;
    PFNPOINTPARAMETERIPROC ټکی_Parameteri;
    PFNPOINTPARAMETERIVPROC ټکی_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC پای_ټکی_Query;
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
    PFNSTENCILMASKSEPARATEPROC Stencil_ماسک_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_پروګرام;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_پروګرام;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC Disable_راس_Attrib_Array;
    PFNENABLEVERTEXATTRIBARRAYPROC Enable_راس_Attrib_Array;
    PFNGETACTIVEATTRIBPROC Get_فعال_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_فعال_یوشان;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_پروګرام_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_سرچینه;
    PFNGETUNIFORMLOCATIONPROC Get_یوشان_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_راس_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_راس_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_راس_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_راس_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_پروګرام;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_پروګرام;
    PFNSHADERSOURCEPROC Shader_سرچینه;
    PFNUSEPROGRAMPROC Use_پروګرام;
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
    PFNUNIFORMMATRIX2FVPROC یوشان_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC یوشان_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC یوشان_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_پروګرام;
    PFNVERTEXATTRIBPOINTERPROC راس_Attrib_اشاره_ګر;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC یوشان_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC یوشان_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC یوشان_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC یوشان_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC یوشان_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC یوشان_Matrix4x3fv;

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
    PFNENDTRANSFORMFEEDBACKPROC پای_ټکی_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_بفر_Range;
    PFNBINDBUFFERBASEPROC Bind_بفر_بنسټ;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC پای_ټکی_Conditional_Render;
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
    PFNGETUNIFORMINDICESPROC Get_یوشان_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_فعال_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_فعال_یوشان_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_یوشان_بلاک_شاخص;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_فعال_یوشان_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_فعال_یوشان_بلاک_Name;
    PFNUNIFORMBLOCKBINDINGPROC یوشان_بلاک_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_بفر_Sub_معلومات;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_بفر;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_شاخص;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC راس_اشاره_ګر;
    PFNNORMALPOINTERPROC نورمال_اشاره_ګر;
    PFNCOLORPOINTERPROC Color_اشاره_ګر;
    PFNTEXCOORDPOINTERPROC Tex_Coord_اشاره_ګر;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC ماتریکس_ډېرپېښ;
    PFNLOADIDENTITYPROC بارول_بې_اغېزه_عنصر;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_CullFace(GLenum ډېرپېښ)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_پته("glCullFace");
        CullFace(ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FrontFace(GLenum ډېرپېښ)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_پته("glFrontFace");
        FrontFace(ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum ډېرپېښ)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_پته("glHint");
        Hint(target, ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_کرښه_پلنوالی(GLfloat پلنوالی)
    {
        کرښه_پلنوالی = (PFNLINEWIDTHPROC)Int_Get_Proc_پته("glLineWidth");
        کرښه_پلنوالی(پلنوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ټکی_اندازه(GLfloat size)
    {
        ټکی_اندازه = (PFNPOINTSIZEPROC)Int_Get_Proc_پته("glPointSize");
        ټکی_اندازه(size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_ډېرپېښ(GLenum face, GLenum ډېرپېښ)
    {
        Polygon_ډېرپېښ = (PFNPOLYGONMODEPROC)Int_Get_Proc_پته("glPolygonMode");
        Polygon_ډېرپېښ(face, ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei پلنوالی, GLsizei لوړوالی)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_پته("glScissor");
        Scissor(x, y, پلنوالی, لوړوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_پته("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ثابت GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_پته("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_پته("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_پته("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei پلنوالی, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_پته("glTexImage1D");
        TexImage1D(target, level, internalformat, پلنوالی, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei پلنوالی, GLsizei لوړوالی, GLint border, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_پته("glTexImage2D");
        TexImage2D(target, level, internalformat, پلنوالی, لوړوالی, border, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Draw_بفر(GLenum ډېرپېښ)
    {
        Draw_بفر = (PFNDRAWBUFFERPROC)Int_Get_Proc_پته("glDrawBuffer");
        Draw_بفر(ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear(GLbitfield ماسک)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_پته("glClear");
        Clear(ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_پته("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_پته("glClearStencil");
        ClearStencil(s);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Clear_ژوروالی(GLdouble ژوروالی)
    {
        Clear_ژوروالی = (PFNCLEARDEPTHPROC)Int_Get_Proc_پته("glClearDepth");
        Clear_ژوروالی(ژوروالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_ماسک(GLuint ماسک)
    {
        Stencil_ماسک = (PFNSTENCILMASKPROC)Int_Get_Proc_پته("glStencilMask");
        Stencil_ماسک(ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_ماسک(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_ماسک = (PFNCOLORMASKPROC)Int_Get_Proc_پته("glColorMask");
        Color_ماسک(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ژوروالی_ماسک(GLboolean flag)
    {
        ژوروالی_ماسک = (PFNDEPTHMASKPROC)Int_Get_Proc_پته("glDepthMask");
        ژوروالی_ماسک(flag);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_پته("glDisable");
        Disable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_پته("glEnable");
        Enable(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پای()
    {
        پای = (PFNFINISHPROC)Int_Get_Proc_پته("glFinish");
        پای();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_پته("glFlush");
        Flush();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_پته("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_پته("glLogicOp");
        LogicOp(opcode);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint ماسک)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_پته("glStencilFunc");
        StencilFunc(func, ref, ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_پته("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ژوروالی_Func(GLenum func)
    {
        ژوروالی_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_پته("glDepthFunc");
        ژوروالی_Func(func);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_پته("glPixelStoref");
        PixelStoref(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_پته("glPixelStorei");
        PixelStorei(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Read_بفر(GLenum ډېرپېښ)
    {
        Read_بفر = (PFNREADBUFFERPROC)Int_Get_Proc_پته("glReadBuffer");
        Read_بفر(ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_پته("glReadPixels");
        ReadPixels(x, y, پلنوالی, لوړوالی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_پته("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_پته("glGetDoublev");
        GetDoublev(pname, params);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_پته("glGetError");
        أرجع GetError();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_پته("glGetFloatv");
        GetFloatv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_پته("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_پته("glGetString");
        أرجع GetString(name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_پته("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_پته("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_پته("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_پته("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_پته("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_پته("glIsEnabled");
        أرجع IsEnabled(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ژوروالی_Range(GLdouble ren_near, GLdouble ren_far)
    {
        ژوروالی_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_پته("glDepthRange");
        ژوروالی_Range(ren_near, ren_far);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei پلنوالی, GLsizei لوړوالی)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_پته("glViewport");
        Viewport(x, y, پلنوالی, لوړوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArrays(GLenum ډېرپېښ, GLint first, GLsizei شمېر_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_پته("glDrawArrays");
        DrawArrays(ډېرپېښ, first, شمېر_2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElements(GLenum ډېرپېښ, GLsizei شمېر_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_پته("glDrawElements");
        DrawElements(ډېرپېښ, شمېر_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_پته("glGetPointerv");
        GetPointerv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Polygon_دننه_واټن(GLfloat ضریب, GLfloat units)
    {
        Polygon_دننه_واټن = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_پته("glPolygonOffset");
        Polygon_دننه_واټن(ضریب, units);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei پلنوالی, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_پته("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, پلنوالی, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei پلنوالی, GLsizei لوړوالی, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_پته("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, پلنوالی, لوړوالی, border);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei پلنوالی)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_پته("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, پلنوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei پلنوالی, GLsizei لوړوالی)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_پته("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, پلنوالی, لوړوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei پلنوالی, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_پته("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, پلنوالی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_پته("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, پلنوالی, لوړوالی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_پته("glBindTexture");
        BindTexture(target, texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ثابت GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_پته("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_پته("glGenTextures");
        GenTextures(n, textures);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_پته("glIsTexture");
        أرجع IsTexture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_پته("glIndexub");
        Indexub(c);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Indexubv(ثابت GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_پته("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_پته("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquation(GLenum ډېرپېښ)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_پته("glBlendEquation");
        BlendEquation(ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum ډېرپېښ, GLuint پیل, GLuint end, GLsizei شمېر_2, GLenum type, ثابت GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_پته("glDrawRangeElements");
        DrawRangeElements(ډېرپېښ, پیل, end, شمېر_2, type, indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei پلنوالی, GLsizei لوړوالی, GLsizei ژوروالی, GLenum format, GLenum type, ثابت GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_پته("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, پلنوالی, لوړوالی, ژوروالی, format, type, pixels);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei پلنوالی, GLsizei لوړوالی)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_پته("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, پلنوالی, لوړوالی);
    }

    // Extension: 1.3

    ساكن فراغ CODEGEN_FUNCPTR Switch_فعال_Texture(GLenum texture)
    {
        فعال_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_پته("glActiveTexture");
        فعال_Texture(texture);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_پته("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei پلنوالی, GLsizei لوړوالی, GLsizei ژوروالی, GLint border, GLsizei image_اندازه, ثابت GLvoid *معلومات)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_پته("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, پلنوالی, لوړوالی, ژوروالی, border, image_اندازه, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei پلنوالی, GLsizei لوړوالی, GLint border, GLsizei image_اندازه, ثابت GLvoid *معلومات)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_پته("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, پلنوالی, لوړوالی, border, image_اندازه, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei پلنوالی, GLint border, GLsizei image_اندازه, ثابت GLvoid *معلومات)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_پته("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, پلنوالی, border, image_اندازه, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei پلنوالی, GLsizei لوړوالی, GLsizei ژوروالی, GLenum format, GLsizei image_اندازه, ثابت GLvoid *معلومات)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_پته("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, پلنوالی, لوړوالی, ژوروالی, format, image_اندازه, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLsizei image_اندازه, ثابت GLvoid *معلومات)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_پته("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, پلنوالی, لوړوالی, format, image_اندازه, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei پلنوالی, GLenum format, GLsizei image_اندازه, ثابت GLvoid *معلومات)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_پته("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, پلنوالی, format, image_اندازه, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_پته("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_پته("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum ډېرپېښ, ثابت GLint *first, ثابت GLsizei *شمېر_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_پته("glMultiDrawArrays");
        MultiDrawArrays(ډېرپېښ, first, شمېر_2, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum ډېرپېښ, ثابت GLsizei *شمېر_2, GLenum type, ثابت GLvoid* ثابت *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_پته("glMultiDrawElements");
        MultiDrawElements(ډېرپېښ, شمېر_2, type, indices, drawcount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ټکی_Parameterf(GLenum pname, GLfloat param)
    {
        ټکی_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_پته("glPointParameterf");
        ټکی_Parameterf(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ټکی_Parameterfv(GLenum pname, ثابت GLfloat *params)
    {
        ټکی_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_پته("glPointParameterfv");
        ټکی_Parameterfv(pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ټکی_Parameteri(GLenum pname, GLint param)
    {
        ټکی_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_پته("glPointParameteri");
        ټکی_Parameteri(pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ټکی_Parameteriv(GLenum pname, ثابت GLint *params)
    {
        ټکی_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_پته("glPointParameteriv");
        ټکی_Parameteriv(pname, params);
    }

    // Extension: 1.5

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_پته("glGenQueries");
        GenQueries(n, ids);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ثابت GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_پته("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_پته("glIsQuery");
        أرجع IsQuery(id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_پته("glBeginQuery");
        BeginQuery(target, id);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پای_ټکی_Query(GLenum target)
    {
        پای_ټکی_Query = (PFNENDQUERYPROC)Int_Get_Proc_پته("glEndQuery");
        پای_ټکی_Query(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_پته("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_پته("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_پته("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر(GLenum target, GLuint بفر)
    {
        Bind_بفر = (PFNBINDBUFFERPROC)Int_Get_Proc_پته("glBindBuffer");
        Bind_بفر(target, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ثابت GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_پته("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_پته("glGenBuffers");
        GenBuffers(n, buffers);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_بفر(GLuint بفر)
    {
        Is_بفر = (PFNISBUFFERPROC)Int_Get_Proc_پته("glIsBuffer");
        أرجع Is_بفر(بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بفر_معلومات(GLenum target, GLsizeiptr size, ثابت GLvoid *معلومات, GLenum usage)
    {
        بفر_معلومات = (PFNBUFFERDATAPROC)Int_Get_Proc_پته("glBufferData");
        بفر_معلومات(target, size, معلومات, usage);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بفر_Sub_معلومات(GLenum target, GLintptr offset, GLsizeiptr size, ثابت GLvoid *معلومات)
    {
        بفر_Sub_معلومات = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_پته("glBufferSubData");
        بفر_Sub_معلومات(target, offset, size, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Sub_معلومات(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *معلومات)
    {
        Get_بفر_Sub_معلومات = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_پته("glGetBufferSubData");
        Get_بفر_Sub_معلومات(target, offset, size, معلومات);
    }

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_بفر(GLenum target, GLenum access)
    {
        Map_بفر = (PFNMAPBUFFERPROC)Int_Get_Proc_پته("glMapBuffer");
        أرجع Map_بفر(target, access);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Unmap_بفر(GLenum target)
    {
        Unmap_بفر = (PFNUNMAPBUFFERPROC)Int_Get_Proc_پته("glUnmapBuffer");
        أرجع Unmap_بفر(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_بفر_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_پته("glGetBufferParameteriv");
        Get_بفر_Parameteriv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_بفر_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_بفر_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_پته("glGetBufferPointerv");
        Get_بفر_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum ډېرپېښ_RGB, GLenum ډېرپېښ_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_پته("glBlendEquationSeparate");
        BlendEquationSeparate(ډېرپېښ_RGB, ډېرپېښ_Alpha);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ثابت GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_پته("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_پته("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint ماسک)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_پته("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Stencil_ماسک_Separate(GLenum face, GLuint ماسک)
    {
        Stencil_ماسک_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_پته("glStencilMaskSeparate");
        Stencil_ماسک_Separate(face, ماسک);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_AttachShader(GLuint پروګرام, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_پته("glAttachShader");
        AttachShader(پروګرام, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint پروګرام, GLuint شاخص, ثابت GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_پته("glBindAttribLocation");
        BindAttribLocation(پروګرام, شاخص, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_پته("glCompileShader");
        CompileShader(shader);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Create_پروګرام()
    {
        Create_پروګرام = (PFNCREATEPROGRAMPROC)Int_Get_Proc_پته("glCreateProgram");
        أرجع Create_پروګرام();
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_پته("glCreateShader");
        أرجع CreateShader(type);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_پروګرام(GLuint پروګرام)
    {
        Delete_پروګرام = (PFNDELETEPROGRAMPROC)Int_Get_Proc_پته("glDeleteProgram");
        Delete_پروګرام(پروګرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_پته("glDeleteShader");
        DeleteShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DetachShader(GLuint پروګرام, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_پته("glDetachShader");
        DetachShader(پروګرام, shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disable_راس_Attrib_Array(GLuint شاخص)
    {
        Disable_راس_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_پته("glDisableVertexAttribArray");
        Disable_راس_Attrib_Array(شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enable_راس_Attrib_Array(GLuint شاخص)
    {
        Enable_راس_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_پته("glEnableVertexAttribArray");
        Enable_راس_Attrib_Array(شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_Attrib(GLuint پروګرام, GLuint شاخص, GLsizei buf_اندازه, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_فعال_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_پته("glGetActiveAttrib");
        Get_فعال_Attrib(پروګرام, شاخص, buf_اندازه, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یوشان(GLuint پروګرام, GLuint شاخص, GLsizei buf_اندازه, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_فعال_یوشان = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_پته("glGetActiveUniform");
        Get_فعال_یوشان(پروګرام, شاخص, buf_اندازه, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint پروګرام, GLsizei max_شمېر, GLsizei *شمېر_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_پته("glGetAttachedShaders");
        GetAttachedShaders(پروګرام, max_شمېر, شمېر_2, obj);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint پروګرام, ثابت GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_پته("glGetAttribLocation");
        أرجع GetAttribLocation(پروګرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint پروګرام, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_پته("glGetProgramiv");
        GetProgramiv(پروګرام, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_پروګرام_Info_Log(GLuint پروګرام, GLsizei buf_اندازه, GLsizei *length, GLchar *infoLog)
    {
        Get_پروګرام_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_پته("glGetProgramInfoLog");
        Get_پروګرام_Info_Log(پروګرام, buf_اندازه, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_پته("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_اندازه, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_پته("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_اندازه, length, infoLog);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_Shader_سرچینه(GLuint shader, GLsizei buf_اندازه, GLsizei *length, GLchar *سرچینه)
    {
        Get_Shader_سرچینه = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_پته("glGetShaderSource");
        Get_Shader_سرچینه(shader, buf_اندازه, length, سرچینه);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_یوشان_Location(GLuint پروګرام, ثابت GLchar *name)
    {
        Get_یوشان_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_پته("glGetUniformLocation");
        أرجع Get_یوشان_Location(پروګرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint پروګرام, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_پته("glGetUniformfv");
        GetUniformfv(پروګرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint پروګرام, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_پته("glGetUniformiv");
        GetUniformiv(پروګرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribdv(GLuint شاخص, GLenum pname, GLdouble *params)
    {
        Get_راس_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_پته("glGetVertexAttribdv");
        Get_راس_Attribdv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribfv(GLuint شاخص, GLenum pname, GLfloat *params)
    {
        Get_راس_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_پته("glGetVertexAttribfv");
        Get_راس_Attribfv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attribiv(GLuint شاخص, GLenum pname, GLint *params)
    {
        Get_راس_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_پته("glGetVertexAttribiv");
        Get_راس_Attribiv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Pointerv(GLuint شاخص, GLenum pname, GLvoid* *pointer)
    {
        Get_راس_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_پته("glGetVertexAttribPointerv");
        Get_راس_Attrib_Pointerv(شاخص, pname, pointer);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_پروګرام(GLuint پروګرام)
    {
        Is_پروګرام = (PFNISPROGRAMPROC)Int_Get_Proc_پته("glIsProgram");
        أرجع Is_پروګرام(پروګرام);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_پته("glIsShader");
        أرجع IsShader(shader);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Link_پروګرام(GLuint پروګرام)
    {
        Link_پروګرام = (PFNLINKPROGRAMPROC)Int_Get_Proc_پته("glLinkProgram");
        Link_پروګرام(پروګرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Shader_سرچینه(GLuint shader, GLsizei شمېر_2, ثابت GLchar* ثابت *string, ثابت GLint *length)
    {
        Shader_سرچینه = (PFNSHADERSOURCEPROC)Int_Get_Proc_پته("glShaderSource");
        Shader_سرچینه(shader, شمېر_2, string, length);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Use_پروګرام(GLuint پروګرام)
    {
        Use_پروګرام = (PFNUSEPROGRAMPROC)Int_Get_Proc_پته("glUseProgram");
        Use_پروګرام(پروګرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_پته("glUniform1f");
        Uniform1f(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_پته("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_پته("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_پته("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_پته("glUniform1i");
        Uniform1i(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_پته("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_پته("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_پته("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei شمېر_2, ثابت GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_پته("glUniform1fv");
        Uniform1fv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei شمېر_2, ثابت GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_پته("glUniform2fv");
        Uniform2fv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei شمېر_2, ثابت GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_پته("glUniform3fv");
        Uniform3fv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei شمېر_2, ثابت GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_پته("glUniform4fv");
        Uniform4fv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei شمېر_2, ثابت GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_پته("glUniform1iv");
        Uniform1iv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei شمېر_2, ثابت GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_پته("glUniform2iv");
        Uniform2iv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei شمېر_2, ثابت GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_پته("glUniform3iv");
        Uniform3iv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei شمېر_2, ثابت GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_پته("glUniform4iv");
        Uniform4iv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix2fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_پته("glUniformMatrix2fv");
        یوشان_Matrix2fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix3fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_پته("glUniformMatrix3fv");
        یوشان_Matrix3fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix4fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_پته("glUniformMatrix4fv");
        یوشان_Matrix4fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Validate_پروګرام(GLuint پروګرام)
    {
        Validate_پروګرام = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_پته("glValidateProgram");
        Validate_پروګرام(پروګرام);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_اشاره_ګر(GLuint شاخص, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ثابت GLvoid *pointer)
    {
        راس_Attrib_اشاره_ګر = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_پته("glVertexAttribPointer");
        راس_Attrib_اشاره_ګر(شاخص, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix2x3fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_پته("glUniformMatrix2x3fv");
        یوشان_Matrix2x3fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix3x2fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_پته("glUniformMatrix3x2fv");
        یوشان_Matrix3x2fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix2x4fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_پته("glUniformMatrix2x4fv");
        یوشان_Matrix2x4fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix4x2fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_پته("glUniformMatrix4x2fv");
        یوشان_Matrix4x2fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix3x4fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_پته("glUniformMatrix3x4fv");
        یوشان_Matrix3x4fv(location, شمېر_2, transpose, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_Matrix4x3fv(GLint location, GLsizei شمېر_2, GLboolean transpose, ثابت GLfloat *value)
    {
        یوشان_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_پته("glUniformMatrix4x3fv");
        یوشان_Matrix4x3fv(location, شمېر_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_راس_Array(GLuint ren_array)
    {
        Bind_راس_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_پته("glBindVertexArray");
        Bind_راس_Array(ren_array);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Delete_راس_Arrays(GLsizei n, ثابت GLuint *arrays)
    {
        Delete_راس_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_پته("glDeleteVertexArrays");
        Delete_راس_Arrays(n, arrays);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Gen_راس_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_راس_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_پته("glGenVertexArrays");
        Gen_راس_Arrays(n, arrays);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_Is_راس_Array(GLuint ren_array)
    {
        Is_راس_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_پته("glIsVertexArray");
        أرجع Is_راس_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    ساكن GLvoid* CODEGEN_FUNCPTR Switch_Map_بفر_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_بفر_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_پته("glMapBufferRange");
        أرجع Map_بفر_Range(target, offset, length, access);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Flush_Mapped_بفر_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_بفر_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_پته("glFlushMappedBufferRange");
        Flush_Mapped_بفر_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_پته("glIsRenderbuffer");
        أرجع IsRenderbuffer(renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_پته("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ثابت GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_پته("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_پته("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei پلنوالی, GLsizei لوړوالی)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_پته("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, پلنوالی, لوړوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_پته("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_پته("glIsFramebuffer");
        أرجع IsFramebuffer(framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_پته("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ثابت GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_پته("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_پته("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    ساكن GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_حالت(GLenum target)
    {
        Check_Framebuffer_حالت = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_پته("glCheckFramebufferStatus");
        أرجع Check_Framebuffer_حالت(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_پته("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_پته("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_پته("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_پته("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_پته("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_پته("glGenerateMipmap");
        GenerateMipmap(target);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ماسک, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_پته("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, ماسک, filter);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei پلنوالی, GLsizei لوړوالی)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_پته("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, پلنوالی, لوړوالی);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_پته("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    ساكن فراغ CODEGEN_FUNCPTR Switch_ColorMaski(GLuint شاخص, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_پته("glColorMaski");
        ColorMaski(شاخص, r, g, b, a);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint شاخص, GLboolean *معلومات)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_پته("glGetBooleani_v");
        GetBooleani_v(target, شاخص, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint شاخص, GLint *معلومات)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_پته("glGetIntegeri_v");
        GetIntegeri_v(target, شاخص, معلومات);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint شاخص)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_پته("glEnablei");
        Enablei(target, شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint شاخص)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_پته("glDisablei");
        Disablei(target, شاخص);
    }

    ساكن GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint شاخص)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_پته("glIsEnabledi");
        أرجع IsEnabledi(target, شاخص);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_ډېرپېښ)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_پته("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پای_ټکی_Transform_Feedback()
    {
        پای_ټکی_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_پته("glEndTransformFeedback");
        پای_ټکی_Transform_Feedback();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر_Range(GLenum target, GLuint شاخص, GLuint بفر, GLintptr offset, GLsizeiptr size)
    {
        Bind_بفر_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_پته("glBindBufferRange");
        Bind_بفر_Range(target, شاخص, بفر, offset, size);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_بفر_بنسټ(GLenum target, GLuint شاخص, GLuint بفر)
    {
        Bind_بفر_بنسټ = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_پته("glBindBufferBase");
        Bind_بفر_بنسټ(target, شاخص, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint پروګرام, GLsizei شمېر_2, ثابت GLchar* ثابت *varyings, GLenum بفر_ډېرپېښ)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_پته("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(پروګرام, شمېر_2, varyings, بفر_ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint پروګرام, GLuint شاخص, GLsizei buf_اندازه, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_پته("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(پروګرام, شاخص, buf_اندازه, length, size, type, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_پته("glClampColor");
        ClampColor(target, clamp);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum ډېرپېښ)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_پته("glBeginConditionalRender");
        BeginConditionalRender(id, ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_پای_ټکی_Conditional_Render()
    {
        پای_ټکی_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_پته("glEndConditionalRender");
        پای_ټکی_Conditional_Render();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_IPointer(GLuint شاخص, GLint size, GLenum type, GLsizei stride, ثابت GLvoid *pointer)
    {
        راس_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_پته("glVertexAttribIPointer");
        راس_Attrib_IPointer(شاخص, size, type, stride, pointer);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Iiv(GLuint شاخص, GLenum pname, GLint *params)
    {
        Get_راس_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_پته("glGetVertexAttribIiv");
        Get_راس_Attrib_Iiv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_راس_Attrib_Iuiv(GLuint شاخص, GLenum pname, GLuint *params)
    {
        Get_راس_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_پته("glGetVertexAttribIuiv");
        Get_راس_Attrib_Iuiv(شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1i(GLuint شاخص, GLint x)
    {
        راس_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_پته("glVertexAttribI1i");
        راس_Attrib_I1i(شاخص, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2i(GLuint شاخص, GLint x, GLint y)
    {
        راس_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_پته("glVertexAttribI2i");
        راس_Attrib_I2i(شاخص, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3i(GLuint شاخص, GLint x, GLint y, GLint z)
    {
        راس_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_پته("glVertexAttribI3i");
        راس_Attrib_I3i(شاخص, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4i(GLuint شاخص, GLint x, GLint y, GLint z, GLint w)
    {
        راس_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_پته("glVertexAttribI4i");
        راس_Attrib_I4i(شاخص, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1ui(GLuint شاخص, GLuint x)
    {
        راس_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_پته("glVertexAttribI1ui");
        راس_Attrib_I1ui(شاخص, x);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2ui(GLuint شاخص, GLuint x, GLuint y)
    {
        راس_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_پته("glVertexAttribI2ui");
        راس_Attrib_I2ui(شاخص, x, y);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3ui(GLuint شاخص, GLuint x, GLuint y, GLuint z)
    {
        راس_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_پته("glVertexAttribI3ui");
        راس_Attrib_I3ui(شاخص, x, y, z);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4ui(GLuint شاخص, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        راس_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_پته("glVertexAttribI4ui");
        راس_Attrib_I4ui(شاخص, x, y, z, w);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1iv(GLuint شاخص, ثابت GLint *v)
    {
        راس_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_پته("glVertexAttribI1iv");
        راس_Attrib_I1iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2iv(GLuint شاخص, ثابت GLint *v)
    {
        راس_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_پته("glVertexAttribI2iv");
        راس_Attrib_I2iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3iv(GLuint شاخص, ثابت GLint *v)
    {
        راس_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_پته("glVertexAttribI3iv");
        راس_Attrib_I3iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4iv(GLuint شاخص, ثابت GLint *v)
    {
        راس_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_پته("glVertexAttribI4iv");
        راس_Attrib_I4iv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I1uiv(GLuint شاخص, ثابت GLuint *v)
    {
        راس_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_پته("glVertexAttribI1uiv");
        راس_Attrib_I1uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I2uiv(GLuint شاخص, ثابت GLuint *v)
    {
        راس_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_پته("glVertexAttribI2uiv");
        راس_Attrib_I2uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I3uiv(GLuint شاخص, ثابت GLuint *v)
    {
        راس_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_پته("glVertexAttribI3uiv");
        راس_Attrib_I3uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4uiv(GLuint شاخص, ثابت GLuint *v)
    {
        راس_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_پته("glVertexAttribI4uiv");
        راس_Attrib_I4uiv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4bv(GLuint شاخص, ثابت GLbyte *v)
    {
        راس_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_پته("glVertexAttribI4bv");
        راس_Attrib_I4bv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4sv(GLuint شاخص, ثابت GLshort *v)
    {
        راس_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_پته("glVertexAttribI4sv");
        راس_Attrib_I4sv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4ubv(GLuint شاخص, ثابت GLubyte *v)
    {
        راس_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_پته("glVertexAttribI4ubv");
        راس_Attrib_I4ubv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_Attrib_I4usv(GLuint شاخص, ثابت GLushort *v)
    {
        راس_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_پته("glVertexAttribI4usv");
        راس_Attrib_I4usv(شاخص, v);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint پروګرام, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_پته("glGetUniformuiv");
        GetUniformuiv(پروګرام, location, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Bind_Frag_معلومات_Location(GLuint پروګرام, GLuint color, ثابت GLchar *name)
    {
        Bind_Frag_معلومات_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_پته("glBindFragDataLocation");
        Bind_Frag_معلومات_Location(پروګرام, color, name);
    }

    ساكن GLint CODEGEN_FUNCPTR Switch_Get_Frag_معلومات_Location(GLuint پروګرام, ثابت GLchar *name)
    {
        Get_Frag_معلومات_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_پته("glGetFragDataLocation");
        أرجع Get_Frag_معلومات_Location(پروګرام, name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_پته("glUniform1ui");
        Uniform1ui(location, v0);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_پته("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_پته("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_پته("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei شمېر_2, ثابت GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_پته("glUniform1uiv");
        Uniform1uiv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei شمېر_2, ثابت GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_پته("glUniform2uiv");
        Uniform2uiv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei شمېر_2, ثابت GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_پته("glUniform3uiv");
        Uniform3uiv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei شمېر_2, ثابت GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_پته("glUniform4uiv");
        Uniform4uiv(location, شمېر_2, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ثابت GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_پته("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ثابت GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_پته("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_پته("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_پته("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum بفر, GLint drawbuffer, ثابت GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_پته("glClearBufferiv");
        ClearBufferiv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum بفر, GLint drawbuffer, ثابت GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_پته("glClearBufferuiv");
        ClearBufferuiv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum بفر, GLint drawbuffer, ثابت GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_پته("glClearBufferfv");
        ClearBufferfv(بفر, drawbuffer, value);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum بفر, GLint drawbuffer, GLfloat ژوروالی, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_پته("glClearBufferfi");
        ClearBufferfi(بفر, drawbuffer, ژوروالی, stencil);
    }

    ساكن ثابت GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint شاخص)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_پته("glGetStringi");
        أرجع GetStringi(name, شاخص);
    }

    // Extension: ARB_uniform_buffer_object

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_یوشان_Indices(GLuint پروګرام, GLsizei یوشان_شمېر, ثابت GLchar* ثابت *یوشان_نومونه, GLuint *یوشان_Indices)
    {
        Get_یوشان_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_پته("glGetUniformIndices");
        Get_یوشان_Indices(پروګرام, یوشان_شمېر, یوشان_نومونه, یوشان_Indices);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_Uniformsiv(GLuint پروګرام, GLsizei یوشان_شمېر, ثابت GLuint *یوشان_Indices, GLenum pname, GLint *params)
    {
        Get_فعال_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_پته("glGetActiveUniformsiv");
        Get_فعال_Uniformsiv(پروګرام, یوشان_شمېر, یوشان_Indices, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یوشان_Name(GLuint پروګرام, GLuint یوشان_شاخص, GLsizei buf_اندازه, GLsizei *length, GLchar *یوشان_Name)
    {
        Get_فعال_یوشان_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_پته("glGetActiveUniformName");
        Get_فعال_یوشان_Name(پروګرام, یوشان_شاخص, buf_اندازه, length, یوشان_Name);
    }

    ساكن GLuint CODEGEN_FUNCPTR Switch_Get_یوشان_بلاک_شاخص(GLuint پروګرام, ثابت GLchar *یوشان_بلاک_Name)
    {
        Get_یوشان_بلاک_شاخص = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_پته("glGetUniformBlockIndex");
        أرجع Get_یوشان_بلاک_شاخص(پروګرام, یوشان_بلاک_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یوشان_Blockiv(GLuint پروګرام, GLuint یوشان_بلاک_شاخص, GLenum pname, GLint *params)
    {
        Get_فعال_یوشان_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_پته("glGetActiveUniformBlockiv");
        Get_فعال_یوشان_Blockiv(پروګرام, یوشان_بلاک_شاخص, pname, params);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Get_فعال_یوشان_بلاک_Name(GLuint پروګرام, GLuint یوشان_بلاک_شاخص, GLsizei buf_اندازه, GLsizei *length, GLchar *یوشان_بلاک_Name)
    {
        Get_فعال_یوشان_بلاک_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_پته("glGetActiveUniformBlockName");
        Get_فعال_یوشان_بلاک_Name(پروګرام, یوشان_بلاک_شاخص, buf_اندازه, length, یوشان_بلاک_Name);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_یوشان_بلاک_Binding(GLuint پروګرام, GLuint یوشان_بلاک_شاخص, GLuint یوشان_بلاک_Binding_2)
    {
        یوشان_بلاک_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_پته("glUniformBlockBinding");
        یوشان_بلاک_Binding(پروګرام, یوشان_بلاک_شاخص, یوشان_بلاک_Binding_2);
    }

    // Extension: ARB_copy_buffer

    ساكن فراغ CODEGEN_FUNCPTR Switch_Copy_بفر_Sub_معلومات(GLenum read_موخه, GLenum write_موخه, GLintptr read_دننه_واټن, GLintptr write_دننه_واټن, GLsizeiptr size)
    {
        Copy_بفر_Sub_معلومات = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_پته("glCopyBufferSubData");
        Copy_بفر_Sub_معلومات(read_موخه, write_موخه, read_دننه_واټن, write_دننه_واټن, size);
    }

    // Extension: 3.1

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum ډېرپېښ, GLint first, GLsizei شمېر_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_پته("glDrawArraysInstanced");
        DrawArraysInstanced(ډېرپېښ, first, شمېر_2, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum ډېرپېښ, GLsizei شمېر_2, GLenum type, ثابت GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_پته("glDrawElementsInstanced");
        DrawElementsInstanced(ډېرپېښ, شمېر_2, type, indices, instancecount);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_بفر(GLenum target, GLenum internalformat, GLuint بفر)
    {
        Tex_بفر = (PFNTEXBUFFERPROC)Int_Get_Proc_پته("glTexBuffer");
        Tex_بفر(target, internalformat, بفر);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Primitive_Restart_شاخص(GLuint شاخص)
    {
        Primitive_Restart_شاخص = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_پته("glPrimitiveRestartIndex");
        Primitive_Restart_شاخص(شاخص);
    }

    // Legacy

    ساكن فراغ CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_پته("glEnableClientState");
        EnableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_پته("glDisableClientState");
        DisableClientState(cap);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_راس_اشاره_ګر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        راس_اشاره_ګر = (PFNVERTEXPOINTERPROC)Int_Get_Proc_پته("glVertexPointer");
        راس_اشاره_ګر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_نورمال_اشاره_ګر(GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        نورمال_اشاره_ګر = (PFNNORMALPOINTERPROC)Int_Get_Proc_پته("glNormalPointer");
        نورمال_اشاره_ګر(type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color_اشاره_ګر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Color_اشاره_ګر = (PFNCOLORPOINTERPROC)Int_Get_Proc_پته("glColorPointer");
        Color_اشاره_ګر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Tex_Coord_اشاره_ګر(GLint size, GLenum type, GLsizei stride, ثابت GLvoid *ptr)
    {
        Tex_Coord_اشاره_ګر = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_پته("glTexCoordPointer");
        Tex_Coord_اشاره_ګر(size, type, stride, ptr);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_پته("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_ماتریکس_ډېرپېښ(GLenum ډېرپېښ)
    {
        ماتریکس_ډېرپېښ = (PFNMATRIXMODEPROC)Int_Get_Proc_پته("glMatrixMode");
        ماتریکس_ډېرپېښ(ډېرپېښ);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_بارول_بې_اغېزه_عنصر(فراغ)
    {
        بارول_بې_اغېزه_عنصر = (PFNLOADIDENTITYPROC)Int_Get_Proc_پته("glLoadIdentity");
        بارول_بې_اغېزه_عنصر();
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Ortho(GLdouble کیڼ, GLdouble ښی, GLdouble bottom, GLdouble سر_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_پته("glOrtho");
        Ortho(کیڼ, ښی, bottom, سر_2, near_val, far_val);
    }

    ساكن فراغ CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_پته("glColor3d");
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
            کرښه_پلنوالی = Switch_کرښه_پلنوالی;
            ټکی_اندازه = Switch_ټکی_اندازه;
            Polygon_ډېرپېښ = Switch_Polygon_ډېرپېښ;
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
            Clear_ژوروالی = Switch_Clear_ژوروالی;
            Stencil_ماسک = Switch_Stencil_ماسک;
            Color_ماسک = Switch_Color_ماسک;
            ژوروالی_ماسک = Switch_ژوروالی_ماسک;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            پای = Switch_پای;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            ژوروالی_Func = Switch_ژوروالی_Func;
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
            ژوروالی_Range = Switch_ژوروالی_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_دننه_واټن = Switch_Polygon_دننه_واټن;
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
            ټکی_Parameterf = Switch_ټکی_Parameterf;
            ټکی_Parameterfv = Switch_ټکی_Parameterfv;
            ټکی_Parameteri = Switch_ټکی_Parameteri;
            ټکی_Parameteriv = Switch_ټکی_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            پای_ټکی_Query = Switch_پای_ټکی_Query;
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
            Stencil_ماسک_Separate = Switch_Stencil_ماسک_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_پروګرام = Switch_Create_پروګرام;
            CreateShader = Switch_CreateShader;
            Delete_پروګرام = Switch_Delete_پروګرام;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            Disable_راس_Attrib_Array = Switch_Disable_راس_Attrib_Array;
            Enable_راس_Attrib_Array = Switch_Enable_راس_Attrib_Array;
            Get_فعال_Attrib = Switch_Get_فعال_Attrib;
            Get_فعال_یوشان = Switch_Get_فعال_یوشان;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_پروګرام_Info_Log = Switch_Get_پروګرام_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_سرچینه = Switch_Get_Shader_سرچینه;
            Get_یوشان_Location = Switch_Get_یوشان_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_راس_Attribdv = Switch_Get_راس_Attribdv;
            Get_راس_Attribfv = Switch_Get_راس_Attribfv;
            Get_راس_Attribiv = Switch_Get_راس_Attribiv;
            Get_راس_Attrib_Pointerv = Switch_Get_راس_Attrib_Pointerv;
            Is_پروګرام = Switch_Is_پروګرام;
            IsShader = Switch_IsShader;
            Link_پروګرام = Switch_Link_پروګرام;
            Shader_سرچینه = Switch_Shader_سرچینه;
            Use_پروګرام = Switch_Use_پروګرام;
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
            یوشان_Matrix2fv = Switch_یوشان_Matrix2fv;
            یوشان_Matrix3fv = Switch_یوشان_Matrix3fv;
            یوشان_Matrix4fv = Switch_یوشان_Matrix4fv;
            Validate_پروګرام = Switch_Validate_پروګرام;
            راس_Attrib_اشاره_ګر = Switch_راس_Attrib_اشاره_ګر;

            // Extension: 2.1
            یوشان_Matrix2x3fv = Switch_یوشان_Matrix2x3fv;
            یوشان_Matrix3x2fv = Switch_یوشان_Matrix3x2fv;
            یوشان_Matrix2x4fv = Switch_یوشان_Matrix2x4fv;
            یوشان_Matrix4x2fv = Switch_یوشان_Matrix4x2fv;
            یوشان_Matrix3x4fv = Switch_یوشان_Matrix3x4fv;
            یوشان_Matrix4x3fv = Switch_یوشان_Matrix4x3fv;

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
            پای_ټکی_Transform_Feedback = Switch_پای_ټکی_Transform_Feedback;
            Bind_بفر_Range = Switch_Bind_بفر_Range;
            Bind_بفر_بنسټ = Switch_Bind_بفر_بنسټ;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            پای_ټکی_Conditional_Render = Switch_پای_ټکی_Conditional_Render;
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
            Get_یوشان_Indices = Switch_Get_یوشان_Indices;
            Get_فعال_Uniformsiv = Switch_Get_فعال_Uniformsiv;
            Get_فعال_یوشان_Name = Switch_Get_فعال_یوشان_Name;
            Get_یوشان_بلاک_شاخص = Switch_Get_یوشان_بلاک_شاخص;
            Get_فعال_یوشان_Blockiv = Switch_Get_فعال_یوشان_Blockiv;
            Get_فعال_یوشان_بلاک_Name = Switch_Get_فعال_یوشان_بلاک_Name;
            یوشان_بلاک_Binding = Switch_یوشان_بلاک_Binding;

            // Extension: ARB_copy_buffer
            Copy_بفر_Sub_معلومات = Switch_Copy_بفر_Sub_معلومات;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_بفر = Switch_Tex_بفر;
            Primitive_Restart_شاخص = Switch_Primitive_Restart_شاخص;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            راس_اشاره_ګر = Switch_راس_اشاره_ګر;
            نورمال_اشاره_ګر = Switch_نورمال_اشاره_ګر;
            Color_اشاره_ګر = Switch_Color_اشاره_ګر;
            Tex_Coord_اشاره_ګر = Switch_Tex_Coord_اشاره_ګر;
            TexEnvi = Switch_TexEnvi;
            ماتریکس_ډېرپېښ = Switch_ماتریکس_ډېرپېښ;
            بارول_بې_اغېزه_عنصر = Switch_بارول_بې_اغېزه_عنصر;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
