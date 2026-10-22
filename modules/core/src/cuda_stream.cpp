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

کارول نطاق cv;
کارول نطاق cv::cuda;

#إذا defined(_MSC_VER)
#لارښوونه warning(disable : 4702)  // unreachable code
#د_که_پای

/////////////////////////////////////////////////////////////
/// MemoryStack

#که_تعریف_شوی HAVE_CUDA

نطاق
{
    صنف حافظه_Pool;

    صنف حافظه_ډېری
    {
    عام:
        uchar* request_حافظه(size_t size);
        فراغ return_حافظه(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        منطقي isFree;
        حافظه_Pool* pool;

    #إذا !defined(NDEBUG)
        std::vector<size_t> allocations;
    #د_که_پای
    };

    uchar* حافظه_ډېری::request_حافظه(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        إذا (size > freeMem)
            أرجع 0;

        uchar* ptr = tip;

        tip += size;

    #إذا !defined(NDEBUG)
        allocations.push_back(size);
    #د_که_پای

        أرجع ptr;
    }

    فراغ حافظه_ډېری::return_حافظه(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #إذا !defined(NDEBUG)
        ثابت size_t alloc_اندازه = tip - ptr;
        CV_Assert( alloc_اندازه == allocations.back() );
        allocations.pop_back();
    #د_که_پای

        tip = ptr;
    }
}

#د_که_پای

/////////////////////////////////////////////////////////////
/// MemoryPool

#که_تعریف_شوی HAVE_CUDA

نطاق
{
    صنف حافظه_Pool
    {
    عام:
        حافظه_Pool();
        ~حافظه_Pool() { release(); }

        فراغ initialize(size_t ډېری_اندازه, صحيح ډېری_شمېر);
        فراغ release();

        حافظه_ډېری* get_Free_Mem_ډېری();
        فراغ return_Mem_ډېری(حافظه_ډېری* mem_ډېری);

    خاص:
        فراغ initilizeImpl();

        Mutex mtx_;

        منطقي initialized_;
        size_t ډېری_اندازه_2;
        صحيح ډېری_شمېر_2;

        uchar* mem_;

        std::vector<حافظه_ډېری> stacks_;

        حافظه_Pool(ثابت حافظه_Pool&); //= delete;
    };

    حافظه_Pool::حافظه_Pool() : initialized_(خطأ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ډېری_اندازه_2 = 10 * 1024 * 1024;
        ډېری_شمېر_2 = 5;
    }

    فراغ حافظه_Pool::initialize(size_t ډېری_اندازه, صحيح ډېری_شمېر)
    {
        Auto_قلف lock(mtx_);

        release();

        ډېری_اندازه_2 = ډېری_اندازه;
        ډېری_شمېر_2 = ډېری_شمېر;

        initilizeImpl();
    }

    فراغ حافظه_Pool::initilizeImpl()
    {
        ثابت size_t ټول_اندازه = ډېری_اندازه_2 * ډېری_شمېر_2;

        إذا (ټول_اندازه > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, ټول_اندازه);
            إذا (err != cudaSuccess)
                أرجع;

            stacks_.resize(ډېری_شمېر_2);

            uchar* ptr = mem_;

            لكل (صحيح i = 0; i < ډېری_شمېر_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ډېری_اندازه_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = ریښتیا;
                stacks_[i].pool = هذا;

                ptr += ډېری_اندازه_2;
            }

            initialized_ = ریښتیا;
        }
    }

    فراغ حافظه_Pool::release()
    {
        إذا (mem_)
        {
#إذا !defined(NDEBUG)
            لكل (صحيح i = 0; i < ډېری_شمېر_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#د_که_پای

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = خطأ;
        }
    }

    حافظه_ډېری* حافظه_Pool::get_Free_Mem_ډېری()
    {
        Auto_قلف lock(mtx_);

        إذا (!initialized_)
            initilizeImpl();

        إذا (!mem_)
            أرجع 0;

        لكل (صحيح i = 0; i < ډېری_شمېر_2; ++i)
        {
            إذا (stacks_[i].isFree)
            {
                stacks_[i].isFree = خطأ;
                أرجع &stacks_[i];
            }
        }

        أرجع 0;
    }

    فراغ حافظه_Pool::return_Mem_ډېری(حافظه_ډېری* mem_ډېری)
    {
        Auto_قلف lock(mtx_);

        CV_DbgAssert( !mem_ډېری->isFree );

#إذا !defined(NDEBUG)
        منطقي وموندل = خطأ;
        لكل (صحيح i = 0; i < ډېری_شمېر_2; ++i)
        {
            إذا (mem_ډېری == &stacks_[i])
            {
                وموندل = ریښتیا;
                اكسر;
            }
        }
        CV_DbgAssert( وموندل );
#د_که_پای

        CV_DbgAssert( mem_ډېری->tip == mem_ډېری->datastart );

        mem_ډېری->isFree = ریښتیا;
    }
}

#د_که_پای

////////////////////////////////////////////////////////////////
/// Stream::Impl

#که_نه_تعریف_شوی HAVE_CUDA

صنف cv::cuda::بهیر::Impl
{
عام:
    Impl(فراغ* ptr = 0)
    {
        CV_ناکارول_شوی(ptr);
        throw_no_cuda();
    }
};

#وإلا

نطاق
{
    صنف ډېری_Allocator;
}

صنف cv::cuda::بهیر::Impl
{
عام:
    cuda_بهیر_t stream;
    منطقي own_بهیر;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت بینښه صحيح cudaFlags);
    څرګند Impl(cuda_بهیر_t stream);

    ~Impl();
};

