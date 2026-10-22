// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "stat.hpp"

#include "count_non_zero.simd.hpp"
#include "count_non_zero.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv {

static CountNonZeroFunc getCountNonZeroTab(ကိန်းပြည့် depth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getCountNonZeroTab, (depth),
        CV_CPU_DISPATCH_MODES_ALL);
}

#ifdef HAVE_OPENCL
static bool ocl_countNonZero( InputArray _src, ကိန်းပြည့် & res )
{
    ကိန်းပြည့် type = _src.type(), depth = CV_MAT_DEPTH(type), kercn = ocl::predictOptimalVectorWidth(_src);
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    အကယ်၍ (depth == CV_64F && !doubleSupport)
        ပြန်ပေး false;

    ကိန်းပြည့် dbsize = ocl::Device::getDefault().maxComputeUnits();
    size_t wgs = ocl::Device::getDefault().maxWorkGroupSize();

    ကိန်းပြည့် wgs2_aligned = 1;
    နေစဉ် (wgs2_aligned < (ကိန်းပြည့်)wgs)
        wgs2_aligned <<= 1;
    wgs2_aligned >>= 1;

    ocl::Kernel k("reduce", ocl::core::reduce_oclsrc,
                  format("-D srcT=%s -D srcT1=%s -D cn=1 -D OP_COUNT_NON_ZERO"
                         " -D WGS=%d -D kercn=%d -D WGS2_ALIGNED=%d%s%s",
                         ocl::typeToStr(CV_MAKE_TYPE(depth, kercn)),
                         ocl::typeToStr(depth), (ကိန်းပြည့်)wgs, kercn,
                         wgs2_aligned, doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                         _src.isContinuous() ? " -D HAVE_SRC_CONT" : ""));
    အကယ်၍ (k.empty())
        ပြန်ပေး false;

    UMat src = _src.getUMat(), db(1, dbsize, CV_32SC1);
    k.args(ocl::KernelArg::ReadOnlyNoSize(src), src.cols, (ကိန်းပြည့်)src.total(),
           dbsize, ocl::KernelArg::PtrWriteOnly(db));

    size_t globalsize = dbsize * wgs;
    အကယ်၍ (k.run(1, &globalsize, &wgs, true))
        ပြန်ပေး res = saturate_cast<ကိန်းပြည့်>(cv::sum(db.getMat(ACCESS_READ))[0]), true;
    ပြန်ပေး false;
}
#endif

#အကယ်၍ defined HAVE_IPP
static bool ipp_countNonZero( Mat &src, ကိန်းပြည့် &res )
{
    CV_INSTRUMENT_REGION_IPP();

#အကယ်၍ IPP_VERSION_X100 < 201801
    // Poor performance of SSE42
    အကယ်၍(cv::ipp::getIppTopFeatures() == ippCPUID_SSE42)
        ပြန်ပေး false;
#endif

    Ipp32s  count = 0;
    ကိန်းပြည့်     depth = src.depth();

    အကယ်၍(src.dims <= 2)
    {
        IppStatus status;
        IppiSize  size = {src.cols*src.channels(), src.rows};

        အကယ်၍(depth == CV_8U)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, (const Ipp8u *)src.ptr(), (ကိန်းပြည့်)src.step, size, &count, 0, 0);
        မဟုတ်ပါက အကယ်၍(depth == CV_32F)
            status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, (const Ipp32f *)src.ptr(), (ကိန်းပြည့်)src.step, size, &count, 0, 0);
        မဟုတ်ပါက
            ပြန်ပေး false;

        အကယ်၍(status < 0)
            ပြန်ပေး false;

        res = size.width*size.height - count;
    }
    မဟုတ်ပါက
    {
        IppStatus       status;
        const Mat      *arrays[] = {&src, NULL};
        Mat            planes[1];
        NAryMatIterator it(arrays, planes, 1);
        IppiSize        size  = {(ကိန်းပြည့်)it.size*src.channels(), 1};
        res = 0;
        အတွက် (size_t i = 0; i < it.nplanes; i++, ++it)
        {
            အကယ်၍(depth == CV_8U)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_8u_C1R, it.planes->ptr<Ipp8u>(), (ကိန်းပြည့်)it.planes->step, size, &count, 0, 0);
            မဟုတ်ပါက အကယ်၍(depth == CV_32F)
                status = CV_INSTRUMENT_FUN_IPP(ippiCountInRange_32f_C1R, it.planes->ptr<Ipp32f>(), (ကိန်းပြည့်)it.planes->step, size, &count, 0, 0);
            မဟုတ်ပါက
                ပြန်ပေး false;

            အကယ်၍(status < 0 || (ကိန်းပြည့်)it.planes->total()*src.channels() < count)
                ပြန်ပေး false;

            res += (ကိန်းပြည့်)it.planes->total()*src.channels() - count;
        }
    }

    ပြန်ပေး true;
}
#endif

