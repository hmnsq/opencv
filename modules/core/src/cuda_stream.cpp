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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdint>

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::cuda;

#ਜੇ defined(_MSC_VER)
#ਹਦਾਇਤ warning(disable : 4702)  // unreachable code
#ਜੇ_ਅੰਤ

/////////////////////////////////////////////////////////////
/// MemoryStack

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    ਵਰਗ ਸਮ੍ਰਿਤੀ_Pool;

    ਵਰਗ ਸਮ੍ਰਿਤੀ_ਢੇਰ
    {
    ਜਨਤਕ:
        uchar* request_ਸਮ੍ਰਿਤੀ(size_t size);
        ਖਾਲੀ return_ਸਮ੍ਰਿਤੀ(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        ਤਾਰਕਿਕ isFree;
        ਸਮ੍ਰਿਤੀ_Pool* pool;

    #ਜੇ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #ਜੇ_ਅੰਤ
    };

    uchar* ਸਮ੍ਰਿਤੀ_ਢੇਰ::request_ਸਮ੍ਰਿਤੀ(size_t size)
    {
        ਅਚਲ size_t freeMem = dataend - tip;

        ਜੇ (size > freeMem)
            ਵਾਪਸ 0;

        uchar* ptr = tip;

        tip += size;

    #ਜੇ !defined(NDEBUG)
        allocations.push_back(size);
    #ਜੇ_ਅੰਤ

        ਵਾਪਸ ptr;
    }

    ਖਾਲੀ ਸਮ੍ਰਿਤੀ_ਢੇਰ::return_ਸਮ੍ਰਿਤੀ(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #ਜੇ !defined(NDEBUG)
        ਅਚਲ size_t alloc_ਮਾਪ = tip - ptr;
        CV_Assert( alloc_ਮਾਪ == allocations.back() );
        allocations.pop_back();
    #ਜੇ_ਅੰਤ

        tip = ptr;
    }
}

#ਜੇ_ਅੰਤ

