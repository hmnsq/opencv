// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(số_nguyên depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, số_nguyên & res )
{
    số_nguyên type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    nếu (depth == CV_64F && !doubleSupport)
        trả_về false;

    số_nguyên dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    số_nguyên wgs2_aligned = 1;
    trong_khi (wgs2_aligned < (số_nguyên)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (số_nguyên)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    nếu (k.empty())
        trả_về false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (số_nguyên)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    nếu (k.run(1, &globalsize, &wgs, true))
        trả_về res = saturate_cast<số_nguyên>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    trả_về false;
}
#endif

#nếu defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, số_nguyên &res )
{
    CV_INSTRUMENT_REGION_IPP();

#nếu IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    nếu(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        trả_về false;
#endif

    Ipp32s  count = 0;
    số_nguyên     depth = src.depth();

    nếu(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        nếu(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (số_nguyên)src.step, size, &count, 0, 0);
        không_thì nếu(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (số_nguyên)src.step, size, &count, 0, 0);
        không_thì
            trả_về false;

        nếu(status < 0)
            trả_về false;

        res = size.width*size.height - count;
    }
    không_thì
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(số_nguyên)it.size*src.channels(), 1};
        res = 0;
        cho (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            nếu(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (số_nguyên)it.planes->step, size, &count, 0, 0);
            không_thì nếu(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (số_nguyên)it.planes->step, size, &count, 0, 0);
            không_thì
                trả_về false;

            nếu(status < 0 || (số_nguyên)it.planes->total()*src.channels() < count)
                trả_về false;

            res += (số_nguyên)it.planes->total()*src.channels() - count;
        }
    }

    trả_về true;
}
#endif

số_nguyên countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    số_nguyên type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#nếu defined HAVE_OPENCL || defined HAVE_IPP
    số_nguyên res = -1;
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
    số_nguyên total = (số_nguyên)it.size, nz = 0;

    cho( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    trả_về nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    số_nguyên depth = src.depth();
    std::vector<Point> idxvec;
    số_nguyên rows = src.rows, cols = src.cols;
    AutoBuffer<số_nguyên> buf_(cols + 1);
    số_nguyên* buf = buf_.data();

    cho( số_nguyên i = 0; i < rows; i++ )
    {
        số_nguyên j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        nếu( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#nếu (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const số_nguyên vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            cho( ; j <= cols - vlanes; j += vlanes )
            {
                nếu( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                cho( số_nguyên t = j; t < j + vlanes; t++ )
                    nếu( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            cho( ; j < cols; j++ )
                nếu( ptr8[j] != 0 ) buf[k++] = j;
        }
        không_thì nếu( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            cho( j = 0; j < cols; j++ )
                nếu( ptr16[j] != 0 ) buf[k++] = j;
        }
        không_thì nếu( depth == CV_32S || depth == CV_32U )
        {
            const số_nguyên* ptr32s = (const số_nguyên*)ptr8;
            cho( j = 0; j < cols; j++ )
                nếu( ptr32s[j] != 0 ) buf[k++] = j;
        }
        không_thì nếu( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            cho( j = 0; j < cols; j++ )
                nếu( ptr64s[j] != 0 ) buf[k++] = j;
        }
        không_thì nếu( depth == CV_32F )
        {
            const số_nguyên* ptr32s = (const số_nguyên*)ptr8;
            cho( j = 0; j < cols; j++ )
                nếu( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        không_thì nếu( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            cho( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                nếu( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        không_thì nếu( depth == CV_8F_E4M3FN )
        {
            cho( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                nếu( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        không_thì
        {
            const int64* ptr64s = (const int64*)ptr8;
            cho( j = 0; j < cols; j++ )
                nếu( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        nếu( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            cho( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    nếu( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    nếu( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
