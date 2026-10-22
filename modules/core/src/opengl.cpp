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

#شامل "precomp.hpp"

#اگر defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#وگرنه
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    اگر (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#وگرنه // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using فضاینام cv;
using فضاینام cv::cuda;

#اگر defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

فضاینام
{
#ifndef HAVE_OPENGL
درونخطی ایستا CV_NORETURN تهی throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#وگرنهاگر defined _DEBUG
درونخطی ایستا منطقی checkError(ثابت نویسه* file, ثابت صحیح سطر, ثابت نویسه* func = 0)
{
    GLenum err = gl::GetError();
    اگر (err != gl::NO_ERROR_)
    {
        ثابت نویسه* پیام;
        انتخاب (err)
        {
        حالت gl::INVALID_ENUM:
            پیام = "An unacceptable value is specified for an enumerated argument";
            بشکن;
        حالت gl::INVALID_مقدار:
            پیام = "A numeric argument is out of range";
            بشکن;
        حالت gl::INVALID_OPERATION:
            پیام = "The specified operation is not allowed in the current state";
            بشکن;
        حالت gl::OUT_OF_MEMORY:
            پیام = "There is not enough memory left to execute the command";
            بشکن;
        پیشفرض:
            پیام = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, پیام, file, سطر);
    }
    بازگشت درست;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
فضاینام
{
    ثابت GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

تهی cv::cuda::setGlDevice(صحیح device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #وگرنه
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#اگر defined(HAVE_OPENGL) && defined(HAVE_CUDA)

فضاینام
{
    رده CudaResource
    {
    عمومی:
        CudaResource();
        ~CudaResource();

        تهی registerBuffer(GLuint buffer);
        تهی release();

        تهی copyFrom(ثابت تهی* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        تهی copyTo(تهی* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        تهی* map(cudaStream_t stream = 0);
        تهی unmap(cudaStream_t stream = 0);

    خصوصی:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        رده GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    تهی CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        اگر (buffer_ == buffer)
            بازگشت;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    تهی CudaResource::release()
    {
        اگر (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    رده CudaResource::GraphicsMapHolder
    {
    عمومی:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        تهی reset();

    خصوصی:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        اگر (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        اگر (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    تهی CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    تهی CudaResource::copyFrom(ثابت تهی* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        تهی* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        اگر (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        وگرنه
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    تهی CudaResource::copyTo(تهی* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        تهی* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        اگر (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        وگرنه
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    تهی* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        تهی* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        بازگشت ptr;
    }

    تهی CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

رده cv::ogl::Buffer::Impl
{
};

#وگرنه

رده cv::ogl::Buffer::Impl
{
عمومی:
    ایستا ثابت Ptr<Impl>& empty();

    Impl(GLuint bufId, منطقی autoRelease);
    Impl(GLsizeiptr size, ثابت GLvoid* data, GLenum target, منطقی autoRelease);
    ~Impl();

    تهی bind(GLenum target) ثابت;

    تهی copyFrom(GLuint srcBuf, GLsizeiptr size);

    تهی copyFrom(GLsizeiptr size, ثابت GLvoid* data);
    تهی copyTo(GLsizeiptr size, GLvoid* data) ثابت;

    تهی resize(GLsizeiptr size, GLenum target);

    تهی* mapHost(GLenum access);
    تهی unmapHost();

#ifdef HAVE_CUDA
    تهی copyFrom(ثابت تهی* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    تهی copyTo(تهی* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) ثابت;

    تهی* mapDevice(cudaStream_t stream = 0);
    تهی unmapDevice(cudaStream_t stream = 0);
#endif

    تهی setAutoRelease(منطقی flag) { autoRelease_ = flag; }

    GLuint bufId() ثابت { بازگشت bufId_; }

خصوصی:
    Impl();

    GLuint bufId_;
    منطقی autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

ثابت Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    ایستا Ptr<Impl> p(جدید Impl);
    بازگشت p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(نادرست)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, منطقی autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, ثابت GLvoid* data, GLenum target, منطقی autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    اگر (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

تهی cv::ogl::Buffer::Impl::bind(GLenum target) ثابت
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

تهی cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

تهی cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, ثابت GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

تهی cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

تهی cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) ثابت
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

تهی* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    بازگشت data;
}

تهی cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

تهی cv::ogl::Buffer::Impl::copyFrom(ثابت تهی* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

تهی cv::ogl::Buffer::Impl::copyTo(تهی* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) ثابت
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

تهی* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    بازگشت cudaResource_.map(stream);
}

تهی cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(صحیح arows, صحیح acols, صحیح atype, unsigned صحیح abufId, منطقی autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, صحیح atype, unsigned صحیح abufId, منطقی autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(درون_داد_Array arr, Target target, منطقی autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    ثابت صحیح kind = arr.kind();

    انتخاب (kind)
    {
    حالت درون_داد_Array_2::OPENGL_BUFFER:
    حالت درون_داد_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        بشکن;

    پیشفرض:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ثابت GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(جدید Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            بشکن;
        }
    }
#endif
}

تهی cv::ogl::Buffer::create(صحیح arows, صحیح acols, صحیح atype, Target target, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    اگر (rows_ != arows || cols_ != acols || type_ != atype)
    {
        ثابت GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        اگر (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        وگرنه
            impl_.reset(جدید Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

تهی cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    اگر (impl_)
        impl_->setAutoRelease(درست);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

تهی cv::ogl::Buffer::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    impl_->setAutoRelease(flag);
#endif
}

تهی cv::ogl::Buffer::copyFrom(درون_داد_Array arr, Target target, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    ثابت صحیح kind = arr.kind();

    ثابت Size asize = arr.size();
    ثابت صحیح atype = arr.type();
    create(asize, atype, target, autoRelease);

    انتخاب (kind)
    {
    حالت درون_داد_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            بشکن;
        }

    حالت درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وگرنه
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            بشکن;
        }

    پیشفرض:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

تهی cv::ogl::Buffer::copyFrom(درون_داد_Array arr, cuda::Stream& stream, Target target, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #وگرنه
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

تهی cv::ogl::Buffer::copyTo(OutputArray arr) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#وگرنه
    ثابت صحیح kind = arr.kind();

    انتخاب (kind)
    {
    حالت درون_داد_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*این);
            بشکن;
        }

    حالت درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وگرنه
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            بشکن;
        }

    پیشفرض:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

تهی cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #وگرنه
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, منطقی autoRelease) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    ogl::Buffer buf;
    buf.copyFrom(*این, target, autoRelease);
    بازگشت buf;
#endif
}

تهی cv::ogl::Buffer::bind(Target target) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#وگرنه
    impl_->bind(target);
#endif
}

تهی cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#وگرنه
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#وگرنه
    بازگشت Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

تهی cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #وگرنه
        بازگشت GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

تهی cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #وگرنه
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #وگرنه
        بازگشت GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

تهی cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#وگرنه
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #وگرنه
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned صحیح cv::ogl::Buffer::bufId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

رده cv::ogl::Texture2D::Impl
{
};

#وگرنه

رده cv::ogl::Texture2D::Impl
{
عمومی:
    ایستا ثابت Ptr<Impl> empty();

    Impl(GLuint texId, منطقی autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقی autoRelease);
    ~Impl();

    تهی copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid *pixels);
    تهی copyTo(GLenum format, GLenum type, GLvoid* pixels) ثابت;

    تهی bind() ثابت;

    تهی setAutoRelease(منطقی flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { بازگشت texId_; }

خصوصی:
    Impl();

    GLuint texId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ایستا Ptr<Impl> p(جدید Impl);
    بازگشت p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(نادرست)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, منطقی autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقی autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    اگر (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

تهی cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid *pixels)
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

تهی cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) ثابت
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

تهی cv::ogl::Texture2D::Impl::bind() ثابت
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(صحیح arows, صحیح acols, Format aformat, unsigned صحیح atexId, منطقی autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned صحیح atexId, منطقی autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(درون_داد_Array arr, منطقی autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    ثابت صحیح kind = arr.kind();

    ثابت Size asize = arr.size();
    ثابت صحیح atype = arr.type();

    ثابت صحیح depth = CV_MAT_DEPTH(atype);
    ثابت صحیح cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    انتخاب (kind)
    {
    حالت درون_داد_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(جدید Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            بشکن;
        }

    حالت درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وگرنه
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(درست);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(جدید Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            بشکن;
        }

    پیشفرض:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(جدید Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            بشکن;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

تهی cv::ogl::Texture2D::create(صحیح arows, صحیح acols, Format aformat, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    اگر (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(جدید Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

تهی cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    اگر (impl_)
        impl_->setAutoRelease(درست);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

تهی cv::ogl::Texture2D::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    impl_->setAutoRelease(flag);
#endif
}

تهی cv::ogl::Texture2D::copyFrom(درون_داد_Array arr, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    ثابت صحیح kind = arr.kind();

    ثابت Size asize = arr.size();
    ثابت صحیح atype = arr.type();

    ثابت صحیح depth = CV_MAT_DEPTH(atype);
    ثابت صحیح cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    انتخاب(kind)
    {
    حالت درون_داد_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            بشکن;
        }

    حالت درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وگرنه
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(درست);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            بشکن;
        }

    پیشفرض:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

تهی cv::ogl::Texture2D::copyTo(OutputArray arr, صحیح ddepth, منطقی autoRelease) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    ثابت صحیح kind = arr.kind();

    ثابت صحیح cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ثابت GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    انتخاب(kind)
    {
    حالت درون_داد_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            بشکن;
        }

    حالت درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وگرنه
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(درست);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            بشکن;
        }

    پیشفرض:
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

تهی cv::ogl::Texture2D::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_->bind();
#endif
}

unsigned صحیح cv::ogl::Texture2D::texId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

رده cv::ogl::TextureBuffer::Impl
{
};

#وگرنه

رده cv::ogl::TextureBuffer::Impl
{
عمومی:
    ایستا ثابت Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, منطقی autoRelease);
    ~Impl();

    تهی bind(GLint unit) ثابت;

    تهی setAutoRelease(منطقی flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { بازگشت texId_; }

خصوصی:
    GLuint texId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    ایستا Ptr<Impl> p = makePtr<Impl>();
    بازگشت p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(نادرست)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, منطقی autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    اگر (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

تهی cv::ogl::TextureBuffer::Impl::bind(GLint unit) ثابت
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
#وگرنه
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(ثابت Buffer& buf, Format aformat, منطقی autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

تهی cv::ogl::TextureBuffer::create(ثابت Buffer& buf, Format aformat, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

تهی cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    اگر (impl_)
        impl_->setAutoRelease(درست);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

تهی cv::ogl::TextureBuffer::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    impl_->setAutoRelease(flag);
#endif
}

تهی cv::ogl::TextureBuffer::bind(صحیح unit) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#وگرنه
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() ثابت
{
    بازگشت format_;
}

منطقی cv::ogl::TextureBuffer::empty() ثابت
{
    بازگشت format_ == NONE;
}

unsigned صحیح cv::ogl::TextureBuffer::texId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

تهی cv::ogl::Arrays::setVertexArray(درون_داد_Array vertex)
{
    ثابت صحیح cn = vertex.channels();
    ثابت صحیح depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    اگر (vertex.kind() == درون_داد_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    وگرنه
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

تهی cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

تهی cv::ogl::Arrays::setColorArray(درون_داد_Array color)
{
    ثابت صحیح cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    اگر (color.kind() == درون_داد_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    وگرنه
        color_.copyFrom(color);
}

تهی cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

تهی cv::ogl::Arrays::setNormalArray(درون_داد_Array normal)
{
    ثابت صحیح cn = normal.channels();
    ثابت صحیح depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    اگر (normal.kind() == درون_داد_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    وگرنه
        normal_.copyFrom(normal);
}

تهی cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

تهی cv::ogl::Arrays::setTexCoordArray(درون_داد_Array texCoord)
{
    ثابت صحیح cn = texCoord.channels();
    ثابت صحیح depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    اگر (texCoord.kind() == درون_داد_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    وگرنه
        texCoord_.copyFrom(texCoord);
}

تهی cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

تهی cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

تهی cv::ogl::Arrays::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

تهی cv::ogl::Arrays::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    اگر (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    وگرنه
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    اگر (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    وگرنه
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    اگر (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    وگرنه
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        ثابت صحیح cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    اگر (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    وگرنه
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

رده cv::ogl::VertexArray::Impl
{
};

#وگرنه

رده cv::ogl::VertexArray::Impl
{
عمومی:
    ایستا ثابت Ptr<Impl>& empty();

    Impl(GLuint vaId, منطقی autoRelease);
    Impl(std::initializer_list<Attribute> attributes, منطقی autoRelease);
    ~Impl();

    تهی bind();

    تهی setAutoRelease(منطقی flag) { autoRelease_ = flag; }

    GLuint vaId() ثابت { بازگشت vaId_; }

خصوصی:
    Impl();

    GLuint vaId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    ایستا Ptr<Impl> p(جدید Impl);
    بازگشت p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(نادرست)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, منطقی autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, منطقی autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    برای (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        اگر (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت تهی*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        وگرنه
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت تهی*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    اگر (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

تهی cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(attributes, autoRelease));
#endif
}

تهی cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(attributes, autoRelease));
#endif
}

تهی cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    اگر (impl_)
        impl_->setAutoRelease(درست);
    impl_ = Impl::empty();
#endif
}

تهی cv::ogl::VertexArray::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    impl_->setAutoRelease(flag);
#endif
}

تهی cv::ogl::VertexArray::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_->bind();
#endif
}

تهی cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    gl::BindVertexArray(0);
#endif
}

unsigned صحیح cv::ogl::VertexArray::vaId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

رده cv::ogl::Shader::Impl
{
};

#وگرنه

رده cv::ogl::Shader::Impl
{
عمومی:
    ایستا ثابت Ptr<Impl>& empty();

    Impl(GLuint shaderId, منطقی autoRelease);
    Impl(ثابت نویسه* src, GLenum type, منطقی autoRelease);
    ~Impl();

    تهی setAutoRelease(منطقی flag) { autoRelease_ = flag; }

    GLuint shaderId() ثابت { بازگشت shaderId_; }

خصوصی:
    Impl();

    GLuint shaderId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ایستا Ptr<Impl> p(جدید Impl);
    بازگشت p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(نادرست)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, منطقی autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ثابت نویسه* src, GLenum type, منطقی autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    اگر (!status) {
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
    اگر (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(ثابت نویسه* src, ShaderType type, منطقی autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(src, type, autoRelease));
#endif
}

تهی cv::ogl::Shader::create(ثابت نویسه* src, ShaderType type, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(src, type, autoRelease));
    type_ = type;
#endif
}

تهی cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    اگر (impl_)
        impl_->setAutoRelease(درست);
    impl_ = Impl::empty();
#endif
}

تهی cv::ogl::Shader::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    impl_->setAutoRelease(flag);
#endif
}

unsigned صحیح cv::ogl::Shader::shaderId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ثابت
{
    بازگشت type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

رده cv::ogl::Program::Impl
{
};

#وگرنه

رده cv::ogl::Program::Impl
{
عمومی:
    ایستا ثابت Ptr<Impl>& empty();

    Impl(GLuint programId, منطقی autoRelease);
    Impl(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease);
    ~Impl();

    تهی bind();

    صحیح getAttributeLocation(ثابت نویسه* name) ثابت;
    صحیح getUniformLocation(ثابت نویسه* name) ثابت;

    تهی setAutoRelease(منطقی flag) { autoRelease_ = flag; }

    GLuint programId() ثابت { بازگشت programId_; }

خصوصی:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    ایستا Ptr<Impl> p(جدید Impl);
    بازگشت p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(نادرست)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, منطقی autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    اگر (!status) {
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
    اگر (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

تهی cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

صحیح cv::ogl::Program::Impl::getAttributeLocation(ثابت نویسه* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحیح location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    بازگشت location;
}

صحیح cv::ogl::Program::Impl::getUniformLocation(ثابت نویسه* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحیح location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    بازگشت location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(vert, frag, autoRelease));
#endif
}

تهی cv::ogl::Program::create(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وگرنه
    impl_.reset(جدید Impl(vert, frag, autoRelease));
#endif
}

تهی cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    اگر (impl_)
        impl_->setAutoRelease(درست);
    impl_ = Impl::empty();
#endif
}

تهی cv::ogl::Program::setAutoRelease(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    impl_->setAutoRelease(flag);
#endif
}

تهی cv::ogl::Program::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    impl_->bind();
#endif
}

تهی cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    gl::UseProgram(0);
#endif
}

صحیح cv::ogl::Program::getAttributeLocation(ثابت نویسه* name) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#وگرنه
    بازگشت impl_->getAttributeLocation(name);
#endif
}

صحیح cv::ogl::Program::getUniformLocation(ثابت نویسه* name) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#وگرنه
    بازگشت impl_->getUniformLocation(name);
#endif
}

تهی cv::ogl::Program::setUniformVec3(صحیح loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#وگرنه
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

تهی cv::ogl::Program::setUniformVec2(صحیح loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#وگرنه
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

تهی cv::ogl::Program::setUniformVec4(صحیح loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#وگرنه
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

تهی cv::ogl::Program::setUniformMat4x4(صحیح loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#وگرنه
    gl::UniformMatrix4fv(loc, 1, درست, mat.val);
#endif
}

تهی cv::ogl::Program::setUniform1f(صحیح loc, اعشاری value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#وگرنه
    gl::Uniform1f(loc, value);
#endif
}

تهی cv::ogl::Program::setUniform1i(صحیح loc, صحیح value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#وگرنه
    gl::Uniform1i(loc, value);
#endif
}

unsigned صحیح cv::ogl::Program::programId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وگرنه
    بازگشت impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

تهی cv::ogl::render(ثابت ogl::Texture2D& tex, Rect_<دوگانه> wndRect, Rect_<دوگانه> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#وگرنه
    اگر (!tex.empty())
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

        ثابت دوگانه vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        ثابت دوگانه texCoords[] =
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

تهی cv::ogl::render(ثابت ogl::Arrays& arr, صحیح mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#وگرنه
    اگر (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

تهی cv::ogl::render(ثابت ogl::Arrays& arr, درون_داد_Array indices, صحیح mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#وگرنه
    اگر (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ثابت صحیح kind = indices.kind();

        انتخاب (kind)
        {
        حالت درون_داد_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                ثابت صحیح depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                اگر (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وگرنه اگر (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وگرنه
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                بشکن;
            }

        پیشفرض:
            {
                Mat mat = indices.getMat();

                ثابت صحیح depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                اگر (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وگرنه اگر (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وگرنه
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

تهی cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#وگرنه
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

تهی cv::ogl::clearDepth(اعشاری depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#وگرنه
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

تهی cv::ogl::drawArrays(صحیح first, صحیح شمار_2, صحیح mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(شمار_2); CV_UNUSED(mode);
    throw_no_ogl();
#وگرنه
    gl::DrawArrays(mode, first, شمار_2);
#endif // HAVE_OPENGL
}

تهی cv::ogl::drawElements(صحیح first, صحیح شمار_2, صحیح type, صحیح mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(شمار_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#وگرنه
    gl::DrawElements(mode, شمار_2, type, reinterpret_cast<ثابت تهی*>(static_cast<size_t>(first)));
#endif
}

تهی cv::ogl::drawElementsInstanced(صحیح first, صحیح شمار_2, صحیح type, صحیح mode, صحیح instance_شمار)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(شمار_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_شمار);
    throw_no_ogl();
#وگرنه
    gl::DrawElementsInstanced(mode, شمار_2, type, reinterpret_cast<ثابت تهی*>(static_cast<size_t>(first)), instance_شمار);
    CV_CheckGlError();
#endif
}

تهی cv::ogl::blendFunc(صحیح sfactor, صحیح dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#وگرنه
    gl::BlendFunc(sfactor, dfactor);
#endif
}

تهی cv::ogl::depthMask(منطقی flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وگرنه
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

تهی cv::ogl::enable(صحیح cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#وگرنه
    gl::Enable(cap);
#endif
}

تهی cv::ogl::disable(صحیح cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#وگرنه
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
#  وگرنه
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#وگرنه // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#اگر defined(HAVE_OPENGL)
#  اگر defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

فضاینام cv { فضاینام ogl {

#اگر defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ایستا تهی checkOpenCLVersion()
{
    using فضاینام cv::ocl;
    ثابت Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, اندازه(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == اندازه(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    صحیح versionMajor = pi.versionMajor();
    صحیح versionMinor = pi.versionMinor();
    اگر (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

فضاینام ocl {

Context& initializeContextFromGL()
{
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وگرنه
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, پوچ, &platformsCnt);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    اگر (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    منطقی sharingSupported = نادرست;

    برای (unsigned صحیح i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, پوچ, &devCnt);
        اگر (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        تلاش {
            devices = جدید cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, پوچ);
            اگر (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            برای (unsigned صحیح j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, پوچ, &extensionSize );
                اگر (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                اگر(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    اگر (status != CL_SUCCESS)
                        ادامه;
                    اگر (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    وقتی (spacePos != devString.npos) {
                        اگر (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = درست;
                            بشکن;
                        }
                        انجامبده {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } وقتی (spacePos == oldPos);
                    }
                }
            }
        } بگیر(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            اگر(devices != nullptr) {
                delete[] devices;
            }
            ادامه;
        }

        اگر (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#اگر defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, پوچ, پوچ, &ci_Err_رقم);
#وگرنهاگر defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], پوچ, پوچ, &status);
#وگرنهاگر defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], پوچ, پوچ, &status);
#وگرنهاگر defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], پوچ, پوچ, &status);
#endif
        }

        اگر (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        وگرنه
            بشکن;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    بازگشت const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

تهی convertToGLTexture2D(درون_داد_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وگرنه
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (صحیح)texture.cols() && srcSize.height == (صحیح)texture.rows());

    using فضاینام cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

تهی convertFromGLTexture2D(ثابت Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وگرنه
    // check texture format
    ثابت صحیح dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    صحیح textureType = dtype;
    CV_Assert(textureType >= 0);

    using فضاینام cv::ocl;
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
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ثابت Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وگرنه
    using فضاینام cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    صحیح clAccessFlags = 0;
    انتخاب (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    پیشفرض:
    حالت ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        بشکن;
    حالت ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        بشکن;
    حالت ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        بشکن;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    صحیح rows = buffer.rows();
    صحیح cols = buffer.cols();
    صحیح type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    بازگشت u;
#endif
}

تهی unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنهاگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وگرنه
    using فضاینام cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, پوچ, پوچ);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    اگر (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
