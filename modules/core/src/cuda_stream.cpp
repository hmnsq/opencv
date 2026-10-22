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

#ضمن "precomp.hpp"
#ضمن <cstdint>

ورتوں نطاق cv;
ورتوں نطاق cv::cuda;

#إذا defined(_MSC_VER)
#ہدایت warning(disable : 4702)  // unreachable code
#جے_انت

/////////////////////////////////////////////////////////////
/// MemoryStack

#جے_تعریف_شدہ HAVE_CUDA

نطاق
{
    صنف حافظہ_Pool;

    صنف حافظہ_ڈھیر
    {
    عام:
        uchar* request_حافظہ(size_t size);
        فراغ return_حافظہ(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        منطقي isFree;
        حافظہ_Pool* pool;

    #إذا !defined(NDEBUG)
        std::vector<size_t> allocations;
    #جے_انت
    };

    uchar* حافظہ_ڈھیر::request_حافظہ(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        إذا (size > freeMem)
            أرجع 0;

        uchar* ptr = tip;

        tip += size;

    #إذا !defined(NDEBUG)
        allocations.push_back(size);
    #جے_انت

        أرجع ptr;
    }

    فراغ حافظہ_ڈھیر::return_حافظہ(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #إذا !defined(NDEBUG)
        ثابت size_t alloc_ناپ = tip - ptr;
        CV_Assert( alloc_ناپ == allocations.back() );
        allocations.pop_back();
    #جے_انت

        tip = ptr;
    }
}

#جے_انت

/////////////////////////////////////////////////////////////
/// MemoryPool

#جے_تعریف_شدہ HAVE_CUDA

نطاق
{
    صنف حافظہ_Pool
    {
    عام:
        حافظہ_Pool();
        ~حافظہ_Pool() { release(); }

        فراغ initialize(size_t ڈھیر_ناپ, صحيح ڈھیر_گنتی);
        فراغ release();

        حافظہ_ڈھیر* get_Free_Mem_ڈھیر();
        فراغ return_Mem_ڈھیر(حافظہ_ڈھیر* mem_ڈھیر);

    خاص:
        فراغ initilizeImpl();

        Mutex mtx_;

        منطقي initialized_;
        size_t ڈھیر_ناپ_2;
        صحيح ڈھیر_گنتی_2;

        uchar* mem_;

        std::vector<حافظہ_ڈھیر> stacks_;

        حافظہ_Pool(ثابت حافظہ_Pool&); //= delete;
    };

    حافظہ_Pool::حافظہ_Pool() : initialized_(خطأ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ڈھیر_ناپ_2 = 10 * 1024 * 1024;
        ڈھیر_گنتی_2 = 5;
    }

    فراغ حافظہ_Pool::initialize(size_t ڈھیر_ناپ, صحيح ڈھیر_گنتی)
    {
        Auto_جندرا lock(mtx_);

        release();

        ڈھیر_ناپ_2 = ڈھیر_ناپ;
        ڈھیر_گنتی_2 = ڈھیر_گنتی;

        initilizeImpl();
    }

    فراغ حافظہ_Pool::initilizeImpl()
    {
        ثابت size_t کل_ناپ = ڈھیر_ناپ_2 * ڈھیر_گنتی_2;

        إذا (کل_ناپ > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, کل_ناپ);
            إذا (err != cudaSuccess)
                أرجع;

            stacks_.resize(ڈھیر_گنتی_2);

            uchar* ptr = mem_;

            لكل (صحيح i = 0; i < ڈھیر_گنتی_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ڈھیر_ناپ_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = سچ;
                stacks_[i].pool = هذا;

                ptr += ڈھیر_ناپ_2;
            }

            initialized_ = سچ;
        }
    }

    فراغ حافظہ_Pool::release()
    {
        إذا (mem_)
        {
#إذا !defined(NDEBUG)
            لكل (صحيح i = 0; i < ڈھیر_گنتی_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#جے_انت

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = خطأ;
        }
    }

    حافظہ_ڈھیر* حافظہ_Pool::get_Free_Mem_ڈھیر()
    {
        Auto_جندرا lock(mtx_);

        إذا (!initialized_)
            initilizeImpl();

        إذا (!mem_)
            أرجع 0;

        لكل (صحيح i = 0; i < ڈھیر_گنتی_2; ++i)
        {
            إذا (stacks_[i].isFree)
            {
                stacks_[i].isFree = خطأ;
                أرجع &stacks_[i];
            }
        }

        أرجع 0;
    }

    فراغ حافظہ_Pool::return_Mem_ڈھیر(حافظہ_ڈھیر* mem_ڈھیر)
    {
        Auto_جندرا lock(mtx_);

        CV_DbgAssert( !mem_ڈھیر->isFree );

#إذا !defined(NDEBUG)
        منطقي لبھیا = خطأ;
        لكل (صحيح i = 0; i < ڈھیر_گنتی_2; ++i)
        {
            إذا (mem_ڈھیر == &stacks_[i])
            {
                لبھیا = سچ;
                اكسر;
            }
        }
        CV_DbgAssert( لبھیا );
#جے_انت

        CV_DbgAssert( mem_ڈھیر->tip == mem_ڈھیر->datastart );

        mem_ڈھیر->isFree = سچ;
    }
}

#جے_انت

////////////////////////////////////////////////////////////////
/// Stream::Impl

#جے_بنا_تعریف HAVE_CUDA

صنف cv::cuda::دھارا::Impl
{
عام:
    Impl(فراغ* ptr = 0)
    {
        CV_بن_ورتیا(ptr);
        throw_no_cuda();
    }
};

#وإلا

نطاق
{
    صنف ڈھیر_Allocator;
}

صنف cv::cuda::دھارا::Impl
{
عام:
    cuda_دھارا_t stream;
    منطقي own_دھارا;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت بنانشانی صحيح cudaFlags);
    واضح Impl(cuda_دھارا_t stream);

    ~Impl();
};

