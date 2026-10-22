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

#include "precomp.hpp"
#include <cstdint>

using namespace cv;
using namespace cv::cuda;

#如果 defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

namespace
{
    class MemoryPool;

    class MemoryStack
    {
    public:
        uchar* requestMemory(size_t size);
        void returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        bool isFree;
        MemoryPool* pool;

    #如果 !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* MemoryStack::requestMemory(size_t size)
    {
        const size_t freeMem = dataend - tip;

        如果 (size > freeMem)
            返回 0;

        uchar* ptr = tip;

        tip += size;

    #如果 !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        返回 ptr;
    }

    void MemoryStack::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #如果 !defined(NDEBUG)
        const size_t allocSize = tip - ptr;
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

namespace
{
    class MemoryPool
    {
    public:
        MemoryPool();
        ~MemoryPool() { release(); }

        void initialize(size_t stackSize, 整数 stackCount);
        void release();

        MemoryStack* getFreeMemStack();
        void returnMemStack(MemoryStack* memStack);

    private:
        void initilizeImpl();

        Mutex mtx_;

        bool initialized_;
        size_t stackSize_;
        整数 stackCount_;

        uchar* mem_;

        std::vector<MemoryStack> stacks_;

        MemoryPool(const MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(false), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        stackSize_ = 10 * 1024 * 1024;
        stackCount_ = 5;
    }

    void MemoryPool::initialize(size_t stackSize, 整数 stackCount)
    {
        AutoLock lock(mtx_);

        release();

        stackSize_ = stackSize;
        stackCount_ = stackCount;

        initilizeImpl();
    }

    void MemoryPool::initilizeImpl()
    {
        const size_t totalSize = stackSize_ * stackCount_;

        如果 (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            如果 (err != cudaSuccess)
                返回;

            stacks_.resize(stackCount_);

            uchar* ptr = mem_;

            为 (整数 i = 0; i < stackCount_; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + stackSize_;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = true;
                stacks_[i].pool = this;

                ptr += stackSize_;
            }

            initialized_ = true;
        }
    }

