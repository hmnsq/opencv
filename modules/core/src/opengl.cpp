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

#شامل "precomp.hpp"

#اگر defined (__APPLE__) || defined(MACOSX)
   #تعریف_کرو GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#ورنہ
   #تعریف_کرو GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENGL
#  include "gl_core_3_1.hpp"
#  ifdef HAVE_CUDA
#    اگر (defined(__arm__) || defined(__aarch64__)) \
         && !defined(OPENCV_SKIP_CUDA_OPENGL_ARM_WORKAROUND)
#      include <GL/gl.h>
#      ifndef GL_نسخہ
#        define GL_نسخہ 0x1F02
#      endif
#    endif
#    include <cuda_gl_interop.h>
#  endif
#ورنہ // HAVE_OPENGL
#  define NO_OPENGL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenGL support")
#اگر_ختم // HAVE_OPENGL

استعمال نامخلا cv;
استعمال نامخلا cv::cuda;

#اگر defined(_MSC_VER)
#ہدایت warning(disable : 4702)  // unreachable code
#اگر_ختم

نامخلا
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
اندرونی ساکن CV_NORETURN خالی throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#وگرنہ_اگر defined _DEBUG
اندرونی ساکن منطقی checkError(ثابت حرف* file, ثابت صحیح سطر, ثابت حرف* func = 0)
{
    GLenum err = gl::GetError();
    اگر (err != gl::NO_ERROR_)
    {
        ثابت حرف* پیغام;
        انتخاب (err)
        {
        حالت gl::INVALID_ENUM:
            پیغام = "An unacceptable value is specified for an enumerated argument";
            توڑو;
        حالت gl::INVALID_قدر:
            پیغام = "A numeric argument is out of range";
            توڑو;
        حالت gl::INVALID_OPERATION:
            پیغام = "The specified operation is not allowed in the current state";
            توڑو;
        حالت gl::برآمد_کا_حافظہ:
            پیغام = "There is not enough memory left to execute the command";
            توڑو;
        طےشدہ:
            پیغام = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, پیغام, file, سطر);
    }
    واپس سچ;
}
#اگر_ختم // HAVE_OPENGL
} // namespace

#تعریف_کرو CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#اگر_تعریف_شدہ HAVE_OPENGL
نامخلا
{
    ثابت GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#اگر_ختم

////////////////////////////////////////////////////////////////////////
// setGlDevice

خالی cv::cuda::سیٹ_Gl_Device(صحیح device)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(device);
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        CV_غیر_مستعمل(device);
        throw_no_cuda();
    #ورنہ
        cuda_محفوظ_Call( cudaGLSetGLDevice(device) );
    #اگر_ختم
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// CudaResource

#اگر defined(HAVE_OPENGL) && defined(HAVE_CUDA)

نامخلا
{
    جماعت CudaResource
    {
    عوامی:
        CudaResource();
        ~CudaResource();

        خالی رجسٹر_بفر(GLuint بفر);
        خالی release();

        خالی copyFrom(ثابت خالی* src, size_t spitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream = 0);
        خالی copy_مقصد(خالی* dst, size_t dpitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream = 0);

        خالی* map(cuda_دھارا_t stream = 0);
        خالی unmap(cuda_دھارا_t stream = 0);

    نجی:
        cudaGraphicsResource_t resource_;
        GLuint بفر_3;

        جماعت GraphicsMapHolder;
    };

    CudaResource::CudaResource() : resource_(0), بفر_3(0)
    {
    }

    CudaResource::~CudaResource()
    {
        release();
    }

    خالی CudaResource::رجسٹر_بفر(GLuint بفر)
    {
        CV_DbgAssert( بفر != 0 );

        اگر (بفر_3 == بفر)
            واپس;

        cudaGraphicsResource_t resource;
        cuda_محفوظ_Call( cuda_Graphics_GLRegister_بفر(&resource, بفر, cudaGraphicsMapFlagsNone) );

        release();

        resource_ = resource;
        بفر_3 = بفر;
    }

    خالی CudaResource::release()
    {
        اگر (resource_)
            cudaGraphicsUnregisterResource(resource_);

        resource_ = 0;
        بفر_3 = 0;
    }

    جماعت CudaResource::GraphicsMapHolder
    {
    عوامی:
        GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_دھارا_t stream);
        ~GraphicsMapHolder();

        خالی reset();

    نجی:
        cudaGraphicsResource_t* resource_;
        cuda_دھارا_t دھارا_3;
    };

    CudaResource::GraphicsMapHolder::GraphicsMapHolder(cudaGraphicsResource_t* resource, cuda_دھارا_t stream) : resource_(resource), دھارا_3(stream)
    {
        اگر (resource_)
            cuda_محفوظ_Call( cudaGraphicsMapResources(1, resource_, دھارا_3) );
    }

    CudaResource::GraphicsMapHolder::~GraphicsMapHolder()
    {
        اگر (resource_)
            cudaGraphicsUnmapResources(1, resource_, دھارا_3);
    }

    خالی CudaResource::GraphicsMapHolder::reset()
    {
        resource_ = 0;
    }

    خالی CudaResource::copyFrom(ثابت خالی* src, size_t spitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_غیر_مستعمل(h);

        خالی* dst;
        size_t size;
        cuda_محفوظ_Call( cuda_Graphics_Resource_Get_Mapped_اشارہ_گر(&dst, &size, resource_) );

        CV_DbgAssert( چوڑائی * اونچائی == size );

        اگر (stream == 0)
            cuda_محفوظ_Call( cudaMemcpy2D(dst, چوڑائی, src, spitch, چوڑائی, اونچائی, cuda_Memcpy_Device_مقصد_Device) );
        ورنہ
            cuda_محفوظ_Call( cudaMemcpy2DAsync(dst, چوڑائی, src, spitch, چوڑائی, اونچائی, cuda_Memcpy_Device_مقصد_Device, stream) );
    }

    خالی CudaResource::copy_مقصد(خالی* dst, size_t dpitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);
        CV_غیر_مستعمل(h);

        خالی* src;
        size_t size;
        cuda_محفوظ_Call( cuda_Graphics_Resource_Get_Mapped_اشارہ_گر(&src, &size, resource_) );

        CV_DbgAssert( چوڑائی * اونچائی == size );

        اگر (stream == 0)
            cuda_محفوظ_Call( cudaMemcpy2D(dst, dpitch, src, چوڑائی, چوڑائی, اونچائی, cuda_Memcpy_Device_مقصد_Device) );
        ورنہ
            cuda_محفوظ_Call( cudaMemcpy2DAsync(dst, dpitch, src, چوڑائی, چوڑائی, اونچائی, cuda_Memcpy_Device_مقصد_Device, stream) );
    }

    خالی* CudaResource::map(cuda_دھارا_t stream)
    {
        CV_DbgAssert( resource_ != 0 );

        GraphicsMapHolder h(&resource_, stream);

        خالی* ptr;
        size_t size;
        cuda_محفوظ_Call( cuda_Graphics_Resource_Get_Mapped_اشارہ_گر(&ptr, &size, resource_) );

        h.reset();

        واپس ptr;
    }

    خالی CudaResource::unmap(cuda_دھارا_t stream)
    {
        CV_Assert( resource_ != 0 );

        cudaGraphicsUnmapResources(1, &resource_, stream);
    }
}

#اگر_ختم

////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#اگر_غیر_تعریف_شدہ HAVE_OPENGL

جماعت cv::ogl::بفر_2::Impl
{
};

#ورنہ

جماعت cv::ogl::بفر_2::Impl
{
عوامی:
    ساکن ثابت Ptr<Impl>& empty();

    Impl(GLuint bufId, منطقی autoRelease);
    Impl(GLsizeiptr size, ثابت GLvoid* ڈیٹا, GLenum target, منطقی autoRelease);
    ~Impl();

    خالی bind(GLenum target) ثابت;

    خالی copyFrom(GLuint srcBuf, GLsizeiptr size);

    خالی copyFrom(GLsizeiptr size, ثابت GLvoid* ڈیٹا);
    خالی copy_مقصد(GLsizeiptr size, GLvoid* ڈیٹا) ثابت;

    خالی resize(GLsizeiptr size, GLenum target);

    خالی* mapHost(GLenum access);
    خالی unmapHost();

#اگر_تعریف_شدہ HAVE_CUDA
    خالی copyFrom(ثابت خالی* src, size_t spitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream = 0);
    خالی copy_مقصد(خالی* dst, size_t dpitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream = 0) ثابت;

    خالی* mapDevice(cuda_دھارا_t stream = 0);
    خالی unmapDevice(cuda_دھارا_t stream = 0);
#اگر_ختم

    خالی سیٹ_Auto_Release(منطقی flag) { autoRelease_ = flag; }

    GLuint bufId() ثابت { واپس bufId_; }

نجی:
    Impl();

    GLuint bufId_;
    منطقی autoRelease_;

#اگر_تعریف_شدہ HAVE_CUDA
    تغیرپذیر CudaResource cudaResource_;
#اگر_ختم
};

ثابت Ptr<cv::ogl::بفر_2::Impl>& cv::ogl::بفر_2::Impl::empty()
{
    ساکن Ptr<Impl> p(نیا Impl);
    واپس p;
}

