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

#അന്തര്ഭാവ "precomp.hpp"

#എങ്കിൽ defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#അല്ലെങ്കിൽ
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    എങ്കിൽ (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#അല്ലെങ്കിൽ // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using നാമാകാശ cv;
using നാമാകാശ cv::cuda;

#എങ്കിൽ defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

നാമാകാശ
{
#ifndef HAVE_OPENGL
വരിയിൽ സ്ഥിതം CV_NORETURN ശൂന്യം throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#അല്ലെങ്കിലെങ്കിൽ defined _DEBUG
വരിയിൽ സ്ഥിതം തർക്കം checkError(സ്ഥിരം അക്ഷരം* file, സ്ഥിരം പൂർണ്ണസംഖ്യ വരി, സ്ഥിരം അക്ഷരം* func = 0)
{
    GLenum err = gl::GetError();
    എങ്കിൽ (err != gl::NO_ERROR_)
    {
        സ്ഥിരം അക്ഷരം* സന്ദേശം;
        തിരഞ്ഞെടുപ്പ് (err)
        {
        അവസ്ഥ gl::INVALID_ENUM:
            സന്ദേശം = "An unacceptable value is specified for an enumerated argument";
            നിർത്തുക;
        അവസ്ഥ gl::INVALID_മൂല്യം:
            സന്ദേശം = "A numeric argument is out of range";
            നിർത്തുക;
        അവസ്ഥ gl::INVALID_OPERATION:
            സന്ദേശം = "The specified operation is not allowed in the current state";
            നിർത്തുക;
        അവസ്ഥ gl::OUT_OF_MEMORY:
            സന്ദേശം = "There is not enough memory left to execute the command";
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            സന്ദേശം = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, സന്ദേശം, file, വരി);
    }
    തിരികെ സത്യം;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
നാമാകാശ
{
    സ്ഥിരം GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

ശൂന്യം cv::cuda::setGlDevice(പൂർണ്ണസംഖ്യ device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#എങ്കിൽ defined(HAVE_OPENGL) && defined(HAVE_CUDA)

നാമാകാശ
{
    വർഗ്ഗം CudaResource
    {
    പൊതു:
        CudaResource();
        ~CudaResource();

        ശൂന്യം registerBuffer(GLuint buffer);
        ശൂന്യം release();

        ശൂന്യം copyFrom(സ്ഥിരം ശൂന്യം* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        ശൂന്യം copyTo(ശൂന്യം* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        ശൂന്യം* map(cudaStream_t stream = 0);
        ശൂന്യം unmap(cudaStream_t stream = 0);

    സ്വകാര്യ:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        വർഗ്ഗം GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    ശൂന്യം CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        എങ്കിൽ (buffer_ == buffer)
            തിരികെ;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    ശൂന്യം CudaResource::release()
    {
        എങ്കിൽ (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    വർഗ്ഗം CudaResource::GraphicsMapHolder
    {
    പൊതു:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        ശൂന്യം reset();

    സ്വകാര്യ:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        എങ്കിൽ (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        എങ്കിൽ (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    ശൂന്യം CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    ശൂന്യം CudaResource::copyFrom(സ്ഥിരം ശൂന്യം* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        ശൂന്യം* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        എങ്കിൽ (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        അല്ലെങ്കിൽ
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    ശൂന്യം CudaResource::copyTo(ശൂന്യം* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        ശൂന്യം* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        എങ്കിൽ (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        അല്ലെങ്കിൽ
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    ശൂന്യം* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        ശൂന്യം* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        തിരികെ ptr;
    }

    ശൂന്യം CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

വർഗ്ഗം cv::ogl::Buffer::Impl
{
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::ogl::Buffer::Impl
{
പൊതു:
    സ്ഥിതം സ്ഥിരം Ptr<Impl>& empty();

    Impl(GLuint bufId, തർക്കം autoRelease);
    Impl(GLsizeiptr size, സ്ഥിരം GLvoid* data, GLenum target, തർക്കം autoRelease);
    ~Impl();

    ശൂന്യം bind(GLenum target) സ്ഥിരം;

    ശൂന്യം copyFrom(GLuint srcBuf, GLsizeiptr size);

    ശൂന്യം copyFrom(GLsizeiptr size, സ്ഥിരം GLvoid* data);
    ശൂന്യം copyTo(GLsizeiptr size, GLvoid* data) സ്ഥിരം;

    ശൂന്യം resize(GLsizeiptr size, GLenum target);

    ശൂന്യം* mapHost(GLenum access);
    ശൂന്യം unmapHost();

#ifdef HAVE_CUDA
    ശൂന്യം copyFrom(സ്ഥിരം ശൂന്യം* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    ശൂന്യം copyTo(ശൂന്യം* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) സ്ഥിരം;

    ശൂന്യം* mapDevice(cudaStream_t stream = 0);
    ശൂന്യം unmapDevice(cudaStream_t stream = 0);
#endif

    ശൂന്യം setAutoRelease(തർക്കം flag) { autoRelease_ = flag; }

    GLuint bufId() സ്ഥിരം { തിരികെ bufId_; }

സ്വകാര്യ:
    Impl();

    GLuint bufId_;
    തർക്കം autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

സ്ഥിരം Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    സ്ഥിതം Ptr<Impl> p(പുതിയ Impl);
    തിരികെ p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(അസത്യം)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, തർക്കം autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, സ്ഥിരം GLvoid* data, GLenum target, തർക്കം autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    എങ്കിൽ (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

ശൂന്യം cv::ogl::Buffer::Impl::bind(GLenum target) സ്ഥിരം
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

ശൂന്യം cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

ശൂന്യം cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, സ്ഥിരം GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

ശൂന്യം cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

ശൂന്യം cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) സ്ഥിരം
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

ശൂന്യം* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    തിരികെ data;
}

ശൂന്യം cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

ശൂന്യം cv::ogl::Buffer::Impl::copyFrom(സ്ഥിരം ശൂന്യം* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

ശൂന്യം cv::ogl::Buffer::Impl::copyTo(ശൂന്യം* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) സ്ഥിരം
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

ശൂന്യം* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    തിരികെ cudaResource_.map(stream);
}

ശൂന്യം cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(പൂർണ്ണസംഖ്യ arows, പൂർണ്ണസംഖ്യ acols, പൂർണ്ണസംഖ്യ atype, unsigned പൂർണ്ണസംഖ്യ abufId, തർക്കം autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, പൂർണ്ണസംഖ്യ atype, unsigned പൂർണ്ണസംഖ്യ abufId, തർക്കം autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(നിവേശം_Array arr, Target target, തർക്കം autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ kind = arr.kind();

    തിരഞ്ഞെടുപ്പ് (kind)
    {
    അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER:
    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            സ്ഥിരം GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(പുതിയ Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            നിർത്തുക;
        }
    }
#endif
}

ശൂന്യം cv::ogl::Buffer::create(പൂർണ്ണസംഖ്യ arows, പൂർണ്ണസംഖ്യ acols, പൂർണ്ണസംഖ്യ atype, Target target, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    എങ്കിൽ (rows_ != arows || cols_ != acols || type_ != atype)
    {
        സ്ഥിരം GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        എങ്കിൽ (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        അല്ലെങ്കിൽ
            impl_.reset(പുതിയ Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

ശൂന്യം cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    എങ്കിൽ (impl_)
        impl_->setAutoRelease(സത്യം);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

ശൂന്യം cv::ogl::Buffer::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->setAutoRelease(flag);
#endif
}

ശൂന്യം cv::ogl::Buffer::copyFrom(നിവേശം_Array arr, Target target, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ kind = arr.kind();

    സ്ഥിരം Size asize = arr.size();
    സ്ഥിരം പൂർണ്ണസംഖ്യ atype = arr.type();
    create(asize, atype, target, autoRelease);

    തിരഞ്ഞെടുപ്പ് (kind)
    {
    അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            നിർത്തുക;
        }

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #അല്ലെങ്കിൽ
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            നിർത്തുക;
        }

    സ്ഥിരസ്ഥിതി:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

ശൂന്യം cv::ogl::Buffer::copyFrom(നിവേശം_Array arr, cuda::Stream& stream, Target target, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

ശൂന്യം cv::ogl::Buffer::copyTo(OutputArray arr) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ kind = arr.kind();

    തിരഞ്ഞെടുപ്പ് (kind)
    {
    അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*ഇത്);
            നിർത്തുക;
        }

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #അല്ലെങ്കിൽ
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            നിർത്തുക;
        }

    സ്ഥിരസ്ഥിതി:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

ശൂന്യം cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, തർക്കം autoRelease) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    ogl::Buffer buf;
    buf.copyFrom(*ഇത്, target, autoRelease);
    തിരികെ buf;
#endif
}

ശൂന്യം cv::ogl::Buffer::bind(Target target) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->bind(target);
#endif
}

ശൂന്യം cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

ശൂന്യം cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        തിരികെ GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

ശൂന്യം cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        തിരികെ GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

ശൂന്യം cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #അല്ലെങ്കിൽ
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned പൂർണ്ണസംഖ്യ cv::ogl::Buffer::bufId() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

വർഗ്ഗം cv::ogl::Texture2D::Impl
{
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::ogl::Texture2D::Impl
{
പൊതു:
    സ്ഥിതം സ്ഥിരം Ptr<Impl> empty();

    Impl(GLuint texId, തർക്കം autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, സ്ഥിരം GLvoid* pixels, തർക്കം autoRelease);
    ~Impl();

    ശൂന്യം copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, സ്ഥിരം GLvoid *pixels);
    ശൂന്യം copyTo(GLenum format, GLenum type, GLvoid* pixels) സ്ഥിരം;

    ശൂന്യം bind() സ്ഥിരം;

    ശൂന്യം setAutoRelease(തർക്കം flag) { autoRelease_ = flag; }

    GLuint texId() സ്ഥിരം { തിരികെ texId_; }

സ്വകാര്യ:
    Impl();

    GLuint texId_;
    തർക്കം autoRelease_;
};

സ്ഥിരം Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    സ്ഥിതം Ptr<Impl> p(പുതിയ Impl);
    തിരികെ p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(അസത്യം)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, തർക്കം autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, സ്ഥിരം GLvoid* pixels, തർക്കം autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    എങ്കിൽ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ശൂന്യം cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, സ്ഥിരം GLvoid *pixels)
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

ശൂന്യം cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) സ്ഥിരം
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

ശൂന്യം cv::ogl::Texture2D::Impl::bind() സ്ഥിരം
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(പൂർണ്ണസംഖ്യ arows, പൂർണ്ണസംഖ്യ acols, Format aformat, unsigned പൂർണ്ണസംഖ്യ atexId, തർക്കം autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned പൂർണ്ണസംഖ്യ atexId, തർക്കം autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(നിവേശം_Array arr, തർക്കം autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ kind = arr.kind();

    സ്ഥിരം Size asize = arr.size();
    സ്ഥിരം പൂർണ്ണസംഖ്യ atype = arr.type();

    സ്ഥിരം പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(atype);
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    സ്ഥിരം Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    സ്ഥിരം GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    തിരഞ്ഞെടുപ്പ് (kind)
    {
    അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(പുതിയ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            നിർത്തുക;
        }

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #അല്ലെങ്കിൽ
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(സത്യം);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(പുതിയ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            നിർത്തുക;
        }

    സ്ഥിരസ്ഥിതി:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(പുതിയ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            നിർത്തുക;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

ശൂന്യം cv::ogl::Texture2D::create(പൂർണ്ണസംഖ്യ arows, പൂർണ്ണസംഖ്യ acols, Format aformat, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    എങ്കിൽ (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(പുതിയ Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

ശൂന്യം cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    എങ്കിൽ (impl_)
        impl_->setAutoRelease(സത്യം);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

ശൂന്യം cv::ogl::Texture2D::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->setAutoRelease(flag);
#endif
}

ശൂന്യം cv::ogl::Texture2D::copyFrom(നിവേശം_Array arr, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ kind = arr.kind();

    സ്ഥിരം Size asize = arr.size();
    സ്ഥിരം പൂർണ്ണസംഖ്യ atype = arr.type();

    സ്ഥിരം പൂർണ്ണസംഖ്യ depth = CV_MAT_DEPTH(atype);
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    സ്ഥിരം Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    സ്ഥിരം GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    തിരഞ്ഞെടുപ്പ്(kind)
    {
    അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            നിർത്തുക;
        }

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #അല്ലെങ്കിൽ
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(സത്യം);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            നിർത്തുക;
        }

    സ്ഥിരസ്ഥിതി:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

ശൂന്യം cv::ogl::Texture2D::copyTo(OutputArray arr, പൂർണ്ണസംഖ്യ ddepth, തർക്കം autoRelease) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ kind = arr.kind();

    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    സ്ഥിരം GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    തിരഞ്ഞെടുപ്പ്(kind)
    {
    അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            നിർത്തുക;
        }

    അവസ്ഥ നിവേശം_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #അല്ലെങ്കിൽ
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(സത്യം);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            നിർത്തുക;
        }

    സ്ഥിരസ്ഥിതി:
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

ശൂന്യം cv::ogl::Texture2D::bind() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->bind();
#endif
}

unsigned പൂർണ്ണസംഖ്യ cv::ogl::Texture2D::texId() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

വർഗ്ഗം cv::ogl::TextureBuffer::Impl
{
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::ogl::TextureBuffer::Impl
{
പൊതു:
    സ്ഥിതം സ്ഥിരം Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, തർക്കം autoRelease);
    ~Impl();

    ശൂന്യം bind(GLint unit) സ്ഥിരം;

    ശൂന്യം setAutoRelease(തർക്കം flag) { autoRelease_ = flag; }

    GLuint texId() സ്ഥിരം { തിരികെ texId_; }

സ്വകാര്യ:
    GLuint texId_;
    തർക്കം autoRelease_;
};

സ്ഥിരം Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    സ്ഥിതം Ptr<Impl> p = makePtr<Impl>();
    തിരികെ p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(അസത്യം)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, തർക്കം autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    എങ്കിൽ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ശൂന്യം cv::ogl::TextureBuffer::Impl::bind(GLint unit) സ്ഥിരം
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
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(സ്ഥിരം Buffer& buf, Format aformat, തർക്കം autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

ശൂന്യം cv::ogl::TextureBuffer::create(സ്ഥിരം Buffer& buf, Format aformat, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

ശൂന്യം cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    എങ്കിൽ (impl_)
        impl_->setAutoRelease(സത്യം);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

ശൂന്യം cv::ogl::TextureBuffer::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->setAutoRelease(flag);
#endif
}

ശൂന്യം cv::ogl::TextureBuffer::bind(പൂർണ്ണസംഖ്യ unit) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() സ്ഥിരം
{
    തിരികെ format_;
}

തർക്കം cv::ogl::TextureBuffer::empty() സ്ഥിരം
{
    തിരികെ format_ == NONE;
}

unsigned പൂർണ്ണസംഖ്യ cv::ogl::TextureBuffer::texId() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

ശൂന്യം cv::ogl::Arrays::setVertexArray(നിവേശം_Array vertex)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = vertex.channels();
    സ്ഥിരം പൂർണ്ണസംഖ്യ depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    എങ്കിൽ (vertex.kind() == നിവേശം_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    അല്ലെങ്കിൽ
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

ശൂന്യം cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

ശൂന്യം cv::ogl::Arrays::setColorArray(നിവേശം_Array color)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    എങ്കിൽ (color.kind() == നിവേശം_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    അല്ലെങ്കിൽ
        color_.copyFrom(color);
}

ശൂന്യം cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

ശൂന്യം cv::ogl::Arrays::setNormalArray(നിവേശം_Array normal)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = normal.channels();
    സ്ഥിരം പൂർണ്ണസംഖ്യ depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    എങ്കിൽ (normal.kind() == നിവേശം_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    അല്ലെങ്കിൽ
        normal_.copyFrom(normal);
}

ശൂന്യം cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

ശൂന്യം cv::ogl::Arrays::setTexCoordArray(നിവേശം_Array texCoord)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = texCoord.channels();
    സ്ഥിരം പൂർണ്ണസംഖ്യ depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    എങ്കിൽ (texCoord.kind() == നിവേശം_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    അല്ലെങ്കിൽ
        texCoord_.copyFrom(texCoord);
}

ശൂന്യം cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

ശൂന്യം cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

ശൂന്യം cv::ogl::Arrays::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

ശൂന്യം cv::ogl::Arrays::bind() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    എങ്കിൽ (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    അല്ലെങ്കിൽ
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    എങ്കിൽ (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    അല്ലെങ്കിൽ
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    എങ്കിൽ (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    അല്ലെങ്കിൽ
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        സ്ഥിരം പൂർണ്ണസംഖ്യ cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    എങ്കിൽ (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    അല്ലെങ്കിൽ
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

വർഗ്ഗം cv::ogl::VertexArray::Impl
{
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::ogl::VertexArray::Impl
{
പൊതു:
    സ്ഥിതം സ്ഥിരം Ptr<Impl>& empty();

    Impl(GLuint vaId, തർക്കം autoRelease);
    Impl(std::initializer_list<Attribute> attributes, തർക്കം autoRelease);
    ~Impl();

    ശൂന്യം bind();

    ശൂന്യം setAutoRelease(തർക്കം flag) { autoRelease_ = flag; }

    GLuint vaId() സ്ഥിരം { തിരികെ vaId_; }

സ്വകാര്യ:
    Impl();

    GLuint vaId_;
    തർക്കം autoRelease_;
};

സ്ഥിരം Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    സ്ഥിതം Ptr<Impl> p(പുതിയ Impl);
    തിരികെ p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(അസത്യം)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, തർക്കം autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, തർക്കം autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    വേണ്ടി (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        എങ്കിൽ (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<സ്ഥിരം ശൂന്യം*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        അല്ലെങ്കിൽ
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<സ്ഥിരം ശൂന്യം*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    എങ്കിൽ (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

ശൂന്യം cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(attributes, autoRelease));
#endif
}

ശൂന്യം cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(attributes, autoRelease));
#endif
}

ശൂന്യം cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    എങ്കിൽ (impl_)
        impl_->setAutoRelease(സത്യം);
    impl_ = Impl::empty();
#endif
}

ശൂന്യം cv::ogl::VertexArray::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->setAutoRelease(flag);
#endif
}

ശൂന്യം cv::ogl::VertexArray::bind() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->bind();
#endif
}

ശൂന്യം cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::BindVertexArray(0);
#endif
}

unsigned പൂർണ്ണസംഖ്യ cv::ogl::VertexArray::vaId() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

വർഗ്ഗം cv::ogl::Shader::Impl
{
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::ogl::Shader::Impl
{
പൊതു:
    സ്ഥിതം സ്ഥിരം Ptr<Impl>& empty();

    Impl(GLuint shaderId, തർക്കം autoRelease);
    Impl(സ്ഥിരം അക്ഷരം* src, GLenum type, തർക്കം autoRelease);
    ~Impl();

    ശൂന്യം setAutoRelease(തർക്കം flag) { autoRelease_ = flag; }

    GLuint shaderId() സ്ഥിരം { തിരികെ shaderId_; }

സ്വകാര്യ:
    Impl();

    GLuint shaderId_;
    തർക്കം autoRelease_;
};

സ്ഥിരം Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    സ്ഥിതം Ptr<Impl> p(പുതിയ Impl);
    തിരികെ p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(അസത്യം)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, തർക്കം autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(സ്ഥിരം അക്ഷരം* src, GLenum type, തർക്കം autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    എങ്കിൽ (!status) {
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
    എങ്കിൽ (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(സ്ഥിരം അക്ഷരം* src, ShaderType type, തർക്കം autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(src, type, autoRelease));
#endif
}

ശൂന്യം cv::ogl::Shader::create(സ്ഥിരം അക്ഷരം* src, ShaderType type, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(src, type, autoRelease));
    type_ = type;
#endif
}

ശൂന്യം cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    എങ്കിൽ (impl_)
        impl_->setAutoRelease(സത്യം);
    impl_ = Impl::empty();
#endif
}

ശൂന്യം cv::ogl::Shader::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->setAutoRelease(flag);
#endif
}

unsigned പൂർണ്ണസംഖ്യ cv::ogl::Shader::shaderId() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() സ്ഥിരം
{
    തിരികെ type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

വർഗ്ഗം cv::ogl::Program::Impl
{
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::ogl::Program::Impl
{
പൊതു:
    സ്ഥിതം സ്ഥിരം Ptr<Impl>& empty();

    Impl(GLuint programId, തർക്കം autoRelease);
    Impl(സ്ഥിരം Shader& vert, സ്ഥിരം Shader& frag, തർക്കം autoRelease);
    ~Impl();

    ശൂന്യം bind();

    പൂർണ്ണസംഖ്യ getAttributeLocation(സ്ഥിരം അക്ഷരം* name) സ്ഥിരം;
    പൂർണ്ണസംഖ്യ getUniformLocation(സ്ഥിരം അക്ഷരം* name) സ്ഥിരം;

    ശൂന്യം setAutoRelease(തർക്കം flag) { autoRelease_ = flag; }

    GLuint programId() സ്ഥിരം { തിരികെ programId_; }

സ്വകാര്യ:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    തർക്കം autoRelease_;
};

സ്ഥിരം Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    സ്ഥിതം Ptr<Impl> p(പുതിയ Impl);
    തിരികെ p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(അസത്യം)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, തർക്കം autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(സ്ഥിരം Shader& vert, സ്ഥിരം Shader& frag, തർക്കം autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    എങ്കിൽ (!status) {
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
    എങ്കിൽ (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

ശൂന്യം cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

പൂർണ്ണസംഖ്യ cv::ogl::Program::Impl::getAttributeLocation(സ്ഥിരം അക്ഷരം* name) സ്ഥിരം
{
    // -1 if absent (e.g. optimized out); caller decides.
    പൂർണ്ണസംഖ്യ location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    തിരികെ location;
}

പൂർണ്ണസംഖ്യ cv::ogl::Program::Impl::getUniformLocation(സ്ഥിരം അക്ഷരം* name) സ്ഥിരം
{
    // -1 if absent (e.g. optimized out); caller decides.
    പൂർണ്ണസംഖ്യ location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    തിരികെ location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(സ്ഥിരം Shader& vert, സ്ഥിരം Shader& frag, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(vert, frag, autoRelease));
#endif
}

ശൂന്യം cv::ogl::Program::create(സ്ഥിരം Shader& vert, സ്ഥിരം Shader& frag, തർക്കം autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_.reset(പുതിയ Impl(vert, frag, autoRelease));
#endif
}

ശൂന്യം cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    എങ്കിൽ (impl_)
        impl_->setAutoRelease(സത്യം);
    impl_ = Impl::empty();
#endif
}

ശൂന്യം cv::ogl::Program::setAutoRelease(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->setAutoRelease(flag);
#endif
}

ശൂന്യം cv::ogl::Program::bind() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    impl_->bind();
#endif
}

ശൂന്യം cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::UseProgram(0);
#endif
}

പൂർണ്ണസംഖ്യ cv::ogl::Program::getAttributeLocation(സ്ഥിരം അക്ഷരം* name) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->getAttributeLocation(name);
#endif
}

പൂർണ്ണസംഖ്യ cv::ogl::Program::getUniformLocation(സ്ഥിരം അക്ഷരം* name) സ്ഥിരം
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->getUniformLocation(name);
#endif
}

ശൂന്യം cv::ogl::Program::setUniformVec3(പൂർണ്ണസംഖ്യ loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

ശൂന്യം cv::ogl::Program::setUniformVec2(പൂർണ്ണസംഖ്യ loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

ശൂന്യം cv::ogl::Program::setUniformVec4(പൂർണ്ണസംഖ്യ loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

ശൂന്യം cv::ogl::Program::setUniformMat4x4(പൂർണ്ണസംഖ്യ loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::UniformMatrix4fv(loc, 1, സത്യം, mat.val);
#endif
}

ശൂന്യം cv::ogl::Program::setUniform1f(പൂർണ്ണസംഖ്യ loc, ദശാംശം value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::Uniform1f(loc, value);
#endif
}

ശൂന്യം cv::ogl::Program::setUniform1i(പൂർണ്ണസംഖ്യ loc, പൂർണ്ണസംഖ്യ value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::Uniform1i(loc, value);
#endif
}

unsigned പൂർണ്ണസംഖ്യ cv::ogl::Program::programId() സ്ഥിരം
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#അല്ലെങ്കിൽ
    തിരികെ impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

ശൂന്യം cv::ogl::render(സ്ഥിരം ogl::Texture2D& tex, Rect_<ഇരട്ട> wndRect, Rect_<ഇരട്ട> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    എങ്കിൽ (!tex.empty())
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

        സ്ഥിരം ഇരട്ട vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        സ്ഥിരം ഇരട്ട texCoords[] =
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

ശൂന്യം cv::ogl::render(സ്ഥിരം ogl::Arrays& arr, പൂർണ്ണസംഖ്യ mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    എങ്കിൽ (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

ശൂന്യം cv::ogl::render(സ്ഥിരം ogl::Arrays& arr, നിവേശം_Array indices, പൂർണ്ണസംഖ്യ mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    എങ്കിൽ (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        സ്ഥിരം പൂർണ്ണസംഖ്യ kind = indices.kind();

        തിരഞ്ഞെടുപ്പ് (kind)
        {
        അവസ്ഥ നിവേശം_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                സ്ഥിരം പൂർണ്ണസംഖ്യ depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                എങ്കിൽ (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                അല്ലെങ്കിൽ എങ്കിൽ (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                അല്ലെങ്കിൽ
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                നിർത്തുക;
            }

        സ്ഥിരസ്ഥിതി:
            {
                Mat mat = indices.getMat();

                സ്ഥിരം പൂർണ്ണസംഖ്യ depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                എങ്കിൽ (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                അല്ലെങ്കിൽ എങ്കിൽ (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                അല്ലെങ്കിൽ
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

ശൂന്യം cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

ശൂന്യം cv::ogl::clearDepth(ദശാംശം depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

ശൂന്യം cv::ogl::drawArrays(പൂർണ്ണസംഖ്യ first, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(എണ്ണം_2); CV_UNUSED(mode);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::DrawArrays(mode, first, എണ്ണം_2);
#endif // HAVE_OPENGL
}

ശൂന്യം cv::ogl::drawElements(പൂർണ്ണസംഖ്യ first, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(എണ്ണം_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::DrawElements(mode, എണ്ണം_2, type, reinterpret_cast<സ്ഥിരം ശൂന്യം*>(static_cast<size_t>(first)));
#endif
}

ശൂന്യം cv::ogl::drawElementsInstanced(പൂർണ്ണസംഖ്യ first, പൂർണ്ണസംഖ്യ എണ്ണം_2, പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ mode, പൂർണ്ണസംഖ്യ instance_എണ്ണം)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(എണ്ണം_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_എണ്ണം);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::DrawElementsInstanced(mode, എണ്ണം_2, type, reinterpret_cast<സ്ഥിരം ശൂന്യം*>(static_cast<size_t>(first)), instance_എണ്ണം);
    CV_CheckGlError();
#endif
}

ശൂന്യം cv::ogl::blendFunc(പൂർണ്ണസംഖ്യ sfactor, പൂർണ്ണസംഖ്യ dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::BlendFunc(sfactor, dfactor);
#endif
}

ശൂന്യം cv::ogl::depthMask(തർക്കം flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

ശൂന്യം cv::ogl::enable(പൂർണ്ണസംഖ്യ cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#അല്ലെങ്കിൽ
    gl::Enable(cap);
#endif
}

ശൂന്യം cv::ogl::disable(പൂർണ്ണസംഖ്യ cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#അല്ലെങ്കിൽ
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
#  അല്ലെങ്കിൽ
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#അല്ലെങ്കിൽ // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#എങ്കിൽ defined(HAVE_OPENGL)
#  എങ്കിൽ defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

നാമാകാശ cv { നാമാകാശ ogl {

#എങ്കിൽ defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
സ്ഥിതം ശൂന്യം checkOpenCLVersion()
{
    using നാമാകാശ cv::ocl;
    സ്ഥിരം Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, വലുപ്പം(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == വലുപ്പം(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    പൂർണ്ണസംഖ്യ versionMajor = pi.versionMajor();
    പൂർണ്ണസംഖ്യ versionMinor = pi.versionMinor();
    എങ്കിൽ (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

നാമാകാശ ocl {

Context& initializeContextFromGL()
{
#എങ്കിൽ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#അല്ലെങ്കിൽ
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, ശൂന്യ, &platformsCnt);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    എങ്കിൽ (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    തർക്കം sharingSupported = അസത്യം;

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ശൂന്യ, &devCnt);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        ശ്രമിക്കുക {
            devices = പുതിയ cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ശൂന്യ);
            എങ്കിൽ (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ശൂന്യ, &extensionSize );
                എങ്കിൽ (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                എങ്കിൽ(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    എങ്കിൽ (status != CL_SUCCESS)
                        തുടരുക;
                    എങ്കിൽ (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    ആകുമ്പോൾ (spacePos != devString.npos) {
                        എങ്കിൽ (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = സത്യം;
                            നിർത്തുക;
                        }
                        ചെയ്യുക {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } ആകുമ്പോൾ (spacePos == oldPos);
                    }
                }
            }
        } പിടിക്കുക(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            എങ്കിൽ(devices != nullptr) {
                delete[] devices;
            }
            തുടരുക;
        }

        എങ്കിൽ (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#എങ്കിൽ defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, ശൂന്യ, ശൂന്യ, &ci_Err_അക്കം);
#അല്ലെങ്കിലെങ്കിൽ defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ശൂന്യ, ശൂന്യ, &status);
#അല്ലെങ്കിലെങ്കിൽ defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ശൂന്യ, ശൂന്യ, &status);
#അല്ലെങ്കിലെങ്കിൽ defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ശൂന്യ, ശൂന്യ, &status);
#endif
        }

        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        അല്ലെങ്കിൽ
            നിർത്തുക;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    തിരികെ const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

ശൂന്യം convertToGLTexture2D(നിവേശം_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#എങ്കിൽ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#അല്ലെങ്കിൽ
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (പൂർണ്ണസംഖ്യ)texture.cols() && srcSize.height == (പൂർണ്ണസംഖ്യ)texture.rows());

    using നാമാകാശ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

ശൂന്യം convertFromGLTexture2D(സ്ഥിരം Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#എങ്കിൽ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#അല്ലെങ്കിൽ
    // check texture format
    സ്ഥിരം പൂർണ്ണസംഖ്യ dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    പൂർണ്ണസംഖ്യ textureType = dtype;
    CV_Assert(textureType >= 0);

    using നാമാകാശ cv::ocl;
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
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(സ്ഥിരം Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#എങ്കിൽ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#അല്ലെങ്കിൽ
    using നാമാകാശ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    പൂർണ്ണസംഖ്യ clAccessFlags = 0;
    തിരഞ്ഞെടുപ്പ് (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    സ്ഥിരസ്ഥിതി:
    അവസ്ഥ ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        നിർത്തുക;
    അവസ്ഥ ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        നിർത്തുക;
    അവസ്ഥ ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        നിർത്തുക;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    പൂർണ്ണസംഖ്യ rows = buffer.rows();
    പൂർണ്ണസംഖ്യ cols = buffer.cols();
    പൂർണ്ണസംഖ്യ type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    തിരികെ u;
#endif
}

ശൂന്യം unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#എങ്കിൽ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#അല്ലെങ്കിലെങ്കിൽ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#അല്ലെങ്കിൽ
    using നാമാകാശ cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, ശൂന്യ, ശൂന്യ);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    എങ്കിൽ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
