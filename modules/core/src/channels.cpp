// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"
#अन्तर्भाव "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

नामाकाश cv
{

प्रतिरूपम्<प्रकारनाम T> स्थिर शून्य
mixChannels_( ध्रुव T** src, ध्रुव पूर्णाङ्क* sdelta,
              T** dst, ध्रुव पूर्णाङ्क* ddelta,
              पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    पूर्णाङ्क i, k;
    प्रति( k = 0; k < npairs; k++ )
    {
        ध्रुव T* s = src[k];
        T* d = dst[k];
        पूर्णाङ्क ds = sdelta[k], dd = ddelta[k];
        यदि( s )
        {
            प्रति( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            यदि( i < len )
                d[0] = s[0];
        }
        अन्यथा
        {
            प्रति( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            यदि( i < len )
                d[0] = 0;
        }
    }
}


स्थिर शून्य mixChannels8u( ध्रुव शून्य** src, ध्रुव पूर्णाङ्क* sdelta,
                           शून्य** dst, ध्रुव पूर्णाङ्क* ddelta,
                           पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((ध्रुव uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels16u( ध्रुव शून्य** src, ध्रुव पूर्णाङ्क* sdelta,
                            शून्य** dst, ध्रुव पूर्णाङ्क* ddelta,
                            पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((ध्रुव ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels32s( ध्रुव शून्य** src, ध्रुव पूर्णाङ्क* sdelta,
                            शून्य** dst, ध्रुव पूर्णाङ्क* ddelta,
                            पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((ध्रुव पूर्णाङ्क**)src, sdelta, (पूर्णाङ्क**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels64s( ध्रुव शून्य** src, ध्रुव पूर्णाङ्क* sdelta,
                            शून्य** dst, ध्रुव पूर्णाङ्क* ddelta,
                            पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((ध्रुव int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

प्रकारनिर्देश शून्य (*MixChannelsFunc)( ध्रुव शून्य** src, ध्रुव पूर्णाङ्क* sdelta,
        शून्य** dst, ध्रुव पूर्णाङ्क* ddelta, पूर्णाङ्क len, पूर्णाङ्क npairs );

स्थिर MixChannelsFunc getMixchFunc(पूर्णाङ्क गभीरता)
{
    स्थिर MixChannelsFunc mixchTab[CV_गभीरता_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    प्रत्यागम mixchTab[गभीरता];
}

} // cv::


शून्य cv::mixChannels( ध्रुव Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ध्रुव पूर्णाङ्क* from_गन्तव्यम्, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    यदि( npairs == 0 )
        प्रत्यागम;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_गन्तव्यम् && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    पूर्णाङ्क गभीरता = dst[0].गभीरता();

    Auto_बफर<uchar> buf((nsrcs + ndsts + 1)*(परिमाण(Mat*) + परिमाण(uchar*)) + npairs*(परिमाण(uchar*)*2 + परिमाण(पूर्णाङ्क)*6));
    ध्रुव Mat** arrays = (ध्रुव Mat**)(uchar*)buf.दत्त();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ध्रुव uchar** srcs = (ध्रुव uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    पूर्णाङ्क* tab = (पूर्णाङ्क*)(dsts + npairs);
    पूर्णाङ्क *sdelta = (पूर्णाङ्क*)(tab + npairs*4), *ddelta = sdelta + npairs;

    प्रति( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].गभीरता() == गभीरता );
        arrays[i] = &src[i];
    }
    प्रति( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].गभीरता() == गभीरता );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    प्रति( i = 0; i < npairs; i++ )
    {
        पूर्णाङ्क i0 = from_गन्तव्यम्[i*2], i1 = from_गन्तव्यम्[i*2+1];
        यदि( i0 >= 0 )
        {
            प्रति( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                यदि( i0 < src[j].channels() )
                    भङ्ग;
            CV_Assert(j < nsrcs && src[j].गभीरता() == गभीरता);
            tab[i*4] = (पूर्णाङ्क)j; tab[i*4+1] = (पूर्णाङ्क)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        अन्यथा
        {
            tab[i*4] = (पूर्णाङ्क)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        प्रति( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            यदि( i1 < dst[j].channels() )
                भङ्ग;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].गभीरता() == गभीरता);
        tab[i*4+2] = (पूर्णाङ्क)(j + nsrcs); tab[i*4+3] = (पूर्णाङ्क)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (पूर्णाङ्क)(nsrcs + ndsts));
    पूर्णाङ्क योग = (पूर्णाङ्क)it.size, blocksize = std::min(योग, (पूर्णाङ्क)((खण्डम्_आकार + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(गभीरता);
    CV_Assert(func);

    प्रति( i = 0; i < it.nplanes; i++, ++it )
    {
        प्रति( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        प्रति( पूर्णाङ्क t = 0; t < योग; t += blocksize )
        {
            पूर्णाङ्क bsz = std::min(योग - t, blocksize);
            func( (ध्रुव शून्य**)srcs, sdelta, (शून्य **)dsts, ddelta, bsz, (पूर्णाङ्क)npairs );

            यदि( t + blocksize < योग )
                प्रति( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#यदि_परिभाषितम् HAVE_OPENCL

नामाकाश cv {

स्थिर शून्य get_UMat_सूचकांक(ध्रुव std::vector<UMat> & um, पूर्णाङ्क cn, पूर्णाङ्क & idx, पूर्णाङ्क & cnidx)
{
    पूर्णाङ्क योग_Channels = 0;
    प्रति (size_t i = 0, size = um.size(); i < size; ++i)
    {
        पूर्णाङ्क ccn = um[i].channels();
        योग_Channels += ccn;

        यदि (योग_Channels == cn)
        {
            idx = (पूर्णाङ्क)(i + 1);
            cnidx = 0;
            प्रत्यागम;
        }
        अन्यथा यदि (योग_Channels > cn)
        {
            idx = (पूर्णाङ्क)i;
            cnidx = i == 0 ? cn : (cn - योग_Channels + ccn);
            प्रत्यागम;
        }
    }

    idx = cnidx = -1;
}

स्थिर तार्किक ocl_mixChannels(निवेशः_Array_सम्बन्धि_Arrays _src, निवेशः_Output_Array_सम्बन्धि_Arrays _dst,
                            ध्रुव पूर्णाङ्क* from_गन्तव्यम्, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_सदिशम्(src);
    _dst.get_UMat_सदिशम्(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    आकार size = src[0].size();
    पूर्णाङ्क गभीरता = src[0].गभीरता(), esz = CV_ELEM_आकार(गभीरता),
            पङ्क्तिकाः_प्रत्येकम्_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    प्रति (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].गभीरता() == गभीरता);
    प्रति (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].गभीरता() == गभीरता);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    प्रति (size_t i = 0; i < npairs; ++i)
    {
        पूर्णाङ्क scn = from_गन्तव्यम्[i<<1], dcn = from_गन्तव्यम्[(i<<1) + 1];
        पूर्णाङ्क src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_सूचकांक(src, scn, src_idx, src_cnidx);
        get_UMat_सूचकांक(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_गन्तव्यम्_Str(गभीरता), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    यदि (k.empty())
        प्रत्यागम असत्य;

    पूर्णाङ्क argindex = 0;
    प्रति (size_t i = 0; i < npairs; ++i)
        argindex = k.समुच्चयः(argindex, ocl::KernelArg::Read_Only_No_आकार(srcargs[i]));
    प्रति (size_t i = 0; i < npairs; ++i)
        argindex = k.समुच्चयः(argindex, ocl::KernelArg::Write_Only_No_आकार(dstargs[i]));
    argindex = k.समुच्चयः(argindex, size.height);
    argindex = k.समुच्चयः(argindex, size.विस्तारः);
    k.समुच्चयः(argindex, पङ्क्तिकाः_प्रत्येकम्_WI);

    size_t globalsize[2] = { (size_t)size.विस्तारः, ((size_t)size.height + पङ्क्तिकाः_प्रत्येकम्_WI - 1) / पङ्क्तिकाः_प्रत्येकम्_WI };
    प्रत्यागम k.सततम्(2, globalsize, रिक्त, असत्य);
}

}

#यदि_अन्तः

शून्य cv::mixChannels(निवेशः_Array_सम्बन्धि_Arrays src, निवेशः_Output_Array_सम्बन्धि_Arrays dst,
                 ध्रुव पूर्णाङ्क* from_गन्तव्यम्, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    यदि (npairs == 0 || from_गन्तव्यम् == रिक्त)
        प्रत्यागम;

    CV_OCL_सततम्(dst.is_UMat_सदिशम्(),
               ocl_mixChannels(src, dst, from_गन्तव्यम्, npairs))

    तार्किक src_is_mat = src.विधा() != निवेशः_Array_2::STD_सदिशम्_MAT &&
            src.विधा() != निवेशः_Array_2::STD_ARRAY_MAT &&
            src.विधा() != निवेशः_Array_2::STD_सदिशम्_सदिशम् &&
            src.विधा() != निवेशः_Array_2::STD_सदिशम्_UMAT;
    तार्किक dst_is_mat = dst.विधा() != निवेशः_Array_2::STD_सदिशम्_MAT &&
            dst.विधा() != निवेशः_Array_2::STD_ARRAY_MAT &&
            dst.विधा() != निवेशः_Array_2::STD_सदिशम्_सदिशम् &&
            dst.विधा() != निवेशः_Array_2::STD_सदिशम्_UMAT;
    पूर्णाङ्क i;
    पूर्णाङ्क nsrc = src_is_mat ? 1 : (पूर्णाङ्क)src.योग();
    पूर्णाङ्क ndst = dst_is_mat ? 1 : (पूर्णाङ्क)dst.योग();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_बफर<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.दत्त();
    प्रति( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    प्रति( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_गन्तव्यम्, npairs);
}

शून्य cv::mixChannels(निवेशः_Array_सम्बन्धि_Arrays src, निवेशः_Output_Array_सम्बन्धि_Arrays dst,
                     ध्रुव std::vector<पूर्णाङ्क>& from_गन्तव्यम्)
{
    CV_INSTRUMENT_REGION();

    यदि (from_गन्तव्यम्.empty())
        प्रत्यागम;

    CV_OCL_सततम्(dst.is_UMat_सदिशम्(),
               ocl_mixChannels(src, dst, &from_गन्तव्यम्[0], from_गन्तव्यम्.size()>>1))

    तार्किक src_is_mat = src.विधा() != निवेशः_Array_2::STD_सदिशम्_MAT &&
            src.विधा() != निवेशः_Array_2::STD_ARRAY_MAT &&
            src.विधा() != निवेशः_Array_2::STD_सदिशम्_सदिशम् &&
            src.विधा() != निवेशः_Array_2::STD_सदिशम्_UMAT;
    तार्किक dst_is_mat = dst.विधा() != निवेशः_Array_2::STD_सदिशम्_MAT &&
            dst.विधा() != निवेशः_Array_2::STD_ARRAY_MAT &&
            dst.विधा() != निवेशः_Array_2::STD_सदिशम्_सदिशम् &&
            dst.विधा() != निवेशः_Array_2::STD_सदिशम्_UMAT;
    पूर्णाङ्क i;
    पूर्णाङ्क nsrc = src_is_mat ? 1 : (पूर्णाङ्क)src.योग();
    पूर्णाङ्क ndst = dst_is_mat ? 1 : (पूर्णाङ्क)dst.योग();

    CV_Assert(from_गन्तव्यम्.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_बफर<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.दत्त();
    प्रति( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    प्रति( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_गन्तव्यम्[0], from_गन्तव्यम्.size()/2);
}

#यदि_परिभाषितम् HAVE_IPP

नामाकाश cv
{
स्थिर तार्किक ipp_extractChannel(ध्रुव Mat &src, Mat &dst, पूर्णाङ्क channel)
{
#यदि_परिभाषितम् HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णाङ्क srcChannels = src.channels();
    पूर्णाङ्क dstChannels = dst.channels();

    यदि(src.dims != dst.dims)
        प्रत्यागम असत्य;

    यदि(src.dims <= 2)
    {
        Ippi_आकार size = ippi_आकार(src.size());

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णाङ्क)src.चरणः, srcChannels, channel, dst.ptr(), (पूर्णाङ्क)dst.चरणः, dstChannels, 0, size, (पूर्णाङ्क)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat      *arrays[] = {&dst, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार size = {(पूर्णाङ्क)it.size, 1};

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (पूर्णाङ्क)src.elemSize1()) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(channel);
    प्रत्यागम असत्य;
#यदि_अन्तः
}

स्थिर तार्किक ipp_निवेशय_Channel(ध्रुव Mat &src, Mat &dst, पूर्णाङ्क channel)
{
#यदि_परिभाषितम् HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णाङ्क srcChannels = src.channels();
    पूर्णाङ्क dstChannels = dst.channels();

    यदि(src.dims != dst.dims)
        प्रत्यागम असत्य;

    यदि(src.dims <= 2)
    {
        Ippi_आकार size = ippi_आकार(src.size());

        प्रत्यागम CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णाङ्क)src.चरणः, srcChannels, 0, dst.ptr(), (पूर्णाङ्क)dst.चरणः, dstChannels, channel, size, (पूर्णाङ्क)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        ध्रुव Mat      *arrays[] = {&dst, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार size = {(पूर्णाङ्क)it.size, 1};

        प्रति( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (पूर्णाङ्क)src.elemSize1()) < 0)
                प्रत्यागम असत्य;
        }
        प्रत्यागम सत्य;
    }
#अन्यथा
    CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(channel);
    प्रत्यागम असत्य;
#यदि_अन्तः
}
}
#यदि_अन्तः

शून्य cv::extractChannel(निवेशः_Array _src, OutputArray _dst, पूर्णाङ्क coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src.type(), गभीरता = CV_MAT_गभीरता(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    पूर्णाङ्क ch[] = { coi, 0 };

#यदि_परिभाषितम् HAVE_OPENCL
    यदि (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_आकार(_src, गभीरता);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        प्रत्यागम;
    }
#यदि_अन्तः

    Mat src = _src.getMat();
    _dst.create_Same_आकार(_src, गभीरता);
    Mat dst = _dst.getMat();

    CV_IPP_सततम्_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

शून्य cv::निवेशय_Channel(निवेशः_Array _src, निवेशः_Output_Array _dst, पूर्णाङ्क coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क stype = _src.type(), sdepth = CV_MAT_गभीरता(stype), scn = CV_MAT_CN(stype);
    पूर्णाङ्क dtype = _dst.type(), ddepth = CV_MAT_गभीरता(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_आकार(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    पूर्णाङ्क ch[] = { 0, coi };
#यदि_परिभाषितम् HAVE_OPENCL
    यदि (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        प्रत्यागम;
    }
#यदि_अन्तः

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_सततम्_FAST(ipp_निवेशय_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
