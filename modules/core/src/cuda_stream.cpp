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

ಬಳಸಿ ನಾಮಾಕಾಶ cv;
ಬಳಸಿ ನಾಮಾಕಾಶ cv::cuda;

#ಆದರೆ defined(_MSC_VER)
#ಸೂಚನೆ warning(disable : 4702)  // unreachable code
#ಆದರೆ_ಅಂತ್ಯ

/////////////////////////////////////////////////////////////
/// MemoryStack

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ವರ್ಗ ಸ್ಮರಣೆ_Pool;

    ವರ್ಗ ಸ್ಮರಣೆ_ರಾಶಿ
    {
    ಸಾರ್ವಜನಿಕ:
        uchar* request_ಸ್ಮರಣೆ(size_t size);
        ಶೂನ್ಯ return_ಸ್ಮರಣೆ(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        ತಾರ್ಕಿಕ isFree;
        ಸ್ಮರಣೆ_Pool* pool;

    #ಆದರೆ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #ಆದರೆ_ಅಂತ್ಯ
    };

    uchar* ಸ್ಮರಣೆ_ರಾಶಿ::request_ಸ್ಮರಣೆ(size_t size)
    {
        ಸ್ಥಿರಾಂಕ size_t freeMem = dataend - tip;

        ಆದರೆ (size > freeMem)
            ಹಿಂತಿರುಗಿ 0;

        uchar* ptr = tip;

        tip += size;

    #ಆದರೆ !defined(NDEBUG)
        allocations.push_back(size);
    #ಆದರೆ_ಅಂತ್ಯ

        ಹಿಂತಿರುಗಿ ptr;
    }

    ಶೂನ್ಯ ಸ್ಮರಣೆ_ರಾಶಿ::return_ಸ್ಮರಣೆ(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #ಆದರೆ !defined(NDEBUG)
        ಸ್ಥಿರಾಂಕ size_t alloc_ಗಾತ್ರ = tip - ptr;
        CV_Assert( alloc_ಗಾತ್ರ == allocations.back() );
        allocations.pop_back();
    #ಆದರೆ_ಅಂತ್ಯ

        tip = ptr;
    }
}

#ಆದರೆ_ಅಂತ್ಯ

