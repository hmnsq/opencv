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

    ꦪꦼꦤ꧀( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ *m;
        ꦮꦭꦶ m->row(i);
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ m->getMat(accessFlags);
        ꦮꦭꦶ m->getMat(accessFlags).row(i);
    }

    ꦪꦼꦤ꧀ (k == MATX)
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ Mat(sz, flags, obj);
    }

    ꦪꦼꦤ꧀( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        puur_naa_nka t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        puur_naa_nka j, n = (puur_naa_nka)v.size();
        ꦪꦼꦤ꧀( n == 0 )
            ꦮꦭꦶ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        ꦏꦁꦒꦺꦴ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ꦮꦭꦶ m;
    }

    ꦪꦼꦤ꧀( k == NONE )
        ꦮꦭꦶ Mat();

    ꦪꦼꦤ꧀( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        puur_naa_nka t = CV_MAT_TYPE(flags);
        puur_naa_nka esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ꦪꦼꦤ꧀ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ꦭꦶꦪ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            puur_naa_nka v_sz = puur_naa_nka(v->size()); \
            ꦮꦭꦶ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ꦮꦭꦶ v[i];
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ꦮꦭꦶ v[i];
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ꦮꦭꦶ v[i].getMat(accessFlags);
    }

    ꦪꦼꦤ꧀( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ꦮꦭꦶ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ꦪꦼꦤ꧀( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ *m;
        ꦮꦭꦶ m->row(i);
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ꦮꦭꦶ v[i];
    }

    ꦪꦼꦤ꧀( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ m->getUMat(accessFlags);
        ꦮꦭꦶ m->row(i).getUMat(accessFlags);
    }

    ꦮꦭꦶ getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ꦪꦼꦤ꧀( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        puur_naa_nka n = (puur_naa_nka)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        ꦏꦁꦒꦺꦴ( puur_naa_nka i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        puur_naa_nka t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == NONE )
    {
        mv.clear();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_VECTOR )
    {
        puur_naa_nka typ = CV_MAT_TYPE(flags);
        puur_naa_nka esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            ꦏꦁꦒꦺꦴ (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                puur_naa_nka ni = (puur_naa_nka)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ꦮꦭꦶ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ꦪꦼꦤ꧀( k == NONE )
    {
        umv.clear();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ꦏꦁꦒꦺꦴ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ꦮꦭꦶ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀ (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ꦮꦭꦶ *d_mat;
    }

    ꦪꦼꦤ꧀ (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ꦮꦭꦶ cuda_mem->createGpuMatHeader();
    }

    ꦪꦼꦤ꧀ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ꦪꦼꦤ꧀ (k == NONE)
        ꦮꦭꦶ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ꦭꦶꦪ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ꦭꦶꦪ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ꦭꦶꦪ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ꦮꦭꦶ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ꦭꦶꦪ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ꦮꦭꦶ *gl_buf;
}

puur_naa_nka _InputArray::rows_(puur_naa_nka i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ꦮꦭꦶ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ꦮꦭꦶ size_(i).height;
}

puur_naa_nka _InputArray::cols_(puur_naa_nka i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ꦮꦭꦶ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ꦮꦭꦶ size_(i).width;
}

Size _InputArray::size_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ꦮꦭꦶ Size(m->cols, m->rows);
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ꦮꦭꦶ Size(m->cols, m->rows);
    }

    ꦪꦼꦤ꧀ (k == MATX)
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ sz;
    }

    ꦪꦼꦤ꧀( k == NONE )
        ꦮꦭꦶ Size();

    ꦪꦼꦤ꧀( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ꦮꦭꦶ Size((puur_naa_nka)n, 1);
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ꦮꦭꦶ vv[i].size();
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ꦮꦭꦶ vv[i].size();
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ꦪꦼꦤ꧀ (i < 0)
            ꦮꦭꦶ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        ꦮꦭꦶ vv[i].size();
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ꦪꦼꦤ꧀ (i < 0)
            ꦮꦭꦶ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ꦪꦼꦤ꧀ (m.dims == 0)
            ꦮꦭꦶ Size();
        ꦪꦼꦤ꧀ (m.dims == 1)
            ꦮꦭꦶ Size(1, m.size[0]);
        ꦮꦭꦶ Size(m.size[1], m.size[0]);
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ꦮꦭꦶ vv[i].size();
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ꦪꦼꦤ꧀ (i < 0)
            ꦮꦭꦶ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ꦪꦼꦤ꧀ (m.dims == 0)
            ꦮꦭꦶ Size();
        ꦪꦼꦤ꧀ (m.dims == 1)
            ꦮꦭꦶ Size(1, m.size[0]);
        ꦮꦭꦶ Size(m.size[1], m.size[0]);
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ꦮꦭꦶ buf->size();
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ꦮꦭꦶ d_mat->size();
    }

    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ꦮꦭꦶ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

puur_naa_nka _InputArray::sizend(puur_naa_nka* arrsz, puur_naa_nka i) const
{
    puur_naa_nka j, d = 0;
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == NONE )
        ;
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ꦪꦼꦤ꧀(arrsz)
            ꦏꦁꦒꦺꦴ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ꦪꦼꦤ꧀(arrsz)
            ꦏꦁꦒꦺꦴ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (puur_naa_nka)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ꦪꦼꦤ꧀(arrsz)
            ꦏꦁꦒꦺꦴ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ꦪꦼꦤ꧀(arrsz)
            ꦏꦁꦒꦺꦴ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (puur_naa_nka)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ꦪꦼꦤ꧀(arrsz)
            ꦏꦁꦒꦺꦴ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ꦪꦼꦤ꧀(arrsz)
        {
            arrsz[0] = puur_naa_nka(n);
        }
    }
    ꦭꦶꦪ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ꦪꦼꦤ꧀(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ꦮꦭꦶ d;
}

bool _InputArray::empty(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (i >= 0) {
        ꦪꦼꦤ꧀ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ꦮꦭꦶ mv->at(i).empty();
        }
        ꦭꦶꦪ ꦪꦼꦤ꧀ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ꦮꦭꦶ umv->at(i).empty();
        }
        ꦭꦶꦪ ꦪꦼꦤ꧀ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ꦮꦭꦶ n == 0;
        } ꦭꦶꦪ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ꦮꦭꦶ empty();
}

