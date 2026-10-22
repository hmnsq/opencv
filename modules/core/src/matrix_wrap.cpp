// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/mat.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 5> Vec5i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 7> Vec7i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 9> Vec9i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 10> Vec10i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 11> Vec11i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 12> Vec12i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 13> Vec13i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 14> Vec14i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 15> Vec15i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 16> Vec16i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 32> Vec32i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 64> Vec64i;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Vec<ਪੂਰਨ, 128> Vec128i;

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ STD_ਸਦਿਸ਼_SWITCH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STD_ਸਦਿਸ਼_SWITCH(esz, func) \
    ਚੋਣ( esz ) \
    { \
    ਹਾਲਤ 1: func(uchar); \
    ਹਾਲਤ 2: func(Vec2b); \
    ਹਾਲਤ 3: func(Vec3b); \
    ਹਾਲਤ 4: func(ਪੂਰਨ); \
    ਹਾਲਤ 6: func(Vec3s); \
    ਹਾਲਤ 8: func(Vec2i); \
    ਹਾਲਤ 12: func(Vec3i); \
    ਹਾਲਤ 16: func(Vec4i); \
    ਹਾਲਤ 20: func(Vec5i); \
    ਹਾਲਤ 24: func(Vec6i); \
    ਹਾਲਤ 28: func(Vec7i); \
    ਹਾਲਤ 32: func(Vec8i); \
    ਹਾਲਤ 36: func(Vec9i); \
    ਹਾਲਤ 40: func(Vec10i); \
    ਹਾਲਤ 44: func(Vec11i); \
    ਹਾਲਤ 48: func(Vec12i); \
    ਹਾਲਤ 52: func(Vec13i); \
    ਹਾਲਤ 56: func(Vec14i); \
    ਹਾਲਤ 60: func(Vec15i); \
    ਹਾਲਤ 64: func(Vec16i); \
    ਹਾਲਤ 128: func(Vec32i); \
    ਹਾਲਤ 256: func(Vec64i); \
    ਹਾਲਤ 512: func(Vec128i); \
    ਮੂਲ: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat ਨਿਵੇਸ਼_Array_2::getMat_(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    AccessFlag accessFlags = flags & ACCESS_ਨਕਾਬ;

    ਜੇ( k == MAT )
    {
        ਅਚਲ Mat* m = (ਅਚਲ Mat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ *m;
        ਵਾਪਸ m->ਪੰਕਤੀ(i);
    }

    ਜੇ( k == UMAT )
    {
        ਅਚਲ UMat* m = (ਅਚਲ UMat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ m->getMat(accessFlags);
        ਵਾਪਸ m->getMat(accessFlags).ਪੰਕਤੀ(i);
    }

    ਜੇ (k == MATX)
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ Mat(sz, flags, obj);
    }

    ਜੇ( k == STD_BOOL_ਸਦਿਸ਼ )
    {
        CV_Assert( i < 0 );
        ਪੂਰਨ t = CV_8U;
        ਅਚਲ std::vector<ਤਾਰਕਿਕ>& v = *(ਅਚਲ std::vector<ਤਾਰਕਿਕ>*)obj;
        ਪੂਰਨ j, n = (ਪੂਰਨ)v.size();
        ਜੇ( n == 0 )
            ਵਾਪਸ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.ਅੰਕੜੇ;
        ਲਈ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ਵਾਪਸ m;
    }

    ਜੇ( k == NONE )
        ਵਾਪਸ Mat();

    ਜੇ( k == STD_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ )
    {
        ਪੂਰਨ t = CV_MAT_TYPE(flags);
        ਪੂਰਨ esz = CV_ELEM_ਮਾਪ(t);

    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_MAT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ਜੇ (k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ਨਹੀਂਤਾਂ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ਪੂਰਨ v_sz = ਪੂਰਨ(v->size()); \
            ਵਾਪਸ v->empty() ? Mat() : Mat(1, &v_sz, t, v->ਅੰਕੜੇ()); \
        }

        STD_ਸਦਿਸ਼_SWITCH(esz, GET_MAT)
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& v = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ਪੂਰਨ)v.size() );

        ਵਾਪਸ v[i];
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* v = (ਅਚਲ Mat*)obj;
        CV_Assert( 0 <= i && i < sz.ਉਚਾਈ );

        ਵਾਪਸ v[i];
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& v = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ਪੂਰਨ)v.size() );

        ਵਾਪਸ v[i].getMat(accessFlags);
    }

    ਜੇ( k == OPENGL_ਬਫਰ )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ਜੇ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ਜੇ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ਜੇ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ਅਚਲ cuda::HostMem* cuda_mem = (ਅਚਲ cuda::HostMem*)obj;

        ਵਾਪਸ cuda_mem->create_Mat_ਸਿਰਲੇਖ();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat ਨਿਵੇਸ਼_Array_2::getUMat(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    AccessFlag accessFlags = flags & ACCESS_ਨਕਾਬ;

    ਜੇ( k == UMAT )
    {
        ਅਚਲ UMat* m = (ਅਚਲ UMat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ *m;
        ਵਾਪਸ m->ਪੰਕਤੀ(i);
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& v = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ਪੂਰਨ)v.size() );

        ਵਾਪਸ v[i];
    }

    ਜੇ( k == MAT )
    {
        ਅਚਲ Mat* m = (ਅਚਲ Mat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ m->getUMat(accessFlags);
        ਵਾਪਸ m->ਪੰਕਤੀ(i).getUMat(accessFlags);
    }

    ਵਾਪਸ getMat(i).getUMat(accessFlags);
}

