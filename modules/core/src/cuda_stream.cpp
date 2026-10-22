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

#समावेश "precomp.hpp"
#समावेश <cstdint>

using namespace cv;
using namespace cv::cuda;

#अगर defined(_MSC_VER)
#आशय warning(disable : 4702)  // unreachable code
#पूर्ण_अगर

/////////////////////////////////////////////////////////////
/// MemoryStack

#अगर_घोषित HAVE_CUDA

namespace
{
    श्रेणी MemoryPool;

    श्रेणी Memory_ढेर
    {
    खुला:
        uchar* requestMemory(माप_प्रकार size);
        व्योम returnMemory(uchar* ptr);

        uchar* datastart;
        uchar* dataend;
        uchar* tip;

        bool isFree;
        MemoryPool* pool;

    #अगर !defined(न_संशोधन)
        std::vector<माप_प्रकार> allocations;
    #पूर्ण_अगर
    };

    uchar* Memory_ढेर::requestMemory(माप_प्रकार size)
    {
        स्थिर माप_प्रकार freeMem = dataend - tip;

        अगर (size > freeMem)
            वापस 0;

        uchar* ptr = tip;

        tip += size;

    #अगर !defined(न_संशोधन)
        allocations.push_back(size);
    #पूर्ण_अगर

        वापस ptr;
    }

    व्योम Memory_ढेर::returnMemory(uchar* ptr)
    {
        CV_DbgAssert( ptr >= datastart && ptr < dataend );

    #अगर !defined(न_संशोधन)
        स्थिर माप_प्रकार alloc_आकार = tip - ptr;
        CV_Assert( alloc_आकार == allocations.back() );
        allocations.pop_back();
    #पूर्ण_अगर

        tip = ptr;
    }
}

#पूर्ण_अगर

/////////////////////////////////////////////////////////////
/// MemoryPool

#अगर_घोषित HAVE_CUDA

namespace
{
    श्रेणी MemoryPool
    {
    खुला:
        MemoryPool();
        ~MemoryPool() { release(); }

        व्योम initialize(माप_प्रकार ढेर_आकार, पूर्णांक ढेर_गिनती);
        व्योम release();

        Memory_ढेर* get_Free_Mem_ढेर();
        व्योम return_Mem_ढेर(Memory_ढेर* mem_ढेर);

    निजी:
        व्योम initilizeImpl();

        Mutex mtx_;

        bool initialized_;
        माप_प्रकार ढेर_आकार_2;
        पूर्णांक ढेर_गिनती_2;

        uchar* mem_;

        std::vector<Memory_ढेर> stacks_;

        MemoryPool(स्थिर MemoryPool&); //= delete;
    };

    MemoryPool::MemoryPool() : initialized_(false), mem_(0)
    {
        // default : 10 Mb, 5 stacks
        ढेर_आकार_2 = 10 * 1024 * 1024;
        ढेर_गिनती_2 = 5;
    }

    व्योम MemoryPool::initialize(माप_प्रकार ढेर_आकार, पूर्णांक ढेर_गिनती)
    {
        AutoLock lock(mtx_);

        release();

        ढेर_आकार_2 = ढेर_आकार;
        ढेर_गिनती_2 = ढेर_गिनती;

        initilizeImpl();
    }

    व्योम MemoryPool::initilizeImpl()
    {
        स्थिर माप_प्रकार योग_आकार = ढेर_आकार_2 * ढेर_गिनती_2;

        अगर (योग_आकार > 0)
        {
            cudaError_t err = cudaMalloc(&mem_, योग_आकार);
            अगर (err != cudaSuccess)
                वापस;

            stacks_.resize(ढेर_गिनती_2);

            uchar* ptr = mem_;

            क्रम (पूर्णांक i = 0; i < ढेर_गिनती_2; ++i)
            {
                stacks_[i].datastart = ptr;
                stacks_[i].dataend = ptr + ढेर_आकार_2;
                stacks_[i].tip = ptr;
                stacks_[i].isFree = true;
                stacks_[i].pool = यह;

                ptr += ढेर_आकार_2;
            }

            initialized_ = true;
        }
    }

    व्योम MemoryPool::release()
    {
        अगर (mem_)
        {
#अगर !defined(न_संशोधन)
            क्रम (पूर्णांक i = 0; i < ढेर_गिनती_2; ++i)
            {
                CV_DbgAssert( stacks_[i].isFree );
                CV_DbgAssert( stacks_[i].tip == stacks_[i].datastart );
            }
#पूर्ण_अगर

            cudaFree(mem_);

            mem_ = 0;
            initialized_ = false;
        }
    }

