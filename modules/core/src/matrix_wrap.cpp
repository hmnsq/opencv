// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<цяло, 5> Vec5i;
typedef Vec<цяло, 7> Vec7i;
typedef Vec<цяло, 9> Vec9i;
typedef Vec<цяло, 10> Vec10i;
typedef Vec<цяло, 11> Vec11i;
typedef Vec<цяло, 12> Vec12i;
typedef Vec<цяло, 13> Vec13i;
typedef Vec<цяло, 14> Vec14i;
typedef Vec<цяло, 15> Vec15i;
typedef Vec<цяло, 16> Vec16i;
typedef Vec<цяло, 32> Vec32i;
typedef Vec<цяло, 64> Vec64i;
typedef Vec<цяло, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    case 1: func(uchar); \
    case 2: func(Vec2b); \
    case 3: func(Vec3b); \
    case 4: func(цяло); \
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

Mat _InputArray::getMat_(цяло i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ако( i < 0 )
            върни *m;
        върни m->row(i);
    }

    ако( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ако( i < 0 )
            върни m->getMat(accessFlags);
        върни m->getMat(accessFlags).row(i);
    }

    ако (k == MATX)
    {
        CV_Assert( i < 0 );
        върни Mat(sz, flags, obj);
    }

    ако( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        цяло t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        цяло j, n = (цяло)v.size();
        ако( n == 0 )
            върни Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        за( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        върни m;
    }

    ако( k == NONE )
        върни Mat();

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        цяло t = CV_MAT_TYPE(flags);
        цяло esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ако (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } иначе { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            цяло v_sz = цяло(v->size()); \
            върни v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (цяло)v.size() );

        върни v[i];
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        върни v[i];
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (цяло)v.size() );

        върни v[i].getMat(accessFlags);
    }

    ако( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ако( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ако( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        върни cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(цяло i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ако( i < 0 )
            върни *m;
        върни m->row(i);
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (цяло)v.size() );

        върни v[i];
    }

    ако( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ако( i < 0 )
            върни m->getUMat(accessFlags);
        върни m->row(i).getUMat(accessFlags);
    }

    върни getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        цяло n = (цяло)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        за( цяло i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        върни;
    }

    ако (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        върни;
    }

    ако( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        цяло t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        върни;
    }

    ако( k == NONE )
    {
        mv.clear();
        върни;
    }

    ако( k == STD_VECTOR_VECTOR )
    {
        цяло typ = CV_MAT_TYPE(flags);
        цяло esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            за (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                цяло ni = (цяло)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        break

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        върни;
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        върни;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        върни;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        върни;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == NONE )
    {
        umv.clear();
        върни;
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        за( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        върни;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        за( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        върни;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        за( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        върни;
    }

    ако( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        върни;
    }
    ако( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        върни;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ако (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        върни *d_mat;
    }

    ако (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        върни cuda_mem->createGpuMatHeader();
    }

    ако (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ако (k == NONE)
        върни cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#иначе
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#иначе
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#иначе
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ако (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        върни *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#иначе
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    върни *gl_buf;
}

цяло _InputArray::rows_(цяло i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ако (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        върни (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    върни size_(i).height;
}

цяло _InputArray::cols_(цяло i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ако (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        върни (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    върни size_(i).width;
}

Size _InputArray::size_(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        върни Size(m->cols, m->rows);
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        върни Size(m->cols, m->rows);
    }

    ако (k == MATX)
    {
        CV_Assert( i < 0 );
        върни sz;
    }

    ако( k == NONE )
        върни Size();

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        върни Size((цяло)n, 1);
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( i < 0 )
            върни vv.empty() ? Size() : Size((цяло)vv.size(), 1);
        CV_Assert( i < (цяло)vv.size() );

        върни vv[i].size();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( i < 0 )
            върни sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        върни vv[i].size();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ако (i < 0)
            върни vv.empty() ? Size() : Size((цяло)vv.size(), 1);
        CV_Assert(i < (цяло)vv.size());
        върни vv[i].size();
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ако (i < 0)
            върни vv.empty() ? Size() : Size((цяло)vv.size(), 1);
        CV_Assert(i < (цяло)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ако (m.dims == 0)
            върни Size();
        ако (m.dims == 1)
            върни Size(1, m.size[0]);
        върни Size(m.size[1], m.size[0]);
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( i < 0 )
            върни vv.empty() ? Size() : Size((цяло)vv.size(), 1);
        CV_Assert( i < (цяло)vv.size() );

        върни vv[i].size();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ако (i < 0)
            върни vv.empty() ? Size() : Size((цяло)vv.size(), 1);
        CV_Assert(i < (цяло)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ако (m.dims == 0)
            върни Size();
        ако (m.dims == 1)
            върни Size(1, m.size[0]);
        върни Size(m.size[1], m.size[0]);
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        върни buf->size();
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        върни d_mat->size();
    }

    ако( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        върни cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

цяло _InputArray::sizend(цяло* arrsz, цяло i) const
{
    цяло j, d = 0;
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        ;
    иначе ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    иначе ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    иначе ако( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (цяло)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    иначе ако( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    иначе ако( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (цяло)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    иначе ако (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ако(arrsz)
        {
            arrsz[0] = цяло(n);
        }
    }
    иначе
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ако(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    върни d;
}

bool _InputArray::empty(цяло i) const
{
    _InputArray::KindFlag k = kind();
    ако (i >= 0) {
        ако (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            върни mv->at(i).empty();
        }
        иначе ако (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            върни umv->at(i).empty();
        }
        иначе ако (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            върни n == 0;
        } иначе {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    върни empty();
}

MatShape _InputArray::shape(цяло i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ако( k == NONE )
        ;
    иначе ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    иначе ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    иначе ако( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    иначе ако( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    иначе ако( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    иначе
    {
        цяло sizes[CV_MAX_DIM];
        цяло dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    върни shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ако( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ако( k2 == MAT )
            върни m->size == ((const Mat*)arr.obj)->size;
        ако( k2 == UMAT )
            върни m->size == ((const UMat*)arr.obj)->size;
        ако( m->dims > 2 )
            върни false;
        sz1 = m->size();
    }
    иначе ако( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ако( k2 == MAT )
            върни m->size == ((const Mat*)arr.obj)->size;
        ако( k2 == UMAT )
            върни m->size == ((const UMat*)arr.obj)->size;
        ако( m->dims > 2 )
            върни false;
        sz1 = m->size();
    }
    иначе ако ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        върни ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    иначе
        sz1 = size();

    ако( arr.dims() > 2 )
        върни false;
    върни sz1 == arr.size();
}

цяло _InputArray::dims_(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        върни ((const Mat*)obj)->dims;
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        върни ((const UMat*)obj)->dims;
    }

    ако (k == MATX)
    {
        CV_Assert( i < 0 );
        върни 2;
    }

    ако( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        върни 1;
    }

    ако( k == NONE )
        върни 0;

    ако( k == STD_VECTOR_VECTOR )
    {
        ако( i < 0 )
            върни 1;
        CV_Assert( size_t(i) < total() );
        върни 1;
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( i < 0 )
            върни 1;
        CV_Assert( i < (цяло)vv.size() );

        върни vv[i].dims;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( i < 0 )
            върни 1;
        CV_Assert( i < sz.height );

        върни vv[i].dims;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( i < 0 )
            върни 1;
        CV_Assert( i < (цяло)vv.size() );

        върни vv[i].dims;
    }

    ако( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        върни 2;
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        върни 2;
    }

    ако( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        върни ((const cuda::GpuMatND*)obj)->dims;
    }

    ако( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        върни 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        върни ((const Mat*)obj)->total();
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        върни ((const UMat*)obj)->total();
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( i < 0 )
            върни vv.size();

        CV_Assert( i < (цяло)vv.size() );
        върни vv[i].total();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( i < 0 )
            върни sz.height;

        CV_Assert( i < sz.height );
        върни vv[i].total();
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( i < 0 )
            върни vv.size();

        CV_Assert( i < (цяло)vv.size() );
        върни vv[i].total();
    }

    ако( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ако (_gpuMatND.dims > 0)
        {
            res = 1;
            за(цяло d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            върни res;
        }
    }

    ако (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        цяло esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ако (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ако (i < 0) \
                    върни n; \
                CV_Assert((size_t)i < n); \
                върни vv->at(i).size(); \
            } иначе \
                върни ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    върни size(i).area();
}

цяло _InputArray::type_(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        върни ((const Mat*)obj)->type();

    ако( k == UMAT )
        върни ((const UMat*)obj)->type();

    ако( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        върни CV_MAT_TYPE(flags);

    ако( k == NONE )
        върни -1;

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            върни CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (цяло)vv.size() );
        върни vv[i >= 0 ? i : 0].type();
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            върни CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (цяло)vv.size() );
        върни vv[i >= 0 ? i : 0].type();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            върни CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        върни vv[i >= 0 ? i : 0].type();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ако (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            върни CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (цяло)vv.size());
        върни vv[i >= 0 ? i : 0].type();
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ако (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            върни CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (цяло)vv.size());
        върни vv[i >= 0 ? i : 0].type();
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == OPENGL_BUFFER )
        върни ((const ogl::Buffer*)obj)->type();

    ако( k == CUDA_GPU_MAT )
        върни ((const cuda::GpuMat*)obj)->type();

    ако( k == CUDA_GPU_MATND )
        върни ((const cuda::GpuMatND*)obj)->type();

    ако( k == CUDA_HOST_MEM )
        върни ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

цяло _InputArray::depth_(цяло i) const
{
    върни CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        върни ((const Mat*)obj)->empty();

    ако( k == UMAT )
        върни ((const UMat*)obj)->empty();

    ако (k == MATX)
        върни false;

    ако( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        върни n == 0;
    }

    ако( k == NONE )
        върни true;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        върни vv.empty();
    }

    ако( k == STD_ARRAY_MAT )
    {
        върни sz.height == 0;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        върни vv.empty();
    }

    ако( k == OPENGL_BUFFER )
        върни ((const ogl::Buffer*)obj)->empty();

    ако( k == CUDA_GPU_MAT )
        върни ((const cuda::GpuMat*)obj)->empty();

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        върни vv.empty();
    }

    ако( k == CUDA_GPU_MATND )
        върни ((const cuda::GpuMatND*)obj)->empty();

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        върни vv.empty();
    }

    ако( k == CUDA_HOST_MEM )
        върни ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        върни i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ако( k == UMAT )
        върни i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        върни true;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].isContinuous();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        върни vv[i].isContinuous();
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].isContinuous();
    }

    ако( k == CUDA_GPU_MAT )
      върни i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ако( k == CUDA_GPU_MATND )
        върни i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        върни i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ако( k == UMAT )
        върни i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        върни false;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].isSubmatrix();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        върни vv[i].isSubmatrix();
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].isSubmatrix();
    }

    ако( k == CUDA_GPU_MATND )
    {
        върни ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        върни (size_t)(m->ptr() - m->datastart);
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        върни ((const UMat*)obj)->offset;
    }

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        върни 0;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (цяло)vv.size() );

        върни (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        върни (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].offset;
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        върни (size_t)(m->data - m->datastart);
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(цяло i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        върни ((const Mat*)obj)->step;
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        върни ((const UMat*)obj)->step;
    }

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        върни 0;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (цяло)vv.size() );
        върни vv[i].step;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        върни vv[i].step;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].step;
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        върни ((const cuda::GpuMat*)obj)->step;
    }
    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        върни vv[i].step;
    }
    ако( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        върни _gpuMatND.step[i];
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        върни _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        arr.release();
    иначе ако( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    иначе ако( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    иначе ако (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    иначе
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        arr.release();
    иначе ако( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    иначе ако( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    иначе ако (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    иначе
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    върни (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    върни (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, цяло mtype, цяло i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ако( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        върни;
    }
    ако( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        върни;
    }
    ако( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ако( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ако( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ако( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    цяло sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(цяло _rows, цяло _cols, цяло mtype, цяло i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ако( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        върни;
    }
    ако( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        върни;
    }
    ако( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ако( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ако( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ако( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    цяло sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(цяло d, const цяло* sizes, цяло mtype, цяло i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цяло size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ако (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            иначе
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            за(цяло j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        върни;
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ако (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            иначе
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            за(цяло j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        върни;
    }

    ако( k == MATX )
    {
        CV_Assert( i < 0 );
        цяло type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ако (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            цяло total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        иначе
        {
            ако (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            иначе
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        върни;
    }

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        цяло esz = CV_ELEM_SIZE(flags);

        ако( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            цяло type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ако (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ако (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    върни; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } иначе { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        break

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        върни;
    }

    ако( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ако( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ако( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ако( fixedType() )
            {
                цяло _type = CV_MAT_TYPE(flags);
                за( size_t j = len0; j < len; j++ )
                {
                    ако( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            върни;
        }

        CV_Assert( i < (цяло)v.size() );
        Mat& m = v[i];

        ако( allowTransposed )
        {
            ако( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ако( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            иначе
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ако(fixedSize())
        {
            CV_Assert(m.dims == d);
            за(цяло j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        върни;
    }

    ако( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ако( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ако( fixedType() )
            {
                цяло _type = CV_MAT_TYPE(flags);
                за( size_t j = len0; j < len; j++ )
                {
                    ако( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            върни;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ако( allowTransposed )
        {
            ако( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ако( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            иначе
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ако(fixedSize())
        {
            CV_Assert(m.dims == d);
            за(цяло j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        върни;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ако( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ако( fixedType() )
            {
                цяло _type = CV_MAT_TYPE(flags);
                за( size_t j = len0; j < len; j++ )
                {
                    ако( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            върни;
        }

        CV_Assert( i < (цяло)v.size() );
        UMat& m = v[i];

        ако( allowTransposed )
        {
            ако( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ако( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            иначе
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ако(fixedSize())
        {
            CV_Assert(m.dims == d);
            за(цяло j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        върни;
    }

    ако ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        върни;
    }

    ако( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, цяло mtype, цяло i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ако (shape.dims < 0) {
        release();
    } иначе {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(цяло mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    върни getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, цяло mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, цяло mtype, цяло i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цяло sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(цяло rows, цяло cols, цяло mtype, цяло i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цяло sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, цяло mtype, цяло i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ако (shape.dims < 0) {
        release();
        върни;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ако ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } иначе ако ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, цяло mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(цяло d, const цяло* sizes, цяло mtype, цяло i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цяло size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ако( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ако (k == MAT)
            m = (Mat*)obj;
        иначе {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ако (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            иначе
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            за(цяло j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        върни;
    }

    ако( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ако (k == UMAT)
            m = (UMat*)obj;
        иначе {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ако (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            върни;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            иначе
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            за(цяло j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        върни;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        ((Mat*)obj)->release();
        върни;
    }

    ако( k == UMAT )
    {
        ((UMat*)obj)->release();
        върни;
    }

    ако( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ако( k == NONE )
        върни;

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ако (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } иначе { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        break

        цяло esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        върни;
    }

    ако( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        върни;
    }

    ако( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        за( цяло i = 0; i < sz.height; i++ )
            v[i].release();
        върни;
    }

    ако( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        върни;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        върни;
    }
    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        върни;
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        върни;
    }

    release();
}

bool _OutputArray::needed() const
{
    върни kind() != NONE;
}

Mat& _OutputArray::getMatRef(цяло i) const
{
    _InputArray::KindFlag k = kind();
    ако( i < 0 )
    {
        CV_Assert( k == MAT );
        върни *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ако( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (цяло)v.size() );
        върни v[i];
    }
    иначе
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        върни v[i];
    }
}

UMat& _OutputArray::getUMatRef(цяло i) const
{
    _InputArray::KindFlag k = kind();
    ако( i < 0 )
    {
        CV_Assert( k == UMAT );
        върни *(UMat*)obj;
    }
    иначе
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (цяло)v.size() );
        върни v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    върни *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    върни *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    върни *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    върни *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    върни *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    върни *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        ;
    иначе ако (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    иначе ако( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    иначе ако( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<double, 4>(value.ptr<double>())), mask);
#иначе
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    иначе
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        ;
    иначе ако (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    иначе
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ако (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    иначе ако (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    иначе ако (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    иначе
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ако (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    иначе ако (k == MAT)
    {
        *(Mat*)obj = m;
    }
    иначе ако (k == MATX)
    {
        m.copyTo(getMat());
    }
    иначе
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ако (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        върни;
    }
    цяло k = kind();
    ако (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    иначе ако (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    иначе ако (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    иначе
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ако (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        върни;
    }
    цяло k = kind();
    ако (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    иначе ако (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    иначе ако (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    иначе
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ако (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        за (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ако (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    иначе ако (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        за (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ако (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    иначе
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ако (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        за (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ако (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    иначе ако (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        за (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ако (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    иначе
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { върни _none; }

} // cv::
