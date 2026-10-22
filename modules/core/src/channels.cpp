// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#അന്തര്ഭാവ "precomp.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"
#അന്തര്ഭാവ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

നാമാകാശ cv
{

template<typename T> സ്ഥിതം ശൂന്യം
mixChannels_( സ്ഥിരം T** src, സ്ഥിരം പൂർണ്ണസംഖ്യ* sdelta,
              T** dst, സ്ഥിരം പൂർണ്ണസംഖ്യ* ddelta,
              പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ npairs )
{
    പൂർണ്ണസംഖ്യ i, k;
    വേണ്ടി( k = 0; k < npairs; k++ )
    {
        സ്ഥിരം T* s = src[k];
        T* d = dst[k];
        പൂർണ്ണസംഖ്യ ds = sdelta[k], dd = ddelta[k];
        എങ്കിൽ( s )
        {
            വേണ്ടി( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            എങ്കിൽ( i < len )
                d[0] = s[0];
        }
        അല്ലെങ്കിൽ
        {
            വേണ്ടി( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            എങ്കിൽ( i < len )
                d[0] = 0;
        }
    }
}


സ്ഥിതം ശൂന്യം mixChannels8u( സ്ഥിരം ശൂന്യം** src, സ്ഥിരം പൂർണ്ണസംഖ്യ* sdelta,
                           ശൂന്യം** dst, സ്ഥിരം പൂർണ്ണസംഖ്യ* ddelta,
                           പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ npairs )
{
    mixChannels_((സ്ഥിരം uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

സ്ഥിതം ശൂന്യം mixChannels16u( സ്ഥിരം ശൂന്യം** src, സ്ഥിരം പൂർണ്ണസംഖ്യ* sdelta,
                            ശൂന്യം** dst, സ്ഥിരം പൂർണ്ണസംഖ്യ* ddelta,
                            പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ npairs )
{
    mixChannels_((സ്ഥിരം ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

സ്ഥിതം ശൂന്യം mixChannels32s( സ്ഥിരം ശൂന്യം** src, സ്ഥിരം പൂർണ്ണസംഖ്യ* sdelta,
                            ശൂന്യം** dst, സ്ഥിരം പൂർണ്ണസംഖ്യ* ddelta,
                            പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ npairs )
{
    mixChannels_((സ്ഥിരം പൂർണ്ണസംഖ്യ**)src, sdelta, (പൂർണ്ണസംഖ്യ**)dst, ddelta, len, npairs);
}

സ്ഥിതം ശൂന്യം mixChannels64s( സ്ഥിരം ശൂന്യം** src, സ്ഥിരം പൂർണ്ണസംഖ്യ* sdelta,
                            ശൂന്യം** dst, സ്ഥിരം പൂർണ്ണസംഖ്യ* ddelta,
                            പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ npairs )
{
    mixChannels_((സ്ഥിരം int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

തരനിർവചനം ശൂന്യം (*MixChannelsFunc)( സ്ഥിരം ശൂന്യം** src, സ്ഥിരം പൂർണ്ണസംഖ്യ* sdelta,
        ശൂന്യം** dst, സ്ഥിരം പൂർണ്ണസംഖ്യ* ddelta, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ npairs );

സ്ഥിതം MixChannelsFunc getMixchFunc(പൂർണ്ണസംഖ്യ depth)
{
    സ്ഥിതം MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    തിരികെ mixchTab[depth];
}

} // cv::


ശൂന്യം cv::mixChannels( സ്ഥിരം Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, സ്ഥിരം പൂർണ്ണസംഖ്യ* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ( npairs == 0 )
        തിരികെ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    പൂർണ്ണസംഖ്യ depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(വലുപ്പം(Mat*) + വലുപ്പം(uchar*)) + npairs*(വലുപ്പം(uchar*)*2 + വലുപ്പം(പൂർണ്ണസംഖ്യ)*6));
    സ്ഥിരം Mat** arrays = (സ്ഥിരം Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    സ്ഥിരം uchar** srcs = (സ്ഥിരം uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    പൂർണ്ണസംഖ്യ* tab = (പൂർണ്ണസംഖ്യ*)(dsts + npairs);
    പൂർണ്ണസംഖ്യ *sdelta = (പൂർണ്ണസംഖ്യ*)(tab + npairs*4), *ddelta = sdelta + npairs;

    വേണ്ടി( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    വേണ്ടി( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    വേണ്ടി( i = 0; i < npairs; i++ )
    {
        പൂർണ്ണസംഖ്യ i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        എങ്കിൽ( i0 >= 0 )
        {
            വേണ്ടി( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                എങ്കിൽ( i0 < src[j].channels() )
                    നിർത്തുക;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (പൂർണ്ണസംഖ്യ)j; tab[i*4+1] = (പൂർണ്ണസംഖ്യ)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        അല്ലെങ്കിൽ
        {
            tab[i*4] = (പൂർണ്ണസംഖ്യ)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        വേണ്ടി( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            എങ്കിൽ( i1 < dst[j].channels() )
                നിർത്തുക;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (പൂർണ്ണസംഖ്യ)(j + nsrcs); tab[i*4+3] = (പൂർണ്ണസംഖ്യ)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (പൂർണ്ണസംഖ്യ)(nsrcs + ndsts));
    പൂർണ്ണസംഖ്യ total = (പൂർണ്ണസംഖ്യ)it.size, blocksize = std::min(total, (പൂർണ്ണസംഖ്യ)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    വേണ്ടി( i = 0; i < it.nplanes; i++, ++it )
    {
        വേണ്ടി( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        വേണ്ടി( പൂർണ്ണസംഖ്യ t = 0; t < total; t += blocksize )
        {
            പൂർണ്ണസംഖ്യ bsz = std::min(total - t, blocksize);
            func( (സ്ഥിരം ശൂന്യം**)srcs, sdelta, (ശൂന്യം **)dsts, ddelta, bsz, (പൂർണ്ണസംഖ്യ)npairs );

            എങ്കിൽ( t + blocksize < total )
                വേണ്ടി( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

നാമാകാശ cv {

സ്ഥിതം ശൂന്യം getUMatIndex(സ്ഥിരം std::vector<UMat> & um, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ & idx, പൂർണ്ണസംഖ്യ & cnidx)
{
    പൂർണ്ണസംഖ്യ totalChannels = 0;
    വേണ്ടി (size_t i = 0, size = um.size(); i < size; ++i)
    {
        പൂർണ്ണസംഖ്യ ccn = um[i].channels();
        totalChannels += ccn;

        എങ്കിൽ (totalChannels == cn)
        {
            idx = (പൂർണ്ണസംഖ്യ)(i + 1);
            cnidx = 0;
            തിരികെ;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (totalChannels > cn)
        {
            idx = (പൂർണ്ണസംഖ്യ)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            തിരികെ;
        }
    }

    idx = cnidx = -1;
}

സ്ഥിതം തർക്കം ocl_mixChannels(നിവേശം_Array_Of_Arrays _src, നിവേശം_Output_Array_Of_Arrays _dst,
                            സ്ഥിരം പൂർണ്ണസംഖ്യ* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    പൂർണ്ണസംഖ്യ depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    വേണ്ടി (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    വേണ്ടി (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    വേണ്ടി (size_t i = 0; i < npairs; ++i)
    {
        പൂർണ്ണസംഖ്യ scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        പൂർണ്ണസംഖ്യ src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    പൂർണ്ണസംഖ്യ argindex = 0;
    വേണ്ടി (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    വേണ്ടി (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    തിരികെ k.run(2, globalsize, ശൂന്യ, അസത്യം);
}

}

#endif

ശൂന്യം cv::mixChannels(നിവേശം_Array_Of_Arrays src, നിവേശം_Output_Array_Of_Arrays dst,
                 സ്ഥിരം പൂർണ്ണസംഖ്യ* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (npairs == 0 || fromTo == ശൂന്യ)
        തിരികെ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    തർക്കം src_is_mat = src.kind() != നിവേശം_Array_2::STD_VECTOR_MAT &&
            src.kind() != നിവേശം_Array_2::STD_ARRAY_MAT &&
            src.kind() != നിവേശം_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != നിവേശം_Array_2::STD_VECTOR_UMAT;
    തർക്കം dst_is_mat = dst.kind() != നിവേശം_Array_2::STD_VECTOR_MAT &&
            dst.kind() != നിവേശം_Array_2::STD_ARRAY_MAT &&
            dst.kind() != നിവേശം_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != നിവേശം_Array_2::STD_VECTOR_UMAT;
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ nsrc = src_is_mat ? 1 : (പൂർണ്ണസംഖ്യ)src.total();
    പൂർണ്ണസംഖ്യ ndst = dst_is_mat ? 1 : (പൂർണ്ണസംഖ്യ)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    വേണ്ടി( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    വേണ്ടി( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

ശൂന്യം cv::mixChannels(നിവേശം_Array_Of_Arrays src, നിവേശം_Output_Array_Of_Arrays dst,
                     സ്ഥിരം std::vector<പൂർണ്ണസംഖ്യ>& fromTo)
{
    CV_INSTRUMENT_REGION();

    എങ്കിൽ (fromTo.empty())
        തിരികെ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    തർക്കം src_is_mat = src.kind() != നിവേശം_Array_2::STD_VECTOR_MAT &&
            src.kind() != നിവേശം_Array_2::STD_ARRAY_MAT &&
            src.kind() != നിവേശം_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != നിവേശം_Array_2::STD_VECTOR_UMAT;
    തർക്കം dst_is_mat = dst.kind() != നിവേശം_Array_2::STD_VECTOR_MAT &&
            dst.kind() != നിവേശം_Array_2::STD_ARRAY_MAT &&
            dst.kind() != നിവേശം_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != നിവേശം_Array_2::STD_VECTOR_UMAT;
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ nsrc = src_is_mat ? 1 : (പൂർണ്ണസംഖ്യ)src.total();
    പൂർണ്ണസംഖ്യ ndst = dst_is_mat ? 1 : (പൂർണ്ണസംഖ്യ)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    വേണ്ടി( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    വേണ്ടി( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

നാമാകാശ cv
{
സ്ഥിതം തർക്കം ipp_extractChannel(സ്ഥിരം Mat &src, Mat &dst, പൂർണ്ണസംഖ്യ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    പൂർണ്ണസംഖ്യ srcChannels = src.channels();
    പൂർണ്ണസംഖ്യ dstChannels = dst.channels();

    എങ്കിൽ(src.dims != dst.dims)
        തിരികെ അസത്യം;

    എങ്കിൽ(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        തിരികെ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (പൂർണ്ണസംഖ്യ)src.step, srcChannels, channel, dst.ptr(), (പൂർണ്ണസംഖ്യ)dst.step, dstChannels, 0, size, (പൂർണ്ണസംഖ്യ)src.elemSize1()) >= 0;
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat      *arrays[] = {&dst, ശൂന്യ};
        uchar          *ptrs[2]  = {ശൂന്യ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(പൂർണ്ണസംഖ്യ)it.size, 1};

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (പൂർണ്ണസംഖ്യ)src.elemSize1()) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    തിരികെ അസത്യം;
#endif
}

സ്ഥിതം തർക്കം ipp_തിരുകുക_Channel(സ്ഥിരം Mat &src, Mat &dst, പൂർണ്ണസംഖ്യ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    പൂർണ്ണസംഖ്യ srcChannels = src.channels();
    പൂർണ്ണസംഖ്യ dstChannels = dst.channels();

    എങ്കിൽ(src.dims != dst.dims)
        തിരികെ അസത്യം;

    എങ്കിൽ(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        തിരികെ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (പൂർണ്ണസംഖ്യ)src.step, srcChannels, 0, dst.ptr(), (പൂർണ്ണസംഖ്യ)dst.step, dstChannels, channel, size, (പൂർണ്ണസംഖ്യ)src.elemSize1()) >= 0;
    }
    അല്ലെങ്കിൽ
    {
        സ്ഥിരം Mat      *arrays[] = {&dst, ശൂന്യ};
        uchar          *ptrs[2]  = {ശൂന്യ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(പൂർണ്ണസംഖ്യ)it.size, 1};

        വേണ്ടി( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            എങ്കിൽ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (പൂർണ്ണസംഖ്യ)src.elemSize1()) < 0)
                തിരികെ അസത്യം;
        }
        തിരികെ സത്യം;
    }
#അല്ലെങ്കിൽ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    തിരികെ അസത്യം;
#endif
}
}
#endif

ശൂന്യം cv::extractChannel(നിവേശം_Array _src, OutputArray _dst, പൂർണ്ണസംഖ്യ coi)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    പൂർണ്ണസംഖ്യ ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    എങ്കിൽ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        തിരികെ;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

ശൂന്യം cv::തിരുകുക_Channel(നിവേശം_Array _src, നിവേശം_Output_Array _dst, പൂർണ്ണസംഖ്യ coi)
{
    CV_INSTRUMENT_REGION();

    പൂർണ്ണസംഖ്യ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    പൂർണ്ണസംഖ്യ dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    പൂർണ്ണസംഖ്യ ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    എങ്കിൽ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        തിരികെ;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_തിരുകുക_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
