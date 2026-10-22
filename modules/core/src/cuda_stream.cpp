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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <cstdint>

ఉపయోగించి నామాకాశ cv;
ఉపయోగించి నామాకాశ cv::cuda;

#ఒకవేళ defined(_MSC_VER)
#సూచన warning(disable : 4702)  // unreachable code
#అయితే_ముగింపు

/////////////////////////////////////////////////////////////
/// MemoryStack

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    తరగతి జ్ఞాపకం_Pool;

    తరగతి జ్ఞాపకం_దొంతర
    {
    బహిరంగ:
        uchar* request_జ్ఞాపకం(size_t size);
        శూన్య return_జ్ఞాపకం(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        తార్కిక isFree;
        జ్ఞాపకం_Pool* pool;

    #ఒకవేళ !defined(NDEBUG)
        std::vector<size_t> allocations;
    #అయితే_ముగింపు
    };

    uchar* జ్ఞాపకం_దొంతర::request_జ్ఞాపకం(size_t size)
    {
        స్థిరరాశి size_t freeMem = dataend - tip;

        ఒకవేళ (size > freeMem)
            తిరిగి 0;

        uchar* ptr = tip;

        tip += size;

    #ఒకవేళ !defined(NDEBUG)
        allocations.push_back(size);
    #అయితే_ముగింపు

        తిరిగి ptr;
    }

    శూన్య జ్ఞాపకం_దొంతర::return_జ్ఞాపకం(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #ఒకవేళ !defined(NDEBUG)
        స్థిరరాశి size_t alloc_కొలత = tip - ptr;
        CV_Assert( alloc_కొలత == allocations.back() );
        allocations.pop_back();
    #అయితే_ముగింపు

        tip = ptr;
    }
}

#అయితే_ముగింపు

/////////////////////////////////////////////////////////////
/// MemoryPool

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    తరగతి జ్ఞాపకం_Pool
    {
    బహిరంగ:
        జ్ఞాపకం_Pool();
        ~జ్ఞాపకం_Pool() { release(); }

        శూన్య initialize(size_t దొంతర_కొలత, పూర్ణాంకం దొంతర_లెక్క);
        శూన్య release();

        జ్ఞాపకం_దొంతర* get_Free_Mem_దొంతర();
        శూన్య return_Mem_దొంతర(జ్ఞాపకం_దొంతర* mem_దొంతర);

    అంతరంగ:
        శూన్య initilizeImpl();

        Mutex mtx_;

        తార్కిక initialized_;
        size_t దొంతర_కొలత_2;
        పూర్ణాంకం దొంతర_లెక్క_2;

        uchar* mem_;

        std::vector<జ్ఞాపకం_దొంతర> stacks_;

        జ్ఞాపకం_Pool(స్థిరరాశి జ్ఞాపకం_Pool&); //= delete;
    };

    జ్ఞాపకం_Pool::జ్ఞాపకం_Pool() : initialized_(అబద్ధం), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        దొంతర_కొలత_2 = 10 * 1024 * 1024;
        దొంతర_లెక్క_2 = 5;
    }

    శూన్య జ్ఞాపకం_Pool::initialize(size_t దొంతర_కొలత, పూర్ణాంకం దొంతర_లెక్క)
    {
        Auto_తాళం lock(mtx_);

        release();

        దొంతర_కొలత_2 = దొంతర_కొలత;
        దొంతర_లెక్క_2 = దొంతర_లెక్క;

        initilizeImpl();
    }

    శూన్య జ్ఞాపకం_Pool::initilizeImpl()
    {
        స్థిరరాశి size_t మొత్తం_కొలత = దొంతర_కొలత_2 * దొంతర_లెక్క_2;

        ఒకవేళ (మొత్తం_కొలత > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, మొత్తం_కొలత);
            ఒకవేళ (err != cudaSuccess)
                తిరిగి;

            stacks_.resize(దొంతర_లెక్క_2);

            uchar* ptr = mem_;

            కొరకు (పూర్ణాంకం i = 0; i < దొంతర_లెక్క_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + దొంతర_కొలత_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = నిజం;
                stacks_[i].pool = ఇది;

                ptr += దొంతర_కొలత_2;
            }

            initialized_ = నిజం;
        }
    }

    శూన్య జ్ఞాపకం_Pool::release()
    {
        ఒకవేళ (mem_)
        {
#ఒకవేళ !defined(NDEBUG)
            కొరకు (పూర్ణాంకం i = 0; i < దొంతర_లెక్క_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#అయితే_ముగింపు

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = అబద్ధం;
        }
    }

    జ్ఞాపకం_దొంతర* జ్ఞాపకం_Pool::get_Free_Mem_దొంతర()
    {
        Auto_తాళం lock(mtx_);

        ఒకవేళ (!initialized_)
            initilizeImpl();

        ఒకవేళ (!mem_)
            తిరిగి 0;

        కొరకు (పూర్ణాంకం i = 0; i < దొంతర_లెక్క_2; ++i)
        {
            ఒకవేళ (stacks_[i].isFree)
            {
                stacks_[i].isFree = అబద్ధం;
                తిరిగి &stacks_[i];
            }
        }

        తిరిగి 0;
    }

    శూన్య జ్ఞాపకం_Pool::return_Mem_దొంతర(జ్ఞాపకం_దొంతర* mem_దొంతర)
    {
        Auto_తాళం lock(mtx_);

        CV_DbgAssert( !mem_దొంతర->isFree );

#ఒకవేళ !defined(NDEBUG)
        తార్కిక దొరికింది = అబద్ధం;
        కొరకు (పూర్ణాంకం i = 0; i < దొంతర_లెక్క_2; ++i)
        {
            ఒకవేళ (mem_దొంతర == &stacks_[i])
            {
                దొరికింది = నిజం;
                ఆపు;
            }
        }
        CV_DbgAssert( దొరికింది );
#అయితే_ముగింపు

        CV_DbgAssert( mem_దొంతర->tip == mem_దొంతర->datastart );

        mem_దొంతర->isFree = నిజం;
    }
}

