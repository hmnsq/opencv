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
   #تعریف_کن GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#وإلا
   #تعریف_کن GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#پایان_اگر

#اگر_تعریف_شده HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    إذا (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_نسخه
#        define GL_نسخه 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#وإلا // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#پایان_اگر // HAVE_OPENGL

استفاده نطاق cv;
استفاده نطاق cv::cuda;

#إذا defined(_MSC_VER)
#رهنمود warning(disable : 4702)  // unreachable code
#پایان_اگر

نطاق
{
#اگر_تعریف_نشده HAVE_OPENGL
مضمن ساكن CV_NORETURN فراغ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#وإلاإذا defined _DEBUG
مضمن ساكن منطقي checkError(ثابت حرف* file, ثابت صحيح سطر, ثابت حرف* func = 0)
{
    GLenum err = gl::GetError();
    إذا (err != gl::NO_ERROR_)
    {
        ثابت حرف* پیام;
        بدل (err)
        {
        حالة gl::INVALID_ENUM:
            پیام = "An unacceptable value is specified for an enumerated argument";
            اكسر;
        حالة gl::INVALID_مقدار:
            پیام = "A numeric argument is out of range";
            اكسر;
        حالة gl::INVALID_OPERATION:
            پیام = "The specified operation is not allowed in the current state";
            اكسر;
        حالة gl::بیرون_از_حافظه:
            پیام = "There is not enough memory left to execute the command";
            اكسر;
        افتراضي:
            پیام = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, پیام, file, سطر);
    }
    أرجع درست;
}
#پایان_اگر // HAVE_OPENGL
} // namespace

#تعریف_کن CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#اگر_تعریف_شده HAVE_OPENGL
نطاق
{
    ثابت GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#پایان_اگر

////////////////////////////////////////////////////////////////////////
// setGlDevice

فراغ cv::cuda::مجموعه_Gl_Device(صحيح device)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(device);
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        CV_استفادهنشده(device);
        throw_no_cuda();
    #وإلا
        cuda_امن_Call( cudaGLSetGLDevice(device) );
    #پایان_اگر
#پایان_اگر
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

        فراغ ثبات_میانگیر(GLuint میانگیر);
        فراغ release();

        فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream = 0);
        فراغ copy_مقصد(فراغ* dst, size_t dpitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream = 0);

        فراغ* map(cuda_جریان_t stream = 0);
        فراغ unmap(cuda_جریان_t stream = 0);

    خاص:
        cudaGraphicsResource_t resource_;
        GLuint میانگیر_3;

        صنف GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), میانگیر_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    فراغ CudaResource::ثبات_میانگیر(GLuint میانگیر)
    {
        CV_DbgAssert( میانگیر != 0 );

        إذا (میانگیر_3 == میانگیر)
            أرجع;

        cudaGraphicsResource_t resource;
        cuda_امن_Call( cuda_Graphics_GLRegister_میانگیر(&resource, میانگیر, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        میانگیر_3 = میانگیر;
    }

    فراغ CudaResource::release()
    {
        إذا (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        میانگیر_3 = 0;
    }

    صنف CudaResource::GraphicsMapHolder
    {
    عام:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_جریان_t stream);
        ~GraphicsMapHolder();

        فراغ reset();

    خاص:
        cudaGraphicsResource_t* resource_;
        cuda_جریان_t جریان_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_جریان_t stream) : resource_(resource), جریان_3(stream)
    {
        إذا (resource_)
            cuda_امن_Call( cudaGraphicsMapResources(1, resource_, جریان_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        إذا (resource_)
            cudaGraphicsUnmapResources(1, resource_, جریان_3);
    }

    فراغ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    فراغ CudaResource::copyFrom(ثابت فراغ* src, size_t spitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_استفادهنشده(h);

        فراغ* dst;
        size_t size;
        cuda_امن_Call( cuda_Graphics_Resource_Get_Mapped_اشاره_گر(&dst, &size, resource_) );

        CV_DbgAssert( عرض * ارتفاع == size );

        إذا (stream == 0)
            cuda_امن_Call( cudaMemcpy2D(dst, عرض, src, spitch, عرض, ارتفاع, cuda_Memcpy_Device_مقصد_Device) );
        وإلا
            cuda_امن_Call( cudaMemcpy2DAsync(dst, عرض, src, spitch, عرض, ارتفاع, cuda_Memcpy_Device_مقصد_Device, stream) );
    }

    فراغ CudaResource::copy_مقصد(فراغ* dst, size_t dpitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_استفادهنشده(h);

        فراغ* src;
        size_t size;
        cuda_امن_Call( cuda_Graphics_Resource_Get_Mapped_اشاره_گر(&src, &size, resource_) );

        CV_DbgAssert( عرض * ارتفاع == size );

        إذا (stream == 0)
            cuda_امن_Call( cudaMemcpy2D(dst, dpitch, src, عرض, عرض, ارتفاع, cuda_Memcpy_Device_مقصد_Device) );
        وإلا
            cuda_امن_Call( cudaMemcpy2DAsync(dst, dpitch, src, عرض, عرض, ارتفاع, cuda_Memcpy_Device_مقصد_Device, stream) );
    }

    فراغ* CudaResource::map(cuda_جریان_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        فراغ* ptr;
        size_t size;
        cuda_امن_Call( cuda_Graphics_Resource_Get_Mapped_اشاره_گر(&ptr, &size, resource_) );

        h.reset();

        أرجع ptr;
    }

    فراغ CudaResource::unmap(cuda_جریان_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#پایان_اگر

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#اگر_تعریف_نشده HAVE_OPENGL

صنف cv::ogl::میانگیر_2::Impl
{
};

#وإلا

صنف cv::ogl::میانگیر_2::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint bufId, منطقي autoRelease);
    Impl(GLsizeiptr size, ثابت GLvoid* داده, GLenum target, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLenum target) ثابت;

    فراغ copyFrom(GLuint srcBuf, GLsizeiptr size);

    فراغ copyFrom(GLsizeiptr size, ثابت GLvoid* داده);
    فراغ copy_مقصد(GLsizeiptr size, GLvoid* داده) ثابت;

    فراغ resize(GLsizeiptr size, GLenum target);

    فراغ* mapHost(GLenum access);
    فراغ unmapHost();

#اگر_تعریف_شده HAVE_CUDA
    فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream = 0);
    فراغ copy_مقصد(فراغ* dst, size_t dpitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream = 0) ثابت;

    فراغ* mapDevice(cuda_جریان_t stream = 0);
    فراغ unmapDevice(cuda_جریان_t stream = 0);
#پایان_اگر

    فراغ مجموعه_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint bufId() ثابت { أرجع bufId_; }

خاص:
    Impl();

    GLuint bufId_;
    منطقي autoRelease_;

#اگر_تعریف_شده HAVE_CUDA
    تغییرپذیر CudaResource cudaResource_;
#پایان_اگر
};

ثابت Ptr<cv::ogl::میانگیر_2::Impl>& cv::ogl::میانگیر_2::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::میانگیر_2::Impl::Impl() : bufId_(0), autoRelease_(خطأ)
{
}

cv::ogl::میانگیر_2::Impl::Impl(GLuint abufId, منطقي autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_میانگیر(abufId) == gl::TRUE_ );
}

cv::ogl::میانگیر_2::Impl::Impl(GLsizeiptr size, ثابت GLvoid* داده, GLenum target, منطقي autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_میانگیر(target, bufId_);
    CV_CheckGlError();

    gl::میانگیر_داده(target, size, داده, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_میانگیر(target, 0);
    CV_CheckGlError();
}

cv::ogl::میانگیر_2::Impl::~Impl()
{
    إذا (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

فراغ cv::ogl::میانگیر_2::Impl::bind(GLenum target) ثابت
{
    gl::Bind_میانگیر(target, bufId_);
    CV_CheckGlError();
}

فراغ cv::ogl::میانگیر_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_میانگیر(gl::COPY_WRITE_میانگیر, bufId_);
    CV_CheckGlError();

    gl::Bind_میانگیر(gl::COPY_READ_میانگیر, srcBuf);
    CV_CheckGlError();

    gl::Copy_میانگیر_Sub_داده(gl::COPY_READ_میانگیر, gl::COPY_WRITE_میانگیر, 0, 0, size);
    CV_CheckGlError();
}

فراغ cv::ogl::میانگیر_2::Impl::copyFrom(GLsizeiptr size, ثابت GLvoid* داده)
{
    gl::Bind_میانگیر(gl::COPY_WRITE_میانگیر, bufId_);
    CV_CheckGlError();

    gl::میانگیر_Sub_داده(gl::COPY_WRITE_میانگیر, 0, size, داده);
    CV_CheckGlError();
}

فراغ cv::ogl::میانگیر_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_میانگیر(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::میانگیر_داده(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_میانگیر(target, 0);
    CV_CheckGlError();
}

فراغ cv::ogl::میانگیر_2::Impl::copy_مقصد(GLsizeiptr size, GLvoid* داده) ثابت
{
    gl::Bind_میانگیر(gl::COPY_READ_میانگیر, bufId_);
    CV_CheckGlError();

    gl::Get_میانگیر_Sub_داده(gl::COPY_READ_میانگیر, 0, size, داده);
    CV_CheckGlError();
}

فراغ* cv::ogl::میانگیر_2::Impl::mapHost(GLenum access)
{
    gl::Bind_میانگیر(gl::COPY_READ_میانگیر, bufId_);
    CV_CheckGlError();

    GLvoid* داده = gl::Map_میانگیر(gl::COPY_READ_میانگیر, access);
    CV_CheckGlError();

    أرجع داده;
}

فراغ cv::ogl::میانگیر_2::Impl::unmapHost()
{
    gl::Unmap_میانگیر(gl::COPY_READ_میانگیر);
}

#اگر_تعریف_شده HAVE_CUDA

فراغ cv::ogl::میانگیر_2::Impl::copyFrom(ثابت فراغ* src, size_t spitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream)
{
    cudaResource_.ثبات_میانگیر(bufId_);
    cudaResource_.copyFrom(src, spitch, عرض, ارتفاع, stream);
}

فراغ cv::ogl::میانگیر_2::Impl::copy_مقصد(فراغ* dst, size_t dpitch, size_t عرض, size_t ارتفاع, cuda_جریان_t stream) ثابت
{
    cudaResource_.ثبات_میانگیر(bufId_);
    cudaResource_.copy_مقصد(dst, dpitch, عرض, ارتفاع, stream);
}

فراغ* cv::ogl::میانگیر_2::Impl::mapDevice(cuda_جریان_t stream)
{
    cudaResource_.ثبات_میانگیر(bufId_);
    أرجع cudaResource_.map(stream);
}

فراغ cv::ogl::میانگیر_2::Impl::unmapDevice(cuda_جریان_t stream)
{
    cudaResource_.unmap(stream);
}

#پایان_اگر // HAVE_CUDA

#پایان_اگر // HAVE_OPENGL

cv::ogl::میانگیر_2::میانگیر_2() : ردیفها_2(0), تعداد_ستون_2(0), type_(0)
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#پایان_اگر
}

cv::ogl::میانگیر_2::میانگیر_2(صحيح arows, صحيح acols, صحيح atype, بیعلامت صحيح abufId, منطقي autoRelease) : ردیفها_2(0), تعداد_ستون_2(0), type_(0)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arows);
    CV_استفادهنشده(acols);
    CV_استفادهنشده(atype);
    CV_استفادهنشده(abufId);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    ردیفها_2 = arows;
    تعداد_ستون_2 = acols;
    type_ = atype;
#پایان_اگر
}

cv::ogl::میانگیر_2::میانگیر_2(اندازه asize, صحيح atype, بیعلامت صحيح abufId, منطقي autoRelease) : ردیفها_2(0), تعداد_ستون_2(0), type_(0)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(asize);
    CV_استفادهنشده(atype);
    CV_استفادهنشده(abufId);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    ردیفها_2 = asize.ارتفاع;
    تعداد_ستون_2 = asize.عرض;
    type_ = atype;
#پایان_اگر
}

