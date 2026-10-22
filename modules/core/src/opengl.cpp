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

#ఒకవేళ defined (__APPLE__) || defined(MACOSX)
   #నిర్వచించు GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#లేకపోతే
   #నిర్వచించు GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    ఒకవేళ (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_సంస్కరణ
#        define GL_సంస్కరణ 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#లేకపోతే // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#అయితే_ముగింపు // HAVE_OPENGL

ఉపయోగించి నామాకాశ cv;
ఉపయోగించి నామాకాశ cv::cuda;

#ఒకవేళ defined(_MSC_VER)
#సూచన warning(disable : 4702)  // unreachable code
#అయితే_ముగింపు

నామాకాశ
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
అంతఃవరుస స్థిర CV_NORETURN శూన్య throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#లేదావేళ defined _DEBUG
అంతఃవరుస స్థిర తార్కిక checkError(స్థిరరాశి అక్షరం* file, స్థిరరాశి పూర్ణాంకం పంక్తి, స్థిరరాశి అక్షరం* func = 0)
{
    GLenum err = gl::GetError();
    ఒకవేళ (err != gl::NO_ERROR_)
    {
        స్థిరరాశి అక్షరం* సందేశం;
        ఎంపిక (err)
        {
        స్థితి gl::INVALID_ENUM:
            సందేశం = "An unacceptable value is specified for an enumerated argument";
            ఆపు;
        స్థితి gl::INVALID_విలువ:
            సందేశం = "A numeric argument is out of range";
            ఆపు;
        స్థితి gl::INVALID_OPERATION:
            సందేశం = "The specified operation is not allowed in the current state";
            ఆపు;
        స్థితి gl::బహిర్గతం_యొక్క_జ్ఞాపకం:
            సందేశం = "There is not enough memory left to execute the command";
            ఆపు;
        అప్రమేయ:
            సందేశం = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, సందేశం, file, పంక్తి);
    }
    తిరిగి నిజం;
}
#అయితే_ముగింపు // HAVE_OPENGL
} // namespace

