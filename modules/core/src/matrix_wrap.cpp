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

    ᨀᨘ( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ *m;
        ᨀᨘᨒ m->row(i);
    }

    ᨀᨘ( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ m->getMat(accessFlags);
        ᨀᨘᨒ m->getMat(accessFlags).row(i);
    }

    ᨀᨘ (k == MATX)
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ Mat(sz, flags, obj);
    }

    ᨀᨘ( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        puur_naa_nka t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        puur_naa_nka j, n = (puur_naa_nka)v.size();
        ᨀᨘ( n == 0 )
            ᨀᨘᨒ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        for( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ᨀᨘᨒ m;
    }

    ᨀᨘ( k == NONE )
        ᨀᨘᨒ Mat();

    ᨀᨘ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        puur_naa_nka t = CV_MAT_TYPE(flags);
        puur_naa_nka esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ᨀᨘ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ᨀᨘᨁ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            puur_naa_nka v_sz = puur_naa_nka(v->size()); \
            ᨀᨘᨒ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ᨀᨘᨒ v[i];
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ᨀᨘᨒ v[i];
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ᨀᨘᨒ v[i].getMat(accessFlags);
    }

    ᨀᨘ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ᨀᨘ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ᨀᨘ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ᨀᨘᨒ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᨀᨘ( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ *m;
        ᨀᨘᨒ m->row(i);
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (puur_naa_nka)v.size() );

        ᨀᨘᨒ v[i];
    }

    ᨀᨘ( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ m->getUMat(accessFlags);
        ᨀᨘᨒ m->row(i).getUMat(accessFlags);
    }

    ᨀᨘᨒ getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᨀᨘ( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        puur_naa_nka n = (puur_naa_nka)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        for( puur_naa_nka i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ᨀᨘᨒ;
    }

    ᨀᨘ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        puur_naa_nka t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == NONE )
    {
        mv.clear();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_VECTOR )
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
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ᨀᨘᨒ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ᨀᨘ( k == NONE )
    {
        umv.clear();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ᨀᨘᨒ;
    }
    ᨀᨘ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ᨀᨘᨒ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ᨀᨘ (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ᨀᨘᨒ *d_mat;
    }

    ᨀᨘ (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ᨀᨘᨒ cuda_mem->createGpuMatHeader();
    }

    ᨀᨘ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ᨀᨘ (k == NONE)
        ᨀᨘᨒ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ᨀᨘᨁ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ᨀᨘᨁ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ᨀᨘᨁ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ᨀᨘ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ᨀᨘᨒ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ᨀᨘᨁ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ᨀᨘᨒ *gl_buf;
}

puur_naa_nka _InputArray::rows_(puur_naa_nka i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ᨀᨘᨒ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ᨀᨘᨒ size_(i).height;
}

puur_naa_nka _InputArray::cols_(puur_naa_nka i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ᨀᨘᨒ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ᨀᨘᨒ size_(i).width;
}

