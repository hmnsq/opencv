// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 5> Vec5i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 7> Vec7i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 9> Vec9i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 10> Vec10i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 11> Vec11i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 12> Vec12i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 13> Vec13i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 14> Vec14i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 15> Vec15i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 16> Vec16i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 32> Vec32i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 64> Vec64i;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Vec<ಪೂರ್ಣಾಂಕ, 128> Vec128i;

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು STD_ಸದಿಶ_SWITCH
#ವ್ಯಾಖ್ಯಾನಿಸು STD_ಸದಿಶ_SWITCH(esz, func) \
    ಆಯ್ಕೆ( esz ) \
    { \
    ಸ್ಥಿತಿ 1: func(uchar); \
    ಸ್ಥಿತಿ 2: func(Vec2b); \
    ಸ್ಥಿತಿ 3: func(Vec3b); \
    ಸ್ಥಿತಿ 4: func(ಪೂರ್ಣಾಂಕ); \
    ಸ್ಥಿತಿ 6: func(Vec3s); \
    ಸ್ಥಿತಿ 8: func(Vec2i); \
    ಸ್ಥಿತಿ 12: func(Vec3i); \
    ಸ್ಥಿತಿ 16: func(Vec4i); \
    ಸ್ಥಿತಿ 20: func(Vec5i); \
    ಸ್ಥಿತಿ 24: func(Vec6i); \
    ಸ್ಥಿತಿ 28: func(Vec7i); \
    ಸ್ಥಿತಿ 32: func(Vec8i); \
    ಸ್ಥಿತಿ 36: func(Vec9i); \
    ಸ್ಥಿತಿ 40: func(Vec10i); \
    ಸ್ಥಿತಿ 44: func(Vec11i); \
    ಸ್ಥಿತಿ 48: func(Vec12i); \
    ಸ್ಥಿತಿ 52: func(Vec13i); \
    ಸ್ಥಿತಿ 56: func(Vec14i); \
    ಸ್ಥಿತಿ 60: func(Vec15i); \
    ಸ್ಥಿತಿ 64: func(Vec16i); \
    ಸ್ಥಿತಿ 128: func(Vec32i); \
    ಸ್ಥಿತಿ 256: func(Vec64i); \
    ಸ್ಥಿತಿ 512: func(Vec128i); \
    ಪೂರ್ವನಿಯೋಜಿತ: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat ಒಳಹರಿವು_Array_2::getMat_(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    AccessFlag accessFlags = flags & ACCESS_ಮುಖವಾಡ;

    ಆದರೆ( k == MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* m = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ *m;
        ಹಿಂತಿರುಗಿ m->ಅಡ್ಡಸಾಲು(i);
    }

    ಆದರೆ( k == UMAT )
    {
        ಸ್ಥಿರಾಂಕ UMat* m = (ಸ್ಥಿರಾಂಕ UMat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ m->getMat(accessFlags);
        ಹಿಂತಿರುಗಿ m->getMat(accessFlags).ಅಡ್ಡಸಾಲು(i);
    }

    ಆದರೆ (k == MATX)
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ Mat(sz, flags, obj);
    }

    ಆದರೆ( k == STD_BOOL_ಸದಿಶ )
    {
        CV_Assert( i < 0 );
        ಪೂರ್ಣಾಂಕ t = CV_8U;
        ಸ್ಥಿರಾಂಕ std::vector<ತಾರ್ಕಿಕ>& v = *(ಸ್ಥಿರಾಂಕ std::vector<ತಾರ್ಕಿಕ>*)obj;
        ಪೂರ್ಣಾಂಕ j, n = (ಪೂರ್ಣಾಂಕ)v.size();
        ಆದರೆ( n == 0 )
            ಹಿಂತಿರುಗಿ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.ದತ್ತಾಂಶ;
        ಗಾಗಿ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ಹಿಂತಿರುಗಿ m;
    }

    ಆದರೆ( k == NONE )
        ಹಿಂತಿರುಗಿ Mat();

    ಆದರೆ( k == STD_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ )
    {
        ಪೂರ್ಣಾಂಕ t = CV_MAT_TYPE(flags);
        ಪೂರ್ಣಾಂಕ esz = CV_ELEM_ಗಾತ್ರ(t);

    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು GET_MAT
    #ವ್ಯಾಖ್ಯಾನಿಸು GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ಆದರೆ (k == STD_ಸದಿಶ_ಸದಿಶ) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ಇಲ್ಲದಿದ್ದರೆ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ಪೂರ್ಣಾಂಕ v_sz = ಪೂರ್ಣಾಂಕ(v->size()); \
            ಹಿಂತಿರುಗಿ v->empty() ? Mat() : Mat(1, &v_sz, t, v->ದತ್ತಾಂಶ()); \
        }

        STD_ಸದಿಶ_SWITCH(esz, GET_MAT)
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ಪೂರ್ಣಾಂಕ)v.size() );

        ಹಿಂತಿರುಗಿ v[i];
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* v = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert( 0 <= i && i < sz.ಎತ್ತರ );

        ಹಿಂತಿರುಗಿ v[i];
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ಪೂರ್ಣಾಂಕ)v.size() );

        ಹಿಂತಿರುಗಿ v[i].getMat(accessFlags);
    }

    ಆದರೆ( k == OPENGL_ಬಫರ್ )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ಆದರೆ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ಆದರೆ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ಸ್ಥಿರಾಂಕ cuda::HostMem* cuda_mem = (ಸ್ಥಿರಾಂಕ cuda::HostMem*)obj;

        ಹಿಂತಿರುಗಿ cuda_mem->create_Mat_ಶೀರ್ಷಿಕೆ();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat ಒಳಹರಿವು_Array_2::getUMat(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    AccessFlag accessFlags = flags & ACCESS_ಮುಖವಾಡ;

    ಆದರೆ( k == UMAT )
    {
        ಸ್ಥಿರಾಂಕ UMat* m = (ಸ್ಥಿರಾಂಕ UMat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ *m;
        ಹಿಂತಿರುಗಿ m->ಅಡ್ಡಸಾಲು(i);
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ಪೂರ್ಣಾಂಕ)v.size() );

        ಹಿಂತಿರುಗಿ v[i];
    }

    ಆದರೆ( k == MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* m = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ m->getUMat(accessFlags);
        ಹಿಂತಿರುಗಿ m->ಅಡ್ಡಸಾಲು(i).getUMat(accessFlags);
    }

    ಹಿಂತಿರುಗಿ getMat(i).getUMat(accessFlags);
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::get_Mat_ಸದಿಶ(std::vector<Mat>& mv) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    AccessFlag accessFlags = flags & ACCESS_ಮುಖವಾಡ;

    ಆದರೆ( k == MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat& m = *(ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m.type(), (ಶೂನ್ಯ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (ಶೂನ್ಯ*)m.ptr(i), &m.ಹೆಜ್ಜೆ[1]);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (k == MATX)
    {
        size_t n = sz.ಎತ್ತರ, esz = CV_ELEM_ಗಾತ್ರ(flags);
        mv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.ಅಗಲ, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.ಅಗಲ*i);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ )
    {
        ಸ್ಥಿರಾಂಕ std::vector<uchar>& v = *(ಸ್ಥಿರಾಂಕ std::vector<uchar>*)obj;

        size_t n = ಒಟ್ಟು(), esz = CV_ELEM_ಗಾತ್ರ(flags);
        ಪೂರ್ಣಾಂಕ t = CV_MAT_ಆಳ(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (ಶೂನ್ಯ*)(v.ದತ್ತಾಂಶ() + esz*i));
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == NONE )
    {
        mv.clear();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_ಸದಿಶ )
    {
        ಪೂರ್ಣಾಂಕ typ = CV_MAT_TYPE(flags);
        ಪೂರ್ಣಾಂಕ esz = CV_ELEM_ಗಾತ್ರ(typ);

        #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು GET_VEC_VEC_MAT_VEC
        #ವ್ಯಾಖ್ಯಾನಿಸು GET_VEC_VEC_MAT_VEC(T) \
        { \
            ಸ್ಥಿರಾಂಕ std::vector<std::vector<T > >* vv = (ಸ್ಥಿರಾಂಕ std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            ಗಾಗಿ (size_t i = 0; i < n; i++) { \
                ಸ್ಥಿರಾಂಕ std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ); \
                ಪೂರ್ಣಾಂಕ ni = (ಪೂರ್ಣಾಂಕ)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (ಶೂನ್ಯ*)vi.ದತ್ತಾಂಶ()) : Mat(); \
            } \
        } \
        ನಿಲ್ಲಿಸು

        STD_ಸದಿಶ_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* v = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        size_t n = sz.ಎತ್ತರ;
        mv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ಹಿಂತಿರುಗಿ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::get_UMat_ಸದಿಶ(std::vector<UMat>& umv) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    AccessFlag accessFlags = flags & ACCESS_ಮುಖವಾಡ;

    ಆದರೆ( k == NONE )
    {
        umv.clear();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* v = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        size_t n = sz.ಎತ್ತರ;
        umv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& v = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ಗಾಗಿ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ಹಿಂತಿರುಗಿ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat ಒಳಹರಿವು_Array_2::getGpuMat() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ (k == CUDA_GPU_MAT)
    {
        ಸ್ಥಿರಾಂಕ cuda::GpuMat* d_mat = (ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj;
        ಹಿಂತಿರುಗಿ *d_mat;
    }

    ಆದರೆ (k == CUDA_HOST_MEM)
    {
        ಸ್ಥಿರಾಂಕ cuda::HostMem* cuda_mem = (ಸ್ಥಿರಾಂಕ cuda::HostMem*)obj;
        ಹಿಂತಿರುಗಿ cuda_mem->create_Gpu_Mat_ಶೀರ್ಷಿಕೆ();
    }

    ಆದರೆ (k == OPENGL_ಬಫರ್)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ಆದರೆ (k == NONE)
        ಹಿಂತಿರುಗಿ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
}
ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::get_Gpu_Mat_ಸದಿಶ(std::vector<cuda::GpuMat>& gpumv) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
}
ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
}
cuda::GpuMatND ಒಳಹರಿವು_Array_2::getGpuMatND() ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ (k == CUDA_GPU_MATND)
    {
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND* d_mat = (ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj;
        ಹಿಂತಿರುಗಿ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ಇಲ್ಲದಿದ್ದರೆ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
}
ogl::ಬಫರ್_2 ಒಳಹರಿವು_Array_2::get_OGl_ಬಫರ್() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    CV_Assert(k == OPENGL_ಬಫರ್);

    ಸ್ಥಿರಾಂಕ ogl::ಬಫರ್_2* gl_buf = (ಸ್ಥಿರಾಂಕ ogl::ಬಫರ್_2*)obj;
    ಹಿಂತಿರುಗಿ *gl_buf;
}

ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_Array_2::ಅಡ್ಡಸಾಲುಗಳು_2(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == CUDA_GPU_MATND)
    {
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& _gpuMatND = *(ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj;
        ಹಿಂತಿರುಗಿ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ಗಾತ್ರ_4(i).ಎತ್ತರ;
}

ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_Array_2::ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_2(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == CUDA_GPU_MATND)
    {
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& _gpuMatND = *(ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj;
        ಹಿಂತಿರುಗಿ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಹಿಂತಿರುಗಿ ಗಾತ್ರ_4(i).ಅಗಲ;
}

ಗಾತ್ರ_2 ಒಳಹರಿವು_Array_2::ಗಾತ್ರ_4(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ Mat* m = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert(m->dims <= 2);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m->ಅಡ್ಡಸಾಲುಗಳು);
    }

    ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ UMat* m = (ಸ್ಥಿರಾಂಕ UMat*)obj;
        CV_Assert(m->dims <= 2);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, m->ಅಡ್ಡಸಾಲುಗಳು);
    }

    ಆದರೆ (k == MATX)
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ sz;
    }

    ಆದರೆ( k == NONE )
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2();

    ಆದರೆ( k == STD_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
    {
        size_t n = ಒಟ್ಟು(i);
        CV_Assert(n <= (size_t)ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)n, 1);
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ vv.empty() ? ಗಾತ್ರ_2() : ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)vv.size(), 1);
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );

        ಹಿಂತಿರುಗಿ vv[i].size();
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ sz.ಎತ್ತರ==0 ? ಗಾತ್ರ_2() : ಗಾತ್ರ_2(sz.ಎತ್ತರ, 1);
        CV_Assert( i < sz.ಎತ್ತರ );

        ಹಿಂತಿರುಗಿ vv[i].size();
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>*)obj;
        ಆದರೆ (i < 0)
            ಹಿಂತಿರುಗಿ vv.empty() ? ಗಾತ್ರ_2() : ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)vv.size(), 1);
        CV_Assert(i < (ಪೂರ್ಣಾಂಕ)vv.size());
        ಹಿಂತಿರುಗಿ vv[i].size();
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>*)obj;
        ಆದರೆ (i < 0)
            ಹಿಂತಿರುಗಿ vv.empty() ? ಗಾತ್ರ_2() : ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)vv.size(), 1);
        CV_Assert(i < (ಪೂರ್ಣಾಂಕ)vv.size());
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& m = vv[i];
        ಆದರೆ (m.dims == 0)
            ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2();
        ಆದರೆ (m.dims == 1)
            ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(1, m.size[0]);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(m.size[1], m.size[0]);
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ vv.empty() ? ಗಾತ್ರ_2() : ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)vv.size(), 1);
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );

        ಹಿಂತಿರುಗಿ vv[i].size();
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>*)obj;
        ಆದರೆ (i < 0)
            ಹಿಂತಿರುಗಿ vv.empty() ? ಗಾತ್ರ_2() : ಗಾತ್ರ_2((ಪೂರ್ಣಾಂಕ)vv.size(), 1);
        CV_Assert(i < (ಪೂರ್ಣಾಂಕ)vv.size());
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& m = vv[i];
        ಆದರೆ (m.dims == 0)
            ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2();
        ಆದರೆ (m.dims == 1)
            ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(1, m.size[0]);
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2(m.size[1], m.size[0]);
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == OPENGL_ಬಫರ್ )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ ogl::ಬಫರ್_2* buf = (ಸ್ಥಿರಾಂಕ ogl::ಬಫರ್_2*)obj;
        ಹಿಂತಿರುಗಿ buf->size();
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ cuda::GpuMat* d_mat = (ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj;
        ಹಿಂತಿರುಗಿ d_mat->size();
    }

    ಆದರೆ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ cuda::HostMem* cuda_mem = (ಸ್ಥಿರಾಂಕ cuda::HostMem*)obj;
        ಹಿಂತಿರುಗಿ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_Array_2::sizend(ಪೂರ್ಣಾಂಕ* arrsz, ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ j, d = 0;
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == NONE )
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ Mat& m = *(ಸ್ಥಿರಾಂಕ Mat*)obj;
        d = m.dims;
        ಆದರೆ(arrsz)
            ಗಾಗಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ UMat& m = *(ಸ್ಥಿರಾಂಕ UMat*)obj;
        d = m.dims;
        ಆದರೆ(arrsz)
            ಗಾಗಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == STD_ಸದಿಶ_MAT && i >= 0 )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಸ್ಥಿರಾಂಕ Mat& m = vv[i];
        d = m.dims;
        ಆದರೆ(arrsz)
            ಗಾಗಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert( i < sz.ಎತ್ತರ );
        ಸ್ಥಿರಾಂಕ Mat& m = vv[i];
        d = m.dims;
        ಆದರೆ(arrsz)
            ಗಾಗಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == STD_ಸದಿಶ_UMAT && i >= 0 )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಸ್ಥಿರಾಂಕ UMat& m = vv[i];
        d = m.dims;
        ಆದರೆ(arrsz)
            ಗಾಗಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (((k == STD_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ) && i < 0) ||
             (k == STD_ಸದಿಶ_ಸದಿಶ && i >= 0))
    {
        size_t n = ಒಟ್ಟು(i);
        d = 1;
        ಆದರೆ(arrsz)
        {
            arrsz[0] = ಪೂರ್ಣಾಂಕ(n);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        ಗಾತ್ರ_2 sz2d = size(i);
        d = 2;
        ಆದರೆ(arrsz)
        {
            arrsz[0] = sz2d.ಎತ್ತರ;
            arrsz[1] = sz2d.ಅಗಲ;
        }
    }

    ಹಿಂತಿರುಗಿ d;
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::empty(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (i >= 0) {
        ಆದರೆ (k == STD_ಸದಿಶ_MAT) {
            ಸ್ವಯಂಚಾಲಿತ mv = reinterpret_cast<ಸ್ಥಿರಾಂಕ std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ಹಿಂತಿರುಗಿ mv->at(i).empty();
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == STD_ಸದಿಶ_UMAT) {
            ಸ್ವಯಂಚಾಲಿತ umv = reinterpret_cast<ಸ್ಥಿರಾಂಕ std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ಹಿಂತಿರುಗಿ umv->at(i).empty();
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == STD_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ) {
            size_t n = ಒಟ್ಟು(i);
            ಹಿಂತಿರುಗಿ n == 0;
        } ಇಲ್ಲದಿದ್ದರೆ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ಹಿಂತಿರುಗಿ empty();
}

MatShape ಒಳಹರಿವು_Array_2::shape(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    MatShape shape;

    ಆದರೆ( k == NONE )
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ Mat& m = *(ಸ್ಥಿರಾಂಕ Mat*)obj;
        shape = m.size;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ UMat& m = *(ಸ್ಥಿರಾಂಕ UMat*)obj;
        shape = m.size;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == STD_ಸದಿಶ_MAT && i >= 0 )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ಸ್ಥಿರಾಂಕ Mat& m = vv[i];
        shape = m.size;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert( i < sz.ಎತ್ತರ );
        ಸ್ಥಿರಾಂಕ Mat& m = vv[i];
        shape = m.size;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == STD_ಸದಿಶ_UMAT && i >= 0 )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ಸ್ಥಿರಾಂಕ UMat& m = vv[i];
        shape = m.size;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಪೂರ್ಣಾಂಕ sizes[CV_MAX_DIM];
        ಪೂರ್ಣಾಂಕ dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ಹಿಂತಿರುಗಿ shape;
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::same_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& arr) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k1 = ಬಗೆ(), k2 = arr.ಬಗೆ();
    ಗಾತ್ರ_2 sz1;

    ಆದರೆ( k1 == MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* m = ((ಸ್ಥಿರಾಂಕ Mat*)obj);
        ಆದರೆ( k2 == MAT )
            ಹಿಂತಿರುಗಿ m->size == ((ಸ್ಥಿರಾಂಕ Mat*)arr.obj)->size;
        ಆದರೆ( k2 == UMAT )
            ಹಿಂತಿರುಗಿ m->size == ((ಸ್ಥಿರಾಂಕ UMat*)arr.obj)->size;
        ಆದರೆ( m->dims > 2 )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        sz1 = m->size();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k1 == UMAT )
    {
        ಸ್ಥಿರಾಂಕ UMat* m = ((ಸ್ಥಿರಾಂಕ UMat*)obj);
        ಆದರೆ( k2 == MAT )
            ಹಿಂತಿರುಗಿ m->size == ((ಸ್ಥಿರಾಂಕ Mat*)arr.obj)->size;
        ಆದರೆ( k2 == UMAT )
            ಹಿಂತಿರುಗಿ m->size == ((ಸ್ಥಿರಾಂಕ UMat*)arr.obj)->size;
        ಆದರೆ( m->dims > 2 )
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        sz1 = m->size();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj)->size == ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)arr.obj)->size;
    }
    ಇಲ್ಲದಿದ್ದರೆ
        sz1 = size();

    ಆದರೆ( arr.dims() > 2 )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ sz1 == arr.size();
}

ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_Array_2::dims_(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ Mat*)obj)->dims;
    }

    ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ UMat*)obj)->dims;
    }

    ಆದರೆ (k == MATX)
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ 2;
    }

    ಆದರೆ( k == STD_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ( k == NONE )
        ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ( k == STD_ಸದಿಶ_ಸದಿಶ )
    {
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ 1;
        CV_Assert( size_t(i) < ಒಟ್ಟು() );
        ಹಿಂತಿರುಗಿ 1;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ 1;
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );

        ಹಿಂತಿರುಗಿ vv[i].dims;
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ 1;
        CV_Assert( i < sz.ಎತ್ತರ );

        ಹಿಂತಿರುಗಿ vv[i].dims;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ 1;
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );

        ಹಿಂತಿರುಗಿ vv[i].dims;
    }

    ಆದರೆ( k == OPENGL_ಬಫರ್ )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ 2;
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ 2;
    }

    ಆದರೆ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj)->dims;
    }

    ಆದರೆ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t ಒಳಹರಿವು_Array_2::ಒಟ್ಟು(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ Mat*)obj)->ಒಟ್ಟು();
    }

    ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ UMat*)obj)->ಒಟ್ಟು();
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ vv.size();

        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಹಿಂತಿರುಗಿ vv[i].ಒಟ್ಟು();
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ sz.ಎತ್ತರ;

        CV_Assert( i < sz.ಎತ್ತರ );
        ಹಿಂತಿರುಗಿ vv[i].ಒಟ್ಟು();
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        ಆದರೆ( i < 0 )
            ಹಿಂತಿರುಗಿ vv.size();

        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಹಿಂತಿರುಗಿ vv[i].ಒಟ್ಟು();
    }

    ಆದರೆ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& _gpuMatND = *((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj);
        ಆದರೆ (_gpuMatND.dims > 0)
        {
            res = 1;
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ಹಿಂತಿರುಗಿ res;
        }
    }

    ಆದರೆ (k == STD_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ)
    {
        CV_Assert(i < 0 || k == STD_ಸದಿಶ_ಸದಿಶ);
        ಪೂರ್ಣಾಂಕ esz = CV_ELEM_ಗಾತ್ರ(flags);

        #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು GET_VEC_ಗಾತ್ರ
        #ವ್ಯಾಖ್ಯಾನಿಸು GET_VEC_ಗಾತ್ರ(T) \
            ಆದರೆ (k == STD_ಸದಿಶ_ಸದಿಶ) { \
                ಸ್ಥಿರಾಂಕ std::vector<std::vector<T > >* vv = (ಸ್ಥಿರಾಂಕ std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ಆದರೆ (i < 0) \
                    ಹಿಂತಿರುಗಿ n; \
                CV_Assert((size_t)i < n); \
                ಹಿಂತಿರುಗಿ vv->at(i).size(); \
            } ಇಲ್ಲದಿದ್ದರೆ \
                ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ std::vector<T >*)obj)->size()

        STD_ಸದಿಶ_SWITCH(esz, GET_VEC_ಗಾತ್ರ)
    }

    ಹಿಂತಿರುಗಿ size(i).area();
}

ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_Array_2::type_(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ Mat*)obj)->type();

    ಆದರೆ( k == UMAT )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ UMat*)obj)->type();

    ಆದರೆ( k == MATX || k == STD_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
        ಹಿಂತಿರುಗಿ CV_MAT_TYPE(flags);

    ಆದರೆ( k == NONE )
        ಹಿಂತಿರುಗಿ -1;

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        ಆದರೆ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಹಿಂತಿರುಗಿ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಹಿಂತಿರುಗಿ vv[i >= 0 ? i : 0].type();
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        ಆದರೆ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಹಿಂತಿರುಗಿ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಹಿಂತಿರುಗಿ vv[i >= 0 ? i : 0].type();
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        ಆದರೆ( sz.ಎತ್ತರ == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಹಿಂತಿರುಗಿ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.ಎತ್ತರ );
        ಹಿಂತಿರುಗಿ vv[i >= 0 ? i : 0].type();
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>*)obj;
        ಆದರೆ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಹಿಂತಿರುಗಿ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ಪೂರ್ಣಾಂಕ)vv.size());
        ಹಿಂತಿರುಗಿ vv[i >= 0 ? i : 0].type();
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>*)obj;
        ಆದರೆ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಹಿಂತಿರುಗಿ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ಪೂರ್ಣಾಂಕ)vv.size());
        ಹಿಂತಿರುಗಿ vv[i >= 0 ? i : 0].type();
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == OPENGL_ಬಫರ್ )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ ogl::ಬಫರ್_2*)obj)->type();

    ಆದರೆ( k == CUDA_GPU_MAT )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj)->type();

    ಆದರೆ( k == CUDA_GPU_MATND )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj)->type();

    ಆದರೆ( k == CUDA_HOST_MEM )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಪೂರ್ಣಾಂಕ ಒಳಹರಿವು_Array_2::ಆಳ_4(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ CV_MAT_ಆಳ(type(i));
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::ಖಾಲಿಗಳು() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ Mat*)obj)->empty();

    ಆದರೆ( k == UMAT )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ UMat*)obj)->empty();

    ಆದರೆ (k == MATX)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ( k == STD_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ )
    {
        size_t n = ಒಟ್ಟು(-1);
        ಹಿಂತಿರುಗಿ n == 0;
    }

    ಆದರೆ( k == NONE )
        ಹಿಂತಿರುಗಿ ನಿಜ;

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        ಹಿಂತಿರುಗಿ vv.empty();
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಹಿಂತಿರುಗಿ sz.ಎತ್ತರ == 0;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        ಹಿಂತಿರುಗಿ vv.empty();
    }

    ಆದರೆ( k == OPENGL_ಬಫರ್ )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ ogl::ಬಫರ್_2*)obj)->empty();

    ಆದರೆ( k == CUDA_GPU_MAT )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj)->empty();

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>*)obj;
        ಹಿಂತಿರುಗಿ vv.empty();
    }

    ಆದರೆ( k == CUDA_GPU_MATND )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj)->empty();

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>*)obj;
        ಹಿಂತಿರುಗಿ vv.empty();
    }

    ಆದರೆ( k == CUDA_HOST_MEM )
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::isContinuous(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
        ಹಿಂತಿರುಗಿ i < 0 ? ((ಸ್ಥಿರಾಂಕ Mat*)obj)->isContinuous() : ನಿಜ;

    ಆದರೆ( k == UMAT )
        ಹಿಂತಿರುಗಿ i < 0 ? ((ಸ್ಥಿರಾಂಕ UMat*)obj)->isContinuous() : ನಿಜ;

    ಆದರೆ( k == MATX || k == STD_ಸದಿಶ ||
        k == NONE || k == STD_ಸದಿಶ_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
        ಹಿಂತಿರುಗಿ ನಿಜ;

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].isContinuous();
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.ಎತ್ತರ);
        ಹಿಂತಿರುಗಿ vv[i].isContinuous();
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].isContinuous();
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
      ಹಿಂತಿರುಗಿ i < 0 ? ((ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj)->isContinuous() : ನಿಜ;

    ಆದರೆ( k == CUDA_GPU_MATND )
        ಹಿಂತಿರುಗಿ i < 0 ? ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj)->isContinuous() : ನಿಜ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::isSubmatrix(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
        ಹಿಂತಿರುಗಿ i < 0 ? ((ಸ್ಥಿರಾಂಕ Mat*)obj)->isSubmatrix() : ಸುಳ್ಳು;

    ಆದರೆ( k == UMAT )
        ಹಿಂತಿರುಗಿ i < 0 ? ((ಸ್ಥಿರಾಂಕ UMat*)obj)->isSubmatrix() : ಸುಳ್ಳು;

    ಆದರೆ( k == MATX || k == STD_ಸದಿಶ ||
        k == NONE || k == STD_ಸದಿಶ_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].isSubmatrix();
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.ಎತ್ತರ);
        ಹಿಂತಿರುಗಿ vv[i].isSubmatrix();
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].isSubmatrix();
    }

    ಆದರೆ( k == CUDA_GPU_MATND )
    {
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t ಒಳಹರಿವು_Array_2::offset(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ Mat * ಸ್ಥಿರಾಂಕ m = ((ಸ್ಥಿರಾಂಕ Mat*)obj);
        ಹಿಂತಿರುಗಿ (size_t)(m->ptr() - m->datastart);
    }

    ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ UMat*)obj)->offset;
    }

    ಆದರೆ( k == MATX || k == STD_ಸದಿಶ ||
        k == NONE || k == STD_ಸದಿಶ_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
        ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ಪೂರ್ಣಾಂಕ)vv.size() );

        ಹಿಂತಿರುಗಿ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.ಎತ್ತರ );
        ಹಿಂತಿರುಗಿ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].offset;
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಸ್ಥಿರಾಂಕ cuda::GpuMat * ಸ್ಥಿರಾಂಕ m = ((ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj);
        ಹಿಂತಿರುಗಿ (size_t)(m->ದತ್ತಾಂಶ - m->datastart);
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ (size_t)(vv[i].ದತ್ತಾಂಶ - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t ಒಳಹರಿವು_Array_2::ಹೆಜ್ಜೆ(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ Mat*)obj)->ಹೆಜ್ಜೆ;
    }

    ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ UMat*)obj)->ಹೆಜ್ಜೆ;
    }

    ಆದರೆ( k == MATX || k == STD_ಸದಿಶ ||
        k == NONE || k == STD_ಸದಿಶ_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
        ಹಿಂತಿರುಗಿ 0;

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<Mat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ಪೂರ್ಣಾಂಕ)vv.size() );
        ಹಿಂತಿರುಗಿ vv[i].ಹೆಜ್ಜೆ;
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        ಸ್ಥಿರಾಂಕ Mat* vv = (ಸ್ಥಿರಾಂಕ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.ಎತ್ತರ );
        ಹಿಂತಿರುಗಿ vv[i].ಹೆಜ್ಜೆ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ಸ್ಥಿರಾಂಕ std::vector<UMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].ಹೆಜ್ಜೆ;
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಹಿಂತಿರುಗಿ ((ಸ್ಥಿರಾಂಕ cuda::GpuMat*)obj)->ಹೆಜ್ಜೆ;
    }
    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಹಿಂತಿರುಗಿ vv[i].ಹೆಜ್ಜೆ;
    }
    ಆದರೆ( k == CUDA_GPU_MATND )
    {
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& _gpuMatND = *(ಸ್ಥಿರಾಂಕ cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ಹಿಂತಿರುಗಿ _gpuMatND.ಹೆಜ್ಜೆ[i];
    }

    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND)
    {
        ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>& vv = *(ಸ್ಥಿರಾಂಕ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಸ್ಥಿರಾಂಕ cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ಹಿಂತಿರುಗಿ _gpuMatND.ಹೆಜ್ಜೆ[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::copy_ಗಮ್ಯ(ಸ್ಥಿರಾಂಕ _OutputArray& arr) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == NONE )
        arr.release();
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == MAT || k == MATX || k == STD_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
    {
        Mat m = getMat();
        m.copy_ಗಮ್ಯ(arr);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == UMAT )
        ((UMat*)obj)->copy_ಗಮ್ಯ(arr);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_ಗಮ್ಯ(arr);
#ಆದರೆ_ಅಂತ್ಯ
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "");
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::copy_ಗಮ್ಯ(ಸ್ಥಿರಾಂಕ _OutputArray& arr, ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2 & ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == NONE )
        arr.release();
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == MAT || k == MATX || k == STD_ಸದಿಶ || k == STD_BOOL_ಸದಿಶ )
    {
        Mat m = getMat();
        m.copy_ಗಮ್ಯ(arr, ಮುಖವಾಡ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == UMAT )
        ((UMat*)obj)->copy_ಗಮ್ಯ(arr, ಮುಖವಾಡ);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_ಗಮ್ಯ(arr, ಮುಖವಾಡ);
#ಆದರೆ_ಅಂತ್ಯ
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "");
}