#నిర్వచించు CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#నిర్వచితమైతే HAVE_OPENGL
నామాకాశ
{
    స్థిరరాశి GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#అయితే_ముగింపు

////////////////////////////////////////////////////////////////////////
// setGlDevice

శూన్య cv::cuda::సమితి_Gl_Device(పూర్ణాంకం device)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(device);
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        CV_వాడని(device);
        throw_no_cuda();
    #లేకపోతే
        cuda_సురక్షిత_Call( cudaGLSetGLDevice(device) );
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#ఒకవేళ defined(HAVE_OPENGL) && defined(HAVE_CUDA)

నామాకాశ
{
    తరగతి CudaResource
    {
    బహిరంగ:
        CudaResource();
        ~CudaResource();

        శూన్య రిజిస్టరు_బఫరు(GLuint బఫరు);
        శూన్య release();

        శూన్య copyFrom(స్థిరరాశి శూన్య* src, size_t spitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream = 0);
        శూన్య copy_గమ్యం(శూన్య* dst, size_t dpitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream = 0);

        శూన్య* map(cuda_ప్రవాహం_t stream = 0);
        శూన్య unmap(cuda_ప్రవాహం_t stream = 0);

    అంతరంగ:
        cudaGraphicsResource_t resource_;
        GLuint బఫరు_3;

        తరగతి GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), బఫరు_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    శూన్య CudaResource::రిజిస్టరు_బఫరు(GLuint బఫరు)
    {
        CV_DbgAssert( బఫరు != 0 );

        ఒకవేళ (బఫరు_3 == బఫరు)
            తిరిగి;

        cudaGraphicsResource_t resource;
        cuda_సురక్షిత_Call( cuda_Graphics_GLRegister_బఫరు(&resource, బఫరు, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        బఫరు_3 = బఫరు;
    }

    శూన్య CudaResource::release()
    {
        ఒకవేళ (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        బఫరు_3 = 0;
    }

    తరగతి CudaResource::GraphicsMapHolder
    {
    బహిరంగ:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_ప్రవాహం_t stream);
        ~GraphicsMapHolder();

        శూన్య reset();

    అంతరంగ:
        cudaGraphicsResource_t* resource_;
        cuda_ప్రవాహం_t ప్రవాహం_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_ప్రవాహం_t stream) : resource_(resource), ప్రవాహం_3(stream)
    {
        ఒకవేళ (resource_)
            cuda_సురక్షిత_Call( cudaGraphicsMapResources(1, resource_, ప్రవాహం_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        ఒకవేళ (resource_)
            cudaGraphicsUnmapResources(1, resource_, ప్రవాహం_3);
    }

    శూన్య CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    శూన్య CudaResource::copyFrom(స్థిరరాశి శూన్య* src, size_t spitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_వాడని(h);

        శూన్య* dst;
        size_t size;
        cuda_సురక్షిత_Call( cuda_Graphics_Resource_Get_Mapped_సూచిక(&dst, &size, resource_) );

        CV_DbgAssert( వెడల్పు * ఎత్తు == size );

        ఒకవేళ (stream == 0)
            cuda_సురక్షిత_Call( cudaMemcpy2D(dst, వెడల్పు, src, spitch, వెడల్పు, ఎత్తు, cuda_Memcpy_Device_గమ్యం_Device) );
        లేకపోతే
            cuda_సురక్షిత_Call( cudaMemcpy2DAsync(dst, వెడల్పు, src, spitch, వెడల్పు, ఎత్తు, cuda_Memcpy_Device_గమ్యం_Device, stream) );
    }

    శూన్య CudaResource::copy_గమ్యం(శూన్య* dst, size_t dpitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_వాడని(h);

        శూన్య* src;
        size_t size;
        cuda_సురక్షిత_Call( cuda_Graphics_Resource_Get_Mapped_సూచిక(&src, &size, resource_) );

        CV_DbgAssert( వెడల్పు * ఎత్తు == size );

        ఒకవేళ (stream == 0)
            cuda_సురక్షిత_Call( cudaMemcpy2D(dst, dpitch, src, వెడల్పు, వెడల్పు, ఎత్తు, cuda_Memcpy_Device_గమ్యం_Device) );
        లేకపోతే
            cuda_సురక్షిత_Call( cudaMemcpy2DAsync(dst, dpitch, src, వెడల్పు, వెడల్పు, ఎత్తు, cuda_Memcpy_Device_గమ్యం_Device, stream) );
    }

    శూన్య* CudaResource::map(cuda_ప్రవాహం_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        శూన్య* ptr;
        size_t size;
        cuda_సురక్షిత_Call( cuda_Graphics_Resource_Get_Mapped_సూచిక(&ptr, &size, resource_) );

        h.reset();

        తిరిగి ptr;
    }

    శూన్య CudaResource::unmap(cuda_ప్రవాహం_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#అయితే_ముగింపు

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#నిర్వచితం_కాకపోతే HAVE_OPENGL

తరగతి cv::ogl::బఫరు_2::Impl
{
};

#లేకపోతే

తరగతి cv::ogl::బఫరు_2::Impl
{
బహిరంగ:
    స్థిర స్థిరరాశి Ptr<Impl>& empty();

    Impl(GLuint bufId, తార్కిక autoRelease);
    Impl(GLsizeiptr size, స్థిరరాశి GLvoid* దత్తాంశం, GLenum target, తార్కిక autoRelease);
    ~Impl();

    శూన్య bind(GLenum target) స్థిరరాశి;

    శూన్య copyFrom(GLuint srcBuf, GLsizeiptr size);

    శూన్య copyFrom(GLsizeiptr size, స్థిరరాశి GLvoid* దత్తాంశం);
    శూన్య copy_గమ్యం(GLsizeiptr size, GLvoid* దత్తాంశం) స్థిరరాశి;

    శూన్య resize(GLsizeiptr size, GLenum target);

    శూన్య* mapHost(GLenum access);
    శూన్య unmapHost();

#నిర్వచితమైతే HAVE_CUDA
    శూన్య copyFrom(స్థిరరాశి శూన్య* src, size_t spitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream = 0);
    శూన్య copy_గమ్యం(శూన్య* dst, size_t dpitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream = 0) స్థిరరాశి;

    శూన్య* mapDevice(cuda_ప్రవాహం_t stream = 0);
    శూన్య unmapDevice(cuda_ప్రవాహం_t stream = 0);
#అయితే_ముగింపు

    శూన్య సమితి_Auto_Release(తార్కిక flag) { autoRelease_ = flag; }

    GLuint bufId() స్థిరరాశి { తిరిగి bufId_; }

అంతరంగ:
    Impl();

    GLuint bufId_;
    తార్కిక autoRelease_;

#నిర్వచితమైతే HAVE_CUDA
    మార్చదగిన CudaResource cudaResource_;
#అయితే_ముగింపు
};

స్థిరరాశి Ptr<cv::ogl::బఫరు_2::Impl>& cv::ogl::బఫరు_2::Impl::empty()
{
    స్థిర Ptr<Impl> p(కొత్త Impl);
    తిరిగి p;
}

cv::ogl::బఫరు_2::Impl::Impl() : bufId_(0), autoRelease_(అబద్ధం)
{
}

cv::ogl::బఫరు_2::Impl::Impl(GLuint abufId, తార్కిక autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_బఫరు(abufId) == gl::TRUE_ );
}

cv::ogl::బఫరు_2::Impl::Impl(GLsizeiptr size, స్థిరరాశి GLvoid* దత్తాంశం, GLenum target, తార్కిక autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_బఫరు(target, bufId_);
    CV_CheckGlError();

    gl::బఫరు_దత్తాంశం(target, size, దత్తాంశం, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_బఫరు(target, 0);
    CV_CheckGlError();
}

cv::ogl::బఫరు_2::Impl::~Impl()
{
    ఒకవేళ (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

శూన్య cv::ogl::బఫరు_2::Impl::bind(GLenum target) స్థిరరాశి
{
    gl::Bind_బఫరు(target, bufId_);
    CV_CheckGlError();
}

శూన్య cv::ogl::బఫరు_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_బఫరు(gl::COPY_WRITE_బఫరు, bufId_);
    CV_CheckGlError();

    gl::Bind_బఫరు(gl::COPY_READ_బఫరు, srcBuf);
    CV_CheckGlError();

    gl::Copy_బఫరు_Sub_దత్తాంశం(gl::COPY_READ_బఫరు, gl::COPY_WRITE_బఫరు, 0, 0, size);
    CV_CheckGlError();
}

శూన్య cv::ogl::బఫరు_2::Impl::copyFrom(GLsizeiptr size, స్థిరరాశి GLvoid* దత్తాంశం)
{
    gl::Bind_బఫరు(gl::COPY_WRITE_బఫరు, bufId_);
    CV_CheckGlError();

    gl::బఫరు_Sub_దత్తాంశం(gl::COPY_WRITE_బఫరు, 0, size, దత్తాంశం);
    CV_CheckGlError();
}

శూన్య cv::ogl::బఫరు_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_బఫరు(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::బఫరు_దత్తాంశం(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_బఫరు(target, 0);
    CV_CheckGlError();
}

శూన్య cv::ogl::బఫరు_2::Impl::copy_గమ్యం(GLsizeiptr size, GLvoid* దత్తాంశం) స్థిరరాశి
{
    gl::Bind_బఫరు(gl::COPY_READ_బఫరు, bufId_);
    CV_CheckGlError();

    gl::Get_బఫరు_Sub_దత్తాంశం(gl::COPY_READ_బఫరు, 0, size, దత్తాంశం);
    CV_CheckGlError();
}

శూన్య* cv::ogl::బఫరు_2::Impl::mapHost(GLenum access)
{
    gl::Bind_బఫరు(gl::COPY_READ_బఫరు, bufId_);
    CV_CheckGlError();

    GLvoid* దత్తాంశం = gl::Map_బఫరు(gl::COPY_READ_బఫరు, access);
    CV_CheckGlError();

    తిరిగి దత్తాంశం;
}

శూన్య cv::ogl::బఫరు_2::Impl::unmapHost()
{
    gl::Unmap_బఫరు(gl::COPY_READ_బఫరు);
}

#నిర్వచితమైతే HAVE_CUDA

శూన్య cv::ogl::బఫరు_2::Impl::copyFrom(స్థిరరాశి శూన్య* src, size_t spitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream)
{
    cudaResource_.రిజిస్టరు_బఫరు(bufId_);
    cudaResource_.copyFrom(src, spitch, వెడల్పు, ఎత్తు, stream);
}

శూన్య cv::ogl::బఫరు_2::Impl::copy_గమ్యం(శూన్య* dst, size_t dpitch, size_t వెడల్పు, size_t ఎత్తు, cuda_ప్రవాహం_t stream) స్థిరరాశి
{
    cudaResource_.రిజిస్టరు_బఫరు(bufId_);
    cudaResource_.copy_గమ్యం(dst, dpitch, వెడల్పు, ఎత్తు, stream);
}

శూన్య* cv::ogl::బఫరు_2::Impl::mapDevice(cuda_ప్రవాహం_t stream)
{
    cudaResource_.రిజిస్టరు_బఫరు(bufId_);
    తిరిగి cudaResource_.map(stream);
}

శూన్య cv::ogl::బఫరు_2::Impl::unmapDevice(cuda_ప్రవాహం_t stream)
{
    cudaResource_.unmap(stream);
}

#అయితే_ముగింపు // HAVE_CUDA

#అయితే_ముగింపు // HAVE_OPENGL

cv::ogl::బఫరు_2::బఫరు_2() : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), type_(0)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

cv::ogl::బఫరు_2::బఫరు_2(పూర్ణాంకం arows, పూర్ణాంకం acols, పూర్ణాంకం atype, గుర్తులేని పూర్ణాంకం abufId, తార్కిక autoRelease) : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), type_(0)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arows);
    CV_వాడని(acols);
    CV_వాడని(atype);
    CV_వాడని(abufId);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(abufId, autoRelease));
    అడ్డువరుసలు_2 = arows;
    నిలువుల_సంఖ్య_2 = acols;
    type_ = atype;
#అయితే_ముగింపు
}

cv::ogl::బఫరు_2::బఫరు_2(కొలత asize, పూర్ణాంకం atype, గుర్తులేని పూర్ణాంకం abufId, తార్కిక autoRelease) : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), type_(0)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(asize);
    CV_వాడని(atype);
    CV_వాడని(abufId);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(abufId, autoRelease));
    అడ్డువరుసలు_2 = asize.ఎత్తు;
    నిలువుల_సంఖ్య_2 = asize.వెడల్పు;
    type_ = atype;
#అయితే_ముగింపు
}

cv::ogl::బఫరు_2::బఫరు_2(ఆదానం_Array arr, లక్ష్యం target, తార్కిక autoRelease) : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), type_(0)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(target);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం రకం = arr.రకం();

    ఎంపిక (రకం)
    {
    స్థితి ఆదానం_Array_2::OPENGL_బఫరు:
    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        ఆపు;

    అప్రమేయ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            స్థిరరాశి GLsizeiptr asize = mat.అడ్డువరుసలు * mat.నిలువుల_సంఖ్య * mat.elem_కొలత();
            impl_.reset(కొత్త Impl(asize, mat.దత్తాంశం, target, autoRelease));
            అడ్డువరుసలు_2 = mat.అడ్డువరుసలు;
            నిలువుల_సంఖ్య_2 = mat.నిలువుల_సంఖ్య;
            type_ = mat.type();
            ఆపు;
        }
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::create(పూర్ణాంకం arows, పూర్ణాంకం acols, పూర్ణాంకం atype, లక్ష్యం target, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arows);
    CV_వాడని(acols);
    CV_వాడని(atype);
    CV_వాడని(target);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    ఒకవేళ (అడ్డువరుసలు_2 != arows || నిలువుల_సంఖ్య_2 != acols || type_ != atype)
    {
        స్థిరరాశి GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_కొలత(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        ఒకవేళ (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->సమితి_Auto_Release(autoRelease);
        }
        లేకపోతే
            impl_.reset(కొత్త Impl(asize, 0, target, autoRelease));
        అడ్డువరుసలు_2 = arows;
        నిలువుల_సంఖ్య_2 = acols;
        type_ = atype;
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::release()
{
#నిర్వచితమైతే HAVE_OPENGL
    ఒకవేళ (impl_)
        impl_->సమితి_Auto_Release(నిజం);
    impl_ = Impl::empty();
    అడ్డువరుసలు_2 = 0;
    నిలువుల_సంఖ్య_2 = 0;
    type_ = 0;
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    impl_->సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::copyFrom(ఆదానం_Array arr, లక్ష్యం target, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(target);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం రకం = arr.రకం();

    స్థిరరాశి కొలత asize = arr.size();
    స్థిరరాశి పూర్ణాంకం atype = arr.type();
    create(asize, atype, target, autoRelease);

    ఎంపిక (రకం)
    {
    స్థితి ఆదానం_Array_2::OPENGL_బఫరు:
        {
            ogl::బఫరు_2 buf = arr.get_OGl_బఫరు();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_కొలత(atype));
            ఆపు;
        }

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        {
            #నిర్వచితం_కాకపోతే HAVE_CUDA
                throw_no_cuda();
            #లేకపోతే
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.దత్తాంశం, dmat.అడుగు, dmat.నిలువుల_సంఖ్య * dmat.elem_కొలత(), dmat.అడ్డువరుసలు);
            #అయితే_ముగింపు

            ఆపు;
        }

    అప్రమేయ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_కొలత(atype), mat.దత్తాంశం);
        }
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::copyFrom(ఆదానం_Array arr, cuda::ప్రవాహం& stream, లక్ష్యం target, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(stream);
    CV_వాడని(target);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        CV_వాడని(arr);
        CV_వాడని(stream);
        CV_వాడని(target);
        CV_వాడని(autoRelease);
        throw_no_cuda();
    #లేకపోతే
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.దత్తాంశం, dmat.అడుగు, dmat.నిలువుల_సంఖ్య * dmat.elem_కొలత(), dmat.అడ్డువరుసలు, cuda::ప్రవాహం_Accessor::get_ప్రవాహం(stream));
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::copy_గమ్యం(OutputArray arr) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    throw_no_ogl();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం రకం = arr.రకం();

    ఎంపిక (రకం)
    {
    స్థితి ఆదానం_Array_2::OPENGL_బఫరు:
        {
            arr.get_OGl_బఫరు_Ref().copyFrom(*ఇది);
            ఆపు;
        }

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        {
            #నిర్వచితం_కాకపోతే HAVE_CUDA
                throw_no_cuda();
            #లేకపోతే
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type_);
                impl_->copy_గమ్యం(dmat.దత్తాంశం, dmat.అడుగు, dmat.నిలువుల_సంఖ్య * dmat.elem_కొలత(), dmat.అడ్డువరుసలు);
            #అయితే_ముగింపు

            ఆపు;
        }

    అప్రమేయ:
        {
            arr.create(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_గమ్యం(mat.అడ్డువరుసలు * mat.నిలువుల_సంఖ్య * mat.elem_కొలత(), mat.దత్తాంశం);
        }
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::copy_గమ్యం(OutputArray arr, cuda::ప్రవాహం& stream) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(stream);
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        CV_వాడని(arr);
        CV_వాడని(stream);
        throw_no_cuda();
    #లేకపోతే
        arr.create(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_గమ్యం(dmat.దత్తాంశం, dmat.అడుగు, dmat.నిలువుల_సంఖ్య * dmat.elem_కొలత(), dmat.అడ్డువరుసలు, cuda::ప్రవాహం_Accessor::get_ప్రవాహం(stream));
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

cv::ogl::బఫరు_2 cv::ogl::బఫరు_2::clone(లక్ష్యం target, తార్కిక autoRelease) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(target);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    ogl::బఫరు_2 buf;
    buf.copyFrom(*ఇది, target, autoRelease);
    తిరిగి buf;
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::bind(లక్ష్యం target) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(target);
    throw_no_ogl();
#లేకపోతే
    impl_->bind(target);
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::unbind(లక్ష్యం target)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(target);
    throw_no_ogl();
#లేకపోతే
    gl::Bind_బఫరు(target, 0);
    CV_CheckGlError();
#అయితే_ముగింపు
}

Mat cv::ogl::బఫరు_2::mapHost(Access access)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(access);
    throw_no_ogl();
#లేకపోతే
    తిరిగి Mat(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type_, impl_->mapHost(access));
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::unmapHost()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->unmapHost();
#అయితే_ముగింపు
}

GpuMat cv::ogl::బఫరు_2::mapDevice()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        throw_no_cuda();
    #లేకపోతే
        తిరిగి GpuMat(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type_, impl_->mapDevice());
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::unmapDevice()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        throw_no_cuda();
    #లేకపోతే
        impl_->unmapDevice();
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

cuda::GpuMat cv::ogl::బఫరు_2::mapDevice(cuda::ప్రవాహం& stream)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(stream);
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        CV_వాడని(stream);
        throw_no_cuda();
    #లేకపోతే
        తిరిగి GpuMat(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, type_, impl_->mapDevice(cuda::ప్రవాహం_Accessor::get_ప్రవాహం(stream)));
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

శూన్య cv::ogl::బఫరు_2::unmapDevice(cuda::ప్రవాహం& stream)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(stream);
    throw_no_ogl();
#లేకపోతే
    #నిర్వచితం_కాకపోతే HAVE_CUDA
        CV_వాడని(stream);
        throw_no_cuda();
    #లేకపోతే
        impl_->unmapDevice(cuda::ప్రవాహం_Accessor::get_ప్రవాహం(stream));
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

గుర్తులేని పూర్ణాంకం cv::ogl::బఫరు_2::bufId() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->bufId();
#అయితే_ముగింపు
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#నిర్వచితం_కాకపోతే HAVE_OPENGL

తరగతి cv::ogl::Texture2D::Impl
{
};

#లేకపోతే

తరగతి cv::ogl::Texture2D::Impl
{
బహిరంగ:
    స్థిర స్థిరరాశి Ptr<Impl> empty();

    Impl(GLuint texId, తార్కిక autoRelease);
    Impl(GLint అంతర్గత_Format, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, స్థిరరాశి GLvoid* pixels, తార్కిక autoRelease);
    ~Impl();

    శూన్య copyFrom(GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels);
    శూన్య copy_గమ్యం(GLenum format, GLenum type, GLvoid* pixels) స్థిరరాశి;

    శూన్య bind() స్థిరరాశి;

    శూన్య సమితి_Auto_Release(తార్కిక flag) { autoRelease_ = flag; }

    GLuint texId() స్థిరరాశి { తిరిగి texId_; }

అంతరంగ:
    Impl();

    GLuint texId_;
    తార్కిక autoRelease_;
};

స్థిరరాశి Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    స్థిర Ptr<Impl> p(కొత్త Impl);
    తిరిగి p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(అబద్ధం)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, తార్కిక autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint అంతర్గత_Format, GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, స్థిరరాశి GLvoid* pixels, తార్కిక autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, అంతర్గత_Format, వెడల్పు, ఎత్తు, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    ఒకవేళ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

శూన్య cv::ogl::Texture2D::Impl::copyFrom(GLsizei వెడల్పు, GLsizei ఎత్తు, GLenum format, GLenum type, స్థిరరాశి GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, వెడల్పు, ఎత్తు, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

శూన్య cv::ogl::Texture2D::Impl::copy_గమ్యం(GLenum format, GLenum type, GLvoid* pixels) స్థిరరాశి
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

శూన్య cv::ogl::Texture2D::Impl::bind() స్థిరరాశి
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#అయితే_ముగింపు // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), format_(NONE)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

cv::ogl::Texture2D::Texture2D(పూర్ణాంకం arows, పూర్ణాంకం acols, Format aformat, గుర్తులేని పూర్ణాంకం atexId, తార్కిక autoRelease) : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), format_(NONE)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arows);
    CV_వాడని(acols);
    CV_వాడని(aformat);
    CV_వాడని(atexId);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(atexId, autoRelease));
    అడ్డువరుసలు_2 = arows;
    నిలువుల_సంఖ్య_2 = acols;
    format_ = aformat;
