// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(ຈຳນວນເຕັມ depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, ຈຳນວນເຕັມ & res )
{
    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    ຖ້າ (depth == CV_64F && !doubleSupport)
        ສົ່ງຄືນ false;

    ຈຳນວນເຕັມ dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ຈຳນວນເຕັມ wgs2_aligned = 1;
    ໃນຂະນະທີ່ (wgs2_aligned < (ຈຳນວນເຕັມ)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ຈຳນວນເຕັມ)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ຈຳນວນເຕັມ)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    ຖ້າ (k.run(1, &globalsize, &wgs, true))
        ສົ່ງຄືນ res = saturate_cast<ຈຳນວນເຕັມ>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    ສົ່ງຄືນ false;
}
#endif

#ຖ້າ defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, ຈຳນວນເຕັມ &res )
{
    CV_INSTRUMENT_REGION_IPP();

#ຖ້າ IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    ຖ້າ(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        ສົ່ງຄືນ false;
#endif

    Ipp32s  count = 0;
    ຈຳນວນເຕັມ     depth = src.depth();

    ຖ້າ(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        ຖ້າ(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (ຈຳນວນເຕັມ)src.step, size, &count, 0, 0);
        ບໍ່ດັ່ງນັ້ນ ຖ້າ(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (ຈຳນວນເຕັມ)src.step, size, &count, 0, 0);
        ບໍ່ດັ່ງນັ້ນ
            ສົ່ງຄືນ false;

        ຖ້າ(status < 0)
            ສົ່ງຄືນ false;

        res = size.width*size.height - count;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(ຈຳນວນເຕັມ)it.size*src.channels(), 1};
        res = 0;
        ສຳລັບ (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            ຖ້າ(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (ຈຳນວນເຕັມ)it.planes->step, size, &count, 0, 0);
            ບໍ່ດັ່ງນັ້ນ ຖ້າ(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (ຈຳນວນເຕັມ)it.planes->step, size, &count, 0, 0);
            ບໍ່ດັ່ງນັ້ນ
                ສົ່ງຄືນ false;

            ຖ້າ(status < 0 || (ຈຳນວນເຕັມ)it.planes->total()*src.channels() < count)
                ສົ່ງຄືນ false;

            res += (ຈຳນວນເຕັມ)it.planes->total()*src.channels() - count;
        }
    }

    ສົ່ງຄືນ true;
}
#endif

ຈຳນວນເຕັມ countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#ຖ້າ defined HAVE_OPENCL || defined HAVE_IPP
    ຈຳນວນເຕັມ res = -1;
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
    ຈຳນວນເຕັມ total = (ຈຳນວນເຕັມ)it.size, nz = 0;

    ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    ສົ່ງຄືນ nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    ຈຳນວນເຕັມ depth = src.depth();
    std::vector<Point> idxvec;
    ຈຳນວນເຕັມ rows = src.rows, cols = src.cols;
    AutoBuffer<ຈຳນວນເຕັມ> buf_(cols + 1);
    ຈຳນວນເຕັມ* buf = buf_.data();

    ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < rows; i++ )
    {
        ຈຳນວນເຕັມ j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        ຖ້າ( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#ຖ້າ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const ຈຳນວນເຕັມ vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            ສຳລັບ( ; j <= cols - vlanes; j += vlanes )
            {
                ຖ້າ( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                ສຳລັບ( ຈຳນວນເຕັມ t = j; t < j + vlanes; t++ )
                    ຖ້າ( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            ສຳລັບ( ; j < cols; j++ )
                ຖ້າ( ptr8[j] != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            ສຳລັບ( j = 0; j < cols; j++ )
                ຖ້າ( ptr16[j] != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_32S || depth == CV_32U )
        {
            const ຈຳນວນເຕັມ* ptr32s = (const ຈຳນວນເຕັມ*)ptr8;
            ສຳລັບ( j = 0; j < cols; j++ )
                ຖ້າ( ptr32s[j] != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            ສຳລັບ( j = 0; j < cols; j++ )
                ຖ້າ( ptr64s[j] != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_32F )
        {
            const ຈຳນວນເຕັມ* ptr32s = (const ຈຳນວນເຕັມ*)ptr8;
            ສຳລັບ( j = 0; j < cols; j++ )
                ຖ້າ( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            ສຳລັບ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                ຖ້າ( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ( depth == CV_8F_E4M3FN )
        {
            ສຳລັບ( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                ຖ້າ( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            const int64* ptr64s = (const int64*)ptr8;
            ສຳລັບ( j = 0; j < cols; j++ )
                ຖ້າ( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        ຖ້າ( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            ສຳລັບ( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    ຖ້າ( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    ຖ້າ( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
