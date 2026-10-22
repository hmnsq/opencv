// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/mat.hpp"

ನಾಮಾಕಾಶ cv {

ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 5> Vec5i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 7> Vec7i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 9> Vec9i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 10> Vec10i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 11> Vec11i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 12> Vec12i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 13> Vec13i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 14> Vec14i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 15> Vec15i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 16> Vec16i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 32> Vec32i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 64> Vec64i;
ಪ್ರಕಾರನಿರ್ದೇಶ Vec<ಪೂರ್ಣಾಙ್ಕ, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    ವಿಕಲ್ಪ( esz ) \
    { \
    ಪಕ್ಷ 1: func(uchar); \
    ಪಕ್ಷ 2: func(Vec2b); \
    ಪಕ್ಷ 3: func(Vec3b); \
    ಪಕ್ಷ 4: func(ಪೂರ್ಣಾಙ್ಕ); \
    ಪಕ್ಷ 6: func(Vec3s); \
    ಪಕ್ಷ 8: func(Vec2i); \
    ಪಕ್ಷ 12: func(Vec3i); \
    ಪಕ್ಷ 16: func(Vec4i); \
    ಪಕ್ಷ 20: func(Vec5i); \
    ಪಕ್ಷ 24: func(Vec6i); \
    ಪಕ್ಷ 28: func(Vec7i); \
    ಪಕ್ಷ 32: func(Vec8i); \
    ಪಕ್ಷ 36: func(Vec9i); \
    ಪಕ್ಷ 40: func(Vec10i); \
    ಪಕ್ಷ 44: func(Vec11i); \
    ಪಕ್ಷ 48: func(Vec12i); \
    ಪಕ್ಷ 52: func(Vec13i); \
    ಪಕ್ಷ 56: func(Vec14i); \
    ಪಕ್ಷ 60: func(Vec15i); \
    ಪಕ್ಷ 64: func(Vec16i); \
    ಪಕ್ಷ 128: func(Vec32i); \
    ಪಕ್ಷ 256: func(Vec64i); \
    ಪಕ್ಷ 512: func(Vec128i); \
    ಸ್ವಭಾವ: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat ಒಳಹರಿವು_Array_2::getMat_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ಯದಿ( k == MAT )
    {
        ಧ್ರುವ Mat* m = (ಧ್ರುವ Mat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ *m;
        ಪ್ರತ್ಯಾಗಮ m->row(i);
    }

    ಯದಿ( k == UMAT )
    {
        ಧ್ರುವ UMat* m = (ಧ್ರುವ UMat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ m->getMat(accessFlags);
        ಪ್ರತ್ಯಾಗಮ m->getMat(accessFlags).row(i);
    }

    ಯದಿ (k == MATX)
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ Mat(sz, flags, obj);
    }

    ಯದಿ( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ಪೂರ್ಣಾಙ್ಕ t = CV_8U;
        ಧ್ರುವ std::vector<ತಾರ್ಕಿಕ>& v = *(ಧ್ರುವ std::vector<ತಾರ್ಕಿಕ>*)obj;
        ಪೂರ್ಣಾಙ್ಕ j, n = (ಪೂರ್ಣಾಙ್ಕ)v.size();
        ಯದಿ( n == 0 )
            ಪ್ರತ್ಯಾಗಮ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        ಪ್ರತಿ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ಪ್ರತ್ಯಾಗಮ m;
    }

    ಯದಿ( k == NONE )
        ಪ್ರತ್ಯಾಗಮ Mat();

    ಯದಿ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        ಪೂರ್ಣಾಙ್ಕ t = CV_MAT_TYPE(flags);
        ಪೂರ್ಣಾಙ್ಕ esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ಯದಿ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ಅನ್ಯಥಾ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ಪೂರ್ಣಾಙ್ಕ v_sz = ಪೂರ್ಣಾಙ್ಕ(v->size()); \
            ಪ್ರತ್ಯಾಗಮ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& v = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );

        ಪ್ರತ್ಯಾಗಮ v[i];
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* v = (ಧ್ರುವ Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ಪ್ರತ್ಯಾಗಮ v[i];
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& v = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );

        ಪ್ರತ್ಯಾಗಮ v[i].getMat(accessFlags);
    }

    ಯದಿ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ಯದಿ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ಯದಿ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ಯದಿ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ಧ್ರುವ cuda::HostMem* cuda_mem = (ಧ್ರುವ cuda::HostMem*)obj;

        ಪ್ರತ್ಯಾಗಮ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat ಒಳಹರಿವು_Array_2::getUMat(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ಯದಿ( k == UMAT )
    {
        ಧ್ರುವ UMat* m = (ಧ್ರುವ UMat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ *m;
        ಪ್ರತ್ಯಾಗಮ m->row(i);
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& v = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );

        ಪ್ರತ್ಯಾಗಮ v[i];
    }

    ಯದಿ( k == MAT )
    {
        ಧ್ರುವ Mat* m = (ಧ್ರುವ Mat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ m->getUMat(accessFlags);
        ಪ್ರತ್ಯಾಗಮ m->row(i).getUMat(accessFlags);
    }

    ಪ್ರತ್ಯಾಗಮ getMat(i).getUMat(accessFlags);
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::getMatVector(std::vector<Mat>& mv) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ಯದಿ( k == MAT )
    {
        ಧ್ರುವ Mat& m = *(ಧ್ರುವ Mat*)obj;
        ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (ಶೂನ್ಯ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (ಶೂನ್ಯ*)m.ptr(i), &m.step[1]);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR )
    {
        ಧ್ರುವ std::vector<uchar>& v = *(ಧ್ರುವ std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        ಪೂರ್ಣಾಙ್ಕ t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (ಶೂನ್ಯ*)(v.data() + esz*i));
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == NONE )
    {
        mv.clear();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_VECTOR )
    {
        ಪೂರ್ಣಾಙ್ಕ typ = CV_MAT_TYPE(flags);
        ಪೂರ್ಣಾಙ್ಕ esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            ಧ್ರುವ std::vector<std::vector<T > >* vv = (ಧ್ರುವ std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            ಪ್ರತಿ (size_t i = 0; i < n; i++) { \
                ಧ್ರುವ std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                ಪೂರ್ಣಾಙ್ಕ ni = (ಪೂರ್ಣಾಙ್ಕ)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (ಶೂನ್ಯ*)vi.data()) : Mat(); \
            } \
        } \
        ಭಙ್ಗ

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& v = *(ಧ್ರುವ std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* v = (ಧ್ರುವ Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& v = *(ಧ್ರುವ std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::getUMatVector(std::vector<UMat>& umv) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ಯದಿ( k == NONE )
    {
        umv.clear();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& v = *(ಧ್ರುವ std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* v = (ಧ್ರುವ Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& v = *(ಧ್ರುವ std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ಪ್ರತಿ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ಪ್ರತ್ಯಾಗಮ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat ಒಳಹರಿವು_Array_2::getGpuMat() ಧ್ರುವ
{
#ifdef HAVE_CUDA
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ (k == CUDA_GPU_MAT)
    {
        ಧ್ರುವ cuda::GpuMat* d_mat = (ಧ್ರುವ cuda::GpuMat*)obj;
        ಪ್ರತ್ಯಾಗಮ *d_mat;
    }

    ಯದಿ (k == CUDA_HOST_MEM)
    {
        ಧ್ರುವ cuda::HostMem* cuda_mem = (ಧ್ರುವ cuda::HostMem*)obj;
        ಪ್ರತ್ಯಾಗಮ cuda_mem->createGpuMatHeader();
    }

    ಯದಿ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ಯದಿ (k == NONE)
        ಪ್ರತ್ಯಾಗಮ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ಅನ್ಯಥಾ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) ಧ್ರುವ
{
#ifdef HAVE_CUDA
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ಧ್ರುವ
{
#ifdef HAVE_CUDA
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ಅನ್ಯಥಾ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND ಒಳಹರಿವು_Array_2::getGpuMatND() ಧ್ರುವ
{
#ifdef HAVE_CUDA
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ (k == CUDA_GPU_MATND)
    {
        ಧ್ರುವ cuda::GpuMatND* d_mat = (ಧ್ರುವ cuda::GpuMatND*)obj;
        ಪ್ರತ್ಯಾಗಮ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ಅನ್ಯಥಾ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer ಒಳಹರಿವು_Array_2::getOGlBuffer() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    ಧ್ರುವ ogl::Buffer* gl_buf = (ಧ್ರುವ ogl::Buffer*)obj;
    ಪ್ರತ್ಯಾಗಮ *gl_buf;
}

ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_Array_2::rows_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
#ifdef HAVE_CUDA
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == CUDA_GPU_MATND)
    {
        ಧ್ರುವ cuda::GpuMatND& _gpuMatND = *(ಧ್ರುವ cuda::GpuMatND*)obj;
        ಪ್ರತ್ಯಾಗಮ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ಪ್ರತ್ಯಾಗಮ size_(i).height;
}

ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_Array_2::cols_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
#ifdef HAVE_CUDA
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == CUDA_GPU_MATND)
    {
        ಧ್ರುವ cuda::GpuMatND& _gpuMatND = *(ಧ್ರುವ cuda::GpuMatND*)obj;
        ಪ್ರತ್ಯಾಗಮ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ಪ್ರತ್ಯಾಗಮ size_(i).width;
}

Size ಒಳಹರಿವು_Array_2::size_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ Mat* m = (ಧ್ರುವ Mat*)obj;
        CV_Assert(m->dims <= 2);
        ಪ್ರತ್ಯಾಗಮ Size(m->cols, m->rows);
    }

    ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ UMat* m = (ಧ್ರುವ UMat*)obj;
        CV_Assert(m->dims <= 2);
        ಪ್ರತ್ಯಾಗಮ Size(m->cols, m->rows);
    }

    ಯದಿ (k == MATX)
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ sz;
    }

    ಯದಿ( k == NONE )
        ಪ್ರತ್ಯಾಗಮ Size();

    ಯದಿ( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ಪ್ರತ್ಯಾಗಮ Size((ಪೂರ್ಣಾಙ್ಕ)n, 1);
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ vv.empty() ? Size() : Size((ಪೂರ್ಣಾಙ್ಕ)vv.size(), 1);
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );

        ಪ್ರತ್ಯಾಗಮ vv[i].size();
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ಪ್ರತ್ಯಾಗಮ vv[i].size();
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ಧ್ರುವ std::vector<cuda::GpuMat>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMat>*)obj;
        ಯದಿ (i < 0)
            ಪ್ರತ್ಯಾಗಮ vv.empty() ? Size() : Size((ಪೂರ್ಣಾಙ್ಕ)vv.size(), 1);
        CV_Assert(i < (ಪೂರ್ಣಾಙ್ಕ)vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].size();
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ಧ್ರುವ std::vector<cuda::GpuMatND>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMatND>*)obj;
        ಯದಿ (i < 0)
            ಪ್ರತ್ಯಾಗಮ vv.empty() ? Size() : Size((ಪೂರ್ಣಾಙ್ಕ)vv.size(), 1);
        CV_Assert(i < (ಪೂರ್ಣಾಙ್ಕ)vv.size());
        ಧ್ರುವ cuda::GpuMatND& m = vv[i];
        ಯದಿ (m.dims == 0)
            ಪ್ರತ್ಯಾಗಮ Size();
        ಯದಿ (m.dims == 1)
            ಪ್ರತ್ಯಾಗಮ Size(1, m.size[0]);
        ಪ್ರತ್ಯಾಗಮ Size(m.size[1], m.size[0]);
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ vv.empty() ? Size() : Size((ಪೂರ್ಣಾಙ್ಕ)vv.size(), 1);
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );

        ಪ್ರತ್ಯಾಗಮ vv[i].size();
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ಧ್ರುವ std::vector<cuda::GpuMatND>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMatND>*)obj;
        ಯದಿ (i < 0)
            ಪ್ರತ್ಯಾಗಮ vv.empty() ? Size() : Size((ಪೂರ್ಣಾಙ್ಕ)vv.size(), 1);
        CV_Assert(i < (ಪೂರ್ಣಾಙ್ಕ)vv.size());
        ಧ್ರುವ cuda::GpuMatND& m = vv[i];
        ಯದಿ (m.dims == 0)
            ಪ್ರತ್ಯಾಗಮ Size();
        ಯದಿ (m.dims == 1)
            ಪ್ರತ್ಯಾಗಮ Size(1, m.size[0]);
        ಪ್ರತ್ಯಾಗಮ Size(m.size[1], m.size[0]);
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ ogl::Buffer* buf = (ಧ್ರುವ ogl::Buffer*)obj;
        ಪ್ರತ್ಯಾಗಮ buf->size();
    }

    ಯದಿ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ cuda::GpuMat* d_mat = (ಧ್ರುವ cuda::GpuMat*)obj;
        ಪ್ರತ್ಯಾಗಮ d_mat->size();
    }

    ಯದಿ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ cuda::HostMem* cuda_mem = (ಧ್ರುವ cuda::HostMem*)obj;
        ಪ್ರತ್ಯಾಗಮ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_Array_2::sizend(ಪೂರ್ಣಾಙ್ಕ* arrsz, ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ j, d = 0;
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == NONE )
        ;
    ಅನ್ಯಥಾ ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ Mat& m = *(ಧ್ರುವ Mat*)obj;
        d = m.dims;
        ಯದಿ(arrsz)
            ಪ್ರತಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಅನ್ಯಥಾ ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ UMat& m = *(ಧ್ರುವ UMat*)obj;
        d = m.dims;
        ಯದಿ(arrsz)
            ಪ್ರತಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಅನ್ಯಥಾ ಯದಿ( k == STD_VECTOR_MAT && i >= 0 )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಧ್ರುವ Mat& m = vv[i];
        d = m.dims;
        ಯದಿ(arrsz)
            ಪ್ರತಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಅನ್ಯಥಾ ಯದಿ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        CV_Assert( i < sz.height );
        ಧ್ರುವ Mat& m = vv[i];
        d = m.dims;
        ಯದಿ(arrsz)
            ಪ್ರತಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಅನ್ಯಥಾ ಯದಿ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಧ್ರುವ UMat& m = vv[i];
        d = m.dims;
        ಯದಿ(arrsz)
            ಪ್ರತಿ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ಅನ್ಯಥಾ ಯದಿ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ಯದಿ(arrsz)
        {
            arrsz[0] = ಪೂರ್ಣಾಙ್ಕ(n);
        }
    }
    ಅನ್ಯಥಾ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ಯದಿ(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ಪ್ರತ್ಯಾಗಮ d;
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::empty(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (i >= 0) {
        ಯದಿ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<ಧ್ರುವ std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ಪ್ರತ್ಯಾಗಮ mv->at(i).empty();
        }
        ಅನ್ಯಥಾ ಯದಿ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<ಧ್ರುವ std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ಪ್ರತ್ಯಾಗಮ umv->at(i).empty();
        }
        ಅನ್ಯಥಾ ಯದಿ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ಪ್ರತ್ಯಾಗಮ n == 0;
        } ಅನ್ಯಥಾ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ಪ್ರತ್ಯಾಗಮ empty();
}

