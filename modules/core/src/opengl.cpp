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

#अन्तर्भाव "precomp.hpp"

#यदि defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#अन्यथा
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    यदि (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#अन्यथा // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using नामाकाश cv;
using नामाकाश cv::cuda;

#यदि defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

नामाकाश
{
#ifndef HAVE_OPENGL
अन्तःस्थ स्थिर CV_NORETURN शून्य throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#नोचेद्यदि defined _DEBUG
अन्तःस्थ स्थिर तार्किक checkError(ध्रुव वर्ण* file, ध्रुव पूर्णाङ्क पंक्ति, ध्रुव वर्ण* func = 0)
{
    GLenum err = gl::GetError();
    यदि (err != gl::NO_ERROR_)
    {
        ध्रुव वर्ण* संदेश;
        विकल्प (err)
        {
        पक्ष gl::INVALID_ENUM:
            संदेश = "An unacceptable value is specified for an enumerated argument";
            भङ्ग;
        पक्ष gl::INVALID_मूल्य:
            संदेश = "A numeric argument is out of range";
            भङ्ग;
        पक्ष gl::INVALID_OPERATION:
            संदेश = "The specified operation is not allowed in the current state";
            भङ्ग;
        पक्ष gl::OUT_OF_MEMORY:
            संदेश = "There is not enough memory left to execute the command";
            भङ्ग;
        स्वभाव:
            संदेश = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, संदेश, file, पंक्ति);
    }
    प्रत्यागम सत्य;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
नामाकाश
{
    ध्रुव GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

शून्य cv::cuda::setGlDevice(पूर्णाङ्क device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #अन्यथा
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#यदि defined(HAVE_OPENGL) && defined(HAVE_CUDA)

नामाकाश
{
    वर्ग CudaResource
    {
    सार्वजनिक:
        CudaResource();
        ~CudaResource();

        शून्य registerBuffer(GLuint buffer);
        शून्य release();

        शून्य copyFrom(ध्रुव शून्य* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        शून्य copyTo(शून्य* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        शून्य* map(cudaStream_t stream = 0);
        शून्य unmap(cudaStream_t stream = 0);

    गुप्त:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        वर्ग GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    शून्य CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        यदि (buffer_ == buffer)
            प्रत्यागम;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    शून्य CudaResource::release()
    {
        यदि (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    वर्ग CudaResource::GraphicsMapHolder
    {
    सार्वजनिक:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        शून्य reset();

    गुप्त:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        यदि (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        यदि (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    शून्य CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    शून्य CudaResource::copyFrom(ध्रुव शून्य* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        शून्य* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        यदि (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        अन्यथा
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    शून्य CudaResource::copyTo(शून्य* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        शून्य* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        यदि (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        अन्यथा
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    शून्य* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        शून्य* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        प्रत्यागम ptr;
    }

    शून्य CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

वर्ग cv::ogl::Buffer::Impl
{
};

#अन्यथा

वर्ग cv::ogl::Buffer::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint bufId, तार्किक autoRelease);
    Impl(GLsizeiptr size, ध्रुव GLvoid* data, GLenum target, तार्किक autoRelease);
    ~Impl();

    शून्य bind(GLenum target) ध्रुव;

    शून्य copyFrom(GLuint srcBuf, GLsizeiptr size);

    शून्य copyFrom(GLsizeiptr size, ध्रुव GLvoid* data);
    शून्य copyTo(GLsizeiptr size, GLvoid* data) ध्रुव;

    शून्य resize(GLsizeiptr size, GLenum target);

    शून्य* mapHost(GLenum access);
    शून्य unmapHost();

#ifdef HAVE_CUDA
    शून्य copyFrom(ध्रुव शून्य* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    शून्य copyTo(शून्य* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) ध्रुव;

    शून्य* mapDevice(cudaStream_t stream = 0);
    शून्य unmapDevice(cudaStream_t stream = 0);
#endif

    शून्य setAutoRelease(तार्किक flag) { autoRelease_ = flag; }

    GLuint bufId() ध्रुव { प्रत्यागम bufId_; }

गुप्त:
    Impl();

    GLuint bufId_;
    तार्किक autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

ध्रुव Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(असत्य)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, तार्किक autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, ध्रुव GLvoid* data, GLenum target, तार्किक autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    यदि (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

शून्य cv::ogl::Buffer::Impl::bind(GLenum target) ध्रुव
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

शून्य cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

शून्य cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, ध्रुव GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

शून्य cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

शून्य cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) ध्रुव
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

शून्य* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    प्रत्यागम data;
}

शून्य cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

शून्य cv::ogl::Buffer::Impl::copyFrom(ध्रुव शून्य* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

शून्य cv::ogl::Buffer::Impl::copyTo(शून्य* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) ध्रुव
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

शून्य* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    प्रत्यागम cudaResource_.map(stream);
}

शून्य cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(पूर्णाङ्क arows, पूर्णाङ्क acols, पूर्णाङ्क atype, unsigned पूर्णाङ्क abufId, तार्किक autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, पूर्णाङ्क atype, unsigned पूर्णाङ्क abufId, तार्किक autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(निवेश_Array arr, Target target, तार्किक autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क kind = arr.kind();

    विकल्प (kind)
    {
    पक्ष निवेश_Array_2::OPENGL_BUFFER:
    पक्ष निवेश_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        भङ्ग;

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ध्रुव GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(नूतन Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            भङ्ग;
        }
    }
#endif
}

शून्य cv::ogl::Buffer::create(पूर्णाङ्क arows, पूर्णाङ्क acols, पूर्णाङ्क atype, Target target, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    यदि (rows_ != arows || cols_ != acols || type_ != atype)
    {
        ध्रुव GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        यदि (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        अन्यथा
            impl_.reset(नूतन Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

शून्य cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    यदि (impl_)
        impl_->setAutoRelease(सत्य);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

शून्य cv::ogl::Buffer::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#endif
}

शून्य cv::ogl::Buffer::copyFrom(निवेश_Array arr, Target target, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क kind = arr.kind();

    ध्रुव Size asize = arr.size();
    ध्रुव पूर्णाङ्क atype = arr.type();
    create(asize, atype, target, autoRelease);

    विकल्प (kind)
    {
    पक्ष निवेश_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            भङ्ग;
        }

    पक्ष निवेश_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            भङ्ग;
        }

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

शून्य cv::ogl::Buffer::copyFrom(निवेश_Array arr, cuda::Stream& stream, Target target, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #अन्यथा
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

शून्य cv::ogl::Buffer::copyTo(OutputArray arr) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क kind = arr.kind();

    विकल्प (kind)
    {
    पक्ष निवेश_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*एतत्);
            भङ्ग;
        }

    पक्ष निवेश_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            भङ्ग;
        }

    स्वभाव:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

शून्य cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #अन्यथा
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, तार्किक autoRelease) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ogl::Buffer buf;
    buf.copyFrom(*एतत्, target, autoRelease);
    प्रत्यागम buf;
#endif
}

शून्य cv::ogl::Buffer::bind(Target target) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#अन्यथा
    impl_->bind(target);
#endif
}

शून्य cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#अन्यथा
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#अन्यथा
    प्रत्यागम Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

शून्य cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #अन्यथा
        प्रत्यागम GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

शून्य cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #अन्यथा
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #अन्यथा
        प्रत्यागम GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

शून्य cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#अन्यथा
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #अन्यथा
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned पूर्णाङ्क cv::ogl::Buffer::bufId() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

वर्ग cv::ogl::Texture2D::Impl
{
};

#अन्यथा

वर्ग cv::ogl::Texture2D::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl> empty();

    Impl(GLuint texId, तार्किक autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid* pixels, तार्किक autoRelease);
    ~Impl();

    शून्य copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid *pixels);
    शून्य copyTo(GLenum format, GLenum type, GLvoid* pixels) ध्रुव;

    शून्य bind() ध्रुव;

    शून्य setAutoRelease(तार्किक flag) { autoRelease_ = flag; }

    GLuint texId() ध्रुव { प्रत्यागम texId_; }

गुप्त:
    Impl();

    GLuint texId_;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(असत्य)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, तार्किक autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid* pixels, तार्किक autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    यदि (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

शून्य cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
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

शून्य cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) ध्रुव
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

शून्य cv::ogl::Texture2D::Impl::bind() ध्रुव
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(पूर्णाङ्क arows, पूर्णाङ्क acols, Format aformat, unsigned पूर्णाङ्क atexId, तार्किक autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned पूर्णाङ्क atexId, तार्किक autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(निवेश_Array arr, तार्किक autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क kind = arr.kind();

    ध्रुव Size asize = arr.size();
    ध्रुव पूर्णाङ्क atype = arr.type();

    ध्रुव पूर्णाङ्क depth = CV_MAT_DEPTH(atype);
    ध्रुव पूर्णाङ्क cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ध्रुव Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ध्रुव GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    विकल्प (kind)
    {
    पक्ष निवेश_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(नूतन Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            भङ्ग;
        }

    पक्ष निवेश_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(सत्य);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(नूतन Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            भङ्ग;
        }

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(नूतन Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            भङ्ग;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

शून्य cv::ogl::Texture2D::create(पूर्णाङ्क arows, पूर्णाङ्क acols, Format aformat, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    यदि (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(नूतन Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

शून्य cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    यदि (impl_)
        impl_->setAutoRelease(सत्य);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

शून्य cv::ogl::Texture2D::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#endif
}

शून्य cv::ogl::Texture2D::copyFrom(निवेश_Array arr, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क kind = arr.kind();

    ध्रुव Size asize = arr.size();
    ध्रुव पूर्णाङ्क atype = arr.type();

    ध्रुव पूर्णाङ्क depth = CV_MAT_DEPTH(atype);
    ध्रुव पूर्णाङ्क cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ध्रुव Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ध्रुव GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    विकल्प(kind)
    {
    पक्ष निवेश_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            भङ्ग;
        }

    पक्ष निवेश_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(सत्य);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            भङ्ग;
        }

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

शून्य cv::ogl::Texture2D::copyTo(OutputArray arr, पूर्णाङ्क ddepth, तार्किक autoRelease) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क kind = arr.kind();

    ध्रुव पूर्णाङ्क cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ध्रुव GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    विकल्प(kind)
    {
    पक्ष निवेश_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            भङ्ग;
        }

    पक्ष निवेश_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(सत्य);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            भङ्ग;
        }

    स्वभाव:
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

शून्य cv::ogl::Texture2D::bind() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#endif
}

unsigned पूर्णाङ्क cv::ogl::Texture2D::texId() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

वर्ग cv::ogl::TextureBuffer::Impl
{
};

#अन्यथा

वर्ग cv::ogl::TextureBuffer::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, तार्किक autoRelease);
    ~Impl();

    शून्य bind(GLint unit) ध्रुव;

    शून्य setAutoRelease(तार्किक flag) { autoRelease_ = flag; }

    GLuint texId() ध्रुव { प्रत्यागम texId_; }

गुप्त:
    GLuint texId_;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    स्थिर Ptr<Impl> p = makePtr<Impl>();
    प्रत्यागम p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(असत्य)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, तार्किक autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    यदि (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

शून्य cv::ogl::TextureBuffer::Impl::bind(GLint unit) ध्रुव
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
#अन्यथा
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(ध्रुव Buffer& buf, Format aformat, तार्किक autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

शून्य cv::ogl::TextureBuffer::create(ध्रुव Buffer& buf, Format aformat, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

शून्य cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    यदि (impl_)
        impl_->setAutoRelease(सत्य);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

शून्य cv::ogl::TextureBuffer::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#endif
}

शून्य cv::ogl::TextureBuffer::bind(पूर्णाङ्क unit) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#अन्यथा
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() ध्रुव
{
    प्रत्यागम format_;
}

तार्किक cv::ogl::TextureBuffer::empty() ध्रुव
{
    प्रत्यागम format_ == NONE;
}

unsigned पूर्णाङ्क cv::ogl::TextureBuffer::texId() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

शून्य cv::ogl::Arrays::setVertexArray(निवेश_Array vertex)
{
    ध्रुव पूर्णाङ्क cn = vertex.channels();
    ध्रुव पूर्णाङ्क depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    यदि (vertex.kind() == निवेश_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    अन्यथा
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

शून्य cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

शून्य cv::ogl::Arrays::setColorArray(निवेश_Array color)
{
    ध्रुव पूर्णाङ्क cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    यदि (color.kind() == निवेश_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    अन्यथा
        color_.copyFrom(color);
}

शून्य cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

शून्य cv::ogl::Arrays::setNormalArray(निवेश_Array normal)
{
    ध्रुव पूर्णाङ्क cn = normal.channels();
    ध्रुव पूर्णाङ्क depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    यदि (normal.kind() == निवेश_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    अन्यथा
        normal_.copyFrom(normal);
}

शून्य cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

शून्य cv::ogl::Arrays::setTexCoordArray(निवेश_Array texCoord)
{
    ध्रुव पूर्णाङ्क cn = texCoord.channels();
    ध्रुव पूर्णाङ्क depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    यदि (texCoord.kind() == निवेश_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    अन्यथा
        texCoord_.copyFrom(texCoord);
}

शून्य cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

शून्य cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

शून्य cv::ogl::Arrays::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

शून्य cv::ogl::Arrays::bind() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    यदि (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    यदि (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    यदि (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        ध्रुव पूर्णाङ्क cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    यदि (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
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

वर्ग cv::ogl::VertexArray::Impl
{
};

#अन्यथा

वर्ग cv::ogl::VertexArray::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint vaId, तार्किक autoRelease);
    Impl(std::initializer_list<Attribute> attributes, तार्किक autoRelease);
    ~Impl();

    शून्य bind();

    शून्य setAutoRelease(तार्किक flag) { autoRelease_ = flag; }

    GLuint vaId() ध्रुव { प्रत्यागम vaId_; }

गुप्त:
    Impl();

    GLuint vaId_;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(असत्य)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, तार्किक autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, तार्किक autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    प्रति (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        यदि (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ध्रुव शून्य*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        अन्यथा
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ध्रुव शून्य*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    यदि (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

शून्य cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(attributes, autoRelease));
#endif
}

शून्य cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(attributes, autoRelease));
#endif
}

शून्य cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    यदि (impl_)
        impl_->setAutoRelease(सत्य);
    impl_ = Impl::empty();
#endif
}

शून्य cv::ogl::VertexArray::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#endif
}

शून्य cv::ogl::VertexArray::bind() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#endif
}

शून्य cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    gl::BindVertexArray(0);
#endif
}

unsigned पूर्णाङ्क cv::ogl::VertexArray::vaId() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

वर्ग cv::ogl::Shader::Impl
{
};

#अन्यथा

वर्ग cv::ogl::Shader::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint shaderId, तार्किक autoRelease);
    Impl(ध्रुव वर्ण* src, GLenum type, तार्किक autoRelease);
    ~Impl();

    शून्य setAutoRelease(तार्किक flag) { autoRelease_ = flag; }

    GLuint shaderId() ध्रुव { प्रत्यागम shaderId_; }

गुप्त:
    Impl();

    GLuint shaderId_;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(असत्य)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, तार्किक autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ध्रुव वर्ण* src, GLenum type, तार्किक autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    यदि (!status) {
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
    यदि (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(ध्रुव वर्ण* src, ShaderType type, तार्किक autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(src, type, autoRelease));
#endif
}

शून्य cv::ogl::Shader::create(ध्रुव वर्ण* src, ShaderType type, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(src, type, autoRelease));
    type_ = type;
#endif
}

शून्य cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    यदि (impl_)
        impl_->setAutoRelease(सत्य);
    impl_ = Impl::empty();
#endif
}

शून्य cv::ogl::Shader::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#endif
}

unsigned पूर्णाङ्क cv::ogl::Shader::shaderId() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ध्रुव
{
    प्रत्यागम type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

वर्ग cv::ogl::Program::Impl
{
};

#अन्यथा

वर्ग cv::ogl::Program::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint programId, तार्किक autoRelease);
    Impl(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease);
    ~Impl();

    शून्य bind();

    पूर्णाङ्क getAttributeLocation(ध्रुव वर्ण* name) ध्रुव;
    पूर्णाङ्क getUniformLocation(ध्रुव वर्ण* name) ध्रुव;

    शून्य setAutoRelease(तार्किक flag) { autoRelease_ = flag; }

    GLuint programId() ध्रुव { प्रत्यागम programId_; }

गुप्त:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(असत्य)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, तार्किक autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    यदि (!status) {
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
    यदि (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

शून्य cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

पूर्णाङ्क cv::ogl::Program::Impl::getAttributeLocation(ध्रुव वर्ण* name) ध्रुव
{
    // -1 if absent (e.g. optimized out); caller decides.
    पूर्णाङ्क location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    प्रत्यागम location;
}

पूर्णाङ्क cv::ogl::Program::Impl::getUniformLocation(ध्रुव वर्ण* name) ध्रुव
{
    // -1 if absent (e.g. optimized out); caller decides.
    पूर्णाङ्क location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    प्रत्यागम location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(vert, frag, autoRelease));
#endif
}

शून्य cv::ogl::Program::create(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(vert, frag, autoRelease));
#endif
}

शून्य cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    यदि (impl_)
        impl_->setAutoRelease(सत्य);
    impl_ = Impl::empty();
#endif
}

शून्य cv::ogl::Program::setAutoRelease(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#endif
}

शून्य cv::ogl::Program::bind() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#endif
}

शून्य cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    gl::UseProgram(0);
#endif
}

पूर्णाङ्क cv::ogl::Program::getAttributeLocation(ध्रुव वर्ण* name) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->getAttributeLocation(name);
#endif
}

पूर्णाङ्क cv::ogl::Program::getUniformLocation(ध्रुव वर्ण* name) ध्रुव
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->getUniformLocation(name);
#endif
}

शून्य cv::ogl::Program::setUniformVec3(पूर्णाङ्क loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

शून्य cv::ogl::Program::setUniformVec2(पूर्णाङ्क loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

शून्य cv::ogl::Program::setUniformVec4(पूर्णाङ्क loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

शून्य cv::ogl::Program::setUniformMat4x4(पूर्णाङ्क loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#अन्यथा
    gl::UniformMatrix4fv(loc, 1, सत्य, mat.val);
#endif
}

शून्य cv::ogl::Program::setUniform1f(पूर्णाङ्क loc, भिन्न value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#अन्यथा
    gl::Uniform1f(loc, value);
#endif
}

शून्य cv::ogl::Program::setUniform1i(पूर्णाङ्क loc, पूर्णाङ्क value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#अन्यथा
    gl::Uniform1i(loc, value);
#endif
}

unsigned पूर्णाङ्क cv::ogl::Program::programId() ध्रुव
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

शून्य cv::ogl::render(ध्रुव ogl::Texture2D& tex, Rect_<द्विगुण> wndRect, Rect_<द्विगुण> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#अन्यथा
    यदि (!tex.empty())
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

        ध्रुव द्विगुण vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        ध्रुव द्विगुण texCoords[] =
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

शून्य cv::ogl::render(ध्रुव ogl::Arrays& arr, पूर्णाङ्क mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#अन्यथा
    यदि (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

शून्य cv::ogl::render(ध्रुव ogl::Arrays& arr, निवेश_Array indices, पूर्णाङ्क mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#अन्यथा
    यदि (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ध्रुव पूर्णाङ्क kind = indices.kind();

        विकल्प (kind)
        {
        पक्ष निवेश_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                ध्रुव पूर्णाङ्क depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                यदि (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                अन्यथा यदि (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                अन्यथा
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                भङ्ग;
            }

        स्वभाव:
            {
                Mat mat = indices.getMat();

                ध्रुव पूर्णाङ्क depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                यदि (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                अन्यथा यदि (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                अन्यथा
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

शून्य cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#अन्यथा
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

शून्य cv::ogl::clearDepth(भिन्न depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#अन्यथा
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

शून्य cv::ogl::drawArrays(पूर्णाङ्क first, पूर्णाङ्क गिनती_2, पूर्णाङ्क mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(गिनती_2); CV_UNUSED(mode);
    throw_no_ogl();
#अन्यथा
    gl::DrawArrays(mode, first, गिनती_2);
#endif // HAVE_OPENGL
}

शून्य cv::ogl::drawElements(पूर्णाङ्क first, पूर्णाङ्क गिनती_2, पूर्णाङ्क type, पूर्णाङ्क mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(गिनती_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#अन्यथा
    gl::DrawElements(mode, गिनती_2, type, reinterpret_cast<ध्रुव शून्य*>(static_cast<size_t>(first)));
#endif
}

शून्य cv::ogl::drawElementsInstanced(पूर्णाङ्क first, पूर्णाङ्क गिनती_2, पूर्णाङ्क type, पूर्णाङ्क mode, पूर्णाङ्क instance_गिनती)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(गिनती_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_गिनती);
    throw_no_ogl();
#अन्यथा
    gl::DrawElementsInstanced(mode, गिनती_2, type, reinterpret_cast<ध्रुव शून्य*>(static_cast<size_t>(first)), instance_गिनती);
    CV_CheckGlError();
#endif
}

शून्य cv::ogl::blendFunc(पूर्णाङ्क sfactor, पूर्णाङ्क dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#अन्यथा
    gl::BlendFunc(sfactor, dfactor);
#endif
}

शून्य cv::ogl::depthMask(तार्किक flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

शून्य cv::ogl::enable(पूर्णाङ्क cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#अन्यथा
    gl::Enable(cap);
#endif
}

शून्य cv::ogl::disable(पूर्णाङ्क cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#अन्यथा
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
#  अन्यथा
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#अन्यथा // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#यदि defined(HAVE_OPENGL)
#  यदि defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

नामाकाश cv { नामाकाश ogl {

#यदि defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
स्थिर शून्य checkOpenCLVersion()
{
    using नामाकाश cv::ocl;
    ध्रुव Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, परिमाण(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == परिमाण(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    पूर्णाङ्क versionMajor = pi.versionMajor();
    पूर्णाङ्क versionMinor = pi.versionMinor();
    यदि (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

नामाकाश ocl {

Context& initializeContextFromGL()
{
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, रिक्त, &platformsCnt);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    यदि (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    तार्किक sharingSupported = असत्य;

    प्रति (unsigned पूर्णाङ्क i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, रिक्त, &devCnt);
        यदि (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        यत्न {
            devices = नूतन cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, रिक्त);
            यदि (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            प्रति (unsigned पूर्णाङ्क j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, रिक्त, &extensionSize );
                यदि (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                यदि(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    यदि (status != CL_SUCCESS)
                        अनुवर्त;
                    यदि (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    यावत् (spacePos != devString.npos) {
                        यदि (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = सत्य;
                            भङ्ग;
                        }
                        कुरु {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } यावत् (spacePos == oldPos);
                    }
                }
            }
        } गृहाण(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            यदि(devices != nullptr) {
                delete[] devices;
            }
            अनुवर्त;
        }

        यदि (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#यदि defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, रिक्त, रिक्त, &ci_Err_अंक);
#नोचेद्यदि defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], रिक्त, रिक्त, &status);
#नोचेद्यदि defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], रिक्त, रिक्त, &status);
#नोचेद्यदि defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], रिक्त, रिक्त, &status);
#endif
        }

        यदि (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        अन्यथा
            भङ्ग;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    प्रत्यागम const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

शून्य convertToGLTexture2D(निवेश_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (पूर्णाङ्क)texture.cols() && srcSize.height == (पूर्णाङ्क)texture.rows());

    using नामाकाश cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

शून्य convertFromGLTexture2D(ध्रुव Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    // check texture format
    ध्रुव पूर्णाङ्क dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    पूर्णाङ्क textureType = dtype;
    CV_Assert(textureType >= 0);

    using नामाकाश cv::ocl;
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
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ध्रुव Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    using नामाकाश cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    पूर्णाङ्क clAccessFlags = 0;
    विकल्प (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    स्वभाव:
    पक्ष ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        भङ्ग;
    पक्ष ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        भङ्ग;
    पक्ष ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        भङ्ग;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    पूर्णाङ्क rows = buffer.rows();
    पूर्णाङ्क cols = buffer.cols();
    पूर्णाङ्क type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    प्रत्यागम u;
#endif
}

शून्य unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    using नामाकाश cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, रिक्त, रिक्त);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    यदि (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
