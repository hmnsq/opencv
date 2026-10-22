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

#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ <cstdint>

using નામાકાશ cv;
using નામાકાશ cv::cuda;

#જો defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

નામાકાશ
{
    વર્ગ MemoryPool;

    વર્ગ Memory_ઢગલો
    {
    જાહેર:
        uchar* requestMemory(size_t size);
        શૂન્ય returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        તાર્કિક isFree;
        MemoryPool* pool;

    #જો !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_ઢગલો::requestMemory(size_t size)
    {
        અચલ size_t freeMem = dataend - tip;

        જો (size > freeMem)
            પાછા 0;

        uchar* ptr = tip;

        tip += size;

    #જો !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        પાછા ptr;
    }

    શૂન્ય Memory_ઢગલો::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #જો !defined(NDEBUG)
        અચલ size_t allocSize = tip - ptr;
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

નામાકાશ
{
    વર્ગ MemoryPool
    {
    જાહેર:
        MemoryPool();
        ~MemoryPool() { release(); }

        શૂન્ય initialize(size_t ઢગલો_Size, પૂર્ણાંક ઢગલો_ગણતરી);
        શૂન્ય release();

        Memory_ઢગલો* get_Free_Mem_ઢગલો();
        શૂન્ય return_Mem_ઢગલો(Memory_ઢગલો* mem_ઢગલો);

    ખાનગી:
        શૂન્ય initilizeImpl();

        Mutex mtx_;

        તાર્કિક initialized_;
        size_t ઢગલો_Size_2;
        પૂર્ણાંક ઢગલો_ગણતરી_2;

        uchar* mem_;

        std::vector<Memory_ઢગલો> stacks_;

        MemoryPool(અચલ MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(ખોટું), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ઢગલો_Size_2 = 10 * 1024 * 1024;
        ઢગલો_ગણતરી_2 = 5;
    }

    શૂન્ય MemoryPool::initialize(size_t ઢગલો_Size, પૂર્ણાંક ઢગલો_ગણતરી)
    {
        AutoLock lock(mtx_);

        release();

        ઢગલો_Size_2 = ઢગલો_Size;
        ઢગલો_ગણતરી_2 = ઢગલો_ગણતરી;

        initilizeImpl();
    }

    શૂન્ય MemoryPool::initilizeImpl()
    {
        અચલ size_t totalSize = ઢગલો_Size_2 * ઢગલો_ગણતરી_2;

        જો (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            જો (err != cudaSuccess)
                પાછા;

            stacks_.resize(ઢગલો_ગણતરી_2);

            uchar* ptr = mem_;

            માટે (પૂર્ણાંક i = 0; i < ઢગલો_ગણતરી_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ઢગલો_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = સાચું;
                stacks_[i].pool = આ;

                ptr += ઢગલો_Size_2;
            }

            initialized_ = સાચું;
        }
    }

    શૂન્ય MemoryPool::release()
    {
        જો (mem_)
        {
#જો !defined(NDEBUG)
            માટે (પૂર્ણાંક i = 0; i < ઢગલો_ગણતરી_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = ખોટું;
        }
    }

    Memory_ઢગલો* MemoryPool::get_Free_Mem_ઢગલો()
    {
        AutoLock lock(mtx_);

        જો (!initialized_)
            initilizeImpl();

        જો (!mem_)
            પાછા 0;

        માટે (પૂર્ણાંક i = 0; i < ઢગલો_ગણતરી_2; ++i)
        {
            જો (stacks_[i].isFree)
            {
                stacks_[i].isFree = ખોટું;
                પાછા &stacks_[i];
            }
        }

        પાછા 0;
    }

    શૂન્ય MemoryPool::return_Mem_ઢગલો(Memory_ઢગલો* mem_ઢગલો)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_ઢગલો->isFree );

#જો !defined(NDEBUG)
        તાર્કિક found = ખોટું;
        માટે (પૂર્ણાંક i = 0; i < ઢગલો_ગણતરી_2; ++i)
        {
            જો (mem_ઢગલો == &stacks_[i])
            {
                found = સાચું;
                તોડો;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_ઢગલો->tip == mem_ઢગલો->datastart );

        mem_ઢગલો->isFree = સાચું;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

વર્ગ cv::cuda::Stream::Impl
{
જાહેર:
    Impl(શૂન્ય* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#નહીંતર

નામાકાશ
{
    વર્ગ ઢગલો_Allocator;
}

વર્ગ cv::cuda::Stream::Impl
{
જાહેર:
    cudaStream_t stream;
    તાર્કિક ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(અચલ Ptr<GpuMat::Allocator>& allocator);
    Impl(અચલ unsigned પૂર્ણાંક cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(ખોટું)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = સાચું;

    allocator = makePtr<ઢગલો_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(અચલ Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(ખોટું), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = સાચું;
}

cv::cuda::Stream::Impl::Impl(અચલ unsigned પૂર્ણાંક cudaFlags) : stream(0), ownStream(ખોટું)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = સાચું;
    allocator = makePtr<ઢગલો_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(ખોટું)
{
    allocator = makePtr<ઢગલો_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    જો (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

નામાકાશ cv { નામાકાશ cuda
{
    વર્ગ DefaultDeviceInitializer
    {
    જાહેર:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(પૂર્ણાંક deviceId);
        MemoryPool& getMemoryPool(પૂર્ણાંક deviceId);

    ખાનગી:
        શૂન્ય initStreams();
        શૂન્ય initPools();

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

        માટે (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<પૂર્ણાંક>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(પૂર્ણાંક deviceId)
    {
        AutoLock lock(streams_mtx_);

        જો (streams_.empty())
        {
            પૂર્ણાંક device_ગણતરી = get_Cuda_Enabled_Device_ગણતરી();

            જો (device_ગણતરી > 0)
                streams_.resize(device_ગણતરી);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<પૂર્ણાંક>(streams_.size()) );

        જો (streams_[deviceId].empty())
        {
            cudaStream_t stream = ખાલી;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(નવું Stream(impl));
        }

        પાછા *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(પૂર્ણાંક deviceId)
    {
        AutoLock lock(pools_mtx_);

        જો (pools_.empty())
        {
            પૂર્ણાંક device_ગણતરી = get_Cuda_Enabled_Device_ગણતરી();

            જો (device_ગણતરી > 0)
            {
                pools_.resize(device_ગણતરી);
                માટે (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<પૂર્ણાંક>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<પૂર્ણાંક>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        પાછા *p;
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
#નહીંતર
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(અચલ Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#નહીંતર
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(અચલ size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#નહીંતર
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

તાર્કિક cv::cuda::Stream::queryIfComplete() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    cudaError_t err = cudaStreamQuery(impl_->stream);

    જો (err == cudaErrorNotReady || err == cudaSuccess)
        પાછા err == cudaSuccess;

    cudaSafeCall(err);
    પાછા ખોટું;
#endif
}

શૂન્ય cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

શૂન્ય cv::cuda::Stream::waitEvent(અચલ Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#નહીંતર
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#જો defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

નામાકાશ
{
    સંરચના CallbackData
    {
        Stream::StreamCallback callback;
        શૂન્ય* userData;

        CallbackData(Stream::StreamCallback callback_, શૂન્ય* userData_) : callback(callback_), userData(userData_) {}
    };

    શૂન્ય CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, શૂન્ય* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<પૂર્ણાંક>(status), data->userData);
        delete data;
    }
}

#endif

શૂન્ય cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, શૂન્ય* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#નહીંતર
    #જો CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #નહીંતર
        CallbackData* data = નવું CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    અચલ પૂર્ણાંક deviceId = getDevice();
    પાછા initializer.getNullStream(deviceId);
#endif
}

શૂન્ય* cv::cuda::Stream::cudaPtr() અચલ
{
#ifndef HAVE_CUDA
    પાછા nullptr;
#નહીંતર
    પાછા impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() અચલ
{
#ifndef HAVE_CUDA
    પાછા 0;
#નહીંતર
    પાછા (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(અચલ Stream& stream)
{
    પાછા stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    પાછા Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#નહીંતર
    પાછા cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

નામાકાશ
{
    તાર્કિક enableMemoryPool = ખોટું;

    વર્ગ ઢગલો_Allocator : જાહેર GpuMat::Allocator
    {
    જાહેર:
        explicit ઢગલો_Allocator(cudaStream_t stream);
        ~ઢગલો_Allocator();

        તાર્કિક allocate(GpuMat* mat, પૂર્ણાંક rows, પૂર્ણાંક cols, size_t elemSize) CV_OVERRIDE;
        શૂન્ય મુક્ત(GpuMat* mat) CV_OVERRIDE;

    ખાનગી:
        ઢગલો_Allocator(અચલ ઢગલો_Allocator&);
        ઢગલો_Allocator& operator =(અચલ ઢગલો_Allocator&);

        cudaStream_t stream_;
        Memory_ઢગલો* mem_ઢગલો_2;
        size_t alignment_;
    };

    ઢગલો_Allocator::ઢગલો_Allocator(cudaStream_t stream) : stream_(stream), mem_ઢગલો_2(0)
    {
        જો (enableMemoryPool)
        {
            અચલ પૂર્ણાંક deviceId = getDevice();
            mem_ઢગલો_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_ઢગલો();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ઢગલો_Allocator::~ઢગલો_Allocator()
    {
        જો (mem_ઢગલો_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_ઢગલો_2->pool->return_Mem_ઢગલો(mem_ઢગલો_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        પાછા res;
    }

    તાર્કિક ઢગલો_Allocator::allocate(GpuMat* mat, પૂર્ણાંક rows, પૂર્ણાંક cols, size_t elemSize)
    {
        જો (mem_ઢગલો_2 == 0)
            પાછા ખોટું;

        size_t pitch, memSize;

        જો (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        નહીંતર
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_ઢગલો_2->requestMemory(memSize);

        જો (ptr == 0)
            પાછા ખોટું;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (પૂર્ણાંક*) fastMalloc(કદ(પૂર્ણાંક));

        પાછા સાચું;
    }

    શૂન્ય ઢગલો_Allocator::મુક્ત(GpuMat* mat)
    {
        જો (mem_ઢગલો_2 == 0)
            પાછા;

        mem_ઢગલો_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

શૂન્ય cv::cuda::setBufferPoolUsage(તાર્કિક on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#નહીંતર
    enableMemoryPool = on;
#endif
}

શૂન્ય cv::cuda::setBufferPoolConfig(પૂર્ણાંક deviceId, size_t ઢગલો_Size, પૂર્ણાંક ઢગલો_ગણતરી)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(ઢગલો_Size);
    CV_UNUSED(ઢગલો_ગણતરી);
    throw_no_cuda();
#નહીંતર
    અચલ પૂર્ણાંક currentDevice = getDevice();

    જો (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(ઢગલો_Size, ઢગલો_ગણતરી);
    }
    નહીંતર
    {
        અચલ પૂર્ણાંક device_ગણતરી = get_Cuda_Enabled_Device_ગણતરી();

        માટે (deviceId = 0; deviceId < device_ગણતરી; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(ઢગલો_Size, ઢગલો_ગણતરી);
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
#નહીંતર
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#નહીંતર
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    પાછા buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

વર્ગ cv::cuda::Event::Impl
{
જાહેર:
    Impl(unsigned પૂર્ણાંક)
    {
        throw_no_cuda();
    }
};

#નહીંતર

વર્ગ cv::cuda::Event::Impl
{
જાહેર:
    cudaEvent_t event;
    તાર્કિક ownEvent;

    explicit Impl(unsigned પૂર્ણાંક flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned પૂર્ણાંક flags) : event(0), ownEvent(ખોટું)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = સાચું;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(ખોટું)
{
}

cv::cuda::Event::Impl::~Impl()
{
    જો (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(અચલ Event& event)
{
    પાછા event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    પાછા Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(અચલ Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#નહીંતર
    impl_ = makePtr<Impl>(flags);
#endif
}

શૂન્ય cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#નહીંતર
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

તાર્કિક cv::cuda::Event::queryIfComplete() અચલ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    cudaError_t err = cudaEventQuery(impl_->event);

    જો (err == cudaErrorNotReady || err == cudaSuccess)
        પાછા err == cudaSuccess;

    cudaSafeCall(err);
    પાછા ખોટું;
#endif
}

શૂન્ય cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#નહીંતર
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

દશાંશ cv::cuda::Event::elapsedTime(અચલ Event& start, અચલ Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#નહીંતર
    દશાંશ ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    પાછા ms;
#endif
}