ਖਾਲੀ ਨਿਵੇਸ਼_Array_2::get_Mat_ਸਦਿਸ਼(std::vector<Mat>& mv) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    AccessFlag accessFlags = flags & ACCESS_ਨਕਾਬ;

    ਜੇ( k == MAT )
    {
        ਅਚਲ Mat& m = *(ਅਚਲ Mat*)obj;
        ਪੂਰਨ n = (ਪੂਰਨ)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        ਲਈ( ਪੂਰਨ i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.ਖਾਨੇ_ਗਿਣਤੀ, m.type(), (ਖਾਲੀ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (ਖਾਲੀ*)m.ptr(i), &m.ਕਦਮ[1]);
        ਵਾਪਸ;
    }

    ਜੇ (k == MATX)
    {
        size_t n = sz.ਉਚਾਈ, esz = CV_ELEM_ਮਾਪ(flags);
        mv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.ਚੌੜਾਈ, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.ਚੌੜਾਈ*i);
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼ )
    {
        ਅਚਲ std::vector<uchar>& v = *(ਅਚਲ std::vector<uchar>*)obj;

        size_t n = ਕੁੱਲ(), esz = CV_ELEM_ਮਾਪ(flags);
        ਪੂਰਨ t = CV_MAT_ਡੂੰਘਾਈ(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (ਖਾਲੀ*)(v.ਅੰਕੜੇ() + esz*i));
        ਵਾਪਸ;
    }

    ਜੇ( k == NONE )
    {
        mv.clear();
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ )
    {
        ਪੂਰਨ typ = CV_MAT_TYPE(flags);
        ਪੂਰਨ esz = CV_ELEM_ਮਾਪ(typ);

        #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_VEC_VEC_MAT_VEC
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_VEC_VEC_MAT_VEC(T) \
        { \
            ਅਚਲ std::vector<std::vector<T > >* vv = (ਅਚਲ std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            ਲਈ (size_t i = 0; i < n; i++) { \
                ਅਚਲ std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ); \
                ਪੂਰਨ ni = (ਪੂਰਨ)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (ਖਾਲੀ*)vi.ਅੰਕੜੇ()) : Mat(); \
            } \
        } \
        ਰੋਕੋ

        STD_ਸਦਿਸ਼_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& v = *(ਅਚਲ std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* v = (ਅਚਲ Mat*)obj;
        size_t n = sz.ਉਚਾਈ;
        mv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& v = *(ਅਚਲ std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ਵਾਪਸ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਖਾਲੀ ਨਿਵੇਸ਼_Array_2::get_UMat_ਸਦਿਸ਼(std::vector<UMat>& umv) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    AccessFlag accessFlags = flags & ACCESS_ਨਕਾਬ;

    ਜੇ( k == NONE )
    {
        umv.clear();
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& v = *(ਅਚਲ std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* v = (ਅਚਲ Mat*)obj;
        size_t n = sz.ਉਚਾਈ;
        umv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& v = *(ਅਚਲ std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ਲਈ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ਵਾਪਸ;
    }

    ਜੇ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ਵਾਪਸ;
    }
    ਜੇ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ਵਾਪਸ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat ਨਿਵੇਸ਼_Array_2::getGpuMat() ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ (k == CUDA_GPU_MAT)
    {
        ਅਚਲ cuda::GpuMat* d_mat = (ਅਚਲ cuda::GpuMat*)obj;
        ਵਾਪਸ *d_mat;
    }

    ਜੇ (k == CUDA_HOST_MEM)
    {
        ਅਚਲ cuda::HostMem* cuda_mem = (ਅਚਲ cuda::HostMem*)obj;
        ਵਾਪਸ cuda_mem->create_Gpu_Mat_ਸਿਰਲੇਖ();
    }

    ਜੇ (k == OPENGL_ਬਫਰ)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ਜੇ (k == NONE)
        ਵਾਪਸ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ਨਹੀਂਤਾਂ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
}
ਖਾਲੀ ਨਿਵੇਸ਼_Array_2::get_Gpu_Mat_ਸਦਿਸ਼(std::vector<cuda::GpuMat>& gpumv) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
}
ਖਾਲੀ ਨਿਵੇਸ਼_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
}
cuda::GpuMatND ਨਿਵੇਸ਼_Array_2::getGpuMatND() ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ (k == CUDA_GPU_MATND)
    {
        ਅਚਲ cuda::GpuMatND* d_mat = (ਅਚਲ cuda::GpuMatND*)obj;
        ਵਾਪਸ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ਨਹੀਂਤਾਂ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
}
ogl::ਬਫਰ_2 ਨਿਵੇਸ਼_Array_2::get_OGl_ਬਫਰ() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    CV_Assert(k == OPENGL_ਬਫਰ);

    ਅਚਲ ogl::ਬਫਰ_2* gl_buf = (ਅਚਲ ogl::ਬਫਰ_2*)obj;
    ਵਾਪਸ *gl_buf;
}

ਪੂਰਨ ਨਿਵੇਸ਼_Array_2::ਪੰਕਤੀਆਂ_2(ਪੂਰਨ i) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == CUDA_GPU_MATND)
    {
        ਅਚਲ cuda::GpuMatND& _gpuMatND = *(ਅਚਲ cuda::GpuMatND*)obj;
        ਵਾਪਸ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ਮਾਪ_3(i).ਉਚਾਈ;
}

ਪੂਰਨ ਨਿਵੇਸ਼_Array_2::ਖਾਨੇ_ਗਿਣਤੀ_2(ਪੂਰਨ i) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == CUDA_GPU_MATND)
    {
        ਅਚਲ cuda::GpuMatND& _gpuMatND = *(ਅਚਲ cuda::GpuMatND*)obj;
        ਵਾਪਸ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#ਜੇ_ਅੰਤ

    ਵਾਪਸ ਮਾਪ_3(i).ਚੌੜਾਈ;
}

ਮਾਪ ਨਿਵੇਸ਼_Array_2::ਮਾਪ_3(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ Mat* m = (ਅਚਲ Mat*)obj;
        CV_Assert(m->dims <= 2);
        ਵਾਪਸ ਮਾਪ(m->ਖਾਨੇ_ਗਿਣਤੀ, m->ਪੰਕਤੀਆਂ);
    }

    ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ UMat* m = (ਅਚਲ UMat*)obj;
        CV_Assert(m->dims <= 2);
        ਵਾਪਸ ਮਾਪ(m->ਖਾਨੇ_ਗਿਣਤੀ, m->ਪੰਕਤੀਆਂ);
    }

    ਜੇ (k == MATX)
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ sz;
    }

    ਜੇ( k == NONE )
        ਵਾਪਸ ਮਾਪ();

    ਜੇ( k == STD_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
    {
        size_t n = ਕੁੱਲ(i);
        CV_Assert(n <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
        ਵਾਪਸ ਮਾਪ((ਪੂਰਨ)n, 1);
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ vv.empty() ? ਮਾਪ() : ਮਾਪ((ਪੂਰਨ)vv.size(), 1);
        CV_Assert( i < (ਪੂਰਨ)vv.size() );

        ਵਾਪਸ vv[i].size();
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ sz.ਉਚਾਈ==0 ? ਮਾਪ() : ਮਾਪ(sz.ਉਚਾਈ, 1);
        CV_Assert( i < sz.ਉਚਾਈ );

        ਵਾਪਸ vv[i].size();
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ਅਚਲ std::vector<cuda::GpuMat>& vv = *(ਅਚਲ std::vector<cuda::GpuMat>*)obj;
        ਜੇ (i < 0)
            ਵਾਪਸ vv.empty() ? ਮਾਪ() : ਮਾਪ((ਪੂਰਨ)vv.size(), 1);
        CV_Assert(i < (ਪੂਰਨ)vv.size());
        ਵਾਪਸ vv[i].size();
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ਅਚਲ std::vector<cuda::GpuMatND>& vv = *(ਅਚਲ std::vector<cuda::GpuMatND>*)obj;
        ਜੇ (i < 0)
            ਵਾਪਸ vv.empty() ? ਮਾਪ() : ਮਾਪ((ਪੂਰਨ)vv.size(), 1);
        CV_Assert(i < (ਪੂਰਨ)vv.size());
        ਅਚਲ cuda::GpuMatND& m = vv[i];
        ਜੇ (m.dims == 0)
            ਵਾਪਸ ਮਾਪ();
        ਜੇ (m.dims == 1)
            ਵਾਪਸ ਮਾਪ(1, m.size[0]);
        ਵਾਪਸ ਮਾਪ(m.size[1], m.size[0]);
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ vv.empty() ? ਮਾਪ() : ਮਾਪ((ਪੂਰਨ)vv.size(), 1);
        CV_Assert( i < (ਪੂਰਨ)vv.size() );

        ਵਾਪਸ vv[i].size();
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ਅਚਲ std::vector<cuda::GpuMatND>& vv = *(ਅਚਲ std::vector<cuda::GpuMatND>*)obj;
        ਜੇ (i < 0)
            ਵਾਪਸ vv.empty() ? ਮਾਪ() : ਮਾਪ((ਪੂਰਨ)vv.size(), 1);
        CV_Assert(i < (ਪੂਰਨ)vv.size());
        ਅਚਲ cuda::GpuMatND& m = vv[i];
        ਜੇ (m.dims == 0)
            ਵਾਪਸ ਮਾਪ();
        ਜੇ (m.dims == 1)
            ਵਾਪਸ ਮਾਪ(1, m.size[0]);
        ਵਾਪਸ ਮਾਪ(m.size[1], m.size[0]);
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == OPENGL_ਬਫਰ )
    {
        CV_Assert( i < 0 );
        ਅਚਲ ogl::ਬਫਰ_2* buf = (ਅਚਲ ogl::ਬਫਰ_2*)obj;
        ਵਾਪਸ buf->size();
    }

    ਜੇ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ cuda::GpuMat* d_mat = (ਅਚਲ cuda::GpuMat*)obj;
        ਵਾਪਸ d_mat->size();
    }

    ਜੇ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ਅਚਲ cuda::HostMem* cuda_mem = (ਅਚਲ cuda::HostMem*)obj;
        ਵਾਪਸ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਪੂਰਨ ਨਿਵੇਸ਼_Array_2::sizend(ਪੂਰਨ* arrsz, ਪੂਰਨ i) ਅਚਲ
{
    ਪੂਰਨ j, d = 0;
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == NONE )
        ;
    ਨਹੀਂਤਾਂ ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ Mat& m = *(ਅਚਲ Mat*)obj;
        d = m.dims;
        ਜੇ(arrsz)
            ਲਈ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ UMat& m = *(ਅਚਲ UMat*)obj;
        d = m.dims;
        ਜੇ(arrsz)
            ਲਈ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == STD_ਸਦਿਸ਼_MAT && i >= 0 )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert( i < (ਪੂਰਨ)vv.size() );
        ਅਚਲ Mat& m = vv[i];
        d = m.dims;
        ਜੇ(arrsz)
            ਲਈ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        CV_Assert( i < sz.ਉਚਾਈ );
        ਅਚਲ Mat& m = vv[i];
        d = m.dims;
        ਜੇ(arrsz)
            ਲਈ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == STD_ਸਦਿਸ਼_UMAT && i >= 0 )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert( i < (ਪੂਰਨ)vv.size() );
        ਅਚਲ UMat& m = vv[i];
        d = m.dims;
        ਜੇ(arrsz)
            ਲਈ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ਨਹੀਂਤਾਂ ਜੇ (((k == STD_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼) && i < 0) ||
             (k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ && i >= 0))
    {
        size_t n = ਕੁੱਲ(i);
        d = 1;
        ਜੇ(arrsz)
        {
            arrsz[0] = ਪੂਰਨ(n);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        ਮਾਪ sz2d = size(i);
        d = 2;
        ਜੇ(arrsz)
        {
            arrsz[0] = sz2d.ਉਚਾਈ;
            arrsz[1] = sz2d.ਚੌੜਾਈ;
        }
    }

    ਵਾਪਸ d;
}

