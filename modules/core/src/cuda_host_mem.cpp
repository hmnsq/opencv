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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ <map>

ಬಳಸಿ ನಾಮಾಕಾಶ cv;
ಬಳಸಿ ನಾಮಾಕಾಶ cv::cuda;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA

ನಾಮಾಕಾಶ {

ವರ್ಗ HostMemAllocator : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಪಷ್ಟ HostMemAllocator(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags) : flags_(flags)
    {
    }

    UMat_ದತ್ತಾಂಶ* allocate(ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ type,
                       ಶೂನ್ಯ* data0, size_t* ಹೆಜ್ಜೆ,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        size_t ಒಟ್ಟು = CV_ELEM_ಗಾತ್ರ(type);
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = dims-1; i >= 0; i--)
        {
            ಆದರೆ (ಹೆಜ್ಜೆ)
            {
                ಆದರೆ (data0 && ಹೆಜ್ಜೆ[i] != CV_AUTOSTEP)
                {
                    CV_Assert(ಒಟ್ಟು <= ಹೆಜ್ಜೆ[i]);
                    ಒಟ್ಟು = ಹೆಜ್ಜೆ[i];
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಹೆಜ್ಜೆ[i] = ಒಟ್ಟು;
                }
            }

            ಒಟ್ಟು *= sizes[i];
        }

        UMat_ದತ್ತಾಂಶ* u = ಹೊಸ UMat_ದತ್ತಾಂಶ(ಇದು);
        u->size = ಒಟ್ಟು;

        ಆದರೆ (data0)
        {
            u->ದತ್ತಾಂಶ = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_ದತ್ತಾಂಶ::USER_ALLOCATED;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಶೂನ್ಯ* ptr = 0;
            cuda_ಸುರಕ್ಷಿತ_Call( cudaHostAlloc(&ptr, ಒಟ್ಟು, flags_) );

            u->ದತ್ತಾಂಶ = u->origdata = static_cast<uchar*>(ptr);
        }

        ಹಿಂತಿರುಗಿ u;
    }

    ತಾರ್ಕಿಕ allocate(UMat_ದತ್ತಾಂಶ* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಹಿಂತಿರುಗಿ (u != ಖಾಲಿ);
    }

    ಶೂನ್ಯ deallocate(UMat_ದತ್ತಾಂಶ* u) ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಆದರೆ (!u)
            ಹಿಂತಿರುಗಿ;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        ಆದರೆ (u->refcount == 0)
        {
            ಆದರೆ ( !(u->flags & UMat_ದತ್ತಾಂಶ::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            ಅಳಿಸು u;
        }
    }

ಖಾಸಗಿ:
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags_;
};

} // namespace

