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
   #تعریف_کړه GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#وإلا
   #تعریف_کړه GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#د_که_پای

#که_تعریف_شوی HAVE_OPENGL
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
#د_که_پای // HAVE_OPENGL

کارول نطاق cv;
کارول نطاق cv::cuda;

#إذا defined(_MSC_VER)
#لارښوونه warning(disable : 4702)  // unreachable code
#د_که_پای

نطاق
{
#که_نه_تعریف_شوی HAVE_OPENGL
مضمن ساكن CV_NORETURN فراغ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#وإلاإذا defined _DEBUG
مضمن ساكن منطقي checkError(ثابت حرف* file, ثابت صحيح کرښه, ثابت حرف* func = 0)
{
    GLenum err = gl::GetError();
    إذا (err != gl::NO_ERROR_)
    {
        ثابت حرف* پیغام;
        بدل (err)
        {
        حالة gl::INVALID_ENUM:
            پیغام = "An unacceptable value is specified for an enumerated argument";
            اكسر;
        حالة gl::INVALID_ارزښت:
            پیغام = "A numeric argument is out of range";
            اكسر;
        حالة gl::INVALID_OPERATION:
            پیغام = "The specified operation is not allowed in the current state";
            اكسر;
        حالة gl::بهر_د_حافظه:
            پیغام = "There is not enough memory left to execute the command";
            اكسر;
        افتراضي:
            پیغام = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, پیغام, file, کرښه);
    }
    أرجع ریښتیا;
}
#د_که_پای // HAVE_OPENGL
} // namespace

