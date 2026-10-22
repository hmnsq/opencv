// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"

#include "split.simd.hpp"
#include "split.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

void split8u(const uchar* src, uchar** dst, ចំនួនគត់ len, ចំនួនគត់ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split8u, cv_hal_split8u, src,dst, len, cn)
    CV_CPU_DISPATCH(split8u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split16u(const ushort* src, ushort** dst, ចំនួនគត់ len, ចំនួនគត់ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split16u, cv_hal_split16u, src,dst, len, cn)
    CV_CPU_DISPATCH(split16u, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split32s(const ចំនួនគត់* src, ចំនួនគត់** dst, ចំនួនគត់ len, ចំនួនគត់ cn )
{
    CV_INSTRUMENT_REGION();
    CALL_HAL(split32s, cv_hal_split32s, src,dst, len, cn)
    CV_CPU_DISPATCH(split32s, (src, dst, len, cn),
        CV_CPU_DISPATCH_MODES_ALL);
}

void split64s(const int64* src, int64** dst, ចំនួនគត់ len, ចំនួនគត់ cn )
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

typedef void (*SplitFunc)(const void* src, void** dst, ចំនួនគត់ len, ចំនួនគត់ cn);

template<typename T, void (*fn)(const T*, T**, ចំនួនគត់, ចំនួនគត់)>
static void splitWrap(const void* src, void** dst, ចំនួនគត់ len, ចំនួនគត់ cn)
{
    fn((const T*)src, (T**)dst, len, cn);
}

static SplitFunc getSplitFunc(ចំនួនគត់ depth)
{
    static SplitFunc splitTab[CV_DEPTH_MAX] =
    {
        splitWrap<uchar, cv::hal::split8u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ចំនួនគត់, cv::hal::split32s>, splitWrap<ចំនួនគត់, cv::hal::split32s>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<ushort, cv::hal::split16u>,
        splitWrap<ushort, cv::hal::split16u>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<int64_t, cv::hal::split64s>, splitWrap<int64_t, cv::hal::split64s>,
        splitWrap<ចំនួនគត់, cv::hal::split32s>, splitWrap<uchar, cv::hal::split8u>,
        splitWrap<uchar, cv::hal::split8u>, 0,
    };

    ត្រឡប់ splitTab[depth];
}

#ifdef HAVE_IPP

static bool ipp_split(const Mat& src, Mat* mv, ចំនួនគត់ channels)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ប្រសិនបើ(channels != 3 && channels != 4)
        ត្រឡប់ false;

    ប្រសិនបើ(src.dims <= 2)
    {
        IppiSize size       = ippiSize(src.size());
        void    *dstPtrs[4] = {NULL};
        size_t   dstStep    = mv[0].step;
        សម្រាប់(ចំនួនគត់ i = 0; i < channels; i++)
        {
            dstPtrs[i] = mv[i].ptr();
            ប្រសិនបើ(dstStep != mv[i].step)
                ត្រឡប់ false;
        }

        ត្រឡប់ CV_INSTRUMENT_FUN_IPP(llwiCopySplit, src.ptr(), (ចំនួនគត់)src.step, dstPtrs, (ចំនួនគត់)dstStep, size, (ចំនួនគត់)src.elemSize1(), channels, 0) >= 0;
    }
    បើមិនដូច្នេះ
    {
        const Mat *arrays[5] = {NULL};
        uchar     *ptrs[5]   = {NULL};
        arrays[0] = &src;

        សម្រាប់(ចំនួនគត់ i = 1; i < channels; i++)
        {
            arrays[i] = &mv[i-1];
        }

        NAryMatIterator it(arrays, ptrs);
        IppiSize size = { (ចំនួនគត់)it.size, 1 };

        សម្រាប់( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ប្រសិនបើ(CV_INSTRUMENT_FUN_IPP(llwiCopySplit, ptrs[0], 0, (void**)&ptrs[1], 0, size, (ចំនួនគត់)src.elemSize1(), channels, 0) < 0)
                ត្រឡប់ false;
        }
        ត្រឡប់ true;
    }
#បើមិនដូច្នេះ
    CV_UNUSED(src); CV_UNUSED(mv); CV_UNUSED(channels);
    ត្រឡប់ false;
#endif
}
#endif

void split(const Mat& src, Mat* mv)
{
    CV_INSTRUMENT_REGION();

    ចំនួនគត់ k, depth = src.depth(), cn = src.channels();
    ប្រសិនបើ( cn == 1 )
    {
        src.copyTo(mv[0]);
        ត្រឡប់;
    }

    សម្រាប់( k = 0; k < cn; k++ )
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
    សម្រាប់( k = 0; k < cn; k++ )
    {
        arrays[k+1] = &mv[k];
    }

    NAryMatIterator it(arrays, ptrs, cn+1);
    size_t total = it.size;
    size_t blocksize = std::min((size_t)CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn), cn <= 4 ? total : std::min(total, blocksize0));

    សម្រាប់( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        សម្រាប់( size_t j = 0; j < total; j += blocksize )
        {
            size_t bsz = std::min(total - j, blocksize);
            func( ptrs[0], (void**)&ptrs[1], (ចំនួនគត់)bsz, cn );

            ប្រសិនបើ( j + blocksize < total )
            {
                ptrs[0] += bsz*esz;
                សម្រាប់( k = 0; k < cn; k++ )
                    ptrs[k+1] += bsz*esz1;
            }
        }
    }
}

#ifdef HAVE_OPENCL

static bool ocl_split( InputArray _m, OutputArrayOfArrays _mv )
{
    ចំនួនគត់ type = _m.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    String dstargs, processelem, indexdecl;
    សម្រាប់ (ចំនួនគត់ i = 0; i < cn; ++i)
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
    ប្រសិនបើ (k.empty())
        ត្រឡប់ false;

    Size size = _m.size();
    _mv.create(cn, 1, depth);
    សម្រាប់ (ចំនួនគត់ i = 0; i < cn; ++i)
        _mv.create(size, depth, i);

    std::vector<UMat> dst;
    _mv.getUMatVector(dst);

    ចំនួនគត់ argidx = k.set(0, ocl::KernelArg::ReadOnly(_m.getUMat()));
    សម្រាប់ (ចំនួនគត់ i = 0; i < cn; ++i)
        argidx = k.set(argidx, ocl::KernelArg::WriteOnlyNoSize(dst[i]));
    k.set(argidx, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ត្រឡប់ k.run(2, globalsize, NULL, false);
}

#endif

void split(InputArray _m, OutputArrayOfArrays _mv)
{
    CV_INSTRUMENT_REGION();

    CV_OCL_RUN(_m.dims() <= 2 && _mv.isUMatVector(),
               ocl_split(_m, _mv))

    Mat m = _m.getMat();
    ប្រសិនបើ( m.empty() )
    {
        _mv.release();
        ត្រឡប់;
    }

    CV_Assert( !_mv.fixedType() || _mv.empty() || _mv.type() == m.depth() );

    ចំនួនគត់ depth = m.depth(), cn = m.channels();
    _mv.create(cn, 1, depth);
    សម្រាប់ (ចំនួនគត់ i = 0; i < cn; ++i)
        _mv.create(m.dims, m.size.p, depth, i);

    std::vector<Mat> dst;
    _mv.getMatVector(dst);

    split(m, &dst[0]);
}

} // namespace
