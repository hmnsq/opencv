// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<จำนวนเต็ม, 5> Vec5i;
typedef Vec<จำนวนเต็ม, 7> Vec7i;
typedef Vec<จำนวนเต็ม, 9> Vec9i;
typedef Vec<จำนวนเต็ม, 10> Vec10i;
typedef Vec<จำนวนเต็ม, 11> Vec11i;
typedef Vec<จำนวนเต็ม, 12> Vec12i;
typedef Vec<จำนวนเต็ม, 13> Vec13i;
typedef Vec<จำนวนเต็ม, 14> Vec14i;
typedef Vec<จำนวนเต็ม, 15> Vec15i;
typedef Vec<จำนวนเต็ม, 16> Vec16i;
typedef Vec<จำนวนเต็ม, 32> Vec32i;
typedef Vec<จำนวนเต็ม, 64> Vec64i;
typedef Vec<จำนวนเต็ม, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    pak_sa 1: func(uchar); \
    pak_sa 2: func(Vec2b); \
    pak_sa 3: func(Vec3b); \
    pak_sa 4: func(จำนวนเต็ม); \
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

Mat _InputArray::getMat_(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ถ้า( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ถ้า( i < 0 )
            คืน *m;
        คืน m->row(i);
    }

    ถ้า( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ถ้า( i < 0 )
            คืน m->getMat(accessFlags);
        คืน m->getMat(accessFlags).row(i);
    }

    ถ้า (k == MATX)
    {
        CV_Assert( i < 0 );
        คืน Mat(sz, flags, obj);
    }

    ถ้า( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        จำนวนเต็ม t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        จำนวนเต็ม j, n = (จำนวนเต็ม)v.size();
        ถ้า( n == 0 )
            คืน Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        สำหรับ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        คืน m;
    }

    ถ้า( k == NONE )
        คืน Mat();

    ถ้า( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        จำนวนเต็ม t = CV_MAT_TYPE(flags);
        จำนวนเต็ม esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ถ้า (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } มิฉะนั้น { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            จำนวนเต็ม v_sz = จำนวนเต็ม(v->size()); \
            คืน v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (จำนวนเต็ม)v.size() );

        คืน v[i];
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        คืน v[i];
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (จำนวนเต็ม)v.size() );

        คืน v[i].getMat(accessFlags);
    }

    ถ้า( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ถ้า( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ถ้า( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ถ้า( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        คืน cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ถ้า( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ถ้า( i < 0 )
            คืน *m;
        คืน m->row(i);
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (จำนวนเต็ม)v.size() );

        คืน v[i];
    }

    ถ้า( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ถ้า( i < 0 )
            คืน m->getUMat(accessFlags);
        คืน m->row(i).getUMat(accessFlags);
    }

    คืน getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ถ้า( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        จำนวนเต็ม n = (จำนวนเต็ม)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        สำหรับ( จำนวนเต็ม i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        คืน;
    }

    ถ้า (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        คืน;
    }

    ถ้า( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        จำนวนเต็ม t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        คืน;
    }

    ถ้า( k == NONE )
    {
        mv.clear();
        คืน;
    }

    ถ้า( k == STD_VECTOR_VECTOR )
    {
        จำนวนเต็ม typ = CV_MAT_TYPE(flags);
        จำนวนเต็ม esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            สำหรับ (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                จำนวนเต็ม ni = (จำนวนเต็ม)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        คืน;
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        คืน;
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        คืน;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        คืน;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ถ้า( k == NONE )
    {
        umv.clear();
        คืน;
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        คืน;
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        คืน;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        สำหรับ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        คืน;
    }

    ถ้า( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        คืน;
    }
    ถ้า( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        คืน;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ถ้า (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        คืน *d_mat;
    }

    ถ้า (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        คืน cuda_mem->createGpuMatHeader();
    }

    ถ้า (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ถ้า (k == NONE)
        คืน cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#มิฉะนั้น
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#มิฉะนั้น
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#มิฉะนั้น
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ถ้า (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        คืน *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#มิฉะนั้น
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    คืน *gl_buf;
}

จำนวนเต็ม _InputArray::rows_(จำนวนเต็ม i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ถ้า (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        คืน (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    คืน size_(i).height;
}

จำนวนเต็ม _InputArray::cols_(จำนวนเต็ม i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ถ้า (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        คืน (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    คืน size_(i).width;
}

Size _InputArray::size_(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        คืน Size(m->cols, m->rows);
    }

    ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        คืน Size(m->cols, m->rows);
    }

    ถ้า (k == MATX)
    {
        CV_Assert( i < 0 );
        คืน sz;
    }

    ถ้า( k == NONE )
        คืน Size();

    ถ้า( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        คืน Size((จำนวนเต็ม)n, 1);
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ถ้า( i < 0 )
            คืน vv.empty() ? Size() : Size((จำนวนเต็ม)vv.size(), 1);
        CV_Assert( i < (จำนวนเต็ม)vv.size() );

        คืน vv[i].size();
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ถ้า( i < 0 )
            คืน sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        คืน vv[i].size();
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ถ้า (i < 0)
            คืน vv.empty() ? Size() : Size((จำนวนเต็ม)vv.size(), 1);
        CV_Assert(i < (จำนวนเต็ม)vv.size());
        คืน vv[i].size();
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ถ้า (i < 0)
            คืน vv.empty() ? Size() : Size((จำนวนเต็ม)vv.size(), 1);
        CV_Assert(i < (จำนวนเต็ม)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ถ้า (m.dims == 0)
            คืน Size();
        ถ้า (m.dims == 1)
            คืน Size(1, m.size[0]);
        คืน Size(m.size[1], m.size[0]);
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ถ้า( i < 0 )
            คืน vv.empty() ? Size() : Size((จำนวนเต็ม)vv.size(), 1);
        CV_Assert( i < (จำนวนเต็ม)vv.size() );

        คืน vv[i].size();
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ถ้า (i < 0)
            คืน vv.empty() ? Size() : Size((จำนวนเต็ม)vv.size(), 1);
        CV_Assert(i < (จำนวนเต็ม)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ถ้า (m.dims == 0)
            คืน Size();
        ถ้า (m.dims == 1)
            คืน Size(1, m.size[0]);
        คืน Size(m.size[1], m.size[0]);
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        คืน buf->size();
    }

    ถ้า( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        คืน d_mat->size();
    }

    ถ้า( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        คืน cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

จำนวนเต็ม _InputArray::sizend(จำนวนเต็ม* arrsz, จำนวนเต็ม i) const
{
    จำนวนเต็ม j, d = 0;
    _InputArray::KindFlag k = kind();

    ถ้า( k == NONE )
        ;
    มิฉะนั้น ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ถ้า(arrsz)
            สำหรับ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    มิฉะนั้น ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ถ้า(arrsz)
            สำหรับ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    มิฉะนั้น ถ้า( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (จำนวนเต็ม)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ถ้า(arrsz)
            สำหรับ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    มิฉะนั้น ถ้า( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ถ้า(arrsz)
            สำหรับ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    มิฉะนั้น ถ้า( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (จำนวนเต็ม)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ถ้า(arrsz)
            สำหรับ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    มิฉะนั้น ถ้า (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ถ้า(arrsz)
        {
            arrsz[0] = จำนวนเต็ม(n);
        }
    }
    มิฉะนั้น
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ถ้า(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    คืน d;
}

bool _InputArray::empty(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();
    ถ้า (i >= 0) {
        ถ้า (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            คืน mv->at(i).empty();
        }
        มิฉะนั้น ถ้า (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            คืน umv->at(i).empty();
        }
        มิฉะนั้น ถ้า (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            คืน n == 0;
        } มิฉะนั้น {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    คืน empty();
}

MatShape _InputArray::shape(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ถ้า( k == NONE )
        ;
    มิฉะนั้น ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    มิฉะนั้น ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    มิฉะนั้น ถ้า( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    มิฉะนั้น ถ้า( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    มิฉะนั้น ถ้า( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    มิฉะนั้น
    {
        จำนวนเต็ม sizes[CV_MAX_DIM];
        จำนวนเต็ม dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    คืน shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ถ้า( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ถ้า( k2 == MAT )
            คืน m->size == ((const Mat*)arr.obj)->size;
        ถ้า( k2 == UMAT )
            คืน m->size == ((const UMat*)arr.obj)->size;
        ถ้า( m->dims > 2 )
            คืน false;
        sz1 = m->size();
    }
    มิฉะนั้น ถ้า( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ถ้า( k2 == MAT )
            คืน m->size == ((const Mat*)arr.obj)->size;
        ถ้า( k2 == UMAT )
            คืน m->size == ((const UMat*)arr.obj)->size;
        ถ้า( m->dims > 2 )
            คืน false;
        sz1 = m->size();
    }
    มิฉะนั้น ถ้า ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        คืน ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    มิฉะนั้น
        sz1 = size();

    ถ้า( arr.dims() > 2 )
        คืน false;
    คืน sz1 == arr.size();
}

จำนวนเต็ม _InputArray::dims_(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        คืน ((const Mat*)obj)->dims;
    }

    ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        คืน ((const UMat*)obj)->dims;
    }

    ถ้า (k == MATX)
    {
        CV_Assert( i < 0 );
        คืน 2;
    }

    ถ้า( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        คืน 1;
    }

    ถ้า( k == NONE )
        คืน 0;

    ถ้า( k == STD_VECTOR_VECTOR )
    {
        ถ้า( i < 0 )
            คืน 1;
        CV_Assert( size_t(i) < total() );
        คืน 1;
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ถ้า( i < 0 )
            คืน 1;
        CV_Assert( i < (จำนวนเต็ม)vv.size() );

        คืน vv[i].dims;
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ถ้า( i < 0 )
            คืน 1;
        CV_Assert( i < sz.height );

        คืน vv[i].dims;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ถ้า( i < 0 )
            คืน 1;
        CV_Assert( i < (จำนวนเต็ม)vv.size() );

        คืน vv[i].dims;
    }

    ถ้า( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        คืน 2;
    }

    ถ้า( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        คืน 2;
    }

    ถ้า( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        คืน ((const cuda::GpuMatND*)obj)->dims;
    }

    ถ้า( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        คืน 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        คืน ((const Mat*)obj)->total();
    }

    ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        คืน ((const UMat*)obj)->total();
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ถ้า( i < 0 )
            คืน vv.size();

        CV_Assert( i < (จำนวนเต็ม)vv.size() );
        คืน vv[i].total();
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ถ้า( i < 0 )
            คืน sz.height;

        CV_Assert( i < sz.height );
        คืน vv[i].total();
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ถ้า( i < 0 )
            คืน vv.size();

        CV_Assert( i < (จำนวนเต็ม)vv.size() );
        คืน vv[i].total();
    }

    ถ้า( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ถ้า (_gpuMatND.dims > 0)
        {
            res = 1;
            สำหรับ(จำนวนเต็ม d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            คืน res;
        }
    }

    ถ้า (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        จำนวนเต็ม esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ถ้า (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ถ้า (i < 0) \
                    คืน n; \
                CV_Assert((size_t)i < n); \
                คืน vv->at(i).size(); \
            } มิฉะนั้น \
                คืน ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    คืน size(i).area();
}

จำนวนเต็ม _InputArray::type_(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
        คืน ((const Mat*)obj)->type();

    ถ้า( k == UMAT )
        คืน ((const UMat*)obj)->type();

    ถ้า( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        คืน CV_MAT_TYPE(flags);

    ถ้า( k == NONE )
        คืน -1;

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ถ้า( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            คืน CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (จำนวนเต็ม)vv.size() );
        คืน vv[i >= 0 ? i : 0].type();
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ถ้า( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            คืน CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (จำนวนเต็ม)vv.size() );
        คืน vv[i >= 0 ? i : 0].type();
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ถ้า( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            คืน CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        คืน vv[i >= 0 ? i : 0].type();
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ถ้า (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            คืน CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (จำนวนเต็ม)vv.size());
        คืน vv[i >= 0 ? i : 0].type();
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ถ้า (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            คืน CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (จำนวนเต็ม)vv.size());
        คืน vv[i >= 0 ? i : 0].type();
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า( k == OPENGL_BUFFER )
        คืน ((const ogl::Buffer*)obj)->type();

    ถ้า( k == CUDA_GPU_MAT )
        คืน ((const cuda::GpuMat*)obj)->type();

    ถ้า( k == CUDA_GPU_MATND )
        คืน ((const cuda::GpuMatND*)obj)->type();

    ถ้า( k == CUDA_HOST_MEM )
        คืน ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

จำนวนเต็ม _InputArray::depth_(จำนวนเต็ม i) const
{
    คืน CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
        คืน ((const Mat*)obj)->empty();

    ถ้า( k == UMAT )
        คืน ((const UMat*)obj)->empty();

    ถ้า (k == MATX)
        คืน false;

    ถ้า( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        คืน n == 0;
    }

    ถ้า( k == NONE )
        คืน true;

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        คืน vv.empty();
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        คืน sz.height == 0;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        คืน vv.empty();
    }

    ถ้า( k == OPENGL_BUFFER )
        คืน ((const ogl::Buffer*)obj)->empty();

    ถ้า( k == CUDA_GPU_MAT )
        คืน ((const cuda::GpuMat*)obj)->empty();

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        คืน vv.empty();
    }

    ถ้า( k == CUDA_GPU_MATND )
        คืน ((const cuda::GpuMatND*)obj)->empty();

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        คืน vv.empty();
    }

    ถ้า( k == CUDA_HOST_MEM )
        คืน ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
        คืน i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ถ้า( k == UMAT )
        คืน i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ถ้า( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        คืน true;

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].isContinuous();
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        คืน vv[i].isContinuous();
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].isContinuous();
    }

    ถ้า( k == CUDA_GPU_MAT )
      คืน i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ถ้า( k == CUDA_GPU_MATND )
        คืน i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
        คืน i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ถ้า( k == UMAT )
        คืน i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ถ้า( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        คืน false;

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].isSubmatrix();
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        คืน vv[i].isSubmatrix();
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].isSubmatrix();
    }

    ถ้า( k == CUDA_GPU_MATND )
    {
        คืน ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        คืน (size_t)(m->ptr() - m->datastart);
    }

    ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        คืน ((const UMat*)obj)->offset;
    }

    ถ้า( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        คืน 0;

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (จำนวนเต็ม)vv.size() );

        คืน (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        คืน (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].offset;
    }

    ถ้า( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        คืน (size_t)(m->data - m->datastart);
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        คืน ((const Mat*)obj)->step;
    }

    ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        คืน ((const UMat*)obj)->step;
    }

    ถ้า( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        คืน 0;

    ถ้า( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (จำนวนเต็ม)vv.size() );
        คืน vv[i].step;
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        คืน vv[i].step;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].step;
    }

    ถ้า( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        คืน ((const cuda::GpuMat*)obj)->step;
    }
    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        คืน vv[i].step;
    }
    ถ้า( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        คืน _gpuMatND.step[i];
    }

    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        คืน _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == NONE )
        arr.release();
    มิฉะนั้น ถ้า( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    มิฉะนั้น ถ้า( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    มิฉะนั้น ถ้า (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == NONE )
        arr.release();
    มิฉะนั้น ถ้า( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    มิฉะนั้น ถ้า( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    มิฉะนั้น ถ้า (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    คืน (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    คืน (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, จำนวนเต็ม mtype, จำนวนเต็ม i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ถ้า( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        คืน;
    }
    ถ้า( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        คืน;
    }
    ถ้า( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ถ้า( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ถ้า( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ถ้า( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    จำนวนเต็ม sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(จำนวนเต็ม _rows, จำนวนเต็ม _cols, จำนวนเต็ม mtype, จำนวนเต็ม i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ถ้า( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        คืน;
    }
    ถ้า( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        คืน;
    }
    ถ้า( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ถ้า( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ถ้า( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ถ้า( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    จำนวนเต็ม sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(จำนวนเต็ม d, const จำนวนเต็ม* sizes, จำนวนเต็ม mtype, จำนวนเต็ม i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    จำนวนเต็ม size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ถ้า( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ถ้า (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            มิฉะนั้น
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ถ้า(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        คืน;
    }

    ถ้า( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ถ้า (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            มิฉะนั้น
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ถ้า(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        คืน;
    }

    ถ้า( k == MATX )
    {
        CV_Assert( i < 0 );
        จำนวนเต็ม type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ถ้า (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            จำนวนเต็ม total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        มิฉะนั้น
        {
            ถ้า (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            มิฉะนั้น
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        คืน;
    }

    ถ้า( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        จำนวนเต็ม esz = CV_ELEM_SIZE(flags);

        ถ้า( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            จำนวนเต็ม type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ถ้า (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ถ้า (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    คืน; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } มิฉะนั้น { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        คืน;
    }

    ถ้า( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ถ้า( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ถ้า( fixedType() )
            {
                จำนวนเต็ม _type = CV_MAT_TYPE(flags);
                สำหรับ( size_t j = len0; j < len; j++ )
                {
                    ถ้า( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            คืน;
        }

        CV_Assert( i < (จำนวนเต็ม)v.size() );
        Mat& m = v[i];

        ถ้า( allowTransposed )
        {
            ถ้า( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ถ้า( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            มิฉะนั้น
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ถ้า(fixedSize())
        {
            CV_Assert(m.dims == d);
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        คืน;
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ถ้า( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ถ้า( fixedType() )
            {
                จำนวนเต็ม _type = CV_MAT_TYPE(flags);
                สำหรับ( size_t j = len0; j < len; j++ )
                {
                    ถ้า( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            คืน;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ถ้า( allowTransposed )
        {
            ถ้า( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ถ้า( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            มิฉะนั้น
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ถ้า(fixedSize())
        {
            CV_Assert(m.dims == d);
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        คืน;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ถ้า( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ถ้า( fixedType() )
            {
                จำนวนเต็ม _type = CV_MAT_TYPE(flags);
                สำหรับ( size_t j = len0; j < len; j++ )
                {
                    ถ้า( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            คืน;
        }

        CV_Assert( i < (จำนวนเต็ม)v.size() );
        UMat& m = v[i];

        ถ้า( allowTransposed )
        {
            ถ้า( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ถ้า( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            มิฉะนั้น
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ถ้า(fixedSize())
        {
            CV_Assert(m.dims == d);
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        คืน;
    }

    ถ้า ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        คืน;
    }

    ถ้า( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, จำนวนเต็ม mtype, จำนวนเต็ม i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ถ้า (shape.dims < 0) {
        release();
    } มิฉะนั้น {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(จำนวนเต็ม mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    คืน getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, จำนวนเต็ม mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, จำนวนเต็ม mtype, จำนวนเต็ม i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    จำนวนเต็ม sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(จำนวนเต็ม rows, จำนวนเต็ม cols, จำนวนเต็ม mtype, จำนวนเต็ม i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    จำนวนเต็ม sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, จำนวนเต็ม mtype, จำนวนเต็ม i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ถ้า (shape.dims < 0) {
        release();
        คืน;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ถ้า ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } มิฉะนั้น ถ้า ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, จำนวนเต็ม mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(จำนวนเต็ม d, const จำนวนเต็ม* sizes, จำนวนเต็ม mtype, จำนวนเต็ม i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    จำนวนเต็ม size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ถ้า( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ถ้า (k == MAT)
            m = (Mat*)obj;
        มิฉะนั้น {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ถ้า (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            มิฉะนั้น
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ถ้า(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        คืน;
    }

    ถ้า( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ถ้า (k == UMAT)
            m = (UMat*)obj;
        มิฉะนั้น {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ถ้า (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            คืน;
        }

        ถ้า(fixedType())
        {
            ถ้า(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            มิฉะนั้น
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ถ้า(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            สำหรับ(จำนวนเต็ม j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        คืน;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        ((Mat*)obj)->release();
        คืน;
    }

    ถ้า( k == UMAT )
    {
        ((UMat*)obj)->release();
        คืน;
    }

    ถ้า( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ถ้า( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ถ้า( k == NONE )
        คืน;

    ถ้า( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ถ้า (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } มิฉะนั้น { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        จำนวนเต็ม esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        คืน;
    }

    ถ้า( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        คืน;
    }

    ถ้า( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        สำหรับ( จำนวนเต็ม i = 0; i < sz.height; i++ )
            v[i].release();
        คืน;
    }

    ถ้า( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        คืน;
    }

    ถ้า( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        คืน;
    }
    ถ้า (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        คืน;
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        คืน;
    }

    release();
}

bool _OutputArray::needed() const
{
    คืน kind() != NONE;
}

Mat& _OutputArray::getMatRef(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();
    ถ้า( i < 0 )
    {
        CV_Assert( k == MAT );
        คืน *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ถ้า( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (จำนวนเต็ม)v.size() );
        คืน v[i];
    }
    มิฉะนั้น
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        คืน v[i];
    }
}

UMat& _OutputArray::getUMatRef(จำนวนเต็ม i) const
{
    _InputArray::KindFlag k = kind();
    ถ้า( i < 0 )
    {
        CV_Assert( k == UMAT );
        คืน *(UMat*)obj;
    }
    มิฉะนั้น
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (จำนวนเต็ม)v.size() );
        คืน v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    คืน *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    คืน *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    คืน *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    คืน *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    คืน *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    คืน *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == NONE )
        ;
    มิฉะนั้น ถ้า (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    มิฉะนั้น ถ้า( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    มิฉะนั้น ถ้า( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    มิฉะนั้น
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ถ้า( k == NONE )
        ;
    มิฉะนั้น ถ้า (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    มิฉะนั้น
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ถ้า (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    มิฉะนั้น ถ้า (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    มิฉะนั้น ถ้า (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    มิฉะนั้น
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ถ้า (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    มิฉะนั้น ถ้า (k == MAT)
    {
        *(Mat*)obj = m;
    }
    มิฉะนั้น ถ้า (k == MATX)
    {
        m.copyTo(getMat());
    }
    มิฉะนั้น
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ถ้า (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        คืน;
    }
    จำนวนเต็ม k = kind();
    ถ้า (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    มิฉะนั้น ถ้า (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    มิฉะนั้น ถ้า (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    มิฉะนั้น
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ถ้า (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        คืน;
    }
    จำนวนเต็ม k = kind();
    ถ้า (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    มิฉะนั้น ถ้า (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    มิฉะนั้น ถ้า (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    มิฉะนั้น
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ถ้า (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        สำหรับ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ถ้า (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    มิฉะนั้น ถ้า (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        สำหรับ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ถ้า (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    มิฉะนั้น
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ถ้า (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        สำหรับ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ถ้า (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    มิฉะนั้น ถ้า (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        สำหรับ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ถ้า (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    มิฉะนั้น
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { คืน _none; }

} // cv::
