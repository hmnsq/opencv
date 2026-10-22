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
#ضمن <cstdint>

استعمال نطاق cv;
استعمال نطاق cv::cuda;

#إذا defined(_MSC_VER)
#هدايت warning(disable : 4702)  // unreachable code
#جيڪڏهن_پورو

/////////////////////////////////////////////////////////////
/// MemoryStack

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    صنف يادگيري_Pool;

    صنف يادگيري_ڍير
    {
    عام:
        uchar* request_يادگيري(size_t size);
        فراغ return_يادگيري(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        منطقي isFree;
        يادگيري_Pool* pool;

    #إذا !defined(NDEBUG)
        std::vector<size_t> allocations;
    #جيڪڏهن_پورو
    };

    uchar* يادگيري_ڍير::request_يادگيري(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        إذا (size > freeMem)
            أرجع 0;

        uchar* ptr = tip;

        tip += size;

    #إذا !defined(NDEBUG)
        allocations.push_back(size);
    #جيڪڏهن_پورو

        أرجع ptr;
    }

    فراغ يادگيري_ڍير::return_يادگيري(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #إذا !defined(NDEBUG)
        ثابت size_t alloc_سائيز = tip - ptr;
        CV_Assert( alloc_سائيز == allocations.back() );
        allocations.pop_back();
    #جيڪڏهن_پورو

        tip = ptr;
    }
}

#جيڪڏهن_پورو

/////////////////////////////////////////////////////////////
/// MemoryPool

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    صنف يادگيري_Pool
    {
    عام:
        يادگيري_Pool();
        ~يادگيري_Pool() { release(); }

        فراغ initialize(size_t ڍير_سائيز, صحيح ڍير_ڳڻپ);
        فراغ release();

        يادگيري_ڍير* get_Free_Mem_ڍير();
        فراغ return_Mem_ڍير(يادگيري_ڍير* mem_ڍير);

    خاص:
        فراغ initilizeImpl();

        Mutex mtx_;

        منطقي initialized_;
        size_t ڍير_سائيز_2;
        صحيح ڍير_ڳڻپ_2;

        uchar* mem_;

        std::vector<يادگيري_ڍير> stacks_;

        يادگيري_Pool(ثابت يادگيري_Pool&); //= delete;
    };

    يادگيري_Pool::يادگيري_Pool() : initialized_(خطأ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ڍير_سائيز_2 = 10 * 1024 * 1024;
        ڍير_ڳڻپ_2 = 5;
    }

    فراغ يادگيري_Pool::initialize(size_t ڍير_سائيز, صحيح ڍير_ڳڻپ)
    {
        Auto_تالو lock(mtx_);

        release();

        ڍير_سائيز_2 = ڍير_سائيز;
        ڍير_ڳڻپ_2 = ڍير_ڳڻپ;

        initilizeImpl();
    }

    فراغ يادگيري_Pool::initilizeImpl()
    {
        ثابت size_t ڪل_سائيز = ڍير_سائيز_2 * ڍير_ڳڻپ_2;

        إذا (ڪل_سائيز > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, ڪل_سائيز);
            إذا (err != cudaSuccess)
                أرجع;

            stacks_.resize(ڍير_ڳڻپ_2);

            uchar* ptr = mem_;

            لكل (صحيح i = 0; i < ڍير_ڳڻپ_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ڍير_سائيز_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = سچ;
                stacks_[i].pool = هذا;

                ptr += ڍير_سائيز_2;
            }

            initialized_ = سچ;
        }
    }

    فراغ يادگيري_Pool::release()
    {
        إذا (mem_)
        {
#إذا !defined(NDEBUG)
            لكل (صحيح i = 0; i < ڍير_ڳڻپ_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#جيڪڏهن_پورو

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = خطأ;
        }
    }

    يادگيري_ڍير* يادگيري_Pool::get_Free_Mem_ڍير()
    {
        Auto_تالو lock(mtx_);

        إذا (!initialized_)
            initilizeImpl();

        إذا (!mem_)
            أرجع 0;

        لكل (صحيح i = 0; i < ڍير_ڳڻپ_2; ++i)
        {
            إذا (stacks_[i].isFree)
            {
                stacks_[i].isFree = خطأ;
                أرجع &stacks_[i];
            }
        }

        أرجع 0;
    }

    فراغ يادگيري_Pool::return_Mem_ڍير(يادگيري_ڍير* mem_ڍير)
    {
        Auto_تالو lock(mtx_);

        CV_DbgAssert( !mem_ڍير->isFree );

#إذا !defined(NDEBUG)
        منطقي مليو = خطأ;
        لكل (صحيح i = 0; i < ڍير_ڳڻپ_2; ++i)
        {
            إذا (mem_ڍير == &stacks_[i])
            {
                مليو = سچ;
                اكسر;
            }
        }
        CV_DbgAssert( مليو );
#جيڪڏهن_پورو

        CV_DbgAssert( mem_ڍير->tip == mem_ڍير->datastart );

        mem_ڍير->isFree = سچ;
    }
}

