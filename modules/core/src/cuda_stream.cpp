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

#شامل "precomp.hpp"
#شامل <cstdint>

استعمال نامخلا cv;
استعمال نامخلا cv::cuda;

#اگر defined(_MSC_VER)
#ہدایت warning(disable : 4702)  // unreachable code
#اگر_ختم

/////////////////////////////////////////////////////////////
/// MemoryStack

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    جماعت حافظہ_Pool;

    جماعت حافظہ_ڈھیر
    {
    عوامی:
        uchar* request_حافظہ(size_t size);
        خالی return_حافظہ(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        منطقی isFree;
        حافظہ_Pool* pool;

    #اگر !defined(NDEBUG)
        std::vector<size_t> allocations;
    #اگر_ختم
    };

    uchar* حافظہ_ڈھیر::request_حافظہ(size_t size)
    {
        ثابت size_t freeMem = dataend - tip;

        اگر (size > freeMem)
            واپس 0;

        uchar* ptr = tip;

        tip += size;

    #اگر !defined(NDEBUG)
        allocations.push_back(size);
    #اگر_ختم

        واپس ptr;
    }

    خالی حافظہ_ڈھیر::return_حافظہ(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #اگر !defined(NDEBUG)
        ثابت size_t alloc_سائز = tip - ptr;
        CV_Assert( alloc_سائز == allocations.back() );
        allocations.pop_back();
    #اگر_ختم

        tip = ptr;
    }
}

#اگر_ختم

/////////////////////////////////////////////////////////////
/// MemoryPool

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    جماعت حافظہ_Pool
    {
    عوامی:
        حافظہ_Pool();
        ~حافظہ_Pool() { release(); }

        خالی initialize(size_t ڈھیر_سائز, صحیح ڈھیر_گنتی);
        خالی release();

        حافظہ_ڈھیر* get_Free_Mem_ڈھیر();
        خالی return_Mem_ڈھیر(حافظہ_ڈھیر* mem_ڈھیر);

    نجی:
        خالی initilizeImpl();

        Mutex mtx_;

        منطقی initialized_;
        size_t ڈھیر_سائز_2;
        صحیح ڈھیر_گنتی_2;

        uchar* mem_;

        std::vector<حافظہ_ڈھیر> stacks_;

        حافظہ_Pool(ثابت حافظہ_Pool&); //= delete;
    };

    حافظہ_Pool::حافظہ_Pool() : initialized_(جھوٹ), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ڈھیر_سائز_2 = 10 * 1024 * 1024;
        ڈھیر_گنتی_2 = 5;
    }

    خالی حافظہ_Pool::initialize(size_t ڈھیر_سائز, صحیح ڈھیر_گنتی)
    {
        Auto_تالا lock(mtx_);

        release();

        ڈھیر_سائز_2 = ڈھیر_سائز;
        ڈھیر_گنتی_2 = ڈھیر_گنتی;

        initilizeImpl();
    }

    خالی حافظہ_Pool::initilizeImpl()
    {
        ثابت size_t کل_سائز = ڈھیر_سائز_2 * ڈھیر_گنتی_2;

        اگر (کل_سائز > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, کل_سائز);
            اگر (err != cudaSuccess)
                واپس;

            stacks_.resize(ڈھیر_گنتی_2);

            uchar* ptr = mem_;

            برائے (صحیح i = 0; i < ڈھیر_گنتی_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ڈھیر_سائز_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = سچ;
                stacks_[i].pool = یہ;

                ptr += ڈھیر_سائز_2;
            }

            initialized_ = سچ;
        }
    }

    خالی حافظہ_Pool::release()
    {
        اگر (mem_)
        {
#اگر !defined(NDEBUG)
            برائے (صحیح i = 0; i < ڈھیر_گنتی_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#اگر_ختم

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = جھوٹ;
        }
    }

    حافظہ_ڈھیر* حافظہ_Pool::get_Free_Mem_ڈھیر()
    {
        Auto_تالا lock(mtx_);

        اگر (!initialized_)
            initilizeImpl();

        اگر (!mem_)
            واپس 0;

        برائے (صحیح i = 0; i < ڈھیر_گنتی_2; ++i)
        {
            اگر (stacks_[i].isFree)
            {
                stacks_[i].isFree = جھوٹ;
                واپس &stacks_[i];
            }
        }

        واپس 0;
    }

    خالی حافظہ_Pool::return_Mem_ڈھیر(حافظہ_ڈھیر* mem_ڈھیر)
    {
        Auto_تالا lock(mtx_);

        CV_DbgAssert( !mem_ڈھیر->isFree );

#اگر !defined(NDEBUG)
        منطقی ملا = جھوٹ;
        برائے (صحیح i = 0; i < ڈھیر_گنتی_2; ++i)
        {
            اگر (mem_ڈھیر == &stacks_[i])
            {
                ملا = سچ;
                توڑو;
            }
        }
        CV_DbgAssert( ملا );
#اگر_ختم

        CV_DbgAssert( mem_ڈھیر->tip == mem_ڈھیر->datastart );

        mem_ڈھیر->isFree = سچ;
    }
}

