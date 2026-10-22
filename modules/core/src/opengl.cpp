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
   #تعریف_کرو GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#وإلا
   #تعریف_کرو GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#جے_انت

#جے_تعریف_شدہ HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    إذا (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_نسخہ
#        define GL_نسخہ 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#وإلا // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#جے_انت // HAVE_OPENGL

ورتوں نطاق cv;
ورتوں نطاق cv::cuda;

#إذا defined(_MSC_VER)
#ہدایت warning(disable : 4702)  // unreachable code
#جے_انت

نطاق
{
#جے_بنا_تعریف HAVE_OPENGL
مضمن ساكن CV_NORETURN فراغ throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#وإلاإذا defined _DEBUG
مضمن ساكن منطقي checkError(ثابت حرف* file, ثابت صحيح سطر, ثابت حرف* func = 0)
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
        حالة gl::INVALID_قدر:
            پیغام = "A numeric argument is out of range";
            اكسر;
        حالة gl::INVALID_OPERATION:
            پیغام = "The specified operation is not allowed in the current state";
            اكسر;
        حالة gl::باہرلا_دا_حافظہ:
            پیغام = "There is not enough memory left to execute the command";
            اكسر;
        افتراضي:
            پیغام = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, پیغام, file, سطر);
    }
    أرجع سچ;
}
#جے_انت // HAVE_OPENGL
} // namespace

#تعریف_کرو CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#جے_تعریف_شدہ HAVE_OPENGL
نطاق
{
    ثابت GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#جے_انت

////////////////////////////////////////////////////////////////////////
// setGlDevice

فراغ cv::cuda::سیٹ_Gl_Device(صحيح device)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(device);
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        CV_بن_ورتیا(device);
        throw_no_cuda();
    #وإلا
        cuda_محفوظ_Call( cudaGLSetGLDevice(device) );
    #جے_انت
#جے_انت
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

        فراغ رجسٹر_بفر(GLuint بفر);
        فراغ release();

        فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream = 0);
        فراغ copy_منزل(فراغ* dst, size_t dpitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream = 0);

        فراغ* map(cuda_دھارا_t stream = 0);
        فراغ unmap(cuda_دھارا_t stream = 0);

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

    فراغ CudaResource::رجسٹر_بفر(GLuint بفر)
    {
        CV_DbgAssert( بفر != 0 );

        إذا (بفر_3 == بفر)
            أرجع;

        cudaGraphicsResource_t resource;
        cuda_محفوظ_Call( cuda_Graphics_GLRegister_بفر(&resource, بفر, cudaGraphicsMapFlagsNone) );

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
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_دھارا_t stream);
        ~GraphicsMapHolder();

        فراغ reset();

    خاص:
        cudaGraphicsResource_t* resource_;
        cuda_دھارا_t دھارا_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_دھارا_t stream) : resource_(resource), دھارا_3(stream)
    {
        إذا (resource_)
            cuda_محفوظ_Call( cudaGraphicsMapResources(1, resource_, دھارا_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        إذا (resource_)
            cudaGraphicsUnmapResources(1, resource_, دھارا_3);
    }

    فراغ CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    فراغ CudaResource::copyFrom(ثابت فراغ* src, size_t spitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_بن_ورتیا(h);

        فراغ* dst;
        size_t size;
        cuda_محفوظ_Call( cuda_Graphics_Resource_Get_Mapped_اشارہ_گر(&dst, &size, resource_) );

        CV_DbgAssert( چوڑائی * اچائی == size );

        إذا (stream == 0)
            cuda_محفوظ_Call( cudaMemcpy2D(dst, چوڑائی, src, spitch, چوڑائی, اچائی, cuda_Memcpy_Device_منزل_Device) );
        وإلا
            cuda_محفوظ_Call( cudaMemcpy2DAsync(dst, چوڑائی, src, spitch, چوڑائی, اچائی, cuda_Memcpy_Device_منزل_Device, stream) );
    }

    فراغ CudaResource::copy_منزل(فراغ* dst, size_t dpitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_بن_ورتیا(h);

        فراغ* src;
        size_t size;
        cuda_محفوظ_Call( cuda_Graphics_Resource_Get_Mapped_اشارہ_گر(&src, &size, resource_) );

        CV_DbgAssert( چوڑائی * اچائی == size );

        إذا (stream == 0)
            cuda_محفوظ_Call( cudaMemcpy2D(dst, dpitch, src, چوڑائی, چوڑائی, اچائی, cuda_Memcpy_Device_منزل_Device) );
        وإلا
            cuda_محفوظ_Call( cudaMemcpy2DAsync(dst, dpitch, src, چوڑائی, چوڑائی, اچائی, cuda_Memcpy_Device_منزل_Device, stream) );
    }

    فراغ* CudaResource::map(cuda_دھارا_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        فراغ* ptr;
        size_t size;
        cuda_محفوظ_Call( cuda_Graphics_Resource_Get_Mapped_اشارہ_گر(&ptr, &size, resource_) );

        h.reset();

        أرجع ptr;
    }

    فراغ CudaResource::unmap(cuda_دھارا_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#جے_انت

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#جے_بنا_تعریف HAVE_OPENGL

صنف cv::ogl::بفر_2::Impl
{
};

#وإلا

صنف cv::ogl::بفر_2::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint bufId, منطقي autoRelease);
    Impl(GLsizeiptr size, ثابت GLvoid* انکڑے, GLenum target, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLenum target) ثابت;

    فراغ copyFrom(GLuint srcBuf, GLsizeiptr size);

    فراغ copyFrom(GLsizeiptr size, ثابت GLvoid* انکڑے);
    فراغ copy_منزل(GLsizeiptr size, GLvoid* انکڑے) ثابت;

    فراغ resize(GLsizeiptr size, GLenum target);

    فراغ* mapHost(GLenum access);
    فراغ unmapHost();

#جے_تعریف_شدہ HAVE_CUDA
    فراغ copyFrom(ثابت فراغ* src, size_t spitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream = 0);
    فراغ copy_منزل(فراغ* dst, size_t dpitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream = 0) ثابت;

    فراغ* mapDevice(cuda_دھارا_t stream = 0);
    فراغ unmapDevice(cuda_دھارا_t stream = 0);
#جے_انت

    فراغ سیٹ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint bufId() ثابت { أرجع bufId_; }

خاص:
    Impl();

    GLuint bufId_;
    منطقي autoRelease_;

#جے_تعریف_شدہ HAVE_CUDA
    بدلنجوگ CudaResource cudaResource_;
#جے_انت
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

cv::ogl::بفر_2::Impl::Impl(GLsizeiptr size, ثابت GLvoid* انکڑے, GLenum target, منطقي autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();

    gl::بفر_انکڑے(target, size, انکڑے, gl::DYNAMIC_DRAW);
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

    gl::Copy_بفر_Sub_انکڑے(gl::COPY_READ_بفر, gl::COPY_WRITE_بفر, 0, 0, size);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::copyFrom(GLsizeiptr size, ثابت GLvoid* انکڑے)
{
    gl::Bind_بفر(gl::COPY_WRITE_بفر, bufId_);
    CV_CheckGlError();

    gl::بفر_Sub_انکڑے(gl::COPY_WRITE_بفر, 0, size, انکڑے);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::بفر_انکڑے(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
}

فراغ cv::ogl::بفر_2::Impl::copy_منزل(GLsizeiptr size, GLvoid* انکڑے) ثابت
{
    gl::Bind_بفر(gl::COPY_READ_بفر, bufId_);
    CV_CheckGlError();

    gl::Get_بفر_Sub_انکڑے(gl::COPY_READ_بفر, 0, size, انکڑے);
    CV_CheckGlError();
}

فراغ* cv::ogl::بفر_2::Impl::mapHost(GLenum access)
{
    gl::Bind_بفر(gl::COPY_READ_بفر, bufId_);
    CV_CheckGlError();

    GLvoid* انکڑے = gl::Map_بفر(gl::COPY_READ_بفر, access);
    CV_CheckGlError();

    أرجع انکڑے;
}

فراغ cv::ogl::بفر_2::Impl::unmapHost()
{
    gl::Unmap_بفر(gl::COPY_READ_بفر);
}

#جے_تعریف_شدہ HAVE_CUDA

فراغ cv::ogl::بفر_2::Impl::copyFrom(ثابت فراغ* src, size_t spitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream)
{
    cudaResource_.رجسٹر_بفر(bufId_);
    cudaResource_.copyFrom(src, spitch, چوڑائی, اچائی, stream);
}

فراغ cv::ogl::بفر_2::Impl::copy_منزل(فراغ* dst, size_t dpitch, size_t چوڑائی, size_t اچائی, cuda_دھارا_t stream) ثابت
{
    cudaResource_.رجسٹر_بفر(bufId_);
    cudaResource_.copy_منزل(dst, dpitch, چوڑائی, اچائی, stream);
}

فراغ* cv::ogl::بفر_2::Impl::mapDevice(cuda_دھارا_t stream)
{
    cudaResource_.رجسٹر_بفر(bufId_);
    أرجع cudaResource_.map(stream);
}

فراغ cv::ogl::بفر_2::Impl::unmapDevice(cuda_دھارا_t stream)
{
    cudaResource_.unmap(stream);
}

#جے_انت // HAVE_CUDA

#جے_انت // HAVE_OPENGL

cv::ogl::بفر_2::بفر_2() : صفاں_2(0), خانے_گنتی_2(0), type_(0)
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#جے_انت
}

cv::ogl::بفر_2::بفر_2(صحيح arows, صحيح acols, صحيح atype, بنانشانی صحيح abufId, منطقي autoRelease) : صفاں_2(0), خانے_گنتی_2(0), type_(0)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arows);
    CV_بن_ورتیا(acols);
    CV_بن_ورتیا(atype);
    CV_بن_ورتیا(abufId);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    صفاں_2 = arows;
    خانے_گنتی_2 = acols;
    type_ = atype;
#جے_انت
}

cv::ogl::بفر_2::بفر_2(ناپ asize, صحيح atype, بنانشانی صحيح abufId, منطقي autoRelease) : صفاں_2(0), خانے_گنتی_2(0), type_(0)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(asize);
    CV_بن_ورتیا(atype);
    CV_بن_ورتیا(abufId);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(abufId, autoRelease));
    صفاں_2 = asize.اچائی;
    خانے_گنتی_2 = asize.چوڑائی;
    type_ = atype;