#جيڪڏهن_پورو

////////////////////////////////////////////////////////////////
/// Stream::Impl

#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA

صنف cv::cuda::وهڪرو::Impl
{
عام:
    Impl(فراغ* ptr = 0)
    {
        CV_اڻ_استعمال_ٿيل(ptr);
        throw_no_cuda();
    }
};

#وإلا

نطاق
{
    صنف ڍير_Allocator;
}

صنف cv::cuda::وهڪرو::Impl
{
عام:
    cuda_وهڪرو_t stream;
    منطقي own_وهڪرو;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت بنانشان صحيح cudaFlags);
    واضح Impl(cuda_وهڪرو_t stream);

    ~Impl();
};

cv::cuda::وهڪرو::Impl::Impl() : stream(0), own_وهڪرو(خطأ)
{
    cuda_محفوظ_Call( cuda_وهڪرو_Create(&stream) );
    own_وهڪرو = سچ;

    allocator = makePtr<ڍير_Allocator>(stream);
}

cv::cuda::وهڪرو::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), own_وهڪرو(خطأ), allocator(allocator)
{
    cuda_محفوظ_Call( cuda_وهڪرو_Create(&stream) );
    own_وهڪرو = سچ;
}

cv::cuda::وهڪرو::Impl::Impl(ثابت بنانشان صحيح cudaFlags) : stream(0), own_وهڪرو(خطأ)
{
    cuda_محفوظ_Call(cuda_وهڪرو_Create_With_Flags(&stream, cudaFlags));
    own_وهڪرو = سچ;
    allocator = makePtr<ڍير_Allocator>(stream);
}

cv::cuda::وهڪرو::Impl::Impl(cuda_وهڪرو_t وهڪرو_3) : stream(وهڪرو_3), own_وهڪرو(خطأ)
{
    allocator = makePtr<ڍير_Allocator>(stream);
}

cv::cuda::وهڪرو::Impl::~Impl()
{
    allocator.release();

    إذا (stream && own_وهڪرو)
    {
        cuda_وهڪرو_Destroy(stream);
    }
}

