// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<ចំនួនគត់, 5> Vec5i;
typedef Vec<ចំនួនគត់, 7> Vec7i;
typedef Vec<ចំនួនគត់, 9> Vec9i;
typedef Vec<ចំនួនគត់, 10> Vec10i;
typedef Vec<ចំនួនគត់, 11> Vec11i;
typedef Vec<ចំនួនគត់, 12> Vec12i;
typedef Vec<ចំនួនគត់, 13> Vec13i;
typedef Vec<ចំនួនគត់, 14> Vec14i;
typedef Vec<ចំនួនគត់, 15> Vec15i;
typedef Vec<ចំនួនគត់, 16> Vec16i;
typedef Vec<ចំនួនគត់, 32> Vec32i;
typedef Vec<ចំនួនគត់, 64> Vec64i;
typedef Vec<ចំនួនគត់, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    pak_sa 1: func(uchar); \
    pak_sa 2: func(Vec2b); \
    pak_sa 3: func(Vec3b); \
    pak_sa 4: func(ចំនួនគត់); \
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

Mat _InputArray::getMat_(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ប្រសិនបើ( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ *m;
        ត្រឡប់ m->row(i);
    }

    ប្រសិនបើ( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ m->getMat(accessFlags);
        ត្រឡប់ m->getMat(accessFlags).row(i);
    }

    ប្រសិនបើ (k == MATX)
    {
        CV_Assert( i < 0 );
        ត្រឡប់ Mat(sz, flags, obj);
    }

    ប្រសិនបើ( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ចំនួនគត់ t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        ចំនួនគត់ j, n = (ចំនួនគត់)v.size();
        ប្រសិនបើ( n == 0 )
            ត្រឡប់ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        សម្រាប់( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ត្រឡប់ m;
    }

    ប្រសិនបើ( k == NONE )
        ត្រឡប់ Mat();

    ប្រសិនបើ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        ចំនួនគត់ t = CV_MAT_TYPE(flags);
        ចំនួនគត់ esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ប្រសិនបើ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } បើមិនដូច្នេះ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ចំនួនគត់ v_sz = ចំនួនគត់(v->size()); \
            ត្រឡប់ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ចំនួនគត់)v.size() );

        ត្រឡប់ v[i];
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ត្រឡប់ v[i];
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ចំនួនគត់)v.size() );

        ត្រឡប់ v[i].getMat(accessFlags);
    }

    ប្រសិនបើ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ប្រសិនបើ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ត្រឡប់ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ប្រសិនបើ( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ *m;
        ត្រឡប់ m->row(i);
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ចំនួនគត់)v.size() );

        ត្រឡប់ v[i];
    }

    ប្រសិនបើ( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ m->getUMat(accessFlags);
        ត្រឡប់ m->row(i).getUMat(accessFlags);
    }

    ត្រឡប់ getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ប្រសិនបើ( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        ចំនួនគត់ n = (ចំនួនគត់)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        សម្រាប់( ចំនួនគត់ i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ត្រឡប់;
    }

    ប្រសិនបើ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        ចំនួនគត់ t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ត្រឡប់;
    }

    ប្រសិនបើ( k == NONE )
    {
        mv.clear();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_VECTOR )
    {
        ចំនួនគត់ typ = CV_MAT_TYPE(flags);
        ចំនួនគត់ esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            សម្រាប់ (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                ចំនួនគត់ ni = (ចំនួនគត់)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ត្រឡប់;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ប្រសិនបើ( k == NONE )
    {
        umv.clear();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        សម្រាប់( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ត្រឡប់;
    }

    ប្រសិនបើ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ត្រឡប់;
    }
    ប្រសិនបើ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ត្រឡប់;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ត្រឡប់ *d_mat;
    }

    ប្រសិនបើ (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ត្រឡប់ cuda_mem->createGpuMatHeader();
    }

    ប្រសិនបើ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ប្រសិនបើ (k == NONE)
        ត្រឡប់ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#បើមិនដូច្នេះ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#បើមិនដូច្នេះ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#បើមិនដូច្នេះ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ត្រឡប់ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#បើមិនដូច្នេះ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ត្រឡប់ *gl_buf;
}

