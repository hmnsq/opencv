// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"

نامخلا cv {

قسمتعریف Vec<صحیح, 5> Vec5i;
قسمتعریف Vec<صحیح, 7> Vec7i;
قسمتعریف Vec<صحیح, 9> Vec9i;
قسمتعریف Vec<صحیح, 10> Vec10i;
قسمتعریف Vec<صحیح, 11> Vec11i;
قسمتعریف Vec<صحیح, 12> Vec12i;
قسمتعریف Vec<صحیح, 13> Vec13i;
قسمتعریف Vec<صحیح, 14> Vec14i;
قسمتعریف Vec<صحیح, 15> Vec15i;
قسمتعریف Vec<صحیح, 16> Vec16i;
قسمتعریف Vec<صحیح, 32> Vec32i;
قسمتعریف Vec<صحیح, 64> Vec64i;
قسمتعریف Vec<صحیح, 128> Vec128i;

#تعریف_منسوخ STD_سمتیہ_SWITCH
#تعریف_کرو STD_سمتیہ_SWITCH(esz, func) \
    انتخاب( esz ) \
    { \
    حالت 1: func(uchar); \
    حالت 2: func(Vec2b); \
    حالت 3: func(Vec3b); \
    حالت 4: func(صحیح); \
    حالت 6: func(Vec3s); \
    حالت 8: func(Vec2i); \
    حالت 12: func(Vec3i); \
    حالت 16: func(Vec4i); \
    حالت 20: func(Vec5i); \
    حالت 24: func(Vec6i); \
    حالت 28: func(Vec7i); \
    حالت 32: func(Vec8i); \
    حالت 36: func(Vec9i); \
    حالت 40: func(Vec10i); \
    حالت 44: func(Vec11i); \
    حالت 48: func(Vec12i); \
    حالت 52: func(Vec13i); \
    حالت 56: func(Vec14i); \
    حالت 60: func(Vec15i); \
    حالت 64: func(Vec16i); \
    حالت 128: func(Vec32i); \
    حالت 256: func(Vec64i); \
    حالت 512: func(Vec128i); \
    طےشدہ: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat داخلہ_Array_2::getMat_(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    اگر( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        اگر( i < 0 )
            واپس *m;
        واپس m->صف(i);
    }

    اگر( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        اگر( i < 0 )
            واپس m->getMat(accessFlags);
        واپس m->getMat(accessFlags).صف(i);
    }

    اگر (k == MATX)
    {
        CV_Assert( i < 0 );
        واپس Mat(sz, flags, obj);
    }

    اگر( k == STD_BOOL_سمتیہ )
    {
        CV_Assert( i < 0 );
        صحیح t = CV_8U;
        ثابت std::vector<منطقی>& v = *(ثابت std::vector<منطقی>*)obj;
        صحیح j, n = (صحیح)v.size();
        اگر( n == 0 )
            واپس Mat();
        Mat m(1, &n, t);
        uchar* dst = m.ڈیٹا;
        برائے( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        واپس m;
    }

    اگر( k == NONE )
        واپس Mat();

    اگر( k == STD_سمتیہ || k == STD_سمتیہ_سمتیہ )
    {
        صحیح t = CV_MAT_TYPE(flags);
        صحیح esz = CV_ELEM_سائز(t);

    #تعریف_منسوخ GET_MAT
    #تعریف_کرو GET_MAT(T) \
        { \
            std::vector<T>* v; \
            اگر (k == STD_سمتیہ_سمتیہ) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ورنہ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            صحیح v_sz = صحیح(v->size()); \
            واپس v->empty() ? Mat() : Mat(1, &v_sz, t, v->ڈیٹا()); \
        }

        STD_سمتیہ_SWITCH(esz, GET_MAT)
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (صحیح)v.size() );

        واپس v[i];
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        CV_Assert( 0 <= i && i < sz.اونچائی );

        واپس v[i];
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحیح)v.size() );

        واپس v[i].getMat(accessFlags);
    }

    اگر( k == OPENGL_بفر )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    اگر( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    اگر( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;

        واپس cuda_mem->create_Mat_سرخی();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat داخلہ_Array_2::getUMat(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    اگر( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        اگر( i < 0 )
            واپس *m;
        واپس m->صف(i);
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحیح)v.size() );

        واپس v[i];
    }

    اگر( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        اگر( i < 0 )
            واپس m->getUMat(accessFlags);
        واپس m->صف(i).getUMat(accessFlags);
    }

    واپس getMat(i).getUMat(accessFlags);
}

خالی داخلہ_Array_2::get_Mat_سمتیہ(std::vector<Mat>& mv) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    اگر( k == MAT )
    {
        ثابت Mat& m = *(ثابت Mat*)obj;
        صحیح n = (صحیح)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        برائے( صحیح i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.ستون_تعداد, m.type(), (خالی*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (خالی*)m.ptr(i), &m.قدم[1]);
        واپس;
    }

    اگر (k == MATX)
    {
        size_t n = sz.اونچائی, esz = CV_ELEM_سائز(flags);
        mv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.چوڑائی, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.چوڑائی*i);
        واپس;
    }

    اگر( k == STD_سمتیہ )
    {
        ثابت std::vector<uchar>& v = *(ثابت std::vector<uchar>*)obj;

        size_t n = کل(), esz = CV_ELEM_سائز(flags);
        صحیح t = CV_MAT_گہرائی(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (خالی*)(v.ڈیٹا() + esz*i));
        واپس;
    }

    اگر( k == NONE )
    {
        mv.clear();
        واپس;
    }

    اگر( k == STD_سمتیہ_سمتیہ )
    {
        صحیح typ = CV_MAT_TYPE(flags);
        صحیح esz = CV_ELEM_سائز(typ);

        #تعریف_منسوخ GET_VEC_VEC_MAT_VEC
        #تعریف_کرو GET_VEC_VEC_MAT_VEC(T) \
        { \
            ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            برائے (size_t i = 0; i < n; i++) { \
                ثابت std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)زیادہ_سے_زیادہ_عدد); \
                صحیح ni = (صحیح)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (خالی*)vi.ڈیٹا()) : Mat(); \
            } \
        } \
        توڑو

        STD_سمتیہ_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        واپس;
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        واپس;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.اونچائی;
        mv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        واپس;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        واپس;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

