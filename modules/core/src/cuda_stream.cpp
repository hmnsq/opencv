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

استفاده نطاق cv;
استفاده نطاق cv::cuda;

#إذا defined(_MSC_VER)
#رهنمود warning(disable : 4702)  // unreachable code
#پایان_اگر

/////////////////////////////////////////////////////////////
/// MemoryStack

#اگر_تعریف_شده HAVE_CUDA

نطاق
{
    صنف حافظه_Pool;

    صنف حافظه_پشته
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
    #پایان_اگر
    };

    uchar* حافظه_پشته::request_حافظه(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        إذا (size > freeMem)
            أرجع 0;

        uchar* ptr = tip;

        tip += size;

    #إذا !defined(NDEBUG)
        allocations.push_back(size);
    #پایان_اگر

        أرجع ptr;
    }

    فراغ حافظه_پشته::return_حافظه(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #إذا !defined(NDEBUG)
        ثابت size_t alloc_اندازه = tip - ptr;
        CV_Assert( alloc_اندازه == allocations.back() );
        allocations.pop_back();
    #پایان_اگر

        tip = ptr;
    }
}

#پایان_اگر

/////////////////////////////////////////////////////////////
/// MemoryPool

#اگر_تعریف_شده HAVE_CUDA

نطاق
{
    صنف حافظه_Pool
    {
    عام:
        حافظه_Pool();
        ~حافظه_Pool() { release(); }

        فراغ initialize(size_t پشته_اندازه, صحيح پشته_شمار);
        فراغ release();

        حافظه_پشته* get_Free_Mem_پشته();
        فراغ return_Mem_پشته(حافظه_پشته* mem_پشته);

    خاص:
        فراغ initilizeImpl();

        Mutex mtx_;

        منطقي initialized_;
        size_t پشته_اندازه_2;
        صحيح پشته_شمار_2;

        uchar* mem_;

        std::vector<حافظه_پشته> stacks_;

        حافظه_Pool(ثابت حافظه_Pool&); //= delete;
    };

    حافظه_Pool::حافظه_Pool() : initialized_(خطأ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        پشته_اندازه_2 = 10 * 1024 * 1024;
        پشته_شمار_2 = 5;
    }

    فراغ حافظه_Pool::initialize(size_t پشته_اندازه, صحيح پشته_شمار)
    {
        Auto_قفل lock(mtx_);

        release();

        پشته_اندازه_2 = پشته_اندازه;
        پشته_شمار_2 = پشته_شمار;

        initilizeImpl();
    }

    فراغ حافظه_Pool::initilizeImpl()
    {
        ثابت size_t مجموع_اندازه = پشته_اندازه_2 * پشته_شمار_2;

        إذا (مجموع_اندازه > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, مجموع_اندازه);
            إذا (err != cudaSuccess)
                أرجع;

            stacks_.resize(پشته_شمار_2);

            uchar* ptr = mem_;

            لكل (صحيح i = 0; i < پشته_شمار_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + پشته_اندازه_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = درست;
                stacks_[i].pool = هذا;

                ptr += پشته_اندازه_2;
            }

            initialized_ = درست;
        }
    }

    فراغ حافظه_Pool::release()
    {
        إذا (mem_)
        {
#إذا !defined(NDEBUG)
            لكل (صحيح i = 0; i < پشته_شمار_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#پایان_اگر

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = خطأ;
        }
    }

    حافظه_پشته* حافظه_Pool::get_Free_Mem_پشته()
    {
        Auto_قفل lock(mtx_);

        إذا (!initialized_)
            initilizeImpl();

        إذا (!mem_)
            أرجع 0;

        لكل (صحيح i = 0; i < پشته_شمار_2; ++i)
        {
            إذا (stacks_[i].isFree)
            {
                stacks_[i].isFree = خطأ;
                أرجع &stacks_[i];
            }
        }

        أرجع 0;
    }

    فراغ حافظه_Pool::return_Mem_پشته(حافظه_پشته* mem_پشته)
    {
        Auto_قفل lock(mtx_);

        CV_DbgAssert( !mem_پشته->isFree );

#إذا !defined(NDEBUG)
        منطقي یافت = خطأ;
        لكل (صحيح i = 0; i < پشته_شمار_2; ++i)
        {
            إذا (mem_پشته == &stacks_[i])
            {
                یافت = درست;
                اكسر;
            }
        }
        CV_DbgAssert( یافت );
#پایان_اگر

        CV_DbgAssert( mem_پشته->tip == mem_پشته->datastart );

        mem_پشته->isFree = درست;
    }
}

#پایان_اگر