#اگر_ختم

////////////////////////////////////////////////////////////////
/// Stream::Impl

#اگر_غیر_تعریف_شدہ HAVE_CUDA

جماعت cv::cuda::دھارا::Impl
{
عوامی:
    Impl(خالی* ptr = 0)
    {
        CV_غیر_مستعمل(ptr);
        throw_no_cuda();
    }
};

#ورنہ

نامخلا
{
    جماعت ڈھیر_Allocator;
}

جماعت cv::cuda::دھارا::Impl
{
عوامی:
    cuda_دھارا_t stream;
    منطقی own_دھارا;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(ثابت Ptr<GpuMat::Allocator>& allocator);
    Impl(ثابت غیرعلامتی صحیح cudaFlags);
    صریح Impl(cuda_دھارا_t stream);

    ~Impl();
};

cv::cuda::دھارا::Impl::Impl() : stream(0), own_دھارا(جھوٹ)
{
    cuda_محفوظ_Call( cuda_دھارا_Create(&stream) );
    own_دھارا = سچ;

    allocator = makePtr<ڈھیر_Allocator>(stream);
}

cv::cuda::دھارا::Impl::Impl(ثابت Ptr<GpuMat::Allocator>& allocator) : stream(0), own_دھارا(جھوٹ), allocator(allocator)
{
    cuda_محفوظ_Call( cuda_دھارا_Create(&stream) );
    own_دھارا = سچ;
}

cv::cuda::دھارا::Impl::Impl(ثابت غیرعلامتی صحیح cudaFlags) : stream(0), own_دھارا(جھوٹ)
{
    cuda_محفوظ_Call(cuda_دھارا_Create_With_Flags(&stream, cudaFlags));
    own_دھارا = سچ;
    allocator = makePtr<ڈھیر_Allocator>(stream);
}

cv::cuda::دھارا::Impl::Impl(cuda_دھارا_t دھارا_3) : stream(دھارا_3), own_دھارا(جھوٹ)
{
    allocator = makePtr<ڈھیر_Allocator>(stream);
}

cv::cuda::دھارا::Impl::~Impl()
{
    allocator.release();

    اگر (stream && own_دھارا)
    {
        cuda_دھارا_Destroy(stream);
    }
}

