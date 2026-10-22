// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(จำนวนเต็ม depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, จำนวนเต็ม & res )
{
    จำนวนเต็ม type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ถ้า (depth == CV_64F && !doubleSupport)
        คืน false;

    จำนวนเต็ม dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    จำนวนเต็ม wgs2_aligned = 1;
    ขณะ (wgs2_aligned < (จำนวนเต็ม)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (จำนวนเต็ม)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ถ้า (k.empty())
        คืน false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (จำนวนเต็ม)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ถ้า (k.run(1, &globalsize, &wgs, true))
        คืน res = saturate_cast<จำนวนเต็ม>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    คืน false;
}
#endif

#ถ้า defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, จำนวนเต็ม &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ถ้า IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    ถ้า(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        คืน false;
#endif

    Ipp32s  count = 0;
    จำนวนเต็ม     depth = src.depth();

    ถ้า(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        ถ้า(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (จำนวนเต็ม)src.step, size, &count, 0, 0);
        มิฉะนั้น ถ้า(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (จำนวนเต็ม)src.step, size, &count, 0, 0);
        มิฉะนั้น
            คืน false;

        ถ้า(status < 0)
            คืน false;

        res = size.width*size.height - count;
    }
    มิฉะนั้น
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(จำนวนเต็ม)it.size*src.channels(), 1};
        res = 0;
        สำหรับ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ถ้า(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (จำนวนเต็ม)it.planes->step, size, &count, 0, 0);
            มิฉะนั้น ถ้า(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (จำนวนเต็ม)it.planes->step, size, &count, 0, 0);
            มิฉะนั้น
                คืน false;

            ถ้า(status < 0 || (จำนวนเต็ม)it.planes->total()*src.channels() < count)
                คืน false;

            res += (จำนวนเต็ม)it.planes->total()*src.channels() - count;
        }
    }

    คืน true;
}
#endif

จำนวนเต็ม countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    จำนวนเต็ม type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ถ้า defined HAVE_OPENCL || defined HAVE_IPP
    จำนวนเต็ม res = -1;
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
    จำนวนเต็ม total = (จำนวนเต็ม)it.size, nz = 0;

    สำหรับ( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    คืน nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    จำนวนเต็ม depth = src.depth();
    std::vector<Point> idxvec;
    จำนวนเต็ม rows = src.rows, cols = src.cols;
    AutoBuffer<จำนวนเต็ม> buf_(cols + 1);
    จำนวนเต็ม* buf = buf_.data();

    สำหรับ( จำนวนเต็ม i = 0; i < rows; i++ )
    {
        จำนวนเต็ม j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        ถ้า( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ถ้า (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const จำนวนเต็ม vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            สำหรับ( ; j <= cols - vlanes; j += vlanes )
            {
                ถ้า( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                สำหรับ( จำนวนเต็ม t = j; t < j + vlanes; t++ )
                    ถ้า( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            สำหรับ( ; j < cols; j++ )
                ถ้า( ptr8[j] != 0 ) buf[k++] = j;
        }
        มิฉะนั้น ถ้า( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            สำหรับ( j = 0; j < cols; j++ )
                ถ้า( ptr16[j] != 0 ) buf[k++] = j;
        }
        มิฉะนั้น ถ้า( depth == CV_32S || depth == CV_32U )
        {
            const จำนวนเต็ม* ptr32s = (const จำนวนเต็ม*)ptr8;
            สำหรับ( j = 0; j < cols; j++ )
                ถ้า( ptr32s[j] != 0 ) buf[k++] = j;
        }
        มิฉะนั้น ถ้า( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            สำหรับ( j = 0; j < cols; j++ )
                ถ้า( ptr64s[j] != 0 ) buf[k++] = j;
        }
        มิฉะนั้น ถ้า( depth == CV_32F )
        {
            const จำนวนเต็ม* ptr32s = (const จำนวนเต็ม*)ptr8;
            สำหรับ( j = 0; j < cols; j++ )
                ถ้า( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        มิฉะนั้น ถ้า( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            สำหรับ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ถ้า( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        มิฉะนั้น ถ้า( depth == CV_8F_E4M3FN )
        {
            สำหรับ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ถ้า( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        มิฉะนั้น
        {
            const int64* ptr64s = (const int64*)ptr8;
            สำหรับ( j = 0; j < cols; j++ )
                ถ้า( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ถ้า( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            สำหรับ( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    ถ้า( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ถ้า( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