#అయితే_ముగింపు

////////////////////////////////////////////////////////////////
/// Stream::Impl

#నిర్వచితం_కాకపోతే HAVE_CUDA

తరగతి cv::cuda::ప్రవాహం::Impl
{
బహిరంగ:
    Impl(శూన్య* ptr = 0)
    {
        CV_వాడని(ptr);
        throw_no_cuda();
    }
};

#లేకపోతే

నామాకాశ
{
    తరగతి దొంతర_Allocator;
}

తరగతి cv::cuda::ప్రవాహం::Impl
{
బహిరంగ:
    cuda_ప్రవాహం_t stream;
    తార్కిక own_ప్రవాహం;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(స్థిరరాశి Ptr<GpuMat::Allocator>& allocator);
    Impl(స్థిరరాశి గుర్తులేని పూర్ణాంకం cudaFlags);
    స్పష్టమైన Impl(cuda_ప్రవాహం_t stream);

    ~Impl();
};

cv::cuda::ప్రవాహం::Impl::Impl() : stream(0), own_ప్రవాహం(అబద్ధం)
{
    cuda_సురక్షిత_Call( cuda_ప్రవాహం_Create(&stream) );
    own_ప్రవాహం = నిజం;

    allocator = makePtr<దొంతర_Allocator>(stream);
}

cv::cuda::ప్రవాహం::Impl::Impl(స్థిరరాశి Ptr<GpuMat::Allocator>& allocator) : stream(0), own_ప్రవాహం(అబద్ధం), allocator(allocator)
{
    cuda_సురక్షిత_Call( cuda_ప్రవాహం_Create(&stream) );
    own_ప్రవాహం = నిజం;
}

cv::cuda::ప్రవాహం::Impl::Impl(స్థిరరాశి గుర్తులేని పూర్ణాంకం cudaFlags) : stream(0), own_ప్రవాహం(అబద్ధం)
{
    cuda_సురక్షిత_Call(cuda_ప్రవాహం_Create_With_Flags(&stream, cudaFlags));
    own_ప్రవాహం = నిజం;
    allocator = makePtr<దొంతర_Allocator>(stream);
}

cv::cuda::ప్రవాహం::Impl::Impl(cuda_ప్రవాహం_t ప్రవాహం_3) : stream(ప్రవాహం_3), own_ప్రవాహం(అబద్ధం)
{
    allocator = makePtr<దొంతర_Allocator>(stream);
}

cv::cuda::ప్రవాహం::Impl::~Impl()
{
    allocator.release();

    ఒకవేళ (stream && own_ప్రవాహం)
    {
        cuda_ప్రవాహం_Destroy(stream);
    }
}

