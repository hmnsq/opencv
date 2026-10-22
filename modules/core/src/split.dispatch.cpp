// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "split.simd.hpp"
#अन्तर्भाव "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv { नामाकाश hal {

शून्य split8u(ध्रुव uchar* src, uchar** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य split16u(ध्रुव ushort* src, ushort** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य split32s(ध्रुव पूर्णाङ्क* src, पूर्णाङ्क** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

शून्य split64s(ध्रुव int64* src, int64** dst, पूर्णाङ्क len, पूर्णाङ्क cn )
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

प्रकारनिर्देश शून्य (*SplitFunc)(ध्रुव शून्य* src, शून्य** dst, पूर्णाङ्क len, पूर्णाङ्क cn);

ढाँचा<typename T, शून्य (*fn)(ध्रुव T*, T**, पूर्णाङ्क, पूर्णाङ्क)>
स्थिर शून्य splitWrap(ध्रुव शून्य* src, शून्य** dst, पूर्णाङ्क len, पूर्णाङ्क cn)
{
    fn((ध्रुव T*)src, (T**)dst, len, cn);
}

स्थिर SplitFunc getSplitFunc(पूर्णाङ्क depth)
{
    स्थिर SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<पूर्णाङ्क, cv::hal::split32s>, splitWrap<पूर्णाङ्क, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<पूर्णाङ्क, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    प्रत्यागम splitTab[depth];
}

#ifdef HAVE_IPP

स्थिर तार्किक ipp_split(ध्रुव Mat& src, Mat* mv, पूर्णाङ्क channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    यदि(channels != 3 && channels != 4)
        प्रत्यागम असत्य;

    यदि(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        शून्य    *dstPtrs[4] = {रिक्त};
        माप_प्रकार   dstStep    = mv[0].step;
        प्रति(पूर्णाङ्क i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            यदि(dstStep != mv[i].step)
                प्रत्यागम असत्य;
        }

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (पूर्णाङ्क)src.step, dstPtrs, (पूर्णाङ्क)dstStep, size, (पूर्णाङ्क)src.elemSize1(), channels, 0) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat *arrays[5] = {रिक्त};
        uchar     *ptrs[5]   = {रिक्त};
        arrays[0] = &src;

        प्रति(पूर्णाङ्क i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (पूर्णाङ्क)it.size, 1 };

        प्रति( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (शून्य**)&ptrs[1], 0, size, (पूर्णाङ्क)src.elemSize1(), channels, 0) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    प्रत्यागम असत्य;
#endif
}
#endif

शून्य split(ध्रुव Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क k, depth = src.depth(), cn = src.channels();
    यदि( cn == 1 )
    {
        src.copyTo(mv[0]);
        प्रत्यागम;
    }

    प्रति( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    माप_प्रकार esz = src.elemSize(), esz1 = src.elemSize1();
    माप_प्रकार blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(परिमाण(Mat*) + परिमाण(uchar*)) + 16);
    ध्रुव Mat** arrays = (ध्रुव Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    प्रति( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    माप_प्रकार total = it.size;
    माप_प्रकार blocksize = std::min((माप_प्रकार)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    प्रति( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( माप_प्रकार j = 0; j < total; j += blocksize )
        {
            माप_प्रकार bsz = std::min(total - j, blocksize);
            func( ptrs[0], (शून्य**)&ptrs[1], (पूर्णाङ्क)bsz, cn );

            यदि( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                प्रति( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

स्थिर तार्किक ocl_split( निवेश_Array _m, OutputArrayOfArrays _mv )
{
    पूर्णाङ्क type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
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
    यदि (k.empty())
        प्रत्यागम असत्य;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    पूर्णाङ्क argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)size.width, ((माप_प्रकार)size.height + rowsPerWI - 1) / rowsPerWI };
    प्रत्यागम k.run(2, globalsize, रिक्त, असत्य);
}

#endif

शून्य split(निवेश_Array _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    यदि( m.empty() )
    {
        _mv.release();
        प्रत्यागम;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    पूर्णाङ्क depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    प्रति (पूर्णाङ्क i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
