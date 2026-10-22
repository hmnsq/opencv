// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "stat.hpp"

#అన్తర్భావ "count_non_zero.simd.hpp"
#అన్తర్భావ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv {

స్థిర లెక్క_Non_Zero_Func get_లెక్క_Non_Zero_Tab(పూర్ణాంకం లోతు)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_లెక్క_Non_Zero_Tab, (లోతు),
        CV_CPU_DISPATCH_MODES_ALL);
}

#నిర్వచితమైతే HAVE_OPENCL
స్థిర తార్కిక ocl_లెక్క_Non_Zero( ఆదానం_Array _src, పూర్ణాంకం & res )
{
    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), kercn = ocl::predict_Optimal_సదిశ_వెడల్పు(_src);
    తార్కిక doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ఒకవేళ (లోతు == CV_64F && !doubleSupport)
        తిరిగి అబద్ధం;

    పూర్ణాంకం dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().max_Work_సమూహం_కొలత();

    పూర్ణాంకం wgs2_aligned = 1;
    అయితే (wgs2_aligned < (పూర్ణాంకం)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, kercn)),
                         ocl::type_గమ్యం_Str(లోతు), (పూర్ణాంకం)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.వాదనలు(ocl::KernelArg::Read_Only_No_కొలత(src), src.నిలువుల_సంఖ్య, (పూర్ణాంకం)src.మొత్తం(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ఒకవేళ (k.నిరంతరం(1, &globalsize, &wgs, నిజం))
        తిరిగి res = saturate_cast<పూర్ణాంకం>(cv::కూడిక(db.getMat(ACCESS_READ))[0]), నిజం;
    తిరిగి అబద్ధం;
}
#అయితే_ముగింపు

#ఒకవేళ defined HAVE_IPP
స్థిర తార్కిక ipp_లెక్క_Non_Zero( Mat &src, పూర్ణాంకం &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ఒకవేళ IPP_సంస్కరణ_X100 < 201801
    // Poor performance of SSE42
    ఒకవేళ(cv::ipp::get_Ipp_శిఖరం_Features() == ippCPUID_SSE42)
        తిరిగి అబద్ధం;
#అయితే_ముగింపు

    Ipp32s  లెక్క_2 = 0;
    పూర్ణాంకం     లోతు = src.లోతు();

    ఒకవేళ(src.dims <= 2)
    {
        Ipp_స్థితి స్థితి_2;
        Ippi_కొలత  size = {src.నిలువుల_సంఖ్య*src.channels(), src.అడ్డువరుసలు};

        ఒకవేళ(లోతు == CV_8U)
            స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippi_లెక్క_లోపల_Range_8u_C1_R, (స్థిరరాశి Ipp8u *)src.ptr(), (పూర్ణాంకం)src.అడుగు, size, &లెక్క_2, 0, 0);
        లేకపోతే ఒకవేళ(లోతు == CV_32F)
            స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippi_లెక్క_లోపల_Range_32f_C1_R, (స్థిరరాశి Ipp32f *)src.ptr(), (పూర్ణాంకం)src.అడుగు, size, &లెక్క_2, 0, 0);
        లేకపోతే
            తిరిగి అబద్ధం;

        ఒకవేళ(స్థితి_2 < 0)
            తిరిగి అబద్ధం;

        res = size.వెడల్పు*size.ఎత్తు - లెక్క_2;
    }
    లేకపోతే
    {
        Ipp_స్థితి       స్థితి_2;
        స్థిరరాశి Mat      *arrays[] = {&src, ఖాళీ};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_కొలత        size  = {(పూర్ణాంకం)it.size*src.channels(), 1};
        res = 0;
        కొరకు (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ఒకవేళ(లోతు == CV_8U)
                స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippi_లెక్క_లోపల_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (పూర్ణాంకం)it.planes->అడుగు, size, &లెక్క_2, 0, 0);
            లేకపోతే ఒకవేళ(లోతు == CV_32F)
                స్థితి_2 = CV_INSTRUMENT_FUN_IPP(ippi_లెక్క_లోపల_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (పూర్ణాంకం)it.planes->అడుగు, size, &లెక్క_2, 0, 0);
            లేకపోతే
                తిరిగి అబద్ధం;

            ఒకవేళ(స్థితి_2 < 0 || (పూర్ణాంకం)it.planes->మొత్తం()*src.channels() < లెక్క_2)
                తిరిగి అబద్ధం;

            res += (పూర్ణాంకం)it.planes->మొత్తం()*src.channels() - లెక్క_2;
        }
    }

    తిరిగి నిజం;
}
#అయితే_ముగింపు