#అయితే_ముగింపు

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ cv { నామాకాశ cuda
{
    తరగతి DefaultDeviceInitializer
    {
    బహిరంగ:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        ప్రవాహం& get_Null_ప్రవాహం(పూర్ణాంకం deviceId);
        జ్ఞాపకం_Pool& get_జ్ఞాపకం_Pool(పూర్ణాంకం deviceId);

    అంతరంగ:
        శూన్య initStreams();
        శూన్య initPools();

        std::vector<Ptr<ప్రవాహం> > streams_;
        Mutex streams_mtx_;

        std::vector<Ptr<జ్ఞాపకం_Pool> > pools_;
        Mutex pools_mtx_;
    };

    DefaultDeviceInitializer::DefaultDeviceInitializer()
    {
    }

    DefaultDeviceInitializer::~DefaultDeviceInitializer()
    {
        streams_.clear();

        కొరకు (size_t i = 0; i < pools_.size(); ++i)
        {
            cuda_సమితి_Device(static_cast<పూర్ణాంకం>(i));
            pools_[i]->release();
        }

        pools_.clear();
    }

    ప్రవాహం& DefaultDeviceInitializer::get_Null_ప్రవాహం(పూర్ణాంకం deviceId)
    {
        Auto_తాళం lock(streams_mtx_);

        ఒకవేళ (streams_.empty())
        {
            పూర్ణాంకం device_లెక్క = get_Cuda_Enabled_Device_లెక్క();

            ఒకవేళ (device_లెక్క > 0)
                streams_.resize(device_లెక్క);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<పూర్ణాంకం>(streams_.size()) );

        ఒకవేళ (streams_[deviceId].empty())
        {
            cuda_ప్రవాహం_t stream = ఖాళీ;
            Ptr<ప్రవాహం::Impl> impl = makePtr<ప్రవాహం::Impl>(stream);
            streams_[deviceId] = Ptr<ప్రవాహం>(కొత్త ప్రవాహం(impl));
        }

        తిరిగి *streams_[deviceId];
    }

    జ్ఞాపకం_Pool& DefaultDeviceInitializer::get_జ్ఞాపకం_Pool(పూర్ణాంకం deviceId)
    {
        Auto_తాళం lock(pools_mtx_);

        ఒకవేళ (pools_.empty())
        {
            పూర్ణాంకం device_లెక్క = get_Cuda_Enabled_Device_లెక్క();

            ఒకవేళ (device_లెక్క > 0)
            {
                pools_.resize(device_లెక్క);
                కొరకు (size_t i = 0; i < pools_.size(); ++i)
                {
                    cuda_సమితి_Device(static_cast<పూర్ణాంకం>(i));
                    pools_[i] = makePtr<జ్ఞాపకం_Pool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<పూర్ణాంకం>(pools_.size()) );

        జ్ఞాపకం_Pool* p = pools_[deviceId];
        CV_Assert(p);
        తిరిగి *p;
    }

    DefaultDeviceInitializer initializer;
}}

#అయితే_ముగింపు

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::ప్రవాహం::ప్రవాహం()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    impl_ = makePtr<Impl>();
#అయితే_ముగింపు
}

cv::cuda::ప్రవాహం::ప్రవాహం(స్థిరరాశి Ptr<GpuMat::Allocator>& allocator)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(allocator);
    throw_no_cuda();
#లేకపోతే
    impl_ = makePtr<Impl>(allocator);
#అయితే_ముగింపు
}

cv::cuda::ప్రవాహం::ప్రవాహం(స్థిరరాశి size_t cudaFlags)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(cudaFlags);
    throw_no_cuda();
#లేకపోతే
    impl_ = makePtr<Impl>(cudaFlags & UINT_MAX);
#అయితే_ముగింపు
}

తార్కిక cv::cuda::ప్రవాహం::queryIfComplete() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    cudaError_t err = cuda_ప్రవాహం_Query(impl_->stream);

    ఒకవేళ (err == cudaErrorNotReady || err == cudaSuccess)
        తిరిగి err == cudaSuccess;

    cuda_సురక్షిత_Call(err);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

శూన్య cv::cuda::ప్రవాహం::waitForCompletion()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    cuda_సురక్షిత_Call( cuda_ప్రవాహం_Synchronize(impl_->stream) );
#అయితే_ముగింపు
}

