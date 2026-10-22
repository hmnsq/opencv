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

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव <map>

उपयुज्य नामाकाश cv;
उपयुज्य नामाकाश cv::cuda;

#यदि_परिभाषितम् HAVE_CUDA

नामाकाश {

वर्ग HostMemAllocator : सार्वजनिक MatAllocator
{
सार्वजनिक:
    स्पष्ट HostMemAllocator(अचिह्नित पूर्णाङ्क flags) : flags_(flags)
    {
    }

    UMat_दत्त* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type,
                       शून्य* data0, size_t* चरणः,
                       AccessFlag /*flags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        size_t योग = CV_ELEM_आकार(type);
        प्रति (पूर्णाङ्क i = dims-1; i >= 0; i--)
        {
            यदि (चरणः)
            {
                यदि (data0 && चरणः[i] != CV_AUTOSTEP)
                {
                    CV_Assert(योग <= चरणः[i]);
                    योग = चरणः[i];
                }
                अन्यथा
                {
                    चरणः[i] = योग;
                }
            }

            योग *= sizes[i];
        }

        UMat_दत्त* u = नूतन UMat_दत्त(एतत्);
        u->size = योग;

        यदि (data0)
        {
            u->दत्त = u->origdata = static_cast<uchar*>(data0);
            u->flags |= UMat_दत्त::USER_ALLOCATED;
        }
        अन्यथा
        {
            शून्य* ptr = 0;
            cudaSafeCall( cudaHostAlloc(&ptr, योग, flags_) );

            u->दत्त = u->origdata = static_cast<uchar*>(ptr);
        }

        प्रत्यागम u;
    }

    तार्किक allocate(UMat_दत्त* u, AccessFlag /*accessFlags*/, UMatUsageFlags /*usageFlags*/) ध्रुव CV_OVERRIDE
    {
        प्रत्यागम (u != रिक्त);
    }

    शून्य deallocate(UMat_दत्त* u) ध्रुव CV_OVERRIDE
    {
        यदि (!u)
            प्रत्यागम;

        CV_Assert(u->urefcount >= 0);
        CV_Assert(u->refcount >= 0);

        यदि (u->refcount == 0)
        {
            यदि ( !(u->flags & UMat_दत्त::USER_ALLOCATED) )
            {
                cudaFreeHost(u->origdata);
                u->origdata = 0;
            }

            विलोपय u;
        }
    }

गुप्त:
    अचिह्नित पूर्णाङ्क flags_;
};

} // namespace

#यदि_अन्तः

MatAllocator* cv::cuda::HostMem::getAllocator(AllocType alloc_type)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(alloc_type);
    throw_no_cuda();
#अन्यथा
    स्थिर std::map<अचिह्नित पूर्णाङ्क, Ptr<MatAllocator> > allocators;

    अचिह्नित पूर्णाङ्क flag = cudaHostAllocDefault;

    विकल्प (alloc_type)
    {
    पक्ष PAGE_LOCKED:    flag = cudaHostAllocDefault; भङ्ग;
    पक्ष SHARED:         flag = cudaHostAllocMapped;  भङ्ग;
    पक्ष WRITE_COMBINED: flag = cudaHostAllocWriteCombined; भङ्ग;
    स्वभाव:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
    }

    Ptr<MatAllocator>& a = allocators[flag];

    यदि (a.empty())
    {
        a = makePtr<HostMemAllocator>(flag);
    }

    प्रत्यागम a.get();
#यदि_अन्तः
}

#यदि_परिभाषितम् HAVE_CUDA
नामाकाश
{
    size_t align_ऊर्ध्वम्_चरणः(size_t what, size_t alignment)
    {
        size_t align_आवरणम् = alignment - 1;
        size_t inverse_Align_आवरणम् = ~align_आवरणम्;
        size_t res = (what + align_आवरणम्) & inverse_Align_आवरणम्;
        प्रत्यागम res;
    }
}
#यदि_अन्तः

शून्य cv::cuda::HostMem::create(पूर्णाङ्क पङ्क्तिकाः_2, पूर्णाङ्क स्तम्भसङ्ख्या_2, पूर्णाङ्क type_)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(पङ्क्तिकाः_2);
    CV_अप्रयुक्तम्(स्तम्भसङ्ख्या_2);
    CV_अप्रयुक्तम्(type_);
    throw_no_cuda();