cv::ogl::بفر_2::Impl::Impl() : bufId_(0), autoRelease_(جھوٹ)
{
}

cv::ogl::بفر_2::Impl::Impl(GLuint abufId, منطقی autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_بفر(abufId) == gl::TRUE_ );
}

cv::ogl::بفر_2::Impl::Impl(GLsizeiptr size, ثابت GLvoid* ڈیٹا, GLenum target, منطقی autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    CV_CheckGlError();

    CV_Assert( bufId_ != 0 );

    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();

    gl::بفر_ڈیٹا(target, size, ڈیٹا, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
}

cv::ogl::بفر_2::Impl::~Impl()
{
    اگر (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

خالی cv::ogl::بفر_2::Impl::bind(GLenum target) ثابت
{
    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();
}

خالی cv::ogl::بفر_2::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::Bind_بفر(gl::COPY_WRITE_بفر, bufId_);
    CV_CheckGlError();

    gl::Bind_بفر(gl::COPY_READ_بفر, srcBuf);
    CV_CheckGlError();

    gl::Copy_بفر_Sub_ڈیٹا(gl::COPY_READ_بفر, gl::COPY_WRITE_بفر, 0, 0, size);
    CV_CheckGlError();
}

خالی cv::ogl::بفر_2::Impl::copyFrom(GLsizeiptr size, ثابت GLvoid* ڈیٹا)
{
    gl::Bind_بفر(gl::COPY_WRITE_بفر, bufId_);
    CV_CheckGlError();

    gl::بفر_Sub_ڈیٹا(gl::COPY_WRITE_بفر, 0, size, ڈیٹا);
    CV_CheckGlError();
}

خالی cv::ogl::بفر_2::Impl::resize(GLsizeiptr size, GLenum target)
{
    gl::Bind_بفر(target, bufId_);
    CV_CheckGlError();

    // keep the same buffer id so bound vertex arrays stay valid
    gl::بفر_ڈیٹا(target, size, 0, gl::DYNAMIC_DRAW);
    CV_CheckGlError();

    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
}

خالی cv::ogl::بفر_2::Impl::copy_مقصد(GLsizeiptr size, GLvoid* ڈیٹا) ثابت
{
    gl::Bind_بفر(gl::COPY_READ_بفر, bufId_);
    CV_CheckGlError();

    gl::Get_بفر_Sub_ڈیٹا(gl::COPY_READ_بفر, 0, size, ڈیٹا);
    CV_CheckGlError();
}

خالی* cv::ogl::بفر_2::Impl::mapHost(GLenum access)
{
    gl::Bind_بفر(gl::COPY_READ_بفر, bufId_);
    CV_CheckGlError();

    GLvoid* ڈیٹا = gl::Map_بفر(gl::COPY_READ_بفر, access);
    CV_CheckGlError();

    واپس ڈیٹا;
}

خالی cv::ogl::بفر_2::Impl::unmapHost()
{
    gl::Unmap_بفر(gl::COPY_READ_بفر);
}

#اگر_تعریف_شدہ HAVE_CUDA

خالی cv::ogl::بفر_2::Impl::copyFrom(ثابت خالی* src, size_t spitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream)
{
    cudaResource_.رجسٹر_بفر(bufId_);
    cudaResource_.copyFrom(src, spitch, چوڑائی, اونچائی, stream);
}

خالی cv::ogl::بفر_2::Impl::copy_مقصد(خالی* dst, size_t dpitch, size_t چوڑائی, size_t اونچائی, cuda_دھارا_t stream) ثابت
{
    cudaResource_.رجسٹر_بفر(bufId_);
    cudaResource_.copy_مقصد(dst, dpitch, چوڑائی, اونچائی, stream);
}

خالی* cv::ogl::بفر_2::Impl::mapDevice(cuda_دھارا_t stream)
{
    cudaResource_.رجسٹر_بفر(bufId_);
    واپس cudaResource_.map(stream);
}

خالی cv::ogl::بفر_2::Impl::unmapDevice(cuda_دھارا_t stream)
{
    cudaResource_.unmap(stream);
}

#اگر_ختم // HAVE_CUDA

#اگر_ختم // HAVE_OPENGL

cv::ogl::بفر_2::بفر_2() : صفیں_2(0), ستون_تعداد_2(0), type_(0)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
#اگر_ختم
}

cv::ogl::بفر_2::بفر_2(صحیح arows, صحیح acols, صحیح atype, غیرعلامتی صحیح abufId, منطقی autoRelease) : صفیں_2(0), ستون_تعداد_2(0), type_(0)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arows);
    CV_غیر_مستعمل(acols);
    CV_غیر_مستعمل(atype);
    CV_غیر_مستعمل(abufId);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(abufId, autoRelease));
    صفیں_2 = arows;
    ستون_تعداد_2 = acols;
    type_ = atype;
#اگر_ختم
}

cv::ogl::بفر_2::بفر_2(سائز asize, صحیح atype, غیرعلامتی صحیح abufId, منطقی autoRelease) : صفیں_2(0), ستون_تعداد_2(0), type_(0)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(asize);
    CV_غیر_مستعمل(atype);
    CV_غیر_مستعمل(abufId);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(abufId, autoRelease));
    صفیں_2 = asize.اونچائی;
    ستون_تعداد_2 = asize.چوڑائی;
    type_ = atype;
#اگر_ختم
}

cv::ogl::بفر_2::بفر_2(داخلہ_Array arr, ہدف target, منطقی autoRelease) : صفیں_2(0), ستون_تعداد_2(0), type_(0)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(target);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    ثابت صحیح قسم = arr.قسم();

    انتخاب (قسم)
    {
    حالت داخلہ_Array_2::OPENGL_بفر:
    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        توڑو;

    طےشدہ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ثابت GLsizeiptr asize = mat.صفیں * mat.ستون_تعداد * mat.elem_سائز();
            impl_.reset(نیا Impl(asize, mat.ڈیٹا, target, autoRelease));
            صفیں_2 = mat.صفیں;
            ستون_تعداد_2 = mat.ستون_تعداد;
            type_ = mat.type();
            توڑو;
        }
    }
#اگر_ختم
}

خالی cv::ogl::بفر_2::create(صحیح arows, صحیح acols, صحیح atype, ہدف target, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arows);
    CV_غیر_مستعمل(acols);
    CV_غیر_مستعمل(atype);
    CV_غیر_مستعمل(target);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    اگر (صفیں_2 != arows || ستون_تعداد_2 != acols || type_ != atype)
    {
        ثابت GLsizeiptr asize = (GLsizeiptr)arows * acols * CV_ELEM_سائز(atype);
        // resize in place when sole owner (keeps the GL id); else allocate fresh (copy-on-write)
        اگر (impl_->bufId() != 0 && impl_.use_count() == 1)
        {
            impl_->resize(asize, target);
            impl_->سیٹ_Auto_Release(autoRelease);
        }
        ورنہ
            impl_.reset(نیا Impl(asize, 0, target, autoRelease));
        صفیں_2 = arows;
        ستون_تعداد_2 = acols;
        type_ = atype;
    }
#اگر_ختم
}

خالی cv::ogl::بفر_2::release()
{
#اگر_تعریف_شدہ HAVE_OPENGL
    اگر (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
    صفیں_2 = 0;
    ستون_تعداد_2 = 0;
    type_ = 0;
#اگر_ختم
}

خالی cv::ogl::بفر_2::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    impl_->سیٹ_Auto_Release(flag);
#اگر_ختم
}

خالی cv::ogl::بفر_2::copyFrom(داخلہ_Array arr, ہدف target, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(target);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    ثابت صحیح قسم = arr.قسم();

    ثابت سائز asize = arr.size();
    ثابت صحیح atype = arr.type();
    create(asize, atype, target, autoRelease);

    انتخاب (قسم)
    {
    حالت داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_سائز(atype));
            توڑو;
        }

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #اگر_غیر_تعریف_شدہ HAVE_CUDA
                throw_no_cuda();
            #ورنہ
                GpuMat dmat = arr.getGpuMat();
                impl_->copyFrom(dmat.ڈیٹا, dmat.قدم, dmat.ستون_تعداد * dmat.elem_سائز(), dmat.صفیں);
            #اگر_ختم

            توڑو;
        }

    طےشدہ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_سائز(atype), mat.ڈیٹا);
        }
    }
#اگر_ختم
}

خالی cv::ogl::بفر_2::copyFrom(داخلہ_Array arr, cuda::دھارا& stream, ہدف target, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(stream);
    CV_غیر_مستعمل(target);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        CV_غیر_مستعمل(arr);
        CV_غیر_مستعمل(stream);
        CV_غیر_مستعمل(target);
        CV_غیر_مستعمل(autoRelease);
        throw_no_cuda();
    #ورنہ
        GpuMat dmat = arr.getGpuMat();

        create(dmat.size(), dmat.type(), target, autoRelease);

        impl_->copyFrom(dmat.ڈیٹا, dmat.قدم, dmat.ستون_تعداد * dmat.elem_سائز(), dmat.صفیں, cuda::دھارا_Accessor::get_دھارا(stream));
    #اگر_ختم
#اگر_ختم
}

