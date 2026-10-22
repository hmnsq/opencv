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
mixChannels_( const T** src, const ཧྲིལ* sdelta,
              T** dst, const ཧྲིལ* ddelta,
              ཧྲིལ len, ཧྲིལ npairs )
{
    ཧྲིལ i, k;
    དོན་ལུ( k = 0; k < npairs; k++ )
    {
        const T* s = src[k];
        T* d = dst[k];
        ཧྲིལ ds = sdelta[k], dd = ddelta[k];
        གལ་སྲིད( s )
        {
            དོན་ལུ( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            གལ་སྲིད( i < len )
                d[0] = s[0];
        }
        དེ་མིན
        {
            དོན་ལུ( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            གལ་སྲིད( i < len )
                d[0] = 0;
        }
    }
}


static void mixChannels8u( const void** src, const ཧྲིལ* sdelta,
                           void** dst, const ཧྲིལ* ddelta,
                           ཧྲིལ len, ཧྲིལ npairs )
{
    mixChannels_((const uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

static void mixChannels16u( const void** src, const ཧྲིལ* sdelta,
                            void** dst, const ཧྲིལ* ddelta,
                            ཧྲིལ len, ཧྲིལ npairs )
{
    mixChannels_((const ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

static void mixChannels32s( const void** src, const ཧྲིལ* sdelta,
                            void** dst, const ཧྲིལ* ddelta,
                            ཧྲིལ len, ཧྲིལ npairs )
{
    mixChannels_((const ཧྲིལ**)src, sdelta, (ཧྲིལ**)dst, ddelta, len, npairs);
}

static void mixChannels64s( const void** src, const ཧྲིལ* sdelta,
                            void** dst, const ཧྲིལ* ddelta,
                            ཧྲིལ len, ཧྲིལ npairs )
{
    mixChannels_((const int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

typedef void (*MixChannelsFunc)( const void** src, const ཧྲིལ* sdelta,
        void** dst, const ཧྲིལ* ddelta, ཧྲིལ len, ཧྲིལ npairs );

static MixChannelsFunc getMixchFunc(ཧྲིལ depth)
{
    static MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ལོག mixchTab[depth];
}

} // cv::


void cv::mixChannels( const Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, const ཧྲིལ* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    གལ་སྲིད( npairs == 0 )
        ལོག;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    ཧྲིལ depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(parimaa_na(Mat*) + parimaa_na(uchar*)) + npairs*(parimaa_na(uchar*)*2 + parimaa_na(ཧྲིལ)*6));
    const Mat** arrays = (const Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    const uchar** srcs = (const uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    ཧྲིལ* tab = (ཧྲིལ*)(dsts + npairs);
    ཧྲིལ *sdelta = (ཧྲིལ*)(tab + npairs*4), *ddelta = sdelta + npairs;

    དོན་ལུ( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    དོན་ལུ( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    དོན་ལུ( i = 0; i < npairs; i++ )
    {
        ཧྲིལ i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        གལ་སྲིད( i0 >= 0 )
        {
            དོན་ལུ( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                གལ་སྲིད( i0 < src[j].channels() )
                    bha_nga;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (ཧྲིལ)j; tab[i*4+1] = (ཧྲིལ)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        དེ་མིན
        {
            tab[i*4] = (ཧྲིལ)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        དོན་ལུ( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            གལ་སྲིད( i1 < dst[j].channels() )
                bha_nga;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (ཧྲིལ)(j + nsrcs); tab[i*4+3] = (ཧྲིལ)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (ཧྲིལ)(nsrcs + ndsts));
    ཧྲིལ total = (ཧྲིལ)it.size, blocksize = std::min(total, (ཧྲིལ)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    དོན་ལུ( i = 0; i < it.nplanes; i++, ++it )
    {
        དོན་ལུ( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        དོན་ལུ( ཧྲིལ t = 0; t < total; t += blocksize )
        {
            ཧྲིལ bsz = std::min(total - t, blocksize);
            func( (const void**)srcs, sdelta, (void **)dsts, ddelta, bsz, (ཧྲིལ)npairs );

            གལ་སྲིད( t + blocksize < total )
                དོན་ལུ( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

namespace cv {

static void getUMatIndex(const std::vector<UMat> & um, ཧྲིལ cn, ཧྲིལ & idx, ཧྲིལ & cnidx)
{
    ཧྲིལ totalChannels = 0;
    དོན་ལུ (size_t i = 0, size = um.size(); i < size; ++i)
    {
        ཧྲིལ ccn = um[i].channels();
        totalChannels += ccn;

        གལ་སྲིད (totalChannels == cn)
        {
            idx = (ཧྲིལ)(i + 1);
            cnidx = 0;
            ལོག;
        }
        དེ་མིན གལ་སྲིད (totalChannels > cn)
        {
            idx = (ཧྲིལ)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            ལོག;
        }
    }

    idx = cnidx = -1;
}

static bool ocl_mixChannels(InputArrayOfArrays _src, InputOutputArrayOfArrays _dst,
                            const ཧྲིལ* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    ཧྲིལ depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    དོན་ལུ (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    དོན་ལུ (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    དོན་ལུ (size_t i = 0; i < npairs; ++i)
    {
        ཧྲིལ scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        ཧྲིལ src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    གལ་སྲིད (k.empty())
        ལོག false;

    ཧྲིལ argindex = 0;
    དོན་ལུ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    དོན་ལུ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ལོག k.run(2, globalsize, NULL, false);
}

}

#endif

void cv::mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                 const ཧྲིལ* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    གལ་སྲིད (npairs == 0 || fromTo == NULL)
        ལོག;

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
    ཧྲིལ i;
    ཧྲིལ nsrc = src_is_mat ? 1 : (ཧྲིལ)src.total();
    ཧྲིལ ndst = dst_is_mat ? 1 : (ཧྲིལ)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    དོན་ལུ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    དོན་ལུ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

void cv::mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                     const std::vector<ཧྲིལ>& fromTo)
{
    CV_INSTRUMENT_REGION();

    གལ་སྲིད (fromTo.empty())
        ལོག;

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
    ཧྲིལ i;
    ཧྲིལ nsrc = src_is_mat ? 1 : (ཧྲིལ)src.total();
    ཧྲིལ ndst = dst_is_mat ? 1 : (ཧྲིལ)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    དོན་ལུ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    དོན་ལུ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

namespace cv
{
static bool ipp_extractChannel(const Mat &src, Mat &dst, ཧྲིལ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ཧྲིལ srcChannels = src.channels();
    ཧྲིལ dstChannels = dst.channels();

    གལ་སྲིད(src.dims != dst.dims)
        ལོག false;

    གལ་སྲིད(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ལོག CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ཧྲིལ)src.step, srcChannels, channel, dst.ptr(), (ཧྲིལ)dst.step, dstChannels, 0, size, (ཧྲིལ)src.elemSize1()) >= 0;
    }
    དེ་མིན
    {
        const Mat      *arrays[] = {&dst, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ཧྲིལ)it.size, 1};

        དོན་ལུ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            གལ་སྲིད(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (ཧྲིལ)src.elemSize1()) < 0)
                ལོག false;
        }
        ལོག true;
    }
#དེ་མིན
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ལོག false;
#endif
}

static bool ipp_insertChannel(const Mat &src, Mat &dst, ཧྲིལ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ཧྲིལ srcChannels = src.channels();
    ཧྲིལ dstChannels = dst.channels();

    གལ་སྲིད(src.dims != dst.dims)
        ལོག false;

    གལ་སྲིད(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ལོག CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ཧྲིལ)src.step, srcChannels, 0, dst.ptr(), (ཧྲིལ)dst.step, dstChannels, channel, size, (ཧྲིལ)src.elemSize1()) >= 0;
    }
    དེ་མིན
    {
        const Mat      *arrays[] = {&dst, NULL};
        uchar          *ptrs[2]  = {NULL};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ཧྲིལ)it.size, 1};

        དོན་ལུ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            གལ་སྲིད(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (ཧྲིལ)src.elemSize1()) < 0)
                ལོག false;
        }
        ལོག true;
    }
#དེ་མིན
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ལོག false;
#endif
}
}
#endif

void cv::extractChannel(InputArray _src, OutputArray _dst, ཧྲིལ coi)
{
    CV_INSTRUMENT_REGION();

    ཧྲིལ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    ཧྲིལ ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    གལ་སྲིད (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ལོག;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

void cv::insertChannel(InputArray _src, InputOutputArray _dst, ཧྲིལ coi)
{
    CV_INSTRUMENT_REGION();

    ཧྲིལ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    ཧྲིལ dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    ཧྲིལ ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    གལ་སྲིད (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ལོག;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_insertChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
