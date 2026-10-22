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
mixChannels_( अचर T** src, अचर पूर्णाङ्क* sdelta,
              T** dst, अचर पूर्णाङ्क* ddelta,
              पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    पूर्णाङ्क i, k;
    लागि( k = 0; k < npairs; k++ )
    {
        अचर T* s = src[k];
        T* d = dst[k];
        पूर्णाङ्क ds = sdelta[k], dd = ddelta[k];
        यदि( s )
        {
            लागि( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            यदि( i < len )
                d[0] = s[0];
        }
        अन्यथा
        {
            लागि( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            यदि( i < len )
                d[0] = 0;
        }
    }
}


स्थिर शून्य mixChannels8u( अचर शून्य** src, अचर पूर्णाङ्क* sdelta,
                           शून्य** dst, अचर पूर्णाङ्क* ddelta,
                           पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((अचर uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels16u( अचर शून्य** src, अचर पूर्णाङ्क* sdelta,
                            शून्य** dst, अचर पूर्णाङ्क* ddelta,
                            पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((अचर ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels32s( अचर शून्य** src, अचर पूर्णाङ्क* sdelta,
                            शून्य** dst, अचर पूर्णाङ्क* ddelta,
                            पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((अचर पूर्णाङ्क**)src, sdelta, (पूर्णाङ्क**)dst, ddelta, len, npairs);
}

स्थिर शून्य mixChannels64s( अचर शून्य** src, अचर पूर्णाङ्क* sdelta,
                            शून्य** dst, अचर पूर्णाङ्क* ddelta,
                            पूर्णाङ्क len, पूर्णाङ्क npairs )
{
    mixChannels_((अचर int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

प्रकारनिर्देश शून्य (*MixChannelsFunc)( अचर शून्य** src, अचर पूर्णाङ्क* sdelta,
        शून्य** dst, अचर पूर्णाङ्क* ddelta, पूर्णाङ्क len, पूर्णाङ्क npairs );

स्थिर MixChannelsFunc getMixchFunc(पूर्णाङ्क depth)
{
    स्थिर MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    फर्काऊ mixchTab[depth];
}

} // cv::


शून्य cv::mixChannels( अचर Mat* src, माप_प्रकार nsrcs, Mat* dst, माप_प्रकार ndsts, अचर पूर्णाङ्क* fromTo, माप_प्रकार npairs )
{
    CV_INSTRUMENT_REGION();

    यदि( npairs == 0 )
        फर्काऊ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    माप_प्रकार i, j, k, esz1 = dst[0].elemSize1();
    पूर्णाङ्क depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(परिमाण(Mat*) + परिमाण(uchar*)) + npairs*(परिमाण(uchar*)*2 + परिमाण(पूर्णाङ्क)*6));
    अचर Mat** arrays = (अचर Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    अचर uchar** srcs = (अचर uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    पूर्णाङ्क* tab = (पूर्णाङ्क*)(dsts + npairs);
    पूर्णाङ्क *sdelta = (पूर्णाङ्क*)(tab + npairs*4), *ddelta = sdelta + npairs;

    लागि( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    लागि( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    लागि( i = 0; i < npairs; i++ )
    {
        पूर्णाङ्क i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        यदि( i0 >= 0 )
        {
            लागि( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                यदि( i0 < src[j].channels() )
                    रोक;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (पूर्णाङ्क)j; tab[i*4+1] = (पूर्णाङ्क)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        अन्यथा
        {
            tab[i*4] = (पूर्णाङ्क)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        लागि( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            यदि( i1 < dst[j].channels() )
                रोक;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (पूर्णाङ्क)(j + nsrcs); tab[i*4+3] = (पूर्णाङ्क)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (पूर्णाङ्क)(nsrcs + ndsts));
    पूर्णाङ्क total = (पूर्णाङ्क)it.size, blocksize = std::min(total, (पूर्णाङ्क)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    लागि( i = 0; i < it.nplanes; i++, ++it )
    {
        लागि( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        लागि( पूर्णाङ्क t = 0; t < total; t += blocksize )
        {
            पूर्णाङ्क bsz = std::min(total - t, blocksize);
            func( (अचर शून्य**)srcs, sdelta, (शून्य **)dsts, ddelta, bsz, (पूर्णाङ्क)npairs );

            यदि( t + blocksize < total )
                लागि( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

नामाकाश cv {

स्थिर शून्य getUMatIndex(अचर std::vector<UMat> & um, पूर्णाङ्क cn, पूर्णाङ्क & idx, पूर्णाङ्क & cnidx)
{
    पूर्णाङ्क totalChannels = 0;
    लागि (माप_प्रकार i = 0, size = um.size(); i < size; ++i)
    {
        पूर्णाङ्क ccn = um[i].channels();
        totalChannels += ccn;

        यदि (totalChannels == cn)
        {
            idx = (पूर्णाङ्क)(i + 1);
            cnidx = 0;
            फर्काऊ;
        }
        अन्यथा यदि (totalChannels > cn)
        {
            idx = (पूर्णाङ्क)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            फर्काऊ;
        }
    }

    idx = cnidx = -1;
}

स्थिर तार्किक ocl_mixChannels(आगत_Array_Of_Arrays _src, आगत_Output_Array_Of_Arrays _dst,
                            अचर पूर्णाङ्क* fromTo, माप_प्रकार npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    माप_प्रकार nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    पूर्णाङ्क depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    लागि (माप_प्रकार i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    लागि (माप_प्रकार i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    लागि (माप_प्रकार i = 0; i < npairs; ++i)
    {
        पूर्णाङ्क scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        पूर्णाङ्क src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    यदि (k.empty())
        फर्काऊ असत्य;

    पूर्णाङ्क argindex = 0;
    लागि (माप_प्रकार i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    लागि (माप_प्रकार i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    माप_प्रकार globalsize[2] = { (माप_प्रकार)size.width, ((माप_प्रकार)size.height + rowsPerWI - 1) / rowsPerWI };
    फर्काऊ k.run(2, globalsize, रिक्त, असत्य);
}

}

#endif

शून्य cv::mixChannels(आगत_Array_Of_Arrays src, आगत_Output_Array_Of_Arrays dst,
                 अचर पूर्णाङ्क* fromTo, माप_प्रकार npairs)
{
    CV_INSTRUMENT_REGION();

    यदि (npairs == 0 || fromTo == रिक्त)
        फर्काऊ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    तार्किक src_is_mat = src.kind() != आगत_Array_2::STD_VECTOR_MAT &&
            src.kind() != आगत_Array_2::STD_ARRAY_MAT &&
            src.kind() != आगत_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != आगत_Array_2::STD_VECTOR_UMAT;
    तार्किक dst_is_mat = dst.kind() != आगत_Array_2::STD_VECTOR_MAT &&
            dst.kind() != आगत_Array_2::STD_ARRAY_MAT &&
            dst.kind() != आगत_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != आगत_Array_2::STD_VECTOR_UMAT;
    पूर्णाङ्क i;
    पूर्णाङ्क nsrc = src_is_mat ? 1 : (पूर्णाङ्क)src.total();
    पूर्णाङ्क ndst = dst_is_mat ? 1 : (पूर्णाङ्क)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    लागि( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    लागि( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

शून्य cv::mixChannels(आगत_Array_Of_Arrays src, आगत_Output_Array_Of_Arrays dst,
                     अचर std::vector<पूर्णाङ्क>& fromTo)
{
    CV_INSTRUMENT_REGION();

    यदि (fromTo.empty())
        फर्काऊ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    तार्किक src_is_mat = src.kind() != आगत_Array_2::STD_VECTOR_MAT &&
            src.kind() != आगत_Array_2::STD_ARRAY_MAT &&
            src.kind() != आगत_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != आगत_Array_2::STD_VECTOR_UMAT;
    तार्किक dst_is_mat = dst.kind() != आगत_Array_2::STD_VECTOR_MAT &&
            dst.kind() != आगत_Array_2::STD_ARRAY_MAT &&
            dst.kind() != आगत_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != आगत_Array_2::STD_VECTOR_UMAT;
    पूर्णाङ्क i;
    पूर्णाङ्क nsrc = src_is_mat ? 1 : (पूर्णाङ्क)src.total();
    पूर्णाङ्क ndst = dst_is_mat ? 1 : (पूर्णाङ्क)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    लागि( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    लागि( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

नामाकाश cv
{
स्थिर तार्किक ipp_extractChannel(अचर Mat &src, Mat &dst, पूर्णाङ्क channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णाङ्क srcChannels = src.channels();
    पूर्णाङ्क dstChannels = dst.channels();

    यदि(src.dims != dst.dims)
        फर्काऊ असत्य;

    यदि(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        फर्काऊ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णाङ्क)src.step, srcChannels, channel, dst.ptr(), (पूर्णाङ्क)dst.step, dstChannels, 0, size, (पूर्णाङ्क)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        अचर Mat      *arrays[] = {&dst, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(पूर्णाङ्क)it.size, 1};

        लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (पूर्णाङ्क)src.elemSize1()) < 0)
                फर्काऊ असत्य;
        }
        फर्काऊ सत्य;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    फर्काऊ असत्य;
#endif
}

स्थिर तार्किक ipp_घुसाउ_Channel(अचर Mat &src, Mat &dst, पूर्णाङ्क channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    पूर्णाङ्क srcChannels = src.channels();
    पूर्णाङ्क dstChannels = dst.channels();

    यदि(src.dims != dst.dims)
        फर्काऊ असत्य;

    यदि(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        फर्काऊ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (पूर्णाङ्क)src.step, srcChannels, 0, dst.ptr(), (पूर्णाङ्क)dst.step, dstChannels, channel, size, (पूर्णाङ्क)src.elemSize1()) >= 0;
    }
    अन्यथा
    {
        अचर Mat      *arrays[] = {&dst, रिक्त};
        uchar          *ptrs[2]  = {रिक्त};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(पूर्णाङ्क)it.size, 1};

        लागि( माप_प्रकार i = 0; i < it.nplanes; i++, ++it )
        {
            यदि(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (पूर्णाङ्क)src.elemSize1()) < 0)
                फर्काऊ असत्य;
        }
        फर्काऊ सत्य;
    }
#अन्यथा
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    फर्काऊ असत्य;
#endif
}
}
#endif

शून्य cv::extractChannel(आगत_Array _src, OutputArray _dst, पूर्णाङ्क coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    पूर्णाङ्क ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    यदि (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        फर्काऊ;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

शून्य cv::घुसाउ_Channel(आगत_Array _src, आगत_Output_Array _dst, पूर्णाङ्क coi)
{
    CV_INSTRUMENT_REGION();

    पूर्णाङ्क stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    पूर्णाङ्क dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    पूर्णाङ्क ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    यदि (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        फर्काऊ;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_घुसाउ_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