cv::cuda::بهیر::Impl::Impl() : stream(0), own_بهیر(خطأ)
{
    cuda_خوندي_Call( cuda_بهیر_Create(&stream) );
    own_بهیر = ریښتیا;

    allocator = makePtr<ډېری_Allocator>(stream);
}

cv::cuda::بهیر::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), own_بهیر(خطأ), allocator(allocator)
{
    cuda_خوندي_Call( cuda_بهیر_Create(&stream) );
    own_بهیر = ریښتیا;
}

cv::cuda::بهیر::Impl::Impl(ثابت بینښه صحيح cudaFlags) : stream(0), own_بهیر(خطأ)
{
    cuda_خوندي_Call(cuda_بهیر_Create_With_Flags(&stream, cudaFlags));
    own_بهیر = ریښتیا;
    allocator = makePtr<ډېری_Allocator>(stream);
}

cv::cuda::بهیر::Impl::Impl(cuda_بهیر_t بهیر_3) : stream(بهیر_3), own_بهیر(خطأ)
{
    allocator = makePtr<ډېری_Allocator>(stream);
}

cv::cuda::بهیر::Impl::~Impl()
{
    allocator.release();

    إذا (stream && own_بهیر)
    {
        cuda_بهیر_Destroy(stream);
    }
}

#د_که_پای

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#که_تعریف_شوی HAVE_CUDA

