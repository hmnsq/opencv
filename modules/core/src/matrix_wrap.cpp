// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"
#شامل "opencv2/core/mat.hpp"

فضاینام cv {

تعریفنوع Vec<صحیح, 5> Vec5i;
تعریفنوع Vec<صحیح, 7> Vec7i;
تعریفنوع Vec<صحیح, 9> Vec9i;
تعریفنوع Vec<صحیح, 10> Vec10i;
تعریفنوع Vec<صحیح, 11> Vec11i;
تعریفنوع Vec<صحیح, 12> Vec12i;
تعریفنوع Vec<صحیح, 13> Vec13i;
تعریفنوع Vec<صحیح, 14> Vec14i;
تعریفنوع Vec<صحیح, 15> Vec15i;
تعریفنوع Vec<صحیح, 16> Vec16i;
تعریفنوع Vec<صحیح, 32> Vec32i;
تعریفنوع Vec<صحیح, 64> Vec64i;
تعریفنوع Vec<صحیح, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    انتخاب( esz ) \
    { \
    حالت 1: func(uchar); \
    حالت 2: func(Vec2b); \
    حالت 3: func(Vec3b); \
    حالت 4: func(صحیح); \
    حالت 6: func(Vec3s); \
    حالت 8: func(Vec2i); \
    حالت 12: func(Vec3i); \
    حالت 16: func(Vec4i); \
    حالت 20: func(Vec5i); \
    حالت 24: func(Vec6i); \
    حالت 28: func(Vec7i); \
    حالت 32: func(Vec8i); \
    حالت 36: func(Vec9i); \
    حالت 40: func(Vec10i); \
    حالت 44: func(Vec11i); \
    حالت 48: func(Vec12i); \
    حالت 52: func(Vec13i); \
    حالت 56: func(Vec14i); \
    حالت 60: func(Vec15i); \
    حالت 64: func(Vec16i); \
    حالت 128: func(Vec32i); \
    حالت 256: func(Vec64i); \
    حالت 512: func(Vec128i); \
    پیشفرض: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat درون_داد_Array_2::getMat_(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    اگر( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        اگر( i < 0 )
            بازگشت *m;
        بازگشت m->row(i);
    }

    اگر( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        اگر( i < 0 )
            بازگشت m->getMat(accessFlags);
        بازگشت m->getMat(accessFlags).row(i);
    }

    اگر (k == MATX)
    {
        CV_Assert( i < 0 );
        بازگشت Mat(sz, flags, obj);
    }

    اگر( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        صحیح t = CV_8U;
        ثابت std::vector<منطقی>& v = *(ثابت std::vector<منطقی>*)obj;
        صحیح j, n = (صحیح)v.size();
        اگر( n == 0 )
            بازگشت Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        برای( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        بازگشت m;
    }

    اگر( k == NONE )
        بازگشت Mat();

    اگر( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        صحیح t = CV_MAT_TYPE(flags);
        صحیح esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            اگر (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } وگرنه { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            صحیح v_sz = صحیح(v->size()); \
            بازگشت v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (صحیح)v.size() );

        بازگشت v[i];
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        بازگشت v[i];
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحیح)v.size() );

        بازگشت v[i].getMat(accessFlags);
    }

    اگر( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    اگر( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    اگر( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;

        بازگشت cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat درون_داد_Array_2::getUMat(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    اگر( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        اگر( i < 0 )
            بازگشت *m;
        بازگشت m->row(i);
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحیح)v.size() );

        بازگشت v[i];
    }

    اگر( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        اگر( i < 0 )
            بازگشت m->getUMat(accessFlags);
        بازگشت m->row(i).getUMat(accessFlags);
    }

    بازگشت getMat(i).getUMat(accessFlags);
}

تهی درون_داد_Array_2::getMatVector(std::vector<Mat>& mv) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    اگر( k == MAT )
    {
        ثابت Mat& m = *(ثابت Mat*)obj;
        صحیح n = (صحیح)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        برای( صحیح i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (تهی*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (تهی*)m.ptr(i), &m.step[1]);
        بازگشت;
    }

    اگر (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        بازگشت;
    }

    اگر( k == STD_VECTOR )
    {
        ثابت std::vector<uchar>& v = *(ثابت std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        صحیح t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (تهی*)(v.data() + esz*i));
        بازگشت;
    }

    اگر( k == NONE )
    {
        mv.clear();
        بازگشت;
    }

    اگر( k == STD_VECTOR_VECTOR )
    {
        صحیح typ = CV_MAT_TYPE(flags);
        صحیح esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            برای (size_t i = 0; i < n; i++) { \
                ثابت std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                صحیح ni = (صحیح)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (تهی*)vi.data()) : Mat(); \
            } \
        } \
        بشکن

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        بازگشت;
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        بازگشت;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        بازگشت;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        بازگشت;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

تهی درون_داد_Array_2::getUMatVector(std::vector<UMat>& umv) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    اگر( k == NONE )
    {
        umv.clear();
        بازگشت;
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        بازگشت;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        بازگشت;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        برای( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        بازگشت;
    }

    اگر( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        بازگشت;
    }
    اگر( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        بازگشت;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat درون_داد_Array_2::getGpuMat() ثابت
{
#ifdef HAVE_CUDA
    درون_داد_Array_2::KindFlag k = kind();

    اگر (k == CUDA_GPU_MAT)
    {
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        بازگشت *d_mat;
    }

    اگر (k == CUDA_HOST_MEM)
    {
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        بازگشت cuda_mem->createGpuMatHeader();
    }

    اگر (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    اگر (k == NONE)
        بازگشت cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#وگرنه
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
تهی درون_داد_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) ثابت
{
#ifdef HAVE_CUDA
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#وگرنه
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
تهی درون_داد_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ثابت
{
#ifdef HAVE_CUDA
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#وگرنه
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND درون_داد_Array_2::getGpuMatND() ثابت
{
#ifdef HAVE_CUDA
    درون_داد_Array_2::KindFlag k = kind();

    اگر (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND* d_mat = (ثابت cuda::GpuMatND*)obj;
        بازگشت *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#وگرنه
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer درون_داد_Array_2::getOGlBuffer() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    ثابت ogl::Buffer* gl_buf = (ثابت ogl::Buffer*)obj;
    بازگشت *gl_buf;
}

صحیح درون_داد_Array_2::rows_(صحیح i) ثابت
{
#ifdef HAVE_CUDA
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        بازگشت (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    بازگشت size_(i).height;
}

صحیح درون_داد_Array_2::cols_(صحیح i) ثابت
{
#ifdef HAVE_CUDA
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        بازگشت (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    بازگشت size_(i).width;
}

Size درون_داد_Array_2::size_(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat* m = (ثابت Mat*)obj;
        CV_Assert(m->dims <= 2);
        بازگشت Size(m->cols, m->rows);
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat* m = (ثابت UMat*)obj;
        CV_Assert(m->dims <= 2);
        بازگشت Size(m->cols, m->rows);
    }

    اگر (k == MATX)
    {
        CV_Assert( i < 0 );
        بازگشت sz;
    }

    اگر( k == NONE )
        بازگشت Size();

    اگر( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        بازگشت Size((صحیح)n, 1);
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( i < 0 )
            بازگشت vv.empty() ? Size() : Size((صحیح)vv.size(), 1);
        CV_Assert( i < (صحیح)vv.size() );

        بازگشت vv[i].size();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( i < 0 )
            بازگشت sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        بازگشت vv[i].size();
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        اگر (i < 0)
            بازگشت vv.empty() ? Size() : Size((صحیح)vv.size(), 1);
        CV_Assert(i < (صحیح)vv.size());
        بازگشت vv[i].size();
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        اگر (i < 0)
            بازگشت vv.empty() ? Size() : Size((صحیح)vv.size(), 1);
        CV_Assert(i < (صحیح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        اگر (m.dims == 0)
            بازگشت Size();
        اگر (m.dims == 1)
            بازگشت Size(1, m.size[0]);
        بازگشت Size(m.size[1], m.size[0]);
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( i < 0 )
            بازگشت vv.empty() ? Size() : Size((صحیح)vv.size(), 1);
        CV_Assert( i < (صحیح)vv.size() );

        بازگشت vv[i].size();
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        اگر (i < 0)
            بازگشت vv.empty() ? Size() : Size((صحیح)vv.size(), 1);
        CV_Assert(i < (صحیح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        اگر (m.dims == 0)
            بازگشت Size();
        اگر (m.dims == 1)
            بازگشت Size(1, m.size[0]);
        بازگشت Size(m.size[1], m.size[0]);
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ثابت ogl::Buffer* buf = (ثابت ogl::Buffer*)obj;
        بازگشت buf->size();
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        بازگشت d_mat->size();
    }

    اگر( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        بازگشت cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحیح درون_داد_Array_2::sizend(صحیح* arrsz, صحیح i) ثابت
{
    صحیح j, d = 0;
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == NONE )
        ;
    وگرنه اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        d = m.dims;
        اگر(arrsz)
            برای(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وگرنه اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        d = m.dims;
        اگر(arrsz)
            برای(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وگرنه اگر( k == STD_VECTOR_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i < (صحیح)vv.size() );
        ثابت Mat& m = vv[i];
        d = m.dims;
        اگر(arrsz)
            برای(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وگرنه اگر( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.height );
        ثابت Mat& m = vv[i];
        d = m.dims;
        اگر(arrsz)
            برای(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وگرنه اگر( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( i < (صحیح)vv.size() );
        ثابت UMat& m = vv[i];
        d = m.dims;
        اگر(arrsz)
            برای(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وگرنه اگر (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        اگر(arrsz)
        {
            arrsz[0] = صحیح(n);
        }
    }
    وگرنه
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        اگر(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    بازگشت d;
}

منطقی درون_داد_Array_2::empty(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر (i >= 0) {
        اگر (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<ثابت std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            بازگشت mv->at(i).empty();
        }
        وگرنه اگر (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<ثابت std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            بازگشت umv->at(i).empty();
        }
        وگرنه اگر (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            بازگشت n == 0;
        } وگرنه {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    بازگشت empty();
}

MatShape درون_داد_Array_2::shape(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    MatShape shape;

    اگر( k == NONE )
        ;
    وگرنه اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        shape = m.size;
    }
    وگرنه اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        shape = m.size;
    }
    وگرنه اگر( k == STD_VECTOR_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    وگرنه اگر( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.height );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    وگرنه اگر( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت UMat& m = vv[i];
        shape = m.size;
    }
    وگرنه
    {
        صحیح sizes[CV_MAX_DIM];
        صحیح dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    بازگشت shape;
}

منطقی درون_داد_Array_2::sameSize(ثابت درون_داد_Array_2& arr) ثابت
{
    درون_داد_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    اگر( k1 == MAT )
    {
        ثابت Mat* m = ((ثابت Mat*)obj);
        اگر( k2 == MAT )
            بازگشت m->size == ((ثابت Mat*)arr.obj)->size;
        اگر( k2 == UMAT )
            بازگشت m->size == ((ثابت UMat*)arr.obj)->size;
        اگر( m->dims > 2 )
            بازگشت نادرست;
        sz1 = m->size();
    }
    وگرنه اگر( k1 == UMAT )
    {
        ثابت UMat* m = ((ثابت UMat*)obj);
        اگر( k2 == MAT )
            بازگشت m->size == ((ثابت Mat*)arr.obj)->size;
        اگر( k2 == UMAT )
            بازگشت m->size == ((ثابت UMat*)arr.obj)->size;
        اگر( m->dims > 2 )
            بازگشت نادرست;
        sz1 = m->size();
    }
    وگرنه اگر ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        بازگشت ((ثابت cuda::GpuMatND*)obj)->size == ((ثابت cuda::GpuMatND*)arr.obj)->size;
    }
    وگرنه
        sz1 = size();

    اگر( arr.dims() > 2 )
        بازگشت نادرست;
    بازگشت sz1 == arr.size();
}

صحیح درون_داد_Array_2::dims_(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت Mat*)obj)->dims;
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت UMat*)obj)->dims;
    }

    اگر (k == MATX)
    {
        CV_Assert( i < 0 );
        بازگشت 2;
    }

    اگر( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        بازگشت 1;
    }

    اگر( k == NONE )
        بازگشت 0;

    اگر( k == STD_VECTOR_VECTOR )
    {
        اگر( i < 0 )
            بازگشت 1;
        CV_Assert( size_t(i) < total() );
        بازگشت 1;
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( i < 0 )
            بازگشت 1;
        CV_Assert( i < (صحیح)vv.size() );

        بازگشت vv[i].dims;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( i < 0 )
            بازگشت 1;
        CV_Assert( i < sz.height );

        بازگشت vv[i].dims;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( i < 0 )
            بازگشت 1;
        CV_Assert( i < (صحیح)vv.size() );

        بازگشت vv[i].dims;
    }

    اگر( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        بازگشت 2;
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        بازگشت 2;
    }

    اگر( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت cuda::GpuMatND*)obj)->dims;
    }

    اگر( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        بازگشت 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t درون_داد_Array_2::total(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت Mat*)obj)->total();
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت UMat*)obj)->total();
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( i < 0 )
            بازگشت vv.size();

        CV_Assert( i < (صحیح)vv.size() );
        بازگشت vv[i].total();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( i < 0 )
            بازگشت sz.height;

        CV_Assert( i < sz.height );
        بازگشت vv[i].total();
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( i < 0 )
            بازگشت vv.size();

        CV_Assert( i < (صحیح)vv.size() );
        بازگشت vv[i].total();
    }

    اگر( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ثابت cuda::GpuMatND& _gpuMatND = *((ثابت cuda::GpuMatND*)obj);
        اگر (_gpuMatND.dims > 0)
        {
            res = 1;
            برای(صحیح d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            بازگشت res;
        }
    }

    اگر (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        صحیح esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            اگر (k == STD_VECTOR_VECTOR) { \
                ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                اگر (i < 0) \
                    بازگشت n; \
                CV_Assert((size_t)i < n); \
                بازگشت vv->at(i).size(); \
            } وگرنه \
                بازگشت ((ثابت std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    بازگشت size(i).area();
}

صحیح درون_داد_Array_2::type_(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
        بازگشت ((ثابت Mat*)obj)->type();

    اگر( k == UMAT )
        بازگشت ((ثابت UMat*)obj)->type();

    اگر( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        بازگشت CV_MAT_TYPE(flags);

    اگر( k == NONE )
        بازگشت -1;

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        اگر( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            بازگشت CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحیح)vv.size() );
        بازگشت vv[i >= 0 ? i : 0].type();
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        اگر( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            بازگشت CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحیح)vv.size() );
        بازگشت vv[i >= 0 ? i : 0].type();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        اگر( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            بازگشت CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        بازگشت vv[i >= 0 ? i : 0].type();
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        اگر (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            بازگشت CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحیح)vv.size());
        بازگشت vv[i >= 0 ? i : 0].type();
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        اگر (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            بازگشت CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحیح)vv.size());
        بازگشت vv[i >= 0 ? i : 0].type();
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر( k == OPENGL_BUFFER )
        بازگشت ((ثابت ogl::Buffer*)obj)->type();

    اگر( k == CUDA_GPU_MAT )
        بازگشت ((ثابت cuda::GpuMat*)obj)->type();

    اگر( k == CUDA_GPU_MATND )
        بازگشت ((ثابت cuda::GpuMatND*)obj)->type();

    اگر( k == CUDA_HOST_MEM )
        بازگشت ((ثابت cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحیح درون_داد_Array_2::depth_(صحیح i) ثابت
{
    بازگشت CV_MAT_DEPTH(type(i));
}

منطقی درون_داد_Array_2::empty_() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
        بازگشت ((ثابت Mat*)obj)->empty();

    اگر( k == UMAT )
        بازگشت ((ثابت UMat*)obj)->empty();

    اگر (k == MATX)
        بازگشت نادرست;

    اگر( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        بازگشت n == 0;
    }

    اگر( k == NONE )
        بازگشت درست;

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        بازگشت vv.empty();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        بازگشت sz.height == 0;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        بازگشت vv.empty();
    }

    اگر( k == OPENGL_BUFFER )
        بازگشت ((ثابت ogl::Buffer*)obj)->empty();

    اگر( k == CUDA_GPU_MAT )
        بازگشت ((ثابت cuda::GpuMat*)obj)->empty();

    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        بازگشت vv.empty();
    }

    اگر( k == CUDA_GPU_MATND )
        بازگشت ((ثابت cuda::GpuMatND*)obj)->empty();

    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        بازگشت vv.empty();
    }

    اگر( k == CUDA_HOST_MEM )
        بازگشت ((ثابت cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقی درون_داد_Array_2::isContinuous(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
        بازگشت i < 0 ? ((ثابت Mat*)obj)->isContinuous() : درست;

    اگر( k == UMAT )
        بازگشت i < 0 ? ((ثابت UMat*)obj)->isContinuous() : درست;

    اگر( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        بازگشت درست;

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].isContinuous();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        بازگشت vv[i].isContinuous();
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].isContinuous();
    }

    اگر( k == CUDA_GPU_MAT )
      بازگشت i < 0 ? ((ثابت cuda::GpuMat*)obj)->isContinuous() : درست;

    اگر( k == CUDA_GPU_MATND )
        بازگشت i < 0 ? ((ثابت cuda::GpuMatND*)obj)->isContinuous() : درست;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقی درون_داد_Array_2::isSubmatrix(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
        بازگشت i < 0 ? ((ثابت Mat*)obj)->isSubmatrix() : نادرست;

    اگر( k == UMAT )
        بازگشت i < 0 ? ((ثابت UMat*)obj)->isSubmatrix() : نادرست;

    اگر( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        بازگشت نادرست;

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].isSubmatrix();
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        بازگشت vv[i].isSubmatrix();
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].isSubmatrix();
    }

    اگر( k == CUDA_GPU_MATND )
    {
        بازگشت ((ثابت cuda::GpuMatND*)obj)->isSubmatrix();
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t درون_داد_Array_2::offset(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat * ثابت m = ((ثابت Mat*)obj);
        بازگشت (size_t)(m->ptr() - m->datastart);
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت UMat*)obj)->offset;
    }

    اگر( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        بازگشت 0;

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحیح)vv.size() );

        بازگشت (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        بازگشت (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].offset;
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat * ثابت m = ((ثابت cuda::GpuMat*)obj);
        بازگشت (size_t)(m->data - m->datastart);
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t درون_داد_Array_2::step(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت Mat*)obj)->step;
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت UMat*)obj)->step;
    }

    اگر( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        بازگشت 0;

    اگر( k == STD_VECTOR_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحیح)vv.size() );
        بازگشت vv[i].step;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        بازگشت vv[i].step;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].step;
    }

    اگر( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        بازگشت ((ثابت cuda::GpuMat*)obj)->step;
    }
    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        بازگشت vv[i].step;
    }
    اگر( k == CUDA_GPU_MATND )
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        بازگشت _gpuMatND.step[i];
    }

    اگر (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ثابت cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        بازگشت _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

تهی درون_داد_Array_2::copyTo(ثابت _OutputArray& arr) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == NONE )
        arr.release();
    وگرنه اگر( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    وگرنه اگر( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    وگرنه اگر (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    وگرنه
        CV_Error(Error::StsNotImplemented, "");
}

تهی درون_داد_Array_2::copyTo(ثابت _OutputArray& arr, ثابت درون_داد_Array_2 & mask) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == NONE )
        arr.release();
    وگرنه اگر( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    وگرنه اگر( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    وگرنه اگر (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    وگرنه
        CV_Error(Error::StsNotImplemented, "");
}

منطقی _OutputArray::fixedSize() ثابت
{
    بازگشت (flags & FIXED_SIZE) == FIXED_SIZE;
}

منطقی _OutputArray::fixedType() ثابت
{
    بازگشت (flags & FIXED_TYPE) == FIXED_TYPE;
}

تهی _OutputArray::create_(Size _sz, صحیح mtype, صحیح i, منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        بازگشت;
    }
    اگر( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        بازگشت;
    }
    اگر( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    اگر( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    اگر( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    اگر( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    صحیح sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

تهی _OutputArray::create_(صحیح _rows, صحیح _cols, صحیح mtype, صحیح i, منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        بازگشت;
    }
    اگر( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        بازگشت;
    }
    اگر( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    اگر( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    اگر( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    اگر( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    صحیح sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

تهی _OutputArray::create(صحیح d, ثابت صحیح* sizes, صحیح mtype, صحیح i,
                          منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحیح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    درون_داد_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    اگر( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            وگرنه
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            برای(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        بازگشت;
    }

    اگر( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            وگرنه
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            برای(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        بازگشت;
    }

    اگر( k == MATX )
    {
        CV_Assert( i < 0 );
        صحیح type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        اگر (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            صحیح total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        وگرنه
        {
            اگر (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            وگرنه
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        بازگشت;
    }

    اگر( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        صحیح esz = CV_ELEM_SIZE(flags);

        اگر( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            صحیح type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            اگر (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                اگر (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    بازگشت; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } وگرنه { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        بشکن

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        بازگشت;
    }

    اگر( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    اگر( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        اگر( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            اگر( fixedType() )
            {
                صحیح _type = CV_MAT_TYPE(flags);
                برای( size_t j = len0; j < len; j++ )
                {
                    اگر( v[j].type() == _type )
                        ادامه;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            بازگشت;
        }

        CV_Assert( i < (صحیح)v.size() );
        Mat& m = v[i];

        اگر( allowTransposed )
        {
            اگر( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            اگر( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            وگرنه
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        اگر(fixedSize())
        {
            CV_Assert(m.dims == d);
            برای(صحیح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        بازگشت;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        اگر( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            اگر( fixedType() )
            {
                صحیح _type = CV_MAT_TYPE(flags);
                برای( size_t j = len0; j < len; j++ )
                {
                    اگر( v[j].type() == _type )
                        ادامه;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            بازگشت;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        اگر( allowTransposed )
        {
            اگر( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            اگر( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            وگرنه
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        اگر(fixedSize())
        {
            CV_Assert(m.dims == d);
            برای(صحیح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        بازگشت;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        اگر( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            اگر( fixedType() )
            {
                صحیح _type = CV_MAT_TYPE(flags);
                برای( size_t j = len0; j < len; j++ )
                {
                    اگر( v[j].type() == _type )
                        ادامه;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            بازگشت;
        }

        CV_Assert( i < (صحیح)v.size() );
        UMat& m = v[i];

        اگر( allowTransposed )
        {
            اگر( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            اگر( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            وگرنه
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        اگر(fixedSize())
        {
            CV_Assert(m.dims == d);
            برای(صحیح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        بازگشت;
    }

    اگر ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        بازگشت;
    }

    اگر( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

تهی _OutputArray::create(ثابت MatShape& shape, صحیح mtype, صحیح i,
                          منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    اگر (shape.dims < 0) {
        release();
    } وگرنه {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(صحیح mtype) ثابت
{
    mtype = CV_MAT_TYPE(mtype);
    بازگشت getMat().reinterpret(mtype);
}

تهی _OutputArray::createSameSize(ثابت درون_داد_Array_2& arr, صحیح mtype) ثابت
{
    create(arr.shape(), mtype);
}

تهی _OutputArray::fit(Size _sz, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحیح sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

تهی _OutputArray::fit(صحیح rows, صحیح cols, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحیح sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

تهی _OutputArray::fit(ثابت MatShape& shape, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    اگر (shape.dims < 0) {
        release();
        بازگشت;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    درون_داد_Array_2::KindFlag k = kind();
    اگر ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } وگرنه اگر ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

تهی _OutputArray::fitSameSize(ثابت درون_داد_Array_2& arr, صحیح mtype) ثابت
{
    fit(arr.shape(), mtype);
}

تهی _OutputArray::fit(صحیح d, ثابت صحیح* sizes, صحیح mtype, صحیح i,
                       منطقی allowTransposed, _OutputArray::DepthMask fixedDepthMask) ثابت
{
    صحیح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    درون_داد_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    اگر( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        اگر (k == MAT)
            m = (Mat*)obj;
        وگرنه {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            وگرنه
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            برای(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        بازگشت;
    }

    اگر( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        اگر (k == UMAT)
            m = (UMat*)obj;
        وگرنه {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        اگر (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            بازگشت;
        }

        اگر(fixedType())
        {
            اگر(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            وگرنه
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        اگر(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            برای(صحیح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        بازگشت;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

تهی _OutputArray::release() ثابت
{
    CV_Assert(!fixedSize());

    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        ((Mat*)obj)->release();
        بازگشت;
    }

    اگر( k == UMAT )
    {
        ((UMat*)obj)->release();
        بازگشت;
    }

    اگر( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    اگر( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    اگر( k == NONE )
        بازگشت;

    اگر( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        اگر (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } وگرنه { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        بشکن

        صحیح esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        بازگشت;
    }

    اگر( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        بازگشت;
    }

    اگر( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        برای( صحیح i = 0; i < sz.height; i++ )
            v[i].release();
        بازگشت;
    }

    اگر( k == STD_BOOL_VECTOR )
    {
        ((std::vector<منطقی>*)obj)->clear();
        بازگشت;
    }

    اگر( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        بازگشت;
    }
    اگر (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        بازگشت;
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

تهی _OutputArray::clear() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        بازگشت;
    }

    release();
}

منطقی _OutputArray::needed() ثابت
{
    بازگشت kind() != NONE;
}

Mat& _OutputArray::getMatRef(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر( i < 0 )
    {
        CV_Assert( k == MAT );
        بازگشت *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    اگر( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (صحیح)v.size() );
        بازگشت v[i];
    }
    وگرنه
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        بازگشت v[i];
    }
}

UMat& _OutputArray::getUMatRef(صحیح i) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر( i < 0 )
    {
        CV_Assert( k == UMAT );
        بازگشت *(UMat*)obj;
    }
    وگرنه
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (صحیح)v.size() );
        بازگشت v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    بازگشت *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    بازگشت *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    بازگشت *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    بازگشت *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    بازگشت *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    بازگشت *(cuda::HostMem*)obj;
}

تهی _OutputArray::setTo(ثابت درون_داد_Array_2& arr, ثابت درون_داد_Array_2 & mask) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == NONE )
        ;
    وگرنه اگر (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    وگرنه اگر( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    وگرنه اگر( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), درون_داد_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<دوگانه, 4>(value.ptr<دوگانه>())), mask);
#وگرنه
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    وگرنه
        CV_Error(Error::StsNotImplemented, "");
}

تهی _OutputArray::setZero() ثابت
{
    درون_داد_Array_2::KindFlag k = kind();

    اگر( k == NONE )
        ;
    وگرنه اگر (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    وگرنه
    {
        setTo(Scalar::all(0), noArray());
    }
}

تهی _OutputArray::assign(ثابت UMat& u) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    وگرنه اگر (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    وگرنه اگر (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    وگرنه
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


تهی _OutputArray::assign(ثابت Mat& m) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    وگرنه اگر (k == MAT)
    {
        *(Mat*)obj = m;
    }
    وگرنه اگر (k == MATX)
    {
        m.copyTo(getMat());
    }
    وگرنه
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


تهی _OutputArray::move(UMat& u) ثابت
{
    اگر (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        بازگشت;
    }
    صحیح k = kind();
    اگر (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    وگرنه اگر (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    وگرنه اگر (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    وگرنه
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


تهی _OutputArray::move(Mat& m) ثابت
{
    اگر (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        بازگشت;
    }
    صحیح k = kind();
    اگر (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    وگرنه اگر (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    وگرنه اگر (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    وگرنه
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


تهی _OutputArray::assign(ثابت std::vector<UMat>& v) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برای (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            UMat& this_m = this_v[i];
            اگر (this_m.u != پوچ && this_m.u == m.u)
                ادامه; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    وگرنه اگر (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برای (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            Mat& this_m = this_v[i];
            اگر (this_m.u != پوچ && this_m.u == m.u)
                ادامه; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    وگرنه
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


تهی _OutputArray::assign(ثابت std::vector<Mat>& v) ثابت
{
    درون_داد_Array_2::KindFlag k = kind();
    اگر (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برای (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            UMat& this_m = this_v[i];
            اگر (this_m.u != پوچ && this_m.u == m.u)
                ادامه; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    وگرنه اگر (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        برای (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            Mat& this_m = this_v[i];
            اگر (this_m.u != پوچ && this_m.u == m.u)
                ادامه; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    وگرنه
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ایستا درون_داد_Output_Array_2 _none;
درون_داد_Output_Array noArray() { بازگشت _none; }

} // cv::
