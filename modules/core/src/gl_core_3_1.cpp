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

#यदि_परिभाषितम् HAVE_OPENGL

    #यदि_परिभाषितम् __APPLE__
        #अन्तर्भाव <dlfcn.h>

        स्थिर शून्य* Apple_GLGet_Proc_स्थानाङ्कः (ध्रुव वर्ण* name)
        {
            स्थिर तार्किक initialized = असत्य;
            स्थिर शून्य * handle = रिक्त;
            यदि (!handle)
            {
                यदि (!initialized)
                {
                    initialized = सत्य;
                    ध्रुव वर्ण * ध्रुव मार्गः = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(मार्गः, RTLD_LAZY | RTLD_GLOBAL);
                }
                यदि (!handle)
                    प्रत्यागम रिक्त;
            }
            प्रत्यागम dlsym(handle, name);
        }
    #यदि_अन्तः // __APPLE__

    #यदि defined(__sgi) || defined (__sun)
        #अन्तर्भाव <dlfcn.h>
        #अन्तर्भाव <stdio.h>

        स्थिर शून्य* Sun_Get_Proc_स्थानाङ्कः (ध्रुव वर्ण* name)
        {
            प्रकारनिर्देश शून्य* (func_t*)(ध्रुव GLubyte*);

            स्थिर शून्य* h = 0;
            स्थिर func_t gpa = 0;

            यदि (!h)
            {
                h = dlopen(रिक्त, RTLD_LAZY | RTLD_LOCAL);
                यदि (!h)
                    प्रत्यागम 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            प्रत्यागम gpa ? gpa((ध्रुव GLubyte*) name) : dlsym(h, name);
        }
    #यदि_अन्तः // __sgi || __sun

    #यदि defined(_WIN32)
        #यदि_परिभाषितम् _MSC_VER
            #निर्देशः warning(disable: 4055)
            #निर्देशः warning(disable: 4054)
        #यदि_अन्तः

        स्थिर पूर्णाङ्क TestPointer(ध्रुव PROC pTest)
        {
            यदि(!pTest)
                प्रत्यागम 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            यदि (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                प्रत्यागम 0;

            प्रत्यागम 1;
        }

        स्थिर PROC Win_Get_Proc_स्थानाङ्कः(ध्रुव वर्ण* name)
        {
            PROC pFunc = wgl_Get_Proc_स्थानाङ्कः((LPCSTR) name);
            यदि (TestPointer(pFunc))
                प्रत्यागम pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            प्रत्यागम (PROC) Get_Proc_स्थानाङ्कः(glMod, (LPCSTR) name);
        }
    #यदि_अन्तः // _WIN32

    #यदि defined(_WIN32)
        #परिभाषय CV_GL_GET_PROC_स्थानाङ्कः(name) Win_Get_Proc_स्थानाङ्कः(name)
    #नोचेद्यदि defined(__APPLE__)
        #परिभाषय CV_GL_GET_PROC_स्थानाङ्कः(name) Apple_GLGet_Proc_स्थानाङ्कः(name)
    #नोचेद्यदि defined(__sgi) || defined(__sun)
        #परिभाषय CV_GL_GET_PROC_स्थानाङ्कः(name) Sun_Get_Proc_स्थानाङ्कः(name)
    #अन्यथा // GLX
        #अन्तर्भाव <GL/glx.h>

        #परिभाषय CV_GL_GET_PROC_स्थानाङ्कः(name) gl_XGet_Proc_स्थानाङ्कः_ARB((ध्रुव GLubyte*) name)
    #यदि_अन्तः

    स्थिर शून्य* Int_Get_Proc_स्थानाङ्कः(ध्रुव वर्ण* name)
    {
        शून्य* func =  (शून्य*) CV_GL_GET_PROC_स्थानाङ्कः(name);
        यदि (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        प्रत्यागम func;
    }
#अन्यथा
#यदि defined(_MSC_VER)
    #निर्देशः warning(disable : 4702)  // unreachable code
#यदि_अन्तः
    स्थिर शून्य* Int_Get_Proc_स्थानाङ्कः(ध्रुव वर्ण*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#यदि_अन्तः

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
    PFNLINEWIDTHPROC पङ्क्तिः_विस्तारः;
    PFNPOINTSIZEPROC Point_आकार;
    PFNPOLYGONMODEPROC Polygon_बहुलकम्;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_बफर;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC Clear_गभीरता;
    PFNSTENCILMASKPROC Stencil_आवरणम्;
    PFNCOLORMASKPROC Color_आवरणम्;
    PFNDEPTHMASKPROC गभीरता_आवरणम्;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC Finish;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC गभीरता_Func;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC Read_बफर;
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
    PFNDEPTHRANGEPROC गभीरता_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_विस्थापनम्;
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
    PFNACTIVETEXTUREPROC सक्रिय_Texture;
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
    PFNENDQUERYPROC अन्तः_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_बफर;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_बफर;
    PFNBUFFERDATAPROC बफर_दत्त;
    PFNBUFFERSUBDATAPROC बफर_Sub_दत्त;
    PFNGETBUFFERSUBDATAPROC Get_बफर_Sub_दत्त;
    PFNMAPBUFFERPROC Map_बफर;
    PFNUNMAPBUFFERPROC Unmap_बफर;
    PFNGETBUFFERPARAMETERIVPROC Get_बफर_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_बफर_Pointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC Stencil_आवरणम्_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_क्रमादेशः;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_क्रमादेशः;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
    PFNENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
    PFNGETACTIVEATTRIBPROC Get_सक्रिय_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_सक्रिय_एकरूपम्;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_क्रमादेशः_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC GetShaderSource;
    PFNGETUNIFORMLOCATIONPROC Get_एकरूपम्_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC GetVertexAttribdv;
    PFNGETVERTEXATTRIBFVPROC GetVertexAttribfv;
    PFNGETVERTEXATTRIBIVPROC GetVertexAttribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerv;
    PFNISPROGRAMPROC Is_क्रमादेशः;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_क्रमादेशः;
    PFNSHADERSOURCEPROC ShaderSource;
    PFNUSEPROGRAMPROC Use_क्रमादेशः;
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
    PFNUNIFORMMATRIX2FVPROC एकरूपम्_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC एकरूपम्_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC एकरूपम्_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_क्रमादेशः;
    PFNVERTEXATTRIBPOINTERPROC VertexAttribPointer;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC एकरूपम्_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC एकरूपम्_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC एकरूपम्_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC एकरूपम्_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC एकरूपम्_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC एकरूपम्_Matrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC BindVertexArray;
    PFNDELETEVERTEXARRAYSPROC DeleteVertexArrays;
    PFNGENVERTEXARRAYSPROC GenVertexArrays;
    PFNISVERTEXARRAYPROC IsVertexArray;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC Map_बफर_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_बफर_Range;

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
    PFNCHECKFRAMEBUFFERSTATUSPROC Check_Framebuffer_स्थितिः;
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
    PFNENDTRANSFORMFEEDBACKPROC अन्तः_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_बफर_Range;
    PFNBINDBUFFERBASEPROC Bind_बफर_आधारः;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC अन्तः_Conditional_Render;
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
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_दत्त_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_दत्त_Location;
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
    PFNGETUNIFORMINDICESPROC Get_एकरूपम्_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_सक्रिय_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_सक्रिय_एकरूपम्_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_एकरूपम्_खण्डम्_सूचकांक;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_सक्रिय_एकरूपम्_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_सक्रिय_एकरूपम्_खण्डम्_Name;
    PFNUNIFORMBLOCKBINDINGPROC एकरूपम्_खण्डम्_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_बफर_Sub_दत्त;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_बफर;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_सूचकांक;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC VertexPointer;
    PFNNORMALPOINTERPROC प्रसामान्यम्_Pointer;
    PFNCOLORPOINTERPROC ColorPointer;
    PFNTEXCOORDPOINTERPROC TexCoordPointer;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC Matrix_बहुलकम्;
    PFNLOADIDENTITYPROC आरोपय_तटस्थः;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CullFace(GLenum बहुलकम्)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_स्थानाङ्कः("glCullFace");
        CullFace(बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FrontFace(GLenum बहुलकम्)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_स्थानाङ्कः("glFrontFace");
        FrontFace(बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum बहुलकम्)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_स्थानाङ्कः("glHint");
        Hint(target, बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_पङ्क्तिः_विस्तारः(GLfloat विस्तारः)
    {
        पङ्क्तिः_विस्तारः = (PFNLINEWIDTHPROC)Int_Get_Proc_स्थानाङ्कः("glLineWidth");
        पङ्क्तिः_विस्तारः(विस्तारः);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Point_आकार(GLfloat size)
    {
        Point_आकार = (PFNPOINTSIZEPROC)Int_Get_Proc_स्थानाङ्कः("glPointSize");
        Point_आकार(size);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Polygon_बहुलकम्(GLenum face, GLenum बहुलकम्)
    {
        Polygon_बहुलकम् = (PFNPOLYGONMODEPROC)Int_Get_Proc_स्थानाङ्कः("glPolygonMode");
        Polygon_बहुलकम्(face, बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei विस्तारः, GLsizei height)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_स्थानाङ्कः("glScissor");
        Scissor(x, y, विस्तारः, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_स्थानाङ्कः("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, ध्रुव GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_स्थानाङ्कः("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_स्थानाङ्कः("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, ध्रुव GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei विस्तारः, GLint border, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_स्थानाङ्कः("glTexImage1D");
        TexImage1D(target, level, internalformat, विस्तारः, border, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei विस्तारः, GLsizei height, GLint border, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_स्थानाङ्कः("glTexImage2D");
        TexImage2D(target, level, internalformat, विस्तारः, height, border, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Draw_बफर(GLenum बहुलकम्)
    {
        Draw_बफर = (PFNDRAWBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glDrawBuffer");
        Draw_बफर(बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Clear(GLbitfield आवरणम्)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_स्थानाङ्कः("glClear");
        Clear(आवरणम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_स्थानाङ्कः("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_स्थानाङ्कः("glClearStencil");
        ClearStencil(s);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Clear_गभीरता(GLdouble गभीरता)
    {
        Clear_गभीरता = (PFNCLEARDEPTHPROC)Int_Get_Proc_स्थानाङ्कः("glClearDepth");
        Clear_गभीरता(गभीरता);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Stencil_आवरणम्(GLuint आवरणम्)
    {
        Stencil_आवरणम् = (PFNSTENCILMASKPROC)Int_Get_Proc_स्थानाङ्कः("glStencilMask");
        Stencil_आवरणम्(आवरणम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Color_आवरणम्(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_आवरणम् = (PFNCOLORMASKPROC)Int_Get_Proc_स्थानाङ्कः("glColorMask");
        Color_आवरणम्(red, green, blue, alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_गभीरता_आवरणम्(GLboolean flag)
    {
        गभीरता_आवरणम् = (PFNDEPTHMASKPROC)Int_Get_Proc_स्थानाङ्कः("glDepthMask");
        गभीरता_आवरणम्(flag);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_स्थानाङ्कः("glDisable");
        Disable(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_स्थानाङ्कः("glEnable");
        Enable(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Finish()
    {
        Finish = (PFNFINISHPROC)Int_Get_Proc_स्थानाङ्कः("glFinish");
        Finish();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_स्थानाङ्कः("glFlush");
        Flush();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_स्थानाङ्कः("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_स्थानाङ्कः("glLogicOp");
        LogicOp(opcode);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint आवरणम्)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_स्थानाङ्कः("glStencilFunc");
        StencilFunc(func, ref, आवरणम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_स्थानाङ्कः("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_गभीरता_Func(GLenum func)
    {
        गभीरता_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_स्थानाङ्कः("glDepthFunc");
        गभीरता_Func(func);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_स्थानाङ्कः("glPixelStoref");
        PixelStoref(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_स्थानाङ्कः("glPixelStorei");
        PixelStorei(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Read_बफर(GLenum बहुलकम्)
    {
        Read_बफर = (PFNREADBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glReadBuffer");
        Read_बफर(बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei विस्तारः, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_स्थानाङ्कः("glReadPixels");
        ReadPixels(x, y, विस्तारः, height, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_स्थानाङ्कः("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_स्थानाङ्कः("glGetDoublev");
        GetDoublev(pname, params);
    }

    स्थिर GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_स्थानाङ्कः("glGetError");
        प्रत्यागम GetError();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_स्थानाङ्कः("glGetFloatv");
        GetFloatv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_स्थानाङ्कः("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    स्थिर ध्रुव GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_स्थानाङ्कः("glGetString");
        प्रत्यागम GetString(name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_स्थानाङ्कः("glIsEnabled");
        प्रत्यागम IsEnabled(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_गभीरता_Range(GLdouble ren_near, GLdouble ren_far)
    {
        गभीरता_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_स्थानाङ्कः("glDepthRange");
        गभीरता_Range(ren_near, ren_far);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei विस्तारः, GLsizei height)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_स्थानाङ्कः("glViewport");
        Viewport(x, y, विस्तारः, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawArrays(GLenum बहुलकम्, GLint first, GLsizei गणनम्_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_स्थानाङ्कः("glDrawArrays");
        DrawArrays(बहुलकम्, first, गणनम्_2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawElements(GLenum बहुलकम्, GLsizei गणनम्_2, GLenum type, ध्रुव GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_स्थानाङ्कः("glDrawElements");
        DrawElements(बहुलकम्, गणनम्_2, type, indices);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_स्थानाङ्कः("glGetPointerv");
        GetPointerv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Polygon_विस्थापनम्(GLfloat गुणकः, GLfloat units)
    {
        Polygon_विस्थापनम् = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_स्थानाङ्कः("glPolygonOffset");
        Polygon_विस्थापनम्(गुणकः, units);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei विस्तारः, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_स्थानाङ्कः("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, विस्तारः, border);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei विस्तारः, GLsizei height, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_स्थानाङ्कः("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, विस्तारः, height, border);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei विस्तारः)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_स्थानाङ्कः("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, विस्तारः);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei विस्तारः, GLsizei height)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_स्थानाङ्कः("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, विस्तारः, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei विस्तारः, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_स्थानाङ्कः("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, विस्तारः, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei विस्तारः, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_स्थानाङ्कः("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, विस्तारः, height, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_स्थानाङ्कः("glBindTexture");
        BindTexture(target, texture);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, ध्रुव GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_स्थानाङ्कः("glGenTextures");
        GenTextures(n, textures);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_स्थानाङ्कः("glIsTexture");
        प्रत्यागम IsTexture(texture);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_स्थानाङ्कः("glIndexub");
        Indexub(c);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Indexubv(ध्रुव GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_स्थानाङ्कः("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_स्थानाङ्कः("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendEquation(GLenum बहुलकम्)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_स्थानाङ्कः("glBlendEquation");
        BlendEquation(बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum बहुलकम्, GLuint start, GLuint end, GLsizei गणनम्_2, GLenum type, ध्रुव GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_स्थानाङ्कः("glDrawRangeElements");
        DrawRangeElements(बहुलकम्, start, end, गणनम्_2, type, indices);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei विस्तारः, GLsizei height, GLsizei गभीरता, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_स्थानाङ्कः("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, विस्तारः, height, गभीरता, format, type, pixels);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei विस्तारः, GLsizei height)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_स्थानाङ्कः("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, विस्तारः, height);
    }

    // Extension: 1.3

    स्थिर शून्य CODEGEN_FUNCPTR Switch_सक्रिय_Texture(GLenum texture)
    {
        सक्रिय_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_स्थानाङ्कः("glActiveTexture");
        सक्रिय_Texture(texture);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_स्थानाङ्कः("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei विस्तारः, GLsizei height, GLsizei गभीरता, GLint border, GLsizei image_आकार, ध्रुव GLvoid *दत्त)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_स्थानाङ्कः("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, विस्तारः, height, गभीरता, border, image_आकार, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei विस्तारः, GLsizei height, GLint border, GLsizei image_आकार, ध्रुव GLvoid *दत्त)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_स्थानाङ्कः("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, विस्तारः, height, border, image_आकार, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei विस्तारः, GLint border, GLsizei image_आकार, ध्रुव GLvoid *दत्त)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_स्थानाङ्कः("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, विस्तारः, border, image_आकार, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei विस्तारः, GLsizei height, GLsizei गभीरता, GLenum format, GLsizei image_आकार, ध्रुव GLvoid *दत्त)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_स्थानाङ्कः("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, विस्तारः, height, गभीरता, format, image_आकार, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei विस्तारः, GLsizei height, GLenum format, GLsizei image_आकार, ध्रुव GLvoid *दत्त)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_स्थानाङ्कः("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, विस्तारः, height, format, image_आकार, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei विस्तारः, GLenum format, GLsizei image_आकार, ध्रुव GLvoid *दत्त)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_स्थानाङ्कः("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, विस्तारः, format, image_आकार, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_स्थानाङ्कः("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_स्थानाङ्कः("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum बहुलकम्, ध्रुव GLint *first, ध्रुव GLsizei *गणनम्_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_स्थानाङ्कः("glMultiDrawArrays");
        MultiDrawArrays(बहुलकम्, first, गणनम्_2, drawcount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum बहुलकम्, ध्रुव GLsizei *गणनम्_2, GLenum type, ध्रुव GLvoid* ध्रुव *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_स्थानाङ्कः("glMultiDrawElements");
        MultiDrawElements(बहुलकम्, गणनम्_2, type, indices, drawcount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameterf(GLenum pname, GLfloat param)
    {
        PointParameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_स्थानाङ्कः("glPointParameterf");
        PointParameterf(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameterfv(GLenum pname, ध्रुव GLfloat *params)
    {
        PointParameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_स्थानाङ्कः("glPointParameterfv");
        PointParameterfv(pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameteri(GLenum pname, GLint param)
    {
        PointParameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_स्थानाङ्कः("glPointParameteri");
        PointParameteri(pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_PointParameteriv(GLenum pname, ध्रुव GLint *params)
    {
        PointParameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glPointParameteriv");
        PointParameteriv(pname, params);
    }

    // Extension: 1.5

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_स्थानाङ्कः("glGenQueries");
        GenQueries(n, ids);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, ध्रुव GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_स्थानाङ्कः("glIsQuery");
        प्रत्यागम IsQuery(id);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_स्थानाङ्कः("glBeginQuery");
        BeginQuery(target, id);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_अन्तः_Query(GLenum target)
    {
        अन्तः_Query = (PFNENDQUERYPROC)Int_Get_Proc_स्थानाङ्कः("glEndQuery");
        अन्तः_Query(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Bind_बफर(GLenum target, GLuint बफर)
    {
        Bind_बफर = (PFNBINDBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glBindBuffer");
        Bind_बफर(target, बफर);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, ध्रुव GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glGenBuffers");
        GenBuffers(n, buffers);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_Is_बफर(GLuint बफर)
    {
        Is_बफर = (PFNISBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glIsBuffer");
        प्रत्यागम Is_बफर(बफर);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_बफर_दत्त(GLenum target, GLsizeiptr size, ध्रुव GLvoid *दत्त, GLenum usage)
    {
        बफर_दत्त = (PFNBUFFERDATAPROC)Int_Get_Proc_स्थानाङ्कः("glBufferData");
        बफर_दत्त(target, size, दत्त, usage);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_बफर_Sub_दत्त(GLenum target, GLintptr offset, GLsizeiptr size, ध्रुव GLvoid *दत्त)
    {
        बफर_Sub_दत्त = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_स्थानाङ्कः("glBufferSubData");
        बफर_Sub_दत्त(target, offset, size, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_बफर_Sub_दत्त(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *दत्त)
    {
        Get_बफर_Sub_दत्त = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_स्थानाङ्कः("glGetBufferSubData");
        Get_बफर_Sub_दत्त(target, offset, size, दत्त);
    }

    स्थिर GLvoid* CODEGEN_FUNCPTR Switch_Map_बफर(GLenum target, GLenum access)
    {
        Map_बफर = (PFNMAPBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glMapBuffer");
        प्रत्यागम Map_बफर(target, access);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_Unmap_बफर(GLenum target)
    {
        Unmap_बफर = (PFNUNMAPBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glUnmapBuffer");
        प्रत्यागम Unmap_बफर(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_बफर_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_बफर_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetBufferParameteriv");
        Get_बफर_Parameteriv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_बफर_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_बफर_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_स्थानाङ्कः("glGetBufferPointerv");
        Get_बफर_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum बहुलकम्_RGB, GLenum बहुलकम्_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_स्थानाङ्कः("glBlendEquationSeparate");
        BlendEquationSeparate(बहुलकम्_RGB, बहुलकम्_Alpha);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, ध्रुव GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_स्थानाङ्कः("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint आवरणम्)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_स्थानाङ्कः("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, आवरणम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Stencil_आवरणम्_Separate(GLenum face, GLuint आवरणम्)
    {
        Stencil_आवरणम्_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_स्थानाङ्कः("glStencilMaskSeparate");
        Stencil_आवरणम्_Separate(face, आवरणम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_AttachShader(GLuint क्रमादेशः, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_स्थानाङ्कः("glAttachShader");
        AttachShader(क्रमादेशः, shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint क्रमादेशः, GLuint सूचकांक, ध्रुव GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_स्थानाङ्कः("glBindAttribLocation");
        BindAttribLocation(क्रमादेशः, सूचकांक, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_स्थानाङ्कः("glCompileShader");
        CompileShader(shader);
    }

    स्थिर GLuint CODEGEN_FUNCPTR Switch_Create_क्रमादेशः()
    {
        Create_क्रमादेशः = (PFNCREATEPROGRAMPROC)Int_Get_Proc_स्थानाङ्कः("glCreateProgram");
        प्रत्यागम Create_क्रमादेशः();
    }

    स्थिर GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_स्थानाङ्कः("glCreateShader");
        प्रत्यागम CreateShader(type);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Delete_क्रमादेशः(GLuint क्रमादेशः)
    {
        Delete_क्रमादेशः = (PFNDELETEPROGRAMPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteProgram");
        Delete_क्रमादेशः(क्रमादेशः);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteShader");
        DeleteShader(shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DetachShader(GLuint क्रमादेशः, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_स्थानाङ्कः("glDetachShader");
        DetachShader(क्रमादेशः, shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DisableVertexAttribArray(GLuint सूचकांक)
    {
        DisableVertexAttribArray = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_स्थानाङ्कः("glDisableVertexAttribArray");
        DisableVertexAttribArray(सूचकांक);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EnableVertexAttribArray(GLuint सूचकांक)
    {
        EnableVertexAttribArray = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_स्थानाङ्कः("glEnableVertexAttribArray");
        EnableVertexAttribArray(सूचकांक);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_सक्रिय_Attrib(GLuint क्रमादेशः, GLuint सूचकांक, GLsizei buf_आकार, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_सक्रिय_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_स्थानाङ्कः("glGetActiveAttrib");
        Get_सक्रिय_Attrib(क्रमादेशः, सूचकांक, buf_आकार, length, size, type, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_सक्रिय_एकरूपम्(GLuint क्रमादेशः, GLuint सूचकांक, GLsizei buf_आकार, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_सक्रिय_एकरूपम् = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_स्थानाङ्कः("glGetActiveUniform");
        Get_सक्रिय_एकरूपम्(क्रमादेशः, सूचकांक, buf_आकार, length, size, type, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint क्रमादेशः, GLsizei max_गणनम्, GLsizei *गणनम्_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_स्थानाङ्कः("glGetAttachedShaders");
        GetAttachedShaders(क्रमादेशः, max_गणनम्, गणनम्_2, obj);
    }

    स्थिर GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint क्रमादेशः, ध्रुव GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_स्थानाङ्कः("glGetAttribLocation");
        प्रत्यागम GetAttribLocation(क्रमादेशः, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint क्रमादेशः, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetProgramiv");
        GetProgramiv(क्रमादेशः, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_क्रमादेशः_Info_Log(GLuint क्रमादेशः, GLsizei buf_आकार, GLsizei *length, GLchar *infoLog)
    {
        Get_क्रमादेशः_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_स्थानाङ्कः("glGetProgramInfoLog");
        Get_क्रमादेशः_Info_Log(क्रमादेशः, buf_आकार, length, infoLog);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_आकार, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_स्थानाङ्कः("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_आकार, length, infoLog);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetShaderSource(GLuint shader, GLsizei buf_आकार, GLsizei *length, GLchar *source)
    {
        GetShaderSource = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_स्थानाङ्कः("glGetShaderSource");
        GetShaderSource(shader, buf_आकार, length, source);
    }

    स्थिर GLint CODEGEN_FUNCPTR Switch_Get_एकरूपम्_Location(GLuint क्रमादेशः, ध्रुव GLchar *name)
    {
        Get_एकरूपम्_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_स्थानाङ्कः("glGetUniformLocation");
        प्रत्यागम Get_एकरूपम्_Location(क्रमादेशः, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint क्रमादेशः, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_स्थानाङ्कः("glGetUniformfv");
        GetUniformfv(क्रमादेशः, location, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint क्रमादेशः, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetUniformiv");
        GetUniformiv(क्रमादेशः, location, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribdv(GLuint सूचकांक, GLenum pname, GLdouble *params)
    {
        GetVertexAttribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_स्थानाङ्कः("glGetVertexAttribdv");
        GetVertexAttribdv(सूचकांक, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribfv(GLuint सूचकांक, GLenum pname, GLfloat *params)
    {
        GetVertexAttribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_स्थानाङ्कः("glGetVertexAttribfv");
        GetVertexAttribfv(सूचकांक, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribiv(GLuint सूचकांक, GLenum pname, GLint *params)
    {
        GetVertexAttribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetVertexAttribiv");
        GetVertexAttribiv(सूचकांक, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribPointerv(GLuint सूचकांक, GLenum pname, GLvoid* *pointer)
    {
        GetVertexAttribPointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_स्थानाङ्कः("glGetVertexAttribPointerv");
        GetVertexAttribPointerv(सूचकांक, pname, pointer);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_Is_क्रमादेशः(GLuint क्रमादेशः)
    {
        Is_क्रमादेशः = (PFNISPROGRAMPROC)Int_Get_Proc_स्थानाङ्कः("glIsProgram");
        प्रत्यागम Is_क्रमादेशः(क्रमादेशः);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_स्थानाङ्कः("glIsShader");
        प्रत्यागम IsShader(shader);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Link_क्रमादेशः(GLuint क्रमादेशः)
    {
        Link_क्रमादेशः = (PFNLINKPROGRAMPROC)Int_Get_Proc_स्थानाङ्कः("glLinkProgram");
        Link_क्रमादेशः(क्रमादेशः);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ShaderSource(GLuint shader, GLsizei गणनम्_2, ध्रुव GLchar* ध्रुव *string, ध्रुव GLint *length)
    {
        ShaderSource = (PFNSHADERSOURCEPROC)Int_Get_Proc_स्थानाङ्कः("glShaderSource");
        ShaderSource(shader, गणनम्_2, string, length);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Use_क्रमादेशः(GLuint क्रमादेशः)
    {
        Use_क्रमादेशः = (PFNUSEPROGRAMPROC)Int_Get_Proc_स्थानाङ्कः("glUseProgram");
        Use_क्रमादेशः(क्रमादेशः);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_स्थानाङ्कः("glUniform1f");
        Uniform1f(location, v0);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_स्थानाङ्कः("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_स्थानाङ्कः("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_स्थानाङ्कः("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_स्थानाङ्कः("glUniform1i");
        Uniform1i(location, v0);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_स्थानाङ्कः("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_स्थानाङ्कः("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_स्थानाङ्कः("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform1fv");
        Uniform1fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform2fv");
        Uniform2fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform3fv");
        Uniform3fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei गणनम्_2, ध्रुव GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform4fv");
        Uniform4fv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform1iv");
        Uniform1iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform2iv");
        Uniform2iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform3iv");
        Uniform3iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei गणनम्_2, ध्रुव GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform4iv");
        Uniform4iv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix2fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix2fv");
        एकरूपम्_Matrix2fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix3fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix3fv");
        एकरूपम्_Matrix3fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix4fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix4fv");
        एकरूपम्_Matrix4fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Validate_क्रमादेशः(GLuint क्रमादेशः)
    {
        Validate_क्रमादेशः = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_स्थानाङ्कः("glValidateProgram");
        Validate_क्रमादेशः(क्रमादेशः);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribPointer(GLuint सूचकांक, GLint size, GLenum type, GLboolean normalized, GLsizei stride, ध्रुव GLvoid *pointer)
    {
        VertexAttribPointer = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribPointer");
        VertexAttribPointer(सूचकांक, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix2x3fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix2x3fv");
        एकरूपम्_Matrix2x3fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix3x2fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix3x2fv");
        एकरूपम्_Matrix3x2fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix2x4fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix2x4fv");
        एकरूपम्_Matrix2x4fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix4x2fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix4x2fv");
        एकरूपम्_Matrix4x2fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix3x4fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix3x4fv");
        एकरूपम्_Matrix3x4fv(location, गणनम्_2, transpose, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_Matrix4x3fv(GLint location, GLsizei गणनम्_2, GLboolean transpose, ध्रुव GLfloat *value)
    {
        एकरूपम्_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_स्थानाङ्कः("glUniformMatrix4x3fv");
        एकरूपम्_Matrix4x3fv(location, गणनम्_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindVertexArray(GLuint ren_array)
    {
        BindVertexArray = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_स्थानाङ्कः("glBindVertexArray");
        BindVertexArray(ren_array);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteVertexArrays(GLsizei n, ध्रुव GLuint *arrays)
    {
        DeleteVertexArrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteVertexArrays");
        DeleteVertexArrays(n, arrays);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenVertexArrays(GLsizei n, GLuint *arrays)
    {
        GenVertexArrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_स्थानाङ्कः("glGenVertexArrays");
        GenVertexArrays(n, arrays);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsVertexArray(GLuint ren_array)
    {
        IsVertexArray = (PFNISVERTEXARRAYPROC)Int_Get_Proc_स्थानाङ्कः("glIsVertexArray");
        प्रत्यागम IsVertexArray(ren_array);
    }

    // Extension: ARB_map_buffer_range

    स्थिर GLvoid* CODEGEN_FUNCPTR Switch_Map_बफर_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_बफर_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_स्थानाङ्कः("glMapBufferRange");
        प्रत्यागम Map_बफर_Range(target, offset, length, access);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Flush_Mapped_बफर_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_बफर_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_स्थानाङ्कः("glFlushMappedBufferRange");
        Flush_Mapped_बफर_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glIsRenderbuffer");
        प्रत्यागम IsRenderbuffer(renderbuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, ध्रुव GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei विस्तारः, GLsizei height)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_स्थानाङ्कः("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, विस्तारः, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glIsFramebuffer");
        प्रत्यागम IsFramebuffer(framebuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, ध्रुव GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_स्थानाङ्कः("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    स्थिर GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_स्थितिः(GLenum target)
    {
        Check_Framebuffer_स्थितिः = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_स्थानाङ्कः("glCheckFramebufferStatus");
        प्रत्यागम Check_Framebuffer_स्थितिः(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_स्थानाङ्कः("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_स्थानाङ्कः("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_स्थानाङ्कः("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_स्थानाङ्कः("glGenerateMipmap");
        GenerateMipmap(target);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield आवरणम्, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, आवरणम्, filter);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei विस्तारः, GLsizei height)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_स्थानाङ्कः("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, विस्तारः, height);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_स्थानाङ्कः("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ColorMaski(GLuint सूचकांक, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_स्थानाङ्कः("glColorMaski");
        ColorMaski(सूचकांक, r, g, b, a);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint सूचकांक, GLboolean *दत्त)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_स्थानाङ्कः("glGetBooleani_v");
        GetBooleani_v(target, सूचकांक, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint सूचकांक, GLint *दत्त)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_स्थानाङ्कः("glGetIntegeri_v");
        GetIntegeri_v(target, सूचकांक, दत्त);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint सूचकांक)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_स्थानाङ्कः("glEnablei");
        Enablei(target, सूचकांक);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint सूचकांक)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_स्थानाङ्कः("glDisablei");
        Disablei(target, सूचकांक);
    }

    स्थिर GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint सूचकांक)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_स्थानाङ्कः("glIsEnabledi");
        प्रत्यागम IsEnabledi(target, सूचकांक);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_बहुलकम्)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_स्थानाङ्कः("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_अन्तः_Transform_Feedback()
    {
        अन्तः_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_स्थानाङ्कः("glEndTransformFeedback");
        अन्तः_Transform_Feedback();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Bind_बफर_Range(GLenum target, GLuint सूचकांक, GLuint बफर, GLintptr offset, GLsizeiptr size)
    {
        Bind_बफर_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_स्थानाङ्कः("glBindBufferRange");
        Bind_बफर_Range(target, सूचकांक, बफर, offset, size);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Bind_बफर_आधारः(GLenum target, GLuint सूचकांक, GLuint बफर)
    {
        Bind_बफर_आधारः = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_स्थानाङ्कः("glBindBufferBase");
        Bind_बफर_आधारः(target, सूचकांक, बफर);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint क्रमादेशः, GLsizei गणनम्_2, ध्रुव GLchar* ध्रुव *varyings, GLenum बफर_बहुलकम्)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_स्थानाङ्कः("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(क्रमादेशः, गणनम्_2, varyings, बफर_बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint क्रमादेशः, GLuint सूचकांक, GLsizei buf_आकार, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_स्थानाङ्कः("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(क्रमादेशः, सूचकांक, buf_आकार, length, size, type, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_स्थानाङ्कः("glClampColor");
        ClampColor(target, clamp);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum बहुलकम्)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_स्थानाङ्कः("glBeginConditionalRender");
        BeginConditionalRender(id, बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_अन्तः_Conditional_Render()
    {
        अन्तः_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_स्थानाङ्कः("glEndConditionalRender");
        अन्तः_Conditional_Render();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribIPointer(GLuint सूचकांक, GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *pointer)
    {
        VertexAttribIPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribIPointer");
        VertexAttribIPointer(सूचकांक, size, type, stride, pointer);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribIiv(GLuint सूचकांक, GLenum pname, GLint *params)
    {
        GetVertexAttribIiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetVertexAttribIiv");
        GetVertexAttribIiv(सूचकांक, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetVertexAttribIuiv(GLuint सूचकांक, GLenum pname, GLuint *params)
    {
        GetVertexAttribIuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetVertexAttribIuiv");
        GetVertexAttribIuiv(सूचकांक, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1i(GLuint सूचकांक, GLint x)
    {
        VertexAttribI1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI1i");
        VertexAttribI1i(सूचकांक, x);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2i(GLuint सूचकांक, GLint x, GLint y)
    {
        VertexAttribI2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI2i");
        VertexAttribI2i(सूचकांक, x, y);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3i(GLuint सूचकांक, GLint x, GLint y, GLint z)
    {
        VertexAttribI3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI3i");
        VertexAttribI3i(सूचकांक, x, y, z);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4i(GLuint सूचकांक, GLint x, GLint y, GLint z, GLint w)
    {
        VertexAttribI4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4i");
        VertexAttribI4i(सूचकांक, x, y, z, w);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1ui(GLuint सूचकांक, GLuint x)
    {
        VertexAttribI1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI1ui");
        VertexAttribI1ui(सूचकांक, x);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2ui(GLuint सूचकांक, GLuint x, GLuint y)
    {
        VertexAttribI2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI2ui");
        VertexAttribI2ui(सूचकांक, x, y);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3ui(GLuint सूचकांक, GLuint x, GLuint y, GLuint z)
    {
        VertexAttribI3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI3ui");
        VertexAttribI3ui(सूचकांक, x, y, z);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4ui(GLuint सूचकांक, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        VertexAttribI4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4ui");
        VertexAttribI4ui(सूचकांक, x, y, z, w);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1iv(GLuint सूचकांक, ध्रुव GLint *v)
    {
        VertexAttribI1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI1iv");
        VertexAttribI1iv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2iv(GLuint सूचकांक, ध्रुव GLint *v)
    {
        VertexAttribI2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI2iv");
        VertexAttribI2iv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3iv(GLuint सूचकांक, ध्रुव GLint *v)
    {
        VertexAttribI3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI3iv");
        VertexAttribI3iv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4iv(GLuint सूचकांक, ध्रुव GLint *v)
    {
        VertexAttribI4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4iv");
        VertexAttribI4iv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI1uiv(GLuint सूचकांक, ध्रुव GLuint *v)
    {
        VertexAttribI1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI1uiv");
        VertexAttribI1uiv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI2uiv(GLuint सूचकांक, ध्रुव GLuint *v)
    {
        VertexAttribI2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI2uiv");
        VertexAttribI2uiv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI3uiv(GLuint सूचकांक, ध्रुव GLuint *v)
    {
        VertexAttribI3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI3uiv");
        VertexAttribI3uiv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4uiv(GLuint सूचकांक, ध्रुव GLuint *v)
    {
        VertexAttribI4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4uiv");
        VertexAttribI4uiv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4bv(GLuint सूचकांक, ध्रुव GLbyte *v)
    {
        VertexAttribI4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4bv");
        VertexAttribI4bv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4sv(GLuint सूचकांक, ध्रुव GLshort *v)
    {
        VertexAttribI4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4sv");
        VertexAttribI4sv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4ubv(GLuint सूचकांक, ध्रुव GLubyte *v)
    {
        VertexAttribI4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4ubv");
        VertexAttribI4ubv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexAttribI4usv(GLuint सूचकांक, ध्रुव GLushort *v)
    {
        VertexAttribI4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_स्थानाङ्कः("glVertexAttribI4usv");
        VertexAttribI4usv(सूचकांक, v);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint क्रमादेशः, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetUniformuiv");
        GetUniformuiv(क्रमादेशः, location, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Bind_Frag_दत्त_Location(GLuint क्रमादेशः, GLuint color, ध्रुव GLchar *name)
    {
        Bind_Frag_दत्त_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_स्थानाङ्कः("glBindFragDataLocation");
        Bind_Frag_दत्त_Location(क्रमादेशः, color, name);
    }

    स्थिर GLint CODEGEN_FUNCPTR Switch_Get_Frag_दत्त_Location(GLuint क्रमादेशः, ध्रुव GLchar *name)
    {
        Get_Frag_दत्त_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_स्थानाङ्कः("glGetFragDataLocation");
        प्रत्यागम Get_Frag_दत्त_Location(क्रमादेशः, name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_स्थानाङ्कः("glUniform1ui");
        Uniform1ui(location, v0);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_स्थानाङ्कः("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_स्थानाङ्कः("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_स्थानाङ्कः("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform1uiv");
        Uniform1uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform2uiv");
        Uniform2uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform3uiv");
        Uniform3uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei गणनम्_2, ध्रुव GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_स्थानाङ्कः("glUniform4uiv");
        Uniform4uiv(location, गणनम्_2, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, ध्रुव GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_स्थानाङ्कः("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, ध्रुव GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_स्थानाङ्कः("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum बफर, GLint drawbuffer, ध्रुव GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_स्थानाङ्कः("glClearBufferiv");
        ClearBufferiv(बफर, drawbuffer, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum बफर, GLint drawbuffer, ध्रुव GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_स्थानाङ्कः("glClearBufferuiv");
        ClearBufferuiv(बफर, drawbuffer, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum बफर, GLint drawbuffer, ध्रुव GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_स्थानाङ्कः("glClearBufferfv");
        ClearBufferfv(बफर, drawbuffer, value);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum बफर, GLint drawbuffer, GLfloat गभीरता, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_स्थानाङ्कः("glClearBufferfi");
        ClearBufferfi(बफर, drawbuffer, गभीरता, stencil);
    }

    स्थिर ध्रुव GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint सूचकांक)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_स्थानाङ्कः("glGetStringi");
        प्रत्यागम GetStringi(name, सूचकांक);
    }

    // Extension: ARB_uniform_buffer_object

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_एकरूपम्_Indices(GLuint क्रमादेशः, GLsizei एकरूपम्_गणनम्, ध्रुव GLchar* ध्रुव *एकरूपम्_नामानि, GLuint *एकरूपम्_Indices)
    {
        Get_एकरूपम्_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_स्थानाङ्कः("glGetUniformIndices");
        Get_एकरूपम्_Indices(क्रमादेशः, एकरूपम्_गणनम्, एकरूपम्_नामानि, एकरूपम्_Indices);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_सक्रिय_Uniformsiv(GLuint क्रमादेशः, GLsizei एकरूपम्_गणनम्, ध्रुव GLuint *एकरूपम्_Indices, GLenum pname, GLint *params)
    {
        Get_सक्रिय_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetActiveUniformsiv");
        Get_सक्रिय_Uniformsiv(क्रमादेशः, एकरूपम्_गणनम्, एकरूपम्_Indices, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_सक्रिय_एकरूपम्_Name(GLuint क्रमादेशः, GLuint एकरूपम्_सूचकांक, GLsizei buf_आकार, GLsizei *length, GLchar *एकरूपम्_Name)
    {
        Get_सक्रिय_एकरूपम्_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_स्थानाङ्कः("glGetActiveUniformName");
        Get_सक्रिय_एकरूपम्_Name(क्रमादेशः, एकरूपम्_सूचकांक, buf_आकार, length, एकरूपम्_Name);
    }

    स्थिर GLuint CODEGEN_FUNCPTR Switch_Get_एकरूपम्_खण्डम्_सूचकांक(GLuint क्रमादेशः, ध्रुव GLchar *एकरूपम्_खण्डम्_Name)
    {
        Get_एकरूपम्_खण्डम्_सूचकांक = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_स्थानाङ्कः("glGetUniformBlockIndex");
        प्रत्यागम Get_एकरूपम्_खण्डम्_सूचकांक(क्रमादेशः, एकरूपम्_खण्डम्_Name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_सक्रिय_एकरूपम्_Blockiv(GLuint क्रमादेशः, GLuint एकरूपम्_खण्डम्_सूचकांक, GLenum pname, GLint *params)
    {
        Get_सक्रिय_एकरूपम्_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_स्थानाङ्कः("glGetActiveUniformBlockiv");
        Get_सक्रिय_एकरूपम्_Blockiv(क्रमादेशः, एकरूपम्_खण्डम्_सूचकांक, pname, params);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Get_सक्रिय_एकरूपम्_खण्डम्_Name(GLuint क्रमादेशः, GLuint एकरूपम्_खण्डम्_सूचकांक, GLsizei buf_आकार, GLsizei *length, GLchar *एकरूपम्_खण्डम्_Name)
    {
        Get_सक्रिय_एकरूपम्_खण्डम्_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_स्थानाङ्कः("glGetActiveUniformBlockName");
        Get_सक्रिय_एकरूपम्_खण्डम्_Name(क्रमादेशः, एकरूपम्_खण्डम्_सूचकांक, buf_आकार, length, एकरूपम्_खण्डम्_Name);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_एकरूपम्_खण्डम्_Binding(GLuint क्रमादेशः, GLuint एकरूपम्_खण्डम्_सूचकांक, GLuint एकरूपम्_खण्डम्_Binding_2)
    {
        एकरूपम्_खण्डम्_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_स्थानाङ्कः("glUniformBlockBinding");
        एकरूपम्_खण्डम्_Binding(क्रमादेशः, एकरूपम्_खण्डम्_सूचकांक, एकरूपम्_खण्डम्_Binding_2);
    }

    // Extension: ARB_copy_buffer

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Copy_बफर_Sub_दत्त(GLenum readTarget, GLenum writeTarget, GLintptr read_विस्थापनम्, GLintptr write_विस्थापनम्, GLsizeiptr size)
    {
        Copy_बफर_Sub_दत्त = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_स्थानाङ्कः("glCopyBufferSubData");
        Copy_बफर_Sub_दत्त(readTarget, writeTarget, read_विस्थापनम्, write_विस्थापनम्, size);
    }

    // Extension: 3.1

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum बहुलकम्, GLint first, GLsizei गणनम्_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_स्थानाङ्कः("glDrawArraysInstanced");
        DrawArraysInstanced(बहुलकम्, first, गणनम्_2, instancecount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum बहुलकम्, GLsizei गणनम्_2, GLenum type, ध्रुव GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_स्थानाङ्कः("glDrawElementsInstanced");
        DrawElementsInstanced(बहुलकम्, गणनम्_2, type, indices, instancecount);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Tex_बफर(GLenum target, GLenum internalformat, GLuint बफर)
    {
        Tex_बफर = (PFNTEXBUFFERPROC)Int_Get_Proc_स्थानाङ्कः("glTexBuffer");
        Tex_बफर(target, internalformat, बफर);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Primitive_Restart_सूचकांक(GLuint सूचकांक)
    {
        Primitive_Restart_सूचकांक = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_स्थानाङ्कः("glPrimitiveRestartIndex");
        Primitive_Restart_सूचकांक(सूचकांक);
    }

    // Legacy

    स्थिर शून्य CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_स्थानाङ्कः("glEnableClientState");
        EnableClientState(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_स्थानाङ्कः("glDisableClientState");
        DisableClientState(cap);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_VertexPointer(GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        VertexPointer = (PFNVERTEXPOINTERPROC)Int_Get_Proc_स्थानाङ्कः("glVertexPointer");
        VertexPointer(size, type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_प्रसामान्यम्_Pointer(GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        प्रसामान्यम्_Pointer = (PFNNORMALPOINTERPROC)Int_Get_Proc_स्थानाङ्कः("glNormalPointer");
        प्रसामान्यम्_Pointer(type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_ColorPointer(GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        ColorPointer = (PFNCOLORPOINTERPROC)Int_Get_Proc_स्थानाङ्कः("glColorPointer");
        ColorPointer(size, type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexCoordPointer(GLint size, GLenum type, GLsizei stride, ध्रुव GLvoid *ptr)
    {
        TexCoordPointer = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_स्थानाङ्कः("glTexCoordPointer");
        TexCoordPointer(size, type, stride, ptr);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_स्थानाङ्कः("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Matrix_बहुलकम्(GLenum बहुलकम्)
    {
        Matrix_बहुलकम् = (PFNMATRIXMODEPROC)Int_Get_Proc_स्थानाङ्कः("glMatrixMode");
        Matrix_बहुलकम्(बहुलकम्);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_आरोपय_तटस्थः(शून्य)
    {
        आरोपय_तटस्थः = (PFNLOADIDENTITYPROC)Int_Get_Proc_स्थानाङ्कः("glLoadIdentity");
        आरोपय_तटस्थः();
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Ortho(GLdouble वामतः, GLdouble दक्षिणतः, GLdouble bottom, GLdouble शिखरम्_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_स्थानाङ्कः("glOrtho");
        Ortho(वामतः, दक्षिणतः, bottom, शिखरम्_2, near_val, far_val);
    }

    स्थिर शून्य CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_स्थानाङ्कः("glColor3d");
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
            पङ्क्तिः_विस्तारः = Switch_पङ्क्तिः_विस्तारः;
            Point_आकार = Switch_Point_आकार;
            Polygon_बहुलकम् = Switch_Polygon_बहुलकम्;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_बफर = Switch_Draw_बफर;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            Clear_गभीरता = Switch_Clear_गभीरता;
            Stencil_आवरणम् = Switch_Stencil_आवरणम्;
            Color_आवरणम् = Switch_Color_आवरणम्;
            गभीरता_आवरणम् = Switch_गभीरता_आवरणम्;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            Finish = Switch_Finish;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            गभीरता_Func = Switch_गभीरता_Func;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            Read_बफर = Switch_Read_बफर;
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
            गभीरता_Range = Switch_गभीरता_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_विस्थापनम् = Switch_Polygon_विस्थापनम्;
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
            सक्रिय_Texture = Switch_सक्रिय_Texture;
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
            अन्तः_Query = Switch_अन्तः_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_बफर = Switch_Bind_बफर;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_बफर = Switch_Is_बफर;
            बफर_दत्त = Switch_बफर_दत्त;
            बफर_Sub_दत्त = Switch_बफर_Sub_दत्त;
            Get_बफर_Sub_दत्त = Switch_Get_बफर_Sub_दत्त;
            Map_बफर = Switch_Map_बफर;
            Unmap_बफर = Switch_Unmap_बफर;
            Get_बफर_Parameteriv = Switch_Get_बफर_Parameteriv;
            Get_बफर_Pointerv = Switch_Get_बफर_Pointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            Stencil_आवरणम्_Separate = Switch_Stencil_आवरणम्_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_क्रमादेशः = Switch_Create_क्रमादेशः;
            CreateShader = Switch_CreateShader;
            Delete_क्रमादेशः = Switch_Delete_क्रमादेशः;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            DisableVertexAttribArray = Switch_DisableVertexAttribArray;
            EnableVertexAttribArray = Switch_EnableVertexAttribArray;
            Get_सक्रिय_Attrib = Switch_Get_सक्रिय_Attrib;
            Get_सक्रिय_एकरूपम् = Switch_Get_सक्रिय_एकरूपम्;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_क्रमादेशः_Info_Log = Switch_Get_क्रमादेशः_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            GetShaderSource = Switch_GetShaderSource;
            Get_एकरूपम्_Location = Switch_Get_एकरूपम्_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            GetVertexAttribdv = Switch_GetVertexAttribdv;
            GetVertexAttribfv = Switch_GetVertexAttribfv;
            GetVertexAttribiv = Switch_GetVertexAttribiv;
            GetVertexAttribPointerv = Switch_GetVertexAttribPointerv;
            Is_क्रमादेशः = Switch_Is_क्रमादेशः;
            IsShader = Switch_IsShader;
            Link_क्रमादेशः = Switch_Link_क्रमादेशः;
            ShaderSource = Switch_ShaderSource;
            Use_क्रमादेशः = Switch_Use_क्रमादेशः;
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
            एकरूपम्_Matrix2fv = Switch_एकरूपम्_Matrix2fv;
            एकरूपम्_Matrix3fv = Switch_एकरूपम्_Matrix3fv;
            एकरूपम्_Matrix4fv = Switch_एकरूपम्_Matrix4fv;
            Validate_क्रमादेशः = Switch_Validate_क्रमादेशः;
            VertexAttribPointer = Switch_VertexAttribPointer;

            // Extension: 2.1
            एकरूपम्_Matrix2x3fv = Switch_एकरूपम्_Matrix2x3fv;
            एकरूपम्_Matrix3x2fv = Switch_एकरूपम्_Matrix3x2fv;
            एकरूपम्_Matrix2x4fv = Switch_एकरूपम्_Matrix2x4fv;
            एकरूपम्_Matrix4x2fv = Switch_एकरूपम्_Matrix4x2fv;
            एकरूपम्_Matrix3x4fv = Switch_एकरूपम्_Matrix3x4fv;
            एकरूपम्_Matrix4x3fv = Switch_एकरूपम्_Matrix4x3fv;

            // Extension: ARB_vertex_array_object
            BindVertexArray = Switch_BindVertexArray;
            DeleteVertexArrays = Switch_DeleteVertexArrays;
            GenVertexArrays = Switch_GenVertexArrays;
            IsVertexArray = Switch_IsVertexArray;

            // Extension: ARB_map_buffer_range
            Map_बफर_Range = Switch_Map_बफर_Range;
            Flush_Mapped_बफर_Range = Switch_Flush_Mapped_बफर_Range;

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
            Check_Framebuffer_स्थितिः = Switch_Check_Framebuffer_स्थितिः;
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
            अन्तः_Transform_Feedback = Switch_अन्तः_Transform_Feedback;
            Bind_बफर_Range = Switch_Bind_बफर_Range;
            Bind_बफर_आधारः = Switch_Bind_बफर_आधारः;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            अन्तः_Conditional_Render = Switch_अन्तः_Conditional_Render;
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
            Bind_Frag_दत्त_Location = Switch_Bind_Frag_दत्त_Location;
            Get_Frag_दत्त_Location = Switch_Get_Frag_दत्त_Location;
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
            Get_एकरूपम्_Indices = Switch_Get_एकरूपम्_Indices;
            Get_सक्रिय_Uniformsiv = Switch_Get_सक्रिय_Uniformsiv;
            Get_सक्रिय_एकरूपम्_Name = Switch_Get_सक्रिय_एकरूपम्_Name;
            Get_एकरूपम्_खण्डम्_सूचकांक = Switch_Get_एकरूपम्_खण्डम्_सूचकांक;
            Get_सक्रिय_एकरूपम्_Blockiv = Switch_Get_सक्रिय_एकरूपम्_Blockiv;
            Get_सक्रिय_एकरूपम्_खण्डम्_Name = Switch_Get_सक्रिय_एकरूपम्_खण्डम्_Name;
            एकरूपम्_खण्डम्_Binding = Switch_एकरूपम्_खण्डम्_Binding;

            // Extension: ARB_copy_buffer
            Copy_बफर_Sub_दत्त = Switch_Copy_बफर_Sub_दत्त;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_बफर = Switch_Tex_बफर;
            Primitive_Restart_सूचकांक = Switch_Primitive_Restart_सूचकांक;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            VertexPointer = Switch_VertexPointer;
            प्रसामान्यम्_Pointer = Switch_प्रसामान्यम्_Pointer;
            ColorPointer = Switch_ColorPointer;
            TexCoordPointer = Switch_TexCoordPointer;
            TexEnvi = Switch_TexEnvi;
            Matrix_बहुलकम् = Switch_Matrix_बहुलकम्;
            आरोपय_तटस्थः = Switch_आरोपय_तटस्थः;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
