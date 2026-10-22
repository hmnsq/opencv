// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"
#অন্তর্ভাব "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

নামাকাশ cv
{

template<typename T> স্থির শূন্য
mixChannels_( ধ্রুবক T** src, ধ্রুবক পূর্ণসংখ্যা* sdelta,
              T** dst, ধ্রুবক পূর্ণসংখ্যা* ddelta,
              পূর্ণসংখ্যা len, পূর্ণসংখ্যা npairs )
{
    পূর্ণসংখ্যা i, k;
    জন্য( k = 0; k < npairs; k++ )
    {
        ধ্রুবক T* s = src[k];
        T* d = dst[k];
        পূর্ণসংখ্যা ds = sdelta[k], dd = ddelta[k];
        যদি( s )
        {
            জন্য( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            যদি( i < len )
                d[0] = s[0];
        }
        অন্যথা
        {
            জন্য( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            যদি( i < len )
                d[0] = 0;
        }
    }
}


স্থির শূন্য mixChannels8u( ধ্রুবক শূন্য** src, ধ্রুবক পূর্ণসংখ্যা* sdelta,
                           শূন্য** dst, ধ্রুবক পূর্ণসংখ্যা* ddelta,
                           পূর্ণসংখ্যা len, পূর্ণসংখ্যা npairs )
{
    mixChannels_((ধ্রুবক uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

স্থির শূন্য mixChannels16u( ধ্রুবক শূন্য** src, ধ্রুবক পূর্ণসংখ্যা* sdelta,
                            শূন্য** dst, ধ্রুবক পূর্ণসংখ্যা* ddelta,
                            পূর্ণসংখ্যা len, পূর্ণসংখ্যা npairs )
{
    mixChannels_((ধ্রুবক ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

স্থির শূন্য mixChannels32s( ধ্রুবক শূন্য** src, ধ্রুবক পূর্ণসংখ্যা* sdelta,
                            শূন্য** dst, ধ্রুবক পূর্ণসংখ্যা* ddelta,
                            পূর্ণসংখ্যা len, পূর্ণসংখ্যা npairs )
{
    mixChannels_((ধ্রুবক পূর্ণসংখ্যা**)src, sdelta, (পূর্ণসংখ্যা**)dst, ddelta, len, npairs);
}

স্থির শূন্য mixChannels64s( ধ্রুবক শূন্য** src, ধ্রুবক পূর্ণসংখ্যা* sdelta,
                            শূন্য** dst, ধ্রুবক পূর্ণসংখ্যা* ddelta,
                            পূর্ণসংখ্যা len, পূর্ণসংখ্যা npairs )
{
    mixChannels_((ধ্রুবক int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

প্রকারলেখ শূন্য (*MixChannelsFunc)( ধ্রুবক শূন্য** src, ধ্রুবক পূর্ণসংখ্যা* sdelta,
        শূন্য** dst, ধ্রুবক পূর্ণসংখ্যা* ddelta, পূর্ণসংখ্যা len, পূর্ণসংখ্যা npairs );

স্থির MixChannelsFunc getMixchFunc(পূর্ণসংখ্যা depth)
{
    স্থির MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ফেরত mixchTab[depth];
}

} // cv::


শূন্য cv::mixChannels( ধ্রুবক Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ধ্রুবক পূর্ণসংখ্যা* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    যদি( npairs == 0 )
        ফেরত;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    পূর্ণসংখ্যা depth = dst[0].depth();

    Auto_বাফার<uchar> buf((nsrcs + ndsts + 1)*(আকার(Mat*) + আকার(uchar*)) + npairs*(আকার(uchar*)*2 + আকার(পূর্ণসংখ্যা)*6));
    ধ্রুবক Mat** arrays = (ধ্রুবক Mat**)(uchar*)buf.তথ্য();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ধ্রুবক uchar** srcs = (ধ্রুবক uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    পূর্ণসংখ্যা* tab = (পূর্ণসংখ্যা*)(dsts + npairs);
    পূর্ণসংখ্যা *sdelta = (পূর্ণসংখ্যা*)(tab + npairs*4), *ddelta = sdelta + npairs;

    জন্য( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    জন্য( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    জন্য( i = 0; i < npairs; i++ )
    {
        পূর্ণসংখ্যা i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        যদি( i0 >= 0 )
        {
            জন্য( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                যদি( i0 < src[j].channels() )
                    বিরতি;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (পূর্ণসংখ্যা)j; tab[i*4+1] = (পূর্ণসংখ্যা)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        অন্যথা
        {
            tab[i*4] = (পূর্ণসংখ্যা)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        জন্য( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            যদি( i1 < dst[j].channels() )
                বিরতি;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (পূর্ণসংখ্যা)(j + nsrcs); tab[i*4+3] = (পূর্ণসংখ্যা)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (পূর্ণসংখ্যা)(nsrcs + ndsts));
    পূর্ণসংখ্যা যোগ = (পূর্ণসংখ্যা)it.size, blocksize = std::min(যোগ, (পূর্ণসংখ্যা)((BLOCK_আকার + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    জন্য( i = 0; i < it.nplanes; i++, ++it )
    {
        জন্য( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        জন্য( পূর্ণসংখ্যা t = 0; t < যোগ; t += blocksize )
        {
            পূর্ণসংখ্যা bsz = std::min(যোগ - t, blocksize);
            func( (ধ্রুবক শূন্য**)srcs, sdelta, (শূন্য **)dsts, ddelta, bsz, (পূর্ণসংখ্যা)npairs );

            যদি( t + blocksize < যোগ )
                জন্য( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির শূন্য get_UMat_সূচক(ধ্রুবক std::vector<UMat> & um, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা & idx, পূর্ণসংখ্যা & cnidx)
{
    পূর্ণসংখ্যা যোগ_Channels = 0;
    জন্য (size_t i = 0, size = um.size(); i < size; ++i)
    {
        পূর্ণসংখ্যা ccn = um[i].channels();
        যোগ_Channels += ccn;

        যদি (যোগ_Channels == cn)
        {
            idx = (পূর্ণসংখ্যা)(i + 1);
            cnidx = 0;
            ফেরত;
        }
        অন্যথা যদি (যোগ_Channels > cn)
        {
            idx = (পূর্ণসংখ্যা)i;
            cnidx = i == 0 ? cn : (cn - যোগ_Channels + ccn);
            ফেরত;
        }
    }

    idx = cnidx = -1;
}

স্থির যৌক্তিক ocl_mixChannels(প্রবেশ_Array_Of_Arrays _src, প্রবেশ_Output_Array_Of_Arrays _dst,
                            ধ্রুবক পূর্ণসংখ্যা* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    আকার_2 size = src[0].size();
    পূর্ণসংখ্যা depth = src[0].depth(), esz = CV_ELEM_আকার(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    জন্য (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    জন্য (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    জন্য (size_t i = 0; i < npairs; ++i)
    {
        পূর্ণসংখ্যা scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        পূর্ণসংখ্যা src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_সূচক(src, scn, src_idx, src_cnidx);
        get_UMat_সূচক(dst, dcn, dst_idx, dst_cnidx);

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
    যদি (k.empty())
        ফেরত মিথ্যা;

    পূর্ণসংখ্যা argindex = 0;
    জন্য (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::Read_Only_No_আকার(srcargs[i]));
    জন্য (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::Write_Only_No_আকার(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ফেরত k.run(2, globalsize, ফাঁকা, মিথ্যা);
}

}

#endif

শূন্য cv::mixChannels(প্রবেশ_Array_Of_Arrays src, প্রবেশ_Output_Array_Of_Arrays dst,
                 ধ্রুবক পূর্ণসংখ্যা* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    যদি (npairs == 0 || fromTo == ফাঁকা)
        ফেরত;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    যৌক্তিক src_is_mat = src.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    যৌক্তিক dst_is_mat = dst.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা nsrc = src_is_mat ? 1 : (পূর্ণসংখ্যা)src.যোগ();
    পূর্ণসংখ্যা ndst = dst_is_mat ? 1 : (পূর্ণসংখ্যা)dst.যোগ();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_বাফার<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.তথ্য();
    জন্য( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    জন্য( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

শূন্য cv::mixChannels(প্রবেশ_Array_Of_Arrays src, প্রবেশ_Output_Array_Of_Arrays dst,
                     ধ্রুবক std::vector<পূর্ণসংখ্যা>& fromTo)
{
    CV_INSTRUMENT_REGION();

    যদি (fromTo.empty())
        ফেরত;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    যৌক্তিক src_is_mat = src.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    যৌক্তিক dst_is_mat = dst.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা nsrc = src_is_mat ? 1 : (পূর্ণসংখ্যা)src.যোগ();
    পূর্ণসংখ্যা ndst = dst_is_mat ? 1 : (পূর্ণসংখ্যা)dst.যোগ();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_বাফার<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.তথ্য();
    জন্য( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    জন্য( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

নামাকাশ cv
{
স্থির যৌক্তিক ipp_extractChannel(ধ্রুবক Mat &src, Mat &dst, পূর্ণসংখ্যা channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    পূর্ণসংখ্যা srcChannels = src.channels();
    পূর্ণসংখ্যা dstChannels = dst.channels();

    যদি(src.dims != dst.dims)
        ফেরত মিথ্যা;

    যদি(src.dims <= 2)
    {
        Ippi_আকার size = ippi_আকার(src.size());

        ফেরত CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (পূর্ণসংখ্যা)src.step, srcChannels, channel, dst.ptr(), (পূর্ণসংখ্যা)dst.step, dstChannels, 0, size, (পূর্ণসংখ্যা)src.elemSize1()) >= 0;
    }
    অন্যথা
    {
        ধ্রুবক Mat      *arrays[] = {&dst, ফাঁকা};
        uchar          *ptrs[2]  = {ফাঁকা};
        NAryMatIterator it(arrays, ptrs);

        Ippi_আকার size = {(পূর্ণসংখ্যা)it.size, 1};

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (পূর্ণসংখ্যা)src.elemSize1()) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ফেরত মিথ্যা;
#endif
}

স্থির যৌক্তিক ipp_ঢোকাও_Channel(ধ্রুবক Mat &src, Mat &dst, পূর্ণসংখ্যা channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    পূর্ণসংখ্যা srcChannels = src.channels();
    পূর্ণসংখ্যা dstChannels = dst.channels();

    যদি(src.dims != dst.dims)
        ফেরত মিথ্যা;

    যদি(src.dims <= 2)
    {
        Ippi_আকার size = ippi_আকার(src.size());

        ফেরত CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (পূর্ণসংখ্যা)src.step, srcChannels, 0, dst.ptr(), (পূর্ণসংখ্যা)dst.step, dstChannels, channel, size, (পূর্ণসংখ্যা)src.elemSize1()) >= 0;
    }
    অন্যথা
    {
        ধ্রুবক Mat      *arrays[] = {&dst, ফাঁকা};
        uchar          *ptrs[2]  = {ফাঁকা};
        NAryMatIterator it(arrays, ptrs);

        Ippi_আকার size = {(পূর্ণসংখ্যা)it.size, 1};

        জন্য( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (পূর্ণসংখ্যা)src.elemSize1()) < 0)
                ফেরত মিথ্যা;
        }
        ফেরত সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ফেরত মিথ্যা;
#endif
}
}
#endif

শূন্য cv::extractChannel(প্রবেশ_Array _src, OutputArray _dst, পূর্ণসংখ্যা coi)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    পূর্ণসংখ্যা ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    যদি (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_আকার(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ফেরত;
    }
#endif

    Mat src = _src.getMat();
    _dst.create_Same_আকার(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

শূন্য cv::ঢোকাও_Channel(প্রবেশ_Array _src, প্রবেশ_Output_Array _dst, পূর্ণসংখ্যা coi)
{
    CV_INSTRUMENT_REGION();

    পূর্ণসংখ্যা stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    পূর্ণসংখ্যা dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_আকার(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    পূর্ণসংখ্যা ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    যদি (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ফেরত;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_ঢোকাও_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