خالی cv::ogl::بفر_2::copy_مقصد(OutputArray arr) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    throw_no_ogl();
#ورنہ
    ثابت صحیح قسم = arr.قسم();

    انتخاب (قسم)
    {
    حالت داخلہ_Array_2::OPENGL_بفر:
        {
            arr.get_OGl_بفر_Ref().copyFrom(*یہ);
            توڑو;
        }

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #اگر_غیر_تعریف_شدہ HAVE_CUDA
                throw_no_cuda();
            #ورنہ
                GpuMat& dmat = arr.getGpuMatRef();
                dmat.create(صفیں_2, ستون_تعداد_2, type_);
                impl_->copy_مقصد(dmat.ڈیٹا, dmat.قدم, dmat.ستون_تعداد * dmat.elem_سائز(), dmat.صفیں);
            #اگر_ختم

            توڑو;
        }

    طےشدہ:
        {
            arr.create(صفیں_2, ستون_تعداد_2, type_);
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copy_مقصد(mat.صفیں * mat.ستون_تعداد * mat.elem_سائز(), mat.ڈیٹا);
        }
    }
#اگر_ختم
}

خالی cv::ogl::بفر_2::copy_مقصد(OutputArray arr, cuda::دھارا& stream) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(stream);
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        CV_غیر_مستعمل(arr);
        CV_غیر_مستعمل(stream);
        throw_no_cuda();
    #ورنہ
        arr.create(صفیں_2, ستون_تعداد_2, type_);
        GpuMat dmat = arr.getGpuMat();
        impl_->copy_مقصد(dmat.ڈیٹا, dmat.قدم, dmat.ستون_تعداد * dmat.elem_سائز(), dmat.صفیں, cuda::دھارا_Accessor::get_دھارا(stream));
    #اگر_ختم
#اگر_ختم
}

cv::ogl::بفر_2 cv::ogl::بفر_2::clone(ہدف target, منطقی autoRelease) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(target);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    ogl::بفر_2 buf;
    buf.copyFrom(*یہ, target, autoRelease);
    واپس buf;
#اگر_ختم
}

خالی cv::ogl::بفر_2::bind(ہدف target) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(target);
    throw_no_ogl();
#ورنہ
    impl_->bind(target);
#اگر_ختم
}

خالی cv::ogl::بفر_2::unbind(ہدف target)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(target);
    throw_no_ogl();
#ورنہ
    gl::Bind_بفر(target, 0);
    CV_CheckGlError();
#اگر_ختم
}

Mat cv::ogl::بفر_2::mapHost(Access access)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(access);
    throw_no_ogl();
#ورنہ
    واپس Mat(صفیں_2, ستون_تعداد_2, type_, impl_->mapHost(access));
#اگر_ختم
}

خالی cv::ogl::بفر_2::unmapHost()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->unmapHost();
#اگر_ختم
}

GpuMat cv::ogl::بفر_2::mapDevice()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        throw_no_cuda();
    #ورنہ
        واپس GpuMat(صفیں_2, ستون_تعداد_2, type_, impl_->mapDevice());
    #اگر_ختم
#اگر_ختم
}

خالی cv::ogl::بفر_2::unmapDevice()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        throw_no_cuda();
    #ورنہ
        impl_->unmapDevice();
    #اگر_ختم
#اگر_ختم
}

cuda::GpuMat cv::ogl::بفر_2::mapDevice(cuda::دھارا& stream)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(stream);
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        CV_غیر_مستعمل(stream);
        throw_no_cuda();
    #ورنہ
        واپس GpuMat(صفیں_2, ستون_تعداد_2, type_, impl_->mapDevice(cuda::دھارا_Accessor::get_دھارا(stream)));
    #اگر_ختم
#اگر_ختم
}

خالی cv::ogl::بفر_2::unmapDevice(cuda::دھارا& stream)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(stream);
    throw_no_ogl();
#ورنہ
    #اگر_غیر_تعریف_شدہ HAVE_CUDA
        CV_غیر_مستعمل(stream);
        throw_no_cuda();
    #ورنہ
        impl_->unmapDevice(cuda::دھارا_Accessor::get_دھارا(stream));
    #اگر_ختم
#اگر_ختم
}

غیرعلامتی صحیح cv::ogl::بفر_2::bufId() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->bufId();
#اگر_ختم
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#اگر_غیر_تعریف_شدہ HAVE_OPENGL

جماعت cv::ogl::Texture2D::Impl
{
};

#ورنہ

جماعت cv::ogl::Texture2D::Impl
{
عوامی:
    ساکن ثابت Ptr<Impl> empty();

    Impl(GLuint texId, منطقی autoRelease);
    Impl(GLint اندرونی_Format, GLsizei چوڑائی, GLsizei اونچائی, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقی autoRelease);
    ~Impl();

    خالی copyFrom(GLsizei چوڑائی, GLsizei اونچائی, GLenum format, GLenum type, ثابت GLvoid *pixels);
    خالی copy_مقصد(GLenum format, GLenum type, GLvoid* pixels) ثابت;

    خالی bind() ثابت;

    خالی سیٹ_Auto_Release(منطقی flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { واپس texId_; }

نجی:
    Impl();

    GLuint texId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    ساکن Ptr<Impl> p(نیا Impl);
    واپس p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(جھوٹ)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, منطقی autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint اندرونی_Format, GLsizei چوڑائی, GLsizei اونچائی, GLenum format, GLenum type, ثابت GLvoid* pixels, منطقی autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    CV_CheckGlError();

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexImage2D(gl::TEXTURE_2D, 0, اندرونی_Format, چوڑائی, اونچائی, 0, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

cv::ogl::Texture2D::Impl::~Impl()
{
    اگر (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

خالی cv::ogl::Texture2D::Impl::copyFrom(GLsizei چوڑائی, GLsizei اونچائی, GLenum format, GLenum type, ثابت GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, چوڑائی, اونچائی, format, type, pixels);
    CV_CheckGlError();

    gl::GenerateMipmap(gl::TEXTURE_2D);
    CV_CheckGlError();
}

خالی cv::ogl::Texture2D::Impl::copy_مقصد(GLenum format, GLenum type, GLvoid* pixels) ثابت
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    CV_CheckGlError();

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    CV_CheckGlError();
}

خالی cv::ogl::Texture2D::Impl::bind() ثابت
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    CV_CheckGlError();
}

#اگر_ختم // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : صفیں_2(0), ستون_تعداد_2(0), format_(NONE)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
#اگر_ختم
}

cv::ogl::Texture2D::Texture2D(صحیح arows, صحیح acols, Format aformat, غیرعلامتی صحیح atexId, منطقی autoRelease) : صفیں_2(0), ستون_تعداد_2(0), format_(NONE)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arows);
    CV_غیر_مستعمل(acols);
    CV_غیر_مستعمل(aformat);
    CV_غیر_مستعمل(atexId);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(atexId, autoRelease));
    صفیں_2 = arows;
    ستون_تعداد_2 = acols;
    format_ = aformat;
#اگر_ختم
}

cv::ogl::Texture2D::Texture2D(سائز asize, Format aformat, غیرعلامتی صحیح atexId, منطقی autoRelease) : صفیں_2(0), ستون_تعداد_2(0), format_(NONE)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(asize);
    CV_غیر_مستعمل(aformat);
    CV_غیر_مستعمل(atexId);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(atexId, autoRelease));
    صفیں_2 = asize.اونچائی;
    ستون_تعداد_2 = asize.چوڑائی;
    format_ = aformat;
#اگر_ختم
}

cv::ogl::Texture2D::Texture2D(داخلہ_Array arr, منطقی autoRelease) : صفیں_2(0), ستون_تعداد_2(0), format_(NONE)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    ثابت صحیح قسم = arr.قسم();

    ثابت سائز asize = arr.size();
    ثابت صحیح atype = arr.type();

    ثابت صحیح گہرائی = CV_MAT_گہرائی(atype);
    ثابت صحیح cn = CV_MAT_CN(atype);

    CV_Assert( گہرائی <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format اندرونی_Formats[] =
    {
        NONE, گہرائی_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::گہرائی_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    انتخاب (قسم)
    {
    حالت داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_.reset(نیا Impl(اندرونی_Formats[cn], asize.چوڑائی, asize.اونچائی, srcFormats[cn], gl_types[گہرائی], 0, autoRelease));
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            توڑو;
        }

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #اگر_غیر_تعریف_شدہ HAVE_CUDA
                throw_no_cuda();
            #ورنہ
                GpuMat dmat = arr.getGpuMat();
                ogl::بفر_2 buf(dmat, ogl::بفر_2::PIXEL_UNPACK_بفر);
                buf.سیٹ_Auto_Release(سچ);
                buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
                impl_.reset(نیا Impl(اندرونی_Formats[cn], asize.چوڑائی, asize.اونچائی, srcFormats[cn], gl_types[گہرائی], 0, autoRelease));
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            #اگر_ختم

            توڑو;
        }

    طےشدہ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_.reset(نیا Impl(اندرونی_Formats[cn], asize.چوڑائی, asize.اونچائی, srcFormats[cn], gl_types[گہرائی], mat.ڈیٹا, autoRelease));
            توڑو;
        }
    }

    صفیں_2 = asize.اونچائی;
    ستون_تعداد_2 = asize.چوڑائی;
    format_ = اندرونی_Formats[cn];