/////////////////////////////////////////////////////////////
/// MemoryPool

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ವರ್ಗ ಸ್ಮರಣೆ_Pool
    {
    ಸಾರ್ವಜನಿಕ:
        ಸ್ಮರಣೆ_Pool();
        ~ಸ್ಮರಣೆ_Pool() { release(); }

        ಶೂನ್ಯ initialize(size_t ರಾಶಿ_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ ರಾಶಿ_ಎಣಿಕೆ);
        ಶೂನ್ಯ release();

        ಸ್ಮರಣೆ_ರಾಶಿ* get_Free_Mem_ರಾಶಿ();
        ಶೂನ್ಯ return_Mem_ರಾಶಿ(ಸ್ಮರಣೆ_ರಾಶಿ* mem_ರಾಶಿ);

    ಖಾಸಗಿ:
        ಶೂನ್ಯ initilizeImpl();

        Mutex mtx_;

        ತಾರ್ಕಿಕ initialized_;
        size_t ರಾಶಿ_ಗಾತ್ರ_2;
        ಪೂರ್ಣಾಂಕ ರಾಶಿ_ಎಣಿಕೆ_2;

        uchar* mem_;

        std::vector<ಸ್ಮರಣೆ_ರಾಶಿ> stacks_;

        ಸ್ಮರಣೆ_Pool(ಸ್ಥಿರಾಂಕ ಸ್ಮರಣೆ_Pool&); //= delete;
    };

    ಸ್ಮರಣೆ_Pool::ಸ್ಮರಣೆ_Pool() : initialized_(ಸುಳ್ಳು), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ರಾಶಿ_ಗಾತ್ರ_2 = 10 * 1024 * 1024;
        ರಾಶಿ_ಎಣಿಕೆ_2 = 5;
    }

    ಶೂನ್ಯ ಸ್ಮರಣೆ_Pool::initialize(size_t ರಾಶಿ_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ ರಾಶಿ_ಎಣಿಕೆ)
    {
        Auto_ಬೀಗ lock(mtx_);

        release();

        ರಾಶಿ_ಗಾತ್ರ_2 = ರಾಶಿ_ಗಾತ್ರ;
        ರಾಶಿ_ಎಣಿಕೆ_2 = ರಾಶಿ_ಎಣಿಕೆ;

        initilizeImpl();
    }

    ಶೂನ್ಯ ಸ್ಮರಣೆ_Pool::initilizeImpl()
    {
        ಸ್ಥಿರಾಂಕ size_t ಒಟ್ಟು_ಗಾತ್ರ = ರಾಶಿ_ಗಾತ್ರ_2 * ರಾಶಿ_ಎಣಿಕೆ_2;

        ಆದರೆ (ಒಟ್ಟು_ಗಾತ್ರ > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, ಒಟ್ಟು_ಗಾತ್ರ);
            ಆದರೆ (err != cudaSuccess)
                ಹಿಂತಿರುಗಿ;

            stacks_.resize(ರಾಶಿ_ಎಣಿಕೆ_2);

            uchar* ptr = mem_;

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ರಾಶಿ_ಗಾತ್ರ_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = ನಿಜ;
                stacks_[i].pool = ಇದು;

                ptr += ರಾಶಿ_ಗಾತ್ರ_2;
            }

            initialized_ = ನಿಜ;
        }
    }

    ಶೂನ್ಯ ಸ್ಮರಣೆ_Pool::release()
    {
        ಆದರೆ (mem_)
        {
#ಆದರೆ !defined(NDEBUG)
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#ಆದರೆ_ಅಂತ್ಯ

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = ಸುಳ್ಳು;
        }
    }

    ಸ್ಮರಣೆ_ರಾಶಿ* ಸ್ಮರಣೆ_Pool::get_Free_Mem_ರಾಶಿ()
    {
        Auto_ಬೀಗ lock(mtx_);

        ಆದರೆ (!initialized_)
            initilizeImpl();

        ಆದರೆ (!mem_)
            ಹಿಂತಿರುಗಿ 0;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
        {
            ಆದರೆ (stacks_[i].isFree)
            {
                stacks_[i].isFree = ಸುಳ್ಳು;
                ಹಿಂತಿರುಗಿ &stacks_[i];
            }
        }

        ಹಿಂತಿರುಗಿ 0;
    }

    ಶೂನ್ಯ ಸ್ಮರಣೆ_Pool::return_Mem_ರಾಶಿ(ಸ್ಮರಣೆ_ರಾಶಿ* mem_ರಾಶಿ)
    {
        Auto_ಬೀಗ lock(mtx_);

        CV_DbgAssert( !mem_ರಾಶಿ->isFree );

#ಆದರೆ !defined(NDEBUG)
        ತಾರ್ಕಿಕ ಸಿಕ್ಕಿತು = ಸುಳ್ಳು;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ರಾಶಿ_ಎಣಿಕೆ_2; ++i)
        {
            ಆದರೆ (mem_ರಾಶಿ == &stacks_[i])
            {
                ಸಿಕ್ಕಿತು = ನಿಜ;
                ನಿಲ್ಲಿಸು;
            }
        }
        CV_DbgAssert( ಸಿಕ್ಕಿತು );
#ಆದರೆ_ಅಂತ್ಯ

        CV_DbgAssert( mem_ರಾಶಿ->tip == mem_ರಾಶಿ->datastart );

        mem_ರಾಶಿ->isFree = ನಿಜ;
    }
}

#ಆದರೆ_ಅಂತ್ಯ

////////////////////////////////////////////////////////////////
/// Stream::Impl

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA

ವರ್ಗ cv::cuda::ಹರಿವು::Impl
{
ಸಾರ್ವಜನಿಕ:
    Impl(ಶೂನ್ಯ* ptr = 0)
    {
        CV_ಬಳಸದ(ptr);
        throw_no_cuda();
    }
};

#ಇಲ್ಲದಿದ್ದರೆ