#اگر_ختم

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا cv { نامخلا cuda
{
    جماعت DefaultDeviceInitializer
    {
    عوامی:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        دھارا& get_Null_دھارا(صحیح deviceId);
        حافظہ_Pool& get_حافظہ_Pool(صحیح deviceId);

    نجی:
        خالی initStreams();
        خالی initPools();

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

        برائے (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_سیٹ_Device(static_cast<صحیح>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    دھارا& DefaultDeviceInitializer::get_Null_دھارا(صحیح deviceId)
    {
        Auto_تالا lock(streams_mtx_);

        اگر (streams_.empty())
        {
            صحیح device_گنتی = get_Cuda_Enabled_Device_گنتی();

            اگر (device_گنتی > 0)
                streams_.resize(device_گنتی);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحیح>(streams_.size()) );

        اگر (streams_[deviceId].empty())
        {
            cuda_دھارا_t stream = تہی;
            Ptr<دھارا::Impl> impl = makePtr<دھارا::Impl>(stream);
            streams_[deviceId] = Ptr<دھارا>(نیا دھارا(impl));
        }

        واپس *streams_[deviceId];
    }

    حافظہ_Pool& DefaultDeviceInitializer::get_حافظہ_Pool(صحیح deviceId)
    {
        Auto_تالا lock(pools_mtx_);

        اگر (pools_.empty())
        {
            صحیح device_گنتی = get_Cuda_Enabled_Device_گنتی();

            اگر (device_گنتی > 0)
            {
                pools_.resize(device_گنتی);
                برائے (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_سیٹ_Device(static_cast<صحیح>(i));
                    pools_[i] = makePtr<حافظہ_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<صحیح>(pools_.size()) );

        حافظہ_Pool* p = pools_[deviceId];
        CV_Assert(p);
        واپس *p;
    }

    DefaultDeviceInitializer initializer;
}}

#اگر_ختم

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::دھارا::دھارا()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    impl_ = makePtr<Impl>();
#اگر_ختم
}

cv::cuda::دھارا::دھارا(ثابت Ptr<GpuMat::Allocator>& allocator)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(allocator);
    throw_no_cuda();
#ورنہ
    impl_ = makePtr<Impl>(allocator);
#اگر_ختم
}

cv::cuda::دھارا::دھارا(ثابت size_t cudaFlags)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(cudaFlags);
    throw_no_cuda();
#ورنہ
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#اگر_ختم
}

منطقی cv::cuda::دھارا::queryIfComplete() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    cudaError_t err = cuda_دھارا_Query(impl_->stream);

    اگر (err == cudaErrorNotReady || err == cudaSuccess)
        واپس err == cudaSuccess;

    cuda_محفوظ_Call(err);
    واپس جھوٹ;
#اگر_ختم
}

خالی cv::cuda::دھارا::waitForCompletion()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    cuda_محفوظ_Call( cuda_دھارا_Synchronize(impl_->stream) );
#اگر_ختم
}

خالی cv::cuda::دھارا::waitEvent(ثابت Event& event)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(event);
    throw_no_cuda();
#ورنہ
    cuda_محفوظ_Call( cuda_دھارا_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#اگر_ختم
}

#اگر defined(HAVE_CUDA) && (CUDART_نسخہ >= 5000)

نامخلا
{
    ساخت Callback_ڈیٹا
    {
        دھارا::دھارا_Callback callback;
        خالی* user_ڈیٹا;

        Callback_ڈیٹا(دھارا::دھارا_Callback callback_, خالی* user_ڈیٹا_2) : callback(callback_), user_ڈیٹا(user_ڈیٹا_2) {}
    };

    خالی CUDART_CB cuda_دھارا_Callback(cuda_دھارا_t, cudaError_t حالت_2, خالی* user_ڈیٹا)
    {
        Callback_ڈیٹا* ڈیٹا = reinterpret_cast<Callback_ڈیٹا*>(user_ڈیٹا);
        ڈیٹا->callback(static_cast<صحیح>(حالت_2), ڈیٹا->user_ڈیٹا);
        حذف ڈیٹا;
    }
}

#اگر_ختم

خالی cv::cuda::دھارا::قطار_میں_ڈالو_Host_Callback(دھارا_Callback callback, خالی* user_ڈیٹا)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(callback);
    CV_غیر_مستعمل(user_ڈیٹا);
    throw_no_cuda();
#ورنہ
    #اگر CUDART_نسخہ < 5000
        CV_غیر_مستعمل(callback);
        CV_غیر_مستعمل(user_ڈیٹا);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #ورنہ
        Callback_ڈیٹا* ڈیٹا = نیا Callback_ڈیٹا(callback, user_ڈیٹا);

        cuda_محفوظ_Call( cuda_دھارا_جوڑو_Callback(impl_->stream, cuda_دھارا_Callback, ڈیٹا, 0) );
    #اگر_ختم
#اگر_ختم
}

دھارا& cv::cuda::دھارا::Null()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    ثابت صحیح deviceId = getDevice();
    واپس initializer.get_Null_دھارا(deviceId);
#اگر_ختم
}

خالی* cv::cuda::دھارا::cudaPtr() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    واپس خالی_اشاریہ;
#ورنہ
    واپس impl_->stream;
#اگر_ختم
}

cv::cuda::دھارا::عامل bool_type() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    واپس 0;
#ورنہ
    واپس (impl_->stream != 0) ? &دھارا::this_type_does_not_support_comparisons : 0;
