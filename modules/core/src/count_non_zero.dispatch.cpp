// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "stat.hpp"

#अन्तर्भाव "count_non_zero.simd.hpp"
#अन्तर्भाव "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

स्थिर मोजणी_Non_Zero_Func get_मोजणी_Non_Zero_Tab(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_मोजणी_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
स्थिर तार्किक ocl_मोजणी_Non_Zero( आदान_Array _src, पूर्णांक & res )
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    तार्किक doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    जर (depth == CV_64F && !doubleSupport)
        परत असत्य;

    पूर्णांक dbsize = ocl::Device::getDefault().maxComputeUnits();
    माप_प्रकार wgs = ocl::Device::getDefault().maxWorkGroupSize();

    पूर्णांक wgs2_aligned = 1;
    पर्यंत (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (पूर्णांक)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    जर (k.empty())
        परत असत्य;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (पूर्णांक)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    माप_प्रकार globalsize = dbsize * wgs;
    जर (k.run(1, &globalsize, &wgs, सत्य))
        परत res = saturate_cast<पूर्णांक>(cv::sum(db.getMat(ACCESS_READ))[0]), सत्य;
    परत असत्य;
}
#endif

#जर defined HAVE_IPP
स्थिर तार्किक ipp_मोजणी_Non_Zero( Mat &src, पूर्णांक &res )
{
    CV_INSTRUMENT_REGION_IPP();

#जर IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    जर(cv::ipp::get_Ipp_शीर्ष_Features() == ippCPUID_SSE42)
        परत असत्य;
#endif

    Ipp32s  मोजणी_2 = 0;
    पूर्णांक     depth = src.depth();

    जर(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        जर(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_मोजणी_In_Range_8u_C1_R, (स्थिरांक Ipp8u *)src.ptr(), (पूर्णांक)src.step, size, &मोजणी_2, 0, 0);
        अन्यथा जर(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_मोजणी_In_Range_32f_C1_R, (स्थिरांक Ipp32f *)src.ptr(), (पूर्णांक)src.step, size, &मोजणी_2, 0, 0);
        अन्यथा
            परत असत्य;

        जर(status < 0)
            परत असत्य;

        res = size.width*size.height - मोजणी_2;
    }
    अन्यथा
    {
        IppStatus       status;
        स्थिरांक Mat      *arrays[] = {&src, रिक्त};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(पूर्णांक)it.size*src.channels(), 1};
        res = 0;
        साठी (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            जर(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_मोजणी_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (पूर्णांक)it.planes->step, size, &मोजणी_2, 0, 0);
            अन्यथा जर(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_मोजणी_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (पूर्णांक)it.planes->step, size, &मोजणी_2, 0, 0);
            अन्यथा
                परत असत्य;

            जर(status < 0 || (पूर्णांक)it.planes->total()*src.channels() < मोजणी_2)
                परत असत्य;

            res += (पूर्णांक)it.planes->total()*src.channels() - मोजणी_2;
        }
    }

    परत सत्य;
}
#endif

पूर्णांक मोजणी_Non_Zero(आदान_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#जर defined HAVE_OPENCL || defined HAVE_IPP
    पूर्णांक res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_मोजणी_Non_Zero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_मोजणी_Non_Zero(src, res), res);

    मोजणी_Non_Zero_Func func = get_मोजणी_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    स्थिरांक Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक total = (पूर्णांक)it.size, nz = 0;

    साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    परत nz;
}

शून्य findNonZero(आदान_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    पूर्णांक depth = src.depth();
    std::vector<Point> idxvec;
    पूर्णांक rows = src.rows, cols = src.cols;
    AutoBuffer<पूर्णांक> buf_(cols + 1);
    पूर्णांक* buf = buf_.data();

    साठी( पूर्णांक i = 0; i < rows; i++ )
    {
        पूर्णांक j, k = 0;
        स्थिरांक uchar* ptr8 = src.ptr(i);
        जर( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#जर (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            स्थिरांक पूर्णांक vlanes = VTraits<v_uint8>::vlanes();
            स्थिरांक v_uint8 vzero = vx_setzero_u8();
            साठी( ; j <= cols - vlanes; j += vlanes )
            {
                जर( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    सुरूठेवा;
                साठी( पूर्णांक t = j; t < j + vlanes; t++ )
                    जर( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            साठी( ; j < cols; j++ )
                जर( ptr8[j] != 0 ) buf[k++] = j;
        }
        अन्यथा जर( depth == CV_16U || depth == CV_16S )
        {
            स्थिरांक ushort* ptr16 = (स्थिरांक ushort*)ptr8;
            साठी( j = 0; j < cols; j++ )
                जर( ptr16[j] != 0 ) buf[k++] = j;
        }
        अन्यथा जर( depth == CV_32S || depth == CV_32U )
        {
            स्थिरांक पूर्णांक* ptr32s = (स्थिरांक पूर्णांक*)ptr8;
            साठी( j = 0; j < cols; j++ )
                जर( ptr32s[j] != 0 ) buf[k++] = j;
        }
        अन्यथा जर( depth == CV_64S || depth == CV_64U )
        {
            स्थिरांक int64* ptr64s = (स्थिरांक int64*)ptr8;
            साठी( j = 0; j < cols; j++ )
                जर( ptr64s[j] != 0 ) buf[k++] = j;
        }
        अन्यथा जर( depth == CV_32F )
        {
            स्थिरांक पूर्णांक* ptr32s = (स्थिरांक पूर्णांक*)ptr8;
            साठी( j = 0; j < cols; j++ )
                जर( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        अन्यथा जर( depth == CV_16F || depth == CV_16BF )
        {
            स्थिरांक ushort* ptr16 = (स्थिरांक ushort*)ptr8;
            साठी( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                जर( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        अन्यथा जर( depth == CV_8F_E4M3FN )
        {
            साठी( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                जर( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        अन्यथा
        {
            स्थिरांक int64* ptr64s = (स्थिरांक int64*)ptr8;
            साठी( j = 0; j < cols; j++ )
                जर( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        जर( k > 0 )
        {
            माप_प्रकार sz = idxvec.size();
            idxvec.resize(sz + k);
            साठी( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    जर( idxvec.empty() || (_idx.kind() == आदान_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    जर( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