خالی داخلہ_Array_2::get_UMat_سمتیہ(std::vector<UMat>& umv) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    اگر( k == NONE )
    {
        umv.clear();
        واپس;
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        واپس;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.اونچائی;
        umv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        واپس;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        برائے( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        واپس;
    }

    اگر( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        واپس;
    }
    اگر( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        واپس;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat داخلہ_Array_2::getGpuMat() ثابت
{
#اگر_تعریف_شدہ HAVE_CUDA
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر (k == CUDA_GPU_MAT)
    {
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        واپس *d_mat;
    }

    اگر (k == CUDA_HOST_MEM)
    {
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        واپس cuda_mem->create_Gpu_Mat_سرخی();
    }

    اگر (k == OPENGL_بفر)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    اگر (k == NONE)
        واپس cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ورنہ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
}
خالی داخلہ_Array_2::get_Gpu_Mat_سمتیہ(std::vector<cuda::GpuMat>& gpumv) ثابت
{
#اگر_تعریف_شدہ HAVE_CUDA
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ورنہ
    CV_غیر_مستعمل(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
}
خالی داخلہ_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ثابت
{
#اگر_تعریف_شدہ HAVE_CUDA
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ورنہ
    CV_غیر_مستعمل(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
}
cuda::GpuMatND داخلہ_Array_2::getGpuMatND() ثابت
{
#اگر_تعریف_شدہ HAVE_CUDA
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND* d_mat = (ثابت cuda::GpuMatND*)obj;
        واپس *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ورنہ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
}
ogl::بفر_2 داخلہ_Array_2::get_OGl_بفر() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    CV_Assert(k == OPENGL_بفر);

    ثابت ogl::بفر_2* gl_buf = (ثابت ogl::بفر_2*)obj;
    واپس *gl_buf;
}

صحیح داخلہ_Array_2::صفیں_2(صحیح i) ثابت
{
#اگر_تعریف_شدہ HAVE_CUDA
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        واپس (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#اگر_ختم

    واپس سائز_3(i).اونچائی;
}

صحیح داخلہ_Array_2::ستون_تعداد_2(صحیح i) ثابت
{
#اگر_تعریف_شدہ HAVE_CUDA
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        واپس (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#اگر_ختم

    واپس سائز_3(i).چوڑائی;
}

سائز داخلہ_Array_2::سائز_3(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat* m = (ثابت Mat*)obj;
        CV_Assert(m->dims <= 2);
        واپس سائز(m->ستون_تعداد, m->صفیں);
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat* m = (ثابت UMat*)obj;
        CV_Assert(m->dims <= 2);
        واپس سائز(m->ستون_تعداد, m->صفیں);
    }

    اگر (k == MATX)
    {
        CV_Assert( i < 0 );
        واپس sz;
    }

    اگر( k == NONE )
        واپس سائز();

    اگر( k == STD_سمتیہ || k == STD_سمتیہ_سمتیہ || k == STD_BOOL_سمتیہ )
    {
        size_t n = کل(i);
        CV_Assert(n <= (size_t)زیادہ_سے_زیادہ_عدد);
        واپس سائز((صحیح)n, 1);
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( i < 0 )
            واپس vv.empty() ? سائز() : سائز((صحیح)vv.size(), 1);
        CV_Assert( i < (صحیح)vv.size() );

        واپس vv[i].size();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( i < 0 )
            واپس sz.اونچائی==0 ? سائز() : سائز(sz.اونچائی, 1);
        CV_Assert( i < sz.اونچائی );

        واپس vv[i].size();
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        اگر (i < 0)
            واپس vv.empty() ? سائز() : سائز((صحیح)vv.size(), 1);
        CV_Assert(i < (صحیح)vv.size());
        واپس vv[i].size();
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        اگر (i < 0)
            واپس vv.empty() ? سائز() : سائز((صحیح)vv.size(), 1);
        CV_Assert(i < (صحیح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        اگر (m.dims == 0)
            واپس سائز();
        اگر (m.dims == 1)
            واپس سائز(1, m.size[0]);
        واپس سائز(m.size[1], m.size[0]);
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( i < 0 )
            واپس vv.empty() ? سائز() : سائز((صحیح)vv.size(), 1);
        CV_Assert( i < (صحیح)vv.size() );

        واپس vv[i].size();
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        اگر (i < 0)
            واپس vv.empty() ? سائز() : سائز((صحیح)vv.size(), 1);
        CV_Assert(i < (صحیح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        اگر (m.dims == 0)
            واپس سائز();
        اگر (m.dims == 1)
            واپس سائز(1, m.size[0]);
        واپس سائز(m.size[1], m.size[0]);
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر( k == OPENGL_بفر )
    {
        CV_Assert( i < 0 );
        ثابت ogl::بفر_2* buf = (ثابت ogl::بفر_2*)obj;
        واپس buf->size();
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        واپس d_mat->size();
    }

    اگر( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        واپس cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحیح داخلہ_Array_2::sizend(صحیح* arrsz, صحیح i) ثابت
{
    صحیح j, d = 0;
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == NONE )
        ;
    ورنہ اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        d = m.dims;
        اگر(arrsz)
            برائے(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ورنہ اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        d = m.dims;
        اگر(arrsz)
            برائے(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ورنہ اگر( k == STD_سمتیہ_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i < (صحیح)vv.size() );
        ثابت Mat& m = vv[i];
        d = m.dims;
        اگر(arrsz)
            برائے(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ورنہ اگر( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.اونچائی );
        ثابت Mat& m = vv[i];
        d = m.dims;
        اگر(arrsz)
            برائے(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ورنہ اگر( k == STD_سمتیہ_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( i < (صحیح)vv.size() );
        ثابت UMat& m = vv[i];
        d = m.dims;
        اگر(arrsz)
            برائے(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ورنہ اگر (((k == STD_سمتیہ || k == STD_BOOL_سمتیہ || k == STD_سمتیہ_سمتیہ) && i < 0) ||
             (k == STD_سمتیہ_سمتیہ && i >= 0))
    {
        size_t n = کل(i);
        d = 1;
        اگر(arrsz)
        {
            arrsz[0] = صحیح(n);
        }
    }
    ورنہ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        سائز sz2d = size(i);
        d = 2;
        اگر(arrsz)
        {
            arrsz[0] = sz2d.اونچائی;
            arrsz[1] = sz2d.چوڑائی;
        }
    }

    واپس d;
}

منطقی داخلہ_Array_2::empty(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (i >= 0) {
        اگر (k == STD_سمتیہ_MAT) {
            خودکار mv = reinterpret_cast<ثابت std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            واپس mv->at(i).empty();
        }
        ورنہ اگر (k == STD_سمتیہ_UMAT) {
            خودکار umv = reinterpret_cast<ثابت std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            واپس umv->at(i).empty();
        }
        ورنہ اگر (k == STD_سمتیہ || k == STD_BOOL_سمتیہ || k == STD_سمتیہ_سمتیہ) {
            size_t n = کل(i);
            واپس n == 0;
        } ورنہ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    واپس empty();
}

MatShape داخلہ_Array_2::shape(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    MatShape shape;

    اگر( k == NONE )
        ;
    ورنہ اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        shape = m.size;
    }
    ورنہ اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        shape = m.size;
    }
    ورنہ اگر( k == STD_سمتیہ_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    ورنہ اگر( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.اونچائی );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    ورنہ اگر( k == STD_سمتیہ_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت UMat& m = vv[i];
        shape = m.size;
    }
    ورنہ
    {
        صحیح sizes[CV_MAX_DIM];
        صحیح dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    واپس shape;
}

منطقی داخلہ_Array_2::same_سائز(ثابت داخلہ_Array_2& arr) ثابت
{
    داخلہ_Array_2::قسم_Flag k1 = قسم(), k2 = arr.قسم();
    سائز sz1;

    اگر( k1 == MAT )
    {
        ثابت Mat* m = ((ثابت Mat*)obj);
        اگر( k2 == MAT )
            واپس m->size == ((ثابت Mat*)arr.obj)->size;
        اگر( k2 == UMAT )
            واپس m->size == ((ثابت UMat*)arr.obj)->size;
        اگر( m->dims > 2 )
            واپس جھوٹ;
        sz1 = m->size();
    }
    ورنہ اگر( k1 == UMAT )
    {
        ثابت UMat* m = ((ثابت UMat*)obj);
        اگر( k2 == MAT )
            واپس m->size == ((ثابت Mat*)arr.obj)->size;
        اگر( k2 == UMAT )
            واپس m->size == ((ثابت UMat*)arr.obj)->size;
        اگر( m->dims > 2 )
            واپس جھوٹ;
        sz1 = m->size();
    }
    ورنہ اگر ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        واپس ((ثابت cuda::GpuMatND*)obj)->size == ((ثابت cuda::GpuMatND*)arr.obj)->size;
    }
    ورنہ
        sz1 = size();

    اگر( arr.dims() > 2 )
        واپس جھوٹ;
    واپس sz1 == arr.size();
}

صحیح داخلہ_Array_2::dims_(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت Mat*)obj)->dims;
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت UMat*)obj)->dims;
    }

    اگر (k == MATX)
    {
        CV_Assert( i < 0 );
        واپس 2;
    }

    اگر( k == STD_سمتیہ || k == STD_BOOL_سمتیہ )
    {
        CV_Assert( i < 0 );
        واپس 1;
    }

    اگر( k == NONE )
        واپس 0;

    اگر( k == STD_سمتیہ_سمتیہ )
    {
        اگر( i < 0 )
            واپس 1;
        CV_Assert( size_t(i) < کل() );
        واپس 1;
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( i < 0 )
            واپس 1;
        CV_Assert( i < (صحیح)vv.size() );

        واپس vv[i].dims;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( i < 0 )
            واپس 1;
        CV_Assert( i < sz.اونچائی );

        واپس vv[i].dims;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( i < 0 )
            واپس 1;
        CV_Assert( i < (صحیح)vv.size() );

        واپس vv[i].dims;
    }

    اگر( k == OPENGL_بفر )
    {
        CV_Assert( i < 0 );
        واپس 2;
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        واپس 2;
    }

    اگر( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت cuda::GpuMatND*)obj)->dims;
    }

    اگر( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        واپس 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t داخلہ_Array_2::کل(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت Mat*)obj)->کل();
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت UMat*)obj)->کل();
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( i < 0 )
            واپس vv.size();

        CV_Assert( i < (صحیح)vv.size() );
        واپس vv[i].کل();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( i < 0 )
            واپس sz.اونچائی;

        CV_Assert( i < sz.اونچائی );
        واپس vv[i].کل();
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( i < 0 )
            واپس vv.size();

        CV_Assert( i < (صحیح)vv.size() );
        واپس vv[i].کل();
    }

    اگر( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ثابت cuda::GpuMatND& _gpuMatND = *((ثابت cuda::GpuMatND*)obj);
        اگر (_gpuMatND.dims > 0)
        {
            res = 1;
            برائے(صحیح d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            واپس res;
        }
    }

    اگر (k == STD_سمتیہ || k == STD_سمتیہ_سمتیہ)
    {
        CV_Assert(i < 0 || k == STD_سمتیہ_سمتیہ);
        صحیح esz = CV_ELEM_سائز(flags);

        #تعریف_منسوخ GET_VEC_سائز
        #تعریف_کرو GET_VEC_سائز(T) \
            اگر (k == STD_سمتیہ_سمتیہ) { \
                ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                اگر (i < 0) \
                    واپس n; \
                CV_Assert((size_t)i < n); \
                واپس vv->at(i).size(); \
            } ورنہ \
                واپس ((ثابت std::vector<T >*)obj)->size()

        STD_سمتیہ_SWITCH(esz, GET_VEC_سائز)
    }

    واپس size(i).area();
}

صحیح داخلہ_Array_2::type_(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
        واپس ((ثابت Mat*)obj)->type();

    اگر( k == UMAT )
        واپس ((ثابت UMat*)obj)->type();

    اگر( k == MATX || k == STD_سمتیہ || k == STD_سمتیہ_سمتیہ || k == STD_BOOL_سمتیہ )
        واپس CV_MAT_TYPE(flags);

    اگر( k == NONE )
        واپس -1;

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            واپس CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحیح)vv.size() );
        واپس vv[i >= 0 ? i : 0].type();
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            واپس CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحیح)vv.size() );
        واپس vv[i >= 0 ? i : 0].type();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( sz.اونچائی == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            واپس CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.اونچائی );
        واپس vv[i >= 0 ? i : 0].type();
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        اگر (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            واپس CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحیح)vv.size());
        واپس vv[i >= 0 ? i : 0].type();
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        اگر (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            واپس CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحیح)vv.size());
        واپس vv[i >= 0 ? i : 0].type();
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر( k == OPENGL_بفر )
        واپس ((ثابت ogl::بفر_2*)obj)->type();

    اگر( k == CUDA_GPU_MAT )
        واپس ((ثابت cuda::GpuMat*)obj)->type();

    اگر( k == CUDA_GPU_MATND )
        واپس ((ثابت cuda::GpuMatND*)obj)->type();

    اگر( k == CUDA_HOST_MEM )
        واپس ((ثابت cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحیح داخلہ_Array_2::گہرائی_4(صحیح i) ثابت
{
    واپس CV_MAT_گہرائی(type(i));
}

منطقی داخلہ_Array_2::خالی_خانے() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
        واپس ((ثابت Mat*)obj)->empty();

    اگر( k == UMAT )
        واپس ((ثابت UMat*)obj)->empty();

    اگر (k == MATX)
        واپس جھوٹ;

    اگر( k == STD_سمتیہ || k == STD_BOOL_سمتیہ || k == STD_سمتیہ_سمتیہ )
    {
        size_t n = کل(-1);
        واپس n == 0;
    }

    اگر( k == NONE )
        واپس سچ;

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        واپس vv.empty();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        واپس sz.اونچائی == 0;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        واپس vv.empty();
    }

    اگر( k == OPENGL_بفر )
        واپس ((ثابت ogl::بفر_2*)obj)->empty();

    اگر( k == CUDA_GPU_MAT )
        واپس ((ثابت cuda::GpuMat*)obj)->empty();

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        واپس vv.empty();
    }

    اگر( k == CUDA_GPU_MATND )
        واپس ((ثابت cuda::GpuMatND*)obj)->empty();

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        واپس vv.empty();
    }

    اگر( k == CUDA_HOST_MEM )
        واپس ((ثابت cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقی داخلہ_Array_2::isContinuous(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
        واپس i < 0 ? ((ثابت Mat*)obj)->isContinuous() : سچ;

    اگر( k == UMAT )
        واپس i < 0 ? ((ثابت UMat*)obj)->isContinuous() : سچ;

    اگر( k == MATX || k == STD_سمتیہ ||
        k == NONE || k == STD_سمتیہ_سمتیہ || k == STD_BOOL_سمتیہ )
        واپس سچ;

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].isContinuous();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.اونچائی);
        واپس vv[i].isContinuous();
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].isContinuous();
    }

    اگر( k == CUDA_GPU_MAT )
      واپس i < 0 ? ((ثابت cuda::GpuMat*)obj)->isContinuous() : سچ;

    اگر( k == CUDA_GPU_MATND )
        واپس i < 0 ? ((ثابت cuda::GpuMatND*)obj)->isContinuous() : سچ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقی داخلہ_Array_2::isSubmatrix(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
        واپس i < 0 ? ((ثابت Mat*)obj)->isSubmatrix() : جھوٹ;

    اگر( k == UMAT )
        واپس i < 0 ? ((ثابت UMat*)obj)->isSubmatrix() : جھوٹ;

    اگر( k == MATX || k == STD_سمتیہ ||
        k == NONE || k == STD_سمتیہ_سمتیہ || k == STD_BOOL_سمتیہ )
        واپس جھوٹ;

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].isSubmatrix();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.اونچائی);
        واپس vv[i].isSubmatrix();
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].isSubmatrix();
    }

    اگر( k == CUDA_GPU_MATND )
    {
        واپس ((ثابت cuda::GpuMatND*)obj)->isSubmatrix();
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t داخلہ_Array_2::offset(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat * ثابت m = ((ثابت Mat*)obj);
        واپس (size_t)(m->ptr() - m->datastart);
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت UMat*)obj)->offset;
    }

    اگر( k == MATX || k == STD_سمتیہ ||
        k == NONE || k == STD_سمتیہ_سمتیہ || k == STD_BOOL_سمتیہ )
        واپس 0;

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحیح)vv.size() );

        واپس (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.اونچائی );
        واپس (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].offset;
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat * ثابت m = ((ثابت cuda::GpuMat*)obj);
        واپس (size_t)(m->ڈیٹا - m->datastart);
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس (size_t)(vv[i].ڈیٹا - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t داخلہ_Array_2::قدم(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت Mat*)obj)->قدم;
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت UMat*)obj)->قدم;
    }

    اگر( k == MATX || k == STD_سمتیہ ||
        k == NONE || k == STD_سمتیہ_سمتیہ || k == STD_BOOL_سمتیہ )
        واپس 0;

    اگر( k == STD_سمتیہ_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحیح)vv.size() );
        واپس vv[i].قدم;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.اونچائی );
        واپس vv[i].قدم;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].قدم;
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        واپس ((ثابت cuda::GpuMat*)obj)->قدم;
    }
    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        واپس vv[i].قدم;
    }
    اگر( k == CUDA_GPU_MATND )
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        واپس _gpuMatND.قدم[i];
    }

    اگر (k == STD_سمتیہ_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ثابت cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        واپس _gpuMatND.قدم[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

خالی داخلہ_Array_2::copy_مقصد(ثابت _OutputArray& arr) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == NONE )
        arr.release();
    ورنہ اگر( k == MAT || k == MATX || k == STD_سمتیہ || k == STD_BOOL_سمتیہ )
    {
        Mat m = getMat();
        m.copy_مقصد(arr);
    }
    ورنہ اگر( k == UMAT )
        ((UMat*)obj)->copy_مقصد(arr);
#اگر_تعریف_شدہ HAVE_CUDA
    ورنہ اگر (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_مقصد(arr);
#اگر_ختم
    ورنہ
        CV_Error(Error::StsNotImplemented, "");
}

خالی داخلہ_Array_2::copy_مقصد(ثابت _OutputArray& arr, ثابت داخلہ_Array_2 & نقاب) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == NONE )
        arr.release();
    ورنہ اگر( k == MAT || k == MATX || k == STD_سمتیہ || k == STD_BOOL_سمتیہ )
    {
        Mat m = getMat();
        m.copy_مقصد(arr, نقاب);
    }
    ورنہ اگر( k == UMAT )
        ((UMat*)obj)->copy_مقصد(arr, نقاب);