cv::cuda::دھارا::Impl::Impl() : stream(0), own_دھارا(خطأ)
{
    cuda_محفوظ_Call( cuda_دھارا_Create(&stream) );
    own_دھارا = سچ;

    allocator = makePtr<ڈھیر_Allocator>(stream);
}

cv::cuda::دھارا::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), own_دھارا(خطأ), allocator(allocator)
{
    cuda_محفوظ_Call( cuda_دھارا_Create(&stream) );
    own_دھارا = سچ;
}

cv::cuda::دھارا::Impl::Impl(ثابت بنانشانی صحيح cudaFlags) : stream(0), own_دھارا(خطأ)
{
    cuda_محفوظ_Call(cuda_دھارا_Create_With_Flags(&stream, cudaFlags));
    own_دھارا = سچ;
    allocator = makePtr<ڈھیر_Allocator>(stream);
}

cv::cuda::دھارا::Impl::Impl(cuda_دھارا_t دھارا_3) : stream(دھارا_3), own_دھارا(خطأ)
{
    allocator = makePtr<ڈھیر_Allocator>(stream);
}

cv::cuda::دھارا::Impl::~Impl()
{
    allocator.release();

    إذا (stream && own_دھارا)
    {
        cuda_دھارا_Destroy(stream);
    }
}

#جے_انت

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#جے_تعریف_شدہ HAVE_CUDA

