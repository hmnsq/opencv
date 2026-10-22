// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<puur_naa_nka, 5> Vec5i;
typedef Vec<puur_naa_nka, 7> Vec7i;
typedef Vec<puur_naa_nka, 9> Vec9i;
typedef Vec<puur_naa_nka, 10> Vec10i;
typedef Vec<puur_naa_nka, 11> Vec11i;
typedef Vec<puur_naa_nka, 12> Vec12i;
typedef Vec<puur_naa_nka, 13> Vec13i;
typedef Vec<puur_naa_nka, 14> Vec14i;
typedef Vec<puur_naa_nka, 15> Vec15i;
typedef Vec<puur_naa_nka, 16> Vec16i;
typedef Vec<puur_naa_nka, 32> Vec32i;
typedef Vec<puur_naa_nka, 64> Vec64i;
typedef Vec<puur_naa_nka, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    pak_sa 1: func(uchar); \
    pak_sa 2: func(Vec2b); \
    pak_sa 3: func(Vec3b); \
    pak_sa 4: func(puur_naa_nka); \
    pak_sa 6: func(Vec3s); \
    pak_sa 8: func(Vec2i); \
    pak_sa 12: func(Vec3i); \
    pak_sa 16: func(Vec4i); \
    pak_sa 20: func(Vec5i); \
    pak_sa 24: func(Vec6i); \
    pak_sa 28: func(Vec7i); \
    pak_sa 32: func(Vec8i); \
    pak_sa 36: func(Vec9i); \
    pak_sa 40: func(Vec10i); \
    pak_sa 44: func(Vec11i); \
    pak_sa 48: func(Vec12i); \
    pak_sa 52: func(Vec13i); \
    pak_sa 56: func(Vec14i); \
    pak_sa 60: func(Vec15i); \
    pak_sa 64: func(Vec16i); \
    pak_sa 128: func(Vec32i); \
    pak_sa 256: func(Vec64i); \
    pak_sa 512: func(Vec128i); \
    default: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat _InputArray::getMat_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᮚᮦᮔ᮪( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ *m;
        ᮝᮜᮤ m->row(i);
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ m->getMat(accessFlags);
        ᮝᮜᮤ m->getMat(accessFlags).row(i);
    }

    ᮚᮦᮔ᮪ (k == MATX)
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ Mat(sz, flags, obj);
    }

    ᮚᮦᮔ᮪( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        puur_naa_nka t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        puur_naa_nka j, n = (puur_naa_nka)v.size();
        ᮚᮦᮔ᮪( n == 0 )
            ᮝᮜᮤ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        for( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ᮝᮜᮤ m;
    }

    ᮚᮦᮔ᮪( k == NONE )
        ᮝᮜᮤ Mat();

    ᮚᮦᮔ᮪( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        puur_naa_nka t = CV_MAT_TYPE(flags);
        puur_naa_nka esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ᮚᮦᮔ᮪ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ᮜᮤᮚ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            puur_naa_nka v_sz = puur_naa_nka(v->size()); \
            ᮝᮜᮤ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ᮝᮜᮤ v[i];
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ᮝᮜᮤ v[i];
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ᮝᮜᮤ v[i].getMat(accessFlags);
    }

    ᮚᮦᮔ᮪( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ᮝᮜᮤ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᮚᮦᮔ᮪( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ *m;
        ᮝᮜᮤ m->row(i);
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ᮝᮜᮤ v[i];
    }

    ᮚᮦᮔ᮪( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ m->getUMat(accessFlags);
        ᮝᮜᮤ m->row(i).getUMat(accessFlags);
    }

    ᮝᮜᮤ getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᮚᮦᮔ᮪( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        puur_naa_nka n = (puur_naa_nka)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        for( puur_naa_nka i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        puur_naa_nka t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == NONE )
    {
        mv.clear();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_VECTOR )
    {
        puur_naa_nka typ = CV_MAT_TYPE(flags);
        puur_naa_nka esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            for (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                puur_naa_nka ni = (puur_naa_nka)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ᮝᮜᮤ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᮚᮦᮔ᮪( k == NONE )
    {
        umv.clear();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ᮝᮜᮤ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪ (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ᮝᮜᮤ *d_mat;
    }

    ᮚᮦᮔ᮪ (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ᮝᮜᮤ cuda_mem->createGpuMatHeader();
    }

    ᮚᮦᮔ᮪ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ᮚᮦᮔ᮪ (k == NONE)
        ᮝᮜᮤ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ᮜᮤᮚ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ᮜᮤᮚ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ᮜᮤᮚ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ᮝᮜᮤ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ᮜᮤᮚ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ᮝᮜᮤ *gl_buf;
}

puur_naa_nka _InputArray::rows_(puur_naa_nka i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ᮝᮜᮤ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ᮝᮜᮤ size_(i).height;
}

puur_naa_nka _InputArray::cols_(puur_naa_nka i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ᮝᮜᮤ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ᮝᮜᮤ size_(i).width;
}

Size _InputArray::size_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ᮝᮜᮤ Size(m->cols, m->rows);
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ᮝᮜᮤ Size(m->cols, m->rows);
    }

    ᮚᮦᮔ᮪ (k == MATX)
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ sz;
    }

    ᮚᮦᮔ᮪( k == NONE )
        ᮝᮜᮤ Size();

    ᮚᮦᮔ᮪( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ᮝᮜᮤ Size((puur_naa_nka)n, 1);
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᮝᮜᮤ vv[i].size();
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ᮝᮜᮤ vv[i].size();
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ᮚᮦᮔ᮪ (i < 0)
            ᮝᮜᮤ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        ᮝᮜᮤ vv[i].size();
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᮚᮦᮔ᮪ (i < 0)
            ᮝᮜᮤ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ᮚᮦᮔ᮪ (m.dims == 0)
            ᮝᮜᮤ Size();
        ᮚᮦᮔ᮪ (m.dims == 1)
            ᮝᮜᮤ Size(1, m.size[0]);
        ᮝᮜᮤ Size(m.size[1], m.size[0]);
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᮝᮜᮤ vv[i].size();
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᮚᮦᮔ᮪ (i < 0)
            ᮝᮜᮤ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ᮚᮦᮔ᮪ (m.dims == 0)
            ᮝᮜᮤ Size();
        ᮚᮦᮔ᮪ (m.dims == 1)
            ᮝᮜᮤ Size(1, m.size[0]);
        ᮝᮜᮤ Size(m.size[1], m.size[0]);
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ᮝᮜᮤ buf->size();
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ᮝᮜᮤ d_mat->size();
    }

    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ᮝᮜᮤ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

puur_naa_nka _InputArray::sizend(puur_naa_nka* arrsz, puur_naa_nka i) const
{
    puur_naa_nka j, d = 0;
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == NONE )
        ;
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ᮚᮦᮔ᮪(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ᮚᮦᮔ᮪(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (puur_naa_nka)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ᮚᮦᮔ᮪(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ᮚᮦᮔ᮪(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (puur_naa_nka)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ᮚᮦᮔ᮪(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ᮚᮦᮔ᮪(arrsz)
        {
            arrsz[0] = puur_naa_nka(n);
        }
    }
    ᮜᮤᮚ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ᮚᮦᮔ᮪(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ᮝᮜᮤ d;
}

bool _InputArray::empty(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (i >= 0) {
        ᮚᮦᮔ᮪ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ᮝᮜᮤ mv->at(i).empty();
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ᮝᮜᮤ umv->at(i).empty();
        }
        ᮜᮤᮚ ᮚᮦᮔ᮪ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ᮝᮜᮤ n == 0;
        } ᮜᮤᮚ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ᮝᮜᮤ empty();
}

MatShape _InputArray::shape(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ᮚᮦᮔ᮪( k == NONE )
        ;
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    ᮜᮤᮚ
    {
        puur_naa_nka sizes[CV_MAX_DIM];
        puur_naa_nka dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ᮝᮜᮤ shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ᮚᮦᮔ᮪( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ᮚᮦᮔ᮪( k2 == MAT )
            ᮝᮜᮤ m->size == ((const Mat*)arr.obj)->size;
        ᮚᮦᮔ᮪( k2 == UMAT )
            ᮝᮜᮤ m->size == ((const UMat*)arr.obj)->size;
        ᮚᮦᮔ᮪( m->dims > 2 )
            ᮝᮜᮤ false;
        sz1 = m->size();
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ᮚᮦᮔ᮪( k2 == MAT )
            ᮝᮜᮤ m->size == ((const Mat*)arr.obj)->size;
        ᮚᮦᮔ᮪( k2 == UMAT )
            ᮝᮜᮤ m->size == ((const UMat*)arr.obj)->size;
        ᮚᮦᮔ᮪( m->dims > 2 )
            ᮝᮜᮤ false;
        sz1 = m->size();
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ᮝᮜᮤ ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    ᮜᮤᮚ
        sz1 = size();

    ᮚᮦᮔ᮪( arr.dims() > 2 )
        ᮝᮜᮤ false;
    ᮝᮜᮤ sz1 == arr.size();
}

puur_naa_nka _InputArray::dims_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const Mat*)obj)->dims;
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const UMat*)obj)->dims;
    }

    ᮚᮦᮔ᮪ (k == MATX)
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ 2;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ 1;
    }

    ᮚᮦᮔ᮪( k == NONE )
        ᮝᮜᮤ 0;

    ᮚᮦᮔ᮪( k == STD_VECTOR_VECTOR )
    {
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ 1;
        CV_Assert( size_t(i) < total() );
        ᮝᮜᮤ 1;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ 1;
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᮝᮜᮤ vv[i].dims;
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ 1;
        CV_Assert( i < sz.height );

        ᮝᮜᮤ vv[i].dims;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ 1;
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᮝᮜᮤ vv[i].dims;
    }

    ᮚᮦᮔ᮪( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ 2;
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ 2;
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const cuda::GpuMatND*)obj)->dims;
    }

    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const Mat*)obj)->total();
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const UMat*)obj)->total();
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ vv.size();

        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᮝᮜᮤ vv[i].total();
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ sz.height;

        CV_Assert( i < sz.height );
        ᮝᮜᮤ vv[i].total();
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᮚᮦᮔ᮪( i < 0 )
            ᮝᮜᮤ vv.size();

        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᮝᮜᮤ vv[i].total();
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ᮚᮦᮔ᮪ (_gpuMatND.dims > 0)
        {
            res = 1;
            for(puur_naa_nka d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ᮝᮜᮤ res;
        }
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        puur_naa_nka esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ᮚᮦᮔ᮪ (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ᮚᮦᮔ᮪ (i < 0) \
                    ᮝᮜᮤ n; \
                CV_Assert((size_t)i < n); \
                ᮝᮜᮤ vv->at(i).size(); \
            } ᮜᮤᮚ \
                ᮝᮜᮤ ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ᮝᮜᮤ size(i).area();
}

