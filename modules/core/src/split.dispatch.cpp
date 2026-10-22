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

تعريفنوع فراغ (*تقسیم_Func)(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn);

قالب<نام_نوع T, فراغ (*fn)(ثابت T*, T**, صحيح, صحيح)>
ساكن فراغ تقسیم_Wrap(ثابت فراغ* src, فراغ** dst, صحيح len, صحيح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ساكن تقسیم_Func get_تقسیم_Func(صحيح عمق)
{
    ساكن تقسیم_Func تقسیم_Tab[CV_عمق_MAX] =
    {
        تقسیم_Wrap<uchar, cv::hal::split8u>, تقسیم_Wrap<uchar, cv::hal::split8u>,
        تقسیم_Wrap<ushort, cv::hal::split16u>, تقسیم_Wrap<ushort, cv::hal::split16u>,
        تقسیم_Wrap<صحيح, cv::hal::split32s>, تقسیم_Wrap<صحيح, cv::hal::split32s>,
        تقسیم_Wrap<int64_t, cv::hal::split64s>, تقسیم_Wrap<ushort, cv::hal::split16u>,
        تقسیم_Wrap<ushort, cv::hal::split16u>, تقسیم_Wrap<uchar, cv::hal::split8u>,
        تقسیم_Wrap<int64_t, cv::hal::split64s>, تقسیم_Wrap<int64_t, cv::hal::split64s>,
        تقسیم_Wrap<صحيح, cv::hal::split32s>, تقسیم_Wrap<uchar, cv::hal::split8u>,
        تقسیم_Wrap<uchar, cv::hal::split8u>, 0,
    };

    أرجع تقسیم_Tab[عمق];
}

#اگر_تعریف_شده HAVE_IPP

ساكن منطقي ipp_تقسیم(ثابت Mat& src, Mat* mv, صحيح channels)
{
#اگر_تعریف_شده HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    إذا(channels != 3 && channels != 4)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_اندازه size       = ippi_اندازه(src.size());
        فراغ    *dstPtrs[4] = {عدم};
        size_t   dst_گام_3    = mv[0].گام;
        لكل(صحيح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            إذا(dst_گام_3 != mv[i].گام)
                أرجع خطأ;
        }

        أرجع CV_INSTRUMENT_FUN_IPP(llwi_Copy_تقسیم, src.ptr(), (صحيح)src.گام, dstPtrs, (صحيح)dst_گام_3, size, (صحيح)src.elemSize1(), channels, 0) >= 0;
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
            إذا(CV_INSTRUMENT_FUN_IPP(llwi_Copy_تقسیم, ptrs[0], 0, (فراغ**)&ptrs[1], 0, size, (صحيح)src.elemSize1(), channels, 0) < 0)
                أرجع خطأ;
        }
        أرجع درست;
    }
#وإلا
    CV_استفادهنشده(src); CV_استفادهنشده(mv); CV_استفادهنشده(channels);
    أرجع خطأ;
#پایان_اگر
}
#پایان_اگر

فراغ تقسیم(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحيح k, عمق = src.عمق(), cn = src.channels();
    إذا( cn == 1 )
    {
        src.copy_مقصد(mv[0]);
        أرجع;
    }

    لكل( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, عمق);
    }

    CV_IPP_پیاپی_FAST(ipp_تقسیم(src, mv, cn));

    تقسیم_Func func = get_تقسیم_Func(عمق);
    CV_Assert( func != 0 );

    size_t esz = src.elem_اندازه(), esz1 = src.elemSize1();
    size_t blocksize0 = (بلوک_اندازه + esz-1)/esz;
    Auto_میانگیر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.داده();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    لكل( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t مجموع = it.size;
    size_t blocksize = std::min((size_t)CV_تقسیم_ادغام_MAX_بلوک_اندازه(cn), cn <= 4 ? مجموع : std::min(مجموع, blocksize0));

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( size_t j = 0; j < مجموع; j += blocksize )
        {
            size_t bsz = std::min(مجموع - j, blocksize);
            func( ptrs[0], (فراغ**)&ptrs[1], (صحيح)bsz, cn );

            إذا( j + blocksize < مجموع )
            {
                ptrs[0] += bsz*esz;
                لكل( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#اگر_تعریف_شده HAVE_OPENCL

ساكن منطقي ocl_تقسیم( درون_داد_Array _m, Output_Array_از_Arrays _mv )
{
    صحيح type = _m.type(), عمق = CV_MAT_عمق(type), cn = CV_MAT_CN(type),
            ردیفها_در_هر_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    لكل (صحيح i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::تقسیم_ادغام_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_مقصد_Str(عمق), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    اندازه size = _m.size();
    _mv.create(cn, 1, عمق);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(size, عمق, i);

    std::vector<UMat> dst;
    _mv.get_UMat_وکتور(dst);

    صحيح argidx = k.مجموعه(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    لكل (صحيح i = 0; i < cn; ++i)
        argidx = k.مجموعه(argidx, ocl::KernelArg::Write_Only_No_اندازه(dst[i]));
    k.مجموعه(argidx, ردیفها_در_هر_WI);

    size_t globalsize[2] = { (size_t)size.عرض, ((size_t)size.ارتفاع + ردیفها_در_هر_WI - 1) / ردیفها_در_هر_WI };
    أرجع k.پیاپی(2, globalsize, عدم, خطأ);
}

#پایان_اگر

فراغ تقسیم(درون_داد_Array _m, Output_Array_از_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_پیاپی(_m.dims() <= 2 && _mv.is_UMat_وکتور(),
               ocl_تقسیم(_m, _mv))

    Mat m = _m.getMat();
    إذا( m.empty() )
    {
        _mv.release();
        أرجع;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.عمق() );

    صحيح عمق = m.عمق(), cn = m.channels();
    _mv.create(cn, 1, عمق);
    لكل (صحيح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, عمق, i);

    std::vector<Mat> dst;
    _mv.get_Mat_وکتور(dst);

    تقسیم(m, &dst[0]);
}

} // namespace