cv::ogl::میانگیر_2::میانگیر_2(درون_داد_Array arr, هدف target, منطقي autoRelease) : ردیفها_2(0), تعداد_ستون_2(0), type_(0)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(target);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح نوع = arr.نوع();

    بدل (نوع)
    {
    حالة درون_داد_Array_2::OPENGL_میانگیر:
    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        اكسر;

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ثابت GLsizeiptr asize = mat.ردیفها * mat.تعداد_ستون * mat.elem_اندازه();
            impl_.reset(جديد Impl(asize, mat.داده, target, autoRelease));
            ردیفها_2 = mat.ردیفها;
            تعداد_ستون_2 = mat.تعداد_ستون;
            type_ = mat.type();
            اكسر;
        }
    }
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::create(صحيح arows, صحيح acols, صحيح atype, هدف target, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arows);
    CV_استفادهنشده(acols);
    CV_استفادهنشده(atype);
    CV_استفادهنشده(target);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (ردیفها_2 != arows || تعداد_ستون_2 != acols || type_ != atype)
    {
        ثابت GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_اندازه(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        إذا (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->مجموعه_Auto_Release(autoRelease);
        }
        وإلا
            impl_.reset(جديد Impl(asize, 0, target, autoRelease));
        ردیفها_2 = arows;
        تعداد_ستون_2 = acols;
        type_ = atype;
    }
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::release()
{
#اگر_تعریف_شده HAVE_OPENGL
    إذا (impl_)
        impl_->مجموعه_Auto_Release(درست);
    impl_ = Impl::empty();
    ردیفها_2 = 0;
    تعداد_ستون_2 = 0;
    type_ = 0;
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    impl_->مجموعه_Auto_Release(flag);
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::copyFrom(درون_داد_Array arr, هدف target, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(target);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح نوع = arr.نوع();

    ثابت اندازه asize = arr.size();
    ثابت صحيح atype = arr.type();
    create(asize, atype, target, autoRelease);

    بدل (نوع)
    {
    حالة درون_داد_Array_2::OPENGL_میانگیر:
        {
            ogl::میانگیر_2 buf = arr.get_OGl_میانگیر();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_اندازه(atype));
            اكسر;
        }

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #اگر_تعریف_نشده HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.داده, dmat.گام, dmat.تعداد_ستون * dmat.elem_اندازه(), dmat.ردیفها);
            #پایان_اگر

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_اندازه(atype), mat.داده);
        }
    }
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::copyFrom(درون_داد_Array arr, cuda::جریان& stream, هدف target, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(stream);
    CV_استفادهنشده(target);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        CV_استفادهنشده(arr);
        CV_استفادهنشده(stream);
        CV_استفادهنشده(target);
        CV_استفادهنشده(autoRelease);
        throw_no_cuda();
    #وإلا
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.داده, dmat.گام, dmat.تعداد_ستون * dmat.elem_اندازه(), dmat.ردیفها, cuda::جریان_Accessor::get_جریان(stream));
    #پایان_اگر
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::copy_مقصد(OutputArray arr) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    throw_no_ogl();
#وإلا
    ثابت صحيح نوع = arr.نوع();

    بدل (نوع)
    {
    حالة درون_داد_Array_2::OPENGL_میانگیر:
        {
            arr.get_OGl_میانگیر_Ref().copyFrom(*هذا);
            اكسر;
        }

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #اگر_تعریف_نشده HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(ردیفها_2, تعداد_ستون_2, type_);
                impl_->copy_مقصد(dmat.داده, dmat.گام, dmat.تعداد_ستون * dmat.elem_اندازه(), dmat.ردیفها);
            #پایان_اگر

            اكسر;
        }

    افتراضي:
        {
            arr.create(ردیفها_2, تعداد_ستون_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_مقصد(mat.ردیفها * mat.تعداد_ستون * mat.elem_اندازه(), mat.داده);
        }
    }
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::copy_مقصد(OutputArray arr, cuda::جریان& stream) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(stream);
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        CV_استفادهنشده(arr);
        CV_استفادهنشده(stream);
        throw_no_cuda();
    #وإلا
        arr.create(ردیفها_2, تعداد_ستون_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_مقصد(dmat.داده, dmat.گام, dmat.تعداد_ستون * dmat.elem_اندازه(), dmat.ردیفها, cuda::جریان_Accessor::get_جریان(stream));
    #پایان_اگر
#پایان_اگر
}

