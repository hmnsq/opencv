// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html


#شامل "precomp.hpp"
#شامل "opencl_kernels_core.hpp"
#شامل "convert.hpp"

/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

نامخلا cv
{

سانچہ<قسم_نام T> ساکن خالی
mixChannels_( ثابت T** src, ثابت صحیح* sdelta,
              T** dst, ثابت صحیح* ddelta,
              صحیح len, صحیح npairs )
{
    صحیح i, k;
    برائے( k = 0; k < npairs; k++ )
    {
        ثابت T* s = src[k];
        T* d = dst[k];
        صحیح ds = sdelta[k], dd = ddelta[k];
        اگر( s )
        {
            برائے( i = 0; i <= len - 2; i += 2, s += ds*2, d += dd*2 )
            {
                T t0 = s[0], t1 = s[ds];
                d[0] = t0; d[dd] = t1;
            }
            اگر( i < len )
                d[0] = s[0];
        }
        ورنہ
        {
            برائے( i = 0; i <= len - 2; i += 2, d += dd*2 )
                d[0] = d[dd] = 0;
            اگر( i < len )
                d[0] = 0;
        }
    }
}


ساکن خالی mixChannels8u( ثابت خالی** src, ثابت صحیح* sdelta,
                           خالی** dst, ثابت صحیح* ddelta,
                           صحیح len, صحیح npairs )
{
    mixChannels_((ثابت uchar**)src, sdelta, (uchar**)dst, ddelta, len, npairs);
}

ساکن خالی mixChannels16u( ثابت خالی** src, ثابت صحیح* sdelta,
                            خالی** dst, ثابت صحیح* ddelta,
                            صحیح len, صحیح npairs )
{
    mixChannels_((ثابت ushort**)src, sdelta, (ushort**)dst, ddelta, len, npairs);
}

ساکن خالی mixChannels32s( ثابت خالی** src, ثابت صحیح* sdelta,
                            خالی** dst, ثابت صحیح* ddelta,
                            صحیح len, صحیح npairs )
{
    mixChannels_((ثابت صحیح**)src, sdelta, (صحیح**)dst, ddelta, len, npairs);
}

ساکن خالی mixChannels64s( ثابت خالی** src, ثابت صحیح* sdelta,
                            خالی** dst, ثابت صحیح* ddelta,
                            صحیح len, صحیح npairs )
{
    mixChannels_((ثابت int64**)src, sdelta, (int64**)dst, ddelta, len, npairs);
}

قسمتعریف خالی (*MixChannelsFunc)( ثابت خالی** src, ثابت صحیح* sdelta,
        خالی** dst, ثابت صحیح* ddelta, صحیح len, صحیح npairs );

ساکن MixChannelsFunc getMixchFunc(صحیح گہرائی)
{
    ساکن MixChannelsFunc mixchTab[CV_گہرائی_MAX] =
    {
        mixChannels8u, mixChannels8u, mixChannels16u,
        mixChannels16u, mixChannels32s, mixChannels32s,
        mixChannels64s, mixChannels16u, mixChannels16u,
        mixChannels8u, mixChannels64s, mixChannels64s,
        mixChannels32s, mixChannels8u, mixChannels8u
    };

    واپس mixchTab[گہرائی];
}

} // cv::


