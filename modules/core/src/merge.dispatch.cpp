// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "merge.simd.hpp"
#include "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

void merge8u(const uchar** src, uchar* dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void merge16u(const ushort** src, ushort* dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void merge32s(const ціле** src, ціле* dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void merge64s(const int64** src, int64* dst, ціле len, ціле cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


typedef void (*MergeFunc)(const void** src, void* dst, ціле len, ціле cn);

template<typename T, void (*fn)(const T**, T*, ціле, ціле)>
static void mergeWrap(const void** src, void* dst, ціле len, ціле cn)
{
    fn((const T**)src, (T*)dst, len, cn);
}

static MergeFunc getMergeFunc(ціле depth)
{
    static MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ціле, cv::hal::merge32s>, mergeWrap<ціле, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<ціле, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    повернути mergeTab[depth];
}

#ifdef HAVE_IPP

static bool ipp_merge(const Mat* mv, Mat& dst, ціле channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    якщо(channels != 3 && channels != 4)
        повернути false;

    якщо(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        const void *srcPtrs[4] = {NULL};
        size_t      srcStep    = mv[0].step;
        для(ціле i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            якщо(srcStep != mv[i].step)
                повернути false;
        }

        повернути CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (ціле)srcStep, dst.ptr(), (ціле)dst.step, size, (ціле)mv[0].elemSize1(), channels, 0) >= 0;
    }
    інакше
    {
        const Mat *arrays[5] = {NULL};
        uchar     *ptrs[5]   = {NULL};
        arrays[0] = &dst;

        для(ціле i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ціле)it.size, 1 };

        для( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            якщо(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (const void**)&ptrs[1], 0, ptrs[0], 0, size, (ціле)mv[0].elemSize1(), channels, 0) < 0)
                повернути false;
        }
        повернути true;
    }
#інакше
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    повернути false;
#endif
}
#endif

void merge(const Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    ціле depth = mv[0].depth();
    bool allch1 = true;
    ціле k, cn = 0;
    size_t i;

    для( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    якщо( n == 1 )
    {
        mv[0].copyTo(dst);
        повернути;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (ціле)n));

    якщо( !allch1 )
    {
        AutoBuffer<ціле> pairs(cn*2);
        ціле j, ni=0;

        для( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            для( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        повернути;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (ціле)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(sizeof(Mat*) + sizeof(uchar*)) + 16);
    const Mat** arrays = (const Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    для( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (ціле)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    для( i = 0; i < it.nplanes; i++, ++it )
    {
        для( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (const void**)&ptrs[1], ptrs[0], (ціле)bsz, cn );

            якщо( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                для( ціле t = 0; t < cn; t++ )
                    ptrs[t+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

static bool ocl_merge( InputArrayOfArrays _mv, OutputArray _dst )
{
    std::vector<UMat> src, ksrc;
    _mv.getUMatVector(src);
    CV_Assert(!src.empty());

    ціле type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    для (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        ціле itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        якщо (src[i].dims > 2)
            повернути false;

        CV_Assert(size == src[i].size() && depth == idepth);

        для (ціле cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    ціле dcn = (ціле)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    для (ціле i = 0; i < dcn; ++i)
    {
        srcargs += format("DECLARE_SRC_PARAM(%d)", i);
        processelem += format("PROCESS_ELEM(%d)", i);
        indexdecl += format("DECLARE_INDEX(%d)", i);
        cndecl += format(" -D scn%d=%d", i, ksrc[i].channels());
    }

    ocl::Kernel k("merge", ocl::core::split_merge_oclsrc,
                  format("-D OP_MERGE -D cn=%d -D T=%s -D DECLARE_SRC_PARAMS_N=%s"
                         " -D DECLARE_INDEX_N=%s -D PROCESS_ELEMS_N=%s%s",
                         dcn, ocl::memopTypeToStr(depth), srcargs.c_str(),
                         indexdecl.c_str(), processelem.c_str(), cndecl.c_str()));
    якщо (k.empty())
        повернути false;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    ціле argidx = 0;
    для (ціле i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    повернути k.run(2, globalsize, NULL, false);
}

#endif

void merge(InputArrayOfArrays _mv, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_mv.isUMatVector() && _dst.isUMat(),
               ocl_merge(_mv, _dst))

    std::vector<Mat> mv;
    _mv.getMatVector(mv);
    merge(!mv.empty() ? &mv[0] : 0, mv.size(), _dst);
}

} // namespace
