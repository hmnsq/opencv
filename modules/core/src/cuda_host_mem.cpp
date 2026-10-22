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

#شامل "precomp.hpp"
#شامل <map>

استعمال نامخلا cv;
استعمال نامخلا cv::cuda;

#اگر_تعریف_شدہ HAVE_CUDA

نامخلا {

جماعت HostMemAllocator : عوامی MatAllocator
{
عوامی:
    صریح HostMemAllocator(غیرعلامتی صحیح flags) : flags_(flags)
    {
    }

    UMat_ڈیٹا* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type,
                       خالی* data0, size_t* قدم,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        size_t کل = CV_ELEM_سائز(type);
        برائے (صحیح i = dims-1; i >= 0; i--)
        {
            اگر (قدم)
            {
                اگر (data0 && قدم[i] != CV_AUTOSTEP)
                {
                    CV_Assert(کل <= قدم[i]);
                    کل = قدم[i];
                }
                ورنہ
                {
                    قدم[i] = کل;
                }
            }

            کل *= sizes[i];
        }

        UMat_ڈیٹا* u = نیا UMat_ڈیٹا(یہ);
        u->size = کل;

        اگر (data0)
        {
            u->ڈیٹا = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_ڈیٹا::USER_ALLOCATED;
        }
        ورنہ
        {
            خالی* ptr = 0;
            cuda_محفوظ_Call( cudaHostAlloc(&ptr, کل, flags_) );

            u->ڈیٹا = u->origdata = static_cast<uchar*>(ptr);
        }

        واپس u;
    }

    منطقی allocate(UMat_ڈیٹا* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ثابت CV_OVERRIDE
    {
        واپس (u != تہی);
    }

    خالی deallocate(UMat_ڈیٹا* u) ثابت CV_OVERRIDE
    {
        اگر (!u)
            واپس;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        اگر (u->refcount == 0)
        {
            اگر ( !(u->flags & UMat_ڈیٹا::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            حذف u;
        }
    }

نجی:
    غیرعلامتی صحیح flags_;
};

} // namespace

#اگر_ختم

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(alloc_type);
    throw_no_cuda();
#ورنہ
    ساکن std::map<غیرعلامتی صحیح, Ptr<MatAllocator> > allocators;

    غیرعلامتی صحیح flag = cudaHostAllocDefault;

    انتخاب (alloc_type)
    {
    حالت صفحہ_LOCKED:    flag = cudaHostAllocDefault; توڑو;
    حالت SHARED:         flag = cudaHostAllocMapped;  توڑو;
    حالت WRITE_COMBINED: flag = cudaHostAllocWriteCombined; توڑو;
    طےشدہ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    اگر (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    واپس a.get();
#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_CUDA
نامخلا
{
    size_t align_اوپر_قدم(size_t what, size_t alignment)
    {
        size_t align_نقاب = alignment - 1;
        size_t inverse_Align_نقاب = ~align_نقاب;
        size_t res = (what + align_نقاب) & inverse_Align_نقاب;
        واپس res;
    }
}
#اگر_ختم

خالی cv::cuda::HostMem::create(صحیح صفیں_2, صحیح ستون_تعداد_2, صحیح type_)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(صفیں_2);
    CV_غیر_مستعمل(ستون_تعداد_2);
    CV_غیر_مستعمل(type_);
    throw_no_cuda();
#ورنہ
    اگر (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_حافظہ() );
    }

    type_ &= Mat::TYPE_نقاب;

    اگر (صفیں == صفیں_2 && ستون_تعداد == ستون_تعداد_2 && type() == type_ && ڈیٹا)
        واپس;

    اگر (ڈیٹا)
        release();

    CV_DbgAssert( صفیں_2 >= 0 && ستون_تعداد_2 >= 0 );

    اگر (صفیں_2 > 0 && ستون_تعداد_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        صفیں = صفیں_2;
        ستون_تعداد = ستون_تعداد_2;
        قدم = elem_سائز() * ستون_تعداد;
        صحیح sz[] = { صفیں, ستون_تعداد };
        size_t اقدام[] = { قدم, (size_t)CV_ELEM_سائز(type_) };
        flags = updateContinuityFlag(flags, 2, sz, اقدام);

        اگر (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            قدم = align_اوپر_قدم(قدم, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)قدم*صفیں;
        size_t nettosize = (size_t)_nettosize;

        اگر (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_سائز(nettosize, (صحیح)حجم(*refcount));

        خالی* ptr = 0;

        انتخاب (alloc_type)
        {
        حالت صفحہ_LOCKED:    cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); توڑو;
        حالت SHARED:         cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  توڑو;
        حالت WRITE_COMBINED: cuda_محفوظ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); توڑو;
        طےشدہ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = ڈیٹا =  (uchar*)ptr;
        dataend = ڈیٹا + nettosize;

        refcount = (صحیح*)cv::fastMalloc(حجم(*refcount));
        *refcount = 1;
    }
#اگر_ختم
}

HostMem cv::cuda::HostMem::reshape(صحیح new_cn, صحیح new_صفیں) ثابت
{
    HostMem hdr = *یہ;

    صحیح cn = channels();
    اگر (new_cn == 0)
        new_cn = cn;

    صحیح کل_چوڑائی = ستون_تعداد * cn;

    اگر ((new_cn > کل_چوڑائی || کل_چوڑائی % new_cn != 0) && new_صفیں == 0)
        new_صفیں = صفیں * کل_چوڑائی / new_cn;

    اگر (new_صفیں != 0 && new_صفیں != صفیں)
    {
        صحیح کل_سائز_2 = کل_چوڑائی * صفیں;

        اگر (!isContinuous())
            CV_Error(cv::Error::Bad_قدم, "The matrix is not continuous, thus its number of rows can not be changed");

        اگر ((غیرعلامتی)new_صفیں > (غیرعلامتی)کل_سائز_2)
            CV_Error(cv::Error::Sts_برآمد_کا_Range, "Bad new number of rows");

        کل_چوڑائی = کل_سائز_2 / new_صفیں;

        اگر (کل_چوڑائی * new_صفیں != کل_سائز_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.صفیں = new_صفیں;
        hdr.قدم = کل_چوڑائی * elemSize1();
    }

    صحیح new_چوڑائی = کل_چوڑائی / new_cn;

    اگر (new_چوڑائی * new_cn != کل_چوڑائی)
        CV_Error(cv::Error::Bad_عدد_Channels, "The total width is not divisible by the new number of channels");

    hdr.ستون_تعداد = new_چوڑائی;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_نقاب) | ((new_cn - 1) << CV_CN_سرکاؤ);

    واپس hdr;
}

خالی cv::cuda::HostMem::release()
{
#اگر_تعریف_شدہ HAVE_CUDA
    اگر (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = ڈیٹا = datastart = 0;
    قدم = صفیں = ستون_تعداد = 0;
    refcount = 0;
#اگر_ختم
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_سرخی() ثابت
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    throw_no_cuda();
#ورنہ
    CV_Assert( alloc_type == SHARED );

    خالی *pdev;
    cuda_محفوظ_Call( cuda_Host_Get_Device_اشارہ_گر(&pdev, ڈیٹا, 0) );

    واپس GpuMat(صفیں, ستون_تعداد, type(), pdev, قدم);
#اگر_ختم
}

خالی cv::cuda::رجسٹر_صفحہ_Locked(Mat& m)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(m);
    throw_no_cuda();
#ورنہ
    CV_Assert( m.isContinuous() );
    cuda_محفوظ_Call( cuda_Host_رجسٹر(m.ڈیٹا, m.قدم * m.صفیں, cuda_Host_رجسٹر_Portable) );
#اگر_ختم
}

خالی cv::cuda::unregister_صفحہ_Locked(Mat& m)
{
#اگر_غیر_تعریف_شدہ HAVE_CUDA
    CV_غیر_مستعمل(m);
#ورنہ
    cuda_محفوظ_Call( cudaHostUnregister(m.ڈیٹا) );
#اگر_ختم
}
