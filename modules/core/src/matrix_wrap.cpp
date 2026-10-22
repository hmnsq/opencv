// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {

typedef Vec<ຈຳນວນເຕັມ, 5> Vec5i;
typedef Vec<ຈຳນວນເຕັມ, 7> Vec7i;
typedef Vec<ຈຳນວນເຕັມ, 9> Vec9i;
typedef Vec<ຈຳນວນເຕັມ, 10> Vec10i;
typedef Vec<ຈຳນວນເຕັມ, 11> Vec11i;
typedef Vec<ຈຳນວນເຕັມ, 12> Vec12i;
typedef Vec<ຈຳນວນເຕັມ, 13> Vec13i;
typedef Vec<ຈຳນວນເຕັມ, 14> Vec14i;
typedef Vec<ຈຳນວນເຕັມ, 15> Vec15i;
typedef Vec<ຈຳນວນເຕັມ, 16> Vec16i;
typedef Vec<ຈຳນວນເຕັມ, 32> Vec32i;
typedef Vec<ຈຳນວນເຕັມ, 64> Vec64i;
typedef Vec<ຈຳນວນເຕັມ, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    switch( esz ) \
    { \
    pak_sa 1: func(uchar); \
    pak_sa 2: func(Vec2b); \
    pak_sa 3: func(Vec3b); \
    pak_sa 4: func(ຈຳນວນເຕັມ); \
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

Mat _InputArray::getMat_(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ຖ້າ( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ *m;
        ສົ່ງຄືນ m->row(i);
    }

    ຖ້າ( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ m->getMat(accessFlags);
        ສົ່ງຄືນ m->getMat(accessFlags).row(i);
    }

    ຖ້າ (k == MATX)
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ Mat(sz, flags, obj);
    }

    ຖ້າ( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ຈຳນວນເຕັມ t = CV_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        ຈຳນວນເຕັມ j, n = (ຈຳນວນເຕັມ)v.size();
        ຖ້າ( n == 0 )
            ສົ່ງຄືນ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        ສຳລັບ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ສົ່ງຄືນ m;
    }

    ຖ້າ( k == NONE )
        ສົ່ງຄືນ Mat();

    ຖ້າ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        ຈຳນວນເຕັມ t = CV_MAT_TYPE(flags);
        ຈຳນວນເຕັມ esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ຖ້າ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ບໍ່ດັ່ງນັ້ນ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ຈຳນວນເຕັມ v_sz = ຈຳນວນເຕັມ(v->size()); \
            ສົ່ງຄືນ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ຈຳນວນເຕັມ)v.size() );

        ສົ່ງຄືນ v[i];
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ສົ່ງຄືນ v[i];
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ຈຳນວນເຕັມ)v.size() );

        ສົ່ງຄືນ v[i].getMat(accessFlags);
    }

    ຖ້າ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ຖ້າ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ຖ້າ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ຖ້າ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        ສົ່ງຄືນ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ຖ້າ( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ *m;
        ສົ່ງຄືນ m->row(i);
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ຈຳນວນເຕັມ)v.size() );

        ສົ່ງຄືນ v[i];
    }

    ຖ້າ( k == MAT )
    {
        const Mat* m = (const Mat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ m->getUMat(accessFlags);
        ສົ່ງຄືນ m->row(i).getUMat(accessFlags);
    }

    ສົ່ງຄືນ getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<Mat>& mv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ຖ້າ( k == MAT )
    {
        const Mat& m = *(const Mat*)obj;
        ຈຳນວນເຕັມ n = (ຈຳນວນເຕັມ)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (void*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (void*)m.ptr(i), &m.step[1]);
        ສົ່ງຄືນ;
    }

    ຖ້າ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        ຈຳນວນເຕັມ t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (void*)(v.data() + esz*i));
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == NONE )
    {
        mv.clear();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_VECTOR )
    {
        ຈຳນວນເຕັມ typ = CV_MAT_TYPE(flags);
        ຈຳນວນເຕັມ esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            ສຳລັບ (size_t i = 0; i < n; i++) { \
                const std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                ຈຳນວນເຕັມ ni = (ຈຳນວນເຕັມ)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (void*)vi.data()) : Mat(); \
            } \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ສົ່ງຄືນ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ຖ້າ( k == NONE )
    {
        umv.clear();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& v = *(const std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* v = (const Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ສຳລັບ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ສົ່ງຄືນ;
    }
    ຖ້າ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ສົ່ງຄືນ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ຖ້າ (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ສົ່ງຄືນ *d_mat;
    }

    ຖ້າ (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ສົ່ງຄືນ cuda_mem->createGpuMatHeader();
    }

    ຖ້າ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ຖ້າ (k == NONE)
        ສົ່ງຄືນ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ບໍ່ດັ່ງນັ້ນ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
void _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ບໍ່ດັ່ງນັ້ນ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ຖ້າ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND* d_mat = (const cuda::GpuMatND*)obj;
        ສົ່ງຄືນ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ບໍ່ດັ່ງນັ້ນ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    ສົ່ງຄືນ *gl_buf;
}

ຈຳນວນເຕັມ _InputArray::rows_(ຈຳນວນເຕັມ i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ສົ່ງຄືນ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ສົ່ງຄືນ size_(i).height;
}

ຈຳນວນເຕັມ _InputArray::cols_(ຈຳນວນເຕັມ i) const
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == CUDA_GPU_MATND)
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        ສົ່ງຄືນ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ສົ່ງຄືນ size_(i).width;
}

