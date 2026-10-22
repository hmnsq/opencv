// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"
#ଅନ୍ତର୍ଭାଵ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

ନାମାକାଶ cv
{

template<typename T> ସ୍ଥିର ଶୂନ୍ୟ
mixChannels_( ଅଚଳ T** src, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sdelta,
              T** dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ddelta,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା npairs )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, k;
    ପାଇଁ( k = 0; k < npairs; k++ )
    {
        ଅଚଳ T* s = src[k];
        T* d = dst[k];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ds = sdelta[k], dd = ddelta[k];
        ଯଦି( s )
        {
            ପାଇଁ( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            ଯଦି( i < len )
                d[0] = s[0];
        }
        ଅନ୍ୟଥା
        {
            ପାଇଁ( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            ଯଦି( i < len )
                d[0] = 0;
        }
    }
}


ସ୍ଥିର ଶୂନ୍ୟ mixChannels8u( ଅଚଳ ଶୂନ୍ୟ** src, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sdelta,
                           ଶୂନ୍ୟ** dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ddelta,
                           ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା npairs )
{
    mixChannels_((ଅଚଳ uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ସ୍ଥିର ଶୂନ୍ୟ mixChannels16u( ଅଚଳ ଶୂନ୍ୟ** src, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sdelta,
                            ଶୂନ୍ୟ** dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ddelta,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା npairs )
{
    mixChannels_((ଅଚଳ ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ସ୍ଥିର ଶୂନ୍ୟ mixChannels32s( ଅଚଳ ଶୂନ୍ୟ** src, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sdelta,
                            ଶୂନ୍ୟ** dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ddelta,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା npairs )
{
    mixChannels_((ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା**)src, sdelta, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା**)dst, ddelta, len, npairs);
}

ସ୍ଥିର ଶୂନ୍ୟ mixChannels64s( ଅଚଳ ଶୂନ୍ୟ** src, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sdelta,
                            ଶୂନ୍ୟ** dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ddelta,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା npairs )
{
    mixChannels_((ଅଚଳ int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*MixChannelsFunc)( ଅଚଳ ଶୂନ୍ୟ** src, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sdelta,
        ଶୂନ୍ୟ** dst, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* ddelta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା npairs );

ସ୍ଥିର MixChannelsFunc getMixchFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth)
{
    ସ୍ଥିର MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ଫେରନ୍ତୁ mixchTab[depth];
}

} // cv::


ଶୂନ୍ୟ cv::mixChannels( ଅଚଳ Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    ଯଦି( npairs == 0 )
        ଫେରନ୍ତୁ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(ଆକାର(Mat*) + ଆକାର(uchar*)) + npairs*(ଆକାର(uchar*)*2 + ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*6));
    ଅଚଳ Mat** arrays = (ଅଚଳ Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ଅଚଳ uchar** srcs = (ଅଚଳ uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା* tab = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)(dsts + npairs);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *sdelta = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)(tab + npairs*4), *ddelta = sdelta + npairs;

    ପାଇଁ( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    ପାଇଁ( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    ପାଇଁ( i = 0; i < npairs; i++ )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        ଯଦି( i0 >= 0 )
        {
            ପାଇଁ( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                ଯଦି( i0 < src[j].channels() )
                    ଭାଙ୍ଗ;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)j; tab[i*4+1] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        ଅନ୍ୟଥା
        {
            tab[i*4] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        ପାଇଁ( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            ଯଦି( i1 < dst[j].channels() )
                ଭାଙ୍ଗ;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(j + nsrcs); tab[i*4+3] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(nsrcs + ndsts));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, blocksize = std::min(total, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    ପାଇଁ( i = 0; i < it.nplanes; i++, ++it )
    {
        ପାଇଁ( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = 0; t < total; t += blocksize )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsz = std::min(total - t, blocksize);
            func( (ଅଚଳ ଶୂନ୍ୟ**)srcs, sdelta, (ଶୂନ୍ୟ **)dsts, ddelta, bsz, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)npairs );

            ଯଦି( t + blocksize < total )
                ପାଇଁ( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

ନାମାକାଶ cv {

ସ୍ଥିର ଶୂନ୍ୟ getUMatIndex(ଅଚଳ std::vector<UMat> & um, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା & idx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା & cnidx)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା totalChannels = 0;
    ପାଇଁ (size_t i = 0, size = um.size(); i < size; ++i)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ccn = um[i].channels();
        totalChannels += ccn;

        ଯଦି (totalChannels == cn)
        {
            idx = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i + 1);
            cnidx = 0;
            ଫେରନ୍ତୁ;
        }
        ଅନ୍ୟଥା ଯଦି (totalChannels > cn)
        {
            idx = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            ଫେରନ୍ତୁ;
        }
    }

    idx = cnidx = -1;
}

ସ୍ଥିର ତାର୍କିକ ocl_mixChannels(ନିବେଶ_Array_Of_Arrays _src, ନିବେଶ_Output_Array_Of_Arrays _dst,
                            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    ପାଇଁ (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    ପାଇଁ (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    ପାଇଁ (size_t i = 0; i < npairs; ++i)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା argindex = 0;
    ପାଇଁ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    ପାଇଁ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ଫେରନ୍ତୁ k.run(2, globalsize, ଖାଲି, ମିଥ୍ୟା);
}

}

#endif

ଶୂନ୍ୟ cv::mixChannels(ନିବେଶ_Array_Of_Arrays src, ନିବେଶ_Output_Array_Of_Arrays dst,
                 ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    ଯଦି (npairs == 0 || fromTo == ଖାଲି)
        ଫେରନ୍ତୁ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    ତାର୍କିକ src_is_mat = src.kind() != ନିବେଶ_Array_2::STD_VECTOR_MAT &&
            src.kind() != ନିବେଶ_Array_2::STD_ARRAY_MAT &&
            src.kind() != ନିବେଶ_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != ନିବେଶ_Array_2::STD_VECTOR_UMAT;
    ତାର୍କିକ dst_is_mat = dst.kind() != ନିବେଶ_Array_2::STD_VECTOR_MAT &&
            dst.kind() != ନିବେଶ_Array_2::STD_ARRAY_MAT &&
            dst.kind() != ନିବେଶ_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != ନିବେଶ_Array_2::STD_VECTOR_UMAT;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsrc = src_is_mat ? 1 : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndst = dst_is_mat ? 1 : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    ପାଇଁ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ପାଇଁ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

ଶୂନ୍ୟ cv::mixChannels(ନିବେଶ_Array_Of_Arrays src, ନିବେଶ_Output_Array_Of_Arrays dst,
                     ଅଚଳ std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>& fromTo)
{
    CV_INSTRUMENT_REGION();

    ଯଦି (fromTo.empty())
        ଫେରନ୍ତୁ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    ତାର୍କିକ src_is_mat = src.kind() != ନିବେଶ_Array_2::STD_VECTOR_MAT &&
            src.kind() != ନିବେଶ_Array_2::STD_ARRAY_MAT &&
            src.kind() != ନିବେଶ_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != ନିବେଶ_Array_2::STD_VECTOR_UMAT;
    ତାର୍କିକ dst_is_mat = dst.kind() != ନିବେଶ_Array_2::STD_VECTOR_MAT &&
            dst.kind() != ନିବେଶ_Array_2::STD_ARRAY_MAT &&
            dst.kind() != ନିବେଶ_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != ନିବେଶ_Array_2::STD_VECTOR_UMAT;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsrc = src_is_mat ? 1 : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.total();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndst = dst_is_mat ? 1 : (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    ପାଇଁ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ପାଇଁ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

ନାମାକାଶ cv
{
ସ୍ଥିର ତାର୍କିକ ipp_extractChannel(ଅଚଳ Mat &src, Mat &dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcChannels = src.channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstChannels = dst.channels();

    ଯଦି(src.dims != dst.dims)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, srcChannels, channel, dst.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.step, dstChannels, 0, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1()) >= 0;
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat      *arrays[] = {&dst, ଖାଲି};
        uchar          *ptrs[2]  = {ଖାଲି};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, 1};

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1()) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ସ୍ଥିର ତାର୍କିକ ipp_ଭର୍ତ୍ତି_Channel(ଅଚଳ Mat &src, Mat &dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcChannels = src.channels();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstChannels = dst.channels();

    ଯଦି(src.dims != dst.dims)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ଫେରନ୍ତୁ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.step, srcChannels, 0, dst.ptr(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dst.step, dstChannels, channel, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1()) >= 0;
    }
    ଅନ୍ୟଥା
    {
        ଅଚଳ Mat      *arrays[] = {&dst, ଖାଲି};
        uchar          *ptrs[2]  = {ଖାଲି};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)it.size, 1};

        ପାଇଁ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ଯଦି(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)src.elemSize1()) < 0)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}
}
#endif

ଶୂନ୍ୟ cv::extractChannel(ନିବେଶ_Array _src, OutputArray _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coi)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    ଯଦି (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ଫେରନ୍ତୁ;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

ଶୂନ୍ୟ cv::ଭର୍ତ୍ତି_Channel(ନିବେଶ_Array _src, ନିବେଶ_Output_Array _dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coi)
{
    CV_INSTRUMENT_REGION();

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    ଯଦି (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ଫେରନ୍ତୁ;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_ଭର୍ତ୍ତି_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