#جے_انت
}

cv::ogl::بفر_2::بفر_2(داخلہ_Array arr, نشانہ target, منطقي autoRelease) : صفاں_2(0), خانے_گنتی_2(0), type_(0)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(target);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح قسم = arr.قسم();

    بدل (قسم)
    {
    حالة داخلہ_Array_2::OPENGL_بفر:
    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        اكسر;

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ثابت GLsizeiptr asize = mat.صفاں * mat.خانے_گنتی * mat.elem_ناپ();
            impl_.reset(جديد Impl(asize, mat.انکڑے, target, autoRelease));
            صفاں_2 = mat.صفاں;
            خانے_گنتی_2 = mat.خانے_گنتی;
            type_ = mat.type();
            اكسر;
        }
    }
#جے_انت
}

فراغ cv::ogl::بفر_2::create(صحيح arows, صحيح acols, صحيح atype, نشانہ target, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arows);
    CV_بن_ورتیا(acols);
    CV_بن_ورتیا(atype);
    CV_بن_ورتیا(target);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (صفاں_2 != arows || خانے_گنتی_2 != acols || type_ != atype)
    {
        ثابت GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_ناپ(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        إذا (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->سیٹ_Auto_Release(autoRelease);
        }
        وإلا
            impl_.reset(جديد Impl(asize, 0, target, autoRelease));
        صفاں_2 = arows;
        خانے_گنتی_2 = acols;
        type_ = atype;
    }
#جے_انت
}

فراغ cv::ogl::بفر_2::release()
{
#جے_تعریف_شدہ HAVE_OPENGL
    إذا (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
    صفاں_2 = 0;
    خانے_گنتی_2 = 0;
    type_ = 0;
#جے_انت
}

فراغ cv::ogl::بفر_2::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    impl_->سیٹ_Auto_Release(flag);
#جے_انت
}

فراغ cv::ogl::بفر_2::copyFrom(داخلہ_Array arr, نشانہ target, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(target);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح قسم = arr.قسم();

    ثابت ناپ asize = arr.size();
    ثابت صحيح atype = arr.type();
    create(asize, atype, target, autoRelease);

    بدل (قسم)
    {
    حالة داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_ناپ(atype));
            اكسر;
        }

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #جے_بنا_تعریف HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.انکڑے, dmat.قدم, dmat.خانے_گنتی * dmat.elem_ناپ(), dmat.صفاں);
            #جے_انت

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_ناپ(atype), mat.انکڑے);
        }
    }
#جے_انت
}

