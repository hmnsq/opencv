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

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব <cstdint>

using নামাকাশ cv;
using নামাকাশ cv::cuda;

#যদি defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

নামাকাশ
{
    শ্রেণী MemoryPool;

    শ্রেণী Memory_স্তূপ
    {
    সর্বজনীন:
        uchar* requestMemory(size_t size);
        শূন্য returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        যৌক্তিক isFree;
        MemoryPool* pool;

    #যদি !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_স্তূপ::requestMemory(size_t size)
    {
        ধ্রুবক size_t freeMem = dataend - tip;

        যদি (size > freeMem)
            ফেরত 0;

        uchar* ptr = tip;

        tip += size;

    #যদি !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        ফেরত ptr;
    }

    শূন্য Memory_স্তূপ::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #যদি !defined(NDEBUG)
        ধ্রুবক size_t alloc_আকার = tip - ptr;
        CV_Assert( alloc_আকার == allocations.back() );
        allocations.pop_back();
    #endif

        tip = ptr;
    }
}

#endif

/////////////////////////////////////////////////////////////
/// MemoryPool

#ifdef HAVE_CUDA

নামাকাশ
{
    শ্রেণী MemoryPool
    {
    সর্বজনীন:
        MemoryPool();
        ~MemoryPool() { release(); }

        শূন্য initialize(size_t স্তূপ_আকার, পূর্ণসংখ্যা স্তূপ_গুনতি);
        শূন্য release();

        Memory_স্তূপ* get_Free_Mem_স্তূপ();
        শূন্য return_Mem_স্তূপ(Memory_স্তূপ* mem_স্তূপ);

    ব্যক্তিগত:
        শূন্য initilizeImpl();

        Mutex mtx_;

        যৌক্তিক initialized_;
        size_t স্তূপ_আকার_2;
        পূর্ণসংখ্যা স্তূপ_গুনতি_2;

        uchar* mem_;

        std::vector<Memory_স্তূপ> stacks_;

        MemoryPool(ধ্রুবক MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(মিথ্যা), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        স্তূপ_আকার_2 = 10 * 1024 * 1024;
        স্তূপ_গুনতি_2 = 5;
    }

    শূন্য MemoryPool::initialize(size_t স্তূপ_আকার, পূর্ণসংখ্যা স্তূপ_গুনতি)
    {
        AutoLock lock(mtx_);

        release();

        স্তূপ_আকার_2 = স্তূপ_আকার;
        স্তূপ_গুনতি_2 = স্তূপ_গুনতি;

        initilizeImpl();
    }

    শূন্য MemoryPool::initilizeImpl()
    {
        ধ্রুবক size_t যোগ_আকার = স্তূপ_আকার_2 * স্তূপ_গুনতি_2;

        যদি (যোগ_আকার > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, যোগ_আকার);
            যদি (err != cudaSuccess)
                ফেরত;

            stacks_.resize(স্তূপ_গুনতি_2);

            uchar* ptr = mem_;

            জন্য (পূর্ণসংখ্যা i = 0; i < স্তূপ_গুনতি_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + স্তূপ_আকার_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = সত্য;
                stacks_[i].pool = এই;

                ptr += স্তূপ_আকার_2;
            }

            initialized_ = সত্য;
        }
    }

    শূন্য MemoryPool::release()
    {
        যদি (mem_)
        {
#যদি !defined(NDEBUG)
            জন্য (পূর্ণসংখ্যা i = 0; i < স্তূপ_গুনতি_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = মিথ্যা;
        }
    }

    Memory_স্তূপ* MemoryPool::get_Free_Mem_স্তূপ()
    {
        AutoLock lock(mtx_);

        যদি (!initialized_)
            initilizeImpl();

        যদি (!mem_)
            ফেরত 0;

        জন্য (পূর্ণসংখ্যা i = 0; i < স্তূপ_গুনতি_2; ++i)
        {
            যদি (stacks_[i].isFree)
            {
                stacks_[i].isFree = মিথ্যা;
                ফেরত &stacks_[i];
            }
        }

        ফেরত 0;
    }

    শূন্য MemoryPool::return_Mem_স্তূপ(Memory_স্তূপ* mem_স্তূপ)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_স্তূপ->isFree );

#যদি !defined(NDEBUG)
        যৌক্তিক found = মিথ্যা;
        জন্য (পূর্ণসংখ্যা i = 0; i < স্তূপ_গুনতি_2; ++i)
        {
            যদি (mem_স্তূপ == &stacks_[i])
            {
                found = সত্য;
                বিরতি;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_স্তূপ->tip == mem_স্তূপ->datastart );

        mem_স্তূপ->isFree = সত্য;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

শ্রেণী cv::cuda::Stream::Impl
{
সর্বজনীন:
    Impl(শূন্য* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#অন্যথা

নামাকাশ
{
    শ্রেণী স্তূপ_Allocator;
}

শ্রেণী cv::cuda::Stream::Impl
{
সর্বজনীন:
    cudaStream_t stream;
    যৌক্তিক ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ধ্রুবক Ptr<GpuMat::Allocator>& allocator);
    Impl(ধ্রুবক unsigned পূর্ণসংখ্যা cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(মিথ্যা)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = সত্য;

    allocator = makePtr<স্তূপ_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ধ্রুবক Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(মিথ্যা), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = সত্য;
}

cv::cuda::Stream::Impl::Impl(ধ্রুবক unsigned পূর্ণসংখ্যা cudaFlags) : stream(0), ownStream(মিথ্যা)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = সত্য;
    allocator = makePtr<স্তূপ_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(মিথ্যা)
{
    allocator = makePtr<স্তূপ_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    যদি (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

নামাকাশ cv { নামাকাশ cuda
{
    শ্রেণী DefaultDeviceInitializer
    {
    সর্বজনীন:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(পূর্ণসংখ্যা deviceId);
        MemoryPool& getMemoryPool(পূর্ণসংখ্যা deviceId);

    ব্যক্তিগত:
        শূন্য initStreams();
        শূন্য initPools();

        std::vector<Ptr<Stream> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<MemoryPool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        জন্য (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<পূর্ণসংখ্যা>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(পূর্ণসংখ্যা deviceId)
    {
        AutoLock lock(streams_mtx_);

        যদি (streams_.empty())
        {
            পূর্ণসংখ্যা device_গুনতি = get_Cuda_Enabled_Device_গুনতি();

            যদি (device_গুনতি > 0)
                streams_.resize(device_গুনতি);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<পূর্ণসংখ্যা>(streams_.size()) );

        যদি (streams_[deviceId].empty())
        {
            cudaStream_t stream = ফাঁকা;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(নতুন Stream(impl));
        }

        ফেরত *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(পূর্ণসংখ্যা deviceId)
    {
        AutoLock lock(pools_mtx_);

        যদি (pools_.empty())
        {
            পূর্ণসংখ্যা device_গুনতি = get_Cuda_Enabled_Device_গুনতি();

            যদি (device_গুনতি > 0)
            {
                pools_.resize(device_গুনতি);
                জন্য (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<পূর্ণসংখ্যা>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<পূর্ণসংখ্যা>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        ফেরত *p;
    }

    DefaultDeviceInitializer initializer;
}}

#endif

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::Stream::Stream()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(ধ্রুবক Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#অন্যথা
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ধ্রুবক size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#অন্যথা
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

যৌক্তিক cv::cuda::Stream::queryIfComplete() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaError_t err = cudaStreamQuery(impl_->stream);

    যদি (err == cudaErrorNotReady || err == cudaSuccess)
        ফেরত err == cudaSuccess;

    cudaSafeCall(err);
    ফেরত মিথ্যা;
#endif
}

শূন্য cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

শূন্য cv::cuda::Stream::waitEvent(ধ্রুবক Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#যদি defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

নামাকাশ
{
    কাঠামো Callback_তথ্য
    {
        Stream::StreamCallback callback;
        শূন্য* user_তথ্য;

        Callback_তথ্য(Stream::StreamCallback callback_, শূন্য* user_তথ্য_2) : callback(callback_), user_তথ্য(user_তথ্য_2) {}
    };

    শূন্য CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, শূন্য* user_তথ্য)
    {
        Callback_তথ্য* তথ্য = reinterpret_cast<Callback_তথ্য*>(user_তথ্য);
        তথ্য->callback(static_cast<পূর্ণসংখ্যা>(status), তথ্য->user_তথ্য);
        delete তথ্য;
    }
}

#endif

শূন্য cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, শূন্য* user_তথ্য)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(user_তথ্য);
    throw_no_cuda();
#অন্যথা
    #যদি CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(user_তথ্য);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #অন্যথা
        Callback_তথ্য* তথ্য = নতুন Callback_তথ্য(callback, user_তথ্য);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, তথ্য, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ধ্রুবক পূর্ণসংখ্যা deviceId = getDevice();
    ফেরত initializer.getNullStream(deviceId);
#endif
}

শূন্য* cv::cuda::Stream::cudaPtr() ধ্রুবক
{
#ifndef HAVE_CUDA
    ফেরত nullptr;
#অন্যথা
    ফেরত impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ধ্রুবক
{
#ifndef HAVE_CUDA
    ফেরত 0;
#অন্যথা
    ফেরত (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ধ্রুবক Stream& stream)
{
    ফেরত stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    ফেরত Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#অন্যথা
    ফেরত cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

নামাকাশ
{
    যৌক্তিক enableMemoryPool = মিথ্যা;

    শ্রেণী স্তূপ_Allocator : সর্বজনীন GpuMat::Allocator
    {
    সর্বজনীন:
        explicit স্তূপ_Allocator(cudaStream_t stream);
        ~স্তূপ_Allocator();

        যৌক্তিক allocate(GpuMat* mat, পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, size_t elem_আকার) CV_OVERRIDE;
        শূন্য মুক্ত(GpuMat* mat) CV_OVERRIDE;

    ব্যক্তিগত:
        স্তূপ_Allocator(ধ্রুবক স্তূপ_Allocator&);
        স্তূপ_Allocator& operator =(ধ্রুবক স্তূপ_Allocator&);

        cudaStream_t stream_;
        Memory_স্তূপ* mem_স্তূপ_2;
        size_t alignment_;
    };

    স্তূপ_Allocator::স্তূপ_Allocator(cudaStream_t stream) : stream_(stream), mem_স্তূপ_2(0)
    {
        যদি (enableMemoryPool)
        {
            ধ্রুবক পূর্ণসংখ্যা deviceId = getDevice();
            mem_স্তূপ_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_স্তূপ();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    স্তূপ_Allocator::~স্তূপ_Allocator()
    {
        যদি (mem_স্তূপ_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_স্তূপ_2->pool->return_Mem_স্তূপ(mem_স্তূপ_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ফেরত res;
    }

    যৌক্তিক স্তূপ_Allocator::allocate(GpuMat* mat, পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, size_t elem_আকার)
    {
        যদি (mem_স্তূপ_2 == 0)
            ফেরত মিথ্যা;

        size_t pitch, mem_আকার;

        যদি (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elem_আকার, alignment_);
            mem_আকার = pitch * rows;
        }
        অন্যথা
        {
            // Single row or single column must be continuous
            pitch = elem_আকার * cols;
            mem_আকার = alignUp(elem_আকার * cols * rows, 64);
        }

        uchar* ptr = mem_স্তূপ_2->requestMemory(mem_আকার);

        যদি (ptr == 0)
            ফেরত মিথ্যা;

        mat->তথ্য = ptr;
        mat->step = pitch;
        mat->refcount = (পূর্ণসংখ্যা*) fastMalloc(আকার(পূর্ণসংখ্যা));

        ফেরত সত্য;
    }

    শূন্য স্তূপ_Allocator::মুক্ত(GpuMat* mat)
    {
        যদি (mem_স্তূপ_2 == 0)
            ফেরত;

        mem_স্তূপ_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

শূন্য cv::cuda::set_বাফার_Pool_Usage(যৌক্তিক on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#অন্যথা
    enableMemoryPool = on;
#endif
}

শূন্য cv::cuda::set_বাফার_Pool_Config(পূর্ণসংখ্যা deviceId, size_t স্তূপ_আকার, পূর্ণসংখ্যা স্তূপ_গুনতি)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(স্তূপ_আকার);
    CV_UNUSED(স্তূপ_গুনতি);
    throw_no_cuda();
#অন্যথা
    ধ্রুবক পূর্ণসংখ্যা currentDevice = getDevice();

    যদি (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(স্তূপ_আকার, স্তূপ_গুনতি);
    }
    অন্যথা
    {
        ধ্রুবক পূর্ণসংখ্যা device_গুনতি = get_Cuda_Enabled_Device_গুনতি();

        জন্য (deviceId = 0; deviceId < device_গুনতি; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(স্তূপ_আকার, স্তূপ_গুনতি);
        }
    }

    setDevice(currentDevice);
#endif
}

#ifndef HAVE_CUDA
cv::cuda::বাফার_Pool::বাফার_Pool(Stream& stream)
{
    CV_UNUSED(stream);
    throw_no_cuda();
}
#অন্যথা
cv::cuda::বাফার_Pool::বাফার_Pool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::বাফার_Pool::get_বাফার(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#অন্যথা
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    ফেরত buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

শ্রেণী cv::cuda::Event::Impl
{
সর্বজনীন:
    Impl(unsigned পূর্ণসংখ্যা)
    {
        throw_no_cuda();
    }
};

#অন্যথা

শ্রেণী cv::cuda::Event::Impl
{
সর্বজনীন:
    cudaEvent_t event;
    যৌক্তিক ownEvent;

    explicit Impl(unsigned পূর্ণসংখ্যা flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned পূর্ণসংখ্যা flags) : event(0), ownEvent(মিথ্যা)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = সত্য;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(মিথ্যা)
{
}

cv::cuda::Event::Impl::~Impl()
{
    যদি (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ধ্রুবক Event& event)
{
    ফেরত event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ফেরত Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ধ্রুবক Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#অন্যথা
    impl_ = makePtr<Impl>(flags);
#endif
}

শূন্য cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

যৌক্তিক cv::cuda::Event::queryIfComplete() ধ্রুবক
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaError_t err = cudaEventQuery(impl_->event);

    যদি (err == cudaErrorNotReady || err == cudaSuccess)
        ফেরত err == cudaSuccess;

    cudaSafeCall(err);
    ফেরত মিথ্যা;
#endif
}

শূন্য cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

দশমিক cv::cuda::Event::elapsedTime(ধ্রুবক Event& start, ধ্রুবক Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#অন্যথা
    দশমিক ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    ফেরত ms;
#endif
}
