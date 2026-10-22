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

#အကယ်၍ defined(_MSC_VER)
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

    #အကယ်၍ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* MemoryStack::requestMemory(size_t size)
    {
        const size_t freeMem = dataend - tip;

        အကယ်၍ (size > freeMem)
            ပြန်ပေး 0;

        uchar* ptr = tip;

        tip += size;

    #အကယ်၍ !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        ပြန်ပေး ptr;
    }

    void MemoryStack::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #အကယ်၍ !defined(NDEBUG)
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

        void initialize(size_t stackSize, ကိန်းပြည့် stackCount);
        void release();

        MemoryStack* getFreeMemStack();
        void returnMemStack(MemoryStack* memStack);

    private:
        void initilizeImpl();

        Mutex mtx_;

        bool initialized_;
        size_t stackSize_;
        ကိန်းပြည့် stackCount_;

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

    void MemoryPool::initialize(size_t stackSize, ကိန်းပြည့် stackCount)
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

        အကယ်၍ (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            အကယ်၍ (err != cudaSuccess)
                ပြန်ပေး;

            stacks_.resize(stackCount_);

            uchar* ptr = mem_;

            အတွက် (ကိန်းပြည့် i = 0; i < stackCount_; ++i)
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
        အကယ်၍ (mem_)
        {
#အကယ်၍ !defined(NDEBUG)
            အတွက် (ကိန်းပြည့် i = 0; i < stackCount_; ++i)
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

        အကယ်၍ (!initialized_)
            initilizeImpl();

        အကယ်၍ (!mem_)
            ပြန်ပေး 0;

        အတွက် (ကိန်းပြည့် i = 0; i < stackCount_; ++i)
        {
            အကယ်၍ (stacks_[i].isFree)
            {
                stacks_[i].isFree = false;
                ပြန်ပေး &stacks_[i];
            }
        }

        ပြန်ပေး 0;
    }

    void MemoryPool::returnMemStack(MemoryStack* memStack)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !memStack->isFree );