    Memory_ढेर* MemoryPool::get_Free_Mem_ढेर()
    {
        AutoLock lock(mtx_);

        अगर (!initialized_)
            initilizeImpl();

        अगर (!mem_)
            वापस 0;

        क्रम (पूर्णांक i = 0; i < ढेर_गिनती_2; ++i)
        {
            अगर (stacks_[i].isFree)
            {
                stacks_[i].isFree = false;
                वापस &stacks_[i];
            }
        }

        वापस 0;
    }

    व्योम MemoryPool::return_Mem_ढेर(Memory_ढेर* mem_ढेर)
    {
        AutoLock lock(mtx_);

        CV_DbgAssert( !mem_ढेर->isFree );

#अगर !defined(न_संशोधन)
        bool found = false;
        क्रम (पूर्णांक i = 0; i < ढेर_गिनती_2; ++i)
        {
            अगर (mem_ढेर == &stacks_[i])
            {
                found = true;
                अवरोध;
            }
        }
        CV_DbgAssert( found );
#पूर्ण_अगर

        CV_DbgAssert( mem_ढेर->tip == mem_ढेर->datastart );

        mem_ढेर->isFree = true;
    }
}

#पूर्ण_अगर

////////////////////////////////////////////////////////////////
/// Stream::Impl

#अगर_अघोषित HAVE_CUDA

श्रेणी cv::cuda::Stream::Impl
{
खुला:
    Impl(व्योम* ptr = 0)
    {
        CV_UNUSED(ptr);
        throw_no_cuda();
    }
};

#अन्यथा

namespace
{
    श्रेणी ढेर_Allocator;
}

श्रेणी cv::cuda::Stream::Impl
{
खुला:
    cudaStream_t stream;
    bool ownStream;

    Ptr<GpuMat::Allocator> allocator;

    Impl();
    Impl(स्थिर Ptr<GpuMat::Allocator>& allocator);
    Impl(स्थिर अचिन्हित पूर्णांक cudaFlags);
    explicit Impl(cudaStream_t stream);

    ~Impl();
};

cv::cuda::Stream::Impl::Impl() : stream(0), ownStream(false)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = true;

    allocator = makePtr<ढेर_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(स्थिर Ptr<GpuMat::Allocator>& allocator) : stream(0), ownStream(false), allocator(allocator)
{
    cudaSafeCall( cudaStreamCreate(&stream) );
    ownStream = true;
}

cv::cuda::Stream::Impl::Impl(स्थिर अचिन्हित पूर्णांक cudaFlags) : stream(0), ownStream(false)
{
    cudaSafeCall(cudaStreamCreateWithFlags(&stream, cudaFlags));
    ownStream = true;
    allocator = makePtr<ढेर_Allocator>(stream);
}

cv::cuda::Stream::Impl::Impl(cudaStream_t stream_) : stream(stream_), ownStream(false)
{
    allocator = makePtr<ढेर_Allocator>(stream);
}

cv::cuda::Stream::Impl::~Impl()
{
    allocator.release();

    अगर (stream && ownStream)
    {
        cudaStreamDestroy(stream);
    }
}

#पूर्ण_अगर

/////////////////////////////////////////////////////////////
/// DefaultDeviceInitializer

#अगर_घोषित HAVE_CUDA

namespace cv { namespace cuda
{
    श्रेणी DefaultDeviceInitializer
    {
    खुला:
        DefaultDeviceInitializer();
        ~DefaultDeviceInitializer();

        Stream& getNullStream(पूर्णांक deviceId);
        MemoryPool& getMemoryPool(पूर्णांक deviceId);

    निजी:
        व्योम initStreams();
        व्योम initPools();

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
        streams_.सफा();

        क्रम (माप_प्रकार i = 0; i < pools_.size(); ++i)
        {
            cudaSetDevice(static_cast<पूर्णांक>(i));
            pools_[i]->release();
        }

        pools_.सफा();
    }

    Stream& DefaultDeviceInitializer::getNullStream(पूर्णांक deviceId)
    {
        AutoLock lock(streams_mtx_);

        अगर (streams_.empty())
        {
            पूर्णांक device_गिनती = get_Cuda_Enabled_Device_गिनती();

            अगर (device_गिनती > 0)
                streams_.resize(device_गिनती);
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णांक>(streams_.size()) );

        अगर (streams_[deviceId].empty())
        {
            cudaStream_t stream = शून्य;
            Ptr<Stream::Impl> impl = makePtr<Stream::Impl>(stream);
            streams_[deviceId] = Ptr<Stream>(नया Stream(impl));
        }

        वापस *streams_[deviceId];
    }

