// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#ضمن "precomp.hpp"
#ضمن "opencl_kernels_core.hpp"
#ضمن "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

نطاق cv
{

سانچو<قسم_نالو T> ساكن فراغ
mixChannels_( ثابت T** src, ثابت صحيح* sdelta,
              T** dst, ثابت صحيح* ddelta,
              صحيح len, صحيح npairs )
{
    صحيح i, k;
    لكل( k = 0; k < npairs; k++ )
    {
        ثابت T* s = src[k];
        T* d = dst[k];
        صحيح ds = sdelta[k], dd = ddelta[k];
        إذا( s )
        {
            لكل( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            إذا( i < len )
                d[0] = s[0];
        }
        وإلا
        {
            لكل( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            إذا( i < len )
                d[0] = 0;
        }
    }
}


ساكن فراغ mixChannels8u( ثابت فراغ** src, ثابت صحيح* sdelta,
                           فراغ** dst, ثابت صحيح* ddelta,
                           صحيح len, صحيح npairs )
{
    mixChannels_((ثابت uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ساكن فراغ mixChannels16u( ثابت فراغ** src, ثابت صحيح* sdelta,
                            فراغ** dst, ثابت صحيح* ddelta,
                            صحيح len, صحيح npairs )
{
    mixChannels_((ثابت ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ساكن فراغ mixChannels32s( ثابت فراغ** src, ثابت صحيح* sdelta,
                            فراغ** dst, ثابت صحيح* ddelta,
                            صحيح len, صحيح npairs )
{
    mixChannels_((ثابت صحيح**)src, sdelta, (صحيح**)dst, ddelta, len, npairs);
}

ساكن فراغ mixChannels64s( ثابت فراغ** src, ثابت صحيح* sdelta,
                            فراغ** dst, ثابت صحيح* ddelta,
                            صحيح len, صحيح npairs )
{
    mixChannels_((ثابت int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

تعريفنوع فراغ (*MixChannelsFunc)( ثابت فراغ** src, ثابت صحيح* sdelta,
        فراغ** dst, ثابت صحيح* ddelta, صحيح len, صحيح npairs );

ساكن MixChannelsFunc getMixchFunc(صحيح اونهائي)
{
    ساكن MixChannelsFunc mixchTab[CV_اونهائي_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    أرجع mixchTab[اونهائي];
}

} // cv::


فراغ cv::mixChannels( ثابت Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ثابت صحيح* from_منزل, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    إذا( npairs == 0 )
        أرجع;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_منزل && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    صحيح اونهائي = dst[0].اونهائي();

    Auto_بفر<uchar> buf((nsrcs + ndsts + 1)*(حجم(Mat*) + حجم(uchar*)) + npairs*(حجم(uchar*)*2 + حجم(صحيح)*6));
    ثابت Mat** arrays = (ثابت Mat**)(uchar*)buf.معلومات();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ثابت uchar** srcs = (ثابت uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    صحيح* tab = (صحيح*)(dsts + npairs);
    صحيح *sdelta = (صحيح*)(tab + npairs*4), *ddelta = sdelta + npairs;

    لكل( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].اونهائي() == اونهائي );
        arrays[i] = &src[i];
    }
    لكل( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].اونهائي() == اونهائي );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    لكل( i = 0; i < npairs; i++ )
    {
        صحيح i0 = from_منزل[i*2], i1 = from_منزل[i*2+1];
        إذا( i0 >= 0 )
        {
            لكل( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                إذا( i0 < src[j].channels() )
                    اكسر;
            CV_Assert(j < nsrcs && src[j].اونهائي() == اونهائي);
            tab[i*4] = (صحيح)j; tab[i*4+1] = (صحيح)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        وإلا
        {
            tab[i*4] = (صحيح)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        لكل( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            إذا( i1 < dst[j].channels() )
                اكسر;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].اونهائي() == اونهائي);
        tab[i*4+2] = (صحيح)(j + nsrcs); tab[i*4+3] = (صحيح)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (صحيح)(nsrcs + ndsts));
    صحيح ڪل = (صحيح)it.size, blocksize = std::min(ڪل, (صحيح)((بلاڪ_سائيز + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(اونهائي);
    CV_Assert(func);

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        لكل( صحيح t = 0; t < ڪل; t += blocksize )
        {
            صحيح bsz = std::min(ڪل - t, blocksize);
            func( (ثابت فراغ**)srcs, sdelta, (فراغ **)dsts, ddelta, bsz, (صحيح)npairs );

            إذا( t + blocksize < ڪل )
                لكل( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL

نطاق cv {

ساكن فراغ get_UMat_اشاريو(ثابت std::vector<UMat> & um, صحيح cn, صحيح & idx, صحيح & cnidx)
{
    صحيح ڪل_Channels = 0;
    لكل (size_t i = 0, size = um.size(); i < size; ++i)
    {
        صحيح ccn = um[i].channels();
        ڪل_Channels += ccn;

        إذا (ڪل_Channels == cn)
        {
            idx = (صحيح)(i + 1);
            cnidx = 0;
            أرجع;
        }
        وإلا إذا (ڪل_Channels > cn)
        {
            idx = (صحيح)i;
            cnidx = i == 0 ? cn : (cn - ڪل_Channels + ccn);
            أرجع;
        }
    }

    idx = cnidx = -1;
}

ساكن منطقي ocl_mixChannels(داخلا_Array_جو_Arrays _src, داخلا_Output_Array_جو_Arrays _dst,
                            ثابت صحيح* from_منزل, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_ويڪٽر(src);
    _dst.get_UMat_ويڪٽر(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    سائيز size = src[0].size();
    صحيح اونهائي = src[0].اونهائي(), esz = CV_ELEM_سائيز(اونهائي),
            صفون_في_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    لكل (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].اونهائي() == اونهائي);
    لكل (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].اونهائي() == اونهائي);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    لكل (size_t i = 0; i < npairs; ++i)
    {
        صحيح scn = from_منزل[i<<1], dcn = from_منزل[(i<<1) + 1];
        صحيح src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_اشاريو(src, scn, src_idx, src_cnidx);
        get_UMat_اشاريو(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_منزل_Str(اونهائي), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    صحيح argindex = 0;
    لكل (size_t i = 0; i < npairs; ++i)
        argindex = k.سيٽ(argindex, ocl::KernelArg::Read_Only_No_سائيز(srcargs[i]));
    لكل (size_t i = 0; i < npairs; ++i)
        argindex = k.سيٽ(argindex, ocl::KernelArg::Write_Only_No_سائيز(dstargs[i]));
    argindex = k.سيٽ(argindex, size.اوچائي);
    argindex = k.سيٽ(argindex, size.ويڪر);
    k.سيٽ(argindex, صفون_في_WI);

    size_t globalsize[2] = { (size_t)size.ويڪر, ((size_t)size.اوچائي + صفون_في_WI - 1) / صفون_في_WI };
    أرجع k.لاڳيتو(2, globalsize, عدم, خطأ);
}

}

#جيڪڏهن_پورو

فراغ cv::mixChannels(داخلا_Array_جو_Arrays src, داخلا_Output_Array_جو_Arrays dst,
                 ثابت صحيح* from_منزل, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    إذا (npairs == 0 || from_منزل == عدم)
        أرجع;

    CV_OCL_لاڳيتو(dst.is_UMat_ويڪٽر(),
               ocl_mixChannels(src, dst, from_منزل, npairs))

    منطقي src_is_mat = src.قسم() != داخلا_Array_2::STD_ويڪٽر_MAT &&
            src.قسم() != داخلا_Array_2::STD_ARRAY_MAT &&
            src.قسم() != داخلا_Array_2::STD_ويڪٽر_ويڪٽر &&
            src.قسم() != داخلا_Array_2::STD_ويڪٽر_UMAT;
    منطقي dst_is_mat = dst.قسم() != داخلا_Array_2::STD_ويڪٽر_MAT &&
            dst.قسم() != داخلا_Array_2::STD_ARRAY_MAT &&
            dst.قسم() != داخلا_Array_2::STD_ويڪٽر_ويڪٽر &&
            dst.قسم() != داخلا_Array_2::STD_ويڪٽر_UMAT;
    صحيح i;
    صحيح nsrc = src_is_mat ? 1 : (صحيح)src.ڪل();
    صحيح ndst = dst_is_mat ? 1 : (صحيح)dst.ڪل();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.معلومات();
    لكل( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    لكل( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_منزل, npairs);
}

فراغ cv::mixChannels(داخلا_Array_جو_Arrays src, داخلا_Output_Array_جو_Arrays dst,
                     ثابت std::vector<صحيح>& from_منزل)
{
    CV_INSTRUMENT_REGION();

    إذا (from_منزل.empty())
        أرجع;

    CV_OCL_لاڳيتو(dst.is_UMat_ويڪٽر(),
               ocl_mixChannels(src, dst, &from_منزل[0], from_منزل.size()>>1))

    منطقي src_is_mat = src.قسم() != داخلا_Array_2::STD_ويڪٽر_MAT &&
            src.قسم() != داخلا_Array_2::STD_ARRAY_MAT &&
            src.قسم() != داخلا_Array_2::STD_ويڪٽر_ويڪٽر &&
            src.قسم() != داخلا_Array_2::STD_ويڪٽر_UMAT;
    منطقي dst_is_mat = dst.قسم() != داخلا_Array_2::STD_ويڪٽر_MAT &&
            dst.قسم() != داخلا_Array_2::STD_ARRAY_MAT &&
            dst.قسم() != داخلا_Array_2::STD_ويڪٽر_ويڪٽر &&
            dst.قسم() != داخلا_Array_2::STD_ويڪٽر_UMAT;
    صحيح i;
    صحيح nsrc = src_is_mat ? 1 : (صحيح)src.ڪل();
    صحيح ndst = dst_is_mat ? 1 : (صحيح)dst.ڪل();

    CV_Assert(from_منزل.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.معلومات();
    لكل( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    لكل( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_منزل[0], from_منزل.size()/2);
}

#جيڪڏهن_وصف_ٿيل HAVE_IPP

نطاق cv
{
ساكن منطقي ipp_ڪڍو_Channel(ثابت Mat &src, Mat &dst, صحيح channel)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحيح srcChannels = src.channels();
    صحيح dstChannels = dst.channels();

    إذا(src.dims != dst.dims)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_سائيز size = ippi_سائيز(src.size());

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحيح)src.قدم, srcChannels, channel, dst.ptr(), (صحيح)dst.قدم, dstChannels, 0, size, (صحيح)src.elemSize1()) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائيز size = {(صحيح)it.size, 1};

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (صحيح)src.elemSize1()) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(src); CV_اڻ_استعمال_ٿيل(dst); CV_اڻ_استعمال_ٿيل(channel);
    أرجع خطأ;
#جيڪڏهن_پورو
}

ساكن منطقي ipp_وجهو_Channel(ثابت Mat &src, Mat &dst, صحيح channel)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحيح srcChannels = src.channels();
    صحيح dstChannels = dst.channels();

    إذا(src.dims != dst.dims)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_سائيز size = ippi_سائيز(src.size());

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحيح)src.قدم, srcChannels, 0, dst.ptr(), (صحيح)dst.قدم, dstChannels, channel, size, (صحيح)src.elemSize1()) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائيز size = {(صحيح)it.size, 1};

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (صحيح)src.elemSize1()) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(src); CV_اڻ_استعمال_ٿيل(dst); CV_اڻ_استعمال_ٿيل(channel);
    أرجع خطأ;
#جيڪڏهن_پورو
}
}
#جيڪڏهن_پورو

فراغ cv::ڪڍو_Channel(داخلا_Array _src, OutputArray _dst, صحيح coi)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), اونهائي = CV_MAT_اونهائي(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    صحيح ch[] = { coi, 0 };

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    إذا (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_سائيز(_src, اونهائي);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        أرجع;
    }
#جيڪڏهن_پورو

    Mat src = _src.getMat();
    _dst.create_Same_سائيز(_src, اونهائي);
    Mat dst = _dst.getMat();

    CV_IPP_لاڳيتو_FAST(ipp_ڪڍو_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

فراغ cv::وجهو_Channel(داخلا_Array _src, داخلا_Output_Array _dst, صحيح coi)
{
    CV_INSTRUMENT_REGION();

    صحيح stype = _src.type(), sdepth = CV_MAT_اونهائي(stype), scn = CV_MAT_CN(stype);
    صحيح dtype = _dst.type(), ddepth = CV_MAT_اونهائي(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_سائيز(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    صحيح ch[] = { 0, coi };
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    إذا (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        أرجع;
    }
#جيڪڏهن_پورو

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_لاڳيتو_FAST(ipp_وجهو_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