فراغ cv::ogl::بفر_2::copyFrom(داخلہ_Array arr, cuda::دھارا& stream, نشانہ target, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(stream);
    CV_بن_ورتیا(target);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        CV_بن_ورتیا(arr);
        CV_بن_ورتیا(stream);
        CV_بن_ورتیا(target);
        CV_بن_ورتیا(autoRelease);
        throw_no_cuda();
    #وإلا
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.انکڑے, dmat.قدم, dmat.خانے_گنتی * dmat.elem_ناپ(), dmat.صفاں, cuda::دھارا_Accessor::get_دھارا(stream));
    #جے_انت
#جے_انت
}

فراغ cv::ogl::بفر_2::copy_منزل(OutputArray arr) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    throw_no_ogl();
#وإلا
    ثابت صحيح قسم = arr.قسم();

    بدل (قسم)
    {
    حالة داخلہ_Array_2::OPENGL_بفر:
        {
            arr.get_OGl_بفر_Ref().copyFrom(*هذا);
            اكسر;
        }

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #جے_بنا_تعریف HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(صفاں_2, خانے_گنتی_2, type_);
                impl_->copy_منزل(dmat.انکڑے, dmat.قدم, dmat.خانے_گنتی * dmat.elem_ناپ(), dmat.صفاں);
            #جے_انت

            اكسر;
        }

    افتراضي:
        {
            arr.create(صفاں_2, خانے_گنتی_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_منزل(mat.صفاں * mat.خانے_گنتی * mat.elem_ناپ(), mat.انکڑے);
        }
    }
#جے_انت
}

فراغ cv::ogl::بفر_2::copy_منزل(OutputArray arr, cuda::دھارا& stream) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(stream);
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        CV_بن_ورتیا(arr);
        CV_بن_ورتیا(stream);
        throw_no_cuda();
    #وإلا
        arr.create(صفاں_2, خانے_گنتی_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_منزل(dmat.انکڑے, dmat.قدم, dmat.خانے_گنتی * dmat.elem_ناپ(), dmat.صفاں, cuda::دھارا_Accessor::get_دھارا(stream));
    #جے_انت
#جے_انت
}

cv::ogl::بفر_2 cv::ogl::بفر_2::clone(نشانہ target, منطقي autoRelease) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(target);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    ogl::بفر_2 buf;
    buf.copyFrom(*هذا, target, autoRelease);
    أرجع buf;
#جے_انت
}

فراغ cv::ogl::بفر_2::bind(نشانہ target) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(target);
    throw_no_ogl();
#وإلا
    impl_->bind(target);
#جے_انت
}

فراغ cv::ogl::بفر_2::unbind(نشانہ target)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(target);
    throw_no_ogl();
#وإلا
    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
#جے_انت
}

Mat cv::ogl::بفر_2::mapHost(Access access)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(access);
    throw_no_ogl();
#وإلا
    أرجع Mat(صفاں_2, خانے_گنتی_2, type_, impl_->mapHost(access));
#جے_انت
}

فراغ cv::ogl::بفر_2::unmapHost()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->unmapHost();
#جے_انت
}

GpuMat cv::ogl::بفر_2::mapDevice()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(صفاں_2, خانے_گنتی_2, type_, impl_->mapDevice());
    #جے_انت
#جے_انت
}

فراغ cv::ogl::بفر_2::unmapDevice()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice();
    #جے_انت
#جے_انت
}

cuda::GpuMat cv::ogl::بفر_2::mapDevice(cuda::دھارا& stream)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(stream);
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        CV_بن_ورتیا(stream);
        throw_no_cuda();
    #وإلا
        أرجع GpuMat(صفاں_2, خانے_گنتی_2, type_, impl_->mapDevice(cuda::دھارا_Accessor::get_دھارا(stream)));
    #جے_انت
#جے_انت
}

فراغ cv::ogl::بفر_2::unmapDevice(cuda::دھارا& stream)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(stream);
    throw_no_ogl();
#وإلا
    #جے_بنا_تعریف HAVE_CUDA
        CV_بن_ورتیا(stream);
        throw_no_cuda();
    #وإلا
        impl_->unmapDevice(cuda::دھارا_Accessor::get_دھارا(stream));
    #جے_انت
#جے_انت
}

بنانشانی صحيح cv::ogl::بفر_2::bufId() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->bufId();
#جے_انت
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#جے_بنا_تعریف HAVE_OPENGL

صنف cv::ogl::Texture2D::Impl
{
};

#وإلا

صنف cv::ogl::Texture2D::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl(GLuint texId, منطقي autoRelease);
    Impl(GLint اندرونی_Format, GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease);
    ~Impl();

    فراغ copyFrom(GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLenum type, ثابت GLvoid *pixels);
    فراغ copy_منزل(GLenum format, GLenum type, GLvoid* pixels) ثابت;

    فراغ bind() ثابت;

    فراغ سیٹ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

cv::ogl::Texture2D::Impl::Impl(GLint اندرونی_Format, GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, اندرونی_Format, چوڑائی, اچائی, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    إذا (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

فراغ cv::ogl::Texture2D::Impl::copyFrom(GLsizei چوڑائی, GLsizei اچائی, GLenum format, GLenum type, ثابت GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, چوڑائی, اچائی, format, type, pixels);
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

#جے_انت // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : صفاں_2(0), خانے_گنتی_2(0), format_(NONE)
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#جے_انت
}

cv::ogl::Texture2D::Texture2D(صحيح arows, صحيح acols, Format aformat, بنانشانی صحيح atexId, منطقي autoRelease) : صفاں_2(0), خانے_گنتی_2(0), format_(NONE)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arows);
    CV_بن_ورتیا(acols);
    CV_بن_ورتیا(aformat);
    CV_بن_ورتیا(atexId);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    صفاں_2 = arows;
    خانے_گنتی_2 = acols;
    format_ = aformat;
#جے_انت
}

cv::ogl::Texture2D::Texture2D(ناپ asize, Format aformat, بنانشانی صحيح atexId, منطقي autoRelease) : صفاں_2(0), خانے_گنتی_2(0), format_(NONE)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(asize);
    CV_بن_ورتیا(aformat);
    CV_بن_ورتیا(atexId);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(atexId, autoRelease));
    صفاں_2 = asize.اچائی;
    خانے_گنتی_2 = asize.چوڑائی;
    format_ = aformat;
#جے_انت
}

