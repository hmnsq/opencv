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

تعريفنوع فراغ (*ورهايو_Func)(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn);

سانچو<قسم_نالو T, فراغ (*fn)(ثابت T*, T**, صحيح, صحيح)>
ساكن فراغ ورهايو_Wrap(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ساكن ورهايو_Func get_ورهايو_Func(صحيح اونهائي)
{
    ساكن ورهايو_Func ورهايو_Tab[CV_اونهائي_MAX] =
    {
        ورهايو_Wrap<uchar, cv::hal::split8u>, ورهايو_Wrap<uchar, cv::hal::split8u>,
        ورهايو_Wrap<ushort, cv::hal::split16u>, ورهايو_Wrap<ushort, cv::hal::split16u>,
        ورهايو_Wrap<صحيح, cv::hal::split32s>, ورهايو_Wrap<صحيح, cv::hal::split32s>,
        ورهايو_Wrap<int64_t, cv::hal::split64s>, ورهايو_Wrap<ushort, cv::hal::split16u>,
        ورهايو_Wrap<ushort, cv::hal::split16u>, ورهايو_Wrap<uchar, cv::hal::split8u>,
        ورهايو_Wrap<int64_t, cv::hal::split64s>, ورهايو_Wrap<int64_t, cv::hal::split64s>,
        ورهايو_Wrap<صحيح, cv::hal::split32s>, ورهايو_Wrap<uchar, cv::hal::split8u>,
        ورهايو_Wrap<uchar, cv::hal::split8u>, 0,
    };

    أرجع ورهايو_Tab[اونهائي];
}

#جيڪڏهن_وصف_ٿيل HAVE_IPP

ساكن منطقي ipp_ورهايو(ثابت Mat& src, Mat* mv, صحيح channels)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_سائيز size       = ippi_سائيز(src.size());
        فراغ    *dstPtrs[4] = {عدم};
        size_t   dst_قدم_3    = mv[0].قدم;
        لكل(صحيح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            إذا(dst_قدم_3 != mv[i].قدم)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_ورهايو, src.ptr(), (صحيح)src.قدم, dstPtrs, (صحيح)dst_قدم_3, size, (صحيح)src.elemSize1(), channels, 0) >= 0;
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
        Ippi_سائيز size = { (صحيح)it.size, 1 };

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ورهايو, ptrs[0], 0, (فراغ**)&ptrs[1], 0, size, (صحيح)src.elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(src); CV_اڻ_استعمال_ٿيل(mv); CV_اڻ_استعمال_ٿيل(channels);
    أرجع خطأ;
#جيڪڏهن_پورو
}
#جيڪڏهن_پورو

فراغ ورهايو(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحيح k, اونهائي = src.اونهائي(), cn = src.channels();
    إذا( cn == 1 )
    {
        src.copy_منزل(mv[0]);
        أرجع;
    }

    لكل( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, اونهائي);
    }

    CV_IPP_لاڳيتو_FAST(ipp_ورهايو(src, mv, cn));

    ورهايو_Func func = get_ورهايو_Func(اونهائي);
    CV_Assert( func != 0 );

    size_t esz = src.elem_سائيز(), esz1 = src.elemSize1();
    size_t blocksize0 = (بلاڪ_سائيز + esz-1)/esz;
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.معلومات();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    لكل( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t ڪل = it.size;
    size_t blocksize = std::min((size_t)CV_ورهايو_ملايو_MAX_بلاڪ_سائيز(cn), cn <= 4 ? ڪل : std::min(ڪل, blocksize0));

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < ڪل; j += blocksize )
        {
            size_t bsz = std::min(ڪل - j, blocksize);
            func( ptrs[0], (فراغ**)&ptrs[1], (صحيح)bsz, cn );

            إذا( j + blocksize < ڪل )
            {
                ptrs[0] += bsz*esz;
                لكل( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

ساكن منطقي ocl_ورهايو( داخلا_Array _m, Output_Array_جو_Arrays _mv )
{
    صحيح type = _m.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type),
            صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::ورهايو_ملايو_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_منزل_Str(اونهائي), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    سائيز size = _m.size();
    _mv.create(cn, 1, اونهائي);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(size, اونهائي, i);

    std::vector<UMat> dst;
    _mv.get_UMat_ويڪٽر(dst);

    صحيح argidx = k.سيٽ(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    لكل (صحيح i = 0; i < cn; ++i)
        argidx = k.سيٽ(argidx, ocl::KernelArg::Write_Only_No_سائيز(dst[i]));
    k.سيٽ(argidx, صفون_في_WI);

    size_t globalsize[2] = { (size_t)size.ويڪر, ((size_t)size.اوچائي + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

#جيڪڏهن_پورو

فراغ ورهايو(داخلا_Array _m, Output_Array_جو_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_لاڳيتو(_m.dims() <= 2 && _mv.is_UMat_ويڪٽر(),
               ocl_ورهايو(_m, _mv))

    Mat m = _m.getMat();
    إذا( m.empty() )
    {
        _mv.release();
        أرجع;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.اونهائي() );

    صحيح اونهائي = m.اونهائي(), cn = m.channels();
    _mv.create(cn, 1, اونهائي);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, اونهائي, i);

    std::vector<Mat> dst;
    _mv.get_Mat_ويڪٽر(dst);

    ورهايو(m, &dst[0]);
}

} // namespace