ਤਾਰਕਿਕ ਨਿਵੇਸ਼_Array_2::empty(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (i >= 0) {
        ਜੇ (k == STD_ਸਦਿਸ਼_MAT) {
            ਸਵੈਚਾਲਿਤ mv = reinterpret_cast<ਅਚਲ std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ਵਾਪਸ mv->at(i).empty();
        }
        ਨਹੀਂਤਾਂ ਜੇ (k == STD_ਸਦਿਸ਼_UMAT) {
            ਸਵੈਚਾਲਿਤ umv = reinterpret_cast<ਅਚਲ std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ਵਾਪਸ umv->at(i).empty();
        }
        ਨਹੀਂਤਾਂ ਜੇ (k == STD_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼) {
            size_t n = ਕੁੱਲ(i);
            ਵਾਪਸ n == 0;
        } ਨਹੀਂਤਾਂ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ਵਾਪਸ empty();
}

MatShape ਨਿਵੇਸ਼_Array_2::shape(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    MatShape shape;

    ਜੇ( k == NONE )
        ;
    ਨਹੀਂਤਾਂ ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ Mat& m = *(ਅਚਲ Mat*)obj;
        shape = m.size;
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ UMat& m = *(ਅਚਲ UMat*)obj;
        shape = m.size;
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == STD_ਸਦਿਸ਼_MAT && i >= 0 )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ਅਚਲ Mat& m = vv[i];
        shape = m.size;
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        CV_Assert( i < sz.ਉਚਾਈ );
        ਅਚਲ Mat& m = vv[i];
        shape = m.size;
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == STD_ਸਦਿਸ਼_UMAT && i >= 0 )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ਅਚਲ UMat& m = vv[i];
        shape = m.size;
    }
    ਨਹੀਂਤਾਂ
    {
        ਪੂਰਨ sizes[CV_MAX_DIM];
        ਪੂਰਨ dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ਵਾਪਸ shape;
}

ਤਾਰਕਿਕ ਨਿਵੇਸ਼_Array_2::same_ਮਾਪ(ਅਚਲ ਨਿਵੇਸ਼_Array_2& arr) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k1 = ਕਿਸਮ(), k2 = arr.ਕਿਸਮ();
    ਮਾਪ sz1;

    ਜੇ( k1 == MAT )
    {
        ਅਚਲ Mat* m = ((ਅਚਲ Mat*)obj);
        ਜੇ( k2 == MAT )
            ਵਾਪਸ m->size == ((ਅਚਲ Mat*)arr.obj)->size;
        ਜੇ( k2 == UMAT )
            ਵਾਪਸ m->size == ((ਅਚਲ UMat*)arr.obj)->size;
        ਜੇ( m->dims > 2 )
            ਵਾਪਸ ਝੂਠ;
        sz1 = m->size();
    }
    ਨਹੀਂਤਾਂ ਜੇ( k1 == UMAT )
    {
        ਅਚਲ UMat* m = ((ਅਚਲ UMat*)obj);
        ਜੇ( k2 == MAT )
            ਵਾਪਸ m->size == ((ਅਚਲ Mat*)arr.obj)->size;
        ਜੇ( k2 == UMAT )
            ਵਾਪਸ m->size == ((ਅਚਲ UMat*)arr.obj)->size;
        ਜੇ( m->dims > 2 )
            ਵਾਪਸ ਝੂਠ;
        sz1 = m->size();
    }
    ਨਹੀਂਤਾਂ ਜੇ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMatND*)obj)->size == ((ਅਚਲ cuda::GpuMatND*)arr.obj)->size;
    }
    ਨਹੀਂਤਾਂ
        sz1 = size();

    ਜੇ( arr.dims() > 2 )
        ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ sz1 == arr.size();
}