ತಾರ್ಕಿಕ _OutputArray::fixed_ಗಾತ್ರ() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ (flags & FIXED_ಗಾತ್ರ) == FIXED_ಗಾತ್ರ;
}

ತಾರ್ಕಿಕ _OutputArray::fixedType() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ (flags & FIXED_TYPE) == FIXED_TYPE;
}

ಶೂನ್ಯ _OutputArray::create_(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i, ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ( k == MAT && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((Mat*)obj)->size.ಕ್ರಿಯಾಕಾರಕ()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ( k == UMAT && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((UMat*)obj)->size.ಕ್ರಿಯಾಕಾರಕ()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.ಎತ್ತರ) && (((cuda::GpuMatND*)obj)->size[1] == _sz.ಅಗಲ)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        cuda::GpuMatND::ಗಾತ್ರ_Array sizes({_sz.ಎತ್ತರ, _sz.ಅಗಲ});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ( k == OPENGL_ಬಫರ್ && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((ogl::ಬಫರ್_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::ಬಫರ್_2*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
        ((ogl::ಬಫರ್_2*)obj)->create(_sz, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಪೂರ್ಣಾಂಕ sizes[] = {_sz.ಎತ್ತರ, _sz.ಅಗಲ};
    create(2, sizes, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
}

ಶೂನ್ಯ _OutputArray::create_(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು_3, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i, ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ( k == MAT && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((Mat*)obj)->size.ಕ್ರಿಯಾಕಾರಕ()() == ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಅಡ್ಡಸಾಲುಗಳು_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, mtype);
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ( k == UMAT && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((UMat*)obj)->size.ಕ್ರಿಯಾಕಾರಕ()() == ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಅಡ್ಡಸಾಲುಗಳು_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, mtype);
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((cuda::GpuMat*)obj)->size() == ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಅಡ್ಡಸಾಲುಗಳು_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == ಅಡ್ಡಸಾಲುಗಳು_3) && (((cuda::GpuMatND*)obj)->size[1] == ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        cuda::GpuMatND::ಗಾತ್ರ_Array sizes({ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ( k == OPENGL_ಬಫರ್ && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((ogl::ಬಫರ್_2*)obj)->size() == ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಅಡ್ಡಸಾಲುಗಳು_3));
        CV_Assert(!fixedType() || ((ogl::ಬಫರ್_2*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
        ((ogl::ಬಫರ್_2*)obj)->create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಆದರೆ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
        CV_Assert(!fixed_ಗಾತ್ರ() || ((cuda::HostMem*)obj)->size() == ಗಾತ್ರ_2(ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, ಅಡ್ಡಸಾಲುಗಳು_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::HostMem*)obj)->create(ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಪೂರ್ಣಾಂಕ sizes[] = {ಅಡ್ಡಸಾಲುಗಳು_3, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
}

ಶೂನ್ಯ _OutputArray::create(ಪೂರ್ಣಾಂಕ d, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i,
                          ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    mtype = CV_MAT_TYPE(mtype);

    ಆದರೆ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_ಗಾತ್ರ()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ಆದರೆ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.ಅಡ್ಡಸಾಲುಗಳು == size0 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size1) ||
            (allowTransposed && m.ಅಡ್ಡಸಾಲುಗಳು == size1 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size0 && m.isContinuous())))
        {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_ಆಳ(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m.type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_ಗಾತ್ರ()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ಆದರೆ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.ಅಡ್ಡಸಾಲುಗಳು == size0 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size1) ||
            (allowTransposed && m.ಅಡ್ಡಸಾಲುಗಳು == size1 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size0 && m.isContinuous())))
        {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m.type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == MATX )
    {
        CV_Assert( i < 0 );
        ಪೂರ್ಣಾಂಕ type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_ಆಳ_ಮುಖವಾಡ) != 0) );
        CV_CheckLE(d, 2, "");
        ಗಾತ್ರ_2 requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ಆದರೆ (sz.ಅಗಲ == 1 || sz.ಎತ್ತರ == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ಪೂರ್ಣಾಂಕ ಒಟ್ಟು_1d = std::max(sz.ಅಗಲ, sz.ಎತ್ತರ);
            CV_Check(requested_size, std::max(requested_size.ಅಗಲ, requested_size.ಎತ್ತರ) == ಒಟ್ಟು_1d, "");
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.ಎತ್ತರ == sz.ಅಗಲ && requested_size.ಅಗಲ == sz.ಎತ್ತರ)),
                        "");
            }
        }
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ )
    {
        CV_Assert(!fixed_ಗಾತ್ರ());
        CV_Assert(k == STD_ಸದಿಶ_ಸದಿಶ || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ಪೂರ್ಣಾಂಕ esz = CV_ELEM_ಗಾತ್ರ(flags);

        ಆದರೆ( k == STD_ಸದಿಶ || (k == STD_ಸದಿಶ_ಸದಿಶ && i >= 0) )
        {
            ಪೂರ್ಣಾಂಕ type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_ಆಳ_ಮುಖವಾಡ) != 0) );
        }

    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು RESIZE_VEC
    #ವ್ಯಾಖ್ಯಾನಿಸು RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ಆದರೆ (k == STD_ಸದಿಶ_ಸದಿಶ) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ಆದರೆ (i < 0) { \
                    CV_Assert(!fixed_ಗಾತ್ರ() || len == vv->size()); \
                    vv->resize(len); \
                    ಹಿಂತಿರುಗಿ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ಇಲ್ಲದಿದ್ದರೆ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_ಗಾತ್ರ() || len == v->size()); \
            v->resize(len); \
        } \
        ನಿಲ್ಲಿಸು

        STD_ಸದಿಶ_SWITCH(esz, RESIZE_VEC)
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ಆದರೆ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_ಗಾತ್ರ() || len == len0);
            v.resize(len);
            ಆದರೆ( fixedType() )
            {
                ಪೂರ್ಣಾಂಕ _type = CV_MAT_TYPE(flags);
                ಗಾಗಿ( size_t j = len0; j < len; j++ )
                {
                    ಆದರೆ( v[j].type() == _type )
                        ಮುಂದುವರಿ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ಮುಖವಾಡ) | _type;
                }
            }
            ಹಿಂತಿರುಗಿ;
        }

        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)v.size() );
        Mat& m = v[i];

        ಆದರೆ( allowTransposed )
        {
            ಆದರೆ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_ಗಾತ್ರ());
                m.release();
            }

            ಆದರೆ( d == 2 && m.dims == 2 && m.ದತ್ತಾಂಶ &&
                m.type() == mtype && m.ಅಡ್ಡಸಾಲುಗಳು == sizes[1] && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == sizes[0] )
                ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m.type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_Assert(m.dims == d);
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ಆದರೆ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.ಎತ್ತರ;

            CV_Assert(len == len0);
            ಆದರೆ( fixedType() )
            {
                ಪೂರ್ಣಾಂಕ _type = CV_MAT_TYPE(flags);
                ಗಾಗಿ( size_t j = len0; j < len; j++ )
                {
                    ಆದರೆ( v[j].type() == _type )
                        ಮುಂದುವರಿ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ಮುಖವಾಡ) | _type;
                }
            }
            ಹಿಂತಿರುಗಿ;
        }

        CV_Assert( i < sz.ಎತ್ತರ );
        Mat& m = v[i];

        ಆದರೆ( allowTransposed )
        {
            ಆದರೆ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_ಗಾತ್ರ());
                m.release();
            }

            ಆದರೆ( d == 2 && m.dims == 2 && m.ದತ್ತಾಂಶ &&
                m.type() == mtype && m.ಅಡ್ಡಸಾಲುಗಳು == sizes[1] && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == sizes[0] )
                ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m.type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_Assert(m.dims == d);
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ಆದರೆ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_ಗಾತ್ರ() || len == len0);
            v.resize(len);
            ಆದರೆ( fixedType() )
            {
                ಪೂರ್ಣಾಂಕ _type = CV_MAT_TYPE(flags);
                ಗಾಗಿ( size_t j = len0; j < len; j++ )
                {
                    ಆದರೆ( v[j].type() == _type )
                        ಮುಂದುವರಿ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ಮುಖವಾಡ) | _type;
                }
            }
            ಹಿಂತಿರುಗಿ;
        }

        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)v.size() );
        UMat& m = v[i];

        ಆದರೆ( allowTransposed )
        {
            ಆದರೆ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_ಗಾತ್ರ());
                m.release();
            }

            ಆದರೆ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.ಅಡ್ಡಸಾಲುಗಳು == sizes[1] && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == sizes[0] )
                ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m.type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_Assert(m.dims == d);
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_ಆಳ_ಮುಖವಾಡ == 0 )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        cuda::GpuMatND::ಗಾತ್ರ_Array ಗಾತ್ರ_Array_2 = cuda::GpuMatND::ಗಾತ್ರ_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(ಗಾತ್ರ_Array_2, mtype);
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಶೂನ್ಯ _OutputArray::create(ಸ್ಥಿರಾಂಕ MatShape& shape, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i,
                          ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (shape.dims < 0) {
        release();
    } ಇಲ್ಲದಿದ್ದರೆ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
    }
}