cv::ogl::میانگیر_2 cv::ogl::میانگیر_2::clone(هدف target, منطقي autoRelease) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(target);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    ogl::میانگیر_2 buf;
    buf.copyFrom(*هذا, target, autoRelease);
    أرجع buf;
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::bind(هدف target) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(target);
    throw_no_ogl();
#وإلا
    impl_->bind(target);
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::unbind(هدف target)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(target);
    throw_no_ogl();
#وإلا
    gl::Bind_میانگیر(target, 0);
    CV_CheckGlError();
#پایان_اگر
}

Mat cv::ogl::میانگیر_2::mapHost(Access access)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(access);
    throw_no_ogl();
#وإلا
    أرجع Mat(ردیفها_2, تعداد_ستون_2, type_, impl_->mapHost(access));
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::unmapHost()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->unmapHost();
#پایان_اگر
}

GpuMat cv::ogl::میانگیر_2::mapDevice()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(ردیفها_2, تعداد_ستون_2, type_, impl_->mapDevice());
    #پایان_اگر
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::unmapDevice()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice();
    #پایان_اگر
#پایان_اگر
}

cuda::GpuMat cv::ogl::میانگیر_2::mapDevice(cuda::جریان& stream)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(stream);
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        CV_استفادهنشده(stream);
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(ردیفها_2, تعداد_ستون_2, type_, impl_->mapDevice(cuda::جریان_Accessor::get_جریان(stream)));
    #پایان_اگر
#پایان_اگر
}

فراغ cv::ogl::میانگیر_2::unmapDevice(cuda::جریان& stream)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(stream);
    throw_no_ogl();
#وإلا
    #اگر_تعریف_نشده HAVE_CUDA
        CV_استفادهنشده(stream);
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice(cuda::جریان_Accessor::get_جریان(stream));
    #پایان_اگر
#پایان_اگر
}

بیعلامت صحيح cv::ogl::میانگیر_2::bufId() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->bufId();
#پایان_اگر
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#اگر_تعریف_نشده HAVE_OPENGL

صنف cv::ogl::Texture2D::Impl
{
};

#وإلا

صنف cv::ogl::Texture2D::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl(GLuint texId, منطقي autoRelease);
    Impl(GLint داخلی_Format, GLsizei عرض, GLsizei ارتفاع, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease);
    ~Impl();

    فراغ copyFrom(GLsizei عرض, GLsizei ارتفاع, GLenum format, GLenum type, ثابت GLvoid *pixels);
    فراغ copy_مقصد(GLenum format, GLenum type, GLvoid* pixels) ثابت;

    فراغ bind() ثابت;

    فراغ مجموعه_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

cv::ogl::Texture2D::Impl::Impl(GLint داخلی_Format, GLsizei عرض, GLsizei ارتفاع, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, داخلی_Format, عرض, ارتفاع, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::Texture2D::Impl::copyFrom(GLsizei عرض, GLsizei ارتفاع, GLenum format, GLenum type, ثابت GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, عرض, ارتفاع, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

فراغ cv::ogl::Texture2D::Impl::copy_مقصد(GLenum format, GLenum type, GLvoid* pixels) ثابت
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

#پایان_اگر // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : ردیفها_2(0), تعداد_ستون_2(0), format_(NONE)
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#پایان_اگر
}

cv::ogl::Texture2D::Texture2D(صحيح arows, صحيح acols, Format aformat, بیعلامت صحيح atexId, منطقي autoRelease) : ردیفها_2(0), تعداد_ستون_2(0), format_(NONE)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arows);
    CV_استفادهنشده(acols);
    CV_استفادهنشده(aformat);
    CV_استفادهنشده(atexId);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    ردیفها_2 = arows;
    تعداد_ستون_2 = acols;
    format_ = aformat;
#پایان_اگر
}

cv::ogl::Texture2D::Texture2D(اندازه asize, Format aformat, بیعلامت صحيح atexId, منطقي autoRelease) : ردیفها_2(0), تعداد_ستون_2(0), format_(NONE)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(asize);
    CV_استفادهنشده(aformat);
    CV_استفادهنشده(atexId);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    ردیفها_2 = asize.ارتفاع;
    تعداد_ستون_2 = asize.عرض;
    format_ = aformat;
#پایان_اگر
}