#اگر_تعریف_شدہ HAVE_CUDA
    ورنہ اگر (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_مقصد(arr, نقاب);
#اگر_ختم
    ورنہ
        CV_Error(Error::StsNotImplemented, "");
}

منطقی _OutputArray::fixed_سائز() ثابت
{
    واپس (flags & FIXED_سائز) == FIXED_سائز;
}

منطقی _OutputArray::fixedType() ثابت
{
    واپس (flags & FIXED_TYPE) == FIXED_TYPE;
}

خالی _OutputArray::create_(سائز _sz, صحیح mtype, صحیح i, منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر( k == MAT && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((Mat*)obj)->size.عامل()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        واپس;
    }
    اگر( k == UMAT && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((UMat*)obj)->size.عامل()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        واپس;
    }
    اگر( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    اگر( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.اونچائی) && (((cuda::GpuMatND*)obj)->size[1] == _sz.چوڑائی)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_CUDA
        cuda::GpuMatND::سائز_Array sizes({_sz.اونچائی, _sz.چوڑائی});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    اگر( k == OPENGL_بفر && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((ogl::بفر_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::بفر_2*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_OPENGL
        ((ogl::بفر_2*)obj)->create(_sz, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#اگر_ختم
    }
    اگر( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    صحیح sizes[] = {_sz.اونچائی, _sz.چوڑائی};
    create(2, sizes, mtype, i, allowTransposed, fixed_گہرائی_نقاب);
}

خالی _OutputArray::create_(صحیح صفیں_3, صحیح ستون_تعداد_3, صحیح mtype, صحیح i, منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر( k == MAT && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((Mat*)obj)->size.عامل()() == سائز(ستون_تعداد_3, صفیں_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(صفیں_3, ستون_تعداد_3, mtype);
        واپس;
    }
    اگر( k == UMAT && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((UMat*)obj)->size.عامل()() == سائز(ستون_تعداد_3, صفیں_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(صفیں_3, ستون_تعداد_3, mtype);
        واپس;
    }
    اگر( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((cuda::GpuMat*)obj)->size() == سائز(ستون_تعداد_3, صفیں_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(صفیں_3, ستون_تعداد_3, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    اگر( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == صفیں_3) && (((cuda::GpuMatND*)obj)->size[1] == ستون_تعداد_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_CUDA
        cuda::GpuMatND::سائز_Array sizes({صفیں_3, ستون_تعداد_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    اگر( k == OPENGL_بفر && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((ogl::بفر_2*)obj)->size() == سائز(ستون_تعداد_3, صفیں_3));
        CV_Assert(!fixedType() || ((ogl::بفر_2*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_OPENGL
        ((ogl::بفر_2*)obj)->create(صفیں_3, ستون_تعداد_3, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#اگر_ختم
    }
    اگر( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
        CV_Assert(!fixed_سائز() || ((cuda::HostMem*)obj)->size() == سائز(ستون_تعداد_3, صفیں_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::HostMem*)obj)->create(صفیں_3, ستون_تعداد_3, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    صحیح sizes[] = {صفیں_3, ستون_تعداد_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_گہرائی_نقاب);
}

خالی _OutputArray::create(صحیح d, ثابت صحیح* sizes, صحیح mtype, صحیح i,
                          منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    صحیح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    داخلہ_Array_2::قسم_Flag k = قسم();
    mtype = CV_MAT_TYPE(mtype);

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_سائز()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.صفیں == size0 && m.ستون_تعداد == size1) ||
            (allowTransposed && m.صفیں == size1 && m.ستون_تعداد == size0 && m.isContinuous())))
        {
            واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_گہرائی(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m.type();
            ورنہ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixed_سائز())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            برائے(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        واپس;
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_سائز()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.صفیں == size0 && m.ستون_تعداد == size1) ||
            (allowTransposed && m.صفیں == size1 && m.ستون_تعداد == size0 && m.isContinuous())))
        {
            واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m.type();
            ورنہ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixed_سائز())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            برائے(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        واپس;
    }

    اگر( k == MATX )
    {
        CV_Assert( i < 0 );
        صحیح type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_گہرائی_نقاب) != 0) );
        CV_CheckLE(d, 2, "");
        سائز requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        اگر (sz.چوڑائی == 1 || sz.اونچائی == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            صحیح کل_1d = std::max(sz.چوڑائی, sz.اونچائی);
            CV_Check(requested_size, std::max(requested_size.چوڑائی, requested_size.اونچائی) == کل_1d, "");
        }
        ورنہ
        {
            اگر (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ورنہ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.اونچائی == sz.چوڑائی && requested_size.چوڑائی == sz.اونچائی)),
                        "");
            }
        }
        واپس;
    }

    اگر( k == STD_سمتیہ || k == STD_سمتیہ_سمتیہ )
    {
        CV_Assert(!fixed_سائز());
        CV_Assert(k == STD_سمتیہ_سمتیہ || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        صحیح esz = CV_ELEM_سائز(flags);

        اگر( k == STD_سمتیہ || (k == STD_سمتیہ_سمتیہ && i >= 0) )
        {
            صحیح type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_گہرائی_نقاب) != 0) );
        }

    #تعریف_منسوخ RESIZE_VEC
    #تعریف_کرو RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            اگر (k == STD_سمتیہ_سمتیہ) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                اگر (i < 0) { \
                    CV_Assert(!fixed_سائز() || len == vv->size()); \
                    vv->resize(len); \
                    واپس; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ورنہ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_سائز() || len == v->size()); \
            v->resize(len); \
        } \
        توڑو

        STD_سمتیہ_SWITCH(esz, RESIZE_VEC)
        واپس;
    }

    اگر( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        اگر( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_سائز() || len == len0);
            v.resize(len);
            اگر( fixedType() )
            {
                صحیح _type = CV_MAT_TYPE(flags);
                برائے( size_t j = len0; j < len; j++ )
                {
                    اگر( v[j].type() == _type )
                        جاری;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_نقاب) | _type;
                }
            }
            واپس;
        }

        CV_Assert( i < (صحیح)v.size() );
        Mat& m = v[i];

        اگر( allowTransposed )
        {
            اگر( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_سائز());
                m.release();
            }

            اگر( d == 2 && m.dims == 2 && m.ڈیٹا &&
                m.type() == mtype && m.صفیں == sizes[1] && m.ستون_تعداد == sizes[0] )
                واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m.type();
            ورنہ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        اگر(fixed_سائز())
        {
            CV_Assert(m.dims == d);
            برائے(صحیح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        واپس;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        اگر( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.اونچائی;

            CV_Assert(len == len0);
            اگر( fixedType() )
            {
                صحیح _type = CV_MAT_TYPE(flags);
                برائے( size_t j = len0; j < len; j++ )
                {
                    اگر( v[j].type() == _type )
                        جاری;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_نقاب) | _type;
                }
            }
            واپس;
        }

        CV_Assert( i < sz.اونچائی );
        Mat& m = v[i];

        اگر( allowTransposed )
        {
            اگر( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_سائز());
                m.release();
            }

            اگر( d == 2 && m.dims == 2 && m.ڈیٹا &&
                m.type() == mtype && m.صفیں == sizes[1] && m.ستون_تعداد == sizes[0] )
                واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m.type();
            ورنہ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        اگر(fixed_سائز())
        {
            CV_Assert(m.dims == d);
            برائے(صحیح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        واپس;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        اگر( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_سائز() || len == len0);
            v.resize(len);
            اگر( fixedType() )
            {
                صحیح _type = CV_MAT_TYPE(flags);
                برائے( size_t j = len0; j < len; j++ )
                {
                    اگر( v[j].type() == _type )
                        جاری;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_نقاب) | _type;
                }
            }
            واپس;
        }

        CV_Assert( i < (صحیح)v.size() );
        UMat& m = v[i];

        اگر( allowTransposed )
        {
            اگر( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_سائز());
                m.release();
            }

            اگر( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.صفیں == sizes[1] && m.ستون_تعداد == sizes[0] )
                واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m.type();
            ورنہ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        اگر(fixed_سائز())
        {
            CV_Assert(m.dims == d);
            برائے(صحیح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        واپس;
    }

    اگر ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_گہرائی_نقاب);
        واپس;
    }

    اگر( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_گہرائی_نقاب == 0 )
    {
#اگر_تعریف_شدہ HAVE_CUDA
        cuda::GpuMatND::سائز_Array سائز_Array_2 = cuda::GpuMatND::سائز_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(سائز_Array_2, mtype);
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

خالی _OutputArray::create(ثابت MatShape& shape, صحیح mtype, صحیح i,
                          منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    اگر (shape.dims < 0) {
        release();
    } ورنہ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_گہرائی_نقاب);
    }
}

Mat _OutputArray::reinterpret(صحیح mtype) ثابت
{
    mtype = CV_MAT_TYPE(mtype);
    واپس getMat().reinterpret(mtype);
}

خالی _OutputArray::create_Same_سائز(ثابت داخلہ_Array_2& arr, صحیح mtype) ثابت
{
    create(arr.shape(), mtype);
}

خالی _OutputArray::fit(سائز _sz, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    صحیح sizes[] = {_sz.اونچائی, _sz.چوڑائی};
    fit(2, sizes, mtype, i, allowTransposed, fixed_گہرائی_نقاب);
}

خالی _OutputArray::fit(صحیح صفیں, صحیح ستون_تعداد, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    صحیح sizes[] = {صفیں, ستون_تعداد};
    fit(2, sizes, mtype, i, allowTransposed, fixed_گہرائی_نقاب);
}

خالی _OutputArray::fit(ثابت MatShape& shape, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    اگر (shape.dims < 0) {
        release();
        واپس;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_گہرائی_نقاب);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر ((k == MAT && i < 0) || (k == STD_سمتیہ_MAT && i >= 0)) {
        Mat_سائز& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ورنہ اگر ((k == UMAT && i < 0) || (k == STD_سمتیہ_UMAT && i >= 0)) {
        Mat_سائز& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

خالی _OutputArray::fit_Same_سائز(ثابت داخلہ_Array_2& arr, صحیح mtype) ثابت
{
    fit(arr.shape(), mtype);
}

خالی _OutputArray::fit(صحیح d, ثابت صحیح* sizes, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::گہرائی_نقاب fixed_گہرائی_نقاب) ثابت
{
    صحیح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    داخلہ_Array_2::قسم_Flag k = قسم();
    mtype = CV_MAT_TYPE(mtype);

    اگر( (k == MAT && i < 0) || (k == STD_سمتیہ_MAT && i >= 0) )
    {
        Mat* m;
        اگر (k == MAT)
            m = (Mat*)obj;
        ورنہ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_سائز()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->صفیں == size0 && m->ستون_تعداد == size1) ||
             (allowTransposed && m->صفیں == size1 && m->ستون_تعداد == size0 && m->isContinuous())))
        {
            واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_گہرائی(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m->type();
            ورنہ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixed_سائز())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            برائے(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        واپس;
    }

    اگر( (k == UMAT && i < 0) || (k == STD_سمتیہ_UMAT && i >= 0) )
    {
        UMat* m;
        اگر (k == UMAT)
            m = (UMat*)obj;
        ورنہ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_سائز()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->صفیں == size0 && m->ستون_تعداد == size1) ||
             (allowTransposed && m->صفیں == size1 && m->ستون_تعداد == size0 && m->isContinuous())))
        {
            واپس;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_گہرائی(flags)) & fixed_گہرائی_نقاب) != 0 )
                mtype = m->type();
            ورنہ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixed_سائز())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            برائے(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        واپس;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_گہرائی_نقاب);
}