#تعریف_کړه CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#که_تعریف_شوی HAVE_OPENGL
نطاق
{
    ثابت GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#د_که_پای

////////////////////////////////////////////////////////////////////////
// setGlDevice

فراغ cv::cuda::سېټ_Gl_Device(صحيح device)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(device);
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        CV_ناکارول_شوی(device);
        throw_no_cuda();
    #وإلا
        cuda_خوندي_Call( cudaGLSetGLDevice(device) );
    #د_که_پای
#د_که_پای
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

        فراغ ثبتوونکی_بفر(GLuint بفر);
        فراغ release();

        فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream = 0);
        فراغ copy_منزل(فراغ* dst, size_t dpitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream = 0);

        فراغ* map(cuda_بهیر_t stream = 0);
        فراغ unmap(cuda_بهیر_t stream = 0);

    خاص:
        cudaGraphicsResource_t resource_;
        GLuint بفر_3;

        صنف GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), بفر_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    فراغ CudaResource::ثبتوونکی_بفر(GLuint بفر)
    {
        CV_DbgAssert( بفر != 0 );

        إذا (بفر_3 == بفر)
            أرجع;

        cudaGraphicsResource_t resource;
        cuda_خوندي_Call( cuda_Graphics_GLRegister_بفر(&resource, بفر, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        بفر_3 = بفر;
    }

    فراغ CudaResource::release()
    {
        إذا (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        بفر_3 = 0;
    }

    صنف CudaResource::GraphicsMapHolder
    {
    عام:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_بهیر_t stream);
        ~GraphicsMapHolder();

        فراغ reset();

    خاص:
        cudaGraphicsResource_t* resource_;
        cuda_بهیر_t بهیر_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_بهیر_t stream) : resource_(resource), بهیر_3(stream)
    {
        إذا (resource_)
            cuda_خوندي_Call( cudaGraphicsMapResources(1, resource_, بهیر_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        إذا (resource_)
            cudaGraphicsUnmapResources(1, resource_, بهیر_3);
    }

    فراغ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    فراغ CudaResource::copyFrom(ثابت فراغ* src, size_t spitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_ناکارول_شوی(h);

        فراغ* dst;
        size_t size;
        cuda_خوندي_Call( cuda_Graphics_Resource_Get_Mapped_اشاره_ګر(&dst, &size, resource_) );

        CV_DbgAssert( پلنوالی * لوړوالی == size );

        إذا (stream == 0)
            cuda_خوندي_Call( cudaMemcpy2D(dst, پلنوالی, src, spitch, پلنوالی, لوړوالی, cuda_Memcpy_Device_منزل_Device) );
        وإلا
            cuda_خوندي_Call( cudaMemcpy2DAsync(dst, پلنوالی, src, spitch, پلنوالی, لوړوالی, cuda_Memcpy_Device_منزل_Device, stream) );
    }

    فراغ CudaResource::copy_منزل(فراغ* dst, size_t dpitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_ناکارول_شوی(h);

        فراغ* src;
        size_t size;
        cuda_خوندي_Call( cuda_Graphics_Resource_Get_Mapped_اشاره_ګر(&src, &size, resource_) );

        CV_DbgAssert( پلنوالی * لوړوالی == size );

        إذا (stream == 0)
            cuda_خوندي_Call( cudaMemcpy2D(dst, dpitch, src, پلنوالی, پلنوالی, لوړوالی, cuda_Memcpy_Device_منزل_Device) );
        وإلا
            cuda_خوندي_Call( cudaMemcpy2DAsync(dst, dpitch, src, پلنوالی, پلنوالی, لوړوالی, cuda_Memcpy_Device_منزل_Device, stream) );
    }

    فراغ* CudaResource::map(cuda_بهیر_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        فراغ* ptr;
        size_t size;
        cuda_خوندي_Call( cuda_Graphics_Resource_Get_Mapped_اشاره_ګر(&ptr, &size, resource_) );

        h.reset();

        أرجع ptr;
    }

    فراغ CudaResource::unmap(cuda_بهیر_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#د_که_پای

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#که_نه_تعریف_شوی HAVE_OPENGL

صنف cv::ogl::بفر_2::Impl
{
};

#وإلا

صنف cv::ogl::بفر_2::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint bufId, منطقي autoRelease);
    Impl(GLsizeiptr size, ثابت GLvoid* معلومات, GLenum target, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLenum target) ثابت;

    فراغ copyFrom(GLuint srcBuf, GLsizeiptr size);

    فراغ copyFrom(GLsizeiptr size, ثابت GLvoid* معلومات);
    فراغ copy_منزل(GLsizeiptr size, GLvoid* معلومات) ثابت;

    فراغ resize(GLsizeiptr size, GLenum target);

    فراغ* mapHost(GLenum access);
    فراغ unmapHost();

#که_تعریف_شوی HAVE_CUDA
    فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream = 0);
    فراغ copy_منزل(فراغ* dst, size_t dpitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream = 0) ثابت;

    فراغ* mapDevice(cuda_بهیر_t stream = 0);
    فراغ unmapDevice(cuda_بهیر_t stream = 0);
#د_که_پای

    فراغ سېټ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint bufId() ثابت { أرجع bufId_; }

خاص:
    Impl();

    GLuint bufId_;
    منطقي autoRelease_;

#که_تعریف_شوی HAVE_CUDA
    بدلیدونکی CudaResource cudaResource_;
#د_که_پای
};

ثابت Ptr<cv::ogl::بفر_2::Impl>& cv::ogl::بفر_2::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::بفر_2::Impl::Impl() : bufId_(0), autoRelease_(خطأ)
{
}

cv::ogl::بفر_2::Impl::Impl(GLuint abufId, منطقي autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_بفر(abufId) == gl::TRUE_ );
}

cv::ogl::بفر_2::Impl::Impl(GLsizeiptr size, ثابت GLvoid* معلومات, GLenum target, منطقي autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();

    gl::بفر_معلومات(target, size, معلومات, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
}

cv::ogl::بفر_2::Impl::~Impl()
{
    إذا (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

فراغ cv::ogl::بفر_2::Impl::bind(GLenum target) ثابت
{
    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_بفر(gl::COPY_WRITE_بفر, bufId_);
    CV_CheckGlError();

    gl::Bind_بفر(gl::COPY_READ_بفر, srcBuf);
    CV_CheckGlError();

    gl::Copy_بفر_Sub_معلومات(gl::COPY_READ_بفر, gl::COPY_WRITE_بفر, 0, 0, size);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::copyFrom(GLsizeiptr size, ثابت GLvoid* معلومات)
{
    gl::Bind_بفر(gl::COPY_WRITE_بفر, bufId_);
    CV_CheckGlError();

    gl::بفر_Sub_معلومات(gl::COPY_WRITE_بفر, 0, size, معلومات);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::بفر_معلومات(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::copy_منزل(GLsizeiptr size, GLvoid* معلومات) ثابت
{
    gl::Bind_بفر(gl::COPY_READ_بفر, bufId_);
    CV_CheckGlError();

    gl::Get_بفر_Sub_معلومات(gl::COPY_READ_بفر, 0, size, معلومات);
    CV_CheckGlError();
}

فراغ* cv::ogl::بفر_2::Impl::mapHost(GLenum access)
{
    gl::Bind_بفر(gl::COPY_READ_بفر, bufId_);
    CV_CheckGlError();

    GLvoid* معلومات = gl::Map_بفر(gl::COPY_READ_بفر, access);
    CV_CheckGlError();

    أرجع معلومات;
}

فراغ cv::ogl::بفر_2::Impl::unmapHost()
{
    gl::Unmap_بفر(gl::COPY_READ_بفر);
}

#که_تعریف_شوی HAVE_CUDA

فراغ cv::ogl::بفر_2::Impl::copyFrom(ثابت فراغ* src, size_t spitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream)
{
    cudaResource_.ثبتوونکی_بفر(bufId_);
    cudaResource_.copyFrom(src, spitch, پلنوالی, لوړوالی, stream);
}

فراغ cv::ogl::بفر_2::Impl::copy_منزل(فراغ* dst, size_t dpitch, size_t پلنوالی, size_t لوړوالی, cuda_بهیر_t stream) ثابت
{
    cudaResource_.ثبتوونکی_بفر(bufId_);
    cudaResource_.copy_منزل(dst, dpitch, پلنوالی, لوړوالی, stream);
}

فراغ* cv::ogl::بفر_2::Impl::mapDevice(cuda_بهیر_t stream)
{
    cudaResource_.ثبتوونکی_بفر(bufId_);
    أرجع cudaResource_.map(stream);
}

فراغ cv::ogl::بفر_2::Impl::unmapDevice(cuda_بهیر_t stream)
{
    cudaResource_.unmap(stream);
}

#د_که_پای // HAVE_CUDA

#د_که_پای // HAVE_OPENGL

cv::ogl::بفر_2::بفر_2() : لیکې_2(0), ستون_شمېر_2(0), type_(0)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#د_که_پای
}

cv::ogl::بفر_2::بفر_2(صحيح arows, صحيح acols, صحيح atype, بینښه صحيح abufId, منطقي autoRelease) : لیکې_2(0), ستون_شمېر_2(0), type_(0)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arows);
    CV_ناکارول_شوی(acols);
    CV_ناکارول_شوی(atype);
    CV_ناکارول_شوی(abufId);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    لیکې_2 = arows;
    ستون_شمېر_2 = acols;
    type_ = atype;
#د_که_پای
}

cv::ogl::بفر_2::بفر_2(اندازه asize, صحيح atype, بینښه صحيح abufId, منطقي autoRelease) : لیکې_2(0), ستون_شمېر_2(0), type_(0)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(asize);
    CV_ناکارول_شوی(atype);
    CV_ناکارول_شوی(abufId);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    لیکې_2 = asize.لوړوالی;
    ستون_شمېر_2 = asize.پلنوالی;
    type_ = atype;
#د_که_پای
}

cv::ogl::بفر_2::بفر_2(ننوت_Array arr, موخه target, منطقي autoRelease) : لیکې_2(0), ستون_شمېر_2(0), type_(0)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(target);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح ډول = arr.ډول();

    بدل (ډول)
    {
    حالة ننوت_Array_2::OPENGL_بفر:
    حالة ننوت_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        اكسر;

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ثابت GLsizeiptr asize = mat.لیکې * mat.ستون_شمېر * mat.elem_اندازه();
            impl_.reset(جديد Impl(asize, mat.معلومات, target, autoRelease));
            لیکې_2 = mat.لیکې;
            ستون_شمېر_2 = mat.ستون_شمېر;
            type_ = mat.type();
            اكسر;
        }
    }
#د_که_پای
}

فراغ cv::ogl::بفر_2::create(صحيح arows, صحيح acols, صحيح atype, موخه target, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arows);
    CV_ناکارول_شوی(acols);
    CV_ناکارول_شوی(atype);
    CV_ناکارول_شوی(target);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (لیکې_2 != arows || ستون_شمېر_2 != acols || type_ != atype)
    {
        ثابت GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_اندازه(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        إذا (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->سېټ_Auto_Release(autoRelease);
        }
        وإلا
            impl_.reset(جديد Impl(asize, 0, target, autoRelease));
        لیکې_2 = arows;
        ستون_شمېر_2 = acols;
        type_ = atype;
    }
#د_که_پای
}

فراغ cv::ogl::بفر_2::release()
{
#که_تعریف_شوی HAVE_OPENGL
    إذا (impl_)
        impl_->سېټ_Auto_Release(ریښتیا);
    impl_ = Impl::empty();
    لیکې_2 = 0;
    ستون_شمېر_2 = 0;
    type_ = 0;
#د_که_پای
}

فراغ cv::ogl::بفر_2::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    impl_->سېټ_Auto_Release(flag);
#د_که_پای
}

