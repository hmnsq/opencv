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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਜੇ defined (__APPLE__) || defined(MACOSX)
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#ਨਹੀਂਤਾਂ
   #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    ਜੇ (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_ਸੰਸਕਰਣ
#        define GL_ਸੰਸਕਰਣ 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#ਨਹੀਂਤਾਂ // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#ਜੇ_ਅੰਤ // HAVE_OPENGL

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::cuda;

#ਜੇ defined(_MSC_VER)
#ਹਦਾਇਤ warning(disable : 4702)  // unreachable code
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
ਸਤਰਅੰਦਰ ਸਥਿਰ CV_NORETURN ਖਾਲੀ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#ਜਾਂਜੇ defined _DEBUG
ਸਤਰਅੰਦਰ ਸਥਿਰ ਤਾਰਕਿਕ checkError(ਅਚਲ ਅੱਖਰ* file, ਅਚਲ ਪੂਰਨ ਸਤਰ, ਅਚਲ ਅੱਖਰ* func = 0)
{
    GLenum err = gl::GetError();
    ਜੇ (err != gl::NO_ERROR_)
    {
        ਅਚਲ ਅੱਖਰ* ਸੁਨੇਹਾ;
        ਚੋਣ (err)
        {
        ਹਾਲਤ gl::INVALID_ENUM:
            ਸੁਨੇਹਾ = "An unacceptable value is specified for an enumerated argument";
            ਰੋਕੋ;
        ਹਾਲਤ gl::INVALID_ਮੁੱਲ:
            ਸੁਨੇਹਾ = "A numeric argument is out of range";
            ਰੋਕੋ;
        ਹਾਲਤ gl::INVALID_OPERATION:
            ਸੁਨੇਹਾ = "The specified operation is not allowed in the current state";
            ਰੋਕੋ;
        ਹਾਲਤ gl::ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ:
            ਸੁਨੇਹਾ = "There is not enough memory left to execute the command";
            ਰੋਕੋ;
        ਮੂਲ:
            ਸੁਨੇਹਾ = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, ਸੁਨੇਹਾ, file, ਸਤਰ);
    }
    ਵਾਪਸ ਸੱਚ;
}
#ਜੇ_ਅੰਤ // HAVE_OPENGL
} // namespace

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
ਨਾਮਾਕਾਸ਼
{
    ਅਚਲ GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#ਜੇ_ਅੰਤ

////////////////////////////////////////////////////////////////////////
// setGlDevice

ਖਾਲੀ cv::cuda::ਸੈੱਟ_Gl_Device(ਪੂਰਨ device)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(device);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        CV_ਅਣਵਰਤਿਆ(device);
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        cuda_ਸੁਰੱਖਿਅਤ_Call( cudaGLSetGLDevice(device) );
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#ਜੇ defined(HAVE_OPENGL) && defined(HAVE_CUDA)

ਨਾਮਾਕਾਸ਼
{
    ਵਰਗ CudaResource
    {
    ਜਨਤਕ:
        CudaResource();
        ~CudaResource();

        ਖਾਲੀ ਰਜਿਸਟਰ_ਬਫਰ(GLuint ਬਫਰ);
        ਖਾਲੀ release();

        ਖਾਲੀ copyFrom(ਅਚਲ ਖਾਲੀ* src, size_t spitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream = 0);
        ਖਾਲੀ copy_ਮੰਜ਼ਿਲ(ਖਾਲੀ* dst, size_t dpitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream = 0);

        ਖਾਲੀ* map(cuda_ਧਾਰਾ_t stream = 0);
        ਖਾਲੀ unmap(cuda_ਧਾਰਾ_t stream = 0);

    ਨਿਜੀ:
        cudaGraphicsResource_t resource_;
        GLuint ਬਫਰ_3;

        ਵਰਗ GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), ਬਫਰ_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    ਖਾਲੀ CudaResource::ਰਜਿਸਟਰ_ਬਫਰ(GLuint ਬਫਰ)
    {
        CV_DbgAssert( ਬਫਰ != 0 );

        ਜੇ (ਬਫਰ_3 == ਬਫਰ)
            ਵਾਪਸ;

        cudaGraphicsResource_t resource;
        cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Graphics_GLRegister_ਬਫਰ(&resource, ਬਫਰ, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        ਬਫਰ_3 = ਬਫਰ;
    }

    ਖਾਲੀ CudaResource::release()
    {
        ਜੇ (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        ਬਫਰ_3 = 0;
    }

    ਵਰਗ CudaResource::GraphicsMapHolder
    {
    ਜਨਤਕ:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_ਧਾਰਾ_t stream);
        ~GraphicsMapHolder();

        ਖਾਲੀ reset();

    ਨਿਜੀ:
        cudaGraphicsResource_t* resource_;
        cuda_ਧਾਰਾ_t ਧਾਰਾ_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_ਧਾਰਾ_t stream) : resource_(resource), ਧਾਰਾ_3(stream)
    {
        ਜੇ (resource_)
            cuda_ਸੁਰੱਖਿਅਤ_Call( cudaGraphicsMapResources(1, resource_, ਧਾਰਾ_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        ਜੇ (resource_)
            cudaGraphicsUnmapResources(1, resource_, ਧਾਰਾ_3);
    }

    ਖਾਲੀ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    ਖਾਲੀ CudaResource::copyFrom(ਅਚਲ ਖਾਲੀ* src, size_t spitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_ਅਣਵਰਤਿਆ(h);

        ਖਾਲੀ* dst;
        size_t size;
        cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Graphics_Resource_Get_Mapped_ਸੂਚਕ(&dst, &size, resource_) );

        CV_DbgAssert( ਚੌੜਾਈ * ਉਚਾਈ == size );

        ਜੇ (stream == 0)
            cuda_ਸੁਰੱਖਿਅਤ_Call( cudaMemcpy2D(dst, ਚੌੜਾਈ, src, spitch, ਚੌੜਾਈ, ਉਚਾਈ, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Device) );
        ਨਹੀਂਤਾਂ
            cuda_ਸੁਰੱਖਿਅਤ_Call( cudaMemcpy2DAsync(dst, ਚੌੜਾਈ, src, spitch, ਚੌੜਾਈ, ਉਚਾਈ, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Device, stream) );
    }

    ਖਾਲੀ CudaResource::copy_ਮੰਜ਼ਿਲ(ਖਾਲੀ* dst, size_t dpitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_ਅਣਵਰਤਿਆ(h);

        ਖਾਲੀ* src;
        size_t size;
        cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Graphics_Resource_Get_Mapped_ਸੂਚਕ(&src, &size, resource_) );

        CV_DbgAssert( ਚੌੜਾਈ * ਉਚਾਈ == size );

        ਜੇ (stream == 0)
            cuda_ਸੁਰੱਖਿਅਤ_Call( cudaMemcpy2D(dst, dpitch, src, ਚੌੜਾਈ, ਚੌੜਾਈ, ਉਚਾਈ, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Device) );
        ਨਹੀਂਤਾਂ
            cuda_ਸੁਰੱਖਿਅਤ_Call( cudaMemcpy2DAsync(dst, dpitch, src, ਚੌੜਾਈ, ਚੌੜਾਈ, ਉਚਾਈ, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Device, stream) );
    }

    ਖਾਲੀ* CudaResource::map(cuda_ਧਾਰਾ_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        ਖਾਲੀ* ptr;
        size_t size;
        cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Graphics_Resource_Get_Mapped_ਸੂਚਕ(&ptr, &size, resource_) );

        h.reset();

        ਵਾਪਸ ptr;
    }

    ਖਾਲੀ CudaResource::unmap(cuda_ਧਾਰਾ_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#ਜੇ_ਅੰਤ

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

ਵਰਗ cv::ogl::ਬਫਰ_2::Impl
{
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::ogl::ਬਫਰ_2::Impl
{
ਜਨਤਕ:
    ਸਥਿਰ ਅਚਲ Ptr<Impl>& empty();

    Impl(GLuint bufId, ਤਾਰਕਿਕ autoRelease);
    Impl(GLsizeiptr size, ਅਚਲ GLvoid* ਅੰਕੜੇ, GLenum target, ਤਾਰਕਿਕ autoRelease);
    ~Impl();

    ਖਾਲੀ bind(GLenum target) ਅਚਲ;

    ਖਾਲੀ copyFrom(GLuint srcBuf, GLsizeiptr size);

    ਖਾਲੀ copyFrom(GLsizeiptr size, ਅਚਲ GLvoid* ਅੰਕੜੇ);
    ਖਾਲੀ copy_ਮੰਜ਼ਿਲ(GLsizeiptr size, GLvoid* ਅੰਕੜੇ) ਅਚਲ;

    ਖਾਲੀ resize(GLsizeiptr size, GLenum target);

    ਖਾਲੀ* mapHost(GLenum access);
    ਖਾਲੀ unmapHost();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਖਾਲੀ copyFrom(ਅਚਲ ਖਾਲੀ* src, size_t spitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream = 0);
    ਖਾਲੀ copy_ਮੰਜ਼ਿਲ(ਖਾਲੀ* dst, size_t dpitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream = 0) ਅਚਲ;

    ਖਾਲੀ* mapDevice(cuda_ਧਾਰਾ_t stream = 0);
    ਖਾਲੀ unmapDevice(cuda_ਧਾਰਾ_t stream = 0);
#ਜੇ_ਅੰਤ

    ਖਾਲੀ ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag) { autoRelease_ = flag; }

    GLuint bufId() ਅਚਲ { ਵਾਪਸ bufId_; }

ਨਿਜੀ:
    Impl();

    GLuint bufId_;
    ਤਾਰਕਿਕ autoRelease_;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਬਦਲਣਯੋਗ CudaResource cudaResource_;
#ਜੇ_ਅੰਤ
};

ਅਚਲ Ptr<cv::ogl::ਬਫਰ_2::Impl>& cv::ogl::ਬਫਰ_2::Impl::empty()
{
    ਸਥਿਰ Ptr<Impl> p(ਨਵਾਂ Impl);
    ਵਾਪਸ p;
}

cv::ogl::ਬਫਰ_2::Impl::Impl() : bufId_(0), autoRelease_(ਝੂਠ)
{
}

cv::ogl::ਬਫਰ_2::Impl::Impl(GLuint abufId, ਤਾਰਕਿਕ autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_ਬਫਰ(abufId) == gl::TRUE_ );
}

cv::ogl::ਬਫਰ_2::Impl::Impl(GLsizeiptr size, ਅਚਲ GLvoid* ਅੰਕੜੇ, GLenum target, ਤਾਰਕਿਕ autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_ਬਫਰ(target, bufId_);
    CV_CheckGlError();

    gl::ਬਫਰ_ਅੰਕੜੇ(target, size, ਅੰਕੜੇ, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_ਬਫਰ(target, 0);
    CV_CheckGlError();
}

cv::ogl::ਬਫਰ_2::Impl::~Impl()
{
    ਜੇ (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::bind(GLenum target) ਅਚਲ
{
    gl::Bind_ਬਫਰ(target, bufId_);
    CV_CheckGlError();
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_ਬਫਰ(gl::COPY_WRITE_ਬਫਰ, bufId_);
    CV_CheckGlError();

    gl::Bind_ਬਫਰ(gl::COPY_READ_ਬਫਰ, srcBuf);
    CV_CheckGlError();

    gl::Copy_ਬਫਰ_Sub_ਅੰਕੜੇ(gl::COPY_READ_ਬਫਰ, gl::COPY_WRITE_ਬਫਰ, 0, 0, size);
    CV_CheckGlError();
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::copyFrom(GLsizeiptr size, ਅਚਲ GLvoid* ਅੰਕੜੇ)
{
    gl::Bind_ਬਫਰ(gl::COPY_WRITE_ਬਫਰ, bufId_);
    CV_CheckGlError();

    gl::ਬਫਰ_Sub_ਅੰਕੜੇ(gl::COPY_WRITE_ਬਫਰ, 0, size, ਅੰਕੜੇ);
    CV_CheckGlError();
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_ਬਫਰ(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::ਬਫਰ_ਅੰਕੜੇ(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_ਬਫਰ(target, 0);
    CV_CheckGlError();
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::copy_ਮੰਜ਼ਿਲ(GLsizeiptr size, GLvoid* ਅੰਕੜੇ) ਅਚਲ
{
    gl::Bind_ਬਫਰ(gl::COPY_READ_ਬਫਰ, bufId_);
    CV_CheckGlError();

    gl::Get_ਬਫਰ_Sub_ਅੰਕੜੇ(gl::COPY_READ_ਬਫਰ, 0, size, ਅੰਕੜੇ);
    CV_CheckGlError();
}

ਖਾਲੀ* cv::ogl::ਬਫਰ_2::Impl::mapHost(GLenum access)
{
    gl::Bind_ਬਫਰ(gl::COPY_READ_ਬਫਰ, bufId_);
    CV_CheckGlError();

    GLvoid* ਅੰਕੜੇ = gl::Map_ਬਫਰ(gl::COPY_READ_ਬਫਰ, access);
    CV_CheckGlError();

    ਵਾਪਸ ਅੰਕੜੇ;
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::unmapHost()
{
    gl::Unmap_ਬਫਰ(gl::COPY_READ_ਬਫਰ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::copyFrom(ਅਚਲ ਖਾਲੀ* src, size_t spitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream)
{
    cudaResource_.ਰਜਿਸਟਰ_ਬਫਰ(bufId_);
    cudaResource_.copyFrom(src, spitch, ਚੌੜਾਈ, ਉਚਾਈ, stream);
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::copy_ਮੰਜ਼ਿਲ(ਖਾਲੀ* dst, size_t dpitch, size_t ਚੌੜਾਈ, size_t ਉਚਾਈ, cuda_ਧਾਰਾ_t stream) ਅਚਲ
{
    cudaResource_.ਰਜਿਸਟਰ_ਬਫਰ(bufId_);
    cudaResource_.copy_ਮੰਜ਼ਿਲ(dst, dpitch, ਚੌੜਾਈ, ਉਚਾਈ, stream);
}

ਖਾਲੀ* cv::ogl::ਬਫਰ_2::Impl::mapDevice(cuda_ਧਾਰਾ_t stream)
{
    cudaResource_.ਰਜਿਸਟਰ_ਬਫਰ(bufId_);
    ਵਾਪਸ cudaResource_.map(stream);
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::Impl::unmapDevice(cuda_ਧਾਰਾ_t stream)
{
    cudaResource_.unmap(stream);
}

#ਜੇ_ਅੰਤ // HAVE_CUDA

#ਜੇ_ਅੰਤ // HAVE_OPENGL

cv::ogl::ਬਫਰ_2::ਬਫਰ_2() : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), type_(0)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

cv::ogl::ਬਫਰ_2::ਬਫਰ_2(ਪੂਰਨ arows, ਪੂਰਨ acols, ਪੂਰਨ atype, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ abufId, ਤਾਰਕਿਕ autoRelease) : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), type_(0)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arows);
    CV_ਅਣਵਰਤਿਆ(acols);
    CV_ਅਣਵਰਤਿਆ(atype);
    CV_ਅਣਵਰਤਿਆ(abufId);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(abufId, autoRelease));
    ਪੰਕਤੀਆਂ_2 = arows;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = acols;
    type_ = atype;
#ਜੇ_ਅੰਤ
}

cv::ogl::ਬਫਰ_2::ਬਫਰ_2(ਮਾਪ asize, ਪੂਰਨ atype, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ abufId, ਤਾਰਕਿਕ autoRelease) : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), type_(0)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(asize);
    CV_ਅਣਵਰਤਿਆ(atype);
    CV_ਅਣਵਰਤਿਆ(abufId);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(abufId, autoRelease));
    ਪੰਕਤੀਆਂ_2 = asize.ਉਚਾਈ;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = asize.ਚੌੜਾਈ;
    type_ = atype;
#ਜੇ_ਅੰਤ
}

cv::ogl::ਬਫਰ_2::ਬਫਰ_2(ਨਿਵੇਸ਼_Array arr, ਨਿਸ਼ਾਨਾ target, ਤਾਰਕਿਕ autoRelease) : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), type_(0)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(target);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ ਕਿਸਮ = arr.ਕਿਸਮ();

    ਚੋਣ (ਕਿਸਮ)
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ:
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        ਰੋਕੋ;

    ਮੂਲ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ਅਚਲ GLsizeiptr asize = mat.ਪੰਕਤੀਆਂ * mat.ਖਾਨੇ_ਗਿਣਤੀ * mat.elem_ਮਾਪ();
            impl_.reset(ਨਵਾਂ Impl(asize, mat.ਅੰਕੜੇ, target, autoRelease));
            ਪੰਕਤੀਆਂ_2 = mat.ਪੰਕਤੀਆਂ;
            ਖਾਨੇ_ਗਿਣਤੀ_2 = mat.ਖਾਨੇ_ਗਿਣਤੀ;
            type_ = mat.type();
            ਰੋਕੋ;
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::create(ਪੂਰਨ arows, ਪੂਰਨ acols, ਪੂਰਨ atype, ਨਿਸ਼ਾਨਾ target, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arows);
    CV_ਅਣਵਰਤਿਆ(acols);
    CV_ਅਣਵਰਤਿਆ(atype);
    CV_ਅਣਵਰਤਿਆ(target);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਜੇ (ਪੰਕਤੀਆਂ_2 != arows || ਖਾਨੇ_ਗਿਣਤੀ_2 != acols || type_ != atype)
    {
        ਅਚਲ GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_ਮਾਪ(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        ਜੇ (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->ਸੈੱਟ_Auto_Release(autoRelease);
        }
        ਨਹੀਂਤਾਂ
            impl_.reset(ਨਵਾਂ Impl(asize, 0, target, autoRelease));
        ਪੰਕਤੀਆਂ_2 = arows;
        ਖਾਨੇ_ਗਿਣਤੀ_2 = acols;
        type_ = atype;
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    ਜੇ (impl_)
        impl_->ਸੈੱਟ_Auto_Release(ਸੱਚ);
    impl_ = Impl::empty();
    ਪੰਕਤੀਆਂ_2 = 0;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = 0;
    type_ = 0;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::copyFrom(ਨਿਵੇਸ਼_Array arr, ਨਿਸ਼ਾਨਾ target, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(target);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ ਕਿਸਮ = arr.ਕਿਸਮ();

    ਅਚਲ ਮਾਪ asize = arr.size();
    ਅਚਲ ਪੂਰਨ atype = arr.type();
    create(asize, atype, target, autoRelease);

    ਚੋਣ (ਕਿਸਮ)
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ:
        {
            ogl::ਬਫਰ_2 buf = arr.get_OGl_ਬਫਰ();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_ਮਾਪ(atype));
            ਰੋਕੋ;
        }

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        {
            #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
                throw_no_cuda();
            #ਨਹੀਂਤਾਂ
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.ਅੰਕੜੇ, dmat.ਕਦਮ, dmat.ਖਾਨੇ_ਗਿਣਤੀ * dmat.elem_ਮਾਪ(), dmat.ਪੰਕਤੀਆਂ);
            #ਜੇ_ਅੰਤ

            ਰੋਕੋ;
        }

    ਮੂਲ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_ਮਾਪ(atype), mat.ਅੰਕੜੇ);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::copyFrom(ਨਿਵੇਸ਼_Array arr, cuda::ਧਾਰਾ& stream, ਨਿਸ਼ਾਨਾ target, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(stream);
    CV_ਅਣਵਰਤਿਆ(target);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        CV_ਅਣਵਰਤਿਆ(arr);
        CV_ਅਣਵਰਤਿਆ(stream);
        CV_ਅਣਵਰਤਿਆ(target);
        CV_ਅਣਵਰਤਿਆ(autoRelease);
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.ਅੰਕੜੇ, dmat.ਕਦਮ, dmat.ਖਾਨੇ_ਗਿਣਤੀ * dmat.elem_ਮਾਪ(), dmat.ਪੰਕਤੀਆਂ, cuda::ਧਾਰਾ_Accessor::get_ਧਾਰਾ(stream));
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::copy_ਮੰਜ਼ਿਲ(OutputArray arr) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ ਕਿਸਮ = arr.ਕਿਸਮ();

    ਚੋਣ (ਕਿਸਮ)
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ:
        {
            arr.get_OGl_ਬਫਰ_Ref().copyFrom(*ਇਹ);
            ਰੋਕੋ;
        }

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        {
            #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
                throw_no_cuda();
            #ਨਹੀਂਤਾਂ
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type_);
                impl_->copy_ਮੰਜ਼ਿਲ(dmat.ਅੰਕੜੇ, dmat.ਕਦਮ, dmat.ਖਾਨੇ_ਗਿਣਤੀ * dmat.elem_ਮਾਪ(), dmat.ਪੰਕਤੀਆਂ);
            #ਜੇ_ਅੰਤ

            ਰੋਕੋ;
        }

    ਮੂਲ:
        {
            arr.create(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_ਮੰਜ਼ਿਲ(mat.ਪੰਕਤੀਆਂ * mat.ਖਾਨੇ_ਗਿਣਤੀ * mat.elem_ਮਾਪ(), mat.ਅੰਕੜੇ);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::copy_ਮੰਜ਼ਿਲ(OutputArray arr, cuda::ਧਾਰਾ& stream) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        CV_ਅਣਵਰਤਿਆ(arr);
        CV_ਅਣਵਰਤਿਆ(stream);
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        arr.create(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_ਮੰਜ਼ਿਲ(dmat.ਅੰਕੜੇ, dmat.ਕਦਮ, dmat.ਖਾਨੇ_ਗਿਣਤੀ * dmat.elem_ਮਾਪ(), dmat.ਪੰਕਤੀਆਂ, cuda::ਧਾਰਾ_Accessor::get_ਧਾਰਾ(stream));
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

cv::ogl::ਬਫਰ_2 cv::ogl::ਬਫਰ_2::clone(ਨਿਸ਼ਾਨਾ target, ਤਾਰਕਿਕ autoRelease) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(target);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ogl::ਬਫਰ_2 buf;
    buf.copyFrom(*ਇਹ, target, autoRelease);
    ਵਾਪਸ buf;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::bind(ਨਿਸ਼ਾਨਾ target) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(target);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->bind(target);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::unbind(ਨਿਸ਼ਾਨਾ target)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(target);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Bind_ਬਫਰ(target, 0);
    CV_CheckGlError();
#ਜੇ_ਅੰਤ
}

Mat cv::ogl::ਬਫਰ_2::mapHost(Access access)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(access);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ Mat(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type_, impl_->mapHost(access));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::unmapHost()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->unmapHost();
#ਜੇ_ਅੰਤ
}

GpuMat cv::ogl::ਬਫਰ_2::mapDevice()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        ਵਾਪਸ GpuMat(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type_, impl_->mapDevice());
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::unmapDevice()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        impl_->unmapDevice();
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

cuda::GpuMat cv::ogl::ਬਫਰ_2::mapDevice(cuda::ਧਾਰਾ& stream)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        CV_ਅਣਵਰਤਿਆ(stream);
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        ਵਾਪਸ GpuMat(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, type_, impl_->mapDevice(cuda::ਧਾਰਾ_Accessor::get_ਧਾਰਾ(stream)));
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਬਫਰ_2::unmapDevice(cuda::ਧਾਰਾ& stream)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        CV_ਅਣਵਰਤਿਆ(stream);
        throw_no_cuda();
    #ਨਹੀਂਤਾਂ
        impl_->unmapDevice(cuda::ਧਾਰਾ_Accessor::get_ਧਾਰਾ(stream));
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv::ogl::ਬਫਰ_2::bufId() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->bufId();
#ਜੇ_ਅੰਤ
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

ਵਰਗ cv::ogl::Texture2D::Impl
{
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::ogl::Texture2D::Impl
{
ਜਨਤਕ:
    ਸਥਿਰ ਅਚਲ Ptr<Impl> empty();

    Impl(GLuint texId, ਤਾਰਕਿਕ autoRelease);
    Impl(GLint ਅੰਦਰੂਨੀ_Format, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid* pixels, ਤਾਰਕਿਕ autoRelease);
    ~Impl();

    ਖਾਲੀ copyFrom(GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels);
    ਖਾਲੀ copy_ਮੰਜ਼ਿਲ(GLenum format, GLenum type, GLvoid* pixels) ਅਚਲ;

    ਖਾਲੀ bind() ਅਚਲ;

    ਖਾਲੀ ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag) { autoRelease_ = flag; }

    GLuint texId() ਅਚਲ { ਵਾਪਸ texId_; }

ਨਿਜੀ:
    Impl();

    GLuint texId_;
    ਤਾਰਕਿਕ autoRelease_;
};

ਅਚਲ Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ਸਥਿਰ Ptr<Impl> p(ਨਵਾਂ Impl);
    ਵਾਪਸ p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(ਝੂਠ)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, ਤਾਰਕਿਕ autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint ਅੰਦਰੂਨੀ_Format, GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid* pixels, ਤਾਰਕਿਕ autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, ਅੰਦਰੂਨੀ_Format, ਚੌੜਾਈ, ਉਚਾਈ, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    ਜੇ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ਖਾਲੀ cv::ogl::Texture2D::Impl::copyFrom(GLsizei ਚੌੜਾਈ, GLsizei ਉਚਾਈ, GLenum format, GLenum type, ਅਚਲ GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, ਚੌੜਾਈ, ਉਚਾਈ, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

ਖਾਲੀ cv::ogl::Texture2D::Impl::copy_ਮੰਜ਼ਿਲ(GLenum format, GLenum type, GLvoid* pixels) ਅਚਲ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

ਖਾਲੀ cv::ogl::Texture2D::Impl::bind() ਅਚਲ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#ਜੇ_ਅੰਤ // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), format_(NONE)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

cv::ogl::Texture2D::Texture2D(ਪੂਰਨ arows, ਪੂਰਨ acols, Format aformat, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ atexId, ਤਾਰਕਿਕ autoRelease) : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), format_(NONE)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arows);
    CV_ਅਣਵਰਤਿਆ(acols);
    CV_ਅਣਵਰਤਿਆ(aformat);
    CV_ਅਣਵਰਤਿਆ(atexId);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(atexId, autoRelease));
    ਪੰਕਤੀਆਂ_2 = arows;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = acols;
    format_ = aformat;
#ਜੇ_ਅੰਤ
}

cv::ogl::Texture2D::Texture2D(ਮਾਪ asize, Format aformat, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ atexId, ਤਾਰਕਿਕ autoRelease) : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), format_(NONE)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(asize);
    CV_ਅਣਵਰਤਿਆ(aformat);
    CV_ਅਣਵਰਤਿਆ(atexId);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(atexId, autoRelease));
    ਪੰਕਤੀਆਂ_2 = asize.ਉਚਾਈ;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = asize.ਚੌੜਾਈ;
    format_ = aformat;
