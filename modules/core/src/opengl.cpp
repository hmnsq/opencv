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

#समावेश "precomp.hpp"

#अगर defined (__APPLE__) || defined(MACOSX)
   #घोषणा GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#अन्यथा
   #घोषणा GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    अगर (defined(__arm__) || defined(__aarch64__)) \
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
#पूर्ण_अगर // HAVE_OPENGL

using namespace cv;
using namespace cv::cuda;

#अगर defined(_MSC_VER)
#आशय warning(disable : 4702)  // unreachable code
#पूर्ण_अगर

namespace
{
#अगर_अघोषित HAVE_OPENGL
अंतरभूत अटल CV_NORETURN व्योम throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#या_अगर defined _DEBUG
अंतरभूत अटल bool checkError(स्थिर अक्षर* file, स्थिर पूर्णांक पंक्ति, स्थिर अक्षर* func = 0)
{
    GLenum err = gl::GetError();
    अगर (err != gl::NO_ERROR_)
    {
        स्थिर अक्षर* संदेश;
        चयन (err)
        {
        हाल gl::INVALID_ENUM:
            संदेश = "An unacceptable value is specified for an enumerated argument";
            अवरोध;
        हाल gl::INVALID_मूल्य:
            संदेश = "A numeric argument is out of range";
            अवरोध;
        हाल gl::INVALID_OPERATION:
            संदेश = "The specified operation is not allowed in the current state";
            अवरोध;
        हाल gl::OUT_OF_MEMORY:
            संदेश = "There is not enough memory left to execute the command";
            अवरोध;
        शेष:
            संदेश = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, संदेश, file, पंक्ति);
    }
    वापस true;
}
#पूर्ण_अगर // HAVE_OPENGL
} // namespace

#घोषणा CV_CheckGlError() CV_DbgAssert( (checkError(__खाता__, __LINE__, CV_Func)) )