#అయితే_ముగింపు
}

cv::ogl::Texture2D::Texture2D(కొలత asize, Format aformat, గుర్తులేని పూర్ణాంకం atexId, తార్కిక autoRelease) : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), format_(NONE)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(asize);
    CV_వాడని(aformat);
    CV_వాడని(atexId);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(atexId, autoRelease));
    అడ్డువరుసలు_2 = asize.ఎత్తు;
    నిలువుల_సంఖ్య_2 = asize.వెడల్పు;
    format_ = aformat;
#అయితే_ముగింపు
}

cv::ogl::Texture2D::Texture2D(ఆదానం_Array arr, తార్కిక autoRelease) : అడ్డువరుసలు_2(0), నిలువుల_సంఖ్య_2(0), format_(NONE)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం రకం = arr.రకం();

    స్థిరరాశి కొలత asize = arr.size();
    స్థిరరాశి పూర్ణాంకం atype = arr.type();

    స్థిరరాశి పూర్ణాంకం లోతు = CV_MAT_లోతు(atype);
    స్థిరరాశి పూర్ణాంకం cn = CV_MAT_CN(atype);

    CV_Assert( లోతు <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    స్థిరరాశి Format అంతర్గత_Formats[] =
    {
        NONE, లోతు_COMPONENT, NONE, RGB, RGBA
    };
    స్థిరరాశి GLenum srcFormats[] =
    {
        0, gl::లోతు_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    ఎంపిక (రకం)
    {
    స్థితి ఆదానం_Array_2::OPENGL_బఫరు:
        {
            ogl::బఫరు_2 buf = arr.get_OGl_బఫరు();
            buf.bind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            impl_.reset(కొత్త Impl(అంతర్గత_Formats[cn], asize.వెడల్పు, asize.ఎత్తు, srcFormats[cn], gl_types[లోతు], 0, autoRelease));
            ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            ఆపు;
        }

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        {
            #నిర్వచితం_కాకపోతే HAVE_CUDA
                throw_no_cuda();
            #లేకపోతే
                GpuMat dmat = arr.getGpuMat();
                ogl::బఫరు_2 buf(dmat, ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
                buf.సమితి_Auto_Release(నిజం);
                buf.bind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
                impl_.reset(కొత్త Impl(అంతర్గత_Formats[cn], asize.వెడల్పు, asize.ఎత్తు, srcFormats[cn], gl_types[లోతు], 0, autoRelease));
                ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            #అయితే_ముగింపు

            ఆపు;
        }

    అప్రమేయ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            impl_.reset(కొత్త Impl(అంతర్గత_Formats[cn], asize.వెడల్పు, asize.ఎత్తు, srcFormats[cn], gl_types[లోతు], mat.దత్తాంశం, autoRelease));
            ఆపు;
        }
    }

    అడ్డువరుసలు_2 = asize.ఎత్తు;
    నిలువుల_సంఖ్య_2 = asize.వెడల్పు;
    format_ = అంతర్గత_Formats[cn];
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture2D::create(పూర్ణాంకం arows, పూర్ణాంకం acols, Format aformat, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arows);
    CV_వాడని(acols);
    CV_వాడని(aformat);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    ఒకవేళ (అడ్డువరుసలు_2 != arows || నిలువుల_సంఖ్య_2 != acols || format_ != aformat)
    {
        ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
        impl_.reset(కొత్త Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        అడ్డువరుసలు_2 = arows;
        నిలువుల_సంఖ్య_2 = acols;
        format_ = aformat;
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture2D::release()
{
#నిర్వచితమైతే HAVE_OPENGL
    ఒకవేళ (impl_)
        impl_->సమితి_Auto_Release(నిజం);
    impl_ = Impl::empty();
    అడ్డువరుసలు_2 = 0;
    నిలువుల_సంఖ్య_2 = 0;
    format_ = NONE;
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture2D::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    impl_->సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture2D::copyFrom(ఆదానం_Array arr, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం రకం = arr.రకం();

    స్థిరరాశి కొలత asize = arr.size();
    స్థిరరాశి పూర్ణాంకం atype = arr.type();

    స్థిరరాశి పూర్ణాంకం లోతు = CV_MAT_లోతు(atype);
    స్థిరరాశి పూర్ణాంకం cn = CV_MAT_CN(atype);

    CV_Assert( లోతు <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    స్థిరరాశి Format అంతర్గత_Formats[] =
    {
        NONE, లోతు_COMPONENT, NONE, RGB, RGBA
    };
    స్థిరరాశి GLenum srcFormats[] =
    {
        0, gl::లోతు_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, అంతర్గత_Formats[cn], autoRelease);

    ఎంపిక(రకం)
    {
    స్థితి ఆదానం_Array_2::OPENGL_బఫరు:
        {
            ogl::బఫరు_2 buf = arr.get_OGl_బఫరు();
            buf.bind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            impl_->copyFrom(asize.వెడల్పు, asize.ఎత్తు, srcFormats[cn], gl_types[లోతు], 0);
            ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            ఆపు;
        }

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        {
            #నిర్వచితం_కాకపోతే HAVE_CUDA
                throw_no_cuda();
            #లేకపోతే
                GpuMat dmat = arr.getGpuMat();
                ogl::బఫరు_2 buf(dmat, ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
                buf.సమితి_Auto_Release(నిజం);
                buf.bind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
                impl_->copyFrom(asize.వెడల్పు, asize.ఎత్తు, srcFormats[cn], gl_types[లోతు], 0);
                ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            #అయితే_ముగింపు

            ఆపు;
        }

    అప్రమేయ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_UNPACK_బఫరు);
            impl_->copyFrom(asize.వెడల్పు, asize.ఎత్తు, srcFormats[cn], gl_types[లోతు], mat.దత్తాంశం);
        }
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture2D::copy_గమ్యం(OutputArray arr, పూర్ణాంకం ddepth, తార్కిక autoRelease) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(ddepth);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం రకం = arr.రకం();

    స్థిరరాశి పూర్ణాంకం cn = format_ == లోతు_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    స్థిరరాశి GLenum dstFormat = format_ == లోతు_COMPONENT ? gl::లోతు_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    ఎంపిక(రకం)
    {
    స్థితి ఆదానం_Array_2::OPENGL_బఫరు:
        {
            ogl::బఫరు_2& buf = arr.get_OGl_బఫరు_Ref();
            buf.create(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, CV_MAKE_TYPE(ddepth, cn), ogl::బఫరు_2::PIXEL_PACK_బఫరు, autoRelease);
            buf.bind(ogl::బఫరు_2::PIXEL_PACK_బఫరు);
            impl_->copy_గమ్యం(dstFormat, gl_types[ddepth], 0);
            ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_PACK_బఫరు);
            ఆపు;
        }

    స్థితి ఆదానం_Array_2::CUDA_GPU_MAT:
        {
            #నిర్వచితం_కాకపోతే HAVE_CUDA
                throw_no_cuda();
            #లేకపోతే
                ogl::బఫరు_2 buf(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, CV_MAKE_TYPE(ddepth, cn), ogl::బఫరు_2::PIXEL_PACK_బఫరు);
                buf.సమితి_Auto_Release(నిజం);
                buf.bind(ogl::బఫరు_2::PIXEL_PACK_బఫరు);
                impl_->copy_గమ్యం(dstFormat, gl_types[ddepth], 0);
                ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_PACK_బఫరు);
                buf.copy_గమ్యం(arr);
            #అయితే_ముగింపు

            ఆపు;
        }

    అప్రమేయ:
        {
            arr.create(అడ్డువరుసలు_2, నిలువుల_సంఖ్య_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::బఫరు_2::unbind(ogl::బఫరు_2::PIXEL_PACK_బఫరు);
            impl_->copy_గమ్యం(dstFormat, gl_types[ddepth], mat.దత్తాంశం);
        }
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture2D::bind() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_->bind();
#అయితే_ముగింపు
}

గుర్తులేని పూర్ణాంకం cv::ogl::Texture2D::texId() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->texId();
#అయితే_ముగింపు
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#నిర్వచితం_కాకపోతే HAVE_OPENGL

తరగతి cv::ogl::Texture_బఫరు::Impl
{
};

#లేకపోతే

తరగతి cv::ogl::Texture_బఫరు::Impl
{
బహిరంగ:
    స్థిర స్థిరరాశి Ptr<Impl> empty();

    Impl();
    Impl(GLenum అంతర్గత_Format, GLuint bufId, తార్కిక autoRelease);
    ~Impl();

    శూన్య bind(GLint unit) స్థిరరాశి;

    శూన్య సమితి_Auto_Release(తార్కిక flag) { autoRelease_ = flag; }

    GLuint texId() స్థిరరాశి { తిరిగి texId_; }

అంతరంగ:
    GLuint texId_;
    తార్కిక autoRelease_;
};

స్థిరరాశి Ptr<cv::ogl::Texture_బఫరు::Impl> cv::ogl::Texture_బఫరు::Impl::empty()
{
    స్థిర Ptr<Impl> p = makePtr<Impl>();
    తిరిగి p;
}

cv::ogl::Texture_బఫరు::Impl::Impl() : texId_(0), autoRelease_(అబద్ధం)
{
}

cv::ogl::Texture_బఫరు::Impl::Impl(GLenum అంతర్గత_Format, GLuint bufId, తార్కిక autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_బఫరు, texId_);
    CV_CheckGlError();

    gl::Tex_బఫరు(gl::TEXTURE_బఫరు, అంతర్గత_Format, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_బఫరు, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_బఫరు::Impl::~Impl()
{
    ఒకవేళ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

శూన్య cv::ogl::Texture_బఫరు::Impl::bind(GLint unit) స్థిరరాశి
{
    gl::చురుకైన_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_బఫరు, texId_);
    CV_CheckGlError();
}

#అయితే_ముగింపు // HAVE_OPENGL

cv::ogl::Texture_బఫరు::Texture_బఫరు() : format_(NONE)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

cv::ogl::Texture_బఫరు::Texture_బఫరు(స్థిరరాశి బఫరు_2& buf, Format aformat, తార్కిక autoRelease) : format_(NONE)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(buf);
    CV_వాడని(aformat);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture_బఫరు::create(స్థిరరాశి బఫరు_2& buf, Format aformat, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(buf);
    CV_వాడని(aformat);
    CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture_బఫరు::release()
{
#నిర్వచితమైతే HAVE_OPENGL
    ఒకవేళ (impl_)
        impl_->సమితి_Auto_Release(నిజం);
    impl_ = Impl::empty();
#అయితే_ముగింపు
    buf_ = బఫరు_2();
    format_ = NONE;
}

శూన్య cv::ogl::Texture_బఫరు::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    impl_->సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

శూన్య cv::ogl::Texture_బఫరు::bind(పూర్ణాంకం unit) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(unit);
    throw_no_ogl();
#లేకపోతే
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#అయితే_ముగింపు
}

cv::ogl::Texture_బఫరు::Format cv::ogl::Texture_బఫరు::format() స్థిరరాశి
{
    తిరిగి format_;
}

తార్కిక cv::ogl::Texture_బఫరు::empty() స్థిరరాశి
{
    తిరిగి format_ == NONE;
}

గుర్తులేని పూర్ణాంకం cv::ogl::Texture_బఫరు::texId() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->texId();
#అయితే_ముగింపు
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

శూన్య cv::ogl::Arrays::సమితి_శీర్షం_Array(ఆదానం_Array శీర్షం)
{
    స్థిరరాశి పూర్ణాంకం cn = శీర్షం.channels();
    స్థిరరాశి పూర్ణాంకం లోతు = శీర్షం.లోతు();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( లోతు == CV_16S || లోతు == CV_32S || లోతు == CV_32F || లోతు == CV_64F );

    ఒకవేళ (శీర్షం.రకం() == ఆదానం_Array_2::OPENGL_బఫరు)
        శీర్షం_2 = శీర్షం.get_OGl_బఫరు();
    లేకపోతే
        శీర్షం_2.copyFrom(శీర్షం);

    కొలత_3 = శీర్షం_2.size().area();
}

శూన్య cv::ogl::Arrays::reset_శీర్షం_Array()
{
    శీర్షం_2.release();
    కొలత_3 = 0;
}

శూన్య cv::ogl::Arrays::సమితి_Color_Array(ఆదానం_Array color)
{
    స్థిరరాశి పూర్ణాంకం cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    ఒకవేళ (color.రకం() == ఆదానం_Array_2::OPENGL_బఫరు)
        color_ = color.get_OGl_బఫరు();
    లేకపోతే
        color_.copyFrom(color);
}

శూన్య cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

శూన్య cv::ogl::Arrays::సమితి_సాధారణ_Array(ఆదానం_Array సాధారణ)
{
    స్థిరరాశి పూర్ణాంకం cn = సాధారణ.channels();
    స్థిరరాశి పూర్ణాంకం లోతు = సాధారణ.లోతు();

    CV_Assert( cn == 3 );
    CV_Assert( లోతు == CV_8S || లోతు == CV_16S || లోతు == CV_32S || లోతు == CV_32F || లోతు == CV_64F );

    ఒకవేళ (సాధారణ.రకం() == ఆదానం_Array_2::OPENGL_బఫరు)
        సాధారణ_2 = సాధారణ.get_OGl_బఫరు();
    లేకపోతే
        సాధారణ_2.copyFrom(సాధారణ);
}

శూన్య cv::ogl::Arrays::reset_సాధారణ_Array()
{
    సాధారణ_2.release();
}

శూన్య cv::ogl::Arrays::సమితి_Tex_Coord_Array(ఆదానం_Array texCoord)
{
    స్థిరరాశి పూర్ణాంకం cn = texCoord.channels();
    స్థిరరాశి పూర్ణాంకం లోతు = texCoord.లోతు();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( లోతు == CV_16S || లోతు == CV_32S || లోతు == CV_32F || లోతు == CV_64F );

    ఒకవేళ (texCoord.రకం() == ఆదానం_Array_2::OPENGL_బఫరు)
        texCoord_ = texCoord.get_OGl_బఫరు();
    లేకపోతే
        texCoord_.copyFrom(texCoord);
}

శూన్య cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

శూన్య cv::ogl::Arrays::release()
{
    reset_శీర్షం_Array();
    resetColorArray();
    reset_సాధారణ_Array();
    resetTexCoordArray();
}

శూన్య cv::ogl::Arrays::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    శీర్షం_2.సమితి_Auto_Release(flag);
    color_.సమితి_Auto_Release(flag);
    సాధారణ_2.సమితి_Auto_Release(flag);
    texCoord_.సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

శూన్య cv::ogl::Arrays::bind() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == కొలత_3 );
    CV_Assert( సాధారణ_2.empty() || సాధారణ_2.size().area() == కొలత_3 );
    CV_Assert( color_.empty() || color_.size().area() == కొలత_3 );

    ఒకవేళ (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    లేకపోతే
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::బఫరు_2::ARRAY_బఫరు);

        gl::Tex_Coord_సూచిక(texCoord_.channels(), gl_types[texCoord_.లోతు()], 0, 0);
        CV_CheckGlError();
    }

    ఒకవేళ (సాధారణ_2.empty())
    {
        gl::DisableClientState(gl::సాధారణ_ARRAY);
        CV_CheckGlError();
    }
    లేకపోతే
    {
        gl::EnableClientState(gl::సాధారణ_ARRAY);
        CV_CheckGlError();

        సాధారణ_2.bind(ogl::బఫరు_2::ARRAY_బఫరు);

        gl::సాధారణ_సూచిక(gl_types[సాధారణ_2.లోతు()], 0, 0);
        CV_CheckGlError();
    }

    ఒకవేళ (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    లేకపోతే
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::బఫరు_2::ARRAY_బఫరు);

        స్థిరరాశి పూర్ణాంకం cn = color_.channels();

        gl::Color_సూచిక(cn, gl_types[color_.లోతు()], 0, 0);
        CV_CheckGlError();
    }

    ఒకవేళ (శీర్షం_2.empty())
    {
        gl::DisableClientState(gl::శీర్షం_ARRAY);
        CV_CheckGlError();
    }
    లేకపోతే
    {
        gl::EnableClientState(gl::శీర్షం_ARRAY);
        CV_CheckGlError();

        శీర్షం_2.bind(ogl::బఫరు_2::ARRAY_బఫరు);

        gl::శీర్షం_సూచిక(శీర్షం_2.channels(), gl_types[శీర్షం_2.లోతు()], 0, 0);
        CV_CheckGlError();
    }

    ogl::బఫరు_2::unbind(ogl::బఫరు_2::ARRAY_బఫరు);
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#నిర్వచితం_కాకపోతే HAVE_OPENGL

తరగతి cv::ogl::శీర్షం_Array::Impl
{
};

#లేకపోతే

తరగతి cv::ogl::శీర్షం_Array::Impl
{
బహిరంగ:
    స్థిర స్థిరరాశి Ptr<Impl>& empty();

    Impl(GLuint vaId, తార్కిక autoRelease);
    Impl(std::initializer_list<Attribute> లక్షణాలు, తార్కిక autoRelease);
    ~Impl();

    శూన్య bind();

    శూన్య సమితి_Auto_Release(తార్కిక flag) { autoRelease_ = flag; }

    GLuint vaId() స్థిరరాశి { తిరిగి vaId_; }

అంతరంగ:
    Impl();

    GLuint vaId_;
    తార్కిక autoRelease_;
};

స్థిరరాశి Ptr<cv::ogl::శీర్షం_Array::Impl>& cv::ogl::శీర్షం_Array::Impl::empty()
{
    స్థిర Ptr<Impl> p(కొత్త Impl);
    తిరిగి p;
}

cv::ogl::శీర్షం_Array::Impl::Impl() : vaId_(0), autoRelease_(అబద్ధం)
{
}

cv::ogl::శీర్షం_Array::Impl::Impl(GLuint vaId, తార్కిక autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_శీర్షం_Array(vaId) == gl::TRUE_ );
}

cv::ogl::శీర్షం_Array::Impl::Impl(std::initializer_list<Attribute> లక్షణాలు, తార్కిక autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_శీర్షం_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_శీర్షం_Array(vaId_);
    CV_CheckGlError();

    కొరకు (స్వయంచాలక& attribute : లక్షణాలు)
    {
        attribute.బఫరు_3.bind(బఫరు_2::ARRAY_బఫరు);
        ఒకవేళ (attribute.integer_)
        {
            gl::శీర్షం_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.కొలత_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<స్థిరరాశి శూన్య*>(attribute.స్థానభ్రంశం)
            );
            CV_CheckGlError();
        }
        లేకపోతే
        {
            gl::శీర్షం_Attrib_సూచిక(
                attribute.shader_loc_,
                attribute.కొలత_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<స్థిరరాశి శూన్య*>(attribute.స్థానభ్రంశం)
            );
            CV_CheckGlError();
        }

        gl::Enable_శీర్షం_Attrib_Array(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::శీర్షం_Array::Impl::~Impl()
{
    ఒకవేళ (autoRelease_ && vaId_)
        gl::Delete_శీర్షం_Arrays(1, &vaId_);
}

శూన్య cv::ogl::శీర్షం_Array::Impl::bind()
{
    gl::Bind_శీర్షం_Array(vaId_);
    CV_CheckGlError();
}

#అయితే_ముగింపు // HAVE_OPENGL

cv::ogl::శీర్షం_Array::శీర్షం_Array()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

cv::ogl::శీర్షం_Array::శీర్షం_Array(std::initializer_list<Attribute> లక్షణాలు, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(లక్షణాలు); CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(లక్షణాలు, autoRelease));
#అయితే_ముగింపు
}

శూన్య cv::ogl::శీర్షం_Array::create(std::initializer_list<Attribute> లక్షణాలు, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(లక్షణాలు); CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(లక్షణాలు, autoRelease));
#అయితే_ముగింపు
}

శూన్య cv::ogl::శీర్షం_Array::release()
{
#నిర్వచితమైతే HAVE_OPENGL
    ఒకవేళ (impl_)
        impl_->సమితి_Auto_Release(నిజం);
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

శూన్య cv::ogl::శీర్షం_Array::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    impl_->సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

శూన్య cv::ogl::శీర్షం_Array::bind() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_->bind();
#అయితే_ముగింపు
}

శూన్య cv::ogl::శీర్షం_Array::unbind()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    gl::Bind_శీర్షం_Array(0);
#అయితే_ముగింపు
}

గుర్తులేని పూర్ణాంకం cv::ogl::శీర్షం_Array::vaId() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->vaId();
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#నిర్వచితం_కాకపోతే HAVE_OPENGL

తరగతి cv::ogl::Shader::Impl
{
};

#లేకపోతే

తరగతి cv::ogl::Shader::Impl
{
బహిరంగ:
    స్థిర స్థిరరాశి Ptr<Impl>& empty();

    Impl(GLuint shaderId, తార్కిక autoRelease);
    Impl(స్థిరరాశి అక్షరం* src, GLenum type, తార్కిక autoRelease);
    ~Impl();

    శూన్య సమితి_Auto_Release(తార్కిక flag) { autoRelease_ = flag; }

    GLuint shaderId() స్థిరరాశి { తిరిగి shaderId_; }

అంతరంగ:
    Impl();

    GLuint shaderId_;
    తార్కిక autoRelease_;
};

స్థిరరాశి Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    స్థిర Ptr<Impl> p(కొత్త Impl);
    తిరిగి p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(అబద్ధం)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, తార్కిక autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(స్థిరరాశి అక్షరం* src, GLenum type, తార్కిక autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint స్థితి_2;
    gl::Shader_ఆధారం(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_స్థితి, &స్థితి_2);
    ఒకవేళ (!స్థితి_2) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_నిడివి, &స్థితి_2);
        info_log.resize(స్థితి_2);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, శూన్యసూచిక, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    ఒకవేళ (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#అయితే_ముగింపు // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::శీర్షం_3)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

cv::ogl::Shader::Shader(స్థిరరాశి అక్షరం* src, ShaderType type, తార్కిక autoRelease) : type_(type)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(src); CV_వాడని(type); CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(src, type, autoRelease));
#అయితే_ముగింపు
}

శూన్య cv::ogl::Shader::create(స్థిరరాశి అక్షరం* src, ShaderType type, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(src); CV_వాడని(type); CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(src, type, autoRelease));
    type_ = type;
#అయితే_ముగింపు
}