#اگر_ختم
}

خالی cv::ogl::Texture2D::create(صحیح arows, صحیح acols, Format aformat, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arows);
    CV_غیر_مستعمل(acols);
    CV_غیر_مستعمل(aformat);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    اگر (صفیں_2 != arows || ستون_تعداد_2 != acols || format_ != aformat)
    {
        ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
        impl_.reset(نیا Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        صفیں_2 = arows;
        ستون_تعداد_2 = acols;
        format_ = aformat;
    }
#اگر_ختم
}

خالی cv::ogl::Texture2D::release()
{
#اگر_تعریف_شدہ HAVE_OPENGL
    اگر (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
    صفیں_2 = 0;
    ستون_تعداد_2 = 0;
    format_ = NONE;
#اگر_ختم
}

خالی cv::ogl::Texture2D::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    impl_->سیٹ_Auto_Release(flag);
#اگر_ختم
}

خالی cv::ogl::Texture2D::copyFrom(داخلہ_Array arr, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    ثابت صحیح قسم = arr.قسم();

    ثابت سائز asize = arr.size();
    ثابت صحیح atype = arr.type();

    ثابت صحیح گہرائی = CV_MAT_گہرائی(atype);
    ثابت صحیح cn = CV_MAT_CN(atype);

    CV_Assert( گہرائی <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    ثابت Format اندرونی_Formats[] =
    {
        NONE, گہرائی_COMPONENT, NONE, RGB, RGBA
    };
    ثابت GLenum srcFormats[] =
    {
        0, gl::گہرائی_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, اندرونی_Formats[cn], autoRelease);

    انتخاب(قسم)
    {
    حالت داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2 buf = arr.get_OGl_بفر();
            buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_->copyFrom(asize.چوڑائی, asize.اونچائی, srcFormats[cn], gl_types[گہرائی], 0);
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            توڑو;
        }

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #اگر_غیر_تعریف_شدہ HAVE_CUDA
                throw_no_cuda();
            #ورنہ
                GpuMat dmat = arr.getGpuMat();
                ogl::بفر_2 buf(dmat, ogl::بفر_2::PIXEL_UNPACK_بفر);
                buf.سیٹ_Auto_Release(سچ);
                buf.bind(ogl::بفر_2::PIXEL_UNPACK_بفر);
                impl_->copyFrom(asize.چوڑائی, asize.اونچائی, srcFormats[cn], gl_types[گہرائی], 0);
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            #اگر_ختم

            توڑو;
        }

    طےشدہ:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_UNPACK_بفر);
            impl_->copyFrom(asize.چوڑائی, asize.اونچائی, srcFormats[cn], gl_types[گہرائی], mat.ڈیٹا);
        }
    }
#اگر_ختم
}

خالی cv::ogl::Texture2D::copy_مقصد(OutputArray arr, صحیح ddepth, منطقی autoRelease) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(ddepth);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    ثابت صحیح قسم = arr.قسم();

    ثابت صحیح cn = format_ == گہرائی_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    ثابت GLenum dstFormat = format_ == گہرائی_COMPONENT ? gl::گہرائی_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    انتخاب(قسم)
    {
    حالت داخلہ_Array_2::OPENGL_بفر:
        {
            ogl::بفر_2& buf = arr.get_OGl_بفر_Ref();
            buf.create(صفیں_2, ستون_تعداد_2, CV_MAKE_TYPE(ddepth, cn), ogl::بفر_2::PIXEL_PACK_بفر, autoRelease);
            buf.bind(ogl::بفر_2::PIXEL_PACK_بفر);
            impl_->copy_مقصد(dstFormat, gl_types[ddepth], 0);
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
            توڑو;
        }

    حالت داخلہ_Array_2::CUDA_GPU_MAT:
        {
            #اگر_غیر_تعریف_شدہ HAVE_CUDA
                throw_no_cuda();
            #ورنہ
                ogl::بفر_2 buf(صفیں_2, ستون_تعداد_2, CV_MAKE_TYPE(ddepth, cn), ogl::بفر_2::PIXEL_PACK_بفر);
                buf.سیٹ_Auto_Release(سچ);
                buf.bind(ogl::بفر_2::PIXEL_PACK_بفر);
                impl_->copy_مقصد(dstFormat, gl_types[ddepth], 0);
                ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
                buf.copy_مقصد(arr);
            #اگر_ختم

            توڑو;
        }

    طےشدہ:
        {
            arr.create(صفیں_2, ستون_تعداد_2, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::بفر_2::unbind(ogl::بفر_2::PIXEL_PACK_بفر);
            impl_->copy_مقصد(dstFormat, gl_types[ddepth], mat.ڈیٹا);
        }
    }
#اگر_ختم
}

خالی cv::ogl::Texture2D::bind() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_->bind();
#اگر_ختم
}

غیرعلامتی صحیح cv::ogl::Texture2D::texId() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->texId();
#اگر_ختم
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::TextureBuffer

#اگر_غیر_تعریف_شدہ HAVE_OPENGL

جماعت cv::ogl::Texture_بفر::Impl
{
};

#ورنہ

جماعت cv::ogl::Texture_بفر::Impl
{
عوامی:
    ساکن ثابت Ptr<Impl> empty();

    Impl();
    Impl(GLenum اندرونی_Format, GLuint bufId, منطقی autoRelease);
    ~Impl();

    خالی bind(GLint unit) ثابت;

    خالی سیٹ_Auto_Release(منطقی flag) { autoRelease_ = flag; }

    GLuint texId() ثابت { واپس texId_; }

نجی:
    GLuint texId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::Texture_بفر::Impl> cv::ogl::Texture_بفر::Impl::empty()
{
    ساکن Ptr<Impl> p = makePtr<Impl>();
    واپس p;
}

cv::ogl::Texture_بفر::Impl::Impl() : texId_(0), autoRelease_(جھوٹ)
{
}

cv::ogl::Texture_بفر::Impl::Impl(GLenum اندرونی_Format, GLuint bufId, منطقی autoRelease) : texId_(0), autoRelease_(autoRelease)
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
    اگر (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

خالی cv::ogl::Texture_بفر::Impl::bind(GLint unit) ثابت
{
    gl::فعال_Texture(gl::TEXTURE0 + unit);
    CV_CheckGlError();

    gl::BindTexture(gl::TEXTURE_بفر, texId_);
    CV_CheckGlError();
}

#اگر_ختم // HAVE_OPENGL

cv::ogl::Texture_بفر::Texture_بفر() : format_(NONE)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
#اگر_ختم
}

cv::ogl::Texture_بفر::Texture_بفر(ثابت بفر_2& buf, Format aformat, منطقی autoRelease) : format_(NONE)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(buf);
    CV_غیر_مستعمل(aformat);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
    create(buf, aformat, autoRelease);
#اگر_ختم
}

خالی cv::ogl::Texture_بفر::create(ثابت بفر_2& buf, Format aformat, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(buf);
    CV_غیر_مستعمل(aformat);
    CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    CV_Assert( !buf.empty() );
    CV_Assert( aformat != NONE );

    impl_ = makePtr<Impl>(aformat, buf.bufId(), autoRelease);
    buf_ = buf;
    format_ = aformat;
#اگر_ختم
}

خالی cv::ogl::Texture_بفر::release()
{
#اگر_تعریف_شدہ HAVE_OPENGL
    اگر (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#اگر_ختم
    buf_ = بفر_2();
    format_ = NONE;
}

خالی cv::ogl::Texture_بفر::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    impl_->سیٹ_Auto_Release(flag);
#اگر_ختم
}

خالی cv::ogl::Texture_بفر::bind(صحیح unit) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(unit);
    throw_no_ogl();
#ورنہ
    CV_Assert( unit >= 0 );
    impl_->bind(unit);
#اگر_ختم
}

cv::ogl::Texture_بفر::Format cv::ogl::Texture_بفر::format() ثابت
{
    واپس format_;
}

منطقی cv::ogl::Texture_بفر::empty() ثابت
{
    واپس format_ == NONE;
}

غیرعلامتی صحیح cv::ogl::Texture_بفر::texId() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->texId();
#اگر_ختم
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

خالی cv::ogl::Arrays::سیٹ_راس_Array(داخلہ_Array راس)
{
    ثابت صحیح cn = راس.channels();
    ثابت صحیح گہرائی = راس.گہرائی();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( گہرائی == CV_16S || گہرائی == CV_32S || گہرائی == CV_32F || گہرائی == CV_64F );

    اگر (راس.قسم() == داخلہ_Array_2::OPENGL_بفر)
        راس_2 = راس.get_OGl_بفر();
    ورنہ
        راس_2.copyFrom(راس);

    سائز_3 = راس_2.size().area();
}

خالی cv::ogl::Arrays::reset_راس_Array()
{
    راس_2.release();
    سائز_3 = 0;
}

خالی cv::ogl::Arrays::سیٹ_Color_Array(داخلہ_Array color)
{
    ثابت صحیح cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    اگر (color.قسم() == داخلہ_Array_2::OPENGL_بفر)
        color_ = color.get_OGl_بفر();
    ورنہ
        color_.copyFrom(color);
}

خالی cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