Mat _OutputArray::reinterpret(ಪೂರ್ಣಾಂಕ mtype) ಸ್ಥಿರಾಂಕ
{
    mtype = CV_MAT_TYPE(mtype);
    ಹಿಂತಿರುಗಿ getMat().reinterpret(mtype);
}

ಶೂನ್ಯ _OutputArray::create_Same_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& arr, ಪೂರ್ಣಾಂಕ mtype) ಸ್ಥಿರಾಂಕ
{
    create(arr.shape(), mtype);
}

ಶೂನ್ಯ _OutputArray::fit(ಗಾತ್ರ_2 _sz, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ sizes[] = {_sz.ಎತ್ತರ, _sz.ಅಗಲ};
    fit(2, sizes, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
}

ಶೂನ್ಯ _OutputArray::fit(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ sizes[] = {ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ};
    fit(2, sizes, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
}

ಶೂನ್ಯ _OutputArray::fit(ಸ್ಥಿರಾಂಕ MatShape& shape, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (shape.dims < 0) {
        release();
        ಹಿಂತಿರುಗಿ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ ((k == MAT && i < 0) || (k == STD_ಸದಿಶ_MAT && i >= 0)) {
        Mat_ಗಾತ್ರ& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((k == UMAT && i < 0) || (k == STD_ಸದಿಶ_UMAT && i >= 0)) {
        Mat_ಗಾತ್ರ& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

ಶೂನ್ಯ _OutputArray::fit_Same_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& arr, ಪೂರ್ಣಾಂಕ mtype) ಸ್ಥಿರಾಂಕ
{
    fit(arr.shape(), mtype);
}

ಶೂನ್ಯ _OutputArray::fit(ಪೂರ್ಣಾಂಕ d, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* sizes, ಪೂರ್ಣಾಂಕ mtype, ಪೂರ್ಣಾಂಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::ಆಳ_ಮುಖವಾಡ fixed_ಆಳ_ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಪೂರ್ಣಾಂಕ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    mtype = CV_MAT_TYPE(mtype);

    ಆದರೆ( (k == MAT && i < 0) || (k == STD_ಸದಿಶ_MAT && i >= 0) )
    {
        Mat* m;
        ಆದರೆ (k == MAT)
            m = (Mat*)obj;
        ಇಲ್ಲದಿದ್ದರೆ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_ಗಾತ್ರ()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ಆದರೆ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->ಅಡ್ಡಸಾಲುಗಳು == size0 && m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size1) ||
             (allowTransposed && m->ಅಡ್ಡಸಾಲುಗಳು == size1 && m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size0 && m->isContinuous())))
        {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_ಆಳ(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m->type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( (k == UMAT && i < 0) || (k == STD_ಸದಿಶ_UMAT && i >= 0) )
    {
        UMat* m;
        ಆದರೆ (k == UMAT)
            m = (UMat*)obj;
        ಇಲ್ಲದಿದ್ದರೆ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_ಗಾತ್ರ()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ಆದರೆ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->ಅಡ್ಡಸಾಲುಗಳು == size0 && m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size1) ||
             (allowTransposed && m->ಅಡ್ಡಸಾಲುಗಳು == size1 && m->ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == size0 && m->isContinuous())))
        {
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(fixedType())
        {
            ಆದರೆ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_ಆಳ(flags)) & fixed_ಆಳ_ಮುಖವಾಡ) != 0 )
                mtype = m->type();
            ಇಲ್ಲದಿದ್ದರೆ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ಆದರೆ(fixed_ಗಾತ್ರ())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ಗಾಗಿ(ಪೂರ್ಣಾಂಕ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ಹಿಂತಿರುಗಿ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_ಆಳ_ಮುಖವಾಡ);
}

ಶೂನ್ಯ _OutputArray::release() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(!fixed_ಗಾತ್ರ());

    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        ((Mat*)obj)->release();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == CUDA_GPU_MAT )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == CUDA_GPU_MATND )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == CUDA_HOST_MEM )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == OPENGL_ಬಫರ್ )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENGL
        ((ogl::ಬಫರ್_2*)obj)->release();
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#ಆದರೆ_ಅಂತ್ಯ
    }

    ಆದರೆ( k == NONE )
        ಹಿಂತಿರುಗಿ;

    ಆದರೆ( k == STD_ಸದಿಶ || k == STD_ಸದಿಶ_ಸದಿಶ )
    {
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CLEAR_VEC
    #ವ್ಯಾಖ್ಯಾನಿಸು CLEAR_VEC(T) \
        ಆದರೆ (k == STD_ಸದಿಶ_ಸದಿಶ) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ಇಲ್ಲದಿದ್ದರೆ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        ನಿಲ್ಲಿಸು

        ಪೂರ್ಣಾಂಕ esz = CV_ELEM_ಗಾತ್ರ(flags);
        STD_ಸದಿಶ_SWITCH(esz, CLEAR_VEC);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        ಗಾಗಿ( ಪೂರ್ಣಾಂಕ i = 0; i < sz.ಎತ್ತರ; i++ )
            v[i].release();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_BOOL_ಸದಿಶ )
    {
        ((std::vector<ತಾರ್ಕಿಕ>*)obj)->clear();
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ( k == STD_ಸದಿಶ_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ಹಿಂತಿರುಗಿ;
    }
    ಆದರೆ (k == STD_ಸದಿಶ_CUDA_GPU_MAT)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ಹಿಂತಿರುಗಿ;
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಶೂನ್ಯ _OutputArray::clear() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == MAT )
    {
        CV_Assert(!fixed_ಗಾತ್ರ());
        ((Mat*)obj)->resize(0);
        ಹಿಂತಿರುಗಿ;
    }

    release();
}