MatShape ಒಳಹರಿವು_Array_2::shape(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    MatShape shape;

    ಯದಿ( k == NONE )
        ;
    ಅನ್ಯಥಾ ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ Mat& m = *(ಧ್ರುವ Mat*)obj;
        shape = m.size;
    }
    ಅನ್ಯಥಾ ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ UMat& m = *(ಧ್ರುವ UMat*)obj;
        shape = m.size;
    }
    ಅನ್ಯಥಾ ಯದಿ( k == STD_VECTOR_MAT && i >= 0 )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ಧ್ರುವ Mat& m = vv[i];
        shape = m.size;
    }
    ಅನ್ಯಥಾ ಯದಿ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        CV_Assert( i < sz.height );
        ಧ್ರುವ Mat& m = vv[i];
        shape = m.size;
    }
    ಅನ್ಯಥಾ ಯದಿ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ಧ್ರುವ UMat& m = vv[i];
        shape = m.size;
    }
    ಅನ್ಯಥಾ
    {
        ಪೂರ್ಣಾಙ್ಕ sizes[CV_MAX_DIM];
        ಪೂರ್ಣಾಙ್ಕ dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ಪ್ರತ್ಯಾಗಮ shape;
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::sameSize(ಧ್ರುವ ಒಳಹರಿವು_Array_2& arr) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ಯದಿ( k1 == MAT )
    {
        ಧ್ರುವ Mat* m = ((ಧ್ರುವ Mat*)obj);
        ಯದಿ( k2 == MAT )
            ಪ್ರತ್ಯಾಗಮ m->size == ((ಧ್ರುವ Mat*)arr.obj)->size;
        ಯದಿ( k2 == UMAT )
            ಪ್ರತ್ಯಾಗಮ m->size == ((ಧ್ರುವ UMat*)arr.obj)->size;
        ಯದಿ( m->dims > 2 )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        sz1 = m->size();
    }
    ಅನ್ಯಥಾ ಯದಿ( k1 == UMAT )
    {
        ಧ್ರುವ UMat* m = ((ಧ್ರುವ UMat*)obj);
        ಯದಿ( k2 == MAT )
            ಪ್ರತ್ಯಾಗಮ m->size == ((ಧ್ರುವ Mat*)arr.obj)->size;
        ಯದಿ( k2 == UMAT )
            ಪ್ರತ್ಯಾಗಮ m->size == ((ಧ್ರುವ UMat*)arr.obj)->size;
        ಯದಿ( m->dims > 2 )
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        sz1 = m->size();
    }
    ಅನ್ಯಥಾ ಯದಿ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMatND*)obj)->size == ((ಧ್ರುವ cuda::GpuMatND*)arr.obj)->size;
    }
    ಅನ್ಯಥಾ
        sz1 = size();

    ಯದಿ( arr.dims() > 2 )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ sz1 == arr.size();
}

ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_Array_2::dims_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ Mat*)obj)->dims;
    }

    ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ UMat*)obj)->dims;
    }

    ಯದಿ (k == MATX)
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ 2;
    }

    ಯದಿ( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    ಯದಿ( k == NONE )
        ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ( k == STD_VECTOR_VECTOR )
    {
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ 1;
        CV_Assert( size_t(i) < total() );
        ಪ್ರತ್ಯಾಗಮ 1;
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ 1;
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );

        ಪ್ರತ್ಯಾಗಮ vv[i].dims;
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ 1;
        CV_Assert( i < sz.height );

        ಪ್ರತ್ಯಾಗಮ vv[i].dims;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ 1;
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );

        ಪ್ರತ್ಯಾಗಮ vv[i].dims;
    }

    ಯದಿ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ 2;
    }

    ಯದಿ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ 2;
    }

    ಯದಿ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMatND*)obj)->dims;
    }

    ಯದಿ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t ಒಳಹರಿವು_Array_2::total(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ Mat*)obj)->total();
    }

    ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ UMat*)obj)->total();
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ vv.size();

        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಪ್ರತ್ಯಾಗಮ vv[i].total();
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ sz.height;

        CV_Assert( i < sz.height );
        ಪ್ರತ್ಯಾಗಮ vv[i].total();
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        ಯದಿ( i < 0 )
            ಪ್ರತ್ಯಾಗಮ vv.size();

        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಪ್ರತ್ಯಾಗಮ vv[i].total();
    }

    ಯದಿ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ಧ್ರುವ cuda::GpuMatND& _gpuMatND = *((ಧ್ರುವ cuda::GpuMatND*)obj);
        ಯದಿ (_gpuMatND.dims > 0)
        {
            res = 1;
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ಪ್ರತ್ಯಾಗಮ res;
        }
    }

    ಯದಿ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        ಪೂರ್ಣಾಙ್ಕ esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ಯದಿ (k == STD_VECTOR_VECTOR) { \
                ಧ್ರುವ std::vector<std::vector<T > >* vv = (ಧ್ರುವ std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ಯದಿ (i < 0) \
                    ಪ್ರತ್ಯಾಗಮ n; \
                CV_Assert((size_t)i < n); \
                ಪ್ರತ್ಯಾಗಮ vv->at(i).size(); \
            } ಅನ್ಯಥಾ \
                ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ಪ್ರತ್ಯಾಗಮ size(i).area();
}

ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_Array_2::type_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ Mat*)obj)->type();

    ಯದಿ( k == UMAT )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ UMat*)obj)->type();

    ಯದಿ( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ಪ್ರತ್ಯಾಗಮ CV_MAT_TYPE(flags);

    ಯದಿ( k == NONE )
        ಪ್ರತ್ಯಾಗಮ -1;

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        ಯದಿ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಪ್ರತ್ಯಾಗಮ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಪ್ರತ್ಯಾಗಮ vv[i >= 0 ? i : 0].type();
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        ಯದಿ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಪ್ರತ್ಯಾಗಮ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಪ್ರತ್ಯಾಗಮ vv[i >= 0 ? i : 0].type();
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        ಯದಿ( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಪ್ರತ್ಯಾಗಮ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ಪ್ರತ್ಯಾಗಮ vv[i >= 0 ? i : 0].type();
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ಧ್ರುವ std::vector<cuda::GpuMat>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMat>*)obj;
        ಯದಿ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಪ್ರತ್ಯಾಗಮ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ಪೂರ್ಣಾಙ್ಕ)vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i >= 0 ? i : 0].type();
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ಧ್ರುವ std::vector<cuda::GpuMatND>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMatND>*)obj;
        ಯದಿ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ಪ್ರತ್ಯಾಗಮ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ಪೂರ್ಣಾಙ್ಕ)vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i >= 0 ? i : 0].type();
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ( k == OPENGL_BUFFER )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ ogl::Buffer*)obj)->type();

    ಯದಿ( k == CUDA_GPU_MAT )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMat*)obj)->type();

    ಯದಿ( k == CUDA_GPU_MATND )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMatND*)obj)->type();

    ಯದಿ( k == CUDA_HOST_MEM )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಪೂರ್ಣಾಙ್ಕ ಒಳಹರಿವು_Array_2::depth_(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ CV_MAT_DEPTH(type(i));
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::empty_() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ Mat*)obj)->empty();

    ಯದಿ( k == UMAT )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ UMat*)obj)->empty();

    ಯದಿ (k == MATX)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ಪ್ರತ್ಯಾಗಮ n == 0;
    }

    ಯದಿ( k == NONE )
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        ಪ್ರತ್ಯಾಗಮ vv.empty();
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಪ್ರತ್ಯಾಗಮ sz.height == 0;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        ಪ್ರತ್ಯಾಗಮ vv.empty();
    }

    ಯದಿ( k == OPENGL_BUFFER )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ ogl::Buffer*)obj)->empty();

    ಯದಿ( k == CUDA_GPU_MAT )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMat*)obj)->empty();

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ಧ್ರುವ std::vector<cuda::GpuMat>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMat>*)obj;
        ಪ್ರತ್ಯಾಗಮ vv.empty();
    }

    ಯದಿ( k == CUDA_GPU_MATND )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMatND*)obj)->empty();

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ಧ್ರುವ std::vector<cuda::GpuMatND>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMatND>*)obj;
        ಪ್ರತ್ಯಾಗಮ vv.empty();
    }

    ಯದಿ( k == CUDA_HOST_MEM )
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::isContinuous(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
        ಪ್ರತ್ಯಾಗಮ i < 0 ? ((ಧ್ರುವ Mat*)obj)->isContinuous() : ಸತ್ಯ;

    ಯದಿ( k == UMAT )
        ಪ್ರತ್ಯಾಗಮ i < 0 ? ((ಧ್ರುವ UMat*)obj)->isContinuous() : ಸತ್ಯ;

    ಯದಿ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].isContinuous();
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ಪ್ರತ್ಯಾಗಮ vv[i].isContinuous();
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].isContinuous();
    }

    ಯದಿ( k == CUDA_GPU_MAT )
      ಪ್ರತ್ಯಾಗಮ i < 0 ? ((ಧ್ರುವ cuda::GpuMat*)obj)->isContinuous() : ಸತ್ಯ;

    ಯದಿ( k == CUDA_GPU_MATND )
        ಪ್ರತ್ಯಾಗಮ i < 0 ? ((ಧ್ರುವ cuda::GpuMatND*)obj)->isContinuous() : ಸತ್ಯ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