خالی cv::mixChannels( ثابت Mat* src, size_t nsrcs, Mat* dst, size_t ndsts, ثابت صحیح* from_مقصد, size_t npairs )
{
    CV_INSTRUMENT_REGION();

    اگر( npairs == 0 )
        واپس;
    CV_Assert( src && nsrcs > 0 && dst && ndsts > 0 && from_مقصد && npairs > 0 );

    size_t i, j, k, esz1 = dst[0].elemSize1();
    صحیح گہرائی = dst[0].گہرائی();

    Auto_بفر<uchar> buf((nsrcs + ndsts + 1)*(حجم(Mat*) + حجم(uchar*)) + npairs*(حجم(uchar*)*2 + حجم(صحیح)*6));
    ثابت Mat** arrays = (ثابت Mat**)(uchar*)buf.ڈیٹا();
    uchar** ptrs = (uchar**)(arrays + nsrcs + ndsts);
    ثابت uchar** srcs = (ثابت uchar**)(ptrs + nsrcs + ndsts + 1);
    uchar** dsts = (uchar**)(srcs + npairs);
    صحیح* tab = (صحیح*)(dsts + npairs);
    صحیح *sdelta = (صحیح*)(tab + npairs*4), *ddelta = sdelta + npairs;

    برائے( i = 0; i < nsrcs; i++ )
    {
        CV_Assert( !src[i].empty() && src[i].size == dst[0].size && src[i].گہرائی() == گہرائی );
        arrays[i] = &src[i];
    }
    برائے( i = 0; i < ndsts; i++ )
    {
        CV_Assert( !dst[i].empty() && dst[i].size == dst[0].size && dst[i].گہرائی() == گہرائی );
        arrays[i + nsrcs] = &dst[i];
    }
    ptrs[nsrcs + ndsts] = 0;

    برائے( i = 0; i < npairs; i++ )
    {
        صحیح i0 = from_مقصد[i*2], i1 = from_مقصد[i*2+1];
        اگر( i0 >= 0 )
        {
            برائے( j = 0; j < nsrcs; i0 -= src[j].channels(), j++ )
                اگر( i0 < src[j].channels() )
                    توڑو;
            CV_Assert(j < nsrcs && src[j].گہرائی() == گہرائی);
            tab[i*4] = (صحیح)j; tab[i*4+1] = (صحیح)(i0*esz1);
            sdelta[i] = src[j].channels();
        }
        ورنہ
        {
            tab[i*4] = (صحیح)(nsrcs + ndsts); tab[i*4+1] = 0;
            sdelta[i] = 0;
        }

        برائے( j = 0; j < ndsts; i1 -= dst[j].channels(), j++ )
            اگر( i1 < dst[j].channels() )
                توڑو;
        CV_Assert(i1 >= 0 && j < ndsts && dst[j].گہرائی() == گہرائی);
        tab[i*4+2] = (صحیح)(j + nsrcs); tab[i*4+3] = (صحیح)(i1*esz1);
        ddelta[i] = dst[j].channels();
    }

    NAryMatIterator it(arrays, ptrs, (صحیح)(nsrcs + ndsts));
    صحیح کل = (صحیح)it.size, blocksize = std::min(کل, (صحیح)((بلاک_سائز + esz1-1)/esz1));
    MixChannelsFunc func = getMixchFunc(گہرائی);
    CV_Assert(func);

    برائے( i = 0; i < it.nplanes; i++, ++it )
    {
        برائے( k = 0; k < npairs; k++ )
        {
            srcs[k] = ptrs[tab[k*4]] + tab[k*4+1];
            dsts[k] = ptrs[tab[k*4+2]] + tab[k*4+3];
        }

        برائے( صحیح t = 0; t < کل; t += blocksize )
        {
            صحیح bsz = std::min(کل - t, blocksize);
            func( (ثابت خالی**)srcs, sdelta, (خالی **)dsts, ddelta, bsz, (صحیح)npairs );

            اگر( t + blocksize < کل )
                برائے( k = 0; k < npairs; k++ )
                {
                    srcs[k] += blocksize*sdelta[k]*esz1;
                    dsts[k] += blocksize*ddelta[k]*esz1;
                }
        }
    }
}

#اگر_تعریف_شدہ HAVE_OPENCL

نامخلا cv {

ساکن خالی get_UMat_اشاریہ(ثابت std::vector<UMat> & um, صحیح cn, صحیح & idx, صحیح & cnidx)
{
    صحیح کل_Channels = 0;
    برائے (size_t i = 0, size = um.size(); i < size; ++i)
    {
        صحیح ccn = um[i].channels();
        کل_Channels += ccn;

        اگر (کل_Channels == cn)
        {
            idx = (صحیح)(i + 1);
            cnidx = 0;
            واپس;
        }
        ورنہ اگر (کل_Channels > cn)
        {
            idx = (صحیح)i;
            cnidx = i == 0 ? cn : (cn - کل_Channels + ccn);
            واپس;
        }
    }

    idx = cnidx = -1;
}

ساکن منطقی ocl_mixChannels(داخلہ_Array_کا_Arrays _src, داخلہ_Output_Array_کا_Arrays _dst,
                            ثابت صحیح* from_مقصد, size_t npairs)
{
    std::vector<UMat> src, dst;
    _src.get_UMat_سمتیہ(src);
    _dst.get_UMat_سمتیہ(dst);

    size_t nsrc = src.size(), ndst = dst.size();
    CV_Assert(nsrc > 0 && ndst > 0);

    سائز size = src[0].size();
    صحیح گہرائی = src[0].گہرائی(), esz = CV_ELEM_سائز(گہرائی),
            صفیں_فی_WI = ocl::Device::getDefault().isIntel() ? 4 : 1;

    برائے (size_t i = 1, ssize = src.size(); i < ssize; ++i)
        CV_Assert(src[i].size() == size && src[i].گہرائی() == گہرائی);
    برائے (size_t i = 0, dsize = dst.size(); i < dsize; ++i)
        CV_Assert(dst[i].size() == size && dst[i].گہرائی() == گہرائی);

    String declsrc, decldst, declproc, declcn, indexdecl;
    std::vector<UMat> srcargs(npairs), dstargs(npairs);

    برائے (size_t i = 0; i < npairs; ++i)
    {
        صحیح scn = from_مقصد[i<<1], dcn = from_مقصد[(i<<1) + 1];
        صحیح src_idx, src_cnidx, dst_idx, dst_cnidx;

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
                         ocl::memop_Type_مقصد_Str(گہرائی), declsrc.c_str(), decldst.c_str(),
                         declproc.c_str(), indexdecl.c_str(), declcn.c_str()));
    اگر (k.empty())
        واپس جھوٹ;

    صحیح argindex = 0;
    برائے (size_t i = 0; i < npairs; ++i)
        argindex = k.سیٹ(argindex, ocl::KernelArg::Read_Only_No_سائز(srcargs[i]));
    برائے (size_t i = 0; i < npairs; ++i)
        argindex = k.سیٹ(argindex, ocl::KernelArg::Write_Only_No_سائز(dstargs[i]));
    argindex = k.سیٹ(argindex, size.اونچائی);
    argindex = k.سیٹ(argindex, size.چوڑائی);
    k.سیٹ(argindex, صفیں_فی_WI);

    size_t globalsize[2] = { (size_t)size.چوڑائی, ((size_t)size.اونچائی + صفیں_فی_WI - 1) / صفیں_فی_WI };
    واپس k.تسلسل(2, globalsize, تہی, جھوٹ);
}

}