    MemoryPool& DefaultDeviceInitializer::getMemoryPool(पूर्णांक deviceId)
    {
        AutoLock lock(pools_mtx_);

        अगर (pools_.empty())
        {
            पूर्णांक device_गिनती = get_Cuda_Enabled_Device_गिनती();

            अगर (device_गिनती > 0)
            {
                pools_.resize(device_गिनती);
                क्रम (माप_प्रकार i = 0; i < pools_.size(); ++i)
                {
                    cudaSetDevice(static_cast<पूर्णांक>(i));
                    pools_[i] = makePtr<MemoryPool>();
                }
            }
        }

        CV_DbgAssert( deviceId >= 0 && deviceId < static_cast<पूर्णांक>(pools_.size()) );

        MemoryPool* p = pools_[deviceId];
        CV_Assert(p);
        वापस *p;
    }

    DefaultDeviceInitializer initializer;
}}

#पूर्ण_अगर

/////////////////////////////////////////////////////////////
/// Stream

cv::cuda::Stream::Stream()
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>();
#पूर्ण_अगर
}

cv::cuda::Stream::Stream(स्थिर Ptr<GpuMat::Allocator>& allocator)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(allocator);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(allocator);
#पूर्ण_अगर
}

cv::cuda::Stream::Stream(स्थिर माप_प्रकार cudaFlags)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(cudaFlags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(cudaFlags & अच_पूर्णांक_उच्च);
#पूर्ण_अगर
}

bool cv::cuda::Stream::queryIfComplete() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaStreamQuery(impl_->stream);

    अगर (err == cudaErrorNotReady || err == cudaSuccess)
        वापस err == cudaSuccess;

    cudaSafeCall(err);
    वापस false;
#पूर्ण_अगर
}

व्योम cv::cuda::Stream::waitForCompletion()
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaStreamSynchronize(impl_->stream) );
#पूर्ण_अगर
}

व्योम cv::cuda::Stream::waitEvent(स्थिर Event& event)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(event);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaStreamWaitEvent(impl_->stream, EventAccessor::getEvent(event), 0) );
#पूर्ण_अगर
}

#अगर defined(HAVE_CUDA) && (CUDART_VERSION >= 5000)

namespace
{
    काष्ठा Callback_तथ्य
    {
        Stream::StreamCallback callback;
        व्योम* user_तथ्य;

        Callback_तथ्य(Stream::StreamCallback callback_, व्योम* user_तथ्य_2) : callback(callback_), user_तथ्य(user_तथ्य_2) {}
    };

    व्योम CUDART_CB cudaStreamCallback(cudaStream_t, cudaError_t status, व्योम* user_तथ्य)
    {
        Callback_तथ्य* तथ्य = reinterpret_cast<Callback_तथ्य*>(user_तथ्य);
        तथ्य->callback(static_cast<पूर्णांक>(status), तथ्य->user_तथ्य);
        मिटाओ तथ्य;
    }
}

#पूर्ण_अगर

व्योम cv::cuda::Stream::enqueueHostCallback(StreamCallback callback, व्योम* user_तथ्य)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(callback);
    CV_UNUSED(user_तथ्य);
    throw_no_cuda();
#अन्यथा
    #अगर CUDART_VERSION < 5000
        CV_UNUSED(callback);
        CV_UNUSED(user_तथ्य);
        CV_Error(cv::Error::StsNotImplemented, "This function requires CUDA >= 5.0");
    #अन्यथा
        Callback_तथ्य* तथ्य = नया Callback_तथ्य(callback, user_तथ्य);

        cudaSafeCall( cudaStreamAddCallback(impl_->stream, cudaStreamCallback, तथ्य, 0) );
    #पूर्ण_अगर
#पूर्ण_अगर
}

Stream& cv::cuda::Stream::Null()
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    स्थिर पूर्णांक deviceId = getDevice();
    वापस initializer.getNullStream(deviceId);
#पूर्ण_अगर
}

व्योम* cv::cuda::Stream::cudaPtr() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    वापस nullptr;
#अन्यथा
    वापस impl_->stream;
#पूर्ण_अगर
}

cv::cuda::Stream::चालक bool_type() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    वापस 0;
#अन्यथा
    वापस (impl_->stream != 0) ? &Stream::this_type_does_not_support_comparisons : 0;