ನಾಮಾಕಾಶ
{
    ವರ್ಗ ರಾಶಿ_Allocator;
}

ವರ್ಗ cv::cuda::ಹರಿವು::Impl
{
ಸಾರ್ವಜನಿಕ:
    cuda_ಹರಿವು_t stream;
    ತಾರ್ಕಿಕ own_ಹರಿವು;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ಸ್ಥಿರಾಂಕ Ptr<GpuMat::Allocator>& allocator);
    Impl(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cudaFlags);
    ಸ್ಪಷ್ಟ Impl(cuda_ಹರಿವು_t stream);

    ~Impl();
};

cv::cuda::ಹರಿವು::Impl::Impl() : stream(0), own_ಹರಿವು(ಸುಳ್ಳು)
{
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_ಹರಿವು_Create(&stream) );
    own_ಹರಿವು = ನಿಜ;

    allocator = makePtr<ರಾಶಿ_Allocator>(stream);
}

cv::cuda::ಹರಿವು::Impl::Impl(ಸ್ಥಿರಾಂಕ Ptr<GpuMat::Allocator>& allocator) : stream(0), own_ಹರಿವು(ಸುಳ್ಳು), allocator(allocator)
{
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_ಹರಿವು_Create(&stream) );
    own_ಹರಿವು = ನಿಜ;
}

cv::cuda::ಹರಿವು::Impl::Impl(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cudaFlags) : stream(0), own_ಹರಿವು(ಸುಳ್ಳು)
{
    cuda_ಸುರಕ್ಷಿತ_Call(cuda_ಹರಿವು_Create_With_Flags(&stream, cudaFlags));
    own_ಹರಿವು = ನಿಜ;
    allocator = makePtr<ರಾಶಿ_Allocator>(stream);
}

cv::cuda::ಹರಿವು::Impl::Impl(cuda_ಹರಿವು_t ಹರಿವು_3) : stream(ಹರಿವು_3), own_ಹರಿವು(ಸುಳ್ಳು)
{
    allocator = makePtr<ರಾಶಿ_Allocator>(stream);
}

cv::cuda::ಹರಿವು::Impl::~Impl()
{
    allocator.release();

    ಆದರೆ (stream && own_ಹರಿವು)
    {
        cuda_ಹರಿವು_Destroy(stream);
    }
}