نطاق cv { نطاق cuda
{
    صنف DefaultDeviceInitializer
    {
    عام:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        دھارا& get_Null_دھارا(صحيح deviceId);
        حافظہ_Pool& get_حافظہ_Pool(صحيح deviceId);

    خاص:
        فراغ initStreams();
        فراغ initPools();

        std::vector<Ptr<دھارا> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<حافظہ_Pool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        لكل (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_سیٹ_Device(static_cast<صحيح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    دھارا& DefaultDeviceInitializer::get_Null_دھارا(صحيح deviceId)
    {
        Auto_جندرا lock(streams_mtx_);

        إذا (streams_.empty())
        {
            صحيح device_گنتی = get_Cuda_Enabled_Device_گنتی();

            إذا (device_گنتی > 0)
                streams_.resize(device_گنتی);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(streams_.size()) );

        إذا (streams_[deviceId].empty())
        {
            cuda_دھارا_t stream = عدم;
            Ptr<دھارا::Impl> impl = makePtr<دھارا::Impl>(stream);
            streams_[deviceId] = Ptr<دھارا>(جديد دھارا(impl));
        }

        أرجع *streams_[deviceId];
    }

    حافظہ_Pool& DefaultDeviceInitializer::get_حافظہ_Pool(صحيح deviceId)
    {
        Auto_جندرا lock(pools_mtx_);

        إذا (pools_.empty())
        {
            صحيح device_گنتی = get_Cuda_Enabled_Device_گنتی();

            إذا (device_گنتی > 0)
            {
                pools_.resize(device_گنتی);
                لكل (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_سیٹ_Device(static_cast<صحيح>(i));
                    pools_[i] = makePtr<حافظہ_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحيح>(pools_.size()) );

        حافظہ_Pool* p = pools_[deviceId];
        CV_Assert(p);
        أرجع *p;
    }

    DefaultDeviceInitializer initializer;
}}

#جے_انت

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::دھارا::دھارا()
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>();
#جے_انت
}

cv::cuda::دھارا::دھارا(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(allocator);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(allocator);
#جے_انت
}

cv::cuda::دھارا::دھارا(ثابت size_t cudaFlags)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(cudaFlags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#جے_انت
}

منطقي cv::cuda::دھارا::queryIfComplete() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cuda_دھارا_Query(impl_->stream);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_محفوظ_Call(err);
    أرجع خطأ;
#جے_انت
}

فراغ cv::cuda::دھارا::waitForCompletion()
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_دھارا_Synchronize(impl_->stream) );
#جے_انت
}

فراغ cv::cuda::دھارا::waitEvent(ثابت Event& event)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(event);
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_دھارا_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#جے_انت
}

#إذا defined(HAVE_CUDA) && (CUDART_نسخہ >= 5000)

نطاق
{
    بنية Callback_انکڑے
    {
        دھارا::دھارا_Callback callback;
        فراغ* user_انکڑے;

        Callback_انکڑے(دھارا::دھارا_Callback callback_, فراغ* user_انکڑے_2) : callback(callback_), user_انکڑے(user_انکڑے_2) {}
    };

    فراغ CUDART_CB cuda_دھارا_Callback(cuda_دھارا_t, cudaError_t حالت, فراغ* user_انکڑے)
    {
        Callback_انکڑے* انکڑے = reinterpret_cast<Callback_انکڑے*>(user_انکڑے);
        انکڑے->callback(static_cast<صحيح>(حالت), انکڑے->user_انکڑے);
        مٹاؤ انکڑے;
    }
}

#جے_انت

فراغ cv::cuda::دھارا::قطار_وچ_پاؤ_Host_Callback(دھارا_Callback callback, فراغ* user_انکڑے)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(callback);
    CV_بن_ورتیا(user_انکڑے);
    throw_no_cuda();
#وإلا
    #إذا CUDART_نسخہ < 5000
        CV_بن_ورتیا(callback);
        CV_بن_ورتیا(user_انکڑے);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #وإلا
        Callback_انکڑے* انکڑے = جديد Callback_انکڑے(callback, user_انکڑے);

        cuda_محفوظ_Call( cuda_دھارا_جوڑو_Callback(impl_->stream, cuda_دھارا_Callback, انکڑے, 0) );
    #جے_انت
#جے_انت
}

دھارا& cv::cuda::دھارا::Null()
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    ثابت صحيح deviceId = getDevice();
    أرجع initializer.get_Null_دھارا(deviceId);
#جے_انت
}

فراغ* cv::cuda::دھارا::cudaPtr() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    أرجع خالی_سوچک;
#وإلا
    أرجع impl_->stream;
#جے_انت
}

cv::cuda::دھارا::عامل bool_type() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    أرجع 0;
#وإلا
    أرجع (impl_->stream != 0) ? &دھارا::this_type_does_not_support_comparisons : 0;
#جے_انت
}

#جے_تعریف_شدہ HAVE_CUDA

cuda_دھارا_t cv::cuda::دھارا_Accessor::get_دھارا(ثابت دھارا& stream)
{
    أرجع stream.impl_->stream;
}

