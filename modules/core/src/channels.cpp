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

کينډۍ<ډول_نوم T> ساكن فراغ
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

ساكن MixChannelsFunc getMixchFunc(صحيح ژوروالی)
{
    ساكن MixChannelsFunc mixchTab[CV_ژوروالی_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    أرجع mixchTab[ژوروالی];
}

} // cv::


فراغ cv::mixChannels( ثابت Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ثابت صحيح* from_منزل, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    إذا( npairs == 0 )
        أرجع;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_منزل && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    صحيح ژوروالی = dst[0].ژوروالی();

    Auto_بفر<uchar> buf((nsrcs + ndsts + 1)*(حجم(Mat*) + حجم(uchar*)) + npairs*(حجم(uchar*)*2 + حجم(صحيح)*6));
    ثابت Mat** arrays = (ثابت Mat**)(uchar*)buf.معلومات();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ثابت uchar** srcs = (ثابت uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    صحيح* tab = (صحيح*)(dsts + npairs);
    صحيح *sdelta = (صحيح*)(tab + npairs*4), *ddelta = sdelta + npairs;

    لكل( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].ژوروالی() == ژوروالی );
        arrays[i] = &src[i];
    }
    لكل( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].ژوروالی() == ژوروالی );
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
            CV_Assert(j < nsrcs && src[j].ژوروالی() == ژوروالی);
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
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].ژوروالی() == ژوروالی);
        tab[i*4+2] = (صحيح)(j + nsrcs); tab[i*4+3] = (صحيح)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (صحيح)(nsrcs + ndsts));
    صحيح ټول = (صحيح)it.size, blocksize = std::min(ټول, (صحيح)((بلاک_اندازه + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(ژوروالی);
    CV_Assert(func);

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        لكل( صحيح t = 0; t < ټول; t += blocksize )
        {
            صحيح bsz = std::min(ټول - t, blocksize);
            func( (ثابت فراغ**)srcs, sdelta, (فراغ **)dsts, ddelta, bsz, (صحيح)npairs );

            إذا( t + blocksize < ټول )
                لكل( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#که_تعریف_شوی HAVE_OPENCL

نطاق cv {

ساكن فراغ get_UMat_شاخص(ثابت std::vector<UMat> & um, صحيح cn, صحيح & idx, صحيح & cnidx)
{
    صحيح ټول_Channels = 0;
    لكل (size_t i = 0, size = um.size(); i < size; ++i)
    {
        صحيح ccn = um[i].channels();
        ټول_Channels += ccn;

        إذا (ټول_Channels == cn)
        {
            idx = (صحيح)(i + 1);
            cnidx = 0;
            أرجع;
        }
        وإلا إذا (ټول_Channels > cn)
        {
            idx = (صحيح)i;
            cnidx = i == 0 ? cn : (cn - ټول_Channels + ccn);
            أرجع;
        }
    }

    idx = cnidx = -1;
}

ساكن منطقي ocl_mixChannels(ننوت_Array_د_Arrays _src, ننوت_Output_Array_د_Arrays _dst,
                            ثابت صحيح* from_منزل, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_ویکټور(src);
    _dst.get_UMat_ویکټور(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    اندازه size = src[0].size();
    صحيح ژوروالی = src[0].ژوروالی(), esz = CV_ELEM_اندازه(ژوروالی),
            لیکې_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    لكل (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].ژوروالی() == ژوروالی);
    لكل (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].ژوروالی() == ژوروالی);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    لكل (size_t i = 0; i < npairs; ++i)
    {
        صحيح scn = from_منزل[i<<1], dcn = from_منزل[(i<<1) + 1];
        صحيح src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_شاخص(src, scn, src_idx, src_cnidx);
        get_UMat_شاخص(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_منزل_Str(ژوروالی), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    صحيح argindex = 0;
    لكل (size_t i = 0; i < npairs; ++i)
        argindex = k.سېټ(argindex, ocl::KernelArg::Read_Only_No_اندازه(srcargs[i]));
    لكل (size_t i = 0; i < npairs; ++i)
        argindex = k.سېټ(argindex, ocl::KernelArg::Write_Only_No_اندازه(dstargs[i]));
    argindex = k.سېټ(argindex, size.لوړوالی);
    argindex = k.سېټ(argindex, size.پلنوالی);
    k.سېټ(argindex, لیکې_فی_WI);

    size_t globalsize[2] = { (size_t)size.پلنوالی, ((size_t)size.لوړوالی + لیکې_فی_WI - 1) / لیکې_فی_WI };
    أرجع k.پرلپسې(2, globalsize, عدم, خطأ);
}

}

#د_که_پای

فراغ cv::mixChannels(ننوت_Array_د_Arrays src, ننوت_Output_Array_د_Arrays dst,
                 ثابت صحيح* from_منزل, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    إذا (npairs == 0 || from_منزل == عدم)
        أرجع;

    CV_OCL_پرلپسې(dst.is_UMat_ویکټور(),
               ocl_mixChannels(src, dst, from_منزل, npairs))

    منطقي src_is_mat = src.ډول() != ننوت_Array_2::STD_ویکټور_MAT &&
            src.ډول() != ننوت_Array_2::STD_ARRAY_MAT &&
            src.ډول() != ننوت_Array_2::STD_ویکټور_ویکټور &&
            src.ډول() != ننوت_Array_2::STD_ویکټور_UMAT;
    منطقي dst_is_mat = dst.ډول() != ننوت_Array_2::STD_ویکټور_MAT &&
            dst.ډول() != ننوت_Array_2::STD_ARRAY_MAT &&
            dst.ډول() != ننوت_Array_2::STD_ویکټور_ویکټور &&
            dst.ډول() != ننوت_Array_2::STD_ویکټور_UMAT;
    صحيح i;
    صحيح nsrc = src_is_mat ? 1 : (صحيح)src.ټول();
    صحيح ndst = dst_is_mat ? 1 : (صحيح)dst.ټول();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.معلومات();
    لكل( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    لكل( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_منزل, npairs);
}

فراغ cv::mixChannels(ننوت_Array_د_Arrays src, ننوت_Output_Array_د_Arrays dst,
                     ثابت std::vector<صحيح>& from_منزل)
{
    CV_INSTRUMENT_REGION();

    إذا (from_منزل.empty())
        أرجع;

    CV_OCL_پرلپسې(dst.is_UMat_ویکټور(),
               ocl_mixChannels(src, dst, &from_منزل[0], from_منزل.size()>>1))

    منطقي src_is_mat = src.ډول() != ننوت_Array_2::STD_ویکټور_MAT &&
            src.ډول() != ننوت_Array_2::STD_ARRAY_MAT &&
            src.ډول() != ننوت_Array_2::STD_ویکټور_ویکټور &&
            src.ډول() != ننوت_Array_2::STD_ویکټور_UMAT;
    منطقي dst_is_mat = dst.ډول() != ننوت_Array_2::STD_ویکټور_MAT &&
            dst.ډول() != ننوت_Array_2::STD_ARRAY_MAT &&
            dst.ډول() != ننوت_Array_2::STD_ویکټور_ویکټور &&
            dst.ډول() != ننوت_Array_2::STD_ویکټور_UMAT;
    صحيح i;
    صحيح nsrc = src_is_mat ? 1 : (صحيح)src.ټول();
    صحيح ndst = dst_is_mat ? 1 : (صحيح)dst.ټول();

    CV_Assert(from_منزل.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.معلومات();
    لكل( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    لكل( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_منزل[0], from_منزل.size()/2);
}

#که_تعریف_شوی HAVE_IPP

نطاق cv
{
ساكن منطقي ipp_ایستل_Channel(ثابت Mat &src, Mat &dst, صحيح channel)
{
#که_تعریف_شوی HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحيح srcChannels = src.channels();
    صحيح dstChannels = dst.channels();

    إذا(src.dims != dst.dims)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_اندازه size = ippi_اندازه(src.size());

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحيح)src.ګام, srcChannels, channel, dst.ptr(), (صحيح)dst.ګام, dstChannels, 0, size, (صحيح)src.elemSize1()) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_اندازه size = {(صحيح)it.size, 1};

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (صحيح)src.elemSize1()) < 0)
                أرجع خطأ;
        }
        أرجع ریښتیا;
    }
#وإلا
    CV_ناکارول_شوی(src); CV_ناکارول_شوی(dst); CV_ناکارول_شوی(channel);
    أرجع خطأ;
#د_که_پای
}

ساكن منطقي ipp_ورننباسه_Channel(ثابت Mat &src, Mat &dst, صحيح channel)
{
#که_تعریف_شوی HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحيح srcChannels = src.channels();
    صحيح dstChannels = dst.channels();

    إذا(src.dims != dst.dims)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_اندازه size = ippi_اندازه(src.size());

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحيح)src.ګام, srcChannels, 0, dst.ptr(), (صحيح)dst.ګام, dstChannels, channel, size, (صحيح)src.elemSize1()) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_اندازه size = {(صحيح)it.size, 1};

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (صحيح)src.elemSize1()) < 0)
                أرجع خطأ;
        }
        أرجع ریښتیا;
    }