cv::ogl::Texture2D::Texture2D(درون_داد_Array arr, منطقي autoRelease) : ردیفها_2(0), تعداد_ستون_2(0), format_(NONE)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح نوع = arr.نوع();

    ثابت اندازه asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح عمق = CV_MAT_عمق(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

    CV_Assert( عمق <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format داخلی_Formats[] =
    {
        NONE, عمق_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::عمق_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    بدل (نوع)
    {
    حالة درون_داد_Array_2::OPENGL_میانگیر:
        {
            ogl::میانگیر_2 buf = arr.get_OGl_میانگیر();
            buf.bind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            impl_.reset(جديد Impl(داخلی_Formats[cn], asize.عرض, asize.ارتفاع, srcFormats[cn], gl_types[عمق], 0, autoRelease));
            ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            اكسر;
        }

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #اگر_تعریف_نشده HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::میانگیر_2 buf(dmat, ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
                buf.مجموعه_Auto_Release(درست);
                buf.bind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
                impl_.reset(جديد Impl(داخلی_Formats[cn], asize.عرض, asize.ارتفاع, srcFormats[cn], gl_types[عمق], 0, autoRelease));
                ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            #پایان_اگر

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            impl_.reset(جديد Impl(داخلی_Formats[cn], asize.عرض, asize.ارتفاع, srcFormats[cn], gl_types[عمق], mat.داده, autoRelease));
            اكسر;
        }
    }

    ردیفها_2 = asize.ارتفاع;
    تعداد_ستون_2 = asize.عرض;
    format_ = داخلی_Formats[cn];
#پایان_اگر
}

فراغ cv::ogl::Texture2D::create(صحيح arows, صحيح acols, Format aformat, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arows);
    CV_استفادهنشده(acols);
    CV_استفادهنشده(aformat);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (ردیفها_2 != arows || تعداد_ستون_2 != acols || format_ != aformat)
    {
        ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
        impl_.reset(جديد Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        ردیفها_2 = arows;
        تعداد_ستون_2 = acols;
        format_ = aformat;
    }
#پایان_اگر
}

فراغ cv::ogl::Texture2D::release()
{
#اگر_تعریف_شده HAVE_OPENGL
    إذا (impl_)
        impl_->مجموعه_Auto_Release(درست);
    impl_ = Impl::empty();
    ردیفها_2 = 0;
    تعداد_ستون_2 = 0;
    format_ = NONE;
#پایان_اگر
}

فراغ cv::ogl::Texture2D::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    impl_->مجموعه_Auto_Release(flag);
#پایان_اگر
}

فراغ cv::ogl::Texture2D::copyFrom(درون_داد_Array arr, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح نوع = arr.نوع();

    ثابت اندازه asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح عمق = CV_MAT_عمق(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

    CV_Assert( عمق <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format داخلی_Formats[] =
    {
        NONE, عمق_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::عمق_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, داخلی_Formats[cn], autoRelease);

    بدل(نوع)
    {
    حالة درون_داد_Array_2::OPENGL_میانگیر:
        {
            ogl::میانگیر_2 buf = arr.get_OGl_میانگیر();
            buf.bind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            impl_->copyFrom(asize.عرض, asize.ارتفاع, srcFormats[cn], gl_types[عمق], 0);
            ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            اكسر;
        }

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #اگر_تعریف_نشده HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::میانگیر_2 buf(dmat, ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
                buf.مجموعه_Auto_Release(درست);
                buf.bind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
                impl_->copyFrom(asize.عرض, asize.ارتفاع, srcFormats[cn], gl_types[عمق], 0);
                ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            #پایان_اگر

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_UNPACK_میانگیر);
            impl_->copyFrom(asize.عرض, asize.ارتفاع, srcFormats[cn], gl_types[عمق], mat.داده);
        }
    }
#پایان_اگر
}

فراغ cv::ogl::Texture2D::copy_مقصد(OutputArray arr, صحيح ddepth, منطقي autoRelease) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(ddepth);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح نوع = arr.نوع();

    ثابت صحيح cn = format_ == عمق_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ثابت GLenum dstFormat = format_ == عمق_COMPONENT ? gl::عمق_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    بدل(نوع)
    {
    حالة درون_داد_Array_2::OPENGL_میانگیر:
        {
            ogl::میانگیر_2& buf = arr.get_OGl_میانگیر_Ref();
            buf.create(ردیفها_2, تعداد_ستون_2, CV_MAKE_TYPE(ddepth, cn), ogl::میانگیر_2::PIXEL_PACK_میانگیر, autoRelease);
            buf.bind(ogl::میانگیر_2::PIXEL_PACK_میانگیر);
            impl_->copy_مقصد(dstFormat, gl_types[ddepth], 0);
            ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_PACK_میانگیر);
            اكسر;
        }

    حالة درون_داد_Array_2::CUDA_GPU_MAT:
        {
            #اگر_تعریف_نشده HAVE_CUDA
                throw_no_cuda();
            #وإلا
                ogl::میانگیر_2 buf(ردیفها_2, تعداد_ستون_2, CV_MAKE_TYPE(ddepth, cn), ogl::میانگیر_2::PIXEL_PACK_میانگیر);
                buf.مجموعه_Auto_Release(درست);
                buf.bind(ogl::میانگیر_2::PIXEL_PACK_میانگیر);
                impl_->copy_مقصد(dstFormat, gl_types[ddepth], 0);
                ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_PACK_میانگیر);
                buf.copy_مقصد(arr);
            #پایان_اگر

            اكسر;
        }

    افتراضي:
        {
            arr.create(ردیفها_2, تعداد_ستون_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::میانگیر_2::unbind(ogl::میانگیر_2::PIXEL_PACK_میانگیر);
            impl_->copy_مقصد(dstFormat, gl_types[ddepth], mat.داده);
        }
    }
#پایان_اگر
}

فراغ cv::ogl::Texture2D::bind() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#پایان_اگر
}

بیعلامت صحيح cv::ogl::Texture2D::texId() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#پایان_اگر
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#اگر_تعریف_نشده HAVE_OPENGL

صنف cv::ogl::Texture_میانگیر::Impl
{
};

#وإلا

صنف cv::ogl::Texture_میانگیر::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl();
    Impl(GLenum داخلی_Format, GLuint bufId, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLint unit) ثابت;

    فراغ مجموعه_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { أرجع texId_; }

خاص:
    GLuint texId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::Texture_میانگیر::Impl> cv::ogl::Texture_میانگیر::Impl::empty()
{
    ساكن Ptr<Impl> p = makePtr<Impl>();
    أرجع p;
}

cv::ogl::Texture_میانگیر::Impl::Impl() : texId_(0), autoRelease_(خطأ)
{
}

cv::ogl::Texture_میانگیر::Impl::Impl(GLenum داخلی_Format, GLuint bufId, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_میانگیر, texId_);
    CV_CheckGlError();

    gl::Tex_میانگیر(gl::TEXTURE_میانگیر, داخلی_Format, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_میانگیر, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_میانگیر::Impl::~Impl()
{
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::Texture_میانگیر::Impl::bind(GLint unit) ثابت
{
    gl::فعال_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_میانگیر, texId_);
    CV_CheckGlError();
}

#پایان_اگر // HAVE_OPENGL

cv::ogl::Texture_میانگیر::Texture_میانگیر() : format_(NONE)
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#پایان_اگر
}

