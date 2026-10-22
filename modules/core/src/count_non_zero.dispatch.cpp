// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.hpp"

#ଅନ୍ତର୍ଭାଵ "count_non_zero.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv {

ସ୍ଥିର ଗଣନା_Non_Zero_Func get_ଗଣନା_Non_Zero_Tab(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ଗଣନା_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ସ୍ଥିର ତାର୍କିକ ocl_ଗଣନା_Non_Zero( ନିବେଶ_Array _src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା & res )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    ତାର୍କିକ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ଯଦି (depth == CV_64F && !doubleSupport)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା wgs2_aligned = 1;
    ଯେତେବେଳେ (wgs2_aligned < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ଯଦି (k.run(1, &globalsize, &wgs, ସତ୍ୟ))
        ଫେରନ୍ତୁ res = saturate_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(cv::sum(db.getMat(ACCESS_READ))[0]), ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
#endif

#ଯଦି defined HAVE_IPP
ସ୍ଥିର ତାର୍କିକ ipp_ଗଣନା_Non_Zero( Mat &src, ପୂର୍ଣ୍ଣସଂଖ୍ୟା &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ଯଦି IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    ଯଦି(cv::ipp::get_Ipp_ଶୀର୍ଷ_Features() == ippCPUID_SSE42)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif

    Ipp32s  ଗଣନା_3 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା     depth = src.depth();

    ଯଦି(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        ଯଦି(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_ଗଣନା_In_Range_8u_C1_R, (ଅଚଳ Ipp8u *)src.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, size, &ଗଣନା_3, 0, 0);
        ଅନ୍ୟଥା ଯଦି(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_ଗଣନା_In_Range_32f_C1_R, (ଅଚଳ Ipp32f *)src.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, size, &ଗଣନା_3, 0, 0);
        ଅନ୍ୟଥା
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        ଯଦି(status < 0)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        res = size.width*size.height - ଗଣନା_3;
    }
    ଅନ୍ୟଥା
    {
        IppStatus       status;
        ଅଚଳ Mat      *arrays[] = {&src, ଖାଲି};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size*src.channels(), 1};
        res = 0;
        ପାଇଁ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ଯଦି(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_ଗଣନା_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.planes->step, size, &ଗଣନା_3, 0, 0);
            ଅନ୍ୟଥା ଯଦି(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_ଗଣନା_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.planes->step, size, &ଗଣନା_3, 0, 0);
            ଅନ୍ୟଥା
                ଫେରନ୍ତୁ ମିଥ୍ୟା;

            ଯଦି(status < 0 || (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.planes->total()*src.channels() < ଗଣନା_3)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;

            res += (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.planes->total()*src.channels() - ଗଣନା_3;
        }
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
}
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_Non_Zero(ନିବେଶ_Array _src)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ଯଦି defined HAVE_OPENCL || defined HAVE_IPP
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ଗଣନା_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_ଗଣନା_Non_Zero(src, res), res);

    ଗଣନା_Non_Zero_Func func = get_ଗଣନା_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    ଅଚଳ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, nz = 0;

    ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    ଫେରନ୍ତୁ nz;
}

ଶୂନ୍ୟ findNonZero(ନିବେଶ_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src.depth();
    std::vector<Point> idxvec;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows = src.rows, cols = src.cols;
    AutoBuffer<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> buf_(cols + 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* buf = buf_.data();

    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < rows; i++ )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, k = 0;
        ଅଚଳ uchar* ptr8 = src.ptr(i);
        ଯଦି( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ଯଦି (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା vlanes = VTraits<v_uint8>::vlanes();
            ଅଚଳ v_uint8 vzero = vx_setzero_u8();
            ପାଇଁ( ; j <= cols - vlanes; j += vlanes )
            {
                ଯଦି( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    ଜାରି;
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = j; t < j + vlanes; t++ )
                    ଯଦି( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            ପାଇଁ( ; j < cols; j++ )
                ଯଦି( ptr8[j] != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_16U || depth == CV_16S )
        {
            ଅଚଳ ushort* ptr16 = (ଅଚଳ ushort*)ptr8;
            ପାଇଁ( j = 0; j < cols; j++ )
                ଯଦି( ptr16[j] != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_32S || depth == CV_32U )
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ptr32s = (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)ptr8;
            ପାଇଁ( j = 0; j < cols; j++ )
                ଯଦି( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_64S || depth == CV_64U )
        {
            ଅଚଳ int64* ptr64s = (ଅଚଳ int64*)ptr8;
            ପାଇଁ( j = 0; j < cols; j++ )
                ଯଦି( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_32F )
        {
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ptr32s = (ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)ptr8;
            ପାଇଁ( j = 0; j < cols; j++ )
                ଯଦି( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_16F || depth == CV_16BF )
        {
            ଅଚଳ ushort* ptr16 = (ଅଚଳ ushort*)ptr8;
            ପାଇଁ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ଯଦି( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା ଯଦି( depth == CV_8F_E4M3FN )
        {
            ପାଇଁ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ଯଦି( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ଅନ୍ୟଥା
        {
            ଅଚଳ int64* ptr64s = (ଅଚଳ int64*)ptr8;
            ପାଇଁ( j = 0; j < cols; j++ )
                ଯଦି( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ଯଦି( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            ପାଇଁ( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    ଯଦି( idxvec.empty() || (_idx.kind() == ନିବେଶ_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ଯଦି( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
