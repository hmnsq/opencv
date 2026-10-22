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

#અન્તર્ભાવ "precomp.hpp"

#જો defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#નહીંતર
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    જો (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#નહીંતર // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using નામાકાશ cv;
using નામાકાશ cv::cuda;

#જો defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

નામાકાશ
{
#ifndef HAVE_OPENGL
અંતઃરેખ સ્થિર CV_NORETURN શૂન્ય throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#અથવાજો defined _DEBUG
અંતઃરેખ સ્થિર તાર્કિક checkError(અચલ અક્ષર* file, અચલ પૂર્ણાંક લીટી, અચલ અક્ષર* func = 0)
{
    GLenum err = gl::GetError();
    જો (err != gl::NO_ERROR_)
    {
        અચલ અક્ષર* સંદેશ;
        પસંદગી (err)
        {
        સ્થિતિ gl::INVALID_ENUM:
            સંદેશ = "An unacceptable value is specified for an enumerated argument";
            તોડો;
        સ્થિતિ gl::INVALID_કિંમત:
            સંદેશ = "A numeric argument is out of range";
            તોડો;
        સ્થિતિ gl::INVALID_OPERATION:
            સંદેશ = "The specified operation is not allowed in the current state";
            તોડો;
        સ્થિતિ gl::OUT_OF_MEMORY:
            સંદેશ = "There is not enough memory left to execute the command";
            તોડો;
        મૂળભૂત:
            સંદેશ = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, સંદેશ, file, લીટી);
    }
    પાછા સાચું;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
નામાકાશ
{
    અચલ GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

શૂન્ય cv::cuda::setGlDevice(પૂર્ણાંક device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #નહીંતર
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#જો defined(HAVE_OPENGL) && defined(HAVE_CUDA)

નામાકાશ
{
    વર્ગ CudaResource
    {
    જાહેર:
        CudaResource();
        ~CudaResource();

        શૂન્ય registerBuffer(GLuint buffer);
        શૂન્ય release();

        શૂન્ય copyFrom(અચલ શૂન્ય* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        શૂન્ય copyTo(શૂન્ય* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        શૂન્ય* map(cudaStream_t stream = 0);
        શૂન્ય unmap(cudaStream_t stream = 0);

    ખાનગી:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        વર્ગ GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    શૂન્ય CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        જો (buffer_ == buffer)
            પાછા;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    શૂન્ય CudaResource::release()
    {
        જો (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    વર્ગ CudaResource::GraphicsMapHolder
    {
    જાહેર:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        શૂન્ય reset();

    ખાનગી:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        જો (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        જો (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    શૂન્ય CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    શૂન્ય CudaResource::copyFrom(અચલ શૂન્ય* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        શૂન્ય* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        જો (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        નહીંતર
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    શૂન્ય CudaResource::copyTo(શૂન્ય* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        શૂન્ય* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        જો (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        નહીંતર
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    શૂન્ય* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        શૂન્ય* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        પાછા ptr;
    }

    શૂન્ય CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

વર્ગ cv::ogl::Buffer::Impl
{
};

#નહીંતર

વર્ગ cv::ogl::Buffer::Impl
{
જાહેર:
    સ્થિર અચલ Ptr<Impl>& empty();

    Impl(GLuint bufId, તાર્કિક autoRelease);
    Impl(GLsizeiptr size, અચલ GLvoid* data, GLenum target, તાર્કિક autoRelease);
    ~Impl();

    શૂન્ય bind(GLenum target) અચલ;

    શૂન્ય copyFrom(GLuint srcBuf, GLsizeiptr size);

    શૂન્ય copyFrom(GLsizeiptr size, અચલ GLvoid* data);
    શૂન્ય copyTo(GLsizeiptr size, GLvoid* data) અચલ;

    શૂન્ય resize(GLsizeiptr size, GLenum target);

    શૂન્ય* mapHost(GLenum access);
    શૂન્ય unmapHost();

#ifdef HAVE_CUDA
    શૂન્ય copyFrom(અચલ શૂન્ય* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    શૂન્ય copyTo(શૂન્ય* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) અચલ;

    શૂન્ય* mapDevice(cudaStream_t stream = 0);
    શૂન્ય unmapDevice(cudaStream_t stream = 0);
#endif

    શૂન્ય setAutoRelease(તાર્કિક flag) { autoRelease_ = flag; }

    GLuint bufId() અચલ { પાછા bufId_; }

ખાનગી:
    Impl();

    GLuint bufId_;
    તાર્કિક autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

અચલ Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    સ્થિર Ptr<Impl> p(નવું Impl);
    પાછા p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(ખોટું)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, તાર્કિક autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, અચલ GLvoid* data, GLenum target, તાર્કિક autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    gl::BufferData(target, size, data, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

cv::ogl::Buffer::Impl::~Impl()
{
    જો (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

શૂન્ય cv::ogl::Buffer::Impl::bind(GLenum target) અચલ
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

શૂન્ય cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

શૂન્ય cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, અચલ GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

શૂન્ય cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

શૂન્ય cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) અચલ
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

શૂન્ય* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    પાછા data;
}

શૂન્ય cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

શૂન્ય cv::ogl::Buffer::Impl::copyFrom(અચલ શૂન્ય* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

શૂન્ય cv::ogl::Buffer::Impl::copyTo(શૂન્ય* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) અચલ
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

શૂન્ય* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    પાછા cudaResource_.map(stream);
}

શૂન્ય cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(પૂર્ણાંક arows, પૂર્ણાંક acols, પૂર્ણાંક atype, unsigned પૂર્ણાંક abufId, તાર્કિક autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, પૂર્ણાંક atype, unsigned પૂર્ણાંક abufId, તાર્કિક autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(આગત_Array arr, Target target, તાર્કિક autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    અચલ પૂર્ણાંક kind = arr.kind();

    પસંદગી (kind)
    {
    સ્થિતિ આગત_Array_2::OPENGL_BUFFER:
    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        તોડો;

    મૂળભૂત:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            અચલ GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(નવું Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            તોડો;
        }
    }
#endif
}

શૂન્ય cv::ogl::Buffer::create(પૂર્ણાંક arows, પૂર્ણાંક acols, પૂર્ણાંક atype, Target target, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    જો (rows_ != arows || cols_ != acols || type_ != atype)
    {
        અચલ GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        જો (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        નહીંતર
            impl_.reset(નવું Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

શૂન્ય cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    જો (impl_)
        impl_->setAutoRelease(સાચું);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

શૂન્ય cv::ogl::Buffer::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    impl_->setAutoRelease(flag);
#endif
}

શૂન્ય cv::ogl::Buffer::copyFrom(આગત_Array arr, Target target, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    અચલ પૂર્ણાંક kind = arr.kind();

    અચલ Size asize = arr.size();
    અચલ પૂર્ણાંક atype = arr.type();
    create(asize, atype, target, autoRelease);

    પસંદગી (kind)
    {
    સ્થિતિ આગત_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            તોડો;
        }

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #નહીંતર
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            તોડો;
        }

    મૂળભૂત:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

શૂન્ય cv::ogl::Buffer::copyFrom(આગત_Array arr, cuda::Stream& stream, Target target, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #નહીંતર
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

શૂન્ય cv::ogl::Buffer::copyTo(OutputArray arr) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#નહીંતર
    અચલ પૂર્ણાંક kind = arr.kind();

    પસંદગી (kind)
    {
    સ્થિતિ આગત_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*આ);
            તોડો;
        }

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #નહીંતર
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            તોડો;
        }

    મૂળભૂત:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

શૂન્ય cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #નહીંતર
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, તાર્કિક autoRelease) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    ogl::Buffer buf;
    buf.copyFrom(*આ, target, autoRelease);
    પાછા buf;
#endif
}

શૂન્ય cv::ogl::Buffer::bind(Target target) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#નહીંતર
    impl_->bind(target);
#endif
}

શૂન્ય cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#નહીંતર
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#નહીંતર
    પાછા Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

શૂન્ય cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #નહીંતર
        પાછા GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

શૂન્ય cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #નહીંતર
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #નહીંતર
        પાછા GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

શૂન્ય cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#નહીંતર
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #નહીંતર
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned પૂર્ણાંક cv::ogl::Buffer::bufId() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

વર્ગ cv::ogl::Texture2D::Impl
{
};

#નહીંતર

વર્ગ cv::ogl::Texture2D::Impl
{
જાહેર:
    સ્થિર અચલ Ptr<Impl> empty();

    Impl(GLuint texId, તાર્કિક autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, અચલ GLvoid* pixels, તાર્કિક autoRelease);
    ~Impl();

    શૂન્ય copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, અચલ GLvoid *pixels);
    શૂન્ય copyTo(GLenum format, GLenum type, GLvoid* pixels) અચલ;

    શૂન્ય bind() અચલ;

    શૂન્ય setAutoRelease(તાર્કિક flag) { autoRelease_ = flag; }

    GLuint texId() અચલ { પાછા texId_; }

ખાનગી:
    Impl();

    GLuint texId_;
    તાર્કિક autoRelease_;
};

અચલ Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    સ્થિર Ptr<Impl> p(નવું Impl);
    પાછા p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(ખોટું)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, તાર્કિક autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, અચલ GLvoid* pixels, તાર્કિક autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    જો (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

શૂન્ય cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, અચલ GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, width, height, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

શૂન્ય cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) અચલ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

શૂન્ય cv::ogl::Texture2D::Impl::bind() અચલ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(પૂર્ણાંક arows, પૂર્ણાંક acols, Format aformat, unsigned પૂર્ણાંક atexId, તાર્કિક autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned પૂર્ણાંક atexId, તાર્કિક autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(આગત_Array arr, તાર્કિક autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    અચલ પૂર્ણાંક kind = arr.kind();

    અચલ Size asize = arr.size();
    અચલ પૂર્ણાંક atype = arr.type();

    અચલ પૂર્ણાંક depth = CV_MAT_DEPTH(atype);
    અચલ પૂર્ણાંક cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    અચલ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    અચલ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    પસંદગી (kind)
    {
    સ્થિતિ આગત_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(નવું Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            તોડો;
        }

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #નહીંતર
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(સાચું);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(નવું Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            તોડો;
        }

    મૂળભૂત:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(નવું Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            તોડો;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

શૂન્ય cv::ogl::Texture2D::create(પૂર્ણાંક arows, પૂર્ણાંક acols, Format aformat, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    જો (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(નવું Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

શૂન્ય cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    જો (impl_)
        impl_->setAutoRelease(સાચું);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

શૂન્ય cv::ogl::Texture2D::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    impl_->setAutoRelease(flag);
#endif
}

શૂન્ય cv::ogl::Texture2D::copyFrom(આગત_Array arr, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    અચલ પૂર્ણાંક kind = arr.kind();

    અચલ Size asize = arr.size();
    અચલ પૂર્ણાંક atype = arr.type();

    અચલ પૂર્ણાંક depth = CV_MAT_DEPTH(atype);
    અચલ પૂર્ણાંક cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    અચલ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    અચલ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    પસંદગી(kind)
    {
    સ્થિતિ આગત_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            તોડો;
        }

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #નહીંતર
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(સાચું);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            તોડો;
        }

    મૂળભૂત:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

શૂન્ય cv::ogl::Texture2D::copyTo(OutputArray arr, પૂર્ણાંક ddepth, તાર્કિક autoRelease) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    અચલ પૂર્ણાંક kind = arr.kind();

    અચલ પૂર્ણાંક cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    અચલ GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    પસંદગી(kind)
    {
    સ્થિતિ આગત_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            તોડો;
        }

    સ્થિતિ આગત_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #નહીંતર
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(સાચું);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            તોડો;
        }

    મૂળભૂત:
        {
            arr.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], mat.data);
        }
    }
#endif
}

શૂન્ય cv::ogl::Texture2D::bind() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_->bind();
#endif
}

unsigned પૂર્ણાંક cv::ogl::Texture2D::texId() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

વર્ગ cv::ogl::TextureBuffer::Impl
{
};

#નહીંતર

વર્ગ cv::ogl::TextureBuffer::Impl
{
જાહેર:
    સ્થિર અચલ Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, તાર્કિક autoRelease);
    ~Impl();

    શૂન્ય bind(GLint unit) અચલ;

    શૂન્ય setAutoRelease(તાર્કિક flag) { autoRelease_ = flag; }

    GLuint texId() અચલ { પાછા texId_; }

ખાનગી:
    GLuint texId_;
    તાર્કિક autoRelease_;
};

અચલ Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    સ્થિર Ptr<Impl> p = makePtr<Impl>();
    પાછા p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(ખોટું)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, તાર્કિક autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_BUFFER, texId_);
    CV_CheckGlError();

    gl::TexBuffer(gl::TEXTURE_BUFFER, internalFormat, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_BUFFER, 0);
    CV_CheckGlError();
}

cv::ogl::TextureBuffer::Impl::~Impl()
{
    જો (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

શૂન્ય cv::ogl::TextureBuffer::Impl::bind(GLint unit) અચલ
{
    gl::ActiveTexture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_BUFFER, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::TextureBuffer::TextureBuffer() : format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(અચલ Buffer& buf, Format aformat, તાર્કિક autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

શૂન્ય cv::ogl::TextureBuffer::create(અચલ Buffer& buf, Format aformat, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

શૂન્ય cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    જો (impl_)
        impl_->setAutoRelease(સાચું);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

શૂન્ય cv::ogl::TextureBuffer::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    impl_->setAutoRelease(flag);
#endif
}

શૂન્ય cv::ogl::TextureBuffer::bind(પૂર્ણાંક unit) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#નહીંતર
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() અચલ
{
    પાછા format_;
}

તાર્કિક cv::ogl::TextureBuffer::empty() અચલ
{
    પાછા format_ == NONE;
}

unsigned પૂર્ણાંક cv::ogl::TextureBuffer::texId() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

શૂન્ય cv::ogl::Arrays::setVertexArray(આગત_Array vertex)
{
    અચલ પૂર્ણાંક cn = vertex.channels();
    અચલ પૂર્ણાંક depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    જો (vertex.kind() == આગત_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    નહીંતર
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

શૂન્ય cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

શૂન્ય cv::ogl::Arrays::setColorArray(આગત_Array color)
{
    અચલ પૂર્ણાંક cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    જો (color.kind() == આગત_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    નહીંતર
        color_.copyFrom(color);
}

શૂન્ય cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

શૂન્ય cv::ogl::Arrays::setNormalArray(આગત_Array normal)
{
    અચલ પૂર્ણાંક cn = normal.channels();
    અચલ પૂર્ણાંક depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    જો (normal.kind() == આગત_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    નહીંતર
        normal_.copyFrom(normal);
}

શૂન્ય cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

શૂન્ય cv::ogl::Arrays::setTexCoordArray(આગત_Array texCoord)
{
    અચલ પૂર્ણાંક cn = texCoord.channels();
    અચલ પૂર્ણાંક depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    જો (texCoord.kind() == આગત_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    નહીંતર
        texCoord_.copyFrom(texCoord);
}

શૂન્ય cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

શૂન્ય cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

શૂન્ય cv::ogl::Arrays::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

શૂન્ય cv::ogl::Arrays::bind() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    જો (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    નહીંતર
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    જો (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    નહીંતર
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    જો (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    નહીંતર
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        અચલ પૂર્ણાંક cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    જો (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    નહીંતર
    {
        gl::EnableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();

        vertex_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::VertexPointer(vertex_.channels(), gl_types[vertex_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ogl::Buffer::unbind(ogl::Buffer::ARRAY_BUFFER);
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#ifndef HAVE_OPENGL

વર્ગ cv::ogl::VertexArray::Impl
{
};

#નહીંતર

વર્ગ cv::ogl::VertexArray::Impl
{
જાહેર:
    સ્થિર અચલ Ptr<Impl>& empty();

    Impl(GLuint vaId, તાર્કિક autoRelease);
    Impl(std::initializer_list<Attribute> attributes, તાર્કિક autoRelease);
    ~Impl();

    શૂન્ય bind();

    શૂન્ય setAutoRelease(તાર્કિક flag) { autoRelease_ = flag; }

    GLuint vaId() અચલ { પાછા vaId_; }

ખાનગી:
    Impl();

    GLuint vaId_;
    તાર્કિક autoRelease_;
};

અચલ Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    સ્થિર Ptr<Impl> p(નવું Impl);
    પાછા p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(ખોટું)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, તાર્કિક autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, તાર્કિક autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    માટે (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        જો (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<અચલ શૂન્ય*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        નહીંતર
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<અચલ શૂન્ય*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    જો (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

શૂન્ય cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(attributes, autoRelease));
#endif
}

શૂન્ય cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(attributes, autoRelease));
#endif
}

શૂન્ય cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    જો (impl_)
        impl_->setAutoRelease(સાચું);
    impl_ = Impl::empty();
#endif
}

શૂન્ય cv::ogl::VertexArray::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    impl_->setAutoRelease(flag);
#endif
}

શૂન્ય cv::ogl::VertexArray::bind() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_->bind();
#endif
}

શૂન્ય cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    gl::BindVertexArray(0);
#endif
}

unsigned પૂર્ણાંક cv::ogl::VertexArray::vaId() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

વર્ગ cv::ogl::Shader::Impl
{
};

#નહીંતર

વર્ગ cv::ogl::Shader::Impl
{
જાહેર:
    સ્થિર અચલ Ptr<Impl>& empty();

    Impl(GLuint shaderId, તાર્કિક autoRelease);
    Impl(અચલ અક્ષર* src, GLenum type, તાર્કિક autoRelease);
    ~Impl();

    શૂન્ય setAutoRelease(તાર્કિક flag) { autoRelease_ = flag; }

    GLuint shaderId() અચલ { પાછા shaderId_; }

ખાનગી:
    Impl();

    GLuint shaderId_;
    તાર્કિક autoRelease_;
};

અચલ Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    સ્થિર Ptr<Impl> p(નવું Impl);
    પાછા p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(ખોટું)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, તાર્કિક autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(અચલ અક્ષર* src, GLenum type, તાર્કિક autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    જો (!status) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_LENGTH, &status);
        info_log.resize(status);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, nullptr, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    જો (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(અચલ અક્ષર* src, ShaderType type, તાર્કિક autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(src, type, autoRelease));
#endif
}

શૂન્ય cv::ogl::Shader::create(અચલ અક્ષર* src, ShaderType type, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(src, type, autoRelease));
    type_ = type;
#endif
}

શૂન્ય cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    જો (impl_)
        impl_->setAutoRelease(સાચું);
    impl_ = Impl::empty();
#endif
}

શૂન્ય cv::ogl::Shader::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    impl_->setAutoRelease(flag);
#endif
}

unsigned પૂર્ણાંક cv::ogl::Shader::shaderId() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() અચલ
{
    પાછા type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

વર્ગ cv::ogl::Program::Impl
{
};

#નહીંતર

વર્ગ cv::ogl::Program::Impl
{
જાહેર:
    સ્થિર અચલ Ptr<Impl>& empty();

    Impl(GLuint programId, તાર્કિક autoRelease);
    Impl(અચલ Shader& vert, અચલ Shader& frag, તાર્કિક autoRelease);
    ~Impl();

    શૂન્ય bind();

    પૂર્ણાંક getAttributeLocation(અચલ અક્ષર* name) અચલ;
    પૂર્ણાંક getUniformLocation(અચલ અક્ષર* name) અચલ;

    શૂન્ય setAutoRelease(તાર્કિક flag) { autoRelease_ = flag; }

    GLuint programId() અચલ { પાછા programId_; }

ખાનગી:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    તાર્કિક autoRelease_;
};

અચલ Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    સ્થિર Ptr<Impl> p(નવું Impl);
    પાછા p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(ખોટું)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, તાર્કિક autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(અચલ Shader& vert, અચલ Shader& frag, તાર્કિક autoRelease) : programId_(0), autoRelease_(autoRelease)
{
    programId_ = gl::CreateProgram();
    CV_CheckGlError();

    CV_Assert( programId_ != 0 );

    gl::AttachShader(programId_, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(programId_, frag.shaderId());
    CV_CheckGlError();

    GLint status;
    gl::LinkProgram(programId_);
    gl::GetProgramiv(programId_, gl::LINK_STATUS, &status);
    જો (!status) {
        String info_log;
        gl::GetProgramiv(programId_, gl::INFO_LOG_LENGTH, &status);
        info_log.resize(status);
        gl::GetProgramInfoLog(programId_, GLsizei(info_log.size()) + 1, nullptr, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::Program::Impl::~Impl()
{
    જો (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

શૂન્ય cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

પૂર્ણાંક cv::ogl::Program::Impl::getAttributeLocation(અચલ અક્ષર* name) અચલ
{
    // -1 if absent (e.g. optimized out); caller decides.
    પૂર્ણાંક location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    પાછા location;
}

પૂર્ણાંક cv::ogl::Program::Impl::getUniformLocation(અચલ અક્ષર* name) અચલ
{
    // -1 if absent (e.g. optimized out); caller decides.
    પૂર્ણાંક location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    પાછા location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(અચલ Shader& vert, અચલ Shader& frag, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(vert, frag, autoRelease));
#endif
}

શૂન્ય cv::ogl::Program::create(અચલ Shader& vert, અચલ Shader& frag, તાર્કિક autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#નહીંતર
    impl_.reset(નવું Impl(vert, frag, autoRelease));
#endif
}

શૂન્ય cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    જો (impl_)
        impl_->setAutoRelease(સાચું);
    impl_ = Impl::empty();
#endif
}

શૂન્ય cv::ogl::Program::setAutoRelease(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    impl_->setAutoRelease(flag);
#endif
}

શૂન્ય cv::ogl::Program::bind() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    impl_->bind();
#endif
}

શૂન્ય cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    gl::UseProgram(0);
#endif
}

પૂર્ણાંક cv::ogl::Program::getAttributeLocation(અચલ અક્ષર* name) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#નહીંતર
    પાછા impl_->getAttributeLocation(name);
#endif
}

પૂર્ણાંક cv::ogl::Program::getUniformLocation(અચલ અક્ષર* name) અચલ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#નહીંતર
    પાછા impl_->getUniformLocation(name);
#endif
}

શૂન્ય cv::ogl::Program::setUniformVec3(પૂર્ણાંક loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#નહીંતર
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

શૂન્ય cv::ogl::Program::setUniformVec2(પૂર્ણાંક loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#નહીંતર
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

શૂન્ય cv::ogl::Program::setUniformVec4(પૂર્ણાંક loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#નહીંતર
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

શૂન્ય cv::ogl::Program::setUniformMat4x4(પૂર્ણાંક loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#નહીંતર
    gl::UniformMatrix4fv(loc, 1, સાચું, mat.val);
#endif
}

શૂન્ય cv::ogl::Program::setUniform1f(પૂર્ણાંક loc, દશાંશ value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#નહીંતર
    gl::Uniform1f(loc, value);
#endif
}

શૂન્ય cv::ogl::Program::setUniform1i(પૂર્ણાંક loc, પૂર્ણાંક value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#નહીંતર
    gl::Uniform1i(loc, value);
#endif
}

unsigned પૂર્ણાંક cv::ogl::Program::programId() અચલ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#નહીંતર
    પાછા impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

શૂન્ય cv::ogl::render(અચલ ogl::Texture2D& tex, Rect_<દ્વિગુણ> wndRect, Rect_<દ્વિગુણ> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#નહીંતર
    જો (!tex.empty())
    {
        gl::MatrixMode(gl::PROJECTION);
        gl::LoadIdentity();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::MatrixMode(gl::MODELVIEW);
        gl::LoadIdentity();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_MODE, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
        CV_CheckGlError();

        અચલ દ્વિગુણ vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        અચલ દ્વિગુણ texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.height,
            texRect.x + texRect.width, texRect.y + texRect.height,
            texRect.x + texRect.width, texRect.y
        };

        ogl::Buffer::unbind(ogl::Buffer::ARRAY_BUFFER);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::TexCoordPointer(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::NORMAL_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();

        gl::VertexPointer(3, gl::DOUBLE, 0, vertex);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#endif
}

શૂન્ય cv::ogl::render(અચલ ogl::Arrays& arr, પૂર્ણાંક mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#નહીંતર
    જો (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

શૂન્ય cv::ogl::render(અચલ ogl::Arrays& arr, આગત_Array indices, પૂર્ણાંક mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#નહીંતર
    જો (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        અચલ પૂર્ણાંક kind = indices.kind();

        પસંદગી (kind)
        {
        સ્થિતિ આગત_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                અચલ પૂર્ણાંક depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                જો (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                નહીંતર જો (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                નહીંતર
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                તોડો;
            }

        મૂળભૂત:
            {
                Mat mat = indices.getMat();

                અચલ પૂર્ણાંક depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                જો (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                નહીંતર જો (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                નહીંતર
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

શૂન્ય cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#નહીંતર
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

શૂન્ય cv::ogl::clearDepth(દશાંશ depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#નહીંતર
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

શૂન્ય cv::ogl::drawArrays(પૂર્ણાંક first, પૂર્ણાંક ગણતરી_2, પૂર્ણાંક mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ગણતરી_2); CV_UNUSED(mode);
    throw_no_ogl();
#નહીંતર
    gl::DrawArrays(mode, first, ગણતરી_2);
#endif // HAVE_OPENGL
}

શૂન્ય cv::ogl::drawElements(પૂર્ણાંક first, પૂર્ણાંક ગણતરી_2, પૂર્ણાંક type, પૂર્ણાંક mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ગણતરી_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#નહીંતર
    gl::DrawElements(mode, ગણતરી_2, type, reinterpret_cast<અચલ શૂન્ય*>(static_cast<size_t>(first)));
#endif
}

શૂન્ય cv::ogl::drawElementsInstanced(પૂર્ણાંક first, પૂર્ણાંક ગણતરી_2, પૂર્ણાંક type, પૂર્ણાંક mode, પૂર્ણાંક instance_ગણતરી)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ગણતરી_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_ગણતરી);
    throw_no_ogl();
#નહીંતર
    gl::DrawElementsInstanced(mode, ગણતરી_2, type, reinterpret_cast<અચલ શૂન્ય*>(static_cast<size_t>(first)), instance_ગણતરી);
    CV_CheckGlError();
#endif
}

શૂન્ય cv::ogl::blendFunc(પૂર્ણાંક sfactor, પૂર્ણાંક dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#નહીંતર
    gl::BlendFunc(sfactor, dfactor);
#endif
}

શૂન્ય cv::ogl::depthMask(તાર્કિક flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#નહીંતર
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

શૂન્ય cv::ogl::enable(પૂર્ણાંક cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#નહીંતર
    gl::Enable(cap);
#endif
}

શૂન્ય cv::ogl::disable(પૂર્ણાંક cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#નહીંતર
    gl::Disable(cap);
#endif
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#ifdef HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  નહીંતર
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#નહીંતર // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#જો defined(HAVE_OPENGL)
#  જો defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

નામાકાશ cv { નામાકાશ ogl {

#જો defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
સ્થિર શૂન્ય checkOpenCLVersion()
{
    using નામાકાશ cv::ocl;
    અચલ Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, કદ(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == કદ(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    પૂર્ણાંક versionMajor = pi.versionMajor();
    પૂર્ણાંક versionMinor = pi.versionMinor();
    જો (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

નામાકાશ ocl {

Context& initializeContextFromGL()
{
#જો !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#નહીંતર
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, ખાલી, &platformsCnt);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    જો (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    તાર્કિક sharingSupported = ખોટું;

    માટે (unsigned પૂર્ણાંક i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ખાલી, &devCnt);
        જો (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        પ્રયાસ {
            devices = નવું cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ખાલી);
            જો (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            માટે (unsigned પૂર્ણાંક j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ખાલી, &extensionSize );
                જો (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                જો(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    જો (status != CL_SUCCESS)
                        ચાલુ;
                    જો (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    જ્યારે (spacePos != devString.npos) {
                        જો (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = સાચું;
                            તોડો;
                        }
                        કરો {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } જ્યારે (spacePos == oldPos);
                    }
                }
            }
        } પકડો(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            જો(devices != nullptr) {
                delete[] devices;
            }
            ચાલુ;
        }

        જો (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#જો defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, ખાલી, ખાલી, &ci_Err_અંક);
#અથવાજો defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ખાલી, ખાલી, &status);
#અથવાજો defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ખાલી, ખાલી, &status);
#અથવાજો defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ખાલી, ખાલી, &status);
#endif
        }

        જો (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        નહીંતર
            તોડો;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    પાછા const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

શૂન્ય convertToGLTexture2D(આગત_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#જો !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#નહીંતર
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (પૂર્ણાંક)texture.cols() && srcSize.height == (પૂર્ણાંક)texture.rows());

    using નામાકાશ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

શૂન્ય convertFromGLTexture2D(અચલ Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#જો !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#નહીંતર
    // check texture format
    અચલ પૂર્ણાંક dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    પૂર્ણાંક textureType = dtype;
    CV_Assert(textureType >= 0);

    using નામાકાશ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(અચલ Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#જો !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#નહીંતર
    using નામાકાશ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    પૂર્ણાંક clAccessFlags = 0;
    પસંદગી (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    મૂળભૂત:
    સ્થિતિ ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        તોડો;
    સ્થિતિ ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        તોડો;
    સ્થિતિ ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        તોડો;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    પૂર્ણાંક rows = buffer.rows();
    પૂર્ણાંક cols = buffer.cols();
    પૂર્ણાંક type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    પાછા u;
#endif
}

શૂન્ય unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#જો !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#અથવાજો !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#નહીંતર
    using નામાકાશ cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, ખાલી, ખાલી);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    જો (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
