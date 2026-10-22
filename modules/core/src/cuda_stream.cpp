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

#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ <cstdint>

using നാമാകാശ cv;
using നാമാകാശ cv::cuda;

#എങ്കിൽ defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

നാമാകാശ
{
    വർഗ്ഗം MemoryPool;

    വർഗ്ഗം Memory_അട്ടി
    {
    പൊതു:
        uchar* requestMemory(size_t size);
        ശൂന്യം returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        തർക്കം isFree;
        MemoryPool* pool;

    #എങ്കിൽ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_അട്ടി::requestMemory(size_t size)
    {
        സ്ഥിരം size_t freeMem = dataend - tip;

        എങ്കിൽ (size > freeMem)
            തിരികെ 0;

        uchar* ptr = tip;

        tip += size;

    #എങ്കിൽ !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        തിരികെ ptr;
    }

    ശൂന്യം Memory_അട്ടി::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #എങ്കിൽ !defined(NDEBUG)
        സ്ഥിരം size_t allocSize = tip - ptr;
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

നാമാകാശ
{
    വർഗ്ഗം MemoryPool
    {
    പൊതു:
        MemoryPool();
        ~MemoryPool() { release(); }

        ശൂന്യം initialize(size_t അട്ടി_Size, പൂർണ്ണസംഖ്യ അട്ടി_എണ്ണം);
        ശൂന്യം release();

        Memory_അട്ടി* get_Free_Mem_അട്ടി();
        ശൂന്യം return_Mem_അട്ടി(Memory_അട്ടി* mem_അട്ടി);

    സ്വകാര്യ:
        ശൂന്യം initilizeImpl();

        Mutex mtx_;

        തർക്കം initialized_;
        size_t അട്ടി_Size_2;
        പൂർണ്ണസംഖ്യ അട്ടി_എണ്ണം_2;

        uchar* mem_;

        std::vector<Memory_അട്ടി> stacks_;

        MemoryPool(സ്ഥിരം MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(അസത്യം), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        അട്ടി_Size_2 = 10 * 1024 * 1024;
        അട്ടി_എണ്ണം_2 = 5;
    }

    ശൂന്യം MemoryPool::initialize(size_t അട്ടി_Size, പൂർണ്ണസംഖ്യ അട്ടി_എണ്ണം)
    {
        AutoLock lock(mtx_);

        release();

        അട്ടി_Size_2 = അട്ടി_Size;
        അട്ടി_എണ്ണം_2 = അട്ടി_എണ്ണം;

        initilizeImpl();
    }

    ശൂന്യം MemoryPool::initilizeImpl()
    {
        സ്ഥിരം size_t totalSize = അട്ടി_Size_2 * അട്ടി_എണ്ണം_2;

        എങ്കിൽ (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            എങ്കിൽ (err != cudaSuccess)
                തിരികെ;

            stacks_.resize(അട്ടി_എണ്ണം_2);

            uchar* ptr = mem_;

            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അട്ടി_എണ്ണം_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + അട്ടി_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = സത്യം;
                stacks_[i].pool = ഇത്;

                ptr += അട്ടി_Size_2;
            }

            initialized_ = സത്യം;
        }
    }

    ശൂന്യം MemoryPool::release()
    {
        എങ്കിൽ (mem_)
        {
#എങ്കിൽ !defined(NDEBUG)
            വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അട്ടി_എണ്ണം_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = അസത്യം;
        }
    }

    Memory_അട്ടി* MemoryPool::get_Free_Mem_അട്ടി()
    {
        AutoLock lock(mtx_);

        എങ്കിൽ (!initialized_)
            initilizeImpl();

        എങ്കിൽ (!mem_)
            തിരികെ 0;

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അട്ടി_എണ്ണം_2; ++i)
        {
            എങ്കിൽ (stacks_[i].isFree)
            {
                stacks_[i].isFree = അസത്യം;
                തിരികെ &stacks_[i];
            }
        }

        തിരികെ 0;
    }

    ശൂന്യം MemoryPool::return_Mem_അട്ടി(Memory_അട്ടി* mem_അട്ടി)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_അട്ടി->isFree );

#എങ്കിൽ !defined(NDEBUG)
        തർക്കം found = അസത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അട്ടി_എണ്ണം_2; ++i)
        {
            എങ്കിൽ (mem_അട്ടി == &stacks_[i])
            {
                found = സത്യം;
                നിർത്തുക;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_അട്ടി->tip == mem_അട്ടി->datastart );

        mem_അട്ടി->isFree = സത്യം;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

വർഗ്ഗം cv::cuda::Stream::Impl
{
പൊതു:
    Impl(ശൂന്യം* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#അല്ലെങ്കിൽ

നാമാകാശ
{
    വർഗ്ഗം അട്ടി_Allocator;
}

വർഗ്ഗം cv::cuda::Stream::Impl
{
പൊതു:
    cudaStream_t stream;
    തർക്കം ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(സ്ഥിരം Ptr<GpuMat::Allocator>& allocator);
    Impl(സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(അസത്യം)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = സത്യം;

    allocator = makePtr<അട്ടി_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(സ്ഥിരം Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(അസത്യം), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = സത്യം;
}

cv::cuda::Stream::Impl::Impl(സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ cudaFlags) : stream(0), ownStream(അസത്യം)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = സത്യം;
    allocator = makePtr<അട്ടി_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(അസത്യം)
{
    allocator = makePtr<അട്ടി_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    എങ്കിൽ (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

നാമാകാശ cv { നാമാകാശ cuda
{
    വർഗ്ഗം DefaultDeviceInitializer
    {
    പൊതു:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(പൂർണ്ണസംഖ്യ deviceId);
        MemoryPool& getMemoryPool(പൂർണ്ണസംഖ്യ deviceId);

    സ്വകാര്യ:
        ശൂന്യം initStreams();
        ശൂന്യം initPools();

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

        വേണ്ടി (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<പൂർണ്ണസംഖ്യ>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(പൂർണ്ണസംഖ്യ deviceId)
    {
        AutoLock lock(streams_mtx_);

        എങ്കിൽ (streams_.empty())
        {
            പൂർണ്ണസംഖ്യ device_എണ്ണം = get_Cuda_Enabled_Device_എണ്ണം();

            എങ്കിൽ (device_എണ്ണം > 0)
                streams_.resize(device_എണ്ണം);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<പൂർണ്ണസംഖ്യ>(streams_.size()) );

        എങ്കിൽ (streams_[deviceId].empty())
        {
            cudaStream_t stream = ശൂന്യ;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(പുതിയ Stream(impl));
        }

        തിരികെ *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(പൂർണ്ണസംഖ്യ deviceId)
    {
        AutoLock lock(pools_mtx_);

        എങ്കിൽ (pools_.empty())
        {
            പൂർണ്ണസംഖ്യ device_എണ്ണം = get_Cuda_Enabled_Device_എണ്ണം();

            എങ്കിൽ (device_എണ്ണം > 0)
            {
                pools_.resize(device_എണ്ണം);
                വേണ്ടി (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<പൂർണ്ണസംഖ്യ>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<പൂർണ്ണസംഖ്യ>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        തിരികെ *p;
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
#അല്ലെങ്കിൽ
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(സ്ഥിരം Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(സ്ഥിരം size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

തർക്കം cv::cuda::Stream::queryIfComplete() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaError_t err = cudaStreamQuery(impl_->stream);

    എങ്കിൽ (err == cudaErrorNotReady || err == cudaSuccess)
        തിരികെ err == cudaSuccess;

    cudaSafeCall(err);
    തിരികെ അസത്യം;
#endif
}

ശൂന്യം cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

ശൂന്യം cv::cuda::Stream::waitEvent(സ്ഥിരം Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#എങ്കിൽ defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

നാമാകാശ
{
    ഘടന CallbackData
    {
        Stream::StreamCallback callback;
        ശൂന്യം* userData;

        CallbackData(Stream::StreamCallback callback_, ശൂന്യം* userData_) : callback(callback_), userData(userData_) {}
    };

    ശൂന്യം CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, ശൂന്യം* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<പൂർണ്ണസംഖ്യ>(status), data->userData);
        delete data;
    }
}

#endif

ശൂന്യം cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, ശൂന്യം* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    #എങ്കിൽ CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #അല്ലെങ്കിൽ
        CallbackData* data = പുതിയ CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ deviceId = getDevice();
    തിരികെ initializer.getNullStream(deviceId);
#endif
}

ശൂന്യം* cv::cuda::Stream::cudaPtr() സ്ഥിരം
{
#ifndef HAVE_CUDA
    തിരികെ nullptr;
#അല്ലെങ്കിൽ
    തിരികെ impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() സ്ഥിരം
{
#ifndef HAVE_CUDA
    തിരികെ 0;
#അല്ലെങ്കിൽ
    തിരികെ (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(സ്ഥിരം Stream& stream)
{
    തിരികെ stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    തിരികെ Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    തിരികെ cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

നാമാകാശ
{
    തർക്കം enableMemoryPool = അസത്യം;

    വർഗ്ഗം അട്ടി_Allocator : പൊതു GpuMat::Allocator
    {
    പൊതു:
        explicit അട്ടി_Allocator(cudaStream_t stream);
        ~അട്ടി_Allocator();

        തർക്കം allocate(GpuMat* mat, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, size_t elemSize) CV_OVERRIDE;
        ശൂന്യം സ്വതന്ത്രം(GpuMat* mat) CV_OVERRIDE;

    സ്വകാര്യ:
        അട്ടി_Allocator(സ്ഥിരം അട്ടി_Allocator&);
        അട്ടി_Allocator& operator =(സ്ഥിരം അട്ടി_Allocator&);

        cudaStream_t stream_;
        Memory_അട്ടി* mem_അട്ടി_2;
        size_t alignment_;
    };

    അട്ടി_Allocator::അട്ടി_Allocator(cudaStream_t stream) : stream_(stream), mem_അട്ടി_2(0)
    {
        എങ്കിൽ (enableMemoryPool)
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ deviceId = getDevice();
            mem_അട്ടി_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_അട്ടി();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    അട്ടി_Allocator::~അട്ടി_Allocator()
    {
        എങ്കിൽ (mem_അട്ടി_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_അട്ടി_2->pool->return_Mem_അട്ടി(mem_അട്ടി_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        തിരികെ res;
    }

    തർക്കം അട്ടി_Allocator::allocate(GpuMat* mat, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, size_t elemSize)
    {
        എങ്കിൽ (mem_അട്ടി_2 == 0)
            തിരികെ അസത്യം;

        size_t pitch, memSize;

        എങ്കിൽ (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        അല്ലെങ്കിൽ
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_അട്ടി_2->requestMemory(memSize);

        എങ്കിൽ (ptr == 0)
            തിരികെ അസത്യം;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (പൂർണ്ണസംഖ്യ*) fastMalloc(വലുപ്പം(പൂർണ്ണസംഖ്യ));

        തിരികെ സത്യം;
    }

    ശൂന്യം അട്ടി_Allocator::സ്വതന്ത്രം(GpuMat* mat)
    {
        എങ്കിൽ (mem_അട്ടി_2 == 0)
            തിരികെ;

        mem_അട്ടി_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

ശൂന്യം cv::cuda::setBufferPoolUsage(തർക്കം on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    enableMemoryPool = on;
#endif
}

ശൂന്യം cv::cuda::setBufferPoolConfig(പൂർണ്ണസംഖ്യ deviceId, size_t അട്ടി_Size, പൂർണ്ണസംഖ്യ അട്ടി_എണ്ണം)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(അട്ടി_Size);
    CV_UNUSED(അട്ടി_എണ്ണം);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    സ്ഥിരം പൂർണ്ണസംഖ്യ currentDevice = getDevice();

    എങ്കിൽ (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(അട്ടി_Size, അട്ടി_എണ്ണം);
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ device_എണ്ണം = get_Cuda_Enabled_Device_എണ്ണം();

        വേണ്ടി (deviceId = 0; deviceId < device_എണ്ണം; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(അട്ടി_Size, അട്ടി_എണ്ണം);
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
#അല്ലെങ്കിൽ
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    തിരികെ buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

വർഗ്ഗം cv::cuda::Event::Impl
{
പൊതു:
    Impl(unsigned പൂർണ്ണസംഖ്യ)
    {
        throw_no_cuda();
    }
};

#അല്ലെങ്കിൽ

വർഗ്ഗം cv::cuda::Event::Impl
{
പൊതു:
    cudaEvent_t event;
    തർക്കം ownEvent;

    explicit Impl(unsigned പൂർണ്ണസംഖ്യ flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned പൂർണ്ണസംഖ്യ flags) : event(0), ownEvent(അസത്യം)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = സത്യം;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(അസത്യം)
{
}

cv::cuda::Event::Impl::~Impl()
{
    എങ്കിൽ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(സ്ഥിരം Event& event)
{
    തിരികെ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    തിരികെ Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(സ്ഥിരം Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    impl_ = makePtr<Impl>(flags);
#endif
}

ശൂന്യം cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

തർക്കം cv::cuda::Event::queryIfComplete() സ്ഥിരം
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaError_t err = cudaEventQuery(impl_->event);

    എങ്കിൽ (err == cudaErrorNotReady || err == cudaSuccess)
        തിരികെ err == cudaSuccess;

    cudaSafeCall(err);
    തിരികെ അസത്യം;
#endif
}

ശൂന്യം cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#അല്ലെങ്കിൽ
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

ദശാംശം cv::cuda::Event::elapsedTime(സ്ഥിരം Event& start, സ്ഥിരം Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#അല്ലെങ്കിൽ
    ദശാംശം ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    തിരികെ ms;
#endif
}