cv::ogl::Texture2D::Texture2D(داخلہ_Array arr, منطقي autoRelease) : صفاں_2(0), خانے_گنتی_2(0), format_(NONE)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح قسم = arr.قسم();

    ثابت ناپ asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح ڈونگھائی = CV_MAT_ڈونگھائی(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

    CV_Assert( ڈونگھائی <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format اندرونی_Formats[] =
    {
        NONE, ڈونگھائی_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::ڈونگھائی_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    بدل (قسم)
    {
    حالة داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_.reset(جديد Impl(اندرونی_Formats[cn], asize.چوڑائی, asize.اچائی, srcFormats[cn], gl_types[ڈونگھائی], 0, autoRelease));
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            اكسر;
        }

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #جے_بنا_تعریف HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::بفر_2 buf(dmat, ogl::بفر_2::PIXEL_UNPACK_بفر);
                buf.سیٹ_Auto_Release(سچ);
                buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
                impl_.reset(جديد Impl(اندرونی_Formats[cn], asize.چوڑائی, asize.اچائی, srcFormats[cn], gl_types[ڈونگھائی], 0, autoRelease));
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            #جے_انت

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_.reset(جديد Impl(اندرونی_Formats[cn], asize.چوڑائی, asize.اچائی, srcFormats[cn], gl_types[ڈونگھائی], mat.انکڑے, autoRelease));
            اكسر;
        }
    }

    صفاں_2 = asize.اچائی;
    خانے_گنتی_2 = asize.چوڑائی;
    format_ = اندرونی_Formats[cn];
#جے_انت
}

فراغ cv::ogl::Texture2D::create(صحيح arows, صحيح acols, Format aformat, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arows);
    CV_بن_ورتیا(acols);
    CV_بن_ورتیا(aformat);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    إذا (صفاں_2 != arows || خانے_گنتی_2 != acols || format_ != aformat)
    {
        ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
        impl_.reset(جديد Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        صفاں_2 = arows;
        خانے_گنتی_2 = acols;
        format_ = aformat;
    }
#جے_انت
}

فراغ cv::ogl::Texture2D::release()
{
#جے_تعریف_شدہ HAVE_OPENGL
    إذا (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
    صفاں_2 = 0;
    خانے_گنتی_2 = 0;
    format_ = NONE;
#جے_انت
}

فراغ cv::ogl::Texture2D::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    impl_->سیٹ_Auto_Release(flag);
#جے_انت
}

فراغ cv::ogl::Texture2D::copyFrom(داخلہ_Array arr, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح قسم = arr.قسم();

    ثابت ناپ asize = arr.size();
    ثابت صحيح atype = arr.type();

    ثابت صحيح ڈونگھائی = CV_MAT_ڈونگھائی(atype);
    ثابت صحيح cn = CV_MAT_CN(atype);

    CV_Assert( ڈونگھائی <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format اندرونی_Formats[] =
    {
        NONE, ڈونگھائی_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::ڈونگھائی_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, اندرونی_Formats[cn], autoRelease);

    بدل(قسم)
    {
    حالة داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_->copyFrom(asize.چوڑائی, asize.اچائی, srcFormats[cn], gl_types[ڈونگھائی], 0);
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            اكسر;
        }

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #جے_بنا_تعریف HAVE_CUDA
                throw_no_cuda();
            #وإلا
                GpuMat dmat = arr.getGpuMat();
                ogl::بفر_2 buf(dmat, ogl::بفر_2::PIXEL_UNPACK_بفر);
                buf.سیٹ_Auto_Release(سچ);
                buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
                impl_->copyFrom(asize.چوڑائی, asize.اچائی, srcFormats[cn], gl_types[ڈونگھائی], 0);
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            #جے_انت

            اكسر;
        }

    افتراضي:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_->copyFrom(asize.چوڑائی, asize.اچائی, srcFormats[cn], gl_types[ڈونگھائی], mat.انکڑے);
        }
    }
#جے_انت
}

فراغ cv::ogl::Texture2D::copy_منزل(OutputArray arr, صحيح ddepth, منطقي autoRelease) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(ddepth);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    ثابت صحيح قسم = arr.قسم();

    ثابت صحيح cn = format_ == ڈونگھائی_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ثابت GLenum dstFormat = format_ == ڈونگھائی_COMPONENT ? gl::ڈونگھائی_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    بدل(قسم)
    {
    حالة داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2& buf = arr.get_OGl_بفر_Ref();
            buf.create(صفاں_2, خانے_گنتی_2, CV_MAKE_TYPE(ddepth, cn), ogl::بفر_2::PIXEL_PACK_بفر, autoRelease);
            buf.bind(ogl::بفر_2::PIXEL_PACK_بفر);
            impl_->copy_منزل(dstFormat, gl_types[ddepth], 0);
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
            اكسر;
        }

    حالة داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #جے_بنا_تعریف HAVE_CUDA
                throw_no_cuda();
            #وإلا
                ogl::بفر_2 buf(صفاں_2, خانے_گنتی_2, CV_MAKE_TYPE(ddepth, cn), ogl::بفر_2::PIXEL_PACK_بفر);
                buf.سیٹ_Auto_Release(سچ);
                buf.bind(ogl::بفر_2::PIXEL_PACK_بفر);
                impl_->copy_منزل(dstFormat, gl_types[ddepth], 0);
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
                buf.copy_منزل(arr);
            #جے_انت

            اكسر;
        }

    افتراضي:
        {
            arr.create(صفاں_2, خانے_گنتی_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
            impl_->copy_منزل(dstFormat, gl_types[ddepth], mat.انکڑے);
        }
    }
#جے_انت
}

فراغ cv::ogl::Texture2D::bind() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#جے_انت
}

بنانشانی صحيح cv::ogl::Texture2D::texId() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#جے_انت
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#جے_بنا_تعریف HAVE_OPENGL

صنف cv::ogl::Texture_بفر::Impl
{
};

#وإلا

صنف cv::ogl::Texture_بفر::Impl
{
عام:
    ساكن ثابت Ptr<Impl> empty();

    Impl();
    Impl(GLenum اندرونی_Format, GLuint bufId, منطقي autoRelease);
    ~Impl();

    فراغ bind(GLint unit) ثابت;

    فراغ سیٹ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

cv::ogl::Texture_بفر::Impl::Impl(GLenum اندرونی_Format, GLuint bufId, منطقي autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_بفر, texId_);
    CV_CheckGlError();

    gl::Tex_بفر(gl::TEXTURE_بفر, اندرونی_Format, bufId);
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
    gl::سرگرم_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_بفر, texId_);
    CV_CheckGlError();
}

#جے_انت // HAVE_OPENGL