ကိန်းပြည့် countNonZero(InputArray _src)
{
    CV_INSTRUMENT_REGION();

    ကိန်းပြည့် type = _src.type(), cn = CV_MAT_CN(type);
    CV_Assert( cn == 1 );

#အကယ်၍ defined HAVE_OPENCL || defined HAVE_IPP
    ကိန်းပြည့် res = -1;
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
    ကိန်းပြည့် total = (ကိန်းပြည့်)it.size, nz = 0;

    အတွက်( size_t i = 0; i < it.nplanes; i++, ++it )
        nz += func( ptrs[0], total );

    ပြန်ပေး nz;
}

void findNonZero(InputArray _src, OutputArray _idx)
{
    Mat src = _src.getMat();
    CV_Assert( src.channels() == 1 && src.dims == 2 );

    ကိန်းပြည့် depth = src.depth();
    std::vector<Point> idxvec;
    ကိန်းပြည့် rows = src.rows, cols = src.cols;
    AutoBuffer<ကိန်းပြည့်> buf_(cols + 1);
    ကိန်းပြည့်* buf = buf_.data();

    အတွက်( ကိန်းပြည့် i = 0; i < rows; i++ )
    {
        ကိန်းပြည့် j, k = 0;
        const uchar* ptr8 = src.ptr(i);
        အကယ်၍( depth == CV_8U || depth == CV_8S || depth == CV_Bool || depth == CV_8F_E4M3FNUZ )
        {
            j = 0;
#အကယ်၍ (CV_SIMD || CV_SIMD_SCALABLE)
            // skip whole SIMD blocks of zeros; only scan blocks that contain a non-zero byte
            const ကိန်းပြည့် vlanes = VTraits<v_uint8>::vlanes();
            const v_uint8 vzero = vx_setzero_u8();
            အတွက်( ; j <= cols - vlanes; j += vlanes )
            {
                အကယ်၍( !v_check_any(v_ne(vx_load(ptr8 + j), vzero)) )
                    continue;
                အတွက်( ကိန်းပြည့် t = j; t < j + vlanes; t++ )
                    အကယ်၍( ptr8[t] != 0 ) buf[k++] = t;
            }
#endif
            အတွက်( ; j < cols; j++ )
                အကယ်၍( ptr8[j] != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက အကယ်၍( depth == CV_16U || depth == CV_16S )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            အတွက်( j = 0; j < cols; j++ )
                အကယ်၍( ptr16[j] != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက အကယ်၍( depth == CV_32S || depth == CV_32U )
        {
            const ကိန်းပြည့်* ptr32s = (const ကိန်းပြည့်*)ptr8;
            အတွက်( j = 0; j < cols; j++ )
                အကယ်၍( ptr32s[j] != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက အကယ်၍( depth == CV_64S || depth == CV_64U )
        {
            const int64* ptr64s = (const int64*)ptr8;
            အတွက်( j = 0; j < cols; j++ )
                အကယ်၍( ptr64s[j] != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက အကယ်၍( depth == CV_32F )
        {
            const ကိန်းပြည့်* ptr32s = (const ကိန်းပြည့်*)ptr8;
            အတွက်( j = 0; j < cols; j++ )
                အကယ်၍( (ptr32s[j]<<1) != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက အကယ်၍( depth == CV_16F || depth == CV_16BF )
        {
            const ushort* ptr16 = (const ushort*)ptr8;
            အတွက်( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x8000) is treated as zero; cannot use <<1 here
                // because ushort is promoted to int and the sign bit would not be discarded
                အကယ်၍( (ptr16[j] & 0x7fff) != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက အကယ်၍( depth == CV_8F_E4M3FN )
        {
            အတွက်( j = 0; j < cols; j++ )
                // mask sign bit so -0 (0x80) is treated as zero
                အကယ်၍( (ptr8[j] & 0x7f) != 0 ) buf[k++] = j;
        }
        မဟုတ်ပါက
        {
            const int64* ptr64s = (const int64*)ptr8;
            အတွက်( j = 0; j < cols; j++ )
                အကယ်၍( (ptr64s[j]<<1) != 0 ) buf[k++] = j;
        }

        အကယ်၍( k > 0 )
        {
            size_t sz = idxvec.size();
            idxvec.resize(sz + k);
            အတွက်( j = 0; j < k; j++ )
                idxvec[sz + j] = Point(buf[j], i);
        }
    }

    အကယ်၍( idxvec.empty() || (_idx.kind() == _InputArray::MAT && !_idx.getMatRef().isContinuous()) )
        _idx.release();

    အကယ်၍( !idxvec.empty() )
        Mat(idxvec).copyTo(_idx);
}

} // namespace