/////////////////////////////////////////////////////////////
/// MemoryPool

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    ਵਰਗ ਸਮ੍ਰਿਤੀ_Pool
    {
    ਜਨਤਕ:
        ਸਮ੍ਰਿਤੀ_Pool();
        ~ਸਮ੍ਰਿਤੀ_Pool() { release(); }

        ਖਾਲੀ initialize(size_t ਢੇਰ_ਮਾਪ, ਪੂਰਨ ਢੇਰ_ਗਿਣਤੀ);
        ਖਾਲੀ release();

        ਸਮ੍ਰਿਤੀ_ਢੇਰ* get_Free_Mem_ਢੇਰ();
        ਖਾਲੀ return_Mem_ਢੇਰ(ਸਮ੍ਰਿਤੀ_ਢੇਰ* mem_ਢੇਰ);

    ਨਿਜੀ:
        ਖਾਲੀ initilizeImpl();

        Mutex mtx_;

        ਤਾਰਕਿਕ initialized_;
        size_t ਢੇਰ_ਮਾਪ_2;
        ਪੂਰਨ ਢੇਰ_ਗਿਣਤੀ_2;

        uchar* mem_;

        std::vector<ਸਮ੍ਰਿਤੀ_ਢੇਰ> stacks_;

        ਸਮ੍ਰਿਤੀ_Pool(ਅਚਲ ਸਮ੍ਰਿਤੀ_Pool&); //= delete;
    };

    ਸਮ੍ਰਿਤੀ_Pool::ਸਮ੍ਰਿਤੀ_Pool() : initialized_(ਝੂਠ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ਢੇਰ_ਮਾਪ_2 = 10 * 1024 * 1024;
        ਢੇਰ_ਗਿਣਤੀ_2 = 5;
    }

    ਖਾਲੀ ਸਮ੍ਰਿਤੀ_Pool::initialize(size_t ਢੇਰ_ਮਾਪ, ਪੂਰਨ ਢੇਰ_ਗਿਣਤੀ)
    {
        Auto_ਤਾਲਾ lock(mtx_);

        release();

        ਢੇਰ_ਮਾਪ_2 = ਢੇਰ_ਮਾਪ;
        ਢੇਰ_ਗਿਣਤੀ_2 = ਢੇਰ_ਗਿਣਤੀ;

        initilizeImpl();
    }

    ਖਾਲੀ ਸਮ੍ਰਿਤੀ_Pool::initilizeImpl()
    {
        ਅਚਲ size_t ਕੁੱਲ_ਮਾਪ = ਢੇਰ_ਮਾਪ_2 * ਢੇਰ_ਗਿਣਤੀ_2;

        ਜੇ (ਕੁੱਲ_ਮਾਪ > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, ਕੁੱਲ_ਮਾਪ);
            ਜੇ (err != cudaSuccess)
                ਵਾਪਸ;

            stacks_.resize(ਢੇਰ_ਗਿਣਤੀ_2);

            uchar* ptr = mem_;

            ਲਈ (ਪੂਰਨ i = 0; i < ਢੇਰ_ਗਿਣਤੀ_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ਢੇਰ_ਮਾਪ_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = ਸੱਚ;
                stacks_[i].pool = ਇਹ;

                ptr += ਢੇਰ_ਮਾਪ_2;
            }

            initialized_ = ਸੱਚ;
        }
    }

    ਖਾਲੀ ਸਮ੍ਰਿਤੀ_Pool::release()
    {
        ਜੇ (mem_)
        {
#ਜੇ !defined(NDEBUG)
            ਲਈ (ਪੂਰਨ i = 0; i < ਢੇਰ_ਗਿਣਤੀ_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#ਜੇ_ਅੰਤ

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = ਝੂਠ;
        }
    }

    ਸਮ੍ਰਿਤੀ_ਢੇਰ* ਸਮ੍ਰਿਤੀ_Pool::get_Free_Mem_ਢੇਰ()
    {
        Auto_ਤਾਲਾ lock(mtx_);

        ਜੇ (!initialized_)
            initilizeImpl();

        ਜੇ (!mem_)
            ਵਾਪਸ 0;

        ਲਈ (ਪੂਰਨ i = 0; i < ਢੇਰ_ਗਿਣਤੀ_2; ++i)
        {
            ਜੇ (stacks_[i].isFree)
            {
                stacks_[i].isFree = ਝੂਠ;
                ਵਾਪਸ &stacks_[i];
            }
        }

        ਵਾਪਸ 0;
    }

    ਖਾਲੀ ਸਮ੍ਰਿਤੀ_Pool::return_Mem_ਢੇਰ(ਸਮ੍ਰਿਤੀ_ਢੇਰ* mem_ਢੇਰ)
    {
        Auto_ਤਾਲਾ lock(mtx_);

        CV_DbgAssert( !mem_ਢੇਰ->isFree );

#ਜੇ !defined(NDEBUG)
        ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
        ਲਈ (ਪੂਰਨ i = 0; i < ਢੇਰ_ਗਿਣਤੀ_2; ++i)
        {
            ਜੇ (mem_ਢੇਰ == &stacks_[i])
            {
                ਮਿਲਿਆ = ਸੱਚ;
                ਰੋਕੋ;
            }
        }
        CV_DbgAssert( ਮਿਲਿਆ );
#ਜੇ_ਅੰਤ

        CV_DbgAssert( mem_ਢੇਰ->tip == mem_ਢੇਰ->datastart );

        mem_ਢੇਰ->isFree = ਸੱਚ;
    }
}

#ਜੇ_ਅੰਤ

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਵਰਗ cv::cuda::ਧਾਰਾ::Impl
{
ਜਨਤਕ:
    Impl(ਖਾਲੀ* ptr = 0)
    {
        CV_ਅਣਵਰਤਿਆ(ptr);
        throw_no_cuda();
    }
};

#ਨਹੀਂਤਾਂ

ਨਾਮਾਕਾਸ਼
{
    ਵਰਗ ਢੇਰ_Allocator;
}