Size _InputArray::size_(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat* m = (const Mat*)obj;
        CV_Assert(m->dims <= 2);
        ສົ່ງຄືນ Size(m->cols, m->rows);
    }

    ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat* m = (const UMat*)obj;
        CV_Assert(m->dims <= 2);
        ສົ່ງຄືນ Size(m->cols, m->rows);
    }

    ຖ້າ (k == MATX)
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ sz;
    }

    ຖ້າ( k == NONE )
        ສົ່ງຄືນ Size();

    ຖ້າ( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ສົ່ງຄືນ Size((ຈຳນວນເຕັມ)n, 1);
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ vv.empty() ? Size() : Size((ຈຳນວນເຕັມ)vv.size(), 1);
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );

        ສົ່ງຄືນ vv[i].size();
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ສົ່ງຄືນ vv[i].size();
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ຖ້າ (i < 0)
            ສົ່ງຄືນ vv.empty() ? Size() : Size((ຈຳນວນເຕັມ)vv.size(), 1);
        CV_Assert(i < (ຈຳນວນເຕັມ)vv.size());
        ສົ່ງຄືນ vv[i].size();
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ຖ້າ (i < 0)
            ສົ່ງຄືນ vv.empty() ? Size() : Size((ຈຳນວນເຕັມ)vv.size(), 1);
        CV_Assert(i < (ຈຳນວນເຕັມ)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ຖ້າ (m.dims == 0)
            ສົ່ງຄືນ Size();
        ຖ້າ (m.dims == 1)
            ສົ່ງຄືນ Size(1, m.size[0]);
        ສົ່ງຄືນ Size(m.size[1], m.size[0]);
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ vv.empty() ? Size() : Size((ຈຳນວນເຕັມ)vv.size(), 1);
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );

        ສົ່ງຄືນ vv[i].size();
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ຖ້າ (i < 0)
            ສົ່ງຄືນ vv.empty() ? Size() : Size((ຈຳນວນເຕັມ)vv.size(), 1);
        CV_Assert(i < (ຈຳນວນເຕັມ)vv.size());
        const cuda::GpuMatND& m = vv[i];
        ຖ້າ (m.dims == 0)
            ສົ່ງຄືນ Size();
        ຖ້າ (m.dims == 1)
            ສົ່ງຄືນ Size(1, m.size[0]);
        ສົ່ງຄືນ Size(m.size[1], m.size[0]);
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        ສົ່ງຄືນ buf->size();
    }

    ຖ້າ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        ສົ່ງຄືນ d_mat->size();
    }

    ຖ້າ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        ສົ່ງຄືນ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ຈຳນວນເຕັມ _InputArray::sizend(ຈຳນວນເຕັມ* arrsz, ຈຳນວນເຕັມ i) const
{
    ຈຳນວນເຕັມ j, d = 0;
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == NONE )
        ;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        d = m.dims;
        ຖ້າ(arrsz)
            ສຳລັບ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        ຖ້າ(arrsz)
            ສຳລັບ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );
        const Mat& m = vv[i];
        d = m.dims;
        ຖ້າ(arrsz)
            ສຳລັບ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        d = m.dims;
        ຖ້າ(arrsz)
            ສຳລັບ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        ຖ້າ(arrsz)
            ສຳລັບ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ຖ້າ(arrsz)
        {
            arrsz[0] = ຈຳນວນເຕັມ(n);
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ຖ້າ(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ສົ່ງຄືນ d;
}

bool _InputArray::empty(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ (i >= 0) {
        ຖ້າ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<const std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ສົ່ງຄືນ mv->at(i).empty();
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<const std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ສົ່ງຄືນ umv->at(i).empty();
        }
        ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ສົ່ງຄືນ n == 0;
        } ບໍ່ດັ່ງນັ້ນ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ສົ່ງຄືນ empty();
}