MatShape _InputArray::shape(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ꦪꦼꦤ꧀( k == NONE )
        ;
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    ꦭꦶꦪ
    {
        puur_naa_nka sizes[CV_MAX_DIM];
        puur_naa_nka dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ꦮꦭꦶ shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ꦪꦼꦤ꧀( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ꦪꦼꦤ꧀( k2 == MAT )
            ꦮꦭꦶ m->size == ((const Mat*)arr.obj)->size;
        ꦪꦼꦤ꧀( k2 == UMAT )
            ꦮꦭꦶ m->size == ((const UMat*)arr.obj)->size;
        ꦪꦼꦤ꧀( m->dims > 2 )
            ꦮꦭꦶ false;
        sz1 = m->size();
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ꦪꦼꦤ꧀( k2 == MAT )
            ꦮꦭꦶ m->size == ((const Mat*)arr.obj)->size;
        ꦪꦼꦤ꧀( k2 == UMAT )
            ꦮꦭꦶ m->size == ((const UMat*)arr.obj)->size;
        ꦪꦼꦤ꧀( m->dims > 2 )
            ꦮꦭꦶ false;
        sz1 = m->size();
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ꦮꦭꦶ ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    ꦭꦶꦪ
        sz1 = size();

    ꦪꦼꦤ꧀( arr.dims() > 2 )
        ꦮꦭꦶ false;
    ꦮꦭꦶ sz1 == arr.size();
}

puur_naa_nka _InputArray::dims_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const Mat*)obj)->dims;
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const UMat*)obj)->dims;
    }

    ꦪꦼꦤ꧀ (k == MATX)
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ 2;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ 1;
    }

    ꦪꦼꦤ꧀( k == NONE )
        ꦮꦭꦶ 0;

    ꦪꦼꦤ꧀( k == STD_VECTOR_VECTOR )
    {
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ 1;
        CV_Assert( size_t(i) < total() );
        ꦮꦭꦶ 1;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ 1;
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ꦮꦭꦶ vv[i].dims;
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ 1;
        CV_Assert( i < sz.height );

        ꦮꦭꦶ vv[i].dims;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ 1;
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ꦮꦭꦶ vv[i].dims;
    }

    ꦪꦼꦤ꧀( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ 2;
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ 2;
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const cuda::GpuMatND*)obj)->dims;
    }

    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const Mat*)obj)->total();
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const UMat*)obj)->total();
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ vv.size();

        CV_Assert( i < (puur_naa_nka)vv.size() );
        ꦮꦭꦶ vv[i].total();
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ sz.height;

        CV_Assert( i < sz.height );
        ꦮꦭꦶ vv[i].total();
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ꦪꦼꦤ꧀( i < 0 )
            ꦮꦭꦶ vv.size();

        CV_Assert( i < (puur_naa_nka)vv.size() );
        ꦮꦭꦶ vv[i].total();
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ꦪꦼꦤ꧀ (_gpuMatND.dims > 0)
        {
            res = 1;
            ꦏꦁꦒꦺꦴ(puur_naa_nka d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ꦮꦭꦶ res;
        }
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        puur_naa_nka esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ꦪꦼꦤ꧀ (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ꦪꦼꦤ꧀ (i < 0) \
                    ꦮꦭꦶ n; \
                CV_Assert((size_t)i < n); \
                ꦮꦭꦶ vv->at(i).size(); \
            } ꦭꦶꦪ \
                ꦮꦭꦶ ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ꦮꦭꦶ size(i).area();
}

