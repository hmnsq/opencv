// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"
#ضمن "opencv2/core/mat.hpp"

نطاق cv {

تعريفنوع Vec<صحيح, 5> Vec5i;
تعريفنوع Vec<صحيح, 7> Vec7i;
تعريفنوع Vec<صحيح, 9> Vec9i;
تعريفنوع Vec<صحيح, 10> Vec10i;
تعريفنوع Vec<صحيح, 11> Vec11i;
تعريفنوع Vec<صحيح, 12> Vec12i;
تعريفنوع Vec<صحيح, 13> Vec13i;
تعريفنوع Vec<صحيح, 14> Vec14i;
تعريفنوع Vec<صحيح, 15> Vec15i;
تعريفنوع Vec<صحيح, 16> Vec16i;
تعريفنوع Vec<صحيح, 32> Vec32i;
تعريفنوع Vec<صحيح, 64> Vec64i;
تعريفنوع Vec<صحيح, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    بدل( esz ) \
    { \
    حالة 1: func(uchar); \
    حالة 2: func(Vec2b); \
    حالة 3: func(Vec3b); \
    حالة 4: func(صحيح); \
    حالة 6: func(Vec3s); \
    حالة 8: func(Vec2i); \
    حالة 12: func(Vec3i); \
    حالة 16: func(Vec4i); \
    حالة 20: func(Vec5i); \
    حالة 24: func(Vec6i); \
    حالة 28: func(Vec7i); \
    حالة 32: func(Vec8i); \
    حالة 36: func(Vec9i); \
    حالة 40: func(Vec10i); \
    حالة 44: func(Vec11i); \
    حالة 48: func(Vec12i); \
    حالة 52: func(Vec13i); \
    حالة 56: func(Vec14i); \
    حالة 60: func(Vec15i); \
    حالة 64: func(Vec16i); \
    حالة 128: func(Vec32i); \
    حالة 256: func(Vec64i); \
    حالة 512: func(Vec128i); \
    افتراضي: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat _InputArray::getMat_(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ܐܢ( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ *m;
        ܦܢܐ m->row(i);
    }

    ܐܢ( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ m->getMat(accessFlags);
        ܦܢܐ m->getMat(accessFlags).row(i);
    }

    ܐܢ (k == MATX)
    {
        CV_Assert( i < 0 );
        ܦܢܐ Mat(sz, flags, obj);
    }

    ܐܢ( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        صحيح t = CV_8U;
        ثابت std::vector<منطقي>& v = *(ثابت std::vector<منطقي>*)obj;
        صحيح j, n = (صحيح)v.size();
        ܐܢ( n == 0 )
            ܦܢܐ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        لكل( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ܦܢܐ m;
    }

    ܐܢ( k == NONE )
        ܦܢܐ Mat();

    ܐܢ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        صحيح t = CV_MAT_TYPE(flags);
        صحيح esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ܐܢ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ܐܚܪܢܐ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            صحيح v_sz = صحيح(v->size()); \
            ܦܢܐ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        ܦܢܐ v[i];
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ܦܢܐ v[i];
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        ܦܢܐ v[i].getMat(accessFlags);
    }

    ܐܢ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ܐܢ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ܐܢ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ܐܢ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;

        ܦܢܐ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat _InputArray::getUMat(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ܐܢ( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ *m;
        ܦܢܐ m->row(i);
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        ܦܢܐ v[i];
    }

    ܐܢ( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ m->getUMat(accessFlags);
        ܦܢܐ m->row(i).getUMat(accessFlags);
    }

    ܦܢܐ getMat(i).getUMat(accessFlags);
}

فراغ _InputArray::getMatVector(std::vector<Mat>& mv) ثابت
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ܐܢ( k == MAT )
    {
        ثابت Mat& m = *(ثابت Mat*)obj;
        صحيح n = (صحيح)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        لكل( صحيح i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (فراغ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (فراغ*)m.ptr(i), &m.step[1]);
        ܦܢܐ;
    }

    ܐܢ (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR )
    {
        ثابت std::vector<uchar>& v = *(ثابت std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        صحيح t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (فراغ*)(v.data() + esz*i));
        ܦܢܐ;
    }

    ܐܢ( k == NONE )
    {
        mv.clear();
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_VECTOR )
    {
        صحيح typ = CV_MAT_TYPE(flags);
        صحيح esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            لكل (size_t i = 0; i < n; i++) { \
                ثابت std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                صحيح ni = (صحيح)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (فراغ*)vi.data()) : Mat(); \
            } \
        } \
        اكسر

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ܦܢܐ;
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ܦܢܐ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _InputArray::getUMatVector(std::vector<UMat>& umv) ثابت
{
    _InputArray::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ܐܢ( k == NONE )
    {
        umv.clear();
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ܦܢܐ;
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ܦܢܐ;
    }

    ܐܢ( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ܦܢܐ;
    }
    ܐܢ( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ܦܢܐ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() ثابت
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ܐܢ (k == CUDA_GPU_MAT)
    {
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        ܦܢܐ *d_mat;
    }

    ܐܢ (k == CUDA_HOST_MEM)
    {
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        ܦܢܐ cuda_mem->createGpuMatHeader();
    }

    ܐܢ (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ܐܢ (k == NONE)
        ܦܢܐ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ܐܚܪܢܐ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
فراغ _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) ثابت
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ܐܚܪܢܐ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
فراغ _InputArray::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ثابت
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ܐܚܪܢܐ
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND _InputArray::getGpuMatND() ثابت
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();

    ܐܢ (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND* d_mat = (ثابت cuda::GpuMatND*)obj;
        ܦܢܐ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ܐܚܪܢܐ
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer _InputArray::getOGlBuffer() ثابت
{
    _InputArray::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    ثابت ogl::Buffer* gl_buf = (ثابت ogl::Buffer*)obj;
    ܦܢܐ *gl_buf;
}

صحيح _InputArray::rows_(صحيح i) ثابت
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ܐܢ (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        ܦܢܐ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ܦܢܐ size_(i).height;
}

صحيح _InputArray::cols_(صحيح i) ثابت
{
#ifdef HAVE_CUDA
    _InputArray::KindFlag k = kind();
    ܐܢ (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        ܦܢܐ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ܦܢܐ size_(i).width;
}

Size _InputArray::size_(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat* m = (ثابت Mat*)obj;
        CV_Assert(m->dims <= 2);
        ܦܢܐ Size(m->cols, m->rows);
    }

    ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat* m = (ثابت UMat*)obj;
        CV_Assert(m->dims <= 2);
        ܦܢܐ Size(m->cols, m->rows);
    }

    ܐܢ (k == MATX)
    {
        CV_Assert( i < 0 );
        ܦܢܐ sz;
    }

    ܐܢ( k == NONE )
        ܦܢܐ Size();

    ܐܢ( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ܦܢܐ Size((صحيح)n, 1);
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ vv.empty() ? Size() : Size((صحيح)vv.size(), 1);
        CV_Assert( i < (صحيح)vv.size() );

        ܦܢܐ vv[i].size();
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ܦܢܐ vv[i].size();
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        ܐܢ (i < 0)
            ܦܢܐ vv.empty() ? Size() : Size((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ܦܢܐ vv[i].size();
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        ܐܢ (i < 0)
            ܦܢܐ vv.empty() ? Size() : Size((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        ܐܢ (m.dims == 0)
            ܦܢܐ Size();
        ܐܢ (m.dims == 1)
            ܦܢܐ Size(1, m.size[0]);
        ܦܢܐ Size(m.size[1], m.size[0]);
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ vv.empty() ? Size() : Size((صحيح)vv.size(), 1);
        CV_Assert( i < (صحيح)vv.size() );

        ܦܢܐ vv[i].size();
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        ܐܢ (i < 0)
            ܦܢܐ vv.empty() ? Size() : Size((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        ܐܢ (m.dims == 0)
            ܦܢܐ Size();
        ܐܢ (m.dims == 1)
            ܦܢܐ Size(1, m.size[0]);
        ܦܢܐ Size(m.size[1], m.size[0]);
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ثابت ogl::Buffer* buf = (ثابت ogl::Buffer*)obj;
        ܦܢܐ buf->size();
    }

    ܐܢ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        ܦܢܐ d_mat->size();
    }

    ܐܢ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        ܦܢܐ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحيح _InputArray::sizend(صحيح* arrsz, صحيح i) ثابت
{
    صحيح j, d = 0;
    _InputArray::KindFlag k = kind();

    ܐܢ( k == NONE )
        ;
    ܐܚܪܢܐ ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        d = m.dims;
        ܐܢ(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ܐܚܪܢܐ ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        d = m.dims;
        ܐܢ(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ܐܚܪܢܐ ܐܢ( k == STD_VECTOR_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i < (صحيح)vv.size() );
        ثابت Mat& m = vv[i];
        d = m.dims;
        ܐܢ(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ܐܚܪܢܐ ܐܢ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.height );
        ثابت Mat& m = vv[i];
        d = m.dims;
        ܐܢ(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ܐܚܪܢܐ ܐܢ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( i < (صحيح)vv.size() );
        ثابت UMat& m = vv[i];
        d = m.dims;
        ܐܢ(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ܐܚܪܢܐ ܐܢ (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ܐܢ(arrsz)
        {
            arrsz[0] = صحيح(n);
        }
    }
    ܐܚܪܢܐ
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ܐܢ(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ܦܢܐ d;
}

منطقي _InputArray::empty(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ (i >= 0) {
        ܐܢ (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<ثابت std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ܦܢܐ mv->at(i).empty();
        }
        ܐܚܪܢܐ ܐܢ (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<ثابت std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ܦܢܐ umv->at(i).empty();
        }
        ܐܚܪܢܐ ܐܢ (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ܦܢܐ n == 0;
        } ܐܚܪܢܐ {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ܦܢܐ empty();
}

MatShape _InputArray::shape(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();
    MatShape shape;

    ܐܢ( k == NONE )
        ;
    ܐܚܪܢܐ ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        shape = m.size;
    }
    ܐܚܪܢܐ ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        shape = m.size;
    }
    ܐܚܪܢܐ ܐܢ( k == STD_VECTOR_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    ܐܚܪܢܐ ܐܢ( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.height );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    ܐܚܪܢܐ ܐܢ( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت UMat& m = vv[i];
        shape = m.size;
    }
    ܐܚܪܢܐ
    {
        صحيح sizes[CV_MAX_DIM];
        صحيح dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ܦܢܐ shape;
}

منطقي _InputArray::sameSize(ثابت _InputArray& arr) ثابت
{
    _InputArray::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ܐܢ( k1 == MAT )
    {
        ثابت Mat* m = ((ثابت Mat*)obj);
        ܐܢ( k2 == MAT )
            ܦܢܐ m->size == ((ثابت Mat*)arr.obj)->size;
        ܐܢ( k2 == UMAT )
            ܦܢܐ m->size == ((ثابت UMat*)arr.obj)->size;
        ܐܢ( m->dims > 2 )
            ܦܢܐ ܕܓܠܐ;
        sz1 = m->size();
    }
    ܐܚܪܢܐ ܐܢ( k1 == UMAT )
    {
        ثابت UMat* m = ((ثابت UMat*)obj);
        ܐܢ( k2 == MAT )
            ܦܢܐ m->size == ((ثابت Mat*)arr.obj)->size;
        ܐܢ( k2 == UMAT )
            ܦܢܐ m->size == ((ثابت UMat*)arr.obj)->size;
        ܐܢ( m->dims > 2 )
            ܦܢܐ ܕܓܠܐ;
        sz1 = m->size();
    }
    ܐܚܪܢܐ ܐܢ ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ܦܢܐ ((ثابت cuda::GpuMatND*)obj)->size == ((ثابت cuda::GpuMatND*)arr.obj)->size;
    }
    ܐܚܪܢܐ
        sz1 = size();

    ܐܢ( arr.dims() > 2 )
        ܦܢܐ ܕܓܠܐ;
    ܦܢܐ sz1 == arr.size();
}

صحيح _InputArray::dims_(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت Mat*)obj)->dims;
    }

    ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت UMat*)obj)->dims;
    }

    ܐܢ (k == MATX)
    {
        CV_Assert( i < 0 );
        ܦܢܐ 2;
    }

    ܐܢ( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ܦܢܐ 1;
    }

    ܐܢ( k == NONE )
        ܦܢܐ 0;

    ܐܢ( k == STD_VECTOR_VECTOR )
    {
        ܐܢ( i < 0 )
            ܦܢܐ 1;
        CV_Assert( size_t(i) < total() );
        ܦܢܐ 1;
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ 1;
        CV_Assert( i < (صحيح)vv.size() );

        ܦܢܐ vv[i].dims;
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ 1;
        CV_Assert( i < sz.height );

        ܦܢܐ vv[i].dims;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ 1;
        CV_Assert( i < (صحيح)vv.size() );

        ܦܢܐ vv[i].dims;
    }

    ܐܢ( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ܦܢܐ 2;
    }

    ܐܢ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ 2;
    }

    ܐܢ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت cuda::GpuMatND*)obj)->dims;
    }

    ܐܢ( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ܦܢܐ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t _InputArray::total(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت Mat*)obj)->total();
    }

    ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت UMat*)obj)->total();
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ vv.size();

        CV_Assert( i < (صحيح)vv.size() );
        ܦܢܐ vv[i].total();
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ sz.height;

        CV_Assert( i < sz.height );
        ܦܢܐ vv[i].total();
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        ܐܢ( i < 0 )
            ܦܢܐ vv.size();

        CV_Assert( i < (صحيح)vv.size() );
        ܦܢܐ vv[i].total();
    }

    ܐܢ( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ثابت cuda::GpuMatND& _gpuMatND = *((ثابت cuda::GpuMatND*)obj);
        ܐܢ (_gpuMatND.dims > 0)
        {
            res = 1;
            لكل(صحيح d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ܦܢܐ res;
        }
    }

    ܐܢ (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        صحيح esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ܐܢ (k == STD_VECTOR_VECTOR) { \
                ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ܐܢ (i < 0) \
                    ܦܢܐ n; \
                CV_Assert((size_t)i < n); \
                ܦܢܐ vv->at(i).size(); \
            } ܐܚܪܢܐ \
                ܦܢܐ ((ثابت std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ܦܢܐ size(i).area();
}

صحيح _InputArray::type_(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
        ܦܢܐ ((ثابت Mat*)obj)->type();

    ܐܢ( k == UMAT )
        ܦܢܐ ((ثابت UMat*)obj)->type();

    ܐܢ( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ܦܢܐ CV_MAT_TYPE(flags);

    ܐܢ( k == NONE )
        ܦܢܐ -1;

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        ܐܢ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ܦܢܐ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحيح)vv.size() );
        ܦܢܐ vv[i >= 0 ? i : 0].type();
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        ܐܢ( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ܦܢܐ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحيح)vv.size() );
        ܦܢܐ vv[i >= 0 ? i : 0].type();
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        ܐܢ( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ܦܢܐ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ܦܢܐ vv[i >= 0 ? i : 0].type();
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        ܐܢ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ܦܢܐ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحيح)vv.size());
        ܦܢܐ vv[i >= 0 ? i : 0].type();
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        ܐܢ (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ܦܢܐ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحيح)vv.size());
        ܦܢܐ vv[i >= 0 ? i : 0].type();
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ( k == OPENGL_BUFFER )
        ܦܢܐ ((ثابت ogl::Buffer*)obj)->type();

    ܐܢ( k == CUDA_GPU_MAT )
        ܦܢܐ ((ثابت cuda::GpuMat*)obj)->type();

    ܐܢ( k == CUDA_GPU_MATND )
        ܦܢܐ ((ثابت cuda::GpuMatND*)obj)->type();

    ܐܢ( k == CUDA_HOST_MEM )
        ܦܢܐ ((ثابت cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحيح _InputArray::depth_(صحيح i) ثابت
{
    ܦܢܐ CV_MAT_DEPTH(type(i));
}

منطقي _InputArray::empty_() ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
        ܦܢܐ ((ثابت Mat*)obj)->empty();

    ܐܢ( k == UMAT )
        ܦܢܐ ((ثابت UMat*)obj)->empty();

    ܐܢ (k == MATX)
        ܦܢܐ ܕܓܠܐ;

    ܐܢ( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ܦܢܐ n == 0;
    }

    ܐܢ( k == NONE )
        ܦܢܐ ܫܪܪܐ;

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        ܦܢܐ vv.empty();
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ܦܢܐ sz.height == 0;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        ܦܢܐ vv.empty();
    }

    ܐܢ( k == OPENGL_BUFFER )
        ܦܢܐ ((ثابت ogl::Buffer*)obj)->empty();

    ܐܢ( k == CUDA_GPU_MAT )
        ܦܢܐ ((ثابت cuda::GpuMat*)obj)->empty();

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        ܦܢܐ vv.empty();
    }

    ܐܢ( k == CUDA_GPU_MATND )
        ܦܢܐ ((ثابت cuda::GpuMatND*)obj)->empty();

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        ܦܢܐ vv.empty();
    }

    ܐܢ( k == CUDA_HOST_MEM )
        ܦܢܐ ((ثابت cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقي _InputArray::isContinuous(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
        ܦܢܐ i < 0 ? ((ثابت Mat*)obj)->isContinuous() : ܫܪܪܐ;

    ܐܢ( k == UMAT )
        ܦܢܐ i < 0 ? ((ثابت UMat*)obj)->isContinuous() : ܫܪܪܐ;

    ܐܢ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ܦܢܐ ܫܪܪܐ;

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].isContinuous();
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ܦܢܐ vv[i].isContinuous();
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].isContinuous();
    }

    ܐܢ( k == CUDA_GPU_MAT )
      ܦܢܐ i < 0 ? ((ثابت cuda::GpuMat*)obj)->isContinuous() : ܫܪܪܐ;

    ܐܢ( k == CUDA_GPU_MATND )
        ܦܢܐ i < 0 ? ((ثابت cuda::GpuMatND*)obj)->isContinuous() : ܫܪܪܐ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقي _InputArray::isSubmatrix(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
        ܦܢܐ i < 0 ? ((ثابت Mat*)obj)->isSubmatrix() : ܕܓܠܐ;

    ܐܢ( k == UMAT )
        ܦܢܐ i < 0 ? ((ثابت UMat*)obj)->isSubmatrix() : ܕܓܠܐ;

    ܐܢ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ܦܢܐ ܕܓܠܐ;

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].isSubmatrix();
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ܦܢܐ vv[i].isSubmatrix();
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].isSubmatrix();
    }

    ܐܢ( k == CUDA_GPU_MATND )
    {
        ܦܢܐ ((ثابت cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t _InputArray::offset(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat * ثابت m = ((ثابت Mat*)obj);
        ܦܢܐ (size_t)(m->ptr() - m->datastart);
    }

    ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت UMat*)obj)->offset;
    }

    ܐܢ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ܦܢܐ 0;

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحيح)vv.size() );

        ܦܢܐ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ܦܢܐ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].offset;
    }

    ܐܢ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat * ثابت m = ((ثابت cuda::GpuMat*)obj);
        ܦܢܐ (size_t)(m->data - m->datastart);
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t _InputArray::step(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت Mat*)obj)->step;
    }

    ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت UMat*)obj)->step;
    }

    ܐܢ( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ܦܢܐ 0;

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحيح)vv.size() );
        ܦܢܐ vv[i].step;
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ܦܢܐ vv[i].step;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].step;
    }

    ܐܢ( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ܦܢܐ ((ثابت cuda::GpuMat*)obj)->step;
    }
    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ܦܢܐ vv[i].step;
    }
    ܐܢ( k == CUDA_GPU_MATND )
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ܦܢܐ _gpuMatND.step[i];
    }

    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ثابت cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ܦܢܐ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

