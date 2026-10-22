// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<số_nguyên, 5> Vec5i;
typedef Vec<số_nguyên, 7> Vec7i;
typedef Vec<số_nguyên, 9> Vec9i;
typedef Vec<số_nguyên, 10> Vec10i;
typedef Vec<số_nguyên, 11> Vec11i;
typedef Vec<số_nguyên, 12> Vec12i;
typedef Vec<số_nguyên, 13> Vec13i;
typedef Vec<số_nguyên, 14> Vec14i;
typedef Vec<số_nguyên, 15> Vec15i;
typedef Vec<số_nguyên, 16> Vec16i;
typedef Vec<số_nguyên, 32> Vec32i;
typedef Vec<số_nguyên, 64> Vec64i;
typedef Vec<số_nguyên, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    case 1: func(uchar); \
    case 2: func(Vec2b); \
    case 3: func(Vec3b); \
    case 4: func(số_nguyên); \
    case 6: func(Vec3s); \
    case 8: func(Vec2i); \
    case 12: func(Vec3i); \
    case 16: func(Vec4i); \
    case 20: func(Vec5i); \
    case 24: func(Vec6i); \
    case 28: func(Vec7i); \
    case 32: func(Vec8i); \
    case 36: func(Vec9i); \
    case 40: func(Vec10i); \
    case 44: func(Vec11i); \
    case 48: func(Vec12i); \
    case 52: func(Vec13i); \
    case 56: func(Vec14i); \
    case 60: func(Vec15i); \
    case 64: func(Vec16i); \
    case 128: func(Vec32i); \
    case 256: func(Vec64i); \
    case 512: func(Vec128i); \
    default: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat _InputArray::getMat_(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    nếu( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        nếu( i < 0 )
            trả_về *m;
        trả_về m->row(i);
    }

    nếu( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        nếu( i < 0 )
            trả_về m->getMat(accessFlags);
        trả_về m->getMat(accessFlags).row(i);
    }

    nếu (k == MATX)
    {
        CV_Assert( i < 0 );
        trả_về Mat(sz, flags, obj);
    }

    nếu( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        số_nguyên t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        số_nguyên j, n = (số_nguyên)v.size();
        nếu( n == 0 )
            trả_về Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        cho( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        trả_về m;
    }

    nếu( k == NONE )
        trả_về Mat();

    nếu( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        số_nguyên t = CV_MAT_TYPE(flags);
        số_nguyên esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            nếu (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } không_thì { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            số_nguyên v_sz = số_nguyên(v->size()); \
            trả_về v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (số_nguyên)v.size() );

        trả_về v[i];
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        trả_về v[i];
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (số_nguyên)v.size() );

        trả_về v[i].getMat(accessFlags);
    }

    nếu( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    nếu( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    nếu( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    nếu( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        trả_về cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    nếu( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        nếu( i < 0 )
            trả_về *m;
        trả_về m->row(i);
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (số_nguyên)v.size() );

        trả_về v[i];
    }

    nếu( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        nếu( i < 0 )
            trả_về m->getUMat(accessFlags);
        trả_về m->row(i).getUMat(accessFlags);
    }

    trả_về getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    nếu( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        số_nguyên n = (số_nguyên)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        cho( số_nguyên i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        trả_về;
    }

    nếu (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        trả_về;
    }

    nếu( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        số_nguyên t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        trả_về;
    }

    nếu( k == NONE )
    {
        mv.clear();
        trả_về;
    }

    nếu( k == STD_VECTOR_VECTOR )
    {
        số_nguyên typ = CV_MAT_TYPE(flags);
        số_nguyên esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            cho (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                số_nguyên ni = (số_nguyên)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        break

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        trả_về;
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        trả_về;
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        trả_về;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        trả_về;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    nếu( k == NONE )
    {
        umv.clear();
        trả_về;
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        trả_về;
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        trả_về;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        cho( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        trả_về;
    }

    nếu( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        trả_về;
    }
    nếu( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        trả_về;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    nếu (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        trả_về *d_mat;
    }

    nếu (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        trả_về cuda_mem->createGpuMatHeader();
    }

    nếu (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    nếu (k == NONE)
        trả_về cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#không_thì
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#không_thì
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#không_thì
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    nếu (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        trả_về *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#không_thì
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    trả_về *gl_buf;
}

số_nguyên _InputArray::rows_(số_nguyên i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    nếu (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        trả_về (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    trả_về size_(i).height;
}

số_nguyên _InputArray::cols_(số_nguyên i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    nếu (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        trả_về (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    trả_về size_(i).width;
}

Size _InputArray::size_(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        trả_về Size(m->cols, m->rows);
    }

    nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        trả_về Size(m->cols, m->rows);
    }

    nếu (k == MATX)
    {
        CV_Assert( i < 0 );
        trả_về sz;
    }

    nếu( k == NONE )
        trả_về Size();

    nếu( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        trả_về Size((số_nguyên)n, 1);
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        nếu( i < 0 )
            trả_về vv.empty() ? Size() : Size((số_nguyên)vv.size(), 1);
        CV_Assert( i < (số_nguyên)vv.size() );

        trả_về vv[i].size();
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        nếu( i < 0 )
            trả_về sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        trả_về vv[i].size();
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        nếu (i < 0)
            trả_về vv.empty() ? Size() : Size((số_nguyên)vv.size(), 1);
        CV_Assert(i < (số_nguyên)vv.size());
        trả_về vv[i].size();
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        nếu (i < 0)
            trả_về vv.empty() ? Size() : Size((số_nguyên)vv.size(), 1);
        CV_Assert(i < (số_nguyên)vv.size());
        const cuda::GpuMatND& m = vv[i];
        nếu (m.dims == 0)
            trả_về Size();
        nếu (m.dims == 1)
            trả_về Size(1, m.size[0]);
        trả_về Size(m.size[1], m.size[0]);
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        nếu( i < 0 )
            trả_về vv.empty() ? Size() : Size((số_nguyên)vv.size(), 1);
        CV_Assert( i < (số_nguyên)vv.size() );

        trả_về vv[i].size();
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        nếu (i < 0)
            trả_về vv.empty() ? Size() : Size((số_nguyên)vv.size(), 1);
        CV_Assert(i < (số_nguyên)vv.size());
        const cuda::GpuMatND& m = vv[i];
        nếu (m.dims == 0)
            trả_về Size();
        nếu (m.dims == 1)
            trả_về Size(1, m.size[0]);
        trả_về Size(m.size[1], m.size[0]);
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        trả_về buf->size();
    }

    nếu( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        trả_về d_mat->size();
    }

    nếu( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        trả_về cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

số_nguyên _InputArray::sizend(số_nguyên* arrsz, số_nguyên i) const
{
    số_nguyên j, d = 0;
    _InputArray::KindFlag k = kind();

    nếu( k == NONE )
        ;
    không_thì nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        nếu(arrsz)
            cho(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    không_thì nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        nếu(arrsz)
            cho(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    không_thì nếu( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (số_nguyên)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        nếu(arrsz)
            cho(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    không_thì nếu( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        nếu(arrsz)
            cho(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    không_thì nếu( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (số_nguyên)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        nếu(arrsz)
            cho(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    không_thì nếu (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        nếu(arrsz)
        {
            arrsz[0] = số_nguyên(n);
        }
    }
    không_thì
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        nếu(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    trả_về d;
}

bool _InputArray::empty(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();
    nếu (i >= 0) {
        nếu (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            trả_về mv->at(i).empty();
        }
        không_thì nếu (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            trả_về umv->at(i).empty();
        }
        không_thì nếu (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            trả_về n == 0;
        } không_thì {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    trả_về empty();
}

MatShape _InputArray::shape(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    nếu( k == NONE )
        ;
    không_thì nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    không_thì nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    không_thì nếu( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    không_thì nếu( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    không_thì nếu( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    không_thì
    {
        số_nguyên sizes[CV_MAX_DIM];
        số_nguyên dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    trả_về shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    nếu( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        nếu( k2 == MAT )
            trả_về m->size == ((const Mat*)arr.obj)->size;
        nếu( k2 == UMAT )
            trả_về m->size == ((const UMat*)arr.obj)->size;
        nếu( m->dims > 2 )
            trả_về false;
        sz1 = m->size();
    }
    không_thì nếu( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        nếu( k2 == MAT )
            trả_về m->size == ((const Mat*)arr.obj)->size;
        nếu( k2 == UMAT )
            trả_về m->size == ((const UMat*)arr.obj)->size;
        nếu( m->dims > 2 )
            trả_về false;
        sz1 = m->size();
    }
    không_thì nếu ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        trả_về ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    không_thì
        sz1 = size();

    nếu( arr.dims() > 2 )
        trả_về false;
    trả_về sz1 == arr.size();
}

số_nguyên _InputArray::dims_(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const Mat*)obj)->dims;
    }

    nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const UMat*)obj)->dims;
    }

    nếu (k == MATX)
    {
        CV_Assert( i < 0 );
        trả_về 2;
    }

    nếu( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        trả_về 1;
    }

    nếu( k == NONE )
        trả_về 0;

    nếu( k == STD_VECTOR_VECTOR )
    {
        nếu( i < 0 )
            trả_về 1;
        CV_Assert( size_t(i) < total() );
        trả_về 1;
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        nếu( i < 0 )
            trả_về 1;
        CV_Assert( i < (số_nguyên)vv.size() );

        trả_về vv[i].dims;
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        nếu( i < 0 )
            trả_về 1;
        CV_Assert( i < sz.height );

        trả_về vv[i].dims;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        nếu( i < 0 )
            trả_về 1;
        CV_Assert( i < (số_nguyên)vv.size() );

        trả_về vv[i].dims;
    }

    nếu( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        trả_về 2;
    }

    nếu( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        trả_về 2;
    }

    nếu( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        trả_về ((const cuda::GpuMatND*)obj)->dims;
    }

    nếu( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        trả_về 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const Mat*)obj)->total();
    }

    nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const UMat*)obj)->total();
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        nếu( i < 0 )
            trả_về vv.size();

        CV_Assert( i < (số_nguyên)vv.size() );
        trả_về vv[i].total();
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        nếu( i < 0 )
            trả_về sz.height;

        CV_Assert( i < sz.height );
        trả_về vv[i].total();
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        nếu( i < 0 )
            trả_về vv.size();

        CV_Assert( i < (số_nguyên)vv.size() );
        trả_về vv[i].total();
    }

    nếu( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        nếu (_gpuMatND.dims > 0)
        {
            res = 1;
            cho(số_nguyên d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            trả_về res;
        }
    }

    nếu (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        số_nguyên esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            nếu (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                nếu (i < 0) \
                    trả_về n; \
                CV_Assert((size_t)i < n); \
                trả_về vv->at(i).size(); \
            } không_thì \
                trả_về ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    trả_về size(i).area();
}

số_nguyên _InputArray::type_(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
        trả_về ((const Mat*)obj)->type();

    nếu( k == UMAT )
        trả_về ((const UMat*)obj)->type();

    nếu( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        trả_về CV_MAT_TYPE(flags);

    nếu( k == NONE )
        trả_về -1;

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        nếu( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            trả_về CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (số_nguyên)vv.size() );
        trả_về vv[i >= 0 ? i : 0].type();
    }

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        nếu( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            trả_về CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (số_nguyên)vv.size() );
        trả_về vv[i >= 0 ? i : 0].type();
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        nếu( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            trả_về CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        trả_về vv[i >= 0 ? i : 0].type();
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        nếu (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            trả_về CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (số_nguyên)vv.size());
        trả_về vv[i >= 0 ? i : 0].type();
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        nếu (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            trả_về CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (số_nguyên)vv.size());
        trả_về vv[i >= 0 ? i : 0].type();
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu( k == OPENGL_BUFFER )
        trả_về ((const ogl::Buffer*)obj)->type();

    nếu( k == CUDA_GPU_MAT )
        trả_về ((const cuda::GpuMat*)obj)->type();

    nếu( k == CUDA_GPU_MATND )
        trả_về ((const cuda::GpuMatND*)obj)->type();

    nếu( k == CUDA_HOST_MEM )
        trả_về ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

số_nguyên _InputArray::depth_(số_nguyên i) const
{
    trả_về CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
        trả_về ((const Mat*)obj)->empty();

    nếu( k == UMAT )
        trả_về ((const UMat*)obj)->empty();

    nếu (k == MATX)
        trả_về false;

    nếu( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        trả_về n == 0;
    }

    nếu( k == NONE )
        trả_về true;

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        trả_về vv.empty();
    }

    nếu( k == STD_ARRAY_MAT )
    {
        trả_về sz.height == 0;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        trả_về vv.empty();
    }

    nếu( k == OPENGL_BUFFER )
        trả_về ((const ogl::Buffer*)obj)->empty();

    nếu( k == CUDA_GPU_MAT )
        trả_về ((const cuda::GpuMat*)obj)->empty();

    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        trả_về vv.empty();
    }

    nếu( k == CUDA_GPU_MATND )
        trả_về ((const cuda::GpuMatND*)obj)->empty();

    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        trả_về vv.empty();
    }

    nếu( k == CUDA_HOST_MEM )
        trả_về ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
        trả_về i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    nếu( k == UMAT )
        trả_về i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    nếu( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        trả_về true;

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].isContinuous();
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        trả_về vv[i].isContinuous();
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].isContinuous();
    }

    nếu( k == CUDA_GPU_MAT )
      trả_về i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    nếu( k == CUDA_GPU_MATND )
        trả_về i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
        trả_về i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    nếu( k == UMAT )
        trả_về i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    nếu( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        trả_về false;

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].isSubmatrix();
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        trả_về vv[i].isSubmatrix();
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].isSubmatrix();
    }

    nếu( k == CUDA_GPU_MATND )
    {
        trả_về ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        trả_về (size_t)(m->ptr() - m->datastart);
    }

    nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const UMat*)obj)->offset;
    }

    nếu( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        trả_về 0;

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (số_nguyên)vv.size() );

        trả_về (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        trả_về (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].offset;
    }

    nếu( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        trả_về (size_t)(m->data - m->datastart);
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const Mat*)obj)->step;
    }

    nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const UMat*)obj)->step;
    }

    nếu( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        trả_về 0;

    nếu( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (số_nguyên)vv.size() );
        trả_về vv[i].step;
    }

    nếu( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        trả_về vv[i].step;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].step;
    }

    nếu( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        trả_về ((const cuda::GpuMat*)obj)->step;
    }
    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        trả_về vv[i].step;
    }
    nếu( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        trả_về _gpuMatND.step[i];
    }

    nếu (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        trả_về _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == NONE )
        arr.release();
    không_thì nếu( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    không_thì nếu( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    không_thì nếu (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    không_thì
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == NONE )
        arr.release();
    không_thì nếu( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    không_thì nếu( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    không_thì nếu (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    không_thì
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    trả_về (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    trả_về (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, số_nguyên mtype, số_nguyên i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    nếu( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        trả_về;
    }
    nếu( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        trả_về;
    }
    nếu( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    nếu( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    nếu( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    nếu( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    số_nguyên sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(số_nguyên _rows, số_nguyên _cols, số_nguyên mtype, số_nguyên i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    nếu( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        trả_về;
    }
    nếu( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        trả_về;
    }
    nếu( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    nếu( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    nếu( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    nếu( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    số_nguyên sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(số_nguyên d, const số_nguyên* sizes, số_nguyên mtype, số_nguyên i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    số_nguyên size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    nếu( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        nếu (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            không_thì
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        nếu(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            cho(số_nguyên j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        trả_về;
    }

    nếu( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        nếu (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            không_thì
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        nếu(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            cho(số_nguyên j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        trả_về;
    }

    nếu( k == MATX )
    {
        CV_Assert( i < 0 );
        số_nguyên type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        nếu (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            số_nguyên total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        không_thì
        {
            nếu (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            không_thì
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        trả_về;
    }

    nếu( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        số_nguyên esz = CV_ELEM_SIZE(flags);

        nếu( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            số_nguyên type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            nếu (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                nếu (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    trả_về; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } không_thì { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        break

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        trả_về;
    }

    nếu( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    nếu( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        nếu( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            nếu( fixedType() )
            {
                số_nguyên _type = CV_MAT_TYPE(flags);
                cho( size_t j = len0; j < len; j++ )
                {
                    nếu( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            trả_về;
        }

        CV_Assert( i < (số_nguyên)v.size() );
        Mat& m = v[i];

        nếu( allowTransposed )
        {
            nếu( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            nếu( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            không_thì
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        nếu(fixedSize())
        {
            CV_Assert(m.dims == d);
            cho(số_nguyên j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        trả_về;
    }

    nếu( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        nếu( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            nếu( fixedType() )
            {
                số_nguyên _type = CV_MAT_TYPE(flags);
                cho( size_t j = len0; j < len; j++ )
                {
                    nếu( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            trả_về;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        nếu( allowTransposed )
        {
            nếu( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            nếu( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            không_thì
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        nếu(fixedSize())
        {
            CV_Assert(m.dims == d);
            cho(số_nguyên j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        trả_về;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        nếu( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            nếu( fixedType() )
            {
                số_nguyên _type = CV_MAT_TYPE(flags);
                cho( size_t j = len0; j < len; j++ )
                {
                    nếu( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            trả_về;
        }

        CV_Assert( i < (số_nguyên)v.size() );
        UMat& m = v[i];

        nếu( allowTransposed )
        {
            nếu( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            nếu( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            không_thì
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        nếu(fixedSize())
        {
            CV_Assert(m.dims == d);
            cho(số_nguyên j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        trả_về;
    }

    nếu ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        trả_về;
    }

    nếu( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, số_nguyên mtype, số_nguyên i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    nếu (shape.dims < 0) {
        release();
    } không_thì {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(số_nguyên mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    trả_về getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, số_nguyên mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, số_nguyên mtype, số_nguyên i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    số_nguyên sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(số_nguyên rows, số_nguyên cols, số_nguyên mtype, số_nguyên i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    số_nguyên sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, số_nguyên mtype, số_nguyên i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    nếu (shape.dims < 0) {
        release();
        trả_về;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    nếu ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } không_thì nếu ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, số_nguyên mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(số_nguyên d, const số_nguyên* sizes, số_nguyên mtype, số_nguyên i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    số_nguyên size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    nếu( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        nếu (k == MAT)
            m = (Mat*)obj;
        không_thì {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        nếu (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            không_thì
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        nếu(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            cho(số_nguyên j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        trả_về;
    }

    nếu( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        nếu (k == UMAT)
            m = (UMat*)obj;
        không_thì {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        nếu (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            trả_về;
        }

        nếu(fixedType())
        {
            nếu(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            không_thì
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        nếu(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            cho(số_nguyên j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        trả_về;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        ((Mat*)obj)->release();
        trả_về;
    }

    nếu( k == UMAT )
    {
        ((UMat*)obj)->release();
        trả_về;
    }

    nếu( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    nếu( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    nếu( k == NONE )
        trả_về;

    nếu( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        nếu (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } không_thì { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        break

        số_nguyên esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        trả_về;
    }

    nếu( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        trả_về;
    }

    nếu( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        cho( số_nguyên i = 0; i < sz.height; i++ )
            v[i].release();
        trả_về;
    }

    nếu( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        trả_về;
    }

    nếu( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        trả_về;
    }
    nếu (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        trả_về;
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    nếu( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        trả_về;
    }

    release();
}

bool _OutputArray::needed() const
{
    trả_về kind() != NONE;
}

Mat& _OutputArray::getMatRef(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();
    nếu( i < 0 )
    {
        CV_Assert( k == MAT );
        trả_về *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    nếu( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (số_nguyên)v.size() );
        trả_về v[i];
    }
    không_thì
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        trả_về v[i];
    }
}

UMat& _OutputArray::getUMatRef(số_nguyên i) const
{
    _InputArray::KindFlag k = kind();
    nếu( i < 0 )
    {
        CV_Assert( k == UMAT );
        trả_về *(UMat*)obj;
    }
    không_thì
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (số_nguyên)v.size() );
        trả_về v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    trả_về *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    trả_về *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    trả_về *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    trả_về *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    trả_về *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    trả_về *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    nếu( k == NONE )
        ;
    không_thì nếu (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    không_thì nếu( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    không_thì nếu( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<double, 4>(value.ptr<double>())), mask);
#không_thì
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    không_thì
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    nếu( k == NONE )
        ;
    không_thì nếu (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    không_thì
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    nếu (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    không_thì nếu (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    không_thì nếu (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    không_thì
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    nếu (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    không_thì nếu (k == MAT)
    {
        *(Mat*)obj = m;
    }
    không_thì nếu (k == MATX)
    {
        m.copyTo(getMat());
    }
    không_thì
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    nếu (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        trả_về;
    }
    số_nguyên k = kind();
    nếu (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    không_thì nếu (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    không_thì nếu (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    không_thì
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    nếu (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        trả_về;
    }
    số_nguyên k = kind();
    nếu (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    không_thì nếu (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    không_thì nếu (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    không_thì
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    nếu (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        cho (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            nếu (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    không_thì nếu (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        cho (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            nếu (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    không_thì
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    nếu (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        cho (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            nếu (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    không_thì nếu (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        cho (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            nếu (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    không_thì
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { trả_về _none; }

} // cv::