#ಆದರೆ_ಅಂತ್ಯ

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ cuda
{
    ವರ್ಗ DefaultDeviceInitializer
    {
    ಸಾರ್ವಜನಿಕ:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        ಹರಿವು& get_Null_ಹರಿವು(ಪೂರ್ಣಾಂಕ deviceId);
        ಸ್ಮರಣೆ_Pool& get_ಸ್ಮರಣೆ_Pool(ಪೂರ್ಣಾಂಕ deviceId);

    ಖಾಸಗಿ:
        ಶೂನ್ಯ initStreams();
        ಶೂನ್ಯ initPools();

        std::vector<Ptr<ಹರಿವು> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<ಸ್ಮರಣೆ_Pool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        ಗಾಗಿ (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_ಗಣ_Device(static_cast<ಪೂರ್ಣಾಂಕ>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    ಹರಿವು& DefaultDeviceInitializer::get_Null_ಹರಿವು(ಪೂರ್ಣಾಂಕ deviceId)
    {
        Auto_ಬೀಗ lock(streams_mtx_);

        ಆದರೆ (streams_.empty())
        {
            ಪೂರ್ಣಾಂಕ device_ಎಣಿಕೆ = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

            ಆದರೆ (device_ಎಣಿಕೆ > 0)
                streams_.resize(device_ಎಣಿಕೆ);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ಪೂರ್ಣಾಂಕ>(streams_.size()) );

        ಆದರೆ (streams_[deviceId].empty())
        {
            cuda_ಹರಿವು_t stream = ಖಾಲಿ;
            Ptr<ಹರಿವು::Impl> impl = makePtr<ಹರಿವು::Impl>(stream);
            streams_[deviceId] = Ptr<ಹರಿವು>(ಹೊಸ ಹರಿವು(impl));
        }

        ಹಿಂತಿರುಗಿ *streams_[deviceId];
    }

    ಸ್ಮರಣೆ_Pool& DefaultDeviceInitializer::get_ಸ್ಮರಣೆ_Pool(ಪೂರ್ಣಾಂಕ deviceId)
    {
        Auto_ಬೀಗ lock(pools_mtx_);

        ಆದರೆ (pools_.empty())
        {
            ಪೂರ್ಣಾಂಕ device_ಎಣಿಕೆ = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

            ಆದರೆ (device_ಎಣಿಕೆ > 0)
            {
                pools_.resize(device_ಎಣಿಕೆ);
                ಗಾಗಿ (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_ಗಣ_Device(static_cast<ಪೂರ್ಣಾಂಕ>(i));
                    pools_[i] = makePtr<ಸ್ಮರಣೆ_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<ಪೂರ್ಣಾಂಕ>(pools_.size()) );

        ಸ್ಮರಣೆ_Pool* p = pools_[deviceId];
        CV_Assert(p);
        ಹಿಂತಿರುಗಿ *p;
    }

    DefaultDeviceInitializer initializer;
}}

#ಆದರೆ_ಅಂತ್ಯ

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::ಹರಿವು::ಹರಿವು()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = makePtr<Impl>();
#ಆದರೆ_ಅಂತ್ಯ
}

cv::cuda::ಹರಿವು::ಹರಿವು(ಸ್ಥಿರಾಂಕ Ptr<GpuMat::Allocator>& allocator)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(allocator);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = makePtr<Impl>(allocator);
#ಆದರೆ_ಅಂತ್ಯ
}

cv::cuda::ಹರಿವು::ಹರಿವು(ಸ್ಥಿರಾಂಕ size_t cudaFlags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(cudaFlags);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::ಹರಿವು::queryIfComplete() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cudaError_t err = cuda_ಹರಿವು_Query(impl_->stream);

    ಆದರೆ (err == cudaErrorNotReady || err == cudaSuccess)
        ಹಿಂತಿರುಗಿ err == cudaSuccess;

    cuda_ಸುರಕ್ಷಿತ_Call(err);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::ಹರಿವು::waitForCompletion()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_ಹರಿವು_Synchronize(impl_->stream) );
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::ಹರಿವು::waitEvent(ಸ್ಥಿರಾಂಕ Event& event)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(event);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_ಹರಿವು_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ defined(HAVE_CUDA) && (CUDART_ಆವೃತ್ತಿ >= 5000)

ನಾಮಾಕಾಶ
{
    ರಚನೆ Callback_ದತ್ತಾಂಶ
    {
        ಹರಿವು::ಹರಿವು_Callback callback;
        ಶೂನ್ಯ* user_ದತ್ತಾಂಶ;

        Callback_ದತ್ತಾಂಶ(ಹರಿವು::ಹರಿವು_Callback callback_, ಶೂನ್ಯ* user_ದತ್ತಾಂಶ_2) : callback(callback_), user_ದತ್ತಾಂಶ(user_ದತ್ತಾಂಶ_2) {}
    };

    ಶೂನ್ಯ CUDART_CB cuda_ಹರಿವು_Callback(cuda_ಹರಿವು_t, cudaError_t ಸ್ಥಿತಿ_2, ಶೂನ್ಯ* user_ದತ್ತಾಂಶ)
    {
        Callback_ದತ್ತಾಂಶ* ದತ್ತಾಂಶ = reinterpret_cast<Callback_ದತ್ತಾಂಶ*>(user_ದತ್ತಾಂಶ);
        ದತ್ತಾಂಶ->callback(static_cast<ಪೂರ್ಣಾಂಕ>(ಸ್ಥಿತಿ_2), ದತ್ತಾಂಶ->user_ದತ್ತಾಂಶ);
        ಅಳಿಸು ದತ್ತಾಂಶ;
    }
}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::cuda::ಹರಿವು::ಸರತಿಗೆ_ಸೇರಿಸು_Host_Callback(ಹರಿವು_Callback callback, ಶೂನ್ಯ* user_ದತ್ತಾಂಶ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(callback);
    CV_ಬಳಸದ(user_ದತ್ತಾಂಶ);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    #ಆದರೆ CUDART_ಆವೃತ್ತಿ < 5000
        CV_ಬಳಸದ(callback);
        CV_ಬಳಸದ(user_ದತ್ತಾಂಶ);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #ಇಲ್ಲದಿದ್ದರೆ
        Callback_ದತ್ತಾಂಶ* ದತ್ತಾಂಶ = ಹೊಸ Callback_ದತ್ತಾಂಶ(callback, user_ದತ್ತಾಂಶ);

        cuda_ಸುರಕ್ಷಿತ_Call( cuda_ಹರಿವು_ಕೂಡಿಸು_Callback(impl_->stream, cuda_ಹರಿವು_Callback, ದತ್ತಾಂಶ, 0) );
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
}

ಹರಿವು& cv::cuda::ಹರಿವು::Null()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ deviceId = getDevice();
    ಹಿಂತಿರುಗಿ initializer.get_Null_ಹರಿವು(deviceId);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ* cv::cuda::ಹರಿವು::cudaPtr() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    ಹಿಂತಿರುಗಿ ಶೂನ್ಯಸೂಚಕ;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ impl_->stream;
#ಆದರೆ_ಅಂತ್ಯ
}

cv::cuda::ಹರಿವು::ಕ್ರಿಯಾಕಾರಕ bool_type() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ (impl_->stream != 0) ? &ಹರಿವು::this_type_does_not_support_comparisons : 0;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

cuda_ಹರಿವು_t cv::cuda::ಹರಿವು_Accessor::get_ಹರಿವು(ಸ್ಥಿರಾಂಕ ಹರಿವು& stream)
{
    ಹಿಂತಿರುಗಿ stream.impl_->stream;
}

ಹರಿವು cv::cuda::ಹರಿವು_Accessor::wrap_ಹರಿವು(cuda_ಹರಿವು_t stream)
{
    ಹಿಂತಿರುಗಿ ಹರಿವು(makePtr<ಹರಿವು::Impl>(stream));
}

#ಆದರೆ_ಅಂತ್ಯ

ಹರಿವು cv::cuda::wrap_ಹರಿವು(size_t cuda_ಹರಿವು_ಸ್ಮರಣೆ_ವಿಳಾಸ) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(cuda_ಹರಿವು_ಸ್ಮರಣೆ_ವಿಳಾಸ);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ cv::cuda::ಹರಿವು_Accessor::wrap_ಹರಿವು(reinterpret_cast<cuda_ಹರಿವು_t>(cuda_ಹರಿವು_ಸ್ಮರಣೆ_ವಿಳಾಸ));
#ಆದರೆ_ಅಂತ್ಯ
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ
{
    ತಾರ್ಕಿಕ enable_ಸ್ಮರಣೆ_Pool = ಸುಳ್ಳು;

    ವರ್ಗ ರಾಶಿ_Allocator : ಸಾರ್ವಜನಿಕ GpuMat::Allocator
    {
    ಸಾರ್ವಜನಿಕ:
        ಸ್ಪಷ್ಟ ರಾಶಿ_Allocator(cuda_ಹರಿವು_t stream);
        ~ರಾಶಿ_Allocator();

        ತಾರ್ಕಿಕ allocate(GpuMat* mat, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, size_t elem_ಗಾತ್ರ) CV_OVERRIDE;
        ಶೂನ್ಯ ಬಿಡುಗಡೆ(GpuMat* mat) CV_OVERRIDE;

    ಖಾಸಗಿ:
        ರಾಶಿ_Allocator(ಸ್ಥಿರಾಂಕ ರಾಶಿ_Allocator&);
        ರಾಶಿ_Allocator& ಕ್ರಿಯಾಕಾರಕ =(ಸ್ಥಿರಾಂಕ ರಾಶಿ_Allocator&);

        cuda_ಹರಿವು_t ಹರಿವು_3;
        ಸ್ಮರಣೆ_ರಾಶಿ* mem_ರಾಶಿ_2;
        size_t alignment_;
    };

    ರಾಶಿ_Allocator::ರಾಶಿ_Allocator(cuda_ಹರಿವು_t stream) : ಹರಿವು_3(stream), mem_ರಾಶಿ_2(0)
    {
        ಆದರೆ (enable_ಸ್ಮರಣೆ_Pool)
        {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ deviceId = getDevice();
            mem_ರಾಶಿ_2 = initializer.get_ಸ್ಮರಣೆ_Pool(deviceId).get_Free_Mem_ರಾಶಿ();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ರಾಶಿ_Allocator::~ರಾಶಿ_Allocator()
    {
        ಆದರೆ (mem_ರಾಶಿ_2 != 0)
        {
            cuda_ಹರಿವು_Synchronize(ಹರಿವು_3);
            mem_ರಾಶಿ_2->pool->return_Mem_ರಾಶಿ(mem_ರಾಶಿ_2);
        }
    }

    size_t align_ಮೇಲೆ(size_t what, size_t alignment)
    {
        size_t align_ಮುಖವಾಡ = alignment-1;
        size_t inverse_Align_ಮುಖವಾಡ = ~align_ಮುಖವಾಡ;
        size_t res = (what + align_ಮುಖವಾಡ) & inverse_Align_ಮುಖವಾಡ;
        ಹಿಂತಿರುಗಿ res;
    }

    ತಾರ್ಕಿಕ ರಾಶಿ_Allocator::allocate(GpuMat* mat, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, size_t elem_ಗಾತ್ರ)
    {
        ಆದರೆ (mem_ರಾಶಿ_2 == 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        size_t pitch, mem_ಗಾತ್ರ;

        ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು > 1 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ > 1)
        {
            pitch = align_ಮೇಲೆ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * elem_ಗಾತ್ರ, alignment_);
            mem_ಗಾತ್ರ = pitch * ಅಡ್ಡಸಾಲುಗಳು;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            // Single row or single column must be continuous
            pitch = elem_ಗಾತ್ರ * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
            mem_ಗಾತ್ರ = align_ಮೇಲೆ(elem_ಗಾತ್ರ * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * ಅಡ್ಡಸಾಲುಗಳು, 64);
        }

        uchar* ptr = mem_ರಾಶಿ_2->request_ಸ್ಮರಣೆ(mem_ಗಾತ್ರ);

        ಆದರೆ (ptr == 0)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

        mat->ದತ್ತಾಂಶ = ptr;
        mat->ಹೆಜ್ಜೆ = pitch;
        mat->refcount = (ಪೂರ್ಣಾಂಕ*) fastMalloc(ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ));

        ಹಿಂತಿರುಗಿ ನಿಜ;
    }

    ಶೂನ್ಯ ರಾಶಿ_Allocator::ಬಿಡುಗಡೆ(GpuMat* mat)
    {
        ಆದರೆ (mem_ರಾಶಿ_2 == 0)
            ಹಿಂತಿರುಗಿ;

        mem_ರಾಶಿ_2->return_ಸ್ಮರಣೆ(mat->datastart);
        fastFree(mat->refcount);
    }
}

#ಆದರೆ_ಅಂತ್ಯ

/////////////////////////////////////////////////////////////
/// BufferPool

ಶೂನ್ಯ cv::cuda::ಗಣ_ಬಫರ್_Pool_Usage(ತಾರ್ಕಿಕ on)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(on);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    enable_ಸ್ಮರಣೆ_Pool = on;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::ಗಣ_ಬಫರ್_Pool_Config(ಪೂರ್ಣಾಂಕ deviceId, size_t ರಾಶಿ_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ ರಾಶಿ_ಎಣಿಕೆ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(deviceId);
    CV_ಬಳಸದ(ರಾಶಿ_ಗಾತ್ರ);
    CV_ಬಳಸದ(ರಾಶಿ_ಎಣಿಕೆ);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ currentDevice = getDevice();

    ಆದರೆ (deviceId >= 0)
    {
        ಗಣ_Device(deviceId);
        initializer.get_ಸ್ಮರಣೆ_Pool(deviceId).initialize(ರಾಶಿ_ಗಾತ್ರ, ರಾಶಿ_ಎಣಿಕೆ);
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ device_ಎಣಿಕೆ = get_Cuda_Enabled_Device_ಎಣಿಕೆ();

        ಗಾಗಿ (deviceId = 0; deviceId < device_ಎಣಿಕೆ; ++deviceId)
        {
            ಗಣ_Device(deviceId);
            initializer.get_ಸ್ಮರಣೆ_Pool(deviceId).initialize(ರಾಶಿ_ಗಾತ್ರ, ರಾಶಿ_ಎಣಿಕೆ);
        }
    }

    ಗಣ_Device(currentDevice);
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
cv::cuda::ಬಫರ್_Pool::ಬಫರ್_Pool(ಹರಿವು& stream)
{
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
}
#ಇಲ್ಲದಿದ್ದರೆ
cv::cuda::ಬಫರ್_Pool::ಬಫರ್_Pool(ಹರಿವು& stream) : allocator_(stream.impl_->allocator)
{
}
#ಆದರೆ_ಅಂತ್ಯ

GpuMat cv::cuda::ಬಫರ್_Pool::get_ಬಫರ್(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ type)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(ಅಡ್ಡಸಾಲುಗಳು);
    CV_ಬಳಸದ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    CV_ಬಳಸದ(type);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    GpuMat buf(allocator_);
    buf.create(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type);
    ಹಿಂತಿರುಗಿ buf;
#ಆದರೆ_ಅಂತ್ಯ
}


////////////////////////////////////////////////////////////////
// Event

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA

ವರ್ಗ cv::cuda::Event::Impl
{
ಸಾರ್ವಜನಿಕ:
    Impl(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)
    {
        throw_no_cuda();
    }
};

#ಇಲ್ಲದಿದ್ದರೆ

ವರ್ಗ cv::cuda::Event::Impl
{
ಸಾರ್ವಜನಿಕ:
    cudaEvent_t event;
    ತಾರ್ಕಿಕ ownEvent;

    ಸ್ಪಷ್ಟ Impl(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags);
    ಸ್ಪಷ್ಟ Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags) : event(0), ownEvent(ಸುಳ್ಳು)
{
    cuda_ಸುರಕ್ಷಿತ_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = ನಿಜ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(ಸುಳ್ಳು)
{
}

cv::cuda::Event::Impl::~Impl()
{
    ಆದರೆ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ಸ್ಥಿರಾಂಕ Event& event)
{
    ಹಿಂತಿರುಗಿ event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    ಹಿಂತಿರುಗಿ Event(makePtr<Event::Impl>(event));
}

#ಆದರೆ_ಅಂತ್ಯ

cv::cuda::Event::Event(ಸ್ಥಿರಾಂಕ Event::CreateFlags flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(flags);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    impl_ = makePtr<Impl>(flags);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::Event::ದಾಖಲೆ(ಹರಿವು& stream)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(stream);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Event_ದಾಖಲೆ(impl_->event, ಹರಿವು_Accessor::get_ಹರಿವು(stream)) );
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ cv::cuda::Event::queryIfComplete() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cudaError_t err = cudaEventQuery(impl_->event);

    ಆದರೆ (err == cudaErrorNotReady || err == cudaSuccess)
        ಹಿಂತಿರುಗಿ err == cudaSuccess;

    cuda_ಸುರಕ್ಷಿತ_Call(err);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::Event::waitForCompletion()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cudaEventSynchronize(impl_->event) );
#ಆದರೆ_ಅಂತ್ಯ
}

ದಶಮಾಂಶ cv::cuda::Event::elapsed_ಸಮಯ(ಸ್ಥಿರಾಂಕ Event& ಆರಂಭ, ಸ್ಥಿರಾಂಕ Event& end)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(ಆರಂಭ);
    CV_ಬಳಸದ(end);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ದಶಮಾಂಶ ms;
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Event_Elapsed_ಸಮಯ(&ms, ಆರಂಭ.impl_->event, end.impl_->event) );
    ಹಿಂತಿರುಗಿ ms;
#ಆದರೆ_ಅಂತ್ಯ
}
