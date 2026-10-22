// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"
#ಅನ್ತರ್ಭಾವ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

ನಾಮಾಕಾಶ cv
{

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ಸ್ಥಿರ ಶೂನ್ಯ
mixChannels_( ಸ್ಥಿರಾಂಕ T** src, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sdelta,
              T** dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ddelta,
              ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ npairs )
{
    ಪೂರ್ಣಾಂಕ i, k;
    ಗಾಗಿ( k = 0; k < npairs; k++ )
    {
        ಸ್ಥಿರಾಂಕ T* s = src[k];
        T* d = dst[k];
        ಪೂರ್ಣಾಂಕ ds = sdelta[k], dd = ddelta[k];
        ಆದರೆ( s )
        {
            ಗಾಗಿ( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            ಆದರೆ( i < len )
                d[0] = s[0];
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಗಾಗಿ( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            ಆದರೆ( i < len )
                d[0] = 0;
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ mixChannels8u( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sdelta,
                           ಶೂನ್ಯ** dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ddelta,
                           ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ npairs )
{
    mixChannels_((ಸ್ಥಿರಾಂಕ uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ಸ್ಥಿರ ಶೂನ್ಯ mixChannels16u( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sdelta,
                            ಶೂನ್ಯ** dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ddelta,
                            ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ npairs )
{
    mixChannels_((ಸ್ಥಿರಾಂಕ ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ಸ್ಥಿರ ಶೂನ್ಯ mixChannels32s( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sdelta,
                            ಶೂನ್ಯ** dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ddelta,
                            ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ npairs )
{
    mixChannels_((ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ**)src, sdelta, (ಪೂರ್ಣಾಂಕ**)dst, ddelta, len, npairs);
}

ಸ್ಥಿರ ಶೂನ್ಯ mixChannels64s( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sdelta,
                            ಶೂನ್ಯ** dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ddelta,
                            ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ npairs )
{
    mixChannels_((ಸ್ಥಿರಾಂಕ int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*MixChannelsFunc)( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ** src, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sdelta,
        ಶೂನ್ಯ** dst, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ddelta, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ npairs );

ಸ್ಥಿರ MixChannelsFunc getMixchFunc(ಪೂರ್ಣಾಂಕ ಆಳ)
{
    ಸ್ಥಿರ MixChannelsFunc mixchTab[CV_ಆಳ_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ಹಿಂತಿರುಗಿ mixchTab[ಆಳ];
}

} // cv::


ಶೂನ್ಯ cv::mixChannels( ಸ್ಥಿರಾಂಕ Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* from_ಗಮ್ಯ, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ( npairs == 0 )
        ಹಿಂತಿರುಗಿ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_ಗಮ್ಯ && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    ಪೂರ್ಣಾಂಕ ಆಳ = dst[0].ಆಳ();

    Auto_ಬಫರ್<uchar> buf((nsrcs + ndsts + 1)*(ಗಾತ್ರ(Mat*) + ಗಾತ್ರ(uchar*)) + npairs*(ಗಾತ್ರ(uchar*)*2 + ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ)*6));
    ಸ್ಥಿರಾಂಕ Mat** arrays = (ಸ್ಥಿರಾಂಕ Mat**)(uchar*)buf.ದತ್ತಾಂಶ();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ಸ್ಥಿರಾಂಕ uchar** srcs = (ಸ್ಥಿರಾಂಕ uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    ಪೂರ್ಣಾಂಕ* tab = (ಪೂರ್ಣಾಂಕ*)(dsts + npairs);
    ಪೂರ್ಣಾಂಕ *sdelta = (ಪೂರ್ಣಾಂಕ*)(tab + npairs*4), *ddelta = sdelta + npairs;

    ಗಾಗಿ( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].ಆಳ() == ಆಳ );
        arrays[i] = &src[i];
    }
    ಗಾಗಿ( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].ಆಳ() == ಆಳ );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    ಗಾಗಿ( i = 0; i < npairs; i++ )
    {
        ಪೂರ್ಣಾಂಕ i0 = from_ಗಮ್ಯ[i*2], i1 = from_ಗಮ್ಯ[i*2+1];
        ಆದರೆ( i0 >= 0 )
        {
            ಗಾಗಿ( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                ಆದರೆ( i0 < src[j].channels() )
                    ನಿಲ್ಲಿಸು;
            CV_Assert(j < nsrcs && src[j].ಆಳ() == ಆಳ);
            tab[i*4] = (ಪೂರ್ಣಾಂಕ)j; tab[i*4+1] = (ಪೂರ್ಣಾಂಕ)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            tab[i*4] = (ಪೂರ್ಣಾಂಕ)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        ಗಾಗಿ( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            ಆದರೆ( i1 < dst[j].channels() )
                ನಿಲ್ಲಿಸು;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].ಆಳ() == ಆಳ);
        tab[i*4+2] = (ಪೂರ್ಣಾಂಕ)(j + nsrcs); tab[i*4+3] = (ಪೂರ್ಣಾಂಕ)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (ಪೂರ್ಣಾಂಕ)(nsrcs + ndsts));
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = (ಪೂರ್ಣಾಂಕ)it.size, blocksize = std::min(ಒಟ್ಟು, (ಪೂರ್ಣಾಂಕ)((ಖಂಡ_ಗಾತ್ರ + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(ಆಳ);
    CV_Assert(func);

    ಗಾಗಿ( i = 0; i < it.nplanes; i++, ++it )
    {
        ಗಾಗಿ( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ t = 0; t < ಒಟ್ಟು; t += blocksize )
        {
            ಪೂರ್ಣಾಂಕ bsz = std::min(ಒಟ್ಟು - t, blocksize);
            func( (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ**)srcs, sdelta, (ಶೂನ್ಯ **)dsts, ddelta, bsz, (ಪೂರ್ಣಾಂಕ)npairs );

            ಆದರೆ( t + blocksize < ಒಟ್ಟು )
                ಗಾಗಿ( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಶೂನ್ಯ get_UMat_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ std::vector<UMat> & um, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ & idx, ಪೂರ್ಣಾಂಕ & cnidx)
{
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_Channels = 0;
    ಗಾಗಿ (size_t i = 0, size = um.size(); i < size; ++i)
    {
        ಪೂರ್ಣಾಂಕ ccn = um[i].channels();
        ಒಟ್ಟು_Channels += ccn;

        ಆದರೆ (ಒಟ್ಟು_Channels == cn)
        {
            idx = (ಪೂರ್ಣಾಂಕ)(i + 1);
            cnidx = 0;
            ಹಿಂತಿರುಗಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಟ್ಟು_Channels > cn)
        {
            idx = (ಪೂರ್ಣಾಂಕ)i;
            cnidx = i == 0 ? cn : (cn - ಒಟ್ಟು_Channels + ccn);
            ಹಿಂತಿರುಗಿ;
        }
    }

    idx = cnidx = -1;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_mixChannels(ಒಳಹರಿವು_Array_ದ_Arrays _src, ಒಳಹರಿವು_Output_Array_ದ_Arrays _dst,
                            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* from_ಗಮ್ಯ, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_ಸದಿಶ(src);
    _dst.get_UMat_ಸದಿಶ(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    ಗಾತ್ರ_2 size = src[0].size();
    ಪೂರ್ಣಾಂಕ ಆಳ = src[0].ಆಳ(), esz = CV_ELEM_ಗಾತ್ರ(ಆಳ),
            ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    ಗಾಗಿ (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].ಆಳ() == ಆಳ);
    ಗಾಗಿ (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].ಆಳ() == ಆಳ);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    ಗಾಗಿ (size_t i = 0; i < npairs; ++i)
    {
        ಪೂರ್ಣಾಂಕ scn = from_ಗಮ್ಯ[i<<1], dcn = from_ಗಮ್ಯ[(i<<1) + 1];
        ಪೂರ್ಣಾಂಕ src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_ಸೂಚ್ಯಂಕ(src, scn, src_idx, src_cnidx);
        get_UMat_ಸೂಚ್ಯಂಕ(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_ಗಮ್ಯ_Str(ಆಳ), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಪೂರ್ಣಾಂಕ argindex = 0;
    ಗಾಗಿ (size_t i = 0; i < npairs; ++i)
        argindex = k.ಗಣ(argindex, ocl::KernelArg::Read_Only_No_ಗಾತ್ರ(srcargs[i]));
    ಗಾಗಿ (size_t i = 0; i < npairs; ++i)
        argindex = k.ಗಣ(argindex, ocl::KernelArg::Write_Only_No_ಗಾತ್ರ(dstargs[i]));
    argindex = k.ಗಣ(argindex, size.ಎತ್ತರ);
    argindex = k.ಗಣ(argindex, size.ಅಗಲ);
    k.ಗಣ(argindex, ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI);

    size_t globalsize[2] = { (size_t)size.ಅಗಲ, ((size_t)size.ಎತ್ತರ + ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI - 1) / ಅಡ್ಡಸಾಲುಗಳು_ಪ್ರತಿ_WI };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, ಖಾಲಿ, ಸುಳ್ಳು);
}

}

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::mixChannels(ಒಳಹರಿವು_Array_ದ_Arrays src, ಒಳಹರಿವು_Output_Array_ದ_Arrays dst,
                 ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* from_ಗಮ್ಯ, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (npairs == 0 || from_ಗಮ್ಯ == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ;

    CV_OCL_ಸತತ(dst.is_UMat_ಸದಿಶ(),
               ocl_mixChannels(src, dst, from_ಗಮ್ಯ, npairs))

    ತಾರ್ಕಿಕ src_is_mat = src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT &&
            src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_ಸದಿಶ &&
            src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_UMAT;
    ತಾರ್ಕಿಕ dst_is_mat = dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT &&
            dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_ಸದಿಶ &&
            dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_UMAT;
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ nsrc = src_is_mat ? 1 : (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು();
    ಪೂರ್ಣಾಂಕ ndst = dst_is_mat ? 1 : (ಪೂರ್ಣಾಂಕ)dst.ಒಟ್ಟು();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_ಬಫರ್<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.ದತ್ತಾಂಶ();
    ಗಾಗಿ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ಗಾಗಿ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_ಗಮ್ಯ, npairs);
}

ಶೂನ್ಯ cv::mixChannels(ಒಳಹರಿವು_Array_ದ_Arrays src, ಒಳಹರಿವು_Output_Array_ದ_Arrays dst,
                     ಸ್ಥಿರಾಂಕ std::vector<ಪೂರ್ಣಾಂಕ>& from_ಗಮ್ಯ)
{
    CV_INSTRUMENT_REGION();

    ಆದರೆ (from_ಗಮ್ಯ.empty())
        ಹಿಂತಿರುಗಿ;

    CV_OCL_ಸತತ(dst.is_UMat_ಸದಿಶ(),
               ocl_mixChannels(src, dst, &from_ಗಮ್ಯ[0], from_ಗಮ್ಯ.size()>>1))

    ತಾರ್ಕಿಕ src_is_mat = src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT &&
            src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_ಸದಿಶ &&
            src.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_UMAT;
    ತಾರ್ಕಿಕ dst_is_mat = dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_MAT &&
            dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_ಸದಿಶ &&
            dst.ಬಗೆ() != ಒಳಹರಿವು_Array_2::STD_ಸದಿಶ_UMAT;
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ nsrc = src_is_mat ? 1 : (ಪೂರ್ಣಾಂಕ)src.ಒಟ್ಟು();
    ಪೂರ್ಣಾಂಕ ndst = dst_is_mat ? 1 : (ಪೂರ್ಣಾಂಕ)dst.ಒಟ್ಟು();

    CV_Assert(from_ಗಮ್ಯ.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_ಬಫರ್<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.ದತ್ತಾಂಶ();
    ಗಾಗಿ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ಗಾಗಿ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_ಗಮ್ಯ[0], from_ಗಮ್ಯ.size()/2);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP

ನಾಮಾಕಾಶ cv
{
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ಹೊರತೆಗೆ_Channel(ಸ್ಥಿರಾಂಕ Mat &src, Mat &dst, ಪೂರ್ಣಾಂಕ channel)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಪೂರ್ಣಾಂಕ srcChannels = src.channels();
    ಪೂರ್ಣಾಂಕ dstChannels = dst.channels();

    ಆದರೆ(src.dims != dst.dims)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(src.dims <= 2)
    {
        Ippi_ಗಾತ್ರ size = ippi_ಗಾತ್ರ(src.size());

        ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, srcChannels, channel, dst.ptr(), (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ, dstChannels, 0, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1()) >= 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat      *arrays[] = {&dst, ಖಾಲಿ};
        uchar          *ptrs[2]  = {ಖಾಲಿ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ಗಾತ್ರ size = {(ಪೂರ್ಣಾಂಕ)it.size, 1};

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1()) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(dst); CV_ಬಳಸದ(channel);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ಸೇರಿಸು_Channel(ಸ್ಥಿರಾಂಕ Mat &src, Mat &dst, ಪೂರ್ಣಾಂಕ channel)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಪೂರ್ಣಾಂಕ srcChannels = src.channels();
    ಪೂರ್ಣಾಂಕ dstChannels = dst.channels();

    ಆದರೆ(src.dims != dst.dims)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ(src.dims <= 2)
    {
        Ippi_ಗಾತ್ರ size = ippi_ಗಾತ್ರ(src.size());

        ಹಿಂತಿರುಗಿ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ಪೂರ್ಣಾಂಕ)src.ಹೆಜ್ಜೆ, srcChannels, 0, dst.ptr(), (ಪೂರ್ಣಾಂಕ)dst.ಹೆಜ್ಜೆ, dstChannels, channel, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1()) >= 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸ್ಥಿರಾಂಕ Mat      *arrays[] = {&dst, ಖಾಲಿ};
        uchar          *ptrs[2]  = {ಖಾಲಿ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ಗಾತ್ರ size = {(ಪೂರ್ಣಾಂಕ)it.size, 1};

        ಗಾಗಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಆದರೆ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (ಪೂರ್ಣಾಂಕ)src.elemSize1()) < 0)
                ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        }
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(dst); CV_ಬಳಸದ(channel);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ cv::ಹೊರತೆಗೆ_Channel(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಂಕ coi)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ type = _src.type(), ಆಳ = CV_MAT_ಆಳ(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    ಪೂರ್ಣಾಂಕ ch[] = { coi, 0 };

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಆದರೆ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_ಗಾತ್ರ(_src, ಆಳ);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat();
    _dst.create_Same_ಗಾತ್ರ(_src, ಆಳ);
    Mat dst = _dst.getMat();

    CV_IPP_ಸತತ_FAST(ipp_ಹೊರತೆಗೆ_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

ಶೂನ್ಯ cv::ಸೇರಿಸು_Channel(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Output_Array _dst, ಪೂರ್ಣಾಂಕ coi)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಂಕ stype = _src.type(), sdepth = CV_MAT_ಆಳ(stype), scn = CV_MAT_CN(stype);
    ಪೂರ್ಣಾಂಕ dtype = _dst.type(), ddepth = CV_MAT_ಆಳ(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_ಗಾತ್ರ(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    ಪೂರ್ಣಾಂಕ ch[] = { 0, coi };
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ಆದರೆ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_ಸತತ_FAST(ipp_ಸೇರಿಸು_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
