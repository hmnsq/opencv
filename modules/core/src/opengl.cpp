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

#অন্তর্ভাৱ "precomp.hpp"

#যদি defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#অন্যথা
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    যদি (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#অন্যথা // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using নামাকাশ cv;
using নামাকাশ cv::cuda;

#যদি defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

নামাকাশ
{
#ifndef HAVE_OPENGL
অন্তঃস্থ স্থির CV_NORETURN শূন্য throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#নোচেদ্যদি defined _DEBUG
অন্তঃস্থ স্থির তার্কিক checkError(ধ্রুৱ ৱর্ণ* file, ধ্রুৱ পূর্ণাঙ্ক সাৰি, ধ্রুৱ ৱর্ণ* func = 0)
{
    GLenum err = gl::GetError();
    যদি (err != gl::NO_ERROR_)
    {
        ধ্রুৱ ৱর্ণ* বাৰ্তা;
        ৱিকল্প (err)
        {
        পক্ষ gl::INVALID_ENUM:
            বাৰ্তা = "An unacceptable value is specified for an enumerated argument";
            ভঙ্গ;
        পক্ষ gl::INVALID_মান:
            বাৰ্তা = "A numeric argument is out of range";
            ভঙ্গ;
        পক্ষ gl::INVALID_OPERATION:
            বাৰ্তা = "The specified operation is not allowed in the current state";
            ভঙ্গ;
        পক্ষ gl::OUT_OF_MEMORY:
            বাৰ্তা = "There is not enough memory left to execute the command";
            ভঙ্গ;
        স্ৱভাৱ:
            বাৰ্তা = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, বাৰ্তা, file, সাৰি);
    }
    ঘূৰাই সঁচা;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
নামাকাশ
{
    ধ্রুৱ GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

শূন্য cv::cuda::setGlDevice(পূর্ণাঙ্ক device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #অন্যথা
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#যদি defined(HAVE_OPENGL) && defined(HAVE_CUDA)

নামাকাশ
{
    শ্ৰেণী CudaResource
    {
    সার্ৱজনিক:
        CudaResource();
        ~CudaResource();

        শূন্য registerBuffer(GLuint buffer);
        শূন্য release();

        শূন্য copyFrom(ধ্রুৱ শূন্য* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        শূন্য copyTo(শূন্য* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        শূন্য* map(cudaStream_t stream = 0);
        শূন্য unmap(cudaStream_t stream = 0);

    গুপ্ত:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        শ্ৰেণী GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    শূন্য CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        যদি (buffer_ == buffer)
            ঘূৰাই;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    শূন্য CudaResource::release()
    {
        যদি (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    শ্ৰেণী CudaResource::GraphicsMapHolder
    {
    সার্ৱজনিক:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        শূন্য reset();

    গুপ্ত:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        যদি (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        যদি (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    শূন্য CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    শূন্য CudaResource::copyFrom(ধ্রুৱ শূন্য* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        শূন্য* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        যদি (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        অন্যথা
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    শূন্য CudaResource::copyTo(শূন্য* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        শূন্য* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        যদি (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        অন্যথা
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    শূন্য* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        শূন্য* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        ঘূৰাই ptr;
    }

    শূন্য CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

শ্ৰেণী cv::ogl::Buffer::Impl
{
};

#অন্যথা

শ্ৰেণী cv::ogl::Buffer::Impl
{
সার্ৱজনিক:
    স্থির ধ্রুৱ Ptr<Impl>& empty();

    Impl(GLuint bufId, তার্কিক autoRelease);
    Impl(GLsizeiptr size, ধ্রুৱ GLvoid* data, GLenum target, তার্কিক autoRelease);
    ~Impl();

    শূন্য bind(GLenum target) ধ্রুৱ;

    শূন্য copyFrom(GLuint srcBuf, GLsizeiptr size);

    শূন্য copyFrom(GLsizeiptr size, ধ্রুৱ GLvoid* data);
    শূন্য copyTo(GLsizeiptr size, GLvoid* data) ধ্রুৱ;

    শূন্য resize(GLsizeiptr size, GLenum target);

    শূন্য* mapHost(GLenum access);
    শূন্য unmapHost();

#ifdef HAVE_CUDA
    শূন্য copyFrom(ধ্রুৱ শূন্য* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    শূন্য copyTo(শূন্য* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) ধ্রুৱ;

    শূন্য* mapDevice(cudaStream_t stream = 0);
    শূন্য unmapDevice(cudaStream_t stream = 0);
#endif

    শূন্য setAutoRelease(তার্কিক flag) { autoRelease_ = flag; }

    GLuint bufId() ধ্রুৱ { ঘূৰাই bufId_; }

গুপ্ত:
    Impl();

    GLuint bufId_;
    তার্কিক autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

ধ্রুৱ Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    স্থির Ptr<Impl> p(নূতন Impl);
    ঘূৰাই p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(মিছা)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, তার্কিক autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, ধ্রুৱ GLvoid* data, GLenum target, তার্কিক autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    যদি (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

শূন্য cv::ogl::Buffer::Impl::bind(GLenum target) ধ্রুৱ
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

শূন্য cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

শূন্য cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, ধ্রুৱ GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

শূন্য cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

শূন্য cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) ধ্রুৱ
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

শূন্য* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    ঘূৰাই data;
}

শূন্য cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

শূন্য cv::ogl::Buffer::Impl::copyFrom(ধ্রুৱ শূন্য* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

শূন্য cv::ogl::Buffer::Impl::copyTo(শূন্য* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) ধ্রুৱ
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

শূন্য* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    ঘূৰাই cudaResource_.map(stream);
}

শূন্য cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(পূর্ণাঙ্ক arows, পূর্ণাঙ্ক acols, পূর্ণাঙ্ক atype, unsigned পূর্ণাঙ্ক abufId, তার্কিক autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, পূর্ণাঙ্ক atype, unsigned পূর্ণাঙ্ক abufId, তার্কিক autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(প্ৰবেশ_Array arr, Target target, তার্কিক autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    ধ্রুৱ পূর্ণাঙ্ক kind = arr.kind();

    ৱিকল্প (kind)
    {
    পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER:
    পক্ষ প্ৰবেশ_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        ভঙ্গ;

    স্ৱভাৱ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ধ্রুৱ GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(নূতন Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            ভঙ্গ;
        }
    }
#endif
}

শূন্য cv::ogl::Buffer::create(পূর্ণাঙ্ক arows, পূর্ণাঙ্ক acols, পূর্ণাঙ্ক atype, Target target, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    যদি (rows_ != arows || cols_ != acols || type_ != atype)
    {
        ধ্রুৱ GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        যদি (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        অন্যথা
            impl_.reset(নূতন Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

শূন্য cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    যদি (impl_)
        impl_->setAutoRelease(সঁচা);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

শূন্য cv::ogl::Buffer::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    impl_->setAutoRelease(flag);
#endif
}

শূন্য cv::ogl::Buffer::copyFrom(প্ৰবেশ_Array arr, Target target, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    ধ্রুৱ পূর্ণাঙ্ক kind = arr.kind();

    ধ্রুৱ Size asize = arr.size();
    ধ্রুৱ পূর্ণাঙ্ক atype = arr.type();
    create(asize, atype, target, autoRelease);

    ৱিকল্প (kind)
    {
    পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            ভঙ্গ;
        }

    পক্ষ প্ৰবেশ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #অন্যথা
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            ভঙ্গ;
        }

    স্ৱভাৱ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

শূন্য cv::ogl::Buffer::copyFrom(প্ৰবেশ_Array arr, cuda::Stream& stream, Target target, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #অন্যথা
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

শূন্য cv::ogl::Buffer::copyTo(OutputArray arr) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#অন্যথা
    ধ্রুৱ পূর্ণাঙ্ক kind = arr.kind();

    ৱিকল্প (kind)
    {
    পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*এতত্);
            ভঙ্গ;
        }

    পক্ষ প্ৰবেশ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #অন্যথা
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            ভঙ্গ;
        }

    স্ৱভাৱ:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

শূন্য cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #অন্যথা
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, তার্কিক autoRelease) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    ogl::Buffer buf;
    buf.copyFrom(*এতত্, target, autoRelease);
    ঘূৰাই buf;
#endif
}

শূন্য cv::ogl::Buffer::bind(Target target) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#অন্যথা
    impl_->bind(target);
#endif
}

শূন্য cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#অন্যথা
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#অন্যথা
    ঘূৰাই Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

শূন্য cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #অন্যথা
        ঘূৰাই GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

শূন্য cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #অন্যথা
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #অন্যথা
        ঘূৰাই GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

শূন্য cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#অন্যথা
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #অন্যথা
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned পূর্ণাঙ্ক cv::ogl::Buffer::bufId() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

শ্ৰেণী cv::ogl::Texture2D::Impl
{
};

#অন্যথা

শ্ৰেণী cv::ogl::Texture2D::Impl
{
সার্ৱজনিক:
    স্থির ধ্রুৱ Ptr<Impl> empty();

    Impl(GLuint texId, তার্কিক autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ধ্রুৱ GLvoid* pixels, তার্কিক autoRelease);
    ~Impl();

    শূন্য copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ধ্রুৱ GLvoid *pixels);
    শূন্য copyTo(GLenum format, GLenum type, GLvoid* pixels) ধ্রুৱ;

    শূন্য bind() ধ্রুৱ;

    শূন্য setAutoRelease(তার্কিক flag) { autoRelease_ = flag; }

    GLuint texId() ধ্রুৱ { ঘূৰাই texId_; }

গুপ্ত:
    Impl();

    GLuint texId_;
    তার্কিক autoRelease_;
};

ধ্রুৱ Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    স্থির Ptr<Impl> p(নূতন Impl);
    ঘূৰাই p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(মিছা)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, তার্কিক autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ধ্রুৱ GLvoid* pixels, তার্কিক autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    যদি (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

শূন্য cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ধ্রুৱ GLvoid *pixels)
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

শূন্য cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) ধ্রুৱ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

শূন্য cv::ogl::Texture2D::Impl::bind() ধ্রুৱ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(পূর্ণাঙ্ক arows, পূর্ণাঙ্ক acols, Format aformat, unsigned পূর্ণাঙ্ক atexId, তার্কিক autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned পূর্ণাঙ্ক atexId, তার্কিক autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(প্ৰবেশ_Array arr, তার্কিক autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    ধ্রুৱ পূর্ণাঙ্ক kind = arr.kind();

    ধ্রুৱ Size asize = arr.size();
    ধ্রুৱ পূর্ণাঙ্ক atype = arr.type();

    ধ্রুৱ পূর্ণাঙ্ক depth = CV_MAT_DEPTH(atype);
    ধ্রুৱ পূর্ণাঙ্ক cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ধ্রুৱ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ধ্রুৱ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    ৱিকল্প (kind)
    {
    পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(নূতন Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            ভঙ্গ;
        }

    পক্ষ প্ৰবেশ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #অন্যথা
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(সঁচা);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(নূতন Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            ভঙ্গ;
        }

    স্ৱভাৱ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(নূতন Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            ভঙ্গ;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

শূন্য cv::ogl::Texture2D::create(পূর্ণাঙ্ক arows, পূর্ণাঙ্ক acols, Format aformat, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    যদি (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(নূতন Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

শূন্য cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    যদি (impl_)
        impl_->setAutoRelease(সঁচা);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

শূন্য cv::ogl::Texture2D::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    impl_->setAutoRelease(flag);
#endif
}

শূন্য cv::ogl::Texture2D::copyFrom(প্ৰবেশ_Array arr, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    ধ্রুৱ পূর্ণাঙ্ক kind = arr.kind();

    ধ্রুৱ Size asize = arr.size();
    ধ্রুৱ পূর্ণাঙ্ক atype = arr.type();

    ধ্রুৱ পূর্ণাঙ্ক depth = CV_MAT_DEPTH(atype);
    ধ্রুৱ পূর্ণাঙ্ক cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ধ্রুৱ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ধ্রুৱ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    ৱিকল্প(kind)
    {
    পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            ভঙ্গ;
        }

    পক্ষ প্ৰবেশ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #অন্যথা
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(সঁচা);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            ভঙ্গ;
        }

    স্ৱভাৱ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

শূন্য cv::ogl::Texture2D::copyTo(OutputArray arr, পূর্ণাঙ্ক ddepth, তার্কিক autoRelease) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    ধ্রুৱ পূর্ণাঙ্ক kind = arr.kind();

    ধ্রুৱ পূর্ণাঙ্ক cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ধ্রুৱ GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    ৱিকল্প(kind)
    {
    পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            ভঙ্গ;
        }

    পক্ষ প্ৰবেশ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #অন্যথা
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(সঁচা);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            ভঙ্গ;
        }

    স্ৱভাৱ:
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

শূন্য cv::ogl::Texture2D::bind() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_->bind();
#endif
}

unsigned পূর্ণাঙ্ক cv::ogl::Texture2D::texId() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

শ্ৰেণী cv::ogl::TextureBuffer::Impl
{
};

#অন্যথা

শ্ৰেণী cv::ogl::TextureBuffer::Impl
{
সার্ৱজনিক:
    স্থির ধ্রুৱ Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, তার্কিক autoRelease);
    ~Impl();

    শূন্য bind(GLint unit) ধ্রুৱ;

    শূন্য setAutoRelease(তার্কিক flag) { autoRelease_ = flag; }

    GLuint texId() ধ্রুৱ { ঘূৰাই texId_; }

গুপ্ত:
    GLuint texId_;
    তার্কিক autoRelease_;
};

ধ্রুৱ Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    স্থির Ptr<Impl> p = makePtr<Impl>();
    ঘূৰাই p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(মিছা)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, তার্কিক autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    যদি (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

শূন্য cv::ogl::TextureBuffer::Impl::bind(GLint unit) ধ্রুৱ
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
#অন্যথা
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(ধ্রুৱ Buffer& buf, Format aformat, তার্কিক autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

শূন্য cv::ogl::TextureBuffer::create(ধ্রুৱ Buffer& buf, Format aformat, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

শূন্য cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    যদি (impl_)
        impl_->setAutoRelease(সঁচা);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

শূন্য cv::ogl::TextureBuffer::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    impl_->setAutoRelease(flag);
#endif
}

শূন্য cv::ogl::TextureBuffer::bind(পূর্ণাঙ্ক unit) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#অন্যথা
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() ধ্রুৱ
{
    ঘূৰাই format_;
}

তার্কিক cv::ogl::TextureBuffer::empty() ধ্রুৱ
{
    ঘূৰাই format_ == NONE;
}

unsigned পূর্ণাঙ্ক cv::ogl::TextureBuffer::texId() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

শূন্য cv::ogl::Arrays::setVertexArray(প্ৰবেশ_Array vertex)
{
    ধ্রুৱ পূর্ণাঙ্ক cn = vertex.channels();
    ধ্রুৱ পূর্ণাঙ্ক depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    যদি (vertex.kind() == প্ৰবেশ_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    অন্যথা
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

শূন্য cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

শূন্য cv::ogl::Arrays::setColorArray(প্ৰবেশ_Array color)
{
    ধ্রুৱ পূর্ণাঙ্ক cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    যদি (color.kind() == প্ৰবেশ_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    অন্যথা
        color_.copyFrom(color);
}

শূন্য cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

শূন্য cv::ogl::Arrays::setNormalArray(প্ৰবেশ_Array normal)
{
    ধ্রুৱ পূর্ণাঙ্ক cn = normal.channels();
    ধ্রুৱ পূর্ণাঙ্ক depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    যদি (normal.kind() == প্ৰবেশ_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    অন্যথা
        normal_.copyFrom(normal);
}

শূন্য cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

শূন্য cv::ogl::Arrays::setTexCoordArray(প্ৰবেশ_Array texCoord)
{
    ধ্রুৱ পূর্ণাঙ্ক cn = texCoord.channels();
    ধ্রুৱ পূর্ণাঙ্ক depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    যদি (texCoord.kind() == প্ৰবেশ_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    অন্যথা
        texCoord_.copyFrom(texCoord);
}

শূন্য cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

শূন্য cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

শূন্য cv::ogl::Arrays::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

শূন্য cv::ogl::Arrays::bind() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    যদি (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    অন্যথা
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    যদি (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    অন্যথা
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    যদি (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    অন্যথা
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        ধ্রুৱ পূর্ণাঙ্ক cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    যদি (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    অন্যথা
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

শ্ৰেণী cv::ogl::VertexArray::Impl
{
};

#অন্যথা

শ্ৰেণী cv::ogl::VertexArray::Impl
{
সার্ৱজনিক:
    স্থির ধ্রুৱ Ptr<Impl>& empty();

    Impl(GLuint vaId, তার্কিক autoRelease);
    Impl(std::initializer_list<Attribute> attributes, তার্কিক autoRelease);
    ~Impl();

    শূন্য bind();

    শূন্য setAutoRelease(তার্কিক flag) { autoRelease_ = flag; }

    GLuint vaId() ধ্রুৱ { ঘূৰাই vaId_; }

গুপ্ত:
    Impl();

    GLuint vaId_;
    তার্কিক autoRelease_;
};

ধ্রুৱ Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    স্থির Ptr<Impl> p(নূতন Impl);
    ঘূৰাই p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(মিছা)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, তার্কিক autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, তার্কিক autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    বাবে (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        যদি (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ধ্রুৱ শূন্য*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        অন্যথা
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ধ্রুৱ শূন্য*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    যদি (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

শূন্য cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(attributes, autoRelease));
#endif
}

শূন্য cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(attributes, autoRelease));
#endif
}

শূন্য cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    যদি (impl_)
        impl_->setAutoRelease(সঁচা);
    impl_ = Impl::empty();
#endif
}

শূন্য cv::ogl::VertexArray::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    impl_->setAutoRelease(flag);
#endif
}

শূন্য cv::ogl::VertexArray::bind() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_->bind();
#endif
}

শূন্য cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    gl::BindVertexArray(0);
#endif
}

unsigned পূর্ণাঙ্ক cv::ogl::VertexArray::vaId() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

শ্ৰেণী cv::ogl::Shader::Impl
{
};

#অন্যথা

শ্ৰেণী cv::ogl::Shader::Impl
{
সার্ৱজনিক:
    স্থির ধ্রুৱ Ptr<Impl>& empty();

    Impl(GLuint shaderId, তার্কিক autoRelease);
    Impl(ধ্রুৱ ৱর্ণ* src, GLenum type, তার্কিক autoRelease);
    ~Impl();

    শূন্য setAutoRelease(তার্কিক flag) { autoRelease_ = flag; }

    GLuint shaderId() ধ্রুৱ { ঘূৰাই shaderId_; }

গুপ্ত:
    Impl();

    GLuint shaderId_;
    তার্কিক autoRelease_;
};

ধ্রুৱ Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    স্থির Ptr<Impl> p(নূতন Impl);
    ঘূৰাই p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(মিছা)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, তার্কিক autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ধ্রুৱ ৱর্ণ* src, GLenum type, তার্কিক autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    যদি (!status) {
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
    যদি (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(ধ্রুৱ ৱর্ণ* src, ShaderType type, তার্কিক autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(src, type, autoRelease));
#endif
}

শূন্য cv::ogl::Shader::create(ধ্রুৱ ৱর্ণ* src, ShaderType type, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(src, type, autoRelease));
    type_ = type;
#endif
}

শূন্য cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    যদি (impl_)
        impl_->setAutoRelease(সঁচা);
    impl_ = Impl::empty();
#endif
}

শূন্য cv::ogl::Shader::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    impl_->setAutoRelease(flag);
#endif
}

unsigned পূর্ণাঙ্ক cv::ogl::Shader::shaderId() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ধ্রুৱ
{
    ঘূৰাই type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

শ্ৰেণী cv::ogl::Program::Impl
{
};

#অন্যথা

শ্ৰেণী cv::ogl::Program::Impl
{
সার্ৱজনিক:
    স্থির ধ্রুৱ Ptr<Impl>& empty();

    Impl(GLuint programId, তার্কিক autoRelease);
    Impl(ধ্রুৱ Shader& vert, ধ্রুৱ Shader& frag, তার্কিক autoRelease);
    ~Impl();

    শূন্য bind();

    পূর্ণাঙ্ক getAttributeLocation(ধ্রুৱ ৱর্ণ* name) ধ্রুৱ;
    পূর্ণাঙ্ক getUniformLocation(ধ্রুৱ ৱর্ণ* name) ধ্রুৱ;

    শূন্য setAutoRelease(তার্কিক flag) { autoRelease_ = flag; }

    GLuint programId() ধ্রুৱ { ঘূৰাই programId_; }

গুপ্ত:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    তার্কিক autoRelease_;
};

ধ্রুৱ Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    স্থির Ptr<Impl> p(নূতন Impl);
    ঘূৰাই p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(মিছা)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, তার্কিক autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(ধ্রুৱ Shader& vert, ধ্রুৱ Shader& frag, তার্কিক autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    যদি (!status) {
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
    যদি (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

শূন্য cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

পূর্ণাঙ্ক cv::ogl::Program::Impl::getAttributeLocation(ধ্রুৱ ৱর্ণ* name) ধ্রুৱ
{
    // -1 if absent (e.g. optimized out); caller decides.
    পূর্ণাঙ্ক location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    ঘূৰাই location;
}

পূর্ণাঙ্ক cv::ogl::Program::Impl::getUniformLocation(ধ্রুৱ ৱর্ণ* name) ধ্রুৱ
{
    // -1 if absent (e.g. optimized out); caller decides.
    পূর্ণাঙ্ক location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    ঘূৰাই location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(ধ্রুৱ Shader& vert, ধ্রুৱ Shader& frag, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(vert, frag, autoRelease));
#endif
}

শূন্য cv::ogl::Program::create(ধ্রুৱ Shader& vert, ধ্রুৱ Shader& frag, তার্কিক autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#অন্যথা
    impl_.reset(নূতন Impl(vert, frag, autoRelease));
#endif
}

শূন্য cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    যদি (impl_)
        impl_->setAutoRelease(সঁচা);
    impl_ = Impl::empty();
#endif
}

শূন্য cv::ogl::Program::setAutoRelease(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    impl_->setAutoRelease(flag);
#endif
}

শূন্য cv::ogl::Program::bind() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    impl_->bind();
#endif
}

শূন্য cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    gl::UseProgram(0);
#endif
}

পূর্ণাঙ্ক cv::ogl::Program::getAttributeLocation(ধ্রুৱ ৱর্ণ* name) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->getAttributeLocation(name);
#endif
}

পূর্ণাঙ্ক cv::ogl::Program::getUniformLocation(ধ্রুৱ ৱর্ণ* name) ধ্রুৱ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->getUniformLocation(name);
#endif
}

শূন্য cv::ogl::Program::setUniformVec3(পূর্ণাঙ্ক loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#অন্যথা
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

শূন্য cv::ogl::Program::setUniformVec2(পূর্ণাঙ্ক loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#অন্যথা
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

শূন্য cv::ogl::Program::setUniformVec4(পূর্ণাঙ্ক loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#অন্যথা
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

শূন্য cv::ogl::Program::setUniformMat4x4(পূর্ণাঙ্ক loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#অন্যথা
    gl::UniformMatrix4fv(loc, 1, সঁচা, mat.val);
#endif
}

শূন্য cv::ogl::Program::setUniform1f(পূর্ণাঙ্ক loc, ভিন্ন value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#অন্যথা
    gl::Uniform1f(loc, value);
#endif
}

শূন্য cv::ogl::Program::setUniform1i(পূর্ণাঙ্ক loc, পূর্ণাঙ্ক value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#অন্যথা
    gl::Uniform1i(loc, value);
#endif
}

unsigned পূর্ণাঙ্ক cv::ogl::Program::programId() ধ্রুৱ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#অন্যথা
    ঘূৰাই impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

শূন্য cv::ogl::render(ধ্রুৱ ogl::Texture2D& tex, Rect_<দ্ৱিগুণ> wndRect, Rect_<দ্ৱিগুণ> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#অন্যথা
    যদি (!tex.empty())
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

        ধ্রুৱ দ্ৱিগুণ vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        ধ্রুৱ দ্ৱিগুণ texCoords[] =
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

শূন্য cv::ogl::render(ধ্রুৱ ogl::Arrays& arr, পূর্ণাঙ্ক mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#অন্যথা
    যদি (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

শূন্য cv::ogl::render(ধ্রুৱ ogl::Arrays& arr, প্ৰবেশ_Array indices, পূর্ণাঙ্ক mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#অন্যথা
    যদি (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ধ্রুৱ পূর্ণাঙ্ক kind = indices.kind();

        ৱিকল্প (kind)
        {
        পক্ষ প্ৰবেশ_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                ধ্রুৱ পূর্ণাঙ্ক depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                যদি (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                অন্যথা যদি (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                অন্যথা
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                ভঙ্গ;
            }

        স্ৱভাৱ:
            {
                Mat mat = indices.getMat();

                ধ্রুৱ পূর্ণাঙ্ক depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                যদি (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                অন্যথা যদি (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                অন্যথা
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

শূন্য cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#অন্যথা
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

শূন্য cv::ogl::clearDepth(ভিন্ন depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#অন্যথা
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

শূন্য cv::ogl::drawArrays(পূর্ণাঙ্ক first, পূর্ণাঙ্ক গুনতি_2, পূর্ণাঙ্ক mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(গুনতি_2); CV_UNUSED(mode);
    throw_no_ogl();
#অন্যথা
    gl::DrawArrays(mode, first, গুনতি_2);
#endif // HAVE_OPENGL
}

শূন্য cv::ogl::drawElements(পূর্ণাঙ্ক first, পূর্ণাঙ্ক গুনতি_2, পূর্ণাঙ্ক type, পূর্ণাঙ্ক mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(গুনতি_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#অন্যথা
    gl::DrawElements(mode, গুনতি_2, type, reinterpret_cast<ধ্রুৱ শূন্য*>(static_cast<size_t>(first)));
#endif
}

শূন্য cv::ogl::drawElementsInstanced(পূর্ণাঙ্ক first, পূর্ণাঙ্ক গুনতি_2, পূর্ণাঙ্ক type, পূর্ণাঙ্ক mode, পূর্ণাঙ্ক instance_গুনতি)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(গুনতি_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_গুনতি);
    throw_no_ogl();
#অন্যথা
    gl::DrawElementsInstanced(mode, গুনতি_2, type, reinterpret_cast<ধ্রুৱ শূন্য*>(static_cast<size_t>(first)), instance_গুনতি);
    CV_CheckGlError();
#endif
}

শূন্য cv::ogl::blendFunc(পূর্ণাঙ্ক sfactor, পূর্ণাঙ্ক dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#অন্যথা
    gl::BlendFunc(sfactor, dfactor);
#endif
}

শূন্য cv::ogl::depthMask(তার্কিক flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#অন্যথা
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

শূন্য cv::ogl::enable(পূর্ণাঙ্ক cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#অন্যথা
    gl::Enable(cap);
#endif
}

শূন্য cv::ogl::disable(পূর্ণাঙ্ক cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#অন্যথা
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
#  অন্যথা
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#অন্যথা // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#যদি defined(HAVE_OPENGL)
#  যদি defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

নামাকাশ cv { নামাকাশ ogl {

#যদি defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
স্থির শূন্য checkOpenCLVersion()
{
    using নামাকাশ cv::ocl;
    ধ্রুৱ Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, পরিমাণ(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == পরিমাণ(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    পূর্ণাঙ্ক versionMajor = pi.versionMajor();
    পূর্ণাঙ্ক versionMinor = pi.versionMinor();
    যদি (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

নামাকাশ ocl {

Context& initializeContextFromGL()
{
#যদি !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#অন্যথা
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, রিক্ত, &platformsCnt);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    যদি (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    তার্কিক sharingSupported = মিছা;

    বাবে (unsigned পূর্ণাঙ্ক i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, রিক্ত, &devCnt);
        যদি (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        যত্ন {
            devices = নূতন cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, রিক্ত);
            যদি (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            বাবে (unsigned পূর্ণাঙ্ক j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, রিক্ত, &extensionSize );
                যদি (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                যদি(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    যদি (status != CL_SUCCESS)
                        অনুৱর্ত;
                    যদি (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    যেতিয়ালৈ (spacePos != devString.npos) {
                        যদি (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = সঁচা;
                            ভঙ্গ;
                        }
                        কৰা {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } যেতিয়ালৈ (spacePos == oldPos);
                    }
                }
            }
        } গৃহাণ(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            যদি(devices != nullptr) {
                delete[] devices;
            }
            অনুৱর্ত;
        }

        যদি (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#যদি defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, রিক্ত, রিক্ত, &ci_Err_অঙ্ক);
#নোচেদ্যদি defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], রিক্ত, রিক্ত, &status);
#নোচেদ্যদি defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], রিক্ত, রিক্ত, &status);
#নোচেদ্যদি defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], রিক্ত, রিক্ত, &status);
#endif
        }

        যদি (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        অন্যথা
            ভঙ্গ;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    ঘূৰাই const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

শূন্য convertToGLTexture2D(প্ৰবেশ_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#যদি !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#অন্যথা
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (পূর্ণাঙ্ক)texture.cols() && srcSize.height == (পূর্ণাঙ্ক)texture.rows());

    using নামাকাশ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

শূন্য convertFromGLTexture2D(ধ্রুৱ Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#যদি !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#অন্যথা
    // check texture format
    ধ্রুৱ পূর্ণাঙ্ক dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    পূর্ণাঙ্ক textureType = dtype;
    CV_Assert(textureType >= 0);

    using নামাকাশ cv::ocl;
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
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ধ্রুৱ Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#যদি !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#অন্যথা
    using নামাকাশ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    পূর্ণাঙ্ক clAccessFlags = 0;
    ৱিকল্প (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    স্ৱভাৱ:
    পক্ষ ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        ভঙ্গ;
    পক্ষ ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        ভঙ্গ;
    পক্ষ ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        ভঙ্গ;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    পূর্ণাঙ্ক rows = buffer.rows();
    পূর্ণাঙ্ক cols = buffer.cols();
    পূর্ণাঙ্ক type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    ঘূৰাই u;
#endif
}

শূন্য unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#যদি !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#নোচেদ্যদি !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#অন্যথা
    using নামাকাশ cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, রিক্ত, রিক্ত);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    যদি (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
