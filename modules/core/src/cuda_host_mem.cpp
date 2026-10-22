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

#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ <map>

ఉపయోగించి నామాకాశ cv;
ఉపయోగించి నామాకాశ cv::cuda;

#నిర్వచితమైతే HAVE_CUDA

నామాకాశ {

తరగతి HostMemAllocator : బహిరంగ MatAllocator
{
బహిరంగ:
    స్పష్టమైన HostMemAllocator(గుర్తులేని పూర్ణాంకం flags) : flags_(flags)
    {
    }

    UMat_దత్తాంశం* allocate(పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type,
                       శూన్య* data0, size_t* అడుగు,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) స్థిరరాశి CV_OVERRIDE
    {
        size_t మొత్తం = CV_ELEM_కొలత(type);
        కొరకు (పూర్ణాంకం i = dims-1; i >= 0; i--)
        {
            ఒకవేళ (అడుగు)
            {
                ఒకవేళ (data0 && అడుగు[i] != CV_AUTOSTEP)
                {
                    CV_Assert(మొత్తం <= అడుగు[i]);
                    మొత్తం = అడుగు[i];
                }
                లేకపోతే
                {
                    అడుగు[i] = మొత్తం;
                }
            }

            మొత్తం *= sizes[i];
        }

        UMat_దత్తాంశం* u = కొత్త UMat_దత్తాంశం(ఇది);
        u->size = మొత్తం;

        ఒకవేళ (data0)
        {
            u->దత్తాంశం = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_దత్తాంశం::USER_ALLOCATED;
        }
        లేకపోతే
        {
            శూన్య* ptr = 0;
            cuda_సురక్షిత_Call( cudaHostAlloc(&ptr, మొత్తం, flags_) );

            u->దత్తాంశం = u->origdata = static_cast<uchar*>(ptr);
        }

        తిరిగి u;
    }

    తార్కిక allocate(UMat_దత్తాంశం* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) స్థిరరాశి CV_OVERRIDE
    {
        తిరిగి (u != ఖాళీ);
    }

    శూన్య deallocate(UMat_దత్తాంశం* u) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ (!u)
            తిరిగి;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        ఒకవేళ (u->refcount == 0)
        {
            ఒకవేళ ( !(u->flags & UMat_దత్తాంశం::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            తొలగించు u;
        }
    }

అంతరంగ:
    గుర్తులేని పూర్ణాంకం flags_;
};

} // namespace

#అయితే_ముగింపు

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(alloc_type);
    throw_no_cuda();
#లేకపోతే
    స్థిర std::map<గుర్తులేని పూర్ణాంకం, Ptr<MatAllocator> > allocators;

    గుర్తులేని పూర్ణాంకం flag = cudaHostAllocDefault;

    ఎంపిక (alloc_type)
    {
    స్థితి పుట_LOCKED:    flag = cudaHostAllocDefault; ఆపు;
    స్థితి SHARED:         flag = cudaHostAllocMapped;  ఆపు;
    స్థితి WRITE_COMBINED: flag = cudaHostAllocWriteCombined; ఆపు;
    అప్రమేయ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    ఒకవేళ (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    తిరిగి a.get();
#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_CUDA
నామాకాశ
{
    size_t align_పైకి_అడుగు(size_t what, size_t alignment)
    {
        size_t align_ముసుగు = alignment - 1;
        size_t inverse_Align_ముసుగు = ~align_ముసుగు;
        size_t res = (what + align_ముసుగు) & inverse_Align_ముసుగు;
        తిరిగి res;
    }
}
#అయితే_ముగింపు

శూన్య cv::cuda::HostMem::create(పూర్ణాంకం అడ్డువరుసలు_2, పూర్ణాంకం నిలువుల_సంఖ్య_2, పూర్ణాంకం type_)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(అడ్డువరుసలు_2);
    CV_వాడని(నిలువుల_సంఖ్య_2);
    CV_వాడని(type_);
    throw_no_cuda();
#లేకపోతే
    ఒకవేళ (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_జ్ఞాపకం() );
    }

    type_ &= Mat::TYPE_ముసుగు;

    ఒకవేళ (అడ్డువరుసలు == అడ్డువరుసలు_2 && నిలువుల_సంఖ్య == నిలువుల_సంఖ్య_2 && type() == type_ && దత్తాంశం)
        తిరిగి;

    ఒకవేళ (దత్తాంశం)
        release();

    CV_DbgAssert( అడ్డువరుసలు_2 >= 0 && నిలువుల_సంఖ్య_2 >= 0 );

    ఒకవేళ (అడ్డువరుసలు_2 > 0 && నిలువుల_సంఖ్య_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        అడ్డువరుసలు = అడ్డువరుసలు_2;
        నిలువుల_సంఖ్య = నిలువుల_సంఖ్య_2;
        అడుగు = elem_కొలత() * నిలువుల_సంఖ్య;
        పూర్ణాంకం sz[] = { అడ్డువరుసలు, నిలువుల_సంఖ్య };
        size_t అడుగులు[] = { అడుగు, (size_t)CV_ELEM_కొలత(type_) };
        flags = updateContinuityFlag(flags, 2, sz, అడుగులు);

        ఒకవేళ (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            అడుగు = align_పైకి_అడుగు(అడుగు, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)అడుగు*అడ్డువరుసలు;
        size_t nettosize = (size_t)_nettosize;

        ఒకవేళ (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_కొలత(nettosize, (పూర్ణాంకం)పరిమాణం(*refcount));

        శూన్య* ptr = 0;

        ఎంపిక (alloc_type)
        {
        స్థితి పుట_LOCKED:    cuda_సురక్షిత_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); ఆపు;
        స్థితి SHARED:         cuda_సురక్షిత_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  ఆపు;
        స్థితి WRITE_COMBINED: cuda_సురక్షిత_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); ఆపు;
        అప్రమేయ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = దత్తాంశం =  (uchar*)ptr;
        dataend = దత్తాంశం + nettosize;

        refcount = (పూర్ణాంకం*)cv::fastMalloc(పరిమాణం(*refcount));
        *refcount = 1;
    }
#అయితే_ముగింపు
}

HostMem cv::cuda::HostMem::reshape(పూర్ణాంకం new_cn, పూర్ణాంకం new_అడ్డువరుసలు) స్థిరరాశి
{
    HostMem hdr = *ఇది;

    పూర్ణాంకం cn = channels();
    ఒకవేళ (new_cn == 0)
        new_cn = cn;

    పూర్ణాంకం మొత్తం_వెడల్పు = నిలువుల_సంఖ్య * cn;

    ఒకవేళ ((new_cn > మొత్తం_వెడల్పు || మొత్తం_వెడల్పు % new_cn != 0) && new_అడ్డువరుసలు == 0)
        new_అడ్డువరుసలు = అడ్డువరుసలు * మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ (new_అడ్డువరుసలు != 0 && new_అడ్డువరుసలు != అడ్డువరుసలు)
    {
        పూర్ణాంకం మొత్తం_కొలత_2 = మొత్తం_వెడల్పు * అడ్డువరుసలు;

        ఒకవేళ (!isContinuous())
            CV_Error(cv::Error::Bad_అడుగు, "The matrix is not continuous, thus its number of rows can not be changed");

        ఒకవేళ ((గుర్తులేని)new_అడ్డువరుసలు > (గుర్తులేని)మొత్తం_కొలత_2)
            CV_Error(cv::Error::Sts_బహిర్గతం_యొక్క_Range, "Bad new number of rows");

        మొత్తం_వెడల్పు = మొత్తం_కొలత_2 / new_అడ్డువరుసలు;

        ఒకవేళ (మొత్తం_వెడల్పు * new_అడ్డువరుసలు != మొత్తం_కొలత_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.అడ్డువరుసలు = new_అడ్డువరుసలు;
        hdr.అడుగు = మొత్తం_వెడల్పు * elemSize1();
    }

    పూర్ణాంకం new_వెడల్పు = మొత్తం_వెడల్పు / new_cn;

    ఒకవేళ (new_వెడల్పు * new_cn != మొత్తం_వెడల్పు)
        CV_Error(cv::Error::Bad_అంకె_Channels, "The total width is not divisible by the new number of channels");

    hdr.నిలువుల_సంఖ్య = new_వెడల్పు;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ముసుగు) | ((new_cn - 1) << CV_CN_జరుపు);

    తిరిగి hdr;
}

శూన్య cv::cuda::HostMem::release()
{
#నిర్వచితమైతే HAVE_CUDA
    ఒకవేళ (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = దత్తాంశం = datastart = 0;
    అడుగు = అడ్డువరుసలు = నిలువుల_సంఖ్య = 0;
    refcount = 0;
#అయితే_ముగింపు
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_శీర్షిక() స్థిరరాశి
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    throw_no_cuda();
#లేకపోతే
    CV_Assert( alloc_type == SHARED );

    శూన్య *pdev;
    cuda_సురక్షిత_Call( cuda_Host_Get_Device_సూచిక(&pdev, దత్తాంశం, 0) );

    తిరిగి GpuMat(అడ్డువరుసలు, నిలువుల_సంఖ్య, type(), pdev, అడుగు);
#అయితే_ముగింపు
}

శూన్య cv::cuda::రిజిస్టరు_పుట_Locked(Mat& m)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(m);
    throw_no_cuda();
#లేకపోతే
    CV_Assert( m.isContinuous() );
    cuda_సురక్షిత_Call( cuda_Host_రిజిస్టరు(m.దత్తాంశం, m.అడుగు * m.అడ్డువరుసలు, cuda_Host_రిజిస్టరు_Portable) );
#అయితే_ముగింపు
}

శూన్య cv::cuda::unregister_పుట_Locked(Mat& m)
{
#నిర్వచితం_కాకపోతే HAVE_CUDA
    CV_వాడని(m);
#లేకపోతే
    cuda_సురక్షిత_Call( cudaHostUnregister(m.దత్తాంశం) );
#అయితే_ముగింపు
}
