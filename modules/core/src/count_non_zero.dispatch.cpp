// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(정수 depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, 정수 & res )
{
    정수 type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    만약 (depth == CV_64F && !doubleSupport)
        반환 false;

    정수 dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    정수 wgs2_aligned = 1;
    동안 (wgs2_aligned < (정수)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (정수)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    만약 (k.empty())
        반환 false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (정수)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    만약 (k.run(1, &globalsize, &wgs, true))
        반환 res = saturate_cast<정수>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    반환 false;
}
#endif

#만약 defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, 정수 &res )
{
    CV_INSTRUMENT_REGION_IPP();

#만약 IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    만약(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        반환 false;
#endif

    Ipp32s  count = 0;
    정수     depth = src.depth();

    만약(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        만약(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (정수)src.step, size, &count, 0, 0);
        아니면 만약(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (정수)src.step, size, &count, 0, 0);
        아니면
            반환 false;

        만약(status < 0)
            반환 false;

        res = size.width*size.height - count;
    }
    아니면
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(정수)it.size*src.channels(), 1};
        res = 0;
        위해 (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            만약(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (정수)it.planes->step, size, &count, 0, 0);
            아니면 만약(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (정수)it.planes->step, size, &count, 0, 0);
            아니면
                반환 false;

            만약(status < 0 || (정수)it.planes->total()*src.channels() < count)
                반환 false;

            res += (정수)it.planes->total()*src.channels() - count;
        }
    }

    반환 true;
}
#endif

정수 countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    정수 type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#만약 defined HAVE_OPENCL || defined HAVE_IPP
    정수 res = -1;
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
    정수 total = (정수)it.size, nz = 0;

    위해( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    반환 nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    정수 depth = src.depth();
    std::vector<Point> idxvec;
    정수 rows = src.rows, cols = src.cols;
    AutoBuffer<정수> buf_(cols + 1);
    정수* buf = buf_.data();

    위해( 정수 i = 0; i < rows; i++ )
    {
        정수 j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        만약( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#만약 (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const 정수 vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            위해( ; j <= cols - vlanes; j += vlanes )
            {
                만약( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                위해( 정수 t = j; t < j + vlanes; t++ )
                    만약( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            위해( ; j < cols; j++ )
                만약( ptr8[j] != 0 ) buf[k++] = j;
        }
        아니면 만약( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            위해( j = 0; j < cols; j++ )
                만약( ptr16[j] != 0 ) buf[k++] = j;
        }
        아니면 만약( depth == CV_32S || depth == CV_32U )
        {
            const 정수* ptr32s = (const 정수*)ptr8;
            위해( j = 0; j < cols; j++ )
                만약( ptr32s[j] != 0 ) buf[k++] = j;
        }
        아니면 만약( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            위해( j = 0; j < cols; j++ )
                만약( ptr64s[j] != 0 ) buf[k++] = j;
        }
        아니면 만약( depth == CV_32F )
        {
            const 정수* ptr32s = (const 정수*)ptr8;
            위해( j = 0; j < cols; j++ )
                만약( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        아니면 만약( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            위해( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                만약( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        아니면 만약( depth == CV_8F_E4M3FN )
        {
            위해( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                만약( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        아니면
        {
            const int64* ptr64s = (const int64*)ptr8;
            위해( j = 0; j < cols; j++ )
                만약( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        만약( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            위해( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    만약( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    만약( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