خالی _OutputArray::release() ثابت
{
    CV_Assert(!fixed_سائز());

    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        ((Mat*)obj)->release();
        واپس;
    }

    اگر( k == UMAT )
    {
        ((UMat*)obj)->release();
        واپس;
    }

    اگر( k == CUDA_GPU_MAT )
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر( k == CUDA_GPU_MATND )
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر( k == CUDA_HOST_MEM )
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }

    اگر( k == OPENGL_بفر )
    {
#اگر_تعریف_شدہ HAVE_OPENGL
        ((ogl::بفر_2*)obj)->release();
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#اگر_ختم
    }

    اگر( k == NONE )
        واپس;

    اگر( k == STD_سمتیہ || k == STD_سمتیہ_سمتیہ )
    {
    #تعریف_منسوخ CLEAR_VEC
    #تعریف_کرو CLEAR_VEC(T) \
        اگر (k == STD_سمتیہ_سمتیہ) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ورنہ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        توڑو

        صحیح esz = CV_ELEM_سائز(flags);
        STD_سمتیہ_SWITCH(esz, CLEAR_VEC);
        واپس;
    }

    اگر( k == STD_سمتیہ_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        واپس;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        برائے( صحیح i = 0; i < sz.اونچائی; i++ )
            v[i].release();
        واپس;
    }

    اگر( k == STD_BOOL_سمتیہ )
    {
        ((std::vector<منطقی>*)obj)->clear();
        واپس;
    }

    اگر( k == STD_سمتیہ_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        واپس;
    }
    اگر (k == STD_سمتیہ_CUDA_GPU_MAT)
    {
#اگر_تعریف_شدہ HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        واپس;
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

خالی _OutputArray::clear() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == MAT )
    {
        CV_Assert(!fixed_سائز());
        ((Mat*)obj)->resize(0);
        واپس;
    }

    release();
}