فراغ cv::ogl::بفر_2::copyFrom(ننوت_Array arr, موخه target, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(target);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح ډول = arr.ډول();

    ثابت اندازه asize = arr.size();
    ثابت صحيح atype = arr.type();
    create(asize, atype, target, autoRelease);

    بدل (ډول)
    {
    حالة ننوت_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_اندازه(atype));
            اكسر;
        }

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        {
            #که_نه_تعریف_شوی HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.معلومات, dmat.ګام, dmat.ستون_شمېر * dmat.elem_اندازه(), dmat.لیکې);
            #د_که_پای

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_اندازه(atype), mat.معلومات);
        }
    }
#د_که_پای
}

فراغ cv::ogl::بفر_2::copyFrom(ننوت_Array arr, cuda::بهیر& stream, موخه target, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(stream);
    CV_ناکارول_شوی(target);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        CV_ناکارول_شوی(arr);
        CV_ناکارول_شوی(stream);
        CV_ناکارول_شوی(target);
        CV_ناکارول_شوی(autoRelease);
        throw_no_cuda();
    #وإلا
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.معلومات, dmat.ګام, dmat.ستون_شمېر * dmat.elem_اندازه(), dmat.لیکې, cuda::بهیر_Accessor::get_بهیر(stream));
    #د_که_پای
#د_که_پای
}

فراغ cv::ogl::بفر_2::copy_منزل(OutputArray arr) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    throw_no_ogl();
#وإلا
    ثابت صحيح ډول = arr.ډول();

    بدل (ډول)
    {
    حالة ننوت_Array_2::OPENGL_بفر:
        {
            arr.get_OGl_بفر_Ref().copyFrom(*هذا);
            اكسر;
        }

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        {
            #که_نه_تعریف_شوی HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(لیکې_2, ستون_شمېر_2, type_);
                impl_->copy_منزل(dmat.معلومات, dmat.ګام, dmat.ستون_شمېر * dmat.elem_اندازه(), dmat.لیکې);
            #د_که_پای

            اكسر;
        }

    افتراضي:
        {
            arr.create(لیکې_2, ستون_شمېر_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_منزل(mat.لیکې * mat.ستون_شمېر * mat.elem_اندازه(), mat.معلومات);
        }
    }
#د_که_پای
}

فراغ cv::ogl::بفر_2::copy_منزل(OutputArray arr, cuda::بهیر& stream) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(stream);
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        CV_ناکارول_شوی(arr);
        CV_ناکارول_شوی(stream);
        throw_no_cuda();
    #وإلا
        arr.create(لیکې_2, ستون_شمېر_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_منزل(dmat.معلومات, dmat.ګام, dmat.ستون_شمېر * dmat.elem_اندازه(), dmat.لیکې, cuda::بهیر_Accessor::get_بهیر(stream));
    #د_که_پای
#د_که_پای
}

cv::ogl::بفر_2 cv::ogl::بفر_2::clone(موخه target, منطقي autoRelease) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(target);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    ogl::بفر_2 buf;
    buf.copyFrom(*هذا, target, autoRelease);
    أرجع buf;
#د_که_پای
}

فراغ cv::ogl::بفر_2::bind(موخه target) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(target);
    throw_no_ogl();
#وإلا
    impl_->bind(target);
#د_که_پای
}

فراغ cv::ogl::بفر_2::unbind(موخه target)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(target);
    throw_no_ogl();
#وإلا
    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
#د_که_پای
}

Mat cv::ogl::بفر_2::mapHost(Access access)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(access);
    throw_no_ogl();
#وإلا
    أرجع Mat(لیکې_2, ستون_شمېر_2, type_, impl_->mapHost(access));
#د_که_پای
}

فراغ cv::ogl::بفر_2::unmapHost()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->unmapHost();
#د_که_پای
}

GpuMat cv::ogl::بفر_2::mapDevice()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(لیکې_2, ستون_شمېر_2, type_, impl_->mapDevice());
    #د_که_پای
#د_که_پای
}

فراغ cv::ogl::بفر_2::unmapDevice()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice();
    #د_که_پای
#د_که_پای
}

cuda::GpuMat cv::ogl::بفر_2::mapDevice(cuda::بهیر& stream)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(stream);
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        CV_ناکارول_شوی(stream);
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(لیکې_2, ستون_شمېر_2, type_, impl_->mapDevice(cuda::بهیر_Accessor::get_بهیر(stream)));
    #د_که_پای
#د_که_پای
}

فراغ cv::ogl::بفر_2::unmapDevice(cuda::بهیر& stream)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(stream);
    throw_no_ogl();
#وإلا
    #که_نه_تعریف_شوی HAVE_CUDA
        CV_ناکارول_شوی(stream);
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice(cuda::بهیر_Accessor::get_بهیر(stream));
    #د_که_پای
#د_که_پای
}

بینښه صحيح cv::ogl::بفر_2::bufId() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->bufId();
#د_که_پای
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#که_نه_تعریف_شوی HAVE_OPENGL

صنف cv::ogl::Texture2D::Impl
{
};

#وإلا

صنف cv::ogl::Texture2D::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl(GLuint texId, منطقي autoRelease);
    Impl(GLint دننی_Format, GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease);
    ~Impl();

    فراغ copyFrom(GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLenum type, ثابت GLvoid *pixels);
    فراغ copy_منزل(GLenum format, GLenum type, GLvoid* pixels) ثابت;

    فراغ bind() ثابت;

    فراغ سېټ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

cv::ogl::Texture2D::Impl::Impl(GLint دننی_Format, GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, دننی_Format, پلنوالی, لوړوالی, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::Texture2D::Impl::copyFrom(GLsizei پلنوالی, GLsizei لوړوالی, GLenum format, GLenum type, ثابت GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, پلنوالی, لوړوالی, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

فراغ cv::ogl::Texture2D::Impl::copy_منزل(GLenum format, GLenum type, GLvoid* pixels) ثابت
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

#د_که_پای // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : لیکې_2(0), ستون_شمېر_2(0), format_(NONE)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#د_که_پای
}

cv::ogl::Texture2D::Texture2D(صحيح arows, صحيح acols, Format aformat, بینښه صحيح atexId, منطقي autoRelease) : لیکې_2(0), ستون_شمېر_2(0), format_(NONE)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arows);
    CV_ناکارول_شوی(acols);
    CV_ناکارول_شوی(aformat);
    CV_ناکارول_شوی(atexId);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    لیکې_2 = arows;
    ستون_شمېر_2 = acols;
    format_ = aformat;
#د_که_پای
}

