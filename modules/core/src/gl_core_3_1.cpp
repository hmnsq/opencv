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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "gl_core_3_1.hpp"

#నిర్వచితమైతే HAVE_OPENGL

    #నిర్వచితమైతే __APPLE__
        #అన్తర్భావ <dlfcn.h>

        స్థిర శూన్య* Apple_GLGet_Proc_చిరునామా (స్థిరరాశి అక్షరం* name)
        {
            స్థిర తార్కిక initialized = అబద్ధం;
            స్థిర శూన్య * handle = ఖాళీ;
            ఒకవేళ (!handle)
            {
                ఒకవేళ (!initialized)
                {
                    initialized = నిజం;
                    స్థిరరాశి అక్షరం * స్థిరరాశి మార్గం = "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL";

                    handle = dlopen(మార్గం, RTLD_LAZY | RTLD_GLOBAL);
                }
                ఒకవేళ (!handle)
                    తిరిగి ఖాళీ;
            }
            తిరిగి dlsym(handle, name);
        }
    #అయితే_ముగింపు // __APPLE__

    #ఒకవేళ defined(__sgi) || defined (__sun)
        #అన్తర్భావ <dlfcn.h>
        #అన్తర్భావ <stdio.h>

        స్థిర శూన్య* Sun_Get_Proc_చిరునామా (స్థిరరాశి అక్షరం* name)
        {
            రకనిర్వచనం శూన్య* (func_t*)(స్థిరరాశి GLubyte*);

            స్థిర శూన్య* h = 0;
            స్థిర func_t gpa = 0;

            ఒకవేళ (!h)
            {
                h = dlopen(ఖాళీ, RTLD_LAZY | RTLD_LOCAL);
                ఒకవేళ (!h)
                    తిరిగి 0;
                gpa = (func_t) dlsym(h, "glXGetProcAddress");
            }

            తిరిగి gpa ? gpa((స్థిరరాశి GLubyte*) name) : dlsym(h, name);
        }
    #అయితే_ముగింపు // __sgi || __sun

    #ఒకవేళ defined(_WIN32)
        #నిర్వచితమైతే _MSC_VER
            #సూచన warning(disable: 4055)
            #సూచన warning(disable: 4054)
        #అయితే_ముగింపు

        స్థిర పూర్ణాంకం Test_సూచిక(స్థిరరాశి PROC pTest)
        {
            ఒకవేళ(!pTest)
                తిరిగి 0;

            ptrdiff_t iTest = (ptrdiff_t) pTest;

            ఒకవేళ (iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1)
                తిరిగి 0;

            తిరిగి 1;
        }

        స్థిర PROC Win_Get_Proc_చిరునామా(స్థిరరాశి అక్షరం* name)
        {
            PROC pFunc = wgl_Get_Proc_చిరునామా((LPCSTR) name);
            ఒకవేళ (Test_సూచిక(pFunc))
                తిరిగి pFunc;

            HMODULE glMod = GetModuleHandleA("OpenGL32.dll");
            తిరిగి (PROC) Get_Proc_చిరునామా(glMod, (LPCSTR) name);
        }
    #అయితే_ముగింపు // _WIN32

    #ఒకవేళ defined(_WIN32)
        #నిర్వచించు CV_GL_GET_PROC_చిరునామా(name) Win_Get_Proc_చిరునామా(name)
    #లేదావేళ defined(__APPLE__)
        #నిర్వచించు CV_GL_GET_PROC_చిరునామా(name) Apple_GLGet_Proc_చిరునామా(name)
    #లేదావేళ defined(__sgi) || defined(__sun)
        #నిర్వచించు CV_GL_GET_PROC_చిరునామా(name) Sun_Get_Proc_చిరునామా(name)
    #లేకపోతే // GLX
        #అన్తర్భావ <GL/glx.h>

        #నిర్వచించు CV_GL_GET_PROC_చిరునామా(name) gl_XGet_Proc_చిరునామా_ARB((స్థిరరాశి GLubyte*) name)
    #అయితే_ముగింపు

    స్థిర శూన్య* Int_Get_Proc_చిరునామా(స్థిరరాశి అక్షరం* name)
    {
        శూన్య* func =  (శూన్య*) CV_GL_GET_PROC_చిరునామా(name);
        ఒకవేళ (!func)
        {
            CV_Error(cv::Error::OpenGlApiCallError, cv::format("Can't load OpenGL extension [%s]", name) );
        }
        తిరిగి func;
    }
#లేకపోతే
#ఒకవేళ defined(_MSC_VER)
    #సూచన warning(disable : 4702)  // unreachable code
#అయితే_ముగింపు
    స్థిర శూన్య* Int_Get_Proc_చిరునామా(స్థిరరాశి అక్షరం*)
    {
        CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support");
    }
#అయితే_ముగింపు

