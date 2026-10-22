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

उपयुज्य नामाकाश cv;
उपयुज्य नामाकाश cv::cuda;

#यदि defined(_MSC_VER)
#निर्देशः warning(disable : 4702)  // unreachable code
#यदि_अन्तः

/////////////////////////////////////////////////////////////
/// MemoryStack

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    वर्ग स्मृतिः_Pool;

    वर्ग स्मृतिः_राशिः
    {
    सार्वजनिक:
        uchar* request_स्मृतिः(size_t size);
        शून्य return_स्मृतिः(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        तार्किक isFree;
        स्मृतिः_Pool* pool;

    #यदि !defined(NDEBUG)
        std::vector<size_t> allocations;
    #यदि_अन्तः
    };

    uchar* स्मृतिः_राशिः::request_स्मृतिः(size_t size)
    {
        ध्रुव size_t freeMem = dataend - tip;

        यदि (size > freeMem)
            प्रत्यागम 0;

        uchar* ptr = tip;

        tip += size;

    #यदि !defined(NDEBUG)
        allocations.push_back(size);
    #यदि_अन्तः

        प्रत्यागम ptr;
    }

    शून्य स्मृतिः_राशिः::return_स्मृतिः(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #यदि !defined(NDEBUG)
        ध्रुव size_t alloc_आकार = tip - ptr;
        CV_Assert( alloc_आकार == allocations.back() );
        allocations.pop_back();
    #यदि_अन्तः

        tip = ptr;
    }
}

#यदि_अन्तः

/////////////////////////////////////////////////////////////
/// MemoryPool

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    वर्ग स्मृतिः_Pool
    {
    सार्वजनिक:
        स्मृतिः_Pool();
        ~स्मृतिः_Pool() { release(); }

        शून्य initialize(size_t राशिः_आकार, पूर्णाङ्क राशिः_गणनम्);
        शून्य release();

        स्मृतिः_राशिः* get_Free_Mem_राशिः();
        शून्य return_Mem_राशिः(स्मृतिः_राशिः* mem_राशिः);

    गुप्त:
        शून्य initilizeImpl();

        Mutex mtx_;

        तार्किक initialized_;
        size_t राशिः_आकार_2;
        पूर्णाङ्क राशिः_गणनम्_2;

        uchar* mem_;

        std::vector<स्मृतिः_राशिः> stacks_;

        स्मृतिः_Pool(ध्रुव स्मृतिः_Pool&); //= delete;
    };

    स्मृतिः_Pool::स्मृतिः_Pool() : initialized_(असत्य), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        राशिः_आकार_2 = 10 * 1024 * 1024;
        राशिः_गणनम्_2 = 5;
    }

    शून्य स्मृतिः_Pool::initialize(size_t राशिः_आकार, पूर्णाङ्क राशिः_गणनम्)
    {
        Auto_तालकम् lock(mtx_);

        release();

        राशिः_आकार_2 = राशिः_आकार;
        राशिः_गणनम्_2 = राशिः_गणनम्;

        initilizeImpl();
    }

    शून्य स्मृतिः_Pool::initilizeImpl()
    {
        ध्रुव size_t योग_आकार = राशिः_आकार_2 * राशिः_गणनम्_2;

        यदि (योग_आकार > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, योग_आकार);
            यदि (err != cudaSuccess)
                प्रत्यागम;

            stacks_.resize(राशिः_गणनम्_2);

            uchar* ptr = mem_;

            प्रति (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + राशिः_आकार_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = सत्य;
                stacks_[i].pool = एतत्;

                ptr += राशिः_आकार_2;
            }

            initialized_ = सत्य;
        }
    }

    शून्य स्मृतिः_Pool::release()
    {
        यदि (mem_)
        {
#यदि !defined(NDEBUG)
            प्रति (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#यदि_अन्तः

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = असत्य;
        }
    }

    स्मृतिः_राशिः* स्मृतिः_Pool::get_Free_Mem_राशिः()
    {
        Auto_तालकम् lock(mtx_);

        यदि (!initialized_)
            initilizeImpl();

        यदि (!mem_)
            प्रत्यागम 0;

        प्रति (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
        {
            यदि (stacks_[i].isFree)
            {
                stacks_[i].isFree = असत्य;
                प्रत्यागम &stacks_[i];
            }
        }

        प्रत्यागम 0;
    }

    शून्य स्मृतिः_Pool::return_Mem_राशिः(स्मृतिः_राशिः* mem_राशिः)
    {
        Auto_तालकम् lock(mtx_);

        CV_DbgAssert( !mem_राशिः->isFree );

#यदि !defined(NDEBUG)
        तार्किक प्राप्तम् = असत्य;
        प्रति (पूर्णाङ्क i = 0; i < राशिः_गणनम्_2; ++i)
        {
            यदि (mem_राशिः == &stacks_[i])
            {
                प्राप्तम् = सत्य;
                भङ्ग;
            }
        }
        CV_DbgAssert( प्राप्तम् );
#यदि_अन्तः

        CV_DbgAssert( mem_राशिः->tip == mem_राशिः->datastart );

        mem_राशिः->isFree = सत्य;
    }
}

#यदि_अन्तः

////////////////////////////////////////////////////////////////
/// Stream::Impl

#यदि_अपरिभाषितम् HAVE_CUDA

वर्ग cv::cuda::प्रवाहः::Impl
{
सार्वजनिक:
    Impl(शून्य* ptr = 0)
    {
        CV_अप्रयुक्तम्(ptr);
        throw_no_cuda();
    }
};

#अन्यथा

नामाकाश
{
    वर्ग राशिः_Allocator;
}

वर्ग cv::cuda::प्रवाहः::Impl
{
सार्वजनिक:
    cuda_प्रवाहः_t stream;
    तार्किक own_प्रवाहः;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ध्रुव Ptr<GpuMat::Allocator>& allocator);
    Impl(ध्रुव अचिह्नित पूर्णाङ्क cudaFlags);
    स्पष्ट Impl(cuda_प्रवाहः_t stream);

    ~Impl();
};

