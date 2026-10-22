// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<цео, 5> Vec5i;
typedef Vec<цео, 7> Vec7i;
typedef Vec<цео, 9> Vec9i;
typedef Vec<цео, 10> Vec10i;
typedef Vec<цео, 11> Vec11i;
typedef Vec<цео, 12> Vec12i;
typedef Vec<цео, 13> Vec13i;
typedef Vec<цео, 14> Vec14i;
typedef Vec<цео, 15> Vec15i;
typedef Vec<цео, 16> Vec16i;
typedef Vec<цео, 32> Vec32i;
typedef Vec<цео, 64> Vec64i;
typedef Vec<цео, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    case 1: func(uchar); \
    case 2: func(Vec2b); \
    case 3: func(Vec3b); \
    case 4: func(цео); \
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

Mat _InputArray::getMat_(цео i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ако( i < 0 )
            врати *m;
        врати m->row(i);
    }

    ако( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ако( i < 0 )
            врати m->getMat(accessFlags);
        врати m->getMat(accessFlags).row(i);
    }

    ако (k == MATX)
    {
        CV_Assert( i < 0 );
        врати Mat(sz, flags, obj);
    }

    ако( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        цео t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        цео j, n = (цео)v.size();
        ако( n == 0 )
            врати Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        за( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        врати m;
    }

    ако( k == NONE )
        врати Mat();

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        цео t = CV_MAT_TYPE(flags);
        цео esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ако (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } инако { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            цео v_sz = цео(v->size()); \
            врати v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (цео)v.size() );

        врати v[i];
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        врати v[i];
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (цео)v.size() );

        врати v[i].getMat(accessFlags);
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

        врати cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(цео i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ако( i < 0 )
            врати *m;
        врати m->row(i);
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (цео)v.size() );

        врати v[i];
    }

    ако( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ако( i < 0 )
            врати m->getUMat(accessFlags);
        врати m->row(i).getUMat(accessFlags);
    }

    врати getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ако( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        цео n = (цео)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        за( цео i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        врати;
    }

    ако (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        врати;
    }

    ако( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        цео t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        врати;
    }

    ако( k == NONE )
    {
        mv.clear();
        врати;
    }

    ако( k == STD_VECTOR_VECTOR )
    {
        цео typ = CV_MAT_TYPE(flags);
        цео esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            за (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                цео ni = (цео)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        break

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        врати;
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        врати;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        врати;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        за( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        врати;
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
        врати;
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        за( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        врати;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        за( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        врати;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        за( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        врати;
    }

    ако( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        врати;
    }
    ако( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        врати;
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
        врати *d_mat;
    }

    ако (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        врати cuda_mem->createGpuMatHeader();
    }

    ако (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ако (k == NONE)
        врати cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#инако
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
#инако
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
#инако
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
        врати *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#инако
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    врати *gl_buf;
}

цео _InputArray::rows_(цео i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ако (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        врати (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    врати size_(i).height;
}

цео _InputArray::cols_(цео i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ако (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        врати (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    врати size_(i).width;
}

Size _InputArray::size_(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        врати Size(m->cols, m->rows);
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        врати Size(m->cols, m->rows);
    }

    ако (k == MATX)
    {
        CV_Assert( i < 0 );
        врати sz;
    }

    ако( k == NONE )
        врати Size();

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        врати Size((цео)n, 1);
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( i < 0 )
            врати vv.empty() ? Size() : Size((цео)vv.size(), 1);
        CV_Assert( i < (цео)vv.size() );

        врати vv[i].size();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( i < 0 )
            врати sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        врати vv[i].size();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ако (i < 0)
            врати vv.empty() ? Size() : Size((цео)vv.size(), 1);
        CV_Assert(i < (цео)vv.size());
        врати vv[i].size();
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ако (i < 0)
            врати vv.empty() ? Size() : Size((цео)vv.size(), 1);
        CV_Assert(i < (цео)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ако (m.dims == 0)
            врати Size();
        ако (m.dims == 1)
            врати Size(1, m.size[0]);
        врати Size(m.size[1], m.size[0]);
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( i < 0 )
            врати vv.empty() ? Size() : Size((цео)vv.size(), 1);
        CV_Assert( i < (цео)vv.size() );

        врати vv[i].size();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ако (i < 0)
            врати vv.empty() ? Size() : Size((цео)vv.size(), 1);
        CV_Assert(i < (цео)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ако (m.dims == 0)
            врати Size();
        ако (m.dims == 1)
            врати Size(1, m.size[0]);
        врати Size(m.size[1], m.size[0]);
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        врати buf->size();
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        врати d_mat->size();
    }

    ако( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        врати cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

цео _InputArray::sizend(цео* arrsz, цео i) const
{
    цео j, d = 0;
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        ;
    инако ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    инако ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    инако ако( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (цео)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    инако ако( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    инако ако( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (цео)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ако(arrsz)
            за(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    инако ако (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ако(arrsz)
        {
            arrsz[0] = цео(n);
        }
    }
    инако
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

    врати d;
}

bool _InputArray::empty(цео i) const
{
    _InputArray::KindFlag k = kind();
    ако (i >= 0) {
        ако (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            врати mv->at(i).empty();
        }
        инако ако (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            врати umv->at(i).empty();
        }
        инако ако (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            врати n == 0;
        } инако {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    врати empty();
}

MatShape _InputArray::shape(цео i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ако( k == NONE )
        ;
    инако ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    инако ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    инако ако( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    инако ако( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    инако ако( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    инако
    {
        цео sizes[CV_MAX_DIM];
        цео dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    врати shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ако( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ако( k2 == MAT )
            врати m->size == ((const Mat*)arr.obj)->size;
        ако( k2 == UMAT )
            врати m->size == ((const UMat*)arr.obj)->size;
        ако( m->dims > 2 )
            врати false;
        sz1 = m->size();
    }
    инако ако( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ако( k2 == MAT )
            врати m->size == ((const Mat*)arr.obj)->size;
        ако( k2 == UMAT )
            врати m->size == ((const UMat*)arr.obj)->size;
        ако( m->dims > 2 )
            врати false;
        sz1 = m->size();
    }
    инако ако ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        врати ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    инако
        sz1 = size();

    ако( arr.dims() > 2 )
        врати false;
    врати sz1 == arr.size();
}

цео _InputArray::dims_(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        врати ((const Mat*)obj)->dims;
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        врати ((const UMat*)obj)->dims;
    }

    ако (k == MATX)
    {
        CV_Assert( i < 0 );
        врати 2;
    }

    ако( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        врати 1;
    }

    ако( k == NONE )
        врати 0;

    ако( k == STD_VECTOR_VECTOR )
    {
        ако( i < 0 )
            врати 1;
        CV_Assert( size_t(i) < total() );
        врати 1;
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( i < 0 )
            врати 1;
        CV_Assert( i < (цео)vv.size() );

        врати vv[i].dims;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( i < 0 )
            врати 1;
        CV_Assert( i < sz.height );

        врати vv[i].dims;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( i < 0 )
            врати 1;
        CV_Assert( i < (цео)vv.size() );

        врати vv[i].dims;
    }

    ако( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        врати 2;
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        врати 2;
    }

    ако( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        врати ((const cuda::GpuMatND*)obj)->dims;
    }

    ако( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        врати 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        врати ((const Mat*)obj)->total();
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        врати ((const UMat*)obj)->total();
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( i < 0 )
            врати vv.size();

        CV_Assert( i < (цео)vv.size() );
        врати vv[i].total();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( i < 0 )
            врати sz.height;

        CV_Assert( i < sz.height );
        врати vv[i].total();
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( i < 0 )
            врати vv.size();

        CV_Assert( i < (цео)vv.size() );
        врати vv[i].total();
    }

    ако( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ако (_gpuMatND.dims > 0)
        {
            res = 1;
            за(цео d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            врати res;
        }
    }

    ако (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        цео esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ако (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ако (i < 0) \
                    врати n; \
                CV_Assert((size_t)i < n); \
                врати vv->at(i).size(); \
            } инако \
                врати ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    врати size(i).area();
}

цео _InputArray::type_(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        врати ((const Mat*)obj)->type();

    ако( k == UMAT )
        врати ((const UMat*)obj)->type();

    ако( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        врати CV_MAT_TYPE(flags);

    ако( k == NONE )
        врати -1;

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ако( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            врати CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (цео)vv.size() );
        врати vv[i >= 0 ? i : 0].type();
    }

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ако( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            врати CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (цео)vv.size() );
        врати vv[i >= 0 ? i : 0].type();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ако( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            врати CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        врати vv[i >= 0 ? i : 0].type();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ако (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            врати CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (цео)vv.size());
        врати vv[i >= 0 ? i : 0].type();
#инако
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
            врати CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (цео)vv.size());
        врати vv[i >= 0 ? i : 0].type();
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == OPENGL_BUFFER )
        врати ((const ogl::Buffer*)obj)->type();

    ако( k == CUDA_GPU_MAT )
        врати ((const cuda::GpuMat*)obj)->type();

    ако( k == CUDA_GPU_MATND )
        врати ((const cuda::GpuMatND*)obj)->type();

    ако( k == CUDA_HOST_MEM )
        врати ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

цео _InputArray::depth_(цео i) const
{
    врати CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        врати ((const Mat*)obj)->empty();

    ако( k == UMAT )
        врати ((const UMat*)obj)->empty();

    ако (k == MATX)
        врати false;

    ако( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        врати n == 0;
    }

    ако( k == NONE )
        врати true;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        врати vv.empty();
    }

    ако( k == STD_ARRAY_MAT )
    {
        врати sz.height == 0;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        врати vv.empty();
    }

    ако( k == OPENGL_BUFFER )
        врати ((const ogl::Buffer*)obj)->empty();

    ако( k == CUDA_GPU_MAT )
        врати ((const cuda::GpuMat*)obj)->empty();

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        врати vv.empty();
    }

    ако( k == CUDA_GPU_MATND )
        врати ((const cuda::GpuMatND*)obj)->empty();

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        врати vv.empty();
    }

    ако( k == CUDA_HOST_MEM )
        врати ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        врати i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ако( k == UMAT )
        врати i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        врати true;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].isContinuous();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        врати vv[i].isContinuous();
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].isContinuous();
    }

    ако( k == CUDA_GPU_MAT )
      врати i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ако( k == CUDA_GPU_MATND )
        врати i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
        врати i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ако( k == UMAT )
        врати i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        врати false;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].isSubmatrix();
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        врати vv[i].isSubmatrix();
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].isSubmatrix();
    }

    ако( k == CUDA_GPU_MATND )
    {
        врати ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        врати (size_t)(m->ptr() - m->datastart);
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        врати ((const UMat*)obj)->offset;
    }

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        врати 0;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (цео)vv.size() );

        врати (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        врати (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].offset;
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        врати (size_t)(m->data - m->datastart);
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(цео i) const
{
    _InputArray::KindFlag k = kind();

    ако( k == MAT )
    {
        CV_Assert( i < 0 );
        врати ((const Mat*)obj)->step;
    }

    ако( k == UMAT )
    {
        CV_Assert( i < 0 );
        врати ((const UMat*)obj)->step;
    }

    ако( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        врати 0;

    ако( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (цео)vv.size() );
        врати vv[i].step;
    }

    ако( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        врати vv[i].step;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].step;
    }

    ако( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        врати ((const cuda::GpuMat*)obj)->step;
    }
    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        врати vv[i].step;
    }
    ако( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        врати _gpuMatND.step[i];
    }

    ако (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        врати _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        arr.release();
    инако ако( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    инако ако( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    инако ако (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    инако
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        arr.release();
    инако ако( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    инако ако( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    инако ако (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    инако
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    врати (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    врати (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, цео mtype, цео i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ако( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        врати;
    }
    ако( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        врати;
    }
    ако( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        врати;
#инако
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
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ако( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ако( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    цео sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(цео _rows, цео _cols, цео mtype, цео i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ако( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        врати;
    }
    ако( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        врати;
    }
    ако( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        врати;
#инако
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
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ако( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ако( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    цео sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(цео d, const цео* sizes, цео mtype, цео i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цео size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
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
            врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            инако
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            за(цео j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        врати;
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
            врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            инако
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            за(цео j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        врати;
    }

    ако( k == MATX )
    {
        CV_Assert( i < 0 );
        цео type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ако (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            цео total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        инако
        {
            ако (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            инако
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        врати;
    }

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        цео esz = CV_ELEM_SIZE(flags);

        ако( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            цео type0 = CV_MAT_TYPE(flags);
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
                    врати; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } инако { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        break

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        врати;
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
                цео _type = CV_MAT_TYPE(flags);
                за( size_t j = len0; j < len; j++ )
                {
                    ако( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            врати;
        }

        CV_Assert( i < (цео)v.size() );
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
                врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            инако
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ако(fixedSize())
        {
            CV_Assert(m.dims == d);
            за(цео j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        врати;
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
                цео _type = CV_MAT_TYPE(flags);
                за( size_t j = len0; j < len; j++ )
                {
                    ако( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            врати;
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
                врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            инако
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ако(fixedSize())
        {
            CV_Assert(m.dims == d);
            за(цео j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        врати;
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
                цео _type = CV_MAT_TYPE(flags);
                за( size_t j = len0; j < len; j++ )
                {
                    ако( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            врати;
        }

        CV_Assert( i < (цео)v.size() );
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
                врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            инако
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ако(fixedSize())
        {
            CV_Assert(m.dims == d);
            за(цео j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        врати;
    }

    ако ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        врати;
    }

    ако( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, цео mtype, цео i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ако (shape.dims < 0) {
        release();
    } инако {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(цео mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    врати getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, цео mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, цео mtype, цео i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цео sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(цео rows, цео cols, цео mtype, цео i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цео sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, цео mtype, цео i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ако (shape.dims < 0) {
        release();
        врати;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ако ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } инако ако ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, цео mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(цео d, const цео* sizes, цео mtype, цео i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    цео size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ако( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ако (k == MAT)
            m = (Mat*)obj;
        инако {
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
            врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            инако
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            за(цео j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        врати;
    }

    ако( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ако (k == UMAT)
            m = (UMat*)obj;
        инако {
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
            врати;
        }

        ако(fixedType())
        {
            ако(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            инако
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ако(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            за(цео j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        врати;
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
        врати;
    }

    ако( k == UMAT )
    {
        ((UMat*)obj)->release();
        врати;
    }

    ако( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ако( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        врати;
#инако
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ако( k == NONE )
        врати;

    ако( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ако (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } инако { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        break

        цео esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        врати;
    }

    ако( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        врати;
    }

    ако( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        за( цео i = 0; i < sz.height; i++ )
            v[i].release();
        врати;
    }

    ако( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        врати;
    }

    ако( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        врати;
    }
    ако (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        врати;
#инако
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
        врати;
    }

    release();
}

bool _OutputArray::needed() const
{
    врати kind() != NONE;
}

Mat& _OutputArray::getMatRef(цео i) const
{
    _InputArray::KindFlag k = kind();
    ако( i < 0 )
    {
        CV_Assert( k == MAT );
        врати *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ако( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (цео)v.size() );
        врати v[i];
    }
    инако
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        врати v[i];
    }
}

UMat& _OutputArray::getUMatRef(цео i) const
{
    _InputArray::KindFlag k = kind();
    ако( i < 0 )
    {
        CV_Assert( k == UMAT );
        врати *(UMat*)obj;
    }
    инако
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (цео)v.size() );
        врати v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    врати *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    врати *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    врати *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    врати *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    врати *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    врати *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        ;
    инако ако (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    инако ако( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    инако ако( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<double, 4>(value.ptr<double>())), mask);
#инако
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    инако
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ако( k == NONE )
        ;
    инако ако (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    инако
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
    инако ако (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    инако ако (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    инако
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
    инако ако (k == MAT)
    {
        *(Mat*)obj = m;
    }
    инако ако (k == MATX)
    {
        m.copyTo(getMat());
    }
    инако
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
        врати;
    }
    цео k = kind();
    ако (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    инако ако (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    инако ако (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    инако
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
        врати;
    }
    цео k = kind();
    ако (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    инако ако (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    инако ако (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    инако
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
    инако ако (k == STD_VECTOR_MAT)
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
    инако
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
    инако ако (k == STD_VECTOR_MAT)
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
    инако
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { врати _none; }

} // cv::