cv::ogl::Texture_بفر::Texture_بفر() : format_(NONE)
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#جے_انت
}

cv::ogl::Texture_بفر::Texture_بفر(ثابت بفر_2& buf, Format aformat, منطقي autoRelease) : format_(NONE)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(buf);
    CV_بن_ورتیا(aformat);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#جے_انت
}

فراغ cv::ogl::Texture_بفر::create(ثابت بفر_2& buf, Format aformat, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(buf);
    CV_بن_ورتیا(aformat);
    CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#جے_انت
}

فراغ cv::ogl::Texture_بفر::release()
{
#جے_تعریف_شدہ HAVE_OPENGL
    إذا (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#جے_انت
    buf_ = بفر_2();
    format_ = NONE;
}

فراغ cv::ogl::Texture_بفر::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    impl_->سیٹ_Auto_Release(flag);
#جے_انت
}

فراغ cv::ogl::Texture_بفر::bind(صحيح unit) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(unit);
    throw_no_ogl();
#وإلا
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#جے_انت
}

cv::ogl::Texture_بفر::Format cv::ogl::Texture_بفر::format() ثابت
{
    أرجع format_;
}

منطقي cv::ogl::Texture_بفر::empty() ثابت
{
    أرجع format_ == NONE;
}

بنانشانی صحيح cv::ogl::Texture_بفر::texId() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->texId();
#جے_انت
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

فراغ cv::ogl::Arrays::سیٹ_راس_Array(داخلہ_Array راس)
{
    ثابت صحيح cn = راس.channels();
    ثابت صحيح ڈونگھائی = راس.ڈونگھائی();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( ڈونگھائی == CV_16S || ڈونگھائی == CV_32S || ڈونگھائی == CV_32F || ڈونگھائی == CV_64F );

    إذا (راس.قسم() == داخلہ_Array_2::OPENGL_بفر)
        راس_2 = راس.get_OGl_بفر();
    وإلا
        راس_2.copyFrom(راس);

    ناپ_3 = راس_2.size().area();
}

فراغ cv::ogl::Arrays::reset_راس_Array()
{
    راس_2.release();
    ناپ_3 = 0;
}

فراغ cv::ogl::Arrays::سیٹ_Color_Array(داخلہ_Array color)
{
    ثابت صحيح cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    إذا (color.قسم() == داخلہ_Array_2::OPENGL_بفر)
        color_ = color.get_OGl_بفر();
    وإلا
        color_.copyFrom(color);
}

فراغ cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

فراغ cv::ogl::Arrays::سیٹ_عام_Array(داخلہ_Array عام_2)
{
    ثابت صحيح cn = عام_2.channels();
    ثابت صحيح ڈونگھائی = عام_2.ڈونگھائی();

    CV_Assert( cn == 3 );
    CV_Assert( ڈونگھائی == CV_8S || ڈونگھائی == CV_16S || ڈونگھائی == CV_32S || ڈونگھائی == CV_32F || ڈونگھائی == CV_64F );

    إذا (عام_2.قسم() == داخلہ_Array_2::OPENGL_بفر)
        عام_3 = عام_2.get_OGl_بفر();
    وإلا
        عام_3.copyFrom(عام_2);
}

فراغ cv::ogl::Arrays::reset_عام_Array()
{
    عام_3.release();
}

فراغ cv::ogl::Arrays::سیٹ_Tex_Coord_Array(داخلہ_Array texCoord)
{
    ثابت صحيح cn = texCoord.channels();
    ثابت صحيح ڈونگھائی = texCoord.ڈونگھائی();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( ڈونگھائی == CV_16S || ڈونگھائی == CV_32S || ڈونگھائی == CV_32F || ڈونگھائی == CV_64F );

    إذا (texCoord.قسم() == داخلہ_Array_2::OPENGL_بفر)
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
    reset_عام_Array();
    resetTexCoordArray();
}

فراغ cv::ogl::Arrays::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    راس_2.سیٹ_Auto_Release(flag);
    color_.سیٹ_Auto_Release(flag);
    عام_3.سیٹ_Auto_Release(flag);
    texCoord_.سیٹ_Auto_Release(flag);
#جے_انت
}

فراغ cv::ogl::Arrays::bind() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == ناپ_3 );
    CV_Assert( عام_3.empty() || عام_3.size().area() == ناپ_3 );
    CV_Assert( color_.empty() || color_.size().area() == ناپ_3 );

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

        gl::Tex_Coord_اشارہ_گر(texCoord_.channels(), gl_types[texCoord_.ڈونگھائی()], 0, 0);
        CV_CheckGlError();
    }

    إذا (عام_3.empty())
    {
        gl::DisableClientState(gl::عام_ARRAY);
        CV_CheckGlError();
    }
    وإلا
    {
        gl::EnableClientState(gl::عام_ARRAY);
        CV_CheckGlError();

        عام_3.bind(ogl::بفر_2::ARRAY_بفر);

        gl::عام_اشارہ_گر(gl_types[عام_3.ڈونگھائی()], 0, 0);
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

        gl::Color_اشارہ_گر(cn, gl_types[color_.ڈونگھائی()], 0, 0);
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

        gl::راس_اشارہ_گر(راس_2.channels(), gl_types[راس_2.ڈونگھائی()], 0, 0);
        CV_CheckGlError();
    }

    ogl::بفر_2::unbind(ogl::بفر_2::ARRAY_بفر);
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#جے_بنا_تعریف HAVE_OPENGL

صنف cv::ogl::راس_Array::Impl
{
};

#وإلا

صنف cv::ogl::راس_Array::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint vaId, منطقي autoRelease);
    Impl(std::initializer_list<Attribute> گن, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    فراغ سیٹ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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

cv::ogl::راس_Array::Impl::Impl(std::initializer_list<Attribute> گن, منطقي autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_راس_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_راس_Array(vaId_);
    CV_CheckGlError();

    لكل (خودکار& attribute : گن)
    {
        attribute.بفر_3.bind(بفر_2::ARRAY_بفر);
        إذا (attribute.integer_)
        {
            gl::راس_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.ناپ_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.وتھ)
            );
            CV_CheckGlError();
        }
        وإلا
        {
            gl::راس_Attrib_اشارہ_گر(
                attribute.shader_loc_,
                attribute.ناپ_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت فراغ*>(attribute.وتھ)
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

#جے_انت // HAVE_OPENGL

cv::ogl::راس_Array::راس_Array()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#جے_انت
}

cv::ogl::راس_Array::راس_Array(std::initializer_list<Attribute> گن, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(گن); CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(گن, autoRelease));
#جے_انت
}

فراغ cv::ogl::راس_Array::create(std::initializer_list<Attribute> گن, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(گن); CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(گن, autoRelease));
#جے_انت
}