శూన్య cv::cuda::ప్రవాహం::waitEvent(స్థిరరాశి Event& event)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(event);
    throw_no_cuda();
#లేకపోతే
    cuda_సురక్షిత_Call( cuda_ప్రవాహం_Wait_Event(impl_->stream, EventAccessor::getEvent(event), 0) );
#అయితే_ముగింపు
}

#ఒకవేళ defined(HAVE_CUDA) && (CUDART_సంస్కరణ >= 5000)

నామాకాశ
{
    నిర్మాణం Callback_దత్తాంశం
    {
        ప్రవాహం::ప్రవాహం_Callback callback;
        శూన్య* user_దత్తాంశం;

        Callback_దత్తాంశం(ప్రవాహం::ప్రవాహం_Callback callback_, శూన్య* user_దత్తాంశం_2) : callback(callback_), user_దత్తాంశం(user_దత్తాంశం_2) {}
    };

    శూన్య CUDART_CB cuda_ప్రవాహం_Callback(cuda_ప్రవాహం_t, cudaError_t స్థితి_2, శూన్య* user_దత్తాంశం)
    {
        Callback_దత్తాంశం* దత్తాంశం = reinterpret_cast<Callback_దత్తాంశం*>(user_దత్తాంశం);
        దత్తాంశం->callback(static_cast<పూర్ణాంకం>(స్థితి_2), దత్తాంశం->user_దత్తాంశం);
        తొలగించు దత్తాంశం;
    }
}

#అయితే_ముగింపు

శూన్య cv::cuda::ప్రవాహం::వరుసలో_చేర్చు_Host_Callback(ప్రవాహం_Callback callback, శూన్య* user_దత్తాంశం)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(callback);
    CV_వాడని(user_దత్తాంశం);
    throw_no_cuda();
#లేకపోతే
    #ఒకవేళ CUDART_సంస్కరణ < 5000
        CV_వాడని(callback);
        CV_వాడని(user_దత్తాంశం);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #లేకపోతే
        Callback_దత్తాంశం* దత్తాంశం = కొత్త Callback_దత్తాంశం(callback, user_దత్తాంశం);

        cuda_సురక్షిత_Call( cuda_ప్రవాహం_కూడు_Callback(impl_->stream, cuda_ప్రవాహం_Callback, దత్తాంశం, 0) );
    #అయితే_ముగింపు
#అయితే_ముగింపు
}

ప్రవాహం& cv::cuda::ప్రవాహం::Null()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం deviceId = getDevice();
    తిరిగి initializer.get_Null_ప్రవాహం(deviceId);
#అయితే_ముగింపు
}

శూన్య* cv::cuda::ప్రవాహం::cudaPtr() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    తిరిగి శూన్యసూచిక;
#లేకపోతే
    తిరిగి impl_->stream;
#అయితే_ముగింపు
}

cv::cuda::ప్రవాహం::క్రియాకారకం bool_type() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    తిరిగి 0;
#లేకపోతే
    తిరిగి (impl_->stream != 0) ? &ప్రవాహం::this_type_does_not_support_comparisons : 0;
#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_CUDA

cuda_ప్రవాహం_t cv::cuda::ప్రవాహం_Accessor::get_ప్రవాహం(స్థిరరాశి ప్రవాహం& stream)
{
    తిరిగి stream.impl_->stream;
}

ప్రవాహం cv::cuda::ప్రవాహం_Accessor::wrap_ప్రవాహం(cuda_ప్రవాహం_t stream)
{
    తిరిగి ప్రవాహం(makePtr<ప్రవాహం::Impl>(stream));
}

#అయితే_ముగింపు

