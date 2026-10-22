// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#समावेश "precomp.hpp"
#समावेश "opencl_kernels_core.hpp"
#समावेश "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

namespace cv
{

ढाँचा<typename T> अटल व्योम
mixChannels_( स्थिर T** src, स्थिर पूर्णांक* sdelta,
              T** dst, स्थिर पूर्णांक* ddelta,
              पूर्णांक len, पूर्णांक npairs )
{
    पूर्णांक i, k;
    क्रम( k = 0; k < npairs; k++ )
    {
        स्थिर T* s = src[k];
        T* d = dst[k];
        पूर्णांक ds = sdelta[k], dd = ddelta[k];
        अगर( s )
        {
            क्रम( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            अगर( i < len )
                d[0] = s[0];
        }
        अन्यथा
        {
            क्रम( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            अगर( i < len )
                d[0] = 0;
        }
    }
}


अटल व्योम mixChannels8u( स्थिर व्योम** src, स्थिर पूर्णांक* sdelta,
                           व्योम** dst, स्थिर पूर्णांक* ddelta,
                           पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिर uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

अटल व्योम mixChannels16u( स्थिर व्योम** src, स्थिर पूर्णांक* sdelta,
                            व्योम** dst, स्थिर पूर्णांक* ddelta,
                            पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिर ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

अटल व्योम mixChannels32s( स्थिर व्योम** src, स्थिर पूर्णांक* sdelta,
                            व्योम** dst, स्थिर पूर्णांक* ddelta,
                            पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिर पूर्णांक**)src, sdelta, (पूर्णांक**)dst, ddelta, len, npairs);
}

अटल व्योम mixChannels64s( स्थिर व्योम** src, स्थिर पूर्णांक* sdelta,
                            व्योम** dst, स्थिर पूर्णांक* ddelta,
                            पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिर int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

प्रकार व्योम (*MixChannelsFunc)( स्थिर व्योम** src, स्थिर पूर्णांक* sdelta,
        व्योम** dst, स्थिर पूर्णांक* ddelta, पूर्णांक len, पूर्णांक npairs );

अटल MixChannelsFunc getMixchFunc(पूर्णांक depth)
{
    अटल MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    वापस mixchTab[depth];
}

} // cv::


व्योम cv::mixChannels( स्थिर Mat* src, माप_प्रकार nsrcs, Mat* dst, माप_प्रकार ndsts, स्थिर पूर्णांक* fromTo, माप_प्रकार npairs )
{
    CV_INSTRUMENT_REGION();

    अगर( npairs == 0 )
        वापस;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    माप_प्रकार i, j, k, esz1 = dst[0].elemSize1();
    पूर्णांक depth = dst[0].depth();

    Auto_बफर<uchar> buf((nsrcs + ndsts + 1)*(माप(Mat*) + माप(uchar*)) + npairs*(माप(uchar*)*2 + माप(पूर्णांक)*6));
    स्थिर Mat** arrays = (स्थिर Mat**)(uchar*)buf.तथ्य();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    स्थिर uchar** srcs = (स्थिर uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    पूर्णांक* tab = (पूर्णांक*)(dsts + npairs);
    पूर्णांक *sdelta = (पूर्णांक*)(tab + npairs*4), *ddelta = sdelta + npairs;

    क्रम( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    क्रम( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    क्रम( i = 0; i < npairs; i++ )
    {
        पूर्णांक i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        अगर( i0 >= 0 )
        {
            क्रम( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                अगर( i0 < src[j].channels() )
                    अवरोध;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (पूर्णांक)j; tab[i*4+1] = (पूर्णांक)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        अन्यथा
        {
            tab[i*4] = (पूर्णांक)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        क्रम( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            अगर( i1 < dst[j].channels() )
                अवरोध;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (पूर्णांक)(j + nsrcs); tab[i*4+3] = (पूर्णांक)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (पूर्णांक)(nsrcs + ndsts));
    पूर्णांक योग = (पूर्णांक)it.size, blocksize = std::min(योग, (पूर्णांक)((BLOCK_आकार + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    क्रम( i = 0; i < it.nplanes; i++, ++it )
    {
        क्रम( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        क्रम( पूर्णांक t = 0; t < योग; t += blocksize )
        {
            पूर्णांक bsz = std::min(योग - t, blocksize);
            func( (स्थिर व्योम**)srcs, sdelta, (व्योम **)dsts, ddelta, bsz, (पूर्णांक)npairs );

            अगर( t + blocksize < योग )
                क्रम( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#अगर_घोषित HAVE_OPENCL

namespace cv {

अटल व्योम get_UMat_सूचकांक(स्थिर std::vector<UMat> & um, पूर्णांक cn, पूर्णांक & idx, पूर्णांक & cnidx)
{
    पूर्णांक योग_Channels = 0;
    क्रम (माप_प्रकार i = 0, size = um.size(); i < size; ++i)
    {
        पूर्णांक ccn = um[i].channels();
        योग_Channels += ccn;

        अगर (योग_Channels == cn)
        {
            idx = (पूर्णांक)(i + 1);
            cnidx = 0;
            वापस;
        }
        अन्यथा अगर (योग_Channels > cn)
        {
            idx = (पूर्णांक)i;
            cnidx = i == 0 ? cn : (cn - योग_Channels + ccn);
            वापस;
        }
    }

    idx = cnidx = -1;
}

अटल bool ocl_mixChannels(निवेश_Array_Of_Arrays _src, निवेश_Output_Array_Of_Arrays _dst,
                            स्थिर पूर्णांक* fromTo, माप_प्रकार npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    माप_प्रकार nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    आकार size = src[0].size();
    पूर्णांक depth = src[0].depth(), esz = CV_ELEM_आकार(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    क्रम (माप_प्रकार i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    क्रम (माप_प्रकार i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    क्रम (माप_प्रकार i = 0; i < npairs; ++i)
    {
        पूर्णांक scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        पूर्णांक src_idx, src_cnidx, dst_idx, dst_cnidx;

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
                         ocl::memopTypeToStr(depth), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    अगर (k.empty())
        वापस false;

    पूर्णांक argindex = 0;
    क्रम (माप_प्रकार i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::Read_Only_No_आकार(srcargs[i]));
    क्रम (माप_प्रकार i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::Write_Only_No_आकार(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)size.width, ((माप_प्रकार)size.height + rowsPerWI - 1) / rowsPerWI };
    वापस k.run(2, globalsize, शून्य, false);
}

}

#पूर्ण_अगर

व्योम cv::mixChannels(निवेश_Array_Of_Arrays src, निवेश_Output_Array_Of_Arrays dst,
                 स्थिर पूर्णांक* fromTo, माप_प्रकार npairs)
{
    CV_INSTRUMENT_REGION();

    अगर (npairs == 0 || fromTo == शून्य)
        वापस;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    bool src_is_mat = src.kind() != निवेश_Array_2::STD_VECTOR_MAT &&
            src.kind() != निवेश_Array_2::STD_ARRAY_MAT &&
            src.kind() != निवेश_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != निवेश_Array_2::STD_VECTOR_UMAT;
    bool dst_is_mat = dst.kind() != निवेश_Array_2::STD_VECTOR_MAT &&
            dst.kind() != निवेश_Array_2::STD_ARRAY_MAT &&
            dst.kind() != निवेश_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != निवेश_Array_2::STD_VECTOR_UMAT;
    पूर्णांक i;
    पूर्णांक nsrc = src_is_mat ? 1 : (पूर्णांक)src.योग();
    पूर्णांक ndst = dst_is_mat ? 1 : (पूर्णांक)dst.योग();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_बफर<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.तथ्य();
    क्रम( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    क्रम( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

व्योम cv::mixChannels(निवेश_Array_Of_Arrays src, निवेश_Output_Array_Of_Arrays dst,
                     स्थिर std::vector<पूर्णांक>& fromTo)
{
    CV_INSTRUMENT_REGION();

    अगर (fromTo.empty())
        वापस;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    bool src_is_mat = src.kind() != निवेश_Array_2::STD_VECTOR_MAT &&
            src.kind() != निवेश_Array_2::STD_ARRAY_MAT &&
            src.kind() != निवेश_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != निवेश_Array_2::STD_VECTOR_UMAT;
    bool dst_is_mat = dst.kind() != निवेश_Array_2::STD_VECTOR_MAT &&
            dst.kind() != निवेश_Array_2::STD_ARRAY_MAT &&
            dst.kind() != निवेश_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != निवेश_Array_2::STD_VECTOR_UMAT;
    पूर्णांक i;
    पूर्णांक nsrc = src_is_mat ? 1 : (पूर्णांक)src.योग();
    पूर्णांक ndst = dst_is_mat ? 1 : (पूर्णांक)dst.योग();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_बफर<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.तथ्य();
    क्रम( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    क्रम( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#अगर_घोषित HAVE_IPP

namespace cv
{
अटल bool ipp_extractChannel(स्थिर Mat &src, Mat &dst, पूर्णांक channel)
{
#अगर_घोषित HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णांक srcChannels = src.channels();
    पूर्णांक dstChannels = dst.channels();

    अगर(src.dims != dst.dims)
        वापस false;

    अगर(src.dims <= 2)
    {
        Ippi_आकार size = ippi_आकार(src.size());

        वापस CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णांक)src.step, srcChannels, channel, dst.ptr(), (पूर्णांक)dst.step, dstChannels, 0, size, (पूर्णांक)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        स्थिर Mat      *arrays[] = {&dst, शून्य};
        uchar          *ptrs[2]  = {शून्य};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार size = {(पूर्णांक)it.size, 1};

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            अगर(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (पूर्णांक)src.elemSize1()) < 0)
                वापस false;
        }
        वापस true;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    वापस false;
#पूर्ण_अगर
}

अटल bool ipp_जोड़ो_Channel(स्थिर Mat &src, Mat &dst, पूर्णांक channel)
{
#अगर_घोषित HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णांक srcChannels = src.channels();
    पूर्णांक dstChannels = dst.channels();

    अगर(src.dims != dst.dims)
        वापस false;

    अगर(src.dims <= 2)
    {
        Ippi_आकार size = ippi_आकार(src.size());

        वापस CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णांक)src.step, srcChannels, 0, dst.ptr(), (पूर्णांक)dst.step, dstChannels, channel, size, (पूर्णांक)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        स्थिर Mat      *arrays[] = {&dst, शून्य};
        uchar          *ptrs[2]  = {शून्य};
        NAryMatIterator it(arrays, ptrs);

        Ippi_आकार size = {(पूर्णांक)it.size, 1};

        क्रम( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            अगर(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (पूर्णांक)src.elemSize1()) < 0)
                वापस false;
        }
        वापस true;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    वापस false;
#पूर्ण_अगर
}
}
#पूर्ण_अगर

व्योम cv::extractChannel(निवेश_Array _src, OutputArray _dst, पूर्णांक coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    पूर्णांक ch[] = { coi, 0 };

#अगर_घोषित HAVE_OPENCL
    अगर (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_आकार(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        वापस;
    }
#पूर्ण_अगर

    Mat src = _src.getMat();
    _dst.create_Same_आकार(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

व्योम cv::जोड़ो_Channel(निवेश_Array _src, निवेश_Output_Array _dst, पूर्णांक coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    पूर्णांक dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_आकार(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    पूर्णांक ch[] = { 0, coi };
#अगर_घोषित HAVE_OPENCL
    अगर (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        वापस;
    }
#पूर्ण_अगर

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_जोड़ो_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
