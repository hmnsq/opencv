// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(puur_naa_nka depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, puur_naa_nka & res )
{
    puur_naa_nka type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ᬬᬾᬦ᭄ (depth == CV_64F && !doubleSupport)
        ᬯᬮᬶ false;

    puur_naa_nka dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    puur_naa_nka wgs2_aligned = 1;
    ᬲᭂᬤᭂᬮ (wgs2_aligned < (puur_naa_nka)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (puur_naa_nka)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ᬬᬾᬦ᭄ (k.empty())
        ᬯᬮᬶ false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (puur_naa_nka)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ᬬᬾᬦ᭄ (k.run(1, &globalsize, &wgs, true))
        ᬯᬮᬶ res = saturate_cast<puur_naa_nka>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    ᬯᬮᬶ false;
}
#endif

#ᬬᬾᬦ᭄ defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, puur_naa_nka &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ᬬᬾᬦ᭄ IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    ᬬᬾᬦ᭄(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        ᬯᬮᬶ false;
#endif

    Ipp32s  count = 0;
    puur_naa_nka     depth = src.depth();

    ᬬᬾᬦ᭄(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        ᬬᬾᬦ᭄(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (puur_naa_nka)src.step, size, &count, 0, 0);
        ᬮᬶᬬ ᬬᬾᬦ᭄(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (puur_naa_nka)src.step, size, &count, 0, 0);
        ᬮᬶᬬ
            ᬯᬮᬶ false;

        ᬬᬾᬦ᭄(status < 0)
            ᬯᬮᬶ false;

        res = size.width*size.height - count;
    }
    ᬮᬶᬬ
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(puur_naa_nka)it.size*src.channels(), 1};
        res = 0;
        for (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ᬬᬾᬦ᭄(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (puur_naa_nka)it.planes->step, size, &count, 0, 0);
            ᬮᬶᬬ ᬬᬾᬦ᭄(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (puur_naa_nka)it.planes->step, size, &count, 0, 0);
            ᬮᬶᬬ
                ᬯᬮᬶ false;

            ᬬᬾᬦ᭄(status < 0 || (puur_naa_nka)it.planes->total()*src.channels() < count)
                ᬯᬮᬶ false;

            res += (puur_naa_nka)it.planes->total()*src.channels() - count;
        }
    }

    ᬯᬮᬶ true;
}
#endif

puur_naa_nka countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    puur_naa_nka type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ᬬᬾᬦ᭄ defined HAVE_OPENCL || defined HAVE_IPP
    puur_naa_nka res = -1;
#endif

#ifdef HAVE_OPENCL
    CV_OCL_RUN_(OCL_PERFORMANCE_CHECK(_src.isUMat()) && _src.dims() <= 2,
                ocl_countNonZero(_src, res),
                res)
#endif

    Mat src = _src.getMat();
    CV_IPP_RUN_FAST(ipp_countNonZero(src, res), res);

    CountNonZeroFunc func = getCountNonZeroTab(src.depth());
    CV_Assert( func != 0 );

    const Mat* arrays[] = {&src, 0};
    uchar* ptrs[1] = {};
    NAryMatIterator it(arrays, ptrs);
    puur_naa_nka total = (puur_naa_nka)it.size, nz = 0;

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    ᬯᬮᬶ nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    puur_naa_nka depth = src.depth();
    std::vector<Point> idxvec;
    puur_naa_nka rows = src.rows, cols = src.cols;
    AutoBuffer<puur_naa_nka> buf_(cols + 1);
    puur_naa_nka* buf = buf_.data();

    for( puur_naa_nka i = 0; i < rows; i++ )
    {
        puur_naa_nka j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        ᬬᬾᬦ᭄( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ᬬᬾᬦ᭄ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const puur_naa_nka vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            for( ; j <= cols - vlanes; j += vlanes )
            {
                ᬬᬾᬦ᭄( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                for( puur_naa_nka t = j; t < j + vlanes; t++ )
                    ᬬᬾᬦ᭄( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            for( ; j < cols; j++ )
                ᬬᬾᬦ᭄( ptr8[j] != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            for( j = 0; j < cols; j++ )
                ᬬᬾᬦ᭄( ptr16[j] != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄( depth == CV_32S || depth == CV_32U )
        {
            const puur_naa_nka* ptr32s = (const puur_naa_nka*)ptr8;
            for( j = 0; j < cols; j++ )
                ᬬᬾᬦ᭄( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            for( j = 0; j < cols; j++ )
                ᬬᬾᬦ᭄( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄( depth == CV_32F )
        {
            const puur_naa_nka* ptr32s = (const puur_naa_nka*)ptr8;
            for( j = 0; j < cols; j++ )
                ᬬᬾᬦ᭄( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            for( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ᬬᬾᬦ᭄( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄( depth == CV_8F_E4M3FN )
        {
            for( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ᬬᬾᬦ᭄( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ᬮᬶᬬ
        {
            const int64* ptr64s = (const int64*)ptr8;
            for( j = 0; j < cols; j++ )
                ᬬᬾᬦ᭄( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ᬬᬾᬦ᭄( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            for( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    ᬬᬾᬦ᭄( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ᬬᬾᬦ᭄( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