puur_naa_nka _InputArray::type_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
        ꦮꦭꦶ ((const Mat*)obj)->type();

    ꦪꦼꦤ꧀( k == UMAT )
        ꦮꦭꦶ ((const UMat*)obj)->type();

    ꦪꦼꦤ꧀( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ꦮꦭꦶ CV_MAT_TYPE(flags);

    ꦪꦼꦤ꧀( k == NONE )
        ꦮꦭꦶ -1;

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ꦪꦼꦤ꧀( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ꦮꦭꦶ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (puur_naa_nka)vv.size() );
        ꦮꦭꦶ vv[i >= 0 ? i : 0].type();
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ꦪꦼꦤ꧀( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ꦮꦭꦶ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (puur_naa_nka)vv.size() );
        ꦮꦭꦶ vv[i >= 0 ? i : 0].type();
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ꦪꦼꦤ꧀( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ꦮꦭꦶ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ꦮꦭꦶ vv[i >= 0 ? i : 0].type();
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ꦪꦼꦤ꧀ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ꦮꦭꦶ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (puur_naa_nka)vv.size());
        ꦮꦭꦶ vv[i >= 0 ? i : 0].type();
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ꦪꦼꦤ꧀ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ꦮꦭꦶ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (puur_naa_nka)vv.size());
        ꦮꦭꦶ vv[i >= 0 ? i : 0].type();
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀( k == OPENGL_BUFFER )
        ꦮꦭꦶ ((const ogl::Buffer*)obj)->type();

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
        ꦮꦭꦶ ((const cuda::GpuMat*)obj)->type();

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
        ꦮꦭꦶ ((const cuda::GpuMatND*)obj)->type();

    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM )
        ꦮꦭꦶ ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