#अगर_घोषित HAVE_OPENGL
namespace
{
    स्थिर GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#पूर्ण_अगर

////////////////////////////////////////////////////////////////////////
// setGlDevice

व्योम cv::cuda::setGlDevice(पूर्णांक device)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(device);
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        CV_UNUSED(device);
        throw_no_cuda();
    #अन्यथा
        cudaSafeCall( cudaGLSetGLDevice(device) );
    #पूर्ण_अगर
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#अगर defined(HAVE_OPENGL) && defined(HAVE_CUDA)

namespace
{
    श्रेणी CudaResource
    {
    खुला:
        CudaResource();
        ~CudaResource();

        व्योम register_बफर(GLuint बफर);
        व्योम release();

        व्योम copyFrom(स्थिर व्योम* src, माप_प्रकार spitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream = 0);
        व्योम copyTo(व्योम* dst, माप_प्रकार dpitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream = 0);

        व्योम* map(cudaStream_t stream = 0);
        व्योम unmap(cudaStream_t stream = 0);

    निजी:
        cudaGraphicsResource_t resource_;
        GLuint बफर_3;

        श्रेणी GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), बफर_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    व्योम CudaResource::register_बफर(GLuint बफर)
    {
        CV_DbgAssert( बफर != 0 );

        अगर (बफर_3 == बफर)
            वापस;

        cudaGraphicsResource_t resource;
        cudaSafeCall( cuda_Graphics_GLRegister_बफर(&resource, बफर, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        बफर_3 = बफर;
    }

    व्योम CudaResource::release()
    {
        अगर (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        बफर_3 = 0;
    }

    श्रेणी CudaResource::GraphicsMapHolder
    {
    खुला:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream);
        ~GraphicsMapHolder();

        व्योम reset();

    निजी:
        cudaGraphicsResource_t* resource_;
        cudaStream_t stream_;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cudaStream_t stream) : resource_(resource), stream_(stream)
    {
        अगर (resource_)
            cudaSafeCall( cudaGraphicsMapResources(1, resource_, stream_) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        अगर (resource_)
            cudaGraphicsUnmapResources(1, resource_, stream_);
    }

    व्योम CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    व्योम CudaResource::copyFrom(स्थिर व्योम* src, माप_प्रकार spitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        व्योम* dst;
        माप_प्रकार size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&dst, &size, resource_) );

        CV_DbgAssert( width * height == size );

        अगर (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice) );
        अन्यथा
            cudaSafeCall( cudaMemcpy2DAsync(dst, width, src, spitch, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    व्योम CudaResource::copyTo(व्योम* dst, माप_प्रकार dpitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_UNUSED(h);

        व्योम* src;
        माप_प्रकार size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&src, &size, resource_) );

        CV_DbgAssert( width * height == size );

        अगर (stream == 0)
            cudaSafeCall( cudaMemcpy2D(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice) );
        अन्यथा
            cudaSafeCall( cudaMemcpy2DAsync(dst, dpitch, src, width, width, height, cudaMemcpyDeviceToDevice, stream) );
    }

    व्योम* CudaResource::map(cudaStream_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        व्योम* ptr;
        माप_प्रकार size;
        cudaSafeCall( cudaGraphicsResourceGetMappedPointer(&ptr, &size, resource_) );

        h.reset();

        वापस ptr;
    }

    व्योम CudaResource::unmap(cudaStream_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#पूर्ण_अगर

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#अगर_अघोषित HAVE_OPENGL

श्रेणी cv::ogl::बफर_2::Impl
{
};

#अन्यथा

श्रेणी cv::ogl::बफर_2::Impl
{
खुला:
    अटल स्थिर Ptr<Impl>& empty();

    Impl(GLuint bufId, bool autoRelease);
    Impl(GLsizeiptr size, स्थिर GLvoid* तथ्य, GLenum target, bool autoRelease);
    ~Impl();

    व्योम bind(GLenum target) स्थिर;

    व्योम copyFrom(GLuint srcBuf, GLsizeiptr size);

    व्योम copyFrom(GLsizeiptr size, स्थिर GLvoid* तथ्य);
    व्योम copyTo(GLsizeiptr size, GLvoid* तथ्य) स्थिर;

    व्योम resize(GLsizeiptr size, GLenum target);

    व्योम* mapHost(GLenum access);
    व्योम unmapHost();

#अगर_घोषित HAVE_CUDA
    व्योम copyFrom(स्थिर व्योम* src, माप_प्रकार spitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream = 0);
    व्योम copyTo(व्योम* dst, माप_प्रकार dpitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream = 0) स्थिर;

    व्योम* mapDevice(cudaStream_t stream = 0);
    व्योम unmapDevice(cudaStream_t stream = 0);
#पूर्ण_अगर

    व्योम setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint bufId() स्थिर { वापस bufId_; }

निजी:
    Impl();

    GLuint bufId_;
    bool autoRelease_;

#अगर_घोषित HAVE_CUDA
    mutable CudaResource cudaResource_;
#पूर्ण_अगर
};

स्थिर Ptr<cv::ogl::बफर_2::Impl>& cv::ogl::बफर_2::Impl::empty()
{
    अटल Ptr<Impl> p(नया Impl);
    वापस p;
}

cv::ogl::बफर_2::Impl::Impl() : bufId_(0), autoRelease_(false)
{
}

cv::ogl::बफर_2::Impl::Impl(GLuint abufId, bool autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_बफर(abufId) == gl::TRUE_ );
}

cv::ogl::बफर_2::Impl::Impl(GLsizeiptr size, स्थिर GLvoid* तथ्य, GLenum target, bool autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_बफर(target, bufId_);
    CV_CheckGlError();

    gl::बफर_तथ्य(target, size, तथ्य, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_बफर(target, 0);
    CV_CheckGlError();
}

cv::ogl::बफर_2::Impl::~Impl()
{
    अगर (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

व्योम cv::ogl::बफर_2::Impl::bind(GLenum target) स्थिर
{
    gl::Bind_बफर(target, bufId_);
    CV_CheckGlError();
}

व्योम cv::ogl::बफर_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_बफर(gl::COPY_WRITE_बफर, bufId_);
    CV_CheckGlError();

    gl::Bind_बफर(gl::COPY_READ_बफर, srcBuf);
    CV_CheckGlError();

    gl::Copy_बफर_Sub_तथ्य(gl::COPY_READ_बफर, gl::COPY_WRITE_बफर, 0, 0, size);
    CV_CheckGlError();
}

व्योम cv::ogl::बफर_2::Impl::copyFrom(GLsizeiptr size, स्थिर GLvoid* तथ्य)
{
    gl::Bind_बफर(gl::COPY_WRITE_बफर, bufId_);
    CV_CheckGlError();

    gl::बफर_Sub_तथ्य(gl::COPY_WRITE_बफर, 0, size, तथ्य);
    CV_CheckGlError();
}

व्योम cv::ogl::बफर_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_बफर(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::बफर_तथ्य(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_बफर(target, 0);
    CV_CheckGlError();
}

व्योम cv::ogl::बफर_2::Impl::copyTo(GLsizeiptr size, GLvoid* तथ्य) स्थिर
{
    gl::Bind_बफर(gl::COPY_READ_बफर, bufId_);
    CV_CheckGlError();

    gl::Get_बफर_Sub_तथ्य(gl::COPY_READ_बफर, 0, size, तथ्य);
    CV_CheckGlError();
}

व्योम* cv::ogl::बफर_2::Impl::mapHost(GLenum access)
{
    gl::Bind_बफर(gl::COPY_READ_बफर, bufId_);
    CV_CheckGlError();

    GLvoid* तथ्य = gl::Map_बफर(gl::COPY_READ_बफर, access);
    CV_CheckGlError();

    वापस तथ्य;
}

व्योम cv::ogl::बफर_2::Impl::unmapHost()
{
    gl::Unmap_बफर(gl::COPY_READ_बफर);
}

#अगर_घोषित HAVE_CUDA

व्योम cv::ogl::बफर_2::Impl::copyFrom(स्थिर व्योम* src, माप_प्रकार spitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream)
{
    cudaResource_.register_बफर(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

व्योम cv::ogl::बफर_2::Impl::copyTo(व्योम* dst, माप_प्रकार dpitch, माप_प्रकार width, माप_प्रकार height, cudaStream_t stream) स्थिर
{
    cudaResource_.register_बफर(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

व्योम* cv::ogl::बफर_2::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.register_बफर(bufId_);
    वापस cudaResource_.map(stream);
}

व्योम cv::ogl::बफर_2::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#पूर्ण_अगर // HAVE_CUDA

#पूर्ण_अगर // HAVE_OPENGL

cv::ogl::बफर_2::बफर_2() : rows_(0), cols_(0), type_(0)
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#पूर्ण_अगर
}

cv::ogl::बफर_2::बफर_2(पूर्णांक arows, पूर्णांक acols, पूर्णांक atype, अचिन्हित पूर्णांक abufId, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#पूर्ण_अगर
}

cv::ogl::बफर_2::बफर_2(आकार asize, पूर्णांक atype, अचिन्हित पूर्णांक abufId, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#पूर्ण_अगर
}

cv::ogl::बफर_2::बफर_2(निवेश_Array arr, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    स्थिर पूर्णांक kind = arr.kind();

    चयन (kind)
    {
    हाल निवेश_Array_2::OPENGL_बफर:
    हाल निवेश_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        अवरोध;

    शेष:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            स्थिर GLsizeiptr asize = mat.rows * mat.cols * mat.elem_आकार();
            impl_.reset(नया Impl(asize, mat.तथ्य, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            अवरोध;
        }
    }
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::create(पूर्णांक arows, पूर्णांक acols, पूर्णांक atype, Target target, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    अगर (rows_ != arows || cols_ != acols || type_ != atype)
    {
        स्थिर GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_आकार(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        अगर (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->setAutoRelease(autoRelease);
        }
        अन्यथा
            impl_.reset(नया Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::release()
{
#अगर_घोषित HAVE_OPENGL
    अगर (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::copyFrom(निवेश_Array arr, Target target, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    स्थिर पूर्णांक kind = arr.kind();

    स्थिर आकार asize = arr.size();
    स्थिर पूर्णांक atype = arr.type();
    create(asize, atype, target, autoRelease);

    चयन (kind)
    {
    हाल निवेश_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2 buf = arr.get_OGl_बफर();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_आकार(atype));
            अवरोध;
        }

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        {
            #अगर_अघोषित HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.तथ्य, dmat.step, dmat.cols * dmat.elem_आकार(), dmat.rows);
            #पूर्ण_अगर

            अवरोध;
        }

    शेष:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_आकार(atype), mat.तथ्य);
        }
    }
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::copyFrom(निवेश_Array arr, cuda::Stream& stream, Target target, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        CV_UNUSED(target);
        CV_UNUSED(autoRelease);
        throw_no_cuda();
    #अन्यथा
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.तथ्य, dmat.step, dmat.cols * dmat.elem_आकार(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #पूर्ण_अगर
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::copyTo(OutputArray arr) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    throw_no_ogl();
#अन्यथा
    स्थिर पूर्णांक kind = arr.kind();

    चयन (kind)
    {
    हाल निवेश_Array_2::OPENGL_बफर:
        {
            arr.get_OGl_बफर_Ref().copyFrom(*यह);
            अवरोध;
        }

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        {
            #अगर_अघोषित HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(rows_, cols_, type_);
                impl_->copyTo(dmat.तथ्य, dmat.step, dmat.cols * dmat.elem_आकार(), dmat.rows);
            #पूर्ण_अगर

            अवरोध;
        }

    शेष:
        {
            arr.create(rows_, cols_, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyTo(mat.rows * mat.cols * mat.elem_आकार(), mat.तथ्य);
        }
    }
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::copyTo(OutputArray arr, cuda::Stream& stream) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(stream);
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        CV_UNUSED(arr);
        CV_UNUSED(stream);
        throw_no_cuda();
    #अन्यथा
        arr.create(rows_, cols_, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copyTo(dmat.तथ्य, dmat.step, dmat.cols * dmat.elem_आकार(), dmat.rows, cuda::StreamAccessor::getStream(stream));
    #पूर्ण_अगर
#पूर्ण_अगर
}

cv::ogl::बफर_2 cv::ogl::बफर_2::clone(Target target, bool autoRelease) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    ogl::बफर_2 buf;
    buf.copyFrom(*यह, target, autoRelease);
    वापस buf;
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::bind(Target target) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#अन्यथा
    impl_->bind(target);
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::unbind(Target target)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#अन्यथा
    gl::Bind_बफर(target, 0);
    CV_CheckGlError();
#पूर्ण_अगर
}

Mat cv::ogl::बफर_2::mapHost(Access access)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#अन्यथा
    वापस Mat(rows_, cols_, type_, impl_->mapHost(access));
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::unmapHost()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->unmapHost();
#पूर्ण_अगर
}

GpuMat cv::ogl::बफर_2::mapDevice()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        throw_no_cuda();
    #अन्यथा
        वापस GpuMat(rows_, cols_, type_, impl_->mapDevice());
    #पूर्ण_अगर
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::unmapDevice()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        throw_no_cuda();
    #अन्यथा
        impl_->unmapDevice();
    #पूर्ण_अगर
#पूर्ण_अगर
}

cuda::GpuMat cv::ogl::बफर_2::mapDevice(cuda::Stream& stream)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #अन्यथा
        वापस GpuMat(rows_, cols_, type_, impl_->mapDevice(cuda::StreamAccessor::getStream(stream)));
    #पूर्ण_अगर
#पूर्ण_अगर
}

व्योम cv::ogl::बफर_2::unmapDevice(cuda::Stream& stream)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(stream);
    throw_no_ogl();
#अन्यथा
    #अगर_अघोषित HAVE_CUDA
        CV_UNUSED(stream);
        throw_no_cuda();
    #अन्यथा
        impl_->unmapDevice(cuda::StreamAccessor::getStream(stream));
    #पूर्ण_अगर
#पूर्ण_अगर
}

अचिन्हित पूर्णांक cv::ogl::बफर_2::bufId() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->bufId();
#पूर्ण_अगर
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#अगर_अघोषित HAVE_OPENGL

श्रेणी cv::ogl::Texture2D::Impl
{
};

#अन्यथा

श्रेणी cv::ogl::Texture2D::Impl
{
खुला:
    अटल स्थिर Ptr<Impl> empty();

    Impl(GLuint texId, bool autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, स्थिर GLvoid* pixels, bool autoRelease);
    ~Impl();

    व्योम copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, स्थिर GLvoid *pixels);
    व्योम copyTo(GLenum format, GLenum type, GLvoid* pixels) स्थिर;

    व्योम bind() स्थिर;

    व्योम setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint texId() स्थिर { वापस texId_; }

निजी:
    Impl();

    GLuint texId_;
    bool autoRelease_;
};

स्थिर Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    अटल Ptr<Impl> p(नया Impl);
    वापस p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(false)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, bool autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, स्थिर GLvoid* pixels, bool autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    अगर (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

व्योम cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, स्थिर GLvoid *pixels)
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

व्योम cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) स्थिर
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

व्योम cv::ogl::Texture2D::Impl::bind() स्थिर
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#पूर्ण_अगर // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#पूर्ण_अगर
}

cv::ogl::Texture2D::Texture2D(पूर्णांक arows, पूर्णांक acols, Format aformat, अचिन्हित पूर्णांक atexId, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#पूर्ण_अगर
}

cv::ogl::Texture2D::Texture2D(आकार asize, Format aformat, अचिन्हित पूर्णांक atexId, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#पूर्ण_अगर
}

cv::ogl::Texture2D::Texture2D(निवेश_Array arr, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    स्थिर पूर्णांक kind = arr.kind();

    स्थिर आकार asize = arr.size();
    स्थिर पूर्णांक atype = arr.type();

    स्थिर पूर्णांक depth = CV_MAT_DEPTH(atype);
    स्थिर पूर्णांक cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    स्थिर Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    स्थिर GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    चयन (kind)
    {
    हाल निवेश_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2 buf = arr.get_OGl_बफर();
            buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_.reset(नया Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            अवरोध;
        }

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        {
            #अगर_अघोषित HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                ogl::बफर_2 buf(dmat, ogl::बफर_2::PIXEL_UNPACK_बफर);
                buf.setAutoRelease(true);
                buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
                impl_.reset(नया Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
                ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            #पूर्ण_अगर

            अवरोध;
        }

    शेष:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_.reset(नया Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.तथ्य, autoRelease));
            अवरोध;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#पूर्ण_अगर
}

व्योम cv::ogl::Texture2D::create(पूर्णांक arows, पूर्णांक acols, Format aformat, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    अगर (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
        impl_.reset(नया Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#पूर्ण_अगर
}

व्योम cv::ogl::Texture2D::release()
{
#अगर_घोषित HAVE_OPENGL
    अगर (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#पूर्ण_अगर
}

व्योम cv::ogl::Texture2D::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#पूर्ण_अगर
}

व्योम cv::ogl::Texture2D::copyFrom(निवेश_Array arr, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    स्थिर पूर्णांक kind = arr.kind();

    स्थिर आकार asize = arr.size();
    स्थिर पूर्णांक atype = arr.type();

    स्थिर पूर्णांक depth = CV_MAT_DEPTH(atype);
    स्थिर पूर्णांक cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    स्थिर Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    स्थिर GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    चयन(kind)
    {
    हाल निवेश_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2 buf = arr.get_OGl_बफर();
            buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            अवरोध;
        }

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        {
            #अगर_अघोषित HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                GpuMat dmat = arr.getGpuMat();
                ogl::बफर_2 buf(dmat, ogl::बफर_2::PIXEL_UNPACK_बफर);
                buf.setAutoRelease(true);
                buf.bind(ogl::बफर_2::PIXEL_UNPACK_बफर);
                impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
                ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            #पूर्ण_अगर

            अवरोध;
        }

    शेष:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_UNPACK_बफर);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.तथ्य);
        }
    }
#पूर्ण_अगर
}

व्योम cv::ogl::Texture2D::copyTo(OutputArray arr, पूर्णांक ddepth, bool autoRelease) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    स्थिर पूर्णांक kind = arr.kind();

    स्थिर पूर्णांक cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    स्थिर GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    चयन(kind)
    {
    हाल निवेश_Array_2::OPENGL_बफर:
        {
            ogl::बफर_2& buf = arr.get_OGl_बफर_Ref();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::बफर_2::PIXEL_PACK_बफर, autoRelease);
            buf.bind(ogl::बफर_2::PIXEL_PACK_बफर);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_PACK_बफर);
            अवरोध;
        }

    हाल निवेश_Array_2::CUDA_GPU_MAT:
        {
            #अगर_अघोषित HAVE_CUDA
                throw_no_cuda();
            #अन्यथा
                ogl::बफर_2 buf(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::बफर_2::PIXEL_PACK_बफर);
                buf.setAutoRelease(true);
                buf.bind(ogl::बफर_2::PIXEL_PACK_बफर);
                impl_->copyTo(dstFormat, gl_types[ddepth], 0);
                ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_PACK_बफर);
                buf.copyTo(arr);
            #पूर्ण_अगर

            अवरोध;
        }

    शेष:
        {
            arr.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::बफर_2::unbind(ogl::बफर_2::PIXEL_PACK_बफर);
            impl_->copyTo(dstFormat, gl_types[ddepth], mat.तथ्य);
        }
    }
#पूर्ण_अगर
}

व्योम cv::ogl::Texture2D::bind() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#पूर्ण_अगर
}

अचिन्हित पूर्णांक cv::ogl::Texture2D::texId() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->texId();
#पूर्ण_अगर
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#अगर_अघोषित HAVE_OPENGL

श्रेणी cv::ogl::Texture_बफर::Impl
{
};

#अन्यथा

श्रेणी cv::ogl::Texture_बफर::Impl
{
खुला:
    अटल स्थिर Ptr<Impl> empty();

    Impl();
    Impl(GLenum internalFormat, GLuint bufId, bool autoRelease);
    ~Impl();

    व्योम bind(GLint unit) स्थिर;

    व्योम setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint texId() स्थिर { वापस texId_; }

निजी:
    GLuint texId_;
    bool autoRelease_;
};

स्थिर Ptr<cv::ogl::Texture_बफर::Impl> cv::ogl::Texture_बफर::Impl::empty()
{
    अटल Ptr<Impl> p = makePtr<Impl>();
    वापस p;
}

cv::ogl::Texture_बफर::Impl::Impl() : texId_(0), autoRelease_(false)
{
}

cv::ogl::Texture_बफर::Impl::Impl(GLenum internalFormat, GLuint bufId, bool autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_बफर, texId_);
    CV_CheckGlError();

