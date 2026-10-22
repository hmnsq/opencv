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

استفاده نطاق cv;
استفاده نطاق cv::cuda;

#اگر_تعریف_شده HAVE_CUDA

نطاق {

صنف HostMemAllocator : عام MatAllocator
{
عام:
    صریح HostMemAllocator(بیعلامت صحيح flags) : flags_(flags)
    {
    }

    UMat_داده* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* گام,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t مجموع = CV_ELEM_اندازه(type);
        لكل (صحيح i = dims-1; i >= 0; i--)
        {
            إذا (گام)
            {
                إذا (data0 && گام[i] != CV_AUTOSTEP)
                {
                    CV_Assert(مجموع <= گام[i]);
                    مجموع = گام[i];
                }
                وإلا
                {
                    گام[i] = مجموع;
                }
            }

            مجموع *= sizes[i];
        }

        UMat_داده* u = جديد UMat_داده(هذا);
        u->size = مجموع;

        إذا (data0)
        {
            u->داده = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_داده::USER_ALLOCATED;
        }
        وإلا
        {
            فراغ* ptr = 0;
            cuda_امن_Call( cudaHostAlloc(&ptr, مجموع, flags_) );

            u->داده = u->origdata = static_cast<uchar*>(ptr);
        }

        أرجع u;
    }

    منطقي allocate(UMat_داده* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        أرجع (u != عدم);
    }

    فراغ deallocate(UMat_داده* u) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        إذا (u->refcount == 0)
        {
            إذا ( !(u->flags & UMat_داده::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            حذف u;
        }
    }

خاص:
    بیعلامت صحيح flags_;
};

} // namespace

#پایان_اگر

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(alloc_type);
    throw_no_cuda();
#وإلا
    ساكن std::map<بیعلامت صحيح, Ptr<MatAllocator> > allocators;

    بیعلامت صحيح flag = cudaHostAllocDefault;

    بدل (alloc_type)
    {
    حالة صفحه_LOCKED:    flag = cudaHostAllocDefault; اكسر;
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
#پایان_اگر
}

#اگر_تعریف_شده HAVE_CUDA
نطاق
{
    size_t align_به_بالا_گام(size_t what, size_t alignment)
    {
        size_t align_ماسک = alignment - 1;
        size_t inverse_Align_ماسک = ~align_ماسک;
        size_t res = (what + align_ماسک) & inverse_Align_ماسک;
        أرجع res;
    }
}
#پایان_اگر

فراغ cv::cuda::HostMem::create(صحيح ردیفها_2, صحيح تعداد_ستون_2, صحيح type_)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(ردیفها_2);
    CV_استفادهنشده(تعداد_ستون_2);
    CV_استفادهنشده(type_);
    throw_no_cuda();
#وإلا
    إذا (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_حافظه() );
    }

    type_ &= Mat::TYPE_ماسک;

    إذا (ردیفها == ردیفها_2 && تعداد_ستون == تعداد_ستون_2 && type() == type_ && داده)
        أرجع;

    إذا (داده)
        release();

    CV_DbgAssert( ردیفها_2 >= 0 && تعداد_ستون_2 >= 0 );

    إذا (ردیفها_2 > 0 && تعداد_ستون_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        ردیفها = ردیفها_2;
        تعداد_ستون = تعداد_ستون_2;
        گام = elem_اندازه() * تعداد_ستون;
        صحيح sz[] = { ردیفها, تعداد_ستون };
        size_t گامها[] = { گام, (size_t)CV_ELEM_اندازه(type_) };
        flags = updateContinuityFlag(flags, 2, sz, گامها);

        إذا (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            گام = align_به_بالا_گام(گام, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)گام*ردیفها;
        size_t nettosize = (size_t)_nettosize;

        إذا (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_اندازه(nettosize, (صحيح)حجم(*refcount));

        فراغ* ptr = 0;

        بدل (alloc_type)
        {
        حالة صفحه_LOCKED:    cuda_امن_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); اكسر;
        حالة SHARED:         cuda_امن_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  اكسر;
        حالة WRITE_COMBINED: cuda_امن_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); اكسر;
        افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = داده =  (uchar*)ptr;
        dataend = داده + nettosize;

        refcount = (صحيح*)cv::fastMalloc(حجم(*refcount));
        *refcount = 1;
    }
#پایان_اگر
}

HostMem cv::cuda::HostMem::reshape(صحيح new_cn, صحيح new_ردیفها) ثابت
{
    HostMem hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح مجموع_عرض = تعداد_ستون * cn;

    إذا ((new_cn > مجموع_عرض || مجموع_عرض % new_cn != 0) && new_ردیفها == 0)
        new_ردیفها = ردیفها * مجموع_عرض / new_cn;

    إذا (new_ردیفها != 0 && new_ردیفها != ردیفها)
    {
        صحيح مجموع_اندازه_2 = مجموع_عرض * ردیفها;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_گام, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بیعلامت)new_ردیفها > (بیعلامت)مجموع_اندازه_2)
            CV_Error(cv::Error::Sts_بیرون_از_Range, "Bad new number of rows");

        مجموع_عرض = مجموع_اندازه_2 / new_ردیفها;

        إذا (مجموع_عرض * new_ردیفها != مجموع_اندازه_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.ردیفها = new_ردیفها;
        hdr.گام = مجموع_عرض * elemSize1();
    }

    صحيح new_عرض = مجموع_عرض / new_cn;

    إذا (new_عرض * new_cn != مجموع_عرض)
        CV_Error(cv::Error::Bad_رقم_Channels, "The total width is not divisible by the new number of channels");

    hdr.تعداد_ستون = new_عرض;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn - 1) << CV_CN_لغزش);

    أرجع hdr;
}

فراغ cv::cuda::HostMem::release()
{
#اگر_تعریف_شده HAVE_CUDA
    إذا (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = داده = datastart = 0;
    گام = ردیفها = تعداد_ستون = 0;
    refcount = 0;
#پایان_اگر
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_سرآیند() ثابت
{
#اگر_تعریف_نشده HAVE_CUDA
    throw_no_cuda();
#وإلا
    CV_Assert( alloc_type == SHARED );

    فراغ *pdev;
    cuda_امن_Call( cuda_Host_Get_Device_اشاره_گر(&pdev, داده, 0) );

    أرجع GpuMat(ردیفها, تعداد_ستون, type(), pdev, گام);
#پایان_اگر
}

فراغ cv::cuda::ثبات_صفحه_Locked(Mat& m)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(m);
    throw_no_cuda();
#وإلا
    CV_Assert( m.isContinuous() );
    cuda_امن_Call( cuda_Host_ثبات(m.داده, m.گام * m.ردیفها, cuda_Host_ثبات_Portable) );
#پایان_اگر
}

فراغ cv::cuda::unregister_صفحه_Locked(Mat& m)
{
#اگر_تعریف_نشده HAVE_CUDA
    CV_استفادهنشده(m);
#وإلا
    cuda_امن_Call( cudaHostUnregister(m.داده) );
#پایان_اگر
}