puur_naa_nka _InputArray::type_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
        ᮝᮜᮤ ((const Mat*)obj)->type();

    ᮚᮦᮔ᮪( k == UMAT )
        ᮝᮜᮤ ((const UMat*)obj)->type();

    ᮚᮦᮔ᮪( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᮝᮜᮤ CV_MAT_TYPE(flags);

    ᮚᮦᮔ᮪( k == NONE )
        ᮝᮜᮤ -1;

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᮚᮦᮔ᮪( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᮝᮜᮤ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᮝᮜᮤ vv[i >= 0 ? i : 0].type();
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᮚᮦᮔ᮪( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᮝᮜᮤ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᮝᮜᮤ vv[i >= 0 ? i : 0].type();
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᮚᮦᮔ᮪( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᮝᮜᮤ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ᮝᮜᮤ vv[i >= 0 ? i : 0].type();
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ᮚᮦᮔ᮪ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᮝᮜᮤ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (puur_naa_nka)vv.size());
        ᮝᮜᮤ vv[i >= 0 ? i : 0].type();
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᮚᮦᮔ᮪ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᮝᮜᮤ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (puur_naa_nka)vv.size());
        ᮝᮜᮤ vv[i >= 0 ? i : 0].type();
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪( k == OPENGL_BUFFER )
        ᮝᮜᮤ ((const ogl::Buffer*)obj)->type();

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
        ᮝᮜᮤ ((const cuda::GpuMat*)obj)->type();

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
        ᮝᮜᮤ ((const cuda::GpuMatND*)obj)->type();

    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM )
        ᮝᮜᮤ ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

puur_naa_nka _InputArray::depth_(puur_naa_nka i) const
{
    ᮝᮜᮤ CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
        ᮝᮜᮤ ((const Mat*)obj)->empty();

    ᮚᮦᮔ᮪( k == UMAT )
        ᮝᮜᮤ ((const UMat*)obj)->empty();

    ᮚᮦᮔ᮪ (k == MATX)
        ᮝᮜᮤ false;

    ᮚᮦᮔ᮪( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ᮝᮜᮤ n == 0;
    }

    ᮚᮦᮔ᮪( k == NONE )
        ᮝᮜᮤ true;

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᮝᮜᮤ vv.empty();
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        ᮝᮜᮤ sz.height == 0;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᮝᮜᮤ vv.empty();
    }

    ᮚᮦᮔ᮪( k == OPENGL_BUFFER )
        ᮝᮜᮤ ((const ogl::Buffer*)obj)->empty();

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
        ᮝᮜᮤ ((const cuda::GpuMat*)obj)->empty();

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ᮝᮜᮤ vv.empty();
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
        ᮝᮜᮤ ((const cuda::GpuMatND*)obj)->empty();

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᮝᮜᮤ vv.empty();
    }

    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM )
        ᮝᮜᮤ ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
        ᮝᮜᮤ i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ᮚᮦᮔ᮪( k == UMAT )
        ᮝᮜᮤ i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ᮚᮦᮔ᮪( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᮝᮜᮤ true;

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].isContinuous();
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ᮝᮜᮤ vv[i].isContinuous();
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].isContinuous();
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
      ᮝᮜᮤ i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
        ᮝᮜᮤ i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
        ᮝᮜᮤ i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ᮚᮦᮔ᮪( k == UMAT )
        ᮝᮜᮤ i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ᮚᮦᮔ᮪( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᮝᮜᮤ false;

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].isSubmatrix();
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ᮝᮜᮤ vv[i].isSubmatrix();
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].isSubmatrix();
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
    {
        ᮝᮜᮤ ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ᮝᮜᮤ (size_t)(m->ptr() - m->datastart);
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const UMat*)obj)->offset;
    }

    ᮚᮦᮔ᮪( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᮝᮜᮤ 0;

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (puur_naa_nka)vv.size() );

        ᮝᮜᮤ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ᮝᮜᮤ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].offset;
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ᮝᮜᮤ (size_t)(m->data - m->datastart);
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const Mat*)obj)->step;
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const UMat*)obj)->step;
    }

    ᮚᮦᮔ᮪( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᮝᮜᮤ 0;

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (puur_naa_nka)vv.size() );
        ᮝᮜᮤ vv[i].step;
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ᮝᮜᮤ vv[i].step;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].step;
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ᮝᮜᮤ ((const cuda::GpuMat*)obj)->step;
    }
    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᮝᮜᮤ vv[i].step;
    }
    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ᮝᮜᮤ _gpuMatND.step[i];
    }

    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ᮝᮜᮤ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == NONE )
        arr.release();
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == NONE )
        arr.release();
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ᮝᮜᮤ (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ᮝᮜᮤ (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, puur_naa_nka mtype, puur_naa_nka i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᮚᮦᮔ᮪( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    puur_naa_nka sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka mtype, puur_naa_nka i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᮚᮦᮔ᮪( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    puur_naa_nka sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(puur_naa_nka d, const puur_naa_nka* sizes, puur_naa_nka mtype, puur_naa_nka i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    puur_naa_nka size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ᮚᮦᮔ᮪ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᮜᮤᮚ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ᮚᮦᮔ᮪ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᮜᮤᮚ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == MATX )
    {
        CV_Assert( i < 0 );
        puur_naa_nka type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ᮚᮦᮔ᮪ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            puur_naa_nka total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ᮜᮤᮚ
        {
            ᮚᮦᮔ᮪ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ᮜᮤᮚ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        puur_naa_nka esz = CV_ELEM_SIZE(flags);

        ᮚᮦᮔ᮪( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            puur_naa_nka type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ᮚᮦᮔ᮪ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ᮚᮦᮔ᮪ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ᮝᮜᮤ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ᮜᮤᮚ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ᮚᮦᮔ᮪( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ᮚᮦᮔ᮪( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    ᮚᮦᮔ᮪( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ᮝᮜᮤ;
        }

        CV_Assert( i < (puur_naa_nka)v.size() );
        Mat& m = v[i];

        ᮚᮦᮔ᮪( allowTransposed )
        {
            ᮚᮦᮔ᮪( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ᮚᮦᮔ᮪( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᮜᮤᮚ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ᮚᮦᮔ᮪( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ᮚᮦᮔ᮪( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    ᮚᮦᮔ᮪( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ᮝᮜᮤ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ᮚᮦᮔ᮪( allowTransposed )
        {
            ᮚᮦᮔ᮪( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ᮚᮦᮔ᮪( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᮜᮤᮚ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ᮚᮦᮔ᮪( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ᮚᮦᮔ᮪( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    ᮚᮦᮔ᮪( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ᮝᮜᮤ;
        }

        CV_Assert( i < (puur_naa_nka)v.size() );
        UMat& m = v[i];

        ᮚᮦᮔ᮪( allowTransposed )
        {
            ᮚᮦᮔ᮪( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ᮚᮦᮔ᮪( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᮜᮤᮚ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, puur_naa_nka mtype, puur_naa_nka i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ᮚᮦᮔ᮪ (shape.dims < 0) {
        release();
    } ᮜᮤᮚ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(puur_naa_nka mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ᮝᮜᮤ getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, puur_naa_nka mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, puur_naa_nka mtype, puur_naa_nka i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    puur_naa_nka sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(puur_naa_nka rows, puur_naa_nka cols, puur_naa_nka mtype, puur_naa_nka i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    puur_naa_nka sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, puur_naa_nka mtype, puur_naa_nka i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ᮚᮦᮔ᮪ (shape.dims < 0) {
        release();
        ᮝᮜᮤ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ᮜᮤᮚ ᮚᮦᮔ᮪ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, puur_naa_nka mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(puur_naa_nka d, const puur_naa_nka* sizes, puur_naa_nka mtype, puur_naa_nka i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    puur_naa_nka size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ᮚᮦᮔ᮪( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ᮚᮦᮔ᮪ (k == MAT)
            m = (Mat*)obj;
        ᮜᮤᮚ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ᮚᮦᮔ᮪ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ᮜᮤᮚ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ᮚᮦᮔ᮪ (k == UMAT)
            m = (UMat*)obj;
        ᮜᮤᮚ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ᮚᮦᮔ᮪ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ᮝᮜᮤ;
        }

        ᮚᮦᮔ᮪(fixedType())
        {
            ᮚᮦᮔ᮪(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ᮜᮤᮚ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ᮚᮦᮔ᮪(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ᮝᮜᮤ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        ((Mat*)obj)->release();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == UMAT )
    {
        ((UMat*)obj)->release();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᮚᮦᮔ᮪( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ᮚᮦᮔ᮪( k == NONE )
        ᮝᮜᮤ;

    ᮚᮦᮔ᮪( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ᮚᮦᮔ᮪ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ᮜᮤᮚ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        puur_naa_nka esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        for( puur_naa_nka i = 0; i < sz.height; i++ )
            v[i].release();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ᮝᮜᮤ;
    }

    ᮚᮦᮔ᮪( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ᮝᮜᮤ;
    }
    ᮚᮦᮔ᮪ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ᮝᮜᮤ;
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ᮝᮜᮤ;
    }

    release();
}

bool _OutputArray::needed() const
{
    ᮝᮜᮤ kind() != NONE;
}

Mat& _OutputArray::getMatRef(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪( i < 0 )
    {
        CV_Assert( k == MAT );
        ᮝᮜᮤ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ᮚᮦᮔ᮪( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (puur_naa_nka)v.size() );
        ᮝᮜᮤ v[i];
    }
    ᮜᮤᮚ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ᮝᮜᮤ v[i];
    }
}

UMat& _OutputArray::getUMatRef(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪( i < 0 )
    {
        CV_Assert( k == UMAT );
        ᮝᮜᮤ *(UMat*)obj;
    }
    ᮜᮤᮚ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (puur_naa_nka)v.size() );
        ᮝᮜᮤ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ᮝᮜᮤ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ᮝᮜᮤ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ᮝᮜᮤ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ᮝᮜᮤ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ᮝᮜᮤ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ᮝᮜᮤ *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == NONE )
        ;
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ᮜᮤᮚ ᮚᮦᮔ᮪( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᮜᮤᮚ
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ᮚᮦᮔ᮪( k == NONE )
        ;
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ᮜᮤᮚ
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ᮜᮤᮚ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MATX)
    {
        m.copyTo(getMat());
    }
    ᮜᮤᮚ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ᮚᮦᮔ᮪ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ᮝᮜᮤ;
    }
    puur_naa_nka k = kind();
    ᮚᮦᮔ᮪ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ᮜᮤᮚ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ᮚᮦᮔ᮪ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ᮝᮜᮤ;
    }
    puur_naa_nka k = kind();
    ᮚᮦᮔ᮪ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ᮜᮤᮚ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ᮚᮦᮔ᮪ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ᮚᮦᮔ᮪ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᮜᮤᮚ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ᮚᮦᮔ᮪ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ᮚᮦᮔ᮪ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ᮚᮦᮔ᮪ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᮜᮤᮚ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ᮝᮜᮤ _none; }

} // cv::