cv::ogl::Texture2D::Texture2D(اندازه asize, Format aformat, بینښه صحيح atexId, منطقي autoRelease) : لیکې_2(0), ستون_شمېر_2(0), format_(NONE)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(asize);
    CV_ناکارول_شوی(aformat);
    CV_ناکارول_شوی(atexId);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    لیکې_2 = asize.لوړوالی;
    ستون_شمېر_2 = asize.پلنوالی;
    format_ = aformat;
#د_که_پای
}

cv::ogl::Texture2D::Texture2D(ننوت_Array arr, منطقي autoRelease) : لیکې_2(0), ستون_شمېر_2(0), format_(NONE)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح ډول = arr.ډول();

    ثابت اندازه asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح ژوروالی = CV_MAT_ژوروالی(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

    CV_Assert( ژوروالی <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format دننی_Formats[] =
    {
        NONE, ژوروالی_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::ژوروالی_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    بدل (ډول)
    {
    حالة ننوت_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_.reset(جديد Impl(دننی_Formats[cn], asize.پلنوالی, asize.لوړوالی, srcFormats[cn], gl_types[ژوروالی], 0, autoRelease));
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            اكسر;
        }

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        {
            #که_نه_تعریف_شوی HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::بفر_2 buf(dmat, ogl::بفر_2::PIXEL_UNPACK_بفر);
                buf.سېټ_Auto_Release(ریښتیا);
                buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
                impl_.reset(جديد Impl(دننی_Formats[cn], asize.پلنوالی, asize.لوړوالی, srcFormats[cn], gl_types[ژوروالی], 0, autoRelease));
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            #د_که_پای

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_.reset(جديد Impl(دننی_Formats[cn], asize.پلنوالی, asize.لوړوالی, srcFormats[cn], gl_types[ژوروالی], mat.معلومات, autoRelease));
            اكسر;
        }
    }

    لیکې_2 = asize.لوړوالی;
    ستون_شمېر_2 = asize.پلنوالی;
    format_ = دننی_Formats[cn];
#د_که_پای
}

فراغ cv::ogl::Texture2D::create(صحيح arows, صحيح acols, Format aformat, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arows);
    CV_ناکارول_شوی(acols);
    CV_ناکارول_شوی(aformat);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (لیکې_2 != arows || ستون_شمېر_2 != acols || format_ != aformat)
    {
        ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
        impl_.reset(جديد Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        لیکې_2 = arows;
        ستون_شمېر_2 = acols;
        format_ = aformat;
    }
#د_که_پای
}

فراغ cv::ogl::Texture2D::release()
{
#که_تعریف_شوی HAVE_OPENGL
    إذا (impl_)
        impl_->سېټ_Auto_Release(ریښتیا);
    impl_ = Impl::empty();
    لیکې_2 = 0;
    ستون_شمېر_2 = 0;
    format_ = NONE;
#د_که_پای
}

فراغ cv::ogl::Texture2D::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    impl_->سېټ_Auto_Release(flag);
#د_که_پای
}

فراغ cv::ogl::Texture2D::copyFrom(ننوت_Array arr, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح ډول = arr.ډول();

    ثابت اندازه asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح ژوروالی = CV_MAT_ژوروالی(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

    CV_Assert( ژوروالی <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format دننی_Formats[] =
    {
        NONE, ژوروالی_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::ژوروالی_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, دننی_Formats[cn], autoRelease);

    بدل(ډول)
    {
    حالة ننوت_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_->copyFrom(asize.پلنوالی, asize.لوړوالی, srcFormats[cn], gl_types[ژوروالی], 0);
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            اكسر;
        }

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        {
            #که_نه_تعریف_شوی HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::بفر_2 buf(dmat, ogl::بفر_2::PIXEL_UNPACK_بفر);
                buf.سېټ_Auto_Release(ریښتیا);
                buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
                impl_->copyFrom(asize.پلنوالی, asize.لوړوالی, srcFormats[cn], gl_types[ژوروالی], 0);
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            #د_که_پای

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_->copyFrom(asize.پلنوالی, asize.لوړوالی, srcFormats[cn], gl_types[ژوروالی], mat.معلومات);
        }
    }
#د_که_پای
}

فراغ cv::ogl::Texture2D::copy_منزل(OutputArray arr, صحيح ddepth, منطقي autoRelease) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(ddepth);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح ډول = arr.ډول();

    ثابت صحيح cn = format_ == ژوروالی_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ثابت GLenum dstFormat = format_ == ژوروالی_COMPONENT ? gl::ژوروالی_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    بدل(ډول)
    {
    حالة ننوت_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2& buf = arr.get_OGl_بفر_Ref();
            buf.create(لیکې_2, ستون_شمېر_2, CV_MAKE_TYPE(ddepth, cn), ogl::بفر_2::PIXEL_PACK_بفر, autoRelease);
            buf.bind(ogl::بفر_2::PIXEL_PACK_بفر);
            impl_->copy_منزل(dstFormat, gl_types[ddepth], 0);
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
            اكسر;
        }

    حالة ننوت_Array_2::CUDA_GPU_MAT:
        {
            #که_نه_تعریف_شوی HAVE_CUDA
                throw_no_cuda();
            #وإلا
                ogl::بفر_2 buf(لیکې_2, ستون_شمېر_2, CV_MAKE_TYPE(ddepth, cn), ogl::بفر_2::PIXEL_PACK_بفر);
                buf.سېټ_Auto_Release(ریښتیا);
                buf.bind(ogl::بفر_2::PIXEL_PACK_بفر);
                impl_->copy_منزل(dstFormat, gl_types[ddepth], 0);
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
                buf.copy_منزل(arr);
            #د_که_پای

            اكسر;
        }

    افتراضي:
        {
            arr.create(لیکې_2, ستون_شمېر_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
            impl_->copy_منزل(dstFormat, gl_types[ddepth], mat.معلومات);
        }
    }
#د_که_پای
}

فراغ cv::ogl::Texture2D::bind() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#د_که_پای
}

بینښه صحيح cv::ogl::Texture2D::texId() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#د_که_پای
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#که_نه_تعریف_شوی HAVE_OPENGL

صنف cv::ogl::Texture_بفر::Impl
{
};

#وإلا

صنف cv::ogl::Texture_بفر::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl();
    Impl(GLenum دننی_Format, GLuint bufId, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLint unit) ثابت;

    فراغ سېټ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { أرجع texId_; }

خاص:
    GLuint texId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::Texture_بفر::Impl> cv::ogl::Texture_بفر::Impl::empty()
{
    ساكن Ptr<Impl> p = makePtr<Impl>();
    أرجع p;
}

cv::ogl::Texture_بفر::Impl::Impl() : texId_(0), autoRelease_(خطأ)
{
}