دھارا cv::cuda::دھارا_Accessor::wrap_دھارا(cuda_دھارا_t stream)
{
    أرجع دھارا(makePtr<دھارا::Impl>(stream));
}

#جے_انت

دھارا cv::cuda::wrap_دھارا(size_t cuda_دھارا_حافظہ_پتہ) {
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(cuda_دھارا_حافظہ_پتہ);
    throw_no_cuda();
#وإلا
    أرجع cv::cuda::دھارا_Accessor::wrap_دھارا(reinterpret_cast<cuda_دھارا_t>(cuda_دھارا_حافظہ_پتہ));
#جے_انت
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#جے_تعریف_شدہ HAVE_CUDA

نطاق
{
    منطقي enable_حافظہ_Pool = خطأ;

    صنف ڈھیر_Allocator : عام GpuMat::Allocator
    {
    عام:
        واضح ڈھیر_Allocator(cuda_دھارا_t stream);
        ~ڈھیر_Allocator();

        منطقي allocate(GpuMat* mat, صحيح صفاں, صحيح خانے_گنتی, size_t elem_ناپ) CV_OVERRIDE;
        فراغ حرر(GpuMat* mat) CV_OVERRIDE;

    خاص:
        ڈھیر_Allocator(ثابت ڈھیر_Allocator&);
        ڈھیر_Allocator& عامل =(ثابت ڈھیر_Allocator&);

        cuda_دھارا_t دھارا_3;
        حافظہ_ڈھیر* mem_ڈھیر_2;
        size_t alignment_;
    };

    ڈھیر_Allocator::ڈھیر_Allocator(cuda_دھارا_t stream) : دھارا_3(stream), mem_ڈھیر_2(0)
    {
        إذا (enable_حافظہ_Pool)
        {
            ثابت صحيح deviceId = getDevice();
            mem_ڈھیر_2 = initializer.get_حافظہ_Pool(deviceId).get_Free_Mem_ڈھیر();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ڈھیر_Allocator::~ڈھیر_Allocator()
    {
        إذا (mem_ڈھیر_2 != 0)
        {
            cuda_دھارا_Synchronize(دھارا_3);
            mem_ڈھیر_2->pool->return_Mem_ڈھیر(mem_ڈھیر_2);
        }
    }

    size_t align_اتے(size_t what, size_t alignment)
    {
        size_t align_نقاب = alignment-1;
        size_t inverse_Align_نقاب = ~align_نقاب;
        size_t res = (what + align_نقاب) & inverse_Align_نقاب;
        أرجع res;
    }

    منطقي ڈھیر_Allocator::allocate(GpuMat* mat, صحيح صفاں, صحيح خانے_گنتی, size_t elem_ناپ)
    {
        إذا (mem_ڈھیر_2 == 0)
            أرجع خطأ;

        size_t pitch, mem_ناپ;

        إذا (صفاں > 1 && خانے_گنتی > 1)
        {
            pitch = align_اتے(خانے_گنتی * elem_ناپ, alignment_);
            mem_ناپ = pitch * صفاں;
        }
        وإلا
        {
            // Single row or single column must be continuous
            pitch = elem_ناپ * خانے_گنتی;
            mem_ناپ = align_اتے(elem_ناپ * خانے_گنتی * صفاں, 64);
        }

        uchar* ptr = mem_ڈھیر_2->request_حافظہ(mem_ناپ);

        إذا (ptr == 0)
            أرجع خطأ;

        mat->انکڑے = ptr;
        mat->قدم = pitch;
        mat->refcount = (صحيح*) fastMalloc(حجم(صحيح));

        أرجع سچ;
    }

    فراغ ڈھیر_Allocator::حرر(GpuMat* mat)
    {
        إذا (mem_ڈھیر_2 == 0)
            أرجع;

        mem_ڈھیر_2->return_حافظہ(mat->datastart);
        fastFree(mat->refcount);
    }
}

#جے_انت

/////////////////////////////////////////////////////////////
/// BufferPool

فراغ cv::cuda::سیٹ_بفر_Pool_Usage(منطقي on)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(on);
    throw_no_cuda();
#وإلا
    enable_حافظہ_Pool = on;
#جے_انت
}

فراغ cv::cuda::سیٹ_بفر_Pool_Config(صحيح deviceId, size_t ڈھیر_ناپ, صحيح ڈھیر_گنتی)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(deviceId);
    CV_بن_ورتیا(ڈھیر_ناپ);
    CV_بن_ورتیا(ڈھیر_گنتی);
    throw_no_cuda();
#وإلا
    ثابت صحيح currentDevice = getDevice();

    إذا (deviceId >= 0)
    {
        سیٹ_Device(deviceId);
        initializer.get_حافظہ_Pool(deviceId).initialize(ڈھیر_ناپ, ڈھیر_گنتی);
    }
    وإلا
    {
        ثابت صحيح device_گنتی = get_Cuda_Enabled_Device_گنتی();

        لكل (deviceId = 0; deviceId < device_گنتی; ++deviceId)
        {
            سیٹ_Device(deviceId);
            initializer.get_حافظہ_Pool(deviceId).initialize(ڈھیر_ناپ, ڈھیر_گنتی);
        }
    }

    سیٹ_Device(currentDevice);
#جے_انت
}

#جے_بنا_تعریف HAVE_CUDA
cv::cuda::بفر_Pool::بفر_Pool(دھارا& stream)
{
    CV_بن_ورتیا(stream);
    throw_no_cuda();
}
#وإلا
cv::cuda::بفر_Pool::بفر_Pool(دھارا& stream) : allocator_(stream.impl_->allocator)
{
}
#جے_انت

GpuMat cv::cuda::بفر_Pool::get_بفر(صحيح صفاں, صحيح خانے_گنتی, صحيح type)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(صفاں);
    CV_بن_ورتیا(خانے_گنتی);
    CV_بن_ورتیا(type);
    throw_no_cuda();
#وإلا
    GpuMat buf(allocator_);
    buf.create(صفاں, خانے_گنتی, type);
    أرجع buf;
#جے_انت
}