#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_CUDA

cuda_دھارا_t cv::cuda::دھارا_Accessor::get_دھارا(ثابت دھارا& stream)
{
    واپس stream.impl_->stream;
}

دھارا cv::cuda::دھارا_Accessor::wrap_دھارا(cuda_دھارا_t stream)
{
    واپس دھارا(makePtr<دھارا::Impl>(stream));
}

#اگر_ختم

دھارا cv::cuda::wrap_دھارا(size_t cuda_دھارا_حافظہ_پتہ) {
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(cuda_دھارا_حافظہ_پتہ);
    throw_no_cuda();
#ورنہ
    واپس cv::cuda::دھارا_Accessor::wrap_دھارا(reinterpret_cast<cuda_دھارا_t>(cuda_دھارا_حافظہ_پتہ));
#اگر_ختم
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا
{
    منطقی enable_حافظہ_Pool = جھوٹ;

    جماعت ڈھیر_Allocator : عوامی GpuMat::Allocator
    {
    عوامی:
        صریح ڈھیر_Allocator(cuda_دھارا_t stream);
        ~ڈھیر_Allocator();

        منطقی allocate(GpuMat* mat, صحیح صفیں, صحیح ستون_تعداد, size_t elem_سائز) CV_OVERRIDE;
        خالی آزاد(GpuMat* mat) CV_OVERRIDE;

    نجی:
        ڈھیر_Allocator(ثابت ڈھیر_Allocator&);
        ڈھیر_Allocator& عامل =(ثابت ڈھیر_Allocator&);

        cuda_دھارا_t دھارا_3;
        حافظہ_ڈھیر* mem_ڈھیر_2;
        size_t alignment_;
    };

    ڈھیر_Allocator::ڈھیر_Allocator(cuda_دھارا_t stream) : دھارا_3(stream), mem_ڈھیر_2(0)
    {
        اگر (enable_حافظہ_Pool)
        {
            ثابت صحیح deviceId = getDevice();
            mem_ڈھیر_2 = initializer.get_حافظہ_Pool(deviceId).get_Free_Mem_ڈھیر();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ڈھیر_Allocator::~ڈھیر_Allocator()
    {
        اگر (mem_ڈھیر_2 != 0)
        {
            cuda_دھارا_Synchronize(دھارا_3);
            mem_ڈھیر_2->pool->return_Mem_ڈھیر(mem_ڈھیر_2);
        }
    }

    size_t align_اوپر(size_t what, size_t alignment)
    {
        size_t align_نقاب = alignment-1;
        size_t inverse_Align_نقاب = ~align_نقاب;
        size_t res = (what + align_نقاب) & inverse_Align_نقاب;
        واپس res;
    }

    منطقی ڈھیر_Allocator::allocate(GpuMat* mat, صحیح صفیں, صحیح ستون_تعداد, size_t elem_سائز)
    {
        اگر (mem_ڈھیر_2 == 0)
            واپس جھوٹ;

        size_t pitch, mem_سائز;

        اگر (صفیں > 1 && ستون_تعداد > 1)
        {
            pitch = align_اوپر(ستون_تعداد * elem_سائز, alignment_);
            mem_سائز = pitch * صفیں;
        }
        ورنہ
        {
            // Single row or single column must be continuous
            pitch = elem_سائز * ستون_تعداد;
            mem_سائز = align_اوپر(elem_سائز * ستون_تعداد * صفیں, 64);
        }

        uchar* ptr = mem_ڈھیر_2->request_حافظہ(mem_سائز);

        اگر (ptr == 0)
            واپس جھوٹ;

        mat->ڈیٹا = ptr;
        mat->قدم = pitch;
        mat->refcount = (صحیح*) fastMalloc(حجم(صحیح));

        واپس سچ;
    }

    خالی ڈھیر_Allocator::آزاد(GpuMat* mat)
    {
        اگر (mem_ڈھیر_2 == 0)
            واپس;

        mem_ڈھیر_2->return_حافظہ(mat->datastart);
        fastFree(mat->refcount);
    }
}

#اگر_ختم

/////////////////////////////////////////////////////////////
/// BufferPool

خالی cv::cuda::سیٹ_بفر_Pool_Usage(منطقی on)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(on);
    throw_no_cuda();
#ورنہ
    enable_حافظہ_Pool = on;
#اگر_ختم
}

خالی cv::cuda::سیٹ_بفر_Pool_Config(صحیح deviceId, size_t ڈھیر_سائز, صحیح ڈھیر_گنتی)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(deviceId);
    CV_غیر_مستعمل(ڈھیر_سائز);
    CV_غیر_مستعمل(ڈھیر_گنتی);
    throw_no_cuda();
#ورنہ
    ثابت صحیح currentDevice = getDevice();

    اگر (deviceId >= 0)
    {
        سیٹ_Device(deviceId);
        initializer.get_حافظہ_Pool(deviceId).initialize(ڈھیر_سائز, ڈھیر_گنتی);
    }
    ورنہ
    {
        ثابت صحیح device_گنتی = get_Cuda_Enabled_Device_گنتی();

        برائے (deviceId = 0; deviceId < device_گنتی; ++deviceId)
        {
            سیٹ_Device(deviceId);
            initializer.get_حافظہ_Pool(deviceId).initialize(ڈھیر_سائز, ڈھیر_گنتی);
        }
    }

    سیٹ_Device(currentDevice);
#اگر_ختم
}

#اگر_غیر_تعریف_شدہ HAVE_CUDA
cv::cuda::بفر_Pool::بفر_Pool(دھارا& stream)
{
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
}
#ورنہ
cv::cuda::بفر_Pool::بفر_Pool(دھارا& stream) : allocator_(stream.impl_->allocator)
{
}
#اگر_ختم

GpuMat cv::cuda::بفر_Pool::get_بفر(صحیح صفیں, صحیح ستون_تعداد, صحیح type)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(صفیں);
    CV_غیر_مستعمل(ستون_تعداد);
    CV_غیر_مستعمل(type);
    throw_no_cuda();
#ورنہ
    GpuMat buf(allocator_);
    buf.create(صفیں, ستون_تعداد, type);
    واپس buf;
#اگر_ختم
}