منطقی _OutputArray::needed() ثابت
{
    واپس قسم() != NONE;
}

Mat& _OutputArray::getMatRef(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر( i < 0 )
    {
        CV_Assert( k == MAT );
        واپس *(Mat*)obj;
    }

    CV_Assert( k == STD_سمتیہ_MAT || k == STD_ARRAY_MAT );

    اگر( k == STD_سمتیہ_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (صحیح)v.size() );
        واپس v[i];
    }
    ورنہ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.اونچائی );
        واپس v[i];
    }
}

UMat& _OutputArray::getUMatRef(صحیح i) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر( i < 0 )
    {
        CV_Assert( k == UMAT );
        واپس *(UMat*)obj;
    }
    ورنہ
    {
        CV_Assert( k == STD_سمتیہ_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (صحیح)v.size() );
        واپس v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == CUDA_GPU_MAT );
    واپس *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    CV_Assert(k == STD_سمتیہ_CUDA_GPU_MAT);
    واپس *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == CUDA_GPU_MATND );
    واپس *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    CV_Assert(k == STD_سمتیہ_CUDA_GPU_MAT_ND);
    واپس *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::بفر_2& _OutputArray::get_OGl_بفر_Ref() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == OPENGL_بفر );
    واپس *(ogl::بفر_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == CUDA_HOST_MEM );
    واپس *(cuda::HostMem*)obj;
}