#جيڪڏهن_پورو

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق cv { نطاق cuda
{
    صنف DefaultDeviceInitializer
    {
    عام:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        وهڪرو& get_Null_وهڪرو(صحيح deviceId);
        يادگيري_Pool& get_يادگيري_Pool(صحيح deviceId);

    خاص:
        فراغ initStreams();
        فراغ initPools();

        std::vector<Ptr<وهڪرو> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<يادگيري_Pool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        لكل (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_سيٽ_Device(static_cast<صحيح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    وهڪرو& DefaultDeviceInitializer::get_Null_وهڪرو(صحيح deviceId)
    {
        Auto_تالو lock(streams_mtx_);

        إذا (streams_.empty())
        {
            صحيح device_ڳڻپ = get_Cuda_Enabled_Device_ڳڻپ();

            إذا (device_ڳڻپ > 0)
                streams_.resize(device_ڳڻپ);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(streams_.size()) );

        إذا (streams_[deviceId].empty())
        {
            cuda_وهڪرو_t stream = عدم;
            Ptr<وهڪرو::Impl> impl = makePtr<وهڪرو::Impl>(stream);
            streams_[deviceId] = Ptr<وهڪرو>(جديد وهڪرو(impl));
        }

        أرجع *streams_[deviceId];
    }

    يادگيري_Pool& DefaultDeviceInitializer::get_يادگيري_Pool(صحيح deviceId)
    {
        Auto_تالو lock(pools_mtx_);

        إذا (pools_.empty())
        {
            صحيح device_ڳڻپ = get_Cuda_Enabled_Device_ڳڻپ();

            إذا (device_ڳڻپ > 0)
            {
                pools_.resize(device_ڳڻپ);
                لكل (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_سيٽ_Device(static_cast<صحيح>(i));
                    pools_[i] = makePtr<يادگيري_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(pools_.size()) );

        يادگيري_Pool* p = pools_[deviceId];
        CV_Assert(p);
        أرجع *p;
    }

    DefaultDeviceInitializer initializer;
}}

#جيڪڏهن_پورو

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::وهڪرو::وهڪرو()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>();
#جيڪڏهن_پورو
}

cv::cuda::وهڪرو::وهڪرو(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(allocator);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(allocator);
#جيڪڏهن_پورو
}

cv::cuda::وهڪرو::وهڪرو(ثابت size_t cudaFlags)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(cudaFlags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#جيڪڏهن_پورو
}

منطقي cv::cuda::وهڪرو::queryIfComplete() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cuda_وهڪرو_Query(impl_->stream);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_محفوظ_Call(err);
    أرجع خطأ;
#جيڪڏهن_پورو
}

فراغ cv::cuda::وهڪرو::waitForCompletion()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_وهڪرو_Synchronize(impl_->stream) );
#جيڪڏهن_پورو
}

فراغ cv::cuda::وهڪرو::waitEvent(ثابت Event& event)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(event);
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_وهڪرو_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#جيڪڏهن_پورو
}

#إذا defined(HAVE_CUDA) && (CUDART_نسخو >= 5000)

نطاق
{
    بنية Callback_معلومات
    {
        وهڪرو::وهڪرو_Callback callback;
        فراغ* user_معلومات;

        Callback_معلومات(وهڪرو::وهڪرو_Callback callback_, فراغ* user_معلومات_2) : callback(callback_), user_معلومات(user_معلومات_2) {}
    };

    فراغ CUDART_CB cuda_وهڪرو_Callback(cuda_وهڪرو_t, cudaError_t حالت, فراغ* user_معلومات)
    {
        Callback_معلومات* معلومات = reinterpret_cast<Callback_معلومات*>(user_معلومات);
        معلومات->callback(static_cast<صحيح>(حالت), معلومات->user_معلومات);
        ميٽيو معلومات;
    }
}

#جيڪڏهن_پورو

فراغ cv::cuda::وهڪرو::قطار_اندر_وجهو_Host_Callback(وهڪرو_Callback callback, فراغ* user_معلومات)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(callback);
    CV_اڻ_استعمال_ٿيل(user_معلومات);
    throw_no_cuda();
#وإلا
    #إذا CUDART_نسخو < 5000
        CV_اڻ_استعمال_ٿيل(callback);
        CV_اڻ_استعمال_ٿيل(user_معلومات);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #وإلا
        Callback_معلومات* معلومات = جديد Callback_معلومات(callback, user_معلومات);

        cuda_محفوظ_Call( cuda_وهڪرو_جوڙيو_Callback(impl_->stream, cuda_وهڪرو_Callback, معلومات, 0) );
    #جيڪڏهن_پورو
#جيڪڏهن_پورو
}

وهڪرو& cv::cuda::وهڪرو::Null()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    ثابت صحيح deviceId = getDevice();
    أرجع initializer.get_Null_وهڪرو(deviceId);
#جيڪڏهن_پورو
}

فراغ* cv::cuda::وهڪرو::cudaPtr() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    أرجع خالي_اشارو;
#وإلا
    أرجع impl_->stream;
#جيڪڏهن_پورو
}

cv::cuda::وهڪرو::عامل bool_type() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    أرجع 0;
#وإلا
    أرجع (impl_->stream != 0) ? &وهڪرو::this_type_does_not_support_comparisons : 0;
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

