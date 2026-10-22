// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<ကိန်းပြည့်, 5> Vec5i;
typedef Vec<ကိန်းပြည့်, 7> Vec7i;
typedef Vec<ကိန်းပြည့်, 9> Vec9i;
typedef Vec<ကိန်းပြည့်, 10> Vec10i;
typedef Vec<ကိန်းပြည့်, 11> Vec11i;
typedef Vec<ကိန်းပြည့်, 12> Vec12i;
typedef Vec<ကိန်းပြည့်, 13> Vec13i;
typedef Vec<ကိန်းပြည့်, 14> Vec14i;
typedef Vec<ကိန်းပြည့်, 15> Vec15i;
typedef Vec<ကိန်းပြည့်, 16> Vec16i;
typedef Vec<ကိန်းပြည့်, 32> Vec32i;
typedef Vec<ကိန်းပြည့်, 64> Vec64i;
typedef Vec<ကိန်းပြည့်, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    pak_sa 1: func(uchar); \
    pak_sa 2: func(Vec2b); \
    pak_sa 3: func(Vec3b); \
    pak_sa 4: func(ကိန်းပြည့်); \
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

Mat _InputArray::getMat_(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    အကယ်၍( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး *m;
        ပြန်ပေး m->row(i);
    }

    အကယ်၍( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး m->getMat(accessFlags);
        ပြန်ပေး m->getMat(accessFlags).row(i);
    }

    အကယ်၍ (k == MATX)
    {
        CV_Assert( i < 0 );
        ပြန်ပေး Mat(sz, flags, obj);
    }

    အကယ်၍( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ကိန်းပြည့် t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        ကိန်းပြည့် j, n = (ကိန်းပြည့်)v.size();
        အကယ်၍( n == 0 )
            ပြန်ပေး Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        အတွက်( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ပြန်ပေး m;
    }

    အကယ်၍( k == NONE )
        ပြန်ပေး Mat();

    အကယ်၍( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        ကိန်းပြည့် t = CV_MAT_TYPE(flags);
        ကိန်းပြည့် esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            အကယ်၍ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } မဟုတ်ပါက { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ကိန်းပြည့် v_sz = ကိန်းပြည့်(v->size()); \
            ပြန်ပေး v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ကိန်းပြည့်)v.size() );

        ပြန်ပေး v[i];
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ပြန်ပေး v[i];
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ကိန်းပြည့်)v.size() );

        ပြန်ပေး v[i].getMat(accessFlags);
    }

    အကယ်၍( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    အကယ်၍( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    အကယ်၍( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    အကယ်၍( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ပြန်ပေး cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    အကယ်၍( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး *m;
        ပြန်ပေး m->row(i);
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ကိန်းပြည့်)v.size() );

        ပြန်ပေး v[i];
    }

    အကယ်၍( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး m->getUMat(accessFlags);
        ပြန်ပေး m->row(i).getUMat(accessFlags);
    }

    ပြန်ပေး getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    အကယ်၍( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        ကိန်းပြည့် n = (ကိန်းပြည့်)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        အတွက်( ကိန်းပြည့် i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ပြန်ပေး;
    }

    အကယ်၍ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        ကိန်းပြည့် t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ပြန်ပေး;
    }

    အကယ်၍( k == NONE )
    {
        mv.clear();
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_VECTOR )
    {
        ကိန်းပြည့် typ = CV_MAT_TYPE(flags);
        ကိန်းပြည့် esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            အတွက် (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                ကိန်းပြည့် ni = (ကိန်းပြည့်)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ပြန်ပေး;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    အကယ်၍( k == NONE )
    {
        umv.clear();
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        အတွက်( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ပြန်ပေး;
    }

    အကယ်၍( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ပြန်ပေး;
    }
    အကယ်၍( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ပြန်ပေး;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    အကယ်၍ (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ပြန်ပေး *d_mat;
    }

    အကယ်၍ (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ပြန်ပေး cuda_mem->createGpuMatHeader();
    }

    အကယ်၍ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    အကယ်၍ (k == NONE)
        ပြန်ပေး cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#မဟုတ်ပါက
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#မဟုတ်ပါက
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#မဟုတ်ပါက
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    အကယ်၍ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ပြန်ပေး *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#မဟုတ်ပါက
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ပြန်ပေး *gl_buf;
}

ကိန်းပြည့် _InputArray::rows_(ကိန်းပြည့် i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ပြန်ပေး (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ပြန်ပေး size_(i).height;
}

ကိန်းပြည့် _InputArray::cols_(ကိန်းပြည့် i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ပြန်ပေး (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ပြန်ပေး size_(i).width;
}

Size _InputArray::size_(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ပြန်ပေး Size(m->cols, m->rows);
    }

    အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ပြန်ပေး Size(m->cols, m->rows);
    }

    အကယ်၍ (k == MATX)
    {
        CV_Assert( i < 0 );
        ပြန်ပေး sz;
    }

    အကယ်၍( k == NONE )
        ပြန်ပေး Size();

    အကယ်၍( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ပြန်ပေး Size((ကိန်းပြည့်)n, 1);
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး vv.empty() ? Size() : Size((ကိန်းပြည့်)vv.size(), 1);
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );

        ပြန်ပေး vv[i].size();
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ပြန်ပေး vv[i].size();
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        အကယ်၍ (i < 0)
            ပြန်ပေး vv.empty() ? Size() : Size((ကိန်းပြည့်)vv.size(), 1);
        CV_Assert(i < (ကိန်းပြည့်)vv.size());
        ပြန်ပေး vv[i].size();
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        အကယ်၍ (i < 0)
            ပြန်ပေး vv.empty() ? Size() : Size((ကိန်းပြည့်)vv.size(), 1);
        CV_Assert(i < (ကိန်းပြည့်)vv.size());
        const cuda::GpuMatND& m = vv[i];
        အကယ်၍ (m.dims == 0)
            ပြန်ပေး Size();
        အကယ်၍ (m.dims == 1)
            ပြန်ပေး Size(1, m.size[0]);
        ပြန်ပေး Size(m.size[1], m.size[0]);
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး vv.empty() ? Size() : Size((ကိန်းပြည့်)vv.size(), 1);
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );

        ပြန်ပေး vv[i].size();
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        အကယ်၍ (i < 0)
            ပြန်ပေး vv.empty() ? Size() : Size((ကိန်းပြည့်)vv.size(), 1);
        CV_Assert(i < (ကိန်းပြည့်)vv.size());
        const cuda::GpuMatND& m = vv[i];
        အကယ်၍ (m.dims == 0)
            ပြန်ပေး Size();
        အကယ်၍ (m.dims == 1)
            ပြန်ပေး Size(1, m.size[0]);
        ပြန်ပေး Size(m.size[1], m.size[0]);
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ပြန်ပေး buf->size();
    }

    အကယ်၍( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ပြန်ပေး d_mat->size();
    }

    အကယ်၍( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ပြန်ပေး cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ကိန်းပြည့် _InputArray::sizend(ကိန်းပြည့်* arrsz, ကိန်းပြည့် i) const
{
    ကိန်းပြည့် j, d = 0;
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == NONE )
        ;
    မဟုတ်ပါက အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        အကယ်၍(arrsz)
            အတွက်(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    မဟုတ်ပါက အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        အကယ်၍(arrsz)
            အတွက်(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    မဟုတ်ပါက အကယ်၍( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        အကယ်၍(arrsz)
            အတွက်(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    မဟုတ်ပါက အကယ်၍( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        အကယ်၍(arrsz)
            အတွက်(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    မဟုတ်ပါက အကယ်၍( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        အကယ်၍(arrsz)
            အတွက်(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    မဟုတ်ပါက အကယ်၍ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        အကယ်၍(arrsz)
        {
            arrsz[0] = ကိန်းပြည့်(n);
        }
    }
    မဟုတ်ပါက
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        အကယ်၍(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ပြန်ပေး d;
}

bool _InputArray::empty(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍ (i >= 0) {
        အကယ်၍ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ပြန်ပေး mv->at(i).empty();
        }
        မဟုတ်ပါက အကယ်၍ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ပြန်ပေး umv->at(i).empty();
        }
        မဟုတ်ပါက အကယ်၍ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ပြန်ပေး n == 0;
        } မဟုတ်ပါက {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ပြန်ပေး empty();
}

MatShape _InputArray::shape(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    အကယ်၍( k == NONE )
        ;
    မဟုတ်ပါက အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    မဟုတ်ပါက အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    မဟုတ်ပါက အကယ်၍( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    မဟုတ်ပါက အကယ်၍( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    မဟုတ်ပါက အကယ်၍( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    မဟုတ်ပါက
    {
        ကိန်းပြည့် sizes[CV_MAX_DIM];
        ကိန်းပြည့် dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ပြန်ပေး shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    အကယ်၍( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        အကယ်၍( k2 == MAT )
            ပြန်ပေး m->size == ((const Mat*)arr.obj)->size;
        အကယ်၍( k2 == UMAT )
            ပြန်ပေး m->size == ((const UMat*)arr.obj)->size;
        အကယ်၍( m->dims > 2 )
            ပြန်ပေး false;
        sz1 = m->size();
    }
    မဟုတ်ပါက အကယ်၍( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        အကယ်၍( k2 == MAT )
            ပြန်ပေး m->size == ((const Mat*)arr.obj)->size;
        အကယ်၍( k2 == UMAT )
            ပြန်ပေး m->size == ((const UMat*)arr.obj)->size;
        အကယ်၍( m->dims > 2 )
            ပြန်ပေး false;
        sz1 = m->size();
    }
    မဟုတ်ပါက အကယ်၍ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ပြန်ပေး ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    မဟုတ်ပါက
        sz1 = size();

    အကယ်၍( arr.dims() > 2 )
        ပြန်ပေး false;
    ပြန်ပေး sz1 == arr.size();
}

ကိန်းပြည့် _InputArray::dims_(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const Mat*)obj)->dims;
    }

    အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const UMat*)obj)->dims;
    }

    အကယ်၍ (k == MATX)
    {
        CV_Assert( i < 0 );
        ပြန်ပေး 2;
    }

    အကယ်၍( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး 1;
    }

    အကယ်၍( k == NONE )
        ပြန်ပေး 0;

    အကယ်၍( k == STD_VECTOR_VECTOR )
    {
        အကယ်၍( i < 0 )
            ပြန်ပေး 1;
        CV_Assert( size_t(i) < total() );
        ပြန်ပေး 1;
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး 1;
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );

        ပြန်ပေး vv[i].dims;
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး 1;
        CV_Assert( i < sz.height );

        ပြန်ပေး vv[i].dims;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး 1;
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );

        ပြန်ပေး vv[i].dims;
    }

    အကယ်၍( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး 2;
    }

    အကယ်၍( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး 2;
    }

    အကယ်၍( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const cuda::GpuMatND*)obj)->dims;
    }

    အကယ်၍( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const Mat*)obj)->total();
    }

    အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const UMat*)obj)->total();
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး vv.size();

        CV_Assert( i < (ကိန်းပြည့်)vv.size() );
        ပြန်ပေး vv[i].total();
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး sz.height;

        CV_Assert( i < sz.height );
        ပြန်ပေး vv[i].total();
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        အကယ်၍( i < 0 )
            ပြန်ပေး vv.size();

        CV_Assert( i < (ကိန်းပြည့်)vv.size() );
        ပြန်ပေး vv[i].total();
    }

    အကယ်၍( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        အကယ်၍ (_gpuMatND.dims > 0)
        {
            res = 1;
            အတွက်(ကိန်းပြည့် d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ပြန်ပေး res;
        }
    }

    အကယ်၍ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        ကိန်းပြည့် esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            အကယ်၍ (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                အကယ်၍ (i < 0) \
                    ပြန်ပေး n; \
                CV_Assert((size_t)i < n); \
                ပြန်ပေး vv->at(i).size(); \
            } မဟုတ်ပါက \
                ပြန်ပေး ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ပြန်ပေး size(i).area();
}

ကိန်းပြည့် _InputArray::type_(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
        ပြန်ပေး ((const Mat*)obj)->type();

    အကယ်၍( k == UMAT )
        ပြန်ပေး ((const UMat*)obj)->type();

    အကယ်၍( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ပြန်ပေး CV_MAT_TYPE(flags);

    အကယ်၍( k == NONE )
        ပြန်ပေး -1;

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        အကယ်၍( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ပြန်ပေး CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );
        ပြန်ပေး vv[i >= 0 ? i : 0].type();
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        အကယ်၍( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ပြန်ပေး CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ကိန်းပြည့်)vv.size() );
        ပြန်ပေး vv[i >= 0 ? i : 0].type();
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        အကယ်၍( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ပြန်ပေး CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ပြန်ပေး vv[i >= 0 ? i : 0].type();
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        အကယ်၍ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ပြန်ပေး CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ကိန်းပြည့်)vv.size());
        ပြန်ပေး vv[i >= 0 ? i : 0].type();
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        အကယ်၍ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ပြန်ပေး CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ကိန်းပြည့်)vv.size());
        ပြန်ပေး vv[i >= 0 ? i : 0].type();
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍( k == OPENGL_BUFFER )
        ပြန်ပေး ((const ogl::Buffer*)obj)->type();

    အကယ်၍( k == CUDA_GPU_MAT )
        ပြန်ပေး ((const cuda::GpuMat*)obj)->type();

    အကယ်၍( k == CUDA_GPU_MATND )
        ပြန်ပေး ((const cuda::GpuMatND*)obj)->type();

    အကယ်၍( k == CUDA_HOST_MEM )
        ပြန်ပေး ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ကိန်းပြည့် _InputArray::depth_(ကိန်းပြည့် i) const
{
    ပြန်ပေး CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
        ပြန်ပေး ((const Mat*)obj)->empty();

    အကယ်၍( k == UMAT )
        ပြန်ပေး ((const UMat*)obj)->empty();

    အကယ်၍ (k == MATX)
        ပြန်ပေး false;

    အကယ်၍( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ပြန်ပေး n == 0;
    }

    အကယ်၍( k == NONE )
        ပြန်ပေး true;

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ပြန်ပေး vv.empty();
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        ပြန်ပေး sz.height == 0;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ပြန်ပေး vv.empty();
    }

    အကယ်၍( k == OPENGL_BUFFER )
        ပြန်ပေး ((const ogl::Buffer*)obj)->empty();

    အကယ်၍( k == CUDA_GPU_MAT )
        ပြန်ပေး ((const cuda::GpuMat*)obj)->empty();

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ပြန်ပေး vv.empty();
    }

    အကယ်၍( k == CUDA_GPU_MATND )
        ပြန်ပေး ((const cuda::GpuMatND*)obj)->empty();

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ပြန်ပေး vv.empty();
    }

    အကယ်၍( k == CUDA_HOST_MEM )
        ပြန်ပေး ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
        ပြန်ပေး i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    အကယ်၍( k == UMAT )
        ပြန်ပေး i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    အကယ်၍( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ပြန်ပေး true;

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].isContinuous();
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ပြန်ပေး vv[i].isContinuous();
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].isContinuous();
    }

    အကယ်၍( k == CUDA_GPU_MAT )
      ပြန်ပေး i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    အကယ်၍( k == CUDA_GPU_MATND )
        ပြန်ပေး i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
        ပြန်ပေး i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    အကယ်၍( k == UMAT )
        ပြန်ပေး i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    အကယ်၍( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ပြန်ပေး false;

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].isSubmatrix();
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ပြန်ပေး vv[i].isSubmatrix();
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].isSubmatrix();
    }

    အကယ်၍( k == CUDA_GPU_MATND )
    {
        ပြန်ပေး ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ပြန်ပေး (size_t)(m->ptr() - m->datastart);
    }

    အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const UMat*)obj)->offset;
    }

    အကယ်၍( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ပြန်ပေး 0;

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ကိန်းပြည့်)vv.size() );

        ပြန်ပေး (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ပြန်ပေး (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].offset;
    }

    အကယ်၍( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ပြန်ပေး (size_t)(m->data - m->datastart);
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const Mat*)obj)->step;
    }

    အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const UMat*)obj)->step;
    }

    အကယ်၍( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ပြန်ပေး 0;

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ကိန်းပြည့်)vv.size() );
        ပြန်ပေး vv[i].step;
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ပြန်ပေး vv[i].step;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].step;
    }

    အကယ်၍( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ပြန်ပေး ((const cuda::GpuMat*)obj)->step;
    }
    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ပြန်ပေး vv[i].step;
    }
    အကယ်၍( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ပြန်ပေး _gpuMatND.step[i];
    }

    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ပြန်ပေး _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == NONE )
        arr.release();
    မဟုတ်ပါက အကယ်၍( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    မဟုတ်ပါက အကယ်၍( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    မဟုတ်ပါက အကယ်၍ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == NONE )
        arr.release();
    မဟုတ်ပါက အကယ်၍( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    မဟုတ်ပါက အကယ်၍( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    မဟုတ်ပါက အကယ်၍ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ပြန်ပေး (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ပြန်ပေး (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, ကိန်းပြည့် mtype, ကိန်းပြည့် i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ပြန်ပေး;
    }
    အကယ်၍( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ပြန်ပေး;
    }
    အကယ်၍( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    အကယ်၍( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    အကယ်၍( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    အကယ်၍( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ကိန်းပြည့် sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(ကိန်းပြည့် _rows, ကိန်းပြည့် _cols, ကိန်းပြည့် mtype, ကိန်းပြည့် i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ပြန်ပေး;
    }
    အကယ်၍( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ပြန်ပေး;
    }
    အကယ်၍( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    အကယ်၍( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    အကယ်၍( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    အကယ်၍( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ကိန်းပြည့် sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(ကိန်းပြည့် d, const ကိန်းပြည့်* sizes, ကိန်းပြည့် mtype, ကိန်းပြည့် i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ကိန်းပြည့် size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    အကယ်၍( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        အကယ်၍ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            မဟုတ်ပါက
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        အကယ်၍(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ပြန်ပေး;
    }

    အကယ်၍( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        အကယ်၍ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            မဟုတ်ပါက
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        အကယ်၍(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ပြန်ပေး;
    }

    အကယ်၍( k == MATX )
    {
        CV_Assert( i < 0 );
        ကိန်းပြည့် type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        အကယ်၍ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ကိန်းပြည့် total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        မဟုတ်ပါက
        {
            အကယ်၍ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            မဟုတ်ပါက
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ကိန်းပြည့် esz = CV_ELEM_SIZE(flags);

        အကယ်၍( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            ကိန်းပြည့် type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            အကယ်၍ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                အကယ်၍ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ပြန်ပေး; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } မဟုတ်ပါက { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ပြန်ပေး;
    }

    အကယ်၍( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        အကယ်၍( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            အကယ်၍( fixedType() )
            {
                ကိန်းပြည့် _type = CV_MAT_TYPE(flags);
                အတွက်( size_t j = len0; j < len; j++ )
                {
                    အကယ်၍( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ပြန်ပေး;
        }

        CV_Assert( i < (ကိန်းပြည့်)v.size() );
        Mat& m = v[i];

        အကယ်၍( allowTransposed )
        {
            အကယ်၍( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            အကယ်၍( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            မဟုတ်ပါက
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        အကယ်၍(fixedSize())
        {
            CV_Assert(m.dims == d);
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        အကယ်၍( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            အကယ်၍( fixedType() )
            {
                ကိန်းပြည့် _type = CV_MAT_TYPE(flags);
                အတွက်( size_t j = len0; j < len; j++ )
                {
                    အကယ်၍( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ပြန်ပေး;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        အကယ်၍( allowTransposed )
        {
            အကယ်၍( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            အကယ်၍( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            မဟုတ်ပါက
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        အကယ်၍(fixedSize())
        {
            CV_Assert(m.dims == d);
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        အကယ်၍( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            အကယ်၍( fixedType() )
            {
                ကိန်းပြည့် _type = CV_MAT_TYPE(flags);
                အတွက်( size_t j = len0; j < len; j++ )
                {
                    အကယ်၍( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ပြန်ပေး;
        }

        CV_Assert( i < (ကိန်းပြည့်)v.size() );
        UMat& m = v[i];

        အကယ်၍( allowTransposed )
        {
            အကယ်၍( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            အကယ်၍( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            မဟုတ်ပါက
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        အကယ်၍(fixedSize())
        {
            CV_Assert(m.dims == d);
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ပြန်ပေး;
    }

    အကယ်၍ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ပြန်ပေး;
    }

    အကယ်၍( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, ကိန်းပြည့် mtype, ကိန်းပြည့် i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    အကယ်၍ (shape.dims < 0) {
        release();
    } မဟုတ်ပါက {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(ကိန်းပြည့် mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ပြန်ပေး getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, ကိန်းပြည့် mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, ကိန်းပြည့် mtype, ကိန်းပြည့် i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ကိန်းပြည့် sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(ကိန်းပြည့် rows, ကိန်းပြည့် cols, ကိန်းပြည့် mtype, ကိန်းပြည့် i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ကိန်းပြည့် sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, ကိန်းပြည့် mtype, ကိန်းပြည့် i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    အကယ်၍ (shape.dims < 0) {
        release();
        ပြန်ပေး;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    အကယ်၍ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } မဟုတ်ပါက အကယ်၍ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, ကိန်းပြည့် mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(ကိန်းပြည့် d, const ကိန်းပြည့်* sizes, ကိန်းပြည့် mtype, ကိန်းပြည့် i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ကိန်းပြည့် size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    အကယ်၍( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        အကယ်၍ (k == MAT)
            m = (Mat*)obj;
        မဟုတ်ပါက {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        အကယ်၍ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            မဟုတ်ပါက
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        အကယ်၍(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ပြန်ပေး;
    }

    အကယ်၍( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        အကယ်၍ (k == UMAT)
            m = (UMat*)obj;
        မဟုတ်ပါက {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        အကယ်၍ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ပြန်ပေး;
        }

        အကယ်၍(fixedType())
        {
            အကယ်၍(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            မဟုတ်ပါက
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        အကယ်၍(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            အတွက်(ကိန်းပြည့် j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ပြန်ပေး;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        ((Mat*)obj)->release();
        ပြန်ပေး;
    }

    အကယ်၍( k == UMAT )
    {
        ((UMat*)obj)->release();
        ပြန်ပေး;
    }

    အကယ်၍( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    အကယ်၍( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    အကယ်၍( k == NONE )
        ပြန်ပေး;

    အကယ်၍( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        အကယ်၍ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } မဟုတ်ပါက { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        ကိန်းပြည့် esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        အတွက်( ကိန်းပြည့် i = 0; i < sz.height; i++ )
            v[i].release();
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ပြန်ပေး;
    }

    အကယ်၍( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ပြန်ပေး;
    }
    အကယ်၍ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ပြန်ပေး;
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ပြန်ပေး;
    }

    release();
}

bool _OutputArray::needed() const
{
    ပြန်ပေး kind() != NONE;
}

Mat& _OutputArray::getMatRef(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍( i < 0 )
    {
        CV_Assert( k == MAT );
        ပြန်ပေး *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    အကယ်၍( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ကိန်းပြည့်)v.size() );
        ပြန်ပေး v[i];
    }
    မဟုတ်ပါက
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ပြန်ပေး v[i];
    }
}

UMat& _OutputArray::getUMatRef(ကိန်းပြည့် i) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍( i < 0 )
    {
        CV_Assert( k == UMAT );
        ပြန်ပေး *(UMat*)obj;
    }
    မဟုတ်ပါက
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ကိန်းပြည့်)v.size() );
        ပြန်ပေး v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ပြန်ပေး *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ပြန်ပေး *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ပြန်ပေး *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ပြန်ပေး *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ပြန်ပေး *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ပြန်ပေး *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == NONE )
        ;
    မဟုတ်ပါက အကယ်၍ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    မဟုတ်ပါက အကယ်၍( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    မဟုတ်ပါက အကယ်၍( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    မဟုတ်ပါက
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    အကယ်၍( k == NONE )
        ;
    မဟုတ်ပါက အကယ်၍ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    မဟုတ်ပါက
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    မဟုတ်ပါက အကယ်၍ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    မဟုတ်ပါက အကယ်၍ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    မဟုတ်ပါက
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    မဟုတ်ပါက အကယ်၍ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    မဟုတ်ပါက အကယ်၍ (k == MATX)
    {
        m.copyTo(getMat());
    }
    မဟုတ်ပါက
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    အကယ်၍ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ပြန်ပေး;
    }
    ကိန်းပြည့် k = kind();
    အကယ်၍ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    မဟုတ်ပါက အကယ်၍ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    မဟုတ်ပါက အကယ်၍ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    မဟုတ်ပါက
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    အကယ်၍ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ပြန်ပေး;
    }
    ကိန်းပြည့် k = kind();
    အကယ်၍ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    မဟုတ်ပါက အကယ်၍ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    မဟုတ်ပါက အကယ်၍ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    မဟုတ်ပါက
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        အတွက် (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            အကယ်၍ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    မဟုတ်ပါက အကယ်၍ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        အတွက် (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            အကယ်၍ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    မဟုတ်ပါက
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    အကယ်၍ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        အတွက် (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            အကယ်၍ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    မဟုတ်ပါက အကယ်၍ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        အတွက် (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            အကယ်၍ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    မဟုတ်ပါက
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ပြန်ပေး _none; }

} // cv::
