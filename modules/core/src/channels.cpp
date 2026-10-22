// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#include "precomp.hpp"
#include "opencl_kernels_core.hpp"
#include "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

namespace cv
{

template<typename T> static void
mixChannels_( const T** src, const පූර්ණ_සංඛ්‍යා* sdelta,
              T** dst, const පූර්ණ_සංඛ්‍යා* ddelta,
              පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා npairs )
{
    පූර්ණ_සංඛ්‍යා i, k;
    සඳහා( k = 0; k < npairs; k++ )
    {
        const T* s = src[k];
        T* d = dst[k];
        පූර්ණ_සංඛ්‍යා ds = sdelta[k], dd = ddelta[k];
        නම්( s )
        {
            සඳහා( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            නම්( i < len )
                d[0] = s[0];
        }
        නැතහොත්
        {
            සඳහා( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            නම්( i < len )
                d[0] = 0;
        }
    }
}


static void mixChannels8u( const void** src, const පූර්ණ_සංඛ්‍යා* sdelta,
                           void** dst, const පූර්ණ_සංඛ්‍යා* ddelta,
                           පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා npairs )
{
    mixChannels_((const uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

static void mixChannels16u( const void** src, const පූර්ණ_සංඛ්‍යා* sdelta,
                            void** dst, const පූර්ණ_සංඛ්‍යා* ddelta,
                            පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා npairs )
{
    mixChannels_((const ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

static void mixChannels32s( const void** src, const පූර්ණ_සංඛ්‍යා* sdelta,
                            void** dst, const පූර්ණ_සංඛ්‍යා* ddelta,
                            පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා npairs )
{
    mixChannels_((const පූර්ණ_සංඛ්‍යා**)src, sdelta, (පූර්ණ_සංඛ්‍යා**)dst, ddelta, len, npairs);
}

static void mixChannels64s( const void** src, const පූර්ණ_සංඛ්‍යා* sdelta,
                            void** dst, const පූර්ණ_සංඛ්‍යා* ddelta,
                            පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා npairs )
{
    mixChannels_((const int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

typedef void (*MixChannelsFunc)( const void** src, const පූර්ණ_සංඛ්‍යා* sdelta,
        void** dst, const පූර්ණ_සංඛ්‍යා* ddelta, පූර්ණ_සංඛ්‍යා len, පූර්ණ_සංඛ්‍යා npairs );

static MixChannelsFunc getMixchFunc(පූර්ණ_සංඛ්‍යා depth)
{
    static MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ආපසු mixchTab[depth];
}

} // cv::


void cv::mixChannels( const Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, const පූර්ණ_සංඛ්‍යා* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    නම්( npairs == 0 )
        ආපසු;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    පූර්ණ_සංඛ්‍යා depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(parimaa_na(Mat*) + parimaa_na(uchar*)) + npairs*(parimaa_na(uchar*)*2 + parimaa_na(පූර්ණ_සංඛ්‍යා)*6));
    const Mat** arrays = (const Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    const uchar** srcs = (const uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    පූර්ණ_සංඛ්‍යා* tab = (පූර්ණ_සංඛ්‍යා*)(dsts + npairs);
    පූර්ණ_සංඛ්‍යා *sdelta = (පූර්ණ_සංඛ්‍යා*)(tab + npairs*4), *ddelta = sdelta + npairs;

    සඳහා( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    සඳහා( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    සඳහා( i = 0; i < npairs; i++ )
    {
        පූර්ණ_සංඛ්‍යා i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        නම්( i0 >= 0 )
        {
            සඳහා( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                නම්( i0 < src[j].channels() )
                    bha_nga;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (පූර්ණ_සංඛ්‍යා)j; tab[i*4+1] = (පූර්ණ_සංඛ්‍යා)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        නැතහොත්
        {
            tab[i*4] = (පූර්ණ_සංඛ්‍යා)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        සඳහා( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            නම්( i1 < dst[j].channels() )
                bha_nga;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (පූර්ණ_සංඛ්‍යා)(j + nsrcs); tab[i*4+3] = (පූර්ණ_සංඛ්‍යා)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (පූර්ණ_සංඛ්‍යා)(nsrcs + ndsts));
    පූර්ණ_සංඛ්‍යා total = (පූර්ණ_සංඛ්‍යා)it.size, blocksize = std::min(total, (පූර්ණ_සංඛ්‍යා)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    සඳහා( i = 0; i < it.nplanes; i++, ++it )
    {
        සඳහා( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        සඳහා( පූර්ණ_සංඛ්‍යා t = 0; t < total; t += blocksize )
        {
            පූර්ණ_සංඛ්‍යා bsz = std::min(total - t, blocksize);
            func( (const void**)srcs, sdelta, (void **)dsts, ddelta, bsz, (පූර්ණ_සංඛ්‍යා)npairs );

            නම්( t + blocksize < total )
                සඳහා( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

namespace cv {

static void getUMatIndex(const std::vector<UMat> & um, පූර්ණ_සංඛ්‍යා cn, පූර්ණ_සංඛ්‍යා & idx, පූර්ණ_සංඛ්‍යා & cnidx)
{
    පූර්ණ_සංඛ්‍යා totalChannels = 0;
    සඳහා (size_t i = 0, size = um.size(); i < size; ++i)
    {
        පූර්ණ_සංඛ්‍යා ccn = um[i].channels();
        totalChannels += ccn;

        නම් (totalChannels == cn)
        {
            idx = (පූර්ණ_සංඛ්‍යා)(i + 1);
            cnidx = 0;
            ආපසු;
        }
        නැතහොත් නම් (totalChannels > cn)
        {
            idx = (පූර්ණ_සංඛ්‍යා)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            ආපසු;
        }
    }

    idx = cnidx = -1;
}

static bool ocl_mixChannels(InputArrayOfArrays _src, InputOutputArrayOfArrays _dst,
                            const පූර්ණ_සංඛ්‍යා* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    පූර්ණ_සංඛ්‍යා depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    සඳහා (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    සඳහා (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    සඳහා (size_t i = 0; i < npairs; ++i)
    {
        පූර්ණ_සංඛ්‍යා scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        පූර්ණ_සංඛ්‍යා src_idx, src_cnidx, dst_idx, dst_cnidx;

        getUMatIndex(src, scn, src_idx, src_cnidx);
        getUMatIndex(dst, dcn, dst_idx, dst_cnidx);

        CV_Assert(dst_idx >= 0 && src_idx >= 0);

        srcargs[i] = src[src_idx];
        srcargs[i].offset += src_cnidx * esz;

        dstargs[i] = dst[dst_idx];
        dstargs[i].offset += dst_cnidx * esz;

        declsrc += format("DECLARE_INPUT_MAT(%zu)", i);
        decldst += format("DECLARE_OUTPUT_MAT(%zu)", i);
        indexdecl += format("DECLARE_INDEX(%zu)", i);
        declproc += format("PROCESS_ELEM(%zu)", i);
        declcn += format(" -D scn%zu=%d -D dcn%zu=%d", i, src[src_idx].channels(), i, dst[dst_idx].channels());
    }

    ocl::Kernel k("mixChannels", ocl::core::mixchannels_oclsrc,
                  format("-D T=%s -D DECLARE_INPUT_MAT_N=%s -D DECLARE_OUTPUT_MAT_N=%s"
                         " -D PROCESS_ELEM_N=%s -D DECLARE_INDEX_N=%s%s",
                         ocl::memopTypeToStr(depth), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    නම් (k.empty())
        ආපසු false;

    පූර්ණ_සංඛ්‍යා argindex = 0;
    සඳහා (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    සඳහා (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ආපසු k.run(2, globalsize, NULL, false);
}

}

#endif

void cv::mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                 const පූර්ණ_සංඛ්‍යා* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    නම් (npairs == 0 || fromTo == NULL)
        ආපසු;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    bool src_is_mat = src.kind() != _InputArray::STD_VECTOR_MAT &&
            src.kind() != _InputArray::STD_ARRAY_MAT &&
            src.kind() != _InputArray::STD_VECTOR_VECTOR &&
            src.kind() != _InputArray::STD_VECTOR_UMAT;
    bool dst_is_mat = dst.kind() != _InputArray::STD_VECTOR_MAT &&
            dst.kind() != _InputArray::STD_ARRAY_MAT &&
            dst.kind() != _InputArray::STD_VECTOR_VECTOR &&
            dst.kind() != _InputArray::STD_VECTOR_UMAT;
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා nsrc = src_is_mat ? 1 : (පූර්ණ_සංඛ්‍යා)src.total();
    පූර්ණ_සංඛ්‍යා ndst = dst_is_mat ? 1 : (පූර්ණ_සංඛ්‍යා)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    සඳහා( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    සඳහා( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

void cv::mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                     const std::vector<පූර්ණ_සංඛ්‍යා>& fromTo)
{
    CV_INSTRUMENT_REGION();

    නම් (fromTo.empty())
        ආපසු;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    bool src_is_mat = src.kind() != _InputArray::STD_VECTOR_MAT &&
            src.kind() != _InputArray::STD_ARRAY_MAT &&
            src.kind() != _InputArray::STD_VECTOR_VECTOR &&
            src.kind() != _InputArray::STD_VECTOR_UMAT;
    bool dst_is_mat = dst.kind() != _InputArray::STD_VECTOR_MAT &&
            dst.kind() != _InputArray::STD_ARRAY_MAT &&
            dst.kind() != _InputArray::STD_VECTOR_VECTOR &&
            dst.kind() != _InputArray::STD_VECTOR_UMAT;
    පූර්ණ_සංඛ්‍යා i;
    පූර්ණ_සංඛ්‍යා nsrc = src_is_mat ? 1 : (පූර්ණ_සංඛ්‍යා)src.total();
    පූර්ණ_සංඛ්‍යා ndst = dst_is_mat ? 1 : (පූර්ණ_සංඛ්‍යා)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    සඳහා( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    සඳහා( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

namespace cv
{
static bool ipp_extractChannel(const Mat &src, Mat &dst, පූර්ණ_සංඛ්‍යා channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    පූර්ණ_සංඛ්‍යා srcChannels = src.channels();
    පූර්ණ_සංඛ්‍යා dstChannels = dst.channels();

    නම්(src.dims != dst.dims)
        ආපසු false;

    නම්(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ආපසු CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (පූර්ණ_සංඛ්‍යා)src.step, srcChannels, channel, dst.ptr(), (පූර්ණ_සංඛ්‍යා)dst.step, dstChannels, 0, size, (පූර්ණ_සංඛ්‍යා)src.elemSize1()) >= 0;
    }
    නැතහොත්
    {
        const Mat      *arrays[] = {&dst, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(පූර්ණ_සංඛ්‍යා)it.size, 1};

        සඳහා( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            නම්(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (පූර්ණ_සංඛ්‍යා)src.elemSize1()) < 0)
                ආපසු false;
        }
        ආපසු true;
    }
#නැතහොත්
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ආපසු false;
#endif
}

static bool ipp_insertChannel(const Mat &src, Mat &dst, පූර්ණ_සංඛ්‍යා channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    පූර්ණ_සංඛ්‍යා srcChannels = src.channels();
    පූර්ණ_සංඛ්‍යා dstChannels = dst.channels();

    නම්(src.dims != dst.dims)
        ආපසු false;

    නම්(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ආපසු CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (පූර්ණ_සංඛ්‍යා)src.step, srcChannels, 0, dst.ptr(), (පූර්ණ_සංඛ්‍යා)dst.step, dstChannels, channel, size, (පූර්ණ_සංඛ්‍යා)src.elemSize1()) >= 0;
    }
    නැතහොත්
    {
        const Mat      *arrays[] = {&dst, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(පූර්ණ_සංඛ්‍යා)it.size, 1};

        සඳහා( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            නම්(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (පූර්ණ_සංඛ්‍යා)src.elemSize1()) < 0)
                ආපසු false;
        }
        ආපසු true;
    }
#නැතහොත්
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ආපසු false;
#endif
}
}
#endif

void cv::extractChannel(InputArray _src, OutputArray _dst, පූර්ණ_සංඛ්‍යා coi)
{
    CV_INSTRUMENT_REGION();

    පූර්ණ_සංඛ්‍යා type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    පූර්ණ_සංඛ්‍යා ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    නම් (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ආපසු;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

void cv::insertChannel(InputArray _src, InputOutputArray _dst, පූර්ණ_සංඛ්‍යා coi)
{
    CV_INSTRUMENT_REGION();

    පූර්ණ_සංඛ්‍යා stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    පූර්ණ_සංඛ්‍යා dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    පූර්ණ_සංඛ්‍යා ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    නම් (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ආපසු;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_insertChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
