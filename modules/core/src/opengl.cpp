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

#அந்தர்பாவ "precomp.hpp"

#எனில் defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#இல்லையெனில்
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    எனில் (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#இல்லையெனில் // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using நாமாகாஶ cv;
using நாமாகாஶ cv::cuda;

#எனில் defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

நாமாகாஶ
{
#ifndef HAVE_OPENGL
உள்வரி நிலையான CV_NORETURN வெறுமை throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#அல்லதெனில் defined _DEBUG
உள்வரி நிலையான தருக்க checkError(மாறிலி எழுத்து* file, மாறிலி முழு வரி, மாறிலி எழுத்து* func = 0)
{
    GLenum err = gl::GetError();
    எனில் (err != gl::NO_ERROR_)
    {
        மாறிலி எழுத்து* செய்தி;
        தேர்வு (err)
        {
        நிலை gl::INVALID_ENUM:
            செய்தி = "An unacceptable value is specified for an enumerated argument";
            நிறுத்து;
        நிலை gl::INVALID_மதிப்பு:
            செய்தி = "A numeric argument is out of range";
            நிறுத்து;
        நிலை gl::INVALID_OPERATION:
            செய்தி = "The specified operation is not allowed in the current state";
            நிறுத்து;
        நிலை gl::OUT_OF_MEMORY:
            செய்தி = "There is not enough memory left to execute the command";
            நிறுத்து;
        இயல்பு:
            செய்தி = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, செய்தி, file, வரி);
    }
    திரும்பு உண்மை;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
நாமாகாஶ
{
    மாறிலி GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

வெறுமை cv::cuda::setGlDevice(முழு device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #இல்லையெனில்
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#எனில் defined(HAVE_OPENGL) && defined(HAVE_CUDA)

நாமாகாஶ
{
    வகுப்பு CudaResource
    {
    பொது:
        CudaResource();
        ~CudaResource();

        வெறுமை registerBuffer(GLuint buffer);
        வெறுமை release();

        வெறுமை copyFrom(மாறிலி வெறுமை* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        வெறுமை copyTo(வெறுமை* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        வெறுமை* map(cudaStream_t stream = 0);
        வெறுமை unmap(cudaStream_t stream = 0);

    தனிப்பட்ட:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        வகுப்பு GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    வெறுமை CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        எனில் (buffer_ == buffer)
            திரும்பு;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    வெறுமை CudaResource::release()
    {
        எனில் (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    வகுப்பு CudaResource::GraphicsMapHolder
    {
    பொது:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        வெறுமை reset();

    தனிப்பட்ட:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        எனில் (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        எனில் (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    வெறுமை CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    வெறுமை CudaResource::copyFrom(மாறிலி வெறுமை* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        வெறுமை* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        எனில் (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        இல்லையெனில்
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    வெறுமை CudaResource::copyTo(வெறுமை* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        வெறுமை* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        எனில் (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        இல்லையெனில்
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    வெறுமை* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        வெறுமை* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        திரும்பு ptr;
    }

    வெறுமை CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

வகுப்பு cv::ogl::Buffer::Impl
{
};

#இல்லையெனில்

வகுப்பு cv::ogl::Buffer::Impl
{
பொது:
    நிலையான மாறிலி Ptr<Impl>& empty();

    Impl(GLuint bufId, தருக்க autoRelease);
    Impl(GLsizeiptr size, மாறிலி GLvoid* data, GLenum target, தருக்க autoRelease);
    ~Impl();

    வெறுமை bind(GLenum target) மாறிலி;

    வெறுமை copyFrom(GLuint srcBuf, GLsizeiptr size);

    வெறுமை copyFrom(GLsizeiptr size, மாறிலி GLvoid* data);
    வெறுமை copyTo(GLsizeiptr size, GLvoid* data) மாறிலி;

    வெறுமை resize(GLsizeiptr size, GLenum target);

    வெறுமை* mapHost(GLenum access);
    வெறுமை unmapHost();

#ifdef HAVE_CUDA
    வெறுமை copyFrom(மாறிலி வெறுமை* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    வெறுமை copyTo(வெறுமை* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) மாறிலி;

    வெறுமை* mapDevice(cudaStream_t stream = 0);
    வெறுமை unmapDevice(cudaStream_t stream = 0);
#endif

    வெறுமை setAutoRelease(தருக்க flag) { autoRelease_ = flag; }

    GLuint bufId() மாறிலி { திரும்பு bufId_; }

தனிப்பட்ட:
    Impl();

    GLuint bufId_;
    தருக்க autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

மாறிலி Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    நிலையான Ptr<Impl> p(புதிய Impl);
    திரும்பு p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(பொய்)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, தருக்க autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, மாறிலி GLvoid* data, GLenum target, தருக்க autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    எனில் (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

வெறுமை cv::ogl::Buffer::Impl::bind(GLenum target) மாறிலி
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

வெறுமை cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

வெறுமை cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, மாறிலி GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

வெறுமை cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

வெறுமை cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) மாறிலி
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

வெறுமை* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    திரும்பு data;
}

வெறுமை cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

வெறுமை cv::ogl::Buffer::Impl::copyFrom(மாறிலி வெறுமை* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

வெறுமை cv::ogl::Buffer::Impl::copyTo(வெறுமை* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) மாறிலி
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

வெறுமை* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    திரும்பு cudaResource_.map(stream);
}

வெறுமை cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(முழு arows, முழு acols, முழு atype, unsigned முழு abufId, தருக்க autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, முழு atype, unsigned முழு abufId, தருக்க autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(உள்ளீடு_Array arr, Target target, தருக்க autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    மாறிலி முழு kind = arr.kind();

    தேர்வு (kind)
    {
    நிலை உள்ளீடு_Array_2::OPENGL_BUFFER:
    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        நிறுத்து;

    இயல்பு:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            மாறிலி GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(புதிய Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            நிறுத்து;
        }
    }
#endif
}

வெறுமை cv::ogl::Buffer::create(முழு arows, முழு acols, முழு atype, Target target, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    எனில் (rows_ != arows || cols_ != acols || type_ != atype)
    {
        மாறிலி GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        எனில் (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        இல்லையெனில்
            impl_.reset(புதிய Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

வெறுமை cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    எனில் (impl_)
        impl_->setAutoRelease(உண்மை);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

வெறுமை cv::ogl::Buffer::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    impl_->setAutoRelease(flag);
#endif
}

வெறுமை cv::ogl::Buffer::copyFrom(உள்ளீடு_Array arr, Target target, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    மாறிலி முழு kind = arr.kind();

    மாறிலி Size asize = arr.size();
    மாறிலி முழு atype = arr.type();
    create(asize, atype, target, autoRelease);

    தேர்வு (kind)
    {
    நிலை உள்ளீடு_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            நிறுத்து;
        }

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #இல்லையெனில்
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            நிறுத்து;
        }

    இயல்பு:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

வெறுமை cv::ogl::Buffer::copyFrom(உள்ளீடு_Array arr, cuda::Stream& stream, Target target, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #இல்லையெனில்
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

வெறுமை cv::ogl::Buffer::copyTo(OutputArray arr) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#இல்லையெனில்
    மாறிலி முழு kind = arr.kind();

    தேர்வு (kind)
    {
    நிலை உள்ளீடு_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*இது);
            நிறுத்து;
        }

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #இல்லையெனில்
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            நிறுத்து;
        }

    இயல்பு:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

வெறுமை cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #இல்லையெனில்
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, தருக்க autoRelease) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    ogl::Buffer buf;
    buf.copyFrom(*இது, target, autoRelease);
    திரும்பு buf;
#endif
}

வெறுமை cv::ogl::Buffer::bind(Target target) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#இல்லையெனில்
    impl_->bind(target);
#endif
}

வெறுமை cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#இல்லையெனில்
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

வெறுமை cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #இல்லையெனில்
        திரும்பு GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

வெறுமை cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #இல்லையெனில்
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #இல்லையெனில்
        திரும்பு GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

வெறுமை cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#இல்லையெனில்
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #இல்லையெனில்
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned முழு cv::ogl::Buffer::bufId() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

வகுப்பு cv::ogl::Texture2D::Impl
{
};

#இல்லையெனில்

வகுப்பு cv::ogl::Texture2D::Impl
{
பொது:
    நிலையான மாறிலி Ptr<Impl> empty();

    Impl(GLuint texId, தருக்க autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, மாறிலி GLvoid* pixels, தருக்க autoRelease);
    ~Impl();

    வெறுமை copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, மாறிலி GLvoid *pixels);
    வெறுமை copyTo(GLenum format, GLenum type, GLvoid* pixels) மாறிலி;

    வெறுமை bind() மாறிலி;

    வெறுமை setAutoRelease(தருக்க flag) { autoRelease_ = flag; }

    GLuint texId() மாறிலி { திரும்பு texId_; }

தனிப்பட்ட:
    Impl();

    GLuint texId_;
    தருக்க autoRelease_;
};

மாறிலி Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    நிலையான Ptr<Impl> p(புதிய Impl);
    திரும்பு p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(பொய்)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, தருக்க autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, மாறிலி GLvoid* pixels, தருக்க autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    எனில் (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

வெறுமை cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, மாறிலி GLvoid *pixels)
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

வெறுமை cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) மாறிலி
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

வெறுமை cv::ogl::Texture2D::Impl::bind() மாறிலி
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(முழு arows, முழு acols, Format aformat, unsigned முழு atexId, தருக்க autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned முழு atexId, தருக்க autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(உள்ளீடு_Array arr, தருக்க autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    மாறிலி முழு kind = arr.kind();

    மாறிலி Size asize = arr.size();
    மாறிலி முழு atype = arr.type();

    மாறிலி முழு depth = CV_MAT_DEPTH(atype);
    மாறிலி முழு cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    மாறிலி Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    மாறிலி GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    தேர்வு (kind)
    {
    நிலை உள்ளீடு_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(புதிய Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            நிறுத்து;
        }

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #இல்லையெனில்
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(உண்மை);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(புதிய Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            நிறுத்து;
        }

    இயல்பு:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(புதிய Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            நிறுத்து;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

வெறுமை cv::ogl::Texture2D::create(முழு arows, முழு acols, Format aformat, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    எனில் (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(புதிய Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

வெறுமை cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    எனில் (impl_)
        impl_->setAutoRelease(உண்மை);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

வெறுமை cv::ogl::Texture2D::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    impl_->setAutoRelease(flag);
#endif
}

வெறுமை cv::ogl::Texture2D::copyFrom(உள்ளீடு_Array arr, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    மாறிலி முழு kind = arr.kind();

    மாறிலி Size asize = arr.size();
    மாறிலி முழு atype = arr.type();

    மாறிலி முழு depth = CV_MAT_DEPTH(atype);
    மாறிலி முழு cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    மாறிலி Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    மாறிலி GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    தேர்வு(kind)
    {
    நிலை உள்ளீடு_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            நிறுத்து;
        }

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #இல்லையெனில்
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(உண்மை);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            நிறுத்து;
        }

    இயல்பு:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

வெறுமை cv::ogl::Texture2D::copyTo(OutputArray arr, முழு ddepth, தருக்க autoRelease) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    மாறிலி முழு kind = arr.kind();

    மாறிலி முழு cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    மாறிலி GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    தேர்வு(kind)
    {
    நிலை உள்ளீடு_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            நிறுத்து;
        }

    நிலை உள்ளீடு_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #இல்லையெனில்
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(உண்மை);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            நிறுத்து;
        }

    இயல்பு:
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

வெறுமை cv::ogl::Texture2D::bind() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_->bind();
#endif
}

unsigned முழு cv::ogl::Texture2D::texId() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

வகுப்பு cv::ogl::TextureBuffer::Impl
{
};

#இல்லையெனில்

வகுப்பு cv::ogl::TextureBuffer::Impl
{
பொது:
    நிலையான மாறிலி Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, தருக்க autoRelease);
    ~Impl();

    வெறுமை bind(GLint unit) மாறிலி;

    வெறுமை setAutoRelease(தருக்க flag) { autoRelease_ = flag; }

    GLuint texId() மாறிலி { திரும்பு texId_; }

தனிப்பட்ட:
    GLuint texId_;
    தருக்க autoRelease_;
};

மாறிலி Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    நிலையான Ptr<Impl> p = makePtr<Impl>();
    திரும்பு p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(பொய்)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, தருக்க autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    எனில் (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

வெறுமை cv::ogl::TextureBuffer::Impl::bind(GLint unit) மாறிலி
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
#இல்லையெனில்
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(மாறிலி Buffer& buf, Format aformat, தருக்க autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

வெறுமை cv::ogl::TextureBuffer::create(மாறிலி Buffer& buf, Format aformat, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

வெறுமை cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    எனில் (impl_)
        impl_->setAutoRelease(உண்மை);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

வெறுமை cv::ogl::TextureBuffer::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    impl_->setAutoRelease(flag);
#endif
}

வெறுமை cv::ogl::TextureBuffer::bind(முழு unit) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#இல்லையெனில்
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() மாறிலி
{
    திரும்பு format_;
}

தருக்க cv::ogl::TextureBuffer::empty() மாறிலி
{
    திரும்பு format_ == NONE;
}

unsigned முழு cv::ogl::TextureBuffer::texId() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

வெறுமை cv::ogl::Arrays::setVertexArray(உள்ளீடு_Array vertex)
{
    மாறிலி முழு cn = vertex.channels();
    மாறிலி முழு depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    எனில் (vertex.kind() == உள்ளீடு_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    இல்லையெனில்
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

வெறுமை cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

வெறுமை cv::ogl::Arrays::setColorArray(உள்ளீடு_Array color)
{
    மாறிலி முழு cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    எனில் (color.kind() == உள்ளீடு_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    இல்லையெனில்
        color_.copyFrom(color);
}

வெறுமை cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

வெறுமை cv::ogl::Arrays::setNormalArray(உள்ளீடு_Array normal)
{
    மாறிலி முழு cn = normal.channels();
    மாறிலி முழு depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    எனில் (normal.kind() == உள்ளீடு_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    இல்லையெனில்
        normal_.copyFrom(normal);
}

வெறுமை cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

வெறுமை cv::ogl::Arrays::setTexCoordArray(உள்ளீடு_Array texCoord)
{
    மாறிலி முழு cn = texCoord.channels();
    மாறிலி முழு depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    எனில் (texCoord.kind() == உள்ளீடு_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    இல்லையெனில்
        texCoord_.copyFrom(texCoord);
}

வெறுமை cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

வெறுமை cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

வெறுமை cv::ogl::Arrays::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

வெறுமை cv::ogl::Arrays::bind() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    எனில் (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    இல்லையெனில்
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    எனில் (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    இல்லையெனில்
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    எனில் (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    இல்லையெனில்
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        மாறிலி முழு cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    எனில் (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    இல்லையெனில்
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

வகுப்பு cv::ogl::VertexArray::Impl
{
};

#இல்லையெனில்

வகுப்பு cv::ogl::VertexArray::Impl
{
பொது:
    நிலையான மாறிலி Ptr<Impl>& empty();

    Impl(GLuint vaId, தருக்க autoRelease);
    Impl(std::initializer_list<Attribute> attributes, தருக்க autoRelease);
    ~Impl();

    வெறுமை bind();

    வெறுமை setAutoRelease(தருக்க flag) { autoRelease_ = flag; }

    GLuint vaId() மாறிலி { திரும்பு vaId_; }

தனிப்பட்ட:
    Impl();

    GLuint vaId_;
    தருக்க autoRelease_;
};

மாறிலி Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    நிலையான Ptr<Impl> p(புதிய Impl);
    திரும்பு p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(பொய்)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, தருக்க autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, தருக்க autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    க்கு (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        எனில் (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<மாறிலி வெறுமை*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        இல்லையெனில்
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<மாறிலி வெறுமை*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    எனில் (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

வெறுமை cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(attributes, autoRelease));
#endif
}

வெறுமை cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(attributes, autoRelease));
#endif
}

வெறுமை cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    எனில் (impl_)
        impl_->setAutoRelease(உண்மை);
    impl_ = Impl::empty();
#endif
}

வெறுமை cv::ogl::VertexArray::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    impl_->setAutoRelease(flag);
#endif
}

வெறுமை cv::ogl::VertexArray::bind() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_->bind();
#endif
}

வெறுமை cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    gl::BindVertexArray(0);
#endif
}

unsigned முழு cv::ogl::VertexArray::vaId() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

வகுப்பு cv::ogl::Shader::Impl
{
};

#இல்லையெனில்

வகுப்பு cv::ogl::Shader::Impl
{
பொது:
    நிலையான மாறிலி Ptr<Impl>& empty();

    Impl(GLuint shaderId, தருக்க autoRelease);
    Impl(மாறிலி எழுத்து* src, GLenum type, தருக்க autoRelease);
    ~Impl();

    வெறுமை setAutoRelease(தருக்க flag) { autoRelease_ = flag; }

    GLuint shaderId() மாறிலி { திரும்பு shaderId_; }

தனிப்பட்ட:
    Impl();

    GLuint shaderId_;
    தருக்க autoRelease_;
};

மாறிலி Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    நிலையான Ptr<Impl> p(புதிய Impl);
    திரும்பு p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(பொய்)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, தருக்க autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(மாறிலி எழுத்து* src, GLenum type, தருக்க autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    எனில் (!status) {
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
    எனில் (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(மாறிலி எழுத்து* src, ShaderType type, தருக்க autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(src, type, autoRelease));
#endif
}

வெறுமை cv::ogl::Shader::create(மாறிலி எழுத்து* src, ShaderType type, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(src, type, autoRelease));
    type_ = type;
#endif
}

வெறுமை cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    எனில் (impl_)
        impl_->setAutoRelease(உண்மை);
    impl_ = Impl::empty();
#endif
}

வெறுமை cv::ogl::Shader::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    impl_->setAutoRelease(flag);
#endif
}

unsigned முழு cv::ogl::Shader::shaderId() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() மாறிலி
{
    திரும்பு type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

வகுப்பு cv::ogl::Program::Impl
{
};

#இல்லையெனில்

வகுப்பு cv::ogl::Program::Impl
{
பொது:
    நிலையான மாறிலி Ptr<Impl>& empty();

    Impl(GLuint programId, தருக்க autoRelease);
    Impl(மாறிலி Shader& vert, மாறிலி Shader& frag, தருக்க autoRelease);
    ~Impl();

    வெறுமை bind();

    முழு getAttributeLocation(மாறிலி எழுத்து* name) மாறிலி;
    முழு getUniformLocation(மாறிலி எழுத்து* name) மாறிலி;

    வெறுமை setAutoRelease(தருக்க flag) { autoRelease_ = flag; }

    GLuint programId() மாறிலி { திரும்பு programId_; }

தனிப்பட்ட:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    தருக்க autoRelease_;
};

மாறிலி Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    நிலையான Ptr<Impl> p(புதிய Impl);
    திரும்பு p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(பொய்)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, தருக்க autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(மாறிலி Shader& vert, மாறிலி Shader& frag, தருக்க autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    எனில் (!status) {
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
    எனில் (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

வெறுமை cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

முழு cv::ogl::Program::Impl::getAttributeLocation(மாறிலி எழுத்து* name) மாறிலி
{
    // -1 if absent (e.g. optimized out); caller decides.
    முழு location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    திரும்பு location;
}

முழு cv::ogl::Program::Impl::getUniformLocation(மாறிலி எழுத்து* name) மாறிலி
{
    // -1 if absent (e.g. optimized out); caller decides.
    முழு location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    திரும்பு location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(மாறிலி Shader& vert, மாறிலி Shader& frag, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(vert, frag, autoRelease));
#endif
}

வெறுமை cv::ogl::Program::create(மாறிலி Shader& vert, மாறிலி Shader& frag, தருக்க autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#இல்லையெனில்
    impl_.reset(புதிய Impl(vert, frag, autoRelease));
#endif
}

வெறுமை cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    எனில் (impl_)
        impl_->setAutoRelease(உண்மை);
    impl_ = Impl::empty();
#endif
}

வெறுமை cv::ogl::Program::setAutoRelease(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    impl_->setAutoRelease(flag);
#endif
}

வெறுமை cv::ogl::Program::bind() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    impl_->bind();
#endif
}

வெறுமை cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    gl::UseProgram(0);
#endif
}

முழு cv::ogl::Program::getAttributeLocation(மாறிலி எழுத்து* name) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->getAttributeLocation(name);
#endif
}

முழு cv::ogl::Program::getUniformLocation(மாறிலி எழுத்து* name) மாறிலி
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->getUniformLocation(name);
#endif
}

வெறுமை cv::ogl::Program::setUniformVec3(முழு loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#இல்லையெனில்
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

வெறுமை cv::ogl::Program::setUniformVec2(முழு loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#இல்லையெனில்
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

வெறுமை cv::ogl::Program::setUniformVec4(முழு loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#இல்லையெனில்
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

வெறுமை cv::ogl::Program::setUniformMat4x4(முழு loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#இல்லையெனில்
    gl::UniformMatrix4fv(loc, 1, உண்மை, mat.val);
#endif
}

வெறுமை cv::ogl::Program::setUniform1f(முழு loc, தசம value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#இல்லையெனில்
    gl::Uniform1f(loc, value);
#endif
}

வெறுமை cv::ogl::Program::setUniform1i(முழு loc, முழு value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#இல்லையெனில்
    gl::Uniform1i(loc, value);
#endif
}

unsigned முழு cv::ogl::Program::programId() மாறிலி
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#இல்லையெனில்
    திரும்பு impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

வெறுமை cv::ogl::render(மாறிலி ogl::Texture2D& tex, Rect_<இரட்டை> wndRect, Rect_<இரட்டை> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#இல்லையெனில்
    எனில் (!tex.empty())
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

        மாறிலி இரட்டை vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        மாறிலி இரட்டை texCoords[] =
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

வெறுமை cv::ogl::render(மாறிலி ogl::Arrays& arr, முழு mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#இல்லையெனில்
    எனில் (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

வெறுமை cv::ogl::render(மாறிலி ogl::Arrays& arr, உள்ளீடு_Array indices, முழு mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#இல்லையெனில்
    எனில் (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        மாறிலி முழு kind = indices.kind();

        தேர்வு (kind)
        {
        நிலை உள்ளீடு_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                மாறிலி முழு depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                எனில் (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                இல்லையெனில் எனில் (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                இல்லையெனில்
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                நிறுத்து;
            }

        இயல்பு:
            {
                Mat mat = indices.getMat();

                மாறிலி முழு depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                எனில் (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                இல்லையெனில் எனில் (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                இல்லையெனில்
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

வெறுமை cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#இல்லையெனில்
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

வெறுமை cv::ogl::clearDepth(தசம depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#இல்லையெனில்
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

வெறுமை cv::ogl::drawArrays(முழு first, முழு மொத்தம்_2, முழு mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(மொத்தம்_2); CV_UNUSED(mode);
    throw_no_ogl();
#இல்லையெனில்
    gl::DrawArrays(mode, first, மொத்தம்_2);
#endif // HAVE_OPENGL
}

வெறுமை cv::ogl::drawElements(முழு first, முழு மொத்தம்_2, முழு type, முழு mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(மொத்தம்_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#இல்லையெனில்
    gl::DrawElements(mode, மொத்தம்_2, type, reinterpret_cast<மாறிலி வெறுமை*>(static_cast<size_t>(first)));
#endif
}

வெறுமை cv::ogl::drawElementsInstanced(முழு first, முழு மொத்தம்_2, முழு type, முழு mode, முழு instance_மொத்தம்)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(மொத்தம்_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_மொத்தம்);
    throw_no_ogl();
#இல்லையெனில்
    gl::DrawElementsInstanced(mode, மொத்தம்_2, type, reinterpret_cast<மாறிலி வெறுமை*>(static_cast<size_t>(first)), instance_மொத்தம்);
    CV_CheckGlError();
#endif
}

வெறுமை cv::ogl::blendFunc(முழு sfactor, முழு dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#இல்லையெனில்
    gl::BlendFunc(sfactor, dfactor);
#endif
}

வெறுமை cv::ogl::depthMask(தருக்க flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#இல்லையெனில்
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

வெறுமை cv::ogl::enable(முழு cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#இல்லையெனில்
    gl::Enable(cap);
#endif
}

வெறுமை cv::ogl::disable(முழு cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#இல்லையெனில்
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
#  இல்லையெனில்
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#இல்லையெனில் // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#எனில் defined(HAVE_OPENGL)
#  எனில் defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

நாமாகாஶ cv { நாமாகாஶ ogl {

#எனில் defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
நிலையான வெறுமை checkOpenCLVersion()
{
    using நாமாகாஶ cv::ocl;
    மாறிலி Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, அளவு(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == அளவு(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    முழு versionMajor = pi.versionMajor();
    முழு versionMinor = pi.versionMinor();
    எனில் (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

நாமாகாஶ ocl {

Context& initializeContextFromGL()
{
#எனில் !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#இல்லையெனில்
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, வெற்று, &platformsCnt);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    எனில் (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    தருக்க sharingSupported = பொய்;

    க்கு (unsigned முழு i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, வெற்று, &devCnt);
        எனில் (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        முயல் {
            devices = புதிய cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, வெற்று);
            எனில் (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            க்கு (unsigned முழு j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, வெற்று, &extensionSize );
                எனில் (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                எனில்(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    எனில் (status != CL_SUCCESS)
                        தொடர்;
                    எனில் (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    போது (spacePos != devString.npos) {
                        எனில் (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = உண்மை;
                            நிறுத்து;
                        }
                        செய் {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } போது (spacePos == oldPos);
                    }
                }
            }
        } பிடி(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            எனில்(devices != nullptr) {
                delete[] devices;
            }
            தொடர்;
        }

        எனில் (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#எனில் defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, வெற்று, வெற்று, &ci_Err_இலக்கம்);
#அல்லதெனில் defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], வெற்று, வெற்று, &status);
#அல்லதெனில் defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], வெற்று, வெற்று, &status);
#அல்லதெனில் defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], வெற்று, வெற்று, &status);
#endif
        }

        எனில் (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        இல்லையெனில்
            நிறுத்து;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    திரும்பு const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

வெறுமை convertToGLTexture2D(உள்ளீடு_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#எனில் !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#இல்லையெனில்
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (முழு)texture.cols() && srcSize.height == (முழு)texture.rows());

    using நாமாகாஶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

வெறுமை convertFromGLTexture2D(மாறிலி Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#எனில் !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#இல்லையெனில்
    // check texture format
    மாறிலி முழு dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    முழு textureType = dtype;
    CV_Assert(textureType >= 0);

    using நாமாகாஶ cv::ocl;
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
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(மாறிலி Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#எனில் !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#இல்லையெனில்
    using நாமாகாஶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    முழு clAccessFlags = 0;
    தேர்வு (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    இயல்பு:
    நிலை ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        நிறுத்து;
    நிலை ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        நிறுத்து;
    நிலை ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        நிறுத்து;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    முழு rows = buffer.rows();
    முழு cols = buffer.cols();
    முழு type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    திரும்பு u;
#endif
}

வெறுமை unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#எனில் !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#இல்லையெனில்
    using நாமாகாஶ cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    எனில் (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
