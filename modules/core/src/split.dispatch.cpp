// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "split.simd.hpp"
#شامل "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv { نامخلا hal {

خالی split8u(ثابت uchar* src, uchar** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی split16u(ثابت ushort* src, ushort** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی split32s(ثابت صحیح* src, صحیح** dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی split64s(ثابت int64* src, int64** dst, صحیح len, صحیح cn )
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

قسمتعریف خالی (*بانٹو_Func)(ثابت خالی* src, خالی** dst, صحیح len, صحیح cn);

سانچہ<قسم_نام T, خالی (*fn)(ثابت T*, T**, صحیح, صحیح)>
ساکن خالی بانٹو_Wrap(ثابت خالی* src, خالی** dst, صحیح len, صحیح cn)
{
    fn((ثابت T*)src, (T**)dst, len, cn);
}

ساکن بانٹو_Func get_بانٹو_Func(صحیح گہرائی)
{
    ساکن بانٹو_Func بانٹو_Tab[CV_گہرائی_MAX] =
    {
        بانٹو_Wrap<uchar, cv::hal::split8u>, بانٹو_Wrap<uchar, cv::hal::split8u>,
        بانٹو_Wrap<ushort, cv::hal::split16u>, بانٹو_Wrap<ushort, cv::hal::split16u>,
        بانٹو_Wrap<صحیح, cv::hal::split32s>, بانٹو_Wrap<صحیح, cv::hal::split32s>,
        بانٹو_Wrap<int64_t, cv::hal::split64s>, بانٹو_Wrap<ushort, cv::hal::split16u>,
        بانٹو_Wrap<ushort, cv::hal::split16u>, بانٹو_Wrap<uchar, cv::hal::split8u>,
        بانٹو_Wrap<int64_t, cv::hal::split64s>, بانٹو_Wrap<int64_t, cv::hal::split64s>,
        بانٹو_Wrap<صحیح, cv::hal::split32s>, بانٹو_Wrap<uchar, cv::hal::split8u>,
        بانٹو_Wrap<uchar, cv::hal::split8u>, 0,
    };

    واپس بانٹو_Tab[گہرائی];
}

#اگر_تعریف_شدہ HAVE_IPP

ساکن منطقی ipp_بانٹو(ثابت Mat& src, Mat* mv, صحیح channels)
{
#اگر_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(channels != 3 && channels != 4)
        واپس جھوٹ;

    اگر(src.dims <= 2)
    {
        Ippi_سائز size       = ippi_سائز(src.size());
        خالی    *dstPtrs[4] = {تہی};
        size_t   dst_قدم_3    = mv[0].قدم;
        برائے(صحیح i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            اگر(dst_قدم_3 != mv[i].قدم)
                واپس جھوٹ;
        }

        واپس CV_INSTRUMENT_FUN_IPP(llwi_Copy_بانٹو, src.ptr(), (صحیح)src.قدم, dstPtrs, (صحیح)dst_قدم_3, size, (صحیح)src.elemSize1(), channels, 0) >= 0;
    }
    ورنہ
    {
        ثابت Mat *arrays[5] = {تہی};
        uchar     *ptrs[5]   = {تہی};
        arrays[0] = &src;

        برائے(صحیح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_سائز size = { (صحیح)it.size, 1 };

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwi_Copy_بانٹو, ptrs[0], 0, (خالی**)&ptrs[1], 0, size, (صحیح)src.elemSize1(), channels, 0) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
#ورنہ
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(mv); CV_غیر_مستعمل(channels);
    واپس جھوٹ;
#اگر_ختم
}
#اگر_ختم

خالی بانٹو(ثابت Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    صحیح k, گہرائی = src.گہرائی(), cn = src.channels();
    اگر( cn == 1 )
    {
        src.copy_مقصد(mv[0]);
        واپس;
    }

    برائے( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, گہرائی);
    }

    CV_IPP_تسلسل_FAST(ipp_بانٹو(src, mv, cn));

    بانٹو_Func func = get_بانٹو_Func(گہرائی);
    CV_Assert( func != 0 );

    size_t esz = src.elem_سائز(), esz1 = src.elemSize1();
    size_t blocksize0 = (بلاک_سائز + esz-1)/esz;
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.ڈیٹا();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    برائے( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t کل = it.size;
    size_t blocksize = std::min((size_t)CV_بانٹو_ملاؤ_MAX_بلاک_سائز(cn), cn <= 4 ? کل : std::min(کل, blocksize0));

    برائے( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( size_t j = 0; j < کل; j += blocksize )
        {
            size_t bsz = std::min(کل - j, blocksize);
            func( ptrs[0], (خالی**)&ptrs[1], (صحیح)bsz, cn );

            اگر( j + blocksize < کل )
            {
                ptrs[0] += bsz*esz;
                برائے( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_بانٹو( داخلہ_Array _m, Output_Array_کا_Arrays _mv )
{
    صحیح type = _m.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type),
            صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    برائے (صحیح i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::بانٹو_ملاؤ_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memop_Type_مقصد_Str(گہرائی), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    اگر (k.empty())
        واپس جھوٹ;

    سائز size = _m.size();
    _mv.create(cn, 1, گہرائی);
    برائے (صحیح i = 0; i < cn; ++i)
        _mv.create(size, گہرائی, i);

    std::vector<UMat> dst;
    _mv.get_UMat_سمتیہ(dst);

    صحیح argidx = k.سیٹ(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    برائے (صحیح i = 0; i < cn; ++i)
        argidx = k.سیٹ(argidx, ocl::KernelArg::Write_Only_No_سائز(dst[i]));
    k.سیٹ(argidx, صفیں_فی_WI);

    size_t globalsize[2] = { (size_t)size.چوڑائی, ((size_t)size.اونچائی + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی بانٹو(داخلہ_Array _m, Output_Array_کا_Arrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_تسلسل(_m.dims() <= 2 && _mv.is_UMat_سمتیہ(),
               ocl_بانٹو(_m, _mv))

    Mat m = _m.getMat();
    اگر( m.empty() )
    {
        _mv.release();
        واپس;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.گہرائی() );

    صحیح گہرائی = m.گہرائی(), cn = m.channels();
    _mv.create(cn, 1, گہرائی);
    برائے (صحیح i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, گہرائی, i);

    std::vector<Mat> dst;
    _mv.get_Mat_سمتیہ(dst);

    بانٹو(m, &dst[0]);
}

} // namespace