#အကယ်၍ !defined(NDEBUG)
        bool found = false;
        အတွက် (ကိန်းပြည့် i = 0; i < stackCount_; ++i)
        {
            အကယ်၍ (memStack == &stacks_[i])
            {
                found = true;
                bha_nga;
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

#မဟုတ်ပါက

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
    Impl(const unsigned ကိန်းပြည့် cudaFlags);
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

cv::cuda::Stream::Impl::Impl(const unsigned ကိန်းပြည့် cudaFlags) : stream(0), ownStream(false)
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

    အကယ်၍ (stream && ownStream)
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

        Stream& getNullStream(ကိန်းပြည့် deviceId);
        MemoryPool& getMemoryPool(ကိန်းပြည့် deviceId);

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

        အတွက် (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<ကိန်းပြည့်>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(ကိန်းပြည့် deviceId)
    {
        AutoLock lock(streams_mtx_);

        အကယ်၍ (streams_.empty())
        {
            ကိန်းပြည့် deviceCount = getCudaEnabledDeviceCount();

            အကယ်၍ (deviceCount > 0)
                streams_.resize(deviceCount);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ကိန်းပြည့်>(streams_.size()) );

        အကယ်၍ (streams_[deviceId].empty())
        {
            cudaStream_t stream = NULL;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(new Stream(impl));
        }

        ပြန်ပေး *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(ကိန်းပြည့် deviceId)
    {
        AutoLock lock(pools_mtx_);

        အကယ်၍ (pools_.empty())
        {
            ကိန်းပြည့် deviceCount = getCudaEnabledDeviceCount();

            အကယ်၍ (deviceCount > 0)
            {
                pools_.resize(deviceCount);
                အတွက် (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<ကိန်းပြည့်>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ကိန်းပြည့်>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        ပြန်ပေး *p;
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
#မဟုတ်ပါက
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(const Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#မဟုတ်ပါက
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(const size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#မဟုတ်ပါက
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

bool cv::cuda::Stream::queryIfComplete() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#မဟုတ်ပါက
    cudaError_t err = cudaStreamQuery(impl_->stream);

    အကယ်၍ (err == cudaErrorNotReady || err == cudaSuccess)
        ပြန်ပေး err == cudaSuccess;

    cudaSafeCall(err);
    ပြန်ပေး false;
#endif
}

void cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#မဟုတ်ပါက
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

void cv::cuda::Stream::waitEvent(const Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#မဟုတ်ပါက
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#အကယ်၍ defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

namespace
{
    sa_nracanaa CallbackData
    {
        Stream::StreamCallback callback;
        void* userData;

        CallbackData(Stream::StreamCallback callback_, void* userData_) : callback(callback_), userData(userData_) {}
    };

    void CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, void* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<ကိန်းပြည့်>(status), data->userData);
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
#မဟုတ်ပါက
    #အကယ်၍ CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #မဟုတ်ပါက
        CallbackData* data = new CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#မဟုတ်ပါက
    const ကိန်းပြည့် deviceId = getDevice();
    ပြန်ပေး initializer.getNullStream(deviceId);
#endif
}

void* cv::cuda::Stream::cudaPtr() const
{
#ifndef HAVE_CUDA
    ပြန်ပေး nullptr;
#မဟုတ်ပါက
    ပြန်ပေး impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() const
{
#ifndef HAVE_CUDA
    ပြန်ပေး 0;
#မဟုတ်ပါက
    ပြန်ပေး (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(const Stream& stream)
{
    ပြန်ပေး stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    ပြန်ပေး Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#မဟုတ်ပါက
    ပြန်ပေး cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
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

        bool allocate(GpuMat* mat, ကိန်းပြည့် rows, ကိန်းပြည့် cols, size_t elemSize) CV_OVERRIDE;
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
        အကယ်၍ (enableMemoryPool)
        {
            const ကိန်းပြည့် deviceId = getDevice();
            memStack_ = initializer.getMemoryPool(deviceId).getFreeMemStack();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    StackAllocator::~StackAllocator()
    {
        အကယ်၍ (memStack_ != 0)
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
        ပြန်ပေး res;
    }

    bool StackAllocator::allocate(GpuMat* mat, ကိန်းပြည့် rows, ကိန်းပြည့် cols, size_t elemSize)
    {
        အကယ်၍ (memStack_ == 0)
            ပြန်ပေး false;

        size_t pitch, memSize;

        အကယ်၍ (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        မဟုတ်ပါက
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = memStack_->requestMemory(memSize);

        အကယ်၍ (ptr == 0)
            ပြန်ပေး false;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (ကိန်းပြည့်*) fastMalloc(parimaa_na(ကိန်းပြည့်));

        ပြန်ပေး true;
    }

    void StackAllocator::free(GpuMat* mat)
    {
        အကယ်၍ (memStack_ == 0)
            ပြန်ပေး;

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
#မဟုတ်ပါက
    enableMemoryPool = on;
#endif
}

void cv::cuda::setBufferPoolConfig(ကိန်းပြည့် deviceId, size_t stackSize, ကိန်းပြည့် stackCount)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(stackSize);
    CV_UNUSED(stackCount);
    throw_no_cuda();
#မဟုတ်ပါက
    const ကိန်းပြည့် currentDevice = getDevice();

    အကယ်၍ (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(stackSize, stackCount);
    }
    မဟုတ်ပါက
    {
        const ကိန်းပြည့် deviceCount = getCudaEnabledDeviceCount();

        အတွက် (deviceId = 0; deviceId < deviceCount; ++deviceId)
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
#မဟုတ်ပါက
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#မဟုတ်ပါက
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    ပြန်ပေး buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

class cv::cuda::Event::Impl
{
public:
    Impl(unsigned ကိန်းပြည့်)
    {
        throw_no_cuda();
    }
};

#မဟုတ်ပါက

class cv::cuda::Event::Impl
{
public:
    cudaEvent_t event;
    bool ownEvent;

    explicit Impl(unsigned ကိန်းပြည့် flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned ကိန်းပြည့် flags) : event(0), ownEvent(false)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = true;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(false)
{
}

cv::cuda::Event::Impl::~Impl()
{
    အကယ်၍ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(const Event& event)
{
    ပြန်ပေး event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ပြန်ပေး Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(const Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#မဟုတ်ပါက
    impl_ = makePtr<Impl>(flags);
#endif
}

void cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#မဟုတ်ပါက
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

bool cv::cuda::Event::queryIfComplete() const
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#မဟုတ်ပါက
    cudaError_t err = cudaEventQuery(impl_->event);

    အကယ်၍ (err == cudaErrorNotReady || err == cudaSuccess)
        ပြန်ပေး err == cudaSuccess;

    cudaSafeCall(err);
    ပြန်ပေး false;
#endif
}

void cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#မဟုတ်ပါက
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

float cv::cuda::Event::elapsedTime(const Event& start, const Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#မဟုတ်ပါက
    float ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    ပြန်ပေး ms;
#endif
}