خالی _OutputArray::سیٹ_مقصد(ثابت داخلہ_Array_2& arr, ثابت داخلہ_Array_2 & نقاب) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == NONE )
        ;
    ورنہ اگر (k == MAT || k == MATX || k == STD_سمتیہ)
    {
        Mat m = getMat();
        m.سیٹ_مقصد(arr, نقاب);
    }
    ورنہ اگر( k == UMAT )
        ((UMat*)obj)->سیٹ_مقصد(arr, نقاب);
    ورنہ اگر( k == CUDA_GPU_MAT )
    {
#اگر_تعریف_شدہ HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.قسم(), داخلہ_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->سیٹ_مقصد(Scalar(Vec<دوہرا, 4>(value.ptr<دوہرا>())), نقاب);
#ورنہ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#اگر_ختم
    }
    ورنہ
        CV_Error(Error::StsNotImplemented, "");
}

خالی _OutputArray::سیٹ_Zero() ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();

    اگر( k == NONE )
        ;
    ورنہ اگر (k == MAT || k == MATX || k == STD_سمتیہ)
    {
        Mat m = getMat();
        m.سیٹ_Zero();
    }
    ورنہ
    {
        سیٹ_مقصد(Scalar::all(0), noArray());
    }
}

خالی _OutputArray::assign(ثابت UMat& u) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ورنہ اگر (k == MAT)
    {
        u.copy_مقصد(*(Mat*)obj); // TODO check u.getMat()
    }
    ورنہ اگر (k == MATX)
    {
        u.copy_مقصد(getMat()); // TODO check u.getMat()
    }
    ورنہ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


