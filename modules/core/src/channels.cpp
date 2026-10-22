// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

فضاینام cv
{

template<typename T> ایستا تهی
mixChannels_( ثابت T** src, ثابت صحیح* sdelta,
              T** dst, ثابت صحیح* ddelta,
              صحیح len, صحیح npairs )
{
    صحیح i, k;
    برای( k = 0; k < npairs; k++ )
    {
        ثابت T* s = src[k];
        T* d = dst[k];
        صحیح ds = sdelta[k], dd = ddelta[k];
        اگر( s )
        {
            برای( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            اگر( i < len )
                d[0] = s[0];
        }
        وگرنه
        {
            برای( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            اگر( i < len )
                d[0] = 0;
        }
    }
}


ایستا تهی mixChannels8u( ثابت تهی** src, ثابت صحیح* sdelta,
                           تهی** dst, ثابت صحیح* ddelta,
                           صحیح len, صحیح npairs )
{
    mixChannels_((ثابت uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ایستا تهی mixChannels16u( ثابت تهی** src, ثابت صحیح* sdelta,
                            تهی** dst, ثابت صحیح* ddelta,
                            صحیح len, صحیح npairs )
{
    mixChannels_((ثابت ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ایستا تهی mixChannels32s( ثابت تهی** src, ثابت صحیح* sdelta,
                            تهی** dst, ثابت صحیح* ddelta,
                            صحیح len, صحیح npairs )
{
    mixChannels_((ثابت صحیح**)src, sdelta, (صحیح**)dst, ddelta, len, npairs);
}

ایستا تهی mixChannels64s( ثابت تهی** src, ثابت صحیح* sdelta,
                            تهی** dst, ثابت صحیح* ddelta,
                            صحیح len, صحیح npairs )
{
    mixChannels_((ثابت int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

تعریفنوع تهی (*MixChannelsFunc)( ثابت تهی** src, ثابت صحیح* sdelta,
        تهی** dst, ثابت صحیح* ddelta, صحیح len, صحیح npairs );

ایستا MixChannelsFunc getMixchFunc(صحیح depth)
{
    ایستا MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    بازگشت mixchTab[depth];
}

} // cv::


تهی cv::mixChannels( ثابت Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ثابت صحیح* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    اگر( npairs == 0 )
        بازگشت;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    صحیح depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(اندازه(Mat*) + اندازه(uchar*)) + npairs*(اندازه(uchar*)*2 + اندازه(صحیح)*6));
    ثابت Mat** arrays = (ثابت Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ثابت uchar** srcs = (ثابت uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    صحیح* tab = (صحیح*)(dsts + npairs);
    صحیح *sdelta = (صحیح*)(tab + npairs*4), *ddelta = sdelta + npairs;

    برای( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    برای( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    برای( i = 0; i < npairs; i++ )
    {
        صحیح i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        اگر( i0 >= 0 )
        {
            برای( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                اگر( i0 < src[j].channels() )
                    بشکن;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (صحیح)j; tab[i*4+1] = (صحیح)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        وگرنه
        {
            tab[i*4] = (صحیح)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        برای( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            اگر( i1 < dst[j].channels() )
                بشکن;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (صحیح)(j + nsrcs); tab[i*4+3] = (صحیح)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (صحیح)(nsrcs + ndsts));
    صحیح total = (صحیح)it.size, blocksize = std::min(total, (صحیح)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    برای( i = 0; i < it.nplanes; i++, ++it )
    {
        برای( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        برای( صحیح t = 0; t < total; t += blocksize )
        {
            صحیح bsz = std::min(total - t, blocksize);
            func( (ثابت تهی**)srcs, sdelta, (تهی **)dsts, ddelta, bsz, (صحیح)npairs );

            اگر( t + blocksize < total )
                برای( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

فضاینام cv {

ایستا تهی getUMatIndex(ثابت std::vector<UMat> & um, صحیح cn, صحیح & idx, صحیح & cnidx)
{
    صحیح totalChannels = 0;
    برای (size_t i = 0, size = um.size(); i < size; ++i)
    {
        صحیح ccn = um[i].channels();
        totalChannels += ccn;

        اگر (totalChannels == cn)
        {
            idx = (صحیح)(i + 1);
            cnidx = 0;
            بازگشت;
        }
        وگرنه اگر (totalChannels > cn)
        {
            idx = (صحیح)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            بازگشت;
        }
    }

    idx = cnidx = -1;
}

ایستا منطقی ocl_mixChannels(درون_داد_Array_Of_Arrays _src, درون_داد_Output_Array_Of_Arrays _dst,
                            ثابت صحیح* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    صحیح depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    برای (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    برای (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    برای (size_t i = 0; i < npairs; ++i)
    {
        صحیح scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        صحیح src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    اگر (k.empty())
        بازگشت نادرست;

    صحیح argindex = 0;
    برای (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    برای (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    بازگشت k.run(2, globalsize, پوچ, نادرست);
}

}

#endif

تهی cv::mixChannels(درون_داد_Array_Of_Arrays src, درون_داد_Output_Array_Of_Arrays dst,
                 ثابت صحیح* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    اگر (npairs == 0 || fromTo == پوچ)
        بازگشت;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    منطقی src_is_mat = src.kind() != درون_داد_Array_2::STD_VECTOR_MAT &&
            src.kind() != درون_داد_Array_2::STD_ARRAY_MAT &&
            src.kind() != درون_داد_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != درون_داد_Array_2::STD_VECTOR_UMAT;
    منطقی dst_is_mat = dst.kind() != درون_داد_Array_2::STD_VECTOR_MAT &&
            dst.kind() != درون_داد_Array_2::STD_ARRAY_MAT &&
            dst.kind() != درون_داد_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != درون_داد_Array_2::STD_VECTOR_UMAT;
    صحیح i;
    صحیح nsrc = src_is_mat ? 1 : (صحیح)src.total();
    صحیح ndst = dst_is_mat ? 1 : (صحیح)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    برای( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    برای( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

تهی cv::mixChannels(درون_داد_Array_Of_Arrays src, درون_داد_Output_Array_Of_Arrays dst,
                     ثابت std::vector<صحیح>& fromTo)
{
    CV_INSTRUMENT_REGION();

    اگر (fromTo.empty())
        بازگشت;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    منطقی src_is_mat = src.kind() != درون_داد_Array_2::STD_VECTOR_MAT &&
            src.kind() != درون_داد_Array_2::STD_ARRAY_MAT &&
            src.kind() != درون_داد_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != درون_داد_Array_2::STD_VECTOR_UMAT;
    منطقی dst_is_mat = dst.kind() != درون_داد_Array_2::STD_VECTOR_MAT &&
            dst.kind() != درون_داد_Array_2::STD_ARRAY_MAT &&
            dst.kind() != درون_داد_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != درون_داد_Array_2::STD_VECTOR_UMAT;
    صحیح i;
    صحیح nsrc = src_is_mat ? 1 : (صحیح)src.total();
    صحیح ndst = dst_is_mat ? 1 : (صحیح)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    برای( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    برای( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

فضاینام cv
{
ایستا منطقی ipp_extractChannel(ثابت Mat &src, Mat &dst, صحیح channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحیح srcChannels = src.channels();
    صحیح dstChannels = dst.channels();

    اگر(src.dims != dst.dims)
        بازگشت نادرست;

    اگر(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        بازگشت CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحیح)src.step, srcChannels, channel, dst.ptr(), (صحیح)dst.step, dstChannels, 0, size, (صحیح)src.elemSize1()) >= 0;
    }
    وگرنه
    {
        ثابت Mat      *arrays[] = {&dst, پوچ};
        uchar          *ptrs[2]  = {پوچ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(صحیح)it.size, 1};

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (صحیح)src.elemSize1()) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
#وگرنه
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    بازگشت نادرست;
#endif
}

ایستا منطقی ipp_درج_Channel(ثابت Mat &src, Mat &dst, صحیح channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحیح srcChannels = src.channels();
    صحیح dstChannels = dst.channels();

    اگر(src.dims != dst.dims)
        بازگشت نادرست;

    اگر(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        بازگشت CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحیح)src.step, srcChannels, 0, dst.ptr(), (صحیح)dst.step, dstChannels, channel, size, (صحیح)src.elemSize1()) >= 0;
    }
    وگرنه
    {
        ثابت Mat      *arrays[] = {&dst, پوچ};
        uchar          *ptrs[2]  = {پوچ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(صحیح)it.size, 1};

        برای( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (صحیح)src.elemSize1()) < 0)
                بازگشت نادرست;
        }
        بازگشت درست;
    }
#وگرنه
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    بازگشت نادرست;
#endif
}
}
#endif

تهی cv::extractChannel(درون_داد_Array _src, OutputArray _dst, صحیح coi)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    صحیح ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    اگر (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        بازگشت;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

تهی cv::درج_Channel(درون_داد_Array _src, درون_داد_Output_Array _dst, صحیح coi)
{
    CV_INSTRUMENT_REGION();

    صحیح stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    صحیح dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    صحیح ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    اگر (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        بازگشت;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_درج_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
