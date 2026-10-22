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
   #परिभाषय GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#अन्यथा
   #परिभाषय GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    यदि (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_संस्करणम्
#        define GL_संस्करणम् 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#अन्यथा // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#यदि_अन्तः // HAVE_OPENGL

उपयुज्य नामाकाश cv;
उपयुज्य नामाकाश cv::cuda;

#यदि defined(_MSC_VER)
#निर्देशः warning(disable : 4702)  // unreachable code
#यदि_अन्तः

नामाकाश
{
#यदि_अपरिभाषितम् HAVE_OPENGL
अन्तःस्थ स्थिर CV_NORETURN शून्य throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#नोचेद्यदि defined _DEBUG
अन्तःस्थ स्थिर तार्किक checkError(ध्रुव वर्ण* file, ध्रुव पूर्णाङ्क पङ्क्तिः, ध्रुव वर्ण* func = 0)
{
    GLenum err = gl::GetError();
    यदि (err != gl::NO_ERROR_)
    {
        ध्रुव वर्ण* सन्देशः;
        विकल्प (err)
        {
        पक्ष gl::INVALID_ENUM:
            सन्देशः = "An unacceptable value is specified for an enumerated argument";
            भङ्ग;
        पक्ष gl::INVALID_मानम्:
            सन्देशः = "A numeric argument is out of range";
            भङ्ग;
        पक्ष gl::INVALID_OPERATION:
            सन्देशः = "The specified operation is not allowed in the current state";
            भङ्ग;
        पक्ष gl::बहिर्गमनम्_सम्बन्धि_स्मृतिः:
            सन्देशः = "There is not enough memory left to execute the command";
            भङ्ग;
        स्वभाव:
            सन्देशः = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, सन्देशः, file, पङ्क्तिः);
    }
    प्रत्यागम सत्य;
}
#यदि_अन्तः // HAVE_OPENGL
} // namespace

#परिभाषय CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#यदि_परिभाषितम् HAVE_OPENGL
नामाकाश
{
    ध्रुव GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#यदि_अन्तः

////////////////////////////////////////////////////////////////////////
// setGlDevice

शून्य cv::cuda::समुच्चयः_Gl_Device(पूर्णाङ्क device)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(device);
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        CV_अप्रयुक्तम्(device);
        throw_no_cuda();
    #अन्यथा
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #यदि_अन्तः
#यदि_अन्तः
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

        शून्य पञ्जिकाकोशः_बफर(GLuint बफर);
        शून्य release();

        शून्य copyFrom(ध्रुव शून्य* src, size_t spitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream = 0);
        शून्य copy_गन्तव्यम्(शून्य* dst, size_t dpitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream = 0);

        शून्य* map(cuda_प्रवाहः_t stream = 0);
        शून्य unmap(cuda_प्रवाहः_t stream = 0);

    गुप्त:
        cudaGraphicsResource_t resource_;
        GLuint बफर_3;

        वर्ग GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), बफर_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    शून्य CudaResource::पञ्जिकाकोशः_बफर(GLuint बफर)
    {
        CV_DbgAssert( बफर != 0 );

        यदि (बफर_3 == बफर)
            प्रत्यागम;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cuda_Graphics_GLRegister_बफर(&resource, बफर, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        बफर_3 = बफर;
    }

    शून्य CudaResource::release()
    {
        यदि (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        बफर_3 = 0;
    }

    वर्ग CudaResource::GraphicsMapHolder
    {
    सार्वजनिक:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_प्रवाहः_t stream);
        ~GraphicsMapHolder();

        शून्य reset();

    गुप्त:
        cudaGraphicsResource_t* resource_;
        cuda_प्रवाहः_t प्रवाहः_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_प्रवाहः_t stream) : resource_(resource), प्रवाहः_3(stream)
    {
        यदि (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, प्रवाहः_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        यदि (resource_)
            cudaGraphicsUnmapResources(1, resource_, प्रवाहः_3);
    }

    शून्य CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    शून्य CudaResource::copyFrom(ध्रुव शून्य* src, size_t spitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_अप्रयुक्तम्(h);

        शून्य* dst;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( विस्तारः * height == size );

        यदि (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, विस्तारः, src, spitch, विस्तारः, height, cuda_Memcpy_Device_गन्तव्यम्_Device) );
        अन्यथा
            cudaSafeCall( cudaMemcpy2DAsync(dst, विस्तारः, src, spitch, विस्तारः, height, cuda_Memcpy_Device_गन्तव्यम्_Device, stream) );
    }

    शून्य CudaResource::copy_गन्तव्यम्(शून्य* dst, size_t dpitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_अप्रयुक्तम्(h);

        शून्य* src;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( विस्तारः * height == size );

        यदि (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, विस्तारः, विस्तारः, height, cuda_Memcpy_Device_गन्तव्यम्_Device) );
        अन्यथा
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, विस्तारः, विस्तारः, height, cuda_Memcpy_Device_गन्तव्यम्_Device, stream) );
    }

    शून्य* CudaResource::map(cuda_प्रवाहः_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        शून्य* ptr;
        size_t size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        प्रत्यागम ptr;
    }

    शून्य CudaResource::unmap(cuda_प्रवाहः_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#यदि_अन्तः

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#यदि_अपरिभाषितम् HAVE_OPENGL

वर्ग cv::ogl::बफर_2::Impl
{
};

#अन्यथा

वर्ग cv::ogl::बफर_2::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint bufId, तार्किक autoRelease);
    Impl(GLsizeiptr size, ध्रुव GLvoid* दत्त, GLenum target, तार्किक autoRelease);
    ~Impl();

    शून्य bind(GLenum target) ध्रुव;

    शून्य copyFrom(GLuint srcBuf, GLsizeiptr size);

    शून्य copyFrom(GLsizeiptr size, ध्रुव GLvoid* दत्त);
    शून्य copy_गन्तव्यम्(GLsizeiptr size, GLvoid* दत्त) ध्रुव;

    शून्य resize(GLsizeiptr size, GLenum target);

    शून्य* mapHost(GLenum access);
    शून्य unmapHost();

#यदि_परिभाषितम् HAVE_CUDA
    शून्य copyFrom(ध्रुव शून्य* src, size_t spitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream = 0);
    शून्य copy_गन्तव्यम्(शून्य* dst, size_t dpitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream = 0) ध्रुव;

    शून्य* mapDevice(cuda_प्रवाहः_t stream = 0);
    शून्य unmapDevice(cuda_प्रवाहः_t stream = 0);
#यदि_अन्तः

    शून्य समुच्चयः_Auto_Release(तार्किक flag) { autoRelease_ = flag; }

    GLuint bufId() ध्रुव { प्रत्यागम bufId_; }

गुप्त:
    Impl();

    GLuint bufId_;
    तार्किक autoRelease_;

#यदि_परिभाषितम् HAVE_CUDA
    परिवर्तनीय CudaResource cudaResource_;
#यदि_अन्तः
};

ध्रुव Ptr<cv::ogl::बफर_2::Impl>& cv::ogl::बफर_2::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::बफर_2::Impl::Impl() : bufId_(0), autoRelease_(असत्य)
{
}

cv::ogl::बफर_2::Impl::Impl(GLuint abufId, तार्किक autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_बफर(abufId) == gl::TRUE_ );
}