خالی _OutputArray::assign(ثابت Mat& m) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == UMAT)
    {
        m.copy_مقصد(*(UMat*)obj); // TODO check m.getUMat()
    }
    ورنہ اگر (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ورنہ اگر (k == MATX)
    {
        m.copy_مقصد(getMat());
    }
    ورنہ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


خالی _OutputArray::move(UMat& u) ثابت
{
    اگر (fixed_سائز())
    {
        // TODO Performance warning
        assign(u);
        واپس;
    }
    صحیح k = قسم();
    اگر (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ورنہ اگر (k == MAT)
    {
        u.copy_مقصد(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ورنہ اگر (k == MATX)
    {
        u.copy_مقصد(getMat()); // TODO check u.getMat()
        u.release();
    }
    ورنہ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


خالی _OutputArray::move(Mat& m) ثابت
{
    اگر (fixed_سائز())
    {
        // TODO Performance warning
        assign(m);
        واپس;
    }
    صحیح k = قسم();
    اگر (k == UMAT)
    {
        m.copy_مقصد(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ورنہ اگر (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ورنہ اگر (k == MATX)
    {
        m.copy_مقصد(getMat());
        m.release();
    }
    ورنہ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


خالی _OutputArray::assign(ثابت std::vector<UMat>& v) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == STD_سمتیہ_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برائے (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            UMat& this_m = this_v[i];
            اگر (this_m.u != تہی && this_m.u == m.u)
                جاری; // same object (see dnn::Layer::forward_fallback)
            m.copy_مقصد(this_m);
        }
    }
    ورنہ اگر (k == STD_سمتیہ_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برائے (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            Mat& this_m = this_v[i];
            اگر (this_m.u != تہی && this_m.u == m.u)
                جاری; // same object (see dnn::Layer::forward_fallback)
            m.copy_مقصد(this_m);
        }
    }
    ورنہ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


خالی _OutputArray::assign(ثابت std::vector<Mat>& v) ثابت
{
    داخلہ_Array_2::قسم_Flag k = قسم();
    اگر (k == STD_سمتیہ_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برائے (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            UMat& this_m = this_v[i];
            اگر (this_m.u != تہی && this_m.u == m.u)
                جاری; // same object (see dnn::Layer::forward_fallback)
            m.copy_مقصد(this_m);
        }
    }
    ورنہ اگر (k == STD_سمتیہ_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برائے (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            Mat& this_m = this_v[i];
            اگر (this_m.u != تہی && this_m.u == m.u)
                جاری; // same object (see dnn::Layer::forward_fallback)
            m.copy_مقصد(this_m);
        }
    }
    ورنہ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ساکن داخلہ_Output_Array_2 _none;
داخلہ_Output_Array noArray() { واپس _none; }

} // cv::
