// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

ਨਾਮਾਕਾਸ਼ cv
{

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T> ਸਥਿਰ ਖਾਲੀ
mixChannels_( ਅਚਲ T** src, ਅਚਲ ਪੂਰਨ* sdelta,
              T** dst, ਅਚਲ ਪੂਰਨ* ddelta,
              ਪੂਰਨ len, ਪੂਰਨ npairs )
{
    ਪੂਰਨ i, k;
    ਲਈ( k = 0; k < npairs; k++ )
    {
        ਅਚਲ T* s = src[k];
        T* d = dst[k];
        ਪੂਰਨ ds = sdelta[k], dd = ddelta[k];
        ਜੇ( s )
        {
            ਲਈ( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            ਜੇ( i < len )
                d[0] = s[0];
        }
        ਨਹੀਂਤਾਂ
        {
            ਲਈ( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            ਜੇ( i < len )
                d[0] = 0;
        }
    }
}


ਸਥਿਰ ਖਾਲੀ mixChannels8u( ਅਚਲ ਖਾਲੀ** src, ਅਚਲ ਪੂਰਨ* sdelta,
                           ਖਾਲੀ** dst, ਅਚਲ ਪੂਰਨ* ddelta,
                           ਪੂਰਨ len, ਪੂਰਨ npairs )
{
    mixChannels_((ਅਚਲ uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ਸਥਿਰ ਖਾਲੀ mixChannels16u( ਅਚਲ ਖਾਲੀ** src, ਅਚਲ ਪੂਰਨ* sdelta,
                            ਖਾਲੀ** dst, ਅਚਲ ਪੂਰਨ* ddelta,
                            ਪੂਰਨ len, ਪੂਰਨ npairs )
{
    mixChannels_((ਅਚਲ ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ਸਥਿਰ ਖਾਲੀ mixChannels32s( ਅਚਲ ਖਾਲੀ** src, ਅਚਲ ਪੂਰਨ* sdelta,
                            ਖਾਲੀ** dst, ਅਚਲ ਪੂਰਨ* ddelta,
                            ਪੂਰਨ len, ਪੂਰਨ npairs )
{
    mixChannels_((ਅਚਲ ਪੂਰਨ**)src, sdelta, (ਪੂਰਨ**)dst, ddelta, len, npairs);
}

ਸਥਿਰ ਖਾਲੀ mixChannels64s( ਅਚਲ ਖਾਲੀ** src, ਅਚਲ ਪੂਰਨ* sdelta,
                            ਖਾਲੀ** dst, ਅਚਲ ਪੂਰਨ* ddelta,
                            ਪੂਰਨ len, ਪੂਰਨ npairs )
{
    mixChannels_((ਅਚਲ int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*MixChannelsFunc)( ਅਚਲ ਖਾਲੀ** src, ਅਚਲ ਪੂਰਨ* sdelta,
        ਖਾਲੀ** dst, ਅਚਲ ਪੂਰਨ* ddelta, ਪੂਰਨ len, ਪੂਰਨ npairs );

ਸਥਿਰ MixChannelsFunc getMixchFunc(ਪੂਰਨ ਡੂੰਘਾਈ)
{
    ਸਥਿਰ MixChannelsFunc mixchTab[CV_ਡੂੰਘਾਈ_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ਵਾਪਸ mixchTab[ਡੂੰਘਾਈ];
}

} // cv::


ਖਾਲੀ cv::mixChannels( ਅਚਲ Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ਅਚਲ ਪੂਰਨ* from_ਮੰਜ਼ਿਲ, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    ਜੇ( npairs == 0 )
        ਵਾਪਸ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_ਮੰਜ਼ਿਲ && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    ਪੂਰਨ ਡੂੰਘਾਈ = dst[0].ਡੂੰਘਾਈ();

    Auto_ਬਫਰ<uchar> buf((nsrcs + ndsts + 1)*(ਆਕਾਰ(Mat*) + ਆਕਾਰ(uchar*)) + npairs*(ਆਕਾਰ(uchar*)*2 + ਆਕਾਰ(ਪੂਰਨ)*6));
    ਅਚਲ Mat** arrays = (ਅਚਲ Mat**)(uchar*)buf.ਅੰਕੜੇ();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ਅਚਲ uchar** srcs = (ਅਚਲ uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    ਪੂਰਨ* tab = (ਪੂਰਨ*)(dsts + npairs);
    ਪੂਰਨ *sdelta = (ਪੂਰਨ*)(tab + npairs*4), *ddelta = sdelta + npairs;

    ਲਈ( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ );
        arrays[i] = &src[i];
    }
    ਲਈ( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    ਲਈ( i = 0; i < npairs; i++ )
    {
        ਪੂਰਨ i0 = from_ਮੰਜ਼ਿਲ[i*2], i1 = from_ਮੰਜ਼ਿਲ[i*2+1];
        ਜੇ( i0 >= 0 )
        {
            ਲਈ( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                ਜੇ( i0 < src[j].channels() )
                    ਰੋਕੋ;
            CV_Assert(j < nsrcs && src[j].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ);
            tab[i*4] = (ਪੂਰਨ)j; tab[i*4+1] = (ਪੂਰਨ)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        ਨਹੀਂਤਾਂ
        {
            tab[i*4] = (ਪੂਰਨ)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        ਲਈ( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            ਜੇ( i1 < dst[j].channels() )
                ਰੋਕੋ;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ);
        tab[i*4+2] = (ਪੂਰਨ)(j + nsrcs); tab[i*4+3] = (ਪੂਰਨ)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (ਪੂਰਨ)(nsrcs + ndsts));
    ਪੂਰਨ ਕੁੱਲ = (ਪੂਰਨ)it.size, blocksize = std::min(ਕੁੱਲ, (ਪੂਰਨ)((ਖੰਡ_ਮਾਪ + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(ਡੂੰਘਾਈ);
    CV_Assert(func);

    ਲਈ( i = 0; i < it.nplanes; i++, ++it )
    {
        ਲਈ( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        ਲਈ( ਪੂਰਨ t = 0; t < ਕੁੱਲ; t += blocksize )
        {
            ਪੂਰਨ bsz = std::min(ਕੁੱਲ - t, blocksize);
            func( (ਅਚਲ ਖਾਲੀ**)srcs, sdelta, (ਖਾਲੀ **)dsts, ddelta, bsz, (ਪੂਰਨ)npairs );

            ਜੇ( t + blocksize < ਕੁੱਲ )
                ਲਈ( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਖਾਲੀ get_UMat_ਸੂਚਕਾਂਕ(ਅਚਲ std::vector<UMat> & um, ਪੂਰਨ cn, ਪੂਰਨ & idx, ਪੂਰਨ & cnidx)
{
    ਪੂਰਨ ਕੁੱਲ_Channels = 0;
    ਲਈ (size_t i = 0, size = um.size(); i < size; ++i)
    {
        ਪੂਰਨ ccn = um[i].channels();
        ਕੁੱਲ_Channels += ccn;

        ਜੇ (ਕੁੱਲ_Channels == cn)
        {
            idx = (ਪੂਰਨ)(i + 1);
            cnidx = 0;
            ਵਾਪਸ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (ਕੁੱਲ_Channels > cn)
        {
            idx = (ਪੂਰਨ)i;
            cnidx = i == 0 ? cn : (cn - ਕੁੱਲ_Channels + ccn);
            ਵਾਪਸ;
        }
    }

    idx = cnidx = -1;
}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_mixChannels(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays _src, ਨਿਵੇਸ਼_Output_Array_ਦਾ_Arrays _dst,
                            ਅਚਲ ਪੂਰਨ* from_ਮੰਜ਼ਿਲ, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_ਸਦਿਸ਼(src);
    _dst.get_UMat_ਸਦਿਸ਼(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    ਮਾਪ size = src[0].size();
    ਪੂਰਨ ਡੂੰਘਾਈ = src[0].ਡੂੰਘਾਈ(), esz = CV_ELEM_ਮਾਪ(ਡੂੰਘਾਈ),
            ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    ਲਈ (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ);
    ਲਈ (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].ਡੂੰਘਾਈ() == ਡੂੰਘਾਈ);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    ਲਈ (size_t i = 0; i < npairs; ++i)
    {
        ਪੂਰਨ scn = from_ਮੰਜ਼ਿਲ[i<<1], dcn = from_ਮੰਜ਼ਿਲ[(i<<1) + 1];
        ਪੂਰਨ src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_ਸੂਚਕਾਂਕ(src, scn, src_idx, src_cnidx);
        get_UMat_ਸੂਚਕਾਂਕ(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_ਮੰਜ਼ਿਲ_Str(ਡੂੰਘਾਈ), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    ਪੂਰਨ argindex = 0;
    ਲਈ (size_t i = 0; i < npairs; ++i)
        argindex = k.ਸੈੱਟ(argindex, ocl::KernelArg::Read_Only_No_ਮਾਪ(srcargs[i]));
    ਲਈ (size_t i = 0; i < npairs; ++i)
        argindex = k.ਸੈੱਟ(argindex, ocl::KernelArg::Write_Only_No_ਮਾਪ(dstargs[i]));
    argindex = k.ਸੈੱਟ(argindex, size.ਉਚਾਈ);
    argindex = k.ਸੈੱਟ(argindex, size.ਚੌੜਾਈ);
    k.ਸੈੱਟ(argindex, ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI);

    size_t globalsize[2] = { (size_t)size.ਚੌੜਾਈ, ((size_t)size.ਉਚਾਈ + ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI - 1) / ਪੰਕਤੀਆਂ_ਪ੍ਰਤੀ_WI };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, ਨਿਰਾ, ਝੂਠ);
}

}

#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::mixChannels(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays src, ਨਿਵੇਸ਼_Output_Array_ਦਾ_Arrays dst,
                 ਅਚਲ ਪੂਰਨ* from_ਮੰਜ਼ਿਲ, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    ਜੇ (npairs == 0 || from_ਮੰਜ਼ਿਲ == ਨਿਰਾ)
        ਵਾਪਸ;

    CV_OCL_ਲੜੀਵਾਰ(dst.is_UMat_ਸਦਿਸ਼(),
               ocl_mixChannels(src, dst, from_ਮੰਜ਼ਿਲ, npairs))

    ਤਾਰਕਿਕ src_is_mat = src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT &&
            src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT &&
            src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_ਸਦਿਸ਼ &&
            src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_UMAT;
    ਤਾਰਕਿਕ dst_is_mat = dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT &&
            dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT &&
            dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_ਸਦਿਸ਼ &&
            dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_UMAT;
    ਪੂਰਨ i;
    ਪੂਰਨ nsrc = src_is_mat ? 1 : (ਪੂਰਨ)src.ਕੁੱਲ();
    ਪੂਰਨ ndst = dst_is_mat ? 1 : (ਪੂਰਨ)dst.ਕੁੱਲ();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_ਬਫਰ<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.ਅੰਕੜੇ();
    ਲਈ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ਲਈ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_ਮੰਜ਼ਿਲ, npairs);
}

ਖਾਲੀ cv::mixChannels(ਨਿਵੇਸ਼_Array_ਦਾ_Arrays src, ਨਿਵੇਸ਼_Output_Array_ਦਾ_Arrays dst,
                     ਅਚਲ std::vector<ਪੂਰਨ>& from_ਮੰਜ਼ਿਲ)
{
    CV_INSTRUMENT_REGION();

    ਜੇ (from_ਮੰਜ਼ਿਲ.empty())
        ਵਾਪਸ;

    CV_OCL_ਲੜੀਵਾਰ(dst.is_UMat_ਸਦਿਸ਼(),
               ocl_mixChannels(src, dst, &from_ਮੰਜ਼ਿਲ[0], from_ਮੰਜ਼ਿਲ.size()>>1))

    ਤਾਰਕਿਕ src_is_mat = src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT &&
            src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT &&
            src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_ਸਦਿਸ਼ &&
            src.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_UMAT;
    ਤਾਰਕਿਕ dst_is_mat = dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_MAT &&
            dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ARRAY_MAT &&
            dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_ਸਦਿਸ਼ &&
            dst.ਕਿਸਮ() != ਨਿਵੇਸ਼_Array_2::STD_ਸਦਿਸ਼_UMAT;
    ਪੂਰਨ i;
    ਪੂਰਨ nsrc = src_is_mat ? 1 : (ਪੂਰਨ)src.ਕੁੱਲ();
    ਪੂਰਨ ndst = dst_is_mat ? 1 : (ਪੂਰਨ)dst.ਕੁੱਲ();

    CV_Assert(from_ਮੰਜ਼ਿਲ.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_ਬਫਰ<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.ਅੰਕੜੇ();
    ਲਈ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ਲਈ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_ਮੰਜ਼ਿਲ[0], from_ਮੰਜ਼ਿਲ.size()/2);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP

ਨਾਮਾਕਾਸ਼ cv
{
ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਕੱਢੋ_Channel(ਅਚਲ Mat &src, Mat &dst, ਪੂਰਨ channel)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ਪੂਰਨ srcChannels = src.channels();
    ਪੂਰਨ dstChannels = dst.channels();

    ਜੇ(src.dims != dst.dims)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(src.dims <= 2)
    {
        Ippi_ਮਾਪ size = ippi_ਮਾਪ(src.size());

        ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ਪੂਰਨ)src.ਕਦਮ, srcChannels, channel, dst.ptr(), (ਪੂਰਨ)dst.ਕਦਮ, dstChannels, 0, size, (ਪੂਰਨ)src.elemSize1()) >= 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat      *arrays[] = {&dst, ਨਿਰਾ};
        uchar          *ptrs[2]  = {ਨਿਰਾ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ਮਾਪ size = {(ਪੂਰਨ)it.size, 1};

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (ਪੂਰਨ)src.elemSize1()) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(channel);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਤਾਰਕਿਕ ipp_ਪਾਓ_Channel(ਅਚਲ Mat &src, Mat &dst, ਪੂਰਨ channel)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ਪੂਰਨ srcChannels = src.channels();
    ਪੂਰਨ dstChannels = dst.channels();

    ਜੇ(src.dims != dst.dims)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ(src.dims <= 2)
    {
        Ippi_ਮਾਪ size = ippi_ਮਾਪ(src.size());

        ਵਾਪਸ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ਪੂਰਨ)src.ਕਦਮ, srcChannels, 0, dst.ptr(), (ਪੂਰਨ)dst.ਕਦਮ, dstChannels, channel, size, (ਪੂਰਨ)src.elemSize1()) >= 0;
    }
    ਨਹੀਂਤਾਂ
    {
        ਅਚਲ Mat      *arrays[] = {&dst, ਨਿਰਾ};
        uchar          *ptrs[2]  = {ਨਿਰਾ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ਮਾਪ size = {(ਪੂਰਨ)it.size, 1};

        ਲਈ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ਜੇ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (ਪੂਰਨ)src.elemSize1()) < 0)
                ਵਾਪਸ ਝੂਠ;
        }
        ਵਾਪਸ ਸੱਚ;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(channel);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ cv::ਕੱਢੋ_Channel(ਨਿਵੇਸ਼_Array _src, OutputArray _dst, ਪੂਰਨ coi)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ type = _src.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    ਪੂਰਨ ch[] = { coi, 0 };

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਜੇ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_ਮਾਪ(_src, ਡੂੰਘਾਈ);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat();
    _dst.create_Same_ਮਾਪ(_src, ਡੂੰਘਾਈ);
    Mat dst = _dst.getMat();

    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_ਕੱਢੋ_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

ਖਾਲੀ cv::ਪਾਓ_Channel(ਨਿਵੇਸ਼_Array _src, ਨਿਵੇਸ਼_Output_Array _dst, ਪੂਰਨ coi)
{
    CV_INSTRUMENT_REGION();

    ਪੂਰਨ stype = _src.type(), sdepth = CV_MAT_ਡੂੰਘਾਈ(stype), scn = CV_MAT_CN(stype);
    ਪੂਰਨ dtype = _dst.type(), ddepth = CV_MAT_ਡੂੰਘਾਈ(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_ਮਾਪ(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    ਪੂਰਨ ch[] = { 0, coi };
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ਜੇ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_ਲੜੀਵਾਰ_FAST(ipp_ਪਾਓ_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
