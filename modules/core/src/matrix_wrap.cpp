// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<පූර්ණ_සංඛ්‍යා, 5> Vec5i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 7> Vec7i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 9> Vec9i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 10> Vec10i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 11> Vec11i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 12> Vec12i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 13> Vec13i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 14> Vec14i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 15> Vec15i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 16> Vec16i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 32> Vec32i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 64> Vec64i;
typedef Vec<පූර්ණ_සංඛ්‍යා, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    pak_sa 1: func(uchar); \
    pak_sa 2: func(Vec2b); \
    pak_sa 3: func(Vec3b); \
    pak_sa 4: func(පූර්ණ_සංඛ්‍යා); \
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

Mat _InputArray::getMat_(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    නම්( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        නම්( i < 0 )
            ආපසු *m;
        ආපසු m->row(i);
    }

    නම්( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        නම්( i < 0 )
            ආපසු m->getMat(accessFlags);
        ආපසු m->getMat(accessFlags).row(i);
    }

    නම් (k == MATX)
    {
        CV_Assert( i < 0 );
        ආපසු Mat(sz, flags, obj);
    }

    නම්( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        පූර්ණ_සංඛ්‍යා t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        පූර්ණ_සංඛ්‍යා j, n = (පූර්ණ_සංඛ්‍යා)v.size();
        නම්( n == 0 )
            ආපසු Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        සඳහා( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ආපසු m;
    }

    නම්( k == NONE )
        ආපසු Mat();

    නම්( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        පූර්ණ_සංඛ්‍යා t = CV_MAT_TYPE(flags);
        පූර්ණ_සංඛ්‍යා esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            නම් (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } නැතහොත් { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            පූර්ණ_සංඛ්‍යා v_sz = පූර්ණ_සංඛ්‍යා(v->size()); \
            ආපසු v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (පූර්ණ_සංඛ්‍යා)v.size() );

        ආපසු v[i];
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ආපසු v[i];
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (පූර්ණ_සංඛ්‍යා)v.size() );

        ආපසු v[i].getMat(accessFlags);
    }

    නම්( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    නම්( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    නම්( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    නම්( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ආපසු cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    නම්( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        නම්( i < 0 )
            ආපසු *m;
        ආපසු m->row(i);
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (පූර්ණ_සංඛ්‍යා)v.size() );

        ආපසු v[i];
    }

    නම්( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        නම්( i < 0 )
            ආපසු m->getUMat(accessFlags);
        ආපසු m->row(i).getUMat(accessFlags);
    }

    ආපසු getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    නම්( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        පූර්ණ_සංඛ්‍යා n = (පූර්ණ_සංඛ්‍යා)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ආපසු;
    }

    නම් (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ආපසු;
    }

    නම්( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        පූර්ණ_සංඛ්‍යා t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ආපසු;
    }

    නම්( k == NONE )
    {
        mv.clear();
        ආපසු;
    }

    නම්( k == STD_VECTOR_VECTOR )
    {
        පූර්ණ_සංඛ්‍යා typ = CV_MAT_TYPE(flags);
        පූර්ණ_සංඛ්‍යා esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            සඳහා (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                පූර්ණ_සංඛ්‍යා ni = (පූර්ණ_සංඛ්‍යා)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ආපසු;
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ආපසු;
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ආපසු;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ආපසු;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    නම්( k == NONE )
    {
        umv.clear();
        ආපසු;
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ආපසු;
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ආපසු;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        සඳහා( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ආපසු;
    }

    නම්( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ආපසු;
    }
    නම්( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ආපසු;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    නම් (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ආපසු *d_mat;
    }

    නම් (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ආපසු cuda_mem->createGpuMatHeader();
    }

    නම් (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    නම් (k == NONE)
        ආපසු cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#නැතහොත්
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#නැතහොත්
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#නැතහොත්
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    නම් (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ආපසු *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#නැතහොත්
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ආපසු *gl_buf;
}

පූර්ණ_සංඛ්‍යා _InputArray::rows_(පූර්ණ_සංඛ්‍යා i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    නම් (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ආපසු (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ආපසු size_(i).height;
}

පූර්ණ_සංඛ්‍යා _InputArray::cols_(පූර්ණ_සංඛ්‍යා i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    නම් (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ආපසු (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ආපසු size_(i).width;
}

Size _InputArray::size_(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ආපසු Size(m->cols, m->rows);
    }

    නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ආපසු Size(m->cols, m->rows);
    }

    නම් (k == MATX)
    {
        CV_Assert( i < 0 );
        ආපසු sz;
    }

    නම්( k == NONE )
        ආපසු Size();

    නම්( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ආපසු Size((පූර්ණ_සංඛ්‍යා)n, 1);
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        නම්( i < 0 )
            ආපසු vv.empty() ? Size() : Size((පූර්ණ_සංඛ්‍යා)vv.size(), 1);
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );

        ආපසු vv[i].size();
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        නම්( i < 0 )
            ආපසු sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ආපසු vv[i].size();
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        නම් (i < 0)
            ආපසු vv.empty() ? Size() : Size((පූර්ණ_සංඛ්‍යා)vv.size(), 1);
        CV_Assert(i < (පූර්ණ_සංඛ්‍යා)vv.size());
        ආපසු vv[i].size();
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        නම් (i < 0)
            ආපසු vv.empty() ? Size() : Size((පූර්ණ_සංඛ්‍යා)vv.size(), 1);
        CV_Assert(i < (පූර්ණ_සංඛ්‍යා)vv.size());
        const cuda::GpuMatND& m = vv[i];
        නම් (m.dims == 0)
            ආපසු Size();
        නම් (m.dims == 1)
            ආපසු Size(1, m.size[0]);
        ආපසු Size(m.size[1], m.size[0]);
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        නම්( i < 0 )
            ආපසු vv.empty() ? Size() : Size((පූර්ණ_සංඛ්‍යා)vv.size(), 1);
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );

        ආපසු vv[i].size();
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        නම් (i < 0)
            ආපසු vv.empty() ? Size() : Size((පූර්ණ_සංඛ්‍යා)vv.size(), 1);
        CV_Assert(i < (පූර්ණ_සංඛ්‍යා)vv.size());
        const cuda::GpuMatND& m = vv[i];
        නම් (m.dims == 0)
            ආපසු Size();
        නම් (m.dims == 1)
            ආපසු Size(1, m.size[0]);
        ආපසු Size(m.size[1], m.size[0]);
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම්( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ආපසු buf->size();
    }

    නම්( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ආපසු d_mat->size();
    }

    නම්( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ආපසු cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

පූර්ණ_සංඛ්‍යා _InputArray::sizend(පූර්ණ_සංඛ්‍යා* arrsz, පූර්ණ_සංඛ්‍යා i) const
{
    පූර්ණ_සංඛ්‍යා j, d = 0;
    _InputArray::KindFlag k = kind();

    නම්( k == NONE )
        ;
    නැතහොත් නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        නම්(arrsz)
            සඳහා(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    නැතහොත් නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        නම්(arrsz)
            සඳහා(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    නැතහොත් නම්( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        නම්(arrsz)
            සඳහා(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    නැතහොත් නම්( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        නම්(arrsz)
            සඳහා(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    නැතහොත් නම්( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        නම්(arrsz)
            සඳහා(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    නැතහොත් නම් (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        නම්(arrsz)
        {
            arrsz[0] = පූර්ණ_සංඛ්‍යා(n);
        }
    }
    නැතහොත්
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        නම්(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ආපසු d;
}

bool _InputArray::empty(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();
    නම් (i >= 0) {
        නම් (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ආපසු mv->at(i).empty();
        }
        නැතහොත් නම් (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ආපසු umv->at(i).empty();
        }
        නැතහොත් නම් (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ආපසු n == 0;
        } නැතහොත් {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ආපසු empty();
}

MatShape _InputArray::shape(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    නම්( k == NONE )
        ;
    නැතහොත් නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    නැතහොත් නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    නැතහොත් නම්( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    නැතහොත් නම්( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    නැතහොත් නම්( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    නැතහොත්
    {
        පූර්ණ_සංඛ්‍යා sizes[CV_MAX_DIM];
        පූර්ණ_සංඛ්‍යා dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ආපසු shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    නම්( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        නම්( k2 == MAT )
            ආපසු m->size == ((const Mat*)arr.obj)->size;
        නම්( k2 == UMAT )
            ආපසු m->size == ((const UMat*)arr.obj)->size;
        නම්( m->dims > 2 )
            ආපසු false;
        sz1 = m->size();
    }
    නැතහොත් නම්( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        නම්( k2 == MAT )
            ආපසු m->size == ((const Mat*)arr.obj)->size;
        නම්( k2 == UMAT )
            ආපසු m->size == ((const UMat*)arr.obj)->size;
        නම්( m->dims > 2 )
            ආපසු false;
        sz1 = m->size();
    }
    නැතහොත් නම් ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ආපසු ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    නැතහොත්
        sz1 = size();

    නම්( arr.dims() > 2 )
        ආපසු false;
    ආපසු sz1 == arr.size();
}

පූර්ණ_සංඛ්‍යා _InputArray::dims_(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const Mat*)obj)->dims;
    }

    නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const UMat*)obj)->dims;
    }

    නම් (k == MATX)
    {
        CV_Assert( i < 0 );
        ආපසු 2;
    }

    නම්( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ආපසු 1;
    }

    නම්( k == NONE )
        ආපසු 0;

    නම්( k == STD_VECTOR_VECTOR )
    {
        නම්( i < 0 )
            ආපසු 1;
        CV_Assert( size_t(i) < total() );
        ආපසු 1;
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        නම්( i < 0 )
            ආපසු 1;
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );

        ආපසු vv[i].dims;
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        නම්( i < 0 )
            ආපසු 1;
        CV_Assert( i < sz.height );

        ආපසු vv[i].dims;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        නම්( i < 0 )
            ආපසු 1;
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );

        ආපසු vv[i].dims;
    }

    නම්( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ආපසු 2;
    }

    නම්( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ආපසු 2;
    }

    නම්( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ආපසු ((const cuda::GpuMatND*)obj)->dims;
    }

    නම්( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ආපසු 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const Mat*)obj)->total();
    }

    නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const UMat*)obj)->total();
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        නම්( i < 0 )
            ආපසු vv.size();

        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        ආපසු vv[i].total();
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        නම්( i < 0 )
            ආපසු sz.height;

        CV_Assert( i < sz.height );
        ආපසු vv[i].total();
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        නම්( i < 0 )
            ආපසු vv.size();

        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        ආපසු vv[i].total();
    }

    නම්( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        නම් (_gpuMatND.dims > 0)
        {
            res = 1;
            සඳහා(පූර්ණ_සංඛ්‍යා d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ආපසු res;
        }
    }

    නම් (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        පූර්ණ_සංඛ්‍යා esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            නම් (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                නම් (i < 0) \
                    ආපසු n; \
                CV_Assert((size_t)i < n); \
                ආපසු vv->at(i).size(); \
            } නැතහොත් \
                ආපසු ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ආපසු size(i).area();
}

පූර්ණ_සංඛ්‍යා _InputArray::type_(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
        ආපසු ((const Mat*)obj)->type();

    නම්( k == UMAT )
        ආපසු ((const UMat*)obj)->type();

    නම්( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ආපසු CV_MAT_TYPE(flags);

    නම්( k == NONE )
        ආපසු -1;

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        නම්( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ආපසු CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        ආපසු vv[i >= 0 ? i : 0].type();
    }

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        නම්( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ආපසු CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        ආපසු vv[i >= 0 ? i : 0].type();
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        නම්( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ආපසු CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ආපසු vv[i >= 0 ? i : 0].type();
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        නම් (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ආපසු CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (පූර්ණ_සංඛ්‍යා)vv.size());
        ආපසු vv[i >= 0 ? i : 0].type();
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        නම් (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ආපසු CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (පූර්ණ_සංඛ්‍යා)vv.size());
        ආපසු vv[i >= 0 ? i : 0].type();
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම්( k == OPENGL_BUFFER )
        ආපසු ((const ogl::Buffer*)obj)->type();

    නම්( k == CUDA_GPU_MAT )
        ආපසු ((const cuda::GpuMat*)obj)->type();

    නම්( k == CUDA_GPU_MATND )
        ආපසු ((const cuda::GpuMatND*)obj)->type();

    නම්( k == CUDA_HOST_MEM )
        ආපසු ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

පූර්ණ_සංඛ්‍යා _InputArray::depth_(පූර්ණ_සංඛ්‍යා i) const
{
    ආපසු CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
        ආපසු ((const Mat*)obj)->empty();

    නම්( k == UMAT )
        ආපසු ((const UMat*)obj)->empty();

    නම් (k == MATX)
        ආපසු false;

    නම්( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ආපසු n == 0;
    }

    නම්( k == NONE )
        ආපසු true;

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ආපසු vv.empty();
    }

    නම්( k == STD_ARRAY_MAT )
    {
        ආපසු sz.height == 0;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ආපසු vv.empty();
    }

    නම්( k == OPENGL_BUFFER )
        ආපසු ((const ogl::Buffer*)obj)->empty();

    නම්( k == CUDA_GPU_MAT )
        ආපසු ((const cuda::GpuMat*)obj)->empty();

    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ආපසු vv.empty();
    }

    නම්( k == CUDA_GPU_MATND )
        ආපසු ((const cuda::GpuMatND*)obj)->empty();

    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ආපසු vv.empty();
    }

    නම්( k == CUDA_HOST_MEM )
        ආපසු ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
        ආපසු i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    නම්( k == UMAT )
        ආපසු i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    නම්( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ආපසු true;

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].isContinuous();
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ආපසු vv[i].isContinuous();
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].isContinuous();
    }

    නම්( k == CUDA_GPU_MAT )
      ආපසු i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    නම්( k == CUDA_GPU_MATND )
        ආපසු i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
        ආපසු i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    නම්( k == UMAT )
        ආපසු i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    නම්( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ආපසු false;

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].isSubmatrix();
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ආපසු vv[i].isSubmatrix();
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].isSubmatrix();
    }

    නම්( k == CUDA_GPU_MATND )
    {
        ආපසු ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ආපසු (size_t)(m->ptr() - m->datastart);
    }

    නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const UMat*)obj)->offset;
    }

    නම්( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ආපසු 0;

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (පූර්ණ_සංඛ්‍යා)vv.size() );

        ආපසු (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ආපසු (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].offset;
    }

    නම්( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ආපසු (size_t)(m->data - m->datastart);
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const Mat*)obj)->step;
    }

    නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const UMat*)obj)->step;
    }

    නම්( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ආපසු 0;

    නම්( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (පූර්ණ_සංඛ්‍යා)vv.size() );
        ආපසු vv[i].step;
    }

    නම්( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ආපසු vv[i].step;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].step;
    }

    නම්( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ආපසු ((const cuda::GpuMat*)obj)->step;
    }
    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ආපසු vv[i].step;
    }
    නම්( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ආපසු _gpuMatND.step[i];
    }

    නම් (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ආපසු _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == NONE )
        arr.release();
    නැතහොත් නම්( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    නැතහොත් නම්( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    නැතහොත් නම් (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    නැතහොත්
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == NONE )
        arr.release();
    නැතහොත් නම්( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    නැතහොත් නම්( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    නැතහොත් නම් (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    නැතහොත්
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ආපසු (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ආපසු (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    නම්( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ආපසු;
    }
    නම්( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ආපසු;
    }
    නම්( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    නම්( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    නම්( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    නම්( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    පූර්ණ_සංඛ්‍යා sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(පූර්ණ_සංඛ්‍යා _rows, පූර්ණ_සංඛ්‍යා _cols, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    නම්( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ආපසු;
    }
    නම්( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ආපසු;
    }
    නම්( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    නම්( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    නම්( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    නම්( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    පූර්ණ_සංඛ්‍යා sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(පූර්ණ_සංඛ්‍යා d, const පූර්ණ_සංඛ්‍යා* sizes, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    පූර්ණ_සංඛ්‍යා size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    නම්( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        නම් (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            නැතහොත්
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        නම්(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ආපසු;
    }

    නම්( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        නම් (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            නැතහොත්
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        නම්(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ආපසු;
    }

    නම්( k == MATX )
    {
        CV_Assert( i < 0 );
        පූර්ණ_සංඛ්‍යා type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        නම් (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            පූර්ණ_සංඛ්‍යා total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        නැතහොත්
        {
            නම් (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            නැතහොත්
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ආපසු;
    }

    නම්( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        පූර්ණ_සංඛ්‍යා esz = CV_ELEM_SIZE(flags);

        නම්( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            පූර්ණ_සංඛ්‍යා type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            නම් (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                නම් (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ආපසු; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } නැතහොත් { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ආපසු;
    }

    නම්( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    නම්( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        නම්( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            නම්( fixedType() )
            {
                පූර්ණ_සංඛ්‍යා _type = CV_MAT_TYPE(flags);
                සඳහා( size_t j = len0; j < len; j++ )
                {
                    නම්( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ආපසු;
        }

        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)v.size() );
        Mat& m = v[i];

        නම්( allowTransposed )
        {
            නම්( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            නම්( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            නැතහොත්
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        නම්(fixedSize())
        {
            CV_Assert(m.dims == d);
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ආපසු;
    }

    නම්( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        නම්( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            නම්( fixedType() )
            {
                පූර්ණ_සංඛ්‍යා _type = CV_MAT_TYPE(flags);
                සඳහා( size_t j = len0; j < len; j++ )
                {
                    නම්( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ආපසු;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        නම්( allowTransposed )
        {
            නම්( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            නම්( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            නැතහොත්
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        නම්(fixedSize())
        {
            CV_Assert(m.dims == d);
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ආපසු;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        නම්( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            නම්( fixedType() )
            {
                පූර්ණ_සංඛ්‍යා _type = CV_MAT_TYPE(flags);
                සඳහා( size_t j = len0; j < len; j++ )
                {
                    නම්( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ආපසු;
        }

        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)v.size() );
        UMat& m = v[i];

        නම්( allowTransposed )
        {
            නම්( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            නම්( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            නැතහොත්
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        නම්(fixedSize())
        {
            CV_Assert(m.dims == d);
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ආපසු;
    }

    නම් ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ආපසු;
    }

    නම්( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    නම් (shape.dims < 0) {
        release();
    } නැතහොත් {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(පූර්ණ_සංඛ්‍යා mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ආපසු getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, පූර්ණ_සංඛ්‍යා mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    පූර්ණ_සංඛ්‍යා sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(පූර්ණ_සංඛ්‍යා rows, පූර්ණ_සංඛ්‍යා cols, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    පූර්ණ_සංඛ්‍යා sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    නම් (shape.dims < 0) {
        release();
        ආපසු;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    නම් ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } නැතහොත් නම් ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, පූර්ණ_සංඛ්‍යා mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(පූර්ණ_සංඛ්‍යා d, const පූර්ණ_සංඛ්‍යා* sizes, පූර්ණ_සංඛ්‍යා mtype, පූර්ණ_සංඛ්‍යා i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    පූර්ණ_සංඛ්‍යා size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    නම්( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        නම් (k == MAT)
            m = (Mat*)obj;
        නැතහොත් {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        නම් (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            නැතහොත්
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        නම්(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ආපසු;
    }

    නම්( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        නම් (k == UMAT)
            m = (UMat*)obj;
        නැතහොත් {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        නම් (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ආපසු;
        }

        නම්(fixedType())
        {
            නම්(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            නැතහොත්
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        නම්(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            සඳහා(පූර්ණ_සංඛ්‍යා j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ආපසු;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        ((Mat*)obj)->release();
        ආපසු;
    }

    නම්( k == UMAT )
    {
        ((UMat*)obj)->release();
        ආපසු;
    }

    නම්( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම්( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම්( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    නම්( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    නම්( k == NONE )
        ආපසු;

    නම්( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        නම් (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } නැතහොත් { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        පූර්ණ_සංඛ්‍යා esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ආපසු;
    }

    නම්( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ආපසු;
    }

    නම්( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        සඳහා( පූර්ණ_සංඛ්‍යා i = 0; i < sz.height; i++ )
            v[i].release();
        ආපසු;
    }

    නම්( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ආපසු;
    }

    නම්( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ආපසු;
    }
    නම් (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ආපසු;
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    නම්( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ආපසු;
    }

    release();
}

bool _OutputArray::needed() const
{
    ආපසු kind() != NONE;
}

Mat& _OutputArray::getMatRef(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();
    නම්( i < 0 )
    {
        CV_Assert( k == MAT );
        ආපසු *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    නම්( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)v.size() );
        ආපසු v[i];
    }
    නැතහොත්
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ආපසු v[i];
    }
}

UMat& _OutputArray::getUMatRef(පූර්ණ_සංඛ්‍යා i) const
{
    _InputArray::KindFlag k = kind();
    නම්( i < 0 )
    {
        CV_Assert( k == UMAT );
        ආපසු *(UMat*)obj;
    }
    නැතහොත්
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (පූර්ණ_සංඛ්‍යා)v.size() );
        ආපසු v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ආපසු *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ආපසු *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ආපසු *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ආපසු *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ආපසු *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ආපසු *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    නම්( k == NONE )
        ;
    නැතහොත් නම් (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    නැතහොත් නම්( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    නැතහොත් නම්( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#නැතහොත්
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    නැතහොත්
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    නම්( k == NONE )
        ;
    නැතහොත් නම් (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    නැතහොත්
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    නම් (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    නැතහොත් නම් (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    නැතහොත් නම් (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    නැතහොත්
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    නම් (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    නැතහොත් නම් (k == MAT)
    {
        *(Mat*)obj = m;
    }
    නැතහොත් නම් (k == MATX)
    {
        m.copyTo(getMat());
    }
    නැතහොත්
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    නම් (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ආපසු;
    }
    පූර්ණ_සංඛ්‍යා k = kind();
    නම් (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    නැතහොත් නම් (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    නැතහොත් නම් (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    නැතහොත්
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    නම් (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ආපසු;
    }
    පූර්ණ_සංඛ්‍යා k = kind();
    නම් (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    නැතහොත් නම් (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    නැතහොත් නම් (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    නැතහොත්
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    නම් (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        සඳහා (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            නම් (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    නැතහොත් නම් (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        සඳහා (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            නම් (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    නැතහොත්
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    නම් (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        සඳහා (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            නම් (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    නැතහොත් නම් (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        සඳහා (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            නම් (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    නැතහොත්
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ආපසු _none; }

} // cv::