////////////////////////////////////////////////////////////////
// Event

#جے_بنا_تعریف HAVE_CUDA

صنف cv::cuda::Event::Impl
{
عام:
    Impl(بنانشانی صحيح)
    {
        throw_no_cuda();
    }
};

#وإلا

صنف cv::cuda::Event::Impl
{
عام:
    cudaEvent_t event;
    منطقي ownEvent;

    واضح Impl(بنانشانی صحيح flags);
    واضح Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(بنانشانی صحيح flags) : event(0), ownEvent(خطأ)
{
    cuda_محفوظ_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = سچ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(خطأ)
{
}

cv::cuda::Event::Impl::~Impl()
{
    إذا (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ثابت Event& event)
{
    أرجع event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    أرجع Event(makePtr<Event::Impl>(event));
}

#جے_انت

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(flags);
    throw_no_cuda();
#وإلا
    impl_ = makePtr<Impl>(flags);
#جے_انت
}

فراغ cv::cuda::Event::ریکارڈ(دھارا& stream)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(stream);
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cuda_Event_ریکارڈ(impl_->event, دھارا_Accessor::get_دھارا(stream)) );
#جے_انت
}

منطقي cv::cuda::Event::queryIfComplete() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    cudaError_t err = cudaEventQuery(impl_->event);

    إذا (err == cudaErrorNotReady || err == cudaSuccess)
        أرجع err == cudaSuccess;

    cuda_محفوظ_Call(err);
    أرجع خطأ;
#جے_انت
}

فراغ cv::cuda::Event::waitForCompletion()
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    cuda_محفوظ_Call( cudaEventSynchronize(impl_->event) );
#جے_انت
}

عائم cv::cuda::Event::elapsed_ویلا(ثابت Event& شروع, ثابت Event& end)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(شروع);
    CV_بن_ورتیا(end);
    throw_no_cuda();
#وإلا
    عائم ms;
    cuda_محفوظ_Call( cuda_Event_Elapsed_ویلا(&ms, شروع.impl_->event, end.impl_->event) );
    أرجع ms;
#جے_انت
}
