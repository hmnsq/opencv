// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "stat.hpp"

#समावेश "count_non_zero.simd.hpp"
#समावेश "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

अटल गिनती_Non_Zero_Func get_गिनती_Non_Zero_Tab(पूर्णांक depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(get_गिनती_Non_Zero_Tab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#अगर_घोषित HAVE_OPENCL
अटल bool ocl_गिनती_Non_Zero( निवेश_Array _src, पूर्णांक & res )
{
    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    अगर (depth == CV_64F && !doubleSupport)
        वापस false;

    पूर्णांक dbsize = ocl::Device::getDefault().maxComputeUnits();
    माप_प्रकार wgs = ocl::Device::getDefault().max_Work_Group_आकार();

    पूर्णांक wgs2_aligned = 1;
    जबतक (wgs2_aligned < (पूर्णांक)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (पूर्णांक)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    अगर (k.empty())
        वापस false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::Read_Only_No_आकार(src), src.cols, (पूर्णांक)src.योग(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    माप_प्रकार globalsize = dbsize * wgs;
    अगर (k.run(1, &globalsize, &wgs, true))
        वापस res = saturate_cast<पूर्णांक>(cv::योग_2(db.getMat(ACCESS_READ))[0]), true;
    वापस false;
}
#पूर्ण_अगर

#अगर defined HAVE_IPP
अटल bool ipp_गिनती_Non_Zero( Mat &src, पूर्णांक &res )
{
    CV_INSTRUMENT_REGION_IPP();

#अगर IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    अगर(cv::ipp::get_Ipp_शीर्ष_Features() == ippCPUID_SSE42)
        वापस false;
#पूर्ण_अगर

    Ipp32s  गिनती_2 = 0;
    पूर्णांक     depth = src.depth();

    अगर(src.dims <= 2)
    {
        IppStatus status;
        Ippi_आकार  size = {src.cols*src.channels(), src.rows};

        अगर(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippi_गिनती_In_Range_8u_C1_R, (स्थिर Ipp8u *)src.ptr(), (पूर्णांक)src.step, size, &गिनती_2, 0, 0);
        अन्यथा अगर(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippi_गिनती_In_Range_32f_C1_R, (स्थिर Ipp32f *)src.ptr(), (पूर्णांक)src.step, size, &गिनती_2, 0, 0);
        अन्यथा
            वापस false;

        अगर(status < 0)
            वापस false;

        res = size.width*size.height - गिनती_2;
    }
    अन्यथा
    {
        IppStatus       status;
        स्थिर Mat      *arrays[] = {&src, शून्य};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        Ippi_आकार        size  = {(पूर्णांक)it.size*src.channels(), 1};
        res = 0;
        क्रम (माप_प्रकार i = 0; i < it.nplanes; i++, ++it)
        {
            अगर(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippi_गिनती_In_Range_8u_C1_R, it.planes->ptr<Ipp8u>(), (पूर्णांक)it.planes->step, size, &गिनती_2, 0, 0);
            अन्यथा अगर(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippi_गिनती_In_Range_32f_C1_R, it.planes->ptr<Ipp32f>(), (पूर्णांक)it.planes->step, size, &गिनती_2, 0, 0);
            अन्यथा
                वापस false;

            अगर(status < 0 || (पूर्णांक)it.planes->योग()*src.channels() < गिनती_2)
                वापस false;

            res += (पूर्णांक)it.planes->योग()*src.channels() - गिनती_2;
        }
    }

    वापस true;
}
#पूर्ण_अगर

पूर्णांक गिनती_Non_Zero(निवेश_Array _src)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#अगर defined HAVE_OPENCL || defined HAVE_IPP
    पूर्णांक res = -1;
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_गिनती_Non_Zero(_src, res),
                res)
#पूर्ण_अगर

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_गिनती_Non_Zero(src, res), res);

    गिनती_Non_Zero_Func func = get_गिनती_Non_Zero_Tab(src.depth());
    CV_Assert( func != 0 );

    स्थिर Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    पूर्णांक योग = (पूर्णांक)it.size, nz = 0;

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], योग );

    वापस nz;
}

व्योम findNonZero(निवेश_Array _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    पूर्णांक depth = src.depth();
    std::vector<Point> idxvec;
    पूर्णांक rows = src.rows, cols = src.cols;
    Auto_बफर<पूर्णांक> buf_(cols + 1);
    पूर्णांक* buf = buf_.तथ्य();

    क्रम( पूर्णांक i = 0; i < rows; i++ )
    {
        पूर्णांक j, k = 0;
        स्थिर uchar* ptr8 = src.ptr(i);
        अगर( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#अगर (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            स्थिर पूर्णांक vlanes = VTraits<v_uint8>::vlanes();
            स्थिर v_uint8 vzero = vx_setzero_u8();
            क्रम( ; j <= cols - vlanes; j += vlanes )
            {
                अगर( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    जारी;
                क्रम( पूर्णांक t = j; t < j + vlanes; t++ )
                    अगर( ptr8[t] != 0 ) buf[k++] = t;
            }
#पूर्ण_अगर
            क्रम( ; j < cols; j++ )
                अगर( ptr8[j] != 0 ) buf[k++] = j;
        }
        अन्यथा अगर( depth == CV_16U || depth == CV_16S )
        {
            स्थिर ushort* ptr16 = (स्थिर ushort*)ptr8;
            क्रम( j = 0; j < cols; j++ )
                अगर( ptr16[j] != 0 ) buf[k++] = j;
        }
        अन्यथा अगर( depth == CV_32S || depth == CV_32U )
        {
            स्थिर पूर्णांक* ptr32s = (स्थिर पूर्णांक*)ptr8;
            क्रम( j = 0; j < cols; j++ )
                अगर( ptr32s[j] != 0 ) buf[k++] = j;
        }
        अन्यथा अगर( depth == CV_64S || depth == CV_64U )
        {
            स्थिर int64* ptr64s = (स्थिर int64*)ptr8;
            क्रम( j = 0; j < cols; j++ )
                अगर( ptr64s[j] != 0 ) buf[k++] = j;
        }
        अन्यथा अगर( depth == CV_32F )
        {
            स्थिर पूर्णांक* ptr32s = (स्थिर पूर्णांक*)ptr8;
            क्रम( j = 0; j < cols; j++ )
                अगर( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        अन्यथा अगर( depth == CV_16F || depth == CV_16BF )
        {
            स्थिर ushort* ptr16 = (स्थिर ushort*)ptr8;
            क्रम( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                अगर( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        अन्यथा अगर( depth == CV_8F_E4M3FN )
        {
            क्रम( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                अगर( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        अन्यथा
        {
            स्थिर int64* ptr64s = (स्थिर int64*)ptr8;
            क्रम( j = 0; j < cols; j++ )
                अगर( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        अगर( k > 0 )
        {
            माप_प्रकार sz = idxvec.size();
            idxvec.resize(sz + k);
            क्रम( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    अगर( idxvec.empty() || (_idx.kind() == निवेश_Array_2::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    अगर( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
