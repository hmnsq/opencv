// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "merge.simd.hpp"
#include "merge.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

void merge8u(const uchar** src, uchar* dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge8u, cv_hal_merge8u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void merge16u(const ushort** src, ushort* dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge16u, cv_hal_merge16u, src, dst, len, cn)
    CV_CPU_DISPATCH(merge16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void merge32s(const පූර්ණ_සංඛ්‍යා** src, පූර්ණ_සංඛ්‍යා* dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge32s, cv_hal_merge32s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void merge64s(const int64** src, int64* dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(merge64s, cv_hal_merge64s, src, dst, len, cn)
    CV_CPU_DISPATCH(merge64s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv::hal::


typedef void (*MergeFunc)(const void** src, void* dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn);

template<typename T, void (*fn)(const T**, T*, පූර්ණ_සංඛ්‍යා, පූර්ණ_සංඛ්‍යා)>
static void mergeWrap(const void** src, void* dst, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා cn)
{
    fn((const T**)src, (T*)dst, len, cn);
}

static MergeFunc getMergeFunc(පූර්ණ_සංඛ්‍යා depth)
{
    static MergeFunc mergeTab[CV_DEPTH_MAX] =
    {
        mergeWrap<uchar, cv::hal::merge8u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<පූර්ණ_සංඛ්‍යා, cv::hal::merge32s>, mergeWrap<පූර්ණ_සංඛ්‍යා, cv::hal::merge32s>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<ushort, cv::hal::merge16u>,
        mergeWrap<ushort, cv::hal::merge16u>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<int64_t, cv::hal::merge64s>, mergeWrap<int64_t, cv::hal::merge64s>,
        mergeWrap<පූර්ණ_සංඛ්‍යා, cv::hal::merge32s>, mergeWrap<uchar, cv::hal::merge8u>,
        mergeWrap<uchar, cv::hal::merge8u>, 0,
    };

    ආපසු mergeTab[depth];
}

#ifdef HAVE_IPP

static bool ipp_merge(const Mat* mv, Mat& dst, පූර්ණ_සංඛ්‍යා channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    නම්(channels != 3 && channels != 4)
        ආපසු false;

    නම්(mv[0].dims <= 2)
    {
        IppiSize    size       = ippiSize(mv[0].size());
        const void *srcPtrs[4] = {NULL};
        size_t      srcStep    = mv[0].step;
        සඳහා(පූර්ණ_සංඛ්‍යා i = 0; i < channels; i++)
        {
            srcPtrs[i] = mv[i].ptr();
            නම්(srcStep != mv[i].step)
                ආපසු false;
        }

        ආපසු CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, srcPtrs, (පූර්ණ_සංඛ්‍යා)srcStep, dst.ptr(), (පූර්ණ_සංඛ්‍යා)dst.step, size, (පූර්ණ_සංඛ්‍යා)mv[0].elemSize1(), channels, 0) >= 0;
    }
    නැතහොත්
    {
        const Mat *arrays[5] = {NULL};
        uchar     *ptrs[5]   = {NULL};
        arrays[0] = &dst;

        සඳහා(පූර්ණ_සංඛ්‍යා i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (පූර්ණ_සංඛ්‍යා)it.size, 1 };

        සඳහා( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            නම්(CV_INSTRUMENT_FUN_IPP(llwiCopyMerge, (const void**)&ptrs[1], 0, ptrs[0], 0, size, (පූර්ණ_සංඛ්‍යා)mv[0].elemSize1(), channels, 0) < 0)
                ආපසු false;
        }
        ආපසු true;
    }
#නැතහොත්
    CV_UNUSED(dst); CV_UNUSED(mv); CV_UNUSED(channels);
    ආපසු false;
#endif
}
#endif

void merge(const Mat* mv, size_t n, OutputArray _dst)
{
    CV_INSTRUMENT_REGION();

    CV_Assert( mv && n > 0 );
    CV_Assert(!mv[0].empty());

    පූර්ණ_සංඛ්‍යා depth = mv[0].depth();
    bool allch1 = true;
    පූර්ණ_සංඛ්‍යා k, cn = 0;
    size_t i;

    සඳහා( i = 0; i < n; i++ )
    {
        CV_Assert(mv[i].size == mv[0].size && mv[i].depth() == depth);
        allch1 = allch1 && mv[i].channels() == 1;
        cn += mv[i].channels();
    }

    CV_Assert( 0 < cn && cn <= CV_CN_MAX );
    _dst.create(mv[0].size, CV_MAKETYPE(depth, cn));
    Mat dst = _dst.getMat();

    නම්( n == 1 )
    {
        mv[0].copyTo(dst);
        ආපසු;
    }

    CV_IPP_RUN(allch1, ipp_merge(mv, dst, (පූර්ණ_සංඛ්‍යා)n));

    නම්( !allch1 )
    {
        AutoBuffer<පූර්ණ_සංඛ්‍යා> pairs(cn*2);
        පූර්ණ_සංඛ්‍යා j, ni=0;

        සඳහා( i = 0, j = 0; i < n; i++, j += ni )
        {
            ni = mv[i].channels();
            සඳහා( k = 0; k < ni; k++ )
            {
                pairs[(j+k)*2] = j + k;
                pairs[(j+k)*2+1] = j + k;
            }
        }
        mixChannels( mv, n, &dst, 1, &pairs[0], cn );
        ආපසු;
    }

    MergeFunc func = getMergeFunc(depth);
    CV_Assert( func != 0 );

    size_t esz = dst.elemSize(), esz1 = dst.elemSize1();
    size_t blocksize0 = (පූර්ණ_සංඛ්‍යා)((BLOCK_SIZE + esz-1)/esz);
    AutoBuffer<uchar> _buf((cn+1)*(parimaa_na(Mat*) + parimaa_na(uchar*)) + 16);
    const Mat** arrays = (const Mat**)_buf.data();
    uchar** ptrs = (uchar**)alignPtr(arrays + cn + 1, 16);

    arrays[0] = &dst;
    සඳහා( k = 0; k < cn; k++ )
        arrays[k+1] = &mv[k];

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = (පූර්ණ_සංඛ්‍යා)it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    සඳහා( i = 0; i < it.nplanes; i++, ++it )
    {
        සඳහා( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( (const void**)&ptrs[1], ptrs[0], (පූර්ණ_සංඛ්‍යා)bsz, cn );

            නම්( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                සඳහා( පූර්ණ_සංඛ්‍යා t = 0; t < cn; t++ )
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

    පූර්ණ_සංඛ්‍යා type = src[0].type(), depth = CV_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    Size size = src[0].size();

    සඳහා (size_t i = 0, srcsize = src.size(); i < srcsize; ++i)
    {
        පූර්ණ_සංඛ්‍යා itype = src[i].type(), icn = CV_MAT_CN(itype), idepth = CV_MAT_DEPTH(itype),
                esz1 = CV_ELEM_SIZE1(idepth);
        නම් (src[i].dims > 2)
            ආපසු false;

        CV_Assert(size == src[i].size() && depth == idepth);

        සඳහා (පූර්ණ_සංඛ්‍යා cn = 0; cn < icn; ++cn)
        {
            UMat tsrc = src[i];
            tsrc.offset += cn * esz1;
            ksrc.push_back(tsrc);
        }
    }
    පූර්ණ_සංඛ්‍යා dcn = (පූර්ණ_සංඛ්‍යා)ksrc.size();

    String srcargs, processelem, cndecl, indexdecl;
    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < dcn; ++i)
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
    නම් (k.empty())
        ආපසු false;

    _dst.create(size, CV_MAKE_TYPE(depth, dcn));
    UMat dst = _dst.getUMat();

    පූර්ණ_සංඛ්‍යා argidx = 0;
    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < dcn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::ReadOnlyNoSize(ksrc[i]));
    argidx = k.set(argidx, ocl::KernelArg::WriteOnly(dst));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)dst.cols, ((size_t)dst.rows + rowsPerWI - 1) / rowsPerWI };
    ආපසු k.run(2, globalsize, NULL, false);
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