نطاق cv { نطاق cuda
{
    صنف DefaultDeviceInitializer
    {
    عام:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        بهیر& get_Null_بهیر(صحيح deviceId);
        حافظه_Pool& get_حافظه_Pool(صحيح deviceId);

    خاص:
        فراغ initStreams();
        فراغ initPools();

        std::vector<Ptr<بهیر> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<حافظه_Pool> > pools_;
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
            cuda_سېټ_Device(static_cast<صحيح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    بهیر& DefaultDeviceInitializer::get_Null_بهیر(صحيح deviceId)
    {
        Auto_قلف lock(streams_mtx_);

        إذا (streams_.empty())
        {
            صحيح device_شمېر = get_Cuda_Enabled_Device_شمېر();

            إذا (device_شمېر > 0)
                streams_.resize(device_شمېر);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(streams_.size()) );

        إذا (streams_[deviceId].empty())
        {
            cuda_بهیر_t stream = عدم;
            Ptr<بهیر::Impl> impl = makePtr<بهیر::Impl>(stream);
            streams_[deviceId] = Ptr<بهیر>(جديد بهیر(impl));
        }

        أرجع *streams_[deviceId];
    }

    حافظه_Pool& DefaultDeviceInitializer::get_حافظه_Pool(صحيح deviceId)
    {
        Auto_قلف lock(pools_mtx_);

        إذا (pools_.empty())
        {
            صحيح device_شمېر = get_Cuda_Enabled_Device_شمېر();

            إذا (device_شمېر > 0)
            {
                pools_.resize(device_شمېر);
                لكل (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_سېټ_Device(static_cast<صحيح>(i));
                    pools_[i] = makePtr<حافظه_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(pools_.size()) );

        حافظه_Pool* p = pools_[deviceId];
        CV_Assert(p);
        أرجع *p;
    }

    DefaultDeviceInitializer initializer;
}}

#د_که_پای

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::بهیر::بهیر()
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>();
#د_که_پای
}

cv::cuda::بهیر::بهیر(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(allocator);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(allocator);
#د_که_پای
}

cv::cuda::بهیر::بهیر(ثابت size_t cudaFlags)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(cudaFlags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#د_که_پای
}

منطقي cv::cuda::بهیر::queryIfComplete() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cuda_بهیر_Query(impl_->stream);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_خوندي_Call(err);
    أرجع خطأ;
#د_که_پای
}

فراغ cv::cuda::بهیر::waitForCompletion()
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_خوندي_Call( cuda_بهیر_Synchronize(impl_->stream) );
#د_که_پای
}

فراغ cv::cuda::بهیر::waitEvent(ثابت Event& event)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(event);
    throw_no_cuda();
#وإلا
    cuda_خوندي_Call( cuda_بهیر_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#د_که_پای
}

#إذا defined(HAVE_CUDA) && (CUDART_نسخه >= 5000)

نطاق
{
    بنية Callback_معلومات
    {
        بهیر::بهیر_Callback callback;
        فراغ* user_معلومات;

        Callback_معلومات(بهیر::بهیر_Callback callback_, فراغ* user_معلومات_2) : callback(callback_), user_معلومات(user_معلومات_2) {}
    };

    فراغ CUDART_CB cuda_بهیر_Callback(cuda_بهیر_t, cudaError_t حالت, فراغ* user_معلومات)
    {
        Callback_معلومات* معلومات = reinterpret_cast<Callback_معلومات*>(user_معلومات);
        معلومات->callback(static_cast<صحيح>(حالت), معلومات->user_معلومات);
        ړنګول معلومات;
    }
}

#د_که_پای

فراغ cv::cuda::بهیر::کتار_ته_ورزیاتول_Host_Callback(بهیر_Callback callback, فراغ* user_معلومات)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(callback);
    CV_ناکارول_شوی(user_معلومات);
    throw_no_cuda();
#وإلا
    #إذا CUDART_نسخه < 5000
        CV_ناکارول_شوی(callback);
        CV_ناکارول_شوی(user_معلومات);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #وإلا
        Callback_معلومات* معلومات = جديد Callback_معلومات(callback, user_معلومات);

        cuda_خوندي_Call( cuda_بهیر_جمع_کړه_Callback(impl_->stream, cuda_بهیر_Callback, معلومات, 0) );
    #د_که_پای
#د_که_پای
}

بهیر& cv::cuda::بهیر::Null()
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    ثابت صحيح deviceId = getDevice();
    أرجع initializer.get_Null_بهیر(deviceId);
#د_که_پای
}

فراغ* cv::cuda::بهیر::cudaPtr() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    أرجع تش_اشاره;
#وإلا
    أرجع impl_->stream;
#د_که_پای
}