MatShape _InputArray::shape(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ຖ້າ( k == NONE )
        ;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat& m = *(const Mat*)obj;
        shape = m.size;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        shape = m.size;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == STD_ARRAY_MAT && i >= 0 )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i < sz.height );
        const Mat& m = vv[i];
        shape = m.size;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        const UMat& m = vv[i];
        shape = m.size;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        ຈຳນວນເຕັມ sizes[CV_MAX_DIM];
        ຈຳນວນເຕັມ dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ສົ່ງຄືນ shape;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ຖ້າ( k1 == MAT )
    {
        const Mat* m = ((const Mat*)obj);
        ຖ້າ( k2 == MAT )
            ສົ່ງຄືນ m->size == ((const Mat*)arr.obj)->size;
        ຖ້າ( k2 == UMAT )
            ສົ່ງຄືນ m->size == ((const UMat*)arr.obj)->size;
        ຖ້າ( m->dims > 2 )
            ສົ່ງຄືນ false;
        sz1 = m->size();
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        ຖ້າ( k2 == MAT )
            ສົ່ງຄືນ m->size == ((const Mat*)arr.obj)->size;
        ຖ້າ( k2 == UMAT )
            ສົ່ງຄືນ m->size == ((const UMat*)arr.obj)->size;
        ຖ້າ( m->dims > 2 )
            ສົ່ງຄືນ false;
        sz1 = m->size();
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ສົ່ງຄືນ ((const cuda::GpuMatND*)obj)->size == ((const cuda::GpuMatND*)arr.obj)->size;
    }
    ບໍ່ດັ່ງນັ້ນ
        sz1 = size();

    ຖ້າ( arr.dims() > 2 )
        ສົ່ງຄືນ false;
    ສົ່ງຄືນ sz1 == arr.size();
}

