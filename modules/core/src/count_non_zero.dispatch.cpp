// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "stat.hpp"

#അന്തര്ഭാവ "count_non_zero.simd.hpp"
#അന്തര്ഭാവ "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv {

സ്ഥിതം എണ്ണം_Non_Zero_Func get_എണ്ണം_Non_Zero_Tab(പൂർണ്ണസംഖ്യ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_എണ്ണം_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
സ്ഥിതം തർക്കം ocl_എണ്ണം_Non_Zero( നിവേശം_Array _src, പൂർണ്ണസംഖ്യ & res )
{
    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    തർക്കം doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    എങ്കിൽ (depth == CV_64F && !doubleSupport)
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    പൂർണ്ണസംഖ്യ wgs2_aligned = 1;
    ആകുമ്പോൾ (wgs2_aligned < (പൂർണ്ണസംഖ്യ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (പൂർണ്ണസംഖ്യ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (പൂർണ്ണസംഖ്യ)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    എങ്കിൽ (k.run(1, &globalsize, &wgs, സത്യം))
        തിരികെ res = saturate_cast<പൂർണ്ണസംഖ്യ>(cv::sum(db.getMat(ACCESS_READ))[0]), സത്യം;
    തിരികെ അസത്യം;
}
#endif

#എങ്കിൽ defined HAVE_IPP
സ്ഥിതം തർക്കം ipp_എണ്ണം_Non_Zero( Mat &src, പൂർണ്ണസംഖ്യ &res )
{
    CV_INSTRUMENT_REGION_IPP();

#എങ്കിൽ IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    എങ്കിൽ(cv::ipp::get_Ipp_മുകൾ_Features() == ippCPUID_SSE42)
        തിരികെ അസത്യം;
#endif

    Ipp32s  എണ്ണം_2 = 0;
    പൂർണ്ണസംഖ്യ     depth = src.depth();

    എങ്കിൽ(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        എങ്കിൽ(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_എണ്ണം_In_Range_8u_C1_R, (സ്ഥിരം Ipp8u *)src.ptr(), (പൂർണ്ണസംഖ്യ)src.step, size, &എണ്ണം_2, 0, 0);
        അല്ലെങ്കിൽ എങ്കിൽ(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_എണ്ണം_In_Range_32f_C1_R, (സ്ഥിരം Ipp32f *)src.ptr(), (പൂർണ്ണസംഖ്യ)src.step, size, &എണ്ണം_2, 0, 0);
        അല്ലെങ്കിൽ
            തിരികെ അസത്യം;

        എങ്കിൽ(status < 0)
            തിരികെ അസത്യം;

        res = size.width*size.height - എണ്ണം_2;
    }
    അല്ലെങ്കിൽ
    {
        IppStatus       status;
        സ്ഥിരം Mat      *arrays[] = {&src, ശൂന്യ};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(പൂർണ്ണസംഖ്യ)it.size*src.channels(), 1};
        res = 0;
        വേണ്ടി (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            എങ്കിൽ(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_എണ്ണം_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (പൂർണ്ണസംഖ്യ)it.planes->step, size, &എണ്ണം_2, 0, 0);
            അല്ലെങ്കിൽ എങ്കിൽ(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_എണ്ണം_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (പൂർണ്ണസംഖ്യ)it.planes->step, size, &എണ്ണം_2, 0, 0);
            അല്ലെങ്കിൽ
                തിരികെ അസത്യം;

            എങ്കിൽ(status < 0 || (പൂർണ്ണസംഖ്യ)it.planes->total()*src.channels() < എണ്ണം_2)
                തിരികെ അസത്യം;

            res += (പൂർണ്ണസംഖ്യ)it.planes->total()*src.channels() - എണ്ണം_2;
        }
    }

    തിരികെ സത്യം;
}
#endif

പൂർണ്ണസംഖ്യ എണ്ണം_Non_Zero(നിവേശം_Array _src)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#എങ്കിൽ defined HAVE_OPENCL || defined HAVE_IPP
    പൂർണ്ണസംഖ്യ res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_എണ്ണം_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_എണ്ണം_Non_Zero(src, res), res);

    എണ്ണം_Non_Zero_Func func = get_എണ്ണം_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    സ്ഥിരം Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size, nz = 0;

    വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    തിരികെ nz;
}

ശൂന്യം findNonZero(നിവേശം_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    പൂർണ്ണസംഖ്യ depth = src.depth();
    std::vector<Point> idxvec;
    പൂർണ്ണസംഖ്യ rows = src.rows, cols = src.cols;
    AutoBuffer<പൂർണ്ണസംഖ്യ> buf_(cols + 1);
    പൂർണ്ണസംഖ്യ* buf = buf_.data();

    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < rows; i++ )
    {
        പൂർണ്ണസംഖ്യ j, k = 0;
        സ്ഥിരം uchar* ptr8 = src.ptr(i);
        എങ്കിൽ( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#എങ്കിൽ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            സ്ഥിരം പൂർണ്ണസംഖ്യ vlanes = VTraits<v_uint8>::vlanes();
            സ്ഥിരം v_uint8 vzero = vx_setzero_u8();
            വേണ്ടി( ; j <= cols - vlanes; j += vlanes )
            {
                എങ്കിൽ( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    തുടരുക;
                വേണ്ടി( പൂർണ്ണസംഖ്യ t = j; t < j + vlanes; t++ )
                    എങ്കിൽ( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            വേണ്ടി( ; j < cols; j++ )
                എങ്കിൽ( ptr8[j] != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_16U || depth == CV_16S )
        {
            സ്ഥിരം ushort* ptr16 = (സ്ഥിരം ushort*)ptr8;
            വേണ്ടി( j = 0; j < cols; j++ )
                എങ്കിൽ( ptr16[j] != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_32S || depth == CV_32U )
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ* ptr32s = (സ്ഥിരം പൂർണ്ണസംഖ്യ*)ptr8;
            വേണ്ടി( j = 0; j < cols; j++ )
                എങ്കിൽ( ptr32s[j] != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_64S || depth == CV_64U )
        {
            സ്ഥിരം int64* ptr64s = (സ്ഥിരം int64*)ptr8;
            വേണ്ടി( j = 0; j < cols; j++ )
                എങ്കിൽ( ptr64s[j] != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_32F )
        {
            സ്ഥിരം പൂർണ്ണസംഖ്യ* ptr32s = (സ്ഥിരം പൂർണ്ണസംഖ്യ*)ptr8;
            വേണ്ടി( j = 0; j < cols; j++ )
                എങ്കിൽ( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_16F || depth == CV_16BF )
        {
            സ്ഥിരം ushort* ptr16 = (സ്ഥിരം ushort*)ptr8;
            വേണ്ടി( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                എങ്കിൽ( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( depth == CV_8F_E4M3FN )
        {
            വേണ്ടി( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                എങ്കിൽ( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        അല്ലെങ്കിൽ
        {
            സ്ഥിരം int64* ptr64s = (സ്ഥിരം int64*)ptr8;
            വേണ്ടി( j = 0; j < cols; j++ )
                എങ്കിൽ( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        എങ്കിൽ( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            വേണ്ടി( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    എങ്കിൽ( idxvec.empty() || (_idx.kind() == നിവേശം_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    എങ്കിൽ( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