    gl::Tex_बफर(gl::TEXTURE_बफर, internalFormat, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_बफर, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_बफर::Impl::~Impl()
{
    अगर (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

व्योम cv::ogl::Texture_बफर::Impl::bind(GLint unit) स्थिर
{
    gl::ActiveTexture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_बफर, texId_);
    CV_CheckGlError();
}

#पूर्ण_अगर // HAVE_OPENGL

cv::ogl::Texture_बफर::Texture_बफर() : format_(NONE)
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#पूर्ण_अगर
}

cv::ogl::Texture_बफर::Texture_बफर(स्थिर बफर_2& buf, Format aformat, bool autoRelease) : format_(NONE)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(buf);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#पूर्ण_अगर
}

व्योम cv::ogl::Texture_बफर::create(स्थिर बफर_2& buf, Format aformat, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
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
#पूर्ण_अगर
}

व्योम cv::ogl::Texture_बफर::release()
{
#अगर_घोषित HAVE_OPENGL
    अगर (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#पूर्ण_अगर
    buf_ = बफर_2();
    format_ = NONE;
}

व्योम cv::ogl::Texture_बफर::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#पूर्ण_अगर
}

व्योम cv::ogl::Texture_बफर::bind(पूर्णांक unit) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(unit);
    throw_no_ogl();
#अन्यथा
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#पूर्ण_अगर
}

cv::ogl::Texture_बफर::Format cv::ogl::Texture_बफर::format() स्थिर
{
    वापस format_;
}

bool cv::ogl::Texture_बफर::empty() स्थिर
{
    वापस format_ == NONE;
}

अचिन्हित पूर्णांक cv::ogl::Texture_बफर::texId() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->texId();
#पूर्ण_अगर
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

व्योम cv::ogl::Arrays::setVertexArray(निवेश_Array vertex)
{
    स्थिर पूर्णांक cn = vertex.channels();
    स्थिर पूर्णांक depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    अगर (vertex.kind() == निवेश_Array_2::OPENGL_बफर)
        vertex_ = vertex.get_OGl_बफर();
    अन्यथा
        vertex_.copyFrom(vertex);

    आकार_3 = vertex_.size().area();
}

व्योम cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    आकार_3 = 0;
}

