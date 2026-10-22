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
mixChannels_( ধ্রুব T** src, ধ্রুব পূর্ণাঙ্ক* sdelta,
              T** dst, ধ্রুব পূর্ণাঙ্ক* ddelta,
              পূর্ণাঙ্ক len, পূর্ণাঙ্ক npairs )
{
    পূর্ণাঙ্ক i, k;
    প্রতি( k = 0; k < npairs; k++ )
    {
        ধ্রুব T* s = src[k];
        T* d = dst[k];
        পূর্ণাঙ্ক ds = sdelta[k], dd = ddelta[k];
        যদি( s )
        {
            প্রতি( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            যদি( i < len )
                d[0] = s[0];
        }
        অন্যথা
        {
            প্রতি( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            যদি( i < len )
                d[0] = 0;
        }
    }
}


স্থির শূন্য mixChannels8u( ধ্রুব শূন্য** src, ধ্রুব পূর্ণাঙ্ক* sdelta,
                           শূন্য** dst, ধ্রুব পূর্ণাঙ্ক* ddelta,
                           পূর্ণাঙ্ক len, পূর্ণাঙ্ক npairs )
{
    mixChannels_((ধ্রুব uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

স্থির শূন্য mixChannels16u( ধ্রুব শূন্য** src, ধ্রুব পূর্ণাঙ্ক* sdelta,
                            শূন্য** dst, ধ্রুব পূর্ণাঙ্ক* ddelta,
                            পূর্ণাঙ্ক len, পূর্ণাঙ্ক npairs )
{
    mixChannels_((ধ্রুব ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

স্থির শূন্য mixChannels32s( ধ্রুব শূন্য** src, ধ্রুব পূর্ণাঙ্ক* sdelta,
                            শূন্য** dst, ধ্রুব পূর্ণাঙ্ক* ddelta,
                            পূর্ণাঙ্ক len, পূর্ণাঙ্ক npairs )
{
    mixChannels_((ধ্রুব পূর্ণাঙ্ক**)src, sdelta, (পূর্ণাঙ্ক**)dst, ddelta, len, npairs);
}

স্থির শূন্য mixChannels64s( ধ্রুব শূন্য** src, ধ্রুব পূর্ণাঙ্ক* sdelta,
                            শূন্য** dst, ধ্রুব পূর্ণাঙ্ক* ddelta,
                            পূর্ণাঙ্ক len, পূর্ণাঙ্ক npairs )
{
    mixChannels_((ধ্রুব int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

প্রকারনির্দেশ শূন্য (*MixChannelsFunc)( ধ্রুব শূন্য** src, ধ্রুব পূর্ণাঙ্ক* sdelta,
        শূন্য** dst, ধ্রুব পূর্ণাঙ্ক* ddelta, পূর্ণাঙ্ক len, পূর্ণাঙ্ক npairs );

স্থির MixChannelsFunc getMixchFunc(পূর্ণাঙ্ক depth)
{
    স্থির MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    প্রত্যাগম mixchTab[depth];
}

} // cv::


শূন্য cv::mixChannels( ধ্রুব Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ধ্রুব পূর্ণাঙ্ক* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    যদি( npairs == 0 )
        প্রত্যাগম;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    পূর্ণাঙ্ক depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(পরিমাণ(Mat*) + পরিমাণ(uchar*)) + npairs*(পরিমাণ(uchar*)*2 + পরিমাণ(পূর্ণাঙ্ক)*6));
    ধ্রুব Mat** arrays = (ধ্রুব Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ধ্রুব uchar** srcs = (ধ্রুব uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    পূর্ণাঙ্ক* tab = (পূর্ণাঙ্ক*)(dsts + npairs);
    পূর্ণাঙ্ক *sdelta = (পূর্ণাঙ্ক*)(tab + npairs*4), *ddelta = sdelta + npairs;

    প্রতি( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    প্রতি( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    প্রতি( i = 0; i < npairs; i++ )
    {
        পূর্ণাঙ্ক i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        যদি( i0 >= 0 )
        {
            প্রতি( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                যদি( i0 < src[j].channels() )
                    ভঙ্গ;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (পূর্ণাঙ্ক)j; tab[i*4+1] = (পূর্ণাঙ্ক)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        অন্যথা
        {
            tab[i*4] = (পূর্ণাঙ্ক)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        প্রতি( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            যদি( i1 < dst[j].channels() )
                ভঙ্গ;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (পূর্ণাঙ্ক)(j + nsrcs); tab[i*4+3] = (পূর্ণাঙ্ক)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (পূর্ণাঙ্ক)(nsrcs + ndsts));
    পূর্ণাঙ্ক total = (পূর্ণাঙ্ক)it.size, blocksize = std::min(total, (পূর্ণাঙ্ক)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    প্রতি( i = 0; i < it.nplanes; i++, ++it )
    {
        প্রতি( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        প্রতি( পূর্ণাঙ্ক t = 0; t < total; t += blocksize )
        {
            পূর্ণাঙ্ক bsz = std::min(total - t, blocksize);
            func( (ধ্রুব শূন্য**)srcs, sdelta, (শূন্য **)dsts, ddelta, bsz, (পূর্ণাঙ্ক)npairs );

            যদি( t + blocksize < total )
                প্রতি( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

নামাকাশ cv {

স্থির শূন্য getUMatIndex(ধ্রুব std::vector<UMat> & um, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক & idx, পূর্ণাঙ্ক & cnidx)
{
    পূর্ণাঙ্ক totalChannels = 0;
    প্রতি (size_t i = 0, size = um.size(); i < size; ++i)
    {
        পূর্ণাঙ্ক ccn = um[i].channels();
        totalChannels += ccn;

        যদি (totalChannels == cn)
        {
            idx = (পূর্ণাঙ্ক)(i + 1);
            cnidx = 0;
            প্রত্যাগম;
        }
        অন্যথা যদি (totalChannels > cn)
        {
            idx = (পূর্ণাঙ্ক)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            প্রত্যাগম;
        }
    }

    idx = cnidx = -1;
}

স্থির তার্কিক ocl_mixChannels(প্রবেশ_Array_Of_Arrays _src, প্রবেশ_Output_Array_Of_Arrays _dst,
                            ধ্রুব পূর্ণাঙ্ক* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    পূর্ণাঙ্ক depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    প্রতি (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    প্রতি (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    প্রতি (size_t i = 0; i < npairs; ++i)
    {
        পূর্ণাঙ্ক scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        পূর্ণাঙ্ক src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    পূর্ণাঙ্ক argindex = 0;
    প্রতি (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    প্রতি (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    প্রত্যাগম k.run(2, globalsize, রিক্ত, অসত্য);
}

}

#endif

শূন্য cv::mixChannels(প্রবেশ_Array_Of_Arrays src, প্রবেশ_Output_Array_Of_Arrays dst,
                 ধ্রুব পূর্ণাঙ্ক* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    যদি (npairs == 0 || fromTo == রিক্ত)
        প্রত্যাগম;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    তার্কিক src_is_mat = src.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    তার্কিক dst_is_mat = dst.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক nsrc = src_is_mat ? 1 : (পূর্ণাঙ্ক)src.total();
    পূর্ণাঙ্ক ndst = dst_is_mat ? 1 : (পূর্ণাঙ্ক)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    প্রতি( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    প্রতি( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

শূন্য cv::mixChannels(প্রবেশ_Array_Of_Arrays src, প্রবেশ_Output_Array_Of_Arrays dst,
                     ধ্রুব std::vector<পূর্ণাঙ্ক>& fromTo)
{
    CV_INSTRUMENT_REGION();

    যদি (fromTo.empty())
        প্রত্যাগম;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    তার্কিক src_is_mat = src.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    তার্কিক dst_is_mat = dst.kind() != প্রবেশ_Array_2::STD_VECTOR_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_ARRAY_MAT &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != প্রবেশ_Array_2::STD_VECTOR_UMAT;
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক nsrc = src_is_mat ? 1 : (পূর্ণাঙ্ক)src.total();
    পূর্ণাঙ্ক ndst = dst_is_mat ? 1 : (পূর্ণাঙ্ক)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    প্রতি( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    প্রতি( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

নামাকাশ cv
{
স্থির তার্কিক ipp_extractChannel(ধ্রুব Mat &src, Mat &dst, পূর্ণাঙ্ক channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    পূর্ণাঙ্ক srcChannels = src.channels();
    পূর্ণাঙ্ক dstChannels = dst.channels();

    যদি(src.dims != dst.dims)
        প্রত্যাগম অসত্য;

    যদি(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (পূর্ণাঙ্ক)src.step, srcChannels, channel, dst.ptr(), (পূর্ণাঙ্ক)dst.step, dstChannels, 0, size, (পূর্ণাঙ্ক)src.elemSize1()) >= 0;
    }
    অন্যথা
    {
        ধ্রুব Mat      *arrays[] = {&dst, রিক্ত};
        uchar          *ptrs[2]  = {রিক্ত};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(পূর্ণাঙ্ক)it.size, 1};

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (পূর্ণাঙ্ক)src.elemSize1()) < 0)
                প্রত্যাগম অসত্য;
        }
        প্রত্যাগম সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    প্রত্যাগম অসত্য;
#endif
}

স্থির তার্কিক ipp_ঢোকাও_Channel(ধ্রুব Mat &src, Mat &dst, পূর্ণাঙ্ক channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    পূর্ণাঙ্ক srcChannels = src.channels();
    পূর্ণাঙ্ক dstChannels = dst.channels();

    যদি(src.dims != dst.dims)
        প্রত্যাগম অসত্য;

    যদি(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        প্রত্যাগম CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (পূর্ণাঙ্ক)src.step, srcChannels, 0, dst.ptr(), (পূর্ণাঙ্ক)dst.step, dstChannels, channel, size, (পূর্ণাঙ্ক)src.elemSize1()) >= 0;
    }
    অন্যথা
    {
        ধ্রুব Mat      *arrays[] = {&dst, রিক্ত};
        uchar          *ptrs[2]  = {রিক্ত};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(পূর্ণাঙ্ক)it.size, 1};

        প্রতি( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            যদি(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (পূর্ণাঙ্ক)src.elemSize1()) < 0)
                প্রত্যাগম অসত্য;
        }
        প্রত্যাগম সত্য;
    }
#অন্যথা
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    প্রত্যাগম অসত্য;
#endif
}
}
#endif

শূন্য cv::extractChannel(প্রবেশ_Array _src, OutputArray _dst, পূর্ণাঙ্ক coi)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    পূর্ণাঙ্ক ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    যদি (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        প্রত্যাগম;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

শূন্য cv::ঢোকাও_Channel(প্রবেশ_Array _src, প্রবেশ_Output_Array _dst, পূর্ণাঙ্ক coi)
{
    CV_INSTRUMENT_REGION();

    পূর্ণাঙ্ক stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    পূর্ণাঙ্ক dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    পূর্ণাঙ্ক ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    যদি (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        প্রত্যাগম;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_ঢোকাও_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