ចំនួនគត់ _InputArray::rows_(ចំនួនគត់ i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ត្រឡប់ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ត្រឡប់ size_(i).height;
}

ចំនួនគត់ _InputArray::cols_(ចំនួនគត់ i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ត្រឡប់ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ត្រឡប់ size_(i).width;
}

Size _InputArray::size_(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ត្រឡប់ Size(m->cols, m->rows);
    }

    ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ត្រឡប់ Size(m->cols, m->rows);
    }

    ប្រសិនបើ (k == MATX)
    {
        CV_Assert( i < 0 );
        ត្រឡប់ sz;
    }

    ប្រសិនបើ( k == NONE )
        ត្រឡប់ Size();

    ប្រសិនបើ( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ត្រឡប់ Size((ចំនួនគត់)n, 1);
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ vv.empty() ? Size() : Size((ចំនួនគត់)vv.size(), 1);
        CV_Assert( i < (ចំនួនគត់)vv.size() );

        ត្រឡប់ vv[i].size();
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ត្រឡប់ vv[i].size();
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ប្រសិនបើ (i < 0)
            ត្រឡប់ vv.empty() ? Size() : Size((ចំនួនគត់)vv.size(), 1);
        CV_Assert(i < (ចំនួនគត់)vv.size());
        ត្រឡប់ vv[i].size();
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ប្រសិនបើ (i < 0)
            ត្រឡប់ vv.empty() ? Size() : Size((ចំនួនគត់)vv.size(), 1);
        CV_Assert(i < (ចំនួនគត់)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ប្រសិនបើ (m.dims == 0)
            ត្រឡប់ Size();
        ប្រសិនបើ (m.dims == 1)
            ត្រឡប់ Size(1, m.size[0]);
        ត្រឡប់ Size(m.size[1], m.size[0]);
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ vv.empty() ? Size() : Size((ចំនួនគត់)vv.size(), 1);
        CV_Assert( i < (ចំនួនគត់)vv.size() );

        ត្រឡប់ vv[i].size();
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ប្រសិនបើ (i < 0)
            ត្រឡប់ vv.empty() ? Size() : Size((ចំនួនគត់)vv.size(), 1);
        CV_Assert(i < (ចំនួនគត់)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ប្រសិនបើ (m.dims == 0)
            ត្រឡប់ Size();
        ប្រសិនបើ (m.dims == 1)
            ត្រឡប់ Size(1, m.size[0]);
        ត្រឡប់ Size(m.size[1], m.size[0]);
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ត្រឡប់ buf->size();
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ត្រឡប់ d_mat->size();
    }

    ប្រសិនបើ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ត្រឡប់ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ចំនួនគត់ _InputArray::sizend(ចំនួនគត់* arrsz, ចំនួនគត់ i) const
{
    ចំនួនគត់ j, d = 0;
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == NONE )
        ;
    បើមិនដូច្នេះ ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ប្រសិនបើ(arrsz)
            សម្រាប់(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ប្រសិនបើ(arrsz)
            សម្រាប់(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (ចំនួនគត់)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ប្រសិនបើ(arrsz)
            សម្រាប់(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ប្រសិនបើ(arrsz)
            សម្រាប់(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (ចំនួនគត់)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ប្រសិនបើ(arrsz)
            សម្រាប់(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    បើមិនដូច្នេះ ប្រសិនបើ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ប្រសិនបើ(arrsz)
        {
            arrsz[0] = ចំនួនគត់(n);
        }
    }
    បើមិនដូច្នេះ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ប្រសិនបើ(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ត្រឡប់ d;
}

bool _InputArray::empty(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (i >= 0) {
        ប្រសិនបើ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ត្រឡប់ mv->at(i).empty();
        }
        បើមិនដូច្នេះ ប្រសិនបើ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ត្រឡប់ umv->at(i).empty();
        }
        បើមិនដូច្នេះ ប្រសិនបើ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ត្រឡប់ n == 0;
        } បើមិនដូច្នេះ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ត្រឡប់ empty();
}

MatShape _InputArray::shape(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ប្រសិនបើ( k == NONE )
        ;
    បើមិនដូច្នេះ ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    បើមិនដូច្នេះ
    {
        ចំនួនគត់ sizes[CV_MAX_DIM];
        ចំនួនគត់ dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ត្រឡប់ shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ប្រសិនបើ( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ប្រសិនបើ( k2 == MAT )
            ត្រឡប់ m->size == ((const Mat*)arr.obj)->size;
        ប្រសិនបើ( k2 == UMAT )
            ត្រឡប់ m->size == ((const UMat*)arr.obj)->size;
        ប្រសិនបើ( m->dims > 2 )
            ត្រឡប់ false;
        sz1 = m->size();
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ប្រសិនបើ( k2 == MAT )
            ត្រឡប់ m->size == ((const Mat*)arr.obj)->size;
        ប្រសិនបើ( k2 == UMAT )
            ត្រឡប់ m->size == ((const UMat*)arr.obj)->size;
        ប្រសិនបើ( m->dims > 2 )
            ត្រឡប់ false;
        sz1 = m->size();
    }
    បើមិនដូច្នេះ ប្រសិនបើ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ត្រឡប់ ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    បើមិនដូច្នេះ
        sz1 = size();

    ប្រសិនបើ( arr.dims() > 2 )
        ត្រឡប់ false;
    ត្រឡប់ sz1 == arr.size();
}

ចំនួនគត់ _InputArray::dims_(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const Mat*)obj)->dims;
    }

    ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const UMat*)obj)->dims;
    }

    ប្រសិនបើ (k == MATX)
    {
        CV_Assert( i < 0 );
        ត្រឡប់ 2;
    }

    ប្រសិនបើ( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ 1;
    }

    ប្រសិនបើ( k == NONE )
        ត្រឡប់ 0;

    ប្រសិនបើ( k == STD_VECTOR_VECTOR )
    {
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ 1;
        CV_Assert( size_t(i) < total() );
        ត្រឡប់ 1;
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ 1;
        CV_Assert( i < (ចំនួនគត់)vv.size() );

        ត្រឡប់ vv[i].dims;
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ 1;
        CV_Assert( i < sz.height );

        ត្រឡប់ vv[i].dims;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ 1;
        CV_Assert( i < (ចំនួនគត់)vv.size() );

        ត្រឡប់ vv[i].dims;
    }

    ប្រសិនបើ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ 2;
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ 2;
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const cuda::GpuMatND*)obj)->dims;
    }

    ប្រសិនបើ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const Mat*)obj)->total();
    }

    ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const UMat*)obj)->total();
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ vv.size();

        CV_Assert( i < (ចំនួនគត់)vv.size() );
        ត្រឡប់ vv[i].total();
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ sz.height;

        CV_Assert( i < sz.height );
        ត្រឡប់ vv[i].total();
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ប្រសិនបើ( i < 0 )
            ត្រឡប់ vv.size();

        CV_Assert( i < (ចំនួនគត់)vv.size() );
        ត្រឡប់ vv[i].total();
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ប្រសិនបើ (_gpuMatND.dims > 0)
        {
            res = 1;
            សម្រាប់(ចំនួនគត់ d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ត្រឡប់ res;
        }
    }

    ប្រសិនបើ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        ចំនួនគត់ esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ប្រសិនបើ (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ប្រសិនបើ (i < 0) \
                    ត្រឡប់ n; \
                CV_Assert((size_t)i < n); \
                ត្រឡប់ vv->at(i).size(); \
            } បើមិនដូច្នេះ \
                ត្រឡប់ ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ត្រឡប់ size(i).area();
}

