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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <cstdint>

using ನಾಮಾಕಾಶ cv;
using ನಾಮಾಕಾಶ cv::cuda;

#ಯದಿ defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

/////////////////////////////////////////////////////////////
/// MemoryStack

#ifdef HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ವರ್ಗ MemoryPool;

    ವರ್ಗ Memory_ರಾಶಿ
    {
    ಸಾರ್ವಜನಿಕ:
        uchar* requestMemory(size_t size);
        ಶೂನ್ಯ returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        ತಾರ್ಕಿಕ isFree;
        MemoryPool* pool;

    #ಯದಿ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #endif
    };

    uchar* Memory_ರಾಶಿ::requestMemory(size_t size)
    {
        ಧ್ರುವ size_t freeMem = dataend - tip;

        ಯದಿ (size > freeMem)
            ಪ್ರತ್ಯಾಗಮ 0;

        uchar* ptr = tip;

        tip += size;

    #ಯದಿ !defined(NDEBUG)
        allocations.push_back(size);
    #endif

        ಪ್ರತ್ಯಾಗಮ ptr;
    }

    ಶೂನ್ಯ Memory_ರಾಶಿ::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #ಯದಿ !defined(NDEBUG)
        ಧ್ರುವ size_t allocSize = tip - ptr;
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

ನಾಮಾಕಾಶ
{
    ವರ್ಗ MemoryPool
    {
    ಸಾರ್ವಜನಿಕ:
        MemoryPool();
        ~MemoryPool() { release(); }

        ಶೂನ್ಯ initialize(size_t ರಾಶಿ_Size, ಪೂರ್ಣಾಙ್ಕ ರಾಶಿ_ಎಣಿಕೆ);
        ಶೂನ್ಯ release();

        Memory_ರಾಶಿ* get_Free_Mem_ರಾಶಿ();
        ಶೂನ್ಯ return_Mem_ರಾಶಿ(Memory_ರಾಶಿ* mem_ರಾಶಿ);

    ಗುಪ್ತ:
        ಶೂನ್ಯ initilizeImpl();

        Mutex mtx_;

        ತಾರ್ಕಿಕ initialized_;
        size_t ರಾಶಿ_Size_2;
        ಪೂರ್ಣಾಙ್ಕ ರಾಶಿ_ಎಣಿಕೆ_2;

        uchar* mem_;

        std::vector<Memory_ರಾಶಿ> stacks_;

        MemoryPool(ಧ್ರುವ MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(ಅಸತ್ಯ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ರಾಶಿ_Size_2 = 10 * 1024 * 1024;
        ರಾಶಿ_ಎಣಿಕೆ_2 = 5;
    }

    ಶೂನ್ಯ MemoryPool::initialize(size_t ರಾಶಿ_Size, ಪೂರ್ಣಾಙ್ಕ ರಾಶಿ_ಎಣಿಕೆ)
    {
        AutoLock lock(mtx_);

        release();

        ರಾಶಿ_Size_2 = ರಾಶಿ_Size;
        ರಾಶಿ_ಎಣಿಕೆ_2 = ರಾಶಿ_ಎಣಿಕೆ;

        initilizeImpl();
    }

    ಶೂನ್ಯ MemoryPool::initilizeImpl()
    {
        ಧ್ರುವ size_t totalSize = ರಾಶಿ_Size_2 * ರಾಶಿ_ಎಣಿಕೆ_2;

        ಯದಿ (totalSize > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, totalSize);
            ಯದಿ (err != cudaSuccess)
                ಪ್ರತ್ಯಾಗಮ;

            stacks_.resize(ರಾಶಿ_ಎಣಿಕೆ_2);

            uchar* ptr = mem_;

            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ರಾಶಿ_Size_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = ಸತ್ಯ;
                stacks_[i].pool = ಏತತ್;

                ptr += ರಾಶಿ_Size_2;
            }

            initialized_ = ಸತ್ಯ;
        }
    }

    ಶೂನ್ಯ MemoryPool::release()
    {
        ಯದಿ (mem_)
        {
#ಯದಿ !defined(NDEBUG)
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#endif

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = ಅಸತ್ಯ;
        }
    }

    Memory_ರಾಶಿ* MemoryPool::get_Free_Mem_ರಾಶಿ()
    {
        AutoLock lock(mtx_);

        ಯದಿ (!initialized_)
            initilizeImpl();

        ಯದಿ (!mem_)
            ಪ್ರತ್ಯಾಗಮ 0;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
        {
            ಯದಿ (stacks_[i].isFree)
            {
                stacks_[i].isFree = ಅಸತ್ಯ;
                ಪ್ರತ್ಯಾಗಮ &stacks_[i];
            }
        }

        ಪ್ರತ್ಯಾಗಮ 0;
    }

    ಶೂನ್ಯ MemoryPool::return_Mem_ರಾಶಿ(Memory_ರಾಶಿ* mem_ರಾಶಿ)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_ರಾಶಿ->isFree );

