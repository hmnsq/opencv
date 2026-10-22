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

#ಆದರೆ defined (__APPLE__) || defined(MACOSX)
   #ವ್ಯಾಖ್ಯಾನಿಸು GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#ಇಲ್ಲದಿದ್ದರೆ
   #ವ್ಯಾಖ್ಯಾನಿಸು GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    ಆದರೆ (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_ಆವೃತ್ತಿ
#        define GL_ಆವೃತ್ತಿ 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#ಇಲ್ಲದಿದ್ದರೆ // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

ಬಳಸಿ ನಾಮಾಕಾಶ cv;
ಬಳಸಿ ನಾಮಾಕಾಶ cv::cuda;

#ಆದರೆ defined(_MSC_VER)
#ಸೂಚನೆ warning(disable : 4702)  // unreachable code
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
ಸಾಲೊಳಗೆ ಸ್ಥಿರ CV_NORETURN ಶೂನ್ಯ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#ಅಥವಾಆದರೆ defined _DEBUG
ಸಾಲೊಳಗೆ ಸ್ಥಿರ ತಾರ್ಕಿಕ checkError(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* file, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಸಾಲು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* func = 0)
{
    GLenum err = gl::GetError();
    ಆದರೆ (err != gl::NO_ERROR_)
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸಂದೇಶ;
        ಆಯ್ಕೆ (err)
        {
        ಸ್ಥಿತಿ gl::INVALID_ENUM:
            ಸಂದೇಶ = "An unacceptable value is specified for an enumerated argument";
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ gl::INVALID_ಮೌಲ್ಯ:
            ಸಂದೇಶ = "A numeric argument is out of range";
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ gl::INVALID_OPERATION:
            ಸಂದೇಶ = "The specified operation is not allowed in the current state";
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ gl::ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ:
            ಸಂದೇಶ = "There is not enough memory left to execute the command";
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ಸಂದೇಶ = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, ಸಂದೇಶ, file, ಸಾಲು);
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
}
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL
} // namespace

