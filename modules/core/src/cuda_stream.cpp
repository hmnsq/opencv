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
    বর্গ MemoryPool;

    বর্গ Memory_স্তূপ
    {
    সার্বজনিক:
        uchar* requestMemory(size_t size);
        শূন্য returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        তার্কিক isFree;
        MemoryPool* pool;

    #যদি !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_স্তূপ::requestMemory(size_t size)
    {
        ধ্রুব size_t freeMem = dataend - tip;

        যদি (size > freeMem)
            প্রত্যাগম 0;

        uchar* ptr = tip;

        tip += size;

    #যদি !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        প্রত্যাগম ptr;
    }

    শূন্য Memory_স্তূপ::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #যদি !defined(NDEBUG)
        ধ্রুব size_t allocSize = tip - ptr;
        CV_Assert( allocSize == allocations.back() );
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
    বর্গ MemoryPool
    {
    সার্বজনিক:
        MemoryPool();
        ~MemoryPool() { release(); }

        শূন্য initialize(size_t স্তূপ_Size, পূর্ণাঙ্ক স্তূপ_গুনতি);
        শূন্য release();

        Memory_স্তূপ* get_Free_Mem_স্তূপ();
        শূন্য return_Mem_স্তূপ(Memory_স্তূপ* mem_স্তূপ);

    গুপ্ত:
        শূন্য initilizeImpl();

        Mutex mtx_;

        তার্কিক initialized_;
        size_t স্তূপ_Size_2;
        পূর্ণাঙ্ক স্তূপ_গুনতি_2;

        uchar* mem_;

        std::vector<Memory_স্তূপ> stacks_;

        MemoryPool(ধ্রুব MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(অসত্য), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        স্তূপ_Size_2 = 10 * 1024 * 1024;
        স্তূপ_গুনতি_2 = 5;
    }

    শূন্য MemoryPool::initialize(size_t স্তূপ_Size, পূর্ণাঙ্ক স্তূপ_গুনতি)
    {
        AutoLock lock(mtx_);

        release();

        স্তূপ_Size_2 = স্তূপ_Size;
        স্তূপ_গুনতি_2 = স্তূপ_গুনতি;

        initilizeImpl();
    }

    শূন্য MemoryPool::initilizeImpl()
    {
        ধ্রুব size_t totalSize = স্তূপ_Size_2 * স্তূপ_গুনতি_2;

        যদি (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            যদি (err != cudaSuccess)
                প্রত্যাগম;

            stacks_.resize(স্তূপ_গুনতি_2);

            uchar* ptr = mem_;

            প্রতি (পূর্ণাঙ্ক i = 0; i < স্তূপ_গুনতি_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + স্তূপ_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = সত্য;
                stacks_[i].pool = এতত্;

                ptr += স্তূপ_Size_2;
            }

            initialized_ = সত্য;
        }
    }

    শূন্য MemoryPool::release()
    {
        যদি (mem_)
        {
#যদি !defined(NDEBUG)
            প্রতি (পূর্ণাঙ্ক i = 0; i < স্তূপ_গুনতি_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = অসত্য;
        }
    }

    Memory_স্তূপ* MemoryPool::get_Free_Mem_স্তূপ()
    {
        AutoLock lock(mtx_);

        যদি (!initialized_)
            initilizeImpl();

        যদি (!mem_)
            প্রত্যাগম 0;

        প্রতি (পূর্ণাঙ্ক i = 0; i < স্তূপ_গুনতি_2; ++i)
        {
            যদি (stacks_[i].isFree)
            {
                stacks_[i].isFree = অসত্য;
                প্রত্যাগম &stacks_[i];
            }
        }

        প্রত্যাগম 0;
    }

    শূন্য MemoryPool::return_Mem_স্তূপ(Memory_স্তূপ* mem_স্তূপ)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_স্তূপ->isFree );

#যদি !defined(NDEBUG)
        তার্কিক found = অসত্য;
        প্রতি (পূর্ণাঙ্ক i = 0; i < স্তূপ_গুনতি_2; ++i)
        {
            যদি (mem_স্তূপ == &stacks_[i])
            {
                found = সত্য;
                ভঙ্গ;
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

বর্গ cv::cuda::Stream::Impl
{
সার্বজনিক:
    Impl(শূন্য* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#অন্যথা

নামাকাশ
{
    বর্গ স্তূপ_Allocator;
}

বর্গ cv::cuda::Stream::Impl
{
সার্বজনিক:
    cudaStream_t stream;
    তার্কিক ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ধ্রুব Ptr<GpuMat::Allocator>& allocator);
    Impl(ধ্রুব unsigned পূর্ণাঙ্ক cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(অসত্য)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = সত্য;

    allocator = makePtr<স্তূপ_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ধ্রুব Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(অসত্য), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = সত্য;
}

cv::cuda::Stream::Impl::Impl(ধ্রুব unsigned পূর্ণাঙ্ক cudaFlags) : stream(0), ownStream(অসত্য)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = সত্য;
    allocator = makePtr<স্তূপ_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(অসত্য)
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
    বর্গ DefaultDeviceInitializer
    {
    সার্বজনিক:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(পূর্ণাঙ্ক deviceId);
        MemoryPool& getMemoryPool(পূর্ণাঙ্ক deviceId);

    গুপ্ত:
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

        প্রতি (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<পূর্ণাঙ্ক>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(পূর্ণাঙ্ক deviceId)
    {
        AutoLock lock(streams_mtx_);

        যদি (streams_.empty())
        {
            পূর্ণাঙ্ক device_গুনতি = get_Cuda_Enabled_Device_গুনতি();

            যদি (device_গুনতি > 0)
                streams_.resize(device_গুনতি);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<পূর্ণাঙ্ক>(streams_.size()) );

        যদি (streams_[deviceId].empty())
        {
            cudaStream_t stream = রিক্ত;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(নূতন Stream(impl));
        }

        প্রত্যাগম *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(পূর্ণাঙ্ক deviceId)
    {
        AutoLock lock(pools_mtx_);

        যদি (pools_.empty())
        {
            পূর্ণাঙ্ক device_গুনতি = get_Cuda_Enabled_Device_গুনতি();

            যদি (device_গুনতি > 0)
            {
                pools_.resize(device_গুনতি);
                প্রতি (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<পূর্ণাঙ্ক>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<পূর্ণাঙ্ক>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        প্রত্যাগম *p;
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

cv::cuda::Stream::Stream(ধ্রুব Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#অন্যথা
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ধ্রুব size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#অন্যথা
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

তার্কিক cv::cuda::Stream::queryIfComplete() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaError_t err = cudaStreamQuery(impl_->stream);

    যদি (err == cudaErrorNotReady || err == cudaSuccess)
        প্রত্যাগম err == cudaSuccess;

    cudaSafeCall(err);
    প্রত্যাগম অসত্য;
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

শূন্য cv::cuda::Stream::waitEvent(ধ্রুব Event& event)
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
    সংরচনা CallbackData
    {
        Stream::StreamCallback callback;
        শূন্য* userData;

        CallbackData(Stream::StreamCallback callback_, শূন্য* userData_) : callback(callback_), userData(userData_) {}
    };

    শূন্য CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, শূন্য* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<পূর্ণাঙ্ক>(status), data->userData);
        delete data;
    }
}

#endif

শূন্য cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, শূন্য* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#অন্যথা
    #যদি CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #অন্যথা
        CallbackData* data = নূতন CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    ধ্রুব পূর্ণাঙ্ক deviceId = getDevice();
    প্রত্যাগম initializer.getNullStream(deviceId);
#endif
}

শূন্য* cv::cuda::Stream::cudaPtr() ধ্রুব
{
#ifndef HAVE_CUDA
    প্রত্যাগম nullptr;
#অন্যথা
    প্রত্যাগম impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ধ্রুব
{
#ifndef HAVE_CUDA
    প্রত্যাগম 0;
#অন্যথা
    প্রত্যাগম (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ধ্রুব Stream& stream)
{
    প্রত্যাগম stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    প্রত্যাগম Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#অন্যথা
    প্রত্যাগম cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

নামাকাশ
{
    তার্কিক enableMemoryPool = অসত্য;

    বর্গ স্তূপ_Allocator : সার্বজনিক GpuMat::Allocator
    {
    সার্বজনিক:
        explicit স্তূপ_Allocator(cudaStream_t stream);
        ~স্তূপ_Allocator();

        তার্কিক allocate(GpuMat* mat, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, size_t elemSize) CV_OVERRIDE;
        শূন্য মুক্ত(GpuMat* mat) CV_OVERRIDE;

    গুপ্ত:
        স্তূপ_Allocator(ধ্রুব স্তূপ_Allocator&);
        স্তূপ_Allocator& operator =(ধ্রুব স্তূপ_Allocator&);

        cudaStream_t stream_;
        Memory_স্তূপ* mem_স্তূপ_2;
        size_t alignment_;
    };

    স্তূপ_Allocator::স্তূপ_Allocator(cudaStream_t stream) : stream_(stream), mem_স্তূপ_2(0)
    {
        যদি (enableMemoryPool)
        {
            ধ্রুব পূর্ণাঙ্ক deviceId = getDevice();
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
        প্রত্যাগম res;
    }

    তার্কিক স্তূপ_Allocator::allocate(GpuMat* mat, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, size_t elemSize)
    {
        যদি (mem_স্তূপ_2 == 0)
            প্রত্যাগম অসত্য;

        size_t pitch, memSize;

        যদি (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        অন্যথা
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_স্তূপ_2->requestMemory(memSize);

        যদি (ptr == 0)
            প্রত্যাগম অসত্য;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (পূর্ণাঙ্ক*) fastMalloc(পরিমাণ(পূর্ণাঙ্ক));

        প্রত্যাগম সত্য;
    }

    শূন্য স্তূপ_Allocator::মুক্ত(GpuMat* mat)
    {
        যদি (mem_স্তূপ_2 == 0)
            প্রত্যাগম;

        mem_স্তূপ_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

শূন্য cv::cuda::setBufferPoolUsage(তার্কিক on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#অন্যথা
    enableMemoryPool = on;
#endif
}

শূন্য cv::cuda::setBufferPoolConfig(পূর্ণাঙ্ক deviceId, size_t স্তূপ_Size, পূর্ণাঙ্ক স্তূপ_গুনতি)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(স্তূপ_Size);
    CV_UNUSED(স্তূপ_গুনতি);
    throw_no_cuda();
#অন্যথা
    ধ্রুব পূর্ণাঙ্ক currentDevice = getDevice();

    যদি (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(স্তূপ_Size, স্তূপ_গুনতি);
    }
    অন্যথা
    {
        ধ্রুব পূর্ণাঙ্ক device_গুনতি = get_Cuda_Enabled_Device_গুনতি();

        প্রতি (deviceId = 0; deviceId < device_গুনতি; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(স্তূপ_Size, স্তূপ_গুনতি);
        }
    }

    setDevice(currentDevice);
#endif
}

#ifndef HAVE_CUDA
cv::cuda::BufferPool::BufferPool(Stream& stream)
{
    CV_UNUSED(stream);
    throw_no_cuda();
}
#অন্যথা
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#অন্যথা
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    প্রত্যাগম buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

বর্গ cv::cuda::Event::Impl
{
সার্বজনিক:
    Impl(unsigned পূর্ণাঙ্ক)
    {
        throw_no_cuda();
    }
};

#অন্যথা

বর্গ cv::cuda::Event::Impl
{
সার্বজনিক:
    cudaEvent_t event;
    তার্কিক ownEvent;

    explicit Impl(unsigned পূর্ণাঙ্ক flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned পূর্ণাঙ্ক flags) : event(0), ownEvent(অসত্য)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = সত্য;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(অসত্য)
{
}

cv::cuda::Event::Impl::~Impl()
{
    যদি (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ধ্রুব Event& event)
{
    প্রত্যাগম event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    প্রত্যাগম Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ধ্রুব Event::CreateFlags flags)
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

তার্কিক cv::cuda::Event::queryIfComplete() ধ্রুব
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#অন্যথা
    cudaError_t err = cudaEventQuery(impl_->event);

    যদি (err == cudaErrorNotReady || err == cudaSuccess)
        প্রত্যাগম err == cudaSuccess;

    cudaSafeCall(err);
    প্রত্যাগম অসত্য;
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

ভিন্ন cv::cuda::Event::elapsedTime(ধ্রুব Event& start, ধ্রুব Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#অন্যথা
    ভিন্ন ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    প্রত্যাগম ms;
#endif
}