////////////////////////////////////////////////////////////////
/// Stream::Impl

#اگر_تعریف_نشده HAVE_CUDA

صنف cv::cuda::جریان::Impl
{
عام:
    Impl(فراغ* ptr = 0)
    {
        CV_استفادهنشده(ptr);
        throw_no_cuda();
    }
};

#وإلا

نطاق
{
    صنف پشته_Allocator;
}

صنف cv::cuda::جریان::Impl
{
عام:
    cuda_جریان_t stream;
    منطقي own_جریان;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت بیعلامت صحيح cudaFlags);
    صریح Impl(cuda_جریان_t stream);

    ~Impl();
};

cv::cuda::جریان::Impl::Impl() : stream(0), own_جریان(خطأ)
{
    cuda_امن_Call( cuda_جریان_Create(&stream) );
    own_جریان = درست;

    allocator = makePtr<پشته_Allocator>(stream);
}

cv::cuda::جریان::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), own_جریان(خطأ), allocator(allocator)
{
    cuda_امن_Call( cuda_جریان_Create(&stream) );
    own_جریان = درست;
}

cv::cuda::جریان::Impl::Impl(ثابت بیعلامت صحيح cudaFlags) : stream(0), own_جریان(خطأ)
{
    cuda_امن_Call(cuda_جریان_Create_With_Flags(&stream, cudaFlags));
    own_جریان = درست;
    allocator = makePtr<پشته_Allocator>(stream);
}

cv::cuda::جریان::Impl::Impl(cuda_جریان_t جریان_3) : stream(جریان_3), own_جریان(خطأ)
{
    allocator = makePtr<پشته_Allocator>(stream);
}

cv::cuda::جریان::Impl::~Impl()
{
    allocator.release();

    إذا (stream && own_جریان)
    {
        cuda_جریان_Destroy(stream);
    }
}

#پایان_اگر

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#اگر_تعریف_شده HAVE_CUDA