#ವ್ಯಾಖ್ಯಾನಿಸು CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
ನಾಮಾಕಾಶ
{
    ಸ್ಥಿರಾಂಕ GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#ಆದರೆ_ಅಂತ್ಯ

////////////////////////////////////////////////////////////////////////
// setGlDevice

ಶೂನ್ಯ cv::cuda::ಗಣ_Gl_Device(ಪೂರ್ಣಾಂಕ device)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(device);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        CV_ಬಳಸದ(device);
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        cuda_ಸುರಕ್ಷಿತ_Call( cudaGLSetGLDevice(device) );
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#ಆದರೆ defined(HAVE_OPENGL) && defined(HAVE_CUDA)

ನಾಮಾಕಾಶ
{
    ವರ್ಗ CudaResource
    {
    ಸಾರ್ವಜನಿಕ:
        CudaResource();
        ~CudaResource();

        ಶೂನ್ಯ ರಿಜಿಸ್ಟರ್_ಬಫರ್(GLuint ಬಫರ್);
        ಶೂನ್ಯ release();

        ಶೂನ್ಯ copyFrom(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t spitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream = 0);
        ಶೂನ್ಯ copy_ಗಮ್ಯ(ಶೂನ್ಯ* dst, size_t dpitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream = 0);

        ಶೂನ್ಯ* map(cuda_ಹರಿವು_t stream = 0);
        ಶೂನ್ಯ unmap(cuda_ಹರಿವು_t stream = 0);

    ಖಾಸಗಿ:
        cudaGraphicsResource_t resource_;
        GLuint ಬಫರ್_3;

        ವರ್ಗ GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), ಬಫರ್_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    ಶೂನ್ಯ CudaResource::ರಿಜಿಸ್ಟರ್_ಬಫರ್(GLuint ಬಫರ್)
    {
        CV_DbgAssert( ಬಫರ್ != 0 );

        ಆದರೆ (ಬಫರ್_3 == ಬಫರ್)
            ಹಿಂತಿರುಗಿ;

        cudaGraphicsResource_t resource;
        cuda_ಸುರಕ್ಷಿತ_Call( cuda_Graphics_GLRegister_ಬಫರ್(&resource, ಬಫರ್, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        ಬಫರ್_3 = ಬಫರ್;
    }

    ಶೂನ್ಯ CudaResource::release()
    {
        ಆದರೆ (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        ಬಫರ್_3 = 0;
    }

    ವರ್ಗ CudaResource::GraphicsMapHolder
    {
    ಸಾರ್ವಜನಿಕ:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_ಹರಿವು_t stream);
        ~GraphicsMapHolder();

        ಶೂನ್ಯ reset();

    ಖಾಸಗಿ:
        cudaGraphicsResource_t* resource_;
        cuda_ಹರಿವು_t ಹರಿವು_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_ಹರಿವು_t stream) : resource_(resource), ಹರಿವು_3(stream)
    {
        ಆದರೆ (resource_)
            cuda_ಸುರಕ್ಷಿತ_Call( cudaGraphicsMapResources(1, resource_, ಹರಿವು_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        ಆದರೆ (resource_)
            cudaGraphicsUnmapResources(1, resource_, ಹರಿವು_3);
    }

    ಶೂನ್ಯ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    ಶೂನ್ಯ CudaResource::copyFrom(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t spitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_ಬಳಸದ(h);

        ಶೂನ್ಯ* dst;
        size_t size;
        cuda_ಸುರಕ್ಷಿತ_Call( cuda_Graphics_Resource_Get_Mapped_ಸೂಚಕ(&dst, &size, resource_) );

        CV_DbgAssert( ಅಗಲ * ಎತ್ತರ == size );

        ಆದರೆ (stream == 0)
            cuda_ಸುರಕ್ಷಿತ_Call( cudaMemcpy2D(dst, ಅಗಲ, src, spitch, ಅಗಲ, ಎತ್ತರ, cuda_Memcpy_Device_ಗಮ್ಯ_Device) );
        ಇಲ್ಲದಿದ್ದರೆ
            cuda_ಸುರಕ್ಷಿತ_Call( cudaMemcpy2DAsync(dst, ಅಗಲ, src, spitch, ಅಗಲ, ಎತ್ತರ, cuda_Memcpy_Device_ಗಮ್ಯ_Device, stream) );
    }

    ಶೂನ್ಯ CudaResource::copy_ಗಮ್ಯ(ಶೂನ್ಯ* dst, size_t dpitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_ಬಳಸದ(h);

        ಶೂನ್ಯ* src;
        size_t size;
        cuda_ಸುರಕ್ಷಿತ_Call( cuda_Graphics_Resource_Get_Mapped_ಸೂಚಕ(&src, &size, resource_) );

        CV_DbgAssert( ಅಗಲ * ಎತ್ತರ == size );

        ಆದರೆ (stream == 0)
            cuda_ಸುರಕ್ಷಿತ_Call( cudaMemcpy2D(dst, dpitch, src, ಅಗಲ, ಅಗಲ, ಎತ್ತರ, cuda_Memcpy_Device_ಗಮ್ಯ_Device) );
        ಇಲ್ಲದಿದ್ದರೆ
            cuda_ಸುರಕ್ಷಿತ_Call( cudaMemcpy2DAsync(dst, dpitch, src, ಅಗಲ, ಅಗಲ, ಎತ್ತರ, cuda_Memcpy_Device_ಗಮ್ಯ_Device, stream) );
    }

    ಶೂನ್ಯ* CudaResource::map(cuda_ಹರಿವು_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        ಶೂನ್ಯ* ptr;
        size_t size;
        cuda_ಸುರಕ್ಷಿತ_Call( cuda_Graphics_Resource_Get_Mapped_ಸೂಚಕ(&ptr, &size, resource_) );

        h.reset();

        ಹಿಂತಿರುಗಿ ptr;
    }

    ಶೂನ್ಯ CudaResource::unmap(cuda_ಹರಿವು_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#ಆದರೆ_ಅಂತ್ಯ

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL

ವರ್ಗ cv::ogl::ಬಫರ್_2::Impl
{
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::ogl::ಬಫರ್_2::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Ptr<Impl>& empty();

    Impl(GLuint bufId, ತಾರ್ಕಿಕ autoRelease);
    Impl(GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid* ದತ್ತಾಂಶ, GLenum target, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind(GLenum target) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ copyFrom(GLuint srcBuf, GLsizeiptr size);

    ಶೂನ್ಯ copyFrom(GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid* ದತ್ತಾಂಶ);
    ಶೂನ್ಯ copy_ಗಮ್ಯ(GLsizeiptr size, GLvoid* ದತ್ತಾಂಶ) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ resize(GLsizeiptr size, GLenum target);

    ಶೂನ್ಯ* mapHost(GLenum access);
    ಶೂನ್ಯ unmapHost();

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಶೂನ್ಯ copyFrom(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t spitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream = 0);
    ಶೂನ್ಯ copy_ಗಮ್ಯ(ಶೂನ್ಯ* dst, size_t dpitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream = 0) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ* mapDevice(cuda_ಹರಿವು_t stream = 0);
    ಶೂನ್ಯ unmapDevice(cuda_ಹರಿವು_t stream = 0);
#ಆದರೆ_ಅಂತ್ಯ

    ಶೂನ್ಯ ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint bufId() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ bufId_; }

ಖಾಸಗಿ:
    Impl();

    GLuint bufId_;
    ತಾರ್ಕಿಕ autoRelease_;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಬದಲಾಯಿಸಬಹುದಾದ CudaResource cudaResource_;
#ಆದರೆ_ಅಂತ್ಯ
};

ಸ್ಥಿರಾಂಕ Ptr<cv::ogl::ಬಫರ್_2::Impl>& cv::ogl::ಬಫರ್_2::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ಹೊಸ Impl);
    ಹಿಂತಿರುಗಿ p;
}

cv::ogl::ಬಫರ್_2::Impl::Impl() : bufId_(0), autoRelease_(ಸುಳ್ಳು)
{
}

cv::ogl::ಬಫರ್_2::Impl::Impl(GLuint abufId, ತಾರ್ಕಿಕ autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_ಬಫರ್(abufId) == gl::TRUE_ );
}

cv::ogl::ಬಫರ್_2::Impl::Impl(GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid* ದತ್ತಾಂಶ, GLenum target, ತಾರ್ಕಿಕ autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_ಬಫರ್(target, bufId_);
    CV_CheckGlError();

    gl::ಬಫರ್_ದತ್ತಾಂಶ(target, size, ದತ್ತಾಂಶ, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_ಬಫರ್(target, 0);
    CV_CheckGlError();
}

cv::ogl::ಬಫರ್_2::Impl::~Impl()
{
    ಆದರೆ (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::bind(GLenum target) ಸ್ಥಿರಾಂಕ
{
    gl::Bind_ಬಫರ್(target, bufId_);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_ಬಫರ್(gl::COPY_WRITE_ಬಫರ್, bufId_);
    CV_CheckGlError();

    gl::Bind_ಬಫರ್(gl::COPY_READ_ಬಫರ್, srcBuf);
    CV_CheckGlError();

    gl::Copy_ಬಫರ್_Sub_ದತ್ತಾಂಶ(gl::COPY_READ_ಬಫರ್, gl::COPY_WRITE_ಬಫರ್, 0, 0, size);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::copyFrom(GLsizeiptr size, ಸ್ಥಿರಾಂಕ GLvoid* ದತ್ತಾಂಶ)
{
    gl::Bind_ಬಫರ್(gl::COPY_WRITE_ಬಫರ್, bufId_);
    CV_CheckGlError();

    gl::ಬಫರ್_Sub_ದತ್ತಾಂಶ(gl::COPY_WRITE_ಬಫರ್, 0, size, ದತ್ತಾಂಶ);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_ಬಫರ್(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::ಬಫರ್_ದತ್ತಾಂಶ(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_ಬಫರ್(target, 0);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::copy_ಗಮ್ಯ(GLsizeiptr size, GLvoid* ದತ್ತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    gl::Bind_ಬಫರ್(gl::COPY_READ_ಬಫರ್, bufId_);
    CV_CheckGlError();

    gl::Get_ಬಫರ್_Sub_ದತ್ತಾಂಶ(gl::COPY_READ_ಬಫರ್, 0, size, ದತ್ತಾಂಶ);
    CV_CheckGlError();
}

ಶೂನ್ಯ* cv::ogl::ಬಫರ್_2::Impl::mapHost(GLenum access)
{
    gl::Bind_ಬಫರ್(gl::COPY_READ_ಬಫರ್, bufId_);
    CV_CheckGlError();

    GLvoid* ದತ್ತಾಂಶ = gl::Map_ಬಫರ್(gl::COPY_READ_ಬಫರ್, access);
    CV_CheckGlError();

    ಹಿಂತಿರುಗಿ ದತ್ತಾಂಶ;
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::unmapHost()
{
    gl::Unmap_ಬಫರ್(gl::COPY_READ_ಬಫರ್);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::copyFrom(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src, size_t spitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream)
{
    cudaResource_.ರಿಜಿಸ್ಟರ್_ಬಫರ್(bufId_);
    cudaResource_.copyFrom(src, spitch, ಅಗಲ, ಎತ್ತರ, stream);
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::copy_ಗಮ್ಯ(ಶೂನ್ಯ* dst, size_t dpitch, size_t ಅಗಲ, size_t ಎತ್ತರ, cuda_ಹರಿವು_t stream) ಸ್ಥಿರಾಂಕ
{
    cudaResource_.ರಿಜಿಸ್ಟರ್_ಬಫರ್(bufId_);
    cudaResource_.copy_ಗಮ್ಯ(dst, dpitch, ಅಗಲ, ಎತ್ತರ, stream);
}

ಶೂನ್ಯ* cv::ogl::ಬಫರ್_2::Impl::mapDevice(cuda_ಹರಿವು_t stream)
{
    cudaResource_.ರಿಜಿಸ್ಟರ್_ಬಫರ್(bufId_);
    ಹಿಂತಿರುಗಿ cudaResource_.map(stream);
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::Impl::unmapDevice(cuda_ಹರಿವು_t stream)
{
    cudaResource_.unmap(stream);
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_CUDA

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

cv::ogl::ಬಫರ್_2::ಬಫರ್_2() : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), type_(0)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::ಬಫರ್_2::ಬಫರ್_2(ಪೂರ್ಣಾಂಕ arows, ಪೂರ್ಣಾಂಕ acols, ಪೂರ್ಣಾಂಕ atype, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ abufId, ತಾರ್ಕಿಕ autoRelease) : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), type_(0)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arows);
    CV_ಬಳಸದ(acols);
    CV_ಬಳಸದ(atype);
    CV_ಬಳಸದ(abufId);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(abufId, autoRelease));
    ಅಡ್ಡಸಾಲುಗಳು_2 = arows;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = acols;
    type_ = atype;
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::ಬಫರ್_2::ಬಫರ್_2(ಗಾತ್ರ_2 asize, ಪೂರ್ಣಾಂಕ atype, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ abufId, ತಾರ್ಕಿಕ autoRelease) : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), type_(0)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(asize);
    CV_ಬಳಸದ(atype);
    CV_ಬಳಸದ(abufId);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(abufId, autoRelease));
    ಅಡ್ಡಸಾಲುಗಳು_2 = asize.ಎತ್ತರ;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = asize.ಅಗಲ;
    type_ = atype;
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::ಬಫರ್_2::ಬಫರ್_2(ಒಳಹರಿವು_Array arr, ಗುರಿ target, ತಾರ್ಕಿಕ autoRelease) : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), type_(0)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(target);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = arr.ಬಗೆ();

    ಆಯ್ಕೆ (ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್:
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ಸ್ಥಿರಾಂಕ GLsizeiptr asize = mat.ಅಡ್ಡಸಾಲುಗಳು * mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * mat.elem_ಗಾತ್ರ();
            impl_.reset(ಹೊಸ Impl(asize, mat.ದತ್ತಾಂಶ, target, autoRelease));
            ಅಡ್ಡಸಾಲುಗಳು_2 = mat.ಅಡ್ಡಸಾಲುಗಳು;
            ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            type_ = mat.type();
            ನಿಲ್ಲಿಸು;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::create(ಪೂರ್ಣಾಂಕ arows, ಪೂರ್ಣಾಂಕ acols, ಪೂರ್ಣಾಂಕ atype, ಗುರಿ target, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arows);
    CV_ಬಳಸದ(acols);
    CV_ಬಳಸದ(atype);
    CV_ಬಳಸದ(target);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು_2 != arows || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 != acols || type_ != atype)
    {
        ಸ್ಥಿರಾಂಕ GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_ಗಾತ್ರ(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        ಆದರೆ (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->ಗಣ_Auto_Release(autoRelease);
        }
        ಇಲ್ಲದಿದ್ದರೆ
            impl_.reset(ಹೊಸ Impl(asize, 0, target, autoRelease));
        ಅಡ್ಡಸಾಲುಗಳು_2 = arows;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = acols;
        type_ = atype;
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
    ಆದರೆ (impl_)
        impl_->ಗಣ_Auto_Release(ನಿಜ);
    impl_ = Impl::empty();
    ಅಡ್ಡಸಾಲುಗಳು_2 = 0;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = 0;
    type_ = 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::copyFrom(ಒಳಹರಿವು_Array arr, ಗುರಿ target, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(target);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = arr.ಬಗೆ();

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 asize = arr.size();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ atype = arr.type();
    create(asize, atype, target, autoRelease);

    ಆಯ್ಕೆ (ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್:
        {
            ogl::ಬಫರ್_2 buf = arr.get_OGl_ಬಫರ್();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_ಗಾತ್ರ(atype));
            ನಿಲ್ಲಿಸು;
        }

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
                throw_no_cuda();
            #ಇಲ್ಲದಿದ್ದರೆ
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.ದತ್ತಾಂಶ, dmat.ಹೆಜ್ಜೆ, dmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * dmat.elem_ಗಾತ್ರ(), dmat.ಅಡ್ಡಸಾಲುಗಳು);
            #ಆದರೆ_ಅಂತ್ಯ

            ನಿಲ್ಲಿಸು;
        }

    ಪೂರ್ವನಿಯೋಜಿತ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_ಗಾತ್ರ(atype), mat.ದತ್ತಾಂಶ);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::copyFrom(ಒಳಹರಿವು_Array arr, cuda::ಹರಿವು& stream, ಗುರಿ target, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(stream);
    CV_ಬಳಸದ(target);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        CV_ಬಳಸದ(arr);
        CV_ಬಳಸದ(stream);
        CV_ಬಳಸದ(target);
        CV_ಬಳಸದ(autoRelease);
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.ದತ್ತಾಂಶ, dmat.ಹೆಜ್ಜೆ, dmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * dmat.elem_ಗಾತ್ರ(), dmat.ಅಡ್ಡಸಾಲುಗಳು, cuda::ಹರಿವು_Accessor::get_ಹರಿವು(stream));
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::copy_ಗಮ್ಯ(OutputArray arr) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = arr.ಬಗೆ();

    ಆಯ್ಕೆ (ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್:
        {
            arr.get_OGl_ಬಫರ್_Ref().copyFrom(*ಇದು);
            ನಿಲ್ಲಿಸು;
        }

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
                throw_no_cuda();
            #ಇಲ್ಲದಿದ್ದರೆ
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type_);
                impl_->copy_ಗಮ್ಯ(dmat.ದತ್ತಾಂಶ, dmat.ಹೆಜ್ಜೆ, dmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * dmat.elem_ಗಾತ್ರ(), dmat.ಅಡ್ಡಸಾಲುಗಳು);
            #ಆದರೆ_ಅಂತ್ಯ

            ನಿಲ್ಲಿಸು;
        }

    ಪೂರ್ವನಿಯೋಜಿತ:
        {
            arr.create(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_ಗಮ್ಯ(mat.ಅಡ್ಡಸಾಲುಗಳು * mat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * mat.elem_ಗಾತ್ರ(), mat.ದತ್ತಾಂಶ);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::copy_ಗಮ್ಯ(OutputArray arr, cuda::ಹರಿವು& stream) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(stream);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        CV_ಬಳಸದ(arr);
        CV_ಬಳಸದ(stream);
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        arr.create(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_ಗಮ್ಯ(dmat.ದತ್ತಾಂಶ, dmat.ಹೆಜ್ಜೆ, dmat.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * dmat.elem_ಗಾತ್ರ(), dmat.ಅಡ್ಡಸಾಲುಗಳು, cuda::ಹರಿವು_Accessor::get_ಹರಿವು(stream));
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::ಬಫರ್_2 cv::ogl::ಬಫರ್_2::clone(ಗುರಿ target, ತಾರ್ಕಿಕ autoRelease) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(target);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ogl::ಬಫರ್_2 buf;
    buf.copyFrom(*ಇದು, target, autoRelease);
    ಹಿಂತಿರುಗಿ buf;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::bind(ಗುರಿ target) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(target);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->bind(target);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::unbind(ಗುರಿ target)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(target);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Bind_ಬಫರ್(target, 0);
    CV_CheckGlError();
#ಆದರೆ_ಅಂತ್ಯ
}

Mat cv::ogl::ಬಫರ್_2::mapHost(Access access)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(access);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ Mat(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type_, impl_->mapHost(access));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::unmapHost()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->unmapHost();
#ಆದರೆ_ಅಂತ್ಯ
}

GpuMat cv::ogl::ಬಫರ್_2::mapDevice()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ GpuMat(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type_, impl_->mapDevice());
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::unmapDevice()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        impl_->unmapDevice();
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

cuda::GpuMat cv::ogl::ಬಫರ್_2::mapDevice(cuda::ಹರಿವು& stream)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(stream);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        CV_ಬಳಸದ(stream);
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ GpuMat(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, type_, impl_->mapDevice(cuda::ಹರಿವು_Accessor::get_ಹರಿವು(stream)));
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಬಫರ್_2::unmapDevice(cuda::ಹರಿವು& stream)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(stream);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
        CV_ಬಳಸದ(stream);
        throw_no_cuda();
    #ಇಲ್ಲದಿದ್ದರೆ
        impl_->unmapDevice(cuda::ಹರಿವು_Accessor::get_ಹರಿವು(stream));
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cv::ogl::ಬಫರ್_2::bufId() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->bufId();
#ಆದರೆ_ಅಂತ್ಯ
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL

ವರ್ಗ cv::ogl::Texture2D::Impl
{
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::ogl::Texture2D::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Ptr<Impl> empty();

    Impl(GLuint texId, ತಾರ್ಕಿಕ autoRelease);
    Impl(GLint ಆಂತರಿಕ_Format, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid* pixels, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ copyFrom(GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels);
    ಶೂನ್ಯ copy_ಗಮ್ಯ(GLenum format, GLenum type, GLvoid* pixels) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ bind() ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint texId() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ texId_; }

ಖಾಸಗಿ:
    Impl();

    GLuint texId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಸ್ಥಿರಾಂಕ Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ಹೊಸ Impl);
    ಹಿಂತಿರುಗಿ p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(ಸುಳ್ಳು)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, ತಾರ್ಕಿಕ autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint ಆಂತರಿಕ_Format, GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid* pixels, ತಾರ್ಕಿಕ autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, ಆಂತರಿಕ_Format, ಅಗಲ, ಎತ್ತರ, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    ಆದರೆ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ಶೂನ್ಯ cv::ogl::Texture2D::Impl::copyFrom(GLsizei ಅಗಲ, GLsizei ಎತ್ತರ, GLenum format, GLenum type, ಸ್ಥಿರಾಂಕ GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, ಅಗಲ, ಎತ್ತರ, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Texture2D::Impl::copy_ಗಮ್ಯ(GLenum format, GLenum type, GLvoid* pixels) ಸ್ಥಿರಾಂಕ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

ಶೂನ್ಯ cv::ogl::Texture2D::Impl::bind() ಸ್ಥಿರಾಂಕ
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), format_(NONE)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Texture2D::Texture2D(ಪೂರ್ಣಾಂಕ arows, ಪೂರ್ಣಾಂಕ acols, Format aformat, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ atexId, ತಾರ್ಕಿಕ autoRelease) : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), format_(NONE)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arows);
    CV_ಬಳಸದ(acols);
    CV_ಬಳಸದ(aformat);
    CV_ಬಳಸದ(atexId);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(atexId, autoRelease));
    ಅಡ್ಡಸಾಲುಗಳು_2 = arows;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = acols;
    format_ = aformat;
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Texture2D::Texture2D(ಗಾತ್ರ_2 asize, Format aformat, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ atexId, ತಾರ್ಕಿಕ autoRelease) : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), format_(NONE)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(asize);
    CV_ಬಳಸದ(aformat);
    CV_ಬಳಸದ(atexId);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(atexId, autoRelease));
    ಅಡ್ಡಸಾಲುಗಳು_2 = asize.ಎತ್ತರ;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = asize.ಅಗಲ;
    format_ = aformat;
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Texture2D::Texture2D(ಒಳಹರಿವು_Array arr, ತಾರ್ಕಿಕ autoRelease) : ಅಡ್ಡಸಾಲುಗಳು_2(0), ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(0), format_(NONE)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = arr.ಬಗೆ();

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 asize = arr.size();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ atype = arr.type();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(atype);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(atype);

    CV_Assert( ಆಳ <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ಸ್ಥಿರಾಂಕ Format ಆಂತರಿಕ_Formats[] =
    {
        NONE, ಆಳ_COMPONENT, NONE, RGB, RGBA
    };
    ಸ್ಥಿರಾಂಕ GLenum srcFormats[] =
    {
        0, gl::ಆಳ_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    ಆಯ್ಕೆ (ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್:
        {
            ogl::ಬಫರ್_2 buf = arr.get_OGl_ಬಫರ್();
            buf.bind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            impl_.reset(ಹೊಸ Impl(ಆಂತರಿಕ_Formats[cn], asize.ಅಗಲ, asize.ಎತ್ತರ, srcFormats[cn], gl_types[ಆಳ], 0, autoRelease));
            ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            ನಿಲ್ಲಿಸು;
        }

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
                throw_no_cuda();
            #ಇಲ್ಲದಿದ್ದರೆ
                GpuMat dmat = arr.getGpuMat();
                ogl::ಬಫರ್_2 buf(dmat, ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
                buf.ಗಣ_Auto_Release(ನಿಜ);
                buf.bind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
                impl_.reset(ಹೊಸ Impl(ಆಂತರಿಕ_Formats[cn], asize.ಅಗಲ, asize.ಎತ್ತರ, srcFormats[cn], gl_types[ಆಳ], 0, autoRelease));
                ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            #ಆದರೆ_ಅಂತ್ಯ

            ನಿಲ್ಲಿಸು;
        }

    ಪೂರ್ವನಿಯೋಜಿತ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            impl_.reset(ಹೊಸ Impl(ಆಂತರಿಕ_Formats[cn], asize.ಅಗಲ, asize.ಎತ್ತರ, srcFormats[cn], gl_types[ಆಳ], mat.ದತ್ತಾಂಶ, autoRelease));
            ನಿಲ್ಲಿಸು;
        }
    }

    ಅಡ್ಡಸಾಲುಗಳು_2 = asize.ಎತ್ತರ;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = asize.ಅಗಲ;
    format_ = ಆಂತರಿಕ_Formats[cn];
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture2D::create(ಪೂರ್ಣಾಂಕ arows, ಪೂರ್ಣಾಂಕ acols, Format aformat, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arows);
    CV_ಬಳಸದ(acols);
    CV_ಬಳಸದ(aformat);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು_2 != arows || ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 != acols || format_ != aformat)
    {
        ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
        impl_.reset(ಹೊಸ Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        ಅಡ್ಡಸಾಲುಗಳು_2 = arows;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = acols;
        format_ = aformat;
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture2D::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
    ಆದರೆ (impl_)
        impl_->ಗಣ_Auto_Release(ನಿಜ);
    impl_ = Impl::empty();
    ಅಡ್ಡಸಾಲುಗಳು_2 = 0;
    ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 = 0;
    format_ = NONE;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture2D::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture2D::copyFrom(ಒಳಹರಿವು_Array arr, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = arr.ಬಗೆ();

    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_2 asize = arr.size();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ atype = arr.type();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = CV_MAT_ಆಳ(atype);
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(atype);

    CV_Assert( ಆಳ <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ಸ್ಥಿರಾಂಕ Format ಆಂತರಿಕ_Formats[] =
    {
        NONE, ಆಳ_COMPONENT, NONE, RGB, RGBA
    };
    ಸ್ಥಿರಾಂಕ GLenum srcFormats[] =
    {
        0, gl::ಆಳ_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, ಆಂತರಿಕ_Formats[cn], autoRelease);

    ಆಯ್ಕೆ(ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್:
        {
            ogl::ಬಫರ್_2 buf = arr.get_OGl_ಬಫರ್();
            buf.bind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            impl_->copyFrom(asize.ಅಗಲ, asize.ಎತ್ತರ, srcFormats[cn], gl_types[ಆಳ], 0);
            ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            ನಿಲ್ಲಿಸು;
        }

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
                throw_no_cuda();
            #ಇಲ್ಲದಿದ್ದರೆ
                GpuMat dmat = arr.getGpuMat();
                ogl::ಬಫರ್_2 buf(dmat, ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
                buf.ಗಣ_Auto_Release(ನಿಜ);
                buf.bind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
                impl_->copyFrom(asize.ಅಗಲ, asize.ಎತ್ತರ, srcFormats[cn], gl_types[ಆಳ], 0);
                ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            #ಆದರೆ_ಅಂತ್ಯ

            ನಿಲ್ಲಿಸು;
        }

    ಪೂರ್ವನಿಯೋಜಿತ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_UNPACK_ಬಫರ್);
            impl_->copyFrom(asize.ಅಗಲ, asize.ಎತ್ತರ, srcFormats[cn], gl_types[ಆಳ], mat.ದತ್ತಾಂಶ);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture2D::copy_ಗಮ್ಯ(OutputArray arr, ಪೂರ್ಣಾಂಕ ddepth, ತಾರ್ಕಿಕ autoRelease) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(ddepth);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = arr.ಬಗೆ();

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = format_ == ಆಳ_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ಸ್ಥಿರಾಂಕ GLenum dstFormat = format_ == ಆಳ_COMPONENT ? gl::ಆಳ_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    ಆಯ್ಕೆ(ಬಗೆ)
    {
    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್:
        {
            ogl::ಬಫರ್_2& buf = arr.get_OGl_ಬಫರ್_Ref();
            buf.create(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, CV_MAKE_TYPE(ddepth, cn), ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್, autoRelease);
            buf.bind(ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್);
            impl_->copy_ಗಮ್ಯ(dstFormat, gl_types[ddepth], 0);
            ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್);
            ನಿಲ್ಲಿಸು;
        }

    ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT:
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
                throw_no_cuda();
            #ಇಲ್ಲದಿದ್ದರೆ
                ogl::ಬಫರ್_2 buf(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, CV_MAKE_TYPE(ddepth, cn), ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್);
                buf.ಗಣ_Auto_Release(ನಿಜ);
                buf.bind(ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್);
                impl_->copy_ಗಮ್ಯ(dstFormat, gl_types[ddepth], 0);
                ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್);
                buf.copy_ಗಮ್ಯ(arr);
            #ಆದರೆ_ಅಂತ್ಯ

            ನಿಲ್ಲಿಸು;
        }

    ಪೂರ್ವನಿಯೋಜಿತ:
        {
            arr.create(ಅಡ್ಡಸಾಲುಗಳು_2, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::PIXEL_PACK_ಬಫರ್);
            impl_->copy_ಗಮ್ಯ(dstFormat, gl_types[ddepth], mat.ದತ್ತಾಂಶ);
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture2D::bind() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->bind();
#ಆದರೆ_ಅಂತ್ಯ
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cv::ogl::Texture2D::texId() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->texId();
#ಆದರೆ_ಅಂತ್ಯ
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL

ವರ್ಗ cv::ogl::Texture_ಬಫರ್::Impl
{
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::ogl::Texture_ಬಫರ್::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Ptr<Impl> empty();

    Impl();
    Impl(GLenum ಆಂತರಿಕ_Format, GLuint bufId, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind(GLint unit) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint texId() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ texId_; }

ಖಾಸಗಿ:
    GLuint texId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಸ್ಥಿರಾಂಕ Ptr<cv::ogl::Texture_ಬಫರ್::Impl> cv::ogl::Texture_ಬಫರ್::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p = makePtr<Impl>();
    ಹಿಂತಿರುಗಿ p;
}

cv::ogl::Texture_ಬಫರ್::Impl::Impl() : texId_(0), autoRelease_(ಸುಳ್ಳು)
{
}

cv::ogl::Texture_ಬಫರ್::Impl::Impl(GLenum ಆಂತರಿಕ_Format, GLuint bufId, ತಾರ್ಕಿಕ autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_ಬಫರ್, texId_);
    CV_CheckGlError();

    gl::Tex_ಬಫರ್(gl::TEXTURE_ಬಫರ್, ಆಂತರಿಕ_Format, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_ಬಫರ್, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_ಬಫರ್::Impl::~Impl()
{
    ಆದರೆ (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

ಶೂನ್ಯ cv::ogl::Texture_ಬಫರ್::Impl::bind(GLint unit) ಸ್ಥಿರಾಂಕ
{
    gl::ಸಕ್ರಿಯ_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_ಬಫರ್, texId_);
    CV_CheckGlError();
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

cv::ogl::Texture_ಬಫರ್::Texture_ಬಫರ್() : format_(NONE)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Texture_ಬಫರ್::Texture_ಬಫರ್(ಸ್ಥಿರಾಂಕ ಬಫರ್_2& buf, Format aformat, ತಾರ್ಕಿಕ autoRelease) : format_(NONE)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(buf);
    CV_ಬಳಸದ(aformat);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture_ಬಫರ್::create(ಸ್ಥಿರಾಂಕ ಬಫರ್_2& buf, Format aformat, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(buf);
    CV_ಬಳಸದ(aformat);
    CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture_ಬಫರ್::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
    ಆದರೆ (impl_)
        impl_->ಗಣ_Auto_Release(ನಿಜ);
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
    buf_ = ಬಫರ್_2();
    format_ = NONE;
}

ಶೂನ್ಯ cv::ogl::Texture_ಬಫರ್::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Texture_ಬಫರ್::bind(ಪೂರ್ಣಾಂಕ unit) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(unit);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Texture_ಬಫರ್::Format cv::ogl::Texture_ಬಫರ್::format() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ format_;
}

ತಾರ್ಕಿಕ cv::ogl::Texture_ಬಫರ್::empty() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ format_ == NONE;
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cv::ogl::Texture_ಬಫರ್::texId() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->texId();
#ಆದರೆ_ಅಂತ್ಯ
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

ಶೂನ್ಯ cv::ogl::Arrays::ಗಣ_ಶೃಂಗ_Array(ಒಳಹರಿವು_Array ಶೃಂಗ)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = ಶೃಂಗ.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = ಶೃಂಗ.ಆಳ();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( ಆಳ == CV_16S || ಆಳ == CV_32S || ಆಳ == CV_32F || ಆಳ == CV_64F );

    ಆದರೆ (ಶೃಂಗ.ಬಗೆ() == ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್)
        ಶೃಂಗ_2 = ಶೃಂಗ.get_OGl_ಬಫರ್();
    ಇಲ್ಲದಿದ್ದರೆ
        ಶೃಂಗ_2.copyFrom(ಶೃಂಗ);

    ಗಾತ್ರ_4 = ಶೃಂಗ_2.size().area();
}

ಶೂನ್ಯ cv::ogl::Arrays::reset_ಶೃಂಗ_Array()
{
    ಶೃಂಗ_2.release();
    ಗಾತ್ರ_4 = 0;
}

ಶೂನ್ಯ cv::ogl::Arrays::ಗಣ_Color_Array(ಒಳಹರಿವು_Array color)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    ಆದರೆ (color.ಬಗೆ() == ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್)
        color_ = color.get_OGl_ಬಫರ್();
    ಇಲ್ಲದಿದ್ದರೆ
        color_.copyFrom(color);
}

ಶೂನ್ಯ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

ಶೂನ್ಯ cv::ogl::Arrays::ಗಣ_ಸಾಮಾನ್ಯ_Array(ಒಳಹರಿವು_Array ಸಾಮಾನ್ಯ)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = ಸಾಮಾನ್ಯ.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = ಸಾಮಾನ್ಯ.ಆಳ();

    CV_Assert( cn == 3 );
    CV_Assert( ಆಳ == CV_8S || ಆಳ == CV_16S || ಆಳ == CV_32S || ಆಳ == CV_32F || ಆಳ == CV_64F );

    ಆದರೆ (ಸಾಮಾನ್ಯ.ಬಗೆ() == ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್)
        ಸಾಮಾನ್ಯ_2 = ಸಾಮಾನ್ಯ.get_OGl_ಬಫರ್();
    ಇಲ್ಲದಿದ್ದರೆ
        ಸಾಮಾನ್ಯ_2.copyFrom(ಸಾಮಾನ್ಯ);
}

ಶೂನ್ಯ cv::ogl::Arrays::reset_ಸಾಮಾನ್ಯ_Array()
{
    ಸಾಮಾನ್ಯ_2.release();
}

ಶೂನ್ಯ cv::ogl::Arrays::ಗಣ_Tex_Coord_Array(ಒಳಹರಿವು_Array texCoord)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = texCoord.channels();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = texCoord.ಆಳ();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( ಆಳ == CV_16S || ಆಳ == CV_32S || ಆಳ == CV_32F || ಆಳ == CV_64F );

    ಆದರೆ (texCoord.ಬಗೆ() == ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್)
        texCoord_ = texCoord.get_OGl_ಬಫರ್();
    ಇಲ್ಲದಿದ್ದರೆ
        texCoord_.copyFrom(texCoord);
}

ಶೂನ್ಯ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

ಶೂನ್ಯ cv::ogl::Arrays::release()
{
    reset_ಶೃಂಗ_Array();
    resetColorArray();
    reset_ಸಾಮಾನ್ಯ_Array();
    resetTexCoordArray();
}

ಶೂನ್ಯ cv::ogl::Arrays::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಶೃಂಗ_2.ಗಣ_Auto_Release(flag);
    color_.ಗಣ_Auto_Release(flag);
    ಸಾಮಾನ್ಯ_2.ಗಣ_Auto_Release(flag);
    texCoord_.ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Arrays::bind() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == ಗಾತ್ರ_4 );
    CV_Assert( ಸಾಮಾನ್ಯ_2.empty() || ಸಾಮಾನ್ಯ_2.size().area() == ಗಾತ್ರ_4 );
    CV_Assert( color_.empty() || color_.size().area() == ಗಾತ್ರ_4 );

    ಆದರೆ (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::ಬಫರ್_2::ARRAY_ಬಫರ್);

        gl::Tex_Coord_ಸೂಚಕ(texCoord_.channels(), gl_types[texCoord_.ಆಳ()], 0, 0);
        CV_CheckGlError();
    }

    ಆದರೆ (ಸಾಮಾನ್ಯ_2.empty())
    {
        gl::DisableClientState(gl::ಸಾಮಾನ್ಯ_ARRAY);
        CV_CheckGlError();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        gl::EnableClientState(gl::ಸಾಮಾನ್ಯ_ARRAY);
        CV_CheckGlError();

        ಸಾಮಾನ್ಯ_2.bind(ogl::ಬಫರ್_2::ARRAY_ಬಫರ್);

        gl::ಸಾಮಾನ್ಯ_ಸೂಚಕ(gl_types[ಸಾಮಾನ್ಯ_2.ಆಳ()], 0, 0);
        CV_CheckGlError();
    }

    ಆದರೆ (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::ಬಫರ್_2::ARRAY_ಬಫರ್);

        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = color_.channels();

        gl::Color_ಸೂಚಕ(cn, gl_types[color_.ಆಳ()], 0, 0);
        CV_CheckGlError();
    }

    ಆದರೆ (ಶೃಂಗ_2.empty())
    {
        gl::DisableClientState(gl::ಶೃಂಗ_ARRAY);
        CV_CheckGlError();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        gl::EnableClientState(gl::ಶೃಂಗ_ARRAY);
        CV_CheckGlError();

        ಶೃಂಗ_2.bind(ogl::ಬಫರ್_2::ARRAY_ಬಫರ್);

        gl::ಶೃಂಗ_ಸೂಚಕ(ಶೃಂಗ_2.channels(), gl_types[ಶೃಂಗ_2.ಆಳ()], 0, 0);
        CV_CheckGlError();
    }

    ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::ARRAY_ಬಫರ್);
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL

ವರ್ಗ cv::ogl::ಶೃಂಗ_Array::Impl
{
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::ogl::ಶೃಂಗ_Array::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Ptr<Impl>& empty();

    Impl(GLuint vaId, ತಾರ್ಕಿಕ autoRelease);
    Impl(std::initializer_list<Attribute> ಗುಣಲಕ್ಷಣಗಳು, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind();

    ಶೂನ್ಯ ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint vaId() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ vaId_; }

ಖಾಸಗಿ:
    Impl();

    GLuint vaId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಸ್ಥಿರಾಂಕ Ptr<cv::ogl::ಶೃಂಗ_Array::Impl>& cv::ogl::ಶೃಂಗ_Array::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ಹೊಸ Impl);
    ಹಿಂತಿರುಗಿ p;
}