నామాకాశ gl
{
    //////////////////////////////////////////////
    // Function pointer types

    // Extension: 1.1
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCULLFACEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFRONTFACEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNHINTPROC)(GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNLINEWIDTHPROC)(GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOINTSIZEPROC)(GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOLYGONMODEPROC)(GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSCISSORPROC)(GLint , GLint , GLsizei , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXPARAMETERFPROC)(GLenum , GLenum , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXPARAMETERFVPROC)(GLenum , GLenum , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXPARAMETERIPROC)(GLenum , GLenum , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXPARAMETERIVPROC)(GLenum , GLenum , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLint , GLenum , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXIMAGE2DPROC)(GLenum , GLint , GLint , GLsizei , GLsizei , GLint , GLenum , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWBUFFERPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARPROC)(GLbitfield );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARSTENCILPROC)(GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARDEPTHPROC)(GLdouble );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSTENCILMASKPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOLORMASKPROC)(GLboolean , GLboolean , GLboolean , GLboolean );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDEPTHMASKPROC)(GLboolean );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDISABLEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENABLEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFINISHPROC)();
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFLUSHPROC)();
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBLENDFUNCPROC)(GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNLOGICOPPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSTENCILFUNCPROC)(GLenum , GLint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSTENCILOPPROC)(GLenum , GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDEPTHFUNCPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPIXELSTOREFPROC)(GLenum , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPIXELSTOREIPROC)(GLenum , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNREADBUFFERPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNREADPIXELSPROC)(GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETBOOLEANVPROC)(GLenum , GLboolean *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETDOUBLEVPROC)(GLenum , GLdouble *);
    రకనిర్వచనం GLenum (CODEGEN_FUNCPTR *PFNGETERRORPROC)();
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETFLOATVPROC)(GLenum , GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETINTEGERVPROC)(GLenum , GLint *);
    రకనిర్వచనం స్థిరరాశి GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXIMAGEPROC)(GLenum , GLint , GLenum , GLenum , GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERFVPROC)(GLenum , GLenum , GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERFVPROC)(GLenum , GLint , GLenum , GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXLEVELPARAMETERIVPROC)(GLenum , GLint , GLenum , GLint *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDEPTHRANGEPROC)(GLdouble , GLdouble );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVIEWPORTPROC)(GLint , GLint , GLsizei , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWARRAYSPROC)(GLenum , GLint , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWELEMENTSPROC)(GLenum , GLsizei , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETPOINTERVPROC)(GLenum , GLvoid* *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOLYGONOFFSETPROC)(GLfloat , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOPYTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLint , GLint , GLsizei , GLsizei , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDTEXTUREPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETETEXTURESPROC)(GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENTEXTURESPROC)(GLsizei , GLuint *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISTEXTUREPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNINDEXUBPROC)(GLubyte );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNINDEXUBVPROC)(స్థిరరాశి GLubyte *);

    // Extension: 1.2
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBLENDCOLORPROC)(GLfloat , GLfloat , GLfloat , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBLENDEQUATIONPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWRANGEELEMENTSPROC)(GLenum , GLuint , GLuint , GLsizei , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLenum , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOPYTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLint , GLint , GLsizei , GLsizei );

    // Extension: 1.3
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNACTIVETEXTUREPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSAMPLECOVERAGEPROC)(GLfloat , GLboolean );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE3DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLsizei , GLint , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE2DPROC)(GLenum , GLint , GLenum , GLsizei , GLsizei , GLint , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXIMAGE1DPROC)(GLenum , GLint , GLenum , GLsizei , GLint , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum , GLint , GLint , GLint , GLint , GLsizei , GLsizei , GLsizei , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum , GLint , GLint , GLint , GLsizei , GLsizei , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum , GLint , GLint , GLsizei , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETCOMPRESSEDTEXIMAGEPROC)(GLenum , GLint , GLvoid *);

    // Extension: 1.4
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBLENDFUNCSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNMULTIDRAWARRAYSPROC)(GLenum , స్థిరరాశి GLint *, స్థిరరాశి GLsizei *, GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNMULTIDRAWELEMENTSPROC)(GLenum , స్థిరరాశి GLsizei *, GLenum , స్థిరరాశి GLvoid* స్థిరరాశి *, GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFPROC)(GLenum , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOINTPARAMETERFVPROC)(GLenum , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIPROC)(GLenum , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPOINTPARAMETERIVPROC)(GLenum , స్థిరరాశి GLint *);

    // Extension: 1.5
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENQUERIESPROC)(GLsizei , GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETEQUERIESPROC)(GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISQUERYPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBEGINQUERYPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENDQUERYPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETQUERYIVPROC)(GLenum , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTIVPROC)(GLuint , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETQUERYOBJECTUIVPROC)(GLuint , GLenum , GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDBUFFERPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETEBUFFERSPROC)(GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENBUFFERSPROC)(GLsizei , GLuint *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISBUFFERPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBUFFERDATAPROC)(GLenum , GLsizeiptr , స్థిరరాశి GLvoid *, GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETBUFFERSUBDATAPROC)(GLenum , GLintptr , GLsizeiptr , GLvoid *);
    రకనిర్వచనం GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERPROC)(GLenum , GLenum );
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNUNMAPBUFFERPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETBUFFERPOINTERVPROC)(GLenum , GLenum , GLvoid* *);

    // Extension: 2.0
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBLENDEQUATIONSEPARATEPROC)(GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWBUFFERSPROC)(GLsizei , స్థిరరాశి GLenum *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSTENCILOPSEPARATEPROC)(GLenum , GLenum , GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSTENCILFUNCSEPARATEPROC)(GLenum , GLenum , GLint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSTENCILMASKSEPARATEPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNATTACHSHADERPROC)(GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDATTRIBLOCATIONPROC)(GLuint , GLuint , స్థిరరాశి GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOMPILESHADERPROC)(GLuint );
    రకనిర్వచనం GLuint (CODEGEN_FUNCPTR *PFNCREATEPROGRAMPROC)();
    రకనిర్వచనం GLuint (CODEGEN_FUNCPTR *PFNCREATESHADERPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETEPROGRAMPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETESHADERPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDETACHSHADERPROC)(GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDISABLEVERTEXATTRIBARRAYPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENABLEVERTEXATTRIBARRAYPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETACTIVEATTRIBPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLint *, GLenum *, GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETATTACHEDSHADERSPROC)(GLuint , GLsizei , GLsizei *, GLuint *);
    రకనిర్వచనం GLint (CODEGEN_FUNCPTR *PFNGETATTRIBLOCATIONPROC)(GLuint , స్థిరరాశి GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETPROGRAMIVPROC)(GLuint , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETPROGRAMINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETSHADERIVPROC)(GLuint , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETSHADERINFOLOGPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETSHADERSOURCEPROC)(GLuint , GLsizei , GLsizei *, GLchar *);
    రకనిర్వచనం GLint (CODEGEN_FUNCPTR *PFNGETUNIFORMLOCATIONPROC)(GLuint , స్థిరరాశి GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETUNIFORMFVPROC)(GLuint , GLint , GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETUNIFORMIVPROC)(GLuint , GLint , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBDVPROC)(GLuint , GLenum , GLdouble *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBFVPROC)(GLuint , GLenum , GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIVPROC)(GLuint , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBPOINTERVPROC)(GLuint , GLenum , GLvoid* *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISPROGRAMPROC)(GLuint );
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISSHADERPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNLINKPROGRAMPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNSHADERSOURCEPROC)(GLuint , GLsizei , స్థిరరాశి GLchar* స్థిరరాశి *, స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUSEPROGRAMPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM1FPROC)(GLint , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM2FPROC)(GLint , GLfloat , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM3FPROC)(GLint , GLfloat , GLfloat , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM4FPROC)(GLint , GLfloat , GLfloat , GLfloat , GLfloat );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM1IPROC)(GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM2IPROC)(GLint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM3IPROC)(GLint , GLint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM4IPROC)(GLint , GLint , GLint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM1FVPROC)(GLint , GLsizei , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM2FVPROC)(GLint , GLsizei , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM3FVPROC)(GLint , GLsizei , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM4FVPROC)(GLint , GLsizei , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM1IVPROC)(GLint , GLsizei , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM2IVPROC)(GLint , GLsizei , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM3IVPROC)(GLint , GLsizei , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM4IVPROC)(GLint , GLsizei , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVALIDATEPROGRAMPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBPOINTERPROC)(GLuint , GLint , GLenum , GLboolean , GLsizei , స్థిరరాశి GLvoid *);

    // Extension: 2.1
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X3FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X2FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX2X4FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X2FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX3X4FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMMATRIX4X3FVPROC)(GLint , GLsizei , GLboolean , స్థిరరాశి GLfloat *);

    // Extension: ARB_vertex_array_object
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDVERTEXARRAYPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETEVERTEXARRAYSPROC)(GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENVERTEXARRAYSPROC)(GLsizei , GLuint *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISVERTEXARRAYPROC)(GLuint );

    // Extension: ARB_map_buffer_range
    రకనిర్వచనం GLvoid* (CODEGEN_FUNCPTR *PFNMAPBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr , GLbitfield );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFLUSHMAPPEDBUFFERRANGEPROC)(GLenum , GLintptr , GLsizeiptr );

    // Extension: ARB_framebuffer_object
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISRENDERBUFFERPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDRENDERBUFFERPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETERENDERBUFFERSPROC)(GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENRENDERBUFFERSPROC)(GLsizei , GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEPROC)(GLenum , GLenum , GLsizei , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETRENDERBUFFERPARAMETERIVPROC)(GLenum , GLenum , GLint *);
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISFRAMEBUFFERPROC)(GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDFRAMEBUFFERPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDELETEFRAMEBUFFERSPROC)(GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENFRAMEBUFFERSPROC)(GLsizei , GLuint *);
    రకనిర్వచనం GLenum (CODEGEN_FUNCPTR *PFNCHECKFRAMEBUFFERSTATUSPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE1DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE2DPROC)(GLenum , GLenum , GLenum , GLuint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURE3DPROC)(GLenum , GLenum , GLenum , GLuint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFRAMEBUFFERRENDERBUFFERPROC)(GLenum , GLenum , GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum , GLenum , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGENERATEMIPMAPPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBLITFRAMEBUFFERPROC)(GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLint , GLbitfield , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum , GLsizei , GLenum , GLsizei , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNFRAMEBUFFERTEXTURELAYERPROC)(GLenum , GLenum , GLuint , GLint , GLint );

    // Extension: 3.0
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOLORMASKIPROC)(GLuint , GLboolean , GLboolean , GLboolean , GLboolean );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETBOOLEANI_VPROC)(GLenum , GLuint , GLboolean *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETINTEGERI_VPROC)(GLenum , GLuint , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENABLEIPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDISABLEIPROC)(GLenum , GLuint );
    రకనిర్వచనం GLboolean (CODEGEN_FUNCPTR *PFNISENABLEDIPROC)(GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBEGINTRANSFORMFEEDBACKPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENDTRANSFORMFEEDBACKPROC)();
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDBUFFERRANGEPROC)(GLenum , GLuint , GLuint , GLintptr , GLsizeiptr );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDBUFFERBASEPROC)(GLenum , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTRANSFORMFEEDBACKVARYINGSPROC)(GLuint , GLsizei , స్థిరరాశి GLchar* స్థిరరాశి *, GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLsizei *, GLenum *, GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLAMPCOLORPROC)(GLenum , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBEGINCONDITIONALRENDERPROC)(GLuint , GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENDCONDITIONALRENDERPROC)();
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBIPOINTERPROC)(GLuint , GLint , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIIVPROC)(GLuint , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETVERTEXATTRIBIUIVPROC)(GLuint , GLenum , GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IPROC)(GLuint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IPROC)(GLuint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IPROC)(GLuint , GLint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IPROC)(GLuint , GLint , GLint , GLint , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIPROC)(GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIPROC)(GLuint , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIPROC)(GLuint , GLuint , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIPROC)(GLuint , GLuint , GLuint , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1IVPROC)(GLuint , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2IVPROC)(GLuint , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3IVPROC)(GLuint , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4IVPROC)(GLuint , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI1UIVPROC)(GLuint , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI2UIVPROC)(GLuint , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI3UIVPROC)(GLuint , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UIVPROC)(GLuint , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4BVPROC)(GLuint , స్థిరరాశి GLbyte *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4SVPROC)(GLuint , స్థిరరాశి GLshort *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4UBVPROC)(GLuint , స్థిరరాశి GLubyte *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXATTRIBI4USVPROC)(GLuint , స్థిరరాశి GLushort *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETUNIFORMUIVPROC)(GLuint , GLint , GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNBINDFRAGDATALOCATIONPROC)(GLuint , GLuint , స్థిరరాశి GLchar *);
    రకనిర్వచనం GLint (CODEGEN_FUNCPTR *PFNGETFRAGDATALOCATIONPROC)(GLuint , స్థిరరాశి GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM1UIPROC)(GLint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM2UIPROC)(GLint , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM3UIPROC)(GLint , GLuint , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM4UIPROC)(GLint , GLuint , GLuint , GLuint , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM1UIVPROC)(GLint , GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM2UIVPROC)(GLint , GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM3UIVPROC)(GLint , GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORM4UIVPROC)(GLint , GLsizei , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXPARAMETERIIVPROC)(GLenum , GLenum , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXPARAMETERIUIVPROC)(GLenum , GLenum , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIIVPROC)(GLenum , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETTEXPARAMETERIUIVPROC)(GLenum , GLenum , GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARBUFFERIVPROC)(GLenum , GLint , స్థిరరాశి GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARBUFFERUIVPROC)(GLenum , GLint , స్థిరరాశి GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARBUFFERFVPROC)(GLenum , GLint , స్థిరరాశి GLfloat *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCLEARBUFFERFIPROC)(GLenum , GLint , GLfloat , GLint );
    రకనిర్వచనం స్థిరరాశి GLubyte * (CODEGEN_FUNCPTR *PFNGETSTRINGIPROC)(GLenum , GLuint );

    // Extension: ARB_uniform_buffer_object
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETUNIFORMINDICESPROC)(GLuint , GLsizei , స్థిరరాశి GLchar* స్థిరరాశి *, GLuint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMSIVPROC)(GLuint , GLsizei , స్థిరరాశి GLuint *, GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    రకనిర్వచనం GLuint (CODEGEN_FUNCPTR *PFNGETUNIFORMBLOCKINDEXPROC)(GLuint , స్థిరరాశి GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKIVPROC)(GLuint , GLuint , GLenum , GLint *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint , GLuint , GLsizei , GLsizei *, GLchar *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNUNIFORMBLOCKBINDINGPROC)(GLuint , GLuint , GLuint );

    // Extension: ARB_copy_buffer
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOPYBUFFERSUBDATAPROC)(GLenum , GLenum , GLintptr , GLintptr , GLsizeiptr );

    // Extension: 3.1
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWARRAYSINSTANCEDPROC)(GLenum , GLint , GLsizei , GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDRAWELEMENTSINSTANCEDPROC)(GLenum , GLsizei , GLenum , స్థిరరాశి GLvoid *, GLsizei );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXBUFFERPROC)(GLenum , GLenum , GLuint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNPRIMITIVERESTARTINDEXPROC)(GLuint );

    // Legacy
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNENABLECLIENTSTATEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNDISABLECLIENTSTATEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNVERTEXPOINTERPROC)(GLint , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNNORMALPOINTERPROC)(GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOLORPOINTERPROC)(GLint , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXCOORDPOINTERPROC)(GLint , GLenum , GLsizei , స్థిరరాశి GLvoid *);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNTEXENVIPROC)(GLenum , GLenum , GLint );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNMATRIXMODEPROC)(GLenum );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNLOADIDENTITYPROC)(శూన్య);
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNORTHOPROC)(GLdouble , GLdouble , GLdouble , GLdouble , GLdouble , GLdouble );
    రకనిర్వచనం శూన్య (CODEGEN_FUNCPTR *PFNCOLOR3DPROC)(GLdouble , GLdouble , GLdouble );

    //////////////////////////////////////////////
    // Function pointers

    // Extension: 1.1
    PFNCULLFACEPROC CullFace;
    PFNFRONTFACEPROC FrontFace;
    PFNHINTPROC Hint;
    PFNLINEWIDTHPROC పంక్తి_వెడల్పు;
    PFNPOINTSIZEPROC బిందువు_కొలత;
    PFNPOLYGONMODEPROC Polygon_బహుళకం;
    PFNSCISSORPROC Scissor;
    PFNTEXPARAMETERFPROC TexParameterf;
    PFNTEXPARAMETERFVPROC TexParameterfv;
    PFNTEXPARAMETERIPROC TexParameteri;
    PFNTEXPARAMETERIVPROC TexParameteriv;
    PFNTEXIMAGE1DPROC TexImage1D;
    PFNTEXIMAGE2DPROC TexImage2D;
    PFNDRAWBUFFERPROC Draw_బఫరు;
    PFNCLEARPROC Clear;
    PFNCLEARCOLORPROC ClearColor;
    PFNCLEARSTENCILPROC ClearStencil;
    PFNCLEARDEPTHPROC Clear_లోతు;
    PFNSTENCILMASKPROC Stencil_ముసుగు;
    PFNCOLORMASKPROC Color_ముసుగు;
    PFNDEPTHMASKPROC లోతు_ముసుగు;
    PFNDISABLEPROC Disable;
    PFNENABLEPROC Enable;
    PFNFINISHPROC ముగింపు;
    PFNFLUSHPROC Flush;
    PFNBLENDFUNCPROC BlendFunc;
    PFNLOGICOPPROC LogicOp;
    PFNSTENCILFUNCPROC StencilFunc;
    PFNSTENCILOPPROC StencilOp;
    PFNDEPTHFUNCPROC లోతు_Func;
    PFNPIXELSTOREFPROC PixelStoref;
    PFNPIXELSTOREIPROC PixelStorei;
    PFNREADBUFFERPROC Read_బఫరు;
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
    PFNDEPTHRANGEPROC లోతు_Range;
    PFNVIEWPORTPROC Viewport;
    PFNDRAWARRAYSPROC DrawArrays;
    PFNDRAWELEMENTSPROC DrawElements;
    PFNGETPOINTERVPROC GetPointerv;
    PFNPOLYGONOFFSETPROC Polygon_స్థానభ్రంశం;
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
    PFNACTIVETEXTUREPROC చురుకైన_Texture;
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
    PFNPOINTPARAMETERFPROC బిందువు_Parameterf;
    PFNPOINTPARAMETERFVPROC బిందువు_Parameterfv;
    PFNPOINTPARAMETERIPROC బిందువు_Parameteri;
    PFNPOINTPARAMETERIVPROC బిందువు_Parameteriv;

    // Extension: 1.5
    PFNGENQUERIESPROC GenQueries;
    PFNDELETEQUERIESPROC DeleteQueries;
    PFNISQUERYPROC IsQuery;
    PFNBEGINQUERYPROC BeginQuery;
    PFNENDQUERYPROC చివర_Query;
    PFNGETQUERYIVPROC GetQueryiv;
    PFNGETQUERYOBJECTIVPROC GetQueryObjectiv;
    PFNGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
    PFNBINDBUFFERPROC Bind_బఫరు;
    PFNDELETEBUFFERSPROC DeleteBuffers;
    PFNGENBUFFERSPROC GenBuffers;
    PFNISBUFFERPROC Is_బఫరు;
    PFNBUFFERDATAPROC బఫరు_దత్తాంశం;
    PFNBUFFERSUBDATAPROC బఫరు_Sub_దత్తాంశం;
    PFNGETBUFFERSUBDATAPROC Get_బఫరు_Sub_దత్తాంశం;
    PFNMAPBUFFERPROC Map_బఫరు;
    PFNUNMAPBUFFERPROC Unmap_బఫరు;
    PFNGETBUFFERPARAMETERIVPROC Get_బఫరు_Parameteriv;
    PFNGETBUFFERPOINTERVPROC Get_బఫరు_Pointerv;

    // Extension: 2.0
    PFNBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
    PFNDRAWBUFFERSPROC DrawBuffers;
    PFNSTENCILOPSEPARATEPROC StencilOpSeparate;
    PFNSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
    PFNSTENCILMASKSEPARATEPROC Stencil_ముసుగు_Separate;
    PFNATTACHSHADERPROC AttachShader;
    PFNBINDATTRIBLOCATIONPROC BindAttribLocation;
    PFNCOMPILESHADERPROC CompileShader;
    PFNCREATEPROGRAMPROC Create_కార్యక్రమం;
    PFNCREATESHADERPROC CreateShader;
    PFNDELETEPROGRAMPROC Delete_కార్యక్రమం;
    PFNDELETESHADERPROC DeleteShader;
    PFNDETACHSHADERPROC DetachShader;
    PFNDISABLEVERTEXATTRIBARRAYPROC Disable_శీర్షం_Attrib_Array;
    PFNENABLEVERTEXATTRIBARRAYPROC Enable_శీర్షం_Attrib_Array;
    PFNGETACTIVEATTRIBPROC Get_చురుకైన_Attrib;
    PFNGETACTIVEUNIFORMPROC Get_చురుకైన_ఏకరీతి;
    PFNGETATTACHEDSHADERSPROC GetAttachedShaders;
    PFNGETATTRIBLOCATIONPROC GetAttribLocation;
    PFNGETPROGRAMIVPROC GetProgramiv;
    PFNGETPROGRAMINFOLOGPROC Get_కార్యక్రమం_Info_Log;
    PFNGETSHADERIVPROC GetShaderiv;
    PFNGETSHADERINFOLOGPROC GetShaderInfoLog;
    PFNGETSHADERSOURCEPROC Get_Shader_ఆధారం;
    PFNGETUNIFORMLOCATIONPROC Get_ఏకరీతి_Location;
    PFNGETUNIFORMFVPROC GetUniformfv;
    PFNGETUNIFORMIVPROC GetUniformiv;
    PFNGETVERTEXATTRIBDVPROC Get_శీర్షం_Attribdv;
    PFNGETVERTEXATTRIBFVPROC Get_శీర్షం_Attribfv;
    PFNGETVERTEXATTRIBIVPROC Get_శీర్షం_Attribiv;
    PFNGETVERTEXATTRIBPOINTERVPROC Get_శీర్షం_Attrib_Pointerv;
    PFNISPROGRAMPROC Is_కార్యక్రమం;
    PFNISSHADERPROC IsShader;
    PFNLINKPROGRAMPROC Link_కార్యక్రమం;
    PFNSHADERSOURCEPROC Shader_ఆధారం;
    PFNUSEPROGRAMPROC Use_కార్యక్రమం;
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
    PFNUNIFORMMATRIX2FVPROC ఏకరీతి_Matrix2fv;
    PFNUNIFORMMATRIX3FVPROC ఏకరీతి_Matrix3fv;
    PFNUNIFORMMATRIX4FVPROC ఏకరీతి_Matrix4fv;
    PFNVALIDATEPROGRAMPROC Validate_కార్యక్రమం;
    PFNVERTEXATTRIBPOINTERPROC శీర్షం_Attrib_సూచిక;

    // Extension: 2.1
    PFNUNIFORMMATRIX2X3FVPROC ఏకరీతి_Matrix2x3fv;
    PFNUNIFORMMATRIX3X2FVPROC ఏకరీతి_Matrix3x2fv;
    PFNUNIFORMMATRIX2X4FVPROC ఏకరీతి_Matrix2x4fv;
    PFNUNIFORMMATRIX4X2FVPROC ఏకరీతి_Matrix4x2fv;
    PFNUNIFORMMATRIX3X4FVPROC ఏకరీతి_Matrix3x4fv;
    PFNUNIFORMMATRIX4X3FVPROC ఏకరీతి_Matrix4x3fv;

    // Extension: ARB_vertex_array_object
    PFNBINDVERTEXARRAYPROC Bind_శీర్షం_Array;
    PFNDELETEVERTEXARRAYSPROC Delete_శీర్షం_Arrays;
    PFNGENVERTEXARRAYSPROC Gen_శీర్షం_Arrays;
    PFNISVERTEXARRAYPROC Is_శీర్షం_Array;

    // Extension: ARB_map_buffer_range
    PFNMAPBUFFERRANGEPROC Map_బఫరు_Range;
    PFNFLUSHMAPPEDBUFFERRANGEPROC Flush_Mapped_బఫరు_Range;

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
    PFNCHECKFRAMEBUFFERSTATUSPROC Check_Framebuffer_స్థితి;
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
    PFNENDTRANSFORMFEEDBACKPROC చివర_Transform_Feedback;
    PFNBINDBUFFERRANGEPROC Bind_బఫరు_Range;
    PFNBINDBUFFERBASEPROC Bind_బఫరు_పునాది;
    PFNTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
    PFNGETTRANSFORMFEEDBACKVARYINGPROC GetTransformFeedbackVarying;
    PFNCLAMPCOLORPROC ClampColor;
    PFNBEGINCONDITIONALRENDERPROC BeginConditionalRender;
    PFNENDCONDITIONALRENDERPROC చివర_Conditional_Render;
    PFNVERTEXATTRIBIPOINTERPROC శీర్షం_Attrib_IPointer;
    PFNGETVERTEXATTRIBIIVPROC Get_శీర్షం_Attrib_Iiv;
    PFNGETVERTEXATTRIBIUIVPROC Get_శీర్షం_Attrib_Iuiv;
    PFNVERTEXATTRIBI1IPROC శీర్షం_Attrib_I1i;
    PFNVERTEXATTRIBI2IPROC శీర్షం_Attrib_I2i;
    PFNVERTEXATTRIBI3IPROC శీర్షం_Attrib_I3i;
    PFNVERTEXATTRIBI4IPROC శీర్షం_Attrib_I4i;
    PFNVERTEXATTRIBI1UIPROC శీర్షం_Attrib_I1ui;
    PFNVERTEXATTRIBI2UIPROC శీర్షం_Attrib_I2ui;
    PFNVERTEXATTRIBI3UIPROC శీర్షం_Attrib_I3ui;
    PFNVERTEXATTRIBI4UIPROC శీర్షం_Attrib_I4ui;
    PFNVERTEXATTRIBI1IVPROC శీర్షం_Attrib_I1iv;
    PFNVERTEXATTRIBI2IVPROC శీర్షం_Attrib_I2iv;
    PFNVERTEXATTRIBI3IVPROC శీర్షం_Attrib_I3iv;
    PFNVERTEXATTRIBI4IVPROC శీర్షం_Attrib_I4iv;
    PFNVERTEXATTRIBI1UIVPROC శీర్షం_Attrib_I1uiv;
    PFNVERTEXATTRIBI2UIVPROC శీర్షం_Attrib_I2uiv;
    PFNVERTEXATTRIBI3UIVPROC శీర్షం_Attrib_I3uiv;
    PFNVERTEXATTRIBI4UIVPROC శీర్షం_Attrib_I4uiv;
    PFNVERTEXATTRIBI4BVPROC శీర్షం_Attrib_I4bv;
    PFNVERTEXATTRIBI4SVPROC శీర్షం_Attrib_I4sv;
    PFNVERTEXATTRIBI4UBVPROC శీర్షం_Attrib_I4ubv;
    PFNVERTEXATTRIBI4USVPROC శీర్షం_Attrib_I4usv;
    PFNGETUNIFORMUIVPROC GetUniformuiv;
    PFNBINDFRAGDATALOCATIONPROC Bind_Frag_దత్తాంశం_Location;
    PFNGETFRAGDATALOCATIONPROC Get_Frag_దత్తాంశం_Location;
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
    PFNGETUNIFORMINDICESPROC Get_ఏకరీతి_Indices;
    PFNGETACTIVEUNIFORMSIVPROC Get_చురుకైన_Uniformsiv;
    PFNGETACTIVEUNIFORMNAMEPROC Get_చురుకైన_ఏకరీతి_Name;
    PFNGETUNIFORMBLOCKINDEXPROC Get_ఏకరీతి_ఖండం_సూచీ;
    PFNGETACTIVEUNIFORMBLOCKIVPROC Get_చురుకైన_ఏకరీతి_Blockiv;
    PFNGETACTIVEUNIFORMBLOCKNAMEPROC Get_చురుకైన_ఏకరీతి_ఖండం_Name;
    PFNUNIFORMBLOCKBINDINGPROC ఏకరీతి_ఖండం_Binding;

    // Extension: ARB_copy_buffer
    PFNCOPYBUFFERSUBDATAPROC Copy_బఫరు_Sub_దత్తాంశం;

    // Extension: 3.1
    PFNDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
    PFNDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
    PFNTEXBUFFERPROC Tex_బఫరు;
    PFNPRIMITIVERESTARTINDEXPROC Primitive_Restart_సూచీ;

    // Legacy
    PFNENABLECLIENTSTATEPROC EnableClientState;
    PFNDISABLECLIENTSTATEPROC DisableClientState;
    PFNVERTEXPOINTERPROC శీర్షం_సూచిక;
    PFNNORMALPOINTERPROC సాధారణ_సూచిక;
    PFNCOLORPOINTERPROC Color_సూచిక;
    PFNTEXCOORDPOINTERPROC Tex_Coord_సూచిక;

    PFNTEXENVIPROC TexEnvi;

    PFNMATRIXMODEPROC మాతృక_బహుళకం;
    PFNLOADIDENTITYPROC ఎక్కించు_తటస్థం;
    PFNORTHOPROC Ortho;

    PFNCOLOR3DPROC Color3d;

    //////////////////////////////////////////////
    // Switch functions

    // Extension: 1.1

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CullFace(GLenum బహుళకం)
    {
        CullFace = (PFNCULLFACEPROC)Int_Get_Proc_చిరునామా("glCullFace");
        CullFace(బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_FrontFace(GLenum బహుళకం)
    {
        FrontFace = (PFNFRONTFACEPROC)Int_Get_Proc_చిరునామా("glFrontFace");
        FrontFace(బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Hint(GLenum target, GLenum బహుళకం)
    {
        Hint = (PFNHINTPROC)Int_Get_Proc_చిరునామా("glHint");
        Hint(target, బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_పంక్తి_వెడల్పు(GLfloat వెడల్పు)
    {
        పంక్తి_వెడల్పు = (PFNLINEWIDTHPROC)Int_Get_Proc_చిరునామా("glLineWidth");
        పంక్తి_వెడల్పు(వెడల్పు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బిందువు_కొలత(GLfloat size)
    {
        బిందువు_కొలత = (PFNPOINTSIZEPROC)Int_Get_Proc_చిరునామా("glPointSize");
        బిందువు_కొలత(size);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Polygon_బహుళకం(GLenum face, GLenum బహుళకం)
    {
        Polygon_బహుళకం = (PFNPOLYGONMODEPROC)Int_Get_Proc_చిరునామా("glPolygonMode");
        Polygon_బహుళకం(face, బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Scissor(GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు)
    {
        Scissor = (PFNSCISSORPROC)Int_Get_Proc_చిరునామా("glScissor");
        Scissor(x, y, వెడల్పు, ఎత్తు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexParameterf(GLenum target, GLenum pname, GLfloat param)
    {
        TexParameterf = (PFNTEXPARAMETERFPROC)Int_Get_Proc_చిరునామా("glTexParameterf");
        TexParameterf(target, pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexParameterfv(GLenum target, GLenum pname, స్థిరరాశి GLfloat *params)
    {
        TexParameterfv = (PFNTEXPARAMETERFVPROC)Int_Get_Proc_చిరునామా("glTexParameterfv");
        TexParameterfv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexParameteri(GLenum target, GLenum pname, GLint param)
    {
        TexParameteri = (PFNTEXPARAMETERIPROC)Int_Get_Proc_చిరునామా("glTexParameteri");
        TexParameteri(target, pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexParameteriv(GLenum target, GLenum pname, స్థిరరాశి GLint *params)
    {
        TexParameteriv = (PFNTEXPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glTexParameteriv");
        TexParameteriv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei వెడల్పు, GLint border, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels)
    {
        TexImage1D = (PFNTEXIMAGE1DPROC)Int_Get_Proc_చిరునామా("glTexImage1D");
        TexImage1D(target, level, internalformat, వెడల్పు, border, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు, GLint border, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels)
    {
        TexImage2D = (PFNTEXIMAGE2DPROC)Int_Get_Proc_చిరునామా("glTexImage2D");
        TexImage2D(target, level, internalformat, వెడల్పు, ఎత్తు, border, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Draw_బఫరు(GLenum బహుళకం)
    {
        Draw_బఫరు = (PFNDRAWBUFFERPROC)Int_Get_Proc_చిరునామా("glDrawBuffer");
        Draw_బఫరు(బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Clear(GLbitfield ముసుగు)
    {
        Clear = (PFNCLEARPROC)Int_Get_Proc_చిరునామా("glClear");
        Clear(ముసుగు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        ClearColor = (PFNCLEARCOLORPROC)Int_Get_Proc_చిరునామా("glClearColor");
        ClearColor(red, green, blue, alpha);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClearStencil(GLint s)
    {
        ClearStencil = (PFNCLEARSTENCILPROC)Int_Get_Proc_చిరునామా("glClearStencil");
        ClearStencil(s);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Clear_లోతు(GLdouble లోతు)
    {
        Clear_లోతు = (PFNCLEARDEPTHPROC)Int_Get_Proc_చిరునామా("glClearDepth");
        Clear_లోతు(లోతు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Stencil_ముసుగు(GLuint ముసుగు)
    {
        Stencil_ముసుగు = (PFNSTENCILMASKPROC)Int_Get_Proc_చిరునామా("glStencilMask");
        Stencil_ముసుగు(ముసుగు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Color_ముసుగు(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
    {
        Color_ముసుగు = (PFNCOLORMASKPROC)Int_Get_Proc_చిరునామా("glColorMask");
        Color_ముసుగు(red, green, blue, alpha);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_లోతు_ముసుగు(GLboolean flag)
    {
        లోతు_ముసుగు = (PFNDEPTHMASKPROC)Int_Get_Proc_చిరునామా("glDepthMask");
        లోతు_ముసుగు(flag);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Disable(GLenum cap)
    {
        Disable = (PFNDISABLEPROC)Int_Get_Proc_చిరునామా("glDisable");
        Disable(cap);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Enable(GLenum cap)
    {
        Enable = (PFNENABLEPROC)Int_Get_Proc_చిరునామా("glEnable");
        Enable(cap);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ముగింపు()
    {
        ముగింపు = (PFNFINISHPROC)Int_Get_Proc_చిరునామా("glFinish");
        ముగింపు();
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Flush()
    {
        Flush = (PFNFLUSHPROC)Int_Get_Proc_చిరునామా("glFlush");
        Flush();
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BlendFunc(GLenum sfactor, GLenum dfactor)
    {
        BlendFunc = (PFNBLENDFUNCPROC)Int_Get_Proc_చిరునామా("glBlendFunc");
        BlendFunc(sfactor, dfactor);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_LogicOp(GLenum opcode)
    {
        LogicOp = (PFNLOGICOPPROC)Int_Get_Proc_చిరునామా("glLogicOp");
        LogicOp(opcode);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_StencilFunc(GLenum func, GLint ref, GLuint ముసుగు)
    {
        StencilFunc = (PFNSTENCILFUNCPROC)Int_Get_Proc_చిరునామా("glStencilFunc");
        StencilFunc(func, ref, ముసుగు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
    {
        StencilOp = (PFNSTENCILOPPROC)Int_Get_Proc_చిరునామా("glStencilOp");
        StencilOp(fail, zfail, zpass);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_లోతు_Func(GLenum func)
    {
        లోతు_Func = (PFNDEPTHFUNCPROC)Int_Get_Proc_చిరునామా("glDepthFunc");
        లోతు_Func(func);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_PixelStoref(GLenum pname, GLfloat param)
    {
        PixelStoref = (PFNPIXELSTOREFPROC)Int_Get_Proc_చిరునామా("glPixelStoref");
        PixelStoref(pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_PixelStorei(GLenum pname, GLint param)
    {
        PixelStorei = (PFNPIXELSTOREIPROC)Int_Get_Proc_చిరునామా("glPixelStorei");
        PixelStorei(pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Read_బఫరు(GLenum బహుళకం)
    {
        Read_బఫరు = (PFNREADBUFFERPROC)Int_Get_Proc_చిరునామా("glReadBuffer");
        Read_బఫరు(బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ReadPixels(GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, GLvoid *pixels)
    {
        ReadPixels = (PFNREADPIXELSPROC)Int_Get_Proc_చిరునామా("glReadPixels");
        ReadPixels(x, y, వెడల్పు, ఎత్తు, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetBooleanv(GLenum pname, GLboolean *params)
    {
        GetBooleanv = (PFNGETBOOLEANVPROC)Int_Get_Proc_చిరునామా("glGetBooleanv");
        GetBooleanv(pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetDoublev(GLenum pname, GLdouble *params)
    {
        GetDoublev = (PFNGETDOUBLEVPROC)Int_Get_Proc_చిరునామా("glGetDoublev");
        GetDoublev(pname, params);
    }

    స్థిర GLenum CODEGEN_FUNCPTR Switch_GetError()
    {
        GetError = (PFNGETERRORPROC)Int_Get_Proc_చిరునామా("glGetError");
        తిరిగి GetError();
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetFloatv(GLenum pname, GLfloat *params)
    {
        GetFloatv = (PFNGETFLOATVPROC)Int_Get_Proc_చిరునామా("glGetFloatv");
        GetFloatv(pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetIntegerv(GLenum pname, GLint *params)
    {
        GetIntegerv = (PFNGETINTEGERVPROC)Int_Get_Proc_చిరునామా("glGetIntegerv");
        GetIntegerv(pname, params);
    }

    స్థిర స్థిరరాశి GLubyte * CODEGEN_FUNCPTR Switch_GetString(GLenum name)
    {
        GetString = (PFNGETSTRINGPROC)Int_Get_Proc_చిరునామా("glGetString");
        తిరిగి GetString(name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels)
    {
        GetTexImage = (PFNGETTEXIMAGEPROC)Int_Get_Proc_చిరునామా("glGetTexImage");
        GetTexImage(target, level, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
    {
        GetTexParameterfv = (PFNGETTEXPARAMETERFVPROC)Int_Get_Proc_చిరునామా("glGetTexParameterfv");
        GetTexParameterfv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameteriv = (PFNGETTEXPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glGetTexParameteriv");
        GetTexParameteriv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params)
    {
        GetTexLevelParameterfv = (PFNGETTEXLEVELPARAMETERFVPROC)Int_Get_Proc_చిరునామా("glGetTexLevelParameterfv");
        GetTexLevelParameterfv(target, level, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
    {
        GetTexLevelParameteriv = (PFNGETTEXLEVELPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glGetTexLevelParameteriv");
        GetTexLevelParameteriv(target, level, pname, params);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsEnabled(GLenum cap)
    {
        IsEnabled = (PFNISENABLEDPROC)Int_Get_Proc_చిరునామా("glIsEnabled");
        తిరిగి IsEnabled(cap);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_లోతు_Range(GLdouble ren_near, GLdouble ren_far)
    {
        లోతు_Range = (PFNDEPTHRANGEPROC)Int_Get_Proc_చిరునామా("glDepthRange");
        లోతు_Range(ren_near, ren_far);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Viewport(GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు)
    {
        Viewport = (PFNVIEWPORTPROC)Int_Get_Proc_చిరునామా("glViewport");
        Viewport(x, y, వెడల్పు, ఎత్తు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DrawArrays(GLenum బహుళకం, GLint first, GLsizei లెక్క_2)
    {
        DrawArrays = (PFNDRAWARRAYSPROC)Int_Get_Proc_చిరునామా("glDrawArrays");
        DrawArrays(బహుళకం, first, లెక్క_2);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DrawElements(GLenum బహుళకం, GLsizei లెక్క_2, GLenum type, స్థిరరాశి GLvoid *indices)
    {
        DrawElements = (PFNDRAWELEMENTSPROC)Int_Get_Proc_చిరునామా("glDrawElements");
        DrawElements(బహుళకం, లెక్క_2, type, indices);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetPointerv(GLenum pname, GLvoid* *params)
    {
        GetPointerv = (PFNGETPOINTERVPROC)Int_Get_Proc_చిరునామా("glGetPointerv");
        GetPointerv(pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Polygon_స్థానభ్రంశం(GLfloat గుణకం, GLfloat units)
    {
        Polygon_స్థానభ్రంశం = (PFNPOLYGONOFFSETPROC)Int_Get_Proc_చిరునామా("glPolygonOffset");
        Polygon_స్థానభ్రంశం(గుణకం, units);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei వెడల్పు, GLint border)
    {
        CopyTexImage1D = (PFNCOPYTEXIMAGE1DPROC)Int_Get_Proc_చిరునామా("glCopyTexImage1D");
        CopyTexImage1D(target, level, internalformat, x, y, వెడల్పు, border);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు, GLint border)
    {
        CopyTexImage2D = (PFNCOPYTEXIMAGE2DPROC)Int_Get_Proc_చిరునామా("glCopyTexImage2D");
        CopyTexImage2D(target, level, internalformat, x, y, వెడల్పు, ఎత్తు, border);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei వెడల్పు)
    {
        CopyTexSubImage1D = (PFNCOPYTEXSUBIMAGE1DPROC)Int_Get_Proc_చిరునామా("glCopyTexSubImage1D");
        CopyTexSubImage1D(target, level, xoffset, x, y, వెడల్పు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు)
    {
        CopyTexSubImage2D = (PFNCOPYTEXSUBIMAGE2DPROC)Int_Get_Proc_చిరునామా("glCopyTexSubImage2D");
        CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, వెడల్పు, ఎత్తు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei వెడల్పు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels)
    {
        TexSubImage1D = (PFNTEXSUBIMAGE1DPROC)Int_Get_Proc_చిరునామా("glTexSubImage1D");
        TexSubImage1D(target, level, xoffset, వెడల్పు, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels)
    {
        TexSubImage2D = (PFNTEXSUBIMAGE2DPROC)Int_Get_Proc_చిరునామా("glTexSubImage2D");
        TexSubImage2D(target, level, xoffset, yoffset, వెడల్పు, ఎత్తు, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BindTexture(GLenum target, GLuint texture)
    {
        BindTexture = (PFNBINDTEXTUREPROC)Int_Get_Proc_చిరునామా("glBindTexture");
        BindTexture(target, texture);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DeleteTextures(GLsizei n, స్థిరరాశి GLuint *textures)
    {
        DeleteTextures = (PFNDELETETEXTURESPROC)Int_Get_Proc_చిరునామా("glDeleteTextures");
        DeleteTextures(n, textures);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GenTextures(GLsizei n, GLuint *textures)
    {
        GenTextures = (PFNGENTEXTURESPROC)Int_Get_Proc_చిరునామా("glGenTextures");
        GenTextures(n, textures);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsTexture(GLuint texture)
    {
        IsTexture = (PFNISTEXTUREPROC)Int_Get_Proc_చిరునామా("glIsTexture");
        తిరిగి IsTexture(texture);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Indexub(GLubyte c)
    {
        Indexub = (PFNINDEXUBPROC)Int_Get_Proc_చిరునామా("glIndexub");
        Indexub(c);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Indexubv(స్థిరరాశి GLubyte *c)
    {
        Indexubv = (PFNINDEXUBVPROC)Int_Get_Proc_చిరునామా("glIndexubv");
        Indexubv(c);
    }

    // Extension: 1.2

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
    {
        BlendColor = (PFNBLENDCOLORPROC)Int_Get_Proc_చిరునామా("glBlendColor");
        BlendColor(red, green, blue, alpha);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BlendEquation(GLenum బహుళకం)
    {
        BlendEquation = (PFNBLENDEQUATIONPROC)Int_Get_Proc_చిరునామా("glBlendEquation");
        BlendEquation(బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DrawRangeElements(GLenum బహుళకం, GLuint ప్రారంభం, GLuint end, GLsizei లెక్క_2, GLenum type, స్థిరరాశి GLvoid *indices)
    {
        DrawRangeElements = (PFNDRAWRANGEELEMENTSPROC)Int_Get_Proc_చిరునామా("glDrawRangeElements");
        DrawRangeElements(బహుళకం, ప్రారంభం, end, లెక్క_2, type, indices);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLsizei లోతు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels)
    {
        TexSubImage3D = (PFNTEXSUBIMAGE3DPROC)Int_Get_Proc_చిరునామా("glTexSubImage3D");
        TexSubImage3D(target, level, xoffset, yoffset, zoffset, వెడల్పు, ఎత్తు, లోతు, format, type, pixels);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei వెడల్పు, GLsizei ఎత్తు)
    {
        CopyTexSubImage3D = (PFNCOPYTEXSUBIMAGE3DPROC)Int_Get_Proc_చిరునామా("glCopyTexSubImage3D");
        CopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, వెడల్పు, ఎత్తు);
    }

    // Extension: 1.3

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_చురుకైన_Texture(GLenum texture)
    {
        చురుకైన_Texture = (PFNACTIVETEXTUREPROC)Int_Get_Proc_చిరునామా("glActiveTexture");
        చురుకైన_Texture(texture);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_SampleCoverage(GLfloat value, GLboolean invert)
    {
        SampleCoverage = (PFNSAMPLECOVERAGEPROC)Int_Get_Proc_చిరునామా("glSampleCoverage");
        SampleCoverage(value, invert);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు, GLsizei లోతు, GLint border, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        CompressedTexImage3D = (PFNCOMPRESSEDTEXIMAGE3DPROC)Int_Get_Proc_చిరునామా("glCompressedTexImage3D");
        CompressedTexImage3D(target, level, internalformat, వెడల్పు, ఎత్తు, లోతు, border, image_కొలత, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు, GLint border, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        CompressedTexImage2D = (PFNCOMPRESSEDTEXIMAGE2DPROC)Int_Get_Proc_చిరునామా("glCompressedTexImage2D");
        CompressedTexImage2D(target, level, internalformat, వెడల్పు, ఎత్తు, border, image_కొలత, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei వెడల్పు, GLint border, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        CompressedTexImage1D = (PFNCOMPRESSEDTEXIMAGE1DPROC)Int_Get_Proc_చిరునామా("glCompressedTexImage1D");
        CompressedTexImage1D(target, level, internalformat, వెడల్పు, border, image_కొలత, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLsizei లోతు, GLenum format, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        CompressedTexSubImage3D = (PFNCOMPRESSEDTEXSUBIMAGE3DPROC)Int_Get_Proc_చిరునామా("glCompressedTexSubImage3D");
        CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset, వెడల్పు, ఎత్తు, లోతు, format, image_కొలత, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        CompressedTexSubImage2D = (PFNCOMPRESSEDTEXSUBIMAGE2DPROC)Int_Get_Proc_చిరునామా("glCompressedTexSubImage2D");
        CompressedTexSubImage2D(target, level, xoffset, yoffset, వెడల్పు, ఎత్తు, format, image_కొలత, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei వెడల్పు, GLenum format, GLsizei image_కొలత, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        CompressedTexSubImage1D = (PFNCOMPRESSEDTEXSUBIMAGE1DPROC)Int_Get_Proc_చిరునామా("glCompressedTexSubImage1D");
        CompressedTexSubImage1D(target, level, xoffset, వెడల్పు, format, image_కొలత, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetCompressedTexImage(GLenum target, GLint level, GLvoid *img)
    {
        GetCompressedTexImage = (PFNGETCOMPRESSEDTEXIMAGEPROC)Int_Get_Proc_చిరునామా("glGetCompressedTexImage");
        GetCompressedTexImage(target, level, img);
    }

    // Extension: 1.4

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
    {
        BlendFuncSeparate = (PFNBLENDFUNCSEPARATEPROC)Int_Get_Proc_చిరునామా("glBlendFuncSeparate");
        BlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_MultiDrawArrays(GLenum బహుళకం, స్థిరరాశి GLint *first, స్థిరరాశి GLsizei *లెక్క_2, GLsizei drawcount)
    {
        MultiDrawArrays = (PFNMULTIDRAWARRAYSPROC)Int_Get_Proc_చిరునామా("glMultiDrawArrays");
        MultiDrawArrays(బహుళకం, first, లెక్క_2, drawcount);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_MultiDrawElements(GLenum బహుళకం, స్థిరరాశి GLsizei *లెక్క_2, GLenum type, స్థిరరాశి GLvoid* స్థిరరాశి *indices, GLsizei drawcount)
    {
        MultiDrawElements = (PFNMULTIDRAWELEMENTSPROC)Int_Get_Proc_చిరునామా("glMultiDrawElements");
        MultiDrawElements(బహుళకం, లెక్క_2, type, indices, drawcount);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బిందువు_Parameterf(GLenum pname, GLfloat param)
    {
        బిందువు_Parameterf = (PFNPOINTPARAMETERFPROC)Int_Get_Proc_చిరునామా("glPointParameterf");
        బిందువు_Parameterf(pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బిందువు_Parameterfv(GLenum pname, స్థిరరాశి GLfloat *params)
    {
        బిందువు_Parameterfv = (PFNPOINTPARAMETERFVPROC)Int_Get_Proc_చిరునామా("glPointParameterfv");
        బిందువు_Parameterfv(pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బిందువు_Parameteri(GLenum pname, GLint param)
    {
        బిందువు_Parameteri = (PFNPOINTPARAMETERIPROC)Int_Get_Proc_చిరునామా("glPointParameteri");
        బిందువు_Parameteri(pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బిందువు_Parameteriv(GLenum pname, స్థిరరాశి GLint *params)
    {
        బిందువు_Parameteriv = (PFNPOINTPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glPointParameteriv");
        బిందువు_Parameteriv(pname, params);
    }

    // Extension: 1.5

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GenQueries(GLsizei n, GLuint *ids)
    {
        GenQueries = (PFNGENQUERIESPROC)Int_Get_Proc_చిరునామా("glGenQueries");
        GenQueries(n, ids);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DeleteQueries(GLsizei n, స్థిరరాశి GLuint *ids)
    {
        DeleteQueries = (PFNDELETEQUERIESPROC)Int_Get_Proc_చిరునామా("glDeleteQueries");
        DeleteQueries(n, ids);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsQuery(GLuint id)
    {
        IsQuery = (PFNISQUERYPROC)Int_Get_Proc_చిరునామా("glIsQuery");
        తిరిగి IsQuery(id);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BeginQuery(GLenum target, GLuint id)
    {
        BeginQuery = (PFNBEGINQUERYPROC)Int_Get_Proc_చిరునామా("glBeginQuery");
        BeginQuery(target, id);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_చివర_Query(GLenum target)
    {
        చివర_Query = (PFNENDQUERYPROC)Int_Get_Proc_చిరునామా("glEndQuery");
        చివర_Query(target);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetQueryiv(GLenum target, GLenum pname, GLint *params)
    {
        GetQueryiv = (PFNGETQUERYIVPROC)Int_Get_Proc_చిరునామా("glGetQueryiv");
        GetQueryiv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetQueryObjectiv(GLuint id, GLenum pname, GLint *params)
    {
        GetQueryObjectiv = (PFNGETQUERYOBJECTIVPROC)Int_Get_Proc_చిరునామా("glGetQueryObjectiv");
        GetQueryObjectiv(id, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params)
    {
        GetQueryObjectuiv = (PFNGETQUERYOBJECTUIVPROC)Int_Get_Proc_చిరునామా("glGetQueryObjectuiv");
        GetQueryObjectuiv(id, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Bind_బఫరు(GLenum target, GLuint బఫరు)
    {
        Bind_బఫరు = (PFNBINDBUFFERPROC)Int_Get_Proc_చిరునామా("glBindBuffer");
        Bind_బఫరు(target, బఫరు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DeleteBuffers(GLsizei n, స్థిరరాశి GLuint *buffers)
    {
        DeleteBuffers = (PFNDELETEBUFFERSPROC)Int_Get_Proc_చిరునామా("glDeleteBuffers");
        DeleteBuffers(n, buffers);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GenBuffers(GLsizei n, GLuint *buffers)
    {
        GenBuffers = (PFNGENBUFFERSPROC)Int_Get_Proc_చిరునామా("glGenBuffers");
        GenBuffers(n, buffers);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_Is_బఫరు(GLuint బఫరు)
    {
        Is_బఫరు = (PFNISBUFFERPROC)Int_Get_Proc_చిరునామా("glIsBuffer");
        తిరిగి Is_బఫరు(బఫరు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బఫరు_దత్తాంశం(GLenum target, GLsizeiptr size, స్థిరరాశి GLvoid *దత్తాంశం, GLenum usage)
    {
        బఫరు_దత్తాంశం = (PFNBUFFERDATAPROC)Int_Get_Proc_చిరునామా("glBufferData");
        బఫరు_దత్తాంశం(target, size, దత్తాంశం, usage);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_బఫరు_Sub_దత్తాంశం(GLenum target, GLintptr offset, GLsizeiptr size, స్థిరరాశి GLvoid *దత్తాంశం)
    {
        బఫరు_Sub_దత్తాంశం = (PFNBUFFERSUBDATAPROC)Int_Get_Proc_చిరునామా("glBufferSubData");
        బఫరు_Sub_దత్తాంశం(target, offset, size, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_బఫరు_Sub_దత్తాంశం(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *దత్తాంశం)
    {
        Get_బఫరు_Sub_దత్తాంశం = (PFNGETBUFFERSUBDATAPROC)Int_Get_Proc_చిరునామా("glGetBufferSubData");
        Get_బఫరు_Sub_దత్తాంశం(target, offset, size, దత్తాంశం);
    }

    స్థిర GLvoid* CODEGEN_FUNCPTR Switch_Map_బఫరు(GLenum target, GLenum access)
    {
        Map_బఫరు = (PFNMAPBUFFERPROC)Int_Get_Proc_చిరునామా("glMapBuffer");
        తిరిగి Map_బఫరు(target, access);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_Unmap_బఫరు(GLenum target)
    {
        Unmap_బఫరు = (PFNUNMAPBUFFERPROC)Int_Get_Proc_చిరునామా("glUnmapBuffer");
        తిరిగి Unmap_బఫరు(target);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_బఫరు_Parameteriv(GLenum target, GLenum pname, GLint *params)
    {
        Get_బఫరు_Parameteriv = (PFNGETBUFFERPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glGetBufferParameteriv");
        Get_బఫరు_Parameteriv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_బఫరు_Pointerv(GLenum target, GLenum pname, GLvoid* *params)
    {
        Get_బఫరు_Pointerv = (PFNGETBUFFERPOINTERVPROC)Int_Get_Proc_చిరునామా("glGetBufferPointerv");
        Get_బఫరు_Pointerv(target, pname, params);
    }

    // Extension: 2.0

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BlendEquationSeparate(GLenum బహుళకం_RGB, GLenum బహుళకం_Alpha)
    {
        BlendEquationSeparate = (PFNBLENDEQUATIONSEPARATEPROC)Int_Get_Proc_చిరునామా("glBlendEquationSeparate");
        BlendEquationSeparate(బహుళకం_RGB, బహుళకం_Alpha);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DrawBuffers(GLsizei n, స్థిరరాశి GLenum *bufs)
    {
        DrawBuffers = (PFNDRAWBUFFERSPROC)Int_Get_Proc_చిరునామా("glDrawBuffers");
        DrawBuffers(n, bufs);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
    {
        StencilOpSeparate = (PFNSTENCILOPSEPARATEPROC)Int_Get_Proc_చిరునామా("glStencilOpSeparate");
        StencilOpSeparate(face, sfail, dpfail, dppass);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint ముసుగు)
    {
        StencilFuncSeparate = (PFNSTENCILFUNCSEPARATEPROC)Int_Get_Proc_చిరునామా("glStencilFuncSeparate");
        StencilFuncSeparate(face, func, ref, ముసుగు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Stencil_ముసుగు_Separate(GLenum face, GLuint ముసుగు)
    {
        Stencil_ముసుగు_Separate = (PFNSTENCILMASKSEPARATEPROC)Int_Get_Proc_చిరునామా("glStencilMaskSeparate");
        Stencil_ముసుగు_Separate(face, ముసుగు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_AttachShader(GLuint కార్యక్రమం, GLuint shader)
    {
        AttachShader = (PFNATTACHSHADERPROC)Int_Get_Proc_చిరునామా("glAttachShader");
        AttachShader(కార్యక్రమం, shader);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BindAttribLocation(GLuint కార్యక్రమం, GLuint సూచీ, స్థిరరాశి GLchar *name)
    {
        BindAttribLocation = (PFNBINDATTRIBLOCATIONPROC)Int_Get_Proc_చిరునామా("glBindAttribLocation");
        BindAttribLocation(కార్యక్రమం, సూచీ, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_CompileShader(GLuint shader)
    {
        CompileShader = (PFNCOMPILESHADERPROC)Int_Get_Proc_చిరునామా("glCompileShader");
        CompileShader(shader);
    }

    స్థిర GLuint CODEGEN_FUNCPTR Switch_Create_కార్యక్రమం()
    {
        Create_కార్యక్రమం = (PFNCREATEPROGRAMPROC)Int_Get_Proc_చిరునామా("glCreateProgram");
        తిరిగి Create_కార్యక్రమం();
    }

    స్థిర GLuint CODEGEN_FUNCPTR Switch_CreateShader(GLenum type)
    {
        CreateShader = (PFNCREATESHADERPROC)Int_Get_Proc_చిరునామా("glCreateShader");
        తిరిగి CreateShader(type);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Delete_కార్యక్రమం(GLuint కార్యక్రమం)
    {
        Delete_కార్యక్రమం = (PFNDELETEPROGRAMPROC)Int_Get_Proc_చిరునామా("glDeleteProgram");
        Delete_కార్యక్రమం(కార్యక్రమం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DeleteShader(GLuint shader)
    {
        DeleteShader = (PFNDELETESHADERPROC)Int_Get_Proc_చిరునామా("glDeleteShader");
        DeleteShader(shader);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DetachShader(GLuint కార్యక్రమం, GLuint shader)
    {
        DetachShader = (PFNDETACHSHADERPROC)Int_Get_Proc_చిరునామా("glDetachShader");
        DetachShader(కార్యక్రమం, shader);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Disable_శీర్షం_Attrib_Array(GLuint సూచీ)
    {
        Disable_శీర్షం_Attrib_Array = (PFNDISABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_చిరునామా("glDisableVertexAttribArray");
        Disable_శీర్షం_Attrib_Array(సూచీ);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Enable_శీర్షం_Attrib_Array(GLuint సూచీ)
    {
        Enable_శీర్షం_Attrib_Array = (PFNENABLEVERTEXATTRIBARRAYPROC)Int_Get_Proc_చిరునామా("glEnableVertexAttribArray");
        Enable_శీర్షం_Attrib_Array(సూచీ);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_చురుకైన_Attrib(GLuint కార్యక్రమం, GLuint సూచీ, GLsizei buf_కొలత, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_చురుకైన_Attrib = (PFNGETACTIVEATTRIBPROC)Int_Get_Proc_చిరునామా("glGetActiveAttrib");
        Get_చురుకైన_Attrib(కార్యక్రమం, సూచీ, buf_కొలత, length, size, type, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_చురుకైన_ఏకరీతి(GLuint కార్యక్రమం, GLuint సూచీ, GLsizei buf_కొలత, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
    {
        Get_చురుకైన_ఏకరీతి = (PFNGETACTIVEUNIFORMPROC)Int_Get_Proc_చిరునామా("glGetActiveUniform");
        Get_చురుకైన_ఏకరీతి(కార్యక్రమం, సూచీ, buf_కొలత, length, size, type, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetAttachedShaders(GLuint కార్యక్రమం, GLsizei max_లెక్క, GLsizei *లెక్క_2, GLuint *obj)
    {
        GetAttachedShaders = (PFNGETATTACHEDSHADERSPROC)Int_Get_Proc_చిరునామా("glGetAttachedShaders");
        GetAttachedShaders(కార్యక్రమం, max_లెక్క, లెక్క_2, obj);
    }

    స్థిర GLint CODEGEN_FUNCPTR Switch_GetAttribLocation(GLuint కార్యక్రమం, స్థిరరాశి GLchar *name)
    {
        GetAttribLocation = (PFNGETATTRIBLOCATIONPROC)Int_Get_Proc_చిరునామా("glGetAttribLocation");
        తిరిగి GetAttribLocation(కార్యక్రమం, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetProgramiv(GLuint కార్యక్రమం, GLenum pname, GLint *params)
    {
        GetProgramiv = (PFNGETPROGRAMIVPROC)Int_Get_Proc_చిరునామా("glGetProgramiv");
        GetProgramiv(కార్యక్రమం, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_కార్యక్రమం_Info_Log(GLuint కార్యక్రమం, GLsizei buf_కొలత, GLsizei *length, GLchar *infoLog)
    {
        Get_కార్యక్రమం_Info_Log = (PFNGETPROGRAMINFOLOGPROC)Int_Get_Proc_చిరునామా("glGetProgramInfoLog");
        Get_కార్యక్రమం_Info_Log(కార్యక్రమం, buf_కొలత, length, infoLog);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetShaderiv(GLuint shader, GLenum pname, GLint *params)
    {
        GetShaderiv = (PFNGETSHADERIVPROC)Int_Get_Proc_చిరునామా("glGetShaderiv");
        GetShaderiv(shader, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetShaderInfoLog(GLuint shader, GLsizei buf_కొలత, GLsizei *length, GLchar *infoLog)
    {
        GetShaderInfoLog = (PFNGETSHADERINFOLOGPROC)Int_Get_Proc_చిరునామా("glGetShaderInfoLog");
        GetShaderInfoLog(shader, buf_కొలత, length, infoLog);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_Shader_ఆధారం(GLuint shader, GLsizei buf_కొలత, GLsizei *length, GLchar *ఆధారం)
    {
        Get_Shader_ఆధారం = (PFNGETSHADERSOURCEPROC)Int_Get_Proc_చిరునామా("glGetShaderSource");
        Get_Shader_ఆధారం(shader, buf_కొలత, length, ఆధారం);
    }

    స్థిర GLint CODEGEN_FUNCPTR Switch_Get_ఏకరీతి_Location(GLuint కార్యక్రమం, స్థిరరాశి GLchar *name)
    {
        Get_ఏకరీతి_Location = (PFNGETUNIFORMLOCATIONPROC)Int_Get_Proc_చిరునామా("glGetUniformLocation");
        తిరిగి Get_ఏకరీతి_Location(కార్యక్రమం, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetUniformfv(GLuint కార్యక్రమం, GLint location, GLfloat *params)
    {
        GetUniformfv = (PFNGETUNIFORMFVPROC)Int_Get_Proc_చిరునామా("glGetUniformfv");
        GetUniformfv(కార్యక్రమం, location, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetUniformiv(GLuint కార్యక్రమం, GLint location, GLint *params)
    {
        GetUniformiv = (PFNGETUNIFORMIVPROC)Int_Get_Proc_చిరునామా("glGetUniformiv");
        GetUniformiv(కార్యక్రమం, location, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_శీర్షం_Attribdv(GLuint సూచీ, GLenum pname, GLdouble *params)
    {
        Get_శీర్షం_Attribdv = (PFNGETVERTEXATTRIBDVPROC)Int_Get_Proc_చిరునామా("glGetVertexAttribdv");
        Get_శీర్షం_Attribdv(సూచీ, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_శీర్షం_Attribfv(GLuint సూచీ, GLenum pname, GLfloat *params)
    {
        Get_శీర్షం_Attribfv = (PFNGETVERTEXATTRIBFVPROC)Int_Get_Proc_చిరునామా("glGetVertexAttribfv");
        Get_శీర్షం_Attribfv(సూచీ, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_శీర్షం_Attribiv(GLuint సూచీ, GLenum pname, GLint *params)
    {
        Get_శీర్షం_Attribiv = (PFNGETVERTEXATTRIBIVPROC)Int_Get_Proc_చిరునామా("glGetVertexAttribiv");
        Get_శీర్షం_Attribiv(సూచీ, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_శీర్షం_Attrib_Pointerv(GLuint సూచీ, GLenum pname, GLvoid* *pointer)
    {
        Get_శీర్షం_Attrib_Pointerv = (PFNGETVERTEXATTRIBPOINTERVPROC)Int_Get_Proc_చిరునామా("glGetVertexAttribPointerv");
        Get_శీర్షం_Attrib_Pointerv(సూచీ, pname, pointer);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_Is_కార్యక్రమం(GLuint కార్యక్రమం)
    {
        Is_కార్యక్రమం = (PFNISPROGRAMPROC)Int_Get_Proc_చిరునామా("glIsProgram");
        తిరిగి Is_కార్యక్రమం(కార్యక్రమం);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsShader(GLuint shader)
    {
        IsShader = (PFNISSHADERPROC)Int_Get_Proc_చిరునామా("glIsShader");
        తిరిగి IsShader(shader);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Link_కార్యక్రమం(GLuint కార్యక్రమం)
    {
        Link_కార్యక్రమం = (PFNLINKPROGRAMPROC)Int_Get_Proc_చిరునామా("glLinkProgram");
        Link_కార్యక్రమం(కార్యక్రమం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Shader_ఆధారం(GLuint shader, GLsizei లెక్క_2, స్థిరరాశి GLchar* స్థిరరాశి *string, స్థిరరాశి GLint *length)
    {
        Shader_ఆధారం = (PFNSHADERSOURCEPROC)Int_Get_Proc_చిరునామా("glShaderSource");
        Shader_ఆధారం(shader, లెక్క_2, string, length);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Use_కార్యక్రమం(GLuint కార్యక్రమం)
    {
        Use_కార్యక్రమం = (PFNUSEPROGRAMPROC)Int_Get_Proc_చిరునామా("glUseProgram");
        Use_కార్యక్రమం(కార్యక్రమం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform1f(GLint location, GLfloat v0)
    {
        Uniform1f = (PFNUNIFORM1FPROC)Int_Get_Proc_చిరునామా("glUniform1f");
        Uniform1f(location, v0);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform2f(GLint location, GLfloat v0, GLfloat v1)
    {
        Uniform2f = (PFNUNIFORM2FPROC)Int_Get_Proc_చిరునామా("glUniform2f");
        Uniform2f(location, v0, v1);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
    {
        Uniform3f = (PFNUNIFORM3FPROC)Int_Get_Proc_చిరునామా("glUniform3f");
        Uniform3f(location, v0, v1, v2);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
    {
        Uniform4f = (PFNUNIFORM4FPROC)Int_Get_Proc_చిరునామా("glUniform4f");
        Uniform4f(location, v0, v1, v2, v3);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform1i(GLint location, GLint v0)
    {
        Uniform1i = (PFNUNIFORM1IPROC)Int_Get_Proc_చిరునామా("glUniform1i");
        Uniform1i(location, v0);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform2i(GLint location, GLint v0, GLint v1)
    {
        Uniform2i = (PFNUNIFORM2IPROC)Int_Get_Proc_చిరునామా("glUniform2i");
        Uniform2i(location, v0, v1);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
    {
        Uniform3i = (PFNUNIFORM3IPROC)Int_Get_Proc_చిరునామా("glUniform3i");
        Uniform3i(location, v0, v1, v2);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
    {
        Uniform4i = (PFNUNIFORM4IPROC)Int_Get_Proc_చిరునామా("glUniform4i");
        Uniform4i(location, v0, v1, v2, v3);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform1fv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value)
    {
        Uniform1fv = (PFNUNIFORM1FVPROC)Int_Get_Proc_చిరునామా("glUniform1fv");
        Uniform1fv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform2fv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value)
    {
        Uniform2fv = (PFNUNIFORM2FVPROC)Int_Get_Proc_చిరునామా("glUniform2fv");
        Uniform2fv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform3fv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value)
    {
        Uniform3fv = (PFNUNIFORM3FVPROC)Int_Get_Proc_చిరునామా("glUniform3fv");
        Uniform3fv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform4fv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLfloat *value)
    {
        Uniform4fv = (PFNUNIFORM4FVPROC)Int_Get_Proc_చిరునామా("glUniform4fv");
        Uniform4fv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform1iv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value)
    {
        Uniform1iv = (PFNUNIFORM1IVPROC)Int_Get_Proc_చిరునామా("glUniform1iv");
        Uniform1iv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform2iv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value)
    {
        Uniform2iv = (PFNUNIFORM2IVPROC)Int_Get_Proc_చిరునామా("glUniform2iv");
        Uniform2iv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform3iv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value)
    {
        Uniform3iv = (PFNUNIFORM3IVPROC)Int_Get_Proc_చిరునామా("glUniform3iv");
        Uniform3iv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform4iv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLint *value)
    {
        Uniform4iv = (PFNUNIFORM4IVPROC)Int_Get_Proc_చిరునామా("glUniform4iv");
        Uniform4iv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix2fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix2fv = (PFNUNIFORMMATRIX2FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix2fv");
        ఏకరీతి_Matrix2fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix3fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix3fv = (PFNUNIFORMMATRIX3FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix3fv");
        ఏకరీతి_Matrix3fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix4fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix4fv = (PFNUNIFORMMATRIX4FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix4fv");
        ఏకరీతి_Matrix4fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Validate_కార్యక్రమం(GLuint కార్యక్రమం)
    {
        Validate_కార్యక్రమం = (PFNVALIDATEPROGRAMPROC)Int_Get_Proc_చిరునామా("glValidateProgram");
        Validate_కార్యక్రమం(కార్యక్రమం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_సూచిక(GLuint సూచీ, GLint size, GLenum type, GLboolean normalized, GLsizei stride, స్థిరరాశి GLvoid *pointer)
    {
        శీర్షం_Attrib_సూచిక = (PFNVERTEXATTRIBPOINTERPROC)Int_Get_Proc_చిరునామా("glVertexAttribPointer");
        శీర్షం_Attrib_సూచిక(సూచీ, size, type, normalized, stride, pointer);
    }

    // Extension: 2.1

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix2x3fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix2x3fv = (PFNUNIFORMMATRIX2X3FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix2x3fv");
        ఏకరీతి_Matrix2x3fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix3x2fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix3x2fv = (PFNUNIFORMMATRIX3X2FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix3x2fv");
        ఏకరీతి_Matrix3x2fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix2x4fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix2x4fv = (PFNUNIFORMMATRIX2X4FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix2x4fv");
        ఏకరీతి_Matrix2x4fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix4x2fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix4x2fv = (PFNUNIFORMMATRIX4X2FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix4x2fv");
        ఏకరీతి_Matrix4x2fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix3x4fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix3x4fv = (PFNUNIFORMMATRIX3X4FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix3x4fv");
        ఏకరీతి_Matrix3x4fv(location, లెక్క_2, transpose, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_Matrix4x3fv(GLint location, GLsizei లెక్క_2, GLboolean transpose, స్థిరరాశి GLfloat *value)
    {
        ఏకరీతి_Matrix4x3fv = (PFNUNIFORMMATRIX4X3FVPROC)Int_Get_Proc_చిరునామా("glUniformMatrix4x3fv");
        ఏకరీతి_Matrix4x3fv(location, లెక్క_2, transpose, value);
    }

    // Extension: ARB_vertex_array_object

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Bind_శీర్షం_Array(GLuint ren_array)
    {
        Bind_శీర్షం_Array = (PFNBINDVERTEXARRAYPROC)Int_Get_Proc_చిరునామా("glBindVertexArray");
        Bind_శీర్షం_Array(ren_array);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Delete_శీర్షం_Arrays(GLsizei n, స్థిరరాశి GLuint *arrays)
    {
        Delete_శీర్షం_Arrays = (PFNDELETEVERTEXARRAYSPROC)Int_Get_Proc_చిరునామా("glDeleteVertexArrays");
        Delete_శీర్షం_Arrays(n, arrays);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Gen_శీర్షం_Arrays(GLsizei n, GLuint *arrays)
    {
        Gen_శీర్షం_Arrays = (PFNGENVERTEXARRAYSPROC)Int_Get_Proc_చిరునామా("glGenVertexArrays");
        Gen_శీర్షం_Arrays(n, arrays);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_Is_శీర్షం_Array(GLuint ren_array)
    {
        Is_శీర్షం_Array = (PFNISVERTEXARRAYPROC)Int_Get_Proc_చిరునామా("glIsVertexArray");
        తిరిగి Is_శీర్షం_Array(ren_array);
    }

    // Extension: ARB_map_buffer_range

    స్థిర GLvoid* CODEGEN_FUNCPTR Switch_Map_బఫరు_Range(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
    {
        Map_బఫరు_Range = (PFNMAPBUFFERRANGEPROC)Int_Get_Proc_చిరునామా("glMapBufferRange");
        తిరిగి Map_బఫరు_Range(target, offset, length, access);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Flush_Mapped_బఫరు_Range(GLenum target, GLintptr offset, GLsizeiptr length)
    {
        Flush_Mapped_బఫరు_Range = (PFNFLUSHMAPPEDBUFFERRANGEPROC)Int_Get_Proc_చిరునామా("glFlushMappedBufferRange");
        Flush_Mapped_బఫరు_Range(target, offset, length);
    }

    // Extension: ARB_framebuffer_object

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsRenderbuffer(GLuint renderbuffer)
    {
        IsRenderbuffer = (PFNISRENDERBUFFERPROC)Int_Get_Proc_చిరునామా("glIsRenderbuffer");
        తిరిగి IsRenderbuffer(renderbuffer);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BindRenderbuffer(GLenum target, GLuint renderbuffer)
    {
        BindRenderbuffer = (PFNBINDRENDERBUFFERPROC)Int_Get_Proc_చిరునామా("glBindRenderbuffer");
        BindRenderbuffer(target, renderbuffer);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DeleteRenderbuffers(GLsizei n, స్థిరరాశి GLuint *renderbuffers)
    {
        DeleteRenderbuffers = (PFNDELETERENDERBUFFERSPROC)Int_Get_Proc_చిరునామా("glDeleteRenderbuffers");
        DeleteRenderbuffers(n, renderbuffers);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GenRenderbuffers(GLsizei n, GLuint *renderbuffers)
    {
        GenRenderbuffers = (PFNGENRENDERBUFFERSPROC)Int_Get_Proc_చిరునామా("glGenRenderbuffers");
        GenRenderbuffers(n, renderbuffers);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు)
    {
        RenderbufferStorage = (PFNRENDERBUFFERSTORAGEPROC)Int_Get_Proc_చిరునామా("glRenderbufferStorage");
        RenderbufferStorage(target, internalformat, వెడల్పు, ఎత్తు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params)
    {
        GetRenderbufferParameteriv = (PFNGETRENDERBUFFERPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glGetRenderbufferParameteriv");
        GetRenderbufferParameteriv(target, pname, params);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsFramebuffer(GLuint framebuffer)
    {
        IsFramebuffer = (PFNISFRAMEBUFFERPROC)Int_Get_Proc_చిరునామా("glIsFramebuffer");
        తిరిగి IsFramebuffer(framebuffer);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BindFramebuffer(GLenum target, GLuint framebuffer)
    {
        BindFramebuffer = (PFNBINDFRAMEBUFFERPROC)Int_Get_Proc_చిరునామా("glBindFramebuffer");
        BindFramebuffer(target, framebuffer);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DeleteFramebuffers(GLsizei n, స్థిరరాశి GLuint *framebuffers)
    {
        DeleteFramebuffers = (PFNDELETEFRAMEBUFFERSPROC)Int_Get_Proc_చిరునామా("glDeleteFramebuffers");
        DeleteFramebuffers(n, framebuffers);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GenFramebuffers(GLsizei n, GLuint *framebuffers)
    {
        GenFramebuffers = (PFNGENFRAMEBUFFERSPROC)Int_Get_Proc_చిరునామా("glGenFramebuffers");
        GenFramebuffers(n, framebuffers);
    }

    స్థిర GLenum CODEGEN_FUNCPTR Switch_Check_Framebuffer_స్థితి(GLenum target)
    {
        Check_Framebuffer_స్థితి = (PFNCHECKFRAMEBUFFERSTATUSPROC)Int_Get_Proc_చిరునామా("glCheckFramebufferStatus");
        తిరిగి Check_Framebuffer_స్థితి(target);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_FramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture1D = (PFNFRAMEBUFFERTEXTURE1DPROC)Int_Get_Proc_చిరునామా("glFramebufferTexture1D");
        FramebufferTexture1D(target, attachment, textarget, texture, level);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
    {
        FramebufferTexture2D = (PFNFRAMEBUFFERTEXTURE2DPROC)Int_Get_Proc_చిరునామా("glFramebufferTexture2D");
        FramebufferTexture2D(target, attachment, textarget, texture, level);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_FramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)
    {
        FramebufferTexture3D = (PFNFRAMEBUFFERTEXTURE3DPROC)Int_Get_Proc_చిరునామా("glFramebufferTexture3D");
        FramebufferTexture3D(target, attachment, textarget, texture, level, zoffset);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
    {
        FramebufferRenderbuffer = (PFNFRAMEBUFFERRENDERBUFFERPROC)Int_Get_Proc_చిరునామా("glFramebufferRenderbuffer");
        FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params)
    {
        GetFramebufferAttachmentParameteriv = (PFNGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)Int_Get_Proc_చిరునామా("glGetFramebufferAttachmentParameteriv");
        GetFramebufferAttachmentParameteriv(target, attachment, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GenerateMipmap(GLenum target)
    {
        GenerateMipmap = (PFNGENERATEMIPMAPPROC)Int_Get_Proc_చిరునామా("glGenerateMipmap");
        GenerateMipmap(target);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield ముసుగు, GLenum filter)
    {
        BlitFramebuffer = (PFNBLITFRAMEBUFFERPROC)Int_Get_Proc_చిరునామా("glBlitFramebuffer");
        BlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, ముసుగు, filter);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_RenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei వెడల్పు, GLsizei ఎత్తు)
    {
        RenderbufferStorageMultisample = (PFNRENDERBUFFERSTORAGEMULTISAMPLEPROC)Int_Get_Proc_చిరునామా("glRenderbufferStorageMultisample");
        RenderbufferStorageMultisample(target, samples, internalformat, వెడల్పు, ఎత్తు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_FramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)
    {
        FramebufferTextureLayer = (PFNFRAMEBUFFERTEXTURELAYERPROC)Int_Get_Proc_చిరునామా("glFramebufferTextureLayer");
        FramebufferTextureLayer(target, attachment, texture, level, layer);
    }

    // Extension: 3.0

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ColorMaski(GLuint సూచీ, GLboolean r, GLboolean g, GLboolean b, GLboolean a)
    {
        ColorMaski = (PFNCOLORMASKIPROC)Int_Get_Proc_చిరునామా("glColorMaski");
        ColorMaski(సూచీ, r, g, b, a);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetBooleani_v(GLenum target, GLuint సూచీ, GLboolean *దత్తాంశం)
    {
        GetBooleani_v = (PFNGETBOOLEANI_VPROC)Int_Get_Proc_చిరునామా("glGetBooleani_v");
        GetBooleani_v(target, సూచీ, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetIntegeri_v(GLenum target, GLuint సూచీ, GLint *దత్తాంశం)
    {
        GetIntegeri_v = (PFNGETINTEGERI_VPROC)Int_Get_Proc_చిరునామా("glGetIntegeri_v");
        GetIntegeri_v(target, సూచీ, దత్తాంశం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Enablei(GLenum target, GLuint సూచీ)
    {
        Enablei = (PFNENABLEIPROC)Int_Get_Proc_చిరునామా("glEnablei");
        Enablei(target, సూచీ);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Disablei(GLenum target, GLuint సూచీ)
    {
        Disablei = (PFNDISABLEIPROC)Int_Get_Proc_చిరునామా("glDisablei");
        Disablei(target, సూచీ);
    }

    స్థిర GLboolean CODEGEN_FUNCPTR Switch_IsEnabledi(GLenum target, GLuint సూచీ)
    {
        IsEnabledi = (PFNISENABLEDIPROC)Int_Get_Proc_చిరునామా("glIsEnabledi");
        తిరిగి IsEnabledi(target, సూచీ);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BeginTransformFeedback(GLenum primitive_బహుళకం)
    {
        BeginTransformFeedback = (PFNBEGINTRANSFORMFEEDBACKPROC)Int_Get_Proc_చిరునామా("glBeginTransformFeedback");
        BeginTransformFeedback(primitive_బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_చివర_Transform_Feedback()
    {
        చివర_Transform_Feedback = (PFNENDTRANSFORMFEEDBACKPROC)Int_Get_Proc_చిరునామా("glEndTransformFeedback");
        చివర_Transform_Feedback();
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Bind_బఫరు_Range(GLenum target, GLuint సూచీ, GLuint బఫరు, GLintptr offset, GLsizeiptr size)
    {
        Bind_బఫరు_Range = (PFNBINDBUFFERRANGEPROC)Int_Get_Proc_చిరునామా("glBindBufferRange");
        Bind_బఫరు_Range(target, సూచీ, బఫరు, offset, size);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Bind_బఫరు_పునాది(GLenum target, GLuint సూచీ, GLuint బఫరు)
    {
        Bind_బఫరు_పునాది = (PFNBINDBUFFERBASEPROC)Int_Get_Proc_చిరునామా("glBindBufferBase");
        Bind_బఫరు_పునాది(target, సూచీ, బఫరు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TransformFeedbackVaryings(GLuint కార్యక్రమం, GLsizei లెక్క_2, స్థిరరాశి GLchar* స్థిరరాశి *varyings, GLenum బఫరు_బహుళకం)
    {
        TransformFeedbackVaryings = (PFNTRANSFORMFEEDBACKVARYINGSPROC)Int_Get_Proc_చిరునామా("glTransformFeedbackVaryings");
        TransformFeedbackVaryings(కార్యక్రమం, లెక్క_2, varyings, బఫరు_బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTransformFeedbackVarying(GLuint కార్యక్రమం, GLuint సూచీ, GLsizei buf_కొలత, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name)
    {
        GetTransformFeedbackVarying = (PFNGETTRANSFORMFEEDBACKVARYINGPROC)Int_Get_Proc_చిరునామా("glGetTransformFeedbackVarying");
        GetTransformFeedbackVarying(కార్యక్రమం, సూచీ, buf_కొలత, length, size, type, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClampColor(GLenum target, GLenum clamp)
    {
        ClampColor = (PFNCLAMPCOLORPROC)Int_Get_Proc_చిరునామా("glClampColor");
        ClampColor(target, clamp);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_BeginConditionalRender(GLuint id, GLenum బహుళకం)
    {
        BeginConditionalRender = (PFNBEGINCONDITIONALRENDERPROC)Int_Get_Proc_చిరునామా("glBeginConditionalRender");
        BeginConditionalRender(id, బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_చివర_Conditional_Render()
    {
        చివర_Conditional_Render = (PFNENDCONDITIONALRENDERPROC)Int_Get_Proc_చిరునామా("glEndConditionalRender");
        చివర_Conditional_Render();
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_IPointer(GLuint సూచీ, GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *pointer)
    {
        శీర్షం_Attrib_IPointer = (PFNVERTEXATTRIBIPOINTERPROC)Int_Get_Proc_చిరునామా("glVertexAttribIPointer");
        శీర్షం_Attrib_IPointer(సూచీ, size, type, stride, pointer);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_శీర్షం_Attrib_Iiv(GLuint సూచీ, GLenum pname, GLint *params)
    {
        Get_శీర్షం_Attrib_Iiv = (PFNGETVERTEXATTRIBIIVPROC)Int_Get_Proc_చిరునామా("glGetVertexAttribIiv");
        Get_శీర్షం_Attrib_Iiv(సూచీ, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_శీర్షం_Attrib_Iuiv(GLuint సూచీ, GLenum pname, GLuint *params)
    {
        Get_శీర్షం_Attrib_Iuiv = (PFNGETVERTEXATTRIBIUIVPROC)Int_Get_Proc_చిరునామా("glGetVertexAttribIuiv");
        Get_శీర్షం_Attrib_Iuiv(సూచీ, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I1i(GLuint సూచీ, GLint x)
    {
        శీర్షం_Attrib_I1i = (PFNVERTEXATTRIBI1IPROC)Int_Get_Proc_చిరునామా("glVertexAttribI1i");
        శీర్షం_Attrib_I1i(సూచీ, x);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I2i(GLuint సూచీ, GLint x, GLint y)
    {
        శీర్షం_Attrib_I2i = (PFNVERTEXATTRIBI2IPROC)Int_Get_Proc_చిరునామా("glVertexAttribI2i");
        శీర్షం_Attrib_I2i(సూచీ, x, y);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I3i(GLuint సూచీ, GLint x, GLint y, GLint z)
    {
        శీర్షం_Attrib_I3i = (PFNVERTEXATTRIBI3IPROC)Int_Get_Proc_చిరునామా("glVertexAttribI3i");
        శీర్షం_Attrib_I3i(సూచీ, x, y, z);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4i(GLuint సూచీ, GLint x, GLint y, GLint z, GLint w)
    {
        శీర్షం_Attrib_I4i = (PFNVERTEXATTRIBI4IPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4i");
        శీర్షం_Attrib_I4i(సూచీ, x, y, z, w);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I1ui(GLuint సూచీ, GLuint x)
    {
        శీర్షం_Attrib_I1ui = (PFNVERTEXATTRIBI1UIPROC)Int_Get_Proc_చిరునామా("glVertexAttribI1ui");
        శీర్షం_Attrib_I1ui(సూచీ, x);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I2ui(GLuint సూచీ, GLuint x, GLuint y)
    {
        శీర్షం_Attrib_I2ui = (PFNVERTEXATTRIBI2UIPROC)Int_Get_Proc_చిరునామా("glVertexAttribI2ui");
        శీర్షం_Attrib_I2ui(సూచీ, x, y);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I3ui(GLuint సూచీ, GLuint x, GLuint y, GLuint z)
    {
        శీర్షం_Attrib_I3ui = (PFNVERTEXATTRIBI3UIPROC)Int_Get_Proc_చిరునామా("glVertexAttribI3ui");
        శీర్షం_Attrib_I3ui(సూచీ, x, y, z);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4ui(GLuint సూచీ, GLuint x, GLuint y, GLuint z, GLuint w)
    {
        శీర్షం_Attrib_I4ui = (PFNVERTEXATTRIBI4UIPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4ui");
        శీర్షం_Attrib_I4ui(సూచీ, x, y, z, w);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I1iv(GLuint సూచీ, స్థిరరాశి GLint *v)
    {
        శీర్షం_Attrib_I1iv = (PFNVERTEXATTRIBI1IVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI1iv");
        శీర్షం_Attrib_I1iv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I2iv(GLuint సూచీ, స్థిరరాశి GLint *v)
    {
        శీర్షం_Attrib_I2iv = (PFNVERTEXATTRIBI2IVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI2iv");
        శీర్షం_Attrib_I2iv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I3iv(GLuint సూచీ, స్థిరరాశి GLint *v)
    {
        శీర్షం_Attrib_I3iv = (PFNVERTEXATTRIBI3IVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI3iv");
        శీర్షం_Attrib_I3iv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4iv(GLuint సూచీ, స్థిరరాశి GLint *v)
    {
        శీర్షం_Attrib_I4iv = (PFNVERTEXATTRIBI4IVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4iv");
        శీర్షం_Attrib_I4iv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I1uiv(GLuint సూచీ, స్థిరరాశి GLuint *v)
    {
        శీర్షం_Attrib_I1uiv = (PFNVERTEXATTRIBI1UIVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI1uiv");
        శీర్షం_Attrib_I1uiv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I2uiv(GLuint సూచీ, స్థిరరాశి GLuint *v)
    {
        శీర్షం_Attrib_I2uiv = (PFNVERTEXATTRIBI2UIVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI2uiv");
        శీర్షం_Attrib_I2uiv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I3uiv(GLuint సూచీ, స్థిరరాశి GLuint *v)
    {
        శీర్షం_Attrib_I3uiv = (PFNVERTEXATTRIBI3UIVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI3uiv");
        శీర్షం_Attrib_I3uiv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4uiv(GLuint సూచీ, స్థిరరాశి GLuint *v)
    {
        శీర్షం_Attrib_I4uiv = (PFNVERTEXATTRIBI4UIVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4uiv");
        శీర్షం_Attrib_I4uiv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4bv(GLuint సూచీ, స్థిరరాశి GLbyte *v)
    {
        శీర్షం_Attrib_I4bv = (PFNVERTEXATTRIBI4BVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4bv");
        శీర్షం_Attrib_I4bv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4sv(GLuint సూచీ, స్థిరరాశి GLshort *v)
    {
        శీర్షం_Attrib_I4sv = (PFNVERTEXATTRIBI4SVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4sv");
        శీర్షం_Attrib_I4sv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4ubv(GLuint సూచీ, స్థిరరాశి GLubyte *v)
    {
        శీర్షం_Attrib_I4ubv = (PFNVERTEXATTRIBI4UBVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4ubv");
        శీర్షం_Attrib_I4ubv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_Attrib_I4usv(GLuint సూచీ, స్థిరరాశి GLushort *v)
    {
        శీర్షం_Attrib_I4usv = (PFNVERTEXATTRIBI4USVPROC)Int_Get_Proc_చిరునామా("glVertexAttribI4usv");
        శీర్షం_Attrib_I4usv(సూచీ, v);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetUniformuiv(GLuint కార్యక్రమం, GLint location, GLuint *params)
    {
        GetUniformuiv = (PFNGETUNIFORMUIVPROC)Int_Get_Proc_చిరునామా("glGetUniformuiv");
        GetUniformuiv(కార్యక్రమం, location, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Bind_Frag_దత్తాంశం_Location(GLuint కార్యక్రమం, GLuint color, స్థిరరాశి GLchar *name)
    {
        Bind_Frag_దత్తాంశం_Location = (PFNBINDFRAGDATALOCATIONPROC)Int_Get_Proc_చిరునామా("glBindFragDataLocation");
        Bind_Frag_దత్తాంశం_Location(కార్యక్రమం, color, name);
    }

    స్థిర GLint CODEGEN_FUNCPTR Switch_Get_Frag_దత్తాంశం_Location(GLuint కార్యక్రమం, స్థిరరాశి GLchar *name)
    {
        Get_Frag_దత్తాంశం_Location = (PFNGETFRAGDATALOCATIONPROC)Int_Get_Proc_చిరునామా("glGetFragDataLocation");
        తిరిగి Get_Frag_దత్తాంశం_Location(కార్యక్రమం, name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform1ui(GLint location, GLuint v0)
    {
        Uniform1ui = (PFNUNIFORM1UIPROC)Int_Get_Proc_చిరునామా("glUniform1ui");
        Uniform1ui(location, v0);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform2ui(GLint location, GLuint v0, GLuint v1)
    {
        Uniform2ui = (PFNUNIFORM2UIPROC)Int_Get_Proc_చిరునామా("glUniform2ui");
        Uniform2ui(location, v0, v1);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
    {
        Uniform3ui = (PFNUNIFORM3UIPROC)Int_Get_Proc_చిరునామా("glUniform3ui");
        Uniform3ui(location, v0, v1, v2);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
    {
        Uniform4ui = (PFNUNIFORM4UIPROC)Int_Get_Proc_చిరునామా("glUniform4ui");
        Uniform4ui(location, v0, v1, v2, v3);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform1uiv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value)
    {
        Uniform1uiv = (PFNUNIFORM1UIVPROC)Int_Get_Proc_చిరునామా("glUniform1uiv");
        Uniform1uiv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform2uiv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value)
    {
        Uniform2uiv = (PFNUNIFORM2UIVPROC)Int_Get_Proc_చిరునామా("glUniform2uiv");
        Uniform2uiv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform3uiv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value)
    {
        Uniform3uiv = (PFNUNIFORM3UIVPROC)Int_Get_Proc_చిరునామా("glUniform3uiv");
        Uniform3uiv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Uniform4uiv(GLint location, GLsizei లెక్క_2, స్థిరరాశి GLuint *value)
    {
        Uniform4uiv = (PFNUNIFORM4UIVPROC)Int_Get_Proc_చిరునామా("glUniform4uiv");
        Uniform4uiv(location, లెక్క_2, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexParameterIiv(GLenum target, GLenum pname, స్థిరరాశి GLint *params)
    {
        TexParameterIiv = (PFNTEXPARAMETERIIVPROC)Int_Get_Proc_చిరునామా("glTexParameterIiv");
        TexParameterIiv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexParameterIuiv(GLenum target, GLenum pname, స్థిరరాశి GLuint *params)
    {
        TexParameterIuiv = (PFNTEXPARAMETERIUIVPROC)Int_Get_Proc_చిరునామా("glTexParameterIuiv");
        TexParameterIuiv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexParameterIiv(GLenum target, GLenum pname, GLint *params)
    {
        GetTexParameterIiv = (PFNGETTEXPARAMETERIIVPROC)Int_Get_Proc_చిరునామా("glGetTexParameterIiv");
        GetTexParameterIiv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_GetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params)
    {
        GetTexParameterIuiv = (PFNGETTEXPARAMETERIUIVPROC)Int_Get_Proc_చిరునామా("glGetTexParameterIuiv");
        GetTexParameterIuiv(target, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClearBufferiv(GLenum బఫరు, GLint drawbuffer, స్థిరరాశి GLint *value)
    {
        ClearBufferiv = (PFNCLEARBUFFERIVPROC)Int_Get_Proc_చిరునామా("glClearBufferiv");
        ClearBufferiv(బఫరు, drawbuffer, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClearBufferuiv(GLenum బఫరు, GLint drawbuffer, స్థిరరాశి GLuint *value)
    {
        ClearBufferuiv = (PFNCLEARBUFFERUIVPROC)Int_Get_Proc_చిరునామా("glClearBufferuiv");
        ClearBufferuiv(బఫరు, drawbuffer, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClearBufferfv(GLenum బఫరు, GLint drawbuffer, స్థిరరాశి GLfloat *value)
    {
        ClearBufferfv = (PFNCLEARBUFFERFVPROC)Int_Get_Proc_చిరునామా("glClearBufferfv");
        ClearBufferfv(బఫరు, drawbuffer, value);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ClearBufferfi(GLenum బఫరు, GLint drawbuffer, GLfloat లోతు, GLint stencil)
    {
        ClearBufferfi = (PFNCLEARBUFFERFIPROC)Int_Get_Proc_చిరునామా("glClearBufferfi");
        ClearBufferfi(బఫరు, drawbuffer, లోతు, stencil);
    }

    స్థిర స్థిరరాశి GLubyte * CODEGEN_FUNCPTR Switch_GetStringi(GLenum name, GLuint సూచీ)
    {
        GetStringi = (PFNGETSTRINGIPROC)Int_Get_Proc_చిరునామా("glGetStringi");
        తిరిగి GetStringi(name, సూచీ);
    }

    // Extension: ARB_uniform_buffer_object

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_ఏకరీతి_Indices(GLuint కార్యక్రమం, GLsizei ఏకరీతి_లెక్క, స్థిరరాశి GLchar* స్థిరరాశి *ఏకరీతి_పేర్లు, GLuint *ఏకరీతి_Indices)
    {
        Get_ఏకరీతి_Indices = (PFNGETUNIFORMINDICESPROC)Int_Get_Proc_చిరునామా("glGetUniformIndices");
        Get_ఏకరీతి_Indices(కార్యక్రమం, ఏకరీతి_లెక్క, ఏకరీతి_పేర్లు, ఏకరీతి_Indices);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_చురుకైన_Uniformsiv(GLuint కార్యక్రమం, GLsizei ఏకరీతి_లెక్క, స్థిరరాశి GLuint *ఏకరీతి_Indices, GLenum pname, GLint *params)
    {
        Get_చురుకైన_Uniformsiv = (PFNGETACTIVEUNIFORMSIVPROC)Int_Get_Proc_చిరునామా("glGetActiveUniformsiv");
        Get_చురుకైన_Uniformsiv(కార్యక్రమం, ఏకరీతి_లెక్క, ఏకరీతి_Indices, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_చురుకైన_ఏకరీతి_Name(GLuint కార్యక్రమం, GLuint ఏకరీతి_సూచీ, GLsizei buf_కొలత, GLsizei *length, GLchar *ఏకరీతి_Name)
    {
        Get_చురుకైన_ఏకరీతి_Name = (PFNGETACTIVEUNIFORMNAMEPROC)Int_Get_Proc_చిరునామా("glGetActiveUniformName");
        Get_చురుకైన_ఏకరీతి_Name(కార్యక్రమం, ఏకరీతి_సూచీ, buf_కొలత, length, ఏకరీతి_Name);
    }

    స్థిర GLuint CODEGEN_FUNCPTR Switch_Get_ఏకరీతి_ఖండం_సూచీ(GLuint కార్యక్రమం, స్థిరరాశి GLchar *ఏకరీతి_ఖండం_Name)
    {
        Get_ఏకరీతి_ఖండం_సూచీ = (PFNGETUNIFORMBLOCKINDEXPROC)Int_Get_Proc_చిరునామా("glGetUniformBlockIndex");
        తిరిగి Get_ఏకరీతి_ఖండం_సూచీ(కార్యక్రమం, ఏకరీతి_ఖండం_Name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_చురుకైన_ఏకరీతి_Blockiv(GLuint కార్యక్రమం, GLuint ఏకరీతి_ఖండం_సూచీ, GLenum pname, GLint *params)
    {
        Get_చురుకైన_ఏకరీతి_Blockiv = (PFNGETACTIVEUNIFORMBLOCKIVPROC)Int_Get_Proc_చిరునామా("glGetActiveUniformBlockiv");
        Get_చురుకైన_ఏకరీతి_Blockiv(కార్యక్రమం, ఏకరీతి_ఖండం_సూచీ, pname, params);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Get_చురుకైన_ఏకరీతి_ఖండం_Name(GLuint కార్యక్రమం, GLuint ఏకరీతి_ఖండం_సూచీ, GLsizei buf_కొలత, GLsizei *length, GLchar *ఏకరీతి_ఖండం_Name)
    {
        Get_చురుకైన_ఏకరీతి_ఖండం_Name = (PFNGETACTIVEUNIFORMBLOCKNAMEPROC)Int_Get_Proc_చిరునామా("glGetActiveUniformBlockName");
        Get_చురుకైన_ఏకరీతి_ఖండం_Name(కార్యక్రమం, ఏకరీతి_ఖండం_సూచీ, buf_కొలత, length, ఏకరీతి_ఖండం_Name);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఏకరీతి_ఖండం_Binding(GLuint కార్యక్రమం, GLuint ఏకరీతి_ఖండం_సూచీ, GLuint ఏకరీతి_ఖండం_Binding_2)
    {
        ఏకరీతి_ఖండం_Binding = (PFNUNIFORMBLOCKBINDINGPROC)Int_Get_Proc_చిరునామా("glUniformBlockBinding");
        ఏకరీతి_ఖండం_Binding(కార్యక్రమం, ఏకరీతి_ఖండం_సూచీ, ఏకరీతి_ఖండం_Binding_2);
    }

    // Extension: ARB_copy_buffer

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Copy_బఫరు_Sub_దత్తాంశం(GLenum read_లక్ష్యం, GLenum write_లక్ష్యం, GLintptr read_స్థానభ్రంశం, GLintptr write_స్థానభ్రంశం, GLsizeiptr size)
    {
        Copy_బఫరు_Sub_దత్తాంశం = (PFNCOPYBUFFERSUBDATAPROC)Int_Get_Proc_చిరునామా("glCopyBufferSubData");
        Copy_బఫరు_Sub_దత్తాంశం(read_లక్ష్యం, write_లక్ష్యం, read_స్థానభ్రంశం, write_స్థానభ్రంశం, size);
    }

    // Extension: 3.1

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DrawArraysInstanced(GLenum బహుళకం, GLint first, GLsizei లెక్క_2, GLsizei instancecount)
    {
        DrawArraysInstanced = (PFNDRAWARRAYSINSTANCEDPROC)Int_Get_Proc_చిరునామా("glDrawArraysInstanced");
        DrawArraysInstanced(బహుళకం, first, లెక్క_2, instancecount);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DrawElementsInstanced(GLenum బహుళకం, GLsizei లెక్క_2, GLenum type, స్థిరరాశి GLvoid *indices, GLsizei instancecount)
    {
        DrawElementsInstanced = (PFNDRAWELEMENTSINSTANCEDPROC)Int_Get_Proc_చిరునామా("glDrawElementsInstanced");
        DrawElementsInstanced(బహుళకం, లెక్క_2, type, indices, instancecount);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Tex_బఫరు(GLenum target, GLenum internalformat, GLuint బఫరు)
    {
        Tex_బఫరు = (PFNTEXBUFFERPROC)Int_Get_Proc_చిరునామా("glTexBuffer");
        Tex_బఫరు(target, internalformat, బఫరు);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Primitive_Restart_సూచీ(GLuint సూచీ)
    {
        Primitive_Restart_సూచీ = (PFNPRIMITIVERESTARTINDEXPROC)Int_Get_Proc_చిరునామా("glPrimitiveRestartIndex");
        Primitive_Restart_సూచీ(సూచీ);
    }

    // Legacy

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_EnableClientState(GLenum cap)
    {
        EnableClientState = (PFNENABLECLIENTSTATEPROC)Int_Get_Proc_చిరునామా("glEnableClientState");
        EnableClientState(cap);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_DisableClientState(GLenum cap)
    {
        DisableClientState = (PFNDISABLECLIENTSTATEPROC)Int_Get_Proc_చిరునామా("glDisableClientState");
        DisableClientState(cap);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_శీర్షం_సూచిక(GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr)
    {
        శీర్షం_సూచిక = (PFNVERTEXPOINTERPROC)Int_Get_Proc_చిరునామా("glVertexPointer");
        శీర్షం_సూచిక(size, type, stride, ptr);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_సాధారణ_సూచిక(GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr)
    {
        సాధారణ_సూచిక = (PFNNORMALPOINTERPROC)Int_Get_Proc_చిరునామా("glNormalPointer");
        సాధారణ_సూచిక(type, stride, ptr);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Color_సూచిక(GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr)
    {
        Color_సూచిక = (PFNCOLORPOINTERPROC)Int_Get_Proc_చిరునామా("glColorPointer");
        Color_సూచిక(size, type, stride, ptr);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Tex_Coord_సూచిక(GLint size, GLenum type, GLsizei stride, స్థిరరాశి GLvoid *ptr)
    {
        Tex_Coord_సూచిక = (PFNTEXCOORDPOINTERPROC)Int_Get_Proc_చిరునామా("glTexCoordPointer");
        Tex_Coord_సూచిక(size, type, stride, ptr);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_TexEnvi(GLenum target, GLenum pname, GLint param)
    {
        TexEnvi = (PFNTEXENVIPROC)Int_Get_Proc_చిరునామా("glTexEnvi");
        TexEnvi(target, pname, param);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_మాతృక_బహుళకం(GLenum బహుళకం)
    {
        మాతృక_బహుళకం = (PFNMATRIXMODEPROC)Int_Get_Proc_చిరునామా("glMatrixMode");
        మాతృక_బహుళకం(బహుళకం);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_ఎక్కించు_తటస్థం(శూన్య)
    {
        ఎక్కించు_తటస్థం = (PFNLOADIDENTITYPROC)Int_Get_Proc_చిరునామా("glLoadIdentity");
        ఎక్కించు_తటస్థం();
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Ortho(GLdouble ఎడమ, GLdouble కుడి, GLdouble bottom, GLdouble శిఖరం_2, GLdouble near_val, GLdouble far_val)
    {
        Ortho = (PFNORTHOPROC)Int_Get_Proc_చిరునామా("glOrtho");
        Ortho(ఎడమ, కుడి, bottom, శిఖరం_2, near_val, far_val);
    }

    స్థిర శూన్య CODEGEN_FUNCPTR Switch_Color3d(GLdouble red, GLdouble green, GLdouble blue)
    {
        Color3d = (PFNCOLOR3DPROC)Int_Get_Proc_చిరునామా("glColor3d");
        Color3d(red, green, blue);
    }

    నిర్మాణం InitializeVariables
    {
        InitializeVariables()
        {
            // Extension: 1.1
            CullFace = Switch_CullFace;
            FrontFace = Switch_FrontFace;
            Hint = Switch_Hint;
            పంక్తి_వెడల్పు = Switch_పంక్తి_వెడల్పు;
            బిందువు_కొలత = Switch_బిందువు_కొలత;
            Polygon_బహుళకం = Switch_Polygon_బహుళకం;
            Scissor = Switch_Scissor;
            TexParameterf = Switch_TexParameterf;
            TexParameterfv = Switch_TexParameterfv;
            TexParameteri = Switch_TexParameteri;
            TexParameteriv = Switch_TexParameteriv;
            TexImage1D = Switch_TexImage1D;
            TexImage2D = Switch_TexImage2D;
            Draw_బఫరు = Switch_Draw_బఫరు;
            Clear = Switch_Clear;
            ClearColor = Switch_ClearColor;
            ClearStencil = Switch_ClearStencil;
            Clear_లోతు = Switch_Clear_లోతు;
            Stencil_ముసుగు = Switch_Stencil_ముసుగు;
            Color_ముసుగు = Switch_Color_ముసుగు;
            లోతు_ముసుగు = Switch_లోతు_ముసుగు;
            Disable = Switch_Disable;
            Enable = Switch_Enable;
            ముగింపు = Switch_ముగింపు;
            Flush = Switch_Flush;
            BlendFunc = Switch_BlendFunc;
            LogicOp = Switch_LogicOp;
            StencilFunc = Switch_StencilFunc;
            StencilOp = Switch_StencilOp;
            లోతు_Func = Switch_లోతు_Func;
            PixelStoref = Switch_PixelStoref;
            PixelStorei = Switch_PixelStorei;
            Read_బఫరు = Switch_Read_బఫరు;
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
            లోతు_Range = Switch_లోతు_Range;
            Viewport = Switch_Viewport;
            DrawArrays = Switch_DrawArrays;
            DrawElements = Switch_DrawElements;
            GetPointerv = Switch_GetPointerv;
            Polygon_స్థానభ్రంశం = Switch_Polygon_స్థానభ్రంశం;
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
            చురుకైన_Texture = Switch_చురుకైన_Texture;
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
            బిందువు_Parameterf = Switch_బిందువు_Parameterf;
            బిందువు_Parameterfv = Switch_బిందువు_Parameterfv;
            బిందువు_Parameteri = Switch_బిందువు_Parameteri;
            బిందువు_Parameteriv = Switch_బిందువు_Parameteriv;

            // Extension: 1.5
            GenQueries = Switch_GenQueries;
            DeleteQueries = Switch_DeleteQueries;
            IsQuery = Switch_IsQuery;
            BeginQuery = Switch_BeginQuery;
            చివర_Query = Switch_చివర_Query;
            GetQueryiv = Switch_GetQueryiv;
            GetQueryObjectiv = Switch_GetQueryObjectiv;
            GetQueryObjectuiv = Switch_GetQueryObjectuiv;
            Bind_బఫరు = Switch_Bind_బఫరు;
            DeleteBuffers = Switch_DeleteBuffers;
            GenBuffers = Switch_GenBuffers;
            Is_బఫరు = Switch_Is_బఫరు;
            బఫరు_దత్తాంశం = Switch_బఫరు_దత్తాంశం;
            బఫరు_Sub_దత్తాంశం = Switch_బఫరు_Sub_దత్తాంశం;
            Get_బఫరు_Sub_దత్తాంశం = Switch_Get_బఫరు_Sub_దత్తాంశం;
            Map_బఫరు = Switch_Map_బఫరు;
            Unmap_బఫరు = Switch_Unmap_బఫరు;
            Get_బఫరు_Parameteriv = Switch_Get_బఫరు_Parameteriv;
            Get_బఫరు_Pointerv = Switch_Get_బఫరు_Pointerv;

            // Extension: 2.0
            BlendEquationSeparate = Switch_BlendEquationSeparate;
            DrawBuffers = Switch_DrawBuffers;
            StencilOpSeparate = Switch_StencilOpSeparate;
            StencilFuncSeparate = Switch_StencilFuncSeparate;
            Stencil_ముసుగు_Separate = Switch_Stencil_ముసుగు_Separate;
            AttachShader = Switch_AttachShader;
            BindAttribLocation = Switch_BindAttribLocation;
            CompileShader = Switch_CompileShader;
            Create_కార్యక్రమం = Switch_Create_కార్యక్రమం;
            CreateShader = Switch_CreateShader;
            Delete_కార్యక్రమం = Switch_Delete_కార్యక్రమం;
            DeleteShader = Switch_DeleteShader;
            DetachShader = Switch_DetachShader;
            Disable_శీర్షం_Attrib_Array = Switch_Disable_శీర్షం_Attrib_Array;
            Enable_శీర్షం_Attrib_Array = Switch_Enable_శీర్షం_Attrib_Array;
            Get_చురుకైన_Attrib = Switch_Get_చురుకైన_Attrib;
            Get_చురుకైన_ఏకరీతి = Switch_Get_చురుకైన_ఏకరీతి;
            GetAttachedShaders = Switch_GetAttachedShaders;
            GetAttribLocation = Switch_GetAttribLocation;
            GetProgramiv = Switch_GetProgramiv;
            Get_కార్యక్రమం_Info_Log = Switch_Get_కార్యక్రమం_Info_Log;
            GetShaderiv = Switch_GetShaderiv;
            GetShaderInfoLog = Switch_GetShaderInfoLog;
            Get_Shader_ఆధారం = Switch_Get_Shader_ఆధారం;
            Get_ఏకరీతి_Location = Switch_Get_ఏకరీతి_Location;
            GetUniformfv = Switch_GetUniformfv;
            GetUniformiv = Switch_GetUniformiv;
            Get_శీర్షం_Attribdv = Switch_Get_శీర్షం_Attribdv;
            Get_శీర్షం_Attribfv = Switch_Get_శీర్షం_Attribfv;
            Get_శీర్షం_Attribiv = Switch_Get_శీర్షం_Attribiv;
            Get_శీర్షం_Attrib_Pointerv = Switch_Get_శీర్షం_Attrib_Pointerv;
            Is_కార్యక్రమం = Switch_Is_కార్యక్రమం;
            IsShader = Switch_IsShader;
            Link_కార్యక్రమం = Switch_Link_కార్యక్రమం;
            Shader_ఆధారం = Switch_Shader_ఆధారం;
            Use_కార్యక్రమం = Switch_Use_కార్యక్రమం;
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
            ఏకరీతి_Matrix2fv = Switch_ఏకరీతి_Matrix2fv;
            ఏకరీతి_Matrix3fv = Switch_ఏకరీతి_Matrix3fv;
            ఏకరీతి_Matrix4fv = Switch_ఏకరీతి_Matrix4fv;
            Validate_కార్యక్రమం = Switch_Validate_కార్యక్రమం;
            శీర్షం_Attrib_సూచిక = Switch_శీర్షం_Attrib_సూచిక;

            // Extension: 2.1
            ఏకరీతి_Matrix2x3fv = Switch_ఏకరీతి_Matrix2x3fv;
            ఏకరీతి_Matrix3x2fv = Switch_ఏకరీతి_Matrix3x2fv;
            ఏకరీతి_Matrix2x4fv = Switch_ఏకరీతి_Matrix2x4fv;
            ఏకరీతి_Matrix4x2fv = Switch_ఏకరీతి_Matrix4x2fv;
            ఏకరీతి_Matrix3x4fv = Switch_ఏకరీతి_Matrix3x4fv;
            ఏకరీతి_Matrix4x3fv = Switch_ఏకరీతి_Matrix4x3fv;

            // Extension: ARB_vertex_array_object
            Bind_శీర్షం_Array = Switch_Bind_శీర్షం_Array;
            Delete_శీర్షం_Arrays = Switch_Delete_శీర్షం_Arrays;
            Gen_శీర్షం_Arrays = Switch_Gen_శీర్షం_Arrays;
            Is_శీర్షం_Array = Switch_Is_శీర్షం_Array;

            // Extension: ARB_map_buffer_range
            Map_బఫరు_Range = Switch_Map_బఫరు_Range;
            Flush_Mapped_బఫరు_Range = Switch_Flush_Mapped_బఫరు_Range;

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
            Check_Framebuffer_స్థితి = Switch_Check_Framebuffer_స్థితి;
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
            చివర_Transform_Feedback = Switch_చివర_Transform_Feedback;
            Bind_బఫరు_Range = Switch_Bind_బఫరు_Range;
            Bind_బఫరు_పునాది = Switch_Bind_బఫరు_పునాది;
            TransformFeedbackVaryings = Switch_TransformFeedbackVaryings;
            GetTransformFeedbackVarying = Switch_GetTransformFeedbackVarying;
            ClampColor = Switch_ClampColor;
            BeginConditionalRender = Switch_BeginConditionalRender;
            చివర_Conditional_Render = Switch_చివర_Conditional_Render;
            శీర్షం_Attrib_IPointer = Switch_శీర్షం_Attrib_IPointer;
            Get_శీర్షం_Attrib_Iiv = Switch_Get_శీర్షం_Attrib_Iiv;
            Get_శీర్షం_Attrib_Iuiv = Switch_Get_శీర్షం_Attrib_Iuiv;
            శీర్షం_Attrib_I1i = Switch_శీర్షం_Attrib_I1i;
            శీర్షం_Attrib_I2i = Switch_శీర్షం_Attrib_I2i;
            శీర్షం_Attrib_I3i = Switch_శీర్షం_Attrib_I3i;
            శీర్షం_Attrib_I4i = Switch_శీర్షం_Attrib_I4i;
            శీర్షం_Attrib_I1ui = Switch_శీర్షం_Attrib_I1ui;
            శీర్షం_Attrib_I2ui = Switch_శీర్షం_Attrib_I2ui;
            శీర్షం_Attrib_I3ui = Switch_శీర్షం_Attrib_I3ui;
            శీర్షం_Attrib_I4ui = Switch_శీర్షం_Attrib_I4ui;
            శీర్షం_Attrib_I1iv = Switch_శీర్షం_Attrib_I1iv;
            శీర్షం_Attrib_I2iv = Switch_శీర్షం_Attrib_I2iv;
            శీర్షం_Attrib_I3iv = Switch_శీర్షం_Attrib_I3iv;
            శీర్షం_Attrib_I4iv = Switch_శీర్షం_Attrib_I4iv;
            శీర్షం_Attrib_I1uiv = Switch_శీర్షం_Attrib_I1uiv;
            శీర్షం_Attrib_I2uiv = Switch_శీర్షం_Attrib_I2uiv;
            శీర్షం_Attrib_I3uiv = Switch_శీర్షం_Attrib_I3uiv;
            శీర్షం_Attrib_I4uiv = Switch_శీర్షం_Attrib_I4uiv;
            శీర్షం_Attrib_I4bv = Switch_శీర్షం_Attrib_I4bv;
            శీర్షం_Attrib_I4sv = Switch_శీర్షం_Attrib_I4sv;
            శీర్షం_Attrib_I4ubv = Switch_శీర్షం_Attrib_I4ubv;
            శీర్షం_Attrib_I4usv = Switch_శీర్షం_Attrib_I4usv;
            GetUniformuiv = Switch_GetUniformuiv;
            Bind_Frag_దత్తాంశం_Location = Switch_Bind_Frag_దత్తాంశం_Location;
            Get_Frag_దత్తాంశం_Location = Switch_Get_Frag_దత్తాంశం_Location;
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
            Get_ఏకరీతి_Indices = Switch_Get_ఏకరీతి_Indices;
            Get_చురుకైన_Uniformsiv = Switch_Get_చురుకైన_Uniformsiv;
            Get_చురుకైన_ఏకరీతి_Name = Switch_Get_చురుకైన_ఏకరీతి_Name;
            Get_ఏకరీతి_ఖండం_సూచీ = Switch_Get_ఏకరీతి_ఖండం_సూచీ;
            Get_చురుకైన_ఏకరీతి_Blockiv = Switch_Get_చురుకైన_ఏకరీతి_Blockiv;
            Get_చురుకైన_ఏకరీతి_ఖండం_Name = Switch_Get_చురుకైన_ఏకరీతి_ఖండం_Name;
            ఏకరీతి_ఖండం_Binding = Switch_ఏకరీతి_ఖండం_Binding;

            // Extension: ARB_copy_buffer
            Copy_బఫరు_Sub_దత్తాంశం = Switch_Copy_బఫరు_Sub_దత్తాంశం;

            // Extension: 3.1
            DrawArraysInstanced = Switch_DrawArraysInstanced;
            DrawElementsInstanced = Switch_DrawElementsInstanced;
            Tex_బఫరు = Switch_Tex_బఫరు;
            Primitive_Restart_సూచీ = Switch_Primitive_Restart_సూచీ;

            // Legacy
            EnableClientState = Switch_EnableClientState;
            DisableClientState = Switch_DisableClientState;
            శీర్షం_సూచిక = Switch_శీర్షం_సూచిక;
            సాధారణ_సూచిక = Switch_సాధారణ_సూచిక;
            Color_సూచిక = Switch_Color_సూచిక;
            Tex_Coord_సూచిక = Switch_Tex_Coord_సూచిక;
            TexEnvi = Switch_TexEnvi;
            మాతృక_బహుళకం = Switch_మాతృక_బహుళకం;
            ఎక్కించు_తటస్థం = Switch_ఎక్కించు_తటస్థం;
            Ortho = Switch_Ortho;
            Color3d = Switch_Color3d;
        }
    };

    InitializeVariables g_initVariables;
}
