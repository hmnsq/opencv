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

using نطاق cv;
using نطاق cv::cuda;

#ܐܢ defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

نطاق
{
    صنف MemoryPool;

    صنف MemoryStack
    {
    عام:
        uchar* requestMemory(size_t size);
        فراغ returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        منطقي isFree;
        MemoryPool* pool;

    #ܐܢ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* MemoryStack::requestMemory(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        ܐܢ (size > freeMem)
            ܦܢܐ 0;

        uchar* ptr = tip;

        tip += size;

    #ܐܢ !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        ܦܢܐ ptr;
    }

    فراغ MemoryStack::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #ܐܢ !defined(NDEBUG)
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

نطاق
{
    صنف MemoryPool
    {
    عام:
        MemoryPool();
        ~MemoryPool() { release(); }

        فراغ initialize(size_t stackSize, صحيح stackCount);
        فراغ release();

        MemoryStack* getFreeMemStack();
        فراغ returnMemStack(MemoryStack* memStack);

    خاص:
        فراغ initilizeImpl();

        Mutex mtx_;

        منطقي initialized_;
        size_t stackSize_;
        صحيح stackCount_;

        uchar* mem_;

        std::vector<MemoryStack> stacks_;

        MemoryPool(ثابت MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(ܕܓܠܐ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        stackSize_ = 10 * 1024 * 1024;
        stackCount_ = 5;
    }

    فراغ MemoryPool::initialize(size_t stackSize, صحيح stackCount)
    {
        AutoLock lock(mtx_);

        release();

        stackSize_ = stackSize;
        stackCount_ = stackCount;

        initilizeImpl();
    }

    فراغ MemoryPool::initilizeImpl()
    {
        ثابت size_t totalSize = stackSize_ * stackCount_;

        ܐܢ (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            ܐܢ (err != cudaSuccess)
                ܦܢܐ;

            stacks_.resize(stackCount_);

            uchar* ptr = mem_;

            لكل (صحيح i = 0; i < stackCount_; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + stackSize_;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = ܫܪܪܐ;
                stacks_[i].pool = هذا;

                ptr += stackSize_;
            }

            initialized_ = ܫܪܪܐ;
        }
    }

    فراغ MemoryPool::release()
    {
        ܐܢ (mem_)
        {
#ܐܢ !defined(NDEBUG)
            لكل (صحيح i = 0; i < stackCount_; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = ܕܓܠܐ;
        }
    }

    MemoryStack* MemoryPool::getFreeMemStack()
    {
        AutoLock lock(mtx_);

        ܐܢ (!initialized_)
            initilizeImpl();

        ܐܢ (!mem_)
            ܦܢܐ 0;

        لكل (صحيح i = 0; i < stackCount_; ++i)
        {
            ܐܢ (stacks_[i].isFree)
            {
                stacks_[i].isFree = ܕܓܠܐ;
                ܦܢܐ &stacks_[i];
            }
        }

        ܦܢܐ 0;
    }

    فراغ MemoryPool::returnMemStack(MemoryStack* memStack)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !memStack->isFree );

#ܐܢ !defined(NDEBUG)
        منطقي found = ܕܓܠܐ;
        لكل (صحيح i = 0; i < stackCount_; ++i)
        {
            ܐܢ (memStack == &stacks_[i])
            {
                found = ܫܪܪܐ;
                اكسر;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( memStack->tip == memStack->datastart );

        memStack->isFree = ܫܪܪܐ;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

صنف cv::cuda::Stream::Impl
{
عام:
    Impl(فراغ* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#ܐܚܪܢܐ

نطاق
{
    صنف StackAllocator;
}

صنف cv::cuda::Stream::Impl
{
عام:
    cudaStream_t stream;
    منطقي ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت unsigned صحيح cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(ܕܓܠܐ)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = ܫܪܪܐ;

    allocator = makePtr<StackAllocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(ܕܓܠܐ), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = ܫܪܪܐ;
}

cv::cuda::Stream::Impl::Impl(ثابت unsigned صحيح cudaFlags) : stream(0), ownStream(ܕܓܠܐ)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = ܫܪܪܐ;
    allocator = makePtr<StackAllocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(ܕܓܠܐ)
{
    allocator = makePtr<StackAllocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    ܐܢ (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

نطاق cv { نطاق cuda
{
    صنف DefaultDeviceInitializer
    {
    عام:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(صحيح deviceId);
        MemoryPool& getMemoryPool(صحيح deviceId);

    خاص:
        فراغ initStreams();
        فراغ initPools();

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

        لكل (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<صحيح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(صحيح deviceId)
    {
        AutoLock lock(streams_mtx_);

        ܐܢ (streams_.empty())
        {
            صحيح deviceCount = getCudaEnabledDeviceCount();

            ܐܢ (deviceCount > 0)
                streams_.resize(deviceCount);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(streams_.size()) );

        ܐܢ (streams_[deviceId].empty())
        {
            cudaStream_t stream = عدم;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(جديد Stream(impl));
        }

        ܦܢܐ *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(صحيح deviceId)
    {
        AutoLock lock(pools_mtx_);

        ܐܢ (pools_.empty())
        {
            صحيح deviceCount = getCudaEnabledDeviceCount();

            ܐܢ (deviceCount > 0)
            {
                pools_.resize(deviceCount);
                لكل (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<صحيح>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        ܦܢܐ *p;
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
#ܐܚܪܢܐ
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#ܐܚܪܢܐ
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ثابت size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#ܐܚܪܢܐ
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

منطقي cv::cuda::Stream::queryIfComplete() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ܐܚܪܢܐ
    cudaError_t err = cudaStreamQuery(impl_->stream);

    ܐܢ (err == cudaErrorNotReady || err == cudaSuccess)
        ܦܢܐ err == cudaSuccess;

    cudaSafeCall(err);
    ܦܢܐ ܕܓܠܐ;
#endif
}

فراغ cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ܐܚܪܢܐ
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

فراغ cv::cuda::Stream::waitEvent(ثابت Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#ܐܚܪܢܐ
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#ܐܢ defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

نطاق
{
    بنية CallbackData
    {
        Stream::StreamCallback callback;
        فراغ* userData;

        CallbackData(Stream::StreamCallback callback_, فراغ* userData_) : callback(callback_), userData(userData_) {}
    };

    فراغ CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, فراغ* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<صحيح>(status), data->userData);
        delete data;
    }
}

#endif

فراغ cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, فراغ* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#ܐܚܪܢܐ
    #ܐܢ CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #ܐܚܪܢܐ
        CallbackData* data = جديد CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ܐܚܪܢܐ
    ثابت صحيح deviceId = getDevice();
    ܦܢܐ initializer.getNullStream(deviceId);
#endif
}

فراغ* cv::cuda::Stream::cudaPtr() ثابت
{
#ifndef HAVE_CUDA
    ܦܢܐ nullptr;
#ܐܚܪܢܐ
    ܦܢܐ impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ثابت
{
#ifndef HAVE_CUDA
    ܦܢܐ 0;
#ܐܚܪܢܐ
    ܦܢܐ (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ثابت Stream& stream)
{
    ܦܢܐ stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    ܦܢܐ Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#ܐܚܪܢܐ
    ܦܢܐ cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

نطاق
{
    منطقي enableMemoryPool = ܕܓܠܐ;

    صنف StackAllocator : عام GpuMat::Allocator
    {
    عام:
        explicit StackAllocator(cudaStream_t stream);
        ~StackAllocator();

        منطقي allocate(GpuMat* mat, صحيح rows, صحيح cols, size_t elemSize) CV_OVERRIDE;
        فراغ حرر(GpuMat* mat) CV_OVERRIDE;

    خاص:
        StackAllocator(ثابت StackAllocator&);
        StackAllocator& operator =(ثابت StackAllocator&);

        cudaStream_t stream_;
        MemoryStack* memStack_;
        size_t alignment_;
    };

    StackAllocator::StackAllocator(cudaStream_t stream) : stream_(stream), memStack_(0)
    {
        ܐܢ (enableMemoryPool)
        {
            ثابت صحيح deviceId = getDevice();
            memStack_ = initializer.getMemoryPool(deviceId).getFreeMemStack();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    StackAllocator::~StackAllocator()
    {
        ܐܢ (memStack_ != 0)
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
        ܦܢܐ res;
    }

    منطقي StackAllocator::allocate(GpuMat* mat, صحيح rows, صحيح cols, size_t elemSize)
    {
        ܐܢ (memStack_ == 0)
            ܦܢܐ ܕܓܠܐ;

        size_t pitch, memSize;

        ܐܢ (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        ܐܚܪܢܐ
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = memStack_->requestMemory(memSize);

        ܐܢ (ptr == 0)
            ܦܢܐ ܕܓܠܐ;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (صحيح*) fastMalloc(حجم(صحيح));

        ܦܢܐ ܫܪܪܐ;
    }

    فراغ StackAllocator::حرر(GpuMat* mat)
    {
        ܐܢ (memStack_ == 0)
            ܦܢܐ;

        memStack_->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

فراغ cv::cuda::setBufferPoolUsage(منطقي on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#ܐܚܪܢܐ
    enableMemoryPool = on;
#endif
}

فراغ cv::cuda::setBufferPoolConfig(صحيح deviceId, size_t stackSize, صحيح stackCount)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(stackSize);
    CV_UNUSED(stackCount);
    throw_no_cuda();
#ܐܚܪܢܐ
    ثابت صحيح currentDevice = getDevice();

    ܐܢ (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(stackSize, stackCount);
    }
    ܐܚܪܢܐ
    {
        ثابت صحيح deviceCount = getCudaEnabledDeviceCount();

        لكل (deviceId = 0; deviceId < deviceCount; ++deviceId)
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
#ܐܚܪܢܐ
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(صحيح rows, صحيح cols, صحيح type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#ܐܚܪܢܐ
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    ܦܢܐ buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

صنف cv::cuda::Event::Impl
{
عام:
    Impl(unsigned صحيح)
    {
        throw_no_cuda();
    }
};

#ܐܚܪܢܐ

صنف cv::cuda::Event::Impl
{
عام:
    cudaEvent_t event;
    منطقي ownEvent;

    explicit Impl(unsigned صحيح flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned صحيح flags) : event(0), ownEvent(ܕܓܠܐ)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = ܫܪܪܐ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(ܕܓܠܐ)
{
}

cv::cuda::Event::Impl::~Impl()
{
    ܐܢ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ثابت Event& event)
{
    ܦܢܐ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ܦܢܐ Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#ܐܚܪܢܐ
    impl_ = makePtr<Impl>(flags);
#endif
}

فراغ cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#ܐܚܪܢܐ
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

منطقي cv::cuda::Event::queryIfComplete() ثابت
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ܐܚܪܢܐ
    cudaError_t err = cudaEventQuery(impl_->event);

    ܐܢ (err == cudaErrorNotReady || err == cudaSuccess)
        ܦܢܐ err == cudaSuccess;

    cudaSafeCall(err);
    ܦܢܐ ܕܓܠܐ;
#endif
}

فراغ cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ܐܚܪܢܐ
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

عائم cv::cuda::Event::elapsedTime(ثابت Event& start, ثابت Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#ܐܚܪܢܐ
    عائم ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    ܦܢܐ ms;
#endif
}