ਪੂਰਨ ਨਿਵੇਸ਼_Array_2::dims_(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ Mat*)obj)->dims;
    }

    ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ UMat*)obj)->dims;
    }

    ਜੇ (k == MATX)
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ 2;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ 1;
    }

    ਜੇ( k == NONE )
        ਵਾਪਸ 0;

    ਜੇ( k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ )
    {
        ਜੇ( i < 0 )
            ਵਾਪਸ 1;
        CV_Assert( size_t(i) < ਕੁੱਲ() );
        ਵਾਪਸ 1;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ 1;
        CV_Assert( i < (ਪੂਰਨ)vv.size() );

        ਵਾਪਸ vv[i].dims;
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ 1;
        CV_Assert( i < sz.ਉਚਾਈ );

        ਵਾਪਸ vv[i].dims;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ 1;
        CV_Assert( i < (ਪੂਰਨ)vv.size() );

        ਵਾਪਸ vv[i].dims;
    }

    ਜੇ( k == OPENGL_ਬਫਰ )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ 2;
    }

    ਜੇ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ 2;
    }

    ਜੇ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMatND*)obj)->dims;
    }

    ਜੇ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t ਨਿਵੇਸ਼_Array_2::ਕੁੱਲ(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ Mat*)obj)->ਕੁੱਲ();
    }

    ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ UMat*)obj)->ਕੁੱਲ();
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ vv.size();

        CV_Assert( i < (ਪੂਰਨ)vv.size() );
        ਵਾਪਸ vv[i].ਕੁੱਲ();
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ sz.ਉਚਾਈ;

        CV_Assert( i < sz.ਉਚਾਈ );
        ਵਾਪਸ vv[i].ਕੁੱਲ();
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        ਜੇ( i < 0 )
            ਵਾਪਸ vv.size();

        CV_Assert( i < (ਪੂਰਨ)vv.size() );
        ਵਾਪਸ vv[i].ਕੁੱਲ();
    }

    ਜੇ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ਅਚਲ cuda::GpuMatND& _gpuMatND = *((ਅਚਲ cuda::GpuMatND*)obj);
        ਜੇ (_gpuMatND.dims > 0)
        {
            res = 1;
            ਲਈ(ਪੂਰਨ d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ਵਾਪਸ res;
        }
    }

    ਜੇ (k == STD_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼)
    {
        CV_Assert(i < 0 || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼);
        ਪੂਰਨ esz = CV_ELEM_ਮਾਪ(flags);

        #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_VEC_ਮਾਪ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_VEC_ਮਾਪ(T) \
            ਜੇ (k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼) { \
                ਅਚਲ std::vector<std::vector<T > >* vv = (ਅਚਲ std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ਜੇ (i < 0) \
                    ਵਾਪਸ n; \
                CV_Assert((size_t)i < n); \
                ਵਾਪਸ vv->at(i).size(); \
            } ਨਹੀਂਤਾਂ \
                ਵਾਪਸ ((ਅਚਲ std::vector<T >*)obj)->size()

        STD_ਸਦਿਸ਼_SWITCH(esz, GET_VEC_ਮਾਪ)
    }

    ਵਾਪਸ size(i).area();
}