cv::ogl::ಶೃಂಗ_Array::Impl::Impl() : vaId_(0), autoRelease_(ಸುಳ್ಳು)
{
}

cv::ogl::ಶೃಂಗ_Array::Impl::Impl(GLuint vaId, ತಾರ್ಕಿಕ autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_ಶೃಂಗ_Array(vaId) == gl::TRUE_ );
}

cv::ogl::ಶೃಂಗ_Array::Impl::Impl(std::initializer_list<Attribute> ಗುಣಲಕ್ಷಣಗಳು, ತಾರ್ಕಿಕ autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_ಶೃಂಗ_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_ಶೃಂಗ_Array(vaId_);
    CV_CheckGlError();

    ಗಾಗಿ (ಸ್ವಯಂಚಾಲಿತ& attribute : ಗುಣಲಕ್ಷಣಗಳು)
    {
        attribute.ಬಫರ್_3.bind(ಬಫರ್_2::ARRAY_ಬಫರ್);
        ಆದರೆ (attribute.integer_)
        {
            gl::ಶೃಂಗ_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.ಗಾತ್ರ_4,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*>(attribute.ಸ್ಥಾನಾಂತರ)
            );
            CV_CheckGlError();
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            gl::ಶೃಂಗ_Attrib_ಸೂಚಕ(
                attribute.shader_loc_,
                attribute.ಗಾತ್ರ_4,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*>(attribute.ಸ್ಥಾನಾಂತರ)
            );
            CV_CheckGlError();
        }

        gl::Enable_ಶೃಂಗ_Attrib_Array(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::ಶೃಂಗ_Array::Impl::~Impl()
{
    ಆದರೆ (autoRelease_ && vaId_)
        gl::Delete_ಶೃಂಗ_Arrays(1, &vaId_);
}

ಶೂನ್ಯ cv::ogl::ಶೃಂಗ_Array::Impl::bind()
{
    gl::Bind_ಶೃಂಗ_Array(vaId_);
    CV_CheckGlError();
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

cv::ogl::ಶೃಂಗ_Array::ಶೃಂಗ_Array()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::ಶೃಂಗ_Array::ಶೃಂಗ_Array(std::initializer_list<Attribute> ಗುಣಲಕ್ಷಣಗಳು, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(ಗುಣಲಕ್ಷಣಗಳು); CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(ಗುಣಲಕ್ಷಣಗಳು, autoRelease));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಶೃಂಗ_Array::create(std::initializer_list<Attribute> ಗುಣಲಕ್ಷಣಗಳು, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(ಗುಣಲಕ್ಷಣಗಳು); CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(ಗುಣಲಕ್ಷಣಗಳು, autoRelease));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಶೃಂಗ_Array::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
    ಆದರೆ (impl_)
        impl_->ಗಣ_Auto_Release(ನಿಜ);
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಶೃಂಗ_Array::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಶೃಂಗ_Array::bind() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->bind();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಶೃಂಗ_Array::unbind()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Bind_ಶೃಂಗ_Array(0);
#ಆದರೆ_ಅಂತ್ಯ
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cv::ogl::ಶೃಂಗ_Array::vaId() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->vaId();
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL

ವರ್ಗ cv::ogl::Shader::Impl
{
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::ogl::Shader::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Ptr<Impl>& empty();

    Impl(GLuint shaderId, ತಾರ್ಕಿಕ autoRelease);
    Impl(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* src, GLenum type, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint shaderId() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ shaderId_; }

ಖಾಸಗಿ:
    Impl();

    GLuint shaderId_;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಸ್ಥಿರಾಂಕ Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ಹೊಸ Impl);
    ಹಿಂತಿರುಗಿ p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(ಸುಳ್ಳು)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, ತಾರ್ಕಿಕ autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* src, GLenum type, ತಾರ್ಕಿಕ autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint ಸ್ಥಿತಿ_2;
    gl::Shader_ಮೂಲ(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_ಸ್ಥಿತಿ, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (!ಸ್ಥಿತಿ_2) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_ಉದ್ದಳತೆ, &ಸ್ಥಿತಿ_2);
        info_log.resize(ಸ್ಥಿತಿ_2);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, ಶೂನ್ಯಸೂಚಕ, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    ಆದರೆ (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::ಶೃಂಗ_3)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Shader::Shader(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* src, ShaderType type, ತಾರ್ಕಿಕ autoRelease) : type_(type)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(type); CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(src, type, autoRelease));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Shader::create(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* src, ShaderType type, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(type); CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(src, type, autoRelease));
    type_ = type;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Shader::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
    ಆದರೆ (impl_)
        impl_->ಗಣ_Auto_Release(ನಿಜ);
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::Shader::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cv::ogl::Shader::shaderId() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->shaderId();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL

ವರ್ಗ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl
{
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Ptr<Impl>& empty();

    Impl(GLuint ಕಾರ್ಯಕ್ರಮ_Id, ತಾರ್ಕಿಕ autoRelease);
    Impl(ಸ್ಥಿರಾಂಕ Shader& vert, ಸ್ಥಿರಾಂಕ Shader& frag, ತಾರ್ಕಿಕ autoRelease);
    ~Impl();

    ಶೂನ್ಯ bind();

    ಪೂರ್ಣಾಂಕ getAttributeLocation(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) ಸ್ಥಿರಾಂಕ;
    ಪೂರ್ಣಾಂಕ get_ಏಕರೂಪ_Location(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) ಸ್ಥಿರಾಂಕ;

    ಶೂನ್ಯ ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag) { autoRelease_ = flag; }

    GLuint ಕಾರ್ಯಕ್ರಮ_Id() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ಕಾರ್ಯಕ್ರಮ_Id_2; }

ಖಾಸಗಿ:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint ಕಾರ್ಯಕ್ರಮ_Id_2;
    ತಾರ್ಕಿಕ autoRelease_;
};