cv::ogl::बफर_2::Impl::Impl(GLsizeiptr size, ध्रुव GLvoid* दत्त, GLenum target, तार्किक autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_बफर(target, bufId_);
    CV_CheckGlError();

    gl::बफर_दत्त(target, size, दत्त, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_बफर(target, 0);
    CV_CheckGlError();
}

cv::ogl::बफर_2::Impl::~Impl()
{
    यदि (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

शून्य cv::ogl::बफर_2::Impl::bind(GLenum target) ध्रुव
{
    gl::Bind_बफर(target, bufId_);
    CV_CheckGlError();
}

शून्य cv::ogl::बफर_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_बफर(gl::COPY_WRITE_बफर, bufId_);
    CV_CheckGlError();

    gl::Bind_बफर(gl::COPY_READ_बफर, srcBuf);
    CV_CheckGlError();

    gl::Copy_बफर_Sub_दत्त(gl::COPY_READ_बफर, gl::COPY_WRITE_बफर, 0, 0, size);
    CV_CheckGlError();
}

शून्य cv::ogl::बफर_2::Impl::copyFrom(GLsizeiptr size, ध्रुव GLvoid* दत्त)
{
    gl::Bind_बफर(gl::COPY_WRITE_बफर, bufId_);
    CV_CheckGlError();

    gl::बफर_Sub_दत्त(gl::COPY_WRITE_बफर, 0, size, दत्त);
    CV_CheckGlError();
}

शून्य cv::ogl::बफर_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_बफर(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::बफर_दत्त(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_बफर(target, 0);
    CV_CheckGlError();
}

शून्य cv::ogl::बफर_2::Impl::copy_गन्तव्यम्(GLsizeiptr size, GLvoid* दत्त) ध्रुव
{
    gl::Bind_बफर(gl::COPY_READ_बफर, bufId_);
    CV_CheckGlError();

    gl::Get_बफर_Sub_दत्त(gl::COPY_READ_बफर, 0, size, दत्त);
    CV_CheckGlError();
}

शून्य* cv::ogl::बफर_2::Impl::mapHost(GLenum access)
{
    gl::Bind_बफर(gl::COPY_READ_बफर, bufId_);
    CV_CheckGlError();

    GLvoid* दत्त = gl::Map_बफर(gl::COPY_READ_बफर, access);
    CV_CheckGlError();

    प्रत्यागम दत्त;
}

शून्य cv::ogl::बफर_2::Impl::unmapHost()
{
    gl::Unmap_बफर(gl::COPY_READ_बफर);
}

#यदि_परिभाषितम् HAVE_CUDA

शून्य cv::ogl::बफर_2::Impl::copyFrom(ध्रुव शून्य* src, size_t spitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream)
{
    cudaResource_.पञ्जिकाकोशः_बफर(bufId_);
    cudaResource_.copyFrom(src, spitch, विस्तारः, height, stream);
}

शून्य cv::ogl::बफर_2::Impl::copy_गन्तव्यम्(शून्य* dst, size_t dpitch, size_t विस्तारः, size_t height, cuda_प्रवाहः_t stream) ध्रुव
{
    cudaResource_.पञ्जिकाकोशः_बफर(bufId_);
    cudaResource_.copy_गन्तव्यम्(dst, dpitch, विस्तारः, height, stream);
}

शून्य* cv::ogl::बफर_2::Impl::mapDevice(cuda_प्रवाहः_t stream)
{
    cudaResource_.पञ्जिकाकोशः_बफर(bufId_);
    प्रत्यागम cudaResource_.map(stream);
}

शून्य cv::ogl::बफर_2::Impl::unmapDevice(cuda_प्रवाहः_t stream)
{
    cudaResource_.unmap(stream);
}

#यदि_अन्तः // HAVE_CUDA

#यदि_अन्तः // HAVE_OPENGL

cv::ogl::बफर_2::बफर_2() : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), type_(0)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#यदि_अन्तः
}

cv::ogl::बफर_2::बफर_2(पूर्णाङ्क arows, पूर्णाङ्क acols, पूर्णाङ्क atype, अचिह्नित पूर्णाङ्क abufId, तार्किक autoRelease) : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), type_(0)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arows);
    CV_अप्रयुक्तम्(acols);
    CV_अप्रयुक्तम्(atype);
    CV_अप्रयुक्तम्(abufId);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(abufId, autoRelease));
    पङ्क्तिकाः_2 = arows;
    स्तम्भसङ्ख्या_2 = acols;
    type_ = atype;
#यदि_अन्तः
}

cv::ogl::बफर_2::बफर_2(आकार asize, पूर्णाङ्क atype, अचिह्नित पूर्णाङ्क abufId, तार्किक autoRelease) : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), type_(0)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(asize);
    CV_अप्रयुक्तम्(atype);
    CV_अप्रयुक्तम्(abufId);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(abufId, autoRelease));
    पङ्क्तिकाः_2 = asize.height;
    स्तम्भसङ्ख्या_2 = asize.विस्तारः;
    type_ = atype;
#यदि_अन्तः
}

cv::ogl::बफर_2::बफर_2(निवेशः_Array arr, Target target, तार्किक autoRelease) : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), type_(0)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(target);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क विधा = arr.विधा();

    विकल्प (विधा)
    {
    पक्ष निवेशः_Array_2::OPENGL_बफर:
    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        भङ्ग;

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ध्रुव GLsizeiptr asize = mat.पङ्क्तिकाः * mat.स्तम्भसङ्ख्या * mat.elem_आकार();
            impl_.reset(नूतन Impl(asize, mat.दत्त, target, autoRelease));
            पङ्क्तिकाः_2 = mat.पङ्क्तिकाः;
            स्तम्भसङ्ख्या_2 = mat.स्तम्भसङ्ख्या;
            type_ = mat.type();
            भङ्ग;
        }
    }
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::create(पूर्णाङ्क arows, पूर्णाङ्क acols, पूर्णाङ्क atype, Target target, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arows);
    CV_अप्रयुक्तम्(acols);
    CV_अप्रयुक्तम्(atype);
    CV_अप्रयुक्तम्(target);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    यदि (पङ्क्तिकाः_2 != arows || स्तम्भसङ्ख्या_2 != acols || type_ != atype)
    {
        ध्रुव GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_आकार(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        यदि (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->समुच्चयः_Auto_Release(autoRelease);
        }
        अन्यथा
            impl_.reset(नूतन Impl(asize, 0, target, autoRelease));
        पङ्क्तिकाः_2 = arows;
        स्तम्भसङ्ख्या_2 = acols;
        type_ = atype;
    }
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::release()
{
#यदि_परिभाषितम् HAVE_OPENGL
    यदि (impl_)
        impl_->समुच्चयः_Auto_Release(सत्य);
    impl_ = Impl::empty();
    पङ्क्तिकाः_2 = 0;
    स्तम्भसङ्ख्या_2 = 0;
    type_ = 0;
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    impl_->समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::copyFrom(निवेशः_Array arr, Target target, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(target);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क विधा = arr.विधा();

    ध्रुव आकार asize = arr.size();
    ध्रुव पूर्णाङ्क atype = arr.type();
    create(asize, atype, target, autoRelease);

    विकल्प (विधा)
    {
    पक्ष निवेशः_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2 buf = arr.get_OGl_बफर();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_आकार(atype));
            भङ्ग;
        }

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        {
            #यदि_अपरिभाषितम् HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.दत्त, dmat.चरणः, dmat.स्तम्भसङ्ख्या * dmat.elem_आकार(), dmat.पङ्क्तिकाः);
            #यदि_अन्तः

            भङ्ग;
        }

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_आकार(atype), mat.दत्त);
        }
    }
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::copyFrom(निवेशः_Array arr, cuda::प्रवाहः& stream, Target target, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(stream);
    CV_अप्रयुक्तम्(target);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        CV_अप्रयुक्तम्(arr);
        CV_अप्रयुक्तम्(stream);
        CV_अप्रयुक्तम्(target);
        CV_अप्रयुक्तम्(autoRelease);
        throw_no_cuda();
    #अन्यथा
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.दत्त, dmat.चरणः, dmat.स्तम्भसङ्ख्या * dmat.elem_आकार(), dmat.पङ्क्तिकाः, cuda::प्रवाहः_Accessor::get_प्रवाहः(stream));
    #यदि_अन्तः
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::copy_गन्तव्यम्(OutputArray arr) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क विधा = arr.विधा();

    विकल्प (विधा)
    {
    पक्ष निवेशः_Array_2::OPENGL_बफर:
        {
            arr.get_OGl_बफर_Ref().copyFrom(*एतत्);
            भङ्ग;
        }

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        {
            #यदि_अपरिभाषितम् HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type_);
                impl_->copy_गन्तव्यम्(dmat.दत्त, dmat.चरणः, dmat.स्तम्भसङ्ख्या * dmat.elem_आकार(), dmat.पङ्क्तिकाः);
            #यदि_अन्तः

            भङ्ग;
        }

    स्वभाव:
        {
            arr.create(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_गन्तव्यम्(mat.पङ्क्तिकाः * mat.स्तम्भसङ्ख्या * mat.elem_आकार(), mat.दत्त);
        }
    }
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::copy_गन्तव्यम्(OutputArray arr, cuda::प्रवाहः& stream) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(stream);
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        CV_अप्रयुक्तम्(arr);
        CV_अप्रयुक्तम्(stream);
        throw_no_cuda();
    #अन्यथा
        arr.create(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_गन्तव्यम्(dmat.दत्त, dmat.चरणः, dmat.स्तम्भसङ्ख्या * dmat.elem_आकार(), dmat.पङ्क्तिकाः, cuda::प्रवाहः_Accessor::get_प्रवाहः(stream));
    #यदि_अन्तः
#यदि_अन्तः
}

cv::ogl::बफर_2 cv::ogl::बफर_2::clone(Target target, तार्किक autoRelease) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(target);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    ogl::बफर_2 buf;
    buf.copyFrom(*एतत्, target, autoRelease);
    प्रत्यागम buf;
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::bind(Target target) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(target);
    throw_no_ogl();
#अन्यथा
    impl_->bind(target);
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::unbind(Target target)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(target);
    throw_no_ogl();
#अन्यथा
    gl::Bind_बफर(target, 0);
    CV_CheckGlError();
#यदि_अन्तः
}

