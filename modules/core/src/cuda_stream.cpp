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

#якщо defined(_MSC_VER)
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

    #якщо !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* MemoryStack::requestMemory(size_t size)
    {
        const size_t freeMem = dataend - tip;

        якщо (size > freeMem)
            повернути 0;

        uchar* ptr = tip;

        tip += size;

    #якщо !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        повернути ptr;
    }

    void MemoryStack::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #якщо !defined(NDEBUG)
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

        void initialize(size_t stackSize, ціле stackCount);
        void release();

        MemoryStack* getFreeMemStack();
        void returnMemStack(MemoryStack* memStack);

    private:
        void initilizeImpl();

        Mutex mtx_;

        bool initialized_;
        size_t stackSize_;
        ціле stackCount_;

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

    void MemoryPool::initialize(size_t stackSize, ціле stackCount)
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

        якщо (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            якщо (err != cudaSuccess)
                повернути;

            stacks_.resize(stackCount_);

            uchar* ptr = mem_;

            для (ціле i = 0; i < stackCount_; ++i)
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
        якщо (mem_)
        {
#якщо !defined(NDEBUG)
            для (ціле i = 0; i < stackCount_; ++i)
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

        якщо (!initialized_)
            initilizeImpl();

        якщо (!mem_)
            повернути 0;

        для (ціле i = 0; i < stackCount_; ++i)
        {
            якщо (stacks_[i].isFree)
            {
                stacks_[i].isFree = false;
                повернути &stacks_[i];
            }
        }

        повернути 0;
    }

    void MemoryPool::returnMemStack(MemoryStack* memStack)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !memStack->isFree );

#якщо !defined(NDEBUG)
        bool found = false;
        для (ціле i = 0; i < stackCount_; ++i)
        {
            якщо (memStack == &stacks_[i])
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

#інакше

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
    Impl(const unsigned ціле cudaFlags);
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

cv::cuda::Stream::Impl::Impl(const unsigned ціле cudaFlags) : stream(0), ownStream(false)
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

    якщо (stream && ownStream)
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

        Stream& getNullStream(ціле deviceId);
        MemoryPool& getMemoryPool(ціле deviceId);

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

        для (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<ціле>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(ціле deviceId)
    {
        AutoLock lock(streams_mtx_);

        якщо (streams_.empty())
        {
            ціле deviceCount = getCudaEnabledDeviceCount();

            якщо (deviceCount > 0)
                streams_.resize(deviceCount);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ціле>(streams_.size()) );

        якщо (streams_[deviceId].empty())
        {
            cudaStream_t stream = NULL;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(new Stream(impl));
        }

        повернути *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(ціле deviceId)
    {
        AutoLock lock(pools_mtx_);

        якщо (pools_.empty())
        {
            ціле deviceCount = getCudaEnabledDeviceCount();

            якщо (deviceCount > 0)
            {
                pools_.resize(deviceCount);
                для (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<ціле>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ціле>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        повернути *p;
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
#інакше
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(const Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#інакше
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(const size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#інакше
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

bool cv::cuda::Stream::queryIfComplete() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#інакше
    cudaError_t err = cudaStreamQuery(impl_->stream);

    якщо (err == cudaErrorNotReady || err == cudaSuccess)
        повернути err == cudaSuccess;

    cudaSafeCall(err);
    повернути false;
#endif
}

void cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#інакше
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

void cv::cuda::Stream::waitEvent(const Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#інакше
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#якщо defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

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
        data->callback(static_cast<ціле>(status), data->userData);
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
#інакше
    #якщо CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #інакше
        CallbackData* data = new CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#інакше
    const ціле deviceId = getDevice();
    повернути initializer.getNullStream(deviceId);
#endif
}

void* cv::cuda::Stream::cudaPtr() const
{
#ifndef HAVE_CUDA
    повернути nullptr;
#інакше
    повернути impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() const
{
#ifndef HAVE_CUDA
    повернути 0;
#інакше
    повернути (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(const Stream& stream)
{
    повернути stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    повернути Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#інакше
    повернути cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
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

        bool allocate(GpuMat* mat, ціле rows, ціле cols, size_t elemSize) CV_OVERRIDE;
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
        якщо (enableMemoryPool)
        {
            const ціле deviceId = getDevice();
            memStack_ = initializer.getMemoryPool(deviceId).getFreeMemStack();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    StackAllocator::~StackAllocator()
    {
        якщо (memStack_ != 0)
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
        повернути res;
    }

    bool StackAllocator::allocate(GpuMat* mat, ціле rows, ціле cols, size_t elemSize)
    {
        якщо (memStack_ == 0)
            повернути false;

        size_t pitch, memSize;

        якщо (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        інакше
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = memStack_->requestMemory(memSize);

        якщо (ptr == 0)
            повернути false;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (ціле*) fastMalloc(sizeof(ціле));

        повернути true;
    }

    void StackAllocator::free(GpuMat* mat)
    {
        якщо (memStack_ == 0)
            повернути;

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
#інакше
    enableMemoryPool = on;
#endif
}

void cv::cuda::setBufferPoolConfig(ціле deviceId, size_t stackSize, ціле stackCount)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(stackSize);
    CV_UNUSED(stackCount);
    throw_no_cuda();
#інакше
    const ціле currentDevice = getDevice();

    якщо (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(stackSize, stackCount);
    }
    інакше
    {
        const ціле deviceCount = getCudaEnabledDeviceCount();

        для (deviceId = 0; deviceId < deviceCount; ++deviceId)
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
#інакше
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(ціле rows, ціле cols, ціле type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#інакше
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    повернути buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

class cv::cuda::Event::Impl
{
public:
    Impl(unsigned ціле)
    {
        throw_no_cuda();
    }
};

#інакше

class cv::cuda::Event::Impl
{
public:
    cudaEvent_t event;
    bool ownEvent;

    explicit Impl(unsigned ціле flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned ціле flags) : event(0), ownEvent(false)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = true;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(false)
{
}

cv::cuda::Event::Impl::~Impl()
{
    якщо (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(const Event& event)
{
    повернути event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    повернути Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(const Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#інакше
    impl_ = makePtr<Impl>(flags);
#endif
}

void cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#інакше
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

bool cv::cuda::Event::queryIfComplete() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#інакше
    cudaError_t err = cudaEventQuery(impl_->event);

    якщо (err == cudaErrorNotReady || err == cudaSuccess)
        повернути err == cudaSuccess;

    cudaSafeCall(err);
    повернути false;
#endif
}

void cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#інакше
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

float cv::cuda::Event::elapsedTime(const Event& start, const Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#інакше
    float ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    повернути ms;
#endif
}
