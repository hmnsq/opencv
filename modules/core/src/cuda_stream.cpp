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

#जर defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

नामाकाश
{
    वर्ग MemoryPool;

    वर्ग Memory_चळत
    {
    सार्वजनिक:
        uchar* requestMemory(माप_प्रकार size);
        शून्य returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        तार्किक isFree;
        MemoryPool* pool;

    #जर !defined(न_संशोधन)
        std::vector<माप_प्रकार> allocations;
    #endif
    };

    uchar* Memory_चळत::requestMemory(माप_प्रकार size)
    {
        स्थिरांक माप_प्रकार freeMem = dataend - tip;

        जर (size > freeMem)
            परत 0;

        uchar* ptr = tip;

        tip += size;

    #जर !defined(न_संशोधन)
        allocations.push_back(size);
    #endif

        परत ptr;
    }

    शून्य Memory_चळत::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #जर !defined(न_संशोधन)
        स्थिरांक माप_प्रकार allocSize = tip - ptr;
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

        शून्य initialize(माप_प्रकार चळत_Size, पूर्णांक चळत_मोजणी);
        शून्य release();

        Memory_चळत* get_Free_Mem_चळत();
        शून्य return_Mem_चळत(Memory_चळत* mem_चळत);

    खाजगी:
        शून्य initilizeImpl();

        Mutex mtx_;

        तार्किक initialized_;
        माप_प्रकार चळत_Size_2;
        पूर्णांक चळत_मोजणी_2;

        uchar* mem_;

        std::vector<Memory_चळत> stacks_;

        MemoryPool(स्थिरांक MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(असत्य), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        चळत_Size_2 = 10 * 1024 * 1024;
        चळत_मोजणी_2 = 5;
    }

    शून्य MemoryPool::initialize(माप_प्रकार चळत_Size, पूर्णांक चळत_मोजणी)
    {
        AutoLock lock(mtx_);

        release();

        चळत_Size_2 = चळत_Size;
        चळत_मोजणी_2 = चळत_मोजणी;

        initilizeImpl();
    }

    शून्य MemoryPool::initilizeImpl()
    {
        स्थिरांक माप_प्रकार totalSize = चळत_Size_2 * चळत_मोजणी_2;

        जर (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            जर (err != cudaSuccess)
                परत;

            stacks_.resize(चळत_मोजणी_2);

            uchar* ptr = mem_;

            साठी (पूर्णांक i = 0; i < चळत_मोजणी_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + चळत_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = सत्य;
                stacks_[i].pool = हे;

                ptr += चळत_Size_2;
            }

            initialized_ = सत्य;
        }
    }

    शून्य MemoryPool::release()
    {
        जर (mem_)
        {
#जर !defined(न_संशोधन)
            साठी (पूर्णांक i = 0; i < चळत_मोजणी_2; ++i)
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

    Memory_चळत* MemoryPool::get_Free_Mem_चळत()
    {
        AutoLock lock(mtx_);

        जर (!initialized_)
            initilizeImpl();

        जर (!mem_)
            परत 0;

        साठी (पूर्णांक i = 0; i < चळत_मोजणी_2; ++i)
        {
            जर (stacks_[i].isFree)
            {
                stacks_[i].isFree = असत्य;
                परत &stacks_[i];
            }
        }

        परत 0;
    }

    शून्य MemoryPool::return_Mem_चळत(Memory_चळत* mem_चळत)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_चळत->isFree );

#जर !defined(न_संशोधन)
        तार्किक found = असत्य;
        साठी (पूर्णांक i = 0; i < चळत_मोजणी_2; ++i)
        {
            जर (mem_चळत == &stacks_[i])
            {
                found = सत्य;
                थांबा;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_चळत->tip == mem_चळत->datastart );

        mem_चळत->isFree = सत्य;
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
    वर्ग चळत_Allocator;
}

वर्ग cv::cuda::Stream::Impl
{
सार्वजनिक:
    cudaStream_t stream;
    तार्किक ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(स्थिरांक Ptr<GpuMat::Allocator>& allocator);
    Impl(स्थिरांक अचिन्हित पूर्णांक cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(असत्य)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = सत्य;

    allocator = makePtr<चळत_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(स्थिरांक Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(असत्य), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = सत्य;
}

cv::cuda::Stream::Impl::Impl(स्थिरांक अचिन्हित पूर्णांक cudaFlags) : stream(0), ownStream(असत्य)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = सत्य;
    allocator = makePtr<चळत_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(असत्य)
{
    allocator = makePtr<चळत_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    जर (stream && ownStream)
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

        Stream& getNullStream(पूर्णांक deviceId);
        MemoryPool& getMemoryPool(पूर्णांक deviceId);

    खाजगी:
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

        साठी (माप_प्रकार i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<पूर्णांक>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(पूर्णांक deviceId)
    {
        AutoLock lock(streams_mtx_);

        जर (streams_.empty())
        {
            पूर्णांक device_मोजणी = get_Cuda_Enabled_Device_मोजणी();

            जर (device_मोजणी > 0)
                streams_.resize(device_मोजणी);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णांक>(streams_.size()) );

        जर (streams_[deviceId].empty())
        {
            cudaStream_t stream = रिक्त;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(नवीन Stream(impl));
        }

        परत *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(पूर्णांक deviceId)
    {
        AutoLock lock(pools_mtx_);

        जर (pools_.empty())
        {
            पूर्णांक device_मोजणी = get_Cuda_Enabled_Device_मोजणी();

            जर (device_मोजणी > 0)
            {
                pools_.resize(device_मोजणी);
                साठी (माप_प्रकार i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<पूर्णांक>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णांक>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        परत *p;
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

cv::cuda::Stream::Stream(स्थिरांक Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(स्थिरांक माप_प्रकार cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(cudaFlags & अच_पूर्णांक_उच्च);
#endif
}

तार्किक cv::cuda::Stream::queryIfComplete() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaStreamQuery(impl_->stream);

    जर (err == cudaErrorNotReady || err == cudaSuccess)
        परत err == cudaSuccess;

    cudaSafeCall(err);
    परत असत्य;
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

शून्य cv::cuda::Stream::waitEvent(स्थिरांक Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#जर defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

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
        data->callback(static_cast<पूर्णांक>(status), data->userData);
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
    #जर CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #अन्यथा
        CallbackData* data = नवीन CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    स्थिरांक पूर्णांक deviceId = getDevice();
    परत initializer.getNullStream(deviceId);
#endif
}

शून्य* cv::cuda::Stream::cudaPtr() स्थिरांक
{
#ifndef HAVE_CUDA
    परत nullptr;
#अन्यथा
    परत impl_->stream;
#endif
}

cv::cuda::Stream::चालक bool_type() स्थिरांक
{
#ifndef HAVE_CUDA
    परत 0;
#अन्यथा
    परत (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(स्थिरांक Stream& stream)
{
    परत stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    परत Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(माप_प्रकार cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#अन्यथा
    परत cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

नामाकाश
{
    तार्किक enableMemoryPool = असत्य;

    वर्ग चळत_Allocator : सार्वजनिक GpuMat::Allocator
    {
    सार्वजनिक:
        explicit चळत_Allocator(cudaStream_t stream);
        ~चळत_Allocator();

        तार्किक allocate(GpuMat* mat, पूर्णांक rows, पूर्णांक cols, माप_प्रकार elemSize) CV_OVERRIDE;
        शून्य मुक्त(GpuMat* mat) CV_OVERRIDE;

    खाजगी:
        चळत_Allocator(स्थिरांक चळत_Allocator&);
        चळत_Allocator& चालक =(स्थिरांक चळत_Allocator&);

        cudaStream_t stream_;
        Memory_चळत* mem_चळत_2;
        माप_प्रकार alignment_;
    };

    चळत_Allocator::चळत_Allocator(cudaStream_t stream) : stream_(stream), mem_चळत_2(0)
    {
        जर (enableMemoryPool)
        {
            स्थिरांक पूर्णांक deviceId = getDevice();
            mem_चळत_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_चळत();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    चळत_Allocator::~चळत_Allocator()
    {
        जर (mem_चळत_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_चळत_2->pool->return_Mem_चळत(mem_चळत_2);
        }
    }

    माप_प्रकार alignUp(माप_प्रकार what, माप_प्रकार alignment)
    {
        माप_प्रकार alignMask = alignment-1;
        माप_प्रकार inverseAlignMask = ~alignMask;
        माप_प्रकार res = (what + alignMask) & inverseAlignMask;
        परत res;
    }

    तार्किक चळत_Allocator::allocate(GpuMat* mat, पूर्णांक rows, पूर्णांक cols, माप_प्रकार elemSize)
    {
        जर (mem_चळत_2 == 0)
            परत असत्य;

        माप_प्रकार pitch, memSize;

        जर (rows > 1 && cols > 1)
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

        uchar* ptr = mem_चळत_2->requestMemory(memSize);

        जर (ptr == 0)
            परत असत्य;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (पूर्णांक*) fastMalloc(आकार(पूर्णांक));

        परत सत्य;
    }

    शून्य चळत_Allocator::मुक्त(GpuMat* mat)
    {
        जर (mem_चळत_2 == 0)
            परत;

        mem_चळत_2->returnMemory(mat->datastart);
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

शून्य cv::cuda::setBufferPoolConfig(पूर्णांक deviceId, माप_प्रकार चळत_Size, पूर्णांक चळत_मोजणी)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(चळत_Size);
    CV_UNUSED(चळत_मोजणी);
    throw_no_cuda();
#अन्यथा
    स्थिरांक पूर्णांक currentDevice = getDevice();

    जर (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(चळत_Size, चळत_मोजणी);
    }
    अन्यथा
    {
        स्थिरांक पूर्णांक device_मोजणी = get_Cuda_Enabled_Device_मोजणी();

        साठी (deviceId = 0; deviceId < device_मोजणी; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(चळत_Size, चळत_मोजणी);
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

GpuMat cv::cuda::BufferPool::getBuffer(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#अन्यथा
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    परत buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

वर्ग cv::cuda::Event::Impl
{
सार्वजनिक:
    Impl(अचिन्हित पूर्णांक)
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

    explicit Impl(अचिन्हित पूर्णांक flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(अचिन्हित पूर्णांक flags) : event(0), ownEvent(असत्य)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = सत्य;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(असत्य)
{
}

cv::cuda::Event::Impl::~Impl()
{
    जर (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(स्थिरांक Event& event)
{
    परत event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    परत Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(स्थिरांक Event::CreateFlags flags)
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

तार्किक cv::cuda::Event::queryIfComplete() स्थिरांक
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaEventQuery(impl_->event);

    जर (err == cudaErrorNotReady || err == cudaSuccess)
        परत err == cudaSuccess;

    cudaSafeCall(err);
    परत असत्य;
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

दशांश cv::cuda::Event::elapsedTime(स्थिरांक Event& start, स्थिरांक Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#अन्यथा
    दशांश ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    परत ms;
#endif
}
