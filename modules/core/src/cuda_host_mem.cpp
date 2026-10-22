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

کارول نطاق cv;
کارول نطاق cv::cuda;

#که_تعریف_شوی HAVE_CUDA

نطاق {

صنف HostMemAllocator : عام MatAllocator
{
عام:
    څرګند HostMemAllocator(بینښه صحيح flags) : flags_(flags)
    {
    }

    UMat_معلومات* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data0, size_t* ګام,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t ټول = CV_ELEM_اندازه(type);
        لكل (صحيح i = dims-1; i >= 0; i--)
        {
            إذا (ګام)
            {
                إذا (data0 && ګام[i] != CV_AUTOSTEP)
                {
                    CV_Assert(ټول <= ګام[i]);
                    ټول = ګام[i];
                }
                وإلا
                {
                    ګام[i] = ټول;
                }
            }

            ټول *= sizes[i];
        }

        UMat_معلومات* u = جديد UMat_معلومات(هذا);
        u->size = ټول;

        إذا (data0)
        {
            u->معلومات = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_معلومات::USER_ALLOCATED;
        }
        وإلا
        {
            فراغ* ptr = 0;
            cuda_خوندي_Call( cudaHostAlloc(&ptr, ټول, flags_) );

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

            ړنګول u;
        }
    }

خاص:
    بینښه صحيح flags_;
};

} // namespace

#د_که_پای

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(alloc_type);
    throw_no_cuda();
#وإلا
    ساكن std::map<بینښه صحيح, Ptr<MatAllocator> > allocators;

    بینښه صحيح flag = cudaHostAllocDefault;

    بدل (alloc_type)
    {
    حالة پاڼه_LOCKED:    flag = cudaHostAllocDefault; اكسر;
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
#د_که_پای
}

#که_تعریف_شوی HAVE_CUDA
نطاق
{
    size_t align_پورته_ګام(size_t what, size_t alignment)
    {
        size_t align_ماسک = alignment - 1;
        size_t inverse_Align_ماسک = ~align_ماسک;
        size_t res = (what + align_ماسک) & inverse_Align_ماسک;
        أرجع res;
    }
}
#د_که_پای

فراغ cv::cuda::HostMem::create(صحيح لیکې_2, صحيح ستون_شمېر_2, صحيح type_)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(لیکې_2);
    CV_ناکارول_شوی(ستون_شمېر_2);
    CV_ناکارول_شوی(type_);
    throw_no_cuda();
#وإلا
    إذا (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_حافظه() );
    }

    type_ &= Mat::TYPE_ماسک;

    إذا (لیکې == لیکې_2 && ستون_شمېر == ستون_شمېر_2 && type() == type_ && معلومات)
        أرجع;

    إذا (معلومات)
        release();

    CV_DbgAssert( لیکې_2 >= 0 && ستون_شمېر_2 >= 0 );

    إذا (لیکې_2 > 0 && ستون_شمېر_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        لیکې = لیکې_2;
        ستون_شمېر = ستون_شمېر_2;
        ګام = elem_اندازه() * ستون_شمېر;
        صحيح sz[] = { لیکې, ستون_شمېر };
        size_t ګامونه[] = { ګام, (size_t)CV_ELEM_اندازه(type_) };
        flags = updateContinuityFlag(flags, 2, sz, ګامونه);

        إذا (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            ګام = align_پورته_ګام(ګام, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)ګام*لیکې;
        size_t nettosize = (size_t)_nettosize;

        إذا (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_اندازه(nettosize, (صحيح)حجم(*refcount));

        فراغ* ptr = 0;

        بدل (alloc_type)
        {
        حالة پاڼه_LOCKED:    cuda_خوندي_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); اكسر;
        حالة SHARED:         cuda_خوندي_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  اكسر;
        حالة WRITE_COMBINED: cuda_خوندي_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); اكسر;
        افتراضي:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = معلومات =  (uchar*)ptr;
        dataend = معلومات + nettosize;

        refcount = (صحيح*)cv::fastMalloc(حجم(*refcount));
        *refcount = 1;
    }
#د_که_پای
}

HostMem cv::cuda::HostMem::reshape(صحيح new_cn, صحيح new_لیکې) ثابت
{
    HostMem hdr = *هذا;

    صحيح cn = channels();
    إذا (new_cn == 0)
        new_cn = cn;

    صحيح ټول_پلنوالی = ستون_شمېر * cn;

    إذا ((new_cn > ټول_پلنوالی || ټول_پلنوالی % new_cn != 0) && new_لیکې == 0)
        new_لیکې = لیکې * ټول_پلنوالی / new_cn;

    إذا (new_لیکې != 0 && new_لیکې != لیکې)
    {
        صحيح ټول_اندازه_2 = ټول_پلنوالی * لیکې;

        إذا (!isContinuous())
            CV_Error(cv::Error::Bad_ګام, "The matrix is not continuous, thus its number of rows can not be changed");

        إذا ((بینښه)new_لیکې > (بینښه)ټول_اندازه_2)
            CV_Error(cv::Error::Sts_بهر_د_Range, "Bad new number of rows");

        ټول_پلنوالی = ټول_اندازه_2 / new_لیکې;

        إذا (ټول_پلنوالی * new_لیکې != ټول_اندازه_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.لیکې = new_لیکې;
        hdr.ګام = ټول_پلنوالی * elemSize1();
    }

    صحيح new_پلنوالی = ټول_پلنوالی / new_cn;

    إذا (new_پلنوالی * new_cn != ټول_پلنوالی)
        CV_Error(cv::Error::Bad_عدد_Channels, "The total width is not divisible by the new number of channels");

    hdr.ستون_شمېر = new_پلنوالی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ماسک) | ((new_cn - 1) << CV_CN_ښويدنه);

    أرجع hdr;
}

فراغ cv::cuda::HostMem::release()
{
#که_تعریف_شوی HAVE_CUDA
    إذا (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = معلومات = datastart = 0;
    ګام = لیکې = ستون_شمېر = 0;
    refcount = 0;
#د_که_پای
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_سرلیک() ثابت
{
#که_نه_تعریف_شوی HAVE_CUDA
    throw_no_cuda();
#وإلا
    CV_Assert( alloc_type == SHARED );

    فراغ *pdev;
    cuda_خوندي_Call( cuda_Host_Get_Device_اشاره_ګر(&pdev, معلومات, 0) );

    أرجع GpuMat(لیکې, ستون_شمېر, type(), pdev, ګام);
#د_که_پای
}

فراغ cv::cuda::ثبتوونکی_پاڼه_Locked(Mat& m)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(m);
    throw_no_cuda();
#وإلا
    CV_Assert( m.isContinuous() );
    cuda_خوندي_Call( cuda_Host_ثبتوونکی(m.معلومات, m.ګام * m.لیکې, cuda_Host_ثبتوونکی_Portable) );
#د_که_پای
}

فراغ cv::cuda::unregister_پاڼه_Locked(Mat& m)
{
#که_نه_تعریف_شوی HAVE_CUDA
    CV_ناکارول_شوی(m);
#وإلا
    cuda_خوندي_Call( cudaHostUnregister(m.معلومات) );
#د_که_پای
}
