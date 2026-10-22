// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "count_non_zero.simd.hpp"
#ضمن "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن CountNonZeroFunc getCountNonZeroTab(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ساكن منطقي ocl_countNonZero( InputArray _src, صحيح & res )
{
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    אם (depth == CV_64F && !doubleSupport)
        החזר שקר;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    صحيح wgs2_aligned = 1;
    כלעוד (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    אם (k.empty())
        החזר שקר;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    אם (k.run(1, &globalsize, &wgs, אמת))
        החזר res = saturate_cast<صحيح>(cv::sum(db.getMat(ACCESS_READ))[0]), אמת;
    החזר שקר;
}
#endif

#אם defined HAVE_IPP
ساكن منطقي ipp_countNonZero( Mat &src, صحيح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#אם IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    אם(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        החזר שקר;
#endif

    Ipp32s  count = 0;
    صحيح     depth = src.depth();

    אם(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        אם(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (ثابت Ipp8u *)src.ptr(), (صحيح)src.step, size, &count, 0, 0);
        אחרת אם(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (ثابت Ipp32f *)src.ptr(), (صحيح)src.step, size, &count, 0, 0);
        אחרת
            החזר שקר;

        אם(status < 0)
            החזר שקר;

        res = size.width*size.height - count;
    }
    אחרת
    {
        IppStatus       status;
        ثابت Mat      *arrays[] = {&src, عدم};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(صحيح)it.size*src.channels(), 1};
        res = 0;
        עבור (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            אם(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (صحيح)it.planes->step, size, &count, 0, 0);
            אחרת אם(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (صحيح)it.planes->step, size, &count, 0, 0);
            אחרת
                החזר שקר;

            אם(status < 0 || (صحيح)it.planes->total()*src.channels() < count)
                החזר שקר;

            res += (صحيح)it.planes->total()*src.channels() - count;
        }
    }

    החזר אמת;
}
#endif

صحيح countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#אם defined HAVE_OPENCL || defined HAVE_IPP
    صحيح res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_countNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_countNonZero(src, res), res);

    CountNonZeroFunc func = getCountNonZeroTab(src.depth());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح total = (صحيح)it.size, nz = 0;

    עבור( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    החזר nz;
}

فراغ findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحيح depth = src.depth();
    std::vector<Point> idxvec;
    صحيح rows = src.rows, cols = src.cols;
    AutoBuffer<صحيح> buf_(cols + 1);
    صحيح* buf = buf_.data();

    עבור( صحيح i = 0; i < rows; i++ )
    {
        صحيح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        אם( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#אם (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            עבור( ; j <= cols - vlanes; j += vlanes )
            {
                אם( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    استمر;
                עבור( صحيح t = j; t < j + vlanes; t++ )
                    אם( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            עבור( ; j < cols; j++ )
                אם( ptr8[j] != 0 ) buf[k++] = j;
        }
        אחרת אם( depth == CV_16U || depth == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            עבור( j = 0; j < cols; j++ )
                אם( ptr16[j] != 0 ) buf[k++] = j;
        }
        אחרת אם( depth == CV_32S || depth == CV_32U )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            עבור( j = 0; j < cols; j++ )
                אם( ptr32s[j] != 0 ) buf[k++] = j;
        }
        אחרת אם( depth == CV_64S || depth == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            עבור( j = 0; j < cols; j++ )
                אם( ptr64s[j] != 0 ) buf[k++] = j;
        }
        אחרת אם( depth == CV_32F )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            עבור( j = 0; j < cols; j++ )
                אם( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        אחרת אם( depth == CV_16F || depth == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            עבור( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                אם( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        אחרת אם( depth == CV_8F_E4M3FN )
        {
            עבור( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                אם( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        אחרת
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            עבור( j = 0; j < cols; j++ )
                אם( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        אם( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            עבור( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    אם( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    אם( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