cv::cuda::प्रवाहः::Impl::Impl() : stream(0), own_प्रवाहः(असत्य)
{
    cudaSafeCall( cuda_प्रवाहः_Create(&stream) );
    own_प्रवाहः = सत्य;

    allocator = makePtr<राशिः_Allocator>(stream);
}

cv::cuda::प्रवाहः::Impl::Impl(ध्रुव Ptr<GpuMat::Allocator>& allocator) : stream(0), own_प्रवाहः(असत्य), allocator(allocator)
{
    cudaSafeCall( cuda_प्रवाहः_Create(&stream) );
    own_प्रवाहः = सत्य;
}

cv::cuda::प्रवाहः::Impl::Impl(ध्रुव अचिह्नित पूर्णाङ्क cudaFlags) : stream(0), own_प्रवाहः(असत्य)
{
    cudaSafeCall(cuda_प्रवाहः_Create_With_Flags(&stream, cudaFlags));
    own_प्रवाहः = सत्य;
    allocator = makePtr<राशिः_Allocator>(stream);
}

cv::cuda::प्रवाहः::Impl::Impl(cuda_प्रवाहः_t प्रवाहः_3) : stream(प्रवाहः_3), own_प्रवाहः(असत्य)
{
    allocator = makePtr<राशिः_Allocator>(stream);
}

cv::cuda::प्रवाहः::Impl::~Impl()
{
    allocator.release();

    यदि (stream && own_प्रवाहः)
    {
        cuda_प्रवाहः_Destroy(stream);
    }
}

