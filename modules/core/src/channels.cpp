// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#అన్తర్భావ "precomp.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"
#అన్తర్భావ "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

నామాకాశ cv
{

మూస<రకం_పేరు T> స్థిర శూన్య
mixChannels_( స్థిరరాశి T** src, స్థిరరాశి పూర్ణాంకం* sdelta,
              T** dst, స్థిరరాశి పూర్ణాంకం* ddelta,
              పూర్ణాంకం len, పూర్ణాంకం npairs )
{
    పూర్ణాంకం i, k;
    కొరకు( k = 0; k < npairs; k++ )
    {
        స్థిరరాశి T* s = src[k];
        T* d = dst[k];
        పూర్ణాంకం ds = sdelta[k], dd = ddelta[k];
        ఒకవేళ( s )
        {
            కొరకు( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            ఒకవేళ( i < len )
                d[0] = s[0];
        }
        లేకపోతే
        {
            కొరకు( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            ఒకవేళ( i < len )
                d[0] = 0;
        }
    }
}


స్థిర శూన్య mixChannels8u( స్థిరరాశి శూన్య** src, స్థిరరాశి పూర్ణాంకం* sdelta,
                           శూన్య** dst, స్థిరరాశి పూర్ణాంకం* ddelta,
                           పూర్ణాంకం len, పూర్ణాంకం npairs )
{
    mixChannels_((స్థిరరాశి uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

స్థిర శూన్య mixChannels16u( స్థిరరాశి శూన్య** src, స్థిరరాశి పూర్ణాంకం* sdelta,
                            శూన్య** dst, స్థిరరాశి పూర్ణాంకం* ddelta,
                            పూర్ణాంకం len, పూర్ణాంకం npairs )
{
    mixChannels_((స్థిరరాశి ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

స్థిర శూన్య mixChannels32s( స్థిరరాశి శూన్య** src, స్థిరరాశి పూర్ణాంకం* sdelta,
                            శూన్య** dst, స్థిరరాశి పూర్ణాంకం* ddelta,
                            పూర్ణాంకం len, పూర్ణాంకం npairs )
{
    mixChannels_((స్థిరరాశి పూర్ణాంకం**)src, sdelta, (పూర్ణాంకం**)dst, ddelta, len, npairs);
}

స్థిర శూన్య mixChannels64s( స్థిరరాశి శూన్య** src, స్థిరరాశి పూర్ణాంకం* sdelta,
                            శూన్య** dst, స్థిరరాశి పూర్ణాంకం* ddelta,
                            పూర్ణాంకం len, పూర్ణాంకం npairs )
{
    mixChannels_((స్థిరరాశి int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

రకనిర్వచనం శూన్య (*MixChannelsFunc)( స్థిరరాశి శూన్య** src, స్థిరరాశి పూర్ణాంకం* sdelta,
        శూన్య** dst, స్థిరరాశి పూర్ణాంకం* ddelta, పూర్ణాంకం len, పూర్ణాంకం npairs );

స్థిర MixChannelsFunc getMixchFunc(పూర్ణాంకం లోతు)
{
    స్థిర MixChannelsFunc mixchTab[CV_లోతు_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    తిరిగి mixchTab[లోతు];
}

} // cv::


శూన్య cv::mixChannels( స్థిరరాశి Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, స్థిరరాశి పూర్ణాంకం* from_గమ్యం, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ( npairs == 0 )
        తిరిగి;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_గమ్యం && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    పూర్ణాంకం లోతు = dst[0].లోతు();

    Auto_బఫరు<uchar> buf((nsrcs + ndsts + 1)*(పరిమాణం(Mat*) + పరిమాణం(uchar*)) + npairs*(పరిమాణం(uchar*)*2 + పరిమాణం(పూర్ణాంకం)*6));
    స్థిరరాశి Mat** arrays = (స్థిరరాశి Mat**)(uchar*)buf.దత్తాంశం();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    స్థిరరాశి uchar** srcs = (స్థిరరాశి uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    పూర్ణాంకం* tab = (పూర్ణాంకం*)(dsts + npairs);
    పూర్ణాంకం *sdelta = (పూర్ణాంకం*)(tab + npairs*4), *ddelta = sdelta + npairs;

    కొరకు( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].లోతు() == లోతు );
        arrays[i] = &src[i];
    }
    కొరకు( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].లోతు() == లోతు );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    కొరకు( i = 0; i < npairs; i++ )
    {
        పూర్ణాంకం i0 = from_గమ్యం[i*2], i1 = from_గమ్యం[i*2+1];
        ఒకవేళ( i0 >= 0 )
        {
            కొరకు( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                ఒకవేళ( i0 < src[j].channels() )
                    ఆపు;
            CV_Assert(j < nsrcs && src[j].లోతు() == లోతు);
            tab[i*4] = (పూర్ణాంకం)j; tab[i*4+1] = (పూర్ణాంకం)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        లేకపోతే
        {
            tab[i*4] = (పూర్ణాంకం)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        కొరకు( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            ఒకవేళ( i1 < dst[j].channels() )
                ఆపు;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].లోతు() == లోతు);
        tab[i*4+2] = (పూర్ణాంకం)(j + nsrcs); tab[i*4+3] = (పూర్ణాంకం)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (పూర్ణాంకం)(nsrcs + ndsts));
    పూర్ణాంకం మొత్తం = (పూర్ణాంకం)it.size, blocksize = std::min(మొత్తం, (పూర్ణాంకం)((ఖండం_కొలత + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(లోతు);
    CV_Assert(func);

    కొరకు( i = 0; i < it.nplanes; i++, ++it )
    {
        కొరకు( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        కొరకు( పూర్ణాంకం t = 0; t < మొత్తం; t += blocksize )
        {
            పూర్ణాంకం bsz = std::min(మొత్తం - t, blocksize);
            func( (స్థిరరాశి శూన్య**)srcs, sdelta, (శూన్య **)dsts, ddelta, bsz, (పూర్ణాంకం)npairs );

            ఒకవేళ( t + blocksize < మొత్తం )
                కొరకు( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#నిర్వచితమైతే HAVE_OPENCL

నామాకాశ cv {

స్థిర శూన్య get_UMat_సూచీ(స్థిరరాశి std::vector<UMat> & um, పూర్ణాంకం cn, పూర్ణాంకం & idx, పూర్ణాంకం & cnidx)
{
    పూర్ణాంకం మొత్తం_Channels = 0;
    కొరకు (size_t i = 0, size = um.size(); i < size; ++i)
    {
        పూర్ణాంకం ccn = um[i].channels();
        మొత్తం_Channels += ccn;

        ఒకవేళ (మొత్తం_Channels == cn)
        {
            idx = (పూర్ణాంకం)(i + 1);
            cnidx = 0;
            తిరిగి;
        }
        లేకపోతే ఒకవేళ (మొత్తం_Channels > cn)
        {
            idx = (పూర్ణాంకం)i;
            cnidx = i == 0 ? cn : (cn - మొత్తం_Channels + ccn);
            తిరిగి;
        }
    }

    idx = cnidx = -1;
}

స్థిర తార్కిక ocl_mixChannels(ఆదానం_Array_యొక్క_Arrays _src, ఆదానం_Output_Array_యొక్క_Arrays _dst,
                            స్థిరరాశి పూర్ణాంకం* from_గమ్యం, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_సదిశ(src);
    _dst.get_UMat_సదిశ(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    కొలత size = src[0].size();
    పూర్ణాంకం లోతు = src[0].లోతు(), esz = CV_ELEM_కొలత(లోతు),
            అడ్డువరుసలు_చొప్పున_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    కొరకు (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].లోతు() == లోతు);
    కొరకు (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].లోతు() == లోతు);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    కొరకు (size_t i = 0; i < npairs; ++i)
    {
        పూర్ణాంకం scn = from_గమ్యం[i<<1], dcn = from_గమ్యం[(i<<1) + 1];
        పూర్ణాంకం src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_సూచీ(src, scn, src_idx, src_cnidx);
        get_UMat_సూచీ(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_గమ్యం_Str(లోతు), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    పూర్ణాంకం argindex = 0;
    కొరకు (size_t i = 0; i < npairs; ++i)
        argindex = k.సమితి(argindex, ocl::KernelArg::Read_Only_No_కొలత(srcargs[i]));
    కొరకు (size_t i = 0; i < npairs; ++i)
        argindex = k.సమితి(argindex, ocl::KernelArg::Write_Only_No_కొలత(dstargs[i]));
    argindex = k.సమితి(argindex, size.ఎత్తు);
    argindex = k.సమితి(argindex, size.వెడల్పు);
    k.సమితి(argindex, అడ్డువరుసలు_చొప్పున_WI);

    size_t globalsize[2] = { (size_t)size.వెడల్పు, ((size_t)size.ఎత్తు + అడ్డువరుసలు_చొప్పున_WI - 1) / అడ్డువరుసలు_చొప్పున_WI };
    తిరిగి k.నిరంతరం(2, globalsize, ఖాళీ, అబద్ధం);
}

}

#అయితే_ముగింపు

శూన్య cv::mixChannels(ఆదానం_Array_యొక్క_Arrays src, ఆదానం_Output_Array_యొక్క_Arrays dst,
                 స్థిరరాశి పూర్ణాంకం* from_గమ్యం, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (npairs == 0 || from_గమ్యం == ఖాళీ)
        తిరిగి;

    CV_OCL_నిరంతరం(dst.is_UMat_సదిశ(),
               ocl_mixChannels(src, dst, from_గమ్యం, npairs))

    తార్కిక src_is_mat = src.రకం() != ఆదానం_Array_2::STD_సదిశ_MAT &&
            src.రకం() != ఆదానం_Array_2::STD_ARRAY_MAT &&
            src.రకం() != ఆదానం_Array_2::STD_సదిశ_సదిశ &&
            src.రకం() != ఆదానం_Array_2::STD_సదిశ_UMAT;
    తార్కిక dst_is_mat = dst.రకం() != ఆదానం_Array_2::STD_సదిశ_MAT &&
            dst.రకం() != ఆదానం_Array_2::STD_ARRAY_MAT &&
            dst.రకం() != ఆదానం_Array_2::STD_సదిశ_సదిశ &&
            dst.రకం() != ఆదానం_Array_2::STD_సదిశ_UMAT;
    పూర్ణాంకం i;
    పూర్ణాంకం nsrc = src_is_mat ? 1 : (పూర్ణాంకం)src.మొత్తం();
    పూర్ణాంకం ndst = dst_is_mat ? 1 : (పూర్ణాంకం)dst.మొత్తం();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_బఫరు<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.దత్తాంశం();
    కొరకు( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    కొరకు( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_గమ్యం, npairs);
}

శూన్య cv::mixChannels(ఆదానం_Array_యొక్క_Arrays src, ఆదానం_Output_Array_యొక్క_Arrays dst,
                     స్థిరరాశి std::vector<పూర్ణాంకం>& from_గమ్యం)
{
    CV_INSTRUMENT_REGION();

    ఒకవేళ (from_గమ్యం.empty())
        తిరిగి;

    CV_OCL_నిరంతరం(dst.is_UMat_సదిశ(),
               ocl_mixChannels(src, dst, &from_గమ్యం[0], from_గమ్యం.size()>>1))

    తార్కిక src_is_mat = src.రకం() != ఆదానం_Array_2::STD_సదిశ_MAT &&
            src.రకం() != ఆదానం_Array_2::STD_ARRAY_MAT &&
            src.రకం() != ఆదానం_Array_2::STD_సదిశ_సదిశ &&
            src.రకం() != ఆదానం_Array_2::STD_సదిశ_UMAT;
    తార్కిక dst_is_mat = dst.రకం() != ఆదానం_Array_2::STD_సదిశ_MAT &&
            dst.రకం() != ఆదానం_Array_2::STD_ARRAY_MAT &&
            dst.రకం() != ఆదానం_Array_2::STD_సదిశ_సదిశ &&
            dst.రకం() != ఆదానం_Array_2::STD_సదిశ_UMAT;
    పూర్ణాంకం i;
    పూర్ణాంకం nsrc = src_is_mat ? 1 : (పూర్ణాంకం)src.మొత్తం();
    పూర్ణాంకం ndst = dst_is_mat ? 1 : (పూర్ణాంకం)dst.మొత్తం();

    CV_Assert(from_గమ్యం.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_బఫరు<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.దత్తాంశం();
    కొరకు( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    కొరకు( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_గమ్యం[0], from_గమ్యం.size()/2);
}

#నిర్వచితమైతే HAVE_IPP

నామాకాశ cv
{
స్థిర తార్కిక ipp_వెలికితీయి_Channel(స్థిరరాశి Mat &src, Mat &dst, పూర్ణాంకం channel)
{
#నిర్వచితమైతే HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    పూర్ణాంకం srcChannels = src.channels();
    పూర్ణాంకం dstChannels = dst.channels();

    ఒకవేళ(src.dims != dst.dims)
        తిరిగి అబద్ధం;

    ఒకవేళ(src.dims <= 2)
    {
        Ippi_కొలత size = ippi_కొలత(src.size());

        తిరిగి CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (పూర్ణాంకం)src.అడుగు, srcChannels, channel, dst.ptr(), (పూర్ణాంకం)dst.అడుగు, dstChannels, 0, size, (పూర్ణాంకం)src.elemSize1()) >= 0;
    }
    లేకపోతే
    {
        స్థిరరాశి Mat      *arrays[] = {&dst, ఖాళీ};
        uchar          *ptrs[2]  = {ఖాళీ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_కొలత size = {(పూర్ణాంకం)it.size, 1};

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (పూర్ణాంకం)src.elemSize1()) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
#లేకపోతే
    CV_వాడని(src); CV_వాడని(dst); CV_వాడని(channel);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

స్థిర తార్కిక ipp_చొప్పించు_Channel(స్థిరరాశి Mat &src, Mat &dst, పూర్ణాంకం channel)
{
#నిర్వచితమైతే HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    పూర్ణాంకం srcChannels = src.channels();
    పూర్ణాంకం dstChannels = dst.channels();

    ఒకవేళ(src.dims != dst.dims)
        తిరిగి అబద్ధం;

    ఒకవేళ(src.dims <= 2)
    {
        Ippi_కొలత size = ippi_కొలత(src.size());

        తిరిగి CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (పూర్ణాంకం)src.అడుగు, srcChannels, 0, dst.ptr(), (పూర్ణాంకం)dst.అడుగు, dstChannels, channel, size, (పూర్ణాంకం)src.elemSize1()) >= 0;
    }
    లేకపోతే
    {
        స్థిరరాశి Mat      *arrays[] = {&dst, ఖాళీ};
        uchar          *ptrs[2]  = {ఖాళీ};
        NAryMatIterator it(arrays, ptrs);

        Ippi_కొలత size = {(పూర్ణాంకం)it.size, 1};

        కొరకు( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ఒకవేళ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (పూర్ణాంకం)src.elemSize1()) < 0)
                తిరిగి అబద్ధం;
        }
        తిరిగి నిజం;
    }
#లేకపోతే
    CV_వాడని(src); CV_వాడని(dst); CV_వాడని(channel);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}
}
#అయితే_ముగింపు

శూన్య cv::వెలికితీయి_Channel(ఆదానం_Array _src, OutputArray _dst, పూర్ణాంకం coi)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం type = _src.type(), లోతు = CV_MAT_లోతు(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    పూర్ణాంకం ch[] = { coi, 0 };

#నిర్వచితమైతే HAVE_OPENCL
    ఒకవేళ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_కొలత(_src, లోతు);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        తిరిగి;
    }
#అయితే_ముగింపు

    Mat src = _src.getMat();
    _dst.create_Same_కొలత(_src, లోతు);
    Mat dst = _dst.getMat();

    CV_IPP_నిరంతరం_FAST(ipp_వెలికితీయి_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

శూన్య cv::చొప్పించు_Channel(ఆదానం_Array _src, ఆదానం_Output_Array _dst, పూర్ణాంకం coi)
{
    CV_INSTRUMENT_REGION();

    పూర్ణాంకం stype = _src.type(), sdepth = CV_MAT_లోతు(stype), scn = CV_MAT_CN(stype);
    పూర్ణాంకం dtype = _dst.type(), ddepth = CV_MAT_లోతు(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_కొలత(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    పూర్ణాంకం ch[] = { 0, coi };
#నిర్వచితమైతే HAVE_OPENCL
    ఒకవేళ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        తిరిగి;
    }
#అయితే_ముగింపు

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_నిరంతరం_FAST(ipp_చొప్పించు_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
