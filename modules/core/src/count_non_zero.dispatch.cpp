// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "stat.hpp"

#অন্তর্ভাব "count_non_zero.simd.hpp"
#অন্তর্ভাব "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

স্থির গুনতি_Non_Zero_Func get_গুনতি_Non_Zero_Tab(পূর্ণাঙ্ক depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_গুনতি_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
স্থির তার্কিক ocl_গুনতি_Non_Zero( প্রবেশ_Array _src, পূর্ণাঙ্ক & res )
{
    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    তার্কিক doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    যদি (depth == CV_64F && !doubleSupport)
        প্রত্যাগম অসত্য;

    পূর্ণাঙ্ক dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    পূর্ণাঙ্ক wgs2_aligned = 1;
    যাবত্ (wgs2_aligned < (পূর্ণাঙ্ক)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (পূর্ণাঙ্ক)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (পূর্ণাঙ্ক)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    যদি (k.run(1, &globalsize, &wgs, সত্য))
        প্রত্যাগম res = saturate_cast<পূর্ণাঙ্ক>(cv::sum(db.getMat(ACCESS_READ))[0]), সত্য;
    প্রত্যাগম অসত্য;
}
#endif

#যদি defined HAVE_IPP
স্থির তার্কিক ipp_গুনতি_Non_Zero( Mat &src, পূর্ণাঙ্ক &res )
{
    CV_INSTRUMENT_REGION_IPP();

#যদি IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    যদি(cv::ipp::get_Ipp_শীর্ষ_Features() == ippCPUID_SSE42)
        প্রত্যাগম অসত্য;
#endif

    Ipp32s  গুনতি_2 = 0;
    পূর্ণাঙ্ক     depth = src.depth();

    যদি(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        যদি(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_গুনতি_In_Range_8u_C1_R, (ধ্রুব Ipp8u *)src.ptr(), (পূর্ণাঙ্ক)src.step, size, &গুনতি_2, 0, 0);
        অন্যথা যদি(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_গুনতি_In_Range_32f_C1_R, (ধ্রুব Ipp32f *)src.ptr(), (পূর্ণাঙ্ক)src.step, size, &গুনতি_2, 0, 0);
        অন্যথা
            প্রত্যাগম অসত্য;

        যদি(status < 0)
            প্রত্যাগম অসত্য;

        res = size.width*size.height - গুনতি_2;
    }
    অন্যথা
    {
        IppStatus       status;
        ধ্রুব Mat      *arrays[] = {&src, রিক্ত};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(পূর্ণাঙ্ক)it.size*src.channels(), 1};
        res = 0;
        প্রতি (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            যদি(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_গুনতি_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (পূর্ণাঙ্ক)it.planes->step, size, &গুনতি_2, 0, 0);
            অন্যথা যদি(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_গুনতি_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (পূর্ণাঙ্ক)it.planes->step, size, &গুনতি_2, 0, 0);
            অন্যথা
                প্রত্যাগম অসত্য;

            যদি(status < 0 || (পূর্ণাঙ্ক)it.planes->total()*src.channels() < গুনতি_2)
                প্রত্যাগম অসত্য;

            res += (পূর্ণাঙ্ক)it.planes->total()*src.channels() - গুনতি_2;
        }
    }

    প্রত্যাগম সত্য;
}
#endif

পূর্ণাঙ্ক গুনতি_Non_Zero(প্রবেশ_Array _src)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#যদি defined HAVE_OPENCL || defined HAVE_IPP
    পূর্ণাঙ্ক res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_গুনতি_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_গুনতি_Non_Zero(src, res), res);

    গুনতি_Non_Zero_Func func = get_গুনতি_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    ধ্রুব Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size, nz = 0;

    প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    প্রত্যাগম nz;
}

শূন্য findNonZero(প্রবেশ_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    পূর্ণাঙ্ক depth = src.depth();
    std::vector<Point> idxvec;
    পূর্ণাঙ্ক rows = src.rows, cols = src.cols;
    AutoBuffer<পূর্ণাঙ্ক> buf_(cols + 1);
    পূর্ণাঙ্ক* buf = buf_.data();

    প্রতি( পূর্ণাঙ্ক i = 0; i < rows; i++ )
    {
        পূর্ণাঙ্ক j, k = 0;
        ধ্রুব uchar* ptr8 = src.ptr(i);
        যদি( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#যদি (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ধ্রুব পূর্ণাঙ্ক vlanes = VTraits<v_uint8>::vlanes();
            ধ্রুব v_uint8 vzero = vx_setzero_u8();
            প্রতি( ; j <= cols - vlanes; j += vlanes )
            {
                যদি( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    অনুবর্ত;
                প্রতি( পূর্ণাঙ্ক t = j; t < j + vlanes; t++ )
                    যদি( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            প্রতি( ; j < cols; j++ )
                যদি( ptr8[j] != 0 ) buf[k++] = j;
        }
        অন্যথা যদি( depth == CV_16U || depth == CV_16S )
        {
            ধ্রুব ushort* ptr16 = (ধ্রুব ushort*)ptr8;
            প্রতি( j = 0; j < cols; j++ )
                যদি( ptr16[j] != 0 ) buf[k++] = j;
        }
        অন্যথা যদি( depth == CV_32S || depth == CV_32U )
        {
            ধ্রুব পূর্ণাঙ্ক* ptr32s = (ধ্রুব পূর্ণাঙ্ক*)ptr8;
            প্রতি( j = 0; j < cols; j++ )
                যদি( ptr32s[j] != 0 ) buf[k++] = j;
        }
        অন্যথা যদি( depth == CV_64S || depth == CV_64U )
        {
            ধ্রুব int64* ptr64s = (ধ্রুব int64*)ptr8;
            প্রতি( j = 0; j < cols; j++ )
                যদি( ptr64s[j] != 0 ) buf[k++] = j;
        }
        অন্যথা যদি( depth == CV_32F )
        {
            ধ্রুব পূর্ণাঙ্ক* ptr32s = (ধ্রুব পূর্ণাঙ্ক*)ptr8;
            প্রতি( j = 0; j < cols; j++ )
                যদি( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        অন্যথা যদি( depth == CV_16F || depth == CV_16BF )
        {
            ধ্রুব ushort* ptr16 = (ধ্রুব ushort*)ptr8;
            প্রতি( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                যদি( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        অন্যথা যদি( depth == CV_8F_E4M3FN )
        {
            প্রতি( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                যদি( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        অন্যথা
        {
            ধ্রুব int64* ptr64s = (ধ্রুব int64*)ptr8;
            প্রতি( j = 0; j < cols; j++ )
                যদি( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        যদি( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            প্রতি( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    যদি( idxvec.empty() || (_idx.kind() == প্রবেশ_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    যদি( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