////////////////////////////////////////////////////////////////
// Event

#اگر_غیر_تعریف_شدہ HAVE_CUDA

جماعت cv::cuda::Event::Impl
{
عوامی:
    Impl(غیرعلامتی صحیح)
    {
        throw_no_cuda();
    }
};

#ورنہ

جماعت cv::cuda::Event::Impl
{
عوامی:
    cudaEvent_t event;
    منطقی ownEvent;

    صریح Impl(غیرعلامتی صحیح flags);
    صریح Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(غیرعلامتی صحیح flags) : event(0), ownEvent(جھوٹ)
{
    cuda_محفوظ_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = سچ;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(جھوٹ)
{
}

cv::cuda::Event::Impl::~Impl()
{
    اگر (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(ثابت Event& event)
{
    واپس event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    واپس Event(makePtr<Event::Impl>(event));
}

#اگر_ختم

cv::cuda::Event::Event(ثابت Event::CreateFlags flags)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(flags);
    throw_no_cuda();
#ورنہ
    impl_ = makePtr<Impl>(flags);
#اگر_ختم
}

خالی cv::cuda::Event::ریکارڈ(دھارا& stream)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(stream);
    throw_no_cuda();
#ورنہ
    cuda_محفوظ_Call( cuda_Event_ریکارڈ(impl_->event, دھارا_Accessor::get_دھارا(stream)) );
#اگر_ختم
}

منطقی cv::cuda::Event::queryIfComplete() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    cudaError_t err = cudaEventQuery(impl_->event);

    اگر (err == cudaErrorNotReady || err == cudaSuccess)
        واپس err == cudaSuccess;

    cuda_محفوظ_Call(err);
    واپس جھوٹ;
#اگر_ختم
}

خالی cv::cuda::Event::waitForCompletion()
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    cuda_محفوظ_Call( cudaEventSynchronize(impl_->event) );
#اگر_ختم
}

اعشاری cv::cuda::Event::elapsed_وقت(ثابت Event& آغاز, ثابت Event& end)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(آغاز);
    CV_غیر_مستعمل(end);
    throw_no_cuda();
#ورنہ
    اعشاری ms;
    cuda_محفوظ_Call( cuda_Event_Elapsed_وقت(&ms, آغاز.impl_->event, end.impl_->event) );
    واپس ms;
#اگر_ختم
}
