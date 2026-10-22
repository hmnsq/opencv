// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "stat.hpp"

#ضمن "count_non_zero.simd.hpp"
#ضمن "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv {

ساكن تعداد_Non_Zero_Func get_تعداد_Non_Zero_Tab(صحيح depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_تعداد_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
ساكن منطقي ocl_تعداد_Non_Zero( إدخال_Array _src, صحيح & res )
{
    صحيح type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    منطقي doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    إذا (depth == CV_64F && !doubleSupport)
        إرجاع خطأ;

    صحيح dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    صحيح wgs2_aligned = 1;
    بينما (wgs2_aligned < (صحيح)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (صحيح)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    إذا (k.empty())
        إرجاع خطأ;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (صحيح)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    إذا (k.run(1, &globalsize, &wgs, true))
        إرجاع res = saturate_cast<صحيح>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    إرجاع خطأ;
}
#endif

#إذا defined HAVE_IPP
ساكن منطقي ipp_تعداد_Non_Zero( Mat &src, صحيح &res )
{
    CV_INSTRUMENT_REGION_IPP();

#إذا IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    إذا(cv::ipp::get_Ipp_قمة_Features() == ippCPUID_SSE42)
        إرجاع خطأ;
#endif

    Ipp32s  تعداد_3 = 0;
    صحيح     depth = src.depth();

    إذا(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        إذا(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_تعداد_In_Range_8u_C1_R, (ثابت Ipp8u *)src.ptr(), (صحيح)src.step, size, &تعداد_3, 0, 0);
        وإلا إذا(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_تعداد_In_Range_32f_C1_R, (ثابت Ipp32f *)src.ptr(), (صحيح)src.step, size, &تعداد_3, 0, 0);
        وإلا
            إرجاع خطأ;

        إذا(status < 0)
            إرجاع خطأ;

        res = size.width*size.height - تعداد_3;
    }
    وإلا
    {
        IppStatus       status;
        ثابت Mat      *arrays[] = {&src, عدم};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(صحيح)it.size*src.channels(), 1};
        res = 0;
        لكل (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            إذا(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_تعداد_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (صحيح)it.planes->step, size, &تعداد_3, 0, 0);
            وإلا إذا(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_تعداد_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (صحيح)it.planes->step, size, &تعداد_3, 0, 0);
            وإلا
                إرجاع خطأ;

            إذا(status < 0 || (صحيح)it.planes->total()*src.channels() < تعداد_3)
                إرجاع خطأ;

            res += (صحيح)it.planes->total()*src.channels() - تعداد_3;
        }
    }

    إرجاع true;
}
#endif

صحيح تعداد_Non_Zero(إدخال_Array _src)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#إذا defined HAVE_OPENCL || defined HAVE_IPP
    صحيح res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_تعداد_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_تعداد_Non_Zero(src, res), res);

    تعداد_Non_Zero_Func func = get_تعداد_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    ثابت Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    صحيح total = (صحيح)it.size, nz = 0;

    لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    إرجاع nz;
}

فراغ findNonZero(إدخال_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    صحيح depth = src.depth();
    std::vector<Point> idxvec;
    صحيح rows = src.rows, cols = src.cols;
    AutoBuffer<صحيح> buf_(cols + 1);
    صحيح* buf = buf_.data();

    لكل( صحيح i = 0; i < rows; i++ )
    {
        صحيح j, k = 0;
        ثابت uchar* ptr8 = src.ptr(i);
        إذا( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#إذا (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            ثابت صحيح vlanes = VTraits<v_uint8>::vlanes();
            ثابت v_uint8 vzero = vx_setzero_u8();
            لكل( ; j <= cols - vlanes; j += vlanes )
            {
                إذا( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    استمر;
                لكل( صحيح t = j; t < j + vlanes; t++ )
                    إذا( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            لكل( ; j < cols; j++ )
                إذا( ptr8[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( depth == CV_16U || depth == CV_16S )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < cols; j++ )
                إذا( ptr16[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( depth == CV_32S || depth == CV_32U )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < cols; j++ )
                إذا( ptr32s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( depth == CV_64S || depth == CV_64U )
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < cols; j++ )
                إذا( ptr64s[j] != 0 ) buf[k++] = j;
        }
        وإلا إذا( depth == CV_32F )
        {
            ثابت صحيح* ptr32s = (ثابت صحيح*)ptr8;
            لكل( j = 0; j < cols; j++ )
                إذا( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        وإلا إذا( depth == CV_16F || depth == CV_16BF )
        {
            ثابت ushort* ptr16 = (ثابت ushort*)ptr8;
            لكل( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                إذا( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        وإلا إذا( depth == CV_8F_E4M3FN )
        {
            لكل( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                إذا( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        وإلا
        {
            ثابت int64* ptr64s = (ثابت int64*)ptr8;
            لكل( j = 0; j < cols; j++ )
                إذا( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        إذا( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            لكل( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    إذا( idxvec.empty() || (_idx.kind() == إدخال_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    إذا( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