#पूर्ण_अगर
}

#अगर_घोषित HAVE_CUDA

cudaStream_t cv::cuda::StreamAccessor::getStream(स्थिर Stream& stream)
{
    वापस stream.impl_->stream;
}

Stream cv::cuda::StreamAccessor::wrapStream(cudaStream_t stream)
{
    वापस Stream(makePtr<Stream::Impl>(stream));
}

#पूर्ण_अगर

Stream cv::cuda::wrapStream(माप_प्रकार cudaStreamMemoryAddress) {
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(cudaStreamMemoryAddress);
    throw_no_cuda();
#अन्यथा
    वापस cv::cuda::StreamAccessor::wrapStream(reinterpret_cast<cudaStream_t>(cudaStreamMemoryAddress));
#पूर्ण_अगर
}

/////////////////////////////////////////////////////////////
/// StackAllocator

#अगर_घोषित HAVE_CUDA

namespace
{
    bool enableMemoryPool = false;

    श्रेणी ढेर_Allocator : खुला GpuMat::Allocator
    {
    खुला:
        explicit ढेर_Allocator(cudaStream_t stream);
        ~ढेर_Allocator();

        bool allocate(GpuMat* mat, पूर्णांक rows, पूर्णांक cols, माप_प्रकार elem_आकार) CV_OVERRIDE;
        व्योम मुक्त(GpuMat* mat) CV_OVERRIDE;

    निजी:
        ढेर_Allocator(स्थिर ढेर_Allocator&);
        ढेर_Allocator& चालक =(स्थिर ढेर_Allocator&);

        cudaStream_t stream_;
        Memory_ढेर* mem_ढेर_2;
        माप_प्रकार alignment_;
    };

    ढेर_Allocator::ढेर_Allocator(cudaStream_t stream) : stream_(stream), mem_ढेर_2(0)
    {
        अगर (enableMemoryPool)
        {
            स्थिर पूर्णांक deviceId = getDevice();
            mem_ढेर_2 = initializer.getMemoryPool(deviceId).get_Free_Mem_ढेर();
            DeviceInfo devInfo(deviceId);
            alignment_ = devInfo.textureAlignment();
        }
    }

    ढेर_Allocator::~ढेर_Allocator()
    {
        अगर (mem_ढेर_2 != 0)
        {
            cudaStreamSynchronize(stream_);
            mem_ढेर_2->pool->return_Mem_ढेर(mem_ढेर_2);
        }
    }

    माप_प्रकार alignUp(माप_प्रकार what, माप_प्रकार alignment)
    {
        माप_प्रकार alignMask = alignment-1;
        माप_प्रकार inverseAlignMask = ~alignMask;
        माप_प्रकार res = (what + alignMask) & inverseAlignMask;
        वापस res;
    }

    bool ढेर_Allocator::allocate(GpuMat* mat, पूर्णांक rows, पूर्णांक cols, माप_प्रकार elem_आकार)
    {
        अगर (mem_ढेर_2 == 0)
            वापस false;

        माप_प्रकार pitch, mem_आकार;

        अगर (rows > 1 && cols > 1)
        {
            pitch = alignUp(cols * elem_आकार, alignment_);
            mem_आकार = pitch * rows;
        }
        अन्यथा
        {
            // Single row or single column must be continuous
            pitch = elem_आकार * cols;
            mem_आकार = alignUp(elem_आकार * cols * rows, 64);
        }

        uchar* ptr = mem_ढेर_2->requestMemory(mem_आकार);

        अगर (ptr == 0)
            वापस false;

        mat->तथ्य = ptr;
        mat->step = pitch;
        mat->refcount = (पूर्णांक*) fastMalloc(माप(पूर्णांक));

        वापस true;
    }

    व्योम ढेर_Allocator::मुक्त(GpuMat* mat)
    {
        अगर (mem_ढेर_2 == 0)
            वापस;

        mem_ढेर_2->returnMemory(mat->datastart);
        fastFree(mat->refcount);
    }
}

#पूर्ण_अगर

/////////////////////////////////////////////////////////////
/// BufferPool

व्योम cv::cuda::set_बफर_Pool_Usage(bool on)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(on);
    throw_no_cuda();
#अन्यथा
    enableMemoryPool = on;
#पूर्ण_अगर
}