ತಾರ್ಕಿಕ ಒಳಹರಿವು_Array_2::isSubmatrix(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
        ಪ್ರತ್ಯಾಗಮ i < 0 ? ((ಧ್ರುವ Mat*)obj)->isSubmatrix() : ಅಸತ್ಯ;

    ಯದಿ( k == UMAT )
        ಪ್ರತ್ಯಾಗಮ i < 0 ? ((ಧ್ರುವ UMat*)obj)->isSubmatrix() : ಅಸತ್ಯ;

    ಯದಿ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].isSubmatrix();
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ಪ್ರತ್ಯಾಗಮ vv[i].isSubmatrix();
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].isSubmatrix();
    }

    ಯದಿ( k == CUDA_GPU_MATND )
    {
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ಧ್ರುವ std::vector<cuda::GpuMatND>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t ಒಳಹರಿವು_Array_2::offset(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ Mat * ಧ್ರುವ m = ((ಧ್ರುವ Mat*)obj);
        ಪ್ರತ್ಯಾಗಮ (size_t)(m->ptr() - m->datastart);
    }

    ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ UMat*)obj)->offset;
    }

    ಯದಿ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );

        ಪ್ರತ್ಯಾಗಮ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ಪ್ರತ್ಯಾಗಮ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].offset;
    }

    ಯದಿ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಧ್ರುವ cuda::GpuMat * ಧ್ರುವ m = ((ಧ್ರುವ cuda::GpuMat*)obj);
        ಪ್ರತ್ಯಾಗಮ (size_t)(m->data - m->datastart);
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ಧ್ರುವ std::vector<cuda::GpuMat>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t ಒಳಹರಿವು_Array_2::step(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ Mat*)obj)->step;
    }

    ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ UMat*)obj)->step;
    }

    ಯದಿ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ಧ್ರುವ std::vector<Mat>& vv = *(ಧ್ರುವ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ಪೂರ್ಣಾಙ್ಕ)vv.size() );
        ಪ್ರತ್ಯಾಗಮ vv[i].step;
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        ಧ್ರುವ Mat* vv = (ಧ್ರುವ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ಪ್ರತ್ಯಾಗಮ vv[i].step;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ಧ್ರುವ std::vector<UMat>& vv = *(ಧ್ರುವ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].step;
    }

    ಯದಿ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ಪ್ರತ್ಯಾಗಮ ((ಧ್ರುವ cuda::GpuMat*)obj)->step;
    }
    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ಧ್ರುವ std::vector<cuda::GpuMat>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಪ್ರತ್ಯಾಗಮ vv[i].step;
    }
    ಯದಿ( k == CUDA_GPU_MATND )
    {
        ಧ್ರುವ cuda::GpuMatND& _gpuMatND = *(ಧ್ರುವ cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ಪ್ರತ್ಯಾಗಮ _gpuMatND.step[i];
    }

    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ಧ್ರುವ std::vector<cuda::GpuMatND>& vv = *(ಧ್ರುವ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ಧ್ರುವ cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ಪ್ರತ್ಯಾಗಮ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::copyTo(ಧ್ರುವ _OutputArray& arr) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == NONE )
        arr.release();
    ಅನ್ಯಥಾ ಯದಿ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ಅನ್ಯಥಾ ಯದಿ( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ಅನ್ಯಥಾ ಯದಿ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "");
}

