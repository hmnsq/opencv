// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "split.simd.hpp"
#ضمن "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv { نطاق hal {

فراغ split8u(ثابت uchar* src, uchar** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ split16u(ثابت ushort* src, ushort** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ split32s(ثابت صحيح* src, صحيح** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

فراغ split64s(ثابت int64* src, int64** dst, صحيح len, صحيح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split64s, cv_hal_split64s, src,dst, len, cn)
    CV_CPU_DISPATCH(split64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::

/****************************************************************************************\
*                                       split & merge                                    *
\****************************************************************************************/

تعريفنوع فراغ (*وېشل_Func)(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn);

کينډۍ<ډول_نوم T, فراغ (*fn)(ثابت T*, T**, صحيح, صحيح)>
ساكن فراغ وېشل_Wrap(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ساكن وېشل_Func get_وېشل_Func(صحيح ژوروالی)
{
    ساكن وېشل_Func وېشل_Tab[CV_ژوروالی_MAX] =
    {
        وېشل_Wrap<uchar, cv::hal::split8u>, وېشل_Wrap<uchar, cv::hal::split8u>,
        وېشل_Wrap<ushort, cv::hal::split16u>, وېشل_Wrap<ushort, cv::hal::split16u>,
        وېشل_Wrap<صحيح, cv::hal::split32s>, وېشل_Wrap<صحيح, cv::hal::split32s>,
        وېشل_Wrap<int64_t, cv::hal::split64s>, وېشل_Wrap<ushort, cv::hal::split16u>,
        وېشل_Wrap<ushort, cv::hal::split16u>, وېشل_Wrap<uchar, cv::hal::split8u>,
        وېشل_Wrap<int64_t, cv::hal::split64s>, وېشل_Wrap<int64_t, cv::hal::split64s>,
        وېشل_Wrap<صحيح, cv::hal::split32s>, وېشل_Wrap<uchar, cv::hal::split8u>,
        وېشل_Wrap<uchar, cv::hal::split8u>, 0,
    };

    أرجع وېشل_Tab[ژوروالی];
}

#که_تعریف_شوی HAVE_IPP

ساكن منطقي ipp_وېشل(ثابت Mat& src, Mat* mv, صحيح channels)
{
#که_تعریف_شوی HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_اندازه size       = ippi_اندازه(src.size());
        فراغ    *dstPtrs[4] = {عدم};
        size_t   dst_ګام_3    = mv[0].ګام;
        لكل(صحيح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            إذا(dst_ګام_3 != mv[i].ګام)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_وېشل, src.ptr(), (صحيح)src.ګام, dstPtrs, (صحيح)dst_ګام_3, size, (صحيح)src.elemSize1(), channels, 0) >= 0;
    }
    وإلا
    {
        ثابت Mat *arrays[5] = {عدم};
        uchar     *ptrs[5]   = {عدم};
        arrays[0] = &src;

        لكل(صحيح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_اندازه size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_وېشل, ptrs[0], 0, (فراغ**)&ptrs[1], 0, size, (صحيح)src.elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع ریښتیا;
    }
#وإلا
    CV_ناکارول_شوی(src); CV_ناکارول_شوی(mv); CV_ناکارول_شوی(channels);
    أرجع خطأ;
#د_که_پای
}
#د_که_پای

فراغ وېشل(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحيح k, ژوروالی = src.ژوروالی(), cn = src.channels();
    إذا( cn == 1 )
    {
        src.copy_منزل(mv[0]);
        أرجع;
    }

    لكل( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, ژوروالی);
    }

    CV_IPP_پرلپسې_FAST(ipp_وېشل(src, mv, cn));

    وېشل_Func func = get_وېشل_Func(ژوروالی);
    CV_Assert( func != 0 );

    size_t esz = src.elem_اندازه(), esz1 = src.elemSize1();
    size_t blocksize0 = (بلاک_اندازه + esz-1)/esz;
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.معلومات();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    لكل( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ټول = it.size;
    size_t blocksize = std::min((size_t)CV_وېشل_یوځای_MAX_بلاک_اندازه(cn), cn <= 4 ? ټول : std::min(ټول, blocksize0));

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < ټول; j += blocksize )
        {
            size_t bsz = std::min(ټول - j, blocksize);
            func( ptrs[0], (فراغ**)&ptrs[1], (صحيح)bsz, cn );

            إذا( j + blocksize < ټول )
            {
                ptrs[0] += bsz*esz;
                لكل( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#که_تعریف_شوی HAVE_OPENCL

ساكن منطقي ocl_وېشل( ننوت_Array _m, Output_Array_د_Arrays _mv )
{
    صحيح type = _m.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type),
            لیکې_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::وېشل_یوځای_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_منزل_Str(ژوروالی), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    اندازه size = _m.size();
    _mv.create(cn, 1, ژوروالی);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(size, ژوروالی, i);

    std::vector<UMat> dst;
    _mv.get_UMat_ویکټور(dst);

    صحيح argidx = k.سېټ(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    لكل (صحيح i = 0; i < cn; ++i)
        argidx = k.سېټ(argidx, ocl::KernelArg::Write_Only_No_اندازه(dst[i]));
    k.سېټ(argidx, لیکې_فی_WI);

    size_t globalsize[2] = { (size_t)size.پلنوالی, ((size_t)size.لوړوالی + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

#د_که_پای

فراغ وېشل(ننوت_Array _m, Output_Array_د_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پرلپسې(_m.dims() <= 2 && _mv.is_UMat_ویکټور(),
               ocl_وېشل(_m, _mv))

    Mat m = _m.getMat();
    إذا( m.empty() )
    {
        _mv.release();
        أرجع;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.ژوروالی() );

    صحيح ژوروالی = m.ژوروالی(), cn = m.channels();
    _mv.create(cn, 1, ژوروالی);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, ژوروالی, i);

    std::vector<Mat> dst;
    _mv.get_Mat_ویکټور(dst);

    وېشل(m, &dst[0]);
}

} // namespace
