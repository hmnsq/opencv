// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "stat.hpp"

#ಅನ್ತರ್ಭಾವ "count_non_zero.simd.hpp"
#ಅನ್ತರ್ಭಾವ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಎಣಿಕೆ_Non_Zero_Func get_ಎಣಿಕೆ_Non_Zero_Tab(ಪೂರ್ಣಾಙ್ಕ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ಎಣಿಕೆ_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_ಎಣಿಕೆ_Non_Zero( ಒಳಹರಿವು_Array _src, ಪೂರ್ಣಾಙ್ಕ & res )
{
    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    ತಾರ್ಕಿಕ doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ಯದಿ (depth == CV_64F && !doubleSupport)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ಪೂರ್ಣಾಙ್ಕ wgs2_aligned = 1;
    ಯಾವತ್ (wgs2_aligned < (ಪೂರ್ಣಾಙ್ಕ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ಪೂರ್ಣಾಙ್ಕ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ಪೂರ್ಣಾಙ್ಕ)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ಯದಿ (k.run(1, &globalsize, &wgs, ಸತ್ಯ))
        ಪ್ರತ್ಯಾಗಮ res = saturate_cast<ಪೂರ್ಣಾಙ್ಕ>(cv::sum(db.getMat(ACCESS_READ))[0]), ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
#endif

#ಯದಿ defined HAVE_IPP
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ಎಣಿಕೆ_Non_Zero( Mat &src, ಪೂರ್ಣಾಙ್ಕ &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ಯದಿ IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    ಯದಿ(cv::ipp::get_Ipp_ತುದಿ_Features() == ippCPUID_SSE42)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif

    Ipp32s  ಎಣಿಕೆ_2 = 0;
    ಪೂರ್ಣಾಙ್ಕ     depth = src.depth();

    ಯದಿ(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        ಯದಿ(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_In_Range_8u_C1_R, (ಧ್ರುವ Ipp8u *)src.ptr(), (ಪೂರ್ಣಾಙ್ಕ)src.step, size, &ಎಣಿಕೆ_2, 0, 0);
        ಅನ್ಯಥಾ ಯದಿ(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_In_Range_32f_C1_R, (ಧ್ರುವ Ipp32f *)src.ptr(), (ಪೂರ್ಣಾಙ್ಕ)src.step, size, &ಎಣಿಕೆ_2, 0, 0);
        ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        ಯದಿ(status < 0)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        res = size.width*size.height - ಎಣಿಕೆ_2;
    }
    ಅನ್ಯಥಾ
    {
        IppStatus       status;
        ಧ್ರುವ Mat      *arrays[] = {&src, ರಿಕ್ತ};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(ಪೂರ್ಣಾಙ್ಕ)it.size*src.channels(), 1};
        res = 0;
        ಪ್ರತಿ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ಯದಿ(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (ಪೂರ್ಣಾಙ್ಕ)it.planes->step, size, &ಎಣಿಕೆ_2, 0, 0);
            ಅನ್ಯಥಾ ಯದಿ(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_ಎಣಿಕೆ_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (ಪೂರ್ಣಾಙ್ಕ)it.planes->step, size, &ಎಣಿಕೆ_2, 0, 0);
            ಅನ್ಯಥಾ
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

            ಯದಿ(status < 0 || (ಪೂರ್ಣಾಙ್ಕ)it.planes->total()*src.channels() < ಎಣಿಕೆ_2)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

            res += (ಪೂರ್ಣಾಙ್ಕ)it.planes->total()*src.channels() - ಎಣಿಕೆ_2;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}
#endif

ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_Non_Zero(ಒಳಹರಿವು_Array _src)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ಯದಿ defined HAVE_OPENCL || defined HAVE_IPP
    ಪೂರ್ಣಾಙ್ಕ res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ಎಣಿಕೆ_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_ಎಣಿಕೆ_Non_Zero(src, res), res);

    ಎಣಿಕೆ_Non_Zero_Func func = get_ಎಣಿಕೆ_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    ಧ್ರುವ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)it.size, nz = 0;

    ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    ಪ್ರತ್ಯಾಗಮ nz;
}

ಶೂನ್ಯ findNonZero(ಒಳಹರಿವು_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    ಪೂರ್ಣಾಙ್ಕ depth = src.depth();
    std::vector<Point> idxvec;
    ಪೂರ್ಣಾಙ್ಕ rows = src.rows, cols = src.cols;
    AutoBuffer<ಪೂರ್ಣಾಙ್ಕ> buf_(cols + 1);
    ಪೂರ್ಣಾಙ್ಕ* buf = buf_.data();

    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < rows; i++ )
    {
        ಪೂರ್ಣಾಙ್ಕ j, k = 0;
        ಧ್ರುವ uchar* ptr8 = src.ptr(i);
        ಯದಿ( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ಯದಿ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ vlanes = VTraits<v_uint8>::vlanes();
            ಧ್ರುವ v_uint8 vzero = vx_setzero_u8();
            ಪ್ರತಿ( ; j <= cols - vlanes; j += vlanes )
            {
                ಯದಿ( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    ಅನುವರ್ತ;
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ t = j; t < j + vlanes; t++ )
                    ಯದಿ( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            ಪ್ರತಿ( ; j < cols; j++ )
                ಯದಿ( ptr8[j] != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_16U || depth == CV_16S )
        {
            ಧ್ರುವ ushort* ptr16 = (ಧ್ರುವ ushort*)ptr8;
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                ಯದಿ( ptr16[j] != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_32S || depth == CV_32U )
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ptr32s = (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)ptr8;
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                ಯದಿ( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_64S || depth == CV_64U )
        {
            ಧ್ರುವ int64* ptr64s = (ಧ್ರುವ int64*)ptr8;
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                ಯದಿ( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_32F )
        {
            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ptr32s = (ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)ptr8;
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                ಯದಿ( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_16F || depth == CV_16BF )
        {
            ಧ್ರುವ ushort* ptr16 = (ಧ್ರುವ ushort*)ptr8;
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ಯದಿ( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ ಯದಿ( depth == CV_8F_E4M3FN )
        {
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ಯದಿ( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ಅನ್ಯಥಾ
        {
            ಧ್ರುವ int64* ptr64s = (ಧ್ರುವ int64*)ptr8;
            ಪ್ರತಿ( j = 0; j < cols; j++ )
                ಯದಿ( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ಯದಿ( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            ಪ್ರತಿ( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    ಯದಿ( idxvec.empty() || (_idx.kind() == ಒಳಹರಿವು_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ಯದಿ( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