व्योम cv::cuda::set_बफर_Pool_Config(पूर्णांक deviceId, माप_प्रकार ढेर_आकार, पूर्णांक ढेर_गिनती)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(deviceId);
    CV_UNUSED(ढेर_आकार);
    CV_UNUSED(ढेर_गिनती);
    throw_no_cuda();
#अन्यथा
    स्थिर पूर्णांक currentDevice = getDevice();

    अगर (deviceId >= 0)
    {
        setDevice(deviceId);
        initializer.getMemoryPool(deviceId).initialize(ढेर_आकार, ढेर_गिनती);
    }
    अन्यथा
    {
        स्थिर पूर्णांक device_गिनती = get_Cuda_Enabled_Device_गिनती();

        क्रम (deviceId = 0; deviceId < device_गिनती; ++deviceId)
        {
            setDevice(deviceId);
            initializer.getMemoryPool(deviceId).initialize(ढेर_आकार, ढेर_गिनती);
        }
    }

    setDevice(currentDevice);
#पूर्ण_अगर
}

#अगर_अघोषित HAVE_CUDA
cv::cuda::बफर_Pool::बफर_Pool(Stream& stream)
{
    CV_UNUSED(stream);
    throw_no_cuda();
}
#अन्यथा
cv::cuda::बफर_Pool::बफर_Pool(Stream& stream) : allocator_(stream.impl_->allocator)
{
}
#पूर्ण_अगर

GpuMat cv::cuda::बफर_Pool::get_बफर(पूर्णांक rows, पूर्णांक cols, पूर्णांक type)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(rows);
    CV_UNUSED(cols);
    CV_UNUSED(type);
    throw_no_cuda();
#अन्यथा
    GpuMat buf(allocator_);
    buf.create(rows, cols, type);
    वापस buf;
#पूर्ण_अगर
}


////////////////////////////////////////////////////////////////
// Event

#अगर_अघोषित HAVE_CUDA

श्रेणी cv::cuda::Event::Impl
{
खुला:
    Impl(अचिन्हित पूर्णांक)
    {
        throw_no_cuda();
    }
};

#अन्यथा

श्रेणी cv::cuda::Event::Impl
{
खुला:
    cudaEvent_t event;
    bool ownEvent;

    explicit Impl(अचिन्हित पूर्णांक flags);
    explicit Impl(cudaEvent_t event);
    ~Impl();
};

cv::cuda::Event::Impl::Impl(अचिन्हित पूर्णांक flags) : event(0), ownEvent(false)
{
    cudaSafeCall( cudaEventCreateWithFlags(&event, flags) );
    ownEvent = true;
}

cv::cuda::Event::Impl::Impl(cudaEvent_t e) : event(e), ownEvent(false)
{
}

cv::cuda::Event::Impl::~Impl()
{
    अगर (event && ownEvent)
    {
        cudaEventDestroy(event);
    }
}

cudaEvent_t cv::cuda::EventAccessor::getEvent(स्थिर Event& event)
{
    वापस event.impl_->event;
}

Event cv::cuda::EventAccessor::wrapEvent(cudaEvent_t event)
{
    वापस Event(makePtr<Event::Impl>(event));
}

#पूर्ण_अगर

cv::cuda::Event::Event(स्थिर Event::CreateFlags flags)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(flags);
    throw_no_cuda();
#अन्यथा
    impl_ = makePtr<Impl>(flags);
#पूर्ण_अगर
}

व्योम cv::cuda::Event::record(Stream& stream)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(stream);
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaEventRecord(impl_->event, StreamAccessor::getStream(stream)) );
#पूर्ण_अगर
}

bool cv::cuda::Event::queryIfComplete() स्थिर
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaError_t err = cudaEventQuery(impl_->event);

    अगर (err == cudaErrorNotReady || err == cudaSuccess)
        वापस err == cudaSuccess;

    cudaSafeCall(err);
    वापस false;
#पूर्ण_अगर
}

व्योम cv::cuda::Event::waitForCompletion()
{
#अगर_अघोषित HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    cudaSafeCall( cudaEventSynchronize(impl_->event) );
#पूर्ण_अगर
}

भग्न cv::cuda::Event::elapsedTime(स्थिर Event& start, स्थिर Event& end)
{
#अगर_अघोषित HAVE_CUDA
    CV_UNUSED(start);
    CV_UNUSED(end);
    throw_no_cuda();
#अन्यथा
    भग्न ms;
    cudaSafeCall( cudaEventElapsedTime(&ms, start.impl_->event, end.impl_->event) );
    वापस ms;
#पूर्ण_अगर
}