Size _InputArray::size_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ᨀᨘᨒ Size(m->cols, m->rows);
    }

    ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ᨀᨘᨒ Size(m->cols, m->rows);
    }

    ᨀᨘ (k == MATX)
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ sz;
    }

    ᨀᨘ( k == NONE )
        ᨀᨘᨒ Size();

    ᨀᨘ( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ᨀᨘᨒ Size((puur_naa_nka)n, 1);
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᨀᨘᨒ vv[i].size();
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ᨀᨘᨒ vv[i].size();
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ᨀᨘ (i < 0)
            ᨀᨘᨒ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        ᨀᨘᨒ vv[i].size();
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᨀᨘ (i < 0)
            ᨀᨘᨒ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ᨀᨘ (m.dims == 0)
            ᨀᨘᨒ Size();
        ᨀᨘ (m.dims == 1)
            ᨀᨘᨒ Size(1, m.size[0]);
        ᨀᨘᨒ Size(m.size[1], m.size[0]);
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᨀᨘᨒ vv[i].size();
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᨀᨘ (i < 0)
            ᨀᨘᨒ vv.empty() ? Size() : Size((puur_naa_nka)vv.size(), 1);
        CV_Assert(i < (puur_naa_nka)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ᨀᨘ (m.dims == 0)
            ᨀᨘᨒ Size();
        ᨀᨘ (m.dims == 1)
            ᨀᨘᨒ Size(1, m.size[0]);
        ᨀᨘᨒ Size(m.size[1], m.size[0]);
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ᨀᨘᨒ buf->size();
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ᨀᨘᨒ d_mat->size();
    }

    ᨀᨘ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ᨀᨘᨒ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

puur_naa_nka _InputArray::sizend(puur_naa_nka* arrsz, puur_naa_nka i) const
{
    puur_naa_nka j, d = 0;
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == NONE )
        ;
    ᨀᨘᨁ ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ᨀᨘ(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᨀᨘᨁ ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ᨀᨘ(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᨀᨘᨁ ᨀᨘ( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (puur_naa_nka)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ᨀᨘ(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᨀᨘᨁ ᨀᨘ( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ᨀᨘ(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᨀᨘᨁ ᨀᨘ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (puur_naa_nka)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ᨀᨘ(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ᨀᨘᨁ ᨀᨘ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ᨀᨘ(arrsz)
        {
            arrsz[0] = puur_naa_nka(n);
        }
    }
    ᨀᨘᨁ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ᨀᨘ(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ᨀᨘᨒ d;
}

bool _InputArray::empty(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ (i >= 0) {
        ᨀᨘ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ᨀᨘᨒ mv->at(i).empty();
        }
        ᨀᨘᨁ ᨀᨘ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ᨀᨘᨒ umv->at(i).empty();
        }
        ᨀᨘᨁ ᨀᨘ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ᨀᨘᨒ n == 0;
        } ᨀᨘᨁ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ᨀᨘᨒ empty();
}

MatShape _InputArray::shape(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ᨀᨘ( k == NONE )
        ;
    ᨀᨘᨁ ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    ᨀᨘᨁ ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    ᨀᨘᨁ ᨀᨘ( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ᨀᨘᨁ ᨀᨘ( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ᨀᨘᨁ ᨀᨘ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    ᨀᨘᨁ
    {
        puur_naa_nka sizes[CV_MAX_DIM];
        puur_naa_nka dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ᨀᨘᨒ shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ᨀᨘ( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ᨀᨘ( k2 == MAT )
            ᨀᨘᨒ m->size == ((const Mat*)arr.obj)->size;
        ᨀᨘ( k2 == UMAT )
            ᨀᨘᨒ m->size == ((const UMat*)arr.obj)->size;
        ᨀᨘ( m->dims > 2 )
            ᨀᨘᨒ false;
        sz1 = m->size();
    }
    ᨀᨘᨁ ᨀᨘ( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ᨀᨘ( k2 == MAT )
            ᨀᨘᨒ m->size == ((const Mat*)arr.obj)->size;
        ᨀᨘ( k2 == UMAT )
            ᨀᨘᨒ m->size == ((const UMat*)arr.obj)->size;
        ᨀᨘ( m->dims > 2 )
            ᨀᨘᨒ false;
        sz1 = m->size();
    }
    ᨀᨘᨁ ᨀᨘ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ᨀᨘᨒ ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    ᨀᨘᨁ
        sz1 = size();

    ᨀᨘ( arr.dims() > 2 )
        ᨀᨘᨒ false;
    ᨀᨘᨒ sz1 == arr.size();
}

puur_naa_nka _InputArray::dims_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const Mat*)obj)->dims;
    }

    ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const UMat*)obj)->dims;
    }

    ᨀᨘ (k == MATX)
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ 2;
    }

    ᨀᨘ( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ 1;
    }

    ᨀᨘ( k == NONE )
        ᨀᨘᨒ 0;

    ᨀᨘ( k == STD_VECTOR_VECTOR )
    {
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ 1;
        CV_Assert( size_t(i) < total() );
        ᨀᨘᨒ 1;
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ 1;
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᨀᨘᨒ vv[i].dims;
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ 1;
        CV_Assert( i < sz.height );

        ᨀᨘᨒ vv[i].dims;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ 1;
        CV_Assert( i < (puur_naa_nka)vv.size() );

        ᨀᨘᨒ vv[i].dims;
    }

    ᨀᨘ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ 2;
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ 2;
    }

    ᨀᨘ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const cuda::GpuMatND*)obj)->dims;
    }

    ᨀᨘ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const Mat*)obj)->total();
    }

    ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const UMat*)obj)->total();
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ vv.size();

        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᨀᨘᨒ vv[i].total();
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ sz.height;

        CV_Assert( i < sz.height );
        ᨀᨘᨒ vv[i].total();
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᨀᨘ( i < 0 )
            ᨀᨘᨒ vv.size();

        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᨀᨘᨒ vv[i].total();
    }

    ᨀᨘ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ᨀᨘ (_gpuMatND.dims > 0)
        {
            res = 1;
            for(puur_naa_nka d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ᨀᨘᨒ res;
        }
    }

    ᨀᨘ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        puur_naa_nka esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ᨀᨘ (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ᨀᨘ (i < 0) \
                    ᨀᨘᨒ n; \
                CV_Assert((size_t)i < n); \
                ᨀᨘᨒ vv->at(i).size(); \
            } ᨀᨘᨁ \
                ᨀᨘᨒ ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ᨀᨘᨒ size(i).area();
}

