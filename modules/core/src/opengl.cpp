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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಯದಿ defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#ಅನ್ಯಥಾ
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    ಯದಿ (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#ಅನ್ಯಥಾ // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using ನಾಮಾಕಾಶ cv;
using ನಾಮಾಕಾಶ cv::cuda;

#ಯದಿ defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

ನಾಮಾಕಾಶ
{
#ifndef HAVE_OPENGL
ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ CV_NORETURN ಶೂನ್ಯ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#ನೋಚೇದ್ಯದಿ defined _DEBUG
ಅನ್ತಃಸ್ಥ ಸ್ಥಿರ ತಾರ್ಕಿಕ checkError(ಧ್ರುವ ವರ್ಣ* file, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಸಾಲು, ಧ್ರುವ ವರ್ಣ* func = 0)
{
    GLenum err = gl::GetError();
    ಯದಿ (err != gl::NO_ERROR_)
    {
        ಧ್ರುವ ವರ್ಣ* ಸಂದೇಶ;
        ವಿಕಲ್ಪ (err)
        {
        ಪಕ್ಷ gl::INVALID_ENUM:
            ಸಂದೇಶ = "An unacceptable value is specified for an enumerated argument";
            ಭಙ್ಗ;
        ಪಕ್ಷ gl::INVALID_ಮೌಲ್ಯ:
            ಸಂದೇಶ = "A numeric argument is out of range";
            ಭಙ್ಗ;
        ಪಕ್ಷ gl::INVALID_OPERATION:
            ಸಂದೇಶ = "The specified operation is not allowed in the current state";
            ಭಙ್ಗ;
        ಪಕ್ಷ gl::OUT_OF_MEMORY:
            ಸಂದೇಶ = "There is not enough memory left to execute the command";
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            ಸಂದೇಶ = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, ಸಂದೇಶ, file, ಸಾಲು);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
ನಾಮಾಕಾಶ
{
    ಧ್ರುವ GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

ಶೂನ್ಯ cv::cuda::setGlDevice(ಪೂರ್ಣಾಙ್ಕ device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#ಯದಿ defined(HAVE_OPENGL) && defined(HAVE_CUDA)

ನಾಮಾಕಾಶ
{
    ವರ್ಗ CudaResource
    {
    ಸಾರ್ವಜನಿಕ:
        CudaResource();
        ~CudaResource();

        ಶೂನ್ಯ registerBuffer(GLuint buffer);
        ಶೂನ್ಯ release();

        ಶೂನ್ಯ copyFrom(ಧ್ರುವ ಶೂನ್ಯ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        ಶೂನ್ಯ copyTo(ಶೂನ್ಯ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        ಶೂನ್ಯ* map(cudaStream_t stream = 0);
        ಶೂನ್ಯ unmap(cudaStream_t stream = 0);

    ಗುಪ್ತ:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        ವರ್ಗ GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    ಶೂನ್ಯ CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        ಯದಿ (buffer_ == buffer)
            ಪ್ರತ್ಯಾಗಮ;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    ಶೂನ್ಯ CudaResource::release()
    {
        ಯದಿ (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    ವರ್ಗ CudaResource::GraphicsMapHolder
    {
    ಸಾರ್ವಜನಿಕ:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        ಶೂನ್ಯ reset();

    ಗುಪ್ತ:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        ಯದಿ (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        ಯದಿ (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    ಶೂನ್ಯ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    ಶೂನ್ಯ CudaResource::copyFrom(ಧ್ರುವ ಶೂನ್ಯ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        ಶೂನ್ಯ* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        ಯದಿ (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        ಅನ್ಯಥಾ
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    ಶೂನ್ಯ CudaResource::copyTo(ಶೂನ್ಯ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        ಶೂನ್ಯ* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        ಯದಿ (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        ಅನ್ಯಥಾ
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    ಶೂನ್ಯ* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        ಶೂನ್ಯ* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ಶೂನ್ಯ CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

ವರ್ಗ cv::ogl::Buffer::Impl
{
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::ogl::Buffer::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಧ್ರುವ Ptr<Impl>& empty();

    Impl(GLuint bufId, ತಾರ್ಕಿಕ autoRelease);
    Impl(GLsizeiptr size, ಧ್ರುವ GLvoid* data, GLenum target, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind(GLenum target) ಧ್ರುವ;

    ಶೂನ್ಯ copyFrom(GLuint srcBuf, GLsizeiptr size);

    ಶೂನ್ಯ copyFrom(GLsizeiptr size, ಧ್ರುವ GLvoid* data);
    ಶೂನ್ಯ copyTo(GLsizeiptr size, GLvoid* data) ಧ್ರುವ;

    ಶೂನ್ಯ resize(GLsizeiptr size, GLenum target);

    ಶೂನ್ಯ* mapHost(GLenum access);
    ಶೂನ್ಯ unmapHost();

#ifdef HAVE_CUDA
    ಶೂನ್ಯ copyFrom(ಧ್ರುವ ಶೂನ್ಯ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    ಶೂನ್ಯ copyTo(ಶೂನ್ಯ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) ಧ್ರುವ;

    ಶೂನ್ಯ* mapDevice(cudaStream_t stream = 0);
    ಶೂನ್ಯ unmapDevice(cudaStream_t stream = 0);
#endif

    ಶೂನ್ಯ setAutoRelease(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint bufId() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ bufId_; }

ಗುಪ್ತ:
    Impl();

    GLuint bufId_;
    ತಾರ್ಕಿಕ autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

ಧ್ರುವ Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ನೂತನ Impl);
    ಪ್ರತ್ಯಾಗಮ p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(ಅಸತ್ಯ)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, ತಾರ್ಕಿಕ autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, ಧ್ರುವ GLvoid* data, GLenum target, ತಾರ್ಕಿಕ autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    ಯದಿ (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::bind(GLenum target) ಧ್ರುವ
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, ಧ್ರುವ GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) ಧ್ರುವ
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

ಶೂನ್ಯ* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    ಪ್ರತ್ಯಾಗಮ data;
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

ಶೂನ್ಯ cv::ogl::Buffer::Impl::copyFrom(ಧ್ರುವ ಶೂನ್ಯ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::copyTo(ಶೂನ್ಯ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) ಧ್ರುವ
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

ಶೂನ್ಯ* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    ಪ್ರತ್ಯಾಗಮ cudaResource_.map(stream);
}

ಶೂನ್ಯ cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(ಪೂರ್ಣಾಙ್ಕ arows, ಪೂರ್ಣಾಙ್ಕ acols, ಪೂರ್ಣಾಙ್ಕ atype, unsigned ಪೂರ್ಣಾಙ್ಕ abufId, ತಾರ್ಕಿಕ autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, ಪೂರ್ಣಾಙ್ಕ atype, unsigned ಪೂರ್ಣಾಙ್ಕ abufId, ತಾರ್ಕಿಕ autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(ಒಳಹರಿವು_Array arr, Target target, ತಾರ್ಕಿಕ autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = arr.kind();

    ವಿಕಲ್ಪ (kind)
    {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER:
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ಧ್ರುವ GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(ನೂತನ Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            ಭಙ್ಗ;
        }
    }
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::create(ಪೂರ್ಣಾಙ್ಕ arows, ಪೂರ್ಣಾಙ್ಕ acols, ಪೂರ್ಣಾಙ್ಕ atype, Target target, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಯದಿ (rows_ != arows || cols_ != acols || type_ != atype)
    {
        ಧ್ರುವ GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        ಯದಿ (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        ಅನ್ಯಥಾ
            impl_.reset(ನೂತನ Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    ಯದಿ (impl_)
        impl_->setAutoRelease(ಸತ್ಯ);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->setAutoRelease(flag);
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::copyFrom(ಒಳಹರಿವು_Array arr, Target target, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = arr.kind();

    ಧ್ರುವ Size asize = arr.size();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ atype = arr.type();
    create(asize, atype, target, autoRelease);

    ವಿಕಲ್ಪ (kind)
    {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            ಭಙ್ಗ;
        }

    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ಅನ್ಯಥಾ
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            ಭಙ್ಗ;
        }

    ಸ್ವಭಾವ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::copyFrom(ಒಳಹರಿವು_Array arr, cuda::Stream& stream, Target target, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::copyTo(OutputArray arr) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = arr.kind();

    ವಿಕಲ್ಪ (kind)
    {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*ಏತತ್);
            ಭಙ್ಗ;
        }

    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ಅನ್ಯಥಾ
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            ಭಙ್ಗ;
        }

    ಸ್ವಭಾವ:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, ತಾರ್ಕಿಕ autoRelease) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ogl::Buffer buf;
    buf.copyFrom(*ಏತತ್, target, autoRelease);
    ಪ್ರತ್ಯಾಗಮ buf;
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::bind(Target target) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->bind(target);
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

ಶೂನ್ಯ cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #ಅನ್ಯಥಾ
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned ಪೂರ್ಣಾಙ್ಕ cv::ogl::Buffer::bufId() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

ವರ್ಗ cv::ogl::Texture2D::Impl
{
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::ogl::Texture2D::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಧ್ರುವ Ptr<Impl> empty();

    Impl(GLuint texId, ತಾರ್ಕಿಕ autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ಧ್ರುವ GLvoid* pixels, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ಧ್ರುವ GLvoid *pixels);
    ಶೂನ್ಯ copyTo(GLenum format, GLenum type, GLvoid* pixels) ಧ್ರುವ;

    ಶೂನ್ಯ bind() ಧ್ರುವ;

    ಶೂನ್ಯ setAutoRelease(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint texId() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ texId_; }

ಗುಪ್ತ:
    Impl();

    GLuint texId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಧ್ರುವ Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ನೂತನ Impl);
    ಪ್ರತ್ಯಾಗಮ p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(ಅಸತ್ಯ)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, ತಾರ್ಕಿಕ autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ಧ್ರುವ GLvoid* pixels, ತಾರ್ಕಿಕ autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    ಯದಿ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ಶೂನ್ಯ cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ಧ್ರುವ GLvoid *pixels)
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

ಶೂನ್ಯ cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) ಧ್ರುವ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Texture2D::Impl::bind() ಧ್ರುವ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(ಪೂರ್ಣಾಙ್ಕ arows, ಪೂರ್ಣಾಙ್ಕ acols, Format aformat, unsigned ಪೂರ್ಣಾಙ್ಕ atexId, ತಾರ್ಕಿಕ autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned ಪೂರ್ಣಾಙ್ಕ atexId, ತಾರ್ಕಿಕ autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(ಒಳಹರಿವು_Array arr, ತಾರ್ಕಿಕ autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = arr.kind();

    ಧ್ರುವ Size asize = arr.size();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ atype = arr.type();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(atype);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ಧ್ರುವ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ಧ್ರುವ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    ವಿಕಲ್ಪ (kind)
    {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(ನೂತನ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            ಭಙ್ಗ;
        }

    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ಅನ್ಯಥಾ
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(ಸತ್ಯ);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(ನೂತನ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            ಭಙ್ಗ;
        }

    ಸ್ವಭಾವ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(ನೂತನ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            ಭಙ್ಗ;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

ಶೂನ್ಯ cv::ogl::Texture2D::create(ಪೂರ್ಣಾಙ್ಕ arows, ಪೂರ್ಣಾಙ್ಕ acols, Format aformat, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಯದಿ (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(ನೂತನ Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

ಶೂನ್ಯ cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    ಯದಿ (impl_)
        impl_->setAutoRelease(ಸತ್ಯ);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

ಶೂನ್ಯ cv::ogl::Texture2D::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->setAutoRelease(flag);
#endif
}

ಶೂನ್ಯ cv::ogl::Texture2D::copyFrom(ಒಳಹರಿವು_Array arr, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = arr.kind();

    ಧ್ರುವ Size asize = arr.size();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ atype = arr.type();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = CV_MAT_DEPTH(atype);
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ಧ್ರುವ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ಧ್ರುವ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    ವಿಕಲ್ಪ(kind)
    {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            ಭಙ್ಗ;
        }

    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ಅನ್ಯಥಾ
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(ಸತ್ಯ);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            ಭಙ್ಗ;
        }

    ಸ್ವಭಾವ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

ಶೂನ್ಯ cv::ogl::Texture2D::copyTo(OutputArray arr, ಪೂರ್ಣಾಙ್ಕ ddepth, ತಾರ್ಕಿಕ autoRelease) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = arr.kind();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ಧ್ರುವ GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    ವಿಕಲ್ಪ(kind)
    {
    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            ಭಙ್ಗ;
        }

    ಪಕ್ಷ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ಅನ್ಯಥಾ
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(ಸತ್ಯ);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            ಭಙ್ಗ;
        }

    ಸ್ವಭಾವ:
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

ಶೂನ್ಯ cv::ogl::Texture2D::bind() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->bind();
#endif
}

unsigned ಪೂರ್ಣಾಙ್ಕ cv::ogl::Texture2D::texId() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

ವರ್ಗ cv::ogl::TextureBuffer::Impl
{
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::ogl::TextureBuffer::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಧ್ರುವ Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind(GLint unit) ಧ್ರುವ;

    ಶೂನ್ಯ setAutoRelease(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint texId() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ texId_; }

ಗುಪ್ತ:
    GLuint texId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಧ್ರುವ Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p = makePtr<Impl>();
    ಪ್ರತ್ಯಾಗಮ p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(ಅಸತ್ಯ)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, ತಾರ್ಕಿಕ autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    ಯದಿ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ಶೂನ್ಯ cv::ogl::TextureBuffer::Impl::bind(GLint unit) ಧ್ರುವ
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
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(ಧ್ರುವ Buffer& buf, Format aformat, ತಾರ್ಕಿಕ autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

ಶೂನ್ಯ cv::ogl::TextureBuffer::create(ಧ್ರುವ Buffer& buf, Format aformat, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

ಶೂನ್ಯ cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    ಯದಿ (impl_)
        impl_->setAutoRelease(ಸತ್ಯ);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

ಶೂನ್ಯ cv::ogl::TextureBuffer::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->setAutoRelease(flag);
#endif
}

ಶೂನ್ಯ cv::ogl::TextureBuffer::bind(ಪೂರ್ಣಾಙ್ಕ unit) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ format_;
}

ತಾರ್ಕಿಕ cv::ogl::TextureBuffer::empty() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ format_ == NONE;
}

unsigned ಪೂರ್ಣಾಙ್ಕ cv::ogl::TextureBuffer::texId() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

ಶೂನ್ಯ cv::ogl::Arrays::setVertexArray(ಒಳಹರಿವು_Array vertex)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = vertex.channels();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    ಯದಿ (vertex.kind() == ಒಳಹರಿವು_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    ಅನ್ಯಥಾ
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

ಶೂನ್ಯ cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

ಶೂನ್ಯ cv::ogl::Arrays::setColorArray(ಒಳಹರಿವು_Array color)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    ಯದಿ (color.kind() == ಒಳಹರಿವು_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    ಅನ್ಯಥಾ
        color_.copyFrom(color);
}

ಶೂನ್ಯ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

ಶೂನ್ಯ cv::ogl::Arrays::setNormalArray(ಒಳಹರಿವು_Array normal)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = normal.channels();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    ಯದಿ (normal.kind() == ಒಳಹರಿವು_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    ಅನ್ಯಥಾ
        normal_.copyFrom(normal);
}

ಶೂನ್ಯ cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

ಶೂನ್ಯ cv::ogl::Arrays::setTexCoordArray(ಒಳಹರಿವು_Array texCoord)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = texCoord.channels();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    ಯದಿ (texCoord.kind() == ಒಳಹರಿವು_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    ಅನ್ಯಥಾ
        texCoord_.copyFrom(texCoord);
}

ಶೂನ್ಯ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

ಶೂನ್ಯ cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

ಶೂನ್ಯ cv::ogl::Arrays::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

ಶೂನ್ಯ cv::ogl::Arrays::bind() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    ಯದಿ (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    ಅನ್ಯಥಾ
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ಯದಿ (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    ಅನ್ಯಥಾ
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ಯದಿ (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    ಅನ್ಯಥಾ
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ಯದಿ (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    ಅನ್ಯಥಾ
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

ವರ್ಗ cv::ogl::VertexArray::Impl
{
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::ogl::VertexArray::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಧ್ರುವ Ptr<Impl>& empty();

    Impl(GLuint vaId, ತಾರ್ಕಿಕ autoRelease);
    Impl(std::initializer_list<Attribute> attributes, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind();

    ಶೂನ್ಯ setAutoRelease(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint vaId() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ vaId_; }

ಗುಪ್ತ:
    Impl();

    GLuint vaId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಧ್ರುವ Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ನೂತನ Impl);
    ಪ್ರತ್ಯಾಗಮ p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(ಅಸತ್ಯ)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, ತಾರ್ಕಿಕ autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, ತಾರ್ಕಿಕ autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    ಪ್ರತಿ (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        ಯದಿ (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ಧ್ರುವ ಶೂನ್ಯ*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        ಅನ್ಯಥಾ
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ಧ್ರುವ ಶೂನ್ಯ*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    ಯದಿ (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

ಶೂನ್ಯ cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(attributes, autoRelease));
#endif
}

ಶೂನ್ಯ cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(attributes, autoRelease));
#endif
}

ಶೂನ್ಯ cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    ಯದಿ (impl_)
        impl_->setAutoRelease(ಸತ್ಯ);
    impl_ = Impl::empty();
#endif
}

ಶೂನ್ಯ cv::ogl::VertexArray::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->setAutoRelease(flag);
#endif
}

ಶೂನ್ಯ cv::ogl::VertexArray::bind() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->bind();
#endif
}

ಶೂನ್ಯ cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::BindVertexArray(0);
#endif
}

unsigned ಪೂರ್ಣಾಙ್ಕ cv::ogl::VertexArray::vaId() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

ವರ್ಗ cv::ogl::Shader::Impl
{
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::ogl::Shader::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಧ್ರುವ Ptr<Impl>& empty();

    Impl(GLuint shaderId, ತಾರ್ಕಿಕ autoRelease);
    Impl(ಧ್ರುವ ವರ್ಣ* src, GLenum type, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ setAutoRelease(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint shaderId() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ shaderId_; }

ಗುಪ್ತ:
    Impl();

    GLuint shaderId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಧ್ರುವ Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ನೂತನ Impl);
    ಪ್ರತ್ಯಾಗಮ p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(ಅಸತ್ಯ)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, ತಾರ್ಕಿಕ autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ಧ್ರುವ ವರ್ಣ* src, GLenum type, ತಾರ್ಕಿಕ autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    ಯದಿ (!status) {
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
    ಯದಿ (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(ಧ್ರುವ ವರ್ಣ* src, ShaderType type, ತಾರ್ಕಿಕ autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(src, type, autoRelease));
#endif
}

ಶೂನ್ಯ cv::ogl::Shader::create(ಧ್ರುವ ವರ್ಣ* src, ShaderType type, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(src, type, autoRelease));
    type_ = type;
#endif
}

ಶೂನ್ಯ cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    ಯದಿ (impl_)
        impl_->setAutoRelease(ಸತ್ಯ);
    impl_ = Impl::empty();
#endif
}

ಶೂನ್ಯ cv::ogl::Shader::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->setAutoRelease(flag);
#endif
}

unsigned ಪೂರ್ಣಾಙ್ಕ cv::ogl::Shader::shaderId() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

ವರ್ಗ cv::ogl::Program::Impl
{
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::ogl::Program::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಧ್ರುವ Ptr<Impl>& empty();

    Impl(GLuint programId, ತಾರ್ಕಿಕ autoRelease);
    Impl(ಧ್ರುವ Shader& vert, ಧ್ರುವ Shader& frag, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind();

    ಪೂರ್ಣಾಙ್ಕ getAttributeLocation(ಧ್ರುವ ವರ್ಣ* name) ಧ್ರುವ;
    ಪೂರ್ಣಾಙ್ಕ getUniformLocation(ಧ್ರುವ ವರ್ಣ* name) ಧ್ರುವ;

    ಶೂನ್ಯ setAutoRelease(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint programId() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ programId_; }

ಗುಪ್ತ:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಧ್ರುವ Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ನೂತನ Impl);
    ಪ್ರತ್ಯಾಗಮ p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(ಅಸತ್ಯ)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, ತಾರ್ಕಿಕ autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(ಧ್ರುವ Shader& vert, ಧ್ರುವ Shader& frag, ತಾರ್ಕಿಕ autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    ಯದಿ (!status) {
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
    ಯದಿ (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

ಶೂನ್ಯ cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

ಪೂರ್ಣಾಙ್ಕ cv::ogl::Program::Impl::getAttributeLocation(ಧ್ರುವ ವರ್ಣ* name) ಧ್ರುವ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ಪೂರ್ಣಾಙ್ಕ location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    ಪ್ರತ್ಯಾಗಮ location;
}

ಪೂರ್ಣಾಙ್ಕ cv::ogl::Program::Impl::getUniformLocation(ಧ್ರುವ ವರ್ಣ* name) ಧ್ರುವ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ಪೂರ್ಣಾಙ್ಕ location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    ಪ್ರತ್ಯಾಗಮ location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(ಧ್ರುವ Shader& vert, ಧ್ರುವ Shader& frag, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(vert, frag, autoRelease));
#endif
}

ಶೂನ್ಯ cv::ogl::Program::create(ಧ್ರುವ Shader& vert, ಧ್ರುವ Shader& frag, ತಾರ್ಕಿಕ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_.reset(ನೂತನ Impl(vert, frag, autoRelease));
#endif
}

ಶೂನ್ಯ cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    ಯದಿ (impl_)
        impl_->setAutoRelease(ಸತ್ಯ);
    impl_ = Impl::empty();
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setAutoRelease(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->setAutoRelease(flag);
#endif
}

ಶೂನ್ಯ cv::ogl::Program::bind() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    impl_->bind();
#endif
}

ಶೂನ್ಯ cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::UseProgram(0);
#endif
}

ಪೂರ್ಣಾಙ್ಕ cv::ogl::Program::getAttributeLocation(ಧ್ರುವ ವರ್ಣ* name) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->getAttributeLocation(name);
#endif
}

ಪೂರ್ಣಾಙ್ಕ cv::ogl::Program::getUniformLocation(ಧ್ರುವ ವರ್ಣ* name) ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->getUniformLocation(name);
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setUniformVec3(ಪೂರ್ಣಾಙ್ಕ loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setUniformVec2(ಪೂರ್ಣಾಙ್ಕ loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setUniformVec4(ಪೂರ್ಣಾಙ್ಕ loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setUniformMat4x4(ಪೂರ್ಣಾಙ್ಕ loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::UniformMatrix4fv(loc, 1, ಸತ್ಯ, mat.val);
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setUniform1f(ಪೂರ್ಣಾಙ್ಕ loc, ಭಿನ್ನ value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::Uniform1f(loc, value);
#endif
}

ಶೂನ್ಯ cv::ogl::Program::setUniform1i(ಪೂರ್ಣಾಙ್ಕ loc, ಪೂರ್ಣಾಙ್ಕ value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::Uniform1i(loc, value);
#endif
}

unsigned ಪೂರ್ಣಾಙ್ಕ cv::ogl::Program::programId() ಧ್ರುವ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

ಶೂನ್ಯ cv::ogl::render(ಧ್ರುವ ogl::Texture2D& tex, Rect_<ದ್ವಿಗುಣ> wndRect, Rect_<ದ್ವಿಗುಣ> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಯದಿ (!tex.empty())
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

        ಧ್ರುವ ದ್ವಿಗುಣ vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        ಧ್ರುವ ದ್ವಿಗುಣ texCoords[] =
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

ಶೂನ್ಯ cv::ogl::render(ಧ್ರುವ ogl::Arrays& arr, ಪೂರ್ಣಾಙ್ಕ mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಯದಿ (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

ಶೂನ್ಯ cv::ogl::render(ಧ್ರುವ ogl::Arrays& arr, ಒಳಹರಿವು_Array indices, ಪೂರ್ಣಾಙ್ಕ mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    ಯದಿ (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ kind = indices.kind();

        ವಿಕಲ್ಪ (kind)
        {
        ಪಕ್ಷ ಒಳಹರಿವು_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                ಯದಿ (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ಅನ್ಯಥಾ ಯದಿ (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ಅನ್ಯಥಾ
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                ಭಙ್ಗ;
            }

        ಸ್ವಭಾವ:
            {
                Mat mat = indices.getMat();

                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                ಯದಿ (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ಅನ್ಯಥಾ ಯದಿ (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ಅನ್ಯಥಾ
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

ಶೂನ್ಯ cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

ಶೂನ್ಯ cv::ogl::clearDepth(ಭಿನ್ನ depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

ಶೂನ್ಯ cv::ogl::drawArrays(ಪೂರ್ಣಾಙ್ಕ first, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಙ್ಕ mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ಎಣಿಕೆ_2); CV_UNUSED(mode);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::DrawArrays(mode, first, ಎಣಿಕೆ_2);
#endif // HAVE_OPENGL
}

ಶೂನ್ಯ cv::ogl::drawElements(ಪೂರ್ಣಾಙ್ಕ first, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ಎಣಿಕೆ_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::DrawElements(mode, ಎಣಿಕೆ_2, type, reinterpret_cast<ಧ್ರುವ ಶೂನ್ಯ*>(static_cast<size_t>(first)));
#endif
}

ಶೂನ್ಯ cv::ogl::drawElementsInstanced(ಪೂರ್ಣಾಙ್ಕ first, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2, ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ mode, ಪೂರ್ಣಾಙ್ಕ instance_ಎಣಿಕೆ)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ಎಣಿಕೆ_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_ಎಣಿಕೆ);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::DrawElementsInstanced(mode, ಎಣಿಕೆ_2, type, reinterpret_cast<ಧ್ರುವ ಶೂನ್ಯ*>(static_cast<size_t>(first)), instance_ಎಣಿಕೆ);
    CV_CheckGlError();
#endif
}

ಶೂನ್ಯ cv::ogl::blendFunc(ಪೂರ್ಣಾಙ್ಕ sfactor, ಪೂರ್ಣಾಙ್ಕ dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::BlendFunc(sfactor, dfactor);
#endif
}

ಶೂನ್ಯ cv::ogl::depthMask(ತಾರ್ಕಿಕ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

ಶೂನ್ಯ cv::ogl::enable(ಪೂರ್ಣಾಙ್ಕ cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#ಅನ್ಯಥಾ
    gl::Enable(cap);
#endif
}

ಶೂನ್ಯ cv::ogl::disable(ಪೂರ್ಣಾಙ್ಕ cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#ಅನ್ಯಥಾ
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
#  ಅನ್ಯಥಾ
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#ಅನ್ಯಥಾ // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#ಯದಿ defined(HAVE_OPENGL)
#  ಯದಿ defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ogl {

#ಯದಿ defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ಸ್ಥಿರ ಶೂನ್ಯ checkOpenCLVersion()
{
    using ನಾಮಾಕಾಶ cv::ocl;
    ಧ್ರುವ Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, ಪರಿಮಾಣ(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == ಪರಿಮಾಣ(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    ಪೂರ್ಣಾಙ್ಕ versionMajor = pi.versionMajor();
    ಪೂರ್ಣಾಙ್ಕ versionMinor = pi.versionMinor();
    ಯದಿ (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

ನಾಮಾಕಾಶ ocl {

Context& initializeContextFromGL()
{
#ಯದಿ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಅನ್ಯಥಾ
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, ರಿಕ್ತ, &platformsCnt);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    ಯದಿ (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ತಾರ್ಕಿಕ sharingSupported = ಅಸತ್ಯ;

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ರಿಕ್ತ, &devCnt);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        ಯತ್ನ {
            devices = ನೂತನ cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ರಿಕ್ತ);
            ಯದಿ (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ರಿಕ್ತ, &extensionSize );
                ಯದಿ (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                ಯದಿ(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    ಯದಿ (status != CL_SUCCESS)
                        ಅನುವರ್ತ;
                    ಯದಿ (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    ಯಾವತ್ (spacePos != devString.npos) {
                        ಯದಿ (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = ಸತ್ಯ;
                            ಭಙ್ಗ;
                        }
                        ಕುರು {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } ಯಾವತ್ (spacePos == oldPos);
                    }
                }
            }
        } ಗೃಹಾಣ(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            ಯದಿ(devices != nullptr) {
                delete[] devices;
            }
            ಅನುವರ್ತ;
        }

        ಯದಿ (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#ಯದಿ defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, ರಿಕ್ತ, ರಿಕ್ತ, &ci_Err_ಅಂಕೆ);
#ನೋಚೇದ್ಯದಿ defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ರಿಕ್ತ, ರಿಕ್ತ, &status);
#ನೋಚೇದ್ಯದಿ defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ರಿಕ್ತ, ರಿಕ್ತ, &status);
#ನೋಚೇದ್ಯದಿ defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ರಿಕ್ತ, ರಿಕ್ತ, &status);
#endif
        }

        ಯದಿ (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        ಅನ್ಯಥಾ
            ಭಙ್ಗ;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    ಪ್ರತ್ಯಾಗಮ const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

ಶೂನ್ಯ convertToGLTexture2D(ಒಳಹರಿವು_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#ಯದಿ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಅನ್ಯಥಾ
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (ಪೂರ್ಣಾಙ್ಕ)texture.cols() && srcSize.height == (ಪೂರ್ಣಾಙ್ಕ)texture.rows());

    using ನಾಮಾಕಾಶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

ಶೂನ್ಯ convertFromGLTexture2D(ಧ್ರುವ Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#ಯದಿ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಅನ್ಯಥಾ
    // check texture format
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    ಪೂರ್ಣಾಙ್ಕ textureType = dtype;
    CV_Assert(textureType >= 0);

    using ನಾಮಾಕಾಶ cv::ocl;
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
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ಧ್ರುವ Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#ಯದಿ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಅನ್ಯಥಾ
    using ನಾಮಾಕಾಶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    ಪೂರ್ಣಾಙ್ಕ clAccessFlags = 0;
    ವಿಕಲ್ಪ (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    ಸ್ವಭಾವ:
    ಪಕ್ಷ ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        ಭಙ್ಗ;
    ಪಕ್ಷ ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        ಭಙ್ಗ;
    ಪಕ್ಷ ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        ಭಙ್ಗ;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    ಪೂರ್ಣಾಙ್ಕ rows = buffer.rows();
    ಪೂರ್ಣಾಙ್ಕ cols = buffer.cols();
    ಪೂರ್ಣಾಙ್ಕ type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    ಪ್ರತ್ಯಾಗಮ u;
#endif
}

ಶೂನ್ಯ unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#ಯದಿ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ನೋಚೇದ್ಯದಿ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಅನ್ಯಥಾ
    using ನಾಮಾಕಾಶ cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, ರಿಕ್ತ, ರಿಕ್ತ);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    ಯದಿ (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