فراغ cv::ogl::راس_Array::release()
{
#جے_تعریف_شدہ HAVE_OPENGL
    إذا (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#جے_انت
}

فراغ cv::ogl::راس_Array::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    impl_->سیٹ_Auto_Release(flag);
#جے_انت
}

فراغ cv::ogl::راس_Array::bind() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#جے_انت
}

فراغ cv::ogl::راس_Array::unbind()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::Bind_راس_Array(0);
#جے_انت
}

بنانشانی صحيح cv::ogl::راس_Array::vaId() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->vaId();
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#جے_بنا_تعریف HAVE_OPENGL

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

    فراغ سیٹ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

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
    gl::Shader_سروت(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_حالت, &حالت);
    إذا (!حالت) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_لمائی, &حالت);
        info_log.resize(حالت);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, خالی_سوچک, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    إذا (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#جے_انت // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::راس_3)
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#جے_انت
}

cv::ogl::Shader::Shader(ثابت حرف* src, ShaderType type, منطقي autoRelease) : type_(type)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(src); CV_بن_ورتیا(type); CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
#جے_انت
}

فراغ cv::ogl::Shader::create(ثابت حرف* src, ShaderType type, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(src); CV_بن_ورتیا(type); CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(src, type, autoRelease));
    type_ = type;
#جے_انت
}

فراغ cv::ogl::Shader::release()
{
#جے_تعریف_شدہ HAVE_OPENGL
    إذا (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#جے_انت
}

فراغ cv::ogl::Shader::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    impl_->سیٹ_Auto_Release(flag);
#جے_انت
}

بنانشانی صحيح cv::ogl::Shader::shaderId() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->shaderId();
#جے_انت
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ثابت
{
    أرجع type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#جے_بنا_تعریف HAVE_OPENGL

صنف cv::ogl::پروگرام_2::Impl
{
};

#وإلا

صنف cv::ogl::پروگرام_2::Impl
{
عام:
    ساكن ثابت Ptr<Impl>& empty();

    Impl(GLuint پروگرام_Id, منطقي autoRelease);
    Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease);
    ~Impl();

    فراغ bind();

    صحيح getAttributeLocation(ثابت حرف* name) ثابت;
    صحيح get_اکسار_Location(ثابت حرف* name) ثابت;

    فراغ سیٹ_Auto_Release(منطقي flag) { autoRelease_ = flag; }

    GLuint پروگرام_Id() ثابت { أرجع پروگرام_Id_2; }

خاص:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint پروگرام_Id_2;
    منطقي autoRelease_;
};

ثابت Ptr<cv::ogl::پروگرام_2::Impl>& cv::ogl::پروگرام_2::Impl::empty()
{
    ساكن Ptr<Impl> p(جديد Impl);
    أرجع p;
}

cv::ogl::پروگرام_2::Impl::Impl() : پروگرام_Id_2(0), autoRelease_(خطأ)
{
}

cv::ogl::پروگرام_2::Impl::Impl(GLuint پروگرام_Id, منطقي autoRelease) : پروگرام_Id_2(پروگرام_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_پروگرام(پروگرام_Id) == gl::TRUE_ );
}

cv::ogl::پروگرام_2::Impl::Impl(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease) : پروگرام_Id_2(0), autoRelease_(autoRelease)
{
    پروگرام_Id_2 = gl::Create_پروگرام();
    CV_CheckGlError();

    CV_Assert( پروگرام_Id_2 != 0 );

    gl::AttachShader(پروگرام_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(پروگرام_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint حالت;
    gl::Link_پروگرام(پروگرام_Id_2);
    gl::GetProgramiv(پروگرام_Id_2, gl::LINK_حالت, &حالت);
    إذا (!حالت) {
        String info_log;
        gl::GetProgramiv(پروگرام_Id_2, gl::INFO_LOG_لمائی, &حالت);
        info_log.resize(حالت);
        gl::Get_پروگرام_Info_Log(پروگرام_Id_2, GLsizei(info_log.size()) + 1, خالی_سوچک, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::پروگرام_2::Impl::~Impl()
{
    إذا (autoRelease_ && پروگرام_Id_2)
        gl::Delete_پروگرام(پروگرام_Id_2);
}

فراغ cv::ogl::پروگرام_2::Impl::bind()
{
    gl::Use_پروگرام(پروگرام_Id_2);
    CV_CheckGlError();
}

صحيح cv::ogl::پروگرام_2::Impl::getAttributeLocation(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::GetAttribLocation(پروگرام_Id_2, name);
    CV_CheckGlError();
    أرجع location;
}

صحيح cv::ogl::پروگرام_2::Impl::get_اکسار_Location(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحيح location = gl::Get_اکسار_Location(پروگرام_Id_2, name);
    CV_CheckGlError();
    أرجع location;
}

#جے_انت // HAVE_OPENGL

cv::ogl::پروگرام_2::پروگرام_2()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_ = Impl::empty();
#جے_انت
}

cv::ogl::پروگرام_2::پروگرام_2(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(vert); CV_بن_ورتیا(frag); CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#جے_انت
}

فراغ cv::ogl::پروگرام_2::create(ثابت Shader& vert, ثابت Shader& frag, منطقي autoRelease)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(vert); CV_بن_ورتیا(frag); CV_بن_ورتیا(autoRelease);
    throw_no_ogl();
#وإلا
    impl_.reset(جديد Impl(vert, frag, autoRelease));
#جے_انت
}

فراغ cv::ogl::پروگرام_2::release()
{
#جے_تعریف_شدہ HAVE_OPENGL
    إذا (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_Auto_Release(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    impl_->سیٹ_Auto_Release(flag);
#جے_انت
}

فراغ cv::ogl::پروگرام_2::bind() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    impl_->bind();
#جے_انت
}

فراغ cv::ogl::پروگرام_2::unbind()
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    gl::Use_پروگرام(0);
#جے_انت
}

صحيح cv::ogl::پروگرام_2::getAttributeLocation(ثابت حرف* name) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->getAttributeLocation(name);
#جے_انت
}

صحيح cv::ogl::پروگرام_2::get_اکسار_Location(ثابت حرف* name) ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(name);
    throw_no_ogl();
#وإلا
    أرجع impl_->get_اکسار_Location(name);
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_اکسار_Vec3(صحيح loc, Vec3f vec)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(loc); CV_بن_ورتیا(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_اکسار_Vec2(صحيح loc, Vec2f vec)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(loc); CV_بن_ورتیا(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform2f(loc, vec(0), vec(1));
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_اکسار_Vec4(صحيح loc, Vec4f vec)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(loc); CV_بن_ورتیا(vec);
    throw_no_ogl();
#وإلا
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_اکسار_Mat4x4(صحيح loc, Matx44f mat)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(loc); CV_بن_ورتیا(mat);
    throw_no_ogl();
#وإلا
    gl::اکسار_Matrix4fv(loc, 1, سچ, mat.val);
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_Uniform1f(صحيح loc, عائم value)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(loc); CV_بن_ورتیا(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1f(loc, value);
#جے_انت
}

فراغ cv::ogl::پروگرام_2::سیٹ_Uniform1i(صحيح loc, صحيح value)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(loc); CV_بن_ورتیا(value);
    throw_no_ogl();
#وإلا
    gl::Uniform1i(loc, value);
#جے_انت
}

بنانشانی صحيح cv::ogl::پروگرام_2::پروگرام_Id() ثابت
{
#جے_بنا_تعریف HAVE_OPENGL
    throw_no_ogl();
#وإلا
    أرجع impl_->پروگرام_Id();
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// Rendering

فراغ cv::ogl::render(ثابت ogl::Texture2D& tex, Rect_<مزدوج> wndRect, Rect_<مزدوج> texRect)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(tex);
    CV_بن_ورتیا(wndRect);
    CV_بن_ورتیا(texRect);
    throw_no_ogl();
#وإلا
    إذا (!tex.empty())
    {
        gl::میٹرکس_منوال(gl::PROJECTION);
        gl::لدو_پچھان_تت();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::میٹرکس_منوال(gl::MODELVIEW);
        gl::لدو_پچھان_تت();
        CV_CheckGlError();

        gl::Disable(gl::LIGHTING);
        CV_CheckGlError();

        tex.bind();

        gl::Enable(gl::TEXTURE_2D);
        CV_CheckGlError();

        gl::TexEnvi(gl::TEXTURE_ENV, gl::TEXTURE_ENV_منوال, gl::REPLACE);
        CV_CheckGlError();

        gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::خطی);
        CV_CheckGlError();

        ثابت مزدوج راس[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.اچائی), 0.0,
            wndRect.x + wndRect.چوڑائی, (wndRect.y + wndRect.اچائی), 0.0,
            wndRect.x + wndRect.چوڑائی, wndRect.y, 0.0
        };
        ثابت مزدوج texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.اچائی,
            texRect.x + texRect.چوڑائی, texRect.y + texRect.اچائی,
            texRect.x + texRect.چوڑائی, texRect.y
        };

        ogl::بفر_2::unbind(ogl::بفر_2::ARRAY_بفر);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_اشارہ_گر(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::عام_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::راس_ARRAY);
        CV_CheckGlError();

        gl::راس_اشارہ_گر(3, gl::DOUBLE, 0, راس);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#جے_انت
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, صحيح منوال, Scalar color)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(منوال);
    CV_بن_ورتیا(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(منوال, 0, arr.size());
    }
#جے_انت
}