Mat cv::ogl::बफर_2::mapHost(Access access)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(access);
    throw_no_ogl();
#अन्यथा
    प्रत्यागम Mat(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type_, impl_->mapHost(access));
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::unmapHost()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->unmapHost();
#यदि_अन्तः
}

GpuMat cv::ogl::बफर_2::mapDevice()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        throw_no_cuda();
    #अन्यथा
        प्रत्यागम GpuMat(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type_, impl_->mapDevice());
    #यदि_अन्तः
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::unmapDevice()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        throw_no_cuda();
    #अन्यथा
        impl_->unmapDevice();
    #यदि_अन्तः
#यदि_अन्तः
}

cuda::GpuMat cv::ogl::बफर_2::mapDevice(cuda::प्रवाहः& stream)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(stream);
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        CV_अप्रयुक्तम्(stream);
        throw_no_cuda();
    #अन्यथा
        प्रत्यागम GpuMat(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, type_, impl_->mapDevice(cuda::प्रवाहः_Accessor::get_प्रवाहः(stream)));
    #यदि_अन्तः
#यदि_अन्तः
}

शून्य cv::ogl::बफर_2::unmapDevice(cuda::प्रवाहः& stream)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(stream);
    throw_no_ogl();
#अन्यथा
    #यदि_अपरिभाषितम् HAVE_CUDA
        CV_अप्रयुक्तम्(stream);
        throw_no_cuda();
    #अन्यथा
        impl_->unmapDevice(cuda::प्रवाहः_Accessor::get_प्रवाहः(stream));
    #यदि_अन्तः
#यदि_अन्तः
}

अचिह्नित पूर्णाङ्क cv::ogl::बफर_2::bufId() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->bufId();
#यदि_अन्तः
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#यदि_अपरिभाषितम् HAVE_OPENGL

वर्ग cv::ogl::Texture2D::Impl
{
};

#अन्यथा

वर्ग cv::ogl::Texture2D::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl> empty();

    Impl(GLuint texId, तार्किक autoRelease);
    Impl(GLint आन्तरिकम्_Format, GLsizei विस्तारः, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid* pixels, तार्किक autoRelease);
    ~Impl();

    शून्य copyFrom(GLsizei विस्तारः, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid *pixels);
    शून्य copy_गन्तव्यम्(GLenum format, GLenum type, GLvoid* pixels) ध्रुव;

    शून्य bind() ध्रुव;

    शून्य समुच्चयः_Auto_Release(तार्किक flag) { autoRelease_ = flag; }

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

cv::ogl::Texture2D::Impl::Impl(GLint आन्तरिकम्_Format, GLsizei विस्तारः, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid* pixels, तार्किक autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, आन्तरिकम्_Format, विस्तारः, height, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    यदि (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

शून्य cv::ogl::Texture2D::Impl::copyFrom(GLsizei विस्तारः, GLsizei height, GLenum format, GLenum type, ध्रुव GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, विस्तारः, height, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

शून्य cv::ogl::Texture2D::Impl::copy_गन्तव्यम्(GLenum format, GLenum type, GLvoid* pixels) ध्रुव
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

#यदि_अन्तः // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), format_(NONE)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#यदि_अन्तः
}

cv::ogl::Texture2D::Texture2D(पूर्णाङ्क arows, पूर्णाङ्क acols, Format aformat, अचिह्नित पूर्णाङ्क atexId, तार्किक autoRelease) : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), format_(NONE)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arows);
    CV_अप्रयुक्तम्(acols);
    CV_अप्रयुक्तम्(aformat);
    CV_अप्रयुक्तम्(atexId);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(atexId, autoRelease));
    पङ्क्तिकाः_2 = arows;
    स्तम्भसङ्ख्या_2 = acols;
    format_ = aformat;
#यदि_अन्तः
}

cv::ogl::Texture2D::Texture2D(आकार asize, Format aformat, अचिह्नित पूर्णाङ्क atexId, तार्किक autoRelease) : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), format_(NONE)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(asize);
    CV_अप्रयुक्तम्(aformat);
    CV_अप्रयुक्तम्(atexId);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(atexId, autoRelease));
    पङ्क्तिकाः_2 = asize.height;
    स्तम्भसङ्ख्या_2 = asize.विस्तारः;
    format_ = aformat;
#यदि_अन्तः
}

cv::ogl::Texture2D::Texture2D(निवेशः_Array arr, तार्किक autoRelease) : पङ्क्तिकाः_2(0), स्तम्भसङ्ख्या_2(0), format_(NONE)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क विधा = arr.विधा();

    ध्रुव आकार asize = arr.size();
    ध्रुव पूर्णाङ्क atype = arr.type();

    ध्रुव पूर्णाङ्क गभीरता = CV_MAT_गभीरता(atype);
    ध्रुव पूर्णाङ्क cn = CV_MAT_CN(atype);

    CV_Assert( गभीरता <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ध्रुव Format आन्तरिकम्_Formats[] =
    {
        NONE, गभीरता_COMPONENT, NONE, RGB, RGBA
    };
    ध्रुव GLenum srcFormats[] =
    {
        0, gl::गभीरता_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    विकल्प (विधा)
    {
    पक्ष निवेशः_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2 buf = arr.get_OGl_बफर();
            buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_.reset(नूतन Impl(आन्तरिकम्_Formats[cn], asize.विस्तारः, asize.height, srcFormats[cn], gl_types[गभीरता], 0, autoRelease));
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            भङ्ग;
        }

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        {
            #यदि_अपरिभाषितम् HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                ogl::बफर_2 buf(dmat, ogl::बफर_2::PIXEL_UNPACK_बफर);
                buf.समुच्चयः_Auto_Release(सत्य);
                buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
                impl_.reset(नूतन Impl(आन्तरिकम्_Formats[cn], asize.विस्तारः, asize.height, srcFormats[cn], gl_types[गभीरता], 0, autoRelease));
                ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            #यदि_अन्तः

            भङ्ग;
        }

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_.reset(नूतन Impl(आन्तरिकम्_Formats[cn], asize.विस्तारः, asize.height, srcFormats[cn], gl_types[गभीरता], mat.दत्त, autoRelease));
            भङ्ग;
        }
    }

    पङ्क्तिकाः_2 = asize.height;
    स्तम्भसङ्ख्या_2 = asize.विस्तारः;
    format_ = आन्तरिकम्_Formats[cn];
