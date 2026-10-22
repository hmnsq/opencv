// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(පූර්ණ_සංඛ්‍යා depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, පූර්ණ_සංඛ්‍යා & res )
{
    පූර්ණ_සංඛ්‍යා type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    නම් (depth == CV_64F && !doubleSupport)
        ආපසු false;

    පූර්ණ_සංඛ්‍යා dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    පූර්ණ_සංඛ්‍යා wgs2_aligned = 1;
    අතරතුර (wgs2_aligned < (පූර්ණ_සංඛ්‍යා)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (පූර්ණ_සංඛ්‍යා)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    නම් (k.empty())
        ආපසු false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (පූර්ණ_සංඛ්‍යා)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    නම් (k.run(1, &globalsize, &wgs, true))
        ආපසු res = saturate_cast<පූර්ණ_සංඛ්‍යා>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    ආපසු false;
}
#endif

#නම් defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, පූර්ණ_සංඛ්‍යා &res )
{
    CV_INSTRUMENT_REGION_IPP();

#නම් IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    නම්(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        ආපසු false;
#endif

    Ipp32s  count = 0;
    පූර්ණ_සංඛ්‍යා     depth = src.depth();

    නම්(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        නම්(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (පූර්ණ_සංඛ්‍යා)src.step, size, &count, 0, 0);
        නැතහොත් නම්(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (පූර්ණ_සංඛ්‍යා)src.step, size, &count, 0, 0);
        නැතහොත්
            ආපසු false;

        නම්(status < 0)
            ආපසු false;

        res = size.width*size.height - count;
    }
    නැතහොත්
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(පූර්ණ_සංඛ්‍යා)it.size*src.channels(), 1};
        res = 0;
        සඳහා (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            නම්(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (පූර්ණ_සංඛ්‍යා)it.planes->step, size, &count, 0, 0);
            නැතහොත් නම්(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (පූර්ණ_සංඛ්‍යා)it.planes->step, size, &count, 0, 0);
            නැතහොත්
                ආපසු false;

            නම්(status < 0 || (පූර්ණ_සංඛ්‍යා)it.planes->total()*src.channels() < count)
                ආපසු false;

            res += (පූර්ණ_සංඛ්‍යා)it.planes->total()*src.channels() - count;
        }
    }

    ආපසු true;
}
#endif

පූර්ණ_සංඛ්‍යා countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    පූර්ණ_සංඛ්‍යා type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#නම් defined HAVE_OPENCL || defined HAVE_IPP
    පූර්ණ_සංඛ්‍යා res = -1;
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
    පූර්ණ_සංඛ්‍යා total = (පූර්ණ_සංඛ්‍යා)it.size, nz = 0;

    සඳහා( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    ආපසු nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    පූර්ණ_සංඛ්‍යා depth = src.depth();
    std::vector<Point> idxvec;
    පූර්ණ_සංඛ්‍යා rows = src.rows, cols = src.cols;
    AutoBuffer<පූර්ණ_සංඛ්‍යා> buf_(cols + 1);
    පූර්ණ_සංඛ්‍යා* buf = buf_.data();

    සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < rows; i++ )
    {
        පූර්ණ_සංඛ්‍යා j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        නම්( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#නම් (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const පූර්ණ_සංඛ්‍යා vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            සඳහා( ; j <= cols - vlanes; j += vlanes )
            {
                නම්( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                සඳහා( පූර්ණ_සංඛ්‍යා t = j; t < j + vlanes; t++ )
                    නම්( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            සඳහා( ; j < cols; j++ )
                නම්( ptr8[j] != 0 ) buf[k++] = j;
        }
        නැතහොත් නම්( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            සඳහා( j = 0; j < cols; j++ )
                නම්( ptr16[j] != 0 ) buf[k++] = j;
        }
        නැතහොත් නම්( depth == CV_32S || depth == CV_32U )
        {
            const පූර්ණ_සංඛ්‍යා* ptr32s = (const පූර්ණ_සංඛ්‍යා*)ptr8;
            සඳහා( j = 0; j < cols; j++ )
                නම්( ptr32s[j] != 0 ) buf[k++] = j;
        }
        නැතහොත් නම්( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            සඳහා( j = 0; j < cols; j++ )
                නම්( ptr64s[j] != 0 ) buf[k++] = j;
        }
        නැතහොත් නම්( depth == CV_32F )
        {
            const පූර්ණ_සංඛ්‍යා* ptr32s = (const පූර්ණ_සංඛ්‍යා*)ptr8;
            සඳහා( j = 0; j < cols; j++ )
                නම්( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        නැතහොත් නම්( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            සඳහා( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                නම්( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        නැතහොත් නම්( depth == CV_8F_E4M3FN )
        {
            සඳහා( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                නම්( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        නැතහොත්
        {
            const int64* ptr64s = (const int64*)ptr8;
            සඳහා( j = 0; j < cols; j++ )
                නම්( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        නම්( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            සඳහා( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    නම්( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    නම්( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