cv::ogl::Texture_میانگیر::Texture_میانگیر(ثابت میانگیر_2& buf, Format aformat, منطقي autoRelease) : format_(NONE)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(buf);
    CV_استفادهنشده(aformat);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#پایان_اگر
}

فراغ cv::ogl::Texture_میانگیر::create(ثابت میانگیر_2& buf, Format aformat, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(buf);
    CV_استفادهنشده(aformat);
    CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#پایان_اگر
}

فراغ cv::ogl::Texture_میانگیر::release()
{
#اگر_تعریف_شده HAVE_OPENGL
    إذا (impl_)
        impl_->مجموعه_Auto_Release(درست);
    impl_ = Impl::empty();
#پایان_اگر
    buf_ = میانگیر_2();
    format_ = NONE;
}

فراغ cv::ogl::Texture_میانگیر::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    impl_->مجموعه_Auto_Release(flag);
#پایان_اگر
}

فراغ cv::ogl::Texture_میانگیر::bind(صحيح unit) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(unit);
    throw_no_ogl();
#وإلا
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#پایان_اگر
}

cv::ogl::Texture_میانگیر::Format cv::ogl::Texture_میانگیر::format() ثابت
{
    أرجع format_;
}

منطقي cv::ogl::Texture_میانگیر::empty() ثابت
{
    أرجع format_ == NONE;
}

بیعلامت صحيح cv::ogl::Texture_میانگیر::texId() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#پایان_اگر
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

فراغ cv::ogl::Arrays::مجموعه_رأس_Array(درون_داد_Array رأس)
{
    ثابت صحيح cn = رأس.channels();
    ثابت صحيح عمق = رأس.عمق();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( عمق == CV_16S || عمق == CV_32S || عمق == CV_32F || عمق == CV_64F );

    إذا (رأس.نوع() == درون_داد_Array_2::OPENGL_میانگیر)
        رأس_2 = رأس.get_OGl_میانگیر();
    وإلا
        رأس_2.copyFrom(رأس);

    اندازه_3 = رأس_2.size().area();
}

فراغ cv::ogl::Arrays::reset_رأس_Array()
{
    رأس_2.release();
    اندازه_3 = 0;
}

فراغ cv::ogl::Arrays::مجموعه_Color_Array(درون_داد_Array color)
{
    ثابت صحيح cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    إذا (color.نوع() == درون_داد_Array_2::OPENGL_میانگیر)
        color_ = color.get_OGl_میانگیر();
    وإلا
        color_.copyFrom(color);
}

فراغ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

فراغ cv::ogl::Arrays::مجموعه_نرمال_Array(درون_داد_Array نرمال)
{
    ثابت صحيح cn = نرمال.channels();
    ثابت صحيح عمق = نرمال.عمق();

    CV_Assert( cn == 3 );
    CV_Assert( عمق == CV_8S || عمق == CV_16S || عمق == CV_32S || عمق == CV_32F || عمق == CV_64F );

    إذا (نرمال.نوع() == درون_داد_Array_2::OPENGL_میانگیر)
        نرمال_2 = نرمال.get_OGl_میانگیر();
    وإلا
        نرمال_2.copyFrom(نرمال);
}

فراغ cv::ogl::Arrays::reset_نرمال_Array()
{
    نرمال_2.release();
}

فراغ cv::ogl::Arrays::مجموعه_Tex_Coord_Array(درون_داد_Array texCoord)
{
    ثابت صحيح cn = texCoord.channels();
    ثابت صحيح عمق = texCoord.عمق();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( عمق == CV_16S || عمق == CV_32S || عمق == CV_32F || عمق == CV_64F );

    إذا (texCoord.نوع() == درون_داد_Array_2::OPENGL_میانگیر)
        texCoord_ = texCoord.get_OGl_میانگیر();
    وإلا
        texCoord_.copyFrom(texCoord);
}

فراغ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

فراغ cv::ogl::Arrays::release()
{
    reset_رأس_Array();
    resetColorArray();
    reset_نرمال_Array();
    resetTexCoordArray();
}

فراغ cv::ogl::Arrays::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    رأس_2.مجموعه_Auto_Release(flag);
    color_.مجموعه_Auto_Release(flag);
    نرمال_2.مجموعه_Auto_Release(flag);
    texCoord_.مجموعه_Auto_Release(flag);
#پایان_اگر
}

فراغ cv::ogl::Arrays::bind() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == اندازه_3 );
    CV_Assert( نرمال_2.empty() || نرمال_2.size().area() == اندازه_3 );
    CV_Assert( color_.empty() || color_.size().area() == اندازه_3 );

    إذا (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::میانگیر_2::ARRAY_میانگیر);

        gl::Tex_Coord_اشاره_گر(texCoord_.channels(), gl_types[texCoord_.عمق()], 0, 0);
        CV_CheckGlError();
    }

    إذا (نرمال_2.empty())
    {
        gl::DisableClientState(gl::نرمال_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::نرمال_ARRAY);
        CV_CheckGlError();

        نرمال_2.bind(ogl::میانگیر_2::ARRAY_میانگیر);

        gl::نرمال_اشاره_گر(gl_types[نرمال_2.عمق()], 0, 0);
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

        color_.bind(ogl::میانگیر_2::ARRAY_میانگیر);

        ثابت صحيح cn = color_.channels();

        gl::Color_اشاره_گر(cn, gl_types[color_.عمق()], 0, 0);
        CV_CheckGlError();
    }

    إذا (رأس_2.empty())
    {
        gl::DisableClientState(gl::رأس_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::رأس_ARRAY);
        CV_CheckGlError();

        رأس_2.bind(ogl::میانگیر_2::ARRAY_میانگیر);

        gl::رأس_اشاره_گر(رأس_2.channels(), gl_types[رأس_2.عمق()], 0, 0);
        CV_CheckGlError();
    }

    ogl::میانگیر_2::unbind(ogl::میانگیر_2::ARRAY_میانگیر);
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#اگر_تعریف_نشده HAVE_OPENGL

صنف cv::ogl::رأس_Array::Impl
{
};

#وإلا

صنف cv::ogl::رأس_Array::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint vaId, منطقي autoRelease);
    Impl(std::initializer_list<Attribute> صفات, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    فراغ مجموعه_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint vaId() ثابت { أرجع vaId_; }

خاص:
    Impl();

    GLuint vaId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::رأس_Array::Impl>& cv::ogl::رأس_Array::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::رأس_Array::Impl::Impl() : vaId_(0), autoRelease_(خطأ)
{
}

cv::ogl::رأس_Array::Impl::Impl(GLuint vaId, منطقي autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_رأس_Array(vaId) == gl::TRUE_ );
}