cv::cuda::بهیر::چلوونکی bool_type() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    أرجع 0;
#وإلا
    أرجع (impl_->stream != 0) ? &بهیر::this_type_does_not_support_comparisons : 0;
#د_که_پای
}

#که_تعریف_شوی HAVE_CUDA

cuda_بهیر_t cv::cuda::بهیر_Accessor::get_بهیر(ثابت بهیر& stream)
{
    أرجع stream.impl_->stream;
}

بهیر cv::cuda::بهیر_Accessor::wrap_بهیر(cuda_بهیر_t stream)
{
    أرجع بهیر(makePtr<بهیر::Impl>(stream));
}

#د_که_پای

بهیر cv::cuda::wrap_بهیر(size_t cuda_بهیر_حافظه_پته) {
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(cuda_بهیر_حافظه_پته);
    throw_no_cuda();
#وإلا
    أرجع cv::cuda::بهیر_Accessor::wrap_بهیر(reinterpret_cast<cuda_بهیر_t>(cuda_بهیر_حافظه_پته));
#د_که_پای
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#که_تعریف_شوی HAVE_CUDA

نطاق
{
    منطقي enable_حافظه_Pool = خطأ;

    صنف ډېری_Allocator : عام GpuMat::Allocator
    {
    عام:
        څرګند ډېری_Allocator(cuda_بهیر_t stream);
        ~ډېری_Allocator();

        منطقي allocate(GpuMat* mat, صحيح لیکې, صحيح ستون_شمېر, size_t elem_اندازه) CV_OVERRIDE;
        فراغ حرر(GpuMat* mat) CV_OVERRIDE;

    خاص:
        ډېری_Allocator(ثابت ډېری_Allocator&);
        ډېری_Allocator& چلوونکی =(ثابت ډېری_Allocator&);

        cuda_بهیر_t بهیر_3;
        حافظه_ډېری* mem_ډېری_2;
        size_t alignment_;
    };

    ډېری_Allocator::ډېری_Allocator(cuda_بهیر_t stream) : بهیر_3(stream), mem_ډېری_2(0)
    {
        إذا (enable_حافظه_Pool)
        {
            ثابت صحيح deviceId = getDevice();
            mem_ډېری_2 = initializer.get_حافظه_Pool(deviceId).get_Free_Mem_ډېری();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ډېری_Allocator::~ډېری_Allocator()
    {
        إذا (mem_ډېری_2 != 0)
        {
            cuda_بهیر_Synchronize(بهیر_3);
            mem_ډېری_2->pool->return_Mem_ډېری(mem_ډېری_2);
        }
    }

    size_t align_پورته(size_t what, size_t alignment)
    {
        size_t align_ماسک = alignment-1;
        size_t inverse_Align_ماسک = ~align_ماسک;
        size_t res = (what + align_ماسک) & inverse_Align_ماسک;
        أرجع res;
    }

    منطقي ډېری_Allocator::allocate(GpuMat* mat, صحيح لیکې, صحيح ستون_شمېر, size_t elem_اندازه)
    {
        إذا (mem_ډېری_2 == 0)
            أرجع خطأ;

        size_t pitch, mem_اندازه;

        إذا (لیکې > 1 && ستون_شمېر > 1)
        {
            pitch = align_پورته(ستون_شمېر * elem_اندازه, alignment_);
            mem_اندازه = pitch * لیکې;
        }
        وإلا
        {
            // Single row or single column must be continuous
            pitch = elem_اندازه * ستون_شمېر;
            mem_اندازه = align_پورته(elem_اندازه * ستون_شمېر * لیکې, 64);
        }

        uchar* ptr = mem_ډېری_2->request_حافظه(mem_اندازه);

        إذا (ptr == 0)
            أرجع خطأ;

        mat->معلومات = ptr;
        mat->ګام = pitch;
        mat->refcount = (صحيح*) fastMalloc(حجم(صحيح));

        أرجع ریښتیا;
    }

    فراغ ډېری_Allocator::حرر(GpuMat* mat)
    {
        إذا (mem_ډېری_2 == 0)
            أرجع;

        mem_ډېری_2->return_حافظه(mat->datastart);
        fastFree(mat->refcount);
    }
}

#د_که_پای

/////////////////////////////////////////////////////////////
/// BufferPool

فراغ cv::cuda::سېټ_بفر_Pool_Usage(منطقي on)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(on);
    throw_no_cuda();
#وإلا
    enable_حافظه_Pool = on;
#د_که_پای
}

فراغ cv::cuda::سېټ_بفر_Pool_Config(صحيح deviceId, size_t ډېری_اندازه, صحيح ډېری_شمېر)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(deviceId);
    CV_ناکارول_شوی(ډېری_اندازه);
    CV_ناکارول_شوی(ډېری_شمېر);
    throw_no_cuda();
#وإلا
    ثابت صحيح currentDevice = getDevice();

    إذا (deviceId >= 0)
    {
        سېټ_Device(deviceId);
        initializer.get_حافظه_Pool(deviceId).initialize(ډېری_اندازه, ډېری_شمېر);
    }
    وإلا
    {
        ثابت صحيح device_شمېر = get_Cuda_Enabled_Device_شمېر();

        لكل (deviceId = 0; deviceId < device_شمېر; ++deviceId)
        {
            سېټ_Device(deviceId);
            initializer.get_حافظه_Pool(deviceId).initialize(ډېری_اندازه, ډېری_شمېر);
        }
    }

    سېټ_Device(currentDevice);
#د_که_پای
}

#که_نه_تعریف_شوی HAVE_CUDA
cv::cuda::بفر_Pool::بفر_Pool(بهیر& stream)
{
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
}
#وإلا
cv::cuda::بفر_Pool::بفر_Pool(بهیر& stream) : allocator_(stream.impl_->allocator)
{
}
#د_که_پای

GpuMat cv::cuda::بفر_Pool::get_بفر(صحيح لیکې, صحيح ستون_شمېر, صحيح type)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(لیکې);
    CV_ناکارول_شوی(ستون_شمېر);
    CV_ناکارول_شوی(type);
    throw_no_cuda();
#وإلا
    GpuMat buf(allocator_);
    buf.create(لیکې, ستون_شمېر, type);
    أرجع buf;
#د_که_پای
}


