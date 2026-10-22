// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "merge.simd.hpp"
#شامل "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نامخلا cv { نامخلا hal {

خالی merge8u(ثابت uchar** src, uchar* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی merge16u(ثابت ushort** src, ushort* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی merge32s(ثابت صحیح** src, صحیح* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

خالی merge64s(ثابت int64** src, int64* dst, صحیح len, صحیح cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


قسمتعریف خالی (*ملاؤ_Func)(ثابت خالی** src, خالی* dst, صحیح len, صحیح cn);

سانچہ<قسم_نام T, خالی (*fn)(ثابت T**, T*, صحیح, صحیح)>
ساکن خالی ملاؤ_Wrap(ثابت خالی** src, خالی* dst, صحیح len, صحیح cn)
{
    fn((ثابت T**)src, (T*)dst, len, cn);
}

ساکن ملاؤ_Func get_ملاؤ_Func(صحیح گہرائی)
{
    ساکن ملاؤ_Func ملاؤ_Tab[CV_گہرائی_MAX] =
    {
        ملاؤ_Wrap<uchar, cv::hal::merge8u>, ملاؤ_Wrap<uchar, cv::hal::merge8u>,
        ملاؤ_Wrap<ushort, cv::hal::merge16u>, ملاؤ_Wrap<ushort, cv::hal::merge16u>,
        ملاؤ_Wrap<صحیح, cv::hal::merge32s>, ملاؤ_Wrap<صحیح, cv::hal::merge32s>,
        ملاؤ_Wrap<int64_t, cv::hal::merge64s>, ملاؤ_Wrap<ushort, cv::hal::merge16u>,
        ملاؤ_Wrap<ushort, cv::hal::merge16u>, ملاؤ_Wrap<uchar, cv::hal::merge8u>,
        ملاؤ_Wrap<int64_t, cv::hal::merge64s>, ملاؤ_Wrap<int64_t, cv::hal::merge64s>,
        ملاؤ_Wrap<صحیح, cv::hal::merge32s>, ملاؤ_Wrap<uchar, cv::hal::merge8u>,
        ملاؤ_Wrap<uchar, cv::hal::merge8u>, 0,
    };

    واپس ملاؤ_Tab[گہرائی];
}

#اگر_تعریف_شدہ HAVE_IPP

ساکن منطقی ipp_ملاؤ(ثابت Mat* mv, Mat& dst, صحیح channels)
{
#اگر_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    اگر(channels != 3 && channels != 4)
        واپس جھوٹ;

    اگر(mv[0].dims <= 2)
    {
        Ippi_سائز    size       = ippi_سائز(mv[0].size());
        ثابت خالی *srcPtrs[4] = {تہی};
        size_t      src_قدم_3    = mv[0].قدم;
        برائے(صحیح i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            اگر(src_قدم_3 != mv[i].قدم)
                واپس جھوٹ;
        }

        واپس CV_INSTRUMENT_FUN_IPP(llwi_Copy_ملاؤ, srcPtrs, (صحیح)src_قدم_3, dst.ptr(), (صحیح)dst.قدم, size, (صحیح)mv[0].elemSize1(), channels, 0) >= 0;
    }
    ورنہ
    {
        ثابت Mat *arrays[5] = {تہی};
        uchar     *ptrs[5]   = {تہی};
        arrays[0] = &dst;

        برائے(صحیح i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_سائز size = { (صحیح)it.size, 1 };

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwi_Copy_ملاؤ, (ثابت خالی**)&ptrs[1], 0, ptrs[0], 0, size, (صحیح)mv[0].elemSize1(), channels, 0) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
#ورنہ
    CV_غیر_مستعمل(dst); CV_غیر_مستعمل(mv); CV_غیر_مستعمل(channels);
    واپس جھوٹ;
#اگر_ختم
}
#اگر_ختم

خالی ملاؤ(ثابت Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    صحیح گہرائی = mv[0].گہرائی();
    منطقی allch1 = سچ;
    صحیح k, cn = 0;
    size_t i;

    برائے( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].گہرائی() == گہرائی);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(گہرائی, cn));
    Mat dst = _dst.getMat();

    اگر( n == 1 )
    {
        mv[0].copy_مقصد(dst);
        واپس;
    }

    CV_IPP_تسلسل(allch1, ipp_ملاؤ(mv, dst, (صحیح)n));

    اگر( !allch1 )
    {
        Auto_بفر<صحیح> جوڑے(cn*2);
        صحیح j, ni=0;

        برائے( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            برائے( k = 0; k < ni; k++ )
            {
                جوڑے[(j+k)*2] = j + k;
                جوڑے[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &جوڑے[0], cn );
        واپس;
    }

    ملاؤ_Func func = get_ملاؤ_Func(گہرائی);
    CV_Assert( func != 0 );

    size_t esz = dst.elem_سائز(), esz1 = dst.elemSize1();
    size_t blocksize0 = (صحیح)((بلاک_سائز + esz-1)/esz);
    Auto_بفر<uchar> _buf((cn+1)*(حجم(Mat*) + حجم(uchar*)) + 16);
    ثابت Mat** arrays = (ثابت Mat**)_buf.ڈیٹا();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    برائے( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t کل = (صحیح)it.size;
    size_t blocksize = std::min((size_t)CV_بانٹو_ملاؤ_MAX_بلاک_سائز(cn), cn <= 4 ? کل : std::min(کل, blocksize0));

    برائے( i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( size_t j = 0; j < کل; j += blocksize )
        {
            size_t bsz = std::min(کل - j, blocksize);
            func( (ثابت خالی**)&ptrs[1], ptrs[0], (صحیح)bsz, cn );

            اگر( j + blocksize < کل )
            {
                ptrs[0] += bsz*esz;
                برائے( صحیح t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#اگر_تعریف_شدہ HAVE_OPENCL

ساکن منطقی ocl_ملاؤ( داخلہ_Array_کا_Arrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.get_UMat_سمتیہ(src);
    CV_Assert(!src.empty());

    صحیح type = src[0].type(), گہرائی = CV_MAT_گہرائی(type),
            صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    سائز size = src[0].size();

    برائے (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        صحیح itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_گہرائی(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        اگر (src[i].dims > 2)
            واپس جھوٹ;

        CV_Assert(size == src[i].size() && گہرائی == idepth);

        برائے (صحیح cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    صحیح dcn = (صحیح)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    برائے (صحیح i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::بانٹو_ملاؤ_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memop_Type_مقصد_Str(گہرائی), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    اگر (k.empty())
        واپس جھوٹ;

    _dst.create(size, CV_MAKE_TYPE(گہرائی, dcn));
    UMat dst = _dst.getUMat();

    صحیح argidx = 0;
    برائے (صحیح i = 0; i < dcn; ++i)
        argidx = k.سیٹ(argidx, ocl::KernelArg::Read_Only_No_سائز(ksrc[i]));
    argidx = k.سیٹ(argidx, ocl::KernelArg::WriteOnly(dst));
    k.سیٹ(argidx, صفیں_فی_WI);

    size_t globalsize[2] = { (size_t)dst.ستون_تعداد, ((size_t)dst.صفیں + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

#اگر_ختم

خالی ملاؤ(داخلہ_Array_کا_Arrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_تسلسل(_mv.is_UMat_سمتیہ() && _dst.isUMat(),
               ocl_ملاؤ(_mv, _dst))

    std::vector<Mat> mv;
    _mv.get_Mat_سمتیہ(mv);
    ملاؤ(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