ತಾರ್ಕಿಕ _OutputArray::needed() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ ಬಗೆ() != NONE;
}

Mat& _OutputArray::getMatRef(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ( i < 0 )
    {
        CV_Assert( k == MAT );
        ಹಿಂತಿರುಗಿ *(Mat*)obj;
    }

    CV_Assert( k == STD_ಸದಿಶ_MAT || k == STD_ARRAY_MAT );

    ಆದರೆ( k == STD_ಸದಿಶ_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)v.size() );
        ಹಿಂತಿರುಗಿ v[i];
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.ಎತ್ತರ );
        ಹಿಂತಿರುಗಿ v[i];
    }
}

UMat& _OutputArray::getUMatRef(ಪೂರ್ಣಾಂಕ i) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ಹಿಂತಿರುಗಿ *(UMat*)obj;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Assert( k == STD_ಸದಿಶ_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಂಕ)v.size() );
        ಹಿಂತಿರುಗಿ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    CV_Assert( k == CUDA_GPU_MAT );
    ಹಿಂತಿರುಗಿ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    CV_Assert(k == STD_ಸದಿಶ_CUDA_GPU_MAT);
    ಹಿಂತಿರುಗಿ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    CV_Assert( k == CUDA_GPU_MATND );
    ಹಿಂತಿರುಗಿ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    CV_Assert(k == STD_ಸದಿಶ_CUDA_GPU_MAT_ND);
    ಹಿಂತಿರುಗಿ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::ಬಫರ್_2& _OutputArray::get_OGl_ಬಫರ್_Ref() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    CV_Assert( k == OPENGL_ಬಫರ್ );
    ಹಿಂತಿರುಗಿ *(ogl::ಬಫರ್_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    CV_Assert( k == CUDA_HOST_MEM );
    ಹಿಂತಿರುಗಿ *(cuda::HostMem*)obj;
}