#ਜੇ_ਅੰਤ
}

cv::ogl::Texture2D::Texture2D(ਨਿਵੇਸ਼_Array arr, ਤਾਰਕਿਕ autoRelease) : ਪੰਕਤੀਆਂ_2(0), ਖਾਨੇ_ਗਿਣਤੀ_2(0), format_(NONE)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ ਕਿਸਮ = arr.ਕਿਸਮ();

    ਅਚਲ ਮਾਪ asize = arr.size();
    ਅਚਲ ਪੂਰਨ atype = arr.type();

    ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(atype);
    ਅਚਲ ਪੂਰਨ cn = CV_MAT_CN(atype);

    CV_Assert( ਡੂੰਘਾਈ <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ਅਚਲ Format ਅੰਦਰੂਨੀ_Formats[] =
    {
        NONE, ਡੂੰਘਾਈ_COMPONENT, NONE, RGB, RGBA
    };
    ਅਚਲ GLenum srcFormats[] =
    {
        0, gl::ਡੂੰਘਾਈ_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    ਚੋਣ (ਕਿਸਮ)
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ:
        {
            ogl::ਬਫਰ_2 buf = arr.get_OGl_ਬਫਰ();
            buf.bind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            impl_.reset(ਨਵਾਂ Impl(ਅੰਦਰੂਨੀ_Formats[cn], asize.ਚੌੜਾਈ, asize.ਉਚਾਈ, srcFormats[cn], gl_types[ਡੂੰਘਾਈ], 0, autoRelease));
            ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            ਰੋਕੋ;
        }

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        {
            #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
                throw_no_cuda();
            #ਨਹੀਂਤਾਂ
                GpuMat dmat = arr.getGpuMat();
                ogl::ਬਫਰ_2 buf(dmat, ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
                buf.ਸੈੱਟ_Auto_Release(ਸੱਚ);
                buf.bind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
                impl_.reset(ਨਵਾਂ Impl(ਅੰਦਰੂਨੀ_Formats[cn], asize.ਚੌੜਾਈ, asize.ਉਚਾਈ, srcFormats[cn], gl_types[ਡੂੰਘਾਈ], 0, autoRelease));
                ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            #ਜੇ_ਅੰਤ

            ਰੋਕੋ;
        }

    ਮੂਲ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            impl_.reset(ਨਵਾਂ Impl(ਅੰਦਰੂਨੀ_Formats[cn], asize.ਚੌੜਾਈ, asize.ਉਚਾਈ, srcFormats[cn], gl_types[ਡੂੰਘਾਈ], mat.ਅੰਕੜੇ, autoRelease));
            ਰੋਕੋ;
        }
    }

    ਪੰਕਤੀਆਂ_2 = asize.ਉਚਾਈ;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = asize.ਚੌੜਾਈ;
    format_ = ਅੰਦਰੂਨੀ_Formats[cn];
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture2D::create(ਪੂਰਨ arows, ਪੂਰਨ acols, Format aformat, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arows);
    CV_ਅਣਵਰਤਿਆ(acols);
    CV_ਅਣਵਰਤਿਆ(aformat);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਜੇ (ਪੰਕਤੀਆਂ_2 != arows || ਖਾਨੇ_ਗਿਣਤੀ_2 != acols || format_ != aformat)
    {
        ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
        impl_.reset(ਨਵਾਂ Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        ਪੰਕਤੀਆਂ_2 = arows;
        ਖਾਨੇ_ਗਿਣਤੀ_2 = acols;
        format_ = aformat;
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture2D::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    ਜੇ (impl_)
        impl_->ਸੈੱਟ_Auto_Release(ਸੱਚ);
    impl_ = Impl::empty();
    ਪੰਕਤੀਆਂ_2 = 0;
    ਖਾਨੇ_ਗਿਣਤੀ_2 = 0;
    format_ = NONE;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture2D::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture2D::copyFrom(ਨਿਵੇਸ਼_Array arr, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ ਕਿਸਮ = arr.ਕਿਸਮ();

    ਅਚਲ ਮਾਪ asize = arr.size();
    ਅਚਲ ਪੂਰਨ atype = arr.type();

    ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(atype);
    ਅਚਲ ਪੂਰਨ cn = CV_MAT_CN(atype);

    CV_Assert( ਡੂੰਘਾਈ <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ਅਚਲ Format ਅੰਦਰੂਨੀ_Formats[] =
    {
        NONE, ਡੂੰਘਾਈ_COMPONENT, NONE, RGB, RGBA
    };
    ਅਚਲ GLenum srcFormats[] =
    {
        0, gl::ਡੂੰਘਾਈ_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, ਅੰਦਰੂਨੀ_Formats[cn], autoRelease);

    ਚੋਣ(ਕਿਸਮ)
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ:
        {
            ogl::ਬਫਰ_2 buf = arr.get_OGl_ਬਫਰ();
            buf.bind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            impl_->copyFrom(asize.ਚੌੜਾਈ, asize.ਉਚਾਈ, srcFormats[cn], gl_types[ਡੂੰਘਾਈ], 0);
            ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            ਰੋਕੋ;
        }

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        {
            #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
                throw_no_cuda();
            #ਨਹੀਂਤਾਂ
                GpuMat dmat = arr.getGpuMat();
                ogl::ਬਫਰ_2 buf(dmat, ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
                buf.ਸੈੱਟ_Auto_Release(ਸੱਚ);
                buf.bind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
                impl_->copyFrom(asize.ਚੌੜਾਈ, asize.ਉਚਾਈ, srcFormats[cn], gl_types[ਡੂੰਘਾਈ], 0);
                ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            #ਜੇ_ਅੰਤ

            ਰੋਕੋ;
        }

    ਮੂਲ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_UNPACK_ਬਫਰ);
            impl_->copyFrom(asize.ਚੌੜਾਈ, asize.ਉਚਾਈ, srcFormats[cn], gl_types[ਡੂੰਘਾਈ], mat.ਅੰਕੜੇ);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture2D::copy_ਮੰਜ਼ਿਲ(OutputArray arr, ਪੂਰਨ ddepth, ਤਾਰਕਿਕ autoRelease) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(ddepth);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ ਕਿਸਮ = arr.ਕਿਸਮ();

    ਅਚਲ ਪੂਰਨ cn = format_ == ਡੂੰਘਾਈ_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ਅਚਲ GLenum dstFormat = format_ == ਡੂੰਘਾਈ_COMPONENT ? gl::ਡੂੰਘਾਈ_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    ਚੋਣ(ਕਿਸਮ)
    {
    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ:
        {
            ogl::ਬਫਰ_2& buf = arr.get_OGl_ਬਫਰ_Ref();
            buf.create(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, CV_MAKE_TYPE(ddepth, cn), ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ, autoRelease);
            buf.bind(ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ);
            impl_->copy_ਮੰਜ਼ਿਲ(dstFormat, gl_types[ddepth], 0);
            ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ);
            ਰੋਕੋ;
        }

    ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT:
        {
            #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
                throw_no_cuda();
            #ਨਹੀਂਤਾਂ
                ogl::ਬਫਰ_2 buf(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, CV_MAKE_TYPE(ddepth, cn), ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ);
                buf.ਸੈੱਟ_Auto_Release(ਸੱਚ);
                buf.bind(ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ);
                impl_->copy_ਮੰਜ਼ਿਲ(dstFormat, gl_types[ddepth], 0);
                ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ);
                buf.copy_ਮੰਜ਼ਿਲ(arr);
            #ਜੇ_ਅੰਤ

            ਰੋਕੋ;
        }

    ਮੂਲ:
        {
            arr.create(ਪੰਕਤੀਆਂ_2, ਖਾਨੇ_ਗਿਣਤੀ_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::PIXEL_PACK_ਬਫਰ);
            impl_->copy_ਮੰਜ਼ਿਲ(dstFormat, gl_types[ddepth], mat.ਅੰਕੜੇ);
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture2D::bind() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->bind();
#ਜੇ_ਅੰਤ
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv::ogl::Texture2D::texId() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->texId();
#ਜੇ_ਅੰਤ
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

ਵਰਗ cv::ogl::Texture_ਬਫਰ::Impl
{
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::ogl::Texture_ਬਫਰ::Impl
{
ਜਨਤਕ:
    ਸਥਿਰ ਅਚਲ Ptr<Impl> empty();

    Impl();
    Impl(GLenum ਅੰਦਰੂਨੀ_Format, GLuint bufId, ਤਾਰਕਿਕ autoRelease);
    ~Impl();

    ਖਾਲੀ bind(GLint unit) ਅਚਲ;

    ਖਾਲੀ ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag) { autoRelease_ = flag; }

    GLuint texId() ਅਚਲ { ਵਾਪਸ texId_; }

ਨਿਜੀ:
    GLuint texId_;
    ਤਾਰਕਿਕ autoRelease_;
};

ਅਚਲ Ptr<cv::ogl::Texture_ਬਫਰ::Impl> cv::ogl::Texture_ਬਫਰ::Impl::empty()
{
    ਸਥਿਰ Ptr<Impl> p = makePtr<Impl>();
    ਵਾਪਸ p;
}

cv::ogl::Texture_ਬਫਰ::Impl::Impl() : texId_(0), autoRelease_(ਝੂਠ)
{
}

cv::ogl::Texture_ਬਫਰ::Impl::Impl(GLenum ਅੰਦਰੂਨੀ_Format, GLuint bufId, ਤਾਰਕਿਕ autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_ਬਫਰ, texId_);
    CV_CheckGlError();

    gl::Tex_ਬਫਰ(gl::TEXTURE_ਬਫਰ, ਅੰਦਰੂਨੀ_Format, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_ਬਫਰ, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_ਬਫਰ::Impl::~Impl()
{
    ਜੇ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ਖਾਲੀ cv::ogl::Texture_ਬਫਰ::Impl::bind(GLint unit) ਅਚਲ
{
    gl::ਸਰਗਰਮ_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_ਬਫਰ, texId_);
    CV_CheckGlError();
}

#ਜੇ_ਅੰਤ // HAVE_OPENGL

cv::ogl::Texture_ਬਫਰ::Texture_ਬਫਰ() : format_(NONE)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

cv::ogl::Texture_ਬਫਰ::Texture_ਬਫਰ(ਅਚਲ ਬਫਰ_2& buf, Format aformat, ਤਾਰਕਿਕ autoRelease) : format_(NONE)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(buf);
    CV_ਅਣਵਰਤਿਆ(aformat);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture_ਬਫਰ::create(ਅਚਲ ਬਫਰ_2& buf, Format aformat, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(buf);
    CV_ਅਣਵਰਤਿਆ(aformat);
    CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture_ਬਫਰ::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    ਜੇ (impl_)
        impl_->ਸੈੱਟ_Auto_Release(ਸੱਚ);
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
    buf_ = ਬਫਰ_2();
    format_ = NONE;
}

ਖਾਲੀ cv::ogl::Texture_ਬਫਰ::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Texture_ਬਫਰ::bind(ਪੂਰਨ unit) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(unit);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#ਜੇ_ਅੰਤ
}

cv::ogl::Texture_ਬਫਰ::Format cv::ogl::Texture_ਬਫਰ::format() ਅਚਲ
{
    ਵਾਪਸ format_;
}

ਤਾਰਕਿਕ cv::ogl::Texture_ਬਫਰ::empty() ਅਚਲ
{
    ਵਾਪਸ format_ == NONE;
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv::ogl::Texture_ਬਫਰ::texId() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->texId();
#ਜੇ_ਅੰਤ
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

ਖਾਲੀ cv::ogl::Arrays::ਸੈੱਟ_ਸ਼ੀਰਸ਼_Array(ਨਿਵੇਸ਼_Array ਸ਼ੀਰਸ਼)
{
    ਅਚਲ ਪੂਰਨ cn = ਸ਼ੀਰਸ਼.channels();
    ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = ਸ਼ੀਰਸ਼.ਡੂੰਘਾਈ();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( ਡੂੰਘਾਈ == CV_16S || ਡੂੰਘਾਈ == CV_32S || ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F );

    ਜੇ (ਸ਼ੀਰਸ਼.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ)
        ਸ਼ੀਰਸ਼_2 = ਸ਼ੀਰਸ਼.get_OGl_ਬਫਰ();
    ਨਹੀਂਤਾਂ
        ਸ਼ੀਰਸ਼_2.copyFrom(ਸ਼ੀਰਸ਼);

    ਮਾਪ_3 = ਸ਼ੀਰਸ਼_2.size().area();
}

ਖਾਲੀ cv::ogl::Arrays::reset_ਸ਼ੀਰਸ਼_Array()
{
    ਸ਼ੀਰਸ਼_2.release();
    ਮਾਪ_3 = 0;
}

ਖਾਲੀ cv::ogl::Arrays::ਸੈੱਟ_Color_Array(ਨਿਵੇਸ਼_Array color)
{
    ਅਚਲ ਪੂਰਨ cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    ਜੇ (color.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ)
        color_ = color.get_OGl_ਬਫਰ();
    ਨਹੀਂਤਾਂ
        color_.copyFrom(color);
}

ਖਾਲੀ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

ਖਾਲੀ cv::ogl::Arrays::ਸੈੱਟ_ਸਾਧਾਰਨ_Array(ਨਿਵੇਸ਼_Array ਸਾਧਾਰਨ)
{
    ਅਚਲ ਪੂਰਨ cn = ਸਾਧਾਰਨ.channels();
    ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = ਸਾਧਾਰਨ.ਡੂੰਘਾਈ();

    CV_Assert( cn == 3 );
    CV_Assert( ਡੂੰਘਾਈ == CV_8S || ਡੂੰਘਾਈ == CV_16S || ਡੂੰਘਾਈ == CV_32S || ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F );

    ਜੇ (ਸਾਧਾਰਨ.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ)
        ਸਾਧਾਰਨ_2 = ਸਾਧਾਰਨ.get_OGl_ਬਫਰ();
    ਨਹੀਂਤਾਂ
        ਸਾਧਾਰਨ_2.copyFrom(ਸਾਧਾਰਨ);
}

ਖਾਲੀ cv::ogl::Arrays::reset_ਸਾਧਾਰਨ_Array()
{
    ਸਾਧਾਰਨ_2.release();
}

ਖਾਲੀ cv::ogl::Arrays::ਸੈੱਟ_Tex_Coord_Array(ਨਿਵੇਸ਼_Array texCoord)
{
    ਅਚਲ ਪੂਰਨ cn = texCoord.channels();
    ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = texCoord.ਡੂੰਘਾਈ();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( ਡੂੰਘਾਈ == CV_16S || ਡੂੰਘਾਈ == CV_32S || ਡੂੰਘਾਈ == CV_32F || ਡੂੰਘਾਈ == CV_64F );

    ਜੇ (texCoord.ਕਿਸਮ() == ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ)
        texCoord_ = texCoord.get_OGl_ਬਫਰ();
    ਨਹੀਂਤਾਂ
        texCoord_.copyFrom(texCoord);
}

ਖਾਲੀ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

ਖਾਲੀ cv::ogl::Arrays::release()
{
    reset_ਸ਼ੀਰਸ਼_Array();
    resetColorArray();
    reset_ਸਾਧਾਰਨ_Array();
    resetTexCoordArray();
}

ਖਾਲੀ cv::ogl::Arrays::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਸ਼ੀਰਸ਼_2.ਸੈੱਟ_Auto_Release(flag);
    color_.ਸੈੱਟ_Auto_Release(flag);
    ਸਾਧਾਰਨ_2.ਸੈੱਟ_Auto_Release(flag);
    texCoord_.ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Arrays::bind() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == ਮਾਪ_3 );
    CV_Assert( ਸਾਧਾਰਨ_2.empty() || ਸਾਧਾਰਨ_2.size().area() == ਮਾਪ_3 );
    CV_Assert( color_.empty() || color_.size().area() == ਮਾਪ_3 );

    ਜੇ (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    ਨਹੀਂਤਾਂ
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::ਬਫਰ_2::ARRAY_ਬਫਰ);

        gl::Tex_Coord_ਸੂਚਕ(texCoord_.channels(), gl_types[texCoord_.ਡੂੰਘਾਈ()], 0, 0);
        CV_CheckGlError();
    }

    ਜੇ (ਸਾਧਾਰਨ_2.empty())
    {
        gl::DisableClientState(gl::ਸਾਧਾਰਨ_ARRAY);
        CV_CheckGlError();
    }
    ਨਹੀਂਤਾਂ
    {
        gl::EnableClientState(gl::ਸਾਧਾਰਨ_ARRAY);
        CV_CheckGlError();

        ਸਾਧਾਰਨ_2.bind(ogl::ਬਫਰ_2::ARRAY_ਬਫਰ);

        gl::ਸਾਧਾਰਨ_ਸੂਚਕ(gl_types[ਸਾਧਾਰਨ_2.ਡੂੰਘਾਈ()], 0, 0);
        CV_CheckGlError();
    }

    ਜੇ (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    ਨਹੀਂਤਾਂ
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::ਬਫਰ_2::ARRAY_ਬਫਰ);

        ਅਚਲ ਪੂਰਨ cn = color_.channels();

        gl::Color_ਸੂਚਕ(cn, gl_types[color_.ਡੂੰਘਾਈ()], 0, 0);
        CV_CheckGlError();
    }

    ਜੇ (ਸ਼ੀਰਸ਼_2.empty())
    {
        gl::DisableClientState(gl::ਸ਼ੀਰਸ਼_ARRAY);
        CV_CheckGlError();
    }
    ਨਹੀਂਤਾਂ
    {
        gl::EnableClientState(gl::ਸ਼ੀਰਸ਼_ARRAY);
        CV_CheckGlError();

        ਸ਼ੀਰਸ਼_2.bind(ogl::ਬਫਰ_2::ARRAY_ਬਫਰ);

        gl::ਸ਼ੀਰਸ਼_ਸੂਚਕ(ਸ਼ੀਰਸ਼_2.channels(), gl_types[ਸ਼ੀਰਸ਼_2.ਡੂੰਘਾਈ()], 0, 0);
        CV_CheckGlError();
    }

    ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::ARRAY_ਬਫਰ);
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

ਵਰਗ cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl
{
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl
{
ਜਨਤਕ:
    ਸਥਿਰ ਅਚਲ Ptr<Impl>& empty();

    Impl(GLuint vaId, ਤਾਰਕਿਕ autoRelease);
    Impl(std::initializer_list<Attribute> ਗੁਣ, ਤਾਰਕਿਕ autoRelease);
    ~Impl();

    ਖਾਲੀ bind();

    ਖਾਲੀ ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag) { autoRelease_ = flag; }

    GLuint vaId() ਅਚਲ { ਵਾਪਸ vaId_; }

ਨਿਜੀ:
    Impl();

    GLuint vaId_;
    ਤਾਰਕਿਕ autoRelease_;
};

ਅਚਲ Ptr<cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl>& cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl::empty()
{
    ਸਥਿਰ Ptr<Impl> p(ਨਵਾਂ Impl);
    ਵਾਪਸ p;
}

cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl::Impl() : vaId_(0), autoRelease_(ਝੂਠ)
{
}

cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl::Impl(GLuint vaId, ਤਾਰਕਿਕ autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_ਸ਼ੀਰਸ਼_Array(vaId) == gl::TRUE_ );
}

cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl::Impl(std::initializer_list<Attribute> ਗੁਣ, ਤਾਰਕਿਕ autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_ਸ਼ੀਰਸ਼_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_ਸ਼ੀਰਸ਼_Array(vaId_);
    CV_CheckGlError();

    ਲਈ (ਸਵੈਚਾਲਿਤ& attribute : ਗੁਣ)
    {
        attribute.ਬਫਰ_3.bind(ਬਫਰ_2::ARRAY_ਬਫਰ);
        ਜੇ (attribute.integer_)
        {
            gl::ਸ਼ੀਰਸ਼_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.ਮਾਪ_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ਅਚਲ ਖਾਲੀ*>(attribute.ਵਿੱਥ)
            );
            CV_CheckGlError();
        }
        ਨਹੀਂਤਾਂ
        {
            gl::ਸ਼ੀਰਸ਼_Attrib_ਸੂਚਕ(
                attribute.shader_loc_,
                attribute.ਮਾਪ_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ਅਚਲ ਖਾਲੀ*>(attribute.ਵਿੱਥ)
            );
            CV_CheckGlError();
        }

        gl::Enable_ਸ਼ੀਰਸ਼_Attrib_Array(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl::~Impl()
{
    ਜੇ (autoRelease_ && vaId_)
        gl::Delete_ਸ਼ੀਰਸ਼_Arrays(1, &vaId_);
}

ਖਾਲੀ cv::ogl::ਸ਼ੀਰਸ਼_Array::Impl::bind()
{
    gl::Bind_ਸ਼ੀਰਸ਼_Array(vaId_);
    CV_CheckGlError();
}

#ਜੇ_ਅੰਤ // HAVE_OPENGL

cv::ogl::ਸ਼ੀਰਸ਼_Array::ਸ਼ੀਰਸ਼_Array()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

cv::ogl::ਸ਼ੀਰਸ਼_Array::ਸ਼ੀਰਸ਼_Array(std::initializer_list<Attribute> ਗੁਣ, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(ਗੁਣ); CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(ਗੁਣ, autoRelease));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਸ਼ੀਰਸ਼_Array::create(std::initializer_list<Attribute> ਗੁਣ, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(ਗੁਣ); CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(ਗੁਣ, autoRelease));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਸ਼ੀਰਸ਼_Array::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    ਜੇ (impl_)
        impl_->ਸੈੱਟ_Auto_Release(ਸੱਚ);
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਸ਼ੀਰਸ਼_Array::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਸ਼ੀਰਸ਼_Array::bind() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->bind();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਸ਼ੀਰਸ਼_Array::unbind()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Bind_ਸ਼ੀਰਸ਼_Array(0);
#ਜੇ_ਅੰਤ
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv::ogl::ਸ਼ੀਰਸ਼_Array::vaId() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->vaId();
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

ਵਰਗ cv::ogl::Shader::Impl
{
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::ogl::Shader::Impl
{
ਜਨਤਕ:
    ਸਥਿਰ ਅਚਲ Ptr<Impl>& empty();

    Impl(GLuint shaderId, ਤਾਰਕਿਕ autoRelease);
    Impl(ਅਚਲ ਅੱਖਰ* src, GLenum type, ਤਾਰਕਿਕ autoRelease);
    ~Impl();

    ਖਾਲੀ ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag) { autoRelease_ = flag; }

    GLuint shaderId() ਅਚਲ { ਵਾਪਸ shaderId_; }

ਨਿਜੀ:
    Impl();

    GLuint shaderId_;
    ਤਾਰਕਿਕ autoRelease_;
};

ਅਚਲ Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ਸਥਿਰ Ptr<Impl> p(ਨਵਾਂ Impl);
    ਵਾਪਸ p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(ਝੂਠ)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, ਤਾਰਕਿਕ autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ਅਚਲ ਅੱਖਰ* src, GLenum type, ਤਾਰਕਿਕ autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint ਸਥਿਤੀ;
    gl::Shader_ਸਰੋਤ(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_ਸਥਿਤੀ, &ਸਥਿਤੀ);
    ਜੇ (!ਸਥਿਤੀ) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_ਲੰਮਾਈ, &ਸਥਿਤੀ);
        info_log.resize(ਸਥਿਤੀ);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, ਖਾਲੀ_ਸੂਚਕ, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    ਜੇ (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#ਜੇ_ਅੰਤ // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::ਸ਼ੀਰਸ਼_3)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

cv::ogl::Shader::Shader(ਅਚਲ ਅੱਖਰ* src, ShaderType type, ਤਾਰਕਿਕ autoRelease) : type_(type)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(type); CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(src, type, autoRelease));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Shader::create(ਅਚਲ ਅੱਖਰ* src, ShaderType type, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(type); CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(src, type, autoRelease));
    type_ = type;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Shader::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    ਜੇ (impl_)
        impl_->ਸੈੱਟ_Auto_Release(ਸੱਚ);
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::Shader::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv::ogl::Shader::shaderId() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->shaderId();
#ਜੇ_ਅੰਤ
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ਅਚਲ
{
    ਵਾਪਸ type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL

ਵਰਗ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl
{
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl
{
ਜਨਤਕ:
    ਸਥਿਰ ਅਚਲ Ptr<Impl>& empty();

    Impl(GLuint ਪ੍ਰੋਗਰਾਮ_Id, ਤਾਰਕਿਕ autoRelease);
    Impl(ਅਚਲ Shader& vert, ਅਚਲ Shader& frag, ਤਾਰਕਿਕ autoRelease);
    ~Impl();

    ਖਾਲੀ bind();

    ਪੂਰਨ getAttributeLocation(ਅਚਲ ਅੱਖਰ* name) ਅਚਲ;
    ਪੂਰਨ get_ਇਕਸਾਰ_Location(ਅਚਲ ਅੱਖਰ* name) ਅਚਲ;

    ਖਾਲੀ ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag) { autoRelease_ = flag; }

    GLuint ਪ੍ਰੋਗਰਾਮ_Id() ਅਚਲ { ਵਾਪਸ ਪ੍ਰੋਗਰਾਮ_Id_2; }

ਨਿਜੀ:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint ਪ੍ਰੋਗਰਾਮ_Id_2;
    ਤਾਰਕਿਕ autoRelease_;
};

ਅਚਲ Ptr<cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl>& cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::empty()
{
    ਸਥਿਰ Ptr<Impl> p(ਨਵਾਂ Impl);
    ਵਾਪਸ p;
}

cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::Impl() : ਪ੍ਰੋਗਰਾਮ_Id_2(0), autoRelease_(ਝੂਠ)
{
}

cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::Impl(GLuint ਪ੍ਰੋਗਰਾਮ_Id, ਤਾਰਕਿਕ autoRelease) : ਪ੍ਰੋਗਰਾਮ_Id_2(ਪ੍ਰੋਗਰਾਮ_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ_Id) == gl::TRUE_ );
}

cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::Impl(ਅਚਲ Shader& vert, ਅਚਲ Shader& frag, ਤਾਰਕਿਕ autoRelease) : ਪ੍ਰੋਗਰਾਮ_Id_2(0), autoRelease_(autoRelease)
{
    ਪ੍ਰੋਗਰਾਮ_Id_2 = gl::Create_ਪ੍ਰੋਗਰਾਮ();
    CV_CheckGlError();

    CV_Assert( ਪ੍ਰੋਗਰਾਮ_Id_2 != 0 );

    gl::AttachShader(ਪ੍ਰੋਗਰਾਮ_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(ਪ੍ਰੋਗਰਾਮ_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint ਸਥਿਤੀ;
    gl::Link_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ_Id_2);
    gl::GetProgramiv(ਪ੍ਰੋਗਰਾਮ_Id_2, gl::LINK_ਸਥਿਤੀ, &ਸਥਿਤੀ);
    ਜੇ (!ਸਥਿਤੀ) {
        String info_log;
        gl::GetProgramiv(ਪ੍ਰੋਗਰਾਮ_Id_2, gl::INFO_LOG_ਲੰਮਾਈ, &ਸਥਿਤੀ);
        info_log.resize(ਸਥਿਤੀ);
        gl::Get_ਪ੍ਰੋਗਰਾਮ_Info_Log(ਪ੍ਰੋਗਰਾਮ_Id_2, GLsizei(info_log.size()) + 1, ਖਾਲੀ_ਸੂਚਕ, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::~Impl()
{
    ਜੇ (autoRelease_ && ਪ੍ਰੋਗਰਾਮ_Id_2)
        gl::Delete_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ_Id_2);
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::bind()
{
    gl::Use_ਪ੍ਰੋਗਰਾਮ(ਪ੍ਰੋਗਰਾਮ_Id_2);
    CV_CheckGlError();
}

ਪੂਰਨ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::getAttributeLocation(ਅਚਲ ਅੱਖਰ* name) ਅਚਲ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ਪੂਰਨ location = gl::GetAttribLocation(ਪ੍ਰੋਗਰਾਮ_Id_2, name);
    CV_CheckGlError();
    ਵਾਪਸ location;
}

ਪੂਰਨ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::Impl::get_ਇਕਸਾਰ_Location(ਅਚਲ ਅੱਖਰ* name) ਅਚਲ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ਪੂਰਨ location = gl::Get_ਇਕਸਾਰ_Location(ਪ੍ਰੋਗਰਾਮ_Id_2, name);
    CV_CheckGlError();
    ਵਾਪਸ location;
}

#ਜੇ_ਅੰਤ // HAVE_OPENGL

cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਅਚਲ Shader& vert, ਅਚਲ Shader& frag, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(vert); CV_ਅਣਵਰਤਿਆ(frag); CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(vert, frag, autoRelease));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::create(ਅਚਲ Shader& vert, ਅਚਲ Shader& frag, ਤਾਰਕਿਕ autoRelease)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(vert); CV_ਅਣਵਰਤਿਆ(frag); CV_ਅਣਵਰਤਿਆ(autoRelease);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_.reset(ਨਵਾਂ Impl(vert, frag, autoRelease));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    ਜੇ (impl_)
        impl_->ਸੈੱਟ_Auto_Release(ਸੱਚ);
    impl_ = Impl::empty();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_Auto_Release(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->ਸੈੱਟ_Auto_Release(flag);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::bind() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    impl_->bind();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::unbind()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Use_ਪ੍ਰੋਗਰਾਮ(0);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::getAttributeLocation(ਅਚਲ ਅੱਖਰ* name) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(name);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->getAttributeLocation(name);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::get_ਇਕਸਾਰ_Location(ਅਚਲ ਅੱਖਰ* name) ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(name);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->get_ਇਕਸਾਰ_Location(name);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_ਇਕਸਾਰ_Vec3(ਪੂਰਨ loc, Vec3f vec)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(loc); CV_ਅਣਵਰਤਿਆ(vec);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_ਇਕਸਾਰ_Vec2(ਪੂਰਨ loc, Vec2f vec)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(loc); CV_ਅਣਵਰਤਿਆ(vec);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Uniform2f(loc, vec(0), vec(1));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_ਇਕਸਾਰ_Vec4(ਪੂਰਨ loc, Vec4f vec)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(loc); CV_ਅਣਵਰਤਿਆ(vec);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_ਇਕਸਾਰ_Mat4x4(ਪੂਰਨ loc, Matx44f mat)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(loc); CV_ਅਣਵਰਤਿਆ(mat);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::ਇਕਸਾਰ_Matrix4fv(loc, 1, ਸੱਚ, mat.val);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_Uniform1f(ਪੂਰਨ loc, ਦਸ਼ਮਲਵ value)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(loc); CV_ਅਣਵਰਤਿਆ(value);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Uniform1f(loc, value);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਸੈੱਟ_Uniform1i(ਪੂਰਨ loc, ਪੂਰਨ value)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(loc); CV_ਅਣਵਰਤਿਆ(value);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Uniform1i(loc, value);
#ਜੇ_ਅੰਤ
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cv::ogl::ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_Id() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->ਪ੍ਰੋਗਰਾਮ_Id();
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// Rendering

ਖਾਲੀ cv::ogl::render(ਅਚਲ ogl::Texture2D& tex, Rect_<ਦੁੱਗਣਾ> wndRect, Rect_<ਦੁੱਗਣਾ> texRect)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(tex);
    CV_ਅਣਵਰਤਿਆ(wndRect);
    CV_ਅਣਵਰਤਿਆ(texRect);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਜੇ (!tex.empty())
    {
        gl::ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ(gl::PROJECTION);
        gl::ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::ਮੈਟ੍ਰਿਕਸ_ਬਹੁਲਕ(gl::MODELVIEW);
        gl::ਲੱਦੋ_ਪਛਾਣ_ਤੱਤ();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_ਬਹੁਲਕ, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::ਰੇਖਿਕ);
        CV_CheckGlError();

        ਅਚਲ ਦੁੱਗਣਾ ਸ਼ੀਰਸ਼[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.ਉਚਾਈ), 0.0,
            wndRect.x + wndRect.ਚੌੜਾਈ, (wndRect.y + wndRect.ਉਚਾਈ), 0.0,
            wndRect.x + wndRect.ਚੌੜਾਈ, wndRect.y, 0.0
        };
        ਅਚਲ ਦੁੱਗਣਾ texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.ਉਚਾਈ,
            texRect.x + texRect.ਚੌੜਾਈ, texRect.y + texRect.ਉਚਾਈ,
            texRect.x + texRect.ਚੌੜਾਈ, texRect.y
        };

        ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::ARRAY_ਬਫਰ);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_ਸੂਚਕ(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::ਸਾਧਾਰਨ_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::ਸ਼ੀਰਸ਼_ARRAY);
        CV_CheckGlError();

        gl::ਸ਼ੀਰਸ਼_ਸੂਚਕ(3, gl::DOUBLE, 0, ਸ਼ੀਰਸ਼);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::render(ਅਚਲ ogl::Arrays& arr, ਪੂਰਨ ਬਹੁਲਕ, Scalar color)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(ਬਹੁਲਕ);
    CV_ਅਣਵਰਤਿਆ(color);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਜੇ (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(ਬਹੁਲਕ, 0, arr.size());
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::render(ਅਚਲ ogl::Arrays& arr, ਨਿਵੇਸ਼_Array indices, ਪੂਰਨ ਬਹੁਲਕ, Scalar color)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(arr);
    CV_ਅਣਵਰਤਿਆ(indices);
    CV_ਅਣਵਰਤਿਆ(ਬਹੁਲਕ);
    CV_ਅਣਵਰਤਿਆ(color);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    ਜੇ (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ਅਚਲ ਪੂਰਨ ਕਿਸਮ = indices.ਕਿਸਮ();

        ਚੋਣ (ਕਿਸਮ)
        {
        ਹਾਲਤ ਨਿਵੇਸ਼_Array_2::OPENGL_ਬਫਰ :
            {
                ogl::ਬਫਰ_2 buf = indices.get_OGl_ਬਫਰ();

                ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = buf.ਡੂੰਘਾਈ();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( ਡੂੰਘਾਈ <= CV_32S );

                GLenum type;
                ਜੇ (ਡੂੰਘਾਈ < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ਨਹੀਂਤਾਂ
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::ਬਫਰ_2::ELEMENT_ARRAY_ਬਫਰ);

                gl::DrawElements(ਬਹੁਲਕ, buf.size().area(), type, 0);

                ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::ELEMENT_ARRAY_ਬਫਰ);

                ਰੋਕੋ;
            }

        ਮੂਲ:
            {
                Mat mat = indices.getMat();

                ਅਚਲ ਪੂਰਨ ਡੂੰਘਾਈ = mat.ਡੂੰਘਾਈ();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( ਡੂੰਘਾਈ <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                ਜੇ (ਡੂੰਘਾਈ < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ਨਹੀਂਤਾਂ
                    type = gl::UNSIGNED_INT;

                ogl::ਬਫਰ_2::unbind(ogl::ਬਫਰ_2::ELEMENT_ARRAY_ਬਫਰ);

                gl::DrawElements(ਬਹੁਲਕ, mat.size().area(), type, mat.ਅੰਕੜੇ);
            }
        }
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::clearColor(Scalar color)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(color);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_ਬਫਰ_ਦੋਆਧਾਰੀ_ਅੰਕ);
#ਜੇ_ਅੰਤ // HAVE_OPENGL
}

ਖਾਲੀ cv::ogl::clear_ਡੂੰਘਾਈ(ਦਸ਼ਮਲਵ ਡੂੰਘਾਈ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(ਡੂੰਘਾਈ);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Clear_ਡੂੰਘਾਈ(ਡੂੰਘਾਈ);
    gl::Clear(gl::ਡੂੰਘਾਈ_ਬਫਰ_ਦੋਆਧਾਰੀ_ਅੰਕ);
#ਜੇ_ਅੰਤ // HAVE_OPENGL
}

ਖਾਲੀ cv::ogl::drawArrays(ਪੂਰਨ first, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ ਬਹੁਲਕ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(first); CV_ਅਣਵਰਤਿਆ(ਗਿਣਤੀ_3); CV_ਅਣਵਰਤਿਆ(ਬਹੁਲਕ);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::DrawArrays(ਬਹੁਲਕ, first, ਗਿਣਤੀ_3);
#ਜੇ_ਅੰਤ // HAVE_OPENGL
}

ਖਾਲੀ cv::ogl::drawElements(ਪੂਰਨ first, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ type, ਪੂਰਨ ਬਹੁਲਕ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(first); CV_ਅਣਵਰਤਿਆ(ਗਿਣਤੀ_3); CV_ਅਣਵਰਤਿਆ(type); CV_ਅਣਵਰਤਿਆ(ਬਹੁਲਕ);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::DrawElements(ਬਹੁਲਕ, ਗਿਣਤੀ_3, type, reinterpret_cast<ਅਚਲ ਖਾਲੀ*>(static_cast<size_t>(first)));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::drawElementsInstanced(ਪੂਰਨ first, ਪੂਰਨ ਗਿਣਤੀ_3, ਪੂਰਨ type, ਪੂਰਨ ਬਹੁਲਕ, ਪੂਰਨ instance_ਗਿਣਤੀ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(first); CV_ਅਣਵਰਤਿਆ(ਗਿਣਤੀ_3); CV_ਅਣਵਰਤਿਆ(type); CV_ਅਣਵਰਤਿਆ(ਬਹੁਲਕ); CV_ਅਣਵਰਤਿਆ(instance_ਗਿਣਤੀ);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::DrawElementsInstanced(ਬਹੁਲਕ, ਗਿਣਤੀ_3, type, reinterpret_cast<ਅਚਲ ਖਾਲੀ*>(static_cast<size_t>(first)), instance_ਗਿਣਤੀ);
    CV_CheckGlError();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::blendFunc(ਪੂਰਨ sfactor, ਪੂਰਨ dfactor)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(sfactor); CV_ਅਣਵਰਤਿਆ(dfactor);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::BlendFunc(sfactor, dfactor);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::ਡੂੰਘਾਈ_ਨਕਾਬ_2(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(flag);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::ਡੂੰਘਾਈ_ਨਕਾਬ(flag ? gl::TRUE_ : gl::FALSE_);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::enable(ਪੂਰਨ cap)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(cap);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Enable(cap);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::ogl::disable(ਪੂਰਨ cap)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
    CV_ਅਣਵਰਤਿਆ(cap);
    throw_no_ogl();
#ਨਹੀਂਤਾਂ
    gl::Disable(cap);
#ਜੇ_ਅੰਤ
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  ਨਹੀਂਤਾਂ
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#ਨਹੀਂਤਾਂ // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#ਜੇ_ਅੰਤ // HAVE_OPENCL

#ਜੇ defined(HAVE_OPENGL)
#  ਜੇ defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#ਜੇ_ਅੰਤ // HAVE_OPENGL

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ogl {

#ਜੇ defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ਸਥਿਰ ਖਾਲੀ checkOpenCLVersion()
{
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ocl;
    ਅਚਲ Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int ਸਥਿਤੀ = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, ਆਕਾਰ(platform_id), &platform_id, &sz);
    CV_Assert(ਸਥਿਤੀ == CL_SUCCESS && sz == ਆਕਾਰ(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    ਪੂਰਨ ਸੰਸਕਰਣ_Major_2 = pi.ਸੰਸਕਰਣ_Major_2();
    ਪੂਰਨ ਸੰਸਕਰਣ_Minor_2 = pi.ਸੰਸਕਰਣ_Minor_2();
    ਜੇ (ਸੰਸਕਰਣ_Major_2 < 1 || (ਸੰਸਕਰਣ_Major_2 == 1 && ਸੰਸਕਰਣ_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                ਸੰਸਕਰਣ_Major_2, ਸੰਸਕਰਣ_Minor_2, pi.name().c_str(), pi.ਸੰਸਕਰਣ().c_str())
        );
}
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ ocl {

Context& initializeContextFromGL()
{
#ਜੇ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ਨਹੀਂਤਾਂ
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = ਖਾਲੀ_ਸੂਚਕ;
    cl_uint dev_ਵਰਤਿਆ = 0;
    cl_context context = ਖਾਲੀ_ਸੂਚਕ;

    cl_int ਸਥਿਤੀ = clGetPlatformIDs(0, ਨਿਰਾ, &platformsCnt);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", ਸਥਿਤੀ));
    ਜੇ (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    ਸਥਿਤੀ = clGetPlatformIDs(platformsCnt, &platforms[0], ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", ਸਥਿਤੀ));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ਤਾਰਕਿਕ sharingSupported = ਝੂਠ;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        ਸਥਿਤੀ = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ਨਿਰਾ, &devCnt);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", ਸਥਿਤੀ));

        ਕੋਸ਼ਿਸ਼ {
            devices = ਨਵਾਂ cl_device_id[devCnt];

            ਸਥਿਤੀ = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ਨਿਰਾ);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", ਸਥਿਤੀ));

            ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_ਮਾਪ;
                ਸਥਿਤੀ = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ਨਿਰਾ, &extension_ਮਾਪ );
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", ਸਥਿਤੀ));

                ਜੇ(extension_ਮਾਪ > 0)
                {
                    std::string devString(extension_ਮਾਪ, '\0');
                    ਸਥਿਤੀ = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_ਮਾਪ);
                    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                        ਜਾਰੀ;
                    ਜੇ (extension_ਮਾਪ > 0 && devString.size() >= extension_ਮਾਪ)
                        devString.resize(extension_ਮਾਪ - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    ਜਦੋਂ (spacePos != devString.npos) {
                        ਜੇ (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_ਵਰਤਿਆ = i;
                            sharingSupported = ਸੱਚ;
                            ਰੋਕੋ;
                        }
                        ਕਰੋ {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } ਜਦੋਂ (spacePos == oldPos);
                    }
                }
            }
        } ਫੜੋ(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            ਜੇ(devices != ਖਾਲੀ_ਸੂਚਕ) {
                ਮਿਟਾਓ[] devices;
            }
            ਜਾਰੀ;
        }

        ਜੇ (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#ਜੇ defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_ਸਮੂਹ_Obj cgl_Share_ਸਮੂਹ = CGLGet_Share_ਸਮੂਹ(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_ਸਮੂਹ,
                    0
                };
                context = clCreateContext(props, 0,0, ਨਿਰਾ, ਨਿਰਾ, &ci_Err_ਅੰਕ);
#ਜਾਂਜੇ defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ਵਰਤਿਆ], ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
#ਜਾਂਜੇ defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ਵਰਤਿਆ], ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
#ਜਾਂਜੇ defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ਵਰਤਿਆ], ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
#ਜੇ_ਅੰਤ
        }

        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", ਸਥਿਤੀ));
        ਨਹੀਂਤਾਂ
            ਰੋਕੋ;
    }


    cl_platform_id platform = platforms[dev_ਵਰਤਿਆ];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_ਵਰਤਿਆ]);
    clReleaseDevice(devices[dev_ਵਰਤਿਆ]);
    clReleaseContext(context);
    clExecCtx.bind();
    ਵਾਪਸ const_cast<Context&>(clExecCtx.getContext());
#ਜੇ_ਅੰਤ
}

} // namespace cv::ogl::ocl

ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_GLTexture2_D(ਨਿਵੇਸ਼_Array src, Texture2D& texture)
{
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(texture);
#ਜੇ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ਨਹੀਂਤਾਂ
    ਮਾਪ src_ਮਾਪ = src.size();
    CV_Assert(src_ਮਾਪ.ਚੌੜਾਈ == (ਪੂਰਨ)texture.ਖਾਨੇ_ਗਿਣਤੀ() && src_ਮਾਪ.ਉਚਾਈ == (ਪੂਰਨ)texture.ਪੰਕਤੀਆਂ());

    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", ਸਥਿਤੀ));

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_GLObjects(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", ਸਥਿਤੀ));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1};
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_ਮੰਜ਼ਿਲ_Image(q, cl_ਬਫਰ, clImage, offset, dst_origin, region, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", ਸਥਿਤੀ));
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_GLObjects(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", ਸਥਿਤੀ));

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", ਸਥਿਤੀ));

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", ਸਥਿਤੀ));
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ convertFromGLTexture2D(ਅਚਲ Texture2D& texture, OutputArray dst)
{
    CV_ਅਣਵਰਤਿਆ(texture); CV_ਅਣਵਰਤਿਆ(dst);
#ਜੇ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ਨਹੀਂਤਾਂ
    // check texture format
    ਅਚਲ ਪੂਰਨ dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    ਪੂਰਨ textureType = dtype;
    CV_Assert(textureType >= 0);

    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", ਸਥਿਤੀ));

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_GLObjects(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", ਸਥਿਤੀ));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1};
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_Image_ਮੰਜ਼ਿਲ_ਬਫਰ(q, clImage, cl_ਬਫਰ, src_origin, region, offset, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", ਸਥਿਤੀ));
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_GLObjects(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", ਸਥਿਤੀ));

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", ਸਥਿਤੀ));

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", ਸਥਿਤੀ));
#ਜੇ_ਅੰਤ
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ਅਚਲ ਬਫਰ_2& ਬਫਰ, AccessFlag accessFlags)
{
    CV_ਅਣਵਰਤਿਆ(ਬਫਰ); CV_ਅਣਵਰਤਿਆ(accessFlags);
#ਜੇ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ਨਹੀਂਤਾਂ
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_ਕਤਾਰ cl_ਕਤਾਰ = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

    ਪੂਰਨ clAccessFlags = 0;
    ਚੋਣ (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    ਮੂਲ:
    ਹਾਲਤ ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        ਰੋਕੋ;
    ਹਾਲਤ ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        ਰੋਕੋ;
    ਹਾਲਤ ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        ਰੋਕੋ;
    }

    cl_int ਸਥਿਤੀ = 0;
    cl_mem cl_ਬਫਰ = clCreateFromGLBuffer(context, clAccessFlags, ਬਫਰ.bufId(), &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", ਸਥਿਤੀ));

    gl::ਅੰਤ();

    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_GLObjects(cl_ਕਤਾਰ, 1, &cl_ਬਫਰ, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", ਸਥਿਤੀ));

    size_t ਕਦਮ = ਬਫਰ.ਖਾਨੇ_ਗਿਣਤੀ() * ਬਫਰ.elem_ਮਾਪ();
    ਪੂਰਨ ਪੰਕਤੀਆਂ = ਬਫਰ.ਪੰਕਤੀਆਂ();
    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = ਬਫਰ.ਖਾਨੇ_ਗਿਣਤੀ();
    ਪੂਰਨ type = ਬਫਰ.type();

    UMat u;
    convert_From_ਬਫਰ(cl_ਬਫਰ, ਕਦਮ, ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type, u);
    ਵਾਪਸ u;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ unmapGLBuffer(UMat& u)
{
    CV_ਅਣਵਰਤਿਆ(u);
#ਜੇ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ਨਹੀਂਤਾਂ
    ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ocl;
    cl_command_ਕਤਾਰ cl_ਕਤਾਰ = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_GLObjects(cl_ਕਤਾਰ, 1, &cl_ਬਫਰ, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", ਸਥਿਤੀ));

    ਸਥਿਤੀ = cl_ਅੰਤ(cl_ਕਤਾਰ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", ਸਥਿਤੀ));

    ਸਥਿਤੀ = clReleaseMemObject(cl_ਬਫਰ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", ਸਥਿਤੀ));
#ਜੇ_ਅੰਤ
}

}} // namespace cv::ogl
