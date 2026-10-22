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
mixChannels_( const T** src, const ຈຳນວນເຕັມ* sdelta,
              T** dst, const ຈຳນວນເຕັມ* ddelta,
              ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ npairs )
{
    ຈຳນວນເຕັມ i, k;
    ສຳລັບ( k = 0; k < npairs; k++ )
    {
        const T* s = src[k];
        T* d = dst[k];
        ຈຳນວນເຕັມ ds = sdelta[k], dd = ddelta[k];
        ຖ້າ( s )
        {
            ສຳລັບ( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            ຖ້າ( i < len )
                d[0] = s[0];
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ສຳລັບ( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            ຖ້າ( i < len )
                d[0] = 0;
        }
    }
}


static void mixChannels8u( const void** src, const ຈຳນວນເຕັມ* sdelta,
                           void** dst, const ຈຳນວນເຕັມ* ddelta,
                           ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ npairs )
{
    mixChannels_((const uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

static void mixChannels16u( const void** src, const ຈຳນວນເຕັມ* sdelta,
                            void** dst, const ຈຳນວນເຕັມ* ddelta,
                            ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ npairs )
{
    mixChannels_((const ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

static void mixChannels32s( const void** src, const ຈຳນວນເຕັມ* sdelta,
                            void** dst, const ຈຳນວນເຕັມ* ddelta,
                            ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ npairs )
{
    mixChannels_((const ຈຳນວນເຕັມ**)src, sdelta, (ຈຳນວນເຕັມ**)dst, ddelta, len, npairs);
}

static void mixChannels64s( const void** src, const ຈຳນວນເຕັມ* sdelta,
                            void** dst, const ຈຳນວນເຕັມ* ddelta,
                            ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ npairs )
{
    mixChannels_((const int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

typedef void (*MixChannelsFunc)( const void** src, const ຈຳນວນເຕັມ* sdelta,
        void** dst, const ຈຳນວນເຕັມ* ddelta, ຈຳນວນເຕັມ len, ຈຳນວນເຕັມ npairs );

static MixChannelsFunc getMixchFunc(ຈຳນວນເຕັມ depth)
{
    static MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ສົ່ງຄືນ mixchTab[depth];
}

} // cv::


void cv::mixChannels( const Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, const ຈຳນວນເຕັມ* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    ຖ້າ( npairs == 0 )
        ສົ່ງຄືນ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    ຈຳນວນເຕັມ depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(parimaa_na(Mat*) + parimaa_na(uchar*)) + npairs*(parimaa_na(uchar*)*2 + parimaa_na(ຈຳນວນເຕັມ)*6));
    const Mat** arrays = (const Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    const uchar** srcs = (const uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    ຈຳນວນເຕັມ* tab = (ຈຳນວນເຕັມ*)(dsts + npairs);
    ຈຳນວນເຕັມ *sdelta = (ຈຳນວນເຕັມ*)(tab + npairs*4), *ddelta = sdelta + npairs;

    ສຳລັບ( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    ສຳລັບ( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    ສຳລັບ( i = 0; i < npairs; i++ )
    {
        ຈຳນວນເຕັມ i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        ຖ້າ( i0 >= 0 )
        {
            ສຳລັບ( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                ຖ້າ( i0 < src[j].channels() )
                    bha_nga;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (ຈຳນວນເຕັມ)j; tab[i*4+1] = (ຈຳນວນເຕັມ)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            tab[i*4] = (ຈຳນວນເຕັມ)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        ສຳລັບ( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            ຖ້າ( i1 < dst[j].channels() )
                bha_nga;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (ຈຳນວນເຕັມ)(j + nsrcs); tab[i*4+3] = (ຈຳນວນເຕັມ)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (ຈຳນວນເຕັມ)(nsrcs + ndsts));
    ຈຳນວນເຕັມ total = (ຈຳນວນເຕັມ)it.size, blocksize = std::min(total, (ຈຳນວນເຕັມ)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    ສຳລັບ( i = 0; i < it.nplanes; i++, ++it )
    {
        ສຳລັບ( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        ສຳລັບ( ຈຳນວນເຕັມ t = 0; t < total; t += blocksize )
        {
            ຈຳນວນເຕັມ bsz = std::min(total - t, blocksize);
            func( (const void**)srcs, sdelta, (void **)dsts, ddelta, bsz, (ຈຳນວນເຕັມ)npairs );

            ຖ້າ( t + blocksize < total )
                ສຳລັບ( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

namespace cv {

static void getUMatIndex(const std::vector<UMat> & um, ຈຳນວນເຕັມ cn, ຈຳນວນເຕັມ & idx, ຈຳນວນເຕັມ & cnidx)
{
    ຈຳນວນເຕັມ totalChannels = 0;
    ສຳລັບ (size_t i = 0, size = um.size(); i < size; ++i)
    {
        ຈຳນວນເຕັມ ccn = um[i].channels();
        totalChannels += ccn;

        ຖ້າ (totalChannels == cn)
        {
            idx = (ຈຳນວນເຕັມ)(i + 1);
            cnidx = 0;
            ສົ່ງຄືນ;
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (totalChannels > cn)
        {
            idx = (ຈຳນວນເຕັມ)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            ສົ່ງຄືນ;
        }
    }

    idx = cnidx = -1;
}

static bool ocl_mixChannels(InputArrayOfArrays _src, InputOutputArrayOfArrays _dst,
                            const ຈຳນວນເຕັມ* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    ຈຳນວນເຕັມ depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    ສຳລັບ (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    ສຳລັບ (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    ສຳລັບ (size_t i = 0; i < npairs; ++i)
    {
        ຈຳນວນເຕັມ scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        ຈຳນວນເຕັມ src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    ຖ້າ (k.empty())
        ສົ່ງຄືນ false;

    ຈຳນວນເຕັມ argindex = 0;
    ສຳລັບ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    ສຳລັບ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ສົ່ງຄືນ k.run(2, globalsize, NULL, false);
}

}

#endif

void cv::mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                 const ຈຳນວນເຕັມ* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    ຖ້າ (npairs == 0 || fromTo == NULL)
        ສົ່ງຄືນ;

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
    ຈຳນວນເຕັມ i;
    ຈຳນວນເຕັມ nsrc = src_is_mat ? 1 : (ຈຳນວນເຕັມ)src.total();
    ຈຳນວນເຕັມ ndst = dst_is_mat ? 1 : (ຈຳນວນເຕັມ)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    ສຳລັບ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ສຳລັບ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

void cv::mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                     const std::vector<ຈຳນວນເຕັມ>& fromTo)
{
    CV_INSTRUMENT_REGION();

    ຖ້າ (fromTo.empty())
        ສົ່ງຄືນ;

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
    ຈຳນວນເຕັມ i;
    ຈຳນວນເຕັມ nsrc = src_is_mat ? 1 : (ຈຳນວນເຕັມ)src.total();
    ຈຳນວນເຕັມ ndst = dst_is_mat ? 1 : (ຈຳນວນເຕັມ)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    ສຳລັບ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ສຳລັບ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

namespace cv
{
static bool ipp_extractChannel(const Mat &src, Mat &dst, ຈຳນວນເຕັມ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ຈຳນວນເຕັມ srcChannels = src.channels();
    ຈຳນວນເຕັມ dstChannels = dst.channels();

    ຖ້າ(src.dims != dst.dims)
        ສົ່ງຄືນ false;

    ຖ້າ(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ສົ່ງຄືນ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ຈຳນວນເຕັມ)src.step, srcChannels, channel, dst.ptr(), (ຈຳນວນເຕັມ)dst.step, dstChannels, 0, size, (ຈຳນວນເຕັມ)src.elemSize1()) >= 0;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        const Mat      *arrays[] = {&dst, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ຈຳນວນເຕັມ)it.size, 1};

        ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ຖ້າ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (ຈຳນວນເຕັມ)src.elemSize1()) < 0)
                ສົ່ງຄືນ false;
        }
        ສົ່ງຄືນ true;
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ສົ່ງຄືນ false;
#endif
}

static bool ipp_insertChannel(const Mat &src, Mat &dst, ຈຳນວນເຕັມ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ຈຳນວນເຕັມ srcChannels = src.channels();
    ຈຳນວນເຕັມ dstChannels = dst.channels();

    ຖ້າ(src.dims != dst.dims)
        ສົ່ງຄືນ false;

    ຖ້າ(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ສົ່ງຄືນ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ຈຳນວນເຕັມ)src.step, srcChannels, 0, dst.ptr(), (ຈຳນວນເຕັມ)dst.step, dstChannels, channel, size, (ຈຳນວນເຕັມ)src.elemSize1()) >= 0;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        const Mat      *arrays[] = {&dst, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ຈຳນວນເຕັມ)it.size, 1};

        ສຳລັບ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ຖ້າ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (ຈຳນວນເຕັມ)src.elemSize1()) < 0)
                ສົ່ງຄືນ false;
        }
        ສົ່ງຄືນ true;
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ສົ່ງຄືນ false;
#endif
}
}
#endif

void cv::extractChannel(InputArray _src, OutputArray _dst, ຈຳນວນເຕັມ coi)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    ຈຳນວນເຕັມ ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    ຖ້າ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ສົ່ງຄືນ;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

void cv::insertChannel(InputArray _src, InputOutputArray _dst, ຈຳນວນເຕັມ coi)
{
    CV_INSTRUMENT_REGION();

    ຈຳນວນເຕັມ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    ຈຳນວນເຕັມ dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    ຈຳນວນເຕັມ ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    ຖ້າ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ສົ່ງຄືນ;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_insertChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