#وإلا
    CV_ناکارول_شوی(src); CV_ناکارول_شوی(dst); CV_ناکارول_شوی(channel);
    أرجع خطأ;
#د_که_پای
}
}
#د_که_پای

فراغ cv::ایستل_Channel(ننوت_Array _src, OutputArray _dst, صحيح coi)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), ژوروالی = CV_MAT_ژوروالی(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    صحيح ch[] = { coi, 0 };

#که_تعریف_شوی HAVE_OPENCL
    إذا (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_اندازه(_src, ژوروالی);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        أرجع;
    }
#د_که_پای

    Mat src = _src.getMat();
    _dst.create_Same_اندازه(_src, ژوروالی);
    Mat dst = _dst.getMat();

    CV_IPP_پرلپسې_FAST(ipp_ایستل_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

فراغ cv::ورننباسه_Channel(ننوت_Array _src, ننوت_Output_Array _dst, صحيح coi)
{
    CV_INSTRUMENT_REGION();

    صحيح stype = _src.type(), sdepth = CV_MAT_ژوروالی(stype), scn = CV_MAT_CN(stype);
    صحيح dtype = _dst.type(), ddepth = CV_MAT_ژوروالی(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_اندازه(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    صحيح ch[] = { 0, coi };
#که_تعریف_شوی HAVE_OPENCL
    إذا (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        أرجع;
    }
#د_که_پای

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_پرلپسې_FAST(ipp_ورننباسه_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