فراغ cv::ogl::render(ثابت ogl::Arrays& arr, داخلہ_Array indices, صحيح منوال, Scalar color)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(arr);
    CV_بن_ورتیا(indices);
    CV_بن_ورتیا(منوال);
    CV_بن_ورتیا(color);
    throw_no_ogl();
#وإلا
    إذا (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ثابت صحيح قسم = indices.قسم();

        بدل (قسم)
        {
        حالة داخلہ_Array_2::OPENGL_بفر :
            {
                ogl::بفر_2 buf = indices.get_OGl_بفر();

                ثابت صحيح ڈونگھائی = buf.ڈونگھائی();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( ڈونگھائی <= CV_32S );

                GLenum type;
                إذا (ڈونگھائی < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (ڈونگھائی < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                gl::DrawElements(منوال, buf.size().area(), type, 0);

                ogl::بفر_2::unbind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                اكسر;
            }

        افتراضي:
            {
                Mat mat = indices.getMat();

                ثابت صحيح ڈونگھائی = mat.ڈونگھائی();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( ڈونگھائی <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                إذا (ڈونگھائی < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                وإلا إذا (ڈونگھائی < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                وإلا
                    type = gl::UNSIGNED_INT;

                ogl::بفر_2::unbind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                gl::DrawElements(منوال, mat.size().area(), type, mat.انکڑے);
            }
        }
    }
#جے_انت
}

فراغ cv::ogl::clearColor(Scalar color)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(color);
    throw_no_ogl();
#وإلا
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_بفر_ثنائی_ہندسہ);
#جے_انت // HAVE_OPENGL
}

فراغ cv::ogl::clear_ڈونگھائی(عائم ڈونگھائی)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(ڈونگھائی);
    throw_no_ogl();
#وإلا
    gl::Clear_ڈونگھائی(ڈونگھائی);
    gl::Clear(gl::ڈونگھائی_بفر_ثنائی_ہندسہ);
#جے_انت // HAVE_OPENGL
}

فراغ cv::ogl::drawArrays(صحيح first, صحيح گنتی_2, صحيح منوال)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(first); CV_بن_ورتیا(گنتی_2); CV_بن_ورتیا(منوال);
    throw_no_ogl();
#وإلا
    gl::DrawArrays(منوال, first, گنتی_2);
#جے_انت // HAVE_OPENGL
}

فراغ cv::ogl::drawElements(صحيح first, صحيح گنتی_2, صحيح type, صحيح منوال)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(first); CV_بن_ورتیا(گنتی_2); CV_بن_ورتیا(type); CV_بن_ورتیا(منوال);
    throw_no_ogl();
#وإلا
    gl::DrawElements(منوال, گنتی_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)));
#جے_انت
}

فراغ cv::ogl::drawElementsInstanced(صحيح first, صحيح گنتی_2, صحيح type, صحيح منوال, صحيح instance_گنتی)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(first); CV_بن_ورتیا(گنتی_2); CV_بن_ورتیا(type); CV_بن_ورتیا(منوال); CV_بن_ورتیا(instance_گنتی);
    throw_no_ogl();
#وإلا
    gl::DrawElementsInstanced(منوال, گنتی_2, type, reinterpret_cast<ثابت فراغ*>(static_cast<size_t>(first)), instance_گنتی);
    CV_CheckGlError();
#جے_انت
}

