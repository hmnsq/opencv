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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ <cstdint>

using ନାମାକାଶ cv;
using ନାମାକାଶ cv::cuda;

#ଯଦି defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

ନାମାକାଶ
{
    ଶ୍ରେଣୀ MemoryPool;

    ଶ୍ରେଣୀ Memory_ଗଦା
    {
    ସର୍ବସାଧାରଣ:
        uchar* requestMemory(size_t size);
        ଶୂନ୍ୟ returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        ତାର୍କିକ isFree;
        MemoryPool* pool;

    #ଯଦି !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_ଗଦା::requestMemory(size_t size)
    {
        ଅଚଳ size_t freeMem = dataend - tip;

        ଯଦି (size > freeMem)
            ଫେରନ୍ତୁ 0;

        uchar* ptr = tip;

        tip += size;

    #ଯଦି !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        ଫେରନ୍ତୁ ptr;
    }

    ଶୂନ୍ୟ Memory_ଗଦା::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #ଯଦି !defined(NDEBUG)
        ଅଚଳ size_t allocSize = tip - ptr;
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

ନାମାକାଶ
{
    ଶ୍ରେଣୀ MemoryPool
    {
    ସର୍ବସାଧାରଣ:
        MemoryPool();
        ~MemoryPool() { release(); }

        ଶୂନ୍ୟ initialize(size_t ଗଦା_Size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଦା_ଗଣନା);
        ଶୂନ୍ୟ release();

        Memory_ଗଦା* get_Free_Mem_ଗଦା();
        ଶୂନ୍ୟ return_Mem_ଗଦା(Memory_ଗଦା* mem_ଗଦା);

    ବ୍ୟକ୍ତିଗତ:
        ଶୂନ୍ୟ initilizeImpl();

        Mutex mtx_;

        ତାର୍କିକ initialized_;
        size_t ଗଦା_Size_2;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଦା_ଗଣନା_2;

        uchar* mem_;

        std::vector<Memory_ଗଦା> stacks_;

        MemoryPool(ଅଚଳ MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(ମିଥ୍ୟା), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ଗଦା_Size_2 = 10 * 1024 * 1024;
        ଗଦା_ଗଣନା_2 = 5;
    }

    ଶୂନ୍ୟ MemoryPool::initialize(size_t ଗଦା_Size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଦା_ଗଣନା)
    {
        AutoLock lock(mtx_);

        release();

        ଗଦା_Size_2 = ଗଦା_Size;
        ଗଦା_ଗଣନା_2 = ଗଦା_ଗଣନା;

        initilizeImpl();
    }

    ଶୂନ୍ୟ MemoryPool::initilizeImpl()
    {
        ଅଚଳ size_t totalSize = ଗଦା_Size_2 * ଗଦା_ଗଣନା_2;

        ଯଦି (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            ଯଦି (err != cudaSuccess)
                ଫେରନ୍ତୁ;

            stacks_.resize(ଗଦା_ଗଣନା_2);

            uchar* ptr = mem_;

            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଦା_ଗଣନା_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ଗଦା_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = ସତ୍ୟ;
                stacks_[i].pool = ଏହି;

                ptr += ଗଦା_Size_2;
            }

            initialized_ = ସତ୍ୟ;
        }
    }

    ଶୂନ୍ୟ MemoryPool::release()
    {
        ଯଦି (mem_)
        {
#ଯଦି !defined(NDEBUG)
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଦା_ଗଣନା_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = ମିଥ୍ୟା;
        }
    }

    Memory_ଗଦା* MemoryPool::get_Free_Mem_ଗଦା()
    {
        AutoLock lock(mtx_);

        ଯଦି (!initialized_)
            initilizeImpl();

        ଯଦି (!mem_)
            ଫେରନ୍ତୁ 0;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଦା_ଗଣନା_2; ++i)
        {
            ଯଦି (stacks_[i].isFree)
            {
                stacks_[i].isFree = ମିଥ୍ୟା;
                ଫେରନ୍ତୁ &stacks_[i];
            }
        }

        ଫେରନ୍ତୁ 0;
    }

    ଶୂନ୍ୟ MemoryPool::return_Mem_ଗଦା(Memory_ଗଦା* mem_ଗଦା)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_ଗଦା->isFree );

#ଯଦି !defined(NDEBUG)
        ତାର୍କିକ found = ମିଥ୍ୟା;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଦା_ଗଣନା_2; ++i)
        {
            ଯଦି (mem_ଗଦା == &stacks_[i])
            {
                found = ସତ୍ୟ;
                ଭାଙ୍ଗ;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_ଗଦା->tip == mem_ଗଦା->datastart );

        mem_ଗଦା->isFree = ସତ୍ୟ;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

ଶ୍ରେଣୀ cv::cuda::Stream::Impl
{
ସର୍ବସାଧାରଣ:
    Impl(ଶୂନ୍ୟ* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#ଅନ୍ୟଥା

ନାମାକାଶ
{
    ଶ୍ରେଣୀ ଗଦା_Allocator;
}

ଶ୍ରେଣୀ cv::cuda::Stream::Impl
{
ସର୍ବସାଧାରଣ:
    cudaStream_t stream;
    ତାର୍କିକ ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ଅଚଳ Ptr<GpuMat::Allocator>& allocator);
    Impl(ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(ମିଥ୍ୟା)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = ସତ୍ୟ;

    allocator = makePtr<ଗଦା_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ଅଚଳ Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(ମିଥ୍ୟା), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = ସତ୍ୟ;
}

cv::cuda::Stream::Impl::Impl(ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cudaFlags) : stream(0), ownStream(ମିଥ୍ୟା)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = ସତ୍ୟ;
    allocator = makePtr<ଗଦା_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(ମିଥ୍ୟା)
{
    allocator = makePtr<ଗଦା_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    ଯଦି (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

ନାମାକାଶ cv { ନାମାକାଶ cuda
{
    ଶ୍ରେଣୀ DefaultDeviceInitializer
    {
    ସର୍ବସାଧାରଣ:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId);
        MemoryPool& getMemoryPool(ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId);

    ବ୍ୟକ୍ତିଗତ:
        ଶୂନ୍ୟ initStreams();
        ଶୂନ୍ୟ initPools();

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

        ପାଇଁ (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId)
    {
        AutoLock lock(streams_mtx_);

        ଯଦି (streams_.empty())
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା device_ଗଣନା = get_Cuda_Enabled_Device_ଗଣନା();

            ଯଦି (device_ଗଣନା > 0)
                streams_.resize(device_ଗଣନା);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(streams_.size()) );

        ଯଦି (streams_[deviceId].empty())
        {
            cudaStream_t stream = ଖାଲି;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(ନୂତନ Stream(impl));
        }

        ଫେରନ୍ତୁ *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId)
    {
        AutoLock lock(pools_mtx_);

        ଯଦି (pools_.empty())
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା device_ଗଣନା = get_Cuda_Enabled_Device_ଗଣନା();

            ଯଦି (device_ଗଣନା > 0)
            {
                pools_.resize(device_ଗଣନା);
                ପାଇଁ (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        ଫେରନ୍ତୁ *p;
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
#ଅନ୍ୟଥା
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(ଅଚଳ Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#ଅନ୍ୟଥା
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ଅଚଳ size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#ଅନ୍ୟଥା
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

ତାର୍କିକ cv::cuda::Stream::queryIfComplete() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaError_t err = cudaStreamQuery(impl_->stream);

    ଯଦି (err == cudaErrorNotReady || err == cudaSuccess)
        ଫେରନ୍ତୁ err == cudaSuccess;

    cudaSafeCall(err);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ଶୂନ୍ୟ cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

ଶୂନ୍ୟ cv::cuda::Stream::waitEvent(ଅଚଳ Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#ଯଦି defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

ନାମାକାଶ
{
    ସଂରଚନା CallbackData
    {
        Stream::StreamCallback callback;
        ଶୂନ୍ୟ* userData;

        CallbackData(Stream::StreamCallback callback_, ଶୂନ୍ୟ* userData_) : callback(callback_), userData(userData_) {}
    };

    ଶୂନ୍ୟ CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, ଶୂନ୍ୟ* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(status), data->userData);
        delete data;
    }
}

#endif

ଶୂନ୍ୟ cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, ଶୂନ୍ୟ* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#ଅନ୍ୟଥା
    #ଯଦି CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #ଅନ୍ୟଥା
        CallbackData* data = ନୂତନ CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId = getDevice();
    ଫେରନ୍ତୁ initializer.getNullStream(deviceId);
#endif
}

ଶୂନ୍ୟ* cv::cuda::Stream::cudaPtr() ଅଚଳ
{
#ifndef HAVE_CUDA
    ଫେରନ୍ତୁ nullptr;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ଅଚଳ
{
#ifndef HAVE_CUDA
    ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ଅଚଳ Stream& stream)
{
    ଫେରନ୍ତୁ stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    ଫେରନ୍ତୁ Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

ନାମାକାଶ
{
    ତାର୍କିକ enableMemoryPool = ମିଥ୍ୟା;

    ଶ୍ରେଣୀ ଗଦା_Allocator : ସର୍ବସାଧାରଣ GpuMat::Allocator
    {
    ସର୍ବସାଧାରଣ:
        explicit ଗଦା_Allocator(cudaStream_t stream);
        ~ଗଦା_Allocator();

        ତାର୍କିକ allocate(GpuMat* mat, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, size_t elemSize) CV_OVERRIDE;
        ଶୂନ୍ୟ ମୁକ୍ତ(GpuMat* mat) CV_OVERRIDE;

    ବ୍ୟକ୍ତିଗତ:
        ଗଦା_Allocator(ଅଚଳ ଗଦା_Allocator&);
        ଗଦା_Allocator& operator =(ଅଚଳ ଗଦା_Allocator&);

        cudaStream_t stream_;
        Memory_ଗଦା* mem_ଗଦା_2;
        size_t alignment_;
    };

    ଗଦା_Allocator::ଗଦା_Allocator(cudaStream_t stream) : stream_(stream), mem_ଗଦା_2(0)
    {
        ଯଦି (enableMemoryPool)
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId = getDevice();
            mem_ଗଦା_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_ଗଦା();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ଗଦା_Allocator::~ଗଦା_Allocator()
    {
        ଯଦି (mem_ଗଦା_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_ଗଦା_2->pool->return_Mem_ଗଦା(mem_ଗଦା_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ଫେରନ୍ତୁ res;
    }

    ତାର୍କିକ ଗଦା_Allocator::allocate(GpuMat* mat, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, size_t elemSize)
    {
        ଯଦି (mem_ଗଦା_2 == 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        size_t pitch, memSize;

        ଯଦି (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        ଅନ୍ୟଥା
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_ଗଦା_2->requestMemory(memSize);

        ଯଦି (ptr == 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*) fastMalloc(ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା));

        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଶୂନ୍ୟ ଗଦା_Allocator::ମୁକ୍ତ(GpuMat* mat)
    {
        ଯଦି (mem_ଗଦା_2 == 0)
            ଫେରନ୍ତୁ;

        mem_ଗଦା_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

ଶୂନ୍ୟ cv::cuda::setBufferPoolUsage(ତାର୍କିକ on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#ଅନ୍ୟଥା
    enableMemoryPool = on;
#endif
}

ଶୂନ୍ୟ cv::cuda::setBufferPoolConfig(ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId, size_t ଗଦା_Size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଦା_ଗଣନା)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(ଗଦା_Size);
    CV_UNUSED(ଗଦା_ଗଣନା);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା currentDevice = getDevice();

    ଯଦି (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(ଗଦା_Size, ଗଦା_ଗଣନା);
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା device_ଗଣନା = get_Cuda_Enabled_Device_ଗଣନା();

        ପାଇଁ (deviceId = 0; deviceId < device_ଗଣନା; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(ଗଦା_Size, ଗଦା_ଗଣନା);
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
#ଅନ୍ୟଥା
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#ଅନ୍ୟଥା
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    ଫେରନ୍ତୁ buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

ଶ୍ରେଣୀ cv::cuda::Event::Impl
{
ସର୍ବସାଧାରଣ:
    Impl(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)
    {
        throw_no_cuda();
    }
};

#ଅନ୍ୟଥା

ଶ୍ରେଣୀ cv::cuda::Event::Impl
{
ସର୍ବସାଧାରଣ:
    cudaEvent_t event;
    ତାର୍କିକ ownEvent;

    explicit Impl(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) : event(0), ownEvent(ମିଥ୍ୟା)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = ସତ୍ୟ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(ମିଥ୍ୟା)
{
}

cv::cuda::Event::Impl::~Impl()
{
    ଯଦି (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ଅଚଳ Event& event)
{
    ଫେରନ୍ତୁ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ଫେରନ୍ତୁ Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ଅଚଳ Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#ଅନ୍ୟଥା
    impl_ = makePtr<Impl>(flags);
#endif
}

ଶୂନ୍ୟ cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

ତାର୍କିକ cv::cuda::Event::queryIfComplete() ଅଚଳ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaError_t err = cudaEventQuery(impl_->event);

    ଯଦି (err == cudaErrorNotReady || err == cudaSuccess)
        ଫେରନ୍ତୁ err == cudaSuccess;

    cudaSafeCall(err);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ଶୂନ୍ୟ cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ଅନ୍ୟଥା
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

ଦଶମିକ cv::cuda::Event::elapsedTime(ଅଚଳ Event& start, ଅଚଳ Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#ଅନ୍ୟଥା
    ଦଶମିକ ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    ଫେରନ୍ତୁ ms;
#endif
}
