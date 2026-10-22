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

#यदि defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

नामाकाश
{
    वर्ग MemoryPool;

    वर्ग Memory_थाक
    {
    सार्वजनिक:
        uchar* requestMemory(माप_प्रकार size);
        शून्य returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        तार्किक isFree;
        MemoryPool* pool;

    #यदि !defined(न_संशोधन)
        std::vector<माप_प्रकार> allocations;
    #endif
    };

    uchar* Memory_थाक::requestMemory(माप_प्रकार size)
    {
        अचर माप_प्रकार freeMem = dataend - tip;

        यदि (size > freeMem)
            फर्काऊ 0;

        uchar* ptr = tip;

        tip += size;

    #यदि !defined(न_संशोधन)
        allocations.push_back(size);
    #endif

        फर्काऊ ptr;
    }

    शून्य Memory_थाक::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #यदि !defined(न_संशोधन)
        अचर माप_प्रकार allocSize = tip - ptr;
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
    वर्ग MemoryPool
    {
    सार्वजनिक:
        MemoryPool();
        ~MemoryPool() { release(); }

        शून्य initialize(माप_प्रकार थाक_Size, पूर्णाङ्क थाक_गन्ती);
        शून्य release();

        Memory_थाक* get_Free_Mem_थाक();
        शून्य return_Mem_थाक(Memory_थाक* mem_थाक);

    गुप्त:
        शून्य initilizeImpl();

        Mutex mtx_;

        तार्किक initialized_;
        माप_प्रकार थाक_Size_2;
        पूर्णाङ्क थाक_गन्ती_2;

        uchar* mem_;

        std::vector<Memory_थाक> stacks_;

        MemoryPool(अचर MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(असत्य), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        थाक_Size_2 = 10 * 1024 * 1024;
        थाक_गन्ती_2 = 5;
    }

    शून्य MemoryPool::initialize(माप_प्रकार थाक_Size, पूर्णाङ्क थाक_गन्ती)
    {
        AutoLock lock(mtx_);

        release();

        थाक_Size_2 = थाक_Size;
        थाक_गन्ती_2 = थाक_गन्ती;

        initilizeImpl();
    }

    शून्य MemoryPool::initilizeImpl()
    {
        अचर माप_प्रकार totalSize = थाक_Size_2 * थाक_गन्ती_2;

        यदि (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            यदि (err != cudaSuccess)
                फर्काऊ;

            stacks_.resize(थाक_गन्ती_2);

            uchar* ptr = mem_;

            लागि (पूर्णाङ्क i = 0; i < थाक_गन्ती_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + थाक_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = सत्य;
                stacks_[i].pool = एतत्;

                ptr += थाक_Size_2;
            }

            initialized_ = सत्य;
        }
    }

    शून्य MemoryPool::release()
    {
        यदि (mem_)
        {
#यदि !defined(न_संशोधन)
            लागि (पूर्णाङ्क i = 0; i < थाक_गन्ती_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = असत्य;
        }
    }

    Memory_थाक* MemoryPool::get_Free_Mem_थाक()
    {
        AutoLock lock(mtx_);

        यदि (!initialized_)
            initilizeImpl();

        यदि (!mem_)
            फर्काऊ 0;

        लागि (पूर्णाङ्क i = 0; i < थाक_गन्ती_2; ++i)
        {
            यदि (stacks_[i].isFree)
            {
                stacks_[i].isFree = असत्य;
                फर्काऊ &stacks_[i];
            }
        }

        फर्काऊ 0;
    }

    शून्य MemoryPool::return_Mem_थाक(Memory_थाक* mem_थाक)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_थाक->isFree );

#यदि !defined(न_संशोधन)
        तार्किक found = असत्य;
        लागि (पूर्णाङ्क i = 0; i < थाक_गन्ती_2; ++i)
        {
            यदि (mem_थाक == &stacks_[i])
            {
                found = सत्य;
                रोक;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_थाक->tip == mem_थाक->datastart );

        mem_थाक->isFree = सत्य;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

वर्ग cv::cuda::Stream::Impl
{
सार्वजनिक:
    Impl(शून्य* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#अन्यथा

नामाकाश
{
    वर्ग थाक_Allocator;
}

वर्ग cv::cuda::Stream::Impl
{
सार्वजनिक:
    cudaStream_t stream;
    तार्किक ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(अचर Ptr<GpuMat::Allocator>& allocator);
    Impl(अचर अचिन्हित पूर्णाङ्क cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(असत्य)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = सत्य;

    allocator = makePtr<थाक_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(अचर Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(असत्य), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = सत्य;
}

cv::cuda::Stream::Impl::Impl(अचर अचिन्हित पूर्णाङ्क cudaFlags) : stream(0), ownStream(असत्य)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = सत्य;
    allocator = makePtr<थाक_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(असत्य)
{
    allocator = makePtr<थाक_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    यदि (stream && ownStream)
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
    वर्ग DefaultDeviceInitializer
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

        लागि (माप_प्रकार i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<पूर्णाङ्क>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(पूर्णाङ्क deviceId)
    {
        AutoLock lock(streams_mtx_);

        यदि (streams_.empty())
        {
            पूर्णाङ्क device_गन्ती = get_Cuda_Enabled_Device_गन्ती();

            यदि (device_गन्ती > 0)
                streams_.resize(device_गन्ती);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णाङ्क>(streams_.size()) );

        यदि (streams_[deviceId].empty())
        {
            cudaStream_t stream = रिक्त;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(नूतन Stream(impl));
        }

        फर्काऊ *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(पूर्णाङ्क deviceId)
    {
        AutoLock lock(pools_mtx_);

        यदि (pools_.empty())
        {
            पूर्णाङ्क device_गन्ती = get_Cuda_Enabled_Device_गन्ती();

            यदि (device_गन्ती > 0)
            {
                pools_.resize(device_गन्ती);
                लागि (माप_प्रकार i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<पूर्णाङ्क>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णाङ्क>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        फर्काऊ *p;
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
#अन्यथा
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(अचर Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(अचर माप_प्रकार cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(cudaFlags & अच_पूर्णांक_उच्च);
#endif
}

तार्किक cv::cuda::Stream::queryIfComplete() अचर
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaStreamQuery(impl_->stream);

    यदि (err == cudaErrorNotReady || err == cudaSuccess)
        फर्काऊ err == cudaSuccess;

    cudaSafeCall(err);
    फर्काऊ असत्य;
#endif
}

शून्य cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

शून्य cv::cuda::Stream::waitEvent(अचर Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#यदि defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

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
#अन्यथा
    #यदि CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #अन्यथा
        CallbackData* data = नूतन CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    अचर पूर्णाङ्क deviceId = getDevice();
    फर्काऊ initializer.getNullStream(deviceId);
#endif
}

शून्य* cv::cuda::Stream::cudaPtr() अचर
{
#ifndef HAVE_CUDA
    फर्काऊ nullptr;
#अन्यथा
    फर्काऊ impl_->stream;
#endif
}

cv::cuda::Stream::चालक bool_type() अचर
{
#ifndef HAVE_CUDA
    फर्काऊ 0;
#अन्यथा
    फर्काऊ (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(अचर Stream& stream)
{
    फर्काऊ stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    फर्काऊ Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(माप_प्रकार cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#अन्यथा
    फर्काऊ cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

नामाकाश
{
    तार्किक enableMemoryPool = असत्य;

    वर्ग थाक_Allocator : सार्वजनिक GpuMat::Allocator
    {
    सार्वजनिक:
        explicit थाक_Allocator(cudaStream_t stream);
        ~थाक_Allocator();

        तार्किक allocate(GpuMat* mat, पूर्णाङ्क rows, पूर्णाङ्क cols, माप_प्रकार elemSize) CV_OVERRIDE;
        शून्य मुक्त(GpuMat* mat) CV_OVERRIDE;

    गुप्त:
        थाक_Allocator(अचर थाक_Allocator&);
        थाक_Allocator& चालक =(अचर थाक_Allocator&);

        cudaStream_t stream_;
        Memory_थाक* mem_थाक_2;
        माप_प्रकार alignment_;
    };

    थाक_Allocator::थाक_Allocator(cudaStream_t stream) : stream_(stream), mem_थाक_2(0)
    {
        यदि (enableMemoryPool)
        {
            अचर पूर्णाङ्क deviceId = getDevice();
            mem_थाक_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_थाक();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    थाक_Allocator::~थाक_Allocator()
    {
        यदि (mem_थाक_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_थाक_2->pool->return_Mem_थाक(mem_थाक_2);
        }
    }

    माप_प्रकार alignUp(माप_प्रकार what, माप_प्रकार alignment)
    {
        माप_प्रकार alignMask = alignment-1;
        माप_प्रकार inverseAlignMask = ~alignMask;
        माप_प्रकार res = (what + alignMask) & inverseAlignMask;
        फर्काऊ res;
    }

    तार्किक थाक_Allocator::allocate(GpuMat* mat, पूर्णाङ्क rows, पूर्णाङ्क cols, माप_प्रकार elemSize)
    {
        यदि (mem_थाक_2 == 0)
            फर्काऊ असत्य;

        माप_प्रकार pitch, memSize;

        यदि (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        अन्यथा
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_थाक_2->requestMemory(memSize);

        यदि (ptr == 0)
            फर्काऊ असत्य;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (पूर्णाङ्क*) fastMalloc(परिमाण(पूर्णाङ्क));

        फर्काऊ सत्य;
    }

    शून्य थाक_Allocator::मुक्त(GpuMat* mat)
    {
        यदि (mem_थाक_2 == 0)
            फर्काऊ;

        mem_थाक_2->returnMemory(mat->datastart);
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
#अन्यथा
    enableMemoryPool = on;
#endif
}

शून्य cv::cuda::setBufferPoolConfig(पूर्णाङ्क deviceId, माप_प्रकार थाक_Size, पूर्णाङ्क थाक_गन्ती)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(थाक_Size);
    CV_UNUSED(थाक_गन्ती);
    throw_no_cuda();
#अन्यथा
    अचर पूर्णाङ्क currentDevice = getDevice();

    यदि (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(थाक_Size, थाक_गन्ती);
    }
    अन्यथा
    {
        अचर पूर्णाङ्क device_गन्ती = get_Cuda_Enabled_Device_गन्ती();

        लागि (deviceId = 0; deviceId < device_गन्ती; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(थाक_Size, थाक_गन्ती);
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
#अन्यथा
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
#अन्यथा
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    फर्काऊ buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

वर्ग cv::cuda::Event::Impl
{
सार्वजनिक:
    Impl(अचिन्हित पूर्णाङ्क)
    {
        throw_no_cuda();
    }
};

#अन्यथा

वर्ग cv::cuda::Event::Impl
{
सार्वजनिक:
    cudaEvent_t event;
    तार्किक ownEvent;

    explicit Impl(अचिन्हित पूर्णाङ्क flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(अचिन्हित पूर्णाङ्क flags) : event(0), ownEvent(असत्य)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = सत्य;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(असत्य)
{
}

cv::cuda::Event::Impl::~Impl()
{
    यदि (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(अचर Event& event)
{
    फर्काऊ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    फर्काऊ Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(अचर Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(flags);
#endif
}

शून्य cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

तार्किक cv::cuda::Event::queryIfComplete() अचर
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaEventQuery(impl_->event);

    यदि (err == cudaErrorNotReady || err == cudaSuccess)
        फर्काऊ err == cudaSuccess;

    cudaSafeCall(err);
    फर्काऊ असत्य;
#endif
}

शून्य cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

भिन्न cv::cuda::Event::elapsedTime(अचर Event& start, अचर Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#अन्यथा
    भिन्न ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    फर्काऊ ms;
#endif
}