ಶೂನ್ಯ _OutputArray::ಗಣ_ಗಮ್ಯ(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& arr, ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2 & ಮುಖವಾಡ) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == NONE )
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MAT || k == MATX || k == STD_ಸದಿಶ)
    {
        Mat m = getMat();
        m.ಗಣ_ಗಮ್ಯ(arr, ಮುಖವಾಡ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == UMAT )
        ((UMat*)obj)->ಗಣ_ಗಮ್ಯ(arr, ಮುಖವಾಡ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( k == CUDA_GPU_MAT )
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.ಬಗೆ(), ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->ಗಣ_ಗಮ್ಯ(Scalar(Vec<ದ್ವಿಗುಣ, 4>(value.ptr<ದ್ವಿಗುಣ>())), ಮುಖವಾಡ);
#ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#ಆದರೆ_ಅಂತ್ಯ
    }
    ಇಲ್ಲದಿದ್ದರೆ
        CV_Error(Error::StsNotImplemented, "");
}

ಶೂನ್ಯ _OutputArray::ಗಣ_Zero() ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();

    ಆದರೆ( k == NONE )
        ;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MAT || k == MATX || k == STD_ಸದಿಶ)
    {
        Mat m = getMat();
        m.ಗಣ_Zero();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಗಣ_ಗಮ್ಯ(Scalar::all(0), noArray());
    }
}

