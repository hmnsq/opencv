// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "stat.hpp"

#شامل "count_non_zero.simd.hpp"
#شامل "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

فضاینام cv {

ایستا شمار_Non_Zero_Func get_شمار_Non_Zero_Tab(صحیح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_شمار_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ایستا منطقی ocl_شمار_Non_Zero( درون_داد_Array _src, صحیح & res )
{
    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    منطقی doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    اگر (depth == CV_64F && !doubleSupport)
        بازگشت نادرست;

    صحیح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    صحیح wgs2_aligned = 1;
    وقتی (wgs2_aligned < (صحیح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (صحیح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    اگر (k.empty())
        بازگشت نادرست;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحیح)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    اگر (k.run(1, &globalsize, &wgs, درست))
        بازگشت res = saturate_cast<صحیح>(cv::sum(db.getMat(ACCESS_READ))[0]), درست;
    بازگشت نادرست;
}
#endif

#اگر defined HAVE_IPP
ایستا منطقی ipp_شمار_Non_Zero( Mat &src, صحیح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#اگر IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    اگر(cv::ipp::get_Ipp_بالاترین_Features() == ippCPUID_SSE42)
        بازگشت نادرست;
#endif

    Ipp32s  شمار_2 = 0;
    صحیح     depth = src.depth();

    اگر(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        اگر(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_شمار_In_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحیح)src.step, size, &شمار_2, 0, 0);
        وگرنه اگر(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_شمار_In_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحیح)src.step, size, &شمار_2, 0, 0);
        وگرنه
            بازگشت نادرست;

        اگر(status < 0)
            بازگشت نادرست;

        res = size.width*size.height - شمار_2;
    }
    وگرنه
    {
        IppStatus       status;
        ثابت Mat      *arrays[] = {&src, پوچ};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(صحیح)it.size*src.channels(), 1};
        res = 0;
        برای (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            اگر(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_شمار_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحیح)it.planes->step, size, &شمار_2, 0, 0);
            وگرنه اگر(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_شمار_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحیح)it.planes->step, size, &شمار_2, 0, 0);
            وگرنه
                بازگشت نادرست;

            اگر(status < 0 || (صحیح)it.planes->total()*src.channels() < شمار_2)
                بازگشت نادرست;

            res += (صحیح)it.planes->total()*src.channels() - شمار_2;
        }
    }

    بازگشت درست;
}
#endif

صحیح شمار_Non_Zero(درون_داد_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#اگر defined HAVE_OPENCL || defined HAVE_IPP
    صحیح res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_شمار_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_شمار_Non_Zero(src, res), res);

    شمار_Non_Zero_Func func = get_شمار_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحیح total = (صحیح)it.size, nz = 0;

    برای( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    بازگشت nz;
}

تهی findNonZero(درون_داد_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحیح depth = src.depth();
    std::vector<Point> idxvec;
    صحیح rows = src.rows, cols = src.cols;
    AutoBuffer<صحیح> buf_(cols + 1);
    صحیح* buf = buf_.data();

    برای( صحیح i = 0; i < rows; i++ )
    {
        صحیح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        اگر( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#اگر (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحیح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            برای( ; j <= cols - vlanes; j += vlanes )
            {
                اگر( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    ادامه;
                برای( صحیح t = j; t < j + vlanes; t++ )
                    اگر( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            برای( ; j < cols; j++ )
                اگر( ptr8[j] != 0 ) buf[k++] = j;
        }
        وگرنه اگر( depth == CV_16U || depth == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            برای( j = 0; j < cols; j++ )
                اگر( ptr16[j] != 0 ) buf[k++] = j;
        }
        وگرنه اگر( depth == CV_32S || depth == CV_32U )
        {
            ثابت صحیح* ptr32s = (ثابت صحیح*)ptr8;
            برای( j = 0; j < cols; j++ )
                اگر( ptr32s[j] != 0 ) buf[k++] = j;
        }
        وگرنه اگر( depth == CV_64S || depth == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            برای( j = 0; j < cols; j++ )
                اگر( ptr64s[j] != 0 ) buf[k++] = j;
        }
        وگرنه اگر( depth == CV_32F )
        {
            ثابت صحیح* ptr32s = (ثابت صحیح*)ptr8;
            برای( j = 0; j < cols; j++ )
                اگر( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        وگرنه اگر( depth == CV_16F || depth == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            برای( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                اگر( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        وگرنه اگر( depth == CV_8F_E4M3FN )
        {
            برای( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                اگر( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        وگرنه
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            برای( j = 0; j < cols; j++ )
                اگر( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        اگر( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            برای( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    اگر( idxvec.empty() || (_idx.kind() == درون_داد_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    اگر( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