ਪੂਰਨ ਨਿਵੇਸ਼_Array_2::type_(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
        ਵਾਪਸ ((ਅਚਲ Mat*)obj)->type();

    ਜੇ( k == UMAT )
        ਵਾਪਸ ((ਅਚਲ UMat*)obj)->type();

    ਜੇ( k == MATX || k == STD_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
        ਵਾਪਸ CV_MAT_TYPE(flags);

    ਜੇ( k == NONE )
        ਵਾਪਸ -1;

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        ਜੇ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ਵਾਪਸ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ਪੂਰਨ)vv.size() );
        ਵਾਪਸ vv[i >= 0 ? i : 0].type();
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        ਜੇ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ਵਾਪਸ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ਪੂਰਨ)vv.size() );
        ਵਾਪਸ vv[i >= 0 ? i : 0].type();
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        ਜੇ( sz.ਉਚਾਈ == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ਵਾਪਸ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.ਉਚਾਈ );
        ਵਾਪਸ vv[i >= 0 ? i : 0].type();
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ਅਚਲ std::vector<cuda::GpuMat>& vv = *(ਅਚਲ std::vector<cuda::GpuMat>*)obj;
        ਜੇ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ਵਾਪਸ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ਪੂਰਨ)vv.size());
        ਵਾਪਸ vv[i >= 0 ? i : 0].type();
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ਅਚਲ std::vector<cuda::GpuMatND>& vv = *(ਅਚਲ std::vector<cuda::GpuMatND>*)obj;
        ਜੇ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ਵਾਪਸ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ਪੂਰਨ)vv.size());
        ਵਾਪਸ vv[i >= 0 ? i : 0].type();
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == OPENGL_ਬਫਰ )
        ਵਾਪਸ ((ਅਚਲ ogl::ਬਫਰ_2*)obj)->type();

    ਜੇ( k == CUDA_GPU_MAT )
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMat*)obj)->type();

    ਜੇ( k == CUDA_GPU_MATND )
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMatND*)obj)->type();

    ਜੇ( k == CUDA_HOST_MEM )
        ਵਾਪਸ ((ਅਚਲ cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਪੂਰਨ ਨਿਵੇਸ਼_Array_2::ਡੂੰਘਾਈ_4(ਪੂਰਨ i) ਅਚਲ
{
    ਵਾਪਸ CV_MAT_ਡੂੰਘਾਈ(type(i));
}

ਤਾਰਕਿਕ ਨਿਵੇਸ਼_Array_2::ਖਾਲੀ_ਥਾਵਾਂ() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
        ਵਾਪਸ ((ਅਚਲ Mat*)obj)->empty();

    ਜੇ( k == UMAT )
        ਵਾਪਸ ((ਅਚਲ UMat*)obj)->empty();

    ਜੇ (k == MATX)
        ਵਾਪਸ ਝੂਠ;

    ਜੇ( k == STD_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ )
    {
        size_t n = ਕੁੱਲ(-1);
        ਵਾਪਸ n == 0;
    }

    ਜੇ( k == NONE )
        ਵਾਪਸ ਸੱਚ;

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        ਵਾਪਸ vv.empty();
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਵਾਪਸ sz.ਉਚਾਈ == 0;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        ਵਾਪਸ vv.empty();
    }

    ਜੇ( k == OPENGL_ਬਫਰ )
        ਵਾਪਸ ((ਅਚਲ ogl::ਬਫਰ_2*)obj)->empty();

    ਜੇ( k == CUDA_GPU_MAT )
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMat*)obj)->empty();

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
        ਅਚਲ std::vector<cuda::GpuMat>& vv = *(ਅਚਲ std::vector<cuda::GpuMat>*)obj;
        ਵਾਪਸ vv.empty();
    }

    ਜੇ( k == CUDA_GPU_MATND )
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMatND*)obj)->empty();

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
        ਅਚਲ std::vector<cuda::GpuMatND>& vv = *(ਅਚਲ std::vector<cuda::GpuMatND>*)obj;
        ਵਾਪਸ vv.empty();
    }

    ਜੇ( k == CUDA_HOST_MEM )
        ਵਾਪਸ ((ਅਚਲ cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਤਾਰਕਿਕ ਨਿਵੇਸ਼_Array_2::isContinuous(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
        ਵਾਪਸ i < 0 ? ((ਅਚਲ Mat*)obj)->isContinuous() : ਸੱਚ;

    ਜੇ( k == UMAT )
        ਵਾਪਸ i < 0 ? ((ਅਚਲ UMat*)obj)->isContinuous() : ਸੱਚ;

    ਜੇ( k == MATX || k == STD_ਸਦਿਸ਼ ||
        k == NONE || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
        ਵਾਪਸ ਸੱਚ;

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].isContinuous();
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.ਉਚਾਈ);
        ਵਾਪਸ vv[i].isContinuous();
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].isContinuous();
    }

    ਜੇ( k == CUDA_GPU_MAT )
      ਵਾਪਸ i < 0 ? ((ਅਚਲ cuda::GpuMat*)obj)->isContinuous() : ਸੱਚ;

    ਜੇ( k == CUDA_GPU_MATND )
        ਵਾਪਸ i < 0 ? ((ਅਚਲ cuda::GpuMatND*)obj)->isContinuous() : ਸੱਚ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਤਾਰਕਿਕ ਨਿਵੇਸ਼_Array_2::isSubmatrix(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
        ਵਾਪਸ i < 0 ? ((ਅਚਲ Mat*)obj)->isSubmatrix() : ਝੂਠ;

    ਜੇ( k == UMAT )
        ਵਾਪਸ i < 0 ? ((ਅਚਲ UMat*)obj)->isSubmatrix() : ਝੂਠ;

    ਜੇ( k == MATX || k == STD_ਸਦਿਸ਼ ||
        k == NONE || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
        ਵਾਪਸ ਝੂਠ;

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].isSubmatrix();
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.ਉਚਾਈ);
        ਵਾਪਸ vv[i].isSubmatrix();
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].isSubmatrix();
    }

    ਜੇ( k == CUDA_GPU_MATND )
    {
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
        ਅਚਲ std::vector<cuda::GpuMatND>& vv = *(ਅਚਲ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t ਨਿਵੇਸ਼_Array_2::offset(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ Mat * ਅਚਲ m = ((ਅਚਲ Mat*)obj);
        ਵਾਪਸ (size_t)(m->ptr() - m->datastart);
    }

    ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ UMat*)obj)->offset;
    }

    ਜੇ( k == MATX || k == STD_ਸਦਿਸ਼ ||
        k == NONE || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
        ਵਾਪਸ 0;

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ਪੂਰਨ)vv.size() );

        ਵਾਪਸ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.ਉਚਾਈ );
        ਵਾਪਸ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].offset;
    }

    ਜੇ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ਅਚਲ cuda::GpuMat * ਅਚਲ m = ((ਅਚਲ cuda::GpuMat*)obj);
        ਵਾਪਸ (size_t)(m->ਅੰਕੜੇ - m->datastart);
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
        ਅਚਲ std::vector<cuda::GpuMat>& vv = *(ਅਚਲ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ (size_t)(vv[i].ਅੰਕੜੇ - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t ਨਿਵੇਸ਼_Array_2::ਕਦਮ(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ Mat*)obj)->ਕਦਮ;
    }

    ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ UMat*)obj)->ਕਦਮ;
    }

    ਜੇ( k == MATX || k == STD_ਸਦਿਸ਼ ||
        k == NONE || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
        ਵਾਪਸ 0;

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ਅਚਲ std::vector<Mat>& vv = *(ਅਚਲ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ਪੂਰਨ)vv.size() );
        ਵਾਪਸ vv[i].ਕਦਮ;
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        ਅਚਲ Mat* vv = (ਅਚਲ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.ਉਚਾਈ );
        ਵਾਪਸ vv[i].ਕਦਮ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ਅਚਲ std::vector<UMat>& vv = *(ਅਚਲ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].ਕਦਮ;
    }

    ਜੇ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ਵਾਪਸ ((ਅਚਲ cuda::GpuMat*)obj)->ਕਦਮ;
    }
    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
        ਅਚਲ std::vector<cuda::GpuMat>& vv = *(ਅਚਲ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਵਾਪਸ vv[i].ਕਦਮ;
    }
    ਜੇ( k == CUDA_GPU_MATND )
    {
        ਅਚਲ cuda::GpuMatND& _gpuMatND = *(ਅਚਲ cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ਵਾਪਸ _gpuMatND.ਕਦਮ[i];
    }

    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND)
    {
        ਅਚਲ std::vector<cuda::GpuMatND>& vv = *(ਅਚਲ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ਅਚਲ cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ਵਾਪਸ _gpuMatND.ਕਦਮ[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

ਖਾਲੀ ਨਿਵੇਸ਼_Array_2::copy_ਮੰਜ਼ਿਲ(ਅਚਲ _OutputArray& arr) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == NONE )
        arr.release();
    ਨਹੀਂਤਾਂ ਜੇ( k == MAT || k == MATX || k == STD_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
    {
        Mat m = getMat();
        m.copy_ਮੰਜ਼ਿਲ(arr);
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == UMAT )
        ((UMat*)obj)->copy_ਮੰਜ਼ਿਲ(arr);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਹੀਂਤਾਂ ਜੇ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_ਮੰਜ਼ਿਲ(arr);
#ਜੇ_ਅੰਤ
    ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "");
}

ਖਾਲੀ ਨਿਵੇਸ਼_Array_2::copy_ਮੰਜ਼ਿਲ(ਅਚਲ _OutputArray& arr, ਅਚਲ ਨਿਵੇਸ਼_Array_2 & ਨਕਾਬ) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == NONE )
        arr.release();
    ਨਹੀਂਤਾਂ ਜੇ( k == MAT || k == MATX || k == STD_ਸਦਿਸ਼ || k == STD_BOOL_ਸਦਿਸ਼ )
    {
        Mat m = getMat();
        m.copy_ਮੰਜ਼ਿਲ(arr, ਨਕਾਬ);
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == UMAT )
        ((UMat*)obj)->copy_ਮੰਜ਼ਿਲ(arr, ਨਕਾਬ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
    ਨਹੀਂਤਾਂ ਜੇ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_ਮੰਜ਼ਿਲ(arr, ਨਕਾਬ);
#ਜੇ_ਅੰਤ
    ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "");
}

ਤਾਰਕਿਕ _OutputArray::fixed_ਮਾਪ() ਅਚਲ
{
    ਵਾਪਸ (flags & FIXED_ਮਾਪ) == FIXED_ਮਾਪ;
}

ਤਾਰਕਿਕ _OutputArray::fixedType() ਅਚਲ
{
    ਵਾਪਸ (flags & FIXED_TYPE) == FIXED_TYPE;
}