#यदि_अन्तः
}

शून्य cv::ogl::Texture2D::create(पूर्णाङ्क arows, पूर्णाङ्क acols, Format aformat, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arows);
    CV_अप्रयुक्तम्(acols);
    CV_अप्रयुक्तम्(aformat);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    यदि (पङ्क्तिकाः_2 != arows || स्तम्भसङ्ख्या_2 != acols || format_ != aformat)
    {
        ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
        impl_.reset(नूतन Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        पङ्क्तिकाः_2 = arows;
        स्तम्भसङ्ख्या_2 = acols;
        format_ = aformat;
    }
#यदि_अन्तः
}

शून्य cv::ogl::Texture2D::release()
{
#यदि_परिभाषितम् HAVE_OPENGL
    यदि (impl_)
        impl_->समुच्चयः_Auto_Release(सत्य);
    impl_ = Impl::empty();
    पङ्क्तिकाः_2 = 0;
    स्तम्भसङ्ख्या_2 = 0;
    format_ = NONE;
#यदि_अन्तः
}

शून्य cv::ogl::Texture2D::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    impl_->समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

शून्य cv::ogl::Texture2D::copyFrom(निवेशः_Array arr, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क विधा = arr.विधा();

    ध्रुव आकार asize = arr.size();
    ध्रुव पूर्णाङ्क atype = arr.type();

    ध्रुव पूर्णाङ्क गभीरता = CV_MAT_गभीरता(atype);
    ध्रुव पूर्णाङ्क cn = CV_MAT_CN(atype);

    CV_Assert( गभीरता <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ध्रुव Format आन्तरिकम्_Formats[] =
    {
        NONE, गभीरता_COMPONENT, NONE, RGB, RGBA
    };
    ध्रुव GLenum srcFormats[] =
    {
        0, gl::गभीरता_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, आन्तरिकम्_Formats[cn], autoRelease);

    विकल्प(विधा)
    {
    पक्ष निवेशः_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2 buf = arr.get_OGl_बफर();
            buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_->copyFrom(asize.विस्तारः, asize.height, srcFormats[cn], gl_types[गभीरता], 0);
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            भङ्ग;
        }

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        {
            #यदि_अपरिभाषितम् HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                ogl::बफर_2 buf(dmat, ogl::बफर_2::PIXEL_UNPACK_बफर);
                buf.समुच्चयः_Auto_Release(सत्य);
                buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
                impl_->copyFrom(asize.विस्तारः, asize.height, srcFormats[cn], gl_types[गभीरता], 0);
                ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            #यदि_अन्तः

            भङ्ग;
        }

    स्वभाव:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_->copyFrom(asize.विस्तारः, asize.height, srcFormats[cn], gl_types[गभीरता], mat.दत्त);
        }
    }
#यदि_अन्तः
}

शून्य cv::ogl::Texture2D::copy_गन्तव्यम्(OutputArray arr, पूर्णाङ्क ddepth, तार्किक autoRelease) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(ddepth);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    ध्रुव पूर्णाङ्क विधा = arr.विधा();

    ध्रुव पूर्णाङ्क cn = format_ == गभीरता_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ध्रुव GLenum dstFormat = format_ == गभीरता_COMPONENT ? gl::गभीरता_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    विकल्प(विधा)
    {
    पक्ष निवेशः_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2& buf = arr.get_OGl_बफर_Ref();
            buf.create(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, CV_MAKE_TYPE(ddepth, cn), ogl::बफर_2::PIXEL_PACK_बफर, autoRelease);
            buf.bind(ogl::बफर_2::PIXEL_PACK_बफर);
            impl_->copy_गन्तव्यम्(dstFormat, gl_types[ddepth], 0);
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_PACK_बफर);
            भङ्ग;
        }

    पक्ष निवेशः_Array_2::CUDA_GPU_MAT:
        {
            #यदि_अपरिभाषितम् HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                ogl::बफर_2 buf(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, CV_MAKE_TYPE(ddepth, cn), ogl::बफर_2::PIXEL_PACK_बफर);
                buf.समुच्चयः_Auto_Release(सत्य);
                buf.bind(ogl::बफर_2::PIXEL_PACK_बफर);
                impl_->copy_गन्तव्यम्(dstFormat, gl_types[ddepth], 0);
                ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_PACK_बफर);
                buf.copy_गन्तव्यम्(arr);
            #यदि_अन्तः

            भङ्ग;
        }

    स्वभाव:
        {
            arr.create(पङ्क्तिकाः_2, स्तम्भसङ्ख्या_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_PACK_बफर);
            impl_->copy_गन्तव्यम्(dstFormat, gl_types[ddepth], mat.दत्त);
        }
    }
#यदि_अन्तः
}

शून्य cv::ogl::Texture2D::bind() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#यदि_अन्तः
}

अचिह्नित पूर्णाङ्क cv::ogl::Texture2D::texId() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->texId();
#यदि_अन्तः
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#यदि_अपरिभाषितम् HAVE_OPENGL

वर्ग cv::ogl::Texture_बफर::Impl
{
};

#अन्यथा

वर्ग cv::ogl::Texture_बफर::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl> empty();

    Impl();
    Impl(GLenum आन्तरिकम्_Format, GLuint bufId, तार्किक autoRelease);
    ~Impl();

    शून्य bind(GLint unit) ध्रुव;

    शून्य समुच्चयः_Auto_Release(तार्किक flag) { autoRelease_ = flag; }

    GLuint texId() ध्रुव { प्रत्यागम texId_; }

गुप्त:
    GLuint texId_;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::Texture_बफर::Impl> cv::ogl::Texture_बफर::Impl::empty()
{
    स्थिर Ptr<Impl> p = makePtr<Impl>();
    प्रत्यागम p;
}

cv::ogl::Texture_बफर::Impl::Impl() : texId_(0), autoRelease_(असत्य)
{
}

cv::ogl::Texture_बफर::Impl::Impl(GLenum आन्तरिकम्_Format, GLuint bufId, तार्किक autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_बफर, texId_);
    CV_CheckGlError();

    gl::Tex_बफर(gl::TEXTURE_बफर, आन्तरिकम्_Format, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_बफर, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_बफर::Impl::~Impl()
{
    यदि (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

शून्य cv::ogl::Texture_बफर::Impl::bind(GLint unit) ध्रुव
{
    gl::सक्रिय_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_बफर, texId_);
    CV_CheckGlError();
}

#यदि_अन्तः // HAVE_OPENGL

cv::ogl::Texture_बफर::Texture_बफर() : format_(NONE)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#यदि_अन्तः
}

cv::ogl::Texture_बफर::Texture_बफर(ध्रुव बफर_2& buf, Format aformat, तार्किक autoRelease) : format_(NONE)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(buf);
    CV_अप्रयुक्तम्(aformat);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#यदि_अन्तः
}