#اگر_ختم

خالی cv::mixChannels(داخلہ_Array_کا_Arrays src, داخلہ_Output_Array_کا_Arrays dst,
                 ثابت صحیح* from_مقصد, size_t npairs)
{
    CV_INSTRUMENT_REGION();

    اگر (npairs == 0 || from_مقصد == تہی)
        واپس;

    CV_OCL_تسلسل(dst.is_UMat_سمتیہ(),
               ocl_mixChannels(src, dst, from_مقصد, npairs))

    منطقی src_is_mat = src.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            src.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    منطقی dst_is_mat = dst.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    صحیح i;
    صحیح nsrc = src_is_mat ? 1 : (صحیح)src.کل();
    صحیح ndst = dst_is_mat ? 1 : (صحیح)dst.کل();

    CV_Assert(nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.ڈیٹا();
    برائے( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    برائے( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, from_مقصد, npairs);
}

خالی cv::mixChannels(داخلہ_Array_کا_Arrays src, داخلہ_Output_Array_کا_Arrays dst,
                     ثابت std::vector<صحیح>& from_مقصد)
{
    CV_INSTRUMENT_REGION();

    اگر (from_مقصد.empty())
        واپس;

    CV_OCL_تسلسل(dst.is_UMat_سمتیہ(),
               ocl_mixChannels(src, dst, &from_مقصد[0], from_مقصد.size()>>1))

    منطقی src_is_mat = src.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            src.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            src.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    منطقی dst_is_mat = dst.قسم() != داخلہ_Array_2::STD_سمتیہ_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_ARRAY_MAT &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_سمتیہ &&
            dst.قسم() != داخلہ_Array_2::STD_سمتیہ_UMAT;
    صحیح i;
    صحیح nsrc = src_is_mat ? 1 : (صحیح)src.کل();
    صحیح ndst = dst_is_mat ? 1 : (صحیح)dst.کل();

    CV_Assert(from_مقصد.size()%2 == 0 && nsrc > 0 && ndst > 0);
    cv::Auto_بفر<Mat> _buf(nsrc + ndst);
    Mat* buf = _buf.ڈیٹا();
    برائے( i = 0; i < nsrc; i++ )
        buf[i] = src.getMat(src_is_mat ? -1 : i);
    برائے( i = 0; i < ndst; i++ )
        buf[nsrc + i] = dst.getMat(dst_is_mat ? -1 : i);
    mixChannels(&buf[0], nsrc, &buf[nsrc], ndst, &from_مقصد[0], from_مقصد.size()/2);
}

#اگر_تعریف_شدہ HAVE_IPP

نامخلا cv
{
ساکن منطقی ipp_نکالو_Channel(ثابت Mat &src, Mat &dst, صحیح channel)
{
#اگر_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحیح srcChannels = src.channels();
    صحیح dstChannels = dst.channels();

    اگر(src.dims != dst.dims)
        واپس جھوٹ;

    اگر(src.dims <= 2)
    {
        Ippi_سائز size = ippi_سائز(src.size());

        واپس CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحیح)src.قدم, srcChannels, channel, dst.ptr(), (صحیح)dst.قدم, dstChannels, 0, size, (صحیح)src.elemSize1()) >= 0;
    }
    ورنہ
    {
        ثابت Mat      *arrays[] = {&dst, تہی};
        uchar          *ptrs[2]  = {تہی};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائز size = {(صحیح)it.size, 1};

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, channel, ptrs[1], 0, dstChannels, 0, size, (صحیح)src.elemSize1()) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
#ورنہ
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(dst); CV_غیر_مستعمل(channel);
    واپس جھوٹ;
#اگر_ختم
}

ساکن منطقی ipp_داخل_کرو_Channel(ثابت Mat &src, Mat &dst, صحیح channel)
{
#اگر_تعریف_شدہ HAVE_IPP_IW_LL
    CV_INSTRUMENT_REGION_IPP();

    صحیح srcChannels = src.channels();
    صحیح dstChannels = dst.channels();

    اگر(src.dims != dst.dims)
        واپس جھوٹ;

    اگر(src.dims <= 2)
    {
        Ippi_سائز size = ippi_سائز(src.size());

        واپس CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, src.ptr(), (صحیح)src.قدم, srcChannels, 0, dst.ptr(), (صحیح)dst.قدم, dstChannels, channel, size, (صحیح)src.elemSize1()) >= 0;
    }
    ورنہ
    {
        ثابت Mat      *arrays[] = {&dst, تہی};
        uchar          *ptrs[2]  = {تہی};
        NAryMatIterator it(arrays, ptrs);

        Ippi_سائز size = {(صحیح)it.size, 1};

        برائے( size_t i = 0; i < it.nplanes; i++, ++it )
        {
            اگر(CV_INSTRUMENT_FUN_IPP(llwiCopyChannel, ptrs[0], 0, srcChannels, 0, ptrs[1], 0, dstChannels, channel, size, (صحیح)src.elemSize1()) < 0)
                واپس جھوٹ;
        }
        واپس سچ;
    }
#ورنہ
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(dst); CV_غیر_مستعمل(channel);
    واپس جھوٹ;
#اگر_ختم
}
}
#اگر_ختم

