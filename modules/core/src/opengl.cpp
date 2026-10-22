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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଯଦି defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#ଅନ୍ୟଥା
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#ifdef HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    ଯଦି (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_VERSION
#        define GL_VERSION 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#ଅନ୍ୟଥା // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#endif // HAVE_OPENGL

using ନାମାକାଶ cv;
using ନାମାକାଶ cv::cuda;

#ଯଦି defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

ନାମାକାଶ
{
#ifndef HAVE_OPENGL
ଧାଡ଼ିଭିତରେ ସ୍ଥିର CV_NORETURN ଶୂନ୍ୟ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#ନଚେତଯଦି defined _DEBUG
ଧାଡ଼ିଭିତରେ ସ୍ଥିର ତାର୍କିକ checkError(ଅଚଳ ଅକ୍ଷର* file, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି, ଅଚଳ ଅକ୍ଷର* func = 0)
{
    GLenum err = gl::GetError();
    ଯଦି (err != gl::NO_ERROR_)
    {
        ଅଚଳ ଅକ୍ଷର* ସନ୍ଦେଶ;
        ବାଛ (err)
        {
        ସ୍ଥିତି gl::INVALID_ENUM:
            ସନ୍ଦେଶ = "An unacceptable value is specified for an enumerated argument";
            ଭାଙ୍ଗ;
        ସ୍ଥିତି gl::INVALID_ମୂଲ୍ୟ:
            ସନ୍ଦେଶ = "A numeric argument is out of range";
            ଭାଙ୍ଗ;
        ସ୍ଥିତି gl::INVALID_OPERATION:
            ସନ୍ଦେଶ = "The specified operation is not allowed in the current state";
            ଭାଙ୍ଗ;
        ସ୍ଥିତି gl::OUT_OF_MEMORY:
            ସନ୍ଦେଶ = "There is not enough memory left to execute the command";
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ସନ୍ଦେଶ = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, ସନ୍ଦେଶ, file, ଧାଡ଼ି);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
ନାମାକାଶ
{
    ଅଚଳ GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif

////////////////////////////////////////////////////////////////////////
// setGlDevice

ଶୂନ୍ୟ cv::cuda::setGlDevice(ପୂର୍ଣ୍ଣସଂଖ୍ୟା device)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #ଅନ୍ୟଥା
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #endif
#endif
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#ଯଦି defined(HAVE_OPENGL) && defined(HAVE_CUDA)

ନାମାକାଶ
{
    ଶ୍ରେଣୀ CudaResource
    {
    ସର୍ବସାଧାରଣ:
        CudaResource();
        ~CudaResource();

        ଶୂନ୍ୟ registerBuffer(GLuint buffer);
        ଶୂନ୍ୟ release();

        ଶୂନ୍ୟ copyFrom(ଅଚଳ ଶୂନ୍ୟ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
        ଶୂନ୍ୟ copyTo(ଶୂନ୍ୟ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0);

        ଶୂନ୍ୟ* map(cudaStream_t stream = 0);
        ଶୂନ୍ୟ unmap(cudaStream_t stream = 0);

    ବ୍ୟକ୍ତିଗତ:
        cudaGraphicsResource_t resource_;
        GLuint buffer_;

        ଶ୍ରେଣୀ GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), buffer_(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    ଶୂନ୍ୟ CudaResource::registerBuffer(GLuint buffer)
    {
        CV_DbgAssert( buffer != 0 );

        ଯଦି (buffer_ == buffer)
            ଫେରନ୍ତୁ;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        buffer_ = buffer;
    }

    ଶୂନ୍ୟ CudaResource::release()
    {
        ଯଦି (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        buffer_ = 0;
    }

    ଶ୍ରେଣୀ CudaResource::GraphicsMapHolder
    {
    ସର୍ବସାଧାରଣ:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        ଶୂନ୍ୟ reset();

    ବ୍ୟକ୍ତିଗତ:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        ଯଦି (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        ଯଦି (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    ଶୂନ୍ୟ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    ଶୂନ୍ୟ CudaResource::copyFrom(ଅଚଳ ଶୂନ୍ୟ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        ଶୂନ୍ୟ* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        ଯଦି (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        ଅନ୍ୟଥା
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    ଶୂନ୍ୟ CudaResource::copyTo(ଶୂନ୍ୟ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        ଶୂନ୍ୟ* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        ଯଦି (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        ଅନ୍ୟଥା
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    ଶୂନ୍ୟ* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        ଶୂନ୍ୟ* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        ଫେରନ୍ତୁ ptr;
    }

    ଶୂନ୍ୟ CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#endif

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

ଶ୍ରେଣୀ cv::ogl::Buffer::Impl
{
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::ogl::Buffer::Impl
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ଅଚଳ Ptr<Impl>& empty();

    Impl(GLuint bufId, ତାର୍କିକ autoRelease);
    Impl(GLsizeiptr size, ଅଚଳ GLvoid* data, GLenum target, ତାର୍କିକ autoRelease);
    ~Impl();

    ଶୂନ୍ୟ bind(GLenum target) ଅଚଳ;

    ଶୂନ୍ୟ copyFrom(GLuint srcBuf, GLsizeiptr size);

    ଶୂନ୍ୟ copyFrom(GLsizeiptr size, ଅଚଳ GLvoid* data);
    ଶୂନ୍ୟ copyTo(GLsizeiptr size, GLvoid* data) ଅଚଳ;

    ଶୂନ୍ୟ resize(GLsizeiptr size, GLenum target);

    ଶୂନ୍ୟ* mapHost(GLenum access);
    ଶୂନ୍ୟ unmapHost();

#ifdef HAVE_CUDA
    ଶୂନ୍ୟ copyFrom(ଅଚଳ ଶୂନ୍ୟ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream = 0);
    ଶୂନ୍ୟ copyTo(ଶୂନ୍ୟ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream = 0) ଅଚଳ;

    ଶୂନ୍ୟ* mapDevice(cudaStream_t stream = 0);
    ଶୂନ୍ୟ unmapDevice(cudaStream_t stream = 0);
#endif

    ଶୂନ୍ୟ setAutoRelease(ତାର୍କିକ flag) { autoRelease_ = flag; }

    GLuint bufId() ଅଚଳ { ଫେରନ୍ତୁ bufId_; }

ବ୍ୟକ୍ତିଗତ:
    Impl();

    GLuint bufId_;
    ତାର୍କିକ autoRelease_;

#ifdef HAVE_CUDA
    mutable CudaResource cudaResource_;
#endif
};

ଅଚଳ Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    ସ୍ଥିର Ptr<Impl> p(ନୂତନ Impl);
    ଫେରନ୍ତୁ p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(ମିଥ୍ୟା)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, ତାର୍କିକ autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, ଅଚଳ GLvoid* data, GLenum target, ତାର୍କିକ autoRelease) : bufId_(0), autoRelease_(autoRelease)
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
    ଯଦି (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::bind(GLenum target) ଅଚଳ
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    CV_CheckGlError();

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    CV_CheckGlError();
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, ଅଚଳ GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    CV_CheckGlError();

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    CV_CheckGlError();
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::BindBuffer(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::BufferData(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::BindBuffer(target, 0);
    CV_CheckGlError();
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) ଅଚଳ
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    CV_CheckGlError();
}

ଶୂନ୍ୟ* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    CV_CheckGlError();

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    CV_CheckGlError();

    ଫେରନ୍ତୁ data;
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::copyFrom(ଅଚଳ ଶୂନ୍ୟ* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::copyTo(ଶୂନ୍ୟ* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) ଅଚଳ
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

ଶୂନ୍ୟ* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    ଫେରନ୍ତୁ cudaResource_.map(stream);
}

ଶୂନ୍ୟ cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା arows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା acols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା abufId, ତାର୍କିକ autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା abufId, ତାର୍କିକ autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(ନିବେଶ_Array arr, Target target, ତାର୍କିକ autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = arr.kind();

    ବାଛ (kind)
    {
    ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER:
    ସ୍ଥିତି ନିବେଶ_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ଅଚଳ GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(ନୂତନ Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            ଭାଙ୍ଗ;
        }
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା arows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା acols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype, Target target, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଯଦି (rows_ != arows || cols_ != acols || type_ != atype)
    {
        ଅଚଳ GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_SIZE(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        ଯଦି (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        ଅନ୍ୟଥା
            impl_.reset(ନୂତନ Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    ଯଦି (impl_)
        impl_->setAutoRelease(ସତ୍ୟ);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->setAutoRelease(flag);
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::copyFrom(ନିବେଶ_Array arr, Target target, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = arr.kind();

    ଅଚଳ Size asize = arr.size();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype = arr.type();
    create(asize, atype, target, autoRelease);

    ବାଛ (kind)
    {
    ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            ଭାଙ୍ଗ;
        }

    ସ୍ଥିତି ନିବେଶ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ଅନ୍ୟଥା
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            ଭାଙ୍ଗ;
        }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::copyFrom(ନିବେଶ_Array arr, cuda::Stream& stream, Target target, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #ଅନ୍ୟଥା
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::copyTo(OutputArray arr) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = arr.kind();

    ବାଛ (kind)
    {
    ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER:
        {
            arr.getOGlBufferRef().copyFrom(*ଏହି);
            ଭାଙ୍ଗ;
        }

    ସ୍ଥିତି ନିବେଶ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ଅନ୍ୟଥା
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows);
            #endif

            ଭାଙ୍ଗ;
        }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elemSize(), mat.data);
        }
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #ଅନ୍ୟଥା
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.data, dmat.step, dmat.cols * dmat.elemSize(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

cv::ogl::Buffer cv::ogl::Buffer::clone(Target target, ତାର୍କିକ autoRelease) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ogl::Buffer buf;
    buf.copyFrom(*ଏହି, target, autoRelease);
    ଫେରନ୍ତୁ buf;
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::bind(Target target) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->bind(target);
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::BindBuffer(target, 0);
    CV_CheckGlError();
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->unmapHost();
#endif
}

GpuMat cv::ogl::Buffer::mapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #ଅନ୍ୟଥା
        ଫେରନ୍ତୁ GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #endif
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::unmapDevice()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        throw_no_cuda();
    #ଅନ୍ୟଥା
        impl_->unmapDevice();
    #endif
#endif
}

cuda::GpuMat cv::ogl::Buffer::mapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #ଅନ୍ୟଥା
        ଫେରନ୍ତୁ GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #endif
#endif
}

ଶୂନ୍ୟ cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#ଅନ୍ୟଥା
    #ifndef HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #ଅନ୍ୟଥା
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #endif
#endif
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Buffer::bufId() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

ଶ୍ରେଣୀ cv::ogl::Texture2D::Impl
{
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::ogl::Texture2D::Impl
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ଅଚଳ Ptr<Impl> empty();

    Impl(GLuint texId, ତାର୍କିକ autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ଅଚଳ GLvoid* pixels, ତାର୍କିକ autoRelease);
    ~Impl();

    ଶୂନ୍ୟ copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ଅଚଳ GLvoid *pixels);
    ଶୂନ୍ୟ copyTo(GLenum format, GLenum type, GLvoid* pixels) ଅଚଳ;

    ଶୂନ୍ୟ bind() ଅଚଳ;

    ଶୂନ୍ୟ setAutoRelease(ତାର୍କିକ flag) { autoRelease_ = flag; }

    GLuint texId() ଅଚଳ { ଫେରନ୍ତୁ texId_; }

ବ୍ୟକ୍ତିଗତ:
    Impl();

    GLuint texId_;
    ତାର୍କିକ autoRelease_;
};

ଅଚଳ Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ସ୍ଥିର Ptr<Impl> p(ନୂତନ Impl);
    ଫେରନ୍ତୁ p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(ମିଥ୍ୟା)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, ତାର୍କିକ autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, ଅଚଳ GLvoid* pixels, ତାର୍କିକ autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    ଯଦି (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ଶୂନ୍ୟ cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, ଅଚଳ GLvoid *pixels)
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

ଶୂନ୍ୟ cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) ଅଚଳ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

ଶୂନ୍ୟ cv::ogl::Texture2D::Impl::bind() ଅଚଳ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(ପୂର୍ଣ୍ଣସଂଖ୍ୟା arows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା acols, Format aformat, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା atexId, ତାର୍କିକ autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା atexId, ତାର୍କିକ autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(ନିବେଶ_Array arr, ତାର୍କିକ autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = arr.kind();

    ଅଚଳ Size asize = arr.size();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype = arr.type();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(atype);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ଅଚଳ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ଅଚଳ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    ବାଛ (kind)
    {
    ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(ନୂତନ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            ଭାଙ୍ଗ;
        }

    ସ୍ଥିତି ନିବେଶ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ଅନ୍ୟଥା
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(ସତ୍ୟ);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_.reset(ନୂତନ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            ଭାଙ୍ଗ;
        }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(ନୂତନ Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            ଭାଙ୍ଗ;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

ଶୂନ୍ୟ cv::ogl::Texture2D::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା arows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା acols, Format aformat, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଯଦି (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(ନୂତନ Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    ଯଦି (impl_)
        impl_->setAutoRelease(ସତ୍ୟ);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

ଶୂନ୍ୟ cv::ogl::Texture2D::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->setAutoRelease(flag);
#endif
}

ଶୂନ୍ୟ cv::ogl::Texture2D::copyFrom(ନିବେଶ_Array arr, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = arr.kind();

    ଅଚଳ Size asize = arr.size();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype = arr.type();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_MAT_DEPTH(atype);
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ଅଚଳ Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    ଅଚଳ GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    ବାଛ(kind)
    {
    ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            ଭାଙ୍ଗ;
        }

    ସ୍ଥିତି ନିବେଶ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ଅନ୍ୟଥା
                GpuMat dmat = arr.getGpuMat();
                ogl::Buffer buf(dmat, ogl::Buffer::PIXEL_UNPACK_BUFFER);
                buf.setAutoRelease(ସତ୍ୟ);
                buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            #endif

            ଭାଙ୍ଗ;
        }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::Texture2D::copyTo(OutputArray arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ତାର୍କିକ autoRelease) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = arr.kind();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ଅଚଳ GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    ବାଛ(kind)
    {
    ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            ଭାଙ୍ଗ;
        }

    ସ୍ଥିତି ନିବେଶ_Array_2::CUDA_GPU_MAT:
        {
            #ifndef HAVE_CUDA
                throw_no_cuda();
            #ଅନ୍ୟଥା
                ogl::Buffer buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.setAutoRelease(ସତ୍ୟ);
                buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
                buf.copyTo(arr);
            #endif

            ଭାଙ୍ଗ;
        }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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

ଶୂନ୍ୟ cv::ogl::Texture2D::bind() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->bind();
#endif
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Texture2D::texId() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->texId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ifndef HAVE_OPENGL

ଶ୍ରେଣୀ cv::ogl::TextureBuffer::Impl
{
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::ogl::TextureBuffer::Impl
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ଅଚଳ Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, ତାର୍କିକ autoRelease);
    ~Impl();

    ଶୂନ୍ୟ bind(GLint unit) ଅଚଳ;

    ଶୂନ୍ୟ setAutoRelease(ତାର୍କିକ flag) { autoRelease_ = flag; }

    GLuint texId() ଅଚଳ { ଫେରନ୍ତୁ texId_; }

ବ୍ୟକ୍ତିଗତ:
    GLuint texId_;
    ତାର୍କିକ autoRelease_;
};

ଅଚଳ Ptr<cv::ogl::TextureBuffer::Impl> cv::ogl::TextureBuffer::Impl::empty()
{
    ସ୍ଥିର Ptr<Impl> p = makePtr<Impl>();
    ଫେରନ୍ତୁ p;
}

cv::ogl::TextureBuffer::Impl::Impl() : texId_(0), autoRelease_(ମିଥ୍ୟା)
{
}

cv::ogl::TextureBuffer::Impl::Impl(GLenum internalFormat, GLuint bufId, ତାର୍କିକ autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    ଯଦି (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ଶୂନ୍ୟ cv::ogl::TextureBuffer::Impl::bind(GLint unit) ଅଚଳ
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
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
#endif
}

cv::ogl::TextureBuffer::TextureBuffer(ଅଚଳ Buffer& buf, Format aformat, ତାର୍କିକ autoRelease) : format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#endif
}

ଶୂନ୍ୟ cv::ogl::TextureBuffer::create(ଅଚଳ Buffer& buf, Format aformat, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#endif
}

ଶୂନ୍ୟ cv::ogl::TextureBuffer::release()
{
#ifdef HAVE_OPENGL
    ଯଦି (impl_)
        impl_->setAutoRelease(ସତ୍ୟ);
    impl_ = Impl::empty();
#endif
    buf_ = Buffer();
    format_ = NONE;
}

ଶୂନ୍ୟ cv::ogl::TextureBuffer::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->setAutoRelease(flag);
#endif
}

ଶୂନ୍ୟ cv::ogl::TextureBuffer::bind(ପୂର୍ଣ୍ଣସଂଖ୍ୟା unit) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#ଅନ୍ୟଥା
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#endif
}

cv::ogl::TextureBuffer::Format cv::ogl::TextureBuffer::format() ଅଚଳ
{
    ଫେରନ୍ତୁ format_;
}

ତାର୍କିକ cv::ogl::TextureBuffer::empty() ଅଚଳ
{
    ଫେରନ୍ତୁ format_ == NONE;
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::TextureBuffer::texId() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

ଶୂନ୍ୟ cv::ogl::Arrays::setVertexArray(ନିବେଶ_Array vertex)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = vertex.channels();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    ଯଦି (vertex.kind() == ନିବେଶ_Array_2::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    ଅନ୍ୟଥା
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

ଶୂନ୍ୟ cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

ଶୂନ୍ୟ cv::ogl::Arrays::setColorArray(ନିବେଶ_Array color)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    ଯଦି (color.kind() == ନିବେଶ_Array_2::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    ଅନ୍ୟଥା
        color_.copyFrom(color);
}

ଶୂନ୍ୟ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

ଶୂନ୍ୟ cv::ogl::Arrays::setNormalArray(ନିବେଶ_Array normal)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = normal.channels();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    ଯଦି (normal.kind() == ନିବେଶ_Array_2::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    ଅନ୍ୟଥା
        normal_.copyFrom(normal);
}

ଶୂନ୍ୟ cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

ଶୂନ୍ୟ cv::ogl::Arrays::setTexCoordArray(ନିବେଶ_Array texCoord)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = texCoord.channels();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    ଯଦି (texCoord.kind() == ନିବେଶ_Array_2::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    ଅନ୍ୟଥା
        texCoord_.copyFrom(texCoord);
}

ଶୂନ୍ୟ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

ଶୂନ୍ୟ cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

ଶୂନ୍ୟ cv::ogl::Arrays::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

ଶୂନ୍ୟ cv::ogl::Arrays::bind() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    ଯଦି (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    ଅନ୍ୟଥା
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ଯଦି (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    ଅନ୍ୟଥା
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ଯଦି (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    ଅନ୍ୟଥା
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ଯଦି (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    ଅନ୍ୟଥା
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

ଶ୍ରେଣୀ cv::ogl::VertexArray::Impl
{
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::ogl::VertexArray::Impl
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ଅଚଳ Ptr<Impl>& empty();

    Impl(GLuint vaId, ତାର୍କିକ autoRelease);
    Impl(std::initializer_list<Attribute> attributes, ତାର୍କିକ autoRelease);
    ~Impl();

    ଶୂନ୍ୟ bind();

    ଶୂନ୍ୟ setAutoRelease(ତାର୍କିକ flag) { autoRelease_ = flag; }

    GLuint vaId() ଅଚଳ { ଫେରନ୍ତୁ vaId_; }

ବ୍ୟକ୍ତିଗତ:
    Impl();

    GLuint vaId_;
    ତାର୍କିକ autoRelease_;
};

ଅଚଳ Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    ସ୍ଥିର Ptr<Impl> p(ନୂତନ Impl);
    ଫେରନ୍ତୁ p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(ମିଥ୍ୟା)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, ତାର୍କିକ autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, ତାର୍କିକ autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    ପାଇଁ (auto& attribute : attributes)
    {
        attribute.buffer_.bind(Buffer::ARRAY_BUFFER);
        ଯଦି (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ଅଚଳ ଶୂନ୍ୟ*>(attribute.offset_)
            );
            CV_CheckGlError();
        }
        ଅନ୍ୟଥା
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.size_,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ଅଚଳ ଶୂନ୍ୟ*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    ଯଦି (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

ଶୂନ୍ୟ cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#endif // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
#endif
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(attributes, autoRelease));
#endif
}

ଶୂନ୍ୟ cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(attributes, autoRelease));
#endif
}

ଶୂନ୍ୟ cv::ogl::VertexArray::release()
{
#ifdef HAVE_OPENGL
    ଯଦି (impl_)
        impl_->setAutoRelease(ସତ୍ୟ);
    impl_ = Impl::empty();
#endif
}

ଶୂନ୍ୟ cv::ogl::VertexArray::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->setAutoRelease(flag);
#endif
}

ଶୂନ୍ୟ cv::ogl::VertexArray::bind() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->bind();
#endif
}

ଶୂନ୍ୟ cv::ogl::VertexArray::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::BindVertexArray(0);
#endif
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::VertexArray::vaId() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->vaId();
#endif
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ifndef HAVE_OPENGL

ଶ୍ରେଣୀ cv::ogl::Shader::Impl
{
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::ogl::Shader::Impl
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ଅଚଳ Ptr<Impl>& empty();

    Impl(GLuint shaderId, ତାର୍କିକ autoRelease);
    Impl(ଅଚଳ ଅକ୍ଷର* src, GLenum type, ତାର୍କିକ autoRelease);
    ~Impl();

    ଶୂନ୍ୟ setAutoRelease(ତାର୍କିକ flag) { autoRelease_ = flag; }

    GLuint shaderId() ଅଚଳ { ଫେରନ୍ତୁ shaderId_; }

ବ୍ୟକ୍ତିଗତ:
    Impl();

    GLuint shaderId_;
    ତାର୍କିକ autoRelease_;
};

ଅଚଳ Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ସ୍ଥିର Ptr<Impl> p(ନୂତନ Impl);
    ଫେରନ୍ତୁ p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(ମିଥ୍ୟା)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, ତାର୍କିକ autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ଅଚଳ ଅକ୍ଷର* src, GLenum type, ତାର୍କିକ autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    ଯଦି (!status) {
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
    ଯଦି (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#endif // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
#endif
}

cv::ogl::Shader::Shader(ଅଚଳ ଅକ୍ଷର* src, ShaderType type, ତାର୍କିକ autoRelease) : type_(type)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(src, type, autoRelease));
#endif
}

ଶୂନ୍ୟ cv::ogl::Shader::create(ଅଚଳ ଅକ୍ଷର* src, ShaderType type, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(src, type, autoRelease));
    type_ = type;
#endif
}

ଶୂନ୍ୟ cv::ogl::Shader::release()
{
#ifdef HAVE_OPENGL
    ଯଦି (impl_)
        impl_->setAutoRelease(ସତ୍ୟ);
    impl_ = Impl::empty();
#endif
}

ଶୂନ୍ୟ cv::ogl::Shader::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->setAutoRelease(flag);
#endif
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Shader::shaderId() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->shaderId();
#endif
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ଅଚଳ
{
    ଫେରନ୍ତୁ type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ifndef HAVE_OPENGL

ଶ୍ରେଣୀ cv::ogl::Program::Impl
{
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::ogl::Program::Impl
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର ଅଚଳ Ptr<Impl>& empty();

    Impl(GLuint programId, ତାର୍କିକ autoRelease);
    Impl(ଅଚଳ Shader& vert, ଅଚଳ Shader& frag, ତାର୍କିକ autoRelease);
    ~Impl();

    ଶୂନ୍ୟ bind();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା getAttributeLocation(ଅଚଳ ଅକ୍ଷର* name) ଅଚଳ;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା getUniformLocation(ଅଚଳ ଅକ୍ଷର* name) ଅଚଳ;

    ଶୂନ୍ୟ setAutoRelease(ତାର୍କିକ flag) { autoRelease_ = flag; }

    GLuint programId() ଅଚଳ { ଫେରନ୍ତୁ programId_; }

ବ୍ୟକ୍ତିଗତ:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    ତାର୍କିକ autoRelease_;
};

ଅଚଳ Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    ସ୍ଥିର Ptr<Impl> p(ନୂତନ Impl);
    ଫେରନ୍ତୁ p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(ମିଥ୍ୟା)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, ତାର୍କିକ autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(ଅଚଳ Shader& vert, ଅଚଳ Shader& frag, ତାର୍କିକ autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    ଯଦି (!status) {
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
    ଯଦି (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

ଶୂନ୍ୟ cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Program::Impl::getAttributeLocation(ଅଚଳ ଅକ୍ଷର* name) ଅଚଳ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    ଫେରନ୍ତୁ location;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Program::Impl::getUniformLocation(ଅଚଳ ଅକ୍ଷର* name) ଅଚଳ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    ଫେରନ୍ତୁ location;
}

#endif // HAVE_OPENGL

cv::ogl::Program::Program()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_ = Impl::empty();
#endif
}

cv::ogl::Program::Program(ଅଚଳ Shader& vert, ଅଚଳ Shader& frag, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(vert, frag, autoRelease));
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::create(ଅଚଳ Shader& vert, ଅଚଳ Shader& frag, ତାର୍କିକ autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_.reset(ନୂତନ Impl(vert, frag, autoRelease));
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::release()
{
#ifdef HAVE_OPENGL
    ଯଦି (impl_)
        impl_->setAutoRelease(ସତ୍ୟ);
    impl_ = Impl::empty();
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setAutoRelease(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->setAutoRelease(flag);
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::bind() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    impl_->bind();
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::unbind()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::UseProgram(0);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Program::getAttributeLocation(ଅଚଳ ଅକ୍ଷର* name) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->getAttributeLocation(name);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Program::getUniformLocation(ଅଚଳ ଅକ୍ଷର* name) ଅଚଳ
{
#ifndef HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->getUniformLocation(name);
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setUniformVec3(ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc, Vec3f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setUniformVec2(ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc, Vec2f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::Uniform2f(loc, vec(0), vec(1));
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setUniformVec4(ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc, Vec4f vec)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setUniformMat4x4(ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc, Matx44f mat)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::UniformMatrix4fv(loc, 1, ସତ୍ୟ, mat.val);
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setUniform1f(ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc, ଦଶମିକ value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::Uniform1f(loc, value);
#endif
}

ଶୂନ୍ୟ cv::ogl::Program::setUniform1i(ପୂର୍ଣ୍ଣସଂଖ୍ୟା loc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା value)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::Uniform1i(loc, value);
#endif
}

unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv::ogl::Program::programId() ଅଚଳ
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->programId();
#endif
}

////////////////////////////////////////////////////////////////////////
// Rendering

ଶୂନ୍ୟ cv::ogl::render(ଅଚଳ ogl::Texture2D& tex, Rect_<ଦ୍ୱିଗୁଣ> wndRect, Rect_<ଦ୍ୱିଗୁଣ> texRect)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଯଦି (!tex.empty())
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

        ଅଚଳ ଦ୍ୱିଗୁଣ vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        ଅଚଳ ଦ୍ୱିଗୁଣ texCoords[] =
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

ଶୂନ୍ୟ cv::ogl::render(ଅଚଳ ogl::Arrays& arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଯଦି (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::render(ଅଚଳ ogl::Arrays& arr, ନିବେଶ_Array indices, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#ଅନ୍ୟଥା
    ଯଦି (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା kind = indices.kind();

        ବାଛ (kind)
        {
        ସ୍ଥିତି ନିବେଶ_Array_2::OPENGL_BUFFER :
            {
                ogl::Buffer buf = indices.getOGlBuffer();

                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                ଯଦି (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ଅନ୍ୟଥା ଯଦି (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ଅନ୍ୟଥା
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                ଭାଙ୍ଗ;
            }

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            {
                Mat mat = indices.getMat();

                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                ଯଦି (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ଅନ୍ୟଥା ଯଦି (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ଅନ୍ୟଥା
                    type = gl::UNSIGNED_INT;

                ogl::Buffer::unbind(ogl::Buffer::ELEMENT_ARRAY_BUFFER);

                gl::DrawElements(mode, mat.size().area(), type, mat.data);
            }
        }
    }
#endif
}

ଶୂନ୍ୟ cv::ogl::clearColor(Scalar color)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_BUFFER_BIT);
#endif // HAVE_OPENGL
}

ଶୂନ୍ୟ cv::ogl::clearDepth(ଦଶମିକ depth)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_BUFFER_BIT);
#endif // HAVE_OPENGL
}

ଶୂନ୍ୟ cv::ogl::drawArrays(ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ଗଣନା_3); CV_UNUSED(mode);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::DrawArrays(mode, first, ଗଣନା_3);
#endif // HAVE_OPENGL
}

ଶୂନ୍ୟ cv::ogl::drawElements(ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ଗଣନା_3); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::DrawElements(mode, ଗଣନା_3, type, reinterpret_cast<ଅଚଳ ଶୂନ୍ୟ*>(static_cast<size_t>(first)));
#endif
}

ଶୂନ୍ୟ cv::ogl::drawElementsInstanced(ପୂର୍ଣ୍ଣସଂଖ୍ୟା first, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା instance_ଗଣନା)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(ଗଣନା_3); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_ଗଣନା);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::DrawElementsInstanced(mode, ଗଣନା_3, type, reinterpret_cast<ଅଚଳ ଶୂନ୍ୟ*>(static_cast<size_t>(first)), instance_ଗଣନା);
    CV_CheckGlError();
#endif
}

ଶୂନ୍ୟ cv::ogl::blendFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sfactor, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dfactor)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::BlendFunc(sfactor, dfactor);
#endif
}

ଶୂନ୍ୟ cv::ogl::depthMask(ତାର୍କିକ flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#endif
}

ଶୂନ୍ୟ cv::ogl::enable(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#ଅନ୍ୟଥା
    gl::Enable(cap);
#endif
}

ଶୂନ୍ୟ cv::ogl::disable(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cap)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#ଅନ୍ୟଥା
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
#  ଅନ୍ୟଥା
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#ଅନ୍ୟଥା // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

#ଯଦି defined(HAVE_OPENGL)
#  ଯଦି defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#endif // HAVE_OPENGL

ନାମାକାଶ cv { ନାମାକାଶ ogl {

#ଯଦି defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ସ୍ଥିର ଶୂନ୍ୟ checkOpenCLVersion()
{
    using ନାମାକାଶ cv::ocl;
    ଅଚଳ Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, ଆକାର(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == ଆକାର(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା versionMajor = pi.versionMajor();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା versionMinor = pi.versionMinor();
    ଯଦି (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#endif

ନାମାକାଶ ocl {

Context& initializeContextFromGL()
{
#ଯଦି !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ଅନ୍ୟଥା
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, ଖାଲି, &platformsCnt);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    ଯଦି (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ତାର୍କିକ sharingSupported = ମିଥ୍ୟା;

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ଖାଲି, &devCnt);
        ଯଦି (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        ଚେଷ୍ଟା {
            devices = ନୂତନ cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ଖାଲି);
            ଯଦି (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extensionSize;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ଖାଲି, &extensionSize );
                ଯଦି (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                ଯଦି(extensionSize > 0)
                {
                    std::string devString(extensionSize, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extensionSize);
                    ଯଦି (status != CL_SUCCESS)
                        ଜାରି;
                    ଯଦି (extensionSize > 0 && devString.size() >= extensionSize)
                        devString.resize(extensionSize - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    ଯେତେବେଳେ (spacePos != devString.npos) {
                        ଯଦି (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = ସତ୍ୟ;
                            ଭାଙ୍ଗ;
                        }
                        କର {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } ଯେତେବେଳେ (spacePos == oldPos);
                    }
                }
            }
        } ଧର(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            ଯଦି(devices != nullptr) {
                delete[] devices;
            }
            ଜାରି;
        }

        ଯଦି (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#ଯଦି defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, ଖାଲି, ଖାଲି, &ci_Err_ଅଙ୍କ);
#ନଚେତଯଦି defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ଖାଲି, ଖାଲି, &status);
#ନଚେତଯଦି defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ଖାଲି, ଖାଲି, &status);
#ନଚେତଯଦି defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], ଖାଲି, ଖାଲି, &status);
#endif
        }

        ଯଦି (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        ଅନ୍ୟଥା
            ଭାଙ୍ଗ;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    ଫେରନ୍ତୁ const_cast<Context&>(clExecCtx.getContext());
#endif
}

} // namespace cv::ogl::ocl

ଶୂନ୍ୟ convertToGLTexture2D(ନିବେଶ_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#ଯଦି !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ଅନ୍ୟଥା
    Size srcSize = src.size();
    CV_Assert(srcSize.width == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)texture.cols() && srcSize.height == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)texture.rows());

    using ନାମାକାଶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

ଶୂନ୍ୟ convertFromGLTexture2D(ଅଚଳ Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#ଯଦି !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ଅନ୍ୟଥା
    // check texture format
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା textureType = dtype;
    CV_Assert(textureType >= 0);

    using ନାମାକାଶ cv::ocl;
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
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ଅଚଳ Buffer& buffer, AccessFlag accessFlags)
{
    CV_UNUSED(buffer); CV_UNUSED(accessFlags);
#ଯଦି !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ଅନ୍ୟଥା
    using ନାମାକାଶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା clAccessFlags = 0;
    ବାଛ (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    ସ୍ଥିତି ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        ଭାଙ୍ଗ;
    }

    cl_int status = 0;
    cl_mem clBuffer = clCreateFromGLBuffer(context, clAccessFlags, buffer.bufId(), &status);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &clBuffer, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    size_t step = buffer.cols() * buffer.elemSize();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows = buffer.rows();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols = buffer.cols();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = buffer.type();

    UMat u;
    convertFromBuffer(clBuffer, step, rows, cols, type, u);
    ଫେରନ୍ତୁ u;
#endif
}

ଶୂନ୍ୟ unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#ଯଦି !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ନଚେତଯଦି !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ଅନ୍ୟଥା
    using ନାମାକାଶ cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &clBuffer, 0, ଖାଲି, ଖାଲି);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clBuffer);
    ଯଦି (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#endif
}

}} // namespace cv::ogl