ਖਾਲੀ _OutputArray::create_(ਮਾਪ _sz, ਪੂਰਨ mtype, ਪੂਰਨ i, ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ( k == MAT && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((Mat*)obj)->size.ਸੰਚਾਲਕ()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ਵਾਪਸ;
    }
    ਜੇ( k == UMAT && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((UMat*)obj)->size.ਸੰਚਾਲਕ()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ਵਾਪਸ;
    }
    ਜੇ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਜੇ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.ਉਚਾਈ) && (((cuda::GpuMatND*)obj)->size[1] == _sz.ਚੌੜਾਈ)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        cuda::GpuMatND::ਮਾਪ_Array sizes({_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਜੇ( k == OPENGL_ਬਫਰ && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((ogl::ਬਫਰ_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::ਬਫਰ_2*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
        ((ogl::ਬਫਰ_2*)obj)->create(_sz, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#ਜੇ_ਅੰਤ
    }
    ਜੇ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਪੂਰਨ sizes[] = {_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ};
    create(2, sizes, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
}

ਖਾਲੀ _OutputArray::create_(ਪੂਰਨ ਪੰਕਤੀਆਂ_3, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੂਰਨ mtype, ਪੂਰਨ i, ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ( k == MAT && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((Mat*)obj)->size.ਸੰਚਾਲਕ()() == ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੰਕਤੀਆਂ_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, mtype);
        ਵਾਪਸ;
    }
    ਜੇ( k == UMAT && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((UMat*)obj)->size.ਸੰਚਾਲਕ()() == ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੰਕਤੀਆਂ_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, mtype);
        ਵਾਪਸ;
    }
    ਜੇ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((cuda::GpuMat*)obj)->size() == ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੰਕਤੀਆਂ_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਜੇ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == ਪੰਕਤੀਆਂ_3) && (((cuda::GpuMatND*)obj)->size[1] == ਖਾਨੇ_ਗਿਣਤੀ_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        cuda::GpuMatND::ਮਾਪ_Array sizes({ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਜੇ( k == OPENGL_ਬਫਰ && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((ogl::ਬਫਰ_2*)obj)->size() == ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੰਕਤੀਆਂ_3));
        CV_Assert(!fixedType() || ((ogl::ਬਫਰ_2*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
        ((ogl::ਬਫਰ_2*)obj)->create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#ਜੇ_ਅੰਤ
    }
    ਜੇ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
        CV_Assert(!fixed_ਮਾਪ() || ((cuda::HostMem*)obj)->size() == ਮਾਪ(ਖਾਨੇ_ਗਿਣਤੀ_3, ਪੰਕਤੀਆਂ_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::HostMem*)obj)->create(ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਪੂਰਨ sizes[] = {ਪੰਕਤੀਆਂ_3, ਖਾਨੇ_ਗਿਣਤੀ_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
}

ਖਾਲੀ _OutputArray::create(ਪੂਰਨ d, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ mtype, ਪੂਰਨ i,
                          ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਪੂਰਨ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    mtype = CV_MAT_TYPE(mtype);

    ਜੇ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_ਮਾਪ()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ਜੇ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.ਪੰਕਤੀਆਂ == size0 && m.ਖਾਨੇ_ਗਿਣਤੀ == size1) ||
            (allowTransposed && m.ਪੰਕਤੀਆਂ == size1 && m.ਖਾਨੇ_ਗਿਣਤੀ == size0 && m.isContinuous())))
        {
            ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_ਡੂੰਘਾਈ(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m.type();
            ਨਹੀਂਤਾਂ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ਜੇ(fixed_ਮਾਪ())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ਵਾਪਸ;
    }

    ਜੇ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_ਮਾਪ()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ਜੇ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.ਪੰਕਤੀਆਂ == size0 && m.ਖਾਨੇ_ਗਿਣਤੀ == size1) ||
            (allowTransposed && m.ਪੰਕਤੀਆਂ == size1 && m.ਖਾਨੇ_ਗਿਣਤੀ == size0 && m.isContinuous())))
        {
            ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m.type();
            ਨਹੀਂਤਾਂ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ਜੇ(fixed_ਮਾਪ())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ਵਾਪਸ;
    }

    ਜੇ( k == MATX )
    {
        CV_Assert( i < 0 );
        ਪੂਰਨ type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0) );
        CV_CheckLE(d, 2, "");
        ਮਾਪ requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ਜੇ (sz.ਚੌੜਾਈ == 1 || sz.ਉਚਾਈ == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ਪੂਰਨ ਕੁੱਲ_1d = std::max(sz.ਚੌੜਾਈ, sz.ਉਚਾਈ);
            CV_Check(requested_size, std::max(requested_size.ਚੌੜਾਈ, requested_size.ਉਚਾਈ) == ਕੁੱਲ_1d, "");
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ਨਹੀਂਤਾਂ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.ਉਚਾਈ == sz.ਚੌੜਾਈ && requested_size.ਚੌੜਾਈ == sz.ਉਚਾਈ)),
                        "");
            }
        }
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ )
    {
        CV_Assert(!fixed_ਮਾਪ());
        CV_Assert(k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ਪੂਰਨ esz = CV_ELEM_ਮਾਪ(flags);

        ਜੇ( k == STD_ਸਦਿਸ਼ || (k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ && i >= 0) )
        {
            ਪੂਰਨ type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0) );
        }

    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ RESIZE_VEC
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ਜੇ (k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ਜੇ (i < 0) { \
                    CV_Assert(!fixed_ਮਾਪ() || len == vv->size()); \
                    vv->resize(len); \
                    ਵਾਪਸ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ਨਹੀਂਤਾਂ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_ਮਾਪ() || len == v->size()); \
            v->resize(len); \
        } \
        ਰੋਕੋ

        STD_ਸਦਿਸ਼_SWITCH(esz, RESIZE_VEC)
        ਵਾਪਸ;
    }

    ਜੇ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ਜੇ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_ਮਾਪ() || len == len0);
            v.resize(len);
            ਜੇ( fixedType() )
            {
                ਪੂਰਨ _type = CV_MAT_TYPE(flags);
                ਲਈ( size_t j = len0; j < len; j++ )
                {
                    ਜੇ( v[j].type() == _type )
                        ਜਾਰੀ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ਨਕਾਬ) | _type;
                }
            }
            ਵਾਪਸ;
        }

        CV_Assert( i < (ਪੂਰਨ)v.size() );
        Mat& m = v[i];

        ਜੇ( allowTransposed )
        {
            ਜੇ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_ਮਾਪ());
                m.release();
            }

            ਜੇ( d == 2 && m.dims == 2 && m.ਅੰਕੜੇ &&
                m.type() == mtype && m.ਪੰਕਤੀਆਂ == sizes[1] && m.ਖਾਨੇ_ਗਿਣਤੀ == sizes[0] )
                ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m.type();
            ਨਹੀਂਤਾਂ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ਜੇ(fixed_ਮਾਪ())
        {
            CV_Assert(m.dims == d);
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ਜੇ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.ਉਚਾਈ;

            CV_Assert(len == len0);
            ਜੇ( fixedType() )
            {
                ਪੂਰਨ _type = CV_MAT_TYPE(flags);
                ਲਈ( size_t j = len0; j < len; j++ )
                {
                    ਜੇ( v[j].type() == _type )
                        ਜਾਰੀ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ਨਕਾਬ) | _type;
                }
            }
            ਵਾਪਸ;
        }

        CV_Assert( i < sz.ਉਚਾਈ );
        Mat& m = v[i];

        ਜੇ( allowTransposed )
        {
            ਜੇ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_ਮਾਪ());
                m.release();
            }

            ਜੇ( d == 2 && m.dims == 2 && m.ਅੰਕੜੇ &&
                m.type() == mtype && m.ਪੰਕਤੀਆਂ == sizes[1] && m.ਖਾਨੇ_ਗਿਣਤੀ == sizes[0] )
                ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m.type();
            ਨਹੀਂਤਾਂ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ਜੇ(fixed_ਮਾਪ())
        {
            CV_Assert(m.dims == d);
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ਜੇ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_ਮਾਪ() || len == len0);
            v.resize(len);
            ਜੇ( fixedType() )
            {
                ਪੂਰਨ _type = CV_MAT_TYPE(flags);
                ਲਈ( size_t j = len0; j < len; j++ )
                {
                    ਜੇ( v[j].type() == _type )
                        ਜਾਰੀ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ਨਕਾਬ) | _type;
                }
            }
            ਵਾਪਸ;
        }

        CV_Assert( i < (ਪੂਰਨ)v.size() );
        UMat& m = v[i];

        ਜੇ( allowTransposed )
        {
            ਜੇ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_ਮਾਪ());
                m.release();
            }

            ਜੇ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.ਪੰਕਤੀਆਂ == sizes[1] && m.ਖਾਨੇ_ਗਿਣਤੀ == sizes[0] )
                ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m.type();
            ਨਹੀਂਤਾਂ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ਜੇ(fixed_ਮਾਪ())
        {
            CV_Assert(m.dims == d);
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ਵਾਪਸ;
    }

    ਜੇ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
        ਵਾਪਸ;
    }

    ਜੇ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_ਡੂੰਘਾਈ_ਨਕਾਬ == 0 )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        cuda::GpuMatND::ਮਾਪ_Array ਮਾਪ_Array_2 = cuda::GpuMatND::ਮਾਪ_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(ਮਾਪ_Array_2, mtype);
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਖਾਲੀ _OutputArray::create(ਅਚਲ MatShape& shape, ਪੂਰਨ mtype, ਪੂਰਨ i,
                          ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਜੇ (shape.dims < 0) {
        release();
    } ਨਹੀਂਤਾਂ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
    }
}