#अन्यथा
    यदि (alloc_type == SHARED)
    {
        DeviceInfo devInfo;
        CV_Assert( devInfo.can_Map_Host_स्मृतिः() );
    }

    type_ &= Mat::TYPE_आवरणम्;

    यदि (पङ्क्तिकाः == पङ्क्तिकाः_2 && स्तम्भसङ्ख्या == स्तम्भसङ्ख्या_2 && type() == type_ && दत्त)
        प्रत्यागम;

    यदि (दत्त)
        release();

    CV_DbgAssert( पङ्क्तिकाः_2 >= 0 && स्तम्भसङ्ख्या_2 >= 0 );

    यदि (पङ्क्तिकाः_2 > 0 && स्तम्भसङ्ख्या_2 > 0)
    {
        flags = Mat::MAGIC_VAL + type_;
        पङ्क्तिकाः = पङ्क्तिकाः_2;
        स्तम्भसङ्ख्या = स्तम्भसङ्ख्या_2;
        चरणः = elem_आकार() * स्तम्भसङ्ख्या;
        पूर्णाङ्क sz[] = { पङ्क्तिकाः, स्तम्भसङ्ख्या };
        size_t चरणाः[] = { चरणः, (size_t)CV_ELEM_आकार(type_) };
        flags = updateContinuityFlag(flags, 2, sz, चरणाः);

        यदि (alloc_type == SHARED)
        {
            DeviceInfo devInfo;
            चरणः = align_ऊर्ध्वम्_चरणः(चरणः, devInfo.textureAlignment());
        }

        int64 _nettosize = (int64)चरणः*पङ्क्तिकाः;
        size_t nettosize = (size_t)_nettosize;

        यदि (_nettosize != (int64)nettosize)
            CV_Error(cv::Error::StsNoMem, "Too big buffer is allocated");

        size_t datasize = align_आकार(nettosize, (पूर्णाङ्क)परिमाण(*refcount));

        शून्य* ptr = 0;

        विकल्प (alloc_type)
        {
        पक्ष PAGE_LOCKED:    cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocDefault) ); भङ्ग;
        पक्ष SHARED:         cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocMapped) );  भङ्ग;
        पक्ष WRITE_COMBINED: cudaSafeCall( cudaHostAlloc(&ptr, datasize, cudaHostAllocWriteCombined) ); भङ्ग;
        स्वभाव:             CV_Error(cv::Error::StsBadFlag, "Invalid alloc type");
        }

        datastart = दत्त =  (uchar*)ptr;
        dataend = दत्त + nettosize;

        refcount = (पूर्णाङ्क*)cv::fastMalloc(परिमाण(*refcount));
        *refcount = 1;
    }
#यदि_अन्तः
}

HostMem cv::cuda::HostMem::reshape(पूर्णाङ्क new_cn, पूर्णाङ्क new_पङ्क्तिकाः) ध्रुव
{
    HostMem hdr = *एतत्;

    पूर्णाङ्क cn = channels();
    यदि (new_cn == 0)
        new_cn = cn;

    पूर्णाङ्क योग_विस्तारः = स्तम्भसङ्ख्या * cn;

    यदि ((new_cn > योग_विस्तारः || योग_विस्तारः % new_cn != 0) && new_पङ्क्तिकाः == 0)
        new_पङ्क्तिकाः = पङ्क्तिकाः * योग_विस्तारः / new_cn;

    यदि (new_पङ्क्तिकाः != 0 && new_पङ्क्तिकाः != पङ्क्तिकाः)
    {
        पूर्णाङ्क योग_आकार_2 = योग_विस्तारः * पङ्क्तिकाः;

        यदि (!isContinuous())
            CV_Error(cv::Error::Bad_चरणः, "The matrix is not continuous, thus its number of rows can not be changed");

        यदि ((अचिह्नित)new_पङ्क्तिकाः > (अचिह्नित)योग_आकार_2)
            CV_Error(cv::Error::Sts_बहिर्गमनम्_सम्बन्धि_Range, "Bad new number of rows");

        योग_विस्तारः = योग_आकार_2 / new_पङ्क्तिकाः;

        यदि (योग_विस्तारः * new_पङ्क्तिकाः != योग_आकार_2)
            CV_Error(cv::Error::StsBadArg, "The total number of matrix elements is not divisible by the new number of rows");

        hdr.पङ्क्तिकाः = new_पङ्क्तिकाः;
        hdr.चरणः = योग_विस्तारः * elemSize1();
    }

    पूर्णाङ्क new_विस्तारः = योग_विस्तारः / new_cn;

    यदि (new_विस्तारः * new_cn != योग_विस्तारः)
        CV_Error(cv::Error::Bad_अङ्कः_Channels, "The total width is not divisible by the new number of channels");

    hdr.स्तम्भसङ्ख्या = new_विस्तारः;
    hdr.flags = (hdr.flags & ~CV_MAT_CN_आवरणम्) | ((new_cn - 1) << CV_CN_सरणम्);

    प्रत्यागम hdr;
}

शून्य cv::cuda::HostMem::release()
{
#यदि_परिभाषितम् HAVE_CUDA
    यदि (refcount && CV_XADD(refcount, -1) == 1)
    {
        cudaFreeHost(datastart);
        fastFree(refcount);
    }

    dataend = दत्त = datastart = 0;
    चरणः = पङ्क्तिकाः = स्तम्भसङ्ख्या = 0;
    refcount = 0;
#यदि_अन्तः
}

GpuMat cv::cuda::HostMem::create_Gpu_Mat_शीर्षकम्() ध्रुव
{
#यदि_अपरिभाषितम् HAVE_CUDA
    throw_no_cuda();
#अन्यथा
    CV_Assert( alloc_type == SHARED );

    शून्य *pdev;
    cudaSafeCall( cudaHostGetDevicePointer(&pdev, दत्त, 0) );

    प्रत्यागम GpuMat(पङ्क्तिकाः, स्तम्भसङ्ख्या, type(), pdev, चरणः);
#यदि_अन्तः
}

शून्य cv::cuda::पञ्जिकाकोशः_Page_Locked(Mat& m)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(m);
    throw_no_cuda();
#अन्यथा
    CV_Assert( m.isContinuous() );
    cudaSafeCall( cuda_Host_पञ्जिकाकोशः(m.दत्त, m.चरणः * m.पङ्क्तिकाः, cuda_Host_पञ्जिकाकोशः_Portable) );
#यदि_अन्तः
}

शून्य cv::cuda::unregisterPageLocked(Mat& m)
{
#यदि_अपरिभाषितम् HAVE_CUDA
    CV_अप्रयुक्तम्(m);
#अन्यथा
    cudaSafeCall( cudaHostUnregister(m.दत्त) );
#यदि_अन्तः
}
