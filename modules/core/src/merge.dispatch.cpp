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


تعريفنوع فراغ (*ملاؤ_Func)(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn);

سانچا<قسم_ناں T, فراغ (*fn)(ثابت T**, T*, صحيح, صحيح)>
ساكن فراغ ملاؤ_Wrap(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ساكن ملاؤ_Func get_ملاؤ_Func(صحيح ڈونگھائی)
{
    ساكن ملاؤ_Func ملاؤ_Tab[CV_ڈونگھائی_MAX] =
    {
        ملاؤ_Wrap<uchar, cv::hal::merge8u>, ملاؤ_Wrap<uchar, cv::hal::merge8u>,
        ملاؤ_Wrap<ushort, cv::hal::merge16u>, ملاؤ_Wrap<ushort, cv::hal::merge16u>,
        ملاؤ_Wrap<صحيح, cv::hal::merge32s>, ملاؤ_Wrap<صحيح, cv::hal::merge32s>,
        ملاؤ_Wrap<int64_t, cv::hal::merge64s>, ملاؤ_Wrap<ushort, cv::hal::merge16u>,
        ملاؤ_Wrap<ushort, cv::hal::merge16u>, ملاؤ_Wrap<uchar, cv::hal::merge8u>,
        ملاؤ_Wrap<int64_t, cv::hal::merge64s>, ملاؤ_Wrap<int64_t, cv::hal::merge64s>,
        ملاؤ_Wrap<صحيح, cv::hal::merge32s>, ملاؤ_Wrap<uchar, cv::hal::merge8u>,
        ملاؤ_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    أرجع ملاؤ_Tab[ڈونگھائی];
}

#جے_تعریف_شدہ HAVE_IPP

ساكن منطقي ipp_ملاؤ(ثابت Mat* mv, Mat& dst, صحيح channels)
{
#جے_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(mv[0].dims <= 2)
    {
        Ippi_ناپ    size       = ippi_ناپ(mv[0].size());
        ثابت فراغ *srcPtrs[4] = {عدم};
        size_t      src_قدم_3    = mv[0].قدم;
        لكل(صحيح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            إذا(src_قدم_3 != mv[i].قدم)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_ملاؤ, srcPtrs, (صحيح)src_قدم_3, dst.ptr(), (صحيح)dst.قدم, size, (صحيح)mv[0].elemSize1(), channels, 0) >= 0;
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
        Ippi_ناپ size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ملاؤ, (ثابت فراغ**)&ptrs[1], 0, ptrs[0], 0, size, (صحيح)mv[0].elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_بن_ورتیا(dst); CV_بن_ورتیا(mv); CV_بن_ورتیا(channels);
    أرجع خطأ;
#جے_انت
}
#جے_انت

فراغ ملاؤ(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحيح ڈونگھائی = mv[0].ڈونگھائی();
    منطقي allch1 = سچ;
    صحيح k, cn = 0;
    size_t i;

    لكل( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].ڈونگھائی() == ڈونگھائی);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(ڈونگھائی, cn));
    Mat dst = _dst.getMat();

    إذا( n == 1 )
    {
        mv[0].copy_منزل(dst);
        أرجع;
    }

    CV_IPP_لگاتار(allch1, ipp_ملاؤ(mv, dst, (صحيح)n));

    إذا( !allch1 )
    {
        Auto_بفر<صحيح> جوڑے(cn*2);
        صحيح j, ni=0;

        لكل( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            لكل( k = 0; k < ni; k++ )
            {
                جوڑے[(j+k)*2] = j + k;
                جوڑے[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &جوڑے[0], cn );
        أرجع;
    }

    ملاؤ_Func func = get_ملاؤ_Func(ڈونگھائی);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_ناپ(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحيح)((بلاک_ناپ + esz-1)/esz);
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.انکڑے();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    لكل( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t کل = (صحيح)it.size;
    size_t blocksize = std::min((size_t)CV_ونڈو_ملاؤ_MAX_بلاک_ناپ(cn), cn <= 4 ? کل : std::min(کل, blocksize0));

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < کل; j += blocksize )
        {
            size_t bsz = std::min(کل - j, blocksize);
            func( (ثابت فراغ**)&ptrs[1], ptrs[0], (صحيح)bsz, cn );

            إذا( j + blocksize < کل )
            {
                ptrs[0] += bsz*esz;
                لكل( صحيح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#جے_تعریف_شدہ HAVE_OPENCL

ساكن منطقي ocl_ملاؤ( داخلہ_Array_دا_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_سمتیہ(src);
    CV_Assert(!src.empty());

    صحيح type = src[0].type(), ڈونگھائی = CV_MAT_ڈونگھائی(type),
            صفاں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    ناپ size = src[0].size();

    لكل (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحيح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_ڈونگھائی(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        إذا (src[i].dims > 2)
            أرجع خطأ;

        CV_Assert(size == src[i].size() && ڈونگھائی == idepth);

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

    ocl::Kernel k("merge", ocl::core::ونڈو_ملاؤ_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_منزل_Str(ڈونگھائی), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    _dst.create(size, CV_MAKE_TYPE(ڈونگھائی, dcn));
    UMat dst = _dst.getUMat();

    صحيح argidx = 0;
    لكل (صحيح i = 0; i < dcn; ++i)
        argidx = k.سیٹ(argidx, ocl::KernelArg::Read_Only_No_ناپ(ksrc[i]));
    argidx = k.سیٹ(argidx, ocl::KernelArg::WriteOnly(dst));
    k.سیٹ(argidx, صفاں_فی_WI);

    size_t globalsize[2] = { (size_t)dst.خانے_گنتی, ((size_t)dst.صفاں + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

#جے_انت

فراغ ملاؤ(داخلہ_Array_دا_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_لگاتار(_mv.is_UMat_سمتیہ() && _dst.isUMat(),
               ocl_ملاؤ(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_سمتیہ(mv);
    ملاؤ(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