శూన్య cv::ogl::Shader::release()
{
#నిర్వచితమైతే HAVE_OPENGL
    ఒకవేళ (impl_)
        impl_->సమితి_Auto_Release(నిజం);
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

శూన్య cv::ogl::Shader::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    impl_->సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

గుర్తులేని పూర్ణాంకం cv::ogl::Shader::shaderId() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->shaderId();
#అయితే_ముగింపు
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() స్థిరరాశి
{
    తిరిగి type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#నిర్వచితం_కాకపోతే HAVE_OPENGL

తరగతి cv::ogl::కార్యక్రమం_2::Impl
{
};

#లేకపోతే

తరగతి cv::ogl::కార్యక్రమం_2::Impl
{
బహిరంగ:
    స్థిర స్థిరరాశి Ptr<Impl>& empty();

    Impl(GLuint కార్యక్రమం_Id, తార్కిక autoRelease);
    Impl(స్థిరరాశి Shader& vert, స్థిరరాశి Shader& frag, తార్కిక autoRelease);
    ~Impl();

    శూన్య bind();

    పూర్ణాంకం getAttributeLocation(స్థిరరాశి అక్షరం* name) స్థిరరాశి;
    పూర్ణాంకం get_ఏకరీతి_Location(స్థిరరాశి అక్షరం* name) స్థిరరాశి;

    శూన్య సమితి_Auto_Release(తార్కిక flag) { autoRelease_ = flag; }

    GLuint కార్యక్రమం_Id() స్థిరరాశి { తిరిగి కార్యక్రమం_Id_2; }

అంతరంగ:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint కార్యక్రమం_Id_2;
    తార్కిక autoRelease_;
};

స్థిరరాశి Ptr<cv::ogl::కార్యక్రమం_2::Impl>& cv::ogl::కార్యక్రమం_2::Impl::empty()
{
    స్థిర Ptr<Impl> p(కొత్త Impl);
    తిరిగి p;
}

cv::ogl::కార్యక్రమం_2::Impl::Impl() : కార్యక్రమం_Id_2(0), autoRelease_(అబద్ధం)
{
}

cv::ogl::కార్యక్రమం_2::Impl::Impl(GLuint కార్యక్రమం_Id, తార్కిక autoRelease) : కార్యక్రమం_Id_2(కార్యక్రమం_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_కార్యక్రమం(కార్యక్రమం_Id) == gl::TRUE_ );
}

cv::ogl::కార్యక్రమం_2::Impl::Impl(స్థిరరాశి Shader& vert, స్థిరరాశి Shader& frag, తార్కిక autoRelease) : కార్యక్రమం_Id_2(0), autoRelease_(autoRelease)
{
    కార్యక్రమం_Id_2 = gl::Create_కార్యక్రమం();
    CV_CheckGlError();

    CV_Assert( కార్యక్రమం_Id_2 != 0 );

    gl::AttachShader(కార్యక్రమం_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(కార్యక్రమం_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint స్థితి_2;
    gl::Link_కార్యక్రమం(కార్యక్రమం_Id_2);
    gl::GetProgramiv(కార్యక్రమం_Id_2, gl::LINK_స్థితి, &స్థితి_2);
    ఒకవేళ (!స్థితి_2) {
        String info_log;
        gl::GetProgramiv(కార్యక్రమం_Id_2, gl::INFO_LOG_నిడివి, &స్థితి_2);
        info_log.resize(స్థితి_2);
        gl::Get_కార్యక్రమం_Info_Log(కార్యక్రమం_Id_2, GLsizei(info_log.size()) + 1, శూన్యసూచిక, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::కార్యక్రమం_2::Impl::~Impl()
{
    ఒకవేళ (autoRelease_ && కార్యక్రమం_Id_2)
        gl::Delete_కార్యక్రమం(కార్యక్రమం_Id_2);
}

శూన్య cv::ogl::కార్యక్రమం_2::Impl::bind()
{
    gl::Use_కార్యక్రమం(కార్యక్రమం_Id_2);
    CV_CheckGlError();
}

పూర్ణాంకం cv::ogl::కార్యక్రమం_2::Impl::getAttributeLocation(స్థిరరాశి అక్షరం* name) స్థిరరాశి
{
    // -1 if absent (e.g. optimized out); caller decides.
    పూర్ణాంకం location = gl::GetAttribLocation(కార్యక్రమం_Id_2, name);
    CV_CheckGlError();
    తిరిగి location;
}

పూర్ణాంకం cv::ogl::కార్యక్రమం_2::Impl::get_ఏకరీతి_Location(స్థిరరాశి అక్షరం* name) స్థిరరాశి
{
    // -1 if absent (e.g. optimized out); caller decides.
    పూర్ణాంకం location = gl::Get_ఏకరీతి_Location(కార్యక్రమం_Id_2, name);
    CV_CheckGlError();
    తిరిగి location;
}

#అయితే_ముగింపు // HAVE_OPENGL

cv::ogl::కార్యక్రమం_2::కార్యక్రమం_2()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

cv::ogl::కార్యక్రమం_2::కార్యక్రమం_2(స్థిరరాశి Shader& vert, స్థిరరాశి Shader& frag, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(vert); CV_వాడని(frag); CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(vert, frag, autoRelease));
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::create(స్థిరరాశి Shader& vert, స్థిరరాశి Shader& frag, తార్కిక autoRelease)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(vert); CV_వాడని(frag); CV_వాడని(autoRelease);
    throw_no_ogl();
#లేకపోతే
    impl_.reset(కొత్త Impl(vert, frag, autoRelease));
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::release()
{
#నిర్వచితమైతే HAVE_OPENGL
    ఒకవేళ (impl_)
        impl_->సమితి_Auto_Release(నిజం);
    impl_ = Impl::empty();
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_Auto_Release(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    impl_->సమితి_Auto_Release(flag);
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::bind() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    impl_->bind();
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::unbind()
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    gl::Use_కార్యక్రమం(0);
#అయితే_ముగింపు
}

పూర్ణాంకం cv::ogl::కార్యక్రమం_2::getAttributeLocation(స్థిరరాశి అక్షరం* name) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(name);
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->getAttributeLocation(name);
#అయితే_ముగింపు
}

పూర్ణాంకం cv::ogl::కార్యక్రమం_2::get_ఏకరీతి_Location(స్థిరరాశి అక్షరం* name) స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(name);
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->get_ఏకరీతి_Location(name);
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_ఏకరీతి_Vec3(పూర్ణాంకం loc, Vec3f vec)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(loc); CV_వాడని(vec);
    throw_no_ogl();
#లేకపోతే
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_ఏకరీతి_Vec2(పూర్ణాంకం loc, Vec2f vec)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(loc); CV_వాడని(vec);
    throw_no_ogl();
#లేకపోతే
    gl::Uniform2f(loc, vec(0), vec(1));
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_ఏకరీతి_Vec4(పూర్ణాంకం loc, Vec4f vec)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(loc); CV_వాడని(vec);
    throw_no_ogl();
#లేకపోతే
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_ఏకరీతి_Mat4x4(పూర్ణాంకం loc, Matx44f mat)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(loc); CV_వాడని(mat);
    throw_no_ogl();
#లేకపోతే
    gl::ఏకరీతి_Matrix4fv(loc, 1, నిజం, mat.val);
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_Uniform1f(పూర్ణాంకం loc, దశాంశం value)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(loc); CV_వాడని(value);
    throw_no_ogl();
#లేకపోతే
    gl::Uniform1f(loc, value);
#అయితే_ముగింపు
}

శూన్య cv::ogl::కార్యక్రమం_2::సమితి_Uniform1i(పూర్ణాంకం loc, పూర్ణాంకం value)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(loc); CV_వాడని(value);
    throw_no_ogl();
#లేకపోతే
    gl::Uniform1i(loc, value);
#అయితే_ముగింపు
}

గుర్తులేని పూర్ణాంకం cv::ogl::కార్యక్రమం_2::కార్యక్రమం_Id() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    throw_no_ogl();
#లేకపోతే
    తిరిగి impl_->కార్యక్రమం_Id();
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// Rendering

శూన్య cv::ogl::render(స్థిరరాశి ogl::Texture2D& tex, Rect_<ద్విగుణ> wndRect, Rect_<ద్విగుణ> texRect)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(tex);
    CV_వాడని(wndRect);
    CV_వాడని(texRect);
    throw_no_ogl();
#లేకపోతే
    ఒకవేళ (!tex.empty())
    {
        gl::మాతృక_బహుళకం(gl::PROJECTION);
        gl::ఎక్కించు_తటస్థం();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::మాతృక_బహుళకం(gl::MODELVIEW);
        gl::ఎక్కించు_తటస్థం();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_బహుళకం, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::రేఖీయ);
        CV_CheckGlError();

        స్థిరరాశి ద్విగుణ శీర్షం[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.ఎత్తు), 0.0,
            wndRect.x + wndRect.వెడల్పు, (wndRect.y + wndRect.ఎత్తు), 0.0,
            wndRect.x + wndRect.వెడల్పు, wndRect.y, 0.0
        };
        స్థిరరాశి ద్విగుణ texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.ఎత్తు,
            texRect.x + texRect.వెడల్పు, texRect.y + texRect.ఎత్తు,
            texRect.x + texRect.వెడల్పు, texRect.y
        };

        ogl::బఫరు_2::unbind(ogl::బఫరు_2::ARRAY_బఫరు);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_సూచిక(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::సాధారణ_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::శీర్షం_ARRAY);
        CV_CheckGlError();

        gl::శీర్షం_సూచిక(3, gl::DOUBLE, 0, శీర్షం);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::render(స్థిరరాశి ogl::Arrays& arr, పూర్ణాంకం బహుళకం, Scalar color)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(బహుళకం);
    CV_వాడని(color);
    throw_no_ogl();
#లేకపోతే
    ఒకవేళ (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(బహుళకం, 0, arr.size());
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::render(స్థిరరాశి ogl::Arrays& arr, ఆదానం_Array indices, పూర్ణాంకం బహుళకం, Scalar color)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(arr);
    CV_వాడని(indices);
    CV_వాడని(బహుళకం);
    CV_వాడని(color);
    throw_no_ogl();
#లేకపోతే
    ఒకవేళ (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        స్థిరరాశి పూర్ణాంకం రకం = indices.రకం();

        ఎంపిక (రకం)
        {
        స్థితి ఆదానం_Array_2::OPENGL_బఫరు :
            {
                ogl::బఫరు_2 buf = indices.get_OGl_బఫరు();

                స్థిరరాశి పూర్ణాంకం లోతు = buf.లోతు();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( లోతు <= CV_32S );

                GLenum type;
                ఒకవేళ (లోతు < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                లేకపోతే ఒకవేళ (లోతు < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                లేకపోతే
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::బఫరు_2::ELEMENT_ARRAY_బఫరు);

                gl::DrawElements(బహుళకం, buf.size().area(), type, 0);

                ogl::బఫరు_2::unbind(ogl::బఫరు_2::ELEMENT_ARRAY_బఫరు);

                ఆపు;
            }

        అప్రమేయ:
            {
                Mat mat = indices.getMat();

                స్థిరరాశి పూర్ణాంకం లోతు = mat.లోతు();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( లోతు <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                ఒకవేళ (లోతు < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                లేకపోతే ఒకవేళ (లోతు < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                లేకపోతే
                    type = gl::UNSIGNED_INT;

                ogl::బఫరు_2::unbind(ogl::బఫరు_2::ELEMENT_ARRAY_బఫరు);

                gl::DrawElements(బహుళకం, mat.size().area(), type, mat.దత్తాంశం);
            }
        }
    }
#అయితే_ముగింపు
}

శూన్య cv::ogl::clearColor(Scalar color)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(color);
    throw_no_ogl();
#లేకపోతే
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_బఫరు_బిట్టు);
#అయితే_ముగింపు // HAVE_OPENGL
}

శూన్య cv::ogl::clear_లోతు(దశాంశం లోతు)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(లోతు);
    throw_no_ogl();
#లేకపోతే
    gl::Clear_లోతు(లోతు);
    gl::Clear(gl::లోతు_బఫరు_బిట్టు);
#అయితే_ముగింపు // HAVE_OPENGL
}

శూన్య cv::ogl::drawArrays(పూర్ణాంకం first, పూర్ణాంకం లెక్క_2, పూర్ణాంకం బహుళకం)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(first); CV_వాడని(లెక్క_2); CV_వాడని(బహుళకం);
    throw_no_ogl();
#లేకపోతే
    gl::DrawArrays(బహుళకం, first, లెక్క_2);
#అయితే_ముగింపు // HAVE_OPENGL
}

శూన్య cv::ogl::drawElements(పూర్ణాంకం first, పూర్ణాంకం లెక్క_2, పూర్ణాంకం type, పూర్ణాంకం బహుళకం)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(first); CV_వాడని(లెక్క_2); CV_వాడని(type); CV_వాడని(బహుళకం);
    throw_no_ogl();
#లేకపోతే
    gl::DrawElements(బహుళకం, లెక్క_2, type, reinterpret_cast<స్థిరరాశి శూన్య*>(static_cast<size_t>(first)));
#అయితే_ముగింపు
}

శూన్య cv::ogl::drawElementsInstanced(పూర్ణాంకం first, పూర్ణాంకం లెక్క_2, పూర్ణాంకం type, పూర్ణాంకం బహుళకం, పూర్ణాంకం instance_లెక్క)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(first); CV_వాడని(లెక్క_2); CV_వాడని(type); CV_వాడని(బహుళకం); CV_వాడని(instance_లెక్క);
    throw_no_ogl();
#లేకపోతే
    gl::DrawElementsInstanced(బహుళకం, లెక్క_2, type, reinterpret_cast<స్థిరరాశి శూన్య*>(static_cast<size_t>(first)), instance_లెక్క);
    CV_CheckGlError();
#అయితే_ముగింపు
}

