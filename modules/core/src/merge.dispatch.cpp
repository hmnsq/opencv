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


تعريفنوع فراغ (*ملايو_Func)(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn);

سانچو<قسم_نالو T, فراغ (*fn)(ثابت T**, T*, صحيح, صحيح)>
ساكن فراغ ملايو_Wrap(ثابت فراغ** src, فراغ* dst, صحيح len, صحيح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ساكن ملايو_Func get_ملايو_Func(صحيح اونهائي)
{
    ساكن ملايو_Func ملايو_Tab[CV_اونهائي_MAX] =
    {
        ملايو_Wrap<uchar, cv::hal::merge8u>, ملايو_Wrap<uchar, cv::hal::merge8u>,
        ملايو_Wrap<ushort, cv::hal::merge16u>, ملايو_Wrap<ushort, cv::hal::merge16u>,
        ملايو_Wrap<صحيح, cv::hal::merge32s>, ملايو_Wrap<صحيح, cv::hal::merge32s>,
        ملايو_Wrap<int64_t, cv::hal::merge64s>, ملايو_Wrap<ushort, cv::hal::merge16u>,
        ملايو_Wrap<ushort, cv::hal::merge16u>, ملايو_Wrap<uchar, cv::hal::merge8u>,
        ملايو_Wrap<int64_t, cv::hal::merge64s>, ملايو_Wrap<int64_t, cv::hal::merge64s>,
        ملايو_Wrap<صحيح, cv::hal::merge32s>, ملايو_Wrap<uchar, cv::hal::merge8u>,
        ملايو_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    أرجع ملايو_Tab[اونهائي];
}

#جيڪڏهن_وصف_ٿيل HAVE_IPP

ساكن منطقي ipp_ملايو(ثابت Mat* mv, Mat& dst, صحيح channels)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(mv[0].dims <= 2)
    {
        Ippi_سائيز    size       = ippi_سائيز(mv[0].size());
        ثابت فراغ *srcPtrs[4] = {عدم};
        size_t      src_قدم_3    = mv[0].قدم;
        لكل(صحيح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            إذا(src_قدم_3 != mv[i].قدم)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_ملايو, srcPtrs, (صحيح)src_قدم_3, dst.ptr(), (صحيح)dst.قدم, size, (صحيح)mv[0].elemSize1(), channels, 0) >= 0;
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
        Ippi_سائيز size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ملايو, (ثابت فراغ**)&ptrs[1], 0, ptrs[0], 0, size, (صحيح)mv[0].elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(dst); CV_اڻ_استعمال_ٿيل(mv); CV_اڻ_استعمال_ٿيل(channels);
    أرجع خطأ;
#جيڪڏهن_پورو
}
#جيڪڏهن_پورو

فراغ ملايو(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحيح اونهائي = mv[0].اونهائي();
    منطقي allch1 = سچ;
    صحيح k, cn = 0;
    size_t i;

    لكل( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].اونهائي() == اونهائي);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(اونهائي, cn));
    Mat dst = _dst.getMat();

    إذا( n == 1 )
    {
        mv[0].copy_منزل(dst);
        أرجع;
    }

    CV_IPP_لاڳيتو(allch1, ipp_ملايو(mv, dst, (صحيح)n));

    إذا( !allch1 )
    {
        Auto_بفر<صحيح> جوڙا(cn*2);
        صحيح j, ni=0;

        لكل( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            لكل( k = 0; k < ni; k++ )
            {
                جوڙا[(j+k)*2] = j + k;
                جوڙا[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &جوڙا[0], cn );
        أرجع;
    }

    ملايو_Func func = get_ملايو_Func(اونهائي);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_سائيز(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحيح)((بلاڪ_سائيز + esz-1)/esz);
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.معلومات();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    لكل( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ڪل = (صحيح)it.size;
    size_t blocksize = std::min((size_t)CV_ورهايو_ملايو_MAX_بلاڪ_سائيز(cn), cn <= 4 ? ڪل : std::min(ڪل, blocksize0));

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < ڪل; j += blocksize )
        {
            size_t bsz = std::min(ڪل - j, blocksize);
            func( (ثابت فراغ**)&ptrs[1], ptrs[0], (صحيح)bsz, cn );

            إذا( j + blocksize < ڪل )
            {
                ptrs[0] += bsz*esz;
                لكل( صحيح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_ملايو( داخلا_Array_جو_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_ويڪٽر(src);
    CV_Assert(!src.empty());

    صحيح type = src[0].type(), اونهائي = CV_MAT_اونهائي(type),
            صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    سائيز size = src[0].size();

    لكل (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحيح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_اونهائي(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        إذا (src[i].dims > 2)
            أرجع خطأ;

        CV_Assert(size == src[i].size() && اونهائي == idepth);

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

    ocl::Kernel k("merge", ocl::core::ورهايو_ملايو_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_منزل_Str(اونهائي), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    _dst.create(size, CV_MAKE_TYPE(اونهائي, dcn));
    UMat dst = _dst.getUMat();

    صحيح argidx = 0;
    لكل (صحيح i = 0; i < dcn; ++i)
        argidx = k.سيٽ(argidx, ocl::KernelArg::Read_Only_No_سائيز(ksrc[i]));
    argidx = k.سيٽ(argidx, ocl::KernelArg::WriteOnly(dst));
    k.سيٽ(argidx, صفون_في_WI);

    size_t globalsize[2] = { (size_t)dst.کانن_ڳڻپ, ((size_t)dst.صفون + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

#جيڪڏهن_پورو

فراغ ملايو(داخلا_Array_جو_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_لاڳيتو(_mv.is_UMat_ويڪٽر() && _dst.isUMat(),
               ocl_ملايو(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_ويڪٽر(mv);
    ملايو(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