ಶೂನ್ಯ _OutputArray::assign(ಸ್ಥಿರಾಂಕ UMat& u) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MAT)
    {
        u.copy_ಗಮ್ಯ(*(Mat*)obj); // TODO check u.getMat()
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MATX)
    {
        u.copy_ಗಮ್ಯ(getMat()); // TODO check u.getMat()
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::assign(ಸ್ಥಿರಾಂಕ Mat& m) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == UMAT)
    {
        m.copy_ಗಮ್ಯ(*(UMat*)obj); // TODO check m.getUMat()
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MATX)
    {
        m.copy_ಗಮ್ಯ(getMat());
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::move(UMat& u) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (fixed_ಗಾತ್ರ())
    {
        // TODO Performance warning
        assign(u);
        ಹಿಂತಿರುಗಿ;
    }
    ಪೂರ್ಣಾಂಕ k = ಬಗೆ();
    ಆದರೆ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MAT)
    {
        u.copy_ಗಮ್ಯ(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MATX)
    {
        u.copy_ಗಮ್ಯ(getMat()); // TODO check u.getMat()
        u.release();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::move(Mat& m) ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (fixed_ಗಾತ್ರ())
    {
        // TODO Performance warning
        assign(m);
        ಹಿಂತಿರುಗಿ;
    }
    ಪೂರ್ಣಾಂಕ k = ಬಗೆ();
    ಆದರೆ (k == UMAT)
    {
        m.copy_ಗಮ್ಯ(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == MATX)
    {
        m.copy_ಗಮ್ಯ(getMat());
        m.release();
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::assign(ಸ್ಥಿರಾಂಕ std::vector<UMat>& v) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == STD_ಸದಿಶ_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಗಾಗಿ (size_t i = 0; i < v.size(); i++)
        {
            ಸ್ಥಿರಾಂಕ UMat& m = v[i];
            UMat& this_m = this_v[i];
            ಆದರೆ (this_m.u != ಖಾಲಿ && this_m.u == m.u)
                ಮುಂದುವರಿ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ಗಮ್ಯ(this_m);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == STD_ಸದಿಶ_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಗಾಗಿ (size_t i = 0; i < v.size(); i++)
        {
            ಸ್ಥಿರಾಂಕ UMat& m = v[i];
            Mat& this_m = this_v[i];
            ಆದರೆ (this_m.u != ಖಾಲಿ && this_m.u == m.u)
                ಮುಂದುವರಿ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ಗಮ್ಯ(this_m);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::assign(ಸ್ಥಿರಾಂಕ std::vector<Mat>& v) ಸ್ಥಿರಾಂಕ
{
    ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag k = ಬಗೆ();
    ಆದರೆ (k == STD_ಸದಿಶ_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಗಾಗಿ (size_t i = 0; i < v.size(); i++)
        {
            ಸ್ಥಿರಾಂಕ Mat& m = v[i];
            UMat& this_m = this_v[i];
            ಆದರೆ (this_m.u != ಖಾಲಿ && this_m.u == m.u)
                ಮುಂದುವರಿ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ಗಮ್ಯ(this_m);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (k == STD_ಸದಿಶ_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಗಾಗಿ (size_t i = 0; i < v.size(); i++)
        {
            ಸ್ಥಿರಾಂಕ Mat& m = v[i];
            Mat& this_m = this_v[i];
            ಆದರೆ (this_m.u != ಖಾಲಿ && this_m.u == m.u)
                ಮುಂದುವರಿ; // same object (see dnn::Layer::forward_fallback)
            m.copy_ಗಮ್ಯ(this_m);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಸ್ಥಿರ ಒಳಹರಿವು_Output_Array_2 _none;
ಒಳಹರಿವು_Output_Array noArray() { ಹಿಂತಿರುಗಿ _none; }

} // cv::