व्योम cv::ogl::Arrays::setColorArray(निवेश_Array color)
{
    स्थिर पूर्णांक cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    अगर (color.kind() == निवेश_Array_2::OPENGL_बफर)
        color_ = color.get_OGl_बफर();
    अन्यथा
        color_.copyFrom(color);
}

व्योम cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

व्योम cv::ogl::Arrays::setNormalArray(निवेश_Array normal)
{
    स्थिर पूर्णांक cn = normal.channels();
    स्थिर पूर्णांक depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    अगर (normal.kind() == निवेश_Array_2::OPENGL_बफर)
        normal_ = normal.get_OGl_बफर();
    अन्यथा
        normal_.copyFrom(normal);
}

व्योम cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

व्योम cv::ogl::Arrays::setTexCoordArray(निवेश_Array texCoord)
{
    स्थिर पूर्णांक cn = texCoord.channels();
    स्थिर पूर्णांक depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    अगर (texCoord.kind() == निवेश_Array_2::OPENGL_बफर)
        texCoord_ = texCoord.get_OGl_बफर();
    अन्यथा
        texCoord_.copyFrom(texCoord);
}

व्योम cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

व्योम cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

व्योम cv::ogl::Arrays::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#पूर्ण_अगर
}

व्योम cv::ogl::Arrays::bind() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == आकार_3 );
    CV_Assert( normal_.empty() || normal_.size().area() == आकार_3 );
    CV_Assert( color_.empty() || color_.size().area() == आकार_3 );

    अगर (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::बफर_2::ARRAY_बफर);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        CV_CheckGlError();
    }

    अगर (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        CV_CheckGlError();

        normal_.bind(ogl::बफर_2::ARRAY_बफर);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        CV_CheckGlError();
    }

    अगर (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::बफर_2::ARRAY_बफर);

        स्थिर पूर्णांक cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        CV_CheckGlError();
    }

    अगर (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();
    }
    अन्यथा
    {
        gl::EnableClientState(gl::VERTEX_ARRAY);
        CV_CheckGlError();

        vertex_.bind(ogl::बफर_2::ARRAY_बफर);

        gl::VertexPointer(vertex_.channels(), gl_types[vertex_.depth()], 0, 0);
        CV_CheckGlError();
    }

    ogl::बफर_2::unbind(ogl::बफर_2::ARRAY_बफर);
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#अगर_अघोषित HAVE_OPENGL