////////////////////////////////////////////////////////////////
// Event

#که_نه_تعریف_شوی HAVE_CUDA

صنف cv::cuda::Event::Impl
{
عام:
    Impl(بینښه صحيح)
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

    څرګند Impl(بینښه صحيح flags);
    څرګند Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(بینښه صحيح flags) : event(0), ownEvent(خطأ)
{
    cuda_خوندي_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = ریښتیا;
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

#د_که_پای

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(flags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(flags);
#د_که_پای
}

فراغ cv::cuda::Event::ریکارډ(بهیر& stream)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(stream);
    throw_no_cuda();
#وإلا
    cuda_خوندي_Call( cuda_Event_ریکارډ(impl_->event, بهیر_Accessor::get_بهیر(stream)) );
#د_که_پای
}

منطقي cv::cuda::Event::queryIfComplete() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cudaEventQuery(impl_->event);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_خوندي_Call(err);
    أرجع خطأ;
#د_که_پای
}

فراغ cv::cuda::Event::waitForCompletion()
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_خوندي_Call( cudaEventSynchronize(impl_->event) );
#د_که_پای
}

عائم cv::cuda::Event::elapsed_وخت(ثابت Event& پیل, ثابت Event& end)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(پیل);
    CV_ناکارول_شوی(end);
    throw_no_cuda();
#وإلا
    عائم ms;
    cuda_خوندي_Call( cuda_Event_Elapsed_وخت(&ms, پیل.impl_->event, end.impl_->event) );
    أرجع ms;
#د_که_پای
}