خالی cv::نکالو_Channel(داخلہ_Array _src, OutputArray _dst, صحیح coi)
{
    CV_INSTRUMENT_REGION();

    صحیح type = _src.type(), گہرائی = CV_MAT_گہرائی(type), cn = CV_MAT_CN(type);
    CV_Assert( 0 <= coi && coi < cn );
    صحیح ch[] = { coi, 0 };

#اگر_تعریف_شدہ HAVE_OPENCL
    اگر (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat();
        // NB: fetch the source before (re)allocating the destination so that
        // in-place operation (_src and _dst referencing the same array) keeps
        // the multi-channel source alive while dst is reshaped to 1 channel.
        _dst.create_Same_سائز(_src, گہرائی);
        UMat dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        واپس;
    }
#اگر_ختم

    Mat src = _src.getMat();
    _dst.create_Same_سائز(_src, گہرائی);
    Mat dst = _dst.getMat();

    CV_IPP_تسلسل_FAST(ipp_نکالو_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}

خالی cv::داخل_کرو_Channel(داخلہ_Array _src, داخلہ_Output_Array _dst, صحیح coi)
{
    CV_INSTRUMENT_REGION();

    صحیح stype = _src.type(), sdepth = CV_MAT_گہرائی(stype), scn = CV_MAT_CN(stype);
    صحیح dtype = _dst.type(), ddepth = CV_MAT_گہرائی(dtype), dcn = CV_MAT_CN(dtype);
    CV_Assert( _src.same_سائز(_dst) && sdepth == ddepth );
    CV_Assert( 0 <= coi && coi < dcn && scn == 1 );

    صحیح ch[] = { 0, coi };
#اگر_تعریف_شدہ HAVE_OPENCL
    اگر (ocl::isOpenCLActivated() && _src.dims() <= 2 && _dst.isUMat())
    {
        UMat src = _src.getUMat(), dst = _dst.getUMat();
        mixChannels(std::vector<UMat>(1, src), std::vector<UMat>(1, dst), ch, 1);
        واپس;
    }
#اگر_ختم

    Mat src = _src.getMat(), dst = _dst.getMat();

    CV_IPP_تسلسل_FAST(ipp_داخل_کرو_Channel(src, dst, coi))

    mixChannels(&src, 1, &dst, 1, ch, 1);
}