puur_naa_nka _InputArray::type_(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
        ᨀᨘᨒ ((const Mat*)obj)->type();

    ᨀᨘ( k == UMAT )
        ᨀᨘᨒ ((const UMat*)obj)->type();

    ᨀᨘ( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᨀᨘᨒ CV_MAT_TYPE(flags);

    ᨀᨘ( k == NONE )
        ᨀᨘᨒ -1;

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᨀᨘ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᨀᨘᨒ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᨀᨘᨒ vv[i >= 0 ? i : 0].type();
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᨀᨘ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᨀᨘᨒ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (puur_naa_nka)vv.size() );
        ᨀᨘᨒ vv[i >= 0 ? i : 0].type();
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ᨀᨘ( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᨀᨘᨒ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ᨀᨘᨒ vv[i >= 0 ? i : 0].type();
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ᨀᨘ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᨀᨘᨒ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (puur_naa_nka)vv.size());
        ᨀᨘᨒ vv[i >= 0 ? i : 0].type();
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᨀᨘ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ᨀᨘᨒ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (puur_naa_nka)vv.size());
        ᨀᨘᨒ vv[i >= 0 ? i : 0].type();
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ( k == OPENGL_BUFFER )
        ᨀᨘᨒ ((const ogl::Buffer*)obj)->type();

    ᨀᨘ( k == CUDA_GPU_MAT )
        ᨀᨘᨒ ((const cuda::GpuMat*)obj)->type();

    ᨀᨘ( k == CUDA_GPU_MATND )
        ᨀᨘᨒ ((const cuda::GpuMatND*)obj)->type();

    ᨀᨘ( k == CUDA_HOST_MEM )
        ᨀᨘᨒ ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