نطاق cv { نطاق cuda
{
    صنف DefaultDeviceInitializer
    {
    عام:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        جریان& get_Null_جریان(صحيح deviceId);
        حافظه_Pool& get_حافظه_Pool(صحيح deviceId);

    خاص:
        فراغ initStreams();
        فراغ initPools();

        std::vector<Ptr<جریان> > streams_;
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
            cuda_مجموعه_Device(static_cast<صحيح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    جریان& DefaultDeviceInitializer::get_Null_جریان(صحيح deviceId)
    {
        Auto_قفل lock(streams_mtx_);

        إذا (streams_.empty())
        {
            صحيح device_شمار = get_Cuda_Enabled_Device_شمار();

            إذا (device_شمار > 0)
                streams_.resize(device_شمار);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(streams_.size()) );

        إذا (streams_[deviceId].empty())
        {
            cuda_جریان_t stream = عدم;
            Ptr<جریان::Impl> impl = makePtr<جریان::Impl>(stream);
            streams_[deviceId] = Ptr<جریان>(جديد جریان(impl));
        }

        أرجع *streams_[deviceId];
    }

    حافظه_Pool& DefaultDeviceInitializer::get_حافظه_Pool(صحيح deviceId)
    {
        Auto_قفل lock(pools_mtx_);

        إذا (pools_.empty())
        {
            صحيح device_شمار = get_Cuda_Enabled_Device_شمار();

            إذا (device_شمار > 0)
            {
                pools_.resize(device_شمار);
                لكل (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_مجموعه_Device(static_cast<صحيح>(i));
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

#پایان_اگر

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::جریان::جریان()
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>();
#پایان_اگر
}

cv::cuda::جریان::جریان(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(allocator);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(allocator);
#پایان_اگر
}

cv::cuda::جریان::جریان(ثابت size_t cudaFlags)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(cudaFlags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#پایان_اگر
}

منطقي cv::cuda::جریان::queryIfComplete() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cuda_جریان_Query(impl_->stream);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_امن_Call(err);
    أرجع خطأ;
#پایان_اگر
}

فراغ cv::cuda::جریان::waitForCompletion()
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_امن_Call( cuda_جریان_Synchronize(impl_->stream) );
#پایان_اگر
}

فراغ cv::cuda::جریان::waitEvent(ثابت Event& event)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(event);
    throw_no_cuda();
#وإلا
    cuda_امن_Call( cuda_جریان_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#پایان_اگر
}

#إذا defined(HAVE_CUDA) && (CUDART_نسخه >= 5000)

نطاق
{
    بنية Callback_داده
    {
        جریان::جریان_Callback callback;
        فراغ* user_داده;

        Callback_داده(جریان::جریان_Callback callback_, فراغ* user_داده_2) : callback(callback_), user_داده(user_داده_2) {}
    };

    فراغ CUDART_CB cuda_جریان_Callback(cuda_جریان_t, cudaError_t وضعیت, فراغ* user_داده)
    {
        Callback_داده* داده = reinterpret_cast<Callback_داده*>(user_داده);
        داده->callback(static_cast<صحيح>(وضعیت), داده->user_داده);
        حذف داده;
    }
}

#پایان_اگر

فراغ cv::cuda::جریان::افزودن_به_صف_Host_Callback(جریان_Callback callback, فراغ* user_داده)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(callback);
    CV_استفادهنشده(user_داده);
    throw_no_cuda();
#وإلا
    #إذا CUDART_نسخه < 5000
        CV_استفادهنشده(callback);
        CV_استفادهنشده(user_داده);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #وإلا
        Callback_داده* داده = جديد Callback_داده(callback, user_داده);

        cuda_امن_Call( cuda_جریان_جمع_کن_Callback(impl_->stream, cuda_جریان_Callback, داده, 0) );
    #پایان_اگر
#پایان_اگر
}

جریان& cv::cuda::جریان::Null()
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    ثابت صحيح deviceId = getDevice();
    أرجع initializer.get_Null_جریان(deviceId);
#پایان_اگر
}

فراغ* cv::cuda::جریان::cudaPtr() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    أرجع اشاره_تهی;
#وإلا
    أرجع impl_->stream;
#پایان_اگر
}

cv::cuda::جریان::عملگر bool_type() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    أرجع 0;
#وإلا
    أرجع (impl_->stream != 0) ? &جریان::this_type_does_not_support_comparisons : 0;
#پایان_اگر
}

#اگر_تعریف_شده HAVE_CUDA

cuda_جریان_t cv::cuda::جریان_Accessor::get_جریان(ثابت جریان& stream)
{
    أرجع stream.impl_->stream;
}

جریان cv::cuda::جریان_Accessor::wrap_جریان(cuda_جریان_t stream)
{
    أرجع جریان(makePtr<جریان::Impl>(stream));
}

#پایان_اگر

جریان cv::cuda::wrap_جریان(size_t cuda_جریان_حافظه_آدرس) {
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(cuda_جریان_حافظه_آدرس);
    throw_no_cuda();
#وإلا
    أرجع cv::cuda::جریان_Accessor::wrap_جریان(reinterpret_cast<cuda_جریان_t>(cuda_جریان_حافظه_آدرس));
#پایان_اگر
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#اگر_تعریف_شده HAVE_CUDA

نطاق
{
    منطقي enable_حافظه_Pool = خطأ;

    صنف پشته_Allocator : عام GpuMat::Allocator
    {
    عام:
        صریح پشته_Allocator(cuda_جریان_t stream);
        ~پشته_Allocator();

        منطقي allocate(GpuMat* mat, صحيح ردیفها, صحيح تعداد_ستون, size_t elem_اندازه) CV_OVERRIDE;
        فراغ حرر(GpuMat* mat) CV_OVERRIDE;

    خاص:
        پشته_Allocator(ثابت پشته_Allocator&);
        پشته_Allocator& عملگر =(ثابت پشته_Allocator&);

        cuda_جریان_t جریان_3;
        حافظه_پشته* mem_پشته_2;
        size_t alignment_;
    };

    پشته_Allocator::پشته_Allocator(cuda_جریان_t stream) : جریان_3(stream), mem_پشته_2(0)
    {
        إذا (enable_حافظه_Pool)
        {
            ثابت صحيح deviceId = getDevice();
            mem_پشته_2 = initializer.get_حافظه_Pool(deviceId).get_Free_Mem_پشته();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    پشته_Allocator::~پشته_Allocator()
    {
        إذا (mem_پشته_2 != 0)
        {
            cuda_جریان_Synchronize(جریان_3);
            mem_پشته_2->pool->return_Mem_پشته(mem_پشته_2);
        }
    }

    size_t align_به_بالا(size_t what, size_t alignment)
    {
        size_t align_ماسک = alignment-1;
        size_t inverse_Align_ماسک = ~align_ماسک;
        size_t res = (what + align_ماسک) & inverse_Align_ماسک;
        أرجع res;
    }

    منطقي پشته_Allocator::allocate(GpuMat* mat, صحيح ردیفها, صحيح تعداد_ستون, size_t elem_اندازه)
    {
        إذا (mem_پشته_2 == 0)
            أرجع خطأ;

        size_t pitch, mem_اندازه;

        إذا (ردیفها > 1 && تعداد_ستون > 1)
        {
            pitch = align_به_بالا(تعداد_ستون * elem_اندازه, alignment_);
            mem_اندازه = pitch * ردیفها;
        }
        وإلا
        {
            // Single row or single column must be continuous
            pitch = elem_اندازه * تعداد_ستون;
            mem_اندازه = align_به_بالا(elem_اندازه * تعداد_ستون * ردیفها, 64);
        }

        uchar* ptr = mem_پشته_2->request_حافظه(mem_اندازه);

        إذا (ptr == 0)
            أرجع خطأ;

        mat->داده = ptr;
        mat->گام = pitch;
        mat->refcount = (صحيح*) fastMalloc(حجم(صحيح));

        أرجع درست;
    }

    فراغ پشته_Allocator::حرر(GpuMat* mat)
    {
        إذا (mem_پشته_2 == 0)
            أرجع;

        mem_پشته_2->return_حافظه(mat->datastart);
        fastFree(mat->refcount);
    }
}

#پایان_اگر

/////////////////////////////////////////////////////////////
/// BufferPool

فراغ cv::cuda::مجموعه_میانگیر_Pool_Usage(منطقي on)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(on);
    throw_no_cuda();
#وإلا
    enable_حافظه_Pool = on;
#پایان_اگر
}

فراغ cv::cuda::مجموعه_میانگیر_Pool_Config(صحيح deviceId, size_t پشته_اندازه, صحيح پشته_شمار)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(deviceId);
    CV_استفادهنشده(پشته_اندازه);
    CV_استفادهنشده(پشته_شمار);
    throw_no_cuda();
#وإلا
    ثابت صحيح currentDevice = getDevice();

    إذا (deviceId >= 0)
    {
        مجموعه_Device(deviceId);
        initializer.get_حافظه_Pool(deviceId).initialize(پشته_اندازه, پشته_شمار);
    }
    وإلا
    {
        ثابت صحيح device_شمار = get_Cuda_Enabled_Device_شمار();

        لكل (deviceId = 0; deviceId < device_شمار; ++deviceId)
        {
            مجموعه_Device(deviceId);
            initializer.get_حافظه_Pool(deviceId).initialize(پشته_اندازه, پشته_شمار);
        }
    }

    مجموعه_Device(currentDevice);
#پایان_اگر
}

#اگر_تعریف_نشده HAVE_CUDA
cv::cuda::میانگیر_Pool::میانگیر_Pool(جریان& stream)
{
    CV_استفادهنشده(stream);
    throw_no_cuda();
}
#وإلا
cv::cuda::میانگیر_Pool::میانگیر_Pool(جریان& stream) : allocator_(stream.impl_->allocator)
{
}
#پایان_اگر

GpuMat cv::cuda::میانگیر_Pool::get_میانگیر(صحيح ردیفها, صحيح تعداد_ستون, صحيح type)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(ردیفها);
    CV_استفادهنشده(تعداد_ستون);
    CV_استفادهنشده(type);
    throw_no_cuda();
#وإلا
    GpuMat buf(allocator_);
    buf.create(ردیفها, تعداد_ستون, type);
    أرجع buf;
#پایان_اگر
}


////////////////////////////////////////////////////////////////
// Event

#اگر_تعریف_نشده HAVE_CUDA

صنف cv::cuda::Event::Impl
{
عام:
    Impl(بیعلامت صحيح)
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

    صریح Impl(بیعلامت صحيح flags);
    صریح Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(بیعلامت صحيح flags) : event(0), ownEvent(خطأ)
{
    cuda_امن_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = درست;
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

#پایان_اگر

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(flags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(flags);
#پایان_اگر
}

فراغ cv::cuda::Event::رکورد(جریان& stream)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(stream);
    throw_no_cuda();
#وإلا
    cuda_امن_Call( cuda_Event_رکورد(impl_->event, جریان_Accessor::get_جریان(stream)) );
#پایان_اگر
}

منطقي cv::cuda::Event::queryIfComplete() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cudaEventQuery(impl_->event);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_امن_Call(err);
    أرجع خطأ;
#پایان_اگر
}

فراغ cv::cuda::Event::waitForCompletion()
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_امن_Call( cudaEventSynchronize(impl_->event) );
#پایان_اگر
}

عائم cv::cuda::Event::elapsed_زمان(ثابت Event& شروع, ثابت Event& end)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(شروع);
    CV_استفادهنشده(end);
    throw_no_cuda();
#وإلا
    عائم ms;
    cuda_امن_Call( cuda_Event_Elapsed_زمان(&ms, شروع.impl_->event, end.impl_->event) );
    أرجع ms;
#پایان_اگر
}
