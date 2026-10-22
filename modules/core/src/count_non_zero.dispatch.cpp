// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "stat.hpp"

#அந்தர்பாவ "count_non_zero.simd.hpp"
#அந்தர்பாவ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv {

நிலையான மொத்தம்_Non_Zero_Func get_மொத்தம்_Non_Zero_Tab(முழு depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_மொத்தம்_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
நிலையான தருக்க ocl_மொத்தம்_Non_Zero( உள்ளீடு_Array _src, முழு & res )
{
    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    தருக்க doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    எனில் (depth == CV_64F && !doubleSupport)
        திரும்பு பொய்;

    முழு dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    முழு wgs2_aligned = 1;
    போது (wgs2_aligned < (முழு)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (முழு)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    எனில் (k.empty())
        திரும்பு பொய்;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (முழு)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    எனில் (k.run(1, &globalsize, &wgs, உண்மை))
        திரும்பு res = saturate_cast<முழு>(cv::sum(db.getMat(ACCESS_READ))[0]), உண்மை;
    திரும்பு பொய்;
}
#endif

#எனில் defined HAVE_IPP
நிலையான தருக்க ipp_மொத்தம்_Non_Zero( Mat &src, முழு &res )
{
    CV_INSTRUMENT_REGION_IPP();

#எனில் IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    எனில்(cv::ipp::get_Ipp_உச்சி_Features() == ippCPUID_SSE42)
        திரும்பு பொய்;
#endif

    Ipp32s  மொத்தம்_2 = 0;
    முழு     depth = src.depth();

    எனில்(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        எனில்(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_மொத்தம்_In_Range_8u_C1_R, (மாறிலி Ipp8u *)src.ptr(), (முழு)src.step, size, &மொத்தம்_2, 0, 0);
        இல்லையெனில் எனில்(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_மொத்தம்_In_Range_32f_C1_R, (மாறிலி Ipp32f *)src.ptr(), (முழு)src.step, size, &மொத்தம்_2, 0, 0);
        இல்லையெனில்
            திரும்பு பொய்;

        எனில்(status < 0)
            திரும்பு பொய்;

        res = size.width*size.height - மொத்தம்_2;
    }
    இல்லையெனில்
    {
        IppStatus       status;
        மாறிலி Mat      *arrays[] = {&src, வெற்று};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(முழு)it.size*src.channels(), 1};
        res = 0;
        க்கு (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            எனில்(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_மொத்தம்_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (முழு)it.planes->step, size, &மொத்தம்_2, 0, 0);
            இல்லையெனில் எனில்(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_மொத்தம்_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (முழு)it.planes->step, size, &மொத்தம்_2, 0, 0);
            இல்லையெனில்
                திரும்பு பொய்;

            எனில்(status < 0 || (முழு)it.planes->total()*src.channels() < மொத்தம்_2)
                திரும்பு பொய்;

            res += (முழு)it.planes->total()*src.channels() - மொத்தம்_2;
        }
    }

    திரும்பு உண்மை;
}
#endif

முழு மொத்தம்_Non_Zero(உள்ளீடு_Array _src)
{
    CV_INSTRUMENT_REGION();

    முழு type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#எனில் defined HAVE_OPENCL || defined HAVE_IPP
    முழு res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_மொத்தம்_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_மொத்தம்_Non_Zero(src, res), res);

    மொத்தம்_Non_Zero_Func func = get_மொத்தம்_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    மாறிலி Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    முழு total = (முழு)it.size, nz = 0;

    க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    திரும்பு nz;
}

வெறுமை findNonZero(உள்ளீடு_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    முழு depth = src.depth();
    std::vector<Point> idxvec;
    முழு rows = src.rows, cols = src.cols;
    AutoBuffer<முழு> buf_(cols + 1);
    முழு* buf = buf_.data();

    க்கு( முழு i = 0; i < rows; i++ )
    {
        முழு j, k = 0;
        மாறிலி uchar* ptr8 = src.ptr(i);
        எனில்( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#எனில் (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            மாறிலி முழு vlanes = VTraits<v_uint8>::vlanes();
            மாறிலி v_uint8 vzero = vx_setzero_u8();
            க்கு( ; j <= cols - vlanes; j += vlanes )
            {
                எனில்( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    தொடர்;
                க்கு( முழு t = j; t < j + vlanes; t++ )
                    எனில்( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            க்கு( ; j < cols; j++ )
                எனில்( ptr8[j] != 0 ) buf[k++] = j;
        }
        இல்லையெனில் எனில்( depth == CV_16U || depth == CV_16S )
        {
            மாறிலி ushort* ptr16 = (மாறிலி ushort*)ptr8;
            க்கு( j = 0; j < cols; j++ )
                எனில்( ptr16[j] != 0 ) buf[k++] = j;
        }
        இல்லையெனில் எனில்( depth == CV_32S || depth == CV_32U )
        {
            மாறிலி முழு* ptr32s = (மாறிலி முழு*)ptr8;
            க்கு( j = 0; j < cols; j++ )
                எனில்( ptr32s[j] != 0 ) buf[k++] = j;
        }
        இல்லையெனில் எனில்( depth == CV_64S || depth == CV_64U )
        {
            மாறிலி int64* ptr64s = (மாறிலி int64*)ptr8;
            க்கு( j = 0; j < cols; j++ )
                எனில்( ptr64s[j] != 0 ) buf[k++] = j;
        }
        இல்லையெனில் எனில்( depth == CV_32F )
        {
            மாறிலி முழு* ptr32s = (மாறிலி முழு*)ptr8;
            க்கு( j = 0; j < cols; j++ )
                எனில்( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        இல்லையெனில் எனில்( depth == CV_16F || depth == CV_16BF )
        {
            மாறிலி ushort* ptr16 = (மாறிலி ushort*)ptr8;
            க்கு( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                எனில்( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        இல்லையெனில் எனில்( depth == CV_8F_E4M3FN )
        {
            க்கு( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                எனில்( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        இல்லையெனில்
        {
            மாறிலி int64* ptr64s = (மாறிலி int64*)ptr8;
            க்கு( j = 0; j < cols; j++ )
                எனில்( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        எனில்( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            க்கு( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    எனில்( idxvec.empty() || (_idx.kind() == உள்ளீடு_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    எனில்( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