puur_naa_nka _InputArray::depth_(puur_naa_nka i) const
{
    ᨀᨘᨒ CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
        ᨀᨘᨒ ((const Mat*)obj)->empty();

    ᨀᨘ( k == UMAT )
        ᨀᨘᨒ ((const UMat*)obj)->empty();

    ᨀᨘ (k == MATX)
        ᨀᨘᨒ false;

    ᨀᨘ( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ᨀᨘᨒ n == 0;
    }

    ᨀᨘ( k == NONE )
        ᨀᨘᨒ true;

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ᨀᨘᨒ vv.empty();
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        ᨀᨘᨒ sz.height == 0;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ᨀᨘᨒ vv.empty();
    }

    ᨀᨘ( k == OPENGL_BUFFER )
        ᨀᨘᨒ ((const ogl::Buffer*)obj)->empty();

    ᨀᨘ( k == CUDA_GPU_MAT )
        ᨀᨘᨒ ((const cuda::GpuMat*)obj)->empty();

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ᨀᨘᨒ vv.empty();
    }

    ᨀᨘ( k == CUDA_GPU_MATND )
        ᨀᨘᨒ ((const cuda::GpuMatND*)obj)->empty();

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ᨀᨘᨒ vv.empty();
    }

    ᨀᨘ( k == CUDA_HOST_MEM )
        ᨀᨘᨒ ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
        ᨀᨘᨒ i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ᨀᨘ( k == UMAT )
        ᨀᨘᨒ i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ᨀᨘ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᨀᨘᨒ true;

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].isContinuous();
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ᨀᨘᨒ vv[i].isContinuous();
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].isContinuous();
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
      ᨀᨘᨒ i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ᨀᨘ( k == CUDA_GPU_MATND )
        ᨀᨘᨒ i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
        ᨀᨘᨒ i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ᨀᨘ( k == UMAT )
        ᨀᨘᨒ i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ᨀᨘ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᨀᨘᨒ false;

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].isSubmatrix();
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ᨀᨘᨒ vv[i].isSubmatrix();
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].isSubmatrix();
    }

    ᨀᨘ( k == CUDA_GPU_MATND )
    {
        ᨀᨘᨒ ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ᨀᨘᨒ (size_t)(m->ptr() - m->datastart);
    }

    ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const UMat*)obj)->offset;
    }

    ᨀᨘ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᨀᨘᨒ 0;

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (puur_naa_nka)vv.size() );

        ᨀᨘᨒ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ᨀᨘᨒ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].offset;
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ᨀᨘᨒ (size_t)(m->data - m->datastart);
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const Mat*)obj)->step;
    }

    ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const UMat*)obj)->step;
    }

    ᨀᨘ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ᨀᨘᨒ 0;

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (puur_naa_nka)vv.size() );
        ᨀᨘᨒ vv[i].step;
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ᨀᨘᨒ vv[i].step;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].step;
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ᨀᨘᨒ ((const cuda::GpuMat*)obj)->step;
    }
    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ᨀᨘᨒ vv[i].step;
    }
    ᨀᨘ( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ᨀᨘᨒ _gpuMatND.step[i];
    }

    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ᨀᨘᨒ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == NONE )
        arr.release();
    ᨀᨘᨁ ᨀᨘ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ᨀᨘᨁ ᨀᨘ( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ᨀᨘᨁ ᨀᨘ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == NONE )
        arr.release();
    ᨀᨘᨁ ᨀᨘ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ᨀᨘᨁ ᨀᨘ( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ᨀᨘᨁ ᨀᨘ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ᨀᨘᨒ (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ᨀᨘᨒ (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, puur_naa_nka mtype, puur_naa_nka i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ᨀᨘᨒ;
    }
    ᨀᨘ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ᨀᨘᨒ;
    }
    ᨀᨘ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᨀᨘ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᨀᨘ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ᨀᨘ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    puur_naa_nka sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(puur_naa_nka _rows, puur_naa_nka _cols, puur_naa_nka mtype, puur_naa_nka i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ᨀᨘᨒ;
    }
    ᨀᨘ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ᨀᨘᨒ;
    }
    ᨀᨘ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᨀᨘ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᨀᨘ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ᨀᨘ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
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

    ᨀᨘ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ᨀᨘ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᨀᨘᨁ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ᨀᨘ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ᨀᨘ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᨀᨘᨁ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ᨀᨘ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == MATX )
    {
        CV_Assert( i < 0 );
        puur_naa_nka type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ᨀᨘ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            puur_naa_nka total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ᨀᨘᨁ
        {
            ᨀᨘ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ᨀᨘᨁ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        puur_naa_nka esz = CV_ELEM_SIZE(flags);

        ᨀᨘ( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            puur_naa_nka type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ᨀᨘ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ᨀᨘ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ᨀᨘᨒ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ᨀᨘᨁ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ᨀᨘ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ᨀᨘ( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    ᨀᨘ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ᨀᨘᨒ;
        }

        CV_Assert( i < (puur_naa_nka)v.size() );
        Mat& m = v[i];

        ᨀᨘ( allowTransposed )
        {
            ᨀᨘ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ᨀᨘ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᨀᨘᨁ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ᨀᨘ(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ᨀᨘ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ᨀᨘ( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    ᨀᨘ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ᨀᨘᨒ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ᨀᨘ( allowTransposed )
        {
            ᨀᨘ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ᨀᨘ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᨀᨘᨁ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ᨀᨘ(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ᨀᨘ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ᨀᨘ( fixedType() )
            {
                puur_naa_nka _type = CV_MAT_TYPE(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    ᨀᨘ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ᨀᨘᨒ;
        }

        CV_Assert( i < (puur_naa_nka)v.size() );
        UMat& m = v[i];

        ᨀᨘ( allowTransposed )
        {
            ᨀᨘ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ᨀᨘ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ᨀᨘᨁ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ᨀᨘ(fixedSize())
        {
            CV_Assert(m.dims == d);
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    ᨀᨘ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, puur_naa_nka mtype, puur_naa_nka i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ᨀᨘ (shape.dims < 0) {
        release();
    } ᨀᨘᨁ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(puur_naa_nka mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ᨀᨘᨒ getMat().reinterpret(mtype);
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
    ᨀᨘ (shape.dims < 0) {
        release();
        ᨀᨘᨒ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ᨀᨘ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ᨀᨘᨁ ᨀᨘ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
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

    ᨀᨘ( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ᨀᨘ (k == MAT)
            m = (Mat*)obj;
        ᨀᨘᨁ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ᨀᨘ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ᨀᨘᨁ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ᨀᨘ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    ᨀᨘ( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ᨀᨘ (k == UMAT)
            m = (UMat*)obj;
        ᨀᨘᨁ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ᨀᨘ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ᨀᨘᨒ;
        }

        ᨀᨘ(fixedType())
        {
            ᨀᨘ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ᨀᨘᨁ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ᨀᨘ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            for(puur_naa_nka j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ᨀᨘᨒ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        ((Mat*)obj)->release();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ᨀᨘ( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ᨀᨘ( k == NONE )
        ᨀᨘᨒ;

    ᨀᨘ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ᨀᨘ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ᨀᨘᨁ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        puur_naa_nka esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        for( puur_naa_nka i = 0; i < sz.height; i++ )
            v[i].release();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ᨀᨘᨒ;
    }

    ᨀᨘ( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ᨀᨘᨒ;
    }
    ᨀᨘ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ᨀᨘᨒ;
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ᨀᨘᨒ;
    }

    release();
}

bool _OutputArray::needed() const
{
    ᨀᨘᨒ kind() != NONE;
}

Mat& _OutputArray::getMatRef(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ( i < 0 )
    {
        CV_Assert( k == MAT );
        ᨀᨘᨒ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ᨀᨘ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (puur_naa_nka)v.size() );
        ᨀᨘᨒ v[i];
    }
    ᨀᨘᨁ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ᨀᨘᨒ v[i];
    }
}

UMat& _OutputArray::getUMatRef(puur_naa_nka i) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ᨀᨘᨒ *(UMat*)obj;
    }
    ᨀᨘᨁ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (puur_naa_nka)v.size() );
        ᨀᨘᨒ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ᨀᨘᨒ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ᨀᨘᨒ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ᨀᨘᨒ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ᨀᨘᨒ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ᨀᨘᨒ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ᨀᨘᨒ *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == NONE )
        ;
    ᨀᨘᨁ ᨀᨘ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ᨀᨘᨁ ᨀᨘ( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ᨀᨘᨁ ᨀᨘ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ᨀᨘᨁ
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ᨀᨘ( k == NONE )
        ;
    ᨀᨘᨁ ᨀᨘ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ᨀᨘᨁ
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ᨀᨘᨁ ᨀᨘ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ᨀᨘᨁ ᨀᨘ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ᨀᨘᨁ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ᨀᨘᨁ ᨀᨘ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ᨀᨘᨁ ᨀᨘ (k == MATX)
    {
        m.copyTo(getMat());
    }
    ᨀᨘᨁ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ᨀᨘ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ᨀᨘᨒ;
    }
    puur_naa_nka k = kind();
    ᨀᨘ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ᨀᨘᨁ ᨀᨘ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ᨀᨘᨁ ᨀᨘ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ᨀᨘᨁ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ᨀᨘ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ᨀᨘᨒ;
    }
    puur_naa_nka k = kind();
    ᨀᨘ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ᨀᨘᨁ ᨀᨘ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ᨀᨘᨁ ᨀᨘ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ᨀᨘᨁ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ᨀᨘ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᨀᨘᨁ ᨀᨘ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ᨀᨘ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᨀᨘᨁ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ᨀᨘ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ᨀᨘ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᨀᨘᨁ ᨀᨘ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        for (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ᨀᨘ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ᨀᨘᨁ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ᨀᨘᨒ _none; }

} // cv::