    void MemoryPool::release()
    {
        如果 (mem_)
        {
#如果 !defined(NDEBUG)
            为 (整数 i = 0; i < stackCount_; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = false;
        }
    }

    MemoryStack* MemoryPool::getFreeMemStack()
    {
        AutoLock lock(mtx_);

        如果 (!initialized_)
            initilizeImpl();

        如果 (!mem_)
            返回 0;

        为 (整数 i = 0; i < stackCount_; ++i)
        {
            如果 (stacks_[i].isFree)
            {
                stacks_[i].isFree = false;
                返回 &stacks_[i];
            }
        }

        返回 0;
    }

    void MemoryPool::returnMemStack(MemoryStack* memStack)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !memStack->isFree );

#如果 !defined(NDEBUG)
        bool found = false;
        为 (整数 i = 0; i < stackCount_; ++i)
        {
            如果 (memStack == &stacks_[i])
            {
                found = true;
                break;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( memStack->tip == memStack->datastart );

        memStack->isFree = true;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

class cv::cuda::Stream::Impl
{
public:
    Impl(void* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#否则

namespace
{
    class StackAllocator;
}

class cv::cuda::Stream::Impl
{
public:
    cudaStream_t stream;
    bool ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(const Ptr<GpuMat::Allocator>& allocator);
    Impl(const unsigned 整数 cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(false)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = true;

    allocator = makePtr<StackAllocator>(stream);
}

cv::cuda::Stream::Impl::Impl(const Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(false), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = true;
}

cv::cuda::Stream::Impl::Impl(const unsigned 整数 cudaFlags) : stream(0), ownStream(false)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = true;
    allocator = makePtr<StackAllocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(false)
{
    allocator = makePtr<StackAllocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    如果 (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

namespace cv { namespace cuda
{
    class DefaultDeviceInitializer
    {
    public:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(整数 deviceId);
        MemoryPool& getMemoryPool(整数 deviceId);

    private:
        void initStreams();
        void initPools();

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

        为 (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<整数>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(整数 deviceId)
    {
        AutoLock lock(streams_mtx_);

        如果 (streams_.empty())
        {
            整数 deviceCount = getCudaEnabledDeviceCount();

            如果 (deviceCount > 0)
                streams_.resize(deviceCount);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<整数>(streams_.size()) );

        如果 (streams_[deviceId].empty())
        {
            cudaStream_t stream = NULL;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(new Stream(impl));
        }

        返回 *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(整数 deviceId)
    {
        AutoLock lock(pools_mtx_);

        如果 (pools_.empty())
        {
            整数 deviceCount = getCudaEnabledDeviceCount();

            如果 (deviceCount > 0)
            {
                pools_.resize(deviceCount);
                为 (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<整数>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<整数>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        返回 *p;
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
#否则
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(const Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#否则
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(const size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#否则
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

bool cv::cuda::Stream::queryIfComplete() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#否则
    cudaError_t err = cudaStreamQuery(impl_->stream);

    如果 (err == cudaErrorNotReady || err == cudaSuccess)
        返回 err == cudaSuccess;

    cudaSafeCall(err);
    返回 false;
#endif
}

void cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#否则
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

void cv::cuda::Stream::waitEvent(const Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#否则
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#如果 defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

namespace
{
    struct CallbackData
    {
        Stream::StreamCallback callback;
        void* userData;

        CallbackData(Stream::StreamCallback callback_, void* userData_) : callback(callback_), userData(userData_) {}
    };

    void CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, void* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<整数>(status), data->userData);
        delete data;
    }
}

#endif

void cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, void* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#否则
    #如果 CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #否则
        CallbackData* data = new CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#否则
    const 整数 deviceId = getDevice();
    返回 initializer.getNullStream(deviceId);
#endif
}

void* cv::cuda::Stream::cudaPtr() const
{
#ifndef HAVE_CUDA
    返回 nullptr;
#否则
    返回 impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() const
{
#ifndef HAVE_CUDA
    返回 0;
#否则
    返回 (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(const Stream& stream)
{
    返回 stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    返回 Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#否则
    返回 cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

namespace
{
    bool enableMemoryPool = false;

    class StackAllocator : public GpuMat::Allocator
    {
    public:
        explicit StackAllocator(cudaStream_t stream);
        ~StackAllocator();

        bool allocate(GpuMat* mat, 整数 rows, 整数 cols, size_t elemSize) CV_OVERRIDE;
        void free(GpuMat* mat) CV_OVERRIDE;

    private:
        StackAllocator(const StackAllocator&);
        StackAllocator& operator =(const StackAllocator&);

        cudaStream_t stream_;
        MemoryStack* memStack_;
        size_t alignment_;
    };

    StackAllocator::StackAllocator(cudaStream_t stream) : stream_(stream), memStack_(0)
    {
        如果 (enableMemoryPool)
        {
            const 整数 deviceId = getDevice();
            memStack_ = initializer.getMemoryPool(deviceId).getFreeMemStack();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    StackAllocator::~StackAllocator()
    {
        如果 (memStack_ != 0)
        {
            cudaStreamSynchronize(stream_);
            memStack_->pool->returnMemStack(memStack_);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        返回 res;
    }

    bool StackAllocator::allocate(GpuMat* mat, 整数 rows, 整数 cols, size_t elemSize)
    {
        如果 (memStack_ == 0)
            返回 false;

        size_t pitch, memSize;

        如果 (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        否则
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = memStack_->requestMemory(memSize);

        如果 (ptr == 0)
            返回 false;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (整数*) fastMalloc(sizeof(整数));

        返回 true;
    }

    void StackAllocator::free(GpuMat* mat)
    {
        如果 (memStack_ == 0)
            返回;

        memStack_->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

void cv::cuda::setBufferPoolUsage(bool on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#否则
    enableMemoryPool = on;
#endif
}

void cv::cuda::setBufferPoolConfig(整数 deviceId, size_t stackSize, 整数 stackCount)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(stackSize);
    CV_UNUSED(stackCount);
    throw_no_cuda();
#否则
    const 整数 currentDevice = getDevice();

    如果 (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(stackSize, stackCount);
    }
    否则
    {
        const 整数 deviceCount = getCudaEnabledDeviceCount();

        为 (deviceId = 0; deviceId < deviceCount; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(stackSize, stackCount);
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
#否则
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(整数 rows, 整数 cols, 整数 type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#否则
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    返回 buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

class cv::cuda::Event::Impl
{
public:
    Impl(unsigned 整数)
    {
        throw_no_cuda();
    }
};

#否则

class cv::cuda::Event::Impl
{
public:
    cudaEvent_t event;
    bool ownEvent;

    explicit Impl(unsigned 整数 flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned 整数 flags) : event(0), ownEvent(false)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = true;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(false)
{
}

cv::cuda::Event::Impl::~Impl()
{
    如果 (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(const Event& event)
{
    返回 event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    返回 Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(const Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#否则
    impl_ = makePtr<Impl>(flags);
#endif
}

void cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#否则
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

bool cv::cuda::Event::queryIfComplete() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#否则
    cudaError_t err = cudaEventQuery(impl_->event);

    如果 (err == cudaErrorNotReady || err == cudaSuccess)
        返回 err == cudaSuccess;

    cudaSafeCall(err);
    返回 false;
#endif
}

void cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#否则
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

float cv::cuda::Event::elapsedTime(const Event& start, const Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#否则
    float ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    返回 ms;
#endif
}
