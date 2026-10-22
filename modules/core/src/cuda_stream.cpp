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

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ <cstdint>

using நாமாகாஶ cv;
using நாமாகாஶ cv::cuda;

#எனில் defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

நாமாகாஶ
{
    வகுப்பு MemoryPool;

    வகுப்பு Memory_அடுக்கு
    {
    பொது:
        uchar* requestMemory(size_t size);
        வெறுமை returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        தருக்க isFree;
        MemoryPool* pool;

    #எனில் !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_அடுக்கு::requestMemory(size_t size)
    {
        மாறிலி size_t freeMem = dataend - tip;

        எனில் (size > freeMem)
            திரும்பு 0;

        uchar* ptr = tip;

        tip += size;

    #எனில் !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        திரும்பு ptr;
    }

    வெறுமை Memory_அடுக்கு::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #எனில் !defined(NDEBUG)
        மாறிலி size_t allocSize = tip - ptr;
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

நாமாகாஶ
{
    வகுப்பு MemoryPool
    {
    பொது:
        MemoryPool();
        ~MemoryPool() { release(); }

        வெறுமை initialize(size_t அடுக்கு_Size, முழு அடுக்கு_மொத்தம்);
        வெறுமை release();

        Memory_அடுக்கு* get_Free_Mem_அடுக்கு();
        வெறுமை return_Mem_அடுக்கு(Memory_அடுக்கு* mem_அடுக்கு);

    தனிப்பட்ட:
        வெறுமை initilizeImpl();

        Mutex mtx_;

        தருக்க initialized_;
        size_t அடுக்கு_Size_2;
        முழு அடுக்கு_மொத்தம்_2;

        uchar* mem_;

        std::vector<Memory_அடுக்கு> stacks_;

        MemoryPool(மாறிலி MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(பொய்), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        அடுக்கு_Size_2 = 10 * 1024 * 1024;
        அடுக்கு_மொத்தம்_2 = 5;
    }

    வெறுமை MemoryPool::initialize(size_t அடுக்கு_Size, முழு அடுக்கு_மொத்தம்)
    {
        AutoLock lock(mtx_);

        release();

        அடுக்கு_Size_2 = அடுக்கு_Size;
        அடுக்கு_மொத்தம்_2 = அடுக்கு_மொத்தம்;

        initilizeImpl();
    }

    வெறுமை MemoryPool::initilizeImpl()
    {
        மாறிலி size_t totalSize = அடுக்கு_Size_2 * அடுக்கு_மொத்தம்_2;

        எனில் (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            எனில் (err != cudaSuccess)
                திரும்பு;

            stacks_.resize(அடுக்கு_மொத்தம்_2);

            uchar* ptr = mem_;

            க்கு (முழு i = 0; i < அடுக்கு_மொத்தம்_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + அடுக்கு_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = உண்மை;
                stacks_[i].pool = இது;

                ptr += அடுக்கு_Size_2;
            }

            initialized_ = உண்மை;
        }
    }

    வெறுமை MemoryPool::release()
    {
        எனில் (mem_)
        {
#எனில் !defined(NDEBUG)
            க்கு (முழு i = 0; i < அடுக்கு_மொத்தம்_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = பொய்;
        }
    }

    Memory_அடுக்கு* MemoryPool::get_Free_Mem_அடுக்கு()
    {
        AutoLock lock(mtx_);

        எனில் (!initialized_)
            initilizeImpl();

        எனில் (!mem_)
            திரும்பு 0;

        க்கு (முழு i = 0; i < அடுக்கு_மொத்தம்_2; ++i)
        {
            எனில் (stacks_[i].isFree)
            {
                stacks_[i].isFree = பொய்;
                திரும்பு &stacks_[i];
            }
        }

        திரும்பு 0;
    }

    வெறுமை MemoryPool::return_Mem_அடுக்கு(Memory_அடுக்கு* mem_அடுக்கு)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_அடுக்கு->isFree );

#எனில் !defined(NDEBUG)
        தருக்க found = பொய்;
        க்கு (முழு i = 0; i < அடுக்கு_மொத்தம்_2; ++i)
        {
            எனில் (mem_அடுக்கு == &stacks_[i])
            {
                found = உண்மை;
                நிறுத்து;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_அடுக்கு->tip == mem_அடுக்கு->datastart );

        mem_அடுக்கு->isFree = உண்மை;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

வகுப்பு cv::cuda::Stream::Impl
{
பொது:
    Impl(வெறுமை* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#இல்லையெனில்

நாமாகாஶ
{
    வகுப்பு அடுக்கு_Allocator;
}

வகுப்பு cv::cuda::Stream::Impl
{
பொது:
    cudaStream_t stream;
    தருக்க ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(மாறிலி Ptr<GpuMat::Allocator>& allocator);
    Impl(மாறிலி unsigned முழு cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(பொய்)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = உண்மை;

    allocator = makePtr<அடுக்கு_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(மாறிலி Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(பொய்), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = உண்மை;
}

cv::cuda::Stream::Impl::Impl(மாறிலி unsigned முழு cudaFlags) : stream(0), ownStream(பொய்)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = உண்மை;
    allocator = makePtr<அடுக்கு_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(பொய்)
{
    allocator = makePtr<அடுக்கு_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    எனில் (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

நாமாகாஶ cv { நாமாகாஶ cuda
{
    வகுப்பு DefaultDeviceInitializer
    {
    பொது:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(முழு deviceId);
        MemoryPool& getMemoryPool(முழு deviceId);

    தனிப்பட்ட:
        வெறுமை initStreams();
        வெறுமை initPools();

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

        க்கு (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<முழு>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(முழு deviceId)
    {
        AutoLock lock(streams_mtx_);

        எனில் (streams_.empty())
        {
            முழு device_மொத்தம் = get_Cuda_Enabled_Device_மொத்தம்();

            எனில் (device_மொத்தம் > 0)
                streams_.resize(device_மொத்தம்);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<முழு>(streams_.size()) );

        எனில் (streams_[deviceId].empty())
        {
            cudaStream_t stream = வெற்று;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(புதிய Stream(impl));
        }

        திரும்பு *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(முழு deviceId)
    {
        AutoLock lock(pools_mtx_);

        எனில் (pools_.empty())
        {
            முழு device_மொத்தம் = get_Cuda_Enabled_Device_மொத்தம்();

            எனில் (device_மொத்தம் > 0)
            {
                pools_.resize(device_மொத்தம்);
                க்கு (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<முழு>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<முழு>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        திரும்பு *p;
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
#இல்லையெனில்
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(மாறிலி Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#இல்லையெனில்
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(மாறிலி size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#இல்லையெனில்
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

தருக்க cv::cuda::Stream::queryIfComplete() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    cudaError_t err = cudaStreamQuery(impl_->stream);

    எனில் (err == cudaErrorNotReady || err == cudaSuccess)
        திரும்பு err == cudaSuccess;

    cudaSafeCall(err);
    திரும்பு பொய்;
#endif
}

வெறுமை cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

வெறுமை cv::cuda::Stream::waitEvent(மாறிலி Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#இல்லையெனில்
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#எனில் defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

நாமாகாஶ
{
    கட்டமைப்பு CallbackData
    {
        Stream::StreamCallback callback;
        வெறுமை* userData;

        CallbackData(Stream::StreamCallback callback_, வெறுமை* userData_) : callback(callback_), userData(userData_) {}
    };

    வெறுமை CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, வெறுமை* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<முழு>(status), data->userData);
        delete data;
    }
}

#endif

வெறுமை cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, வெறுமை* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#இல்லையெனில்
    #எனில் CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #இல்லையெனில்
        CallbackData* data = புதிய CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    மாறிலி முழு deviceId = getDevice();
    திரும்பு initializer.getNullStream(deviceId);
#endif
}

வெறுமை* cv::cuda::Stream::cudaPtr() மாறிலி
{
#ifndef HAVE_CUDA
    திரும்பு nullptr;
#இல்லையெனில்
    திரும்பு impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() மாறிலி
{
#ifndef HAVE_CUDA
    திரும்பு 0;
#இல்லையெனில்
    திரும்பு (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(மாறிலி Stream& stream)
{
    திரும்பு stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    திரும்பு Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#இல்லையெனில்
    திரும்பு cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

நாமாகாஶ
{
    தருக்க enableMemoryPool = பொய்;

    வகுப்பு அடுக்கு_Allocator : பொது GpuMat::Allocator
    {
    பொது:
        explicit அடுக்கு_Allocator(cudaStream_t stream);
        ~அடுக்கு_Allocator();

        தருக்க allocate(GpuMat* mat, முழு rows, முழு cols, size_t elemSize) CV_OVERRIDE;
        வெறுமை விடுவி(GpuMat* mat) CV_OVERRIDE;

    தனிப்பட்ட:
        அடுக்கு_Allocator(மாறிலி அடுக்கு_Allocator&);
        அடுக்கு_Allocator& operator =(மாறிலி அடுக்கு_Allocator&);

        cudaStream_t stream_;
        Memory_அடுக்கு* mem_அடுக்கு_2;
        size_t alignment_;
    };

    அடுக்கு_Allocator::அடுக்கு_Allocator(cudaStream_t stream) : stream_(stream), mem_அடுக்கு_2(0)
    {
        எனில் (enableMemoryPool)
        {
            மாறிலி முழு deviceId = getDevice();
            mem_அடுக்கு_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_அடுக்கு();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    அடுக்கு_Allocator::~அடுக்கு_Allocator()
    {
        எனில் (mem_அடுக்கு_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_அடுக்கு_2->pool->return_Mem_அடுக்கு(mem_அடுக்கு_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        திரும்பு res;
    }

    தருக்க அடுக்கு_Allocator::allocate(GpuMat* mat, முழு rows, முழு cols, size_t elemSize)
    {
        எனில் (mem_அடுக்கு_2 == 0)
            திரும்பு பொய்;

        size_t pitch, memSize;

        எனில் (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        இல்லையெனில்
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_அடுக்கு_2->requestMemory(memSize);

        எனில் (ptr == 0)
            திரும்பு பொய்;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (முழு*) fastMalloc(அளவு(முழு));

        திரும்பு உண்மை;
    }

    வெறுமை அடுக்கு_Allocator::விடுவி(GpuMat* mat)
    {
        எனில் (mem_அடுக்கு_2 == 0)
            திரும்பு;

        mem_அடுக்கு_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

வெறுமை cv::cuda::setBufferPoolUsage(தருக்க on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#இல்லையெனில்
    enableMemoryPool = on;
#endif
}

வெறுமை cv::cuda::setBufferPoolConfig(முழு deviceId, size_t அடுக்கு_Size, முழு அடுக்கு_மொத்தம்)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(அடுக்கு_Size);
    CV_UNUSED(அடுக்கு_மொத்தம்);
    throw_no_cuda();
#இல்லையெனில்
    மாறிலி முழு currentDevice = getDevice();

    எனில் (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(அடுக்கு_Size, அடுக்கு_மொத்தம்);
    }
    இல்லையெனில்
    {
        மாறிலி முழு device_மொத்தம் = get_Cuda_Enabled_Device_மொத்தம்();

        க்கு (deviceId = 0; deviceId < device_மொத்தம்; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(அடுக்கு_Size, அடுக்கு_மொத்தம்);
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
#இல்லையெனில்
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(முழு rows, முழு cols, முழு type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#இல்லையெனில்
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    திரும்பு buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

வகுப்பு cv::cuda::Event::Impl
{
பொது:
    Impl(unsigned முழு)
    {
        throw_no_cuda();
    }
};

#இல்லையெனில்

வகுப்பு cv::cuda::Event::Impl
{
பொது:
    cudaEvent_t event;
    தருக்க ownEvent;

    explicit Impl(unsigned முழு flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned முழு flags) : event(0), ownEvent(பொய்)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = உண்மை;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(பொய்)
{
}

cv::cuda::Event::Impl::~Impl()
{
    எனில் (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(மாறிலி Event& event)
{
    திரும்பு event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    திரும்பு Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(மாறிலி Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#இல்லையெனில்
    impl_ = makePtr<Impl>(flags);
#endif
}

வெறுமை cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#இல்லையெனில்
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

தருக்க cv::cuda::Event::queryIfComplete() மாறிலி
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    cudaError_t err = cudaEventQuery(impl_->event);

    எனில் (err == cudaErrorNotReady || err == cudaSuccess)
        திரும்பு err == cudaSuccess;

    cudaSafeCall(err);
    திரும்பு பொய்;
#endif
}

வெறுமை cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#இல்லையெனில்
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

தசம cv::cuda::Event::elapsedTime(மாறிலி Event& start, மாறிலி Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#இல்லையெனில்
    தசம ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    திரும்பு ms;
#endif
}