ចំនួនគត់ _InputArray::type_(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
        ត្រឡប់ ((const Mat*)obj)->type();

    ប្រសិនបើ( k == UMAT )
        ត្រឡប់ ((const UMat*)obj)->type();

    ប្រសិនបើ( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ត្រឡប់ CV_MAT_TYPE(flags);

    ប្រសិនបើ( k == NONE )
        ត្រឡប់ -1;

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ប្រសិនបើ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ត្រឡប់ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ចំនួនគត់)vv.size() );
        ត្រឡប់ vv[i >= 0 ? i : 0].type();
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ប្រសិនបើ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ត្រឡប់ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ចំនួនគត់)vv.size() );
        ត្រឡប់ vv[i >= 0 ? i : 0].type();
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ប្រសិនបើ( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ត្រឡប់ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ត្រឡប់ vv[i >= 0 ? i : 0].type();
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ប្រសិនបើ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ត្រឡប់ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ចំនួនគត់)vv.size());
        ត្រឡប់ vv[i >= 0 ? i : 0].type();
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ប្រសិនបើ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ត្រឡប់ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ចំនួនគត់)vv.size());
        ត្រឡប់ vv[i >= 0 ? i : 0].type();
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ( k == OPENGL_BUFFER )
        ត្រឡប់ ((const ogl::Buffer*)obj)->type();

    ប្រសិនបើ( k == CUDA_GPU_MAT )
        ត្រឡប់ ((const cuda::GpuMat*)obj)->type();

    ប្រសិនបើ( k == CUDA_GPU_MATND )
        ត្រឡប់ ((const cuda::GpuMatND*)obj)->type();

    ប្រសិនបើ( k == CUDA_HOST_MEM )
        ត្រឡប់ ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ចំនួនគត់ _InputArray::depth_(ចំនួនគត់ i) const
{
    ត្រឡប់ CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
        ត្រឡប់ ((const Mat*)obj)->empty();

    ប្រសិនបើ( k == UMAT )
        ត្រឡប់ ((const UMat*)obj)->empty();

    ប្រសិនបើ (k == MATX)
        ត្រឡប់ false;

    ប្រសិនបើ( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ត្រឡប់ n == 0;
    }

    ប្រសិនបើ( k == NONE )
        ត្រឡប់ true;

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ត្រឡប់ vv.empty();
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        ត្រឡប់ sz.height == 0;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ត្រឡប់ vv.empty();
    }

    ប្រសិនបើ( k == OPENGL_BUFFER )
        ត្រឡប់ ((const ogl::Buffer*)obj)->empty();

    ប្រសិនបើ( k == CUDA_GPU_MAT )
        ត្រឡប់ ((const cuda::GpuMat*)obj)->empty();

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ត្រឡប់ vv.empty();
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND )
        ត្រឡប់ ((const cuda::GpuMatND*)obj)->empty();

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ត្រឡប់ vv.empty();
    }

    ប្រសិនបើ( k == CUDA_HOST_MEM )
        ត្រឡប់ ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
        ត្រឡប់ i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ប្រសិនបើ( k == UMAT )
        ត្រឡប់ i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ប្រសិនបើ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ត្រឡប់ true;

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].isContinuous();
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ត្រឡប់ vv[i].isContinuous();
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].isContinuous();
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
      ត្រឡប់ i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ប្រសិនបើ( k == CUDA_GPU_MATND )
        ត្រឡប់ i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
        ត្រឡប់ i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ប្រសិនបើ( k == UMAT )
        ត្រឡប់ i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ប្រសិនបើ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ត្រឡប់ false;

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].isSubmatrix();
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ត្រឡប់ vv[i].isSubmatrix();
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].isSubmatrix();
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND )
    {
        ត្រឡប់ ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ត្រឡប់ (size_t)(m->ptr() - m->datastart);
    }

    ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const UMat*)obj)->offset;
    }

    ប្រសិនបើ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ត្រឡប់ 0;

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ចំនួនគត់)vv.size() );

        ត្រឡប់ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ត្រឡប់ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].offset;
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ត្រឡប់ (size_t)(m->data - m->datastart);
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const Mat*)obj)->step;
    }

    ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const UMat*)obj)->step;
    }

    ប្រសិនបើ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ត្រឡប់ 0;

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ចំនួនគត់)vv.size() );
        ត្រឡប់ vv[i].step;
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ត្រឡប់ vv[i].step;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].step;
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ត្រឡប់ ((const cuda::GpuMat*)obj)->step;
    }
    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ត្រឡប់ vv[i].step;
    }
    ប្រសិនបើ( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ត្រឡប់ _gpuMatND.step[i];
    }

    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ត្រឡប់ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == NONE )
        arr.release();
    បើមិនដូច្នេះ ប្រសិនបើ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    បើមិនដូច្នេះ ប្រសិនបើ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == NONE )
        arr.release();
    បើមិនដូច្នេះ ប្រសិនបើ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    បើមិនដូច្នេះ ប្រសិនបើ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ត្រឡប់ (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ត្រឡប់ (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, ចំនួនគត់ mtype, ចំនួនគត់ i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ត្រឡប់;
    }
    ប្រសិនបើ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ត្រឡប់;
    }
    ប្រសិនបើ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ប្រសិនបើ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ប្រសិនបើ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ប្រសិនបើ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ចំនួនគត់ sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(ចំនួនគត់ _rows, ចំនួនគត់ _cols, ចំនួនគត់ mtype, ចំនួនគត់ i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ត្រឡប់;
    }
    ប្រសិនបើ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ត្រឡប់;
    }
    ប្រសិនបើ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ប្រសិនបើ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ប្រសិនបើ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ប្រសិនបើ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ចំនួនគត់ sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(ចំនួនគត់ d, const ចំនួនគត់* sizes, ចំនួនគត់ mtype, ចំនួនគត់ i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ចំនួនគត់ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ប្រសិនបើ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            បើមិនដូច្នេះ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ប្រសិនបើ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ប្រសិនបើ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            បើមិនដូច្នេះ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ប្រសិនបើ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == MATX )
    {
        CV_Assert( i < 0 );
        ចំនួនគត់ type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ប្រសិនបើ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ចំនួនគត់ total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        បើមិនដូច្នេះ
        {
            ប្រសិនបើ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            បើមិនដូច្នេះ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ចំនួនគត់ esz = CV_ELEM_SIZE(flags);

        ប្រសិនបើ( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            ចំនួនគត់ type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ប្រសិនបើ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ប្រសិនបើ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ត្រឡប់; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } បើមិនដូច្នេះ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ត្រឡប់;
    }

    ប្រសិនបើ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ប្រសិនបើ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ប្រសិនបើ( fixedType() )
            {
                ចំនួនគត់ _type = CV_MAT_TYPE(flags);
                សម្រាប់( size_t j = len0; j < len; j++ )
                {
                    ប្រសិនបើ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ត្រឡប់;
        }

        CV_Assert( i < (ចំនួនគត់)v.size() );
        Mat& m = v[i];

        ប្រសិនបើ( allowTransposed )
        {
            ប្រសិនបើ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ប្រសិនបើ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            បើមិនដូច្នេះ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ប្រសិនបើ(fixedSize())
        {
            CV_Assert(m.dims == d);
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ប្រសិនបើ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ប្រសិនបើ( fixedType() )
            {
                ចំនួនគត់ _type = CV_MAT_TYPE(flags);
                សម្រាប់( size_t j = len0; j < len; j++ )
                {
                    ប្រសិនបើ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ត្រឡប់;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ប្រសិនបើ( allowTransposed )
        {
            ប្រសិនបើ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ប្រសិនបើ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            បើមិនដូច្នេះ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ប្រសិនបើ(fixedSize())
        {
            CV_Assert(m.dims == d);
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ប្រសិនបើ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ប្រសិនបើ( fixedType() )
            {
                ចំនួនគត់ _type = CV_MAT_TYPE(flags);
                សម្រាប់( size_t j = len0; j < len; j++ )
                {
                    ប្រសិនបើ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ត្រឡប់;
        }

        CV_Assert( i < (ចំនួនគត់)v.size() );
        UMat& m = v[i];

        ប្រសិនបើ( allowTransposed )
        {
            ប្រសិនបើ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ប្រសិនបើ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            បើមិនដូច្នេះ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ប្រសិនបើ(fixedSize())
        {
            CV_Assert(m.dims == d);
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ត្រឡប់;
    }

    ប្រសិនបើ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, ចំនួនគត់ mtype, ចំនួនគត់ i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ប្រសិនបើ (shape.dims < 0) {
        release();
    } បើមិនដូច្នេះ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(ចំនួនគត់ mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ត្រឡប់ getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, ចំនួនគត់ mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, ចំនួនគត់ mtype, ចំនួនគត់ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ចំនួនគត់ sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(ចំនួនគត់ rows, ចំនួនគត់ cols, ចំនួនគត់ mtype, ចំនួនគត់ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ចំនួនគត់ sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, ចំនួនគត់ mtype, ចំនួនគត់ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ប្រសិនបើ (shape.dims < 0) {
        release();
        ត្រឡប់;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } បើមិនដូច្នេះ ប្រសិនបើ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, ចំនួនគត់ mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(ចំនួនគត់ d, const ចំនួនគត់* sizes, ចំនួនគត់ mtype, ចំនួនគត់ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ចំនួនគត់ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ប្រសិនបើ( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ប្រសិនបើ (k == MAT)
            m = (Mat*)obj;
        បើមិនដូច្នេះ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ប្រសិនបើ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            បើមិនដូច្នេះ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ប្រសិនបើ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ត្រឡប់;
    }

    ប្រសិនបើ( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ប្រសិនបើ (k == UMAT)
            m = (UMat*)obj;
        បើមិនដូច្នេះ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ប្រសិនបើ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ត្រឡប់;
        }

        ប្រសិនបើ(fixedType())
        {
            ប្រសិនបើ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            បើមិនដូច្នេះ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ប្រសិនបើ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            សម្រាប់(ចំនួនគត់ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ត្រឡប់;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        ((Mat*)obj)->release();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ប្រសិនបើ( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ប្រសិនបើ( k == NONE )
        ត្រឡប់;

    ប្រសិនបើ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ប្រសិនបើ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } បើមិនដូច្នេះ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        ចំនួនគត់ esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        សម្រាប់( ចំនួនគត់ i = 0; i < sz.height; i++ )
            v[i].release();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ត្រឡប់;
    }

    ប្រសិនបើ( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ត្រឡប់;
    }
    ប្រសិនបើ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ត្រឡប់;
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ត្រឡប់;
    }

    release();
}

bool _OutputArray::needed() const
{
    ត្រឡប់ kind() != NONE;
}

Mat& _OutputArray::getMatRef(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ( i < 0 )
    {
        CV_Assert( k == MAT );
        ត្រឡប់ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ប្រសិនបើ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ចំនួនគត់)v.size() );
        ត្រឡប់ v[i];
    }
    បើមិនដូច្នេះ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ត្រឡប់ v[i];
    }
}

UMat& _OutputArray::getUMatRef(ចំនួនគត់ i) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ត្រឡប់ *(UMat*)obj;
    }
    បើមិនដូច្នេះ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ចំនួនគត់)v.size() );
        ត្រឡប់ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ត្រឡប់ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ត្រឡប់ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ត្រឡប់ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ត្រឡប់ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ត្រឡប់ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ត្រឡប់ *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == NONE )
        ;
    បើមិនដូច្នេះ ប្រសិនបើ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    បើមិនដូច្នេះ ប្រសិនបើ( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    បើមិនដូច្នេះ ប្រសិនបើ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    បើមិនដូច្នេះ
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ប្រសិនបើ( k == NONE )
        ;
    បើមិនដូច្នេះ ប្រសិនបើ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    បើមិនដូច្នេះ
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    បើមិនដូច្នេះ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MATX)
    {
        m.copyTo(getMat());
    }
    បើមិនដូច្នេះ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ប្រសិនបើ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ត្រឡប់;
    }
    ចំនួនគត់ k = kind();
    ប្រសិនបើ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    បើមិនដូច្នេះ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ប្រសិនបើ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ត្រឡប់;
    }
    ចំនួនគត់ k = kind();
    ប្រសិនបើ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    បើមិនដូច្នេះ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        សម្រាប់ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ប្រសិនបើ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        សម្រាប់ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ប្រសិនបើ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    បើមិនដូច្នេះ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ប្រសិនបើ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        សម្រាប់ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ប្រសិនបើ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    បើមិនដូច្នេះ ប្រសិនបើ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        សម្រាប់ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ប្រសិនបើ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    បើមិនដូច្នេះ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ត្រឡប់ _none; }

} // cv::