ಶೂನ್ಯ ಒಳಹರಿವು_Array_2::copyTo(ಧ್ರುವ _OutputArray& arr, ಧ್ರುವ ಒಳಹರಿವು_Array_2 & mask) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == NONE )
        arr.release();
    ಅನ್ಯಥಾ ಯದಿ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ಅನ್ಯಥಾ ಯದಿ( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ಅನ್ಯಥಾ ಯದಿ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "");
}

ತಾರ್ಕಿಕ _OutputArray::fixedSize() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ (flags & FIXED_SIZE) == FIXED_SIZE;
}

ತಾರ್ಕಿಕ _OutputArray::fixedType() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ (flags & FIXED_TYPE) == FIXED_TYPE;
}

ಶೂನ್ಯ _OutputArray::create_(Size _sz, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i, ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಯದಿ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಯದಿ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ಯದಿ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಪೂರ್ಣಾಙ್ಕ sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ಶೂನ್ಯ _OutputArray::create_(ಪೂರ್ಣಾಙ್ಕ _rows, ಪೂರ್ಣಾಙ್ಕ _cols, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i, ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಯದಿ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಯದಿ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ಯದಿ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಪೂರ್ಣಾಙ್ಕ sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ಶೂನ್ಯ _OutputArray::create(ಪೂರ್ಣಾಙ್ಕ d, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i,
                          ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ಯದಿ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ಯದಿ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ಅನ್ಯಥಾ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ಯದಿ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ಯದಿ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ಅನ್ಯಥಾ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ಯದಿ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == MATX )
    {
        CV_Assert( i < 0 );
        ಪೂರ್ಣಾಙ್ಕ type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ಯದಿ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ಪೂರ್ಣಾಙ್ಕ total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ಅನ್ಯಥಾ
        {
            ಯದಿ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ಅನ್ಯಥಾ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ಪೂರ್ಣಾಙ್ಕ esz = CV_ELEM_SIZE(flags);

        ಯದಿ( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            ಪೂರ್ಣಾಙ್ಕ type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ಯದಿ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ಯದಿ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ಪ್ರತ್ಯಾಗಮ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ಅನ್ಯಥಾ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        ಭಙ್ಗ

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ಯದಿ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ಯದಿ( fixedType() )
            {
                ಪೂರ್ಣಾಙ್ಕ _type = CV_MAT_TYPE(flags);
                ಪ್ರತಿ( size_t j = len0; j < len; j++ )
                {
                    ಯದಿ( v[j].type() == _type )
                        ಅನುವರ್ತ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ಪ್ರತ್ಯಾಗಮ;
        }

        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );
        Mat& m = v[i];

        ಯದಿ( allowTransposed )
        {
            ಯದಿ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ಯದಿ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ಅನ್ಯಥಾ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ಯದಿ(fixedSize())
        {
            CV_Assert(m.dims == d);
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ಯದಿ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ಯದಿ( fixedType() )
            {
                ಪೂರ್ಣಾಙ್ಕ _type = CV_MAT_TYPE(flags);
                ಪ್ರತಿ( size_t j = len0; j < len; j++ )
                {
                    ಯದಿ( v[j].type() == _type )
                        ಅನುವರ್ತ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ಪ್ರತ್ಯಾಗಮ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ಯದಿ( allowTransposed )
        {
            ಯದಿ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ಯದಿ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ಅನ್ಯಥಾ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ಯದಿ(fixedSize())
        {
            CV_Assert(m.dims == d);
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ಯದಿ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ಯದಿ( fixedType() )
            {
                ಪೂರ್ಣಾಙ್ಕ _type = CV_MAT_TYPE(flags);
                ಪ್ರತಿ( size_t j = len0; j < len; j++ )
                {
                    ಯದಿ( v[j].type() == _type )
                        ಅನುವರ್ತ;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ಪ್ರತ್ಯಾಗಮ;
        }

        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );
        UMat& m = v[i];

        ಯದಿ( allowTransposed )
        {
            ಯದಿ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ಯದಿ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ಅನ್ಯಥಾ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ಯದಿ(fixedSize())
        {
            CV_Assert(m.dims == d);
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಶೂನ್ಯ _OutputArray::create(ಧ್ರುವ MatShape& shape, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i,
                          ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಯದಿ (shape.dims < 0) {
        release();
    } ಅನ್ಯಥಾ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(ಪೂರ್ಣಾಙ್ಕ mtype) ಧ್ರುವ
{
    mtype = CV_MAT_TYPE(mtype);
    ಪ್ರತ್ಯಾಗಮ getMat().reinterpret(mtype);
}

ಶೂನ್ಯ _OutputArray::createSameSize(ಧ್ರುವ ಒಳಹರಿವು_Array_2& arr, ಪೂರ್ಣಾಙ್ಕ mtype) ಧ್ರುವ
{
    create(arr.shape(), mtype);
}

ಶೂನ್ಯ _OutputArray::fit(Size _sz, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ಶೂನ್ಯ _OutputArray::fit(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ಶೂನ್ಯ _OutputArray::fit(ಧ್ರುವ MatShape& shape, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಯದಿ (shape.dims < 0) {
        release();
        ಪ್ರತ್ಯಾಗಮ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ಅನ್ಯಥಾ ಯದಿ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

ಶೂನ್ಯ _OutputArray::fitSameSize(ಧ್ರುವ ಒಳಹರಿವು_Array_2& arr, ಪೂರ್ಣಾಙ್ಕ mtype) ಧ್ರುವ
{
    fit(arr.shape(), mtype);
}

ಶೂನ್ಯ _OutputArray::fit(ಪೂರ್ಣಾಙ್ಕ d, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ mtype, ಪೂರ್ಣಾಙ್ಕ i,
                       ತಾರ್ಕಿಕ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ಧ್ರುವ
{
    ಪೂರ್ಣಾಙ್ಕ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ಯದಿ( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ಯದಿ (k == MAT)
            m = (Mat*)obj;
        ಅನ್ಯಥಾ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ಯದಿ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ಅನ್ಯಥಾ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ಯದಿ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ಯದಿ (k == UMAT)
            m = (UMat*)obj;
        ಅನ್ಯಥಾ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ಯದಿ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(fixedType())
        {
            ಯದಿ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ಅನ್ಯಥಾ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ಯದಿ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ಪ್ರತಿ(ಪೂರ್ಣಾಙ್ಕ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ಪ್ರತ್ಯಾಗಮ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ಶೂನ್ಯ _OutputArray::release() ಧ್ರುವ
{
    CV_Assert(!fixedSize());

    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        ((Mat*)obj)->release();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ಯದಿ( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ಯದಿ( k == NONE )
        ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ಯದಿ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ಅನ್ಯಥಾ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        ಭಙ್ಗ

        ಪೂರ್ಣಾಙ್ಕ esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < sz.height; i++ )
            v[i].release();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_BOOL_VECTOR )
    {
        ((std::vector<ತಾರ್ಕಿಕ>*)obj)->clear();
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಯದಿ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ಪ್ರತ್ಯಾಗಮ;
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ಶೂನ್ಯ _OutputArray::clear() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ಪ್ರತ್ಯಾಗಮ;
    }

    release();
}

ತಾರ್ಕಿಕ _OutputArray::needed() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ kind() != NONE;
}

Mat& _OutputArray::getMatRef(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ( i < 0 )
    {
        CV_Assert( k == MAT );
        ಪ್ರತ್ಯಾಗಮ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ಯದಿ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );
        ಪ್ರತ್ಯಾಗಮ v[i];
    }
    ಅನ್ಯಥಾ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ಪ್ರತ್ಯಾಗಮ v[i];
    }
}

UMat& _OutputArray::getUMatRef(ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ಪ್ರತ್ಯಾಗಮ *(UMat*)obj;
    }
    ಅನ್ಯಥಾ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ಪೂರ್ಣಾಙ್ಕ)v.size() );
        ಪ್ರತ್ಯಾಗಮ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ಪ್ರತ್ಯಾಗಮ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ಪ್ರತ್ಯಾಗಮ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ಪ್ರತ್ಯಾಗಮ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ಪ್ರತ್ಯಾಗಮ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ಪ್ರತ್ಯಾಗಮ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ಪ್ರತ್ಯಾಗಮ *(cuda::HostMem*)obj;
}

ಶೂನ್ಯ _OutputArray::setTo(ಧ್ರುವ ಒಳಹರಿವು_Array_2& arr, ಧ್ರುವ ಒಳಹರಿವು_Array_2 & mask) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == NONE )
        ;
    ಅನ್ಯಥಾ ಯದಿ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ಅನ್ಯಥಾ ಯದಿ( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ಅನ್ಯಥಾ ಯದಿ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), ಒಳಹರಿವು_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<ದ್ವಿಗುಣ, 4>(value.ptr<ದ್ವಿಗುಣ>())), mask);
#ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ಅನ್ಯಥಾ
        CV_Error(Error::StsNotImplemented, "");
}

ಶೂನ್ಯ _OutputArray::setZero() ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();

    ಯದಿ( k == NONE )
        ;
    ಅನ್ಯಥಾ ಯದಿ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ಅನ್ಯಥಾ
    {
        setTo(Scalar::all(0), noArray());
    }
}

ಶೂನ್ಯ _OutputArray::assign(ಧ್ರುವ UMat& u) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ಅನ್ಯಥಾ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::assign(ಧ್ರುವ Mat& m) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MATX)
    {
        m.copyTo(getMat());
    }
    ಅನ್ಯಥಾ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::move(UMat& u) ಧ್ರುವ
{
    ಯದಿ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಪೂರ್ಣಾಙ್ಕ k = kind();
    ಯದಿ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ಅನ್ಯಥಾ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::move(Mat& m) ಧ್ರುವ
{
    ಯದಿ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ಪ್ರತ್ಯಾಗಮ;
    }
    ಪೂರ್ಣಾಙ್ಕ k = kind();
    ಯದಿ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ಅನ್ಯಥಾ ಯದಿ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ಅನ್ಯಥಾ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::assign(ಧ್ರುವ std::vector<UMat>& v) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಪ್ರತಿ (size_t i = 0; i < v.size(); i++)
        {
            ಧ್ರುವ UMat& m = v[i];
            UMat& this_m = this_v[i];
            ಯದಿ (this_m.u != ರಿಕ್ತ && this_m.u == m.u)
                ಅನುವರ್ತ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ಅನ್ಯಥಾ ಯದಿ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಪ್ರತಿ (size_t i = 0; i < v.size(); i++)
        {
            ಧ್ರುವ UMat& m = v[i];
            Mat& this_m = this_v[i];
            ಯದಿ (this_m.u != ರಿಕ್ತ && this_m.u == m.u)
                ಅನುವರ್ತ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ಅನ್ಯಥಾ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಶೂನ್ಯ _OutputArray::assign(ಧ್ರುವ std::vector<Mat>& v) ಧ್ರುವ
{
    ಒಳಹರಿವು_Array_2::KindFlag k = kind();
    ಯದಿ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಪ್ರತಿ (size_t i = 0; i < v.size(); i++)
        {
            ಧ್ರುವ Mat& m = v[i];
            UMat& this_m = this_v[i];
            ಯದಿ (this_m.u != ರಿಕ್ತ && this_m.u == m.u)
                ಅನುವರ್ತ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ಅನ್ಯಥಾ ಯದಿ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ಪ್ರತಿ (size_t i = 0; i < v.size(); i++)
        {
            ಧ್ರುವ Mat& m = v[i];
            Mat& this_m = this_v[i];
            ಯದಿ (this_m.u != ರಿಕ್ತ && this_m.u == m.u)
                ಅನುವರ್ತ; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ಅನ್ಯಥಾ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ಸ್ಥಿರ ಒಳಹರಿವು_Output_Array_2 _none;
ಒಳಹರಿವು_Output_Array noArray() { ಪ್ರತ್ಯಾಗಮ _none; }

} // cv::
