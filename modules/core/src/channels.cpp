// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#અન્તર્ભાવ "precomp.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"
#અન્તર્ભાવ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

નામાકાશ cv
{

template<typename T> સ્થિર શૂન્ય
mixChannels_( અચલ T** src, અચલ પૂર્ણાંક* sdelta,
              T** dst, અચલ પૂર્ણાંક* ddelta,
              પૂર્ણાંક len, પૂર્ણાંક npairs )
{
    પૂર્ણાંક i, k;
    માટે( k = 0; k < npairs; k++ )
    {
        અચલ T* s = src[k];
        T* d = dst[k];
        પૂર્ણાંક ds = sdelta[k], dd = ddelta[k];
        જો( s )
        {
            માટે( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            જો( i < len )
                d[0] = s[0];
        }
        નહીંતર
        {
            માટે( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            જો( i < len )
                d[0] = 0;
        }
    }
}


સ્થિર શૂન્ય mixChannels8u( અચલ શૂન્ય** src, અચલ પૂર્ણાંક* sdelta,
                           શૂન્ય** dst, અચલ પૂર્ણાંક* ddelta,
                           પૂર્ણાંક len, પૂર્ણાંક npairs )
{
    mixChannels_((અચલ uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

સ્થિર શૂન્ય mixChannels16u( અચલ શૂન્ય** src, અચલ પૂર્ણાંક* sdelta,
                            શૂન્ય** dst, અચલ પૂર્ણાંક* ddelta,
                            પૂર્ણાંક len, પૂર્ણાંક npairs )
{
    mixChannels_((અચલ ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

સ્થિર શૂન્ય mixChannels32s( અચલ શૂન્ય** src, અચલ પૂર્ણાંક* sdelta,
                            શૂન્ય** dst, અચલ પૂર્ણાંક* ddelta,
                            પૂર્ણાંક len, પૂર્ણાંક npairs )
{
    mixChannels_((અચલ પૂર્ણાંક**)src, sdelta, (પૂર્ણાંક**)dst, ddelta, len, npairs);
}

સ્થિર શૂન્ય mixChannels64s( અચલ શૂન્ય** src, અચલ પૂર્ણાંક* sdelta,
                            શૂન્ય** dst, અચલ પૂર્ણાંક* ddelta,
                            પૂર્ણાંક len, પૂર્ણાંક npairs )
{
    mixChannels_((અચલ int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

પ્રકારવ્યાખ્યા શૂન્ય (*MixChannelsFunc)( અચલ શૂન્ય** src, અચલ પૂર્ણાંક* sdelta,
        શૂન્ય** dst, અચલ પૂર્ણાંક* ddelta, પૂર્ણાંક len, પૂર્ણાંક npairs );

સ્થિર MixChannelsFunc getMixchFunc(પૂર્ણાંક depth)
{
    સ્થિર MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    પાછા mixchTab[depth];
}

} // cv::


શૂન્ય cv::mixChannels( અચલ Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, અચલ પૂર્ણાંક* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    જો( npairs == 0 )
        પાછા;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    પૂર્ણાંક depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(કદ(Mat*) + કદ(uchar*)) + npairs*(કદ(uchar*)*2 + કદ(પૂર્ણાંક)*6));
    અચલ Mat** arrays = (અચલ Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    અચલ uchar** srcs = (અચલ uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    પૂર્ણાંક* tab = (પૂર્ણાંક*)(dsts + npairs);
    પૂર્ણાંક *sdelta = (પૂર્ણાંક*)(tab + npairs*4), *ddelta = sdelta + npairs;

    માટે( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    માટે( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    માટે( i = 0; i < npairs; i++ )
    {
        પૂર્ણાંક i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        જો( i0 >= 0 )
        {
            માટે( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                જો( i0 < src[j].channels() )
                    તોડો;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (પૂર્ણાંક)j; tab[i*4+1] = (પૂર્ણાંક)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        નહીંતર
        {
            tab[i*4] = (પૂર્ણાંક)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        માટે( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            જો( i1 < dst[j].channels() )
                તોડો;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (પૂર્ણાંક)(j + nsrcs); tab[i*4+3] = (પૂર્ણાંક)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (પૂર્ણાંક)(nsrcs + ndsts));
    પૂર્ણાંક total = (પૂર્ણાંક)it.size, blocksize = std::min(total, (પૂર્ણાંક)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    માટે( i = 0; i < it.nplanes; i++, ++it )
    {
        માટે( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        માટે( પૂર્ણાંક t = 0; t < total; t += blocksize )
        {
            પૂર્ણાંક bsz = std::min(total - t, blocksize);
            func( (અચલ શૂન્ય**)srcs, sdelta, (શૂન્ય **)dsts, ddelta, bsz, (પૂર્ણાંક)npairs );

            જો( t + blocksize < total )
                માટે( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

નામાકાશ cv {

સ્થિર શૂન્ય getUMatIndex(અચલ std::vector<UMat> & um, પૂર્ણાંક cn, પૂર્ણાંક & idx, પૂર્ણાંક & cnidx)
{
    પૂર્ણાંક totalChannels = 0;
    માટે (size_t i = 0, size = um.size(); i < size; ++i)
    {
        પૂર્ણાંક ccn = um[i].channels();
        totalChannels += ccn;

        જો (totalChannels == cn)
        {
            idx = (પૂર્ણાંક)(i + 1);
            cnidx = 0;
            પાછા;
        }
        નહીંતર જો (totalChannels > cn)
        {
            idx = (પૂર્ણાંક)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            પાછા;
        }
    }

    idx = cnidx = -1;
}

સ્થિર તાર્કિક ocl_mixChannels(આગત_Array_Of_Arrays _src, આગત_Output_Array_Of_Arrays _dst,
                            અચલ પૂર્ણાંક* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    પૂર્ણાંક depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    માટે (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    માટે (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    માટે (size_t i = 0; i < npairs; ++i)
    {
        પૂર્ણાંક scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        પૂર્ણાંક src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    જો (k.empty())
        પાછા ખોટું;

    પૂર્ણાંક argindex = 0;
    માટે (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    માટે (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    પાછા k.run(2, globalsize, ખાલી, ખોટું);
}

}

#endif

શૂન્ય cv::mixChannels(આગત_Array_Of_Arrays src, આગત_Output_Array_Of_Arrays dst,
                 અચલ પૂર્ણાંક* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    જો (npairs == 0 || fromTo == ખાલી)
        પાછા;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    તાર્કિક src_is_mat = src.kind() != આગત_Array_2::STD_VECTOR_MAT &&
            src.kind() != આગત_Array_2::STD_ARRAY_MAT &&
            src.kind() != આગત_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != આગત_Array_2::STD_VECTOR_UMAT;
    તાર્કિક dst_is_mat = dst.kind() != આગત_Array_2::STD_VECTOR_MAT &&
            dst.kind() != આગત_Array_2::STD_ARRAY_MAT &&
            dst.kind() != આગત_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != આગત_Array_2::STD_VECTOR_UMAT;
    પૂર્ણાંક i;
    પૂર્ણાંક nsrc = src_is_mat ? 1 : (પૂર્ણાંક)src.total();
    પૂર્ણાંક ndst = dst_is_mat ? 1 : (પૂર્ણાંક)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    માટે( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    માટે( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

શૂન્ય cv::mixChannels(આગત_Array_Of_Arrays src, આગત_Output_Array_Of_Arrays dst,
                     અચલ std::vector<પૂર્ણાંક>& fromTo)
{
    CV_INSTRUMENT_REGION();

    જો (fromTo.empty())
        પાછા;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    તાર્કિક src_is_mat = src.kind() != આગત_Array_2::STD_VECTOR_MAT &&
            src.kind() != આગત_Array_2::STD_ARRAY_MAT &&
            src.kind() != આગત_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != આગત_Array_2::STD_VECTOR_UMAT;
    તાર્કિક dst_is_mat = dst.kind() != આગત_Array_2::STD_VECTOR_MAT &&
            dst.kind() != આગત_Array_2::STD_ARRAY_MAT &&
            dst.kind() != આગત_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != આગત_Array_2::STD_VECTOR_UMAT;
    પૂર્ણાંક i;
    પૂર્ણાંક nsrc = src_is_mat ? 1 : (પૂર્ણાંક)src.total();
    પૂર્ણાંક ndst = dst_is_mat ? 1 : (પૂર્ણાંક)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    માટે( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    માટે( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

નામાકાશ cv
{
સ્થિર તાર્કિક ipp_extractChannel(અચલ Mat &src, Mat &dst, પૂર્ણાંક channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    પૂર્ણાંક srcChannels = src.channels();
    પૂર્ણાંક dstChannels = dst.channels();

    જો(src.dims != dst.dims)
        પાછા ખોટું;

    જો(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        પાછા CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (પૂર્ણાંક)src.step, srcChannels, channel, dst.ptr(), (પૂર્ણાંક)dst.step, dstChannels, 0, size, (પૂર્ણાંક)src.elemSize1()) >= 0;
    }
    નહીંતર
    {
        અચલ Mat      *arrays[] = {&dst, ખાલી};
        uchar          *ptrs[2]  = {ખાલી};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(પૂર્ણાંક)it.size, 1};

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            જો(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (પૂર્ણાંક)src.elemSize1()) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
#નહીંતર
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    પાછા ખોટું;
#endif
}

સ્થિર તાર્કિક ipp_ઉમેરો_Channel(અચલ Mat &src, Mat &dst, પૂર્ણાંક channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    પૂર્ણાંક srcChannels = src.channels();
    પૂર્ણાંક dstChannels = dst.channels();

    જો(src.dims != dst.dims)
        પાછા ખોટું;

    જો(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        પાછા CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (પૂર્ણાંક)src.step, srcChannels, 0, dst.ptr(), (પૂર્ણાંક)dst.step, dstChannels, channel, size, (પૂર્ણાંક)src.elemSize1()) >= 0;
    }
    નહીંતર
    {
        અચલ Mat      *arrays[] = {&dst, ખાલી};
        uchar          *ptrs[2]  = {ખાલી};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(પૂર્ણાંક)it.size, 1};

        માટે( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            જો(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (પૂર્ણાંક)src.elemSize1()) < 0)
                પાછા ખોટું;
        }
        પાછા સાચું;
    }
#નહીંતર
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    પાછા ખોટું;
#endif
}
}
#endif

શૂન્ય cv::extractChannel(આગત_Array _src, OutputArray _dst, પૂર્ણાંક coi)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    પૂર્ણાંક ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    જો (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        પાછા;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

શૂન્ય cv::ઉમેરો_Channel(આગત_Array _src, આગત_Output_Array _dst, પૂર્ણાંક coi)
{
    CV_INSTRUMENT_REGION();

    પૂર્ણાંક stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    પૂર્ણાંક dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    પૂર્ણાંક ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    જો (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        પાછા;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_ઉમેરો_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