cv::ogl::Texture_بفر::Impl::Impl(GLenum دننی_Format, GLuint bufId, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_بفر, texId_);
    CV_CheckGlError();

    gl::Tex_بفر(gl::TEXTURE_بفر, دننی_Format, bufId);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_بفر, 0);
    CV_CheckGlError();
}

cv::ogl::Texture_بفر::Impl::~Impl()
{
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::Texture_بفر::Impl::bind(GLint unit) ثابت
{
    gl::فعال_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_بفر, texId_);
    CV_CheckGlError();
}

#د_که_پای // HAVE_OPENGL

cv::ogl::Texture_بفر::Texture_بفر() : format_(NONE)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#د_که_پای
}

cv::ogl::Texture_بفر::Texture_بفر(ثابت بفر_2& buf, Format aformat, منطقي autoRelease) : format_(NONE)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(buf);
    CV_ناکارول_شوی(aformat);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#د_که_پای
}

فراغ cv::ogl::Texture_بفر::create(ثابت بفر_2& buf, Format aformat, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(buf);
    CV_ناکارول_شوی(aformat);
    CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#د_که_پای
}

فراغ cv::ogl::Texture_بفر::release()
{
#که_تعریف_شوی HAVE_OPENGL
    إذا (impl_)
        impl_->سېټ_Auto_Release(ریښتیا);
    impl_ = Impl::empty();
#د_که_پای
    buf_ = بفر_2();
    format_ = NONE;
}

فراغ cv::ogl::Texture_بفر::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    impl_->سېټ_Auto_Release(flag);
#د_که_پای
}

فراغ cv::ogl::Texture_بفر::bind(صحيح unit) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(unit);
    throw_no_ogl();
#وإلا
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#د_که_پای
}

cv::ogl::Texture_بفر::Format cv::ogl::Texture_بفر::format() ثابت
{
    أرجع format_;
}

منطقي cv::ogl::Texture_بفر::empty() ثابت
{
    أرجع format_ == NONE;
}

بینښه صحيح cv::ogl::Texture_بفر::texId() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#د_که_پای
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

فراغ cv::ogl::Arrays::سېټ_راس_Array(ننوت_Array راس)
{
    ثابت صحيح cn = راس.channels();
    ثابت صحيح ژوروالی = راس.ژوروالی();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( ژوروالی == CV_16S || ژوروالی == CV_32S || ژوروالی == CV_32F || ژوروالی == CV_64F );

    إذا (راس.ډول() == ننوت_Array_2::OPENGL_بفر)
        راس_2 = راس.get_OGl_بفر();
    وإلا
        راس_2.copyFrom(راس);

    اندازه_3 = راس_2.size().area();
}

فراغ cv::ogl::Arrays::reset_راس_Array()
{
    راس_2.release();
    اندازه_3 = 0;
}

فراغ cv::ogl::Arrays::سېټ_Color_Array(ننوت_Array color)
{
    ثابت صحيح cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    إذا (color.ډول() == ننوت_Array_2::OPENGL_بفر)
        color_ = color.get_OGl_بفر();
    وإلا
        color_.copyFrom(color);
}

فراغ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

فراغ cv::ogl::Arrays::سېټ_نورمال_Array(ننوت_Array نورمال)
{
    ثابت صحيح cn = نورمال.channels();
    ثابت صحيح ژوروالی = نورمال.ژوروالی();

    CV_Assert( cn == 3 );
    CV_Assert( ژوروالی == CV_8S || ژوروالی == CV_16S || ژوروالی == CV_32S || ژوروالی == CV_32F || ژوروالی == CV_64F );

    إذا (نورمال.ډول() == ننوت_Array_2::OPENGL_بفر)
        نورمال_2 = نورمال.get_OGl_بفر();
    وإلا
        نورمال_2.copyFrom(نورمال);
}

فراغ cv::ogl::Arrays::reset_نورمال_Array()
{
    نورمال_2.release();
}

فراغ cv::ogl::Arrays::سېټ_Tex_Coord_Array(ننوت_Array texCoord)
{
    ثابت صحيح cn = texCoord.channels();
    ثابت صحيح ژوروالی = texCoord.ژوروالی();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( ژوروالی == CV_16S || ژوروالی == CV_32S || ژوروالی == CV_32F || ژوروالی == CV_64F );

    إذا (texCoord.ډول() == ننوت_Array_2::OPENGL_بفر)
        texCoord_ = texCoord.get_OGl_بفر();
    وإلا
        texCoord_.copyFrom(texCoord);
}

فراغ cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

فراغ cv::ogl::Arrays::release()
{
    reset_راس_Array();
    resetColorArray();
    reset_نورمال_Array();
    resetTexCoordArray();
}

فراغ cv::ogl::Arrays::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    راس_2.سېټ_Auto_Release(flag);
    color_.سېټ_Auto_Release(flag);
    نورمال_2.سېټ_Auto_Release(flag);
    texCoord_.سېټ_Auto_Release(flag);
#د_که_پای
}

فراغ cv::ogl::Arrays::bind() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == اندازه_3 );
    CV_Assert( نورمال_2.empty() || نورمال_2.size().area() == اندازه_3 );
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

        texCoord_.bind(ogl::بفر_2::ARRAY_بفر);

        gl::Tex_Coord_اشاره_ګر(texCoord_.channels(), gl_types[texCoord_.ژوروالی()], 0, 0);
        CV_CheckGlError();
    }

    إذا (نورمال_2.empty())
    {
        gl::DisableClientState(gl::نورمال_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::نورمال_ARRAY);
        CV_CheckGlError();

        نورمال_2.bind(ogl::بفر_2::ARRAY_بفر);

        gl::نورمال_اشاره_ګر(gl_types[نورمال_2.ژوروالی()], 0, 0);
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

        color_.bind(ogl::بفر_2::ARRAY_بفر);

        ثابت صحيح cn = color_.channels();

        gl::Color_اشاره_ګر(cn, gl_types[color_.ژوروالی()], 0, 0);
        CV_CheckGlError();
    }

    إذا (راس_2.empty())
    {
        gl::DisableClientState(gl::راس_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::راس_ARRAY);
        CV_CheckGlError();

        راس_2.bind(ogl::بفر_2::ARRAY_بفر);

        gl::راس_اشاره_ګر(راس_2.channels(), gl_types[راس_2.ژوروالی()], 0, 0);
        CV_CheckGlError();
    }

    ogl::بفر_2::unbind(ogl::بفر_2::ARRAY_بفر);
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#که_نه_تعریف_شوی HAVE_OPENGL

صنف cv::ogl::راس_Array::Impl
{
};

#وإلا

