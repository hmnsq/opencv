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
#شامل <cstdint>

using فضاینام cv;
using فضاینام cv::cuda;

#اگر defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

فضاینام
{
    رده MemoryPool;

    رده Memory_پشته
    {
    عمومی:
        uchar* requestMemory(size_t size);
        تهی returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        منطقی isFree;
        MemoryPool* pool;

    #اگر !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_پشته::requestMemory(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        اگر (size > freeMem)
            بازگشت 0;

        uchar* ptr = tip;

        tip += size;

    #اگر !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        بازگشت ptr;
    }

    تهی Memory_پشته::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #اگر !defined(NDEBUG)
        ثابت size_t allocSize = tip - ptr;
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

فضاینام
{
    رده MemoryPool
    {
    عمومی:
        MemoryPool();
        ~MemoryPool() { release(); }

        تهی initialize(size_t پشته_Size, صحیح پشته_شمار);
        تهی release();

        Memory_پشته* get_Free_Mem_پشته();
        تهی return_Mem_پشته(Memory_پشته* mem_پشته);

    خصوصی:
        تهی initilizeImpl();

        Mutex mtx_;

        منطقی initialized_;
        size_t پشته_Size_2;
        صحیح پشته_شمار_2;

        uchar* mem_;

        std::vector<Memory_پشته> stacks_;

        MemoryPool(ثابت MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(نادرست), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        پشته_Size_2 = 10 * 1024 * 1024;
        پشته_شمار_2 = 5;
    }

    تهی MemoryPool::initialize(size_t پشته_Size, صحیح پشته_شمار)
    {
        AutoLock lock(mtx_);

        release();

        پشته_Size_2 = پشته_Size;
        پشته_شمار_2 = پشته_شمار;

        initilizeImpl();
    }

    تهی MemoryPool::initilizeImpl()
    {
        ثابت size_t totalSize = پشته_Size_2 * پشته_شمار_2;

        اگر (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            اگر (err != cudaSuccess)
                بازگشت;

            stacks_.resize(پشته_شمار_2);

            uchar* ptr = mem_;

            برای (صحیح i = 0; i < پشته_شمار_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + پشته_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = درست;
                stacks_[i].pool = این;

                ptr += پشته_Size_2;
            }

            initialized_ = درست;
        }
    }

    تهی MemoryPool::release()
    {
        اگر (mem_)
        {
#اگر !defined(NDEBUG)
            برای (صحیح i = 0; i < پشته_شمار_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = نادرست;
        }
    }

    Memory_پشته* MemoryPool::get_Free_Mem_پشته()
    {
        AutoLock lock(mtx_);

        اگر (!initialized_)
            initilizeImpl();

        اگر (!mem_)
            بازگشت 0;

        برای (صحیح i = 0; i < پشته_شمار_2; ++i)
        {
            اگر (stacks_[i].isFree)
            {
                stacks_[i].isFree = نادرست;
                بازگشت &stacks_[i];
            }
        }

        بازگشت 0;
    }

    تهی MemoryPool::return_Mem_پشته(Memory_پشته* mem_پشته)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_پشته->isFree );

#اگر !defined(NDEBUG)
        منطقی found = نادرست;
        برای (صحیح i = 0; i < پشته_شمار_2; ++i)
        {
            اگر (mem_پشته == &stacks_[i])
            {
                found = درست;
                بشکن;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_پشته->tip == mem_پشته->datastart );

        mem_پشته->isFree = درست;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

رده cv::cuda::Stream::Impl
{
عمومی:
    Impl(تهی* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#وگرنه

فضاینام
{
    رده پشته_Allocator;
}

رده cv::cuda::Stream::Impl
{
عمومی:
    cudaStream_t stream;
    منطقی ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت unsigned صحیح cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(نادرست)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = درست;

    allocator = makePtr<پشته_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(نادرست), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = درست;
}

cv::cuda::Stream::Impl::Impl(ثابت unsigned صحیح cudaFlags) : stream(0), ownStream(نادرست)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = درست;
    allocator = makePtr<پشته_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(نادرست)
{
    allocator = makePtr<پشته_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    اگر (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

فضاینام cv { فضاینام cuda
{
    رده DefaultDeviceInitializer
    {
    عمومی:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(صحیح deviceId);
        MemoryPool& getMemoryPool(صحیح deviceId);

    خصوصی:
        تهی initStreams();
        تهی initPools();

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

        برای (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<صحیح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(صحیح deviceId)
    {
        AutoLock lock(streams_mtx_);

        اگر (streams_.empty())
        {
            صحیح device_شمار = get_Cuda_Enabled_Device_شمار();

            اگر (device_شمار > 0)
                streams_.resize(device_شمار);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحیح>(streams_.size()) );

        اگر (streams_[deviceId].empty())
        {
            cudaStream_t stream = پوچ;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(جدید Stream(impl));
        }

        بازگشت *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(صحیح deviceId)
    {
        AutoLock lock(pools_mtx_);

        اگر (pools_.empty())
        {
            صحیح device_شمار = get_Cuda_Enabled_Device_شمار();

            اگر (device_شمار > 0)
            {
                pools_.resize(device_شمار);
                برای (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<صحیح>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحیح>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        بازگشت *p;
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
#وگرنه
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#وگرنه
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ثابت size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#وگرنه
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

منطقی cv::cuda::Stream::queryIfComplete() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    cudaError_t err = cudaStreamQuery(impl_->stream);

    اگر (err == cudaErrorNotReady || err == cudaSuccess)
        بازگشت err == cudaSuccess;

    cudaSafeCall(err);
    بازگشت نادرست;
#endif
}

تهی cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

تهی cv::cuda::Stream::waitEvent(ثابت Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#وگرنه
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#اگر defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

فضاینام
{
    ساختار CallbackData
    {
        Stream::StreamCallback callback;
        تهی* userData;

        CallbackData(Stream::StreamCallback callback_, تهی* userData_) : callback(callback_), userData(userData_) {}
    };

    تهی CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, تهی* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<صحیح>(status), data->userData);
        delete data;
    }
}

#endif

تهی cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, تهی* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#وگرنه
    #اگر CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #وگرنه
        CallbackData* data = جدید CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    ثابت صحیح deviceId = getDevice();
    بازگشت initializer.getNullStream(deviceId);
#endif
}

تهی* cv::cuda::Stream::cudaPtr() ثابت
{
#ifndef HAVE_CUDA
    بازگشت nullptr;
#وگرنه
    بازگشت impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ثابت
{
#ifndef HAVE_CUDA
    بازگشت 0;
#وگرنه
    بازگشت (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ثابت Stream& stream)
{
    بازگشت stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    بازگشت Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#وگرنه
    بازگشت cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

فضاینام
{
    منطقی enableMemoryPool = نادرست;

    رده پشته_Allocator : عمومی GpuMat::Allocator
    {
    عمومی:
        explicit پشته_Allocator(cudaStream_t stream);
        ~پشته_Allocator();

        منطقی allocate(GpuMat* mat, صحیح rows, صحیح cols, size_t elemSize) CV_OVERRIDE;
        تهی آزاد(GpuMat* mat) CV_OVERRIDE;

    خصوصی:
        پشته_Allocator(ثابت پشته_Allocator&);
        پشته_Allocator& operator =(ثابت پشته_Allocator&);

        cudaStream_t stream_;
        Memory_پشته* mem_پشته_2;
        size_t alignment_;
    };

    پشته_Allocator::پشته_Allocator(cudaStream_t stream) : stream_(stream), mem_پشته_2(0)
    {
        اگر (enableMemoryPool)
        {
            ثابت صحیح deviceId = getDevice();
            mem_پشته_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_پشته();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    پشته_Allocator::~پشته_Allocator()
    {
        اگر (mem_پشته_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_پشته_2->pool->return_Mem_پشته(mem_پشته_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        بازگشت res;
    }

    منطقی پشته_Allocator::allocate(GpuMat* mat, صحیح rows, صحیح cols, size_t elemSize)
    {
        اگر (mem_پشته_2 == 0)
            بازگشت نادرست;

        size_t pitch, memSize;

        اگر (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        وگرنه
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_پشته_2->requestMemory(memSize);

        اگر (ptr == 0)
            بازگشت نادرست;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (صحیح*) fastMalloc(اندازه(صحیح));

        بازگشت درست;
    }

    تهی پشته_Allocator::آزاد(GpuMat* mat)
    {
        اگر (mem_پشته_2 == 0)
            بازگشت;

        mem_پشته_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

تهی cv::cuda::setBufferPoolUsage(منطقی on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#وگرنه
    enableMemoryPool = on;
#endif
}

تهی cv::cuda::setBufferPoolConfig(صحیح deviceId, size_t پشته_Size, صحیح پشته_شمار)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(پشته_Size);
    CV_UNUSED(پشته_شمار);
    throw_no_cuda();
#وگرنه
    ثابت صحیح currentDevice = getDevice();

    اگر (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(پشته_Size, پشته_شمار);
    }
    وگرنه
    {
        ثابت صحیح device_شمار = get_Cuda_Enabled_Device_شمار();

        برای (deviceId = 0; deviceId < device_شمار; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(پشته_Size, پشته_شمار);
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
#وگرنه
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(صحیح rows, صحیح cols, صحیح type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#وگرنه
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    بازگشت buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

رده cv::cuda::Event::Impl
{
عمومی:
    Impl(unsigned صحیح)
    {
        throw_no_cuda();
    }
};

#وگرنه

رده cv::cuda::Event::Impl
{
عمومی:
    cudaEvent_t event;
    منطقی ownEvent;

    explicit Impl(unsigned صحیح flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned صحیح flags) : event(0), ownEvent(نادرست)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = درست;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(نادرست)
{
}

cv::cuda::Event::Impl::~Impl()
{
    اگر (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ثابت Event& event)
{
    بازگشت event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    بازگشت Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#وگرنه
    impl_ = makePtr<Impl>(flags);
#endif
}

تهی cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#وگرنه
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

منطقی cv::cuda::Event::queryIfComplete() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    cudaError_t err = cudaEventQuery(impl_->event);

    اگر (err == cudaErrorNotReady || err == cudaSuccess)
        بازگشت err == cudaSuccess;

    cudaSafeCall(err);
    بازگشت نادرست;
#endif
}

تهی cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#وگرنه
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

اعشاری cv::cuda::Event::elapsedTime(ثابت Event& start, ثابت Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#وگرنه
    اعشاری ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    بازگشت ms;
#endif
}
