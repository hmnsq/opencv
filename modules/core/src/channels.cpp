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

template<typename T> ಸ್ಥಿರ ಶೂನ್ಯ
mixChannels_( ಧ್ರುವ T** src, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sdelta,
              T** dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ddelta,
              ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ npairs )
{
    ಪೂರ್ಣಾಙ್ಕ i, k;
    ಪ್ರತಿ( k = 0; k < npairs; k++ )
    {
        ಧ್ರುವ T* s = src[k];
        T* d = dst[k];
        ಪೂರ್ಣಾಙ್ಕ ds = sdelta[k], dd = ddelta[k];
        ಯದಿ( s )
        {
            ಪ್ರತಿ( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            ಯದಿ( i < len )
                d[0] = s[0];
        }
        ಅನ್ಯಥಾ
        {
            ಪ್ರತಿ( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            ಯದಿ( i < len )
                d[0] = 0;
        }
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ mixChannels8u( ಧ್ರುವ ಶೂನ್ಯ** src, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sdelta,
                           ಶೂನ್ಯ** dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ddelta,
                           ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ npairs )
{
    mixChannels_((ಧ್ರುವ uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ಸ್ಥಿರ ಶೂನ್ಯ mixChannels16u( ಧ್ರುವ ಶೂನ್ಯ** src, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sdelta,
                            ಶೂನ್ಯ** dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ddelta,
                            ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ npairs )
{
    mixChannels_((ಧ್ರುವ ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ಸ್ಥಿರ ಶೂನ್ಯ mixChannels32s( ಧ್ರುವ ಶೂನ್ಯ** src, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sdelta,
                            ಶೂನ್ಯ** dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ddelta,
                            ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ npairs )
{
    mixChannels_((ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ**)src, sdelta, (ಪೂರ್ಣಾಙ್ಕ**)dst, ddelta, len, npairs);
}

ಸ್ಥಿರ ಶೂನ್ಯ mixChannels64s( ಧ್ರುವ ಶೂನ್ಯ** src, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sdelta,
                            ಶೂನ್ಯ** dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ddelta,
                            ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ npairs )
{
    mixChannels_((ಧ್ರುವ int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*MixChannelsFunc)( ಧ್ರುವ ಶೂನ್ಯ** src, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sdelta,
        ಶೂನ್ಯ** dst, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* ddelta, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ npairs );

ಸ್ಥಿರ MixChannelsFunc getMixchFunc(ಪೂರ್ಣಾಙ್ಕ depth)
{
    ಸ್ಥಿರ MixChannelsFunc mixchTab[CV_DEPTH_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    ಪ್ರತ್ಯಾಗಮ mixchTab[depth];
}

} // cv::


ಶೂನ್ಯ cv::mixChannels( ಧ್ರುವ Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* fromTo, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    ಯದಿ( npairs == 0 )
        ಪ್ರತ್ಯಾಗಮ;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    ಪೂರ್ಣಾಙ್ಕ depth = dst[0].depth();

    AutoBuffer<uchar> buf((nsrcs + ndsts + 1)*(ಪರಿಮಾಣ(Mat*) + ಪರಿಮಾಣ(uchar*)) + npairs*(ಪರಿಮಾಣ(uchar*)*2 + ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ)*6));
    ಧ್ರುವ Mat** arrays = (ಧ್ರುವ Mat**)(uchar*)buf.data();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ಧ್ರುವ uchar** srcs = (ಧ್ರುವ uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    ಪೂರ್ಣಾಙ್ಕ* tab = (ಪೂರ್ಣಾಙ್ಕ*)(dsts + npairs);
    ಪೂರ್ಣಾಙ್ಕ *sdelta = (ಪೂರ್ಣಾಙ್ಕ*)(tab + npairs*4), *ddelta = sdelta + npairs;

    ಪ್ರತಿ( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].depth() == depth );
        arrays[i] = &src[i];
    }
    ಪ್ರತಿ( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].depth() == depth );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    ಪ್ರತಿ( i = 0; i < npairs; i++ )
    {
        ಪೂರ್ಣಾಙ್ಕ i0 = fromTo[i*2], i1 = fromTo[i*2+1];
        ಯದಿ( i0 >= 0 )
        {
            ಪ್ರತಿ( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                ಯದಿ( i0 < src[j].channels() )
                    ಭಙ್ಗ;
            CV_Assert(j < nsrcs && src[j].depth() == depth);
            tab[i*4] = (ಪೂರ್ಣಾಙ್ಕ)j; tab[i*4+1] = (ಪೂರ್ಣಾಙ್ಕ)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        ಅನ್ಯಥಾ
        {
            tab[i*4] = (ಪೂರ್ಣಾಙ್ಕ)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        ಪ್ರತಿ( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            ಯದಿ( i1 < dst[j].channels() )
                ಭಙ್ಗ;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].depth() == depth);
        tab[i*4+2] = (ಪೂರ್ಣಾಙ್ಕ)(j + nsrcs); tab[i*4+3] = (ಪೂರ್ಣಾಙ್ಕ)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (ಪೂರ್ಣಾಙ್ಕ)(nsrcs + ndsts));
    ಪೂರ್ಣಾಙ್ಕ total = (ಪೂರ್ಣಾಙ್ಕ)it.size, blocksize = std::min(total, (ಪೂರ್ಣಾಙ್ಕ)((BLOCK_SIZE + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(depth);
    CV_Assert(func);

    ಪ್ರತಿ( i = 0; i < it.nplanes; i++, ++it )
    {
        ಪ್ರತಿ( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ t = 0; t < total; t += blocksize )
        {
            ಪೂರ್ಣಾಙ್ಕ bsz = std::min(total - t, blocksize);
            func( (ಧ್ರುವ ಶೂನ್ಯ**)srcs, sdelta, (ಶೂನ್ಯ **)dsts, ddelta, bsz, (ಪೂರ್ಣಾಙ್ಕ)npairs );

            ಯದಿ( t + blocksize < total )
                ಪ್ರತಿ( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#ifdef HAVE_OPENCL

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಶೂನ್ಯ getUMatIndex(ಧ್ರುವ std::vector<UMat> & um, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ & idx, ಪೂರ್ಣಾಙ್ಕ & cnidx)
{
    ಪೂರ್ಣಾಙ್ಕ totalChannels = 0;
    ಪ್ರತಿ (size_t i = 0, size = um.size(); i < size; ++i)
    {
        ಪೂರ್ಣಾಙ್ಕ ccn = um[i].channels();
        totalChannels += ccn;

        ಯದಿ (totalChannels == cn)
        {
            idx = (ಪೂರ್ಣಾಙ್ಕ)(i + 1);
            cnidx = 0;
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಅನ್ಯಥಾ ಯದಿ (totalChannels > cn)
        {
            idx = (ಪೂರ್ಣಾಙ್ಕ)i;
            cnidx = i == 0 ? cn : (cn - totalChannels + ccn);
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    idx = cnidx = -1;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_mixChannels(ಒಳಹರಿವು_Array_Of_Arrays _src, ಒಳಹರಿವು_Output_Array_Of_Arrays _dst,
                            ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* fromTo, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.getUMatVector(src);
    _dst.getUMatVector(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    Size size = src[0].size();
    ಪೂರ್ಣಾಙ್ಕ depth = src[0].depth(), esz = CV_ELEM_SIZE(depth),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    ಪ್ರತಿ (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].depth() == depth);
    ಪ್ರತಿ (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].depth() == depth);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    ಪ್ರತಿ (size_t i = 0; i < npairs; ++i)
    {
        ಪೂರ್ಣಾಙ್ಕ scn = fromTo[i<<1], dcn = fromTo[(i<<1) + 1];
        ಪೂರ್ಣಾಙ್ಕ src_idx, src_cnidx, dst_idx, dst_cnidx;

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
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪೂರ್ಣಾಙ್ಕ argindex = 0;
    ಪ್ರತಿ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::ReadOnlyNoSize(srcargs[i]));
    ಪ್ರತಿ (size_t i = 0; i < npairs; ++i)
        argindex = k.set(argindex, ocl::KernelArg::WriteOnlyNoSize(dstargs[i]));
    argindex = k.set(argindex, size.height);
    argindex = k.set(argindex, size.width);
    k.set(argindex, rowsPerWI);

    size_t globalsize[2] = { (size_t)size.width, ((size_t)size.height + rowsPerWI - 1) / rowsPerWI };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, ರಿಕ್ತ, ಅಸತ್ಯ);
}

}

#endif

ಶೂನ್ಯ cv::mixChannels(ಒಳಹರಿವು_Array_Of_Arrays src, ಒಳಹರಿವು_Output_Array_Of_Arrays dst,
                 ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* fromTo, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (npairs == 0 || fromTo == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, fromTo, npairs))

    ತಾರ್ಕಿಕ src_is_mat = src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT &&
            src.kind() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_UMAT;
    ತಾರ್ಕಿಕ dst_is_mat = dst.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT &&
            dst.kind() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            dst.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_UMAT;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ nsrc = src_is_mat ? 1 : (ಪೂರ್ಣಾಙ್ಕ)src.total();
    ಪೂರ್ಣಾಙ್ಕ ndst = dst_is_mat ? 1 : (ಪೂರ್ಣಾಙ್ಕ)dst.total();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    ಪ್ರತಿ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ಪ್ರತಿ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, fromTo, npairs);
}

ಶೂನ್ಯ cv::mixChannels(ಒಳಹರಿವು_Array_Of_Arrays src, ಒಳಹರಿವು_Output_Array_Of_Arrays dst,
                     ಧ್ರುವ std::vector<ಪೂರ್ಣಾಙ್ಕ>& fromTo)
{
    CV_INSTRUMENT_REGION();

    ಯದಿ (fromTo.empty())
        ಪ್ರತ್ಯಾಗಮ;

    CV_OCL_RUN(dst.isUMatVector(),
               ocl_mixChannels(src, dst, &fromTo[0], fromTo.size()>>1))

    ತಾರ್ಕಿಕ src_is_mat = src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT &&
            src.kind() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_VECTOR &&
            src.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_UMAT;
    ತಾರ್ಕಿಕ dst_is_mat = dst.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_MAT &&
            dst.kind() != ಒಳಹರಿವು_Array_2::STD_ARRAY_MAT &&
            dst.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_VECTOR &&
            dst.kind() != ಒಳಹರಿವು_Array_2::STD_VECTOR_UMAT;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ nsrc = src_is_mat ? 1 : (ಪೂರ್ಣಾಙ್ಕ)src.total();
    ಪೂರ್ಣಾಙ್ಕ ndst = dst_is_mat ? 1 : (ಪೂರ್ಣಾಙ್ಕ)dst.total();

    CV_Assert(fromTo.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::AutoBuffer<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.data();
    ಪ್ರತಿ( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    ಪ್ರತಿ( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &fromTo[0], fromTo.size()/2);
}

#ifdef HAVE_IPP

ನಾಮಾಕಾಶ cv
{
ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_extractChannel(ಧ್ರುವ Mat &src, Mat &dst, ಪೂರ್ಣಾಙ್ಕ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಪೂರ್ಣಾಙ್ಕ srcChannels = src.channels();
    ಪೂರ್ಣಾಙ್ಕ dstChannels = dst.channels();

    ಯದಿ(src.dims != dst.dims)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ಪೂರ್ಣಾಙ್ಕ)src.step, srcChannels, channel, dst.ptr(), (ಪೂರ್ಣಾಙ್ಕ)dst.step, dstChannels, 0, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1()) >= 0;
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat      *arrays[] = {&dst, ರಿಕ್ತ};
        uchar          *ptrs[2]  = {ರಿಕ್ತ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ಪೂರ್ಣಾಙ್ಕ)it.size, 1};

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1()) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ipp_ಸೇರಿಸು_Channel(ಧ್ರುವ Mat &src, Mat &dst, ಪೂರ್ಣಾಙ್ಕ channel)
{
#ifdef HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    ಪೂರ್ಣಾಙ್ಕ srcChannels = src.channels();
    ಪೂರ್ಣಾಙ್ಕ dstChannels = dst.channels();

    ಯದಿ(src.dims != dst.dims)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ(src.dims <= 2)
    {
        IppiSize size = ippiSize(src.size());

        ಪ್ರತ್ಯಾಗಮ CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (ಪೂರ್ಣಾಙ್ಕ)src.step, srcChannels, 0, dst.ptr(), (ಪೂರ್ಣಾಙ್ಕ)dst.step, dstChannels, channel, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1()) >= 0;
    }
    ಅನ್ಯಥಾ
    {
        ಧ್ರುವ Mat      *arrays[] = {&dst, ರಿಕ್ತ};
        uchar          *ptrs[2]  = {ರಿಕ್ತ};
        NAryMatIterator it(arrays, ptrs);

        IppiSize size = {(ಪೂರ್ಣಾಙ್ಕ)it.size, 1};

        ಪ್ರತಿ( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            ಯದಿ(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (ಪೂರ್ಣಾಙ್ಕ)src.elemSize1()) < 0)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(src); CV_UNUSED(dst); CV_UNUSED(channel);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}
}
#endif

ಶೂನ್ಯ cv::extractChannel(ಒಳಹರಿವು_Array _src, OutputArray _dst, ಪೂರ್ಣಾಙ್ಕ coi)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ type = _src.type(), depth = CV_MAT_DEPTH(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    ಪೂರ್ಣಾಙ್ಕ ch[] = { coi, 0 };

#ifdef HAVE_OPENCL
    ಯದಿ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.createSameSize(_src, depth);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    Mat src = _src.getMat();
    _dst.createSameSize(_src, depth);
    Mat dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_extractChannel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

ಶೂನ್ಯ cv::ಸೇರಿಸು_Channel(ಒಳಹರಿವು_Array _src, ಒಳಹರಿವು_Output_Array _dst, ಪೂರ್ಣಾಙ್ಕ coi)
{
    CV_INSTRUMENT_REGION();

    ಪೂರ್ಣಾಙ್ಕ stype = _src.type(), sdepth = CV_MAT_DEPTH(stype), scn = CV_MAT_CN(stype);
    ಪೂರ್ಣಾಙ್ಕ dtype = _dst.type(), ddepth = CV_MAT_DEPTH(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.sameSize(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    ಪೂರ್ಣಾಙ್ಕ ch[] = { 0, coi };
#ifdef HAVE_OPENCL
    ಯದಿ (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        ಪ್ರತ್ಯಾಗಮ;
    }
#endif

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_RUN_FAST(ipp_ಸೇರಿಸು_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
