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


تعريفنوع فراغ (*یوځای_Func)(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn);

کينډۍ<ډول_نوم T, فراغ (*fn)(ثابت T**, T*, صحيح, صحيح)>
ساكن فراغ یوځای_Wrap(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ساكن یوځای_Func get_یوځای_Func(صحيح ژوروالی)
{
    ساكن یوځای_Func یوځای_Tab[CV_ژوروالی_MAX] =
    {
        یوځای_Wrap<uchar, cv::hal::merge8u>, یوځای_Wrap<uchar, cv::hal::merge8u>,
        یوځای_Wrap<ushort, cv::hal::merge16u>, یوځای_Wrap<ushort, cv::hal::merge16u>,
        یوځای_Wrap<صحيح, cv::hal::merge32s>, یوځای_Wrap<صحيح, cv::hal::merge32s>,
        یوځای_Wrap<int64_t, cv::hal::merge64s>, یوځای_Wrap<ushort, cv::hal::merge16u>,
        یوځای_Wrap<ushort, cv::hal::merge16u>, یوځای_Wrap<uchar, cv::hal::merge8u>,
        یوځای_Wrap<int64_t, cv::hal::merge64s>, یوځای_Wrap<int64_t, cv::hal::merge64s>,
        یوځای_Wrap<صحيح, cv::hal::merge32s>, یوځای_Wrap<uchar, cv::hal::merge8u>,
        یوځای_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    أرجع یوځای_Tab[ژوروالی];
}

#که_تعریف_شوی HAVE_IPP

ساكن منطقي ipp_یوځای(ثابت Mat* mv, Mat& dst, صحيح channels)
{
#که_تعریف_شوی HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(mv[0].dims <= 2)
    {
        Ippi_اندازه    size       = ippi_اندازه(mv[0].size());
        ثابت فراغ *srcPtrs[4] = {عدم};
        size_t      src_ګام_3    = mv[0].ګام;
        لكل(صحيح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            إذا(src_ګام_3 != mv[i].ګام)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_یوځای, srcPtrs, (صحيح)src_ګام_3, dst.ptr(), (صحيح)dst.ګام, size, (صحيح)mv[0].elemSize1(), channels, 0) >= 0;
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
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_یوځای, (ثابت فراغ**)&ptrs[1], 0, ptrs[0], 0, size, (صحيح)mv[0].elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع ریښتیا;
    }
#وإلا
    CV_ناکارول_شوی(dst); CV_ناکارول_شوی(mv); CV_ناکارول_شوی(channels);
    أرجع خطأ;
#د_که_پای
}
#د_که_پای

فراغ یوځای(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحيح ژوروالی = mv[0].ژوروالی();
    منطقي allch1 = ریښتیا;
    صحيح k, cn = 0;
    size_t i;

    لكل( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].ژوروالی() == ژوروالی);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(ژوروالی, cn));
    Mat dst = _dst.getMat();

    إذا( n == 1 )
    {
        mv[0].copy_منزل(dst);
        أرجع;
    }

    CV_IPP_پرلپسې(allch1, ipp_یوځای(mv, dst, (صحيح)n));

    إذا( !allch1 )
    {
        Auto_بفر<صحيح> جوړې(cn*2);
        صحيح j, ni=0;

        لكل( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            لكل( k = 0; k < ni; k++ )
            {
                جوړې[(j+k)*2] = j + k;
                جوړې[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &جوړې[0], cn );
        أرجع;
    }

    یوځای_Func func = get_یوځای_Func(ژوروالی);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_اندازه(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحيح)((بلاک_اندازه + esz-1)/esz);
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.معلومات();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    لكل( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ټول = (صحيح)it.size;
    size_t blocksize = std::min((size_t)CV_وېشل_یوځای_MAX_بلاک_اندازه(cn), cn <= 4 ? ټول : std::min(ټول, blocksize0));

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < ټول; j += blocksize )
        {
            size_t bsz = std::min(ټول - j, blocksize);
            func( (ثابت فراغ**)&ptrs[1], ptrs[0], (صحيح)bsz, cn );

            إذا( j + blocksize < ټول )
            {
                ptrs[0] += bsz*esz;
                لكل( صحيح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_یوځای( ننوت_Array_د_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_ویکټور(src);
    CV_Assert(!src.empty());

    صحيح type = src[0].type(), ژوروالی = CV_MAT_ژوروالی(type),
            لیکې_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    اندازه size = src[0].size();

    لكل (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحيح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_ژوروالی(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        إذا (src[i].dims > 2)
            أرجع خطأ;

        CV_Assert(size == src[i].size() && ژوروالی == idepth);

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

    ocl::Kernel k("merge", ocl::core::وېشل_یوځای_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_منزل_Str(ژوروالی), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    _dst.create(size, CV_MAKE_TYPE(ژوروالی, dcn));
    UMat dst = _dst.getUMat();

    صحيح argidx = 0;
    لكل (صحيح i = 0; i < dcn; ++i)
        argidx = k.سېټ(argidx, ocl::KernelArg::Read_Only_No_اندازه(ksrc[i]));
    argidx = k.سېټ(argidx, ocl::KernelArg::WriteOnly(dst));
    k.سېټ(argidx, لیکې_فی_WI);

    size_t globalsize[2] = { (size_t)dst.ستون_شمېر, ((size_t)dst.لیکې + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

#د_که_پای

فراغ یوځای(ننوت_Array_د_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پرلپسې(_mv.is_UMat_ویکټور() && _dst.isUMat(),
               ocl_یوځای(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_ویکټور(mv);
    یوځای(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
