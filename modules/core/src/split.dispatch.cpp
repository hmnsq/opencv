// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"

#समावेश "split.simd.hpp"
#समावेश "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

व्योम split8u(स्थिर uchar* src, uchar** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम split16u(स्थिर ushort* src, ushort** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम split32s(स्थिर पूर्णांक* src, पूर्णांक** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

व्योम split64s(स्थिर int64* src, int64** dst, पूर्णांक len, पूर्णांक cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split64s, cv_hal_split64s, src,dst, len, cn)
    CV_CPU_DISPATCH(split64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::

/****************************************************************************************\
*                                       split & merge                                    *
\****************************************************************************************/

प्रकार व्योम (*SplitFunc)(स्थिर व्योम* src, व्योम** dst, पूर्णांक len, पूर्णांक cn);

ढाँचा<typename T, व्योम (*fn)(स्थिर T*, T**, पूर्णांक, पूर्णांक)>
अटल व्योम splitWrap(स्थिर व्योम* src, व्योम** dst, पूर्णांक len, पूर्णांक cn)
{
    fn((स्थिर T*)src, (T**)dst, len, cn);
}

अटल SplitFunc getSplitFunc(पूर्णांक depth)
{
    अटल SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<पूर्णांक, cv::hal::split32s>, splitWrap<पूर्णांक, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<पूर्णांक, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    वापस splitTab[depth];
}

#अगर_घोषित HAVE_IPP

अटल bool ipp_split(स्थिर Mat& src, Mat* mv, पूर्णांक channels)
{
#अगर_घोषित HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    अगर(channels != 3 && channels != 4)
        वापस false;

    अगर(src.dims <= 2)
    {
        Ippi_आकार size       = ippi_आकार(src.size());
        व्योम    *dstPtrs[4] = {शून्य};
        माप_प्रकार   dstStep    = mv[0].step;
        क्रम(पूर्णांक i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            अगर(dstStep != mv[i].step)
                वापस false;
        }

        वापस CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (पूर्णांक)src.step, dstPtrs, (पूर्णांक)dstStep, size, (पूर्णांक)src.elemSize1(), channels, 0) >= 0;
    }
    अन्यथा
    {
        स्थिर Mat *arrays[5] = {शून्य};
        uchar     *ptrs[5]   = {शून्य};
        arrays[0] = &src;

        क्रम(पूर्णांक i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        Ippi_आकार size = { (पूर्णांक)it.size, 1 };

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            अगर(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (व्योम**)&ptrs[1], 0, size, (पूर्णांक)src.elemSize1(), channels, 0) < 0)
                वापस false;
        }
        वापस true;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    वापस false;
#पूर्ण_अगर
}
#पूर्ण_अगर

व्योम split(स्थिर Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक k, depth = src.depth(), cn = src.channels();
    अगर( cn == 1 )
    {
        src.copyTo(mv[0]);
        वापस;
    }

    क्रम( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    माप_प्रकार esz = src.elem_आकार(), esz1 = src.elemSize1();
    माप_प्रकार blocksize0 = (BLOCK_आकार + esz-1)/esz;
    Auto_बफर<uchar> _buf((cn+1)*(माप(Mat*) + माप(uchar*)) + 16);
    स्थिर Mat** arrays = (स्थिर Mat**)_buf.तथ्य();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    क्रम( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    माप_प्रकार योग = it.size;
    माप_प्रकार blocksize = std::min((माप_प्रकार)CV_SPLIT_MERGE_MAX_BLOCK_आकार(cn), cn <= 4 ? योग : std::min(योग, blocksize0));

    क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( माप_प्रकार j = 0; j < योग; j += blocksize )
        {
            माप_प्रकार bsz = std::min(योग - j, blocksize);
            func( ptrs[0], (व्योम**)&ptrs[1], (पूर्णांक)bsz, cn );

            अगर( j + blocksize < योग )
            {
                ptrs[0] += bsz*esz;
                क्रम( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#अगर_घोषित HAVE_OPENCL

अटल bool ocl_split( निवेश_Array _m, OutputArrayOfArrays _mv )
{
    पूर्णांक type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    क्रम (पूर्णांक i = 0; i < cn; ++i)
    {
        dstargs += format("DECLARE_DST_PARAM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
    }

    ocl::Kernel k("split", ocl::core::split_merge_oclsrc,
                  format("-D T=%s -D OP_SPLIT -D cn=%d -D DECLARE_DST_PARAMS=%s"
                         " -D PROCESS_ELEMS_N=%s -D DECLARE_INDEX_N=%s",
                         ocl::memopTypeToStr(depth), cn, dstargs.c_str(),
                         processelem.c_str(), indexdecl.c_str()));
    अगर (k.empty())
        वापस false;

    आकार size = _m.size();
    _mv.create(cn, 1, depth);
    क्रम (पूर्णांक i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    पूर्णांक argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    क्रम (पूर्णांक i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::Write_Only_No_आकार(dst[i]));
    k.set(argidx, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)size.width, ((माप_प्रकार)size.height + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

#पूर्ण_अगर

व्योम split(निवेश_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    अगर( m.empty() )
    {
        _mv.release();
        वापस;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    पूर्णांक depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    क्रम (पूर्णांक i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
