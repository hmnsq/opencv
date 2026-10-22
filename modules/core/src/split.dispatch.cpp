// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "split.simd.hpp"
#include "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

void split8u(const uchar* src, uchar** dst, ကိန်းပြည့် len, ကိန်းပြည့် cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split16u(const ushort* src, ushort** dst, ကိန်းပြည့် len, ကိန်းပြည့် cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split32s(const ကိန်းပြည့်* src, ကိန်းပြည့်** dst, ကိန်းပြည့် len, ကိန်းပြည့် cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split64s(const int64* src, int64** dst, ကိန်းပြည့် len, ကိန်းပြည့် cn )
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

typedef void (*SplitFunc)(const void* src, void** dst, ကိန်းပြည့် len, ကိန်းပြည့် cn);

template<typename T, void (*fn)(const T*, T**, ကိန်းပြည့်, ကိန်းပြည့်)>
static void splitWrap(const void* src, void** dst, ကိန်းပြည့် len, ကိန်းပြည့် cn)
{
    fn((const T*)src, (T**)dst, len, cn);
}

static SplitFunc getSplitFunc(ကိန်းပြည့် depth)
{
    static SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ကိန်းပြည့်, cv::hal::split32s>, splitWrap<ကိန်းပြည့်, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<ကိန်းပြည့်, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    ပြန်ပေး splitTab[depth];
}

#ifdef HAVE_IPP

static bool ipp_split(const Mat& src, Mat* mv, ကိန်းပြည့် channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    အကယ်၍(channels != 3 && channels != 4)
        ပြန်ပေး false;

    အကယ်၍(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        void    *dstPtrs[4] = {NULL};
        size_t   dstStep    = mv[0].step;
        အတွက်(ကိန်းပြည့် i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            အကယ်၍(dstStep != mv[i].step)
                ပြန်ပေး false;
        }

        ပြန်ပေး CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (ကိန်းပြည့်)src.step, dstPtrs, (ကိန်းပြည့်)dstStep, size, (ကိန်းပြည့်)src.elemSize1(), channels, 0) >= 0;
    }
    မဟုတ်ပါက
    {
        const Mat *arrays[5] = {NULL};
        uchar     *ptrs[5]   = {NULL};
        arrays[0] = &src;

        အတွက်(ကိန်းပြည့် i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ကိန်းပြည့်)it.size, 1 };

        အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            အကယ်၍(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (void**)&ptrs[1], 0, size, (ကိန်းပြည့်)src.elemSize1(), channels, 0) < 0)
                ပြန်ပေး false;
        }
        ပြန်ပေး true;
    }
#မဟုတ်ပါက
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    ပြန်ပေး false;
#endif
}
#endif

void split(const Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    ကိန်းပြည့် k, depth = src.depth(), cn = src.channels();
    အကယ်၍( cn == 1 )
    {
        src.copyTo(mv[0]);
        ပြန်ပေး;
    }

    အတွက်( k = 0; k < cn; k++ )
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
    အတွက်( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        အတွက်( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (void**)&ptrs[1], (ကိန်းပြည့်)bsz, cn );

            အကယ်၍( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                အတွက်( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

static bool ocl_split( InputArray _m, OutputArrayOfArrays _mv )
{
    ကိန်းပြည့် type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    အတွက် (ကိန်းပြည့် i = 0; i < cn; ++i)
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
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    အတွက် (ကိန်းပြည့် i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    ကိန်းပြည့် argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    အတွက် (ကိန်းပြည့် i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ပြန်ပေး k.run(2, globalsize, NULL, false);
}

#endif

void split(InputArray _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    အကယ်၍( m.empty() )
    {
        _mv.release();
        ပြန်ပေး;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    ကိန်းပြည့် depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    အတွက် (ကိန်းပြည့် i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