cv::ogl::رأس_Array::Impl::Impl(std::initializer_list<Attribute> صفات, منطقي autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_رأس_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_رأس_Array(vaId_);
    CV_CheckGlError();

    لكل (خودکار& attribute : صفات)
    {
        attribute.میانگیر_3.bind(میانگیر_2::ARRAY_میانگیر);
        إذا (attribute.integer_)
        {
            gl::رأس_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.اندازه_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.جابه_جایی)
            );
            CV_CheckGlError();
        }
        وإلا
        {
            gl::رأس_Attrib_اشاره_گر(
                attribute.shader_loc_,
                attribute.اندازه_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.جابه_جایی)
            );
            CV_CheckGlError();
        }

        gl::Enable_رأس_Attrib_Array(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::رأس_Array::Impl::~Impl()
{
    إذا (autoRelease_ && vaId_)
        gl::Delete_رأس_Arrays(1, &vaId_);
}

فراغ cv::ogl::رأس_Array::Impl::bind()
{
    gl::Bind_رأس_Array(vaId_);
    CV_CheckGlError();
}

#پایان_اگر // HAVE_OPENGL

cv::ogl::رأس_Array::رأس_Array()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#پایان_اگر
}

cv::ogl::رأس_Array::رأس_Array(std::initializer_list<Attribute> صفات, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(صفات); CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(صفات, autoRelease));
#پایان_اگر
}

فراغ cv::ogl::رأس_Array::create(std::initializer_list<Attribute> صفات, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(صفات); CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(صفات, autoRelease));
#پایان_اگر
}

فراغ cv::ogl::رأس_Array::release()
{
#اگر_تعریف_شده HAVE_OPENGL
    إذا (impl_)
        impl_->مجموعه_Auto_Release(درست);
    impl_ = Impl::empty();
#پایان_اگر
}

فراغ cv::ogl::رأس_Array::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    impl_->مجموعه_Auto_Release(flag);
#پایان_اگر
}

فراغ cv::ogl::رأس_Array::bind() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#پایان_اگر
}

فراغ cv::ogl::رأس_Array::unbind()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::Bind_رأس_Array(0);
#پایان_اگر
}

بیعلامت صحيح cv::ogl::رأس_Array::vaId() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->vaId();
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#اگر_تعریف_نشده HAVE_OPENGL

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

    فراغ مجموعه_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

    GLint وضعیت;
    gl::Shader_منبع(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_وضعیت, &وضعیت);
    إذا (!وضعیت) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_درازی, &وضعیت);
        info_log.resize(وضعیت);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, اشاره_تهی, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    إذا (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#پایان_اگر // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::رأس_3)
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#پایان_اگر
}

cv::ogl::Shader::Shader(ثابت حرف* src, ShaderType type, منطقي autoRelease) : type_(type)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(src); CV_استفادهنشده(type); CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
#پایان_اگر
}

فراغ cv::ogl::Shader::create(ثابت حرف* src, ShaderType type, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(src); CV_استفادهنشده(type); CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
    type_ = type;
#پایان_اگر
}

فراغ cv::ogl::Shader::release()
{
#اگر_تعریف_شده HAVE_OPENGL
    إذا (impl_)
        impl_->مجموعه_Auto_Release(درست);
    impl_ = Impl::empty();
#پایان_اگر
}

فراغ cv::ogl::Shader::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    impl_->مجموعه_Auto_Release(flag);
#پایان_اگر
}

بیعلامت صحيح cv::ogl::Shader::shaderId() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->shaderId();
#پایان_اگر
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ثابت
{
    أرجع type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#اگر_تعریف_نشده HAVE_OPENGL

صنف cv::ogl::برنامه_2::Impl
{
};

#وإلا

صنف cv::ogl::برنامه_2::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint برنامه_Id, منطقي autoRelease);
    Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    صحيح getAttributeLocation(ثابت حرف* name) ثابت;
    صحيح get_یکنواخت_Location(ثابت حرف* name) ثابت;

    فراغ مجموعه_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint برنامه_Id() ثابت { أرجع برنامه_Id_2; }

خاص:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint برنامه_Id_2;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::برنامه_2::Impl>& cv::ogl::برنامه_2::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::برنامه_2::Impl::Impl() : برنامه_Id_2(0), autoRelease_(خطأ)
{
}

cv::ogl::برنامه_2::Impl::Impl(GLuint برنامه_Id, منطقي autoRelease) : برنامه_Id_2(برنامه_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_برنامه(برنامه_Id) == gl::TRUE_ );
}