शून्य cv::ogl::Texture_बफर::create(ध्रुव बफर_2& buf, Format aformat, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(buf);
    CV_अप्रयुक्तम्(aformat);
    CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#यदि_अन्तः
}

शून्य cv::ogl::Texture_बफर::release()
{
#यदि_परिभाषितम् HAVE_OPENGL
    यदि (impl_)
        impl_->समुच्चयः_Auto_Release(सत्य);
    impl_ = Impl::empty();
#यदि_अन्तः
    buf_ = बफर_2();
    format_ = NONE;
}

शून्य cv::ogl::Texture_बफर::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    impl_->समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

शून्य cv::ogl::Texture_बफर::bind(पूर्णाङ्क unit) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(unit);
    throw_no_ogl();
#अन्यथा
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#यदि_अन्तः
}

cv::ogl::Texture_बफर::Format cv::ogl::Texture_बफर::format() ध्रुव
{
    प्रत्यागम format_;
}

तार्किक cv::ogl::Texture_बफर::empty() ध्रुव
{
    प्रत्यागम format_ == NONE;
}

अचिह्नित पूर्णाङ्क cv::ogl::Texture_बफर::texId() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->texId();
#यदि_अन्तः
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

शून्य cv::ogl::Arrays::समुच्चयः_Vertex_Array(निवेशः_Array vertex)
{
    ध्रुव पूर्णाङ्क cn = vertex.channels();
    ध्रुव पूर्णाङ्क गभीरता = vertex.गभीरता();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( गभीरता == CV_16S || गभीरता == CV_32S || गभीरता == CV_32F || गभीरता == CV_64F );

    यदि (vertex.विधा() == निवेशः_Array_2::OPENGL_बफर)
        vertex_ = vertex.get_OGl_बफर();
    अन्यथा
        vertex_.copyFrom(vertex);

    आकार_3 = vertex_.size().area();
}

शून्य cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    आकार_3 = 0;
}

शून्य cv::ogl::Arrays::समुच्चयः_Color_Array(निवेशः_Array color)
{
    ध्रुव पूर्णाङ्क cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    यदि (color.विधा() == निवेशः_Array_2::OPENGL_बफर)
        color_ = color.get_OGl_बफर();
    अन्यथा
        color_.copyFrom(color);
}

शून्य cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

शून्य cv::ogl::Arrays::समुच्चयः_प्रसामान्यम्_Array(निवेशः_Array प्रसामान्यम्)
{
    ध्रुव पूर्णाङ्क cn = प्रसामान्यम्.channels();
    ध्रुव पूर्णाङ्क गभीरता = प्रसामान्यम्.गभीरता();

    CV_Assert( cn == 3 );
    CV_Assert( गभीरता == CV_8S || गभीरता == CV_16S || गभीरता == CV_32S || गभीरता == CV_32F || गभीरता == CV_64F );

    यदि (प्रसामान्यम्.विधा() == निवेशः_Array_2::OPENGL_बफर)
        प्रसामान्यम्_2 = प्रसामान्यम्.get_OGl_बफर();
    अन्यथा
        प्रसामान्यम्_2.copyFrom(प्रसामान्यम्);
}

शून्य cv::ogl::Arrays::reset_प्रसामान्यम्_Array()
{
    प्रसामान्यम्_2.release();
}

शून्य cv::ogl::Arrays::समुच्चयः_Tex_Coord_Array(निवेशः_Array texCoord)
{
    ध्रुव पूर्णाङ्क cn = texCoord.channels();
    ध्रुव पूर्णाङ्क गभीरता = texCoord.गभीरता();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( गभीरता == CV_16S || गभीरता == CV_32S || गभीरता == CV_32F || गभीरता == CV_64F );

    यदि (texCoord.विधा() == निवेशः_Array_2::OPENGL_बफर)
        texCoord_ = texCoord.get_OGl_बफर();
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
    reset_प्रसामान्यम्_Array();
    resetTexCoordArray();
}

शून्य cv::ogl::Arrays::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    vertex_.समुच्चयः_Auto_Release(flag);
    color_.समुच्चयः_Auto_Release(flag);
    प्रसामान्यम्_2.समुच्चयः_Auto_Release(flag);
    texCoord_.समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

शून्य cv::ogl::Arrays::bind() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == आकार_3 );
    CV_Assert( प्रसामान्यम्_2.empty() || प्रसामान्यम्_2.size().area() == आकार_3 );
    CV_Assert( color_.empty() || color_.size().area() == आकार_3 );

    यदि (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::बफर_2::ARRAY_बफर);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.गभीरता()], 0, 0);
        CV_CheckGlError();
    }

    यदि (प्रसामान्यम्_2.empty())
    {
        gl::DisableClientState(gl::प्रसामान्यम्_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::प्रसामान्यम्_ARRAY);
        CV_CheckGlError();

        प्रसामान्यम्_2.bind(ogl::बफर_2::ARRAY_बफर);

        gl::प्रसामान्यम्_Pointer(gl_types[प्रसामान्यम्_2.गभीरता()], 0, 0);
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

        color_.bind(ogl::बफर_2::ARRAY_बफर);

        ध्रुव पूर्णाङ्क cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.गभीरता()], 0, 0);
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

        vertex_.bind(ogl::बफर_2::ARRAY_बफर);

        gl::VertexPointer(vertex_.channels(), gl_types[vertex_.गभीरता()], 0, 0);
        CV_CheckGlError();
    }

    ogl::बफर_2::unbind(ogl::बफर_2::ARRAY_बफर);
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#यदि_अपरिभाषितम् HAVE_OPENGL

वर्ग cv::ogl::VertexArray::Impl
{
};

#अन्यथा

वर्ग cv::ogl::VertexArray::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint vaId, तार्किक autoRelease);
    Impl(std::initializer_list<Attribute> गुणाः, तार्किक autoRelease);
    ~Impl();

    शून्य bind();

    शून्य समुच्चयः_Auto_Release(तार्किक flag) { autoRelease_ = flag; }

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

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> गुणाः, तार्किक autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    प्रति (स्वचालित& attribute : गुणाः)
    {
        attribute.बफर_3.bind(बफर_2::ARRAY_बफर);
        यदि (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.आकार_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ध्रुव शून्य*>(attribute.विस्थापनम्)
            );
            CV_CheckGlError();
        }
        अन्यथा
        {
            gl::VertexAttribPointer(
                attribute.shader_loc_,
                attribute.आकार_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ध्रुव शून्य*>(attribute.विस्थापनम्)
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

#यदि_अन्तः // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#यदि_अन्तः
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> गुणाः, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(गुणाः); CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(गुणाः, autoRelease));
#यदि_अन्तः
}

शून्य cv::ogl::VertexArray::create(std::initializer_list<Attribute> गुणाः, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(गुणाः); CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(गुणाः, autoRelease));
#यदि_अन्तः
}

शून्य cv::ogl::VertexArray::release()
{
#यदि_परिभाषितम् HAVE_OPENGL
    यदि (impl_)
        impl_->समुच्चयः_Auto_Release(सत्य);
    impl_ = Impl::empty();
#यदि_अन्तः
}

शून्य cv::ogl::VertexArray::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    impl_->समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

शून्य cv::ogl::VertexArray::bind() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#यदि_अन्तः
}

शून्य cv::ogl::VertexArray::unbind()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    gl::BindVertexArray(0);
#यदि_अन्तः
}