#ಆದರೆ_ಅಂತ್ಯ

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(alloc_type);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರ std::map<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, Ptr<MatAllocator> > allocators;

    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flag = cudaHostAllocDefault;

    ಆಯ್ಕೆ (alloc_type)
    {
    ಸ್ಥಿತಿ ಪುಟ_LOCKED:    flag = cudaHostAllocDefault; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ SHARED:         flag = cudaHostAllocMapped;  ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ WRITE_COMBINED: flag = cudaHostAllocWriteCombined; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    ಆದರೆ (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    ಹಿಂತಿರುಗಿ a.get();
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
ನಾಮಾಕಾಶ
{
    size_t align_ಮೇಲೆ_ಹೆಜ್ಜೆ(size_t what, size_t alignment)
    {
        size_t align_ಮುಖವಾಡ = alignment - 1;
        size_t inverse_Align_ಮುಖವಾಡ = ~align_ಮುಖವಾಡ;
        size_t res = (what + align_ಮುಖವಾಡ) & inverse_Align_ಮುಖವಾಡ;
        ಹಿಂತಿರುಗಿ res;
    }
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::cuda::HostMem::create(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_2, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2, ಪೂರ್ಣಾಂಕ type_)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(ಅಡ್ಡಸಾಲುಗಳು_2);
    CV_ಬಳಸದ(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2);
    CV_ಬಳಸದ(type_);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_ಸ್ಮರಣೆ() );
    }

    type_ &= Mat::TYPE_ಮುಖವಾಡ;

    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು == ಅಡ್ಡಸಾಲುಗಳು_2 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 && type() == type_ && ದತ್ತಾಂಶ)
        ಹಿಂತಿರುಗಿ;

    ಆದರೆ (ದತ್ತಾಂಶ)
        release();

    CV_DbgAssert( ಅಡ್ಡಸಾಲುಗಳು_2 >= 0 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 >= 0 );

    ಆದರೆ (ಅಡ್ಡಸಾಲುಗಳು_2 > 0 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು_2;
        ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2;
        ಹೆಜ್ಜೆ = elem_ಗಾತ್ರ() * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
        ಪೂರ್ಣಾಂಕ sz[] = { ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ };
        size_t ಹೆಜ್ಜೆಗಳು[] = { ಹೆಜ್ಜೆ, (size_t)CV_ELEM_ಗಾತ್ರ(type_) };
        flags = updateContinuityFlag(flags, 2, sz, ಹೆಜ್ಜೆಗಳು);

        ಆದರೆ (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            ಹೆಜ್ಜೆ = align_ಮೇಲೆ_ಹೆಜ್ಜೆ(ಹೆಜ್ಜೆ, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)ಹೆಜ್ಜೆ*ಅಡ್ಡಸಾಲುಗಳು;
        size_t nettosize = (size_t)_nettosize;

        ಆದರೆ (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_ಗಾತ್ರ(nettosize, (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(*refcount));

        ಶೂನ್ಯ* ptr = 0;

        ಆಯ್ಕೆ (alloc_type)
        {
        ಸ್ಥಿತಿ ಪುಟ_LOCKED:    cuda_ಸುರಕ್ಷಿತ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ SHARED:         cuda_ಸುರಕ್ಷಿತ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ WRITE_COMBINED: cuda_ಸುರಕ್ಷಿತ_Call( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = ದತ್ತಾಂಶ =  (uchar*)ptr;
        dataend = ದತ್ತಾಂಶ + nettosize;

        refcount = (ಪೂರ್ಣಾಂಕ*)cv::fastMalloc(ಗಾತ್ರ(*refcount));
        *refcount = 1;
    }
#ಆದರೆ_ಅಂತ್ಯ
}

HostMem cv::cuda::HostMem::reshape(ಪೂರ್ಣಾಂಕ new_cn, ಪೂರ್ಣಾಂಕ new_ಅಡ್ಡಸಾಲುಗಳು) ಸ್ಥಿರಾಂಕ
{
    HostMem hdr = *ಇದು;

    ಪೂರ್ಣಾಂಕ cn = channels();
    ಆದರೆ (new_cn == 0)
        new_cn = cn;

    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಅಗಲ = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * cn;

    ಆದರೆ ((new_cn > ಒಟ್ಟು_ಅಗಲ || ಒಟ್ಟು_ಅಗಲ % new_cn != 0) && new_ಅಡ್ಡಸಾಲುಗಳು == 0)
        new_ಅಡ್ಡಸಾಲುಗಳು = ಅಡ್ಡಸಾಲುಗಳು * ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ (new_ಅಡ್ಡಸಾಲುಗಳು != 0 && new_ಅಡ್ಡಸಾಲುಗಳು != ಅಡ್ಡಸಾಲುಗಳು)
    {
        ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_ಗಾತ್ರ_2 = ಒಟ್ಟು_ಅಗಲ * ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ (!isContinuous())
            CV_Error(cv::Error::Bad_ಹೆಜ್ಜೆ, "The matrix is not continuous, thus its number of rows can not be changed");

        ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ)new_ಅಡ್ಡಸಾಲುಗಳು > (ಚಿಹ್ನೆರಹಿತ)ಒಟ್ಟು_ಗಾತ್ರ_2)
            CV_Error(cv::Error::Sts_ಹೊರಹೋಗು_ದ_Range, "Bad new number of rows");

        ಒಟ್ಟು_ಅಗಲ = ಒಟ್ಟು_ಗಾತ್ರ_2 / new_ಅಡ್ಡಸಾಲುಗಳು;

        ಆದರೆ (ಒಟ್ಟು_ಅಗಲ * new_ಅಡ್ಡಸಾಲುಗಳು != ಒಟ್ಟು_ಗಾತ್ರ_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.ಅಡ್ಡಸಾಲುಗಳು = new_ಅಡ್ಡಸಾಲುಗಳು;
        hdr.ಹೆಜ್ಜೆ = ಒಟ್ಟು_ಅಗಲ * elemSize1();
    }

    ಪೂರ್ಣಾಂಕ new_ಅಗಲ = ಒಟ್ಟು_ಅಗಲ / new_cn;

    ಆದರೆ (new_ಅಗಲ * new_cn != ಒಟ್ಟು_ಅಗಲ)
        CV_Error(cv::Error::Bad_ಅಂಕೆ_Channels, "The total width is not divisible by the new number of channels");

    hdr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = new_ಅಗಲ;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_ಮುಖವಾಡ) | ((new_cn - 1) << CV_CN_ಸರಿಸು);

    ಹಿಂತಿರುಗಿ hdr;
}

ಶೂನ್ಯ cv::cuda::HostMem::release()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಆದರೆ (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = ದತ್ತಾಂಶ = datastart = 0;
    ಹೆಜ್ಜೆ = ಅಡ್ಡಸಾಲುಗಳು = ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;
    refcount = 0;
#ಆದರೆ_ಅಂತ್ಯ
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_ಶೀರ್ಷಿಕೆ() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Assert( alloc_type == SHARED );

    ಶೂನ್ಯ *pdev;
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Host_Get_Device_ಸೂಚಕ(&pdev, ದತ್ತಾಂಶ, 0) );

    ಹಿಂತಿರುಗಿ GpuMat(ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, type(), pdev, ಹೆಜ್ಜೆ);
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::ರಿಜಿಸ್ಟರ್_ಪುಟ_Locked(Mat& m)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(m);
    throw_no_cuda();
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Assert( m.isContinuous() );
    cuda_ಸುರಕ್ಷಿತ_Call( cuda_Host_ರಿಜಿಸ್ಟರ್(m.ದತ್ತಾಂಶ, m.ಹೆಜ್ಜೆ * m.ಅಡ್ಡಸಾಲುಗಳು, cuda_Host_ರಿಜಿಸ್ಟರ್_Portable) );
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ cv::cuda::unregister_ಪುಟ_Locked(Mat& m)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_CUDA
    CV_ಬಳಸದ(m);
#ಇಲ್ಲದಿದ್ದರೆ
    cuda_ಸುರಕ್ಷಿತ_Call( cudaHostUnregister(m.ದತ್ತಾಂಶ) );
#ಆದರೆ_ಅಂತ್ಯ
}
