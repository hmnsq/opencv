// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(целое depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, целое & res )
{
    целое type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    если (depth == CV_64F && !doubleSupport)
        вернуть false;

    целое dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    целое wgs2_aligned = 1;
    пока (wgs2_aligned < (целое)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (целое)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    если (k.empty())
        вернуть false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (целое)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    если (k.run(1, &globalsize, &wgs, true))
        вернуть res = saturate_cast<целое>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    вернуть false;
}
#endif

#если defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, целое &res )
{
    CV_INSTRUMENT_REGION_IPP();

#если IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    если(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        вернуть false;
#endif

    Ipp32s  count = 0;
    целое     depth = src.depth();

    если(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        если(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (целое)src.step, size, &count, 0, 0);
        иначе если(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (целое)src.step, size, &count, 0, 0);
        иначе
            вернуть false;

        если(status < 0)
            вернуть false;

        res = size.width*size.height - count;
    }
    иначе
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(целое)it.size*src.channels(), 1};
        res = 0;
        для (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            если(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (целое)it.planes->step, size, &count, 0, 0);
            иначе если(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (целое)it.planes->step, size, &count, 0, 0);
            иначе
                вернуть false;

            если(status < 0 || (целое)it.planes->total()*src.channels() < count)
                вернуть false;

            res += (целое)it.planes->total()*src.channels() - count;
        }
    }

    вернуть true;
}
#endif

целое countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    целое type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#если defined HAVE_OPENCL || defined HAVE_IPP
    целое res = -1;
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
    целое total = (целое)it.size, nz = 0;

    для( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    вернуть nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    целое depth = src.depth();
    std::vector<Point> idxvec;
    целое rows = src.rows, cols = src.cols;
    AutoBuffer<целое> buf_(cols + 1);
    целое* buf = buf_.data();

    для( целое i = 0; i < rows; i++ )
    {
        целое j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        если( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#если (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const целое vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            для( ; j <= cols - vlanes; j += vlanes )
            {
                если( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                для( целое t = j; t < j + vlanes; t++ )
                    если( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            для( ; j < cols; j++ )
                если( ptr8[j] != 0 ) buf[k++] = j;
        }
        иначе если( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            для( j = 0; j < cols; j++ )
                если( ptr16[j] != 0 ) buf[k++] = j;
        }
        иначе если( depth == CV_32S || depth == CV_32U )
        {
            const целое* ptr32s = (const целое*)ptr8;
            для( j = 0; j < cols; j++ )
                если( ptr32s[j] != 0 ) buf[k++] = j;
        }
        иначе если( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            для( j = 0; j < cols; j++ )
                если( ptr64s[j] != 0 ) buf[k++] = j;
        }
        иначе если( depth == CV_32F )
        {
            const целое* ptr32s = (const целое*)ptr8;
            для( j = 0; j < cols; j++ )
                если( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        иначе если( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            для( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                если( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        иначе если( depth == CV_8F_E4M3FN )
        {
            для( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                если( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        иначе
        {
            const int64* ptr64s = (const int64*)ptr8;
            для( j = 0; j < cols; j++ )
                если( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        если( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            для( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    если( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    если( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
