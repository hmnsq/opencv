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

سانچا<قسم_ناں T> ساكن فراغ
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

ساكن MixChannelsFunc getMixchFunc(صحيح ڈونگھائی)
{
    ساكن MixChannelsFunc mixchTab[CV_ڈونگھائی_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    أرجع mixchTab[ڈونگھائی];
}

} // cv::


فراغ cv::mixChannels( ثابت Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ثابت صحيح* from_منزل, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    إذا( npairs == 0 )
        أرجع;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_منزل && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    صحيح ڈونگھائی = dst[0].ڈونگھائی();

    Auto_بفر<uchar> buf((nsrcs + ndsts + 1)*(حجم(Mat*) + حجم(uchar*)) + npairs*(حجم(uchar*)*2 + حجم(صحيح)*6));
    ثابت Mat** arrays = (ثابت Mat**)(uchar*)buf.انکڑے();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ثابت uchar** srcs = (ثابت uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    صحيح* tab = (صحيح*)(dsts + npairs);
    صحيح *sdelta = (صحيح*)(tab + npairs*4), *ddelta = sdelta + npairs;

    لكل( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].ڈونگھائی() == ڈونگھائی );
        arrays[i] = &src[i];
    }
    لكل( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].ڈونگھائی() == ڈونگھائی );
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
            CV_Assert(j < nsrcs && src[j].ڈونگھائی() == ڈونگھائی);
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
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].ڈونگھائی() == ڈونگھائی);
        tab[i*4+2] = (صحيح)(j + nsrcs); tab[i*4+3] = (صحيح)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (صحيح)(nsrcs + ndsts));
    صحيح کل = (صحيح)it.size, blocksize = std::min(کل, (صحيح)((بلاک_ناپ + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(ڈونگھائی);
    CV_Assert(func);

    لكل( i = 0; i < it.nplanes; i++, ++it )
    {
        لكل( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        لكل( صحيح t = 0; t < کل; t += blocksize )
        {
            صحيح bsz = std::min(کل - t, blocksize);
            func( (ثابت فراغ**)srcs, sdelta, (فراغ **)dsts, ddelta, bsz, (صحيح)npairs );

            إذا( t + blocksize < کل )
                لكل( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#جے_تعریف_شدہ HAVE_OPENCL

نطاق cv {

ساكن فراغ get_UMat_اشاریہ(ثابت std::vector<UMat> & um, صحيح cn, صحيح & idx, صحيح & cnidx)
{
    صحيح کل_Channels = 0;
    لكل (size_t i = 0, size = um.size(); i < size; ++i)
    {
        صحيح ccn = um[i].channels();
        کل_Channels += ccn;

        إذا (کل_Channels == cn)
        {
            idx = (صحيح)(i + 1);
            cnidx = 0;
            أرجع;
        }
        وإلا إذا (کل_Channels > cn)
        {
            idx = (صحيح)i;
            cnidx = i == 0 ? cn : (cn - کل_Channels + ccn);
            أرجع;
        }
    }

    idx = cnidx = -1;
}

ساكن منطقي ocl_mixChannels(داخلہ_Array_دا_Arrays _src, داخلہ_Output_Array_دا_Arrays _dst,
                            ثابت صحيح* from_منزل, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_سمتیہ(src);
    _dst.get_UMat_سمتیہ(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    ناپ size = src[0].size();
    صحيح ڈونگھائی = src[0].ڈونگھائی(), esz = CV_ELEM_ناپ(ڈونگھائی),
            صفاں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    لكل (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].ڈونگھائی() == ڈونگھائی);
    لكل (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].ڈونگھائی() == ڈونگھائی);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    لكل (size_t i = 0; i < npairs; ++i)
    {
        صحيح scn = from_منزل[i<<1], dcn = from_منزل[(i<<1) + 1];
        صحيح src_idx, src_cnidx, dst_idx, dst_cnidx;

        get_UMat_اشاریہ(src, scn, src_idx, src_cnidx);
        get_UMat_اشاریہ(dst, dcn, dst_idx, dst_cnidx);

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
                         ocl::memop_Type_منزل_Str(ڈونگھائی), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    إذا (k.empty())
        أرجع خطأ;

    صحيح argindex = 0;
    لكل (size_t i = 0; i < npairs; ++i)
        argindex = k.سیٹ(argindex, ocl::KernelArg::Read_Only_No_ناپ(srcargs[i]));
    لكل (size_t i = 0; i < npairs; ++i)
        argindex = k.سیٹ(argindex, ocl::KernelArg::Write_Only_No_ناپ(dstargs[i]));
    argindex = k.سیٹ(argindex, size.اچائی);
    argindex = k.سیٹ(argindex, size.چوڑائی);
    k.سیٹ(argindex, صفاں_فی_WI);

    size_t globalsize[2] = { (size_t)size.چوڑائی, ((size_t)size.اچائی + صفاں_فی_WI - 1) / صفاں_فی_WI };
    أرجع k.لگاتار(2, globalsize, عدم, خطأ);
}

}

#جے_انت

فراغ cv::mixChannels(داخلہ_Array_دا_Arrays src, داخلہ_Output_Array_دا_Arrays dst,
                 ثابت صحيح* from_منزل, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    إذا (npairs == 0 || from_منزل == عدم)
        أرجع;

    CV_OCL_لگاتار(dst.is_UMat_سمتیہ(),
               ocl_mixChannels(src, dst, from_منزل, npairs))

    منطقي src_is_mat = src.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            src.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    منطقي dst_is_mat = dst.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    صحيح i;
    صحيح nsrc = src_is_mat ? 1 : (صحيح)src.کل();
    صحيح ndst = dst_is_mat ? 1 : (صحيح)dst.کل();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.انکڑے();
    لكل( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    لكل( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_منزل, npairs);
}

فراغ cv::mixChannels(داخلہ_Array_دا_Arrays src, داخلہ_Output_Array_دا_Arrays dst,
                     ثابت std::vector<صحيح>& from_منزل)
{
    CV_INSTRUMENT_REGION();

    إذا (from_منزل.empty())
        أرجع;

    CV_OCL_لگاتار(dst.is_UMat_سمتیہ(),
               ocl_mixChannels(src, dst, &from_منزل[0], from_منزل.size()>>1))

    منطقي src_is_mat = src.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            src.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    منطقي dst_is_mat = dst.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    صحيح i;
    صحيح nsrc = src_is_mat ? 1 : (صحيح)src.کل();
    صحيح ndst = dst_is_mat ? 1 : (صحيح)dst.کل();

    CV_Assert(from_منزل.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.انکڑے();
    لكل( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    لكل( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_منزل[0], from_منزل.size()/2);
}

#جے_تعریف_شدہ HAVE_IPP

نطاق cv
{
ساكن منطقي ipp_کڈھو_Channel(ثابت Mat &src, Mat &dst, صحيح channel)
{
#جے_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحيح srcChannels = src.channels();
    صحيح dstChannels = dst.channels();

    إذا(src.dims != dst.dims)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_ناپ size = ippi_ناپ(src.size());

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحيح)src.قدم, srcChannels, channel, dst.ptr(), (صحيح)dst.قدم, dstChannels, 0, size, (صحيح)src.elemSize1()) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ناپ size = {(صحيح)it.size, 1};

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (صحيح)src.elemSize1()) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_بن_ورتیا(src); CV_بن_ورتیا(dst); CV_بن_ورتیا(channel);
    أرجع خطأ;
#جے_انت
}

ساكن منطقي ipp_داخل_کرو_Channel(ثابت Mat &src, Mat &dst, صحيح channel)
{
#جے_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحيح srcChannels = src.channels();
    صحيح dstChannels = dst.channels();

    إذا(src.dims != dst.dims)
        أرجع خطأ;

    إذا(src.dims <= 2)
    {
        Ippi_ناپ size = ippi_ناپ(src.size());

        أرجع CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحيح)src.قدم, srcChannels, 0, dst.ptr(), (صحيح)dst.قدم, dstChannels, channel, size, (صحيح)src.elemSize1()) >= 0;
    }
    وإلا
    {
        ثابت Mat      *arrays[] = {&dst, عدم};
        uchar          *ptrs[2]  = {عدم};
        NAryMatIterator it(arrays, ptrs);

        Ippi_ناپ size = {(صحيح)it.size, 1};

        لكل( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            إذا(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (صحيح)src.elemSize1()) < 0)
                أرجع خطأ;
        }
        أرجع سچ;
    }
#وإلا
    CV_بن_ورتیا(src); CV_بن_ورتیا(dst); CV_بن_ورتیا(channel);
    أرجع خطأ;
#جے_انت
}
}
#جے_انت

فراغ cv::کڈھو_Channel(داخلہ_Array _src, OutputArray _dst, صحيح coi)
{
    CV_INSTRUMENT_REGION();

    صحيح type = _src.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    صحيح ch[] = { coi, 0 };

#جے_تعریف_شدہ HAVE_OPENCL
    إذا (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_ناپ(_src, ڈونگھائی);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        أرجع;
    }
#جے_انت

    Mat src = _src.getMat();
    _dst.create_Same_ناپ(_src, ڈونگھائی);
    Mat dst = _dst.getMat();

    CV_IPP_لگاتار_FAST(ipp_کڈھو_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

فراغ cv::داخل_کرو_Channel(داخلہ_Array _src, داخلہ_Output_Array _dst, صحيح coi)
{
    CV_INSTRUMENT_REGION();

    صحيح stype = _src.type(), sdepth = CV_MAT_ڈونگھائی(stype), scn = CV_MAT_CN(stype);
    صحيح dtype = _dst.type(), ddepth = CV_MAT_ڈونگھائی(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_ناپ(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    صحيح ch[] = { 0, coi };
#جے_تعریف_شدہ HAVE_OPENCL
    إذا (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        أرجع;
    }
#جے_انت

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_لگاتار_FAST(ipp_داخل_کرو_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
