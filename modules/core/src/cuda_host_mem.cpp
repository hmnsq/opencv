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

#ضمن "precomp.hpp"
#ضمن <map>

استعمال نطاق cv;
استعمال نطاق cv::cuda;

#جيڪڏهن_وصف_ٿيل HAVE_CUDA

نطاق {

صنف HostMemAllocator : عام MatAllocator
{
عام:
    واضح HostMemAllocator(بنانشان صحيح flags) : flags_(flags)
    {
    }

    UMat_معلومات* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* قدم,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t ڪل = CV_ELEM_سائيز(type);
        لكل (صحيح i = dims-1; i >= 0; i--)
        {
            إذا (قدم)
            {
                إذا (data0 && قدم[i] != CV_AUTOSTEP)
                {
                    CV_Assert(ڪل <= قدم[i]);
                    ڪل = قدم[i];
                }
                وإلا
                {
                    قدم[i] = ڪل;
                }
            }

            ڪل *= sizes[i];
        }

        UMat_معلومات* u = جديد UMat_معلومات(هذا);
        u->size = ڪل;

        إذا (data0)
        {
            u->معلومات = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_معلومات::USER_ALLOCATED;
        }
        وإلا
        {
            فراغ* ptr = 0;
            cuda_محفوظ_Call( cudaHostAlloc(&ptr, ڪل, flags_) );

            u->معلومات = u->origdata = static_cast<uchar*>(ptr);
        }

        أرجع u;
    }

    منطقي allocate(UMat_معلومات* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        أرجع (u != عدم);
    }

    فراغ deallocate(UMat_معلومات* u) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        إذا (u->refcount == 0)
        {
            إذا ( !(u->flags & UMat_معلومات::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            ميٽيو u;
        }
    }

خاص:
    بنانشان صحيح flags_;
};

} // namespace

#جيڪڏهن_پورو

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(alloc_type);
    throw_no_cuda();
#وإلا
    ساكن std::map<بنانشان صحيح, Ptr<MatAllocator> > allocators;

    بنانشان صحيح flag = cudaHostAllocDefault;

    بدل (alloc_type)
    {
    حالة صفحو_LOCKED:    flag = cudaHostAllocDefault; اكسر;
    حالة SHARED:         flag = cudaHostAllocMapped;  اكسر;
    حالة WRITE_COMBINED: flag = cudaHostAllocWriteCombined; اكسر;
    افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    إذا (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    أرجع a.get();
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل HAVE_CUDA
نطاق
{
    size_t align_مٿي_قدم(size_t what, size_t alignment)
    {
        size_t align_نقاب = alignment - 1;
        size_t inverse_Align_نقاب = ~align_نقاب;
        size_t res = (what + align_نقاب) & inverse_Align_نقاب;
        أرجع res;
    }
}
#جيڪڏهن_پورو

فراغ cv::cuda::HostMem::create(صحيح صفون_2, صحيح کانن_ڳڻپ_2, صحيح type_)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(صفون_2);
    CV_اڻ_استعمال_ٿيل(کانن_ڳڻپ_2);
    CV_اڻ_استعمال_ٿيل(type_);
    throw_no_cuda();
#وإلا
    إذا (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_يادگيري() );
    }

    type_ &= Mat::TYPE_نقاب;

    إذا (صفون == صفون_2 && کانن_ڳڻپ == کانن_ڳڻپ_2 && type() == type_ && معلومات)
        أرجع;

    إذا (معلومات)
        release();

    CV_DbgAssert( صفون_2 >= 0 && کانن_ڳڻپ_2 >= 0 );

    إذا (صفون_2 > 0 && کانن_ڳڻپ_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        صفون = صفون_2;
        کانن_ڳڻپ = کانن_ڳڻپ_2;
        قدم = elem_سائيز() * کانن_ڳڻپ;
        صحيح sz[] = { صفون, کانن_ڳڻپ };
        size_t قدمن[] = { قدم, (size_t)CV_ELEM_سائيز(type_) };
        flags = updateContinuityFlag(flags, 2, sz, قدمن);

        إذا (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            قدم = align_مٿي_قدم(قدم, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)قدم*صفون;
        size_t nettosize = (size_t)_nettosize;

        إذا (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_سائيز(nettosize, (صحيح)حجم(*refcount));

        فراغ* ptr = 0;

        بدل (alloc_type)
        {
        حالة صفحو_LOCKED:    cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); اكسر;
        حالة SHARED:         cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  اكسر;
        حالة WRITE_COMBINED: cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); اكسر;
        افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = معلومات =  (uchar*)ptr;
        dataend = معلومات + nettosize;

        refcount = (صحيح*)cv::fastMalloc(حجم(*refcount));
        *refcount = 1;
    }
#جيڪڏهن_پورو
}

HostMem cv::cuda::HostMem::reshape(صحيح new_cn, صحيح new_صفون) ثابت
{
    HostMem hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح ڪل_ويڪر = کانن_ڳڻپ * cn;

    إذا ((new_cn > ڪل_ويڪر || ڪل_ويڪر % new_cn != 0) && new_صفون == 0)
        new_صفون = صفون * ڪل_ويڪر / new_cn;

    إذا (new_صفون != 0 && new_صفون != صفون)
    {
        صحيح ڪل_سائيز_2 = ڪل_ويڪر * صفون;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_قدم, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بنانشان)new_صفون > (بنانشان)ڪل_سائيز_2)
            CV_Error(cv::Error::Sts_ٻاهريون_جو_Range, "Bad new number of rows");

        ڪل_ويڪر = ڪل_سائيز_2 / new_صفون;

        إذا (ڪل_ويڪر * new_صفون != ڪل_سائيز_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.صفون = new_صفون;
        hdr.قدم = ڪل_ويڪر * elemSize1();
    }

    صحيح new_ويڪر = ڪل_ويڪر / new_cn;

    إذا (new_ويڪر * new_cn != ڪل_ويڪر)
        CV_Error(cv::Error::Bad_انگ_Channels, "The total width is not divisible by the new number of channels");

    hdr.کانن_ڳڻپ = new_ويڪر;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn - 1) << CV_CN_سرڪايو);

    أرجع hdr;
}

فراغ cv::cuda::HostMem::release()
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    إذا (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = معلومات = datastart = 0;
    قدم = صفون = کانن_ڳڻپ = 0;
    refcount = 0;
#جيڪڏهن_پورو
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_سرو() ثابت
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    throw_no_cuda();
#وإلا
    CV_Assert( alloc_type == SHARED );

    فراغ *pdev;
    cuda_محفوظ_Call( cuda_Host_Get_Device_اشارو(&pdev, معلومات, 0) );

    أرجع GpuMat(صفون, کانن_ڳڻپ, type(), pdev, قدم);
#جيڪڏهن_پورو
}

فراغ cv::cuda::رجسٽر_صفحو_Locked(Mat& m)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(m);
    throw_no_cuda();
#وإلا
    CV_Assert( m.isContinuous() );
    cuda_محفوظ_Call( cuda_Host_رجسٽر(m.معلومات, m.قدم * m.صفون, cuda_Host_رجسٽر_Portable) );
#جيڪڏهن_پورو
}

فراغ cv::cuda::unregister_صفحو_Locked(Mat& m)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_CUDA
    CV_اڻ_استعمال_ٿيل(m);
#وإلا
    cuda_محفوظ_Call( cudaHostUnregister(m.معلومات) );
#جيڪڏهن_پورو
}
