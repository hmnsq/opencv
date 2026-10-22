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

ورتوں نطاق cv;
ورتوں نطاق cv::cuda;

#جے_تعریف_شدہ HAVE_CUDA

نطاق {

صنف HostMemAllocator : عام MatAllocator
{
عام:
    واضح HostMemAllocator(بنانشانی صحيح flags) : flags_(flags)
    {
    }

    UMat_انکڑے* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* قدم,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t کل = CV_ELEM_ناپ(type);
        لكل (صحيح i = dims-1; i >= 0; i--)
        {
            إذا (قدم)
            {
                إذا (data0 && قدم[i] != CV_AUTOSTEP)
                {
                    CV_Assert(کل <= قدم[i]);
                    کل = قدم[i];
                }
                وإلا
                {
                    قدم[i] = کل;
                }
            }

            کل *= sizes[i];
        }

        UMat_انکڑے* u = جديد UMat_انکڑے(هذا);
        u->size = کل;

        إذا (data0)
        {
            u->انکڑے = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_انکڑے::USER_ALLOCATED;
        }
        وإلا
        {
            فراغ* ptr = 0;
            cuda_محفوظ_Call( cudaHostAlloc(&ptr, کل, flags_) );

            u->انکڑے = u->origdata = static_cast<uchar*>(ptr);
        }

        أرجع u;
    }

    منطقي allocate(UMat_انکڑے* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        أرجع (u != عدم);
    }

    فراغ deallocate(UMat_انکڑے* u) ثابت CV_OVERRIDE
    {
        إذا (!u)
            أرجع;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        إذا (u->refcount == 0)
        {
            إذا ( !(u->flags & UMat_انکڑے::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            مٹاؤ u;
        }
    }

خاص:
    بنانشانی صحيح flags_;
};

} // namespace

#جے_انت

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(alloc_type);
    throw_no_cuda();
#وإلا
    ساكن std::map<بنانشانی صحيح, Ptr<MatAllocator> > allocators;

    بنانشانی صحيح flag = cudaHostAllocDefault;

    بدل (alloc_type)
    {
    حالة ورقہ_LOCKED:    flag = cudaHostAllocDefault; اكسر;
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
#جے_انت
}

#جے_تعریف_شدہ HAVE_CUDA
نطاق
{
    size_t align_اتے_قدم(size_t what, size_t alignment)
    {
        size_t align_نقاب = alignment - 1;
        size_t inverse_Align_نقاب = ~align_نقاب;
        size_t res = (what + align_نقاب) & inverse_Align_نقاب;
        أرجع res;
    }
}
#جے_انت

فراغ cv::cuda::HostMem::create(صحيح صفاں_2, صحيح خانے_گنتی_2, صحيح type_)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(صفاں_2);
    CV_بن_ورتیا(خانے_گنتی_2);
    CV_بن_ورتیا(type_);
    throw_no_cuda();
#وإلا
    إذا (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_حافظہ() );
    }

    type_ &= Mat::TYPE_نقاب;

    إذا (صفاں == صفاں_2 && خانے_گنتی == خانے_گنتی_2 && type() == type_ && انکڑے)
        أرجع;

    إذا (انکڑے)
        release();

    CV_DbgAssert( صفاں_2 >= 0 && خانے_گنتی_2 >= 0 );

    إذا (صفاں_2 > 0 && خانے_گنتی_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        صفاں = صفاں_2;
        خانے_گنتی = خانے_گنتی_2;
        قدم = elem_ناپ() * خانے_گنتی;
        صحيح sz[] = { صفاں, خانے_گنتی };
        size_t قدماں[] = { قدم, (size_t)CV_ELEM_ناپ(type_) };
        flags = updateContinuityFlag(flags, 2, sz, قدماں);

        إذا (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            قدم = align_اتے_قدم(قدم, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)قدم*صفاں;
        size_t nettosize = (size_t)_nettosize;

        إذا (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_ناپ(nettosize, (صحيح)حجم(*refcount));

        فراغ* ptr = 0;

        بدل (alloc_type)
        {
        حالة ورقہ_LOCKED:    cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); اكسر;
        حالة SHARED:         cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  اكسر;
        حالة WRITE_COMBINED: cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); اكسر;
        افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = انکڑے =  (uchar*)ptr;
        dataend = انکڑے + nettosize;

        refcount = (صحيح*)cv::fastMalloc(حجم(*refcount));
        *refcount = 1;
    }
#جے_انت
}

HostMem cv::cuda::HostMem::reshape(صحيح new_cn, صحيح new_صفاں) ثابت
{
    HostMem hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح کل_چوڑائی = خانے_گنتی * cn;

    إذا ((new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفاں == 0)
        new_صفاں = صفاں * کل_چوڑائی / new_cn;

    إذا (new_صفاں != 0 && new_صفاں != صفاں)
    {
        صحيح کل_ناپ_2 = کل_چوڑائی * صفاں;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_قدم, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بنانشانی)new_صفاں > (بنانشانی)کل_ناپ_2)
            CV_Error(cv::Error::Sts_باہرلا_دا_Range, "Bad new number of rows");

        کل_چوڑائی = کل_ناپ_2 / new_صفاں;

        إذا (کل_چوڑائی * new_صفاں != کل_ناپ_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.صفاں = new_صفاں;
        hdr.قدم = کل_چوڑائی * elemSize1();
    }

    صحيح new_چوڑائی = کل_چوڑائی / new_cn;

    إذا (new_چوڑائی * new_cn != کل_چوڑائی)
        CV_Error(cv::Error::Bad_عدد_Channels, "The total width is not divisible by the new number of channels");

    hdr.خانے_گنتی = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn - 1) << CV_CN_کھسکاؤ);

    أرجع hdr;
}

فراغ cv::cuda::HostMem::release()
{
#جے_تعریف_شدہ HAVE_CUDA
    إذا (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = انکڑے = datastart = 0;
    قدم = صفاں = خانے_گنتی = 0;
    refcount = 0;
#جے_انت
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_سرخی() ثابت
{
#جے_بنا_تعریف HAVE_CUDA
    throw_no_cuda();
#وإلا
    CV_Assert( alloc_type == SHARED );

    فراغ *pdev;
    cuda_محفوظ_Call( cuda_Host_Get_Device_اشارہ_گر(&pdev, انکڑے, 0) );

    أرجع GpuMat(صفاں, خانے_گنتی, type(), pdev, قدم);
#جے_انت
}

فراغ cv::cuda::رجسٹر_ورقہ_Locked(Mat& m)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(m);
    throw_no_cuda();
#وإلا
    CV_Assert( m.isContinuous() );
    cuda_محفوظ_Call( cuda_Host_رجسٹر(m.انکڑے, m.قدم * m.صفاں, cuda_Host_رجسٹر_Portable) );
#جے_انت
}

فراغ cv::cuda::unregister_ورقہ_Locked(Mat& m)
{
#جے_بنا_تعریف HAVE_CUDA
    CV_بن_ورتیا(m);
#وإلا
    cuda_محفوظ_Call( cudaHostUnregister(m.انکڑے) );
#جے_انت
}