శూన్య cv::ogl::blendFunc(పూర్ణాంకం sfactor, పూర్ణాంకం dfactor)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(sfactor); CV_వాడని(dfactor);
    throw_no_ogl();
#లేకపోతే
    gl::BlendFunc(sfactor, dfactor);
#అయితే_ముగింపు
}

శూన్య cv::ogl::లోతు_ముసుగు_2(తార్కిక flag)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(flag);
    throw_no_ogl();
#లేకపోతే
    gl::లోతు_ముసుగు(flag ? gl::TRUE_ : gl::FALSE_);
#అయితే_ముగింపు
}

శూన్య cv::ogl::enable(పూర్ణాంకం cap)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(cap);
    throw_no_ogl();
#లేకపోతే
    gl::Enable(cap);
#అయితే_ముగింపు
}

శూన్య cv::ogl::disable(పూర్ణాంకం cap)
{
#నిర్వచితం_కాకపోతే HAVE_OPENGL
    CV_వాడని(cap);
    throw_no_ogl();
#లేకపోతే
    gl::Disable(cap);
#అయితే_ముగింపు
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#నిర్వచితమైతే HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  లేకపోతే
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#లేకపోతే // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#అయితే_ముగింపు // HAVE_OPENCL

#ఒకవేళ defined(HAVE_OPENGL)
#  ఒకవేళ defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#అయితే_ముగింపు // HAVE_OPENGL

నామాకాశ cv { నామాకాశ ogl {

#ఒకవేళ defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
స్థిర శూన్య checkOpenCLVersion()
{
    ఉపయోగించి నామాకాశ cv::ocl;
    స్థిరరాశి Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int స్థితి_2 = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, పరిమాణం(platform_id), &platform_id, &sz);
    CV_Assert(స్థితి_2 == CL_SUCCESS && sz == పరిమాణం(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    పూర్ణాంకం సంస్కరణ_Major_2 = pi.సంస్కరణ_Major_2();
    పూర్ణాంకం సంస్కరణ_Minor_2 = pi.సంస్కరణ_Minor_2();
    ఒకవేళ (సంస్కరణ_Major_2 < 1 || (సంస్కరణ_Major_2 == 1 && సంస్కరణ_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                సంస్కరణ_Major_2, సంస్కరణ_Minor_2, pi.name().c_str(), pi.సంస్కరణ().c_str())
        );
}
#అయితే_ముగింపు

నామాకాశ ocl {

Context& initializeContextFromGL()
{
#ఒకవేళ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#లేకపోతే
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = శూన్యసూచిక;
    cl_uint dev_వాడిన = 0;
    cl_context context = శూన్యసూచిక;

    cl_int స్థితి_2 = clGetPlatformIDs(0, ఖాళీ, &platformsCnt);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", స్థితి_2));
    ఒకవేళ (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    స్థితి_2 = clGetPlatformIDs(platformsCnt, &platforms[0], ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", స్థితి_2));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    తార్కిక sharingSupported = అబద్ధం;

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        స్థితి_2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ఖాళీ, &devCnt);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", స్థితి_2));

        ప్రయత్నించు {
            devices = కొత్త cl_device_id[devCnt];

            స్థితి_2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ఖాళీ);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", స్థితి_2));

            కొరకు (గుర్తులేని పూర్ణాంకం j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_కొలత;
                స్థితి_2 = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ఖాళీ, &extension_కొలత );
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", స్థితి_2));

                ఒకవేళ(extension_కొలత > 0)
                {
                    std::string devString(extension_కొలత, '\0');
                    స్థితి_2 = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_కొలత);
                    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                        కొనసాగు;
                    ఒకవేళ (extension_కొలత > 0 && devString.size() >= extension_కొలత)
                        devString.resize(extension_కొలత - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    అయితే (spacePos != devString.npos) {
                        ఒకవేళ (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_వాడిన = i;
                            sharingSupported = నిజం;
                            ఆపు;
                        }
                        చేయి {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } అయితే (spacePos == oldPos);
                    }
                }
            }
        } పట్టుకో(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            ఒకవేళ(devices != శూన్యసూచిక) {
                తొలగించు[] devices;
            }
            కొనసాగు;
        }

        ఒకవేళ (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#ఒకవేళ defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_సమూహం_Obj cgl_Share_సమూహం = CGLGet_Share_సమూహం(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_సమూహం,
                    0
                };
                context = clCreateContext(props, 0,0, ఖాళీ, ఖాళీ, &ci_Err_అంకె);
#లేదావేళ defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_వాడిన], ఖాళీ, ఖాళీ, &స్థితి_2);
#లేదావేళ defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_వాడిన], ఖాళీ, ఖాళీ, &స్థితి_2);
#లేదావేళ defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_వాడిన], ఖాళీ, ఖాళీ, &స్థితి_2);
#అయితే_ముగింపు
        }

        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", స్థితి_2));
        లేకపోతే
            ఆపు;
    }


    cl_platform_id platform = platforms[dev_వాడిన];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_వాడిన]);
    clReleaseDevice(devices[dev_వాడిన]);
    clReleaseContext(context);
    clExecCtx.bind();
    తిరిగి const_cast<Context&>(clExecCtx.getContext());
#అయితే_ముగింపు
}

} // namespace cv::ogl::ocl