श्रेणी cv::ogl::VertexArray::Impl
{
};

#अन्यथा

श्रेणी cv::ogl::VertexArray::Impl
{
खुला:
    अटल स्थिर Ptr<Impl>& empty();

    Impl(GLuint vaId, bool autoRelease);
    Impl(std::initializer_list<Attribute> attributes, bool autoRelease);
    ~Impl();

    व्योम bind();

    व्योम setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint vaId() स्थिर { वापस vaId_; }

निजी:
    Impl();

    GLuint vaId_;
    bool autoRelease_;
};

स्थिर Ptr<cv::ogl::VertexArray::Impl>& cv::ogl::VertexArray::Impl::empty()
{
    अटल Ptr<Impl> p(नया Impl);
    वापस p;
}

cv::ogl::VertexArray::Impl::Impl() : vaId_(0), autoRelease_(false)
{
}

cv::ogl::VertexArray::Impl::Impl(GLuint vaId, bool autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsVertexArray(vaId) == gl::TRUE_ );
}

cv::ogl::VertexArray::Impl::Impl(std::initializer_list<Attribute> attributes, bool autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::GenVertexArrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();

    क्रम (स्वतः& attribute : attributes)
    {
        attribute.बफर_3.bind(बफर_2::ARRAY_बफर);
        अगर (attribute.integer_)
        {
            gl::VertexAttribIPointer(
                attribute.shader_loc_,
                attribute.आकार_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<स्थिर व्योम*>(attribute.offset_)
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
                reinterpret_cast<स्थिर व्योम*>(attribute.offset_)
            );
            CV_CheckGlError();
        }

        gl::EnableVertexAttribArray(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::VertexArray::Impl::~Impl()
{
    अगर (autoRelease_ && vaId_)
        gl::DeleteVertexArrays(1, &vaId_);
}

व्योम cv::ogl::VertexArray::Impl::bind()
{
    gl::BindVertexArray(vaId_);
    CV_CheckGlError();
}

#पूर्ण_अगर // HAVE_OPENGL

cv::ogl::VertexArray::VertexArray()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#पूर्ण_अगर
}

cv::ogl::VertexArray::VertexArray(std::initializer_list<Attribute> attributes, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(attributes, autoRelease));
#पूर्ण_अगर
}

व्योम cv::ogl::VertexArray::create(std::initializer_list<Attribute> attributes, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(attributes); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(attributes, autoRelease));
#पूर्ण_अगर
}

व्योम cv::ogl::VertexArray::release()
{
#अगर_घोषित HAVE_OPENGL
    अगर (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#पूर्ण_अगर
}

व्योम cv::ogl::VertexArray::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#पूर्ण_अगर
}

व्योम cv::ogl::VertexArray::bind() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#पूर्ण_अगर
}

व्योम cv::ogl::VertexArray::unbind()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    gl::BindVertexArray(0);
#पूर्ण_अगर
}

अचिन्हित पूर्णांक cv::ogl::VertexArray::vaId() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->vaId();
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#अगर_अघोषित HAVE_OPENGL

श्रेणी cv::ogl::Shader::Impl
{
};

#अन्यथा