ప్రవాహం cv::cuda::wrap_ప్రవాహం(size_t cuda_ప్రవాహం_జ్ఞాపకం_చిరునామా) {
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(cuda_ప్రవాహం_జ్ఞాపకం_చిరునామా);
    throw_no_cuda();
#లేకపోతే
    తిరిగి cv::cuda::ప్రవాహం_Accessor::wrap_ప్రవాహం(reinterpret_cast<cuda_ప్రవాహం_t>(cuda_ప్రవాహం_జ్ఞాపకం_చిరునామా));
#అయితే_ముగింపు
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ
{
    తార్కిక enable_జ్ఞాపకం_Pool = అబద్ధం;

    తరగతి దొంతర_Allocator : బహిరంగ GpuMat::Allocator
    {
    బహిరంగ:
        స్పష్టమైన దొంతర_Allocator(cuda_ప్రవాహం_t stream);
        ~దొంతర_Allocator();

        తార్కిక allocate(GpuMat* mat, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, size_t elem_కొలత) CV_OVERRIDE;
        శూన్య విడుదల(GpuMat* mat) CV_OVERRIDE;

    అంతరంగ:
        దొంతర_Allocator(స్థిరరాశి దొంతర_Allocator&);
        దొంతర_Allocator& క్రియాకారకం =(స్థిరరాశి దొంతర_Allocator&);

        cuda_ప్రవాహం_t ప్రవాహం_3;
        జ్ఞాపకం_దొంతర* mem_దొంతర_2;
        size_t alignment_;
    };

    దొంతర_Allocator::దొంతర_Allocator(cuda_ప్రవాహం_t stream) : ప్రవాహం_3(stream), mem_దొంతర_2(0)
    {
        ఒకవేళ (enable_జ్ఞాపకం_Pool)
        {
            స్థిరరాశి పూర్ణాంకం deviceId = getDevice();
            mem_దొంతర_2 = initializer.get_జ్ఞాపకం_Pool(deviceId).get_Free_Mem_దొంతర();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    దొంతర_Allocator::~దొంతర_Allocator()
    {
        ఒకవేళ (mem_దొంతర_2 != 0)
        {
            cuda_ప్రవాహం_Synchronize(ప్రవాహం_3);
            mem_దొంతర_2->pool->return_Mem_దొంతర(mem_దొంతర_2);
        }
    }

    size_t align_పైకి(size_t what, size_t alignment)
    {
        size_t align_ముసుగు = alignment-1;
        size_t inverse_Align_ముసుగు = ~align_ముసుగు;
        size_t res = (what + align_ముసుగు) & inverse_Align_ముసుగు;
        తిరిగి res;
    }

    తార్కిక దొంతర_Allocator::allocate(GpuMat* mat, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, size_t elem_కొలత)
    {
        ఒకవేళ (mem_దొంతర_2 == 0)
            తిరిగి అబద్ధం;

        size_t pitch, mem_కొలత;

        ఒకవేళ (అడ్డువరుసలు > 1 && నిలువుల_సంఖ్య > 1)
        {
            pitch = align_పైకి(నిలువుల_సంఖ్య * elem_కొలత, alignment_);
            mem_కొలత = pitch * అడ్డువరుసలు;
        }
        లేకపోతే
        {
            // Single row or single column must be continuous
            pitch = elem_కొలత * నిలువుల_సంఖ్య;
            mem_కొలత = align_పైకి(elem_కొలత * నిలువుల_సంఖ్య * అడ్డువరుసలు, 64);
        }

        uchar* ptr = mem_దొంతర_2->request_జ్ఞాపకం(mem_కొలత);

        ఒకవేళ (ptr == 0)
            తిరిగి అబద్ధం;

        mat->దత్తాంశం = ptr;
        mat->అడుగు = pitch;
        mat->refcount = (పూర్ణాంకం*) fastMalloc(పరిమాణం(పూర్ణాంకం));

        తిరిగి నిజం;
    }

    శూన్య దొంతర_Allocator::విడుదల(GpuMat* mat)
    {
        ఒకవేళ (mem_దొంతర_2 == 0)
            తిరిగి;

        mem_దొంతర_2->return_జ్ఞాపకం(mat->datastart);
        fastFree(mat->refcount);
    }
}

#అయితే_ముగింపు

/////////////////////////////////////////////////////////////
/// BufferPool

శూన్య cv::cuda::సమితి_బఫరు_Pool_Usage(తార్కిక on)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(on);
    throw_no_cuda();
#లేకపోతే
    enable_జ్ఞాపకం_Pool = on;
#అయితే_ముగింపు
}

శూన్య cv::cuda::సమితి_బఫరు_Pool_Config(పూర్ణాంకం deviceId, size_t దొంతర_కొలత, పూర్ణాంకం దొంతర_లెక్క)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(deviceId);
    CV_వాడని(దొంతర_కొలత);
    CV_వాడని(దొంతర_లెక్క);
    throw_no_cuda();
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం currentDevice = getDevice();

    ఒకవేళ (deviceId >= 0)
    {
        సమితి_Device(deviceId);
        initializer.get_జ్ఞాపకం_Pool(deviceId).initialize(దొంతర_కొలత, దొంతర_లెక్క);
    }
    లేకపోతే
    {
        స్థిరరాశి పూర్ణాంకం device_లెక్క = get_Cuda_Enabled_Device_లెక్క();

        కొరకు (deviceId = 0; deviceId < device_లెక్క; ++deviceId)
        {
            సమితి_Device(deviceId);
            initializer.get_జ్ఞాపకం_Pool(deviceId).initialize(దొంతర_కొలత, దొంతర_లెక్క);
        }
    }

    సమితి_Device(currentDevice);
#అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే HAVE_CUDA
cv::cuda::బఫరు_Pool::బఫరు_Pool(ప్రవాహం& stream)
{
    CV_వాడని(stream);
    throw_no_cuda();
}
#లేకపోతే
cv::cuda::బఫరు_Pool::బఫరు_Pool(ప్రవాహం& stream) : allocator_(stream.impl_->allocator)
{
}
#అయితే_ముగింపు

GpuMat cv::cuda::బఫరు_Pool::get_బఫరు(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(అడ్డువరుసలు);
    CV_వాడని(నిలువుల_సంఖ్య);
    CV_వాడని(type);
    throw_no_cuda();
#లేకపోతే
    GpuMat buf(allocator_);
    buf.create(అడ్డువరుసలు, నిలువుల_సంఖ్య, type);
    తిరిగి buf;
#అయితే_ముగింపు
}


////////////////////////////////////////////////////////////////
// Event

#నిర్వచితం_కాకపోతే HAVE_CUDA

తరగతి cv::cuda::Event::Impl
{
బహిరంగ:
    Impl(గుర్తులేని పూర్ణాంకం)
    {
        throw_no_cuda();
    }
};

#లేకపోతే

తరగతి cv::cuda::Event::Impl
{
బహిరంగ:
    cudaEvent_t event;
    తార్కిక ownEvent;

    స్పష్టమైన Impl(గుర్తులేని పూర్ణాంకం flags);
    స్పష్టమైన Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(గుర్తులేని పూర్ణాంకం flags) : event(0), ownEvent(అబద్ధం)
{
    cuda_సురక్షిత_Call( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = నిజం;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(అబద్ధం)
{
}

cv::cuda::Event::Impl::~Impl()
{
    ఒకవేళ (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(స్థిరరాశి Event& event)
{
    తిరిగి event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    తిరిగి Event(makePtr<Event::Impl>(event));
}

#అయితే_ముగింపు

cv::cuda::Event::Event(స్థిరరాశి Event::CreateFlags flags)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(flags);
    throw_no_cuda();
#లేకపోతే
    impl_ = makePtr<Impl>(flags);
#అయితే_ముగింపు
}

శూన్య cv::cuda::Event::రికార్డు(ప్రవాహం& stream)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(stream);
    throw_no_cuda();
#లేకపోతే
    cuda_సురక్షిత_Call( cuda_Event_రికార్డు(impl_->event, ప్రవాహం_Accessor::get_ప్రవాహం(stream)) );
#అయితే_ముగింపు
}

తార్కిక cv::cuda::Event::queryIfComplete() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    cudaError_t err = cudaEventQuery(impl_->event);

    ఒకవేళ (err == cudaErrorNotReady || err == cudaSuccess)
        తిరిగి err == cudaSuccess;

    cuda_సురక్షిత_Call(err);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

శూన్య cv::cuda::Event::waitForCompletion()
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    cuda_సురక్షిత_Call( cudaEventSynchronize(impl_->event) );
#అయితే_ముగింపు
}

దశాంశం cv::cuda::Event::elapsed_సమయం(స్థిరరాశి Event& ప్రారంభం, స్థిరరాశి Event& end)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(ప్రారంభం);
    CV_వాడని(end);
    throw_no_cuda();
#లేకపోతే
    దశాంశం ms;
    cuda_సురక్షిత_Call( cuda_Event_Elapsed_సమయం(&ms, ప్రారంభం.impl_->event, end.impl_->event) );
    తిరిగి ms;
#అయితే_ముగింపు
}