శూన్య convert_గమ్యం_GLTexture2_D(ఆదానం_Array src, Texture2D& texture)
{
    CV_వాడని(src); CV_వాడని(texture);
#ఒకవేళ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#లేకపోతే
    కొలత src_కొలత = src.size();
    CV_Assert(src_కొలత.వెడల్పు == (పూర్ణాంకం)texture.నిలువుల_సంఖ్య() && src_కొలత.ఎత్తు == (పూర్ణాంకం)texture.అడ్డువరుసలు());

    ఉపయోగించి నామాకాశ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int స్థితి_2 = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", స్థితి_2));

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = cl_వరుసలో_చేర్చు_Acquire_GLObjects(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", స్థితి_2));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1};
    స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_బఫరు_గమ్యం_Image(q, cl_బఫరు, clImage, offset, dst_origin, region, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", స్థితి_2));
    స్థితి_2 = cl_వరుసలో_చేర్చు_Release_GLObjects(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", స్థితి_2));

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", స్థితి_2));

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", స్థితి_2));
#అయితే_ముగింపు
}

శూన్య convertFromGLTexture2D(స్థిరరాశి Texture2D& texture, OutputArray dst)
{
    CV_వాడని(texture); CV_వాడని(dst);
#ఒకవేళ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#లేకపోతే
    // check texture format
    స్థిరరాశి పూర్ణాంకం dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    పూర్ణాంకం textureType = dtype;
    CV_Assert(textureType >= 0);

    ఉపయోగించి నామాకాశ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int స్థితి_2 = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", స్థితి_2));

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = cl_వరుసలో_చేర్చు_Acquire_GLObjects(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", స్థితి_2));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1};
    స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_Image_గమ్యం_బఫరు(q, clImage, cl_బఫరు, src_origin, region, offset, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", స్థితి_2));
    స్థితి_2 = cl_వరుసలో_చేర్చు_Release_GLObjects(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", స్థితి_2));

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", స్థితి_2));

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", స్థితి_2));
#అయితే_ముగింపు
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(స్థిరరాశి బఫరు_2& బఫరు, AccessFlag accessFlags)
{
    CV_వాడని(బఫరు); CV_వాడని(accessFlags);
#ఒకవేళ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#లేకపోతే
    ఉపయోగించి నామాకాశ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_వరుస cl_వరుస = (cl_command_వరుస)వరుస::getDefault().ptr();

    పూర్ణాంకం clAccessFlags = 0;
    ఎంపిక (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    అప్రమేయ:
    స్థితి ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        ఆపు;
    స్థితి ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        ఆపు;
    స్థితి ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        ఆపు;
    }

    cl_int స్థితి_2 = 0;
    cl_mem cl_బఫరు = clCreateFromGLBuffer(context, clAccessFlags, బఫరు.bufId(), &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", స్థితి_2));

    gl::ముగింపు();

    స్థితి_2 = cl_వరుసలో_చేర్చు_Acquire_GLObjects(cl_వరుస, 1, &cl_బఫరు, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", స్థితి_2));

    size_t అడుగు = బఫరు.నిలువుల_సంఖ్య() * బఫరు.elem_కొలత();
    పూర్ణాంకం అడ్డువరుసలు = బఫరు.అడ్డువరుసలు();
    పూర్ణాంకం నిలువుల_సంఖ్య = బఫరు.నిలువుల_సంఖ్య();
    పూర్ణాంకం type = బఫరు.type();

    UMat u;
    convert_From_బఫరు(cl_బఫరు, అడుగు, అడ్డువరుసలు, నిలువుల_సంఖ్య, type, u);
    తిరిగి u;
#అయితే_ముగింపు
}

శూన్య unmapGLBuffer(UMat& u)
{
    CV_వాడని(u);
#ఒకవేళ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#లేకపోతే
    ఉపయోగించి నామాకాశ cv::ocl;
    cl_command_వరుస cl_వరుస = (cl_command_వరుస)వరుస::getDefault().ptr();

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int స్థితి_2 = cl_వరుసలో_చేర్చు_Release_GLObjects(cl_వరుస, 1, &cl_బఫరు, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", స్థితి_2));

    స్థితి_2 = cl_ముగింపు(cl_వరుస);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", స్థితి_2));

    స్థితి_2 = clReleaseMemObject(cl_బఫరు);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", స్థితి_2));
#అయితే_ముగింపు
}

}} // namespace cv::ogl