فراغ _InputArray::copyTo(ثابت _OutputArray& arr) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == NONE )
        arr.release();
    ܐܚܪܢܐ ܐܢ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ܐܚܪܢܐ ܐܢ( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ܐܚܪܢܐ ܐܢ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "");
}

فراغ _InputArray::copyTo(ثابت _OutputArray& arr, ثابت _InputArray & mask) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == NONE )
        arr.release();
    ܐܚܪܢܐ ܐܢ( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ܐܚܪܢܐ ܐܢ( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ܐܚܪܢܐ ܐܢ (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "");
}

منطقي _OutputArray::fixedSize() ثابت
{
    ܦܢܐ (flags & FIXED_SIZE) == FIXED_SIZE;
}

منطقي _OutputArray::fixedType() ثابت
{
    ܦܢܐ (flags & FIXED_TYPE) == FIXED_TYPE;
}

فراغ _OutputArray::create_(Size _sz, صحيح mtype, صحيح i, منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ܦܢܐ;
    }
    ܐܢ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ܦܢܐ;
    }
    ܐܢ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ܐܢ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ܐܢ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ܐܢ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    صحيح sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

فراغ _OutputArray::create_(صحيح _rows, صحيح _cols, صحيح mtype, صحيح i, منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ܦܢܐ;
    }
    ܐܢ( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ܦܢܐ;
    }
    ܐܢ( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ܐܢ( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ܐܢ( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ܐܢ( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    صحيح sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

فراغ _OutputArray::create(صحيح d, ثابت صحيح* sizes, صحيح mtype, صحيح i,
                          منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحيح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ܐܢ( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ܐܢ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ܐܚܪܢܐ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ܐܢ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ܦܢܐ;
    }

    ܐܢ( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ܐܢ (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ܐܚܪܢܐ
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ܐܢ(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ܦܢܐ;
    }

    ܐܢ( k == MATX )
    {
        CV_Assert( i < 0 );
        صحيح type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ܐܢ (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            صحيح total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ܐܚܪܢܐ
        {
            ܐܢ (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ܐܚܪܢܐ
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        صحيح esz = CV_ELEM_SIZE(flags);

        ܐܢ( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            صحيح type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ܐܢ (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ܐܢ (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ܦܢܐ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ܐܚܪܢܐ { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        اكسر

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ܦܢܐ;
    }

    ܐܢ( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ܐܢ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ܐܢ( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    ܐܢ( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ܦܢܐ;
        }

        CV_Assert( i < (صحيح)v.size() );
        Mat& m = v[i];

        ܐܢ( allowTransposed )
        {
            ܐܢ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ܐܢ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ܐܚܪܢܐ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ܐܢ(fixedSize())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ܦܢܐ;
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ܐܢ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ܐܢ( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    ܐܢ( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ܦܢܐ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ܐܢ( allowTransposed )
        {
            ܐܢ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ܐܢ( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ܐܚܪܢܐ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ܐܢ(fixedSize())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ܐܢ( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ܐܢ( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    ܐܢ( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ܦܢܐ;
        }

        CV_Assert( i < (صحيح)v.size() );
        UMat& m = v[i];

        ܐܢ( allowTransposed )
        {
            ܐܢ( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ܐܢ( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ܐܚܪܢܐ
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ܐܢ(fixedSize())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ܦܢܐ;
    }

    ܐܢ ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ܦܢܐ;
    }

    ܐܢ( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _OutputArray::create(ثابت MatShape& shape, صحيح mtype, صحيح i,
                          منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    ܐܢ (shape.dims < 0) {
        release();
    } ܐܚܪܢܐ {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(صحيح mtype) ثابت
{
    mtype = CV_MAT_TYPE(mtype);
    ܦܢܐ getMat().reinterpret(mtype);
}

فراغ _OutputArray::createSameSize(ثابت _InputArray& arr, صحيح mtype) ثابت
{
    create(arr.shape(), mtype);
}

فراغ _OutputArray::fit(Size _sz, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحيح sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

فراغ _OutputArray::fit(صحيح rows, صحيح cols, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحيح sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

فراغ _OutputArray::fit(ثابت MatShape& shape, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    ܐܢ (shape.dims < 0) {
        release();
        ܦܢܐ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    _InputArray::KindFlag k = kind();
    ܐܢ ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ܐܚܪܢܐ ܐܢ ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

فراغ _OutputArray::fitSameSize(ثابت _InputArray& arr, صحيح mtype) ثابت
{
    fit(arr.shape(), mtype);
}

فراغ _OutputArray::fit(صحيح d, ثابت صحيح* sizes, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحيح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    _InputArray::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ܐܢ( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ܐܢ (k == MAT)
            m = (Mat*)obj;
        ܐܚܪܢܐ {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ܐܢ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ܐܚܪܢܐ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ܐܢ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ܦܢܐ;
    }

    ܐܢ( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ܐܢ (k == UMAT)
            m = (UMat*)obj;
        ܐܚܪܢܐ {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ܐܢ (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ܦܢܐ;
        }

        ܐܢ(fixedType())
        {
            ܐܢ(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ܐܚܪܢܐ
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ܐܢ(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ܦܢܐ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

فراغ _OutputArray::release() ثابت
{
    CV_Assert(!fixedSize());

    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        ((Mat*)obj)->release();
        ܦܢܐ;
    }

    ܐܢ( k == UMAT )
    {
        ((UMat*)obj)->release();
        ܦܢܐ;
    }

    ܐܢ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ܐܢ( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ܐܢ( k == NONE )
        ܦܢܐ;

    ܐܢ( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ܐܢ (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ܐܚܪܢܐ { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        اكسر

        صحيح esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ܦܢܐ;
    }

    ܐܢ( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        لكل( صحيح i = 0; i < sz.height; i++ )
            v[i].release();
        ܦܢܐ;
    }

    ܐܢ( k == STD_BOOL_VECTOR )
    {
        ((std::vector<منطقي>*)obj)->clear();
        ܦܢܐ;
    }

    ܐܢ( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ܦܢܐ;
    }
    ܐܢ (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ܦܢܐ;
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _OutputArray::clear() ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ܦܢܐ;
    }

    release();
}

منطقي _OutputArray::needed() ثابت
{
    ܦܢܐ kind() != NONE;
}

Mat& _OutputArray::getMatRef(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ( i < 0 )
    {
        CV_Assert( k == MAT );
        ܦܢܐ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ܐܢ( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (صحيح)v.size() );
        ܦܢܐ v[i];
    }
    ܐܚܪܢܐ
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ܦܢܐ v[i];
    }
}

UMat& _OutputArray::getUMatRef(صحيح i) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ( i < 0 )
    {
        CV_Assert( k == UMAT );
        ܦܢܐ *(UMat*)obj;
    }
    ܐܚܪܢܐ
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (صحيح)v.size() );
        ܦܢܐ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ثابت
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ܦܢܐ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ثابت
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ܦܢܐ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ثابت
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ܦܢܐ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ثابت
{
    _InputArray::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ܦܢܐ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() ثابت
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ܦܢܐ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ثابت
{
    _InputArray::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ܦܢܐ *(cuda::HostMem*)obj;
}

فراغ _OutputArray::setTo(ثابت _InputArray& arr, ثابت _InputArray & mask) ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == NONE )
        ;
    ܐܚܪܢܐ ܐܢ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ܐܚܪܢܐ ܐܢ( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ܐܚܪܢܐ ܐܢ( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<مزدوج, 4>(value.ptr<مزدوج>())), mask);
#ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ܐܚܪܢܐ
        CV_Error(Error::StsNotImplemented, "");
}

فراغ _OutputArray::setZero() ثابت
{
    _InputArray::KindFlag k = kind();

    ܐܢ( k == NONE )
        ;
    ܐܚܪܢܐ ܐܢ (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ܐܚܪܢܐ
    {
        setTo(Scalar::all(0), noArray());
    }
}

فراغ _OutputArray::assign(ثابت UMat& u) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ܐܚܪܢܐ ܐܢ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ܐܚܪܢܐ ܐܢ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ܐܚܪܢܐ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::assign(ثابت Mat& m) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ܐܚܪܢܐ ܐܢ (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ܐܚܪܢܐ ܐܢ (k == MATX)
    {
        m.copyTo(getMat());
    }
    ܐܚܪܢܐ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::move(UMat& u) ثابت
{
    ܐܢ (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ܦܢܐ;
    }
    صحيح k = kind();
    ܐܢ (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ܐܚܪܢܐ ܐܢ (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ܐܚܪܢܐ ܐܢ (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ܐܚܪܢܐ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::move(Mat& m) ثابت
{
    ܐܢ (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ܦܢܐ;
    }
    صحيح k = kind();
    ܐܢ (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ܐܚܪܢܐ ܐܢ (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ܐܚܪܢܐ ܐܢ (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ܐܚܪܢܐ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::assign(ثابت std::vector<UMat>& v) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            UMat& this_m = this_v[i];
            ܐܢ (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ܐܚܪܢܐ ܐܢ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            Mat& this_m = this_v[i];
            ܐܢ (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ܐܚܪܢܐ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::assign(ثابت std::vector<Mat>& v) ثابت
{
    _InputArray::KindFlag k = kind();
    ܐܢ (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            UMat& this_m = this_v[i];
            ܐܢ (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ܐܚܪܢܐ ܐܢ (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            Mat& this_m = this_v[i];
            ܐܢ (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ܐܚܪܢܐ
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ساكن _InputOutputArray _none;
InputOutputArray noArray() { ܦܢܐ _none; }

} // cv::