ਵਰਗ cv::cuda::ਧਾਰਾ::Impl
{
ਜਨਤਕ:
    cuda_ਧਾਰਾ_t stream;
    ਤਾਰਕਿਕ own_ਧਾਰਾ;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ਅਚਲ Ptr<GpuMat::Allocator>& allocator);
    Impl(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cudaFlags);
    ਸਪਸ਼ਟ Impl(cuda_ਧਾਰਾ_t stream);

    ~Impl();
};

cv::cuda::ਧਾਰਾ::Impl::Impl() : stream(0), own_ਧਾਰਾ(ਝੂਠ)
{
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_ਧਾਰਾ_Create(&stream) );
    own_ਧਾਰਾ = ਸੱਚ;

    allocator = makePtr<ਢੇਰ_Allocator>(stream);
}

cv::cuda::ਧਾਰਾ::Impl::Impl(ਅਚਲ Ptr<GpuMat::Allocator>& allocator) : stream(0), own_ਧਾਰਾ(ਝੂਠ), allocator(allocator)
{
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_ਧਾਰਾ_Create(&stream) );
    own_ਧਾਰਾ = ਸੱਚ;
}

cv::cuda::ਧਾਰਾ::Impl::Impl(ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cudaFlags) : stream(0), own_ਧਾਰਾ(ਝੂਠ)
{
    cuda_ਸੁਰੱਖਿਅਤ_Call(cuda_ਧਾਰਾ_Create_With_Flags(&stream, cudaFlags));
    own_ਧਾਰਾ = ਸੱਚ;
    allocator = makePtr<ਢੇਰ_Allocator>(stream);
}

cv::cuda::ਧਾਰਾ::Impl::Impl(cuda_ਧਾਰਾ_t ਧਾਰਾ_3) : stream(ਧਾਰਾ_3), own_ਧਾਰਾ(ਝੂਠ)
{
    allocator = makePtr<ਢੇਰ_Allocator>(stream);
}

cv::cuda::ਧਾਰਾ::Impl::~Impl()
{
    allocator.release();

    ਜੇ (stream && own_ਧਾਰਾ)
    {
        cuda_ਧਾਰਾ_Destroy(stream);
    }
}