श्रेणी cv::ogl::Shader::Impl
{
खुला:
    अटल स्थिर Ptr<Impl>& empty();

    Impl(GLuint shaderId, bool autoRelease);
    Impl(स्थिर अक्षर* src, GLenum type, bool autoRelease);
    ~Impl();

    व्योम setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint shaderId() स्थिर { वापस shaderId_; }

निजी:
    Impl();

    GLuint shaderId_;
    bool autoRelease_;
};

स्थिर Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    अटल Ptr<Impl> p(नया Impl);
    वापस p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(false)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, bool autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(स्थिर अक्षर* src, GLenum type, bool autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint status;
    gl::ShaderSource(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_STATUS, &status);
    अगर (!status) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_लंबाई, &status);
        info_log.resize(status);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, nullptr, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    अगर (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#पूर्ण_अगर // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::VERTEX)
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#पूर्ण_अगर
}

cv::ogl::Shader::Shader(स्थिर अक्षर* src, ShaderType type, bool autoRelease) : type_(type)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(src, type, autoRelease));
#पूर्ण_अगर
}

व्योम cv::ogl::Shader::create(स्थिर अक्षर* src, ShaderType type, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(src); CV_UNUSED(type); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(src, type, autoRelease));
    type_ = type;
#पूर्ण_अगर
}

व्योम cv::ogl::Shader::release()
{
#अगर_घोषित HAVE_OPENGL
    अगर (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#पूर्ण_अगर
}

व्योम cv::ogl::Shader::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#पूर्ण_अगर
}

अचिन्हित पूर्णांक cv::ogl::Shader::shaderId() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->shaderId();
#पूर्ण_अगर
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() स्थिर
{
    वापस type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#अगर_अघोषित HAVE_OPENGL

श्रेणी cv::ogl::Program::Impl
{
};

#अन्यथा

श्रेणी cv::ogl::Program::Impl
{
खुला:
    अटल स्थिर Ptr<Impl>& empty();

    Impl(GLuint programId, bool autoRelease);
    Impl(स्थिर Shader& vert, स्थिर Shader& frag, bool autoRelease);
    ~Impl();

    व्योम bind();

    पूर्णांक getAttributeLocation(स्थिर अक्षर* name) स्थिर;
    पूर्णांक getUniformLocation(स्थिर अक्षर* name) स्थिर;

    व्योम setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint programId() स्थिर { वापस programId_; }

निजी:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint programId_;
    bool autoRelease_;
};

स्थिर Ptr<cv::ogl::Program::Impl>& cv::ogl::Program::Impl::empty()
{
    अटल Ptr<Impl> p(नया Impl);
    वापस p;
}

cv::ogl::Program::Impl::Impl() : programId_(0), autoRelease_(false)
{
}

cv::ogl::Program::Impl::Impl(GLuint programId, bool autoRelease) : programId_(programId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsProgram(programId) == gl::TRUE_ );
}

cv::ogl::Program::Impl::Impl(स्थिर Shader& vert, स्थिर Shader& frag, bool autoRelease) : programId_(0), autoRelease_(autoRelease)
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
    अगर (!status) {
        String info_log;
        gl::GetProgramiv(programId_, gl::INFO_LOG_लंबाई, &status);
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
    अगर (autoRelease_ && programId_)
        gl::DeleteProgram(programId_);
}

व्योम cv::ogl::Program::Impl::bind()
{
    gl::UseProgram(programId_);
    CV_CheckGlError();
}

पूर्णांक cv::ogl::Program::Impl::getAttributeLocation(स्थिर अक्षर* name) स्थिर
{
    // -1 if absent (e.g. optimized out); caller decides.
    पूर्णांक location = gl::GetAttribLocation(programId_, name);
    CV_CheckGlError();
    वापस location;
}

पूर्णांक cv::ogl::Program::Impl::getUniformLocation(स्थिर अक्षर* name) स्थिर
{
    // -1 if absent (e.g. optimized out); caller decides.
    पूर्णांक location = gl::GetUniformLocation(programId_, name);
    CV_CheckGlError();
    वापस location;
}

#पूर्ण_अगर // HAVE_OPENGL

cv::ogl::Program::Program()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_ = Impl::empty();
#पूर्ण_अगर
}

cv::ogl::Program::Program(स्थिर Shader& vert, स्थिर Shader& frag, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(vert, frag, autoRelease));
#पूर्ण_अगर
}

व्योम cv::ogl::Program::create(स्थिर Shader& vert, स्थिर Shader& frag, bool autoRelease)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(vert); CV_UNUSED(frag); CV_UNUSED(autoRelease);
    throw_no_ogl();
#अन्यथा
    impl_.reset(नया Impl(vert, frag, autoRelease));
#पूर्ण_अगर
}

व्योम cv::ogl::Program::release()
{
#अगर_घोषित HAVE_OPENGL
    अगर (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setAutoRelease(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    impl_->setAutoRelease(flag);
#पूर्ण_अगर
}

व्योम cv::ogl::Program::bind() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    impl_->bind();
#पूर्ण_अगर
}

व्योम cv::ogl::Program::unbind()
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    gl::UseProgram(0);
#पूर्ण_अगर
}

पूर्णांक cv::ogl::Program::getAttributeLocation(स्थिर अक्षर* name) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#अन्यथा
    वापस impl_->getAttributeLocation(name);
#पूर्ण_अगर
}

पूर्णांक cv::ogl::Program::getUniformLocation(स्थिर अक्षर* name) स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(name);
    throw_no_ogl();
#अन्यथा
    वापस impl_->getUniformLocation(name);
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setUniformVec3(पूर्णांक loc, Vec3f vec)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setUniformVec2(पूर्णांक loc, Vec2f vec)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform2f(loc, vec(0), vec(1));
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setUniformVec4(पूर्णांक loc, Vec4f vec)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(vec);
    throw_no_ogl();
#अन्यथा
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setUniformMat4x4(पूर्णांक loc, Matx44f mat)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(mat);
    throw_no_ogl();
#अन्यथा
    gl::UniformMatrix4fv(loc, 1, true, mat.val);
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setUniform1f(पूर्णांक loc, भग्न value)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#अन्यथा
    gl::Uniform1f(loc, value);
#पूर्ण_अगर
}