#यदि_अन्तः

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश cv { नामाकाश cuda
{
    वर्ग DefaultDeviceInitializer
    {
    सार्वजनिक:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        प्रवाहः& get_Null_प्रवाहः(पूर्णाङ्क deviceId);
        स्मृतिः_Pool& get_स्मृतिः_Pool(पूर्णाङ्क deviceId);

    गुप्त:
        शून्य initStreams();
        शून्य initPools();

        std::vector<Ptr<प्रवाहः> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<स्मृतिः_Pool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        प्रति (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_समुच्चयः_Device(static_cast<पूर्णाङ्क>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    प्रवाहः& DefaultDeviceInitializer::get_Null_प्रवाहः(पूर्णाङ्क deviceId)
    {
        Auto_तालकम् lock(streams_mtx_);

        यदि (streams_.empty())
        {
            पूर्णाङ्क device_गणनम् = get_Cuda_Enabled_Device_गणनम्();

            यदि (device_गणनम् > 0)
                streams_.resize(device_गणनम्);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णाङ्क>(streams_.size()) );

        यदि (streams_[deviceId].empty())
        {
            cuda_प्रवाहः_t stream = रिक्त;
            Ptr<प्रवाहः::Impl> impl = makePtr<प्रवाहः::Impl>(stream);
            streams_[deviceId] = Ptr<प्रवाहः>(नूतन प्रवाहः(impl));
        }

        प्रत्यागम *streams_[deviceId];
    }

    स्मृतिः_Pool& DefaultDeviceInitializer::get_स्मृतिः_Pool(पूर्णाङ्क deviceId)
    {
        Auto_तालकम् lock(pools_mtx_);

        यदि (pools_.empty())
        {
            पूर्णाङ्क device_गणनम् = get_Cuda_Enabled_Device_गणनम्();

            यदि (device_गणनम् > 0)
            {
                pools_.resize(device_गणनम्);
                प्रति (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_समुच्चयः_Device(static_cast<पूर्णाङ्क>(i));
                    pools_[i] = makePtr<स्मृतिः_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णाङ्क>(pools_.size()) );

        स्मृतिः_Pool* p = pools_[deviceId];
        CV_Assert(p);
        प्रत्यागम *p;
    }

    DefaultDeviceInitializer initializer;
}}

#यदि_अन्तः

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::प्रवाहः::प्रवाहः()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>();
#यदि_अन्तः
}

cv::cuda::प्रवाहः::प्रवाहः(ध्रुव Ptr<GpuMat::Allocator>& allocator)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(allocator);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(allocator);
#यदि_अन्तः
}

cv::cuda::प्रवाहः::प्रवाहः(ध्रुव size_t cudaFlags)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(cudaFlags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#यदि_अन्तः
}

तार्किक cv::cuda::प्रवाहः::queryIfComplete() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cuda_प्रवाहः_Query(impl_->stream);

    यदि (err == cudaErrorNotReady || err == cudaSuccess)
        प्रत्यागम err == cudaSuccess;

    cudaSafeCall(err);
    प्रत्यागम असत्य;
#यदि_अन्तः
}

शून्य cv::cuda::प्रवाहः::waitForCompletion()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cuda_प्रवाहः_Synchronize(impl_->stream) );
#यदि_अन्तः
}

शून्य cv::cuda::प्रवाहः::waitEvent(ध्रुव Event& event)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(event);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cuda_प्रवाहः_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#यदि_अन्तः
}

#यदि defined(HAVE_CUDA) && (CUDART_संस्करणम् >= 5000)

नामाकाश
{
    संरचना Callback_दत्त
    {
        प्रवाहः::प्रवाहः_Callback callback;
        शून्य* user_दत्त;

        Callback_दत्त(प्रवाहः::प्रवाहः_Callback callback_, शून्य* user_दत्त_2) : callback(callback_), user_दत्त(user_दत्त_2) {}
    };

    शून्य CUDART_CB cuda_प्रवाहः_Callback(cuda_प्रवाहः_t, cudaError_t स्थितिः, शून्य* user_दत्त)
    {
        Callback_दत्त* दत्त = reinterpret_cast<Callback_दत्त*>(user_दत्त);
        दत्त->callback(static_cast<पूर्णाङ्क>(स्थितिः), दत्त->user_दत्त);
        विलोपय दत्त;
    }
}

#यदि_अन्तः

शून्य cv::cuda::प्रवाहः::पङ्क्तौ_निक्षिप_Host_Callback(प्रवाहः_Callback callback, शून्य* user_दत्त)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(callback);
    CV_अप्रयुक्तम्(user_दत्त);
    throw_no_cuda();
#अन्यथा
    #यदि CUDART_संस्करणम् < 5000
        CV_अप्रयुक्तम्(callback);
        CV_अप्रयुक्तम्(user_दत्त);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #अन्यथा
        Callback_दत्त* दत्त = नूतन Callback_दत्त(callback, user_दत्त);

        cudaSafeCall( cuda_प्रवाहः_सङ्कलय_Callback(impl_->stream, cuda_प्रवाहः_Callback, दत्त, 0) );
    #यदि_अन्तः
#यदि_अन्तः
}

प्रवाहः& cv::cuda::प्रवाहः::Null()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    ध्रुव पूर्णाङ्क deviceId = getDevice();
    प्रत्यागम initializer.get_Null_प्रवाहः(deviceId);
#यदि_अन्तः
}

शून्य* cv::cuda::प्रवाहः::cudaPtr() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    प्रत्यागम शून्यसूचकः;
#अन्यथा
    प्रत्यागम impl_->stream;
#यदि_अन्तः
}