अचिह्नित पूर्णाङ्क cv::ogl::VertexArray::vaId() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->vaId();
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#यदि_अपरिभाषितम् HAVE_OPENGL

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

    शून्य समुच्चयः_Auto_Release(तार्किक flag) { autoRelease_ = flag; }

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

    GLint स्थितिः;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_स्थितिः, &स्थितिः);
    यदि (!स्थितिः) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_दीर्घता, &स्थितिः);
        info_log.resize(स्थितिः);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, शून्यसूचकः, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    यदि (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#यदि_अन्तः // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#यदि_अन्तः
}

cv::ogl::Shader::Shader(ध्रुव वर्ण* src, ShaderType type, तार्किक autoRelease) : type_(type)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(type); CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(src, type, autoRelease));
#यदि_अन्तः
}

शून्य cv::ogl::Shader::create(ध्रुव वर्ण* src, ShaderType type, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(type); CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(src, type, autoRelease));
    type_ = type;
#यदि_अन्तः
}

शून्य cv::ogl::Shader::release()
{
#यदि_परिभाषितम् HAVE_OPENGL
    यदि (impl_)
        impl_->समुच्चयः_Auto_Release(सत्य);
    impl_ = Impl::empty();
#यदि_अन्तः
}

शून्य cv::ogl::Shader::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    impl_->समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

अचिह्नित पूर्णाङ्क cv::ogl::Shader::shaderId() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->shaderId();
#यदि_अन्तः
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ध्रुव
{
    प्रत्यागम type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#यदि_अपरिभाषितम् HAVE_OPENGL

वर्ग cv::ogl::क्रमादेशः_2::Impl
{
};

#अन्यथा

वर्ग cv::ogl::क्रमादेशः_2::Impl
{
सार्वजनिक:
    स्थिर ध्रुव Ptr<Impl>& empty();

    Impl(GLuint क्रमादेशः_Id, तार्किक autoRelease);
    Impl(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease);
    ~Impl();

    शून्य bind();

    पूर्णाङ्क getAttributeLocation(ध्रुव वर्ण* name) ध्रुव;
    पूर्णाङ्क get_एकरूपम्_Location(ध्रुव वर्ण* name) ध्रुव;

    शून्य समुच्चयः_Auto_Release(तार्किक flag) { autoRelease_ = flag; }

    GLuint क्रमादेशः_Id() ध्रुव { प्रत्यागम क्रमादेशः_Id_2; }

गुप्त:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint क्रमादेशः_Id_2;
    तार्किक autoRelease_;
};

ध्रुव Ptr<cv::ogl::क्रमादेशः_2::Impl>& cv::ogl::क्रमादेशः_2::Impl::empty()
{
    स्थिर Ptr<Impl> p(नूतन Impl);
    प्रत्यागम p;
}

cv::ogl::क्रमादेशः_2::Impl::Impl() : क्रमादेशः_Id_2(0), autoRelease_(असत्य)
{
}

cv::ogl::क्रमादेशः_2::Impl::Impl(GLuint क्रमादेशः_Id, तार्किक autoRelease) : क्रमादेशः_Id_2(क्रमादेशः_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_क्रमादेशः(क्रमादेशः_Id) == gl::TRUE_ );
}

