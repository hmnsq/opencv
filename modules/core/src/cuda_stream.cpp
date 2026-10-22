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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <cstdint>

using नामाकाश cv;
using नामाकाश cv::cuda;

#सचे defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

नामाकाश
{
    वग्ग MemoryPool;

    वग्ग Memory_राशिः
    {
    सार्वजनिक:
        uchar* requestMemory(size_t size);
        शून्य returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        तार्किक isFree;
        MemoryPool* pool;

    #सचे !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_राशिः::requestMemory(size_t size)
    {
        ध्रुव size_t freeMem = dataend - tip;

        सचे (size > freeMem)
            पटिनिवत्त 0;

        uchar* ptr = tip;

        tip += size;

    #सचे !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        पटिनिवत्त ptr;
    }

    शून्य Memory_राशिः::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #सचे !defined(NDEBUG)
        ध्रुव size_t allocSize = tip - ptr;
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

नामाकाश
{
    वग्ग MemoryPool
    {
    सार्वजनिक:
        MemoryPool();
        ~MemoryPool() { release(); }

        शून्य initialize(size_t राशिः_Size, पूर्णाङ्क राशिः_गणनम्);
        शून्य release();

        Memory_राशिः* get_Free_Mem_राशिः();
        शून्य return_Mem_राशिः(Memory_राशिः* mem_राशिः);

    गुप्त:
        शून्य initilizeImpl();

        Mutex mtx_;

        तार्किक initialized_;
        size_t राशिः_Size_2;
        पूर्णाङ्क राशिः_गणनम्_2;

        uchar* mem_;

        std::vector<Memory_राशिः> stacks_;

        MemoryPool(ध्रुव MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(मिच्छा), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        राशिः_Size_2 = 10 * 1024 * 1024;
        राशिः_गणनम्_2 = 5;
    }

    शून्य MemoryPool::initialize(size_t राशिः_Size, पूर्णाङ्क राशिः_गणनम्)
    {
        AutoLock lock(mtx_);

        release();

        राशिः_Size_2 = राशिः_Size;
        राशिः_गणनम्_2 = राशिः_गणनम्;

        initilizeImpl();
    }

    शून्य MemoryPool::initilizeImpl()
    {
        ध्रुव size_t totalSize = राशिः_Size_2 * राशिः_गणनम्_2;

        सचे (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            सचे (err != cudaSuccess)
                पटिनिवत्त;

            stacks_.resize(राशिः_गणनम्_2);

            uchar* ptr = mem_;

            अत्थाय (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + राशिः_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = सच्च;
                stacks_[i].pool = एतत्;

                ptr += राशिः_Size_2;
            }

            initialized_ = सच्च;
        }
    }

    शून्य MemoryPool::release()
    {
        सचे (mem_)
        {
#सचे !defined(NDEBUG)
            अत्थाय (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = मिच्छा;
        }
    }

    Memory_राशिः* MemoryPool::get_Free_Mem_राशिः()
    {
        AutoLock lock(mtx_);

        सचे (!initialized_)
            initilizeImpl();

        सचे (!mem_)
            पटिनिवत्त 0;

        अत्थाय (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
        {
            सचे (stacks_[i].isFree)
            {
                stacks_[i].isFree = मिच्छा;
                पटिनिवत्त &stacks_[i];
            }
        }

        पटिनिवत्त 0;
    }

    शून्य MemoryPool::return_Mem_राशिः(Memory_राशिः* mem_राशिः)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_राशिः->isFree );

#सचे !defined(NDEBUG)
        तार्किक found = मिच्छा;
        अत्थाय (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
        {
            सचे (mem_राशिः == &stacks_[i])
            {
                found = सच्च;
                भङ्ग;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_राशिः->tip == mem_राशिः->datastart );

        mem_राशिः->isFree = सच्च;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

वग्ग cv::cuda::Stream::Impl
{
सार्वजनिक:
    Impl(शून्य* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#अञ्ञथा

नामाकाश
{
    वग्ग राशिः_Allocator;
}

वग्ग cv::cuda::Stream::Impl
{
सार्वजनिक:
    cudaStream_t stream;
    तार्किक ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ध्रुव Ptr<GpuMat::Allocator>& allocator);
    Impl(ध्रुव unsigned पूर्णाङ्क cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(मिच्छा)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = सच्च;

    allocator = makePtr<राशिः_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ध्रुव Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(मिच्छा), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = सच्च;
}

cv::cuda::Stream::Impl::Impl(ध्रुव unsigned पूर्णाङ्क cudaFlags) : stream(0), ownStream(मिच्छा)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = सच्च;
    allocator = makePtr<राशिः_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(मिच्छा)
{
    allocator = makePtr<राशिः_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    सचे (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

नामाकाश cv { नामाकाश cuda
{
    वग्ग DefaultDeviceInitializer
    {
    सार्वजनिक:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(पूर्णाङ्क deviceId);
        MemoryPool& getMemoryPool(पूर्णाङ्क deviceId);

    गुप्त:
        शून्य initStreams();
        शून्य initPools();

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

        अत्थाय (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<पूर्णाङ्क>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(पूर्णाङ्क deviceId)
    {
        AutoLock lock(streams_mtx_);

        सचे (streams_.empty())
        {
            पूर्णाङ्क device_गणनम् = get_Cuda_Enabled_Device_गणनम्();

            सचे (device_गणनम् > 0)
                streams_.resize(device_गणनम्);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णाङ्क>(streams_.size()) );

        सचे (streams_[deviceId].empty())
        {
            cudaStream_t stream = रिक्त;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(नूतन Stream(impl));
        }

        पटिनिवत्त *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(पूर्णाङ्क deviceId)
    {
        AutoLock lock(pools_mtx_);

        सचे (pools_.empty())
        {
            पूर्णाङ्क device_गणनम् = get_Cuda_Enabled_Device_गणनम्();

            सचे (device_गणनम् > 0)
            {
                pools_.resize(device_गणनम्);
                अत्थाय (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<पूर्णाङ्क>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णाङ्क>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        पटिनिवत्त *p;
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
#अञ्ञथा
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(ध्रुव Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#अञ्ञथा
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ध्रुव size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#अञ्ञथा
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

तार्किक cv::cuda::Stream::queryIfComplete() ध्रुव
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अञ्ञथा
    cudaError_t err = cudaStreamQuery(impl_->stream);

    सचे (err == cudaErrorNotReady || err == cudaSuccess)
        पटिनिवत्त err == cudaSuccess;

    cudaSafeCall(err);
    पटिनिवत्त मिच्छा;
#endif
}

शून्य cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अञ्ञथा
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

शून्य cv::cuda::Stream::waitEvent(ध्रुव Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#अञ्ञथा
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#सचे defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

नामाकाश
{
    संरचना CallbackData
    {
        Stream::StreamCallback callback;
        शून्य* userData;

        CallbackData(Stream::StreamCallback callback_, शून्य* userData_) : callback(callback_), userData(userData_) {}
    };

    शून्य CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, शून्य* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<पूर्णाङ्क>(status), data->userData);
        delete data;
    }
}

#endif

शून्य cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, शून्य* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#अञ्ञथा
    #सचे CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #अञ्ञथा
        CallbackData* data = नूतन CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अञ्ञथा
    ध्रुव पूर्णाङ्क deviceId = getDevice();
    पटिनिवत्त initializer.getNullStream(deviceId);
#endif
}

शून्य* cv::cuda::Stream::cudaPtr() ध्रुव
{
#ifndef HAVE_CUDA
    पटिनिवत्त nullptr;
#अञ्ञथा
    पटिनिवत्त impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ध्रुव
{
#ifndef HAVE_CUDA
    पटिनिवत्त 0;
#अञ्ञथा
    पटिनिवत्त (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ध्रुव Stream& stream)
{
    पटिनिवत्त stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    पटिनिवत्त Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#अञ्ञथा
    पटिनिवत्त cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

नामाकाश
{
    तार्किक enableMemoryPool = मिच्छा;

    वग्ग राशिः_Allocator : सार्वजनिक GpuMat::Allocator
    {
    सार्वजनिक:
        explicit राशिः_Allocator(cudaStream_t stream);
        ~राशिः_Allocator();

        तार्किक allocate(GpuMat* mat, पूर्णाङ्क rows, पूर्णाङ्क cols, size_t elemSize) CV_OVERRIDE;
        शून्य मुक्त(GpuMat* mat) CV_OVERRIDE;

    गुप्त:
        राशिः_Allocator(ध्रुव राशिः_Allocator&);
        राशिः_Allocator& operator =(ध्रुव राशिः_Allocator&);

        cudaStream_t stream_;
        Memory_राशिः* mem_राशिः_2;
        size_t alignment_;
    };

    राशिः_Allocator::राशिः_Allocator(cudaStream_t stream) : stream_(stream), mem_राशिः_2(0)
    {
        सचे (enableMemoryPool)
        {
            ध्रुव पूर्णाङ्क deviceId = getDevice();
            mem_राशिः_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_राशिः();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    राशिः_Allocator::~राशिः_Allocator()
    {
        सचे (mem_राशिः_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_राशिः_2->pool->return_Mem_राशिः(mem_राशिः_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        पटिनिवत्त res;
    }

    तार्किक राशिः_Allocator::allocate(GpuMat* mat, पूर्णाङ्क rows, पूर्णाङ्क cols, size_t elemSize)
    {
        सचे (mem_राशिः_2 == 0)
            पटिनिवत्त मिच्छा;

        size_t pitch, memSize;

        सचे (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        अञ्ञथा
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_राशिः_2->requestMemory(memSize);

        सचे (ptr == 0)
            पटिनिवत्त मिच्छा;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (पूर्णाङ्क*) fastMalloc(परिमाण(पूर्णाङ्क));

        पटिनिवत्त सच्च;
    }

    शून्य राशिः_Allocator::मुक्त(GpuMat* mat)
    {
        सचे (mem_राशिः_2 == 0)
            पटिनिवत्त;

        mem_राशिः_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

शून्य cv::cuda::setBufferPoolUsage(तार्किक on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#अञ्ञथा
    enableMemoryPool = on;
#endif
}

शून्य cv::cuda::setBufferPoolConfig(पूर्णाङ्क deviceId, size_t राशिः_Size, पूर्णाङ्क राशिः_गणनम्)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(राशिः_Size);
    CV_UNUSED(राशिः_गणनम्);
    throw_no_cuda();
#अञ्ञथा
    ध्रुव पूर्णाङ्क currentDevice = getDevice();

    सचे (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(राशिः_Size, राशिः_गणनम्);
    }
    अञ्ञथा
    {
        ध्रुव पूर्णाङ्क device_गणनम् = get_Cuda_Enabled_Device_गणनम्();

        अत्थाय (deviceId = 0; deviceId < device_गणनम्; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(राशिः_Size, राशिः_गणनम्);
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
#अञ्ञथा
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#अञ्ञथा
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    पटिनिवत्त buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

वग्ग cv::cuda::Event::Impl
{
सार्वजनिक:
    Impl(unsigned पूर्णाङ्क)
    {
        throw_no_cuda();
    }
};

#अञ्ञथा

वग्ग cv::cuda::Event::Impl
{
सार्वजनिक:
    cudaEvent_t event;
    तार्किक ownEvent;

    explicit Impl(unsigned पूर्णाङ्क flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned पूर्णाङ्क flags) : event(0), ownEvent(मिच्छा)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = सच्च;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(मिच्छा)
{
}

cv::cuda::Event::Impl::~Impl()
{
    सचे (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ध्रुव Event& event)
{
    पटिनिवत्त event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    पटिनिवत्त Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ध्रुव Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#अञ्ञथा
    impl_ = makePtr<Impl>(flags);
#endif
}

शून्य cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#अञ्ञथा
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

तार्किक cv::cuda::Event::queryIfComplete() ध्रुव
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अञ्ञथा
    cudaError_t err = cudaEventQuery(impl_->event);

    सचे (err == cudaErrorNotReady || err == cudaSuccess)
        पटिनिवत्त err == cudaSuccess;

    cudaSafeCall(err);
    पटिनिवत्त मिच्छा;
#endif
}

शून्य cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अञ्ञथा
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

भिन्न cv::cuda::Event::elapsedTime(ध्रुव Event& start, ध्रुव Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#अञ्ञथा
    भिन्न ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    पटिनिवत्त ms;
#endif
}
