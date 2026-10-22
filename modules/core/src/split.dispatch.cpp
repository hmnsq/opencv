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

تعريفنوع فراغ (*ونڈو_Func)(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn);

سانچا<قسم_ناں T, فراغ (*fn)(ثابت T*, T**, صحيح, صحيح)>
ساكن فراغ ونڈو_Wrap(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ساكن ونڈو_Func get_ونڈو_Func(صحيح ڈونگھائی)
{
    ساكن ونڈو_Func ونڈو_Tab[CV_ڈونگھائی_MAX] =
    {
        ونڈو_Wrap<uchar, cv::hal::split8u>, ونڈو_Wrap<uchar, cv::hal::split8u>,
        ونڈو_Wrap<ushort, cv::hal::split16u>, ونڈو_Wrap<ushort, cv::hal::split16u>,
        ونڈو_Wrap<صحيح, cv::hal::split32s>, ونڈو_Wrap<صحيح, cv::hal::split32s>,
        ونڈو_Wrap<int64_t, cv::hal::split64s>, ونڈو_Wrap<ushort, cv::hal::split16u>,
        ونڈو_Wrap<ushort, cv::hal::split16u>, ونڈو_Wrap<uchar, cv::hal::split8u>,
        ونڈو_Wrap<int64_t, cv::hal::split64s>, ونڈو_Wrap<int64_t, cv::hal::split64s>,
        ونڈو_Wrap<صحيح, cv::hal::split32s>, ونڈو_Wrap<uchar, cv::hal::split8u>,
        ونڈو_Wrap<uchar, cv::hal::split8u>, 0,
    };

    أرجع ونڈو_Tab[ڈونگھائی];
}

#جے_تعریف_شدہ HAVE_IPP

ساكن منطقي ipp_ونڈو(ثابت Mat& src, Mat* mv, صحيح channels)
{
#جے_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_ناپ size       = ippi_ناپ(src.size());
        فراغ    *dstPtrs[4] = {عدم};
        size_t   dst_قدم_3    = mv[0].قدم;
        لكل(صحيح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            إذا(dst_قدم_3 != mv[i].قدم)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_ونڈو, src.ptr(), (صحيح)src.قدم, dstPtrs, (صحيح)dst_قدم_3, size, (صحيح)src.elemSize1(), channels, 0) >= 0;
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
        Ippi_ناپ size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ونڈو, ptrs[0], 0, (فراغ**)&ptrs[1], 0, size, (صحيح)src.elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_بن_ورتیا(src); CV_بن_ورتیا(mv); CV_بن_ورتیا(channels);
    أرجع خطأ;
#جے_انت
}
#جے_انت

فراغ ونڈو(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحيح k, ڈونگھائی = src.ڈونگھائی(), cn = src.channels();
    إذا( cn == 1 )
    {
        src.copy_منزل(mv[0]);
        أرجع;
    }

    لكل( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, ڈونگھائی);
    }

    CV_IPP_لگاتار_FAST(ipp_ونڈو(src, mv, cn));

    ونڈو_Func func = get_ونڈو_Func(ڈونگھائی);
    CV_Assert( func != 0 );

    size_t esz = src.elem_ناپ(), esz1 = src.elemSize1();
    size_t blocksize0 = (بلاک_ناپ + esz-1)/esz;
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.انکڑے();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    لكل( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t کل = it.size;
    size_t blocksize = std::min((size_t)CV_ونڈو_ملاؤ_MAX_بلاک_ناپ(cn), cn <= 4 ? کل : std::min(کل, blocksize0));

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < کل; j += blocksize )
        {
            size_t bsz = std::min(کل - j, blocksize);
            func( ptrs[0], (فراغ**)&ptrs[1], (صحيح)bsz, cn );

            إذا( j + blocksize < کل )
            {
                ptrs[0] += bsz*esz;
                لكل( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#جے_تعریف_شدہ HAVE_OPENCL

ساكن منطقي ocl_ونڈو( داخلہ_Array _m, Output_Array_دا_Arrays _mv )
{
    صحيح type = _m.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type),
            صفاں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::ونڈو_ملاؤ_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_منزل_Str(ڈونگھائی), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    ناپ size = _m.size();
    _mv.create(cn, 1, ڈونگھائی);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(size, ڈونگھائی, i);

    std::vector<UMat> dst;
    _mv.get_UMat_سمتیہ(dst);

    صحيح argidx = k.سیٹ(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    لكل (صحيح i = 0; i < cn; ++i)
        argidx = k.سیٹ(argidx, ocl::KernelArg::Write_Only_No_ناپ(dst[i]));
    k.سیٹ(argidx, صفاں_فی_WI);

    size_t globalsize[2] = { (size_t)size.چوڑائی, ((size_t)size.اچائی + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

#جے_انت

فراغ ونڈو(داخلہ_Array _m, Output_Array_دا_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_لگاتار(_m.dims() <= 2 && _mv.is_UMat_سمتیہ(),
               ocl_ونڈو(_m, _mv))

    Mat m = _m.getMat();
    إذا( m.empty() )
    {
        _mv.release();
        أرجع;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.ڈونگھائی() );

    صحيح ڈونگھائی = m.ڈونگھائی(), cn = m.channels();
    _mv.create(cn, 1, ڈونگھائی);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, ڈونگھائی, i);

    std::vector<Mat> dst;
    _mv.get_Mat_سمتیہ(dst);

    ونڈو(m, &dst[0]);
}

} // namespace