#ਜੇ_ਅੰਤ

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ cuda
{
    ਵਰਗ DefaultDeviceInitializer
    {
    ਜਨਤਕ:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        ਧਾਰਾ& get_Null_ਧਾਰਾ(ਪੂਰਨ deviceId);
        ਸਮ੍ਰਿਤੀ_Pool& get_ਸਮ੍ਰਿਤੀ_Pool(ਪੂਰਨ deviceId);

    ਨਿਜੀ:
        ਖਾਲੀ initStreams();
        ਖਾਲੀ initPools();

        std::vector<Ptr<ਧਾਰਾ> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<ਸਮ੍ਰਿਤੀ_Pool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        ਲਈ (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_ਸੈੱਟ_Device(static_cast<ਪੂਰਨ>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    ਧਾਰਾ& DefaultDeviceInitializer::get_Null_ਧਾਰਾ(ਪੂਰਨ deviceId)
    {
        Auto_ਤਾਲਾ lock(streams_mtx_);

        ਜੇ (streams_.empty())
        {
            ਪੂਰਨ device_ਗਿਣਤੀ = get_Cuda_Enabled_Device_ਗਿਣਤੀ();

            ਜੇ (device_ਗਿਣਤੀ > 0)
                streams_.resize(device_ਗਿਣਤੀ);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ਪੂਰਨ>(streams_.size()) );

        ਜੇ (streams_[deviceId].empty())
        {
            cuda_ਧਾਰਾ_t stream = ਨਿਰਾ;
            Ptr<ਧਾਰਾ::Impl> impl = makePtr<ਧਾਰਾ::Impl>(stream);
            streams_[deviceId] = Ptr<ਧਾਰਾ>(ਨਵਾਂ ਧਾਰਾ(impl));
        }

        ਵਾਪਸ *streams_[deviceId];
    }

    ਸਮ੍ਰਿਤੀ_Pool& DefaultDeviceInitializer::get_ਸਮ੍ਰਿਤੀ_Pool(ਪੂਰਨ deviceId)
    {
        Auto_ਤਾਲਾ lock(pools_mtx_);

        ਜੇ (pools_.empty())
        {
            ਪੂਰਨ device_ਗਿਣਤੀ = get_Cuda_Enabled_Device_ਗਿਣਤੀ();

            ਜੇ (device_ਗਿਣਤੀ > 0)
            {
                pools_.resize(device_ਗਿਣਤੀ);
                ਲਈ (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_ਸੈੱਟ_Device(static_cast<ਪੂਰਨ>(i));
                    pools_[i] = makePtr<ਸਮ੍ਰਿਤੀ_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ਪੂਰਨ>(pools_.size()) );

        ਸਮ੍ਰਿਤੀ_Pool* p = pools_[deviceId];
        CV_Assert(p);
        ਵਾਪਸ *p;
    }

    DefaultDeviceInitializer initializer;
}}

#ਜੇ_ਅੰਤ

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::ਧਾਰਾ::ਧਾਰਾ()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    impl_ = makePtr<Impl>();
#ਜੇ_ਅੰਤ
}

cv::cuda::ਧਾਰਾ::ਧਾਰਾ(ਅਚਲ Ptr<GpuMat::Allocator>& allocator)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(allocator);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    impl_ = makePtr<Impl>(allocator);
#ਜੇ_ਅੰਤ
}

cv::cuda::ਧਾਰਾ::ਧਾਰਾ(ਅਚਲ size_t cudaFlags)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(cudaFlags);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::ਧਾਰਾ::queryIfComplete() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cudaError_t err = cuda_ਧਾਰਾ_Query(impl_->stream);

    ਜੇ (err == cudaErrorNotReady || err == cudaSuccess)
        ਵਾਪਸ err == cudaSuccess;

    cuda_ਸੁਰੱਖਿਅਤ_Call(err);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::ਧਾਰਾ::waitForCompletion()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_ਧਾਰਾ_Synchronize(impl_->stream) );
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::ਧਾਰਾ::waitEvent(ਅਚਲ Event& event)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(event);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_ਧਾਰਾ_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#ਜੇ_ਅੰਤ
}

#ਜੇ defined(HAVE_CUDA) && (CUDART_ਸੰਸਕਰਣ >= 5000)

ਨਾਮਾਕਾਸ਼
{
    ਬਣਤਰ Callback_ਅੰਕੜੇ
    {
        ਧਾਰਾ::ਧਾਰਾ_Callback callback;
        ਖਾਲੀ* user_ਅੰਕੜੇ;

        Callback_ਅੰਕੜੇ(ਧਾਰਾ::ਧਾਰਾ_Callback callback_, ਖਾਲੀ* user_ਅੰਕੜੇ_2) : callback(callback_), user_ਅੰਕੜੇ(user_ਅੰਕੜੇ_2) {}
    };

    ਖਾਲੀ CUDART_CB cuda_ਧਾਰਾ_Callback(cuda_ਧਾਰਾ_t, cudaError_t ਸਥਿਤੀ, ਖਾਲੀ* user_ਅੰਕੜੇ)
    {
        Callback_ਅੰਕੜੇ* ਅੰਕੜੇ = reinterpret_cast<Callback_ਅੰਕੜੇ*>(user_ਅੰਕੜੇ);
        ਅੰਕੜੇ->callback(static_cast<ਪੂਰਨ>(ਸਥਿਤੀ), ਅੰਕੜੇ->user_ਅੰਕੜੇ);
        ਮਿਟਾਓ ਅੰਕੜੇ;
    }
}