صنف cv::ogl::راس_Array::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint vaId, منطقي autoRelease);
    Impl(std::initializer_list<Attribute> ځانګړتیاوې, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    فراغ سېټ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint vaId() ثابت { أرجع vaId_; }

خاص:
    Impl();

    GLuint vaId_;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::راس_Array::Impl>& cv::ogl::راس_Array::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::راس_Array::Impl::Impl() : vaId_(0), autoRelease_(خطأ)
{
}

cv::ogl::راس_Array::Impl::Impl(GLuint vaId, منطقي autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_راس_Array(vaId) == gl::TRUE_ );
}

cv::ogl::راس_Array::Impl::Impl(std::initializer_list<Attribute> ځانګړتیاوې, منطقي autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_راس_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_راس_Array(vaId_);
    CV_CheckGlError();

    لكل (خپلکاری& attribute : ځانګړتیاوې)
    {
        attribute.بفر_3.bind(بفر_2::ARRAY_بفر);
        إذا (attribute.integer_)
        {
            gl::راس_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.اندازه_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.دننه_واټن)
            );
            CV_CheckGlError();
        }
        وإلا
        {
            gl::راس_Attrib_اشاره_ګر(
                attribute.shader_loc_,
                attribute.اندازه_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.دننه_واټن)
            );
            CV_CheckGlError();
        }

        gl::Enable_راس_Attrib_Array(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::راس_Array::Impl::~Impl()
{
    إذا (autoRelease_ && vaId_)
        gl::Delete_راس_Arrays(1, &vaId_);
}

فراغ cv::ogl::راس_Array::Impl::bind()
{
    gl::Bind_راس_Array(vaId_);
    CV_CheckGlError();
}

#د_که_پای // HAVE_OPENGL

cv::ogl::راس_Array::راس_Array()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#د_که_پای
}

cv::ogl::راس_Array::راس_Array(std::initializer_list<Attribute> ځانګړتیاوې, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(ځانګړتیاوې); CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(ځانګړتیاوې, autoRelease));
#د_که_پای
}

فراغ cv::ogl::راس_Array::create(std::initializer_list<Attribute> ځانګړتیاوې, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(ځانګړتیاوې); CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(ځانګړتیاوې, autoRelease));
#د_که_پای
}

فراغ cv::ogl::راس_Array::release()
{
#که_تعریف_شوی HAVE_OPENGL
    إذا (impl_)
        impl_->سېټ_Auto_Release(ریښتیا);
    impl_ = Impl::empty();
#د_که_پای
}

فراغ cv::ogl::راس_Array::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    impl_->سېټ_Auto_Release(flag);
#د_که_پای
}

فراغ cv::ogl::راس_Array::bind() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#د_که_پای
}

فراغ cv::ogl::راس_Array::unbind()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::Bind_راس_Array(0);
#د_که_پای
}

بینښه صحيح cv::ogl::راس_Array::vaId() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->vaId();
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#که_نه_تعریف_شوی HAVE_OPENGL

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

    فراغ سېټ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

    GLint حالت;
    gl::Shader_سرچینه(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_حالت, &حالت);
    إذا (!حالت) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_اوږدتیا, &حالت);
        info_log.resize(حالت);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, تش_اشاره, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    إذا (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#د_که_پای // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::راس_3)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#د_که_پای
}

cv::ogl::Shader::Shader(ثابت حرف* src, ShaderType type, منطقي autoRelease) : type_(type)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(src); CV_ناکارول_شوی(type); CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
#د_که_پای
}

فراغ cv::ogl::Shader::create(ثابت حرف* src, ShaderType type, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(src); CV_ناکارول_شوی(type); CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
    type_ = type;
#د_که_پای
}

فراغ cv::ogl::Shader::release()
{
#که_تعریف_شوی HAVE_OPENGL
    إذا (impl_)
        impl_->سېټ_Auto_Release(ریښتیا);
    impl_ = Impl::empty();
#د_که_پای
}

فراغ cv::ogl::Shader::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    impl_->سېټ_Auto_Release(flag);
#د_که_پای
}

بینښه صحيح cv::ogl::Shader::shaderId() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->shaderId();
#د_که_پای
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ثابت
{
    أرجع type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#که_نه_تعریف_شوی HAVE_OPENGL

صنف cv::ogl::پروګرام_2::Impl
{
};

#وإلا

صنف cv::ogl::پروګرام_2::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint پروګرام_Id, منطقي autoRelease);
    Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    صحيح getAttributeLocation(ثابت حرف* name) ثابت;
    صحيح get_یوشان_Location(ثابت حرف* name) ثابت;

    فراغ سېټ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint پروګرام_Id() ثابت { أرجع پروګرام_Id_2; }

خاص:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint پروګرام_Id_2;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::پروګرام_2::Impl>& cv::ogl::پروګرام_2::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::پروګرام_2::Impl::Impl() : پروګرام_Id_2(0), autoRelease_(خطأ)
{
}

cv::ogl::پروګرام_2::Impl::Impl(GLuint پروګرام_Id, منطقي autoRelease) : پروګرام_Id_2(پروګرام_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_پروګرام(پروګرام_Id) == gl::TRUE_ );
}