ಸ್ಥಿರಾಂಕ Ptr<cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl>& cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::empty()
{
    ಸ್ಥಿರ Ptr<Impl> p(ಹೊಸ Impl);
    ಹಿಂತಿರುಗಿ p;
}

cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::Impl() : ಕಾರ್ಯಕ್ರಮ_Id_2(0), autoRelease_(ಸುಳ್ಳು)
{
}

cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::Impl(GLuint ಕಾರ್ಯಕ್ರಮ_Id, ತಾರ್ಕಿಕ autoRelease) : ಕಾರ್ಯಕ್ರಮ_Id_2(ಕಾರ್ಯಕ್ರಮ_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ_Id) == gl::TRUE_ );
}

cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::Impl(ಸ್ಥಿರಾಂಕ Shader& vert, ಸ್ಥಿರಾಂಕ Shader& frag, ತಾರ್ಕಿಕ autoRelease) : ಕಾರ್ಯಕ್ರಮ_Id_2(0), autoRelease_(autoRelease)
{
    ಕಾರ್ಯಕ್ರಮ_Id_2 = gl::Create_ಕಾರ್ಯಕ್ರಮ();
    CV_CheckGlError();

    CV_Assert( ಕಾರ್ಯಕ್ರಮ_Id_2 != 0 );

    gl::AttachShader(ಕಾರ್ಯಕ್ರಮ_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(ಕಾರ್ಯಕ್ರಮ_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint ಸ್ಥಿತಿ_2;
    gl::Link_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ_Id_2);
    gl::GetProgramiv(ಕಾರ್ಯಕ್ರಮ_Id_2, gl::LINK_ಸ್ಥಿತಿ, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (!ಸ್ಥಿತಿ_2) {
        String info_log;
        gl::GetProgramiv(ಕಾರ್ಯಕ್ರಮ_Id_2, gl::INFO_LOG_ಉದ್ದಳತೆ, &ಸ್ಥಿತಿ_2);
        info_log.resize(ಸ್ಥಿತಿ_2);
        gl::Get_ಕಾರ್ಯಕ್ರಮ_Info_Log(ಕಾರ್ಯಕ್ರಮ_Id_2, GLsizei(info_log.size()) + 1, ಶೂನ್ಯಸೂಚಕ, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::~Impl()
{
    ಆದರೆ (autoRelease_ && ಕಾರ್ಯಕ್ರಮ_Id_2)
        gl::Delete_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ_Id_2);
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::bind()
{
    gl::Use_ಕಾರ್ಯಕ್ರಮ(ಕಾರ್ಯಕ್ರಮ_Id_2);
    CV_CheckGlError();
}

ಪೂರ್ಣಾಂಕ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::getAttributeLocation(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) ಸ್ಥಿರಾಂಕ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ಪೂರ್ಣಾಂಕ location = gl::GetAttribLocation(ಕಾರ್ಯಕ್ರಮ_Id_2, name);
    CV_CheckGlError();
    ಹಿಂತಿರುಗಿ location;
}

ಪೂರ್ಣಾಂಕ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::Impl::get_ಏಕರೂಪ_Location(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) ಸ್ಥಿರಾಂಕ
{
    // -1 if absent (e.g. optimized out); caller decides.
    ಪೂರ್ಣಾಂಕ location = gl::Get_ಏಕರೂಪ_Location(ಕಾರ್ಯಕ್ರಮ_Id_2, name);
    CV_CheckGlError();
    ಹಿಂತಿರುಗಿ location;
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_2(ಸ್ಥಿರಾಂಕ Shader& vert, ಸ್ಥಿರಾಂಕ Shader& frag, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(vert); CV_ಬಳಸದ(frag); CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(vert, frag, autoRelease));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::create(ಸ್ಥಿರಾಂಕ Shader& vert, ಸ್ಥಿರಾಂಕ Shader& frag, ತಾರ್ಕಿಕ autoRelease)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(vert); CV_ಬಳಸದ(frag); CV_ಬಳಸದ(autoRelease);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_.reset(ಹೊಸ Impl(vert, frag, autoRelease));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
    ಆದರೆ (impl_)
        impl_->ಗಣ_Auto_Release(ನಿಜ);
    impl_ = Impl::empty();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_Auto_Release(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->ಗಣ_Auto_Release(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::bind() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_->bind();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::unbind()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Use_ಕಾರ್ಯಕ್ರಮ(0);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::getAttributeLocation(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(name);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->getAttributeLocation(name);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::get_ಏಕರೂಪ_Location(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(name);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->get_ಏಕರೂಪ_Location(name);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_ಏಕರೂಪ_Vec3(ಪೂರ್ಣಾಂಕ loc, Vec3f vec)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(loc); CV_ಬಳಸದ(vec);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_ಏಕರೂಪ_Vec2(ಪೂರ್ಣಾಂಕ loc, Vec2f vec)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(loc); CV_ಬಳಸದ(vec);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Uniform2f(loc, vec(0), vec(1));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_ಏಕರೂಪ_Vec4(ಪೂರ್ಣಾಂಕ loc, Vec4f vec)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(loc); CV_ಬಳಸದ(vec);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_ಏಕರೂಪ_Mat4x4(ಪೂರ್ಣಾಂಕ loc, Matx44f mat)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(loc); CV_ಬಳಸದ(mat);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::ಏಕರೂಪ_Matrix4fv(loc, 1, ನಿಜ, mat.val);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_Uniform1f(ಪೂರ್ಣಾಂಕ loc, ದಶಮಾಂಶ value)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(loc); CV_ಬಳಸದ(value);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Uniform1f(loc, value);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಗಣ_Uniform1i(ಪೂರ್ಣಾಂಕ loc, ಪೂರ್ಣಾಂಕ value)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(loc); CV_ಬಳಸದ(value);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Uniform1i(loc, value);
#ಆದರೆ_ಅಂತ್ಯ
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cv::ogl::ಕಾರ್ಯಕ್ರಮ_2::ಕಾರ್ಯಕ್ರಮ_Id() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->ಕಾರ್ಯಕ್ರಮ_Id();
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// Rendering

ಶೂನ್ಯ cv::ogl::render(ಸ್ಥಿರಾಂಕ ogl::Texture2D& tex, Rect_<ದ್ವಿಗುಣ> wndRect, Rect_<ದ್ವಿಗುಣ> texRect)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(tex);
    CV_ಬಳಸದ(wndRect);
    CV_ಬಳಸದ(texRect);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (!tex.empty())
    {
        gl::ಮಾತೃಕೆ_ಬಹುಲಕ(gl::PROJECTION);
        gl::ತುಂಬು_ತಟಸ್ಥ();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::ಮಾತೃಕೆ_ಬಹುಲಕ(gl::MODELVIEW);
        gl::ತುಂಬು_ತಟಸ್ಥ();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_ಬಹುಲಕ, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::ರೇಖೀಯ);
        CV_CheckGlError();

        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ ಶೃಂಗ[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.ಎತ್ತರ), 0.0,
            wndRect.x + wndRect.ಅಗಲ, (wndRect.y + wndRect.ಎತ್ತರ), 0.0,
            wndRect.x + wndRect.ಅಗಲ, wndRect.y, 0.0
        };
        ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.ಎತ್ತರ,
            texRect.x + texRect.ಅಗಲ, texRect.y + texRect.ಎತ್ತರ,
            texRect.x + texRect.ಅಗಲ, texRect.y
        };

        ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::ARRAY_ಬಫರ್);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_ಸೂಚಕ(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::ಸಾಮಾನ್ಯ_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::ಶೃಂಗ_ARRAY);
        CV_CheckGlError();

        gl::ಶೃಂಗ_ಸೂಚಕ(3, gl::DOUBLE, 0, ಶೃಂಗ);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::render(ಸ್ಥಿರಾಂಕ ogl::Arrays& arr, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, Scalar color)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(ಬಹುಲಕ);
    CV_ಬಳಸದ(color);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(ಬಹುಲಕ, 0, arr.size());
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::render(ಸ್ಥಿರಾಂಕ ogl::Arrays& arr, ಒಳಹರಿವು_Array indices, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, Scalar color)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(arr);
    CV_ಬಳಸದ(indices);
    CV_ಬಳಸದ(ಬಹುಲಕ);
    CV_ಬಳಸದ(color);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಬಗೆ = indices.ಬಗೆ();

        ಆಯ್ಕೆ (ಬಗೆ)
        {
        ಸ್ಥಿತಿ ಒಳಹರಿವು_Array_2::OPENGL_ಬಫರ್ :
            {
                ogl::ಬಫರ್_2 buf = indices.get_OGl_ಬಫರ್();

                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = buf.ಆಳ();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( ಆಳ <= CV_32S );

                GLenum type;
                ಆದರೆ (ಆಳ < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ಇಲ್ಲದಿದ್ದರೆ
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::ಬಫರ್_2::ELEMENT_ARRAY_ಬಫರ್);

                gl::DrawElements(ಬಹುಲಕ, buf.size().area(), type, 0);

                ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::ELEMENT_ARRAY_ಬಫರ್);

                ನಿಲ್ಲಿಸು;
            }

        ಪೂರ್ವನಿಯೋಜಿತ:
            {
                Mat mat = indices.getMat();

                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಆಳ = mat.ಆಳ();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( ಆಳ <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                ಆದರೆ (ಆಳ < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಆಳ < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ಇಲ್ಲದಿದ್ದರೆ
                    type = gl::UNSIGNED_INT;

                ogl::ಬಫರ್_2::unbind(ogl::ಬಫರ್_2::ELEMENT_ARRAY_ಬಫರ್);

                gl::DrawElements(ಬಹುಲಕ, mat.size().area(), type, mat.ದತ್ತಾಂಶ);
            }
        }
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::clearColor(Scalar color)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(color);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_ಬಫರ್_ಬಿಟ್);
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL
}

ಶೂನ್ಯ cv::ogl::clear_ಆಳ(ದಶಮಾಂಶ ಆಳ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(ಆಳ);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Clear_ಆಳ(ಆಳ);
    gl::Clear(gl::ಆಳ_ಬಫರ್_ಬಿಟ್);
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL
}

ಶೂನ್ಯ cv::ogl::drawArrays(ಪೂರ್ಣಾಂಕ first, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(first); CV_ಬಳಸದ(ಎಣಿಕೆ_3); CV_ಬಳಸದ(ಬಹುಲಕ);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::DrawArrays(ಬಹುಲಕ, first, ಎಣಿಕೆ_3);
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL
}

ಶೂನ್ಯ cv::ogl::drawElements(ಪೂರ್ಣಾಂಕ first, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(first); CV_ಬಳಸದ(ಎಣಿಕೆ_3); CV_ಬಳಸದ(type); CV_ಬಳಸದ(ಬಹುಲಕ);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::DrawElements(ಬಹುಲಕ, ಎಣಿಕೆ_3, type, reinterpret_cast<ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*>(static_cast<size_t>(first)));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::drawElementsInstanced(ಪೂರ್ಣಾಂಕ first, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, ಪೂರ್ಣಾಂಕ instance_ಎಣಿಕೆ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(first); CV_ಬಳಸದ(ಎಣಿಕೆ_3); CV_ಬಳಸದ(type); CV_ಬಳಸದ(ಬಹುಲಕ); CV_ಬಳಸದ(instance_ಎಣಿಕೆ);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::DrawElementsInstanced(ಬಹುಲಕ, ಎಣಿಕೆ_3, type, reinterpret_cast<ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*>(static_cast<size_t>(first)), instance_ಎಣಿಕೆ);
    CV_CheckGlError();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::blendFunc(ಪೂರ್ಣಾಂಕ sfactor, ಪೂರ್ಣಾಂಕ dfactor)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(sfactor); CV_ಬಳಸದ(dfactor);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::BlendFunc(sfactor, dfactor);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::ಆಳ_ಮುಖವಾಡ_2(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(flag);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::ಆಳ_ಮುಖವಾಡ(flag ? gl::TRUE_ : gl::FALSE_);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::enable(ಪೂರ್ಣಾಂಕ cap)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(cap);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Enable(cap);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::ogl::disable(ಪೂರ್ಣಾಂಕ cap)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENGL
    CV_ಬಳಸದ(cap);
    throw_no_ogl();
#ಇಲ್ಲದಿದ್ದರೆ
    gl::Disable(cap);
#ಆದರೆ_ಅಂತ್ಯ
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  ಇಲ್ಲದಿದ್ದರೆ
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#ಇಲ್ಲದಿದ್ದರೆ // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENCL

#ಆದರೆ defined(HAVE_OPENGL)
#  ಆದರೆ defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENGL

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ogl {

#ಆದರೆ defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ಸ್ಥಿರ ಶೂನ್ಯ checkOpenCLVersion()
{
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::ocl;
    ಸ್ಥಿರಾಂಕ Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int ಸ್ಥಿತಿ_2 = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, ಗಾತ್ರ(platform_id), &platform_id, &sz);
    CV_Assert(ಸ್ಥಿತಿ_2 == CL_SUCCESS && sz == ಗಾತ್ರ(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ_Major_2 = pi.ಆವೃತ್ತಿ_Major_2();
    ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ_Minor_2 = pi.ಆವೃತ್ತಿ_Minor_2();
    ಆದರೆ (ಆವೃತ್ತಿ_Major_2 < 1 || (ಆವೃತ್ತಿ_Major_2 == 1 && ಆವೃತ್ತಿ_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                ಆವೃತ್ತಿ_Major_2, ಆವೃತ್ತಿ_Minor_2, pi.name().c_str(), pi.ಆವೃತ್ತಿ().c_str())
        );
}
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ ocl {

Context& initializeContextFromGL()
{
#ಆದರೆ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = ಶೂನ್ಯಸೂಚಕ;
    cl_uint dev_ಬಳಸಿದ = 0;
    cl_context context = ಶೂನ್ಯಸೂಚಕ;

    cl_int ಸ್ಥಿತಿ_2 = clGetPlatformIDs(0, ಖಾಲಿ, &platformsCnt);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", ಸ್ಥಿತಿ_2));
    ಆದರೆ (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    ಸ್ಥಿತಿ_2 = clGetPlatformIDs(platformsCnt, &platforms[0], ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", ಸ್ಥಿತಿ_2));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ತಾರ್ಕಿಕ sharingSupported = ಸುಳ್ಳು;

    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        ಸ್ಥಿತಿ_2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, ಖಾಲಿ, &devCnt);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", ಸ್ಥಿತಿ_2));

        ಪ್ರಯತ್ನಿಸು {
            devices = ಹೊಸ cl_device_id[devCnt];

            ಸ್ಥಿತಿ_2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, ಖಾಲಿ);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", ಸ್ಥಿತಿ_2));

            ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_ಗಾತ್ರ;
                ಸ್ಥಿತಿ_2 = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, ಖಾಲಿ, &extension_ಗಾತ್ರ );
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", ಸ್ಥಿತಿ_2));

                ಆದರೆ(extension_ಗಾತ್ರ > 0)
                {
                    std::string devString(extension_ಗಾತ್ರ, '\0');
                    ಸ್ಥಿತಿ_2 = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_ಗಾತ್ರ);
                    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                        ಮುಂದುವರಿ;
                    ಆದರೆ (extension_ಗಾತ್ರ > 0 && devString.size() >= extension_ಗಾತ್ರ)
                        devString.resize(extension_ಗಾತ್ರ - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    ಆಗ (spacePos != devString.npos) {
                        ಆದರೆ (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_ಬಳಸಿದ = i;
                            sharingSupported = ನಿಜ;
                            ನಿಲ್ಲಿಸು;
                        }
                        ಮಾಡು {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } ಆಗ (spacePos == oldPos);
                    }
                }
            }
        } ಹಿಡಿ(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            ಆದರೆ(devices != ಶೂನ್ಯಸೂಚಕ) {
                ಅಳಿಸು[] devices;
            }
            ಮುಂದುವರಿ;
        }

        ಆದರೆ (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#ಆದರೆ defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_ಗುಂಪು_Obj cgl_Share_ಗುಂಪು = CGLGet_Share_ಗುಂಪು(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_ಗುಂಪು,
                    0
                };
                context = clCreateContext(props, 0,0, ಖಾಲಿ, ಖಾಲಿ, &ci_Err_ಅಂಕೆ);
#ಅಥವಾಆದರೆ defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ಬಳಸಿದ], ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
#ಅಥವಾಆದರೆ defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ಬಳಸಿದ], ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
#ಅಥವಾಆದರೆ defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ಬಳಸಿದ], ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
#ಆದರೆ_ಅಂತ್ಯ
        }

        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", ಸ್ಥಿತಿ_2));
        ಇಲ್ಲದಿದ್ದರೆ
            ನಿಲ್ಲಿಸು;
    }


    cl_platform_id platform = platforms[dev_ಬಳಸಿದ];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_ಬಳಸಿದ]);
    clReleaseDevice(devices[dev_ಬಳಸಿದ]);
    clReleaseContext(context);
    clExecCtx.bind();
    ಹಿಂತಿರುಗಿ const_cast<Context&>(clExecCtx.getContext());
#ಆದರೆ_ಅಂತ್ಯ
}

} // namespace cv::ogl::ocl

ಶೂನ್ಯ convert_ಗಮ್ಯ_GLTexture2_D(ಒಳಹರಿವು_Array src, Texture2D& texture)
{
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(texture);
#ಆದರೆ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    ಗಾತ್ರ_2 src_ಗಾತ್ರ = src.size();
    CV_Assert(src_ಗಾತ್ರ.ಅಗಲ == (ಪೂರ್ಣಾಂಕ)texture.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ() && src_ಗಾತ್ರ.ಎತ್ತರ == (ಪೂರ್ಣಾಂಕ)texture.ಅಡ್ಡಸಾಲುಗಳು());

    ಬಳಸಿ ನಾಮಾಕಾಶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", ಸ್ಥಿತಿ_2));

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_GLObjects(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", ಸ್ಥಿತಿ_2));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1};
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_ಗಮ್ಯ_Image(q, cl_ಬಫರ್, clImage, offset, dst_origin, region, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", ಸ್ಥಿತಿ_2));
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_GLObjects(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", ಸ್ಥಿತಿ_2));

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", ಸ್ಥಿತಿ_2));

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", ಸ್ಥಿತಿ_2));
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ convertFromGLTexture2D(ಸ್ಥಿರಾಂಕ Texture2D& texture, OutputArray dst)
{
    CV_ಬಳಸದ(texture); CV_ಬಳಸದ(dst);
#ಆದರೆ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    // check texture format
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    ಪೂರ್ಣಾಂಕ textureType = dtype;
    CV_Assert(textureType >= 0);

    ಬಳಸಿ ನಾಮಾಕಾಶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", ಸ್ಥಿತಿ_2));

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_GLObjects(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", ಸ್ಥಿತಿ_2));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1};
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_Image_ಗಮ್ಯ_ಬಫರ್(q, clImage, cl_ಬಫರ್, src_origin, region, offset, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", ಸ್ಥಿತಿ_2));
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_GLObjects(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", ಸ್ಥಿತಿ_2));

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", ಸ್ಥಿತಿ_2));

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", ಸ್ಥಿತಿ_2));
#ಆದರೆ_ಅಂತ್ಯ
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ಸ್ಥಿರಾಂಕ ಬಫರ್_2& ಬಫರ್, AccessFlag accessFlags)
{
    CV_ಬಳಸದ(ಬಫರ್); CV_ಬಳಸದ(accessFlags);
#ಆದರೆ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_ಸರತಿ cl_ಸರತಿ = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

    ಪೂರ್ಣಾಂಕ clAccessFlags = 0;
    ಆಯ್ಕೆ (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    ಪೂರ್ವನಿಯೋಜಿತ:
    ಸ್ಥಿತಿ ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        ನಿಲ್ಲಿಸು;
    }

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem cl_ಬಫರ್ = clCreateFromGLBuffer(context, clAccessFlags, ಬಫರ್.bufId(), &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", ಸ್ಥಿತಿ_2));

    gl::ಮುಕ್ತಾಯ();

    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_GLObjects(cl_ಸರತಿ, 1, &cl_ಬಫರ್, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", ಸ್ಥಿತಿ_2));

    size_t ಹೆಜ್ಜೆ = ಬಫರ್.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ() * ಬಫರ್.elem_ಗಾತ್ರ();
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು = ಬಫರ್.ಅಡ್ಡಸಾಲುಗಳು();
    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಬಫರ್.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ();
    ಪೂರ್ಣಾಂಕ type = ಬಫರ್.type();

    UMat u;
    convert_From_ಬಫರ್(cl_ಬಫರ್, ಹೆಜ್ಜೆ, ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type, u);
    ಹಿಂತಿರುಗಿ u;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ unmapGLBuffer(UMat& u)
{
    CV_ಬಳಸದ(u);
#ಆದರೆ !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    ಬಳಸಿ ನಾಮಾಕಾಶ cv::ocl;
    cl_command_ಸರತಿ cl_ಸರತಿ = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_GLObjects(cl_ಸರತಿ, 1, &cl_ಬಫರ್, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", ಸ್ಥಿತಿ_2));

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(cl_ಸರತಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", ಸ್ಥಿತಿ_2));

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(cl_ಬಫರ್);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", ಸ್ಥಿತಿ_2));
#ಆದರೆ_ಅಂತ್ಯ
}

}} // namespace cv::ogl
