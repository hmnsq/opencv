// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "merge.simd.hpp"
#ضمن "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv { نطاق hal {

فراغ merge8u(ثابت uchar** src, uchar* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ merge16u(ثابت ushort** src, ushort* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ merge32s(ثابت صحيح** src, صحيح* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ merge64s(ثابت int64** src, int64* dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


تعريفنوع فراغ (*ادغام_Func)(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn);

قالب<نام_نوع T, فراغ (*fn)(ثابت T**, T*, صحيح, صحيح)>
ساكن فراغ ادغام_Wrap(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ساكن ادغام_Func get_ادغام_Func(صحيح عمق)
{
    ساكن ادغام_Func ادغام_Tab[CV_عمق_MAX] =
    {
        ادغام_Wrap<uchar, cv::hal::merge8u>, ادغام_Wrap<uchar, cv::hal::merge8u>,
        ادغام_Wrap<ushort, cv::hal::merge16u>, ادغام_Wrap<ushort, cv::hal::merge16u>,
        ادغام_Wrap<صحيح, cv::hal::merge32s>, ادغام_Wrap<صحيح, cv::hal::merge32s>,
        ادغام_Wrap<int64_t, cv::hal::merge64s>, ادغام_Wrap<ushort, cv::hal::merge16u>,
        ادغام_Wrap<ushort, cv::hal::merge16u>, ادغام_Wrap<uchar, cv::hal::merge8u>,
        ادغام_Wrap<int64_t, cv::hal::merge64s>, ادغام_Wrap<int64_t, cv::hal::merge64s>,
        ادغام_Wrap<صحيح, cv::hal::merge32s>, ادغام_Wrap<uchar, cv::hal::merge8u>,
        ادغام_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    أرجع ادغام_Tab[عمق];
}

#اگر_تعریف_شده HAVE_IPP

ساكن منطقي ipp_ادغام(ثابت Mat* mv, Mat& dst, صحيح channels)
{
#اگر_تعریف_شده HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(mv[0].dims <= 2)
    {
        Ippi_اندازه    size       = ippi_اندازه(mv[0].size());
        ثابت فراغ *srcPtrs[4] = {عدم};
        size_t      src_گام_3    = mv[0].گام;
        لكل(صحيح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            إذا(src_گام_3 != mv[i].گام)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_ادغام, srcPtrs, (صحيح)src_گام_3, dst.ptr(), (صحيح)dst.گام, size, (صحيح)mv[0].elemSize1(), channels, 0) >= 0;
    }
    وإلا
    {
        ثابت Mat *arrays[5] = {عدم};
        uchar     *ptrs[5]   = {عدم};
        arrays[0] = &dst;

        لكل(صحيح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_اندازه size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ادغام, (ثابت فراغ**)&ptrs[1], 0, ptrs[0], 0, size, (صحيح)mv[0].elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع درست;
    }
#وإلا
    CV_استفادهنشده(dst); CV_استفادهنشده(mv); CV_استفادهنشده(channels);
    أرجع خطأ;
#پایان_اگر
}
#پایان_اگر

فراغ ادغام(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحيح عمق = mv[0].عمق();
    منطقي allch1 = درست;
    صحيح k, cn = 0;
    size_t i;

    لكل( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].عمق() == عمق);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(عمق, cn));
    Mat dst = _dst.getMat();

    إذا( n == 1 )
    {
        mv[0].copy_مقصد(dst);
        أرجع;
    }

    CV_IPP_پیاپی(allch1, ipp_ادغام(mv, dst, (صحيح)n));

    إذا( !allch1 )
    {
        Auto_میانگیر<صحيح> جفتها(cn*2);
        صحيح j, ni=0;

        لكل( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            لكل( k = 0; k < ni; k++ )
            {
                جفتها[(j+k)*2] = j + k;
                جفتها[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &جفتها[0], cn );
        أرجع;
    }

    ادغام_Func func = get_ادغام_Func(عمق);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_اندازه(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحيح)((بلوک_اندازه + esz-1)/esz);
    Auto_میانگیر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.داده();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    لكل( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t مجموع = (صحيح)it.size;
    size_t blocksize = std::min((size_t)CV_تقسیم_ادغام_MAX_بلوک_اندازه(cn), cn <= 4 ? مجموع : std::min(مجموع, blocksize0));

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < مجموع; j += blocksize )
        {
            size_t bsz = std::min(مجموع - j, blocksize);
            func( (ثابت فراغ**)&ptrs[1], ptrs[0], (صحيح)bsz, cn );

            إذا( j + blocksize < مجموع )
            {
                ptrs[0] += bsz*esz;
                لكل( صحيح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_ادغام( درون_داد_Array_از_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_وکتور(src);
    CV_Assert(!src.empty());

    صحيح type = src[0].type(), عمق = CV_MAT_عمق(type),
            ردیفها_در_هر_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    اندازه size = src[0].size();

    لكل (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحيح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_عمق(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        إذا (src[i].dims > 2)
            أرجع خطأ;

        CV_Assert(size == src[i].size() && عمق == idepth);

        لكل (صحيح cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    صحيح dcn = (صحيح)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    لكل (صحيح i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::تقسیم_ادغام_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_مقصد_Str(عمق), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    _dst.create(size, CV_MAKE_TYPE(عمق, dcn));
    UMat dst = _dst.getUMat();

    صحيح argidx = 0;
    لكل (صحيح i = 0; i < dcn; ++i)
        argidx = k.مجموعه(argidx, ocl::KernelArg::Read_Only_No_اندازه(ksrc[i]));
    argidx = k.مجموعه(argidx, ocl::KernelArg::WriteOnly(dst));
    k.مجموعه(argidx, ردیفها_در_هر_WI);

    size_t globalsize[2] = { (size_t)dst.تعداد_ستون, ((size_t)dst.ردیفها + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

فراغ ادغام(درون_داد_Array_از_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی(_mv.is_UMat_وکتور() && _dst.isUMat(),
               ocl_ادغام(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_وکتور(mv);
    ادغام(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