व्योम cv::ogl::Program::setUniform1i(पूर्णांक loc, पूर्णांक value)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(loc); CV_UNUSED(value);
    throw_no_ogl();
#अन्यथा
    gl::Uniform1i(loc, value);
#पूर्ण_अगर
}

अचिन्हित पूर्णांक cv::ogl::Program::programId() स्थिर
{
#अगर_अघोषित HAVE_OPENGL
    throw_no_ogl();
#अन्यथा
    वापस impl_->programId();
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// Rendering

व्योम cv::ogl::render(स्थिर ogl::Texture2D& tex, Rect_<द्विगुन> wndRect, Rect_<द्विगुन> texRect)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(tex);
    CV_UNUSED(wndRect);
    CV_UNUSED(texRect);
    throw_no_ogl();
#अन्यथा
    अगर (!tex.empty())
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

        स्थिर द्विगुन vertex[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, (wndRect.y + wndRect.height), 0.0,
            wndRect.x + wndRect.width, wndRect.y, 0.0
        };
        स्थिर द्विगुन texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.height,
            texRect.x + texRect.width, texRect.y + texRect.height,
            texRect.x + texRect.width, texRect.y
        };

        ogl::बफर_2::unbind(ogl::बफर_2::ARRAY_बफर);

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
#पूर्ण_अगर
}

व्योम cv::ogl::render(स्थिर ogl::Arrays& arr, पूर्णांक mode, Scalar color)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#अन्यथा
    अगर (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(mode, 0, arr.size());
    }
#पूर्ण_अगर
}

व्योम cv::ogl::render(स्थिर ogl::Arrays& arr, निवेश_Array indices, पूर्णांक mode, Scalar color)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(indices);
    CV_UNUSED(mode);
    CV_UNUSED(color);
    throw_no_ogl();
#अन्यथा
    अगर (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        स्थिर पूर्णांक kind = indices.kind();

        चयन (kind)
        {
        हाल निवेश_Array_2::OPENGL_बफर :
            {
                ogl::बफर_2 buf = indices.get_OGl_बफर();

                स्थिर पूर्णांक depth = buf.depth();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( depth <= CV_32S );

                GLenum type;
                अगर (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                अन्यथा अगर (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                अन्यथा
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::बफर_2::ELEMENT_ARRAY_बफर);

                gl::DrawElements(mode, buf.size().area(), type, 0);

                ogl::बफर_2::unbind(ogl::बफर_2::ELEMENT_ARRAY_बफर);

                अवरोध;
            }

        शेष:
            {
                Mat mat = indices.getMat();

                स्थिर पूर्णांक depth = mat.depth();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( depth <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                अगर (depth < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                अन्यथा अगर (depth < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                अन्यथा
                    type = gl::UNSIGNED_INT;

                ogl::बफर_2::unbind(ogl::बफर_2::ELEMENT_ARRAY_बफर);

                gl::DrawElements(mode, mat.size().area(), type, mat.तथ्य);
            }
        }
    }
#पूर्ण_अगर
}

व्योम cv::ogl::clearColor(Scalar color)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(color);
    throw_no_ogl();
#अन्यथा
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_बफर_BIT);
#पूर्ण_अगर // HAVE_OPENGL
}

व्योम cv::ogl::clearDepth(भग्न depth)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(depth);
    throw_no_ogl();
#अन्यथा
    gl::ClearDepth(depth);
    gl::Clear(gl::DEPTH_बफर_BIT);
#पूर्ण_अगर // HAVE_OPENGL
}

व्योम cv::ogl::drawArrays(पूर्णांक first, पूर्णांक गिनती_2, पूर्णांक mode)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(गिनती_2); CV_UNUSED(mode);
    throw_no_ogl();
#अन्यथा
    gl::DrawArrays(mode, first, गिनती_2);
#पूर्ण_अगर // HAVE_OPENGL
}

व्योम cv::ogl::drawElements(पूर्णांक first, पूर्णांक गिनती_2, पूर्णांक type, पूर्णांक mode)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(गिनती_2); CV_UNUSED(type); CV_UNUSED(mode);
    throw_no_ogl();
#अन्यथा
    gl::DrawElements(mode, गिनती_2, type, reinterpret_cast<स्थिर व्योम*>(static_cast<माप_प्रकार>(first)));
#पूर्ण_अगर
}

व्योम cv::ogl::drawElementsInstanced(पूर्णांक first, पूर्णांक गिनती_2, पूर्णांक type, पूर्णांक mode, पूर्णांक instance_गिनती)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(first); CV_UNUSED(गिनती_2); CV_UNUSED(type); CV_UNUSED(mode); CV_UNUSED(instance_गिनती);
    throw_no_ogl();
#अन्यथा
    gl::DrawElementsInstanced(mode, गिनती_2, type, reinterpret_cast<स्थिर व्योम*>(static_cast<माप_प्रकार>(first)), instance_गिनती);
    CV_CheckGlError();
#पूर्ण_अगर
}

व्योम cv::ogl::blendFunc(पूर्णांक sfactor, पूर्णांक dfactor)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(sfactor); CV_UNUSED(dfactor);
    throw_no_ogl();
#अन्यथा
    gl::BlendFunc(sfactor, dfactor);
#पूर्ण_अगर
}

व्योम cv::ogl::depthMask(bool flag)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#अन्यथा
    gl::DepthMask(flag ? gl::TRUE_ : gl::FALSE_);
#पूर्ण_अगर
}

व्योम cv::ogl::enable(पूर्णांक cap)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#अन्यथा
    gl::Enable(cap);
#पूर्ण_अगर
}