خالی cv::ogl::Arrays::سیٹ_معمول_Array(داخلہ_Array معمول)
{
    ثابت صحیح cn = معمول.channels();
    ثابت صحیح گہرائی = معمول.گہرائی();

    CV_Assert( cn == 3 );
    CV_Assert( گہرائی == CV_8S || گہرائی == CV_16S || گہرائی == CV_32S || گہرائی == CV_32F || گہرائی == CV_64F );

    اگر (معمول.قسم() == داخلہ_Array_2::OPENGL_بفر)
        معمول_2 = معمول.get_OGl_بفر();
    ورنہ
        معمول_2.copyFrom(معمول);
}

خالی cv::ogl::Arrays::reset_معمول_Array()
{
    معمول_2.release();
}

خالی cv::ogl::Arrays::سیٹ_Tex_Coord_Array(داخلہ_Array texCoord)
{
    ثابت صحیح cn = texCoord.channels();
    ثابت صحیح گہرائی = texCoord.گہرائی();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( گہرائی == CV_16S || گہرائی == CV_32S || گہرائی == CV_32F || گہرائی == CV_64F );

    اگر (texCoord.قسم() == داخلہ_Array_2::OPENGL_بفر)
        texCoord_ = texCoord.get_OGl_بفر();
    ورنہ
        texCoord_.copyFrom(texCoord);
}

خالی cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

خالی cv::ogl::Arrays::release()
{
    reset_راس_Array();
    resetColorArray();
    reset_معمول_Array();
    resetTexCoordArray();
}

خالی cv::ogl::Arrays::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    راس_2.سیٹ_Auto_Release(flag);
    color_.سیٹ_Auto_Release(flag);
    معمول_2.سیٹ_Auto_Release(flag);
    texCoord_.سیٹ_Auto_Release(flag);
#اگر_ختم
}

خالی cv::ogl::Arrays::bind() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == سائز_3 );
    CV_Assert( معمول_2.empty() || معمول_2.size().area() == سائز_3 );
    CV_Assert( color_.empty() || color_.size().area() == سائز_3 );

    اگر (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();
    }
    ورنہ
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        texCoord_.bind(ogl::بفر_2::ARRAY_بفر);

        gl::Tex_Coord_اشارہ_گر(texCoord_.channels(), gl_types[texCoord_.گہرائی()], 0, 0);
        CV_CheckGlError();
    }

    اگر (معمول_2.empty())
    {
        gl::DisableClientState(gl::معمول_ARRAY);
        CV_CheckGlError();
    }
    ورنہ
    {
        gl::EnableClientState(gl::معمول_ARRAY);
        CV_CheckGlError();

        معمول_2.bind(ogl::بفر_2::ARRAY_بفر);

        gl::معمول_اشارہ_گر(gl_types[معمول_2.گہرائی()], 0, 0);
        CV_CheckGlError();
    }

    اگر (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();
    }
    ورنہ
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        color_.bind(ogl::بفر_2::ARRAY_بفر);

        ثابت صحیح cn = color_.channels();

        gl::Color_اشارہ_گر(cn, gl_types[color_.گہرائی()], 0, 0);
        CV_CheckGlError();
    }

    اگر (راس_2.empty())
    {
        gl::DisableClientState(gl::راس_ARRAY);
        CV_CheckGlError();
    }
    ورنہ
    {
        gl::EnableClientState(gl::راس_ARRAY);
        CV_CheckGlError();

        راس_2.bind(ogl::بفر_2::ARRAY_بفر);

        gl::راس_اشارہ_گر(راس_2.channels(), gl_types[راس_2.گہرائی()], 0, 0);
        CV_CheckGlError();
    }

    ogl::بفر_2::unbind(ogl::بفر_2::ARRAY_بفر);
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// ogl::VertexArray

#اگر_غیر_تعریف_شدہ HAVE_OPENGL

جماعت cv::ogl::راس_Array::Impl
{
};

#ورنہ

جماعت cv::ogl::راس_Array::Impl
{
عوامی:
    ساکن ثابت Ptr<Impl>& empty();

    Impl(GLuint vaId, منطقی autoRelease);
    Impl(std::initializer_list<Attribute> اوصاف, منطقی autoRelease);
    ~Impl();

    خالی bind();

    خالی سیٹ_Auto_Release(منطقی flag) { autoRelease_ = flag; }

    GLuint vaId() ثابت { واپس vaId_; }

نجی:
    Impl();

    GLuint vaId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::راس_Array::Impl>& cv::ogl::راس_Array::Impl::empty()
{
    ساکن Ptr<Impl> p(نیا Impl);
    واپس p;
}

cv::ogl::راس_Array::Impl::Impl() : vaId_(0), autoRelease_(جھوٹ)
{
}

cv::ogl::راس_Array::Impl::Impl(GLuint vaId, منطقی autoRelease) : vaId_(vaId), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_راس_Array(vaId) == gl::TRUE_ );
}

cv::ogl::راس_Array::Impl::Impl(std::initializer_list<Attribute> اوصاف, منطقی autoRelease) : vaId_(0), autoRelease_(autoRelease)
{
    gl::Gen_راس_Arrays(1, &vaId_);
    CV_CheckGlError();

    CV_Assert( vaId_ != 0 );
    gl::Bind_راس_Array(vaId_);
    CV_CheckGlError();

    برائے (خودکار& attribute : اوصاف)
    {
        attribute.بفر_3.bind(بفر_2::ARRAY_بفر);
        اگر (attribute.integer_)
        {
            gl::راس_Attrib_IPointer(
                attribute.shader_loc_,
                attribute.سائز_3,
                attribute.type_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت خالی*>(attribute.انحراف)
            );
            CV_CheckGlError();
        }
        ورنہ
        {
            gl::راس_Attrib_اشارہ_گر(
                attribute.shader_loc_,
                attribute.سائز_3,
                attribute.type_,
                attribute.normalized_,
                static_cast<GLsizei>(attribute.stride_),
                reinterpret_cast<ثابت خالی*>(attribute.انحراف)
            );
            CV_CheckGlError();
        }

        gl::Enable_راس_Attrib_Array(attribute.shader_loc_);
        CV_CheckGlError();
    }
}

cv::ogl::راس_Array::Impl::~Impl()
{
    اگر (autoRelease_ && vaId_)
        gl::Delete_راس_Arrays(1, &vaId_);
}

خالی cv::ogl::راس_Array::Impl::bind()
{
    gl::Bind_راس_Array(vaId_);
    CV_CheckGlError();
}

#اگر_ختم // HAVE_OPENGL

cv::ogl::راس_Array::راس_Array()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
#اگر_ختم
}

cv::ogl::راس_Array::راس_Array(std::initializer_list<Attribute> اوصاف, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(اوصاف); CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(اوصاف, autoRelease));
#اگر_ختم
}

خالی cv::ogl::راس_Array::create(std::initializer_list<Attribute> اوصاف, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(اوصاف); CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(اوصاف, autoRelease));
#اگر_ختم
}

خالی cv::ogl::راس_Array::release()
{
#اگر_تعریف_شدہ HAVE_OPENGL
    اگر (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#اگر_ختم
}

خالی cv::ogl::راس_Array::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    impl_->سیٹ_Auto_Release(flag);
#اگر_ختم
}

خالی cv::ogl::راس_Array::bind() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_->bind();
#اگر_ختم
}

خالی cv::ogl::راس_Array::unbind()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    gl::Bind_راس_Array(0);
#اگر_ختم
}

غیرعلامتی صحیح cv::ogl::راس_Array::vaId() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->vaId();
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// ogl::Shader

#اگر_غیر_تعریف_شدہ HAVE_OPENGL

جماعت cv::ogl::Shader::Impl
{
};

#ورنہ

جماعت cv::ogl::Shader::Impl
{
عوامی:
    ساکن ثابت Ptr<Impl>& empty();

    Impl(GLuint shaderId, منطقی autoRelease);
    Impl(ثابت حرف* src, GLenum type, منطقی autoRelease);
    ~Impl();

    خالی سیٹ_Auto_Release(منطقی flag) { autoRelease_ = flag; }

    GLuint shaderId() ثابت { واپس shaderId_; }

نجی:
    Impl();

    GLuint shaderId_;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::Shader::Impl>& cv::ogl::Shader::Impl::empty()
{
    ساکن Ptr<Impl> p(نیا Impl);
    واپس p;
}

cv::ogl::Shader::Impl::Impl() : shaderId_(0), autoRelease_(جھوٹ)
{
}

cv::ogl::Shader::Impl::Impl(GLuint shaderId, منطقی autoRelease) : shaderId_(shaderId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsShader(shaderId) == gl::TRUE_ );
}