#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::cuda::ਧਾਰਾ::ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Host_Callback(ਧਾਰਾ_Callback callback, ਖਾਲੀ* user_ਅੰਕੜੇ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(callback);
    CV_ਅਣਵਰਤਿਆ(user_ਅੰਕੜੇ);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    #ਜੇ CUDART_ਸੰਸਕਰਣ < 5000
        CV_ਅਣਵਰਤਿਆ(callback);
        CV_ਅਣਵਰਤਿਆ(user_ਅੰਕੜੇ);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #ਨਹੀਂਤਾਂ
        Callback_ਅੰਕੜੇ* ਅੰਕੜੇ = ਨਵਾਂ Callback_ਅੰਕੜੇ(callback, user_ਅੰਕੜੇ);

        cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_ਧਾਰਾ_ਜੋੜੋ_Callback(impl_->stream, cuda_ਧਾਰਾ_Callback, ਅੰਕੜੇ, 0) );
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
}

ਧਾਰਾ& cv::cuda::ਧਾਰਾ::Null()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ deviceId = getDevice();
    ਵਾਪਸ initializer.get_Null_ਧਾਰਾ(deviceId);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ* cv::cuda::ਧਾਰਾ::cudaPtr() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ impl_->stream;
#ਜੇ_ਅੰਤ
}

cv::cuda::ਧਾਰਾ::ਸੰਚਾਲਕ bool_type() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ (impl_->stream != 0) ? &ਧਾਰਾ::this_type_does_not_support_comparisons : 0;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

cuda_ਧਾਰਾ_t cv::cuda::ਧਾਰਾ_Accessor::get_ਧਾਰਾ(ਅਚਲ ਧਾਰਾ& stream)
{
    ਵਾਪਸ stream.impl_->stream;
}

ਧਾਰਾ cv::cuda::ਧਾਰਾ_Accessor::wrap_ਧਾਰਾ(cuda_ਧਾਰਾ_t stream)
{
    ਵਾਪਸ ਧਾਰਾ(makePtr<ਧਾਰਾ::Impl>(stream));
}

#ਜੇ_ਅੰਤ