Mat _OutputArray::reinterpret(ਪੂਰਨ mtype) ਅਚਲ
{
    mtype = CV_MAT_TYPE(mtype);
    ਵਾਪਸ getMat().reinterpret(mtype);
}

ਖਾਲੀ _OutputArray::create_Same_ਮਾਪ(ਅਚਲ ਨਿਵੇਸ਼_Array_2& arr, ਪੂਰਨ mtype) ਅਚਲ
{
    create(arr.shape(), mtype);
}

ਖਾਲੀ _OutputArray::fit(ਮਾਪ _sz, ਪੂਰਨ mtype, ਪੂਰਨ i,
                       ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਪੂਰਨ sizes[] = {_sz.ਉਚਾਈ, _sz.ਚੌੜਾਈ};
    fit(2, sizes, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
}

ਖਾਲੀ _OutputArray::fit(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ mtype, ਪੂਰਨ i,
                       ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਪੂਰਨ sizes[] = {ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ};
    fit(2, sizes, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
}

ਖਾਲੀ _OutputArray::fit(ਅਚਲ MatShape& shape, ਪੂਰਨ mtype, ਪੂਰਨ i,
                       ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਜੇ (shape.dims < 0) {
        release();
        ਵਾਪਸ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ ((k == MAT && i < 0) || (k == STD_ਸਦਿਸ਼_MAT && i >= 0)) {
        Mat_ਮਾਪ& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ਨਹੀਂਤਾਂ ਜੇ ((k == UMAT && i < 0) || (k == STD_ਸਦਿਸ਼_UMAT && i >= 0)) {
        Mat_ਮਾਪ& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

ਖਾਲੀ _OutputArray::fit_Same_ਮਾਪ(ਅਚਲ ਨਿਵੇਸ਼_Array_2& arr, ਪੂਰਨ mtype) ਅਚਲ
{
    fit(arr.shape(), mtype);
}

ਖਾਲੀ _OutputArray::fit(ਪੂਰਨ d, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ mtype, ਪੂਰਨ i,
                       ਤਾਰਕਿਕ allowTransposed, _OutputArray::ਡੂੰਘਾਈ_ਨਕਾਬ fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) ਅਚਲ
{
    ਪੂਰਨ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    mtype = CV_MAT_TYPE(mtype);

    ਜੇ( (k == MAT && i < 0) || (k == STD_ਸਦਿਸ਼_MAT && i >= 0) )
    {
        Mat* m;
        ਜੇ (k == MAT)
            m = (Mat*)obj;
        ਨਹੀਂਤਾਂ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_ਮਾਪ()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ਜੇ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->ਪੰਕਤੀਆਂ == size0 && m->ਖਾਨੇ_ਗਿਣਤੀ == size1) ||
             (allowTransposed && m->ਪੰਕਤੀਆਂ == size1 && m->ਖਾਨੇ_ਗਿਣਤੀ == size0 && m->isContinuous())))
        {
            ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_ਡੂੰਘਾਈ(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m->type();
            ਨਹੀਂਤਾਂ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ਜੇ(fixed_ਮਾਪ())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ਵਾਪਸ;
    }

    ਜੇ( (k == UMAT && i < 0) || (k == STD_ਸਦਿਸ਼_UMAT && i >= 0) )
    {
        UMat* m;
        ਜੇ (k == UMAT)
            m = (UMat*)obj;
        ਨਹੀਂਤਾਂ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_ਮਾਪ()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ਜੇ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->ਪੰਕਤੀਆਂ == size0 && m->ਖਾਨੇ_ਗਿਣਤੀ == size1) ||
             (allowTransposed && m->ਪੰਕਤੀਆਂ == size1 && m->ਖਾਨੇ_ਗਿਣਤੀ == size0 && m->isContinuous())))
        {
            ਵਾਪਸ;
        }

        ਜੇ(fixedType())
        {
            ਜੇ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_ਡੂੰਘਾਈ(flags)) & fixed_ਡੂੰਘਾਈ_ਨਕਾਬ) != 0 )
                mtype = m->type();
            ਨਹੀਂਤਾਂ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ਜੇ(fixed_ਮਾਪ())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ਲਈ(ਪੂਰਨ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ਵਾਪਸ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_ਡੂੰਘਾਈ_ਨਕਾਬ);
}

ਖਾਲੀ _OutputArray::release() ਅਚਲ
{
    CV_Assert(!fixed_ਮਾਪ());

    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        ((Mat*)obj)->release();
        ਵਾਪਸ;
    }

    ਜੇ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ਵਾਪਸ;
    }

    ਜੇ( k == CUDA_GPU_MAT )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == CUDA_GPU_MATND )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == CUDA_HOST_MEM )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == OPENGL_ਬਫਰ )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENGL
        ((ogl::ਬਫਰ_2*)obj)->release();
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#ਜੇ_ਅੰਤ
    }

    ਜੇ( k == NONE )
        ਵਾਪਸ;

    ਜੇ( k == STD_ਸਦਿਸ਼ || k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼ )
    {
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CLEAR_VEC
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CLEAR_VEC(T) \
        ਜੇ (k == STD_ਸਦਿਸ਼_ਸਦਿਸ਼) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ਨਹੀਂਤਾਂ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        ਰੋਕੋ

        ਪੂਰਨ esz = CV_ELEM_ਮਾਪ(flags);
        STD_ਸਦਿਸ਼_SWITCH(esz, CLEAR_VEC);
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        ਲਈ( ਪੂਰਨ i = 0; i < sz.ਉਚਾਈ; i++ )
            v[i].release();
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_BOOL_ਸਦਿਸ਼ )
    {
        ((std::vector<ਤਾਰਕਿਕ>*)obj)->clear();
        ਵਾਪਸ;
    }

    ਜੇ( k == STD_ਸਦਿਸ਼_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ਵਾਪਸ;
    }
    ਜੇ (k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT)
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ਵਾਪਸ;
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ਖਾਲੀ _OutputArray::clear() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == MAT )
    {
        CV_Assert(!fixed_ਮਾਪ());
        ((Mat*)obj)->resize(0);
        ਵਾਪਸ;
    }

    release();
}

ਤਾਰਕਿਕ _OutputArray::needed() ਅਚਲ
{
    ਵਾਪਸ ਕਿਸਮ() != NONE;
}