cv::ogl::پروګرام_2::Impl::Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease) : پروګرام_Id_2(0), autoRelease_(autoRelease)
{
    پروګرام_Id_2 = gl::Create_پروګرام();
    CV_CheckGlError();

    CV_Assert( پروګرام_Id_2 != 0 );

    gl::AttachShader(پروګرام_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(پروګرام_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint حالت;
    gl::Link_پروګرام(پروګرام_Id_2);
    gl::GetProgramiv(پروګرام_Id_2, gl::LINK_حالت, &حالت);
    إذا (!حالت) {
        String info_log;
        gl::GetProgramiv(پروګرام_Id_2, gl::INFO_LOG_اوږدتیا, &حالت);
        info_log.resize(حالت);
        gl::Get_پروګرام_Info_Log(پروګرام_Id_2, GLsizei(info_log.size()) + 1, تش_اشاره, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::پروګرام_2::Impl::~Impl()
{
    إذا (autoRelease_ && پروګرام_Id_2)
        gl::Delete_پروګرام(پروګرام_Id_2);
}

فراغ cv::ogl::پروګرام_2::Impl::bind()
{
    gl::Use_پروګرام(پروګرام_Id_2);
    CV_CheckGlError();
}

صحيح cv::ogl::پروګرام_2::Impl::getAttributeLocation(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::GetAttribLocation(پروګرام_Id_2, name);
    CV_CheckGlError();
    أرجع location;
}

صحيح cv::ogl::پروګرام_2::Impl::get_یوشان_Location(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::Get_یوشان_Location(پروګرام_Id_2, name);
    CV_CheckGlError();
    أرجع location;
}

#د_که_پای // HAVE_OPENGL

cv::ogl::پروګرام_2::پروګرام_2()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#د_که_پای
}

cv::ogl::پروګرام_2::پروګرام_2(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(vert); CV_ناکارول_شوی(frag); CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::create(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(vert); CV_ناکارول_شوی(frag); CV_ناکارول_شوی(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::release()
{
#که_تعریف_شوی HAVE_OPENGL
    إذا (impl_)
        impl_->سېټ_Auto_Release(ریښتیا);
    impl_ = Impl::empty();
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_Auto_Release(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    impl_->سېټ_Auto_Release(flag);
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::bind() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::unbind()
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::Use_پروګرام(0);
#د_که_پای
}

صحيح cv::ogl::پروګرام_2::getAttributeLocation(ثابت حرف* name) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->getAttributeLocation(name);
#د_که_پای
}

صحيح cv::ogl::پروګرام_2::get_یوشان_Location(ثابت حرف* name) ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->get_یوشان_Location(name);
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_یوشان_Vec3(صحيح loc, Vec3f vec)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(loc); CV_ناکارول_شوی(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_یوشان_Vec2(صحيح loc, Vec2f vec)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(loc); CV_ناکارول_شوی(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform2f(loc, vec(0), vec(1));
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_یوشان_Vec4(صحيح loc, Vec4f vec)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(loc); CV_ناکارول_شوی(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_یوشان_Mat4x4(صحيح loc, Matx44f mat)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(loc); CV_ناکارول_شوی(mat);
    throw_no_ogl();
#وإلا
    gl::یوشان_Matrix4fv(loc, 1, ریښتیا, mat.val);
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_Uniform1f(صحيح loc, عائم value)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(loc); CV_ناکارول_شوی(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1f(loc, value);
#د_که_پای
}

فراغ cv::ogl::پروګرام_2::سېټ_Uniform1i(صحيح loc, صحيح value)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(loc); CV_ناکارول_شوی(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1i(loc, value);
#د_که_پای
}

بینښه صحيح cv::ogl::پروګرام_2::پروګرام_Id() ثابت
{
#که_نه_تعریف_شوی HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->پروګرام_Id();
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// Rendering

فراغ cv::ogl::render(ثابت ogl::Texture2D& tex, Rect_<مزدوج> wndRect, Rect_<مزدوج> texRect)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(tex);
    CV_ناکارول_شوی(wndRect);
    CV_ناکارول_شوی(texRect);
    throw_no_ogl();
#وإلا
    إذا (!tex.empty())
    {
        gl::ماتریکس_ډېرپېښ(gl::PROJECTION);
        gl::بارول_بې_اغېزه_عنصر();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::ماتریکس_ډېرپېښ(gl::MODELVIEW);
        gl::بارول_بې_اغېزه_عنصر();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_ډېرپېښ, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::خطي);
        CV_CheckGlError();

        ثابت مزدوج راس[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.لوړوالی), 0.0,
            wndRect.x + wndRect.پلنوالی, (wndRect.y + wndRect.لوړوالی), 0.0,
            wndRect.x + wndRect.پلنوالی, wndRect.y, 0.0
        };
        ثابت مزدوج texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.لوړوالی,
            texRect.x + texRect.پلنوالی, texRect.y + texRect.لوړوالی,
            texRect.x + texRect.پلنوالی, texRect.y
        };

        ogl::بفر_2::unbind(ogl::بفر_2::ARRAY_بفر);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_اشاره_ګر(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::نورمال_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::راس_ARRAY);
        CV_CheckGlError();

        gl::راس_اشاره_ګر(3, gl::DOUBLE, 0, راس);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#د_که_پای
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, صحيح ډېرپېښ, Scalar color)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(ډېرپېښ);
    CV_ناکارول_شوی(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(ډېرپېښ, 0, arr.size());
    }
#د_که_پای
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, ننوت_Array indices, صحيح ډېرپېښ, Scalar color)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(arr);
    CV_ناکارول_شوی(indices);
    CV_ناکارول_شوی(ډېرپېښ);
    CV_ناکارول_شوی(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ثابت صحيح ډول = indices.ډول();

        بدل (ډول)
        {
        حالة ننوت_Array_2::OPENGL_بفر :
            {
                ogl::بفر_2 buf = indices.get_OGl_بفر();

                ثابت صحيح ژوروالی = buf.ژوروالی();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( ژوروالی <= CV_32S );

                GLenum type;
                إذا (ژوروالی < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (ژوروالی < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                gl::DrawElements(ډېرپېښ, buf.size().area(), type, 0);

                ogl::بفر_2::unbind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                اكسر;
            }

        افتراضي:
            {
                Mat mat = indices.getMat();

                ثابت صحيح ژوروالی = mat.ژوروالی();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( ژوروالی <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                إذا (ژوروالی < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (ژوروالی < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                ogl::بفر_2::unbind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                gl::DrawElements(ډېرپېښ, mat.size().area(), type, mat.معلومات);
            }
        }
    }
#د_که_پای
}

فراغ cv::ogl::clearColor(Scalar color)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(color);
    throw_no_ogl();
#وإلا
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_بفر_بټ);
#د_که_پای // HAVE_OPENGL
}

فراغ cv::ogl::clear_ژوروالی(عائم ژوروالی)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(ژوروالی);
    throw_no_ogl();
#وإلا
    gl::Clear_ژوروالی(ژوروالی);
    gl::Clear(gl::ژوروالی_بفر_بټ);
#د_که_پای // HAVE_OPENGL
}

فراغ cv::ogl::drawArrays(صحيح first, صحيح شمېر_2, صحيح ډېرپېښ)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(first); CV_ناکارول_شوی(شمېر_2); CV_ناکارول_شوی(ډېرپېښ);
    throw_no_ogl();
#وإلا
    gl::DrawArrays(ډېرپېښ, first, شمېر_2);
#د_که_پای // HAVE_OPENGL
}

فراغ cv::ogl::drawElements(صحيح first, صحيح شمېر_2, صحيح type, صحيح ډېرپېښ)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(first); CV_ناکارول_شوی(شمېر_2); CV_ناکارول_شوی(type); CV_ناکارول_شوی(ډېرپېښ);
    throw_no_ogl();
#وإلا
    gl::DrawElements(ډېرپېښ, شمېر_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)));
#د_که_پای
}

فراغ cv::ogl::drawElementsInstanced(صحيح first, صحيح شمېر_2, صحيح type, صحيح ډېرپېښ, صحيح instance_شمېر)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(first); CV_ناکارول_شوی(شمېر_2); CV_ناکارول_شوی(type); CV_ناکارول_شوی(ډېرپېښ); CV_ناکارول_شوی(instance_شمېر);
    throw_no_ogl();
#وإلا
    gl::DrawElementsInstanced(ډېرپېښ, شمېر_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)), instance_شمېر);
    CV_CheckGlError();
#د_که_پای
}

فراغ cv::ogl::blendFunc(صحيح sfactor, صحيح dfactor)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(sfactor); CV_ناکارول_شوی(dfactor);
    throw_no_ogl();
#وإلا
    gl::BlendFunc(sfactor, dfactor);
#د_که_پای
}