ਧਾਰਾ cv::cuda::wrap_ਧਾਰਾ(size_t cuda_ਧਾਰਾ_ਸਮ੍ਰਿਤੀ_ਪਤਾ) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(cuda_ਧਾਰਾ_ਸਮ੍ਰਿਤੀ_ਪਤਾ);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ cv::cuda::ਧਾਰਾ_Accessor::wrap_ਧਾਰਾ(reinterpret_cast<cuda_ਧਾਰਾ_t>(cuda_ਧਾਰਾ_ਸਮ੍ਰਿਤੀ_ਪਤਾ));
#ਜੇ_ਅੰਤ
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼
{
    ਤਾਰਕਿਕ enable_ਸਮ੍ਰਿਤੀ_Pool = ਝੂਠ;

    ਵਰਗ ਢੇਰ_Allocator : ਜਨਤਕ GpuMat::Allocator
    {
    ਜਨਤਕ:
        ਸਪਸ਼ਟ ਢੇਰ_Allocator(cuda_ਧਾਰਾ_t stream);
        ~ਢੇਰ_Allocator();

        ਤਾਰਕਿਕ allocate(GpuMat* mat, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, size_t elem_ਮਾਪ) CV_OVERRIDE;
        ਖਾਲੀ ਮੁਕਤ(GpuMat* mat) CV_OVERRIDE;

    ਨਿਜੀ:
        ਢੇਰ_Allocator(ਅਚਲ ਢੇਰ_Allocator&);
        ਢੇਰ_Allocator& ਸੰਚਾਲਕ =(ਅਚਲ ਢੇਰ_Allocator&);

        cuda_ਧਾਰਾ_t ਧਾਰਾ_3;
        ਸਮ੍ਰਿਤੀ_ਢੇਰ* mem_ਢੇਰ_2;
        size_t alignment_;
    };

    ਢੇਰ_Allocator::ਢੇਰ_Allocator(cuda_ਧਾਰਾ_t stream) : ਧਾਰਾ_3(stream), mem_ਢੇਰ_2(0)
    {
        ਜੇ (enable_ਸਮ੍ਰਿਤੀ_Pool)
        {
            ਅਚਲ ਪੂਰਨ deviceId = getDevice();
            mem_ਢੇਰ_2 = initializer.get_ਸਮ੍ਰਿਤੀ_Pool(deviceId).get_Free_Mem_ਢੇਰ();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ਢੇਰ_Allocator::~ਢੇਰ_Allocator()
    {
        ਜੇ (mem_ਢੇਰ_2 != 0)
        {
            cuda_ਧਾਰਾ_Synchronize(ਧਾਰਾ_3);
            mem_ਢੇਰ_2->pool->return_Mem_ਢੇਰ(mem_ਢੇਰ_2);
        }
    }

    size_t align_ਉੱਪਰ(size_t what, size_t alignment)
    {
        size_t align_ਨਕਾਬ = alignment-1;
        size_t inverse_Align_ਨਕਾਬ = ~align_ਨਕਾਬ;
        size_t res = (what + align_ਨਕਾਬ) & inverse_Align_ਨਕਾਬ;
        ਵਾਪਸ res;
    }

    ਤਾਰਕਿਕ ਢੇਰ_Allocator::allocate(GpuMat* mat, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, size_t elem_ਮਾਪ)
    {
        ਜੇ (mem_ਢੇਰ_2 == 0)
            ਵਾਪਸ ਝੂਠ;

        size_t pitch, mem_ਮਾਪ;

        ਜੇ (ਪੰਕਤੀਆਂ > 1 && ਖਾਨੇ_ਗਿਣਤੀ > 1)
        {
            pitch = align_ਉੱਪਰ(ਖਾਨੇ_ਗਿਣਤੀ * elem_ਮਾਪ, alignment_);
            mem_ਮਾਪ = pitch * ਪੰਕਤੀਆਂ;
        }
        ਨਹੀਂਤਾਂ
        {
            // Single row or single column must be continuous
            pitch = elem_ਮਾਪ * ਖਾਨੇ_ਗਿਣਤੀ;
            mem_ਮਾਪ = align_ਉੱਪਰ(elem_ਮਾਪ * ਖਾਨੇ_ਗਿਣਤੀ * ਪੰਕਤੀਆਂ, 64);
        }

        uchar* ptr = mem_ਢੇਰ_2->request_ਸਮ੍ਰਿਤੀ(mem_ਮਾਪ);

        ਜੇ (ptr == 0)
            ਵਾਪਸ ਝੂਠ;

        mat->ਅੰਕੜੇ = ptr;
        mat->ਕਦਮ = pitch;
        mat->refcount = (ਪੂਰਨ*) fastMalloc(ਆਕਾਰ(ਪੂਰਨ));

        ਵਾਪਸ ਸੱਚ;
    }

    ਖਾਲੀ ਢੇਰ_Allocator::ਮੁਕਤ(GpuMat* mat)
    {
        ਜੇ (mem_ਢੇਰ_2 == 0)
            ਵਾਪਸ;

        mem_ਢੇਰ_2->return_ਸਮ੍ਰਿਤੀ(mat->datastart);
        fastFree(mat->refcount);
    }
}

#ਜੇ_ਅੰਤ

/////////////////////////////////////////////////////////////
/// BufferPool

ਖਾਲੀ cv::cuda::ਸੈੱਟ_ਬਫਰ_Pool_Usage(ਤਾਰਕਿਕ on)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(on);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    enable_ਸਮ੍ਰਿਤੀ_Pool = on;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::ਸੈੱਟ_ਬਫਰ_Pool_Config(ਪੂਰਨ deviceId, size_t ਢੇਰ_ਮਾਪ, ਪੂਰਨ ਢੇਰ_ਗਿਣਤੀ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(deviceId);
    CV_ਅਣਵਰਤਿਆ(ਢੇਰ_ਮਾਪ);
    CV_ਅਣਵਰਤਿਆ(ਢੇਰ_ਗਿਣਤੀ);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ currentDevice = getDevice();

    ਜੇ (deviceId >= 0)
    {
        ਸੈੱਟ_Device(deviceId);
        initializer.get_ਸਮ੍ਰਿਤੀ_Pool(deviceId).initialize(ਢੇਰ_ਮਾਪ, ਢੇਰ_ਗਿਣਤੀ);
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ ਪੂਰਨ device_ਗਿਣਤੀ = get_Cuda_Enabled_Device_ਗਿਣਤੀ();

        ਲਈ (deviceId = 0; deviceId < device_ਗਿਣਤੀ; ++deviceId)
        {
            ਸੈੱਟ_Device(deviceId);
            initializer.get_ਸਮ੍ਰਿਤੀ_Pool(deviceId).initialize(ਢੇਰ_ਮਾਪ, ਢੇਰ_ਗਿਣਤੀ);
        }
    }

    ਸੈੱਟ_Device(currentDevice);
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
cv::cuda::ਬਫਰ_Pool::ਬਫਰ_Pool(ਧਾਰਾ& stream)
{
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
}
#ਨਹੀਂਤਾਂ
cv::cuda::ਬਫਰ_Pool::ਬਫਰ_Pool(ਧਾਰਾ& stream) : allocator_(stream.impl_->allocator)
{
}
#ਜੇ_ਅੰਤ

GpuMat cv::cuda::ਬਫਰ_Pool::get_ਬਫਰ(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(ਪੰਕਤੀਆਂ);
    CV_ਅਣਵਰਤਿਆ(ਖਾਨੇ_ਗਿਣਤੀ);
    CV_ਅਣਵਰਤਿਆ(type);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    GpuMat buf(allocator_);
    buf.create(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type);
    ਵਾਪਸ buf;
#ਜੇ_ਅੰਤ
}


////////////////////////////////////////////////////////////////
// Event

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਵਰਗ cv::cuda::Event::Impl
{
ਜਨਤਕ:
    Impl(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)
    {
        throw_no_cuda();
    }
};

#ਨਹੀਂਤਾਂ

ਵਰਗ cv::cuda::Event::Impl
{
ਜਨਤਕ:
    cudaEvent_t event;
    ਤਾਰਕਿਕ ownEvent;

    ਸਪਸ਼ਟ Impl(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags);
    ਸਪਸ਼ਟ Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags) : event(0), ownEvent(ਝੂਠ)
{
    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = ਸੱਚ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(ਝੂਠ)
{
}

cv::cuda::Event::Impl::~Impl()
{
    ਜੇ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ਅਚਲ Event& event)
{
    ਵਾਪਸ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ਵਾਪਸ Event(makePtr<Event::Impl>(event));
}

#ਜੇ_ਅੰਤ

cv::cuda::Event::Event(ਅਚਲ Event::CreateFlags flags)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(flags);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    impl_ = makePtr<Impl>(flags);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::Event::ਰਿਕਾਰਡ(ਧਾਰਾ& stream)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(stream);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Event_ਰਿਕਾਰਡ(impl_->event, ਧਾਰਾ_Accessor::get_ਧਾਰਾ(stream)) );
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ cv::cuda::Event::queryIfComplete() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cudaError_t err = cudaEventQuery(impl_->event);

    ਜੇ (err == cudaErrorNotReady || err == cudaSuccess)
        ਵਾਪਸ err == cudaSuccess;

    cuda_ਸੁਰੱਖਿਅਤ_Call(err);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::Event::waitForCompletion()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaEventSynchronize(impl_->event) );
#ਜੇ_ਅੰਤ
}

ਦਸ਼ਮਲਵ cv::cuda::Event::elapsed_ਸਮਾਂ(ਅਚਲ Event& ਸ਼ੁਰੂ, ਅਚਲ Event& end)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(ਸ਼ੁਰੂ);
    CV_ਅਣਵਰਤਿਆ(end);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਦਸ਼ਮਲਵ ms;
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Event_Elapsed_ਸਮਾਂ(&ms, ਸ਼ੁਰੂ.impl_->event, end.impl_->event) );
    ਵਾਪਸ ms;
#ਜੇ_ਅੰਤ
}