cv::ogl::Shader::Impl::Impl(ثابت حرف* src, GLenum type, منطقی autoRelease) : shaderId_(0), autoRelease_(autoRelease)
{
    shaderId_ = gl::CreateShader(type);
    CV_CheckGlError();

    CV_Assert( shaderId_ != 0 );

    GLint حالت_2;
    gl::Shader_ماخذ(shaderId_, 1, &src, 0);
    gl::CompileShader(shaderId_);
    gl::GetShaderiv(shaderId_, gl::COMPILE_حالت, &حالت_2);
    اگر (!حالت_2) {
        String info_log;
        gl::GetShaderiv(shaderId_, gl::INFO_LOG_لمبائی, &حالت_2);
        info_log.resize(حالت_2);
        gl::GetShaderInfoLog(shaderId_, GLsizei(info_log.size()) + 1, خالی_اشاریہ, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();
}

cv::ogl::Shader::Impl::~Impl()
{
    اگر (autoRelease_ && shaderId_)
        gl::DeleteShader(shaderId_);
}

#اگر_ختم // HAVE_OPENGL

cv::ogl::Shader::Shader() : type_(ShaderType::راس_3)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
#اگر_ختم
}

cv::ogl::Shader::Shader(ثابت حرف* src, ShaderType type, منطقی autoRelease) : type_(type)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(type); CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(src, type, autoRelease));
#اگر_ختم
}

خالی cv::ogl::Shader::create(ثابت حرف* src, ShaderType type, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(type); CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(src, type, autoRelease));
    type_ = type;
#اگر_ختم
}

خالی cv::ogl::Shader::release()
{
#اگر_تعریف_شدہ HAVE_OPENGL
    اگر (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#اگر_ختم
}

خالی cv::ogl::Shader::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    impl_->سیٹ_Auto_Release(flag);
#اگر_ختم
}

غیرعلامتی صحیح cv::ogl::Shader::shaderId() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->shaderId();
#اگر_ختم
}

cv::ogl::Shader::ShaderType cv::ogl::Shader::type() ثابت
{
    واپس type_;
}

////////////////////////////////////////////////////////////////////////
// ogl::Program

#اگر_غیر_تعریف_شدہ HAVE_OPENGL

جماعت cv::ogl::پروگرام_2::Impl
{
};

#ورنہ

جماعت cv::ogl::پروگرام_2::Impl
{
عوامی:
    ساکن ثابت Ptr<Impl>& empty();

    Impl(GLuint پروگرام_Id, منطقی autoRelease);
    Impl(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease);
    ~Impl();

    خالی bind();

    صحیح getAttributeLocation(ثابت حرف* name) ثابت;
    صحیح get_یکساں_Location(ثابت حرف* name) ثابت;

    خالی سیٹ_Auto_Release(منطقی flag) { autoRelease_ = flag; }

    GLuint پروگرام_Id() ثابت { واپس پروگرام_Id_2; }

نجی:
    Impl();

    Shader vert_;
    Shader frag_;
    GLuint پروگرام_Id_2;
    منطقی autoRelease_;
};

ثابت Ptr<cv::ogl::پروگرام_2::Impl>& cv::ogl::پروگرام_2::Impl::empty()
{
    ساکن Ptr<Impl> p(نیا Impl);
    واپس p;
}

cv::ogl::پروگرام_2::Impl::Impl() : پروگرام_Id_2(0), autoRelease_(جھوٹ)
{
}

cv::ogl::پروگرام_2::Impl::Impl(GLuint پروگرام_Id, منطقی autoRelease) : پروگرام_Id_2(پروگرام_Id), autoRelease_(autoRelease)
{
    CV_Assert( gl::Is_پروگرام(پروگرام_Id) == gl::TRUE_ );
}

cv::ogl::پروگرام_2::Impl::Impl(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease) : پروگرام_Id_2(0), autoRelease_(autoRelease)
{
    پروگرام_Id_2 = gl::Create_پروگرام();
    CV_CheckGlError();

    CV_Assert( پروگرام_Id_2 != 0 );

    gl::AttachShader(پروگرام_Id_2, vert.shaderId());
    CV_CheckGlError();

    gl::AttachShader(پروگرام_Id_2, frag.shaderId());
    CV_CheckGlError();

    GLint حالت_2;
    gl::Link_پروگرام(پروگرام_Id_2);
    gl::GetProgramiv(پروگرام_Id_2, gl::LINK_حالت, &حالت_2);
    اگر (!حالت_2) {
        String info_log;
        gl::GetProgramiv(پروگرام_Id_2, gl::INFO_LOG_لمبائی, &حالت_2);
        info_log.resize(حالت_2);
        gl::Get_پروگرام_Info_Log(پروگرام_Id_2, GLsizei(info_log.size()) + 1, خالی_اشاریہ, &info_log[0]);
        CV_Error(Error::OpenGlApiCallError, info_log);
    }

    CV_CheckGlError();

    vert_ = vert;
    frag_ = frag;
}

cv::ogl::پروگرام_2::Impl::~Impl()
{
    اگر (autoRelease_ && پروگرام_Id_2)
        gl::Delete_پروگرام(پروگرام_Id_2);
}

خالی cv::ogl::پروگرام_2::Impl::bind()
{
    gl::Use_پروگرام(پروگرام_Id_2);
    CV_CheckGlError();
}

صحیح cv::ogl::پروگرام_2::Impl::getAttributeLocation(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحیح location = gl::GetAttribLocation(پروگرام_Id_2, name);
    CV_CheckGlError();
    واپس location;
}

صحیح cv::ogl::پروگرام_2::Impl::get_یکساں_Location(ثابت حرف* name) ثابت
{
    // -1 if absent (e.g. optimized out); caller decides.
    صحیح location = gl::Get_یکساں_Location(پروگرام_Id_2, name);
    CV_CheckGlError();
    واپس location;
}

#اگر_ختم // HAVE_OPENGL

cv::ogl::پروگرام_2::پروگرام_2()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_ = Impl::empty();
#اگر_ختم
}

cv::ogl::پروگرام_2::پروگرام_2(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(vert); CV_غیر_مستعمل(frag); CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(vert, frag, autoRelease));
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::create(ثابت Shader& vert, ثابت Shader& frag, منطقی autoRelease)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(vert); CV_غیر_مستعمل(frag); CV_غیر_مستعمل(autoRelease);
    throw_no_ogl();
#ورنہ
    impl_.reset(نیا Impl(vert, frag, autoRelease));
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::release()
{
#اگر_تعریف_شدہ HAVE_OPENGL
    اگر (impl_)
        impl_->سیٹ_Auto_Release(سچ);
    impl_ = Impl::empty();
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_Auto_Release(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    impl_->سیٹ_Auto_Release(flag);
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::bind() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    impl_->bind();
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::unbind()
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    gl::Use_پروگرام(0);
#اگر_ختم
}

صحیح cv::ogl::پروگرام_2::getAttributeLocation(ثابت حرف* name) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(name);
    throw_no_ogl();
#ورنہ
    واپس impl_->getAttributeLocation(name);
#اگر_ختم
}

صحیح cv::ogl::پروگرام_2::get_یکساں_Location(ثابت حرف* name) ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(name);
    throw_no_ogl();
#ورنہ
    واپس impl_->get_یکساں_Location(name);
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_یکساں_Vec3(صحیح loc, Vec3f vec)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(loc); CV_غیر_مستعمل(vec);
    throw_no_ogl();
#ورنہ
    gl::Uniform3f(loc, vec(0), vec(1), vec(2));
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_یکساں_Vec2(صحیح loc, Vec2f vec)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(loc); CV_غیر_مستعمل(vec);
    throw_no_ogl();
#ورنہ
    gl::Uniform2f(loc, vec(0), vec(1));
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_یکساں_Vec4(صحیح loc, Vec4f vec)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(loc); CV_غیر_مستعمل(vec);
    throw_no_ogl();
#ورنہ
    gl::Uniform4f(loc, vec(0), vec(1), vec(2), vec(3));
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_یکساں_Mat4x4(صحیح loc, Matx44f mat)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(loc); CV_غیر_مستعمل(mat);
    throw_no_ogl();
#ورنہ
    gl::یکساں_Matrix4fv(loc, 1, سچ, mat.val);
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_Uniform1f(صحیح loc, اعشاری value)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(loc); CV_غیر_مستعمل(value);
    throw_no_ogl();
#ورنہ
    gl::Uniform1f(loc, value);
#اگر_ختم
}

خالی cv::ogl::پروگرام_2::سیٹ_Uniform1i(صحیح loc, صحیح value)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(loc); CV_غیر_مستعمل(value);
    throw_no_ogl();
#ورنہ
    gl::Uniform1i(loc, value);
#اگر_ختم
}

غیرعلامتی صحیح cv::ogl::پروگرام_2::پروگرام_Id() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    throw_no_ogl();
#ورنہ
    واپس impl_->پروگرام_Id();
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// Rendering

خالی cv::ogl::render(ثابت ogl::Texture2D& tex, Rect_<دوہرا> wndRect, Rect_<دوہرا> texRect)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(tex);
    CV_غیر_مستعمل(wndRect);
    CV_غیر_مستعمل(texRect);
    throw_no_ogl();