cv::ogl::برنامه_2::Impl::Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease) : برنامه_Id_2(0), autoRelease_(autoRelease)
{
    برنامه_Id_2 = gl::Create_برنامه();
    CV_CheckGlError();

    CV_Assert( برنامه_Id_2 != 0 );

    gl::AttachShader(برنامه_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(برنامه_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint وضعیت;
    gl::Link_برنامه(برنامه_Id_2);
    gl::GetProgramiv(برنامه_Id_2, gl::LINK_وضعیت, &وضعیت);
    إذا (!وضعیت) {
        String info_log;
        gl::GetProgramiv(برنامه_Id_2, gl::INFO_LOG_درازی, &وضعیت);
        info_log.resize(وضعیت);
        gl::Get_برنامه_Info_Log(برنامه_Id_2, GLsizei(info_log.size()) + 1, اشاره_تهی, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::برنامه_2::Impl::~Impl()
{
    إذا (autoRelease_ && برنامه_Id_2)
        gl::Delete_برنامه(برنامه_Id_2);
}

فراغ cv::ogl::برنامه_2::Impl::bind()
{
    gl::Use_برنامه(برنامه_Id_2);
    CV_CheckGlError();
}

صحيح cv::ogl::برنامه_2::Impl::getAttributeLocation(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::GetAttribLocation(برنامه_Id_2, name);
    CV_CheckGlError();
    أرجع location;
}

صحيح cv::ogl::برنامه_2::Impl::get_یکنواخت_Location(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::Get_یکنواخت_Location(برنامه_Id_2, name);
    CV_CheckGlError();
    أرجع location;
}

#پایان_اگر // HAVE_OPENGL

cv::ogl::برنامه_2::برنامه_2()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#پایان_اگر
}

cv::ogl::برنامه_2::برنامه_2(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(vert); CV_استفادهنشده(frag); CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::create(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(vert); CV_استفادهنشده(frag); CV_استفادهنشده(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::release()
{
#اگر_تعریف_شده HAVE_OPENGL
    إذا (impl_)
        impl_->مجموعه_Auto_Release(درست);
    impl_ = Impl::empty();
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_Auto_Release(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    impl_->مجموعه_Auto_Release(flag);
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::bind() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::unbind()
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::Use_برنامه(0);
#پایان_اگر
}

صحيح cv::ogl::برنامه_2::getAttributeLocation(ثابت حرف* name) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->getAttributeLocation(name);
#پایان_اگر
}

صحيح cv::ogl::برنامه_2::get_یکنواخت_Location(ثابت حرف* name) ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->get_یکنواخت_Location(name);
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_یکنواخت_Vec3(صحيح loc, Vec3f vec)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(loc); CV_استفادهنشده(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_یکنواخت_Vec2(صحيح loc, Vec2f vec)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(loc); CV_استفادهنشده(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform2f(loc, vec(0), vec(1));
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_یکنواخت_Vec4(صحيح loc, Vec4f vec)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(loc); CV_استفادهنشده(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_یکنواخت_Mat4x4(صحيح loc, Matx44f mat)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(loc); CV_استفادهنشده(mat);
    throw_no_ogl();
#وإلا
    gl::یکنواخت_Matrix4fv(loc, 1, درست, mat.val);
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_Uniform1f(صحيح loc, عائم value)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(loc); CV_استفادهنشده(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1f(loc, value);
#پایان_اگر
}

فراغ cv::ogl::برنامه_2::مجموعه_Uniform1i(صحيح loc, صحيح value)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(loc); CV_استفادهنشده(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1i(loc, value);
#پایان_اگر
}

بیعلامت صحيح cv::ogl::برنامه_2::برنامه_Id() ثابت
{
#اگر_تعریف_نشده HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->برنامه_Id();
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// Rendering

فراغ cv::ogl::render(ثابت ogl::Texture2D& tex, Rect_<مزدوج> wndRect, Rect_<مزدوج> texRect)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(tex);
    CV_استفادهنشده(wndRect);
    CV_استفادهنشده(texRect);
    throw_no_ogl();
#وإلا
    إذا (!tex.empty())
    {
        gl::ماتریس_نما(gl::PROJECTION);
        gl::بارگیری_عنصر_خنثی();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::ماتریس_نما(gl::MODELVIEW);
        gl::بارگیری_عنصر_خنثی();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_نما, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::خطی);
        CV_CheckGlError();

        ثابت مزدوج رأس[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.ارتفاع), 0.0,
            wndRect.x + wndRect.عرض, (wndRect.y + wndRect.ارتفاع), 0.0,
            wndRect.x + wndRect.عرض, wndRect.y, 0.0
        };
        ثابت مزدوج texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.ارتفاع,
            texRect.x + texRect.عرض, texRect.y + texRect.ارتفاع,
            texRect.x + texRect.عرض, texRect.y
        };

        ogl::میانگیر_2::unbind(ogl::میانگیر_2::ARRAY_میانگیر);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_اشاره_گر(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::نرمال_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::رأس_ARRAY);
        CV_CheckGlError();

        gl::رأس_اشاره_گر(3, gl::DOUBLE, 0, رأس);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#پایان_اگر
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, صحيح نما, Scalar color)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(نما);
    CV_استفادهنشده(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(نما, 0, arr.size());
    }
#پایان_اگر
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, درون_داد_Array indices, صحيح نما, Scalar color)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(arr);
    CV_استفادهنشده(indices);
    CV_استفادهنشده(نما);
    CV_استفادهنشده(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ثابت صحيح نوع = indices.نوع();

        بدل (نوع)
        {
        حالة درون_داد_Array_2::OPENGL_میانگیر :
            {
                ogl::میانگیر_2 buf = indices.get_OGl_میانگیر();

                ثابت صحيح عمق = buf.عمق();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( عمق <= CV_32S );

                GLenum type;
                إذا (عمق < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (عمق < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::میانگیر_2::ELEMENT_ARRAY_میانگیر);

                gl::DrawElements(نما, buf.size().area(), type, 0);

                ogl::میانگیر_2::unbind(ogl::میانگیر_2::ELEMENT_ARRAY_میانگیر);

                اكسر;
            }

        افتراضي:
            {
                Mat mat = indices.getMat();

                ثابت صحيح عمق = mat.عمق();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( عمق <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                إذا (عمق < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (عمق < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                ogl::میانگیر_2::unbind(ogl::میانگیر_2::ELEMENT_ARRAY_میانگیر);

                gl::DrawElements(نما, mat.size().area(), type, mat.داده);
            }
        }
    }
#پایان_اگر
}

فراغ cv::ogl::clearColor(Scalar color)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(color);
    throw_no_ogl();
#وإلا
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_میانگیر_بیت);
#پایان_اگر // HAVE_OPENGL
}

فراغ cv::ogl::clear_عمق(عائم عمق)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(عمق);
    throw_no_ogl();
#وإلا
    gl::Clear_عمق(عمق);
    gl::Clear(gl::عمق_میانگیر_بیت);
#پایان_اگر // HAVE_OPENGL
}

فراغ cv::ogl::drawArrays(صحيح first, صحيح شمار_2, صحيح نما)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(first); CV_استفادهنشده(شمار_2); CV_استفادهنشده(نما);
    throw_no_ogl();
#وإلا
    gl::DrawArrays(نما, first, شمار_2);
#پایان_اگر // HAVE_OPENGL
}

فراغ cv::ogl::drawElements(صحيح first, صحيح شمار_2, صحيح type, صحيح نما)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(first); CV_استفادهنشده(شمار_2); CV_استفادهنشده(type); CV_استفادهنشده(نما);
    throw_no_ogl();
#وإلا
    gl::DrawElements(نما, شمار_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)));
#پایان_اگر
}

فراغ cv::ogl::drawElementsInstanced(صحيح first, صحيح شمار_2, صحيح type, صحيح نما, صحيح instance_شمار)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(first); CV_استفادهنشده(شمار_2); CV_استفادهنشده(type); CV_استفادهنشده(نما); CV_استفادهنشده(instance_شمار);
    throw_no_ogl();
#وإلا
    gl::DrawElementsInstanced(نما, شمار_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)), instance_شمار);
    CV_CheckGlError();
#پایان_اگر
}

فراغ cv::ogl::blendFunc(صحيح sfactor, صحيح dfactor)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(sfactor); CV_استفادهنشده(dfactor);
    throw_no_ogl();
#وإلا
    gl::BlendFunc(sfactor, dfactor);
#پایان_اگر
}

فراغ cv::ogl::عمق_ماسک_2(منطقي flag)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(flag);
    throw_no_ogl();
#وإلا
    gl::عمق_ماسک(flag ? gl::TRUE_ : gl::FALSE_);
#پایان_اگر
}

فراغ cv::ogl::enable(صحيح cap)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(cap);
    throw_no_ogl();
#وإلا
    gl::Enable(cap);
#پایان_اگر
}