فراغ cv::ogl::blendFunc(صحيح sfactor, صحيح dfactor)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(sfactor); CV_بن_ورتیا(dfactor);
    throw_no_ogl();
#وإلا
    gl::BlendFunc(sfactor, dfactor);
#جے_انت
}

فراغ cv::ogl::ڈونگھائی_نقاب_2(منطقي flag)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(flag);
    throw_no_ogl();
#وإلا
    gl::ڈونگھائی_نقاب(flag ? gl::TRUE_ : gl::FALSE_);
#جے_انت
}

فراغ cv::ogl::enable(صحيح cap)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(cap);
    throw_no_ogl();
#وإلا
    gl::Enable(cap);
#جے_انت
}

فراغ cv::ogl::disable(صحيح cap)
{
#جے_بنا_تعریف HAVE_OPENGL
    CV_بن_ورتیا(cap);
    throw_no_ogl();
#وإلا
    gl::Disable(cap);
#جے_انت
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#جے_تعریف_شدہ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  وإلا
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#وإلا // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#جے_انت // HAVE_OPENCL

#إذا defined(HAVE_OPENGL)
#  إذا defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#جے_انت // HAVE_OPENGL

نطاق cv { نطاق ogl {

#إذا defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ساكن فراغ checkOpenCLVersion()
{
    ورتوں نطاق cv::ocl;
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
    صحيح نسخہ_Major_2 = pi.نسخہ_Major_2();
    صحيح نسخہ_Minor_2 = pi.نسخہ_Minor_2();
    إذا (نسخہ_Major_2 < 1 || (نسخہ_Major_2 == 1 && نسخہ_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                نسخہ_Major_2, نسخہ_Minor_2, pi.name().c_str(), pi.نسخہ().c_str())
        );
}
#جے_انت

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
    cl_device_id* devices = خالی_سوچک;
    cl_uint dev_ورتیا = 0;
    cl_context context = خالی_سوچک;

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

    لكل (بنانشانی صحيح i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        حالت = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, عدم, &devCnt);
        إذا (حالت != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", حالت));

        حاول {
            devices = جديد cl_device_id[devCnt];

            حالت = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, عدم);
            إذا (حالت != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", حالت));

            لكل (بنانشانی صحيح j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_ناپ;
                حالت = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, عدم, &extension_ناپ );
                إذا (حالت != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", حالت));

                إذا(extension_ناپ > 0)
                {
                    std::string devString(extension_ناپ, '\0');
                    حالت = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_ناپ);
                    إذا (حالت != CL_SUCCESS)
                        استمر;
                    إذا (extension_ناپ > 0 && devString.size() >= extension_ناپ)
                        devString.resize(extension_ناپ - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    طالما (spacePos != devString.npos) {
                        إذا (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_ورتیا = i;
                            sharingSupported = سچ;
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
            إذا(devices != خالی_سوچک) {
                مٹاؤ[] devices;
            }
            استمر;
        }

        إذا (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#إذا defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_گروہ_Obj cgl_Share_گروہ = CGLGet_Share_گروہ(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_گروہ,
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
                context = clCreateContext(props, 1, &devices[dev_ورتیا], عدم, عدم, &حالت);
#وإلاإذا defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ورتیا], عدم, عدم, &حالت);
#وإلاإذا defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_ورتیا], عدم, عدم, &حالت);
#جے_انت
        }

        إذا (حالت != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", حالت));
        وإلا
            اكسر;
    }


    cl_platform_id platform = platforms[dev_ورتیا];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_ورتیا]);
    clReleaseDevice(devices[dev_ورتیا]);
    clReleaseContext(context);
    clExecCtx.bind();
    أرجع const_cast<Context&>(clExecCtx.getContext());
#جے_انت
}

} // namespace cv::ogl::ocl

فراغ convert_منزل_GLTexture2_D(داخلہ_Array src, Texture2D& texture)
{
    CV_بن_ورتیا(src); CV_بن_ورتیا(texture);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    ناپ src_ناپ = src.size();
    CV_Assert(src_ناپ.چوڑائی == (صحيح)texture.خانے_گنتی() && src_ناپ.اچائی == (صحيح)texture.صفاں());

    ورتوں نطاق cv::ocl;
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

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = cl_قطار_وچ_پاؤ_Acquire_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.خانے_گنتی, (size_t)u.صفاں, 1};
    حالت = cl_قطار_وچ_پاؤ_Copy_بفر_منزل_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", حالت));
    حالت = cl_قطار_وچ_پاؤ_Release_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت));

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت));

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت));
#جے_انت
}

فراغ convertFromGLTexture2D(ثابت Texture2D& texture, OutputArray dst)
{
    CV_بن_ورتیا(texture); CV_بن_ورتیا(dst);
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

    ورتوں نطاق cv::ocl;
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

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = cl_قطار_وچ_پاؤ_Acquire_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.خانے_گنتی, (size_t)u.صفاں, 1};
    حالت = cl_قطار_وچ_پاؤ_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", حالت));
    حالت = cl_قطار_وچ_پاؤ_Release_GLObjects(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت));

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت));

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت));
#جے_انت
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ثابت بفر_2& بفر, AccessFlag accessFlags)
{
    CV_بن_ورتیا(بفر); CV_بن_ورتیا(accessFlags);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    ورتوں نطاق cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_قطار cl_قطار = (cl_command_قطار)قطار::getDefault().ptr();

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

    gl::انت();

    حالت = cl_قطار_وچ_پاؤ_Acquire_GLObjects(cl_قطار, 1, &cl_بفر, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت));

    size_t قدم = بفر.خانے_گنتی() * بفر.elem_ناپ();
    صحيح صفاں = بفر.صفاں();
    صحيح خانے_گنتی = بفر.خانے_گنتی();
    صحيح type = بفر.type();

    UMat u;
    convert_From_بفر(cl_بفر, قدم, صفاں, خانے_گنتی, type, u);
    أرجع u;
#جے_انت
}

فراغ unmapGLBuffer(UMat& u)
{
    CV_بن_ورتیا(u);
#إذا !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#وإلا
    ورتوں نطاق cv::ocl;
    cl_command_قطار cl_قطار = (cl_command_قطار)قطار::getDefault().ptr();

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int حالت = cl_قطار_وچ_پاؤ_Release_GLObjects(cl_قطار, 1, &cl_بفر, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت));

    حالت = cl_انت(cl_قطار);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت));

    حالت = clReleaseMemObject(cl_بفر);
    إذا (حالت != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت));
#جے_انت
}

}} // namespace cv::ogl