#ಯದಿ !defined(NDEBUG)
        ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
        {
            ಯದಿ (mem_ರಾಶಿ == &stacks_[i])
            {
                found = ಸತ್ಯ;
                ಭಙ್ಗ;
            }
        }
        CV_DbgAssert( found );
#endif

        CV_DbgAssert( mem_ರಾಶಿ->tip == mem_ರಾಶಿ->datastart );

        mem_ರಾಶಿ->isFree = ಸತ್ಯ;
    }
}

#endif

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ifndef HAVE_CUDA

ವರ್ಗ cv::cuda::Stream::Impl
{
ಸಾರ್ವಜನಿಕ:
    Impl(ಶೂನ್ಯ* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#ಅನ್ಯಥಾ

ನಾಮಾಕಾಶ
{
    ವರ್ಗ ರಾಶಿ_Allocator;
}

ವರ್ಗ cv::cuda::Stream::Impl
{
ಸಾರ್ವಜನಿಕ:
    cudaStream_t stream;
    ತಾರ್ಕಿಕ ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ಧ್ರುವ Ptr<GpuMat::Allocator>& allocator);
    Impl(ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(ಅಸತ್ಯ)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = ಸತ್ಯ;

    allocator = makePtr<ರಾಶಿ_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(ಧ್ರುವ Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(ಅಸತ್ಯ), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = ಸತ್ಯ;
}

cv::cuda::Stream::Impl::Impl(ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ cudaFlags) : stream(0), ownStream(ಅಸತ್ಯ)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = ಸತ್ಯ;
    allocator = makePtr<ರಾಶಿ_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(ಅಸತ್ಯ)
{
    allocator = makePtr<ರಾಶಿ_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    ಯದಿ (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ifdef HAVE_CUDA

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ cuda
{
    ವರ್ಗ DefaultDeviceInitializer
    {
    ಸಾರ್ವಜನಿಕ:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(ಪೂರ್ಣಾಙ್ಕ deviceId);
        MemoryPool& getMemoryPool(ಪೂರ್ಣಾಙ್ಕ deviceId);

    ಗುಪ್ತ:
        ಶೂನ್ಯ initStreams();
        ಶೂನ್ಯ initPools();

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

        ಪ್ರತಿ (size_t i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<ಪೂರ್ಣಾಙ್ಕ>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    Stream& DefaultDeviceInitializer::getNullStream(ಪೂರ್ಣಾಙ್ಕ deviceId)
    {
        AutoLock lock(streams_mtx_);

        ಯದಿ (streams_.empty())
        {
            ಪೂರ್ಣಾಙ್ಕ device_ಎಣಿಕೆ = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

            ಯದಿ (device_ಎಣಿಕೆ > 0)
                streams_.resize(device_ಎಣಿಕೆ);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ಪೂರ್ಣಾಙ್ಕ>(streams_.size()) );

        ಯದಿ (streams_[deviceId].empty())
        {
            cudaStream_t stream = ರಿಕ್ತ;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(ನೂತನ Stream(impl));
        }

        ಪ್ರತ್ಯಾಗಮ *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(ಪೂರ್ಣಾಙ್ಕ deviceId)
    {
        AutoLock lock(pools_mtx_);

        ಯದಿ (pools_.empty())
        {
            ಪೂರ್ಣಾಙ್ಕ device_ಎಣಿಕೆ = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

            ಯದಿ (device_ಎಣಿಕೆ > 0)
            {
                pools_.resize(device_ಎಣಿಕೆ);
                ಪ್ರತಿ (size_t i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<ಪೂರ್ಣಾಙ್ಕ>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ಪೂರ್ಣಾಙ್ಕ>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        ಪ್ರತ್ಯಾಗಮ *p;
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
#ಅನ್ಯಥಾ
    impl_ = makePtr<Impl>();
#endif
}

cv::cuda::Stream::Stream(ಧ್ರುವ Ptr<GpuMat::Allocator>& allocator)
{
#ifndef HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    impl_ = makePtr<Impl>(allocator);
#endif
}

cv::cuda::Stream::Stream(ಧ್ರುವ size_t cudaFlags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#endif
}

ತಾರ್ಕಿಕ cv::cuda::Stream::queryIfComplete() ಧ್ರುವ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ಅನ್ಯಥಾ
    cudaError_t err = cudaStreamQuery(impl_->stream);

    ಯದಿ (err == cudaErrorNotReady || err == cudaSuccess)
        ಪ್ರತ್ಯಾಗಮ err == cudaSuccess;

    cudaSafeCall(err);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಶೂನ್ಯ cv::cuda::Stream::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ಅನ್ಯಥಾ
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#endif
}

ಶೂನ್ಯ cv::cuda::Stream::waitEvent(ಧ್ರುವ Event& event)
{
#ifndef HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#endif
}

#ಯದಿ defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

ನಾಮಾಕಾಶ
{
    ಸಂರಚನಾ CallbackData
    {
        Stream::StreamCallback callback;
        ಶೂನ್ಯ* userData;

        CallbackData(Stream::StreamCallback callback_, ಶೂನ್ಯ* userData_) : callback(callback_), userData(userData_) {}
    };

    ಶೂನ್ಯ CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, ಶೂನ್ಯ* userData)
    {
        CallbackData* data = reinterpret_cast<CallbackData*>(userData);
        data->callback(static_cast<ಪೂರ್ಣಾಙ್ಕ>(status), data->userData);
        delete data;
    }
}

#endif

ಶೂನ್ಯ cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, ಶೂನ್ಯ* userData)
{
#ifndef HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(userData);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    #ಯದಿ CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(userData);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #ಅನ್ಯಥಾ
        CallbackData* data = ನೂತನ CallbackData(callback, userData);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, data, 0) );
    #endif
#endif
}

Stream& cv::cuda::Stream::Null()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ deviceId = getDevice();
    ಪ್ರತ್ಯಾಗಮ initializer.getNullStream(deviceId);
#endif
}

ಶೂನ್ಯ* cv::cuda::Stream::cudaPtr() ಧ್ರುವ
{
#ifndef HAVE_CUDA
    ಪ್ರತ್ಯಾಗಮ nullptr;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ impl_->stream;
#endif
}

cv::cuda::Stream::operator bool_type() ಧ್ರುವ
{
#ifndef HAVE_CUDA
    ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#endif
}

#ifdef HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(ಧ್ರುವ Stream& stream)
{
    ಪ್ರತ್ಯಾಗಮ stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    ಪ್ರತ್ಯಾಗಮ Stream(makePtr<Stream::Impl>(stream));
}

#endif

Stream cv::cuda::wrapStream(size_t cudaStreamMemoryAddress) {
#ifndef HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#endif
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ifdef HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ತಾರ್ಕಿಕ enableMemoryPool = ಅಸತ್ಯ;

    ವರ್ಗ ರಾಶಿ_Allocator : ಸಾರ್ವಜನಿಕ GpuMat::Allocator
    {
    ಸಾರ್ವಜನಿಕ:
        explicit ರಾಶಿ_Allocator(cudaStream_t stream);
        ~ರಾಶಿ_Allocator();

        ತಾರ್ಕಿಕ allocate(GpuMat* mat, ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, size_t elemSize) CV_OVERRIDE;
        ಶೂನ್ಯ ಮುಕ್ತ(GpuMat* mat) CV_OVERRIDE;

    ಗುಪ್ತ:
        ರಾಶಿ_Allocator(ಧ್ರುವ ರಾಶಿ_Allocator&);
        ರಾಶಿ_Allocator& operator =(ಧ್ರುವ ರಾಶಿ_Allocator&);

        cudaStream_t stream_;
        Memory_ರಾಶಿ* mem_ರಾಶಿ_2;
        size_t alignment_;
    };

    ರಾಶಿ_Allocator::ರಾಶಿ_Allocator(cudaStream_t stream) : stream_(stream), mem_ರಾಶಿ_2(0)
    {
        ಯದಿ (enableMemoryPool)
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ deviceId = getDevice();
            mem_ರಾಶಿ_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_ರಾಶಿ();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ರಾಶಿ_Allocator::~ರಾಶಿ_Allocator()
    {
        ಯದಿ (mem_ರಾಶಿ_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_ರಾಶಿ_2->pool->return_Mem_ರಾಶಿ(mem_ರಾಶಿ_2);
        }
    }

    size_t alignUp(size_t what, size_t alignment)
    {
        size_t alignMask = alignment-1;
        size_t inverseAlignMask = ~alignMask;
        size_t res = (what + alignMask) & inverseAlignMask;
        ಪ್ರತ್ಯಾಗಮ res;
    }

    ತಾರ್ಕಿಕ ರಾಶಿ_Allocator::allocate(GpuMat* mat, ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, size_t elemSize)
    {
        ಯದಿ (mem_ರಾಶಿ_2 == 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        size_t pitch, memSize;

        ಯದಿ (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elemSize, alignment_);
            memSize = pitch * rows;
        }
        ಅನ್ಯಥಾ
        {
            // Single row or single column must be continuous
            pitch = elemSize * cols;
            memSize = alignUp(elemSize * cols * rows, 64);
        }

        uchar* ptr = mem_ರಾಶಿ_2->requestMemory(memSize);

        ಯದಿ (ptr == 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        mat->data = ptr;
        mat->step = pitch;
        mat->refcount = (ಪೂರ್ಣಾಙ್ಕ*) fastMalloc(ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ));

        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಶೂನ್ಯ ರಾಶಿ_Allocator::ಮುಕ್ತ(GpuMat* mat)
    {
        ಯದಿ (mem_ರಾಶಿ_2 == 0)
            ಪ್ರತ್ಯಾಗಮ;

        mem_ರಾಶಿ_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#endif

/////////////////////////////////////////////////////////////
/// BufferPool

ಶೂನ್ಯ cv::cuda::setBufferPoolUsage(ತಾರ್ಕಿಕ on)
{
#ifndef HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    enableMemoryPool = on;
#endif
}

ಶೂನ್ಯ cv::cuda::setBufferPoolConfig(ಪೂರ್ಣಾಙ್ಕ deviceId, size_t ರಾಶಿ_Size, ಪೂರ್ಣಾಙ್ಕ ರಾಶಿ_ಎಣಿಕೆ)
{
#ifndef HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(ರಾಶಿ_Size);
    CV_UNUSED(ರಾಶಿ_ಎಣಿಕೆ);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ currentDevice = getDevice();

    ಯದಿ (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(ರಾಶಿ_Size, ರಾಶಿ_ಎಣಿಕೆ);
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ device_ಎಣಿಕೆ = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

        ಪ್ರತಿ (deviceId = 0; deviceId < device_ಎಣಿಕೆ; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(ರಾಶಿ_Size, ರಾಶಿ_ಎಣಿಕೆ);
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
#ಅನ್ಯಥಾ
cv::cuda::BufferPool::BufferPool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#endif

GpuMat cv::cuda::BufferPool::getBuffer(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type)
{
#ifndef HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    ಪ್ರತ್ಯಾಗಮ buf;
#endif
}


////////////////////////////////////////////////////////////////
// Event

#ifndef HAVE_CUDA

ವರ್ಗ cv::cuda::Event::Impl
{
ಸಾರ್ವಜನಿಕ:
    Impl(unsigned ಪೂರ್ಣಾಙ್ಕ)
    {
        throw_no_cuda();
    }
};

#ಅನ್ಯಥಾ

ವರ್ಗ cv::cuda::Event::Impl
{
ಸಾರ್ವಜನಿಕ:
    cudaEvent_t event;
    ತಾರ್ಕಿಕ ownEvent;

    explicit Impl(unsigned ಪೂರ್ಣಾಙ್ಕ flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(unsigned ಪೂರ್ಣಾಙ್ಕ flags) : event(0), ownEvent(ಅಸತ್ಯ)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = ಸತ್ಯ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(ಅಸತ್ಯ)
{
}

cv::cuda::Event::Impl::~Impl()
{
    ಯದಿ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ಧ್ರುವ Event& event)
{
    ಪ್ರತ್ಯಾಗಮ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ಪ್ರತ್ಯಾಗಮ Event(makePtr<Event::Impl>(event));
}

#endif

cv::cuda::Event::Event(ಧ್ರುವ Event::CreateFlags flags)
{
#ifndef HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    impl_ = makePtr<Impl>(flags);
#endif
}

ಶೂನ್ಯ cv::cuda::Event::record(Stream& stream)
{
#ifndef HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#endif
}

ತಾರ್ಕಿಕ cv::cuda::Event::queryIfComplete() ಧ್ರುವ
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ಅನ್ಯಥಾ
    cudaError_t err = cudaEventQuery(impl_->event);

    ಯದಿ (err == cudaErrorNotReady || err == cudaSuccess)
        ಪ್ರತ್ಯಾಗಮ err == cudaSuccess;

    cudaSafeCall(err);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಶೂನ್ಯ cv::cuda::Event::waitForCompletion()
{
#ifndef HAVE_CUDA
    throw_no_cuda();
#ಅನ್ಯಥಾ
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#endif
}

ಭಿನ್ನ cv::cuda::Event::elapsedTime(ಧ್ರುವ Event& start, ಧ್ರುವ Event& end)
{
#ifndef HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#ಅನ್ಯಥಾ
    ಭಿನ್ನ ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    ಪ್ರತ್ಯಾಗಮ ms;
#endif
}
