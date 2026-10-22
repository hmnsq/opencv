// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<整数, 5> Vec5i;
typedef Vec<整数, 7> Vec7i;
typedef Vec<整数, 9> Vec9i;
typedef Vec<整数, 10> Vec10i;
typedef Vec<整数, 11> Vec11i;
typedef Vec<整数, 12> Vec12i;
typedef Vec<整数, 13> Vec13i;
typedef Vec<整数, 14> Vec14i;
typedef Vec<整数, 15> Vec15i;
typedef Vec<整数, 16> Vec16i;
typedef Vec<整数, 32> Vec32i;
typedef Vec<整数, 64> Vec64i;
typedef Vec<整数, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    case 1: func(uchar); \
    case 2: func(Vec2b); \
    case 3: func(Vec3b); \
    case 4: func(整数); \
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

Mat _InputArray::getMat_(整数 i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    如果( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        如果( i < 0 )
            返回 *m;
        返回 m->row(i);
    }

    如果( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        如果( i < 0 )
            返回 m->getMat(accessFlags);
        返回 m->getMat(accessFlags).row(i);
    }

    如果 (k == MATX)
    {
        CV_Assert( i < 0 );
        返回 Mat(sz, flags, obj);
    }

    如果( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        整数 t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        整数 j, n = (整数)v.size();
        如果( n == 0 )
            返回 Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        为( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        返回 m;
    }

    如果( k == NONE )
        返回 Mat();

    如果( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        整数 t = CV_MAT_TYPE(flags);
        整数 esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            如果 (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } 否则 { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            整数 v_sz = 整数(v->size()); \
            返回 v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (整数)v.size() );

        返回 v[i];
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        返回 v[i];
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (整数)v.size() );

        返回 v[i].getMat(accessFlags);
    }

    如果( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    如果( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    如果( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    如果( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        返回 cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(整数 i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    如果( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        如果( i < 0 )
            返回 *m;
        返回 m->row(i);
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (整数)v.size() );

        返回 v[i];
    }

    如果( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        如果( i < 0 )
            返回 m->getUMat(accessFlags);
        返回 m->row(i).getUMat(accessFlags);
    }

    返回 getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    如果( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        整数 n = (整数)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        为( 整数 i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        返回;
    }

    如果 (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        为( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        返回;
    }

    如果( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        整数 t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        为( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        返回;
    }

    如果( k == NONE )
    {
        mv.clear();
        返回;
    }

    如果( k == STD_VECTOR_VECTOR )
    {
        整数 typ = CV_MAT_TYPE(flags);
        整数 esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            为 (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                整数 ni = (整数)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        break

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        返回;
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        为( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        返回;
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        为( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        返回;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        为( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        返回;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    如果( k == NONE )
    {
        umv.clear();
        返回;
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        为( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        返回;
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        为( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        返回;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        为( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        返回;
    }

    如果( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        返回;
    }
    如果( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        返回;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    如果 (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        返回 *d_mat;
    }

    如果 (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        返回 cuda_mem->createGpuMatHeader();
    }

    如果 (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    如果 (k == NONE)
        返回 cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#否则
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#否则
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#否则
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    如果 (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        返回 *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#否则
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    返回 *gl_buf;
}

整数 _InputArray::rows_(整数 i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    如果 (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        返回 (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    返回 size_(i).height;
}

整数 _InputArray::cols_(整数 i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    如果 (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        返回 (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    返回 size_(i).width;
}

Size _InputArray::size_(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        返回 Size(m->cols, m->rows);
    }

    如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        返回 Size(m->cols, m->rows);
    }

    如果 (k == MATX)
    {
        CV_Assert( i < 0 );
        返回 sz;
    }

    如果( k == NONE )
        返回 Size();

    如果( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        返回 Size((整数)n, 1);
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        如果( i < 0 )
            返回 vv.empty() ? Size() : Size((整数)vv.size(), 1);
        CV_Assert( i < (整数)vv.size() );

        返回 vv[i].size();
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        如果( i < 0 )
            返回 sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        返回 vv[i].size();
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        如果 (i < 0)
            返回 vv.empty() ? Size() : Size((整数)vv.size(), 1);
        CV_Assert(i < (整数)vv.size());
        返回 vv[i].size();
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        如果 (i < 0)
            返回 vv.empty() ? Size() : Size((整数)vv.size(), 1);
        CV_Assert(i < (整数)vv.size());
        const cuda::GpuMatND& m = vv[i];
        如果 (m.dims == 0)
            返回 Size();
        如果 (m.dims == 1)
            返回 Size(1, m.size[0]);
        返回 Size(m.size[1], m.size[0]);
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        如果( i < 0 )
            返回 vv.empty() ? Size() : Size((整数)vv.size(), 1);
        CV_Assert( i < (整数)vv.size() );

        返回 vv[i].size();
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        如果 (i < 0)
            返回 vv.empty() ? Size() : Size((整数)vv.size(), 1);
        CV_Assert(i < (整数)vv.size());
        const cuda::GpuMatND& m = vv[i];
        如果 (m.dims == 0)
            返回 Size();
        如果 (m.dims == 1)
            返回 Size(1, m.size[0]);
        返回 Size(m.size[1], m.size[0]);
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        返回 buf->size();
    }

    如果( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        返回 d_mat->size();
    }

    如果( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        返回 cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

整数 _InputArray::sizend(整数* arrsz, 整数 i) const
{
    整数 j, d = 0;
    _InputArray::KindFlag k = kind();

    如果( k == NONE )
        ;
    否则 如果( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        如果(arrsz)
            为(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    否则 如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        如果(arrsz)
            为(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    否则 如果( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (整数)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        如果(arrsz)
            为(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    否则 如果( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        如果(arrsz)
            为(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    否则 如果( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (整数)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        如果(arrsz)
            为(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    否则 如果 (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        如果(arrsz)
        {
            arrsz[0] = 整数(n);
        }
    }
    否则
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        如果(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    返回 d;
}

bool _InputArray::empty(整数 i) const
{
    _InputArray::KindFlag k = kind();
    如果 (i >= 0) {
        如果 (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            返回 mv->at(i).empty();
        }
        否则 如果 (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            返回 umv->at(i).empty();
        }
        否则 如果 (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            返回 n == 0;
        } 否则 {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    返回 empty();
}

MatShape _InputArray::shape(整数 i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    如果( k == NONE )
        ;
    否则 如果( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    否则 如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    否则 如果( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    否则 如果( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    否则 如果( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    否则
    {
        整数 sizes[CV_MAX_DIM];
        整数 dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    返回 shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    如果( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        如果( k2 == MAT )
            返回 m->size == ((const Mat*)arr.obj)->size;
        如果( k2 == UMAT )
            返回 m->size == ((const UMat*)arr.obj)->size;
        如果( m->dims > 2 )
            返回 false;
        sz1 = m->size();
    }
    否则 如果( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        如果( k2 == MAT )
            返回 m->size == ((const Mat*)arr.obj)->size;
        如果( k2 == UMAT )
            返回 m->size == ((const UMat*)arr.obj)->size;
        如果( m->dims > 2 )
            返回 false;
        sz1 = m->size();
    }
    否则 如果 ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        返回 ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    否则
        sz1 = size();

    如果( arr.dims() > 2 )
        返回 false;
    返回 sz1 == arr.size();
}

整数 _InputArray::dims_(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        CV_Assert( i < 0 );
        返回 ((const Mat*)obj)->dims;
    }

    如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        返回 ((const UMat*)obj)->dims;
    }

    如果 (k == MATX)
    {
        CV_Assert( i < 0 );
        返回 2;
    }

    如果( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        返回 1;
    }

    如果( k == NONE )
        返回 0;

    如果( k == STD_VECTOR_VECTOR )
    {
        如果( i < 0 )
            返回 1;
        CV_Assert( size_t(i) < total() );
        返回 1;
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        如果( i < 0 )
            返回 1;
        CV_Assert( i < (整数)vv.size() );

        返回 vv[i].dims;
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        如果( i < 0 )
            返回 1;
        CV_Assert( i < sz.height );

        返回 vv[i].dims;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        如果( i < 0 )
            返回 1;
        CV_Assert( i < (整数)vv.size() );

        返回 vv[i].dims;
    }

    如果( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        返回 2;
    }

    如果( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        返回 2;
    }

    如果( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        返回 ((const cuda::GpuMatND*)obj)->dims;
    }

    如果( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        返回 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        CV_Assert( i < 0 );
        返回 ((const Mat*)obj)->total();
    }

    如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        返回 ((const UMat*)obj)->total();
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        如果( i < 0 )
            返回 vv.size();

        CV_Assert( i < (整数)vv.size() );
        返回 vv[i].total();
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        如果( i < 0 )
            返回 sz.height;

        CV_Assert( i < sz.height );
        返回 vv[i].total();
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        如果( i < 0 )
            返回 vv.size();

        CV_Assert( i < (整数)vv.size() );
        返回 vv[i].total();
    }

    如果( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        如果 (_gpuMatND.dims > 0)
        {
            res = 1;
            为(整数 d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            返回 res;
        }
    }

    如果 (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        整数 esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            如果 (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                如果 (i < 0) \
                    返回 n; \
                CV_Assert((size_t)i < n); \
                返回 vv->at(i).size(); \
            } 否则 \
                返回 ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    返回 size(i).area();
}

整数 _InputArray::type_(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
        返回 ((const Mat*)obj)->type();

    如果( k == UMAT )
        返回 ((const UMat*)obj)->type();

    如果( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        返回 CV_MAT_TYPE(flags);

    如果( k == NONE )
        返回 -1;

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        如果( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            返回 CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (整数)vv.size() );
        返回 vv[i >= 0 ? i : 0].type();
    }

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        如果( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            返回 CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (整数)vv.size() );
        返回 vv[i >= 0 ? i : 0].type();
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        如果( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            返回 CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        返回 vv[i >= 0 ? i : 0].type();
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        如果 (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            返回 CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (整数)vv.size());
        返回 vv[i >= 0 ? i : 0].type();
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        如果 (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            返回 CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (整数)vv.size());
        返回 vv[i >= 0 ? i : 0].type();
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果( k == OPENGL_BUFFER )
        返回 ((const ogl::Buffer*)obj)->type();

    如果( k == CUDA_GPU_MAT )
        返回 ((const cuda::GpuMat*)obj)->type();

    如果( k == CUDA_GPU_MATND )
        返回 ((const cuda::GpuMatND*)obj)->type();

    如果( k == CUDA_HOST_MEM )
        返回 ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

整数 _InputArray::depth_(整数 i) const
{
    返回 CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
        返回 ((const Mat*)obj)->empty();

    如果( k == UMAT )
        返回 ((const UMat*)obj)->empty();

    如果 (k == MATX)
        返回 false;

    如果( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        返回 n == 0;
    }

    如果( k == NONE )
        返回 true;

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        返回 vv.empty();
    }

    如果( k == STD_ARRAY_MAT )
    {
        返回 sz.height == 0;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        返回 vv.empty();
    }

    如果( k == OPENGL_BUFFER )
        返回 ((const ogl::Buffer*)obj)->empty();

    如果( k == CUDA_GPU_MAT )
        返回 ((const cuda::GpuMat*)obj)->empty();

    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        返回 vv.empty();
    }

    如果( k == CUDA_GPU_MATND )
        返回 ((const cuda::GpuMatND*)obj)->empty();

    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        返回 vv.empty();
    }

    如果( k == CUDA_HOST_MEM )
        返回 ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
        返回 i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    如果( k == UMAT )
        返回 i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    如果( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        返回 true;

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].isContinuous();
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        返回 vv[i].isContinuous();
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].isContinuous();
    }

    如果( k == CUDA_GPU_MAT )
      返回 i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    如果( k == CUDA_GPU_MATND )
        返回 i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
        返回 i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    如果( k == UMAT )
        返回 i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    如果( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        返回 false;

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].isSubmatrix();
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        返回 vv[i].isSubmatrix();
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].isSubmatrix();
    }

    如果( k == CUDA_GPU_MATND )
    {
        返回 ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        返回 (size_t)(m->ptr() - m->datastart);
    }

    如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        返回 ((const UMat*)obj)->offset;
    }

    如果( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        返回 0;

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (整数)vv.size() );

        返回 (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        返回 (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].offset;
    }

    如果( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        返回 (size_t)(m->data - m->datastart);
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(整数 i) const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        CV_Assert( i < 0 );
        返回 ((const Mat*)obj)->step;
    }

    如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        返回 ((const UMat*)obj)->step;
    }

    如果( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        返回 0;

    如果( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (整数)vv.size() );
        返回 vv[i].step;
    }

    如果( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        返回 vv[i].step;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].step;
    }

    如果( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        返回 ((const cuda::GpuMat*)obj)->step;
    }
    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        返回 vv[i].step;
    }
    如果( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        返回 _gpuMatND.step[i];
    }

    如果 (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        返回 _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    如果( k == NONE )
        arr.release();
    否则 如果( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    否则 如果( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    否则 如果 (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    否则
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    如果( k == NONE )
        arr.release();
    否则 如果( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    否则 如果( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    否则 如果 (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    否则
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    返回 (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    返回 (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, 整数 mtype, 整数 i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    如果( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        返回;
    }
    如果( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        返回;
    }
    如果( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    如果( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    如果( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    如果( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    整数 sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(整数 _rows, 整数 _cols, 整数 mtype, 整数 i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    如果( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        返回;
    }
    如果( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        返回;
    }
    如果( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    如果( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    如果( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    如果( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    整数 sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(整数 d, const 整数* sizes, 整数 mtype, 整数 i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    整数 size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    如果( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        如果 (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            否则
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        如果(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            为(整数 j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        返回;
    }

    如果( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        如果 (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            否则
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        如果(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            为(整数 j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        返回;
    }

    如果( k == MATX )
    {
        CV_Assert( i < 0 );
        整数 type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        如果 (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            整数 total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        否则
        {
            如果 (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            否则
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        返回;
    }

    如果( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        整数 esz = CV_ELEM_SIZE(flags);

        如果( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            整数 type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            如果 (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                如果 (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    返回; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } 否则 { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        break

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        返回;
    }

    如果( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    如果( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        如果( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            如果( fixedType() )
            {
                整数 _type = CV_MAT_TYPE(flags);
                为( size_t j = len0; j < len; j++ )
                {
                    如果( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            返回;
        }

        CV_Assert( i < (整数)v.size() );
        Mat& m = v[i];

        如果( allowTransposed )
        {
            如果( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            如果( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            否则
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        如果(fixedSize())
        {
            CV_Assert(m.dims == d);
            为(整数 j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        返回;
    }

    如果( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        如果( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            如果( fixedType() )
            {
                整数 _type = CV_MAT_TYPE(flags);
                为( size_t j = len0; j < len; j++ )
                {
                    如果( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            返回;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        如果( allowTransposed )
        {
            如果( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            如果( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            否则
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        如果(fixedSize())
        {
            CV_Assert(m.dims == d);
            为(整数 j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        返回;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        如果( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            如果( fixedType() )
            {
                整数 _type = CV_MAT_TYPE(flags);
                为( size_t j = len0; j < len; j++ )
                {
                    如果( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            返回;
        }

        CV_Assert( i < (整数)v.size() );
        UMat& m = v[i];

        如果( allowTransposed )
        {
            如果( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            如果( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            否则
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        如果(fixedSize())
        {
            CV_Assert(m.dims == d);
            为(整数 j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        返回;
    }

    如果 ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        返回;
    }

    如果( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, 整数 mtype, 整数 i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    如果 (shape.dims < 0) {
        release();
    } 否则 {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(整数 mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    返回 getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, 整数 mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, 整数 mtype, 整数 i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    整数 sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(整数 rows, 整数 cols, 整数 mtype, 整数 i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    整数 sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, 整数 mtype, 整数 i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    如果 (shape.dims < 0) {
        release();
        返回;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    如果 ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } 否则 如果 ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, 整数 mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(整数 d, const 整数* sizes, 整数 mtype, 整数 i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    整数 size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    如果( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        如果 (k == MAT)
            m = (Mat*)obj;
        否则 {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        如果 (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            否则
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        如果(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            为(整数 j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        返回;
    }

    如果( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        如果 (k == UMAT)
            m = (UMat*)obj;
        否则 {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        如果 (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            返回;
        }

        如果(fixedType())
        {
            如果(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            否则
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        如果(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            为(整数 j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        返回;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        ((Mat*)obj)->release();
        返回;
    }

    如果( k == UMAT )
    {
        ((UMat*)obj)->release();
        返回;
    }

    如果( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    如果( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    如果( k == NONE )
        返回;

    如果( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        如果 (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } 否则 { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        break

        整数 esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        返回;
    }

    如果( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        返回;
    }

    如果( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        为( 整数 i = 0; i < sz.height; i++ )
            v[i].release();
        返回;
    }

    如果( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        返回;
    }

    如果( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        返回;
    }
    如果 (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        返回;
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    如果( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        返回;
    }

    release();
}

bool _OutputArray::needed() const
{
    返回 kind() != NONE;
}

Mat& _OutputArray::getMatRef(整数 i) const
{
    _InputArray::KindFlag k = kind();
    如果( i < 0 )
    {
        CV_Assert( k == MAT );
        返回 *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    如果( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (整数)v.size() );
        返回 v[i];
    }
    否则
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        返回 v[i];
    }
}

UMat& _OutputArray::getUMatRef(整数 i) const
{
    _InputArray::KindFlag k = kind();
    如果( i < 0 )
    {
        CV_Assert( k == UMAT );
        返回 *(UMat*)obj;
    }
    否则
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (整数)v.size() );
        返回 v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    返回 *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    返回 *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    返回 *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    返回 *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    返回 *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    返回 *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    如果( k == NONE )
        ;
    否则 如果 (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    否则 如果( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    否则 如果( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<double, 4>(value.ptr<double>())), mask);
#否则
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    否则
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    如果( k == NONE )
        ;
    否则 如果 (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    否则
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    如果 (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    否则 如果 (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    否则 如果 (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    否则
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    如果 (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    否则 如果 (k == MAT)
    {
        *(Mat*)obj = m;
    }
    否则 如果 (k == MATX)
    {
        m.copyTo(getMat());
    }
    否则
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    如果 (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        返回;
    }
    整数 k = kind();
    如果 (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    否则 如果 (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    否则 如果 (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    否则
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    如果 (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        返回;
    }
    整数 k = kind();
    如果 (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    否则 如果 (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    否则 如果 (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    否则
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    如果 (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        为 (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            如果 (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    否则 如果 (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        为 (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            如果 (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    否则
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    如果 (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        为 (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            如果 (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    否则 如果 (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        为 (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            如果 (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    否则
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { 返回 _none; }

} // cv::