व्योम cv::ogl::disable(पूर्णांक cap)
{
#अगर_अघोषित HAVE_OPENGL
    CV_UNUSED(cap);
    throw_no_ogl();
#अन्यथा
    gl::Disable(cap);
#पूर्ण_अगर
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#अगर_घोषित HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  अन्यथा
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#अन्यथा // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#पूर्ण_अगर // HAVE_OPENCL

#अगर defined(HAVE_OPENGL)
#  अगर defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#पूर्ण_अगर // HAVE_OPENGL

namespace cv { namespace ogl {

#अगर defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
अटल व्योम checkOpenCLVersion()
{
    using namespace cv::ocl;
    स्थिर Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    माप_प्रकार sz = 0;

    cl_int status = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, माप(platform_id), &platform_id, &sz);
    CV_Assert(status == CL_SUCCESS && sz == माप(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    पूर्णांक versionMajor = pi.versionMajor();
    पूर्णांक versionMinor = pi.versionMinor();
    अगर (versionMajor < 1 || (versionMajor == 1 && versionMinor <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                versionMajor, versionMinor, pi.name().c_str(), pi.version().c_str())
        );
}
#पूर्ण_अगर

namespace ocl {

Context& initializeContextFromGL()
{
#अगर !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = nullptr;
    cl_uint devUsed = 0;
    cl_context context = nullptr;

    cl_int status = clGetPlatformIDs(0, शून्य, &platformsCnt);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", status));
    अगर (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    status = clGetPlatformIDs(platformsCnt, &platforms[0], शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", status));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    bool sharingSupported = false;

    क्रम (अचिन्हित पूर्णांक i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, शून्य, &devCnt);
        अगर (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

        प्रयास {
            devices = नया cl_device_id[devCnt];

            status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, शून्य);
            अगर (status != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", status));

            क्रम (अचिन्हित पूर्णांक j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                माप_प्रकार extension_आकार;
                status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, शून्य, &extension_आकार );
                अगर (status != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", status));

                अगर(extension_आकार > 0)
                {
                    std::string devString(extension_आकार, '\0');
                    status = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_आकार);
                    अगर (status != CL_SUCCESS)
                        जारी;
                    अगर (extension_आकार > 0 && devString.size() >= extension_आकार)
                        devString.resize(extension_आकार - 1);

                    माप_प्रकार oldPos = 0;
                    माप_प्रकार spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    जबतक (spacePos != devString.npos) {
                        अगर (म_भेद(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            devUsed = i;
                            sharingSupported = true;
                            अवरोध;
                        }
                        करो {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } जबतक (spacePos == oldPos);
                    }
                }
            }
        } पकड़ो(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            अगर(devices != nullptr) {
                मिटाओ[] devices;
            }
            जारी;
        }

        अगर (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#अगर defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cglShareGroup,
                    0
                };
                context = clCreateContext(props, 0,0, शून्य, शून्य, &ci_Err_अंक);
#या_अगर defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], शून्य, शून्य, &status);
#या_अगर defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], शून्य, शून्य, &status);
#या_अगर defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[devUsed], शून्य, शून्य, &status);
#पूर्ण_अगर
        }

        अगर (status != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", status));
        अन्यथा
            अवरोध;
    }


    cl_platform_id platform = platforms[devUsed];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[devUsed]);
    clReleaseDevice(devices[devUsed]);
    clReleaseContext(context);
    clExecCtx.bind();
    वापस const_cast<Context&>(clExecCtx.getContext());
#पूर्ण_अगर
}

} // namespace cv::ogl::ocl

व्योम convertToGLTexture2D(निवेश_Array src, Texture2D& texture)
{
    CV_UNUSED(src); CV_UNUSED(texture);
#अगर !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    आकार src_आकार = src.size();
    CV_Assert(src_आकार.width == (पूर्णांक)texture.cols() && src_आकार.height == (पूर्णांक)texture.rows());

    using namespace cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &status);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    माप_प्रकार offset = 0; // TODO
    माप_प्रकार dst_origin[3] = {0, 0, 0};
    माप_प्रकार region[3] = { (माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1};
    status = cl_Enqueue_Copy_बफर_To_Image(q, cl_बफर, clImage, offset, dst_origin, region, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#पूर्ण_अगर
}

व्योम convertFromGLTexture2D(स्थिर Texture2D& texture, OutputArray dst)
{
    CV_UNUSED(texture); CV_UNUSED(dst);
#अगर !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    // check texture format
    स्थिर पूर्णांक dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    पूर्णांक textureType = dtype;
    CV_Assert(textureType >= 0);

    using namespace cv::ocl;
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
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", status));

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = clEnqueueAcquireGLObjects(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));
    माप_प्रकार offset = 0; // TODO
    माप_प्रकार src_origin[3] = {0, 0, 0};
    माप_प्रकार region[3] = { (माप_प्रकार)u.cols, (माप_प्रकार)u.rows, 1};
    status = cl_Enqueue_Copy_Image_To_बफर(q, clImage, cl_बफर, src_origin, region, offset, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", status));
    status = clEnqueueReleaseGLObjects(q, 1, &clImage, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(q); // TODO Use events
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(clImage); // TODO RAII
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#पूर्ण_अगर
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(स्थिर बफर_2& बफर, AccessFlag accessFlags)
{
    CV_UNUSED(बफर); CV_UNUSED(accessFlags);
#अगर !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    using namespace cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    पूर्णांक clAccessFlags = 0;
    चयन (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    शेष:
    हाल ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        अवरोध;
    हाल ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        अवरोध;
    हाल ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        अवरोध;
    }

    cl_int status = 0;
    cl_mem cl_बफर = clCreateFromGLBuffer(context, clAccessFlags, बफर.bufId(), &status);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", status));

    gl::Finish();

    status = clEnqueueAcquireGLObjects(clQueue, 1, &cl_बफर, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", status));

    माप_प्रकार step = बफर.cols() * बफर.elem_आकार();
    पूर्णांक rows = बफर.rows();
    पूर्णांक cols = बफर.cols();
    पूर्णांक type = बफर.type();

    UMat u;
    convert_From_बफर(cl_बफर, step, rows, cols, type, u);
    वापस u;
#पूर्ण_अगर
}

व्योम unmapGLBuffer(UMat& u)
{
    CV_UNUSED(u);
#अगर !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#या_अगर !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#अन्यथा
    using namespace cv::ocl;
    cl_command_queue clQueue = (cl_command_queue)Queue::getDefault().ptr();

    cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int status = clEnqueueReleaseGLObjects(clQueue, 1, &cl_बफर, 0, शून्य, शून्य);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", status));

    status = clFinish(clQueue);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", status));

    status = clReleaseMemObject(cl_बफर);
    अगर (status != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", status));
#पूर्ण_अगर
}

}} // namespace cv::ogl