cuda_وهڪرو_t cv::cuda::وهڪرو_Accessor::get_وهڪرو(ثابت وهڪرو& stream)
{
    أرجع stream.impl_->stream;
}

وهڪرو cv::cuda::وهڪرو_Accessor::wrap_وهڪرو(cuda_وهڪرو_t stream)
{
    أرجع وهڪرو(makePtr<وهڪرو::Impl>(stream));
}

#جيڪڏهن_پورو

وهڪرو cv::cuda::wrap_وهڪرو(size_t cuda_وهڪرو_يادگيري_پتو) {
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(cuda_وهڪرو_يادگيري_پتو);
    throw_no_cuda();
#وإلا
    أرجع cv::cuda::وهڪرو_Accessor::wrap_وهڪرو(reinterpret_cast<cuda_وهڪرو_t>(cuda_وهڪرو_يادگيري_پتو));
#جيڪڏهن_پورو
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق
{
    منطقي enable_يادگيري_Pool = خطأ;

    صنف ڍير_Allocator : عام GpuMat::Allocator
    {
    عام:
        واضح ڍير_Allocator(cuda_وهڪرو_t stream);
        ~ڍير_Allocator();

        منطقي allocate(GpuMat* mat, صحيح صفون, صحيح کانن_ڳڻپ, size_t elem_سائيز) CV_OVERRIDE;
        فراغ حرر(GpuMat* mat) CV_OVERRIDE;

    خاص:
        ڍير_Allocator(ثابت ڍير_Allocator&);
        ڍير_Allocator& عامل =(ثابت ڍير_Allocator&);

        cuda_وهڪرو_t وهڪرو_3;
        يادگيري_ڍير* mem_ڍير_2;
        size_t alignment_;
    };

    ڍير_Allocator::ڍير_Allocator(cuda_وهڪرو_t stream) : وهڪرو_3(stream), mem_ڍير_2(0)
    {
        إذا (enable_يادگيري_Pool)
        {
            ثابت صحيح deviceId = getDevice();
            mem_ڍير_2 = initializer.get_يادگيري_Pool(deviceId).get_Free_Mem_ڍير();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ڍير_Allocator::~ڍير_Allocator()
    {
        إذا (mem_ڍير_2 != 0)
        {
            cuda_وهڪرو_Synchronize(وهڪرو_3);
            mem_ڍير_2->pool->return_Mem_ڍير(mem_ڍير_2);
        }
    }

    size_t align_مٿي(size_t what, size_t alignment)
    {
        size_t align_نقاب = alignment-1;
        size_t inverse_Align_نقاب = ~align_نقاب;
        size_t res = (what + align_نقاب) & inverse_Align_نقاب;
        أرجع res;
    }

    منطقي ڍير_Allocator::allocate(GpuMat* mat, صحيح صفون, صحيح کانن_ڳڻپ, size_t elem_سائيز)
    {
        إذا (mem_ڍير_2 == 0)
            أرجع خطأ;

        size_t pitch, mem_سائيز;

        إذا (صفون > 1 && کانن_ڳڻپ > 1)
        {
            pitch = align_مٿي(کانن_ڳڻپ * elem_سائيز, alignment_);
            mem_سائيز = pitch * صفون;
        }
        وإلا
        {
            // Single row or single column must be continuous
            pitch = elem_سائيز * کانن_ڳڻپ;
            mem_سائيز = align_مٿي(elem_سائيز * کانن_ڳڻپ * صفون, 64);
        }

        uchar* ptr = mem_ڍير_2->request_يادگيري(mem_سائيز);

        إذا (ptr == 0)
            أرجع خطأ;

        mat->معلومات = ptr;
        mat->قدم = pitch;
        mat->refcount = (صحيح*) fastMalloc(حجم(صحيح));

        أرجع سچ;
    }

    فراغ ڍير_Allocator::حرر(GpuMat* mat)
    {
        إذا (mem_ڍير_2 == 0)
            أرجع;

        mem_ڍير_2->return_يادگيري(mat->datastart);
        fastFree(mat->refcount);
    }
}

#جيڪڏهن_پورو

/////////////////////////////////////////////////////////////
/// BufferPool

فراغ cv::cuda::سيٽ_بفر_Pool_Usage(منطقي on)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(on);
    throw_no_cuda();
#وإلا
    enable_يادگيري_Pool = on;
#جيڪڏهن_پورو
}

فراغ cv::cuda::سيٽ_بفر_Pool_Config(صحيح deviceId, size_t ڍير_سائيز, صحيح ڍير_ڳڻپ)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(deviceId);
    CV_اڻ_استعمال_ٿيل(ڍير_سائيز);
    CV_اڻ_استعمال_ٿيل(ڍير_ڳڻپ);
    throw_no_cuda();
#وإلا
    ثابت صحيح currentDevice = getDevice();

    إذا (deviceId >= 0)
    {
        سيٽ_Device(deviceId);
        initializer.get_يادگيري_Pool(deviceId).initialize(ڍير_سائيز, ڍير_ڳڻپ);
    }
    وإلا
    {
        ثابت صحيح device_ڳڻپ = get_Cuda_Enabled_Device_ڳڻپ();

        لكل (deviceId = 0; deviceId < device_ڳڻپ; ++deviceId)
        {
            سيٽ_Device(deviceId);
            initializer.get_يادگيري_Pool(deviceId).initialize(ڍير_سائيز, ڍير_ڳڻپ);
        }
    }

    سيٽ_Device(currentDevice);
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
cv::cuda::بفر_Pool::بفر_Pool(وهڪرو& stream)
{
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
}
#وإلا
cv::cuda::بفر_Pool::بفر_Pool(وهڪرو& stream) : allocator_(stream.impl_->allocator)
{
}
#جيڪڏهن_پورو

GpuMat cv::cuda::بفر_Pool::get_بفر(صحيح صفون, صحيح کانن_ڳڻپ, صحيح type)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(صفون);
    CV_اڻ_استعمال_ٿيل(کانن_ڳڻپ);
    CV_اڻ_استعمال_ٿيل(type);
    throw_no_cuda();
#وإلا
    GpuMat buf(allocator_);
    buf.create(صفون, کانن_ڳڻپ, type);
    أرجع buf;
#جيڪڏهن_پورو
}


////////////////////////////////////////////////////////////////
// Event

#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA

صنف cv::cuda::Event::Impl
{
عام:
    Impl(بنانشان صحيح)
    {
        throw_no_cuda();
    }
};

#وإلا

صنف cv::cuda::Event::Impl
{
عام:
    cudaEvent_t event;
    منطقي ownEvent;

    واضح Impl(بنانشان صحيح flags);
    واضح Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(بنانشان صحيح flags) : event(0), ownEvent(خطأ)
{
    cuda_محفوظ_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = سچ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(خطأ)
{
}

cv::cuda::Event::Impl::~Impl()
{
    إذا (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ثابت Event& event)
{
    أرجع event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    أرجع Event(makePtr<Event::Impl>(event));
}

#جيڪڏهن_پورو

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(flags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(flags);
#جيڪڏهن_پورو
}

فراغ cv::cuda::Event::رڪارڊ(وهڪرو& stream)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(stream);
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_Event_رڪارڊ(impl_->event, وهڪرو_Accessor::get_وهڪرو(stream)) );
#جيڪڏهن_پورو
}

منطقي cv::cuda::Event::queryIfComplete() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cudaEventQuery(impl_->event);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_محفوظ_Call(err);
    أرجع خطأ;
#جيڪڏهن_پورو
}

فراغ cv::cuda::Event::waitForCompletion()
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cudaEventSynchronize(impl_->event) );
#جيڪڏهن_پورو
}

عائم cv::cuda::Event::elapsed_وقت(ثابت Event& شروعات, ثابت Event& end)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(شروعات);
    CV_اڻ_استعمال_ٿيل(end);
    throw_no_cuda();
#وإلا
    عائم ms;
    cuda_محفوظ_Call( cuda_Event_Elapsed_وقت(&ms, شروعات.impl_->event, end.impl_->event) );
    أرجع ms;
#جيڪڏهن_پورو
}
