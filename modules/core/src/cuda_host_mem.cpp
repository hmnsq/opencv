/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
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
#ਅਨ੍ਤਰ੍ਭਾਵ <map>

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::cuda;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA

ਨਾਮਾਕਾਸ਼ {

ਵਰਗ HostMemAllocator : ਜਨਤਕ MatAllocator
{
ਜਨਤਕ:
    ਸਪਸ਼ਟ HostMemAllocator(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags) : flags_(flags)
    {
    }

    UMat_ਅੰਕੜੇ* allocate(ਪੂਰਨ dims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type,
                       ਖਾਲੀ* data0, size_t* ਕਦਮ,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ਅਚਲ CV_OVERRIDE
    {
        size_t ਕੁੱਲ = CV_ELEM_ਮਾਪ(type);
        ਲਈ (ਪੂਰਨ i = dims-1; i >= 0; i--)
        {
            ਜੇ (ਕਦਮ)
            {
                ਜੇ (data0 && ਕਦਮ[i] != CV_AUTOSTEP)
                {
                    CV_Assert(ਕੁੱਲ <= ਕਦਮ[i]);
                    ਕੁੱਲ = ਕਦਮ[i];
                }
                ਨਹੀਂਤਾਂ
                {
                    ਕਦਮ[i] = ਕੁੱਲ;
                }
            }

            ਕੁੱਲ *= sizes[i];
        }

        UMat_ਅੰਕੜੇ* u = ਨਵਾਂ UMat_ਅੰਕੜੇ(ਇਹ);
        u->size = ਕੁੱਲ;

        ਜੇ (data0)
        {
            u->ਅੰਕੜੇ = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_ਅੰਕੜੇ::USER_ALLOCATED;
        }
        ਨਹੀਂਤਾਂ
        {
            ਖਾਲੀ* ptr = 0;
            cuda_ਸੁਰੱਖਿਅਤ_Call( cudaHostAlloc(&ptr, ਕੁੱਲ, flags_) );

            u->ਅੰਕੜੇ = u->origdata = static_cast<uchar*>(ptr);
        }

        ਵਾਪਸ u;
    }

    ਤਾਰਕਿਕ allocate(UMat_ਅੰਕੜੇ* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ਅਚਲ CV_OVERRIDE
    {
        ਵਾਪਸ (u != ਨਿਰਾ);
    }

    ਖਾਲੀ deallocate(UMat_ਅੰਕੜੇ* u) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ (!u)
            ਵਾਪਸ;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        ਜੇ (u->refcount == 0)
        {
            ਜੇ ( !(u->flags & UMat_ਅੰਕੜੇ::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            ਮਿਟਾਓ u;
        }
    }

ਨਿਜੀ:
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags_;
};

} // namespace

#ਜੇ_ਅੰਤ

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(alloc_type);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਸਥਿਰ std::map<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, Ptr<MatAllocator> > allocators;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flag = cudaHostAllocDefault;

    ਚੋਣ (alloc_type)
    {
    ਹਾਲਤ ਪੰਨਾ_LOCKED:    flag = cudaHostAllocDefault; ਰੋਕੋ;
    ਹਾਲਤ SHARED:         flag = cudaHostAllocMapped;  ਰੋਕੋ;
    ਹਾਲਤ WRITE_COMBINED: flag = cudaHostAllocWriteCombined; ਰੋਕੋ;
    ਮੂਲ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    ਜੇ (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    ਵਾਪਸ a.get();
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
ਨਾਮਾਕਾਸ਼
{
    size_t align_ਉੱਪਰ_ਕਦਮ(size_t what, size_t alignment)
    {
        size_t align_ਨਕਾਬ = alignment - 1;
        size_t inverse_Align_ਨਕਾਬ = ~align_ਨਕਾਬ;
        size_t res = (what + align_ਨਕਾਬ) & inverse_Align_ਨਕਾਬ;
        ਵਾਪਸ res;
    }
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::cuda::HostMem::create(ਪੂਰਨ ਪੰਕਤੀਆਂ_2, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_2, ਪੂਰਨ type_)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(ਪੰਕਤੀਆਂ_2);
    CV_ਅਣਵਰਤਿਆ(ਖਾਨੇ_ਗਿਣਤੀ_2);
    CV_ਅਣਵਰਤਿਆ(type_);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    ਜੇ (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_ਸਮ੍ਰਿਤੀ() );
    }

    type_ &= Mat::TYPE_ਨਕਾਬ;

    ਜੇ (ਪੰਕਤੀਆਂ == ਪੰਕਤੀਆਂ_2 && ਖਾਨੇ_ਗਿਣਤੀ == ਖਾਨੇ_ਗਿਣਤੀ_2 && type() == type_ && ਅੰਕੜੇ)
        ਵਾਪਸ;

    ਜੇ (ਅੰਕੜੇ)
        release();

    CV_DbgAssert( ਪੰਕਤੀਆਂ_2 >= 0 && ਖਾਨੇ_ਗਿਣਤੀ_2 >= 0 );

    ਜੇ (ਪੰਕਤੀਆਂ_2 > 0 && ਖਾਨੇ_ਗਿਣਤੀ_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ_2;
        ਖਾਨੇ_ਗਿਣਤੀ = ਖਾਨੇ_ਗਿਣਤੀ_2;
        ਕਦਮ = elem_ਮਾਪ() * ਖਾਨੇ_ਗਿਣਤੀ;
        ਪੂਰਨ sz[] = { ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ };
        size_t ਕਦਮਾਂ[] = { ਕਦਮ, (size_t)CV_ELEM_ਮਾਪ(type_) };
        flags = updateContinuityFlag(flags, 2, sz, ਕਦਮਾਂ);

        ਜੇ (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            ਕਦਮ = align_ਉੱਪਰ_ਕਦਮ(ਕਦਮ, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)ਕਦਮ*ਪੰਕਤੀਆਂ;
        size_t nettosize = (size_t)_nettosize;

        ਜੇ (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_ਮਾਪ(nettosize, (ਪੂਰਨ)ਆਕਾਰ(*refcount));

        ਖਾਲੀ* ptr = 0;

        ਚੋਣ (alloc_type)
        {
        ਹਾਲਤ ਪੰਨਾ_LOCKED:    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); ਰੋਕੋ;
        ਹਾਲਤ SHARED:         cuda_ਸੁਰੱਖਿਅਤ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  ਰੋਕੋ;
        ਹਾਲਤ WRITE_COMBINED: cuda_ਸੁਰੱਖਿਅਤ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); ਰੋਕੋ;
        ਮੂਲ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = ਅੰਕੜੇ =  (uchar*)ptr;
        dataend = ਅੰਕੜੇ + nettosize;

        refcount = (ਪੂਰਨ*)cv::fastMalloc(ਆਕਾਰ(*refcount));
        *refcount = 1;
    }
#ਜੇ_ਅੰਤ
}

HostMem cv::cuda::HostMem::reshape(ਪੂਰਨ new_cn, ਪੂਰਨ new_ਪੰਕਤੀਆਂ) ਅਚਲ
{
    HostMem hdr = *ਇਹ;

    ਪੂਰਨ cn = channels();
    ਜੇ (new_cn == 0)
        new_cn = cn;

    ਪੂਰਨ ਕੁੱਲ_ਚੌੜਾਈ = ਖਾਨੇ_ਗਿਣਤੀ * cn;

    ਜੇ ((new_cn > ਕੁੱਲ_ਚੌੜਾਈ || ਕੁੱਲ_ਚੌੜਾਈ % new_cn != 0) && new_ਪੰਕਤੀਆਂ == 0)
        new_ਪੰਕਤੀਆਂ = ਪੰਕਤੀਆਂ * ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ (new_ਪੰਕਤੀਆਂ != 0 && new_ਪੰਕਤੀਆਂ != ਪੰਕਤੀਆਂ)
    {
        ਪੂਰਨ ਕੁੱਲ_ਮਾਪ_2 = ਕੁੱਲ_ਚੌੜਾਈ * ਪੰਕਤੀਆਂ;

        ਜੇ (!isContinuous())
            CV_Error(cv::Error::Bad_ਕਦਮ, "The matrix is not continuous, thus its number of rows can not be changed");

        ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ)new_ਪੰਕਤੀਆਂ > (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਕੁੱਲ_ਮਾਪ_2)
            CV_Error(cv::Error::Sts_ਬਾਹਰਲਾ_ਦਾ_Range, "Bad new number of rows");

        ਕੁੱਲ_ਚੌੜਾਈ = ਕੁੱਲ_ਮਾਪ_2 / new_ਪੰਕਤੀਆਂ;

        ਜੇ (ਕੁੱਲ_ਚੌੜਾਈ * new_ਪੰਕਤੀਆਂ != ਕੁੱਲ_ਮਾਪ_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.ਪੰਕਤੀਆਂ = new_ਪੰਕਤੀਆਂ;
        hdr.ਕਦਮ = ਕੁੱਲ_ਚੌੜਾਈ * elemSize1();
    }

    ਪੂਰਨ new_ਚੌੜਾਈ = ਕੁੱਲ_ਚੌੜਾਈ / new_cn;

    ਜੇ (new_ਚੌੜਾਈ * new_cn != ਕੁੱਲ_ਚੌੜਾਈ)
        CV_Error(cv::Error::Bad_ਅੰਕ_Channels, "The total width is not divisible by the new number of channels");

    hdr.ਖਾਨੇ_ਗਿਣਤੀ = new_ਚੌੜਾਈ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ਨਕਾਬ) | ((new_cn - 1) << CV_CN_ਖਿਸਕਾਓ);

    ਵਾਪਸ hdr;
}

ਖਾਲੀ cv::cuda::HostMem::release()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਜੇ (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = ਅੰਕੜੇ = datastart = 0;
    ਕਦਮ = ਪੰਕਤੀਆਂ = ਖਾਨੇ_ਗਿਣਤੀ = 0;
    refcount = 0;
#ਜੇ_ਅੰਤ
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_ਸਿਰਲੇਖ() ਅਚਲ
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    CV_Assert( alloc_type == SHARED );

    ਖਾਲੀ *pdev;
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Host_Get_Device_ਸੂਚਕ(&pdev, ਅੰਕੜੇ, 0) );

    ਵਾਪਸ GpuMat(ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ, type(), pdev, ਕਦਮ);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::ਰਜਿਸਟਰ_ਪੰਨਾ_Locked(Mat& m)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(m);
    throw_no_cuda();
#ਨਹੀਂਤਾਂ
    CV_Assert( m.isContinuous() );
    cuda_ਸੁਰੱਖਿਅਤ_Call( cuda_Host_ਰਜਿਸਟਰ(m.ਅੰਕੜੇ, m.ਕਦਮ * m.ਪੰਕਤੀਆਂ, cuda_Host_ਰਜਿਸਟਰ_Portable) );
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ cv::cuda::unregister_ਪੰਨਾ_Locked(Mat& m)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    CV_ਅਣਵਰਤਿਆ(m);
#ਨਹੀਂਤਾਂ
    cuda_ਸੁਰੱਖਿਅਤ_Call( cudaHostUnregister(m.ਅੰਕੜੇ) );
#ਜੇ_ਅੰਤ
}
