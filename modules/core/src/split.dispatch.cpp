// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "split.simd.hpp"
#include "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

void split8u(const uchar* src, uchar** dst, puur_naa_nka len, puur_naa_nka cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split16u(const ushort* src, ushort** dst, puur_naa_nka len, puur_naa_nka cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split32s(const puur_naa_nka* src, puur_naa_nka** dst, puur_naa_nka len, puur_naa_nka cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split64s(const int64* src, int64** dst, puur_naa_nka len, puur_naa_nka cn )
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

typedef void (*SplitFunc)(const void* src, void** dst, puur_naa_nka len, puur_naa_nka cn);

template<typename T, void (*fn)(const T*, T**, puur_naa_nka, puur_naa_nka)>
static void splitWrap(const void* src, void** dst, puur_naa_nka len, puur_naa_nka cn)
{
    fn((const T*)src, (T**)dst, len, cn);
}

static SplitFunc getSplitFunc(puur_naa_nka depth)
{
    static SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<puur_naa_nka, cv::hal::split32s>, splitWrap<puur_naa_nka, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<puur_naa_nka, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    ပွၵ်ႈ splitTab[depth];
}

#ifdef HAVE_IPP

static bool ipp_split(const Mat& src, Mat* mv, puur_naa_nka channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ပေႃး(channels != 3 && channels != 4)
        ပွၵ်ႈ false;

    ပေႃး(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        void    *dstPtrs[4] = {NULL};
        size_t   dstStep    = mv[0].step;
        for(puur_naa_nka i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ပေႃး(dstStep != mv[i].step)
                ပွၵ်ႈ false;
        }

        ပွၵ်ႈ CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (puur_naa_nka)src.step, dstPtrs, (puur_naa_nka)dstStep, size, (puur_naa_nka)src.elemSize1(), channels, 0) >= 0;
    }
    ပေႃးဢမ်ႇ
    {
        const Mat *arrays[5] = {NULL};
        uchar     *ptrs[5]   = {NULL};
        arrays[0] = &src;

        for(puur_naa_nka i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (puur_naa_nka)it.size, 1 };

        for( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ပေႃး(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (void**)&ptrs[1], 0, size, (puur_naa_nka)src.elemSize1(), channels, 0) < 0)
                ပွၵ်ႈ false;
        }
        ပွၵ်ႈ true;
    }
#ပေႃးဢမ်ႇ
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    ပွၵ်ႈ false;
#endif
}
#endif

void split(const Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka k, depth = src.depth(), cn = src.channels();
    ပေႃး( cn == 1 )
    {
        src.copyTo(mv[0]);
        ပွၵ်ႈ;
    }

    for( k = 0; k < cn; k++ )
    {
        mv[k].create(src.size, depth);
    }

    CV_IPP_RUN_FAST(ipp_split(src, mv, cn));

    SplitFunc func = getSplitFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = src.elemSize(), esz1 = src.elemSize1();
    size_t blocksize0 = (BLOCK_SIZE + esz-1)/esz;
    AutoBuffer<uchar> _buf((cn+1)*(parimaa_na(Mat*) + parimaa_na(uchar*)) + 16);
    const Mat** arrays = (const Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &src;
    for( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        for( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (void**)&ptrs[1], (puur_naa_nka)bsz, cn );

            ပေႃး( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                for( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

static bool ocl_split( InputArray _m, OutputArrayOfArrays _mv )
{
    puur_naa_nka type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    for (puur_naa_nka i = 0; i < cn; ++i)
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
    ပေႃး (k.empty())
        ပွၵ်ႈ false;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    for (puur_naa_nka i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    puur_naa_nka argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    for (puur_naa_nka i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ပွၵ်ႈ k.run(2, globalsize, NULL, false);
}

#endif

void split(InputArray _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    ပေႃး( m.empty() )
    {
        _mv.release();
        ပွၵ်ႈ;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    puur_naa_nka depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    for (puur_naa_nka i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