cv::ogl::क्रमादेशः_2::Impl::Impl(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease) : क्रमादेशः_Id_2(0), autoRelease_(autoRelease)
{
    क्रमादेशः_Id_2 = gl::Create_क्रमादेशः();
    CV_CheckGlError();

    CV_Assert( क्रमादेशः_Id_2 != 0 );

    gl::AttachShader(क्रमादेशः_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(क्रमादेशः_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint स्थितिः;
    gl::Link_क्रमादेशः(क्रमादेशः_Id_2);
    gl::GetProgramiv(क्रमादेशः_Id_2, gl::LINK_स्थितिः, &स्थितिः);
    यदि (!स्थितिः) {
        String info_log;
        gl::GetProgramiv(क्रमादेशः_Id_2, gl::INFO_LOG_दीर्घता, &स्थितिः);
        info_log.resize(स्थितिः);
        gl::Get_क्रमादेशः_Info_Log(क्रमादेशः_Id_2, GLsizei(info_log.size()) + 1, शून्यसूचकः, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::क्रमादेशः_2::Impl::~Impl()
{
    यदि (autoRelease_ && क्रमादेशः_Id_2)
        gl::Delete_क्रमादेशः(क्रमादेशः_Id_2);
}

शून्य cv::ogl::क्रमादेशः_2::Impl::bind()
{
    gl::Use_क्रमादेशः(क्रमादेशः_Id_2);
    CV_CheckGlError();
}

पूर्णाङ्क cv::ogl::क्रमादेशः_2::Impl::getAttributeLocation(ध्रुव वर्ण* name) ध्रुव
{
    // -1 if absent (e.g. optimized out); caller decides.
    पूर्णाङ्क location = gl::GetAttribLocation(क्रमादेशः_Id_2, name);
    CV_CheckGlError();
    प्रत्यागम location;
}

पूर्णाङ्क cv::ogl::क्रमादेशः_2::Impl::get_एकरूपम्_Location(ध्रुव वर्ण* name) ध्रुव
{
    // -1 if absent (e.g. optimized out); caller decides.
    पूर्णाङ्क location = gl::Get_एकरूपम्_Location(क्रमादेशः_Id_2, name);
    CV_CheckGlError();
    प्रत्यागम location;
}

#यदि_अन्तः // HAVE_OPENGL

cv::ogl::क्रमादेशः_2::क्रमादेशः_2()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#यदि_अन्तः
}

cv::ogl::क्रमादेशः_2::क्रमादेशः_2(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(vert); CV_अप्रयुक्तम्(frag); CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(vert, frag, autoRelease));
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::create(ध्रुव Shader& vert, ध्रुव Shader& frag, तार्किक autoRelease)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(vert); CV_अप्रयुक्तम्(frag); CV_अप्रयुक्तम्(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नूतन Impl(vert, frag, autoRelease));
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::release()
{
#यदि_परिभाषितम् HAVE_OPENGL
    यदि (impl_)
        impl_->समुच्चयः_Auto_Release(सत्य);
    impl_ = Impl::empty();
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_Auto_Release(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    impl_->समुच्चयः_Auto_Release(flag);
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::bind() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::unbind()
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    gl::Use_क्रमादेशः(0);
#यदि_अन्तः
}

पूर्णाङ्क cv::ogl::क्रमादेशः_2::getAttributeLocation(ध्रुव वर्ण* name) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(name);
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->getAttributeLocation(name);
#यदि_अन्तः
}

पूर्णाङ्क cv::ogl::क्रमादेशः_2::get_एकरूपम्_Location(ध्रुव वर्ण* name) ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(name);
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->get_एकरूपम्_Location(name);
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_एकरूपम्_Vec3(पूर्णाङ्क loc, Vec3f vec)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(loc); CV_अप्रयुक्तम्(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_एकरूपम्_Vec2(पूर्णाङ्क loc, Vec2f vec)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(loc); CV_अप्रयुक्तम्(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform2f(loc, vec(0), vec(1));
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_एकरूपम्_Vec4(पूर्णाङ्क loc, Vec4f vec)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(loc); CV_अप्रयुक्तम्(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_एकरूपम्_Mat4x4(पूर्णाङ्क loc, Matx44f mat)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(loc); CV_अप्रयुक्तम्(mat);
    throw_no_ogl();
#अन्यथा
    gl::एकरूपम्_Matrix4fv(loc, 1, सत्य, mat.val);
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_Uniform1f(पूर्णाङ्क loc, भिन्न value)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(loc); CV_अप्रयुक्तम्(value);
    throw_no_ogl();
#अन्यथा
    gl::Uniform1f(loc, value);
#यदि_अन्तः
}

शून्य cv::ogl::क्रमादेशः_2::समुच्चयः_Uniform1i(पूर्णाङ्क loc, पूर्णाङ्क value)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(loc); CV_अप्रयुक्तम्(value);
    throw_no_ogl();
#अन्यथा
    gl::Uniform1i(loc, value);
#यदि_अन्तः
}

अचिह्नित पूर्णाङ्क cv::ogl::क्रमादेशः_2::क्रमादेशः_Id() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    प्रत्यागम impl_->क्रमादेशः_Id();
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// Rendering

शून्य cv::ogl::render(ध्रुव ogl::Texture2D& tex, Rect_<द्विगुण> wndRect, Rect_<द्विगुण> texRect)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(tex);
    CV_अप्रयुक्तम्(wndRect);
    CV_अप्रयुक्तम्(texRect);
    throw_no_ogl();
#अन्यथा
    यदि (!tex.empty())
    {
        gl::Matrix_बहुलकम्(gl::PROJECTION);
        gl::आरोपय_तटस्थः();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::Matrix_बहुलकम्(gl::MODELVIEW);
        gl::आरोपय_तटस्थः();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_बहुलकम्, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::रैखिकम्);
        CV_CheckGlError();

        ध्रुव द्विगुण vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.विस्तारः, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.विस्तारः, wndRect.y, 0.0
        };
        ध्रुव द्विगुण texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.height,
            texRect.x + texRect.विस्तारः, texRect.y + texRect.height,
            texRect.x + texRect.विस्तारः, texRect.y
        };

        ogl::बफर_2::unbind(ogl::बफर_2::ARRAY_बफर);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::TexCoordPointer(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::प्रसामान्यम्_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();

        gl::VertexPointer(3, gl::DOUBLE, 0, vertex);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#यदि_अन्तः
}

शून्य cv::ogl::render(ध्रुव ogl::Arrays& arr, पूर्णाङ्क बहुलकम्, Scalar color)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(बहुलकम्);
    CV_अप्रयुक्तम्(color);
    throw_no_ogl();
#अन्यथा
    यदि (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(बहुलकम्, 0, arr.size());
    }
#यदि_अन्तः
}

शून्य cv::ogl::render(ध्रुव ogl::Arrays& arr, निवेशः_Array indices, पूर्णाङ्क बहुलकम्, Scalar color)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(arr);
    CV_अप्रयुक्तम्(indices);
    CV_अप्रयुक्तम्(बहुलकम्);
    CV_अप्रयुक्तम्(color);
    throw_no_ogl();
#अन्यथा
    यदि (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ध्रुव पूर्णाङ्क विधा = indices.विधा();

        विकल्प (विधा)
        {
        पक्ष निवेशः_Array_2::OPENGL_बफर :
            {
                ogl::बफर_2 buf = indices.get_OGl_बफर();

                ध्रुव पूर्णाङ्क गभीरता = buf.गभीरता();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( गभीरता <= CV_32S );

                GLenum type;
                यदि (गभीरता < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                अन्यथा यदि (गभीरता < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                अन्यथा
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::बफर_2::ELEMENT_ARRAY_बफर);

                gl::DrawElements(बहुलकम्, buf.size().area(), type, 0);

                ogl::बफर_2::unbind(ogl::बफर_2::ELEMENT_ARRAY_बफर);

                भङ्ग;
            }

        स्वभाव:
            {
                Mat mat = indices.getMat();

                ध्रुव पूर्णाङ्क गभीरता = mat.गभीरता();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( गभीरता <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                यदि (गभीरता < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                अन्यथा यदि (गभीरता < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                अन्यथा
                    type = gl::UNSIGNED_INT;

                ogl::बफर_2::unbind(ogl::बफर_2::ELEMENT_ARRAY_बफर);

                gl::DrawElements(बहुलकम्, mat.size().area(), type, mat.दत्त);
            }
        }
    }
#यदि_अन्तः
}

शून्य cv::ogl::clearColor(Scalar color)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(color);
    throw_no_ogl();
#अन्यथा
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_बफर_BIT);
#यदि_अन्तः // HAVE_OPENGL
}

शून्य cv::ogl::clear_गभीरता(भिन्न गभीरता)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(गभीरता);
    throw_no_ogl();
#अन्यथा
    gl::Clear_गभीरता(गभीरता);
    gl::Clear(gl::गभीरता_बफर_BIT);
#यदि_अन्तः // HAVE_OPENGL
}

शून्य cv::ogl::drawArrays(पूर्णाङ्क first, पूर्णाङ्क गणनम्_2, पूर्णाङ्क बहुलकम्)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(first); CV_अप्रयुक्तम्(गणनम्_2); CV_अप्रयुक्तम्(बहुलकम्);
    throw_no_ogl();
#अन्यथा
    gl::DrawArrays(बहुलकम्, first, गणनम्_2);
#यदि_अन्तः // HAVE_OPENGL
}

शून्य cv::ogl::drawElements(पूर्णाङ्क first, पूर्णाङ्क गणनम्_2, पूर्णाङ्क type, पूर्णाङ्क बहुलकम्)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(first); CV_अप्रयुक्तम्(गणनम्_2); CV_अप्रयुक्तम्(type); CV_अप्रयुक्तम्(बहुलकम्);
    throw_no_ogl();
#अन्यथा
    gl::DrawElements(बहुलकम्, गणनम्_2, type, reinterpret_cast<ध्रुव शून्य*>(static_cast<size_t>(first)));
#यदि_अन्तः
}

शून्य cv::ogl::drawElementsInstanced(पूर्णाङ्क first, पूर्णाङ्क गणनम्_2, पूर्णाङ्क type, पूर्णाङ्क बहुलकम्, पूर्णाङ्क instance_गणनम्)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(first); CV_अप्रयुक्तम्(गणनम्_2); CV_अप्रयुक्तम्(type); CV_अप्रयुक्तम्(बहुलकम्); CV_अप्रयुक्तम्(instance_गणनम्);
    throw_no_ogl();
#अन्यथा
    gl::DrawElementsInstanced(बहुलकम्, गणनम्_2, type, reinterpret_cast<ध्रुव शून्य*>(static_cast<size_t>(first)), instance_गणनम्);
    CV_CheckGlError();
#यदि_अन्तः
}

शून्य cv::ogl::blendFunc(पूर्णाङ्क sfactor, पूर्णाङ्क dfactor)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(sfactor); CV_अप्रयुक्तम्(dfactor);
    throw_no_ogl();
#अन्यथा
    gl::BlendFunc(sfactor, dfactor);
#यदि_अन्तः
}

शून्य cv::ogl::गभीरता_आवरणम्_2(तार्किक flag)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(flag);
    throw_no_ogl();
#अन्यथा
    gl::गभीरता_आवरणम्(flag ? gl::TRUE_ : gl::FALSE_);
#यदि_अन्तः
}

शून्य cv::ogl::enable(पूर्णाङ्क cap)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(cap);
    throw_no_ogl();
#अन्यथा
    gl::Enable(cap);
#यदि_अन्तः
}