puur_naa_nka _InputArray::depth_(puur_naa_nka i) const
{
    ꦮꦭꦶ CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
        ꦮꦭꦶ ((const Mat*)obj)->empty();

    ꦪꦼꦤ꧀( k == UMAT )
        ꦮꦭꦶ ((const UMat*)obj)->empty();

    ꦪꦼꦤ꧀ (k == MATX)
        ꦮꦭꦶ false;

    ꦪꦼꦤ꧀( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ꦮꦭꦶ n == 0;
    }

    ꦪꦼꦤ꧀( k == NONE )
        ꦮꦭꦶ true;

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ꦮꦭꦶ vv.empty();
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        ꦮꦭꦶ sz.height == 0;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ꦮꦭꦶ vv.empty();
    }

    ꦪꦼꦤ꧀( k == OPENGL_BUFFER )
        ꦮꦭꦶ ((const ogl::Buffer*)obj)->empty();

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
        ꦮꦭꦶ ((const cuda::GpuMat*)obj)->empty();

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ꦮꦭꦶ vv.empty();
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
        ꦮꦭꦶ ((const cuda::GpuMatND*)obj)->empty();

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ꦮꦭꦶ vv.empty();
    }

    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM )
        ꦮꦭꦶ ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
        ꦮꦭꦶ i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ꦪꦼꦤ꧀( k == UMAT )
        ꦮꦭꦶ i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ꦪꦼꦤ꧀( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ꦮꦭꦶ true;

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].isContinuous();
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ꦮꦭꦶ vv[i].isContinuous();
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].isContinuous();
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
      ꦮꦭꦶ i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
        ꦮꦭꦶ i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
        ꦮꦭꦶ i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ꦪꦼꦤ꧀( k == UMAT )
        ꦮꦭꦶ i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ꦪꦼꦤ꧀( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ꦮꦭꦶ false;

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].isSubmatrix();
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ꦮꦭꦶ vv[i].isSubmatrix();
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].isSubmatrix();
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
    {
        ꦮꦭꦶ ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ꦮꦭꦶ (size_t)(m->ptr() - m->datastart);
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const UMat*)obj)->offset;
    }

    ꦪꦼꦤ꧀( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ꦮꦭꦶ 0;

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (puur_naa_nka)vv.size() );

        ꦮꦭꦶ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ꦮꦭꦶ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].offset;
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ꦮꦭꦶ (size_t)(m->data - m->datastart);
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const Mat*)obj)->step;
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const UMat*)obj)->step;
    }

    ꦪꦼꦤ꧀( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ꦮꦭꦶ 0;

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (puur_naa_nka)vv.size() );
        ꦮꦭꦶ vv[i].step;
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ꦮꦭꦶ vv[i].step;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].step;
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ꦮꦭꦶ ((const cuda::GpuMat*)obj)->step;
    }
    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ꦮꦭꦶ vv[i].step;
    }
    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ꦮꦭꦶ _gpuMatND.step[i];
    }

    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ꦮꦭꦶ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == NONE )
        arr.release();
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == NONE )
        arr.release();
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ꦮꦭꦶ (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ꦮꦭꦶ (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, puur_naa_nka mtype, puur_naa_nka i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ꦪꦼꦤ꧀( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    puur_naa_nka sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka mtype, puur_naa_nka i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ꦪꦼꦤ꧀( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
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

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ꦪꦼꦤ꧀ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ꦭꦶꦪ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ꦪꦼꦤ꧀ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ꦭꦶꦪ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == MATX )
    {
        CV_Assert( i < 0 );
        puur_naa_nka type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ꦪꦼꦤ꧀ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            puur_naa_nka total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ꦭꦶꦪ
        {
            ꦪꦼꦤ꧀ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ꦭꦶꦪ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        puur_naa_nka esz = CV_ELEM_SIZE(flags);

        ꦪꦼꦤ꧀( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            puur_naa_nka type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ꦪꦼꦤ꧀ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ꦪꦼꦤ꧀ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ꦮꦭꦶ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ꦭꦶꦪ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ꦪꦼꦤ꧀( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ꦪꦼꦤ꧀( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                ꦏꦁꦒꦺꦴ( size_t j = len0; j < len; j++ )
                {
                    ꦪꦼꦤ꧀( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ꦮꦭꦶ;
        }

        CV_Assert( i < (puur_naa_nka)v.size() );
        Mat& m = v[i];

        ꦪꦼꦤ꧀( allowTransposed )
        {
            ꦪꦼꦤ꧀( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ꦪꦼꦤ꧀( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ꦭꦶꦪ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_Assert(m.dims == d);
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ꦪꦼꦤ꧀( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ꦪꦼꦤ꧀( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                ꦏꦁꦒꦺꦴ( size_t j = len0; j < len; j++ )
                {
                    ꦪꦼꦤ꧀( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ꦮꦭꦶ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ꦪꦼꦤ꧀( allowTransposed )
        {
            ꦪꦼꦤ꧀( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ꦪꦼꦤ꧀( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ꦭꦶꦪ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_Assert(m.dims == d);
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ꦪꦼꦤ꧀( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ꦪꦼꦤ꧀( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                ꦏꦁꦒꦺꦴ( size_t j = len0; j < len; j++ )
                {
                    ꦪꦼꦤ꧀( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ꦮꦭꦶ;
        }

        CV_Assert( i < (puur_naa_nka)v.size() );
        UMat& m = v[i];

        ꦪꦼꦤ꧀( allowTransposed )
        {
            ꦪꦼꦤ꧀( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ꦪꦼꦤ꧀( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ꦭꦶꦪ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_Assert(m.dims == d);
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, puur_naa_nka mtype, puur_naa_nka i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ꦪꦼꦤ꧀ (shape.dims < 0) {
        release();
    } ꦭꦶꦪ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(puur_naa_nka mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ꦮꦭꦶ getMat().reinterpret(mtype);
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
    ꦪꦼꦤ꧀ (shape.dims < 0) {
        release();
        ꦮꦭꦶ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ꦭꦶꦪ ꦪꦼꦤ꧀ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
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

    ꦪꦼꦤ꧀( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ꦪꦼꦤ꧀ (k == MAT)
            m = (Mat*)obj;
        ꦭꦶꦪ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ꦪꦼꦤ꧀ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ꦭꦶꦪ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ꦪꦼꦤ꧀ (k == UMAT)
            m = (UMat*)obj;
        ꦭꦶꦪ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ꦪꦼꦤ꧀ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ꦮꦭꦶ;
        }

        ꦪꦼꦤ꧀(fixedType())
        {
            ꦪꦼꦤ꧀(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ꦭꦶꦪ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ꦪꦼꦤ꧀(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ꦏꦁꦒꦺꦴ(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ꦮꦭꦶ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        ((Mat*)obj)->release();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == UMAT )
    {
        ((UMat*)obj)->release();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ꦪꦼꦤ꧀( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ꦪꦼꦤ꧀( k == NONE )
        ꦮꦭꦶ;

    ꦪꦼꦤ꧀( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ꦪꦼꦤ꧀ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ꦭꦶꦪ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        puur_naa_nka esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        ꦏꦁꦒꦺꦴ( puur_naa_nka i = 0; i < sz.height; i++ )
            v[i].release();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ꦮꦭꦶ;
    }

    ꦪꦼꦤ꧀( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ꦮꦭꦶ;
    }
    ꦪꦼꦤ꧀ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ꦮꦭꦶ;
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ꦮꦭꦶ;
    }

    release();
}

bool _OutputArray::needed() const
{
    ꦮꦭꦶ kind() != NONE;
}

Mat& _OutputArray::getMatRef(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀( i < 0 )
    {
        CV_Assert( k == MAT );
        ꦮꦭꦶ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ꦪꦼꦤ꧀( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (puur_naa_nka)v.size() );
        ꦮꦭꦶ v[i];
    }
    ꦭꦶꦪ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ꦮꦭꦶ v[i];
    }
}

UMat& _OutputArray::getUMatRef(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀( i < 0 )
    {
        CV_Assert( k == UMAT );
        ꦮꦭꦶ *(UMat*)obj;
    }
    ꦭꦶꦪ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (puur_naa_nka)v.size() );
        ꦮꦭꦶ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ꦮꦭꦶ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ꦮꦭꦶ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ꦮꦭꦶ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ꦮꦭꦶ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ꦮꦭꦶ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ꦮꦭꦶ *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == NONE )
        ;
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ꦭꦶꦪ ꦪꦼꦤ꧀( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ꦭꦶꦪ
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ꦪꦼꦤ꧀( k == NONE )
        ;
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ꦭꦶꦪ
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ꦭꦶꦪ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MATX)
    {
        m.copyTo(getMat());
    }
    ꦭꦶꦪ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ꦪꦼꦤ꧀ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ꦮꦭꦶ;
    }
    puur_naa_nka k = kind();
    ꦪꦼꦤ꧀ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ꦭꦶꦪ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ꦪꦼꦤ꧀ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ꦮꦭꦶ;
    }
    puur_naa_nka k = kind();
    ꦪꦼꦤ꧀ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ꦭꦶꦪ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ꦏꦁꦒꦺꦴ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ꦪꦼꦤ꧀ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ꦏꦁꦒꦺꦴ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ꦪꦼꦤ꧀ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ꦭꦶꦪ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ꦪꦼꦤ꧀ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ꦏꦁꦒꦺꦴ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ꦪꦼꦤ꧀ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ꦭꦶꦪ ꦪꦼꦤ꧀ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ꦏꦁꦒꦺꦴ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ꦪꦼꦤ꧀ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ꦭꦶꦪ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ꦮꦭꦶ _none; }

} // cv::