Mat& _OutputArray::getMatRef(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ( i < 0 )
    {
        CV_Assert( k == MAT );
        ਵਾਪਸ *(Mat*)obj;
    }

    CV_Assert( k == STD_ਸਦਿਸ਼_MAT || k == STD_ARRAY_MAT );

    ਜੇ( k == STD_ਸਦਿਸ਼_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ਪੂਰਨ)v.size() );
        ਵਾਪਸ v[i];
    }
    ਨਹੀਂਤਾਂ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.ਉਚਾਈ );
        ਵਾਪਸ v[i];
    }
}

UMat& _OutputArray::getUMatRef(ਪੂਰਨ i) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ਵਾਪਸ *(UMat*)obj;
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Assert( k == STD_ਸਦਿਸ਼_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ਪੂਰਨ)v.size() );
        ਵਾਪਸ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    CV_Assert( k == CUDA_GPU_MAT );
    ਵਾਪਸ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    CV_Assert(k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT);
    ਵਾਪਸ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    CV_Assert( k == CUDA_GPU_MATND );
    ਵਾਪਸ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    CV_Assert(k == STD_ਸਦਿਸ਼_CUDA_GPU_MAT_ND);
    ਵਾਪਸ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::ਬਫਰ_2& _OutputArray::get_OGl_ਬਫਰ_Ref() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    CV_Assert( k == OPENGL_ਬਫਰ );
    ਵਾਪਸ *(ogl::ਬਫਰ_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    CV_Assert( k == CUDA_HOST_MEM );
    ਵਾਪਸ *(cuda::HostMem*)obj;
}

ਖਾਲੀ _OutputArray::ਸੈੱਟ_ਮੰਜ਼ਿਲ(ਅਚਲ ਨਿਵੇਸ਼_Array_2& arr, ਅਚਲ ਨਿਵੇਸ਼_Array_2 & ਨਕਾਬ) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == NONE )
        ;
    ਨਹੀਂਤਾਂ ਜੇ (k == MAT || k == MATX || k == STD_ਸਦਿਸ਼)
    {
        Mat m = getMat();
        m.ਸੈੱਟ_ਮੰਜ਼ਿਲ(arr, ਨਕਾਬ);
    }
    ਨਹੀਂਤਾਂ ਜੇ( k == UMAT )
        ((UMat*)obj)->ਸੈੱਟ_ਮੰਜ਼ਿਲ(arr, ਨਕਾਬ);
    ਨਹੀਂਤਾਂ ਜੇ( k == CUDA_GPU_MAT )
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.ਕਿਸਮ(), ਨਿਵੇਸ਼_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar(Vec<ਦੁੱਗਣਾ, 4>(value.ptr<ਦੁੱਗਣਾ>())), ਨਕਾਬ);
#ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ਜੇ_ਅੰਤ
    }
    ਨਹੀਂਤਾਂ
        CV_Error(Error::StsNotImplemented, "");
}

ਖਾਲੀ _OutputArray::ਸੈੱਟ_Zero() ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();

    ਜੇ( k == NONE )
        ;
    ਨਹੀਂਤਾਂ ਜੇ (k == MAT || k == MATX || k == STD_ਸਦਿਸ਼)
    {
        Mat m = getMat();
        m.ਸੈੱਟ_Zero();
    }
    ਨਹੀਂਤਾਂ
    {
        ਸੈੱਟ_ਮੰਜ਼ਿਲ(Scalar::all(0), noArray());
    }
}

ਖਾਲੀ _OutputArray::assign(ਅਚਲ UMat& u) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MAT)
    {
        u.copy_ਮੰਜ਼ਿਲ(*(Mat*)obj); // TODO check u.getMat()
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MATX)
    {
        u.copy_ਮੰਜ਼ਿਲ(getMat()); // TODO check u.getMat()
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ਖਾਲੀ _OutputArray::assign(ਅਚਲ Mat& m) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == UMAT)
    {
        m.copy_ਮੰਜ਼ਿਲ(*(UMat*)obj); // TODO check m.getUMat()
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MATX)
    {
        m.copy_ਮੰਜ਼ਿਲ(getMat());
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ਖਾਲੀ _OutputArray::move(UMat& u) ਅਚਲ
{
    ਜੇ (fixed_ਮਾਪ())
    {
        // TODO Performance warning
        assign(u);
        ਵਾਪਸ;
    }
    ਪੂਰਨ k = ਕਿਸਮ();
    ਜੇ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MAT)
    {
        u.copy_ਮੰਜ਼ਿਲ(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MATX)
    {
        u.copy_ਮੰਜ਼ਿਲ(getMat()); // TODO check u.getMat()
        u.release();
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ਖਾਲੀ _OutputArray::move(Mat& m) ਅਚਲ
{
    ਜੇ (fixed_ਮਾਪ())
    {
        // TODO Performance warning
        assign(m);
        ਵਾਪਸ;
    }
    ਪੂਰਨ k = ਕਿਸਮ();
    ਜੇ (k == UMAT)
    {
        m.copy_ਮੰਜ਼ਿਲ(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == MATX)
    {
        m.copy_ਮੰਜ਼ਿਲ(getMat());
        m.release();
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ਖਾਲੀ _OutputArray::assign(ਅਚਲ std::vector<UMat>& v) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == STD_ਸਦਿਸ਼_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ਲਈ (size_t i = 0; i < v.size(); i++)
        {
            ਅਚਲ UMat& m = v[i];
            UMat& this_m = this_v[i];
            ਜੇ (this_m.u != ਨਿਰਾ && this_m.u == m.u)
                ਜਾਰੀ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ਮੰਜ਼ਿਲ(this_m);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == STD_ਸਦਿਸ਼_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ਲਈ (size_t i = 0; i < v.size(); i++)
        {
            ਅਚਲ UMat& m = v[i];
            Mat& this_m = this_v[i];
            ਜੇ (this_m.u != ਨਿਰਾ && this_m.u == m.u)
                ਜਾਰੀ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ਮੰਜ਼ਿਲ(this_m);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ਖਾਲੀ _OutputArray::assign(ਅਚਲ std::vector<Mat>& v) ਅਚਲ
{
    ਨਿਵੇਸ਼_Array_2::ਕਿਸਮ_Flag k = ਕਿਸਮ();
    ਜੇ (k == STD_ਸਦਿਸ਼_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ਲਈ (size_t i = 0; i < v.size(); i++)
        {
            ਅਚਲ Mat& m = v[i];
            UMat& this_m = this_v[i];
            ਜੇ (this_m.u != ਨਿਰਾ && this_m.u == m.u)
                ਜਾਰੀ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ਮੰਜ਼ਿਲ(this_m);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (k == STD_ਸਦਿਸ਼_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ਲਈ (size_t i = 0; i < v.size(); i++)
        {
            ਅਚਲ Mat& m = v[i];
            Mat& this_m = this_v[i];
            ਜੇ (this_m.u != ਨਿਰਾ && this_m.u == m.u)
                ਜਾਰੀ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ਮੰਜ਼ਿਲ(this_m);
        }
    }
    ਨਹੀਂਤਾਂ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ਸਥਿਰ ਨਿਵੇਸ਼_Output_Array_2 _none;
ਨਿਵੇਸ਼_Output_Array noArray() { ਵਾਪਸ _none; }

} // cv::