शून्य cv::ogl::disable(पूर्णाङ्क cap)
{
#यदि_अपरिभाषितम् HAVE_OPENGL
    CV_अप्रयुक्तम्(cap);
    throw_no_ogl();
#अन्यथा
    gl::Disable(cap);
#यदि_अन्तः
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#यदि_परिभाषितम् HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  अन्यथा
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#अन्यथा // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#यदि_अन्तः // HAVE_OPENCL

#यदि defined(HAVE_OPENGL)
#  यदि defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#यदि_अन्तः // HAVE_OPENGL

नामाकाश cv { नामाकाश ogl {

#यदि defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
स्थिर शून्य checkOpenCLVersion()
{
    उपयुज्य नामाकाश cv::ocl;
    ध्रुव Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int स्थितिः = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, परिमाण(platform_id), &platform_id, &sz);
    CV_Assert(स्थितिः == CL_SUCCESS && sz == परिमाण(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    पूर्णाङ्क संस्करणम्_Major_2 = pi.संस्करणम्_Major_2();
    पूर्णाङ्क संस्करणम्_Minor_2 = pi.संस्करणम्_Minor_2();
    यदि (संस्करणम्_Major_2 < 1 || (संस्करणम्_Major_2 == 1 && संस्करणम्_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                संस्करणम्_Major_2, संस्करणम्_Minor_2, pi.name().c_str(), pi.संस्करणम्().c_str())
        );
}
#यदि_अन्तः

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
    cl_device_id* devices = शून्यसूचकः;
    cl_uint dev_प्रयुक्तम् = 0;
    cl_context context = शून्यसूचकः;

    cl_int स्थितिः = clGetPlatformIDs(0, रिक्त, &platformsCnt);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", स्थितिः));
    यदि (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    स्थितिः = clGetPlatformIDs(platformsCnt, &platforms[0], रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", स्थितिः));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    तार्किक sharingSupported = असत्य;

    प्रति (अचिह्नित पूर्णाङ्क i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        स्थितिः = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, रिक्त, &devCnt);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", स्थितिः));

        यत्न {
            devices = नूतन cl_device_id[devCnt];

            स्थितिः = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, रिक्त);
            यदि (स्थितिः != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", स्थितिः));

            प्रति (अचिह्नित पूर्णाङ्क j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_आकार;
                स्थितिः = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, रिक्त, &extension_आकार );
                यदि (स्थितिः != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", स्थितिः));

                यदि(extension_आकार > 0)
                {
                    std::string devString(extension_आकार, '\0');
                    स्थितिः = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_आकार);
                    यदि (स्थितिः != CL_SUCCESS)
                        अनुवर्त;
                    यदि (extension_आकार > 0 && devString.size() >= extension_आकार)
                        devString.resize(extension_आकार - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    यावत् (spacePos != devString.npos) {
                        यदि (वाक्यं_तुलय(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_प्रयुक्तम् = i;
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
            यदि(devices != शून्यसूचकः) {
                विलोपय[] devices;
            }
            अनुवर्त;
        }

        यदि (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#यदि defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_गणः_Obj cgl_Share_गणः = CGLGet_Share_गणः(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_गणः,
                    0
                };
                context = clCreateContext(props, 0,0, रिक्त, रिक्त, &ci_Err_अङ्कः);
#नोचेद्यदि defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_प्रयुक्तम्], रिक्त, रिक्त, &स्थितिः);
#नोचेद्यदि defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_प्रयुक्तम्], रिक्त, रिक्त, &स्थितिः);
#नोचेद्यदि defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_प्रयुक्तम्], रिक्त, रिक्त, &स्थितिः);
#यदि_अन्तः
        }

        यदि (स्थितिः != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", स्थितिः));
        अन्यथा
            भङ्ग;
    }


    cl_platform_id platform = platforms[dev_प्रयुक्तम्];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_प्रयुक्तम्]);
    clReleaseDevice(devices[dev_प्रयुक्तम्]);
    clReleaseContext(context);
    clExecCtx.bind();
    प्रत्यागम const_cast<Context&>(clExecCtx.getContext());
#यदि_अन्तः
}

} // namespace cv::ogl::ocl

शून्य convert_गन्तव्यम्_GLTexture2_D(निवेशः_Array src, Texture2D& texture)
{
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(texture);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    आकार src_आकार = src.size();
    CV_Assert(src_आकार.विस्तारः == (पूर्णाङ्क)texture.स्तम्भसङ्ख्या() && src_आकार.height == (पूर्णाङ्क)texture.पङ्क्तिकाः());

    उपयुज्य नामाकाश cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int स्थितिः = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &स्थितिः);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", स्थितिः));

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    स्थितिः = cl_पङ्क्तौ_निक्षिप_Acquire_GLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", स्थितिः));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.स्तम्भसङ्ख्या, (size_t)u.पङ्क्तिकाः, 1};
    स्थितिः = cl_पङ्क्तौ_निक्षिप_Copy_बफर_गन्तव्यम्_Image(q, cl_बफर, clImage, offset, dst_origin, region, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", स्थितिः));
    स्थितिः = cl_पङ्क्तौ_निक्षिप_Release_GLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", स्थितिः));

    स्थितिः = clFinish(q); // TODO Use events
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", स्थितिः));

    स्थितिः = clReleaseMemObject(clImage); // TODO RAII
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", स्थितिः));
#यदि_अन्तः
}

शून्य convertFromGLTexture2D(ध्रुव Texture2D& texture, OutputArray dst)
{
    CV_अप्रयुक्तम्(texture); CV_अप्रयुक्तम्(dst);
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

    उपयुज्य नामाकाश cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int स्थितिः = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &स्थितिः);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", स्थितिः));

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    स्थितिः = cl_पङ्क्तौ_निक्षिप_Acquire_GLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", स्थितिः));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.स्तम्भसङ्ख्या, (size_t)u.पङ्क्तिकाः, 1};
    स्थितिः = cl_पङ्क्तौ_निक्षिप_Copy_Image_गन्तव्यम्_बफर(q, clImage, cl_बफर, src_origin, region, offset, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", स्थितिः));
    स्थितिः = cl_पङ्क्तौ_निक्षिप_Release_GLObjects(q, 1, &clImage, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", स्थितिः));

    स्थितिः = clFinish(q); // TODO Use events
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", स्थितिः));

    स्थितिः = clReleaseMemObject(clImage); // TODO RAII
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", स्थितिः));
#यदि_अन्तः
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ध्रुव बफर_2& बफर, AccessFlag accessFlags)
{
    CV_अप्रयुक्तम्(बफर); CV_अप्रयुक्तम्(accessFlags);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    उपयुज्य नामाकाश cv::ocl;
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

    cl_int स्थितिः = 0;
    cl_mem cl_बफर = clCreateFromGLBuffer(context, clAccessFlags, बफर.bufId(), &स्थितिः);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", स्थितिः));

    gl::Finish();

    स्थितिः = cl_पङ्क्तौ_निक्षिप_Acquire_GLObjects(clQueue, 1, &cl_बफर, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", स्थितिः));

    size_t चरणः = बफर.स्तम्भसङ्ख्या() * बफर.elem_आकार();
    पूर्णाङ्क पङ्क्तिकाः = बफर.पङ्क्तिकाः();
    पूर्णाङ्क स्तम्भसङ्ख्या = बफर.स्तम्भसङ्ख्या();
    पूर्णाङ्क type = बफर.type();

    UMat u;
    convert_From_बफर(cl_बफर, चरणः, पङ्क्तिकाः, स्तम्भसङ्ख्या, type, u);
    प्रत्यागम u;
#यदि_अन्तः
}

शून्य unmapGLBuffer(UMat& u)
{
    CV_अप्रयुक्तम्(u);
#यदि !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    उपयुज्य नामाकाश cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int स्थितिः = cl_पङ्क्तौ_निक्षिप_Release_GLObjects(clQueue, 1, &cl_बफर, 0, रिक्त, रिक्त);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", स्थितिः));

    स्थितिः = clFinish(clQueue);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", स्थितिः));

    स्थितिः = clReleaseMemObject(cl_बफर);
    यदि (स्थितिः != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", स्थितिः));
#यदि_अन्तः
}

}} // namespace cv::ogl