ຈຳນວນເຕັມ _InputArray::dims_(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const Mat*)obj)->dims;
    }

    ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const UMat*)obj)->dims;
    }

    ຖ້າ (k == MATX)
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ 2;
    }

    ຖ້າ( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ 1;
    }

    ຖ້າ( k == NONE )
        ສົ່ງຄືນ 0;

    ຖ້າ( k == STD_VECTOR_VECTOR )
    {
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ 1;
        CV_Assert( size_t(i) < total() );
        ສົ່ງຄືນ 1;
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ 1;
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );

        ສົ່ງຄືນ vv[i].dims;
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ 1;
        CV_Assert( i < sz.height );

        ສົ່ງຄືນ vv[i].dims;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ 1;
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );

        ສົ່ງຄືນ vv[i].dims;
    }

    ຖ້າ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ 2;
    }

    ຖ້າ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ 2;
    }

    ຖ້າ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const cuda::GpuMatND*)obj)->dims;
    }

    ຖ້າ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const Mat*)obj)->total();
    }

    ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const UMat*)obj)->total();
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ vv.size();

        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );
        ສົ່ງຄືນ vv[i].total();
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ sz.height;

        CV_Assert( i < sz.height );
        ສົ່ງຄືນ vv[i].total();
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ຖ້າ( i < 0 )
            ສົ່ງຄືນ vv.size();

        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );
        ສົ່ງຄືນ vv[i].total();
    }

    ຖ້າ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        const cuda::GpuMatND& _gpuMatND = *((const cuda::GpuMatND*)obj);
        ຖ້າ (_gpuMatND.dims > 0)
        {
            res = 1;
            ສຳລັບ(ຈຳນວນເຕັມ d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ສົ່ງຄືນ res;
        }
    }

    ຖ້າ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        ຈຳນວນເຕັມ esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ຖ້າ (k == STD_VECTOR_VECTOR) { \
                const std::vector<std::vector<T > >* vv = (const std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ຖ້າ (i < 0) \
                    ສົ່ງຄືນ n; \
                CV_Assert((size_t)i < n); \
                ສົ່ງຄືນ vv->at(i).size(); \
            } ບໍ່ດັ່ງນັ້ນ \
                ສົ່ງຄືນ ((const std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ສົ່ງຄືນ size(i).area();
}

ຈຳນວນເຕັມ _InputArray::type_(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
        ສົ່ງຄືນ ((const Mat*)obj)->type();

    ຖ້າ( k == UMAT )
        ສົ່ງຄືນ ((const UMat*)obj)->type();

    ຖ້າ( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ສົ່ງຄືນ CV_MAT_TYPE(flags);

    ຖ້າ( k == NONE )
        ສົ່ງຄືນ -1;

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ຖ້າ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ສົ່ງຄືນ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );
        ສົ່ງຄືນ vv[i >= 0 ? i : 0].type();
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ຖ້າ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ສົ່ງຄືນ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ຈຳນວນເຕັມ)vv.size() );
        ສົ່ງຄືນ vv[i >= 0 ? i : 0].type();
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        ຖ້າ( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ສົ່ງຄືນ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ສົ່ງຄືນ vv[i >= 0 ? i : 0].type();
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ຖ້າ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ສົ່ງຄືນ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ຈຳນວນເຕັມ)vv.size());
        ສົ່ງຄືນ vv[i >= 0 ? i : 0].type();
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ຖ້າ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ສົ່ງຄືນ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ຈຳນວນເຕັມ)vv.size());
        ສົ່ງຄືນ vv[i >= 0 ? i : 0].type();
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ( k == OPENGL_BUFFER )
        ສົ່ງຄືນ ((const ogl::Buffer*)obj)->type();

    ຖ້າ( k == CUDA_GPU_MAT )
        ສົ່ງຄືນ ((const cuda::GpuMat*)obj)->type();

    ຖ້າ( k == CUDA_GPU_MATND )
        ສົ່ງຄືນ ((const cuda::GpuMatND*)obj)->type();

    ຖ້າ( k == CUDA_HOST_MEM )
        ສົ່ງຄືນ ((const cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ຈຳນວນເຕັມ _InputArray::depth_(ຈຳນວນເຕັມ i) const
{
    ສົ່ງຄືນ CV_MAT_DEPTH(type(i));
}

bool _InputArray::empty_() const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
        ສົ່ງຄືນ ((const Mat*)obj)->empty();

    ຖ້າ( k == UMAT )
        ສົ່ງຄືນ ((const UMat*)obj)->empty();

    ຖ້າ (k == MATX)
        ສົ່ງຄືນ false;

    ຖ້າ( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ສົ່ງຄືນ n == 0;
    }

    ຖ້າ( k == NONE )
        ສົ່ງຄືນ true;

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        ສົ່ງຄືນ vv.empty();
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        ສົ່ງຄືນ sz.height == 0;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        ສົ່ງຄືນ vv.empty();
    }

    ຖ້າ( k == OPENGL_BUFFER )
        ສົ່ງຄືນ ((const ogl::Buffer*)obj)->empty();

    ຖ້າ( k == CUDA_GPU_MAT )
        ສົ່ງຄືນ ((const cuda::GpuMat*)obj)->empty();

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        ສົ່ງຄືນ vv.empty();
    }

    ຖ້າ( k == CUDA_GPU_MATND )
        ສົ່ງຄືນ ((const cuda::GpuMatND*)obj)->empty();

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        ສົ່ງຄືນ vv.empty();
    }

    ຖ້າ( k == CUDA_HOST_MEM )
        ສົ່ງຄືນ ((const cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isContinuous(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
        ສົ່ງຄືນ i < 0 ? ((const Mat*)obj)->isContinuous() : true;

    ຖ້າ( k == UMAT )
        ສົ່ງຄືນ i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    ຖ້າ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ສົ່ງຄືນ true;

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].isContinuous();
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ສົ່ງຄືນ vv[i].isContinuous();
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].isContinuous();
    }

    ຖ້າ( k == CUDA_GPU_MAT )
      ສົ່ງຄືນ i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    ຖ້າ( k == CUDA_GPU_MATND )
        ສົ່ງຄືນ i < 0 ? ((const cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool _InputArray::isSubmatrix(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
        ສົ່ງຄືນ i < 0 ? ((const Mat*)obj)->isSubmatrix() : false;

    ຖ້າ( k == UMAT )
        ສົ່ງຄືນ i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    ຖ້າ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ສົ່ງຄືນ false;

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].isSubmatrix();
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ສົ່ງຄືນ vv[i].isSubmatrix();
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].isSubmatrix();
    }

    ຖ້າ( k == CUDA_GPU_MATND )
    {
        ສົ່ງຄືນ ((const cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        const Mat * const m = ((const Mat*)obj);
        ສົ່ງຄືນ (size_t)(m->ptr() - m->datastart);
    }

    ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const UMat*)obj)->offset;
    }

    ຖ້າ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ສົ່ງຄືນ 0;

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ຈຳນວນເຕັມ)vv.size() );

        ສົ່ງຄືນ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ສົ່ງຄືນ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].offset;
    }

    ຖ້າ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        ສົ່ງຄືນ (size_t)(m->data - m->datastart);
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const Mat*)obj)->step;
    }

    ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const UMat*)obj)->step;
    }

    ຖ້າ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ສົ່ງຄືນ 0;

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        const std::vector<Mat>& vv = *(const std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ຈຳນວນເຕັມ)vv.size() );
        ສົ່ງຄືນ vv[i].step;
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        const Mat* vv = (const Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ສົ່ງຄືນ vv[i].step;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].step;
    }

    ຖ້າ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ສົ່ງຄືນ ((const cuda::GpuMat*)obj)->step;
    }
    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ສົ່ງຄືນ vv[i].step;
    }
    ຖ້າ( k == CUDA_GPU_MATND )
    {
        const cuda::GpuMatND& _gpuMatND = *(const cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ສົ່ງຄືນ _gpuMatND.step[i];
    }

    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        const std::vector<cuda::GpuMatND>& vv = *(const std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        const cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ສົ່ງຄືນ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == NONE )
        arr.release();
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == NONE )
        arr.release();
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    ສົ່ງຄືນ (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    ສົ່ງຄືນ (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create_(Size _sz, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ສົ່ງຄືນ;
    }
    ຖ້າ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ສົ່ງຄືນ;
    }
    ຖ້າ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ຖ້າ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ຖ້າ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ຖ້າ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ຈຳນວນເຕັມ sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create_(ຈຳນວນເຕັມ _rows, ຈຳນວນເຕັມ _cols, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ສົ່ງຄືນ;
    }
    ຖ້າ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ສົ່ງຄືນ;
    }
    ຖ້າ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ຖ້າ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ຖ້າ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ຖ້າ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ຈຳນວນເຕັມ sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(ຈຳນວນເຕັມ d, const ຈຳນວນເຕັມ* sizes, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ຈຳນວນເຕັມ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ຖ້າ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ຖ້າ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ບໍ່ດັ່ງນັ້ນ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ຖ້າ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ຖ້າ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ບໍ່ດັ່ງນັ້ນ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ຖ້າ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == MATX )
    {
        CV_Assert( i < 0 );
        ຈຳນວນເຕັມ type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ຖ້າ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ຈຳນວນເຕັມ total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ບໍ່ດັ່ງນັ້ນ
        {
            ຖ້າ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ບໍ່ດັ່ງນັ້ນ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ຈຳນວນເຕັມ esz = CV_ELEM_SIZE(flags);

        ຖ້າ( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            ຈຳນວນເຕັມ type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ຖ້າ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ຖ້າ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ສົ່ງຄືນ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ບໍ່ດັ່ງນັ້ນ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        bha_nga

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ຖ້າ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ຖ້າ( fixedType() )
            {
                ຈຳນວນເຕັມ _type = CV_MAT_TYPE(flags);
                ສຳລັບ( size_t j = len0; j < len; j++ )
                {
                    ຖ້າ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ສົ່ງຄືນ;
        }

        CV_Assert( i < (ຈຳນວນເຕັມ)v.size() );
        Mat& m = v[i];

        ຖ້າ( allowTransposed )
        {
            ຖ້າ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ຖ້າ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ບໍ່ດັ່ງນັ້ນ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ຖ້າ(fixedSize())
        {
            CV_Assert(m.dims == d);
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ຖ້າ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ຖ້າ( fixedType() )
            {
                ຈຳນວນເຕັມ _type = CV_MAT_TYPE(flags);
                ສຳລັບ( size_t j = len0; j < len; j++ )
                {
                    ຖ້າ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ສົ່ງຄືນ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ຖ້າ( allowTransposed )
        {
            ຖ້າ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ຖ້າ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ບໍ່ດັ່ງນັ້ນ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ຖ້າ(fixedSize())
        {
            CV_Assert(m.dims == d);
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ຖ້າ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ຖ້າ( fixedType() )
            {
                ຈຳນວນເຕັມ _type = CV_MAT_TYPE(flags);
                ສຳລັບ( size_t j = len0; j < len; j++ )
                {
                    ຖ້າ( v[j].type() == _type )
                        continue;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ສົ່ງຄືນ;
        }

        CV_Assert( i < (ຈຳນວນເຕັມ)v.size() );
        UMat& m = v[i];

        ຖ້າ( allowTransposed )
        {
            ຖ້າ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ຖ້າ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ບໍ່ດັ່ງນັ້ນ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ຖ້າ(fixedSize())
        {
            CV_Assert(m.dims == d);
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    ຖ້າ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::create(const MatShape& shape, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ຖ້າ (shape.dims < 0) {
        release();
    } ບໍ່ດັ່ງນັ້ນ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(ຈຳນວນເຕັມ mtype) const
{
    mtype = CV_MAT_TYPE(mtype);
    ສົ່ງຄືນ getMat().reinterpret(mtype);
}

void _OutputArray::createSameSize(const _InputArray& arr, ຈຳນວນເຕັມ mtype) const
{
    create(arr.shape(), mtype);
}

void _OutputArray::fit(Size _sz, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ຈຳນວນເຕັມ sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(ຈຳນວນເຕັມ rows, ຈຳນວນເຕັມ cols, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ຈຳນວນເຕັມ sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::fit(const MatShape& shape, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ຖ້າ (shape.dims < 0) {
        release();
        ສົ່ງຄືນ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ຖ້າ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ບໍ່ດັ່ງນັ້ນ ຖ້າ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

void _OutputArray::fitSameSize(const _InputArray& arr, ຈຳນວນເຕັມ mtype) const
{
    fit(arr.shape(), mtype);
}

void _OutputArray::fit(ຈຳນວນເຕັມ d, const ຈຳນວນເຕັມ* sizes, ຈຳນວນເຕັມ mtype, ຈຳນວນເຕັມ i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) const
{
    ຈຳນວນເຕັມ size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ຖ້າ( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ຖ້າ (k == MAT)
            m = (Mat*)obj;
        ບໍ່ດັ່ງນັ້ນ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ຖ້າ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ບໍ່ດັ່ງນັ້ນ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ຖ້າ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    ຖ້າ( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ຖ້າ (k == UMAT)
            m = (UMat*)obj;
        ບໍ່ດັ່ງນັ້ນ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ຖ້າ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ສົ່ງຄືນ;
        }

        ຖ້າ(fixedType())
        {
            ຖ້າ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ບໍ່ດັ່ງນັ້ນ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ຖ້າ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ສຳລັບ(ຈຳນວນເຕັມ j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ສົ່ງຄືນ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::release() const
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        ((Mat*)obj)->release();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ຖ້າ( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ຖ້າ( k == NONE )
        ສົ່ງຄືນ;

    ຖ້າ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ຖ້າ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ບໍ່ດັ່ງນັ້ນ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        bha_nga

        ຈຳນວນເຕັມ esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        ສຳລັບ( ຈຳນວນເຕັມ i = 0; i < sz.height; i++ )
            v[i].release();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->clear();
        ສົ່ງຄືນ;
    }

    ຖ້າ( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ສົ່ງຄືນ;
    }
    ຖ້າ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ສົ່ງຄືນ;
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ສົ່ງຄືນ;
    }

    release();
}

bool _OutputArray::needed() const
{
    ສົ່ງຄືນ kind() != NONE;
}

Mat& _OutputArray::getMatRef(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ( i < 0 )
    {
        CV_Assert( k == MAT );
        ສົ່ງຄືນ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ຖ້າ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ຈຳນວນເຕັມ)v.size() );
        ສົ່ງຄືນ v[i];
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ສົ່ງຄືນ v[i];
    }
}

UMat& _OutputArray::getUMatRef(ຈຳນວນເຕັມ i) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ສົ່ງຄືນ *(UMat*)obj;
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ຈຳນວນເຕັມ)v.size() );
        ສົ່ງຄືນ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ສົ່ງຄືນ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ສົ່ງຄືນ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ສົ່ງຄືນ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ສົ່ງຄືນ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ສົ່ງຄືນ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ສົ່ງຄືນ *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == NONE )
        ;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<dviguu_na, 4>(value.ptr<dviguu_na>())), mask);
#ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ບໍ່ດັ່ງນັ້ນ
        CV_Error(Error::StsNotImplemented, "");
}

void _OutputArray::setZero() const
{
    _InputArray::KindFlag k = kind();

    ຖ້າ( k == NONE )
        ;
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        setTo(Scalar::all(0), noArray());
    }
}

void _OutputArray::assign(const UMat& u) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const Mat& m) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MATX)
    {
        m.copyTo(getMat());
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(UMat& u) const
{
    ຖ້າ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ສົ່ງຄືນ;
    }
    ຈຳນວນເຕັມ k = kind();
    ຖ້າ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::move(Mat& m) const
{
    ຖ້າ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ສົ່ງຄືນ;
    }
    ຈຳນວນເຕັມ k = kind();
    ຖ້າ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<UMat>& v) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ສຳລັບ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            UMat& this_m = this_v[i];
            ຖ້າ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ສຳລັບ (size_t i = 0; i < v.size(); i++)
        {
            const UMat& m = v[i];
            Mat& this_m = this_v[i];
            ຖ້າ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


void _OutputArray::assign(const std::vector<Mat>& v) const
{
    _InputArray::KindFlag k = kind();
    ຖ້າ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ສຳລັບ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            UMat& this_m = this_v[i];
            ຖ້າ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ສຳລັບ (size_t i = 0; i < v.size(); i++)
        {
            const Mat& m = v[i];
            Mat& this_m = this_v[i];
            ຖ້າ (this_m.u != NULL && this_m.u == m.u)
                continue; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ບໍ່ດັ່ງນັ້ນ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray noArray() { ສົ່ງຄືນ _none; }

} // cv::
