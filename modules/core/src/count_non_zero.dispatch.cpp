// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "stat.hpp"

#અન્તર્ભાવ "count_non_zero.simd.hpp"
#અન્તર્ભાવ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv {

સ્થિર ગણતરી_Non_Zero_Func get_ગણતરી_Non_Zero_Tab(પૂર્ણાંક depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_ગણતરી_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
સ્થિર તાર્કિક ocl_ગણતરી_Non_Zero( આગત_Array _src, પૂર્ણાંક & res )
{
    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    તાર્કિક doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    જો (depth == CV_64F && !doubleSupport)
        પાછા ખોટું;

    પૂર્ણાંક dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    પૂર્ણાંક wgs2_aligned = 1;
    જ્યારે (wgs2_aligned < (પૂર્ણાંક)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (પૂર્ણાંક)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    જો (k.empty())
        પાછા ખોટું;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (પૂર્ણાંક)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    જો (k.run(1, &globalsize, &wgs, સાચું))
        પાછા res = saturate_cast<પૂર્ણાંક>(cv::sum(db.getMat(ACCESS_READ))[0]), સાચું;
    પાછા ખોટું;
}
#endif

#જો defined HAVE_IPP
સ્થિર તાર્કિક ipp_ગણતરી_Non_Zero( Mat &src, પૂર્ણાંક &res )
{
    CV_INSTRUMENT_REGION_IPP();

#જો IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    જો(cv::ipp::get_Ipp_ટોચ_Features() == ippCPUID_SSE42)
        પાછા ખોટું;
#endif

    Ipp32s  ગણતરી_2 = 0;
    પૂર્ણાંક     depth = src.depth();

    જો(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        જો(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_ગણતરી_In_Range_8u_C1_R, (અચલ Ipp8u *)src.ptr(), (પૂર્ણાંક)src.step, size, &ગણતરી_2, 0, 0);
        નહીંતર જો(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_ગણતરી_In_Range_32f_C1_R, (અચલ Ipp32f *)src.ptr(), (પૂર્ણાંક)src.step, size, &ગણતરી_2, 0, 0);
        નહીંતર
            પાછા ખોટું;

        જો(status < 0)
            પાછા ખોટું;

        res = size.width*size.height - ગણતરી_2;
    }
    નહીંતર
    {
        IppStatus       status;
        અચલ Mat      *arrays[] = {&src, ખાલી};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(પૂર્ણાંક)it.size*src.channels(), 1};
        res = 0;
        માટે (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            જો(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_ગણતરી_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (પૂર્ણાંક)it.planes->step, size, &ગણતરી_2, 0, 0);
            નહીંતર જો(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_ગણતરી_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (પૂર્ણાંક)it.planes->step, size, &ગણતરી_2, 0, 0);
            નહીંતર
                પાછા ખોટું;

            જો(status < 0 || (પૂર્ણાંક)it.planes->total()*src.channels() < ગણતરી_2)
                પાછા ખોટું;

            res += (પૂર્ણાંક)it.planes->total()*src.channels() - ગણતરી_2;
        }
    }

    પાછા સાચું;
}
#endif

પૂર્ણાંક ગણતરી_Non_Zero(આગત_Array _src)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#જો defined HAVE_OPENCL || defined HAVE_IPP
    પૂર્ણાંક res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_ગણતરી_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_ગણતરી_Non_Zero(src, res), res);

    ગણતરી_Non_Zero_Func func = get_ગણતરી_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    અચલ Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    પૂર્ણાંક total = (પૂર્ણાંક)it.size, nz = 0;

    માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    પાછા nz;
}

શૂન્ય findNonZero(આગત_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    પૂર્ણાંક depth = src.depth();
    std::vector<Point> idxvec;
    પૂર્ણાંક rows = src.rows, cols = src.cols;
    AutoBuffer<પૂર્ણાંક> buf_(cols + 1);
    પૂર્ણાંક* buf = buf_.data();

    માટે( પૂર્ણાંક i = 0; i < rows; i++ )
    {
        પૂર્ણાંક j, k = 0;
        અચલ uchar* ptr8 = src.ptr(i);
        જો( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#જો (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            અચલ પૂર્ણાંક vlanes = VTraits<v_uint8>::vlanes();
            અચલ v_uint8 vzero = vx_setzero_u8();
            માટે( ; j <= cols - vlanes; j += vlanes )
            {
                જો( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    ચાલુ;
                માટે( પૂર્ણાંક t = j; t < j + vlanes; t++ )
                    જો( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            માટે( ; j < cols; j++ )
                જો( ptr8[j] != 0 ) buf[k++] = j;
        }
        નહીંતર જો( depth == CV_16U || depth == CV_16S )
        {
            અચલ ushort* ptr16 = (અચલ ushort*)ptr8;
            માટે( j = 0; j < cols; j++ )
                જો( ptr16[j] != 0 ) buf[k++] = j;
        }
        નહીંતર જો( depth == CV_32S || depth == CV_32U )
        {
            અચલ પૂર્ણાંક* ptr32s = (અચલ પૂર્ણાંક*)ptr8;
            માટે( j = 0; j < cols; j++ )
                જો( ptr32s[j] != 0 ) buf[k++] = j;
        }
        નહીંતર જો( depth == CV_64S || depth == CV_64U )
        {
            અચલ int64* ptr64s = (અચલ int64*)ptr8;
            માટે( j = 0; j < cols; j++ )
                જો( ptr64s[j] != 0 ) buf[k++] = j;
        }
        નહીંતર જો( depth == CV_32F )
        {
            અચલ પૂર્ણાંક* ptr32s = (અચલ પૂર્ણાંક*)ptr8;
            માટે( j = 0; j < cols; j++ )
                જો( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        નહીંતર જો( depth == CV_16F || depth == CV_16BF )
        {
            અચલ ushort* ptr16 = (અચલ ushort*)ptr8;
            માટે( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                જો( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        નહીંતર જો( depth == CV_8F_E4M3FN )
        {
            માટે( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                જો( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        નહીંતર
        {
            અચલ int64* ptr64s = (અચલ int64*)ptr8;
            માટે( j = 0; j < cols; j++ )
                જો( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        જો( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            માટે( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    જો( idxvec.empty() || (_idx.kind() == આગત_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    જો( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