cv::cuda::प्रवाहः::संक्रियकः bool_type() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    प्रत्यागम 0;
#अन्यथा
    प्रत्यागम (impl_->stream != 0) ? &प्रवाहः::this_type_does_not_support_comparisons : 0;
#यदि_अन्तः
}

#यदि_परिभाषितम् HAVE_CUDA

cuda_प्रवाहः_t cv::cuda::प्रवाहः_Accessor::get_प्रवाहः(ध्रुव प्रवाहः& stream)
{
    प्रत्यागम stream.impl_->stream;
}

प्रवाहः cv::cuda::प्रवाहः_Accessor::wrap_प्रवाहः(cuda_प्रवाहः_t stream)
{
    प्रत्यागम प्रवाहः(makePtr<प्रवाहः::Impl>(stream));
}

#यदि_अन्तः

प्रवाहः cv::cuda::wrap_प्रवाहः(size_t cuda_प्रवाहः_स्मृतिः_स्थानाङ्कः) {
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(cuda_प्रवाहः_स्मृतिः_स्थानाङ्कः);
    throw_no_cuda();
#अन्यथा
    प्रत्यागम cv::cuda::प्रवाहः_Accessor::wrap_प्रवाहः(reinterpret_cast<cuda_प्रवाहः_t>(cuda_प्रवाहः_स्मृतिः_स्थानाङ्कः));
#यदि_अन्तः
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश
{
    तार्किक enable_स्मृतिः_Pool = असत्य;

    वर्ग राशिः_Allocator : सार्वजनिक GpuMat::Allocator
    {
    सार्वजनिक:
        स्पष्ट राशिः_Allocator(cuda_प्रवाहः_t stream);
        ~राशिः_Allocator();

        तार्किक allocate(GpuMat* mat, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, size_t elem_आकार) CV_OVERRIDE;
        शून्य मुक्त(GpuMat* mat) CV_OVERRIDE;

    गुप्त:
        राशिः_Allocator(ध्रुव राशिः_Allocator&);
        राशिः_Allocator& संक्रियकः =(ध्रुव राशिः_Allocator&);

        cuda_प्रवाहः_t प्रवाहः_3;
        स्मृतिः_राशिः* mem_राशिः_2;
        size_t alignment_;
    };

    राशिः_Allocator::राशिः_Allocator(cuda_प्रवाहः_t stream) : प्रवाहः_3(stream), mem_राशिः_2(0)
    {
        यदि (enable_स्मृतिः_Pool)
        {
            ध्रुव पूर्णाङ्क deviceId = getDevice();
            mem_राशिः_2 = initializer.get_स्मृतिः_Pool(deviceId).get_Free_Mem_राशिः();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    राशिः_Allocator::~राशिः_Allocator()
    {
        यदि (mem_राशिः_2 != 0)
        {
            cuda_प्रवाहः_Synchronize(प्रवाहः_3);
            mem_राशिः_2->pool->return_Mem_राशिः(mem_राशिः_2);
        }
    }

    size_t align_ऊर्ध्वम्(size_t what, size_t alignment)
    {
        size_t align_आवरणम् = alignment-1;
        size_t inverse_Align_आवरणम् = ~align_आवरणम्;
        size_t res = (what + align_आवरणम्) & inverse_Align_आवरणम्;
        प्रत्यागम res;
    }

    तार्किक राशिः_Allocator::allocate(GpuMat* mat, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, size_t elem_आकार)
    {
        यदि (mem_राशिः_2 == 0)
            प्रत्यागम असत्य;

        size_t pitch, mem_आकार;

        यदि (पङ्क्तिकाः > 1 && स्तम्भसङ्ख्या > 1)
        {
            pitch = align_ऊर्ध्वम्(स्तम्भसङ्ख्या * elem_आकार, alignment_);
            mem_आकार = pitch * पङ्क्तिकाः;
        }
        अन्यथा
        {
            // Single row or single column must be continuous
            pitch = elem_आकार * स्तम्भसङ्ख्या;
            mem_आकार = align_ऊर्ध्वम्(elem_आकार * स्तम्भसङ्ख्या * पङ्क्तिकाः, 64);
        }

        uchar* ptr = mem_राशिः_2->request_स्मृतिः(mem_आकार);

        यदि (ptr == 0)
            प्रत्यागम असत्य;

        mat->दत्त = ptr;
        mat->चरणः = pitch;
        mat->refcount = (पूर्णाङ्क*) fastMalloc(परिमाण(पूर्णाङ्क));

        प्रत्यागम सत्य;
    }

    शून्य राशिः_Allocator::मुक्त(GpuMat* mat)
    {
        यदि (mem_राशिः_2 == 0)
            प्रत्यागम;

        mem_राशिः_2->return_स्मृतिः(mat->datastart);
        fastFree(mat->refcount);
    }
}

#यदि_अन्तः

/////////////////////////////////////////////////////////////
/// BufferPool

शून्य cv::cuda::समुच्चयः_बफर_Pool_Usage(तार्किक on)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(on);
    throw_no_cuda();
#अन्यथा
    enable_स्मृतिः_Pool = on;
#यदि_अन्तः
}

शून्य cv::cuda::समुच्चयः_बफर_Pool_Config(पूर्णाङ्क deviceId, size_t राशिः_आकार, पूर्णाङ्क राशिः_गणनम्)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(deviceId);
    CV_अप्रयुक्तम्(राशिः_आकार);
    CV_अप्रयुक्तम्(राशिः_गणनम्);
    throw_no_cuda();
#अन्यथा
    ध्रुव पूर्णाङ्क currentDevice = getDevice();

    यदि (deviceId >= 0)
    {
        समुच्चयः_Device(deviceId);
        initializer.get_स्मृतिः_Pool(deviceId).initialize(राशिः_आकार, राशिः_गणनम्);
    }
    अन्यथा
    {
        ध्रुव पूर्णाङ्क device_गणनम् = get_Cuda_Enabled_Device_गणनम्();

        प्रति (deviceId = 0; deviceId < device_गणनम्; ++deviceId)
        {
            समुच्चयः_Device(deviceId);
            initializer.get_स्मृतिः_Pool(deviceId).initialize(राशिः_आकार, राशिः_गणनम्);
        }
    }

    समुच्चयः_Device(currentDevice);
#यदि_अन्तः
}

#यदि_अपरिभाषितम् HAVE_CUDA
cv::cuda::बफर_Pool::बफर_Pool(प्रवाहः& stream)
{
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
}
#अन्यथा
cv::cuda::बफर_Pool::बफर_Pool(प्रवाहः& stream) : allocator_(stream.impl_->allocator)
{
}
#यदि_अन्तः

GpuMat cv::cuda::बफर_Pool::get_बफर(पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(पङ्क्तिकाः);
    CV_अप्रयुक्तम्(स्तम्भसङ्ख्या);
    CV_अप्रयुक्तम्(type);
    throw_no_cuda();
#अन्यथा
    GpuMat buf(allocator_);
    buf.create(पङ्क्तिकाः, स्तम्भसङ्ख्या, type);
    प्रत्यागम buf;
#यदि_अन्तः
}


////////////////////////////////////////////////////////////////
// Event

#यदि_अपरिभाषितम् HAVE_CUDA

वर्ग cv::cuda::Event::Impl
{
सार्वजनिक:
    Impl(अचिह्नित पूर्णाङ्क)
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

    स्पष्ट Impl(अचिह्नित पूर्णाङ्क flags);
    स्पष्ट Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(अचिह्नित पूर्णाङ्क flags) : event(0), ownEvent(असत्य)
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

cudaEvent_t cv::cuda::EventAccessor::getEvent(ध्रुव Event& event)
{
    प्रत्यागम event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    प्रत्यागम Event(makePtr<Event::Impl>(event));
}

#यदि_अन्तः

cv::cuda::Event::Event(ध्रुव Event::CreateFlags flags)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(flags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(flags);
#यदि_अन्तः
}

शून्य cv::cuda::Event::लेखः(प्रवाहः& stream)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(stream);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cuda_Event_लेखः(impl_->event, प्रवाहः_Accessor::get_प्रवाहः(stream)) );
#यदि_अन्तः
}

तार्किक cv::cuda::Event::queryIfComplete() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaEventQuery(impl_->event);

    यदि (err == cudaErrorNotReady || err == cudaSuccess)
        प्रत्यागम err == cudaSuccess;

    cudaSafeCall(err);
    प्रत्यागम असत्य;
#यदि_अन्तः
}

शून्य cv::cuda::Event::waitForCompletion()
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#यदि_अन्तः
}

भिन्न cv::cuda::Event::elapsedTime(ध्रुव Event& start, ध्रुव Event& end)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(start);
    CV_अप्रयुक्तम्(end);
    throw_no_cuda();
#अन्यथा
    भिन्न ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    प्रत्यागम ms;
#यदि_अन्तः
}