#ورنہ
    اگر (!tex.empty())
    {
        gl::میٹرکس_منوال(gl::PROJECTION);
        gl::بھرو_شناختی_عنصر();
        gl::Ortho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
        CV_CheckGlError();

        gl::میٹرکس_منوال(gl::MODELVIEW);
        gl::بھرو_شناختی_عنصر();
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

        ثابت دوہرا راس[] =
        {
            wndRect.x, wndRect.y, 0.0,
            wndRect.x, (wndRect.y + wndRect.اونچائی), 0.0,
            wndRect.x + wndRect.چوڑائی, (wndRect.y + wndRect.اونچائی), 0.0,
            wndRect.x + wndRect.چوڑائی, wndRect.y, 0.0
        };
        ثابت دوہرا texCoords[] =
        {
            texRect.x, texRect.y,
            texRect.x, texRect.y + texRect.اونچائی,
            texRect.x + texRect.چوڑائی, texRect.y + texRect.اونچائی,
            texRect.x + texRect.چوڑائی, texRect.y
        };

        ogl::بفر_2::unbind(ogl::بفر_2::ARRAY_بفر);

        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        CV_CheckGlError();

        gl::Tex_Coord_اشارہ_گر(2, gl::DOUBLE, 0, texCoords);
        CV_CheckGlError();

        gl::DisableClientState(gl::معمول_ARRAY);
        gl::DisableClientState(gl::COLOR_ARRAY);
        CV_CheckGlError();

        gl::EnableClientState(gl::راس_ARRAY);
        CV_CheckGlError();

        gl::راس_اشارہ_گر(3, gl::DOUBLE, 0, راس);
        CV_CheckGlError();

        gl::DrawArrays(gl::QUADS, 0, 4);
        CV_CheckGlError();
    }
#اگر_ختم
}

خالی cv::ogl::render(ثابت ogl::Arrays& arr, صحیح منوال, Scalar color)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(منوال);
    CV_غیر_مستعمل(color);
    throw_no_ogl();
#ورنہ
    اگر (!arr.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        gl::DrawArrays(منوال, 0, arr.size());
    }
#اگر_ختم
}

خالی cv::ogl::render(ثابت ogl::Arrays& arr, داخلہ_Array indices, صحیح منوال, Scalar color)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(arr);
    CV_غیر_مستعمل(indices);
    CV_غیر_مستعمل(منوال);
    CV_غیر_مستعمل(color);
    throw_no_ogl();
#ورنہ
    اگر (!arr.empty() && !indices.empty())
    {
        gl::Color3d(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);

        arr.bind();

        ثابت صحیح قسم = indices.قسم();

        انتخاب (قسم)
        {
        حالت داخلہ_Array_2::OPENGL_بفر :
            {
                ogl::بفر_2 buf = indices.get_OGl_بفر();

                ثابت صحیح گہرائی = buf.گہرائی();

                CV_Assert( buf.channels() == 1 );
                CV_Assert( گہرائی <= CV_32S );

                GLenum type;
                اگر (گہرائی < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ورنہ اگر (گہرائی < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ورنہ
                    type = gl::UNSIGNED_INT;

                buf.bind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                gl::DrawElements(منوال, buf.size().area(), type, 0);

                ogl::بفر_2::unbind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                توڑو;
            }

        طےشدہ:
            {
                Mat mat = indices.getMat();

                ثابت صحیح گہرائی = mat.گہرائی();

                CV_Assert( mat.channels() == 1 );
                CV_Assert( گہرائی <= CV_32S );
                CV_Assert( mat.isContinuous() );

                GLenum type;
                اگر (گہرائی < CV_16U)
                    type = gl::UNSIGNED_BYTE;
                ورنہ اگر (گہرائی < CV_32S)
                    type = gl::UNSIGNED_SHORT;
                ورنہ
                    type = gl::UNSIGNED_INT;

                ogl::بفر_2::unbind(ogl::بفر_2::ELEMENT_ARRAY_بفر);

                gl::DrawElements(منوال, mat.size().area(), type, mat.ڈیٹا);
            }
        }
    }
#اگر_ختم
}

خالی cv::ogl::clearColor(Scalar color)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(color);
    throw_no_ogl();
#ورنہ
    gl::ClearColor(static_cast<GLfloat>(color[0] / 255.0), static_cast<GLfloat>(color[1] / 255.0),
                   static_cast<GLfloat>(color[2] / 255.0), static_cast<GLfloat>(color[3] / 255.0));
    gl::Clear(gl::COLOR_بفر_ثنائی_ہندسہ);
#اگر_ختم // HAVE_OPENGL
}

خالی cv::ogl::clear_گہرائی(اعشاری گہرائی)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(گہرائی);
    throw_no_ogl();
#ورنہ
    gl::Clear_گہرائی(گہرائی);
    gl::Clear(gl::گہرائی_بفر_ثنائی_ہندسہ);
#اگر_ختم // HAVE_OPENGL
}

خالی cv::ogl::drawArrays(صحیح first, صحیح گنتی_2, صحیح منوال)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(first); CV_غیر_مستعمل(گنتی_2); CV_غیر_مستعمل(منوال);
    throw_no_ogl();
#ورنہ
    gl::DrawArrays(منوال, first, گنتی_2);
#اگر_ختم // HAVE_OPENGL
}

خالی cv::ogl::drawElements(صحیح first, صحیح گنتی_2, صحیح type, صحیح منوال)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(first); CV_غیر_مستعمل(گنتی_2); CV_غیر_مستعمل(type); CV_غیر_مستعمل(منوال);
    throw_no_ogl();
#ورنہ
    gl::DrawElements(منوال, گنتی_2, type, reinterpret_cast<ثابت خالی*>(static_cast<size_t>(first)));
#اگر_ختم
}

خالی cv::ogl::drawElementsInstanced(صحیح first, صحیح گنتی_2, صحیح type, صحیح منوال, صحیح instance_گنتی)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(first); CV_غیر_مستعمل(گنتی_2); CV_غیر_مستعمل(type); CV_غیر_مستعمل(منوال); CV_غیر_مستعمل(instance_گنتی);
    throw_no_ogl();
#ورنہ
    gl::DrawElementsInstanced(منوال, گنتی_2, type, reinterpret_cast<ثابت خالی*>(static_cast<size_t>(first)), instance_گنتی);
    CV_CheckGlError();
#اگر_ختم
}

خالی cv::ogl::blendFunc(صحیح sfactor, صحیح dfactor)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(sfactor); CV_غیر_مستعمل(dfactor);
    throw_no_ogl();
#ورنہ
    gl::BlendFunc(sfactor, dfactor);
#اگر_ختم
}

خالی cv::ogl::گہرائی_نقاب_2(منطقی flag)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(flag);
    throw_no_ogl();
#ورنہ
    gl::گہرائی_نقاب(flag ? gl::TRUE_ : gl::FALSE_);
#اگر_ختم
}

خالی cv::ogl::enable(صحیح cap)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(cap);
    throw_no_ogl();
#ورنہ
    gl::Enable(cap);
#اگر_ختم
}

خالی cv::ogl::disable(صحیح cap)
{
#اگر_غیر_تعریف_شدہ HAVE_OPENGL
    CV_غیر_مستعمل(cap);
    throw_no_ogl();
#ورنہ
    gl::Disable(cap);
#اگر_ختم
}

////////////////////////////////////////////////////////////////////////
// CL-GL Interoperability

#اگر_تعریف_شدہ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core/opencl/runtime/opencl_gl.hpp"
#  ifdef cl_khr_gl_sharing
#    define HAVE_OPENCL_OPENGL_SHARING
#  ورنہ
#    define NO_OPENCL_SHARING_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL/OpenGL sharing support")
#  endif
#ورنہ // HAVE_OPENCL
#  define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#اگر_ختم // HAVE_OPENCL

#اگر defined(HAVE_OPENGL)
#  اگر defined(__ANDROID__)
#    include <EGL/egl.h>
#  elif defined(__linux__)
#    include <GL/glx.h>
#  endif
#اگر_ختم // HAVE_OPENGL

