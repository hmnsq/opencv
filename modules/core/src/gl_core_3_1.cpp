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

#समावेश "precomp.hpp"
#समावेश "gl_core_3_1.hpp"

#अगर_घोषित HAVE_OPENGL

    #अगर_घोषित __APPLE__
        #समावेश <dlfcn.h>

        अटल व्योम* AppleGLGetProcAddress (स्थिर अक्षर* name)
        {
            अटल bool initialized = false;
            अटल व्योम * handle = शून्य;
            अगर (!handle)
            {
                अगर (!initialized)
                {
                    initialized = true;
                    स्थिर अक्षर * स्थिर path = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
                }
                अगर (!handle)
                    वापस शून्य;
            }
            वापस dlsym(handle, name);
        }
    #पूर्ण_अगर // __APPLE__

    #अगर defined(__sgi) || defined (__sun)
        #समावेश <dlfcn.h>
        #समावेश <stdio.h>

        अटल व्योम* SunGetProcAddress (स्थिर अक्षर* name)
        {
            प्रकार व्योम* (func_t*)(स्थिर GLubyte*);

            अटल व्योम* h = 0;
            अटल func_t gpa = 0;

            अगर (!h)
            {
                h = dlopen(शून्य, RTLD_LAZY | RTLD_LOCAL);
                अगर (!h)
                    वापस 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            वापस gpa ? gpa((स्थिर GLubyte*) name) : dlsym(h, name);
        }
    #पूर्ण_अगर // __sgi || __sun

    #अगर defined(_WIN32)
        #अगर_घोषित _MSC_VER
            #आशय warning(disable: 4055)
            #आशय warning(disable: 4054)
        #पूर्ण_अगर

        अटल पूर्णांक TestPointer(स्थिर PROC pTest)
        {
            अगर(!pTest)
                वापस 0;

            सूचक_भेद_प्रकार iTest = (सूचक_भेद_प्रकार) pTest;

            अगर (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                वापस 0;

            वापस 1;
        }

        अटल PROC WinGetProcAddress(स्थिर अक्षर* name)
        {
            PROC pFunc = wglGetProcAddress((LPCSTR) name);
            अगर (TestPointer(pFunc))
                वापस pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            वापस (PROC) GetProcAddress(glMod, (LPCSTR) name);
        }
    #पूर्ण_अगर // _WIN32

    #अगर defined(_WIN32)
        #घोषणा CV_GL_GET_PROC_ADDRESS(name) WinGetProcAddress(name)
    #या_अगर defined(__APPLE__)
        #घोषणा CV_GL_GET_PROC_ADDRESS(name) AppleGLGetProcAddress(name)
    #या_अगर defined(__sgi) || defined(__sun)
        #घोषणा CV_GL_GET_PROC_ADDRESS(name) SunGetProcAddress(name)
    #अन्यथा // GLX
        #समावेश <GL/glx.h>

        #घोषणा CV_GL_GET_PROC_ADDRESS(name) glXGetProcAddressARB((स्थिर GLubyte*) name)
    #पूर्ण_अगर

    अटल व्योम* IntGetProcAddress(स्थिर अक्षर* name)
    {
        व्योम* func =  (व्योम*) CV_GL_GET_PROC_ADDRESS(name);
        अगर (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        वापस func;
    }
#अन्यथा
#अगर defined(_MSC_VER)
    #आशय warning(disable : 4702)  // unreachable code
#पूर्ण_अगर
    अटल व्योम* IntGetProcAddress(स्थिर अक्षर*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#पूर्ण_अगर

namespace gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    प्रकार GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    प्रकार स्थिर GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(स्थिर GLubyte *);

    // Extension: 1.2
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , स्थिर GLint *, स्थिर GLsizei *, GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , स्थिर GLsizei *, GLenum , स्थिर GLvoid* स्थिर *, GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , स्थिर GLint *);

    // Extension: 1.5
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , स्थिर GLuint *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , स्थिर GLvoid *, GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    प्रकार GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , स्थिर GLenum *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , स्थिर GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    प्रकार GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    प्रकार GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    प्रकार GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , स्थिर GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकार GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , स्थिर GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , स्थिर GLchar* स्थिर *, स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , स्थिर GLvoid *);

    // Extension: 2.1
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , स्थिर GLfloat *);

    // Extension: ARB_vertex_array_object
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    प्रकार GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    प्रकार GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    प्रकार GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , स्थिर GLchar* स्थिर *, GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , स्थिर GLbyte *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , स्थिर GLshort *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , स्थिर GLubyte *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , स्थिर GLushort *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , स्थिर GLchar *);
    प्रकार GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , स्थिर GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , स्थिर GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , स्थिर GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , स्थिर GLfloat *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    प्रकार स्थिर GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , स्थिर GLchar* स्थिर *, GLuint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , स्थिर GLuint *, GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकार GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , स्थिर GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , स्थिर GLvoid *, GLsizei );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , स्थिर GLvoid *);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(व्योम);
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    प्रकार व्योम (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC पंक्ति_Width;
    PFNPOINTSIZEPROC Point_आकार;
    PFNPOLYGONMODEPROC PolygonMode;
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
    PFNBINDBUFFERPROC Bind_बफर;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_बफर;
    PFNBUFFERDATAPROC बफर_तथ्य;
    PFNBUFFERSUBDATAPROC बफर_Sub_तथ्य;
    PFNGETBUFFERSUBDATAPROC Get_बफर_Sub_तथ्य;
    PFNMAPBUFFERPROC Map_बफर;
    PFNUNMAPBUFFERPROC Unmap_बफर;
    PFNGETBUFFERPARAMETERIVPROC Get_बफर_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_बफर_Pointerv;

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
    PFNBINDBUFFERRANGEPROC Bind_बफर_Range;
    PFNBINDBUFFERBASEPROC Bind_बफर_Base;
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
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_तथ्य_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_तथ्य_Location;
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
    PFNGETUNIFORMBLOCKINDEXPROC Get_Uniform_Block_सूचकांक;
    PFNGETACTIVEUNIFORMBLOCKIVPROC GetActiveUniformBlockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC GetActiveUniformBlockName;
    PFNUNIFORMBLOCKBINDINGPROC UniformBlockBinding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_बफर_Sub_तथ्य;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_बफर;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_सूचकांक;

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

    अटल व्योम CODEGEN_FUNCPTR Switch_CullFace(GLenum mode)
    {
        CullFace = (PFNCULLFACEPROC)IntGetProcAddress("glCullFace");
        CullFace(mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_FrontFace(GLenum mode)
    {
        FrontFace = (PFNFRONTFACEPROC)IntGetProcAddress("glFrontFace");
        FrontFace(mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum mode)
    {
        Hint = (PFNHINTPROC)IntGetProcAddress("glHint");
        Hint(target, mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_पंक्ति_Width(GLfloat width)
    {
        पंक्ति_Width = (PFNLINEWIDTHPROC)IntGetProcAddress("glLineWidth");
        पंक्ति_Width(width);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Point_आकार(GLfloat size)
    {
        Point_आकार = (PFNPOINTSIZEPROC)IntGetProcAddress("glPointSize");
        Point_आकार(size);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PolygonMode(GLenum face, GLenum mode)
    {
        PolygonMode = (PFNPOLYGONMODEPROC)IntGetProcAddress("glPolygonMode");
        PolygonMode(face, mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Scissor = (PFNSCISSORPROC)IntGetProcAddress("glScissor");
        Scissor(x, y, width, height);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)IntGetProcAddress("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, स्थिर GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)IntGetProcAddress("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)IntGetProcAddress("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, स्थिर GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)IntGetProcAddress("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, स्थिर GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)IntGetProcAddress("glTexImage1D");
        TexImage1D(target, level, internalformat, width, border, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, स्थिर GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)IntGetProcAddress("glTexImage2D");
        TexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Draw_बफर(GLenum mode)
    {
        Draw_बफर = (PFNDRAWBUFFERPROC)IntGetProcAddress("glDrawBuffer");
        Draw_बफर(mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Clear(GLbitfield mask)
    {
        Clear = (PFNCLEARPROC)IntGetProcAddress("glClear");
        Clear(mask);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)IntGetProcAddress("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)IntGetProcAddress("glClearStencil");
        ClearStencil(s);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearDepth(GLdouble depth)
    {
        ClearDepth = (PFNCLEARDEPTHPROC)IntGetProcAddress("glClearDepth");
        ClearDepth(depth);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_StencilMask(GLuint mask)
    {
        StencilMask = (PFNSTENCILMASKPROC)IntGetProcAddress("glStencilMask");
        StencilMask(mask);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        ColorMask = (PFNCOLORMASKPROC)IntGetProcAddress("glColorMask");
        ColorMask(red, green, blue, alpha);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DepthMask(GLboolean flag)
    {
        DepthMask = (PFNDEPTHMASKPROC)IntGetProcAddress("glDepthMask");
        DepthMask(flag);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)IntGetProcAddress("glDisable");
        Disable(cap);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)IntGetProcAddress("glEnable");
        Enable(cap);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Finish()
    {
        Finish = (PFNFINISHPROC)IntGetProcAddress("glFinish");
        Finish();
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)IntGetProcAddress("glFlush");
        Flush();
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)IntGetProcAddress("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)IntGetProcAddress("glLogicOp");
        LogicOp(opcode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint mask)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)IntGetProcAddress("glStencilFunc");
        StencilFunc(func, ref, mask);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_StencilOp(GLenum हार, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)IntGetProcAddress("glStencilOp");
        StencilOp(हार, zfail, zpass);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DepthFunc(GLenum func)
    {
        DepthFunc = (PFNDEPTHFUNCPROC)IntGetProcAddress("glDepthFunc");
        DepthFunc(func);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)IntGetProcAddress("glPixelStoref");
        PixelStoref(pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)IntGetProcAddress("glPixelStorei");
        PixelStorei(pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Read_बफर(GLenum mode)
    {
        Read_बफर = (PFNREADBUFFERPROC)IntGetProcAddress("glReadBuffer");
        Read_बफर(mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)IntGetProcAddress("glReadPixels");
        ReadPixels(x, y, width, height, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)IntGetProcAddress("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)IntGetProcAddress("glGetDoublev");
        GetDoublev(pname, params);
    }

    अटल GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)IntGetProcAddress("glGetError");
        वापस GetError();
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)IntGetProcAddress("glGetFloatv");
        GetFloatv(pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)IntGetProcAddress("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    अटल स्थिर GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)IntGetProcAddress("glGetString");
        वापस GetString(name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)IntGetProcAddress("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)IntGetProcAddress("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)IntGetProcAddress("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)IntGetProcAddress("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)IntGetProcAddress("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)IntGetProcAddress("glIsEnabled");
        वापस IsEnabled(cap);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DepthRange(GLdouble ren_near, GLdouble ren_far)
    {
        DepthRange = (PFNDEPTHRANGEPROC)IntGetProcAddress("glDepthRange");
        DepthRange(ren_near, ren_far);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
    {
        Viewport = (PFNVIEWPORTPROC)IntGetProcAddress("glViewport");
        Viewport(x, y, width, height);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DrawArrays(GLenum mode, GLint first, GLsizei गिनती_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)IntGetProcAddress("glDrawArrays");
        DrawArrays(mode, first, गिनती_2);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DrawElements(GLenum mode, GLsizei गिनती_2, GLenum type, स्थिर GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)IntGetProcAddress("glDrawElements");
        DrawElements(mode, गिनती_2, type, indices);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)IntGetProcAddress("glGetPointerv");
        GetPointerv(pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PolygonOffset(GLfloat factor, GLfloat units)
    {
        PolygonOffset = (PFNPOLYGONOFFSETPROC)IntGetProcAddress("glPolygonOffset");
        PolygonOffset(factor, units);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)IntGetProcAddress("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, width, border);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)IntGetProcAddress("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)IntGetProcAddress("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, width);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)IntGetProcAddress("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, स्थिर GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)IntGetProcAddress("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, width, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, स्थिर GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)IntGetProcAddress("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)IntGetProcAddress("glBindTexture");
        BindTexture(target, texture);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, स्थिर GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)IntGetProcAddress("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)IntGetProcAddress("glGenTextures");
        GenTextures(n, textures);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)IntGetProcAddress("glIsTexture");
        वापस IsTexture(texture);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)IntGetProcAddress("glIndexub");
        Indexub(c);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Indexubv(स्थिर GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)IntGetProcAddress("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    अटल व्योम CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)IntGetProcAddress("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BlendEquation(GLenum mode)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)IntGetProcAddress("glBlendEquation");
        BlendEquation(mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei गिनती_2, GLenum type, स्थिर GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)IntGetProcAddress("glDrawRangeElements");
        DrawRangeElements(mode, start, end, गिनती_2, type, indices);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, स्थिर GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)IntGetProcAddress("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)IntGetProcAddress("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
    }

    // Extension: 1.3

    अटल व्योम CODEGEN_FUNCPTR Switch_ActiveTexture(GLenum texture)
    {
        ActiveTexture = (PFNACTIVETEXTUREPROC)IntGetProcAddress("glActiveTexture");
        ActiveTexture(texture);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)IntGetProcAddress("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei image_आकार, स्थिर GLvoid *तथ्य)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)IntGetProcAddress("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, width, height, depth, border, image_आकार, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei image_आकार, स्थिर GLvoid *तथ्य)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)IntGetProcAddress("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, width, height, border, image_आकार, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei image_आकार, स्थिर GLvoid *तथ्य)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)IntGetProcAddress("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, width, border, image_आकार, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei image_आकार, स्थिर GLvoid *तथ्य)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)IntGetProcAddress("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, image_आकार, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei image_आकार, स्थिर GLvoid *तथ्य)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)IntGetProcAddress("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, image_आकार, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei image_आकार, स्थिर GLvoid *तथ्य)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)IntGetProcAddress("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, width, format, image_आकार, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)IntGetProcAddress("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    अटल व्योम CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)IntGetProcAddress("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum mode, स्थिर GLint *first, स्थिर GLsizei *गिनती_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)IntGetProcAddress("glMultiDrawArrays");
        MultiDrawArrays(mode, first, गिनती_2, drawcount);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum mode, स्थिर GLsizei *गिनती_2, GLenum type, स्थिर GLvoid* स्थिर *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)IntGetProcAddress("glMultiDrawElements");
        MultiDrawElements(mode, गिनती_2, type, indices, drawcount);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PointParameterf(GLenum pname, GLfloat param)
    {
        PointParameterf = (PFNPOINTPARAMETERFPROC)IntGetProcAddress("glPointParameterf");
        PointParameterf(pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PointParameterfv(GLenum pname, स्थिर GLfloat *params)
    {
        PointParameterfv = (PFNPOINTPARAMETERFVPROC)IntGetProcAddress("glPointParameterfv");
        PointParameterfv(pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PointParameteri(GLenum pname, GLint param)
    {
        PointParameteri = (PFNPOINTPARAMETERIPROC)IntGetProcAddress("glPointParameteri");
        PointParameteri(pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_PointParameteriv(GLenum pname, स्थिर GLint *params)
    {
        PointParameteriv = (PFNPOINTPARAMETERIVPROC)IntGetProcAddress("glPointParameteriv");
        PointParameteriv(pname, params);
    }

    // Extension: 1.5

    अटल व्योम CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)IntGetProcAddress("glGenQueries");
        GenQueries(n, ids);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, स्थिर GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)IntGetProcAddress("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)IntGetProcAddress("glIsQuery");
        वापस IsQuery(id);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)IntGetProcAddress("glBeginQuery");
        BeginQuery(target, id);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_EndQuery(GLenum target)
    {
        EndQuery = (PFNENDQUERYPROC)IntGetProcAddress("glEndQuery");
        EndQuery(target);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)IntGetProcAddress("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)IntGetProcAddress("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)IntGetProcAddress("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Bind_बफर(GLenum target, GLuint बफर)
    {
        Bind_बफर = (PFNBINDBUFFERPROC)IntGetProcAddress("glBindBuffer");
        Bind_बफर(target, बफर);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, स्थिर GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)IntGetProcAddress("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)IntGetProcAddress("glGenBuffers");
        GenBuffers(n, buffers);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_Is_बफर(GLuint बफर)
    {
        Is_बफर = (PFNISBUFFERPROC)IntGetProcAddress("glIsBuffer");
        वापस Is_बफर(बफर);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_बफर_तथ्य(GLenum target, GLsizeiptr size, स्थिर GLvoid *तथ्य, GLenum usage)
    {
        बफर_तथ्य = (PFNBUFFERDATAPROC)IntGetProcAddress("glBufferData");
        बफर_तथ्य(target, size, तथ्य, usage);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_बफर_Sub_तथ्य(GLenum target, GLintptr offset, GLsizeiptr size, स्थिर GLvoid *तथ्य)
    {
        बफर_Sub_तथ्य = (PFNBUFFERSUBDATAPROC)IntGetProcAddress("glBufferSubData");
        बफर_Sub_तथ्य(target, offset, size, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Get_बफर_Sub_तथ्य(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *तथ्य)
    {
        Get_बफर_Sub_तथ्य = (PFNGETBUFFERSUBDATAPROC)IntGetProcAddress("glGetBufferSubData");
        Get_बफर_Sub_तथ्य(target, offset, size, तथ्य);
    }

    अटल GLvoid* CODEGEN_FUNCPTR Switch_Map_बफर(GLenum target, GLenum access)
    {
        Map_बफर = (PFNMAPBUFFERPROC)IntGetProcAddress("glMapBuffer");
        वापस Map_बफर(target, access);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_Unmap_बफर(GLenum target)
    {
        Unmap_बफर = (PFNUNMAPBUFFERPROC)IntGetProcAddress("glUnmapBuffer");
        वापस Unmap_बफर(target);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Get_बफर_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_बफर_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetBufferParameteriv");
        Get_बफर_Parameteriv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Get_बफर_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_बफर_Pointerv = (PFNGETBUFFERPOINTERVPROC)IntGetProcAddress("glGetBufferPointerv");
        Get_बफर_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    अटल व्योम CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)IntGetProcAddress("glBlendEquationSeparate");
        BlendEquationSeparate(modeRGB, modeAlpha);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, स्थिर GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)IntGetProcAddress("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)IntGetProcAddress("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)IntGetProcAddress("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, mask);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_StencilMaskSeparate(GLenum face, GLuint mask)
    {
        StencilMaskSeparate = (PFNSTENCILMASKSEPARATEPROC)IntGetProcAddress("glStencilMaskSeparate");
        StencilMaskSeparate(face, mask);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_AttachShader(GLuint program, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)IntGetProcAddress("glAttachShader");
        AttachShader(program, shader);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint program, GLuint सूचकांक, स्थिर GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)IntGetProcAddress("glBindAttribLocation");
        BindAttribLocation(program, सूचकांक, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)IntGetProcAddress("glCompileShader");
        CompileShader(shader);
    }

    अटल GLuint CODEGEN_FUNCPTR Switch_CreateProgram()
    {
        CreateProgram = (PFNCREATEPROGRAMPROC)IntGetProcAddress("glCreateProgram");
        वापस CreateProgram();
    }

    अटल GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)IntGetProcAddress("glCreateShader");
        वापस CreateShader(type);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteProgram(GLuint program)
    {
        DeleteProgram = (PFNDELETEPROGRAMPROC)IntGetProcAddress("glDeleteProgram");
        DeleteProgram(program);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)IntGetProcAddress("glDeleteShader");
        DeleteShader(shader);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DetachShader(GLuint program, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)IntGetProcAddress("glDetachShader");
        DetachShader(program, shader);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DisableVertexAttribArray(GLuint सूचकांक)
    {
        DisableVertexAttribArray = (PFNDISABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glDisableVertexAttribArray");
        DisableVertexAttribArray(सूचकांक);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_EnableVertexAttribArray(GLuint सूचकांक)
    {
        EnableVertexAttribArray = (PFNENABLEVERTEXATTRIBARRAYPROC)IntGetProcAddress("glEnableVertexAttribArray");
        EnableVertexAttribArray(सूचकांक);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetActiveAttrib(GLuint program, GLuint सूचकांक, GLsizei buf_आकार, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveAttrib = (PFNGETACTIVEATTRIBPROC)IntGetProcAddress("glGetActiveAttrib");
        GetActiveAttrib(program, सूचकांक, buf_आकार, length, size, type, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetActiveUniform(GLuint program, GLuint सूचकांक, GLsizei buf_आकार, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        GetActiveUniform = (PFNGETACTIVEUNIFORMPROC)IntGetProcAddress("glGetActiveUniform");
        GetActiveUniform(program, सूचकांक, buf_आकार, length, size, type, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint program, GLsizei max_गिनती, GLsizei *गिनती_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)IntGetProcAddress("glGetAttachedShaders");
        GetAttachedShaders(program, max_गिनती, गिनती_2, obj);
    }

    अटल GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint program, स्थिर GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)IntGetProcAddress("glGetAttribLocation");
        वापस GetAttribLocation(program, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint program, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)IntGetProcAddress("glGetProgramiv");
        GetProgramiv(program, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetProgramInfoLog(GLuint program, GLsizei buf_आकार, GLsizei *length, GLchar *infoLog)
    {
        GetProgramInfoLog = (PFNGETPROGRAMINFOLOGPROC)IntGetProcAddress("glGetProgramInfoLog");
        GetProgramInfoLog(program, buf_आकार, length, infoLog);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)IntGetProcAddress("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_आकार, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)IntGetProcAddress("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_आकार, length, infoLog);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetShaderSource(GLuint shader, GLsizei buf_आकार, GLsizei *length, GLchar *source)
    {
        GetShaderSource = (PFNGETSHADERSOURCEPROC)IntGetProcAddress("glGetShaderSource");
        GetShaderSource(shader, buf_आकार, length, source);
    }

    अटल GLint CODEGEN_FUNCPTR Switch_GetUniformLocation(GLuint program, स्थिर GLchar *name)
    {
        GetUniformLocation = (PFNGETUNIFORMLOCATIONPROC)IntGetProcAddress("glGetUniformLocation");
        वापस GetUniformLocation(program, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint program, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)IntGetProcAddress("glGetUniformfv");
        GetUniformfv(program, location, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint program, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)IntGetProcAddress("glGetUniformiv");
        GetUniformiv(program, location, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetVertexAttribdv(GLuint सूचकांक, GLenum pname, GLdouble *params)
    {
        GetVertexAttribdv = (PFNGETVERTEXATTRIBDVPROC)IntGetProcAddress("glGetVertexAttribdv");
        GetVertexAttribdv(सूचकांक, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetVertexAttribfv(GLuint सूचकांक, GLenum pname, GLfloat *params)
    {
        GetVertexAttribfv = (PFNGETVERTEXATTRIBFVPROC)IntGetProcAddress("glGetVertexAttribfv");
        GetVertexAttribfv(सूचकांक, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetVertexAttribiv(GLuint सूचकांक, GLenum pname, GLint *params)
    {
        GetVertexAttribiv = (PFNGETVERTEXATTRIBIVPROC)IntGetProcAddress("glGetVertexAttribiv");
        GetVertexAttribiv(सूचकांक, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetVertexAttribPointerv(GLuint सूचकांक, GLenum pname, GLvoid* *pointer)
    {
        GetVertexAttribPointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)IntGetProcAddress("glGetVertexAttribPointerv");
        GetVertexAttribPointerv(सूचकांक, pname, pointer);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsProgram(GLuint program)
    {
        IsProgram = (PFNISPROGRAMPROC)IntGetProcAddress("glIsProgram");
        वापस IsProgram(program);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)IntGetProcAddress("glIsShader");
        वापस IsShader(shader);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_LinkProgram(GLuint program)
    {
        LinkProgram = (PFNLINKPROGRAMPROC)IntGetProcAddress("glLinkProgram");
        LinkProgram(program);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ShaderSource(GLuint shader, GLsizei गिनती_2, स्थिर GLchar* स्थिर *string, स्थिर GLint *length)
    {
        ShaderSource = (PFNSHADERSOURCEPROC)IntGetProcAddress("glShaderSource");
        ShaderSource(shader, गिनती_2, string, length);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UseProgram(GLuint program)
    {
        UseProgram = (PFNUSEPROGRAMPROC)IntGetProcAddress("glUseProgram");
        UseProgram(program);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)IntGetProcAddress("glUniform1f");
        Uniform1f(location, v0);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)IntGetProcAddress("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)IntGetProcAddress("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)IntGetProcAddress("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)IntGetProcAddress("glUniform1i");
        Uniform1i(location, v0);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)IntGetProcAddress("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)IntGetProcAddress("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)IntGetProcAddress("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei गिनती_2, स्थिर GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)IntGetProcAddress("glUniform1fv");
        Uniform1fv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei गिनती_2, स्थिर GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)IntGetProcAddress("glUniform2fv");
        Uniform2fv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei गिनती_2, स्थिर GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)IntGetProcAddress("glUniform3fv");
        Uniform3fv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei गिनती_2, स्थिर GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)IntGetProcAddress("glUniform4fv");
        Uniform4fv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei गिनती_2, स्थिर GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)IntGetProcAddress("glUniform1iv");
        Uniform1iv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei गिनती_2, स्थिर GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)IntGetProcAddress("glUniform2iv");
        Uniform2iv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei गिनती_2, स्थिर GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)IntGetProcAddress("glUniform3iv");
        Uniform3iv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei गिनती_2, स्थिर GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)IntGetProcAddress("glUniform4iv");
        Uniform4iv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix2fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix2fv = (PFNUNIFORMMATRIX2FVPROC)IntGetProcAddress("glUniformMatrix2fv");
        UniformMatrix2fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix3fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix3fv = (PFNUNIFORMMATRIX3FVPROC)IntGetProcAddress("glUniformMatrix3fv");
        UniformMatrix3fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix4fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix4fv = (PFNUNIFORMMATRIX4FVPROC)IntGetProcAddress("glUniformMatrix4fv");
        UniformMatrix4fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ValidateProgram(GLuint program)
    {
        ValidateProgram = (PFNVALIDATEPROGRAMPROC)IntGetProcAddress("glValidateProgram");
        ValidateProgram(program);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribPointer(GLuint सूचकांक, GLint size, GLenum type, GLboolean normalized, GLsizei stride, स्थिर GLvoid *pointer)
    {
        VertexAttribPointer = (PFNVERTEXATTRIBPOINTERPROC)IntGetProcAddress("glVertexAttribPointer");
        VertexAttribPointer(सूचकांक, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix2x3fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)IntGetProcAddress("glUniformMatrix2x3fv");
        UniformMatrix2x3fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix3x2fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)IntGetProcAddress("glUniformMatrix3x2fv");
        UniformMatrix3x2fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix2x4fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)IntGetProcAddress("glUniformMatrix2x4fv");
        UniformMatrix2x4fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix4x2fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)IntGetProcAddress("glUniformMatrix4x2fv");
        UniformMatrix4x2fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix3x4fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)IntGetProcAddress("glUniformMatrix3x4fv");
        UniformMatrix3x4fv(location, गिनती_2, transpose, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformMatrix4x3fv(GLint location, GLsizei गिनती_2, GLboolean transpose, स्थिर GLfloat *value)
    {
        UniformMatrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)IntGetProcAddress("glUniformMatrix4x3fv");
        UniformMatrix4x3fv(location, गिनती_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    अटल व्योम CODEGEN_FUNCPTR Switch_BindVertexArray(GLuint ren_array)
    {
        BindVertexArray = (PFNBINDVERTEXARRAYPROC)IntGetProcAddress("glBindVertexArray");
        BindVertexArray(ren_array);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteVertexArrays(GLsizei n, स्थिर GLuint *arrays)
    {
        DeleteVertexArrays = (PFNDELETEVERTEXARRAYSPROC)IntGetProcAddress("glDeleteVertexArrays");
        DeleteVertexArrays(n, arrays);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GenVertexArrays(GLsizei n, GLuint *arrays)
    {
        GenVertexArrays = (PFNGENVERTEXARRAYSPROC)IntGetProcAddress("glGenVertexArrays");
        GenVertexArrays(n, arrays);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsVertexArray(GLuint ren_array)
    {
        IsVertexArray = (PFNISVERTEXARRAYPROC)IntGetProcAddress("glIsVertexArray");
        वापस IsVertexArray(ren_array);
    }

    // Extension: ARB_map_buffer_range

    अटल GLvoid* CODEGEN_FUNCPTR Switch_Map_बफर_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_बफर_Range = (PFNMAPBUFFERRANGEPROC)IntGetProcAddress("glMapBufferRange");
        वापस Map_बफर_Range(target, offset, length, access);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Flush_Mapped_बफर_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_बफर_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)IntGetProcAddress("glFlushMappedBufferRange");
        Flush_Mapped_बफर_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)IntGetProcAddress("glIsRenderbuffer");
        वापस IsRenderbuffer(renderbuffer);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)IntGetProcAddress("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, स्थिर GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)IntGetProcAddress("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)IntGetProcAddress("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)IntGetProcAddress("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, width, height);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)IntGetProcAddress("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)IntGetProcAddress("glIsFramebuffer");
        वापस IsFramebuffer(framebuffer);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)IntGetProcAddress("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, स्थिर GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)IntGetProcAddress("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)IntGetProcAddress("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    अटल GLenum CODEGEN_FUNCPTR Switch_CheckFramebufferStatus(GLenum target)
    {
        CheckFramebufferStatus = (PFNCHECKFRAMEBUFFERSTATUSPROC)IntGetProcAddress("glCheckFramebufferStatus");
        वापस CheckFramebufferStatus(target);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)IntGetProcAddress("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)IntGetProcAddress("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)IntGetProcAddress("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)IntGetProcAddress("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)IntGetProcAddress("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)IntGetProcAddress("glGenerateMipmap");
        GenerateMipmap(target);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)IntGetProcAddress("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)IntGetProcAddress("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, width, height);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)IntGetProcAddress("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    अटल व्योम CODEGEN_FUNCPTR Switch_ColorMaski(GLuint सूचकांक, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)IntGetProcAddress("glColorMaski");
        ColorMaski(सूचकांक, r, g, b, a);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint सूचकांक, GLboolean *तथ्य)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)IntGetProcAddress("glGetBooleani_v");
        GetBooleani_v(target, सूचकांक, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint सूचकांक, GLint *तथ्य)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)IntGetProcAddress("glGetIntegeri_v");
        GetIntegeri_v(target, सूचकांक, तथ्य);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint सूचकांक)
    {
        Enablei = (PFNENABLEIPROC)IntGetProcAddress("glEnablei");
        Enablei(target, सूचकांक);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint सूचकांक)
    {
        Disablei = (PFNDISABLEIPROC)IntGetProcAddress("glDisablei");
        Disablei(target, सूचकांक);
    }

    अटल GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint सूचकांक)
    {
        IsEnabledi = (PFNISENABLEDIPROC)IntGetProcAddress("glIsEnabledi");
        वापस IsEnabledi(target, सूचकांक);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitiveMode)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)IntGetProcAddress("glBeginTransformFeedback");
        BeginTransformFeedback(primitiveMode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_EndTransformFeedback()
    {
        EndTransformFeedback = (PFNENDTRANSFORMFEEDBACKPROC)IntGetProcAddress("glEndTransformFeedback");
        EndTransformFeedback();
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Bind_बफर_Range(GLenum target, GLuint सूचकांक, GLuint बफर, GLintptr offset, GLsizeiptr size)
    {
        Bind_बफर_Range = (PFNBINDBUFFERRANGEPROC)IntGetProcAddress("glBindBufferRange");
        Bind_बफर_Range(target, सूचकांक, बफर, offset, size);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Bind_बफर_Base(GLenum target, GLuint सूचकांक, GLuint बफर)
    {
        Bind_बफर_Base = (PFNBINDBUFFERBASEPROC)IntGetProcAddress("glBindBufferBase");
        Bind_बफर_Base(target, सूचकांक, बफर);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint program, GLsizei गिनती_2, स्थिर GLchar* स्थिर *varyings, GLenum बफर_Mode)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)IntGetProcAddress("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(program, गिनती_2, varyings, बफर_Mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint program, GLuint सूचकांक, GLsizei buf_आकार, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)IntGetProcAddress("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(program, सूचकांक, buf_आकार, length, size, type, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)IntGetProcAddress("glClampColor");
        ClampColor(target, clamp);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum mode)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)IntGetProcAddress("glBeginConditionalRender");
        BeginConditionalRender(id, mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_EndConditionalRender()
    {
        EndConditionalRender = (PFNENDCONDITIONALRENDERPROC)IntGetProcAddress("glEndConditionalRender");
        EndConditionalRender();
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribIPointer(GLuint सूचकांक, GLint size, GLenum type, GLsizei stride, स्थिर GLvoid *pointer)
    {
        VertexAttribIPointer = (PFNVERTEXATTRIBIPOINTERPROC)IntGetProcAddress("glVertexAttribIPointer");
        VertexAttribIPointer(सूचकांक, size, type, stride, pointer);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetVertexAttribIiv(GLuint सूचकांक, GLenum pname, GLint *params)
    {
        GetVertexAttribIiv = (PFNGETVERTEXATTRIBIIVPROC)IntGetProcAddress("glGetVertexAttribIiv");
        GetVertexAttribIiv(सूचकांक, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetVertexAttribIuiv(GLuint सूचकांक, GLenum pname, GLuint *params)
    {
        GetVertexAttribIuiv = (PFNGETVERTEXATTRIBIUIVPROC)IntGetProcAddress("glGetVertexAttribIuiv");
        GetVertexAttribIuiv(सूचकांक, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI1i(GLuint सूचकांक, GLint x)
    {
        VertexAttribI1i = (PFNVERTEXATTRIBI1IPROC)IntGetProcAddress("glVertexAttribI1i");
        VertexAttribI1i(सूचकांक, x);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI2i(GLuint सूचकांक, GLint x, GLint y)
    {
        VertexAttribI2i = (PFNVERTEXATTRIBI2IPROC)IntGetProcAddress("glVertexAttribI2i");
        VertexAttribI2i(सूचकांक, x, y);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI3i(GLuint सूचकांक, GLint x, GLint y, GLint z)
    {
        VertexAttribI3i = (PFNVERTEXATTRIBI3IPROC)IntGetProcAddress("glVertexAttribI3i");
        VertexAttribI3i(सूचकांक, x, y, z);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4i(GLuint सूचकांक, GLint x, GLint y, GLint z, GLint w)
    {
        VertexAttribI4i = (PFNVERTEXATTRIBI4IPROC)IntGetProcAddress("glVertexAttribI4i");
        VertexAttribI4i(सूचकांक, x, y, z, w);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI1ui(GLuint सूचकांक, GLuint x)
    {
        VertexAttribI1ui = (PFNVERTEXATTRIBI1UIPROC)IntGetProcAddress("glVertexAttribI1ui");
        VertexAttribI1ui(सूचकांक, x);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI2ui(GLuint सूचकांक, GLuint x, GLuint y)
    {
        VertexAttribI2ui = (PFNVERTEXATTRIBI2UIPROC)IntGetProcAddress("glVertexAttribI2ui");
        VertexAttribI2ui(सूचकांक, x, y);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI3ui(GLuint सूचकांक, GLuint x, GLuint y, GLuint z)
    {
        VertexAttribI3ui = (PFNVERTEXATTRIBI3UIPROC)IntGetProcAddress("glVertexAttribI3ui");
        VertexAttribI3ui(सूचकांक, x, y, z);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4ui(GLuint सूचकांक, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        VertexAttribI4ui = (PFNVERTEXATTRIBI4UIPROC)IntGetProcAddress("glVertexAttribI4ui");
        VertexAttribI4ui(सूचकांक, x, y, z, w);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI1iv(GLuint सूचकांक, स्थिर GLint *v)
    {
        VertexAttribI1iv = (PFNVERTEXATTRIBI1IVPROC)IntGetProcAddress("glVertexAttribI1iv");
        VertexAttribI1iv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI2iv(GLuint सूचकांक, स्थिर GLint *v)
    {
        VertexAttribI2iv = (PFNVERTEXATTRIBI2IVPROC)IntGetProcAddress("glVertexAttribI2iv");
        VertexAttribI2iv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI3iv(GLuint सूचकांक, स्थिर GLint *v)
    {
        VertexAttribI3iv = (PFNVERTEXATTRIBI3IVPROC)IntGetProcAddress("glVertexAttribI3iv");
        VertexAttribI3iv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4iv(GLuint सूचकांक, स्थिर GLint *v)
    {
        VertexAttribI4iv = (PFNVERTEXATTRIBI4IVPROC)IntGetProcAddress("glVertexAttribI4iv");
        VertexAttribI4iv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI1uiv(GLuint सूचकांक, स्थिर GLuint *v)
    {
        VertexAttribI1uiv = (PFNVERTEXATTRIBI1UIVPROC)IntGetProcAddress("glVertexAttribI1uiv");
        VertexAttribI1uiv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI2uiv(GLuint सूचकांक, स्थिर GLuint *v)
    {
        VertexAttribI2uiv = (PFNVERTEXATTRIBI2UIVPROC)IntGetProcAddress("glVertexAttribI2uiv");
        VertexAttribI2uiv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI3uiv(GLuint सूचकांक, स्थिर GLuint *v)
    {
        VertexAttribI3uiv = (PFNVERTEXATTRIBI3UIVPROC)IntGetProcAddress("glVertexAttribI3uiv");
        VertexAttribI3uiv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4uiv(GLuint सूचकांक, स्थिर GLuint *v)
    {
        VertexAttribI4uiv = (PFNVERTEXATTRIBI4UIVPROC)IntGetProcAddress("glVertexAttribI4uiv");
        VertexAttribI4uiv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4bv(GLuint सूचकांक, स्थिर GLbyte *v)
    {
        VertexAttribI4bv = (PFNVERTEXATTRIBI4BVPROC)IntGetProcAddress("glVertexAttribI4bv");
        VertexAttribI4bv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4sv(GLuint सूचकांक, स्थिर GLshort *v)
    {
        VertexAttribI4sv = (PFNVERTEXATTRIBI4SVPROC)IntGetProcAddress("glVertexAttribI4sv");
        VertexAttribI4sv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4ubv(GLuint सूचकांक, स्थिर GLubyte *v)
    {
        VertexAttribI4ubv = (PFNVERTEXATTRIBI4UBVPROC)IntGetProcAddress("glVertexAttribI4ubv");
        VertexAttribI4ubv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexAttribI4usv(GLuint सूचकांक, स्थिर GLushort *v)
    {
        VertexAttribI4usv = (PFNVERTEXATTRIBI4USVPROC)IntGetProcAddress("glVertexAttribI4usv");
        VertexAttribI4usv(सूचकांक, v);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint program, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)IntGetProcAddress("glGetUniformuiv");
        GetUniformuiv(program, location, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Bind_Frag_तथ्य_Location(GLuint program, GLuint color, स्थिर GLchar *name)
    {
        Bind_Frag_तथ्य_Location = (PFNBINDFRAGDATALOCATIONPROC)IntGetProcAddress("glBindFragDataLocation");
        Bind_Frag_तथ्य_Location(program, color, name);
    }

    अटल GLint CODEGEN_FUNCPTR Switch_Get_Frag_तथ्य_Location(GLuint program, स्थिर GLchar *name)
    {
        Get_Frag_तथ्य_Location = (PFNGETFRAGDATALOCATIONPROC)IntGetProcAddress("glGetFragDataLocation");
        वापस Get_Frag_तथ्य_Location(program, name);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)IntGetProcAddress("glUniform1ui");
        Uniform1ui(location, v0);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)IntGetProcAddress("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)IntGetProcAddress("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)IntGetProcAddress("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei गिनती_2, स्थिर GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)IntGetProcAddress("glUniform1uiv");
        Uniform1uiv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei गिनती_2, स्थिर GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)IntGetProcAddress("glUniform2uiv");
        Uniform2uiv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei गिनती_2, स्थिर GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)IntGetProcAddress("glUniform3uiv");
        Uniform3uiv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei गिनती_2, स्थिर GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)IntGetProcAddress("glUniform4uiv");
        Uniform4uiv(location, गिनती_2, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, स्थिर GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)IntGetProcAddress("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, स्थिर GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)IntGetProcAddress("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)IntGetProcAddress("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)IntGetProcAddress("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum बफर, GLint drawbuffer, स्थिर GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)IntGetProcAddress("glClearBufferiv");
        ClearBufferiv(बफर, drawbuffer, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum बफर, GLint drawbuffer, स्थिर GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)IntGetProcAddress("glClearBufferuiv");
        ClearBufferuiv(बफर, drawbuffer, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum बफर, GLint drawbuffer, स्थिर GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)IntGetProcAddress("glClearBufferfv");
        ClearBufferfv(बफर, drawbuffer, value);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum बफर, GLint drawbuffer, GLfloat depth, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)IntGetProcAddress("glClearBufferfi");
        ClearBufferfi(बफर, drawbuffer, depth, stencil);
    }

    अटल स्थिर GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint सूचकांक)
    {
        GetStringi = (PFNGETSTRINGIPROC)IntGetProcAddress("glGetStringi");
        वापस GetStringi(name, सूचकांक);
    }

    // Extension: ARB_uniform_buffer_object

    अटल व्योम CODEGEN_FUNCPTR Switch_GetUniformIndices(GLuint program, GLsizei uniform_गिनती, स्थिर GLchar* स्थिर *uniformNames, GLuint *uniformIndices)
    {
        GetUniformIndices = (PFNGETUNIFORMINDICESPROC)IntGetProcAddress("glGetUniformIndices");
        GetUniformIndices(program, uniform_गिनती, uniformNames, uniformIndices);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetActiveUniformsiv(GLuint program, GLsizei uniform_गिनती, स्थिर GLuint *uniformIndices, GLenum pname, GLint *params)
    {
        GetActiveUniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)IntGetProcAddress("glGetActiveUniformsiv");
        GetActiveUniformsiv(program, uniform_गिनती, uniformIndices, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetActiveUniformName(GLuint program, GLuint uniform_सूचकांक, GLsizei buf_आकार, GLsizei *length, GLchar *uniformName)
    {
        GetActiveUniformName = (PFNGETACTIVEUNIFORMNAMEPROC)IntGetProcAddress("glGetActiveUniformName");
        GetActiveUniformName(program, uniform_सूचकांक, buf_आकार, length, uniformName);
    }

    अटल GLuint CODEGEN_FUNCPTR Switch_Get_Uniform_Block_सूचकांक(GLuint program, स्थिर GLchar *uniformBlockName)
    {
        Get_Uniform_Block_सूचकांक = (PFNGETUNIFORMBLOCKINDEXPROC)IntGetProcAddress("glGetUniformBlockIndex");
        वापस Get_Uniform_Block_सूचकांक(program, uniformBlockName);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetActiveUniformBlockiv(GLuint program, GLuint uniform_Block_सूचकांक, GLenum pname, GLint *params)
    {
        GetActiveUniformBlockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)IntGetProcAddress("glGetActiveUniformBlockiv");
        GetActiveUniformBlockiv(program, uniform_Block_सूचकांक, pname, params);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_GetActiveUniformBlockName(GLuint program, GLuint uniform_Block_सूचकांक, GLsizei buf_आकार, GLsizei *length, GLchar *uniformBlockName)
    {
        GetActiveUniformBlockName = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)IntGetProcAddress("glGetActiveUniformBlockName");
        GetActiveUniformBlockName(program, uniform_Block_सूचकांक, buf_आकार, length, uniformBlockName);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_UniformBlockBinding(GLuint program, GLuint uniform_Block_सूचकांक, GLuint uniformBlockBinding)
    {
        UniformBlockBinding = (PFNUNIFORMBLOCKBINDINGPROC)IntGetProcAddress("glUniformBlockBinding");
        UniformBlockBinding(program, uniform_Block_सूचकांक, uniformBlockBinding);
    }

    // Extension: ARB_copy_buffer

    अटल व्योम CODEGEN_FUNCPTR Switch_Copy_बफर_Sub_तथ्य(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
    {
        Copy_बफर_Sub_तथ्य = (PFNCOPYBUFFERSUBDATAPROC)IntGetProcAddress("glCopyBufferSubData");
        Copy_बफर_Sub_तथ्य(readTarget, writeTarget, readOffset, writeOffset, size);
    }

    // Extension: 3.1

    अटल व्योम CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum mode, GLint first, GLsizei गिनती_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)IntGetProcAddress("glDrawArraysInstanced");
        DrawArraysInstanced(mode, first, गिनती_2, instancecount);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum mode, GLsizei गिनती_2, GLenum type, स्थिर GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)IntGetProcAddress("glDrawElementsInstanced");
        DrawElementsInstanced(mode, गिनती_2, type, indices, instancecount);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Tex_बफर(GLenum target, GLenum internalformat, GLuint बफर)
    {
        Tex_बफर = (PFNTEXBUFFERPROC)IntGetProcAddress("glTexBuffer");
        Tex_बफर(target, internalformat, बफर);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Primitive_Restart_सूचकांक(GLuint सूचकांक)
    {
        Primitive_Restart_सूचकांक = (PFNPRIMITIVERESTARTINDEXPROC)IntGetProcAddress("glPrimitiveRestartIndex");
        Primitive_Restart_सूचकांक(सूचकांक);
    }

    // Legacy

    अटल व्योम CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)IntGetProcAddress("glEnableClientState");
        EnableClientState(cap);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)IntGetProcAddress("glDisableClientState");
        DisableClientState(cap);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_VertexPointer(GLint size, GLenum type, GLsizei stride, स्थिर GLvoid *ptr)
    {
        VertexPointer = (PFNVERTEXPOINTERPROC)IntGetProcAddress("glVertexPointer");
        VertexPointer(size, type, stride, ptr);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_NormalPointer(GLenum type, GLsizei stride, स्थिर GLvoid *ptr)
    {
        NormalPointer = (PFNNORMALPOINTERPROC)IntGetProcAddress("glNormalPointer");
        NormalPointer(type, stride, ptr);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_ColorPointer(GLint size, GLenum type, GLsizei stride, स्थिर GLvoid *ptr)
    {
        ColorPointer = (PFNCOLORPOINTERPROC)IntGetProcAddress("glColorPointer");
        ColorPointer(size, type, stride, ptr);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexCoordPointer(GLint size, GLenum type, GLsizei stride, स्थिर GLvoid *ptr)
    {
        TexCoordPointer = (PFNTEXCOORDPOINTERPROC)IntGetProcAddress("glTexCoordPointer");
        TexCoordPointer(size, type, stride, ptr);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)IntGetProcAddress("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_MatrixMode(GLenum mode)
    {
        MatrixMode = (PFNMATRIXMODEPROC)IntGetProcAddress("glMatrixMode");
        MatrixMode(mode);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_LoadIdentity(व्योम)
    {
        LoadIdentity = (PFNLOADIDENTITYPROC)IntGetProcAddress("glLoadIdentity");
        LoadIdentity();
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Ortho(GLdouble बाँए, GLdouble दाँए, GLdouble bottom, GLdouble शीर्ष_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)IntGetProcAddress("glOrtho");
        Ortho(बाँए, दाँए, bottom, शीर्ष_2, near_val, far_val);
    }

    अटल व्योम CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)IntGetProcAddress("glColor3d");
        Color3d(red, green, blue);
    }

    काष्ठा InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            पंक्ति_Width = Switch_पंक्ति_Width;
            Point_आकार = Switch_Point_आकार;
            PolygonMode = Switch_PolygonMode;
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
            Bind_बफर = Switch_Bind_बफर;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_बफर = Switch_Is_बफर;
            बफर_तथ्य = Switch_बफर_तथ्य;
            बफर_Sub_तथ्य = Switch_बफर_Sub_तथ्य;
            Get_बफर_Sub_तथ्य = Switch_Get_बफर_Sub_तथ्य;
            Map_बफर = Switch_Map_बफर;
            Unmap_बफर = Switch_Unmap_बफर;
            Get_बफर_Parameteriv = Switch_Get_बफर_Parameteriv;
            Get_बफर_Pointerv = Switch_Get_बफर_Pointerv;

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
            Bind_बफर_Range = Switch_Bind_बफर_Range;
            Bind_बफर_Base = Switch_Bind_बफर_Base;
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
            Bind_Frag_तथ्य_Location = Switch_Bind_Frag_तथ्य_Location;
            Get_Frag_तथ्य_Location = Switch_Get_Frag_तथ्य_Location;
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
            Get_Uniform_Block_सूचकांक = Switch_Get_Uniform_Block_सूचकांक;
            GetActiveUniformBlockiv = Switch_GetActiveUniformBlockiv;
            GetActiveUniformBlockName = Switch_GetActiveUniformBlockName;
            UniformBlockBinding = Switch_UniformBlockBinding;

            // Extension: ARB_copy_buffer
            Copy_बफर_Sub_तथ्य = Switch_Copy_बफर_Sub_तथ्य;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_बफर = Switch_Tex_बफर;
            Primitive_Restart_सूचकांक = Switch_Primitive_Restart_सूचकांक;

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