فراغ cv::ogl::ژوروالی_ماسک_2(منطقي flag)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(flag);
    throw_no_ogl();
#وإلا
    gl::ژوروالی_ماسک(flag ? gl::TRUE_ : gl::FALSE_);
#د_که_پای
}

فراغ cv::ogl::enable(صحيح cap)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(cap);
    throw_no_ogl();
#وإلا
    gl::Enable(cap);
#د_که_پای
}

فراغ cv::ogl::disable(صحيح cap)
{
#که_نه_تعریف_شوی HAVE_OPENGL
    CV_ناکارول_شوی(cap);
    throw_no_ogl();
#وإلا
    gl::Disable(cap);
#د_که_پای
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#که_تعریف_شوی HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  وإلا
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#وإلا // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#د_که_پای // HAVE_OPENCL

#إذا defined(HAVE_OPENGL)
#  إذا defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#د_که_پای // HAVE_OPENGL

نطاق cv { نطاق ogl {

#إذا defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ساكن فراغ checkOpenCLVersion()
{
    کارول نطاق cv::ocl;
    ثابت Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int حالت = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, حجم(platform_id), &platform_id, &sz);
    CV_Assert(حالت == CL_SUCCESS && sz == حجم(cl_platform_id));
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
#د_که_پای

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
    cl_device_id* devices = تش_اشاره;
    cl_uint dev_کارول_شوی = 0;
    cl_context context = تش_اشاره;

    cl_int حالت = clGetPlatformIDs(0, عدم, &platformsCnt);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", حالت));
    إذا (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    حالت = clGetPlatformIDs(platformsCnt, &platforms[0], عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", حالت));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    منطقي sharingSupported = خطأ;

    لكل (بینښه صحيح i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        حالت = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, عدم, &devCnt);
        إذا (حالت != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", حالت));

        حاول {
            devices = جديد cl_device_id[devCnt];

            حالت = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, عدم);
            إذا (حالت != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", حالت));

            لكل (بینښه صحيح j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_اندازه;
                حالت = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, عدم, &extension_اندازه );
                إذا (حالت != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", حالت));

                إذا(extension_اندازه > 0)
                {
                    std::string devString(extension_اندازه, '\0');
                    حالت = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_اندازه);
                    إذا (حالت != CL_SUCCESS)
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
                            dev_کارول_شوی = i;
                            sharingSupported = ریښتیا;
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
            إذا(devices != تش_اشاره) {
                ړنګول[] devices;
            }
            استمر;
        }

        إذا (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#إذا defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_ډله_Obj cgl_Share_ډله = CGLGet_Share_ډله(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_ډله,
                    0
                };
                context = clCreateContext(props, 0,0, عدم, عدم, &ci_Err_عدد);
#وإلاإذا defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_کارول_شوی], عدم, عدم, &حالت);
#وإلاإذا defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_کارول_شوی], عدم, عدم, &حالت);
#وإلاإذا defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_کارول_شوی], عدم, عدم, &حالت);
#د_که_پای
        }

        إذا (حالت != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", حالت));
        وإلا
            اكسر;
    }


    cl_platform_id platform = platforms[dev_کارول_شوی];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_کارول_شوی]);
    clReleaseDevice(devices[dev_کارول_شوی]);
    clReleaseContext(context);
    clExecCtx.bind();
    أرجع const_cast<Context&>(clExecCtx.getContext());
#د_که_پای
}

} // namespace cv::ogl::ocl

فراغ convert_منزل_GLTexture2_D(ننوت_Array src, Texture2D& texture)
{
    CV_ناکارول_شوی(src); CV_ناکارول_شوی(texture);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    اندازه src_اندازه = src.size();
    CV_Assert(src_اندازه.پلنوالی == (صحيح)texture.ستون_شمېر() && src_اندازه.لوړوالی == (صحيح)texture.لیکې());

    کارول نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", حالت));

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();
    حالت = cl_کتار_ته_ورزیاتول_Acquire_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ستون_شمېر, (size_t)u.لیکې, 1};
    حالت = cl_کتار_ته_ورزیاتول_Copy_بفر_منزل_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", حالت));
    حالت = cl_کتار_ته_ورزیاتول_Release_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت));

    حالت = cl_پای(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت));

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت));
#د_که_پای
}

فراغ convertFromGLTexture2D(ثابت Texture2D& texture, OutputArray dst)
{
    CV_ناکارول_شوی(texture); CV_ناکارول_شوی(dst);
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

    کارول نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", حالت));

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();
    حالت = cl_کتار_ته_ورزیاتول_Acquire_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ستون_شمېر, (size_t)u.لیکې, 1};
    حالت = cl_کتار_ته_ورزیاتول_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", حالت));
    حالت = cl_کتار_ته_ورزیاتول_Release_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت));

    حالت = cl_پای(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت));

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت));
#د_که_پای
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ثابت بفر_2& بفر, AccessFlag accessFlags)
{
    CV_ناکارول_شوی(بفر); CV_ناکارول_شوی(accessFlags);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    کارول نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_کتار cl_کتار = (cl_command_کتار)کتار::getDefault().ptr();

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

    cl_int حالت = 0;
    cl_mem cl_بفر = clCreateFromGLBuffer(context, clAccessFlags, بفر.bufId(), &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", حالت));

    gl::پای();

    حالت = cl_کتار_ته_ورزیاتول_Acquire_GLObjects(cl_کتار, 1, &cl_بفر, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت));

    size_t ګام = بفر.ستون_شمېر() * بفر.elem_اندازه();
    صحيح لیکې = بفر.لیکې();
    صحيح ستون_شمېر = بفر.ستون_شمېر();
    صحيح type = بفر.type();

    UMat u;
    convert_From_بفر(cl_بفر, ګام, لیکې, ستون_شمېر, type, u);
    أرجع u;
#د_که_پای
}

فراغ unmapGLBuffer(UMat& u)
{
    CV_ناکارول_شوی(u);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    کارول نطاق cv::ocl;
    cl_command_کتار cl_کتار = (cl_command_کتار)کتار::getDefault().ptr();

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int حالت = cl_کتار_ته_ورزیاتول_Release_GLObjects(cl_کتار, 1, &cl_بفر, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت));

    حالت = cl_پای(cl_کتار);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت));

    حالت = clReleaseMemObject(cl_بفر);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت));
#د_که_پای
}

}} // namespace cv::ogl
