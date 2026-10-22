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

ढाँचा<typename T> स्थिर शून्य
mixChannels_( स्थिरांक T** src, स्थिरांक पूर्णांक* sdelta,
              T** dst, स्थिरांक पूर्णांक* ddelta,
              पूर्णांक len, पूर्णांक npairs )
{
    पूर्णांक i, k;
    साठी( k = 0; k < npairs; k++ )
    {
        स्थिरांक T* s = src[k];
        T* d = dst[k];
        पूर्णांक ds = sdelta[k], dd = ddelta[k];
        जर( s )
        {
            साठी( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            जर( i < len )
                d[0] = s[0];
        }
        अन्यथा
        {
            साठी( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            जर( i < len )
                d[0] = 0;
        }
    }
}


स्थिर शून्य mixChannels8u( स्थिरांक शून्य** src, स्थिरांक पूर्णांक* sdelta,
                           शून्य** dst, स्थिरांक पूर्णांक* ddelta,
                           पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिरांक uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels16u( स्थिरांक शून्य** src, स्थिरांक पूर्णांक* sdelta,
                            शून्य** dst, स्थिरांक पूर्णांक* ddelta,
                            पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिरांक ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels32s( स्थिरांक शून्य** src, स्थिरांक पूर्णांक* sdelta,
                            शून्य** dst, स्थिरांक पूर्णांक* ddelta,
                            पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिरांक पूर्णांक**)src, sdelta, (पूर्णांक**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels64s( स्थिरांक शून्य** src, स्थिरांक पूर्णांक* sdelta,
                            शून्य** dst, स्थिरांक पूर्णांक* ddelta,
                            पूर्णांक len, पूर्णांक npairs )
{
    mixChannels_((स्थिरांक int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

प्रकारव्याख्या शून्य (*MixChannelsFunc)( स्थिरांक शून्य** src, स्थिरांक पूर्णांक* sdelta,
        शून्य** dst, स्थिरांक पूर्णांक* ddelta, पूर्णांक len, पूर्णांक npairs );

स्थिर MixChannelsFunc getMixchFunc(पूर्णांक depth)
{
    स्थिर MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    परत mixchTab[depth];
}

} // cv::


शून्य cv::mixChannels( स्थिरांक Mat* src, माप_प्रकार nsrcs, Mat* dst, माप_प्रकार ndsts, स्थिरांक पूर्णांक* fromTo, माप_प्रकार npairs )
{
    CV_INSTRUMENT_REGION();

    जर( npairs == 0 )
        परत;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    माप_प्रकार i, j, k, esz1 = dst[0].elemSize1();
    पूर्णांक depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(आकार(Mat*) + आकार(uchar*)) + npairs*(आकार(uchar*)*2 + आकार(पूर्णांक)*6));
    स्थिरांक Mat** arrays = (स्थिरांक Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    स्थिरांक uchar** srcs = (स्थिरांक uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    पूर्णांक* tab = (पूर्णांक*)(dsts + npairs);
    पूर्णांक *sdelta = (पूर्णांक*)(tab + npairs*4), *ddelta = sdelta + npairs;

    साठी( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    साठी( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    साठी( i = 0; i < npairs; i++ )
    {
        पूर्णांक i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        जर( i0 >= 0 )
        {
            साठी( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                जर( i0 < src[j].channels() )
                    थांबा;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (पूर्णांक)j; tab[i*4+1] = (पूर्णांक)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        अन्यथा
        {
            tab[i*4] = (पूर्णांक)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        साठी( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            जर( i1 < dst[j].channels() )
                थांबा;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (पूर्णांक)(j + nsrcs); tab[i*4+3] = (पूर्णांक)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (पूर्णांक)(nsrcs + ndsts));
    पूर्णांक total = (पूर्णांक)it.size, blocksize = std::min(total, (पूर्णांक)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    साठी( i = 0; i < it.nplanes; i++, ++it )
    {
        साठी( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        साठी( पूर्णांक t = 0; t < total; t += blocksize )
        {
            पूर्णांक bsz = std::min(total - t, blocksize);
            func( (स्थिरांक शून्य**)srcs, sdelta, (शून्य **)dsts, ddelta, bsz, (पूर्णांक)npairs );

            जर( t + blocksize < total )
                साठी( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

नामाकाश cv {

स्थिर शून्य getUMatIndex(स्थिरांक std::vector<UMat> & um, पूर्णांक cn, पूर्णांक & idx, पूर्णांक & cnidx)
{
    पूर्णांक totalChannels = 0;
    साठी (माप_प्रकार i = 0, size = um.size(); i < size; ++i)
    {
        पूर्णांक ccn = um[i].channels();
        totalChannels += ccn;

        जर (totalChannels == cn)
        {
            idx = (पूर्णांक)(i + 1);
            cnidx = 0;
            परत;
        }
        अन्यथा जर (totalChannels > cn)
        {
            idx = (पूर्णांक)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            परत;
        }
    }

    idx = cnidx = -1;
}

स्थिर तार्किक ocl_mixChannels(आदान_Array_Of_Arrays _src, आदान_Output_Array_Of_Arrays _dst,
                            स्थिरांक पूर्णांक* fromTo, माप_प्रकार npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    माप_प्रकार nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    पूर्णांक depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    साठी (माप_प्रकार i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    साठी (माप_प्रकार i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    साठी (माप_प्रकार i = 0; i < npairs; ++i)
    {
        पूर्णांक scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        पूर्णांक src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    जर (k.empty())
        परत असत्य;

    पूर्णांक argindex = 0;
    साठी (माप_प्रकार i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    साठी (माप_प्रकार i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)size.width, ((माप_प्रकार)size.height + rowsPerWI - 1) / rowsPerWI };
    परत k.run(2, globalsize, रिक्त, असत्य);
}

}

#endif

शून्य cv::mixChannels(आदान_Array_Of_Arrays src, आदान_Output_Array_Of_Arrays dst,
                 स्थिरांक पूर्णांक* fromTo, माप_प्रकार npairs)
{
    CV_INSTRUMENT_REGION();

    जर (npairs == 0 || fromTo == रिक्त)
        परत;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    तार्किक src_is_mat = src.kind() != आदान_Array_2::STD_VECTOR_MAT &&
            src.kind() != आदान_Array_2::STD_ARRAY_MAT &&
            src.kind() != आदान_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != आदान_Array_2::STD_VECTOR_UMAT;
    तार्किक dst_is_mat = dst.kind() != आदान_Array_2::STD_VECTOR_MAT &&
            dst.kind() != आदान_Array_2::STD_ARRAY_MAT &&
            dst.kind() != आदान_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != आदान_Array_2::STD_VECTOR_UMAT;
    पूर्णांक i;
    पूर्णांक nsrc = src_is_mat ? 1 : (पूर्णांक)src.total();
    पूर्णांक ndst = dst_is_mat ? 1 : (पूर्णांक)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    साठी( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    साठी( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

शून्य cv::mixChannels(आदान_Array_Of_Arrays src, आदान_Output_Array_Of_Arrays dst,
                     स्थिरांक std::vector<पूर्णांक>& fromTo)
{
    CV_INSTRUMENT_REGION();

    जर (fromTo.empty())
        परत;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    तार्किक src_is_mat = src.kind() != आदान_Array_2::STD_VECTOR_MAT &&
            src.kind() != आदान_Array_2::STD_ARRAY_MAT &&
            src.kind() != आदान_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != आदान_Array_2::STD_VECTOR_UMAT;
    तार्किक dst_is_mat = dst.kind() != आदान_Array_2::STD_VECTOR_MAT &&
            dst.kind() != आदान_Array_2::STD_ARRAY_MAT &&
            dst.kind() != आदान_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != आदान_Array_2::STD_VECTOR_UMAT;
    पूर्णांक i;
    पूर्णांक nsrc = src_is_mat ? 1 : (पूर्णांक)src.total();
    पूर्णांक ndst = dst_is_mat ? 1 : (पूर्णांक)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    साठी( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    साठी( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

नामाकाश cv
{
स्थिर तार्किक ipp_extractChannel(स्थिरांक Mat &src, Mat &dst, पूर्णांक channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णांक srcChannels = src.channels();
    पूर्णांक dstChannels = dst.channels();

    जर(src.dims != dst.dims)
        परत असत्य;

    जर(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        परत CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णांक)src.step, srcChannels, channel, dst.ptr(), (पूर्णांक)dst.step, dstChannels, 0, size, (पूर्णांक)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        स्थिरांक Mat      *arrays[] = {&dst, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(पूर्णांक)it.size, 1};

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            जर(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (पूर्णांक)src.elemSize1()) < 0)
                परत असत्य;
        }
        परत सत्य;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    परत असत्य;
#endif
}

स्थिर तार्किक ipp_घाला_Channel(स्थिरांक Mat &src, Mat &dst, पूर्णांक channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णांक srcChannels = src.channels();
    पूर्णांक dstChannels = dst.channels();

    जर(src.dims != dst.dims)
        परत असत्य;

    जर(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        परत CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णांक)src.step, srcChannels, 0, dst.ptr(), (पूर्णांक)dst.step, dstChannels, channel, size, (पूर्णांक)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        स्थिरांक Mat      *arrays[] = {&dst, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(पूर्णांक)it.size, 1};

        साठी( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            जर(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (पूर्णांक)src.elemSize1()) < 0)
                परत असत्य;
        }
        परत सत्य;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    परत असत्य;
#endif
}
}
#endif

शून्य cv::extractChannel(आदान_Array _src, OutputArray _dst, पूर्णांक coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    पूर्णांक ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    जर (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        परत;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

शून्य cv::घाला_Channel(आदान_Array _src, आदान_Output_Array _dst, पूर्णांक coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णांक stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    पूर्णांक dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    पूर्णांक ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    जर (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        परत;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_घाला_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
