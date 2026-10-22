// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "count_non_zero.simd.hpp"
#अन्तर्भाव "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर गणनम्_Non_Zero_Func get_गणनम्_Non_Zero_Tab(पूर्णाङ्क depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_गणनम्_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_गणनम्_Non_Zero( निवेशः_Array _src, पूर्णाङ्क & res )
{
    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    सचे (depth == CV_64F && !doubleSupport)
        पटिनिवत्त मिच्छा;

    पूर्णाङ्क dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    पूर्णाङ्क wgs2_aligned = 1;
    याव (wgs2_aligned < (पूर्णाङ्क)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (पूर्णाङ्क)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णाङ्क)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    सचे (k.run(1, &globalsize, &wgs, सच्च))
        पटिनिवत्त res = saturate_cast<पूर्णाङ्क>(cv::sum(db.getMat(ACCESS_READ))[0]), सच्च;
    पटिनिवत्त मिच्छा;
}
#endif

#सचे defined HAVE_IPP
स्थिर तार्किक ipp_गणनम्_Non_Zero( Mat &src, पूर्णाङ्क &res )
{
    CV_INSTRUMENT_REGION_IPP();

#सचे IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    सचे(cv::ipp::get_Ipp_शिखरम्_Features() == ippCPUID_SSE42)
        पटिनिवत्त मिच्छा;
#endif

    Ipp32s  गणनम्_2 = 0;
    पूर्णाङ्क     depth = src.depth();

    सचे(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        सचे(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_गणनम्_In_Range_8u_C1_R, (ध्रुव Ipp8u *)src.ptr(), (पूर्णाङ्क)src.step, size, &गणनम्_2, 0, 0);
        अञ्ञथा सचे(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_गणनम्_In_Range_32f_C1_R, (ध्रुव Ipp32f *)src.ptr(), (पूर्णाङ्क)src.step, size, &गणनम्_2, 0, 0);
        अञ्ञथा
            पटिनिवत्त मिच्छा;

        सचे(status < 0)
            पटिनिवत्त मिच्छा;

        res = size.width*size.height - गणनम्_2;
    }
    अञ्ञथा
    {
        IppStatus       status;
        ध्रुव Mat      *arrays[] = {&src, रिक्त};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(पूर्णाङ्क)it.size*src.channels(), 1};
        res = 0;
        अत्थाय (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            सचे(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_गणनम्_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (पूर्णाङ्क)it.planes->step, size, &गणनम्_2, 0, 0);
            अञ्ञथा सचे(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_गणनम्_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (पूर्णाङ्क)it.planes->step, size, &गणनम्_2, 0, 0);
            अञ्ञथा
                पटिनिवत्त मिच्छा;

            सचे(status < 0 || (पूर्णाङ्क)it.planes->total()*src.channels() < गणनम्_2)
                पटिनिवत्त मिच्छा;

            res += (पूर्णाङ्क)it.planes->total()*src.channels() - गणनम्_2;
        }
    }

    पटिनिवत्त सच्च;
}
#endif

पूर्णाङ्क गणनम्_Non_Zero(निवेशः_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#सचे defined HAVE_OPENCL || defined HAVE_IPP
    पूर्णाङ्क res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_गणनम्_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_गणनम्_Non_Zero(src, res), res);

    गणनम्_Non_Zero_Func func = get_गणनम्_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    ध्रुव Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णाङ्क total = (पूर्णाङ्क)it.size, nz = 0;

    अत्थाय( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    पटिनिवत्त nz;
}

शून्य findNonZero(निवेशः_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    पूर्णाङ्क depth = src.depth();
    std::vector<Point> idxvec;
    पूर्णाङ्क rows = src.rows, cols = src.cols;
    AutoBuffer<पूर्णाङ्क> buf_(cols + 1);
    पूर्णाङ्क* buf = buf_.data();

    अत्थाय( पूर्णाङ्क i = 0; i < rows; i++ )
    {
        पूर्णाङ्क j, k = 0;
        ध्रुव uchar* ptr8 = src.ptr(i);
        सचे( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#सचे (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ध्रुव पूर्णाङ्क vlanes = VTraits<v_uint8>::vlanes();
            ध्रुव v_uint8 vzero = vx_setzero_u8();
            अत्थाय( ; j <= cols - vlanes; j += vlanes )
            {
                सचे( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    अनुवर्त;
                अत्थाय( पूर्णाङ्क t = j; t < j + vlanes; t++ )
                    सचे( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            अत्थाय( ; j < cols; j++ )
                सचे( ptr8[j] != 0 ) buf[k++] = j;
        }
        अञ्ञथा सचे( depth == CV_16U || depth == CV_16S )
        {
            ध्रुव ushort* ptr16 = (ध्रुव ushort*)ptr8;
            अत्थाय( j = 0; j < cols; j++ )
                सचे( ptr16[j] != 0 ) buf[k++] = j;
        }
        अञ्ञथा सचे( depth == CV_32S || depth == CV_32U )
        {
            ध्रुव पूर्णाङ्क* ptr32s = (ध्रुव पूर्णाङ्क*)ptr8;
            अत्थाय( j = 0; j < cols; j++ )
                सचे( ptr32s[j] != 0 ) buf[k++] = j;
        }
        अञ्ञथा सचे( depth == CV_64S || depth == CV_64U )
        {
            ध्रुव int64* ptr64s = (ध्रुव int64*)ptr8;
            अत्थाय( j = 0; j < cols; j++ )
                सचे( ptr64s[j] != 0 ) buf[k++] = j;
        }
        अञ्ञथा सचे( depth == CV_32F )
        {
            ध्रुव पूर्णाङ्क* ptr32s = (ध्रुव पूर्णाङ्क*)ptr8;
            अत्थाय( j = 0; j < cols; j++ )
                सचे( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        अञ्ञथा सचे( depth == CV_16F || depth == CV_16BF )
        {
            ध्रुव ushort* ptr16 = (ध्रुव ushort*)ptr8;
            अत्थाय( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                सचे( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        अञ्ञथा सचे( depth == CV_8F_E4M3FN )
        {
            अत्थाय( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                सचे( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        अञ्ञथा
        {
            ध्रुव int64* ptr64s = (ध्रुव int64*)ptr8;
            अत्थाय( j = 0; j < cols; j++ )
                सचे( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        सचे( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            अत्थाय( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    सचे( idxvec.empty() || (_idx.kind() == निवेशः_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    सचे( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