فراغ cv::ogl::disable(صحيح cap)
{
#اگر_تعریف_نشده HAVE_OPENGL
    CV_استفادهنشده(cap);
    throw_no_ogl();
#وإلا
    gl::Disable(cap);
#پایان_اگر
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#اگر_تعریف_شده HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  وإلا
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#وإلا // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#پایان_اگر // HAVE_OPENCL

#إذا defined(HAVE_OPENGL)
#  إذا defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#پایان_اگر // HAVE_OPENGL

نطاق cv { نطاق ogl {

#إذا defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ساكن فراغ checkOpenCLVersion()
{
    استفاده نطاق cv::ocl;
    ثابت Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int وضعیت = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, حجم(platform_id), &platform_id, &sz);
    CV_Assert(وضعیت == CL_SUCCESS && sz == حجم(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    صحيح نسخه_Major_2 = pi.نسخه_Major_2();
    صحيح نسخه_Minor_2 = pi.نسخه_Minor_2();
    إذا (نسخه_Major_2 < 1 || (نسخه_Major_2 == 1 && نسخه_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                نسخه_Major_2, نسخه_Minor_2, pi.name().c_str(), pi.نسخه().c_str())
        );
}
#پایان_اگر

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
    cl_device_id* devices = اشاره_تهی;
    cl_uint dev_استفاده_شده = 0;
    cl_context context = اشاره_تهی;

    cl_int وضعیت = clGetPlatformIDs(0, عدم, &platformsCnt);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", وضعیت));
    إذا (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    وضعیت = clGetPlatformIDs(platformsCnt, &platforms[0], عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", وضعیت));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    منطقي sharingSupported = خطأ;

    لكل (بیعلامت صحيح i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        وضعیت = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, عدم, &devCnt);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", وضعیت));

        حاول {
            devices = جديد cl_device_id[devCnt];

            وضعیت = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, عدم);
            إذا (وضعیت != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", وضعیت));

            لكل (بیعلامت صحيح j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_اندازه;
                وضعیت = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, عدم, &extension_اندازه );
                إذا (وضعیت != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", وضعیت));

                إذا(extension_اندازه > 0)
                {
                    std::string devString(extension_اندازه, '\0');
                    وضعیت = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_اندازه);
                    إذا (وضعیت != CL_SUCCESS)
                        استمر;
                    إذا (extension_اندازه > 0 && devString.size() >= extension_اندازه)
                        devString.resize(extension_اندازه - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    طالما (spacePos != devString.npos) {
                        إذا (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_استفاده_شده = i;
                            sharingSupported = درست;
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
            إذا(devices != اشاره_تهی) {
                حذف[] devices;
            }
            استمر;
        }

        إذا (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#إذا defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_گروه_Obj cgl_Share_گروه = CGLGet_Share_گروه(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_گروه,
                    0
                };
                context = clCreateContext(props, 0,0, عدم, عدم, &ci_Err_رقم);
#وإلاإذا defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_استفاده_شده], عدم, عدم, &وضعیت);
#وإلاإذا defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_استفاده_شده], عدم, عدم, &وضعیت);
#وإلاإذا defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_استفاده_شده], عدم, عدم, &وضعیت);
#پایان_اگر
        }

        إذا (وضعیت != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", وضعیت));
        وإلا
            اكسر;
    }


    cl_platform_id platform = platforms[dev_استفاده_شده];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_استفاده_شده]);
    clReleaseDevice(devices[dev_استفاده_شده]);
    clReleaseContext(context);
    clExecCtx.bind();
    أرجع const_cast<Context&>(clExecCtx.getContext());
#پایان_اگر
}

} // namespace cv::ogl::ocl

فراغ convert_مقصد_GLTexture2_D(درون_داد_Array src, Texture2D& texture)
{
    CV_استفادهنشده(src); CV_استفادهنشده(texture);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    اندازه src_اندازه = src.size();
    CV_Assert(src_اندازه.عرض == (صحيح)texture.تعداد_ستون() && src_اندازه.ارتفاع == (صحيح)texture.ردیفها());

    استفاده نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int وضعیت = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &وضعیت);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", وضعیت));

    cl_mem cl_میانگیر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();
    وضعیت = cl_افزودن_به_صف_Acquire_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", وضعیت));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.تعداد_ستون, (size_t)u.ردیفها, 1};
    وضعیت = cl_افزودن_به_صف_Copy_میانگیر_مقصد_Image(q, cl_میانگیر, clImage, offset, dst_origin, region, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", وضعیت));
    وضعیت = cl_افزودن_به_صف_Release_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", وضعیت));

    وضعیت = cl_پایان(q); // TODO Use events
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", وضعیت));

    وضعیت = clReleaseMemObject(clImage); // TODO RAII
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", وضعیت));
#پایان_اگر
}

فراغ convertFromGLTexture2D(ثابت Texture2D& texture, OutputArray dst)
{
    CV_استفادهنشده(texture); CV_استفادهنشده(dst);
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

    استفاده نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int وضعیت = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &وضعیت);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", وضعیت));

    cl_mem cl_میانگیر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();
    وضعیت = cl_افزودن_به_صف_Acquire_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", وضعیت));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.تعداد_ستون, (size_t)u.ردیفها, 1};
    وضعیت = cl_افزودن_به_صف_Copy_Image_مقصد_میانگیر(q, clImage, cl_میانگیر, src_origin, region, offset, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", وضعیت));
    وضعیت = cl_افزودن_به_صف_Release_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", وضعیت));

    وضعیت = cl_پایان(q); // TODO Use events
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", وضعیت));

    وضعیت = clReleaseMemObject(clImage); // TODO RAII
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", وضعیت));
#پایان_اگر
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ثابت میانگیر_2& میانگیر, AccessFlag accessFlags)
{
    CV_استفادهنشده(میانگیر); CV_استفادهنشده(accessFlags);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    استفاده نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_صف cl_صف = (cl_command_صف)صف::getDefault().ptr();

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

    cl_int وضعیت = 0;
    cl_mem cl_میانگیر = clCreateFromGLBuffer(context, clAccessFlags, میانگیر.bufId(), &وضعیت);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", وضعیت));

    gl::پایان();

    وضعیت = cl_افزودن_به_صف_Acquire_GLObjects(cl_صف, 1, &cl_میانگیر, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", وضعیت));

    size_t گام = میانگیر.تعداد_ستون() * میانگیر.elem_اندازه();
    صحيح ردیفها = میانگیر.ردیفها();
    صحيح تعداد_ستون = میانگیر.تعداد_ستون();
    صحيح type = میانگیر.type();

    UMat u;
    convert_From_میانگیر(cl_میانگیر, گام, ردیفها, تعداد_ستون, type, u);
    أرجع u;
#پایان_اگر
}

فراغ unmapGLBuffer(UMat& u)
{
    CV_استفادهنشده(u);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    استفاده نطاق cv::ocl;
    cl_command_صف cl_صف = (cl_command_صف)صف::getDefault().ptr();

    cl_mem cl_میانگیر = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int وضعیت = cl_افزودن_به_صف_Release_GLObjects(cl_صف, 1, &cl_میانگیر, 0, عدم, عدم);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", وضعیت));

    وضعیت = cl_پایان(cl_صف);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", وضعیت));

    وضعیت = clReleaseMemObject(cl_میانگیر);
    إذا (وضعیت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", وضعیت));
#پایان_اگر
}

}} // namespace cv::ogl
