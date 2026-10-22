// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"
#அந்தர்பாவ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

நாமாகாஶ cv
{

template<typename T> நிலையான வெறுமை
mixChannels_( மாறிலி T** src, மாறிலி முழு* sdelta,
              T** dst, மாறிலி முழு* ddelta,
              முழு len, முழு npairs )
{
    முழு i, k;
    க்கு( k = 0; k < npairs; k++ )
    {
        மாறிலி T* s = src[k];
        T* d = dst[k];
        முழு ds = sdelta[k], dd = ddelta[k];
        எனில்( s )
        {
            க்கு( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            எனில்( i < len )
                d[0] = s[0];
        }
        இல்லையெனில்
        {
            க்கு( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            எனில்( i < len )
                d[0] = 0;
        }
    }
}


நிலையான வெறுமை mixChannels8u( மாறிலி வெறுமை** src, மாறிலி முழு* sdelta,
                           வெறுமை** dst, மாறிலி முழு* ddelta,
                           முழு len, முழு npairs )
{
    mixChannels_((மாறிலி uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

நிலையான வெறுமை mixChannels16u( மாறிலி வெறுமை** src, மாறிலி முழு* sdelta,
                            வெறுமை** dst, மாறிலி முழு* ddelta,
                            முழு len, முழு npairs )
{
    mixChannels_((மாறிலி ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

நிலையான வெறுமை mixChannels32s( மாறிலி வெறுமை** src, மாறிலி முழு* sdelta,
                            வெறுமை** dst, மாறிலி முழு* ddelta,
                            முழு len, முழு npairs )
{
    mixChannels_((மாறிலி முழு**)src, sdelta, (முழு**)dst, ddelta, len, npairs);
}

நிலையான வெறுமை mixChannels64s( மாறிலி வெறுமை** src, மாறிலி முழு* sdelta,
                            வெறுமை** dst, மாறிலி முழு* ddelta,
                            முழு len, முழு npairs )
{
    mixChannels_((மாறிலி int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

வகைவரை வெறுமை (*MixChannelsFunc)( மாறிலி வெறுமை** src, மாறிலி முழு* sdelta,
        வெறுமை** dst, மாறிலி முழு* ddelta, முழு len, முழு npairs );

நிலையான MixChannelsFunc getMixchFunc(முழு depth)
{
    நிலையான MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    திரும்பு mixchTab[depth];
}

} // cv::


வெறுமை cv::mixChannels( மாறிலி Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, மாறிலி முழு* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    எனில்( npairs == 0 )
        திரும்பு;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    முழு depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(அளவு(Mat*) + அளவு(uchar*)) + npairs*(அளவு(uchar*)*2 + அளவு(முழு)*6));
    மாறிலி Mat** arrays = (மாறிலி Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    மாறிலி uchar** srcs = (மாறிலி uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    முழு* tab = (முழு*)(dsts + npairs);
    முழு *sdelta = (முழு*)(tab + npairs*4), *ddelta = sdelta + npairs;

    க்கு( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    க்கு( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    க்கு( i = 0; i < npairs; i++ )
    {
        முழு i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        எனில்( i0 >= 0 )
        {
            க்கு( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                எனில்( i0 < src[j].channels() )
                    நிறுத்து;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (முழு)j; tab[i*4+1] = (முழு)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        இல்லையெனில்
        {
            tab[i*4] = (முழு)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        க்கு( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            எனில்( i1 < dst[j].channels() )
                நிறுத்து;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (முழு)(j + nsrcs); tab[i*4+3] = (முழு)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (முழு)(nsrcs + ndsts));
    முழு total = (முழு)it.size, blocksize = std::min(total, (முழு)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    க்கு( i = 0; i < it.nplanes; i++, ++it )
    {
        க்கு( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        க்கு( முழு t = 0; t < total; t += blocksize )
        {
            முழு bsz = std::min(total - t, blocksize);
            func( (மாறிலி வெறுமை**)srcs, sdelta, (வெறுமை **)dsts, ddelta, bsz, (முழு)npairs );

            எனில்( t + blocksize < total )
                க்கு( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

நாமாகாஶ cv {

நிலையான வெறுமை getUMatIndex(மாறிலி std::vector<UMat> & um, முழு cn, முழு & idx, முழு & cnidx)
{
    முழு totalChannels = 0;
    க்கு (size_t i = 0, size = um.size(); i < size; ++i)
    {
        முழு ccn = um[i].channels();
        totalChannels += ccn;

        எனில் (totalChannels == cn)
        {
            idx = (முழு)(i + 1);
            cnidx = 0;
            திரும்பு;
        }
        இல்லையெனில் எனில் (totalChannels > cn)
        {
            idx = (முழு)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            திரும்பு;
        }
    }

    idx = cnidx = -1;
}

நிலையான தருக்க ocl_mixChannels(உள்ளீடு_Array_Of_Arrays _src, உள்ளீடு_Output_Array_Of_Arrays _dst,
                            மாறிலி முழு* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    முழு depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    க்கு (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    க்கு (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    க்கு (size_t i = 0; i < npairs; ++i)
    {
        முழு scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        முழு src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    எனில் (k.empty())
        திரும்பு பொய்;

    முழு argindex = 0;
    க்கு (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    க்கு (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    திரும்பு k.run(2, globalsize, வெற்று, பொய்);
}

}

#endif

வெறுமை cv::mixChannels(உள்ளீடு_Array_Of_Arrays src, உள்ளீடு_Output_Array_Of_Arrays dst,
                 மாறிலி முழு* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    எனில் (npairs == 0 || fromTo == வெற்று)
        திரும்பு;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    தருக்க src_is_mat = src.kind() != உள்ளீடு_Array_2::STD_VECTOR_MAT &&
            src.kind() != உள்ளீடு_Array_2::STD_ARRAY_MAT &&
            src.kind() != உள்ளீடு_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != உள்ளீடு_Array_2::STD_VECTOR_UMAT;
    தருக்க dst_is_mat = dst.kind() != உள்ளீடு_Array_2::STD_VECTOR_MAT &&
            dst.kind() != உள்ளீடு_Array_2::STD_ARRAY_MAT &&
            dst.kind() != உள்ளீடு_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != உள்ளீடு_Array_2::STD_VECTOR_UMAT;
    முழு i;
    முழு nsrc = src_is_mat ? 1 : (முழு)src.total();
    முழு ndst = dst_is_mat ? 1 : (முழு)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    க்கு( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    க்கு( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

வெறுமை cv::mixChannels(உள்ளீடு_Array_Of_Arrays src, உள்ளீடு_Output_Array_Of_Arrays dst,
                     மாறிலி std::vector<முழு>& fromTo)
{
    CV_INSTRUMENT_REGION();

    எனில் (fromTo.empty())
        திரும்பு;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    தருக்க src_is_mat = src.kind() != உள்ளீடு_Array_2::STD_VECTOR_MAT &&
            src.kind() != உள்ளீடு_Array_2::STD_ARRAY_MAT &&
            src.kind() != உள்ளீடு_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != உள்ளீடு_Array_2::STD_VECTOR_UMAT;
    தருக்க dst_is_mat = dst.kind() != உள்ளீடு_Array_2::STD_VECTOR_MAT &&
            dst.kind() != உள்ளீடு_Array_2::STD_ARRAY_MAT &&
            dst.kind() != உள்ளீடு_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != உள்ளீடு_Array_2::STD_VECTOR_UMAT;
    முழு i;
    முழு nsrc = src_is_mat ? 1 : (முழு)src.total();
    முழு ndst = dst_is_mat ? 1 : (முழு)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    க்கு( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    க்கு( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

நாமாகாஶ cv
{
நிலையான தருக்க ipp_extractChannel(மாறிலி Mat &src, Mat &dst, முழு channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    முழு srcChannels = src.channels();
    முழு dstChannels = dst.channels();

    எனில்(src.dims != dst.dims)
        திரும்பு பொய்;

    எனில்(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        திரும்பு CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (முழு)src.step, srcChannels, channel, dst.ptr(), (முழு)dst.step, dstChannels, 0, size, (முழு)src.elemSize1()) >= 0;
    }
    இல்லையெனில்
    {
        மாறிலி Mat      *arrays[] = {&dst, வெற்று};
        uchar          *ptrs[2]  = {வெற்று};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(முழு)it.size, 1};

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (முழு)src.elemSize1()) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
#இல்லையெனில்
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    திரும்பு பொய்;
#endif
}

நிலையான தருக்க ipp_செருகு_Channel(மாறிலி Mat &src, Mat &dst, முழு channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    முழு srcChannels = src.channels();
    முழு dstChannels = dst.channels();

    எனில்(src.dims != dst.dims)
        திரும்பு பொய்;

    எனில்(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        திரும்பு CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (முழு)src.step, srcChannels, 0, dst.ptr(), (முழு)dst.step, dstChannels, channel, size, (முழு)src.elemSize1()) >= 0;
    }
    இல்லையெனில்
    {
        மாறிலி Mat      *arrays[] = {&dst, வெற்று};
        uchar          *ptrs[2]  = {வெற்று};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(முழு)it.size, 1};

        க்கு( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            எனில்(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (முழு)src.elemSize1()) < 0)
                திரும்பு பொய்;
        }
        திரும்பு உண்மை;
    }
#இல்லையெனில்
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    திரும்பு பொய்;
#endif
}
}
#endif

வெறுமை cv::extractChannel(உள்ளீடு_Array _src, OutputArray _dst, முழு coi)
{
    CV_INSTRUMENT_REGION();

    முழு type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    முழு ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    எனில் (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        திரும்பு;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

வெறுமை cv::செருகு_Channel(உள்ளீடு_Array _src, உள்ளீடு_Output_Array _dst, முழு coi)
{
    CV_INSTRUMENT_REGION();

    முழு stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    முழு dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    முழு ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    எனில் (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        திரும்பு;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_செருகு_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