نامخلا cv { نامخلا ogl {

#اگر defined(HAVE_OPENCL) && defined(HAVE_OPENGL) && defined(HAVE_OPENCL_OPENGL_SHARING)
// Check to avoid crash in OpenCL runtime: https://github.com/opencv/opencv/issues/5209
ساکن خالی checkOpenCLVersion()
{
    استعمال نامخلا cv::ocl;
    ثابت Device& device = Device::getDefault();
    //CV_Assert(!device.empty());
    cl_device_id dev = (cl_device_id)device.ptr();
    CV_Assert(dev);

    cl_platform_id platform_id = 0;
    size_t sz = 0;

    cl_int حالت_2 = clGetDeviceInfo(dev, CL_DEVICE_PLATFORM, حجم(platform_id), &platform_id, &sz);
    CV_Assert(حالت_2 == CL_SUCCESS && sz == حجم(cl_platform_id));
    CV_Assert(platform_id);

    PlatformInfo pi(&platform_id);
    صحیح نسخہ_Major_2 = pi.نسخہ_Major_2();
    صحیح نسخہ_Minor_2 = pi.نسخہ_Minor_2();
    اگر (نسخہ_Major_2 < 1 || (نسخہ_Major_2 == 1 && نسخہ_Minor_2 <= 1))
        CV_Error_(cv::Error::OpenCLApiCallError,
            ("OpenCL: clCreateFromGLTexture requires OpenCL 1.2+ version: %d.%d - %s (%s)",
                نسخہ_Major_2, نسخہ_Minor_2, pi.name().c_str(), pi.نسخہ().c_str())
        );
}
#اگر_ختم

نامخلا ocl {

Context& initializeContextFromGL()
{
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ورنہ
    cl_uint platformsCnt = 0;
    cl_uint devCnt = 0;
    cl_device_id* devices = خالی_اشاریہ;
    cl_uint dev_مستعمل = 0;
    cl_context context = خالی_اشاریہ;

    cl_int حالت_2 = clGetPlatformIDs(0, تہی, &platformsCnt);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get number of platforms: %d", حالت_2));
    اگر (platformsCnt == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(platformsCnt);
    حالت_2 = clGetPlatformIDs(platformsCnt, &platforms[0], تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platforms: %d", حالت_2));


    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    منطقی sharingSupported = جھوٹ;

    برائے (غیرعلامتی صحیح i = 0; (!sharingSupported && (i < platformsCnt)); ++i) {
        حالت_2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, تہی, &devCnt);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", حالت_2));

        کوشش {
            devices = نیا cl_device_id[devCnt];

            حالت_2 = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, devCnt, devices, تہی);
            اگر (حالت_2 != CL_SUCCESS)
                CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't get platform devices: %d", حالت_2));

            برائے (غیرعلامتی صحیح j = 0; (!sharingSupported && (j < devCnt)); ++j) {
                size_t extension_سائز;
                حالت_2 = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, 0, تہی, &extension_سائز );
                اگر (حالت_2 != CL_SUCCESS)
                    CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: No devices available: %d", حالت_2));

                اگر(extension_سائز > 0)
                {
                    std::string devString(extension_سائز, '\0');
                    حالت_2 = clGetDeviceInfo(devices[j], CL_DEVICE_EXTENSIONS, devString.size(), &devString[0], &extension_سائز);
                    اگر (حالت_2 != CL_SUCCESS)
                        جاری;
                    اگر (extension_سائز > 0 && devString.size() >= extension_سائز)
                        devString.resize(extension_سائز - 1);

                    size_t oldPos = 0;
                    size_t spacePos = devString.find(' ', oldPos); // extensions string is space delimited
                    جب (spacePos != devString.npos) {
                        اگر (strcmp(GL_SHARING_EXTENSION,
                                devString.substr(oldPos, spacePos - oldPos).c_str())
                                == 0) {
                            // Device supports context sharing with OpenGL
                            dev_مستعمل = i;
                            sharingSupported = سچ;
                            توڑو;
                        }
                        کرو {
                            oldPos = spacePos + 1;
                            spacePos = devString.find(' ', oldPos);
                        } جب (spacePos == oldPos);
                    }
                }
            }
        } پکڑو(...) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Exception thrown during device information gathering");
            اگر(devices != خالی_اشاریہ) {
                حذف[] devices;
            }
            جاری;
        }

        اگر (sharingSupported) {
            // Define OS-specific context properties and create the OpenCL context
#اگر defined (__APPLE__)
                CGLContextObj cglContext = CGLGetCurrentContext();
                CGLShare_گروہ_Obj cgl_Share_گروہ = CGLGet_Share_گروہ(cglContext);
                cl_context_properties props[] =
                {
                    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)cgl_Share_گروہ,
                    0
                };
                context = clCreateContext(props, 0,0, تہی, تہی, &ci_Err_عدد);
#وگرنہ_اگر defined(__ANDROID__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_مستعمل], تہی, تہی, &حالت_2);
#وگرنہ_اگر defined(_WIN32)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
                    CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_مستعمل], تہی, تہی, &حالت_2);
#وگرنہ_اگر defined(__linux__)
                cl_context_properties props[] =
                {
                    CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
                    CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
                    CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                    0
                };
                context = clCreateContext(props, 1, &devices[dev_مستعمل], تہی, تہی, &حالت_2);
#اگر_ختم
        }

        اگر (حالت_2 != CL_SUCCESS)
            CV_Error_(cv::Error::OpenCLInitError, ("OpenCL: Can't create context for OpenGL interop: %d", حالت_2));
        ورنہ
            توڑو;
    }


    cl_platform_id platform = platforms[dev_مستعمل];
    std::string platformName = PlatformInfo(&platform).name();

    OpenCLExecutionContext clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, devices[dev_مستعمل]);
    clReleaseDevice(devices[dev_مستعمل]);
    clReleaseContext(context);
    clExecCtx.bind();
    واپس const_cast<Context&>(clExecCtx.getContext());
#اگر_ختم
}

} // namespace cv::ogl::ocl

خالی convert_مقصد_GLTexture2_D(داخلہ_Array src, Texture2D& texture)
{
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(texture);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ورنہ
    سائز src_سائز = src.size();
    CV_Assert(src_سائز.چوڑائی == (صحیح)texture.ستون_تعداد() && src_سائز.اونچائی == (صحیح)texture.صفیں());

    استعمال نامخلا cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت_2 = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_WRITE_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", حالت_2));

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = cl_قطار_میں_ڈالو_Acquire_GLObjects(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت_2));
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ستون_تعداد, (size_t)u.صفیں, 1};
    حالت_2 = cl_قطار_میں_ڈالو_Copy_بفر_مقصد_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyBufferToImage failed: %d", حالت_2));
    حالت_2 = cl_قطار_میں_ڈالو_Release_GLObjects(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت_2));

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت_2));

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت_2));
#اگر_ختم
}

خالی convertFromGLTexture2D(ثابت Texture2D& texture, OutputArray dst)
{
    CV_غیر_مستعمل(texture); CV_غیر_مستعمل(dst);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ورنہ
    // check texture format
    ثابت صحیح dtype = CV_8UC4;
    CV_Assert(texture.format() == Texture2D::RGBA);

    صحیح textureType = dtype;
    CV_Assert(textureType >= 0);

    استعمال نامخلا cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();

    checkOpenCLVersion();  // clCreateFromGLTexture requires OpenCL 1.2

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(texture.size(), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت_2 = 0;
    cl_mem clImage = clCreateFromGLTexture(context, CL_MEM_READ_ONLY, gl::TEXTURE_2D, 0, texture.texId(), &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLTexture failed: %d", حالت_2));

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = cl_قطار_میں_ڈالو_Acquire_GLObjects(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت_2));
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = { (size_t)u.ستون_تعداد, (size_t)u.صفیں, 1};
    حالت_2 = cl_قطار_میں_ڈالو_Copy_Image_مقصد_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueCopyImageToBuffer failed: %d", حالت_2));
    حالت_2 = cl_قطار_میں_ڈالو_Release_GLObjects(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت_2));

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت_2));

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت_2));
#اگر_ختم
}

//void mapGLBuffer(const Buffer& buffer, UMat& dst, AccessFlag accessFlags)
UMat mapGLBuffer(ثابت بفر_2& بفر, AccessFlag accessFlags)
{
    CV_غیر_مستعمل(بفر); CV_غیر_مستعمل(accessFlags);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ورنہ
    استعمال نامخلا cv::ocl;
    Context& ctx = Context::getDefault();
    cl_context context = (cl_context)ctx.ptr();
    cl_command_قطار cl_قطار = (cl_command_قطار)قطار::getDefault().ptr();

    صحیح clAccessFlags = 0;
    انتخاب (accessFlags & (ACCESS_READ|ACCESS_WRITE))
    {
    طےشدہ:
    حالت ACCESS_READ+ACCESS_WRITE:
        clAccessFlags = CL_MEM_READ_WRITE;
        توڑو;
    حالت ACCESS_READ:
        clAccessFlags = CL_MEM_READ_ONLY;
        توڑو;
    حالت ACCESS_WRITE:
        clAccessFlags = CL_MEM_WRITE_ONLY;
        توڑو;
    }

    cl_int حالت_2 = 0;
    cl_mem cl_بفر = clCreateFromGLBuffer(context, clAccessFlags, بفر.bufId(), &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clCreateFromGLBuffer failed: %d", حالت_2));

    gl::اختتام();

    حالت_2 = cl_قطار_میں_ڈالو_Acquire_GLObjects(cl_قطار, 1, &cl_بفر, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueAcquireGLObjects failed: %d", حالت_2));

    size_t قدم = بفر.ستون_تعداد() * بفر.elem_سائز();
    صحیح صفیں = بفر.صفیں();
    صحیح ستون_تعداد = بفر.ستون_تعداد();
    صحیح type = بفر.type();

    UMat u;
    convert_From_بفر(cl_بفر, قدم, صفیں, ستون_تعداد, type, u);
    واپس u;
#اگر_ختم
}

خالی unmapGLBuffer(UMat& u)
{
    CV_غیر_مستعمل(u);
#اگر !defined(HAVE_OPENGL)
    NO_OPENGL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL_OPENGL_SHARING)
    NO_OPENCL_SHARING_ERROR;
#ورنہ
    استعمال نامخلا cv::ocl;
    cl_command_قطار cl_قطار = (cl_command_قطار)قطار::getDefault().ptr();

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    u.release();

    cl_int حالت_2 = cl_قطار_میں_ڈالو_Release_GLObjects(cl_قطار, 1, &cl_بفر, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clEnqueueReleaseGLObjects failed: %d", حالت_2));

    حالت_2 = cl_اختتام(cl_قطار);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clFinish failed: %d", حالت_2));

    حالت_2 = clReleaseMemObject(cl_بفر);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error_(cv::Error::OpenCLApiCallError, ("OpenCL: clReleaseMemObject failed: %d", حالت_2));
#اگر_ختم
}

}} // namespace cv::ogl