పూర్ణాంకం లెక్క_Non_Zero(ఆదానం_Array _src)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ఒకవేళ defined HAVE_OPENCL || defined HAVE_IPP
    పూర్ణాంకం res = -1;
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL
    CV_OCL_నిరంతరం_2(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_లెక్క_Non_Zero(_src, res),
                res)
#అయితే_ముగింపు

    Mat src = _src.getMat();
    CV_IPP_నిరంతరం_FAST(ipp_లెక్క_Non_Zero(src, res), res);

    లెక్క_Non_Zero_Func func = get_లెక్క_Non_Zero_Tab(src.లోతు());
    CV_Assert( func != 0 );

    స్థిరరాశి Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size, nz = 0;

    కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], మొత్తం );

    తిరిగి nz;
}

శూన్య కనుగొను_Non_Zero(ఆదానం_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    పూర్ణాంకం లోతు = src.లోతు();
    std::vector<బిందువు> idxvec;
    పూర్ణాంకం అడ్డువరుసలు = src.అడ్డువరుసలు, నిలువుల_సంఖ్య = src.నిలువుల_సంఖ్య;
    Auto_బఫరు<పూర్ణాంకం> buf_(నిలువుల_సంఖ్య + 1);
    పూర్ణాంకం* buf = buf_.దత్తాంశం();

    కొరకు( పూర్ణాంకం i = 0; i < అడ్డువరుసలు; i++ )
    {
        పూర్ణాంకం j, k = 0;
        స్థిరరాశి uchar* ptr8 = src.ptr(i);
        ఒకవేళ( లోతు == CV_8U || లోతు == CV_8S || లోతు == CV_Bool || లోతు == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ఒకవేళ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            స్థిరరాశి పూర్ణాంకం vlanes = VTraits<v_uint8>::vlanes();
            స్థిరరాశి v_uint8 vzero = vx_setzero_u8();
            కొరకు( ; j <= నిలువుల_సంఖ్య - vlanes; j += vlanes )
            {
                ఒకవేళ( !v_check_any(v_ne(vx_ఎక్కించు(ptr8 + j), vzero)) )
                    కొనసాగు;
                కొరకు( పూర్ణాంకం t = j; t < j + vlanes; t++ )
                    ఒకవేళ( ptr8[t] != 0 ) buf[k++] = t;
            }
#అయితే_ముగింపు
            కొరకు( ; j < నిలువుల_సంఖ్య; j++ )
                ఒకవేళ( ptr8[j] != 0 ) buf[k++] = j;
        }
        లేకపోతే ఒకవేళ( లోతు == CV_16U || లోతు == CV_16S )
        {
            స్థిరరాశి ushort* ptr16 = (స్థిరరాశి ushort*)ptr8;
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                ఒకవేళ( ptr16[j] != 0 ) buf[k++] = j;
        }
        లేకపోతే ఒకవేళ( లోతు == CV_32S || లోతు == CV_32U )
        {
            స్థిరరాశి పూర్ణాంకం* ptr32s = (స్థిరరాశి పూర్ణాంకం*)ptr8;
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                ఒకవేళ( ptr32s[j] != 0 ) buf[k++] = j;
        }
        లేకపోతే ఒకవేళ( లోతు == CV_64S || లోతు == CV_64U )
        {
            స్థిరరాశి int64* ptr64s = (స్థిరరాశి int64*)ptr8;
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                ఒకవేళ( ptr64s[j] != 0 ) buf[k++] = j;
        }
        లేకపోతే ఒకవేళ( లోతు == CV_32F )
        {
            స్థిరరాశి పూర్ణాంకం* ptr32s = (స్థిరరాశి పూర్ణాంకం*)ptr8;
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                ఒకవేళ( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        లేకపోతే ఒకవేళ( లోతు == CV_16F || లోతు == CV_16BF )
        {
            స్థిరరాశి ushort* ptr16 = (స్థిరరాశి ushort*)ptr8;
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ఒకవేళ( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        లేకపోతే ఒకవేళ( లోతు == CV_8F_E4M3FN )
        {
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ఒకవేళ( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        లేకపోతే
        {
            స్థిరరాశి int64* ptr64s = (స్థిరరాశి int64*)ptr8;
            కొరకు( j = 0; j < నిలువుల_సంఖ్య; j++ )
                ఒకవేళ( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ఒకవేళ( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            కొరకు( j = 0; j < k; j++ )
                idxvec[sz + j] = బిందువు(buf[j], i);
        }
    }

    ఒకవేళ( idxvec.empty() || (_idx.రకం() == ఆదానం_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ఒకవేళ( !idxvec.empty() )
        Mat(idxvec).copy_గమ్యం(_idx);
}

} // namespace
