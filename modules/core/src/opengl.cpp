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

#ضمن "precomp.hpp"

#إذا defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#وإلا
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    إذا (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#وإلا // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using نطاق cv;
using نطاق cv::cuda;

#إذا defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

نطاق
{
#ifndef HAVE_OPENGL
مضمن ساكن CV_NORETURN فراغ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#وإلاإذا defined _DEBUG
مضمن ساكن منطقي checkError(ثابت حرف* file, ثابت صحيح لي, ثابت حرف* func = 0)
{
    GLenum err = gl::GetError();
    إذا (err != gl::NO_ERROR_)
    {
        ثابت حرف* سكو;
        بدل (err)
        {
        حالة gl::INVALID_ENUM:
            سكو = "An unacceptable value is specified for an enumerated argument";
            اكسر;
        حالة gl::INVALID_درج:
            سكو = "A numeric argument is out of range";
            اكسر;
        حالة gl::INVALID_OPERATION:
            سكو = "The specified operation is not allowed in the current state";
            اكسر;
        حالة gl::OUT_OF_MEMORY:
            سكو = "There is not enough memory left to execute the command";
            اكسر;
        افتراضي:
            سكو = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, سكو, file, لي);
    }
    أرجع true;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
نطاق
{
    ثابت GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

فراغ cv::cuda::setGlDevice(صحيح device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #وإلا
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#إذا defined(HAVE_OPENGL) && defined(HAVE_CUDA)

نطاق
{
    صنف CudaResource
    {
    عام:
        CudaResource();
        ~CudaResource();

        فراغ registerBuffer(GLuint buffer);
        فراغ release();

        فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        فراغ copyTo(فراغ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        فراغ* map(cudaStream_t stream = 0);
        فراغ unmap(cudaStream_t stream = 0);

    خاص:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        صنف GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    فراغ CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        إذا (buffer_ == buffer)
            أرجع;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    فراغ CudaResource::release()
    {
        إذا (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    صنف CudaResource::GraphicsMapHolder
    {
    عام:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        فراغ reset();

    خاص:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        إذا (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        إذا (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    فراغ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    فراغ CudaResource::copyFrom(ثابت فراغ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        فراغ* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        إذا (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        وإلا
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    فراغ CudaResource::copyTo(فراغ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        فراغ* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        إذا (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        وإلا
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    فراغ* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        فراغ* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        أرجع ptr;
    }

    فراغ CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

صنف cv::ogl::Buffer::Impl
{
};

#وإلا

صنف cv::ogl::Buffer::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint bufId, منطقي autoRelease);
    Impl(GLsizeiptr size, ثابت GLvoid* data, GLenum target, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLenum target) ثابت;

    فراغ copyFrom(GLuint srcBuf, GLsizeiptr size);

    فراغ copyFrom(GLsizeiptr size, ثابت GLvoid* data);
    فراغ copyTo(GLsizeiptr size, GLvoid* data) ثابت;

    فراغ resize(GLsizeiptr size, GLenum target);

    فراغ* mapHost(GLenum access);
    فراغ unmapHost();

#ifdef HAVE_CUDA
    فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    فراغ copyTo(فراغ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) ثابت;

    فراغ* mapDevice(cudaStream_t stream = 0);
    فراغ unmapDevice(cudaStream_t stream = 0);
#endif

    فراغ setAutoRelease(منطقي flag) { autoRelease_ = flag; }

    GLuint bufId() ثابت { أرجع bufId_; }

خاص:
    Impl();

    GLuint bufId_;
    منطقي autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

ثابت Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(خطأ)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, منطقي autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, ثابت GLvoid* data, GLenum target, منطقي autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    إذا (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

فراغ cv::ogl::Buffer::Impl::bind(GLenum target) ثابت
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

فراغ cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

فراغ cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, ثابت GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

فراغ cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

فراغ cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) ثابت
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

فراغ* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    أرجع data;
}

فراغ cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

فراغ cv::ogl::Buffer::Impl::copyFrom(ثابت فراغ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

فراغ cv::ogl::Buffer::Impl::copyTo(فراغ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) ثابت
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

فراغ* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    أرجع cudaResource_.map(stream);
}

فراغ cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(صحيح arows, صحيح acols, صحيح atype, unsigned صحيح abufId, منطقي autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, صحيح atype, unsigned صحيح abufId, منطقي autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(شغرو_Array arr, Target target, منطقي autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح kind = arr.kind();

    بدل (kind)
    {
    حالة شغرو_Array_2::OPENGL_BUFFER:
    حالة شغرو_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        اكسر;

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ثابت GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(جديد Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            اكسر;
        }
    }
#endif
}

فراغ cv::ogl::Buffer::create(صحيح arows, صحيح acols, صحيح atype, Target target, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (rows_ != arows || cols_ != acols || type_ != atype)
    {
        ثابت GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        إذا (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        وإلا
            impl_.reset(جديد Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

فراغ cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    إذا (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

فراغ cv::ogl::Buffer::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    impl_->setAutoRelease(flag);
#endif
}

فراغ cv::ogl::Buffer::copyFrom(شغرو_Array arr, Target target, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح kind = arr.kind();

    ثابت Size asize = arr.size();
    ثابت صحيح atype = arr.type();
    create(asize, atype, target, autoRelease);

    بدل (kind)
    {
    حالة شغرو_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            اكسر;
        }

    حالة شغرو_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

فراغ cv::ogl::Buffer::copyFrom(شغرو_Array arr, cuda::Stream& stream, Target target, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #وإلا
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

فراغ cv::ogl::Buffer::copyTo(OutputArray arr) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#وإلا
    ثابت صحيح kind = arr.kind();

    بدل (kind)
    {
    حالة شغرو_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*هذا);
            اكسر;
        }

    حالة شغرو_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            اكسر;
        }

    افتراضي:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

فراغ cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #وإلا
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, منطقي autoRelease) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    ogl::Buffer buf;
    buf.copyFrom(*هذا, target, autoRelease);
    أرجع buf;
#endif
}

فراغ cv::ogl::Buffer::bind(Target target) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#وإلا
    impl_->bind(target);
#endif
}

فراغ cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#وإلا
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#وإلا
    أرجع Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

فراغ cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

فراغ cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

فراغ cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#وإلا
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned صحيح cv::ogl::Buffer::bufId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

صنف cv::ogl::Texture2D::Impl
{
};

#وإلا

صنف cv::ogl::Texture2D::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl(GLuint texId, منطقي autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease);
    ~Impl();

    فراغ copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid *pixels);
    فراغ copyTo(GLenum format, GLenum type, GLvoid* pixels) ثابت;

    فراغ bind() ثابت;

    فراغ setAutoRelease(منطقي flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { أرجع texId_; }

خاص:
    Impl();

    GLuint texId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(خطأ)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, منطقي autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ثابت GLvoid *pixels)
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

فراغ cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) ثابت
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

فراغ cv::ogl::Texture2D::Impl::bind() ثابت
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(صحيح arows, صحيح acols, Format aformat, unsigned صحيح atexId, منطقي autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned صحيح atexId, منطقي autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(شغرو_Array arr, منطقي autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح kind = arr.kind();

    ثابت Size asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح depth = CV_MAT_DEPTH(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

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

    بدل (kind)
    {
    حالة شغرو_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(جديد Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            اكسر;
        }

    حالة شغرو_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(true);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(جديد Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(جديد Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            اكسر;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

فراغ cv::ogl::Texture2D::create(صحيح arows, صحيح acols, Format aformat, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(جديد Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

فراغ cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    إذا (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

فراغ cv::ogl::Texture2D::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    impl_->setAutoRelease(flag);
#endif
}

فراغ cv::ogl::Texture2D::copyFrom(شغرو_Array arr, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح kind = arr.kind();

    ثابت Size asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح depth = CV_MAT_DEPTH(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

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

    بدل(kind)
    {
    حالة شغرو_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            اكسر;
        }

    حالة شغرو_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(true);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

فراغ cv::ogl::Texture2D::copyTo(OutputArray arr, صحيح ddepth, منطقي autoRelease) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح kind = arr.kind();

    ثابت صحيح cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ثابت GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    بدل(kind)
    {
    حالة شغرو_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            اكسر;
        }

    حالة شغرو_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #وإلا
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(true);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            اكسر;
        }

    افتراضي:
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

فراغ cv::ogl::Texture2D::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#endif
}

unsigned صحيح cv::ogl::Texture2D::texId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

صنف cv::ogl::TextureBuffer::Impl
{
};

#وإلا

صنف cv::ogl::TextureBuffer::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLint unit) ثابت;

    فراغ setAutoRelease(منطقي flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { أرجع texId_; }

خاص:
    GLuint texId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    ساكن Ptr<Impl> p = makePtr<Impl>();
    أرجع p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(خطأ)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::TextureBuffer::Impl::bind(GLint unit) ثابت
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
#وإلا
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(ثابت Buffer& buf, Format aformat, منطقي autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

فراغ cv::ogl::TextureBuffer::create(ثابت Buffer& buf, Format aformat, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

فراغ cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    إذا (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

فراغ cv::ogl::TextureBuffer::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    impl_->setAutoRelease(flag);
#endif
}

فراغ cv::ogl::TextureBuffer::bind(صحيح unit) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#وإلا
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() ثابت
{
    أرجع format_;
}

منطقي cv::ogl::TextureBuffer::empty() ثابت
{
    أرجع format_ == NONE;
}

unsigned صحيح cv::ogl::TextureBuffer::texId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

فراغ cv::ogl::Arrays::setVertexArray(شغرو_Array vertex)
{
    ثابت صحيح cn = vertex.channels();
    ثابت صحيح depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    إذا (vertex.kind() == شغرو_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    وإلا
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

فراغ cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

فراغ cv::ogl::Arrays::setColorArray(شغرو_Array color)
{
    ثابت صحيح cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    إذا (color.kind() == شغرو_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    وإلا
        color_.copyFrom(color);
}

فراغ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

فراغ cv::ogl::Arrays::setNormalArray(شغرو_Array normal)
{
    ثابت صحيح cn = normal.channels();
    ثابت صحيح depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    إذا (normal.kind() == شغرو_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    وإلا
        normal_.copyFrom(normal);
}

فراغ cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

فراغ cv::ogl::Arrays::setTexCoordArray(شغرو_Array texCoord)
{
    ثابت صحيح cn = texCoord.channels();
    ثابت صحيح depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    إذا (texCoord.kind() == شغرو_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    وإلا
        texCoord_.copyFrom(texCoord);
}

فراغ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

فراغ cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

فراغ cv::ogl::Arrays::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

فراغ cv::ogl::Arrays::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    إذا (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    إذا (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    إذا (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        ثابت صحيح cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    إذا (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    وإلا
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

صنف cv::ogl::VertexArray::Impl
{
};

#وإلا

صنف cv::ogl::VertexArray::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint vaId, منطقي autoRelease);
    Impl(std::initializer_list<Attribute> attributes, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    فراغ setAutoRelease(منطقي flag) { autoRelease_ = flag; }

    GLuint vaId() ثابت { أرجع vaId_; }

خاص:
    Impl();

    GLuint vaId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(خطأ)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, منطقي autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, منطقي autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    لكل (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        إذا (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        وإلا
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    إذا (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

فراغ cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(attributes, autoRelease));
#endif
}

فراغ cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(attributes, autoRelease));
#endif
}

فراغ cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    إذا (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#endif
}

فراغ cv::ogl::VertexArray::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    impl_->setAutoRelease(flag);
#endif
}

فراغ cv::ogl::VertexArray::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#endif
}

فراغ cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::BindVertexArray(0);
#endif
}

unsigned صحيح cv::ogl::VertexArray::vaId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

صنف cv::ogl::Shader::Impl
{
};

#وإلا

صنف cv::ogl::Shader::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint shaderId, منطقي autoRelease);
    Impl(ثابت حرف* src, GLenum type, منطقي autoRelease);
    ~Impl();

    فراغ setAutoRelease(منطقي flag) { autoRelease_ = flag; }

    GLuint shaderId() ثابت { أرجع shaderId_; }

خاص:
    Impl();

    GLuint shaderId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(خطأ)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, منطقي autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ثابت حرف* src, GLenum type, منطقي autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    إذا (!status) {
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
    إذا (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(ثابت حرف* src, ShaderType type, منطقي autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
#endif
}

فراغ cv::ogl::Shader::create(ثابت حرف* src, ShaderType type, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
    type_ = type;
#endif
}

فراغ cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    إذا (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#endif
}

فراغ cv::ogl::Shader::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    impl_->setAutoRelease(flag);
#endif
}

unsigned صحيح cv::ogl::Shader::shaderId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ثابت
{
    أرجع type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

صنف cv::ogl::Program::Impl
{
};

#وإلا

صنف cv::ogl::Program::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint programId, منطقي autoRelease);
    Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    صحيح getAttributeLocation(ثابت حرف* name) ثابت;
    صحيح getUniformLocation(ثابت حرف* name) ثابت;

    فراغ setAutoRelease(منطقي flag) { autoRelease_ = flag; }

    GLuint programId() ثابت { أرجع programId_; }

خاص:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(خطأ)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, منطقي autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    إذا (!status) {
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
    إذا (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

فراغ cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

صحيح cv::ogl::Program::Impl::getAttributeLocation(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    أرجع location;
}

صحيح cv::ogl::Program::Impl::getUniformLocation(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    أرجع location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#endif
}

فراغ cv::ogl::Program::create(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#endif
}

فراغ cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    إذا (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#endif
}

فراغ cv::ogl::Program::setAutoRelease(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    impl_->setAutoRelease(flag);
#endif
}

فراغ cv::ogl::Program::bind() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#endif
}

فراغ cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::UseProgram(0);
#endif
}

صحيح cv::ogl::Program::getAttributeLocation(ثابت حرف* name) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->getAttributeLocation(name);
#endif
}

صحيح cv::ogl::Program::getUniformLocation(ثابت حرف* name) ثابت
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->getUniformLocation(name);
#endif
}

فراغ cv::ogl::Program::setUniformVec3(صحيح loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

فراغ cv::ogl::Program::setUniformVec2(صحيح loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

فراغ cv::ogl::Program::setUniformVec4(صحيح loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

فراغ cv::ogl::Program::setUniformMat4x4(صحيح loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#وإلا
    gl::UniformMatrix4fv(loc, 1, true, mat.val);
#endif
}

فراغ cv::ogl::Program::setUniform1f(صحيح loc, عائم value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1f(loc, value);
#endif
}

فراغ cv::ogl::Program::setUniform1i(صحيح loc, صحيح value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1i(loc, value);
#endif
}

unsigned صحيح cv::ogl::Program::programId() ثابت
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

فراغ cv::ogl::render(ثابت ogl::Texture2D& tex, Rect_<مزدوج> wndRect, Rect_<مزدوج> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#وإلا
    إذا (!tex.empty())
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

        ثابت مزدوج vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        ثابت مزدوج texCoords[] =
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

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, صحيح mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, شغرو_Array indices, صحيح mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ثابت صحيح kind = indices.kind();

        بدل (kind)
        {
        حالة شغرو_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                ثابت صحيح depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                إذا (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                اكسر;
            }

        افتراضي:
            {
                Mat mat = indices.getMat();

                ثابت صحيح depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                إذا (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

فراغ cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#وإلا
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

فراغ cv::ogl::clearDepth(عائم depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#وإلا
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

فراغ cv::ogl::drawArrays(صحيح first, صحيح كدي_2, صحيح mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(كدي_2); CV_UNUSED(mode);
    throw_no_ogl();
#وإلا
    gl::DrawArrays(mode, first, كدي_2);
#endif // HAVE_OPENGL
}

فراغ cv::ogl::drawElements(صحيح first, صحيح كدي_2, صحيح type, صحيح mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(كدي_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#وإلا
    gl::DrawElements(mode, كدي_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)));
#endif
}

فراغ cv::ogl::drawElementsInstanced(صحيح first, صحيح كدي_2, صحيح type, صحيح mode, صحيح instance_كدي)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(كدي_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_كدي);
    throw_no_ogl();
#وإلا
    gl::DrawElementsInstanced(mode, كدي_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)), instance_كدي);
    CV_CheckGlError();
#endif
}

فراغ cv::ogl::blendFunc(صحيح sfactor, صحيح dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#وإلا
    gl::BlendFunc(sfactor, dfactor);
#endif
}

فراغ cv::ogl::depthMask(منطقي flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#وإلا
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

فراغ cv::ogl::enable(صحيح cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#وإلا
    gl::Enable(cap);
#endif
}

فراغ cv::ogl::disable(صحيح cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#وإلا
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
#  وإلا
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#وإلا // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#إذا defined(HAVE_OPENGL)
#  إذا defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

نطاق cv { نطاق ogl {

#إذا defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ساكن فراغ checkOpenCLVersion()
{
    using نطاق cv::ocl;
    ثابت Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, حجم(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == حجم(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    صحيح versionMajor = pi.versionMajor();
    صحيح versionMinor = pi.versionMinor();
    إذا (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

نطاق ocl {

Context& initializeContextFromGL()
{
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, عدم, &platformsCnt);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    إذا (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    منطقي sharingSupported = خطأ;

    لكل (unsigned صحيح i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, عدم, &devCnt);
        إذا (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        حاول {
            devices = جديد cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, عدم);
            إذا (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            لكل (unsigned صحيح j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, عدم, &extensionSize );
                إذا (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                إذا(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    إذا (status != CL_SUCCESS)
                        استمر;
                    إذا (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    طالما (spacePos != devString.npos) {
                        إذا (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = true;
                            اكسر;
                        }
                        افعل {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } طالما (spacePos == oldPos);
                    }
                }
            }
        } امسك(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            إذا(devices != nullptr) {
                delete[] devices;
            }
            استمر;
        }

        إذا (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#إذا defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, عدم, عدم, &ci_Err_لمب);
#وإلاإذا defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], عدم, عدم, &status);
#وإلاإذا defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], عدم, عدم, &status);
#وإلاإذا defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], عدم, عدم, &status);
#endif
        }

        إذا (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        وإلا
            اكسر;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    أرجع const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

فراغ convertToGLTexture2D(شغرو_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (صحيح)texture.cols() && srcSize.height == (صحيح)texture.rows());

    using نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

فراغ convertFromGLTexture2D(ثابت Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    // check texture format
    ثابت صحيح dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    صحيح textureType = dtype;
    CV_Assert(textureType >= 0);

    using نطاق cv::ocl;
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
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ثابت Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    using نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    صحيح clAccessFlags = 0;
    بدل (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    افتراضي:
    حالة ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        اكسر;
    حالة ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        اكسر;
    حالة ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        اكسر;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    صحيح rows = buffer.rows();
    صحيح cols = buffer.cols();
    صحيح type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    أرجع u;
#endif
}

فراغ unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    using نطاق cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, عدم, عدم);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    إذا (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
