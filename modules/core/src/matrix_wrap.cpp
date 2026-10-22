// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"
#அந்தர்பாவ "opencv2/core/mat.hpp"

நாமாகாஶ cv {

வகைவரை Vec<முழு, 5> Vec5i;
வகைவரை Vec<முழு, 7> Vec7i;
வகைவரை Vec<முழு, 9> Vec9i;
வகைவரை Vec<முழு, 10> Vec10i;
வகைவரை Vec<முழு, 11> Vec11i;
வகைவரை Vec<முழு, 12> Vec12i;
வகைவரை Vec<முழு, 13> Vec13i;
வகைவரை Vec<முழு, 14> Vec14i;
வகைவரை Vec<முழு, 15> Vec15i;
வகைவரை Vec<முழு, 16> Vec16i;
வகைவரை Vec<முழு, 32> Vec32i;
வகைவரை Vec<முழு, 64> Vec64i;
வகைவரை Vec<முழு, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    தேர்வு( esz ) \
    { \
    நிலை 1: func(uchar); \
    நிலை 2: func(Vec2b); \
    நிலை 3: func(Vec3b); \
    நிலை 4: func(முழு); \
    நிலை 6: func(Vec3s); \
    நிலை 8: func(Vec2i); \
    நிலை 12: func(Vec3i); \
    நிலை 16: func(Vec4i); \
    நிலை 20: func(Vec5i); \
    நிலை 24: func(Vec6i); \
    நிலை 28: func(Vec7i); \
    நிலை 32: func(Vec8i); \
    நிலை 36: func(Vec9i); \
    நிலை 40: func(Vec10i); \
    நிலை 44: func(Vec11i); \
    நிலை 48: func(Vec12i); \
    நிலை 52: func(Vec13i); \
    நிலை 56: func(Vec14i); \
    நிலை 60: func(Vec15i); \
    நிலை 64: func(Vec16i); \
    நிலை 128: func(Vec32i); \
    நிலை 256: func(Vec64i); \
    நிலை 512: func(Vec128i); \
    இயல்பு: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat உள்ளீடு_Array_2::getMat_(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    எனில்( k == MAT )
    {
        மாறிலி Mat* m = (மாறிலி Mat*)obj;
        எனில்( i < 0 )
            திரும்பு *m;
        திரும்பு m->row(i);
    }

    எனில்( k == UMAT )
    {
        மாறிலி UMat* m = (மாறிலி UMat*)obj;
        எனில்( i < 0 )
            திரும்பு m->getMat(accessFlags);
        திரும்பு m->getMat(accessFlags).row(i);
    }

    எனில் (k == MATX)
    {
        CV_Assert( i < 0 );
        திரும்பு Mat(sz, flags, obj);
    }

    எனில்( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        முழு t = CV_8U;
        மாறிலி std::vector<தருக்க>& v = *(மாறிலி std::vector<தருக்க>*)obj;
        முழு j, n = (முழு)v.size();
        எனில்( n == 0 )
            திரும்பு Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        க்கு( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        திரும்பு m;
    }

    எனில்( k == NONE )
        திரும்பு Mat();

    எனில்( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        முழு t = CV_MAT_TYPE(flags);
        முழு esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            எனில் (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } இல்லையெனில் { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            முழு v_sz = முழு(v->size()); \
            திரும்பு v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& v = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (முழு)v.size() );

        திரும்பு v[i];
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* v = (மாறிலி Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        திரும்பு v[i];
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& v = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (முழு)v.size() );

        திரும்பு v[i].getMat(accessFlags);
    }

    எனில்( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    எனில்( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    எனில்( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    எனில்( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        மாறிலி cuda::HostMem* cuda_mem = (மாறிலி cuda::HostMem*)obj;

        திரும்பு cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat உள்ளீடு_Array_2::getUMat(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    எனில்( k == UMAT )
    {
        மாறிலி UMat* m = (மாறிலி UMat*)obj;
        எனில்( i < 0 )
            திரும்பு *m;
        திரும்பு m->row(i);
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& v = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (முழு)v.size() );

        திரும்பு v[i];
    }

    எனில்( k == MAT )
    {
        மாறிலி Mat* m = (மாறிலி Mat*)obj;
        எனில்( i < 0 )
            திரும்பு m->getUMat(accessFlags);
        திரும்பு m->row(i).getUMat(accessFlags);
    }

    திரும்பு getMat(i).getUMat(accessFlags);
}

வெறுமை உள்ளீடு_Array_2::getMatVector(std::vector<Mat>& mv) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    எனில்( k == MAT )
    {
        மாறிலி Mat& m = *(மாறிலி Mat*)obj;
        முழு n = (முழு)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        க்கு( முழு i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (வெறுமை*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (வெறுமை*)m.ptr(i), &m.step[1]);
        திரும்பு;
    }

    எனில் (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        திரும்பு;
    }

    எனில்( k == STD_VECTOR )
    {
        மாறிலி std::vector<uchar>& v = *(மாறிலி std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        முழு t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (வெறுமை*)(v.data() + esz*i));
        திரும்பு;
    }

    எனில்( k == NONE )
    {
        mv.clear();
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_VECTOR )
    {
        முழு typ = CV_MAT_TYPE(flags);
        முழு esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            மாறிலி std::vector<std::vector<T > >* vv = (மாறிலி std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            க்கு (size_t i = 0; i < n; i++) { \
                மாறிலி std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                முழு ni = (முழு)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (வெறுமை*)vi.data()) : Mat(); \
            } \
        } \
        நிறுத்து

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& v = *(மாறிலி std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        திரும்பு;
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* v = (மாறிலி Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& v = *(மாறிலி std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        திரும்பு;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

வெறுமை உள்ளீடு_Array_2::getUMatVector(std::vector<UMat>& umv) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    எனில்( k == NONE )
    {
        umv.clear();
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& v = *(மாறிலி std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        திரும்பு;
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* v = (மாறிலி Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& v = *(மாறிலி std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        க்கு( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        திரும்பு;
    }

    எனில்( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        திரும்பு;
    }
    எனில்( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        திரும்பு;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat உள்ளீடு_Array_2::getGpuMat() மாறிலி
{
#ifdef HAVE_CUDA
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில் (k == CUDA_GPU_MAT)
    {
        மாறிலி cuda::GpuMat* d_mat = (மாறிலி cuda::GpuMat*)obj;
        திரும்பு *d_mat;
    }

    எனில் (k == CUDA_HOST_MEM)
    {
        மாறிலி cuda::HostMem* cuda_mem = (மாறிலி cuda::HostMem*)obj;
        திரும்பு cuda_mem->createGpuMatHeader();
    }

    எனில் (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    எனில் (k == NONE)
        திரும்பு cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#இல்லையெனில்
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
வெறுமை உள்ளீடு_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) மாறிலி
{
#ifdef HAVE_CUDA
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#இல்லையெனில்
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
வெறுமை உள்ளீடு_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) மாறிலி
{
#ifdef HAVE_CUDA
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#இல்லையெனில்
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND உள்ளீடு_Array_2::getGpuMatND() மாறிலி
{
#ifdef HAVE_CUDA
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில் (k == CUDA_GPU_MATND)
    {
        மாறிலி cuda::GpuMatND* d_mat = (மாறிலி cuda::GpuMatND*)obj;
        திரும்பு *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#இல்லையெனில்
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer உள்ளீடு_Array_2::getOGlBuffer() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    மாறிலி ogl::Buffer* gl_buf = (மாறிலி ogl::Buffer*)obj;
    திரும்பு *gl_buf;
}

முழு உள்ளீடு_Array_2::rows_(முழு i) மாறிலி
{
#ifdef HAVE_CUDA
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == CUDA_GPU_MATND)
    {
        மாறிலி cuda::GpuMatND& _gpuMatND = *(மாறிலி cuda::GpuMatND*)obj;
        திரும்பு (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    திரும்பு size_(i).height;
}

முழு உள்ளீடு_Array_2::cols_(முழு i) மாறிலி
{
#ifdef HAVE_CUDA
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == CUDA_GPU_MATND)
    {
        மாறிலி cuda::GpuMatND& _gpuMatND = *(மாறிலி cuda::GpuMatND*)obj;
        திரும்பு (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    திரும்பு size_(i).width;
}

Size உள்ளீடு_Array_2::size_(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        மாறிலி Mat* m = (மாறிலி Mat*)obj;
        CV_Assert(m->dims <= 2);
        திரும்பு Size(m->cols, m->rows);
    }

    எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        மாறிலி UMat* m = (மாறிலி UMat*)obj;
        CV_Assert(m->dims <= 2);
        திரும்பு Size(m->cols, m->rows);
    }

    எனில் (k == MATX)
    {
        CV_Assert( i < 0 );
        திரும்பு sz;
    }

    எனில்( k == NONE )
        திரும்பு Size();

    எனில்( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        திரும்பு Size((முழு)n, 1);
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        எனில்( i < 0 )
            திரும்பு vv.empty() ? Size() : Size((முழு)vv.size(), 1);
        CV_Assert( i < (முழு)vv.size() );

        திரும்பு vv[i].size();
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        எனில்( i < 0 )
            திரும்பு sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        திரும்பு vv[i].size();
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        மாறிலி std::vector<cuda::GpuMat>& vv = *(மாறிலி std::vector<cuda::GpuMat>*)obj;
        எனில் (i < 0)
            திரும்பு vv.empty() ? Size() : Size((முழு)vv.size(), 1);
        CV_Assert(i < (முழு)vv.size());
        திரும்பு vv[i].size();
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        மாறிலி std::vector<cuda::GpuMatND>& vv = *(மாறிலி std::vector<cuda::GpuMatND>*)obj;
        எனில் (i < 0)
            திரும்பு vv.empty() ? Size() : Size((முழு)vv.size(), 1);
        CV_Assert(i < (முழு)vv.size());
        மாறிலி cuda::GpuMatND& m = vv[i];
        எனில் (m.dims == 0)
            திரும்பு Size();
        எனில் (m.dims == 1)
            திரும்பு Size(1, m.size[0]);
        திரும்பு Size(m.size[1], m.size[0]);
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        எனில்( i < 0 )
            திரும்பு vv.empty() ? Size() : Size((முழு)vv.size(), 1);
        CV_Assert( i < (முழு)vv.size() );

        திரும்பு vv[i].size();
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        மாறிலி std::vector<cuda::GpuMatND>& vv = *(மாறிலி std::vector<cuda::GpuMatND>*)obj;
        எனில் (i < 0)
            திரும்பு vv.empty() ? Size() : Size((முழு)vv.size(), 1);
        CV_Assert(i < (முழு)vv.size());
        மாறிலி cuda::GpuMatND& m = vv[i];
        எனில் (m.dims == 0)
            திரும்பு Size();
        எனில் (m.dims == 1)
            திரும்பு Size(1, m.size[0]);
        திரும்பு Size(m.size[1], m.size[0]);
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில்( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        மாறிலி ogl::Buffer* buf = (மாறிலி ogl::Buffer*)obj;
        திரும்பு buf->size();
    }

    எனில்( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        மாறிலி cuda::GpuMat* d_mat = (மாறிலி cuda::GpuMat*)obj;
        திரும்பு d_mat->size();
    }

    எனில்( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        மாறிலி cuda::HostMem* cuda_mem = (மாறிலி cuda::HostMem*)obj;
        திரும்பு cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

முழு உள்ளீடு_Array_2::sizend(முழு* arrsz, முழு i) மாறிலி
{
    முழு j, d = 0;
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == NONE )
        ;
    இல்லையெனில் எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        மாறிலி Mat& m = *(மாறிலி Mat*)obj;
        d = m.dims;
        எனில்(arrsz)
            க்கு(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    இல்லையெனில் எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        மாறிலி UMat& m = *(மாறிலி UMat*)obj;
        d = m.dims;
        எனில்(arrsz)
            க்கு(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    இல்லையெனில் எனில்( k == STD_VECTOR_MAT && i >= 0 )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert( i < (முழு)vv.size() );
        மாறிலி Mat& m = vv[i];
        d = m.dims;
        எனில்(arrsz)
            க்கு(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    இல்லையெனில் எனில்( k == STD_ARRAY_MAT && i >= 0 )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        CV_Assert( i < sz.height );
        மாறிலி Mat& m = vv[i];
        d = m.dims;
        எனில்(arrsz)
            க்கு(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    இல்லையெனில் எனில்( k == STD_VECTOR_UMAT && i >= 0 )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert( i < (முழு)vv.size() );
        மாறிலி UMat& m = vv[i];
        d = m.dims;
        எனில்(arrsz)
            க்கு(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    இல்லையெனில் எனில் (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        எனில்(arrsz)
        {
            arrsz[0] = முழு(n);
        }
    }
    இல்லையெனில்
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        எனில்(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    திரும்பு d;
}

தருக்க உள்ளீடு_Array_2::empty(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (i >= 0) {
        எனில் (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<மாறிலி std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            திரும்பு mv->at(i).empty();
        }
        இல்லையெனில் எனில் (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<மாறிலி std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            திரும்பு umv->at(i).empty();
        }
        இல்லையெனில் எனில் (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            திரும்பு n == 0;
        } இல்லையெனில் {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    திரும்பு empty();
}

MatShape உள்ளீடு_Array_2::shape(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    MatShape shape;

    எனில்( k == NONE )
        ;
    இல்லையெனில் எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        மாறிலி Mat& m = *(மாறிலி Mat*)obj;
        shape = m.size;
    }
    இல்லையெனில் எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        மாறிலி UMat& m = *(மாறிலி UMat*)obj;
        shape = m.size;
    }
    இல்லையெனில் எனில்( k == STD_VECTOR_MAT && i >= 0 )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        மாறிலி Mat& m = vv[i];
        shape = m.size;
    }
    இல்லையெனில் எனில்( k == STD_ARRAY_MAT && i >= 0 )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        CV_Assert( i < sz.height );
        மாறிலி Mat& m = vv[i];
        shape = m.size;
    }
    இல்லையெனில் எனில்( k == STD_VECTOR_UMAT && i >= 0 )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        மாறிலி UMat& m = vv[i];
        shape = m.size;
    }
    இல்லையெனில்
    {
        முழு sizes[CV_MAX_DIM];
        முழு dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    திரும்பு shape;
}

தருக்க உள்ளீடு_Array_2::sameSize(மாறிலி உள்ளீடு_Array_2& arr) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    எனில்( k1 == MAT )
    {
        மாறிலி Mat* m = ((மாறிலி Mat*)obj);
        எனில்( k2 == MAT )
            திரும்பு m->size == ((மாறிலி Mat*)arr.obj)->size;
        எனில்( k2 == UMAT )
            திரும்பு m->size == ((மாறிலி UMat*)arr.obj)->size;
        எனில்( m->dims > 2 )
            திரும்பு பொய்;
        sz1 = m->size();
    }
    இல்லையெனில் எனில்( k1 == UMAT )
    {
        மாறிலி UMat* m = ((மாறிலி UMat*)obj);
        எனில்( k2 == MAT )
            திரும்பு m->size == ((மாறிலி Mat*)arr.obj)->size;
        எனில்( k2 == UMAT )
            திரும்பு m->size == ((மாறிலி UMat*)arr.obj)->size;
        எனில்( m->dims > 2 )
            திரும்பு பொய்;
        sz1 = m->size();
    }
    இல்லையெனில் எனில் ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        திரும்பு ((மாறிலி cuda::GpuMatND*)obj)->size == ((மாறிலி cuda::GpuMatND*)arr.obj)->size;
    }
    இல்லையெனில்
        sz1 = size();

    எனில்( arr.dims() > 2 )
        திரும்பு பொய்;
    திரும்பு sz1 == arr.size();
}

முழு உள்ளீடு_Array_2::dims_(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி Mat*)obj)->dims;
    }

    எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி UMat*)obj)->dims;
    }

    எனில் (k == MATX)
    {
        CV_Assert( i < 0 );
        திரும்பு 2;
    }

    எனில்( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        திரும்பு 1;
    }

    எனில்( k == NONE )
        திரும்பு 0;

    எனில்( k == STD_VECTOR_VECTOR )
    {
        எனில்( i < 0 )
            திரும்பு 1;
        CV_Assert( size_t(i) < total() );
        திரும்பு 1;
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        எனில்( i < 0 )
            திரும்பு 1;
        CV_Assert( i < (முழு)vv.size() );

        திரும்பு vv[i].dims;
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        எனில்( i < 0 )
            திரும்பு 1;
        CV_Assert( i < sz.height );

        திரும்பு vv[i].dims;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        எனில்( i < 0 )
            திரும்பு 1;
        CV_Assert( i < (முழு)vv.size() );

        திரும்பு vv[i].dims;
    }

    எனில்( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        திரும்பு 2;
    }

    எனில்( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        திரும்பு 2;
    }

    எனில்( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி cuda::GpuMatND*)obj)->dims;
    }

    எனில்( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        திரும்பு 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t உள்ளீடு_Array_2::total(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி Mat*)obj)->total();
    }

    எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி UMat*)obj)->total();
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        எனில்( i < 0 )
            திரும்பு vv.size();

        CV_Assert( i < (முழு)vv.size() );
        திரும்பு vv[i].total();
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        எனில்( i < 0 )
            திரும்பு sz.height;

        CV_Assert( i < sz.height );
        திரும்பு vv[i].total();
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        எனில்( i < 0 )
            திரும்பு vv.size();

        CV_Assert( i < (முழு)vv.size() );
        திரும்பு vv[i].total();
    }

    எனில்( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        மாறிலி cuda::GpuMatND& _gpuMatND = *((மாறிலி cuda::GpuMatND*)obj);
        எனில் (_gpuMatND.dims > 0)
        {
            res = 1;
            க்கு(முழு d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            திரும்பு res;
        }
    }

    எனில் (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        முழு esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            எனில் (k == STD_VECTOR_VECTOR) { \
                மாறிலி std::vector<std::vector<T > >* vv = (மாறிலி std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                எனில் (i < 0) \
                    திரும்பு n; \
                CV_Assert((size_t)i < n); \
                திரும்பு vv->at(i).size(); \
            } இல்லையெனில் \
                திரும்பு ((மாறிலி std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    திரும்பு size(i).area();
}

முழு உள்ளீடு_Array_2::type_(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
        திரும்பு ((மாறிலி Mat*)obj)->type();

    எனில்( k == UMAT )
        திரும்பு ((மாறிலி UMat*)obj)->type();

    எனில்( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        திரும்பு CV_MAT_TYPE(flags);

    எனில்( k == NONE )
        திரும்பு -1;

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        எனில்( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            திரும்பு CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (முழு)vv.size() );
        திரும்பு vv[i >= 0 ? i : 0].type();
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        எனில்( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            திரும்பு CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (முழு)vv.size() );
        திரும்பு vv[i >= 0 ? i : 0].type();
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        எனில்( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            திரும்பு CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        திரும்பு vv[i >= 0 ? i : 0].type();
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        மாறிலி std::vector<cuda::GpuMat>& vv = *(மாறிலி std::vector<cuda::GpuMat>*)obj;
        எனில் (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            திரும்பு CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (முழு)vv.size());
        திரும்பு vv[i >= 0 ? i : 0].type();
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        மாறிலி std::vector<cuda::GpuMatND>& vv = *(மாறிலி std::vector<cuda::GpuMatND>*)obj;
        எனில் (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            திரும்பு CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (முழு)vv.size());
        திரும்பு vv[i >= 0 ? i : 0].type();
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில்( k == OPENGL_BUFFER )
        திரும்பு ((மாறிலி ogl::Buffer*)obj)->type();

    எனில்( k == CUDA_GPU_MAT )
        திரும்பு ((மாறிலி cuda::GpuMat*)obj)->type();

    எனில்( k == CUDA_GPU_MATND )
        திரும்பு ((மாறிலி cuda::GpuMatND*)obj)->type();

    எனில்( k == CUDA_HOST_MEM )
        திரும்பு ((மாறிலி cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

முழு உள்ளீடு_Array_2::depth_(முழு i) மாறிலி
{
    திரும்பு CV_MAT_DEPTH(type(i));
}

தருக்க உள்ளீடு_Array_2::empty_() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
        திரும்பு ((மாறிலி Mat*)obj)->empty();

    எனில்( k == UMAT )
        திரும்பு ((மாறிலி UMat*)obj)->empty();

    எனில் (k == MATX)
        திரும்பு பொய்;

    எனில்( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        திரும்பு n == 0;
    }

    எனில்( k == NONE )
        திரும்பு உண்மை;

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        திரும்பு vv.empty();
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        திரும்பு sz.height == 0;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        திரும்பு vv.empty();
    }

    எனில்( k == OPENGL_BUFFER )
        திரும்பு ((மாறிலி ogl::Buffer*)obj)->empty();

    எனில்( k == CUDA_GPU_MAT )
        திரும்பு ((மாறிலி cuda::GpuMat*)obj)->empty();

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        மாறிலி std::vector<cuda::GpuMat>& vv = *(மாறிலி std::vector<cuda::GpuMat>*)obj;
        திரும்பு vv.empty();
    }

    எனில்( k == CUDA_GPU_MATND )
        திரும்பு ((மாறிலி cuda::GpuMatND*)obj)->empty();

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        மாறிலி std::vector<cuda::GpuMatND>& vv = *(மாறிலி std::vector<cuda::GpuMatND>*)obj;
        திரும்பு vv.empty();
    }

    எனில்( k == CUDA_HOST_MEM )
        திரும்பு ((மாறிலி cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

தருக்க உள்ளீடு_Array_2::isContinuous(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
        திரும்பு i < 0 ? ((மாறிலி Mat*)obj)->isContinuous() : உண்மை;

    எனில்( k == UMAT )
        திரும்பு i < 0 ? ((மாறிலி UMat*)obj)->isContinuous() : உண்மை;

    எனில்( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        திரும்பு உண்மை;

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].isContinuous();
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        திரும்பு vv[i].isContinuous();
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].isContinuous();
    }

    எனில்( k == CUDA_GPU_MAT )
      திரும்பு i < 0 ? ((மாறிலி cuda::GpuMat*)obj)->isContinuous() : உண்மை;

    எனில்( k == CUDA_GPU_MATND )
        திரும்பு i < 0 ? ((மாறிலி cuda::GpuMatND*)obj)->isContinuous() : உண்மை;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

தருக்க உள்ளீடு_Array_2::isSubmatrix(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
        திரும்பு i < 0 ? ((மாறிலி Mat*)obj)->isSubmatrix() : பொய்;

    எனில்( k == UMAT )
        திரும்பு i < 0 ? ((மாறிலி UMat*)obj)->isSubmatrix() : பொய்;

    எனில்( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        திரும்பு பொய்;

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].isSubmatrix();
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        திரும்பு vv[i].isSubmatrix();
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].isSubmatrix();
    }

    எனில்( k == CUDA_GPU_MATND )
    {
        திரும்பு ((மாறிலி cuda::GpuMatND*)obj)->isSubmatrix();
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        மாறிலி std::vector<cuda::GpuMatND>& vv = *(மாறிலி std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t உள்ளீடு_Array_2::offset(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        மாறிலி Mat * மாறிலி m = ((மாறிலி Mat*)obj);
        திரும்பு (size_t)(m->ptr() - m->datastart);
    }

    எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி UMat*)obj)->offset;
    }

    எனில்( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        திரும்பு 0;

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (முழு)vv.size() );

        திரும்பு (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        திரும்பு (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].offset;
    }

    எனில்( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        மாறிலி cuda::GpuMat * மாறிலி m = ((மாறிலி cuda::GpuMat*)obj);
        திரும்பு (size_t)(m->data - m->datastart);
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        மாறிலி std::vector<cuda::GpuMat>& vv = *(மாறிலி std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t உள்ளீடு_Array_2::step(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி Mat*)obj)->step;
    }

    எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி UMat*)obj)->step;
    }

    எனில்( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        திரும்பு 0;

    எனில்( k == STD_VECTOR_MAT )
    {
        மாறிலி std::vector<Mat>& vv = *(மாறிலி std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (முழு)vv.size() );
        திரும்பு vv[i].step;
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        மாறிலி Mat* vv = (மாறிலி Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        திரும்பு vv[i].step;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        மாறிலி std::vector<UMat>& vv = *(மாறிலி std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].step;
    }

    எனில்( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        திரும்பு ((மாறிலி cuda::GpuMat*)obj)->step;
    }
    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        மாறிலி std::vector<cuda::GpuMat>& vv = *(மாறிலி std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        திரும்பு vv[i].step;
    }
    எனில்( k == CUDA_GPU_MATND )
    {
        மாறிலி cuda::GpuMatND& _gpuMatND = *(மாறிலி cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        திரும்பு _gpuMatND.step[i];
    }

    எனில் (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        மாறிலி std::vector<cuda::GpuMatND>& vv = *(மாறிலி std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        மாறிலி cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        திரும்பு _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

வெறுமை உள்ளீடு_Array_2::copyTo(மாறிலி _OutputArray& arr) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == NONE )
        arr.release();
    இல்லையெனில் எனில்( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    இல்லையெனில் எனில்( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    இல்லையெனில் எனில் (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "");
}

வெறுமை உள்ளீடு_Array_2::copyTo(மாறிலி _OutputArray& arr, மாறிலி உள்ளீடு_Array_2 & mask) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == NONE )
        arr.release();
    இல்லையெனில் எனில்( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    இல்லையெனில் எனில்( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    இல்லையெனில் எனில் (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "");
}

தருக்க _OutputArray::fixedSize() மாறிலி
{
    திரும்பு (flags & FIXED_SIZE) == FIXED_SIZE;
}

தருக்க _OutputArray::fixedType() மாறிலி
{
    திரும்பு (flags & FIXED_TYPE) == FIXED_TYPE;
}

வெறுமை _OutputArray::create_(Size _sz, முழு mtype, முழு i, தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில்( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        திரும்பு;
    }
    எனில்( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        திரும்பு;
    }
    எனில்( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    எனில்( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    எனில்( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    எனில்( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    முழு sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

வெறுமை _OutputArray::create_(முழு _rows, முழு _cols, முழு mtype, முழு i, தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில்( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        திரும்பு;
    }
    எனில்( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        திரும்பு;
    }
    எனில்( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    எனில்( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    எனில்( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    எனில்( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    முழு sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

வெறுமை _OutputArray::create(முழு d, மாறிலி முழு* sizes, முழு mtype, முழு i,
                          தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    முழு size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    உள்ளீடு_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    எனில்( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        எனில் (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            இல்லையெனில்
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        எனில்(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            க்கு(முழு j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        திரும்பு;
    }

    எனில்( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        எனில் (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            இல்லையெனில்
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        எனில்(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            க்கு(முழு j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        திரும்பு;
    }

    எனில்( k == MATX )
    {
        CV_Assert( i < 0 );
        முழு type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        எனில் (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            முழு total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        இல்லையெனில்
        {
            எனில் (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            இல்லையெனில்
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        திரும்பு;
    }

    எனில்( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        முழு esz = CV_ELEM_SIZE(flags);

        எனில்( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            முழு type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            எனில் (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                எனில் (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    திரும்பு; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } இல்லையெனில் { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        நிறுத்து

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        திரும்பு;
    }

    எனில்( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        எனில்( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            எனில்( fixedType() )
            {
                முழு _type = CV_MAT_TYPE(flags);
                க்கு( size_t j = len0; j < len; j++ )
                {
                    எனில்( v[j].type() == _type )
                        தொடர்;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            திரும்பு;
        }

        CV_Assert( i < (முழு)v.size() );
        Mat& m = v[i];

        எனில்( allowTransposed )
        {
            எனில்( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            எனில்( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            இல்லையெனில்
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        எனில்(fixedSize())
        {
            CV_Assert(m.dims == d);
            க்கு(முழு j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        திரும்பு;
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        எனில்( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            எனில்( fixedType() )
            {
                முழு _type = CV_MAT_TYPE(flags);
                க்கு( size_t j = len0; j < len; j++ )
                {
                    எனில்( v[j].type() == _type )
                        தொடர்;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            திரும்பு;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        எனில்( allowTransposed )
        {
            எனில்( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            எனில்( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            இல்லையெனில்
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        எனில்(fixedSize())
        {
            CV_Assert(m.dims == d);
            க்கு(முழு j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        எனில்( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            எனில்( fixedType() )
            {
                முழு _type = CV_MAT_TYPE(flags);
                க்கு( size_t j = len0; j < len; j++ )
                {
                    எனில்( v[j].type() == _type )
                        தொடர்;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            திரும்பு;
        }

        CV_Assert( i < (முழு)v.size() );
        UMat& m = v[i];

        எனில்( allowTransposed )
        {
            எனில்( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            எனில்( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            இல்லையெனில்
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        எனில்(fixedSize())
        {
            CV_Assert(m.dims == d);
            க்கு(முழு j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        திரும்பு;
    }

    எனில் ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        திரும்பு;
    }

    எனில்( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

வெறுமை _OutputArray::create(மாறிலி MatShape& shape, முழு mtype, முழு i,
                          தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    எனில் (shape.dims < 0) {
        release();
    } இல்லையெனில் {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(முழு mtype) மாறிலி
{
    mtype = CV_MAT_TYPE(mtype);
    திரும்பு getMat().reinterpret(mtype);
}

வெறுமை _OutputArray::createSameSize(மாறிலி உள்ளீடு_Array_2& arr, முழு mtype) மாறிலி
{
    create(arr.shape(), mtype);
}

வெறுமை _OutputArray::fit(Size _sz, முழு mtype, முழு i,
                       தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    முழு sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

வெறுமை _OutputArray::fit(முழு rows, முழு cols, முழு mtype, முழு i,
                       தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    முழு sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

வெறுமை _OutputArray::fit(மாறிலி MatShape& shape, முழு mtype, முழு i,
                       தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    எனில் (shape.dims < 0) {
        release();
        திரும்பு;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } இல்லையெனில் எனில் ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

வெறுமை _OutputArray::fitSameSize(மாறிலி உள்ளீடு_Array_2& arr, முழு mtype) மாறிலி
{
    fit(arr.shape(), mtype);
}

வெறுமை _OutputArray::fit(முழு d, மாறிலி முழு* sizes, முழு mtype, முழு i,
                       தருக்க allowTransposed, _OutputArray::DepthMask fixedDepthMask) மாறிலி
{
    முழு size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    உள்ளீடு_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    எனில்( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        எனில் (k == MAT)
            m = (Mat*)obj;
        இல்லையெனில் {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        எனில் (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            இல்லையெனில்
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        எனில்(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            க்கு(முழு j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        திரும்பு;
    }

    எனில்( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        எனில் (k == UMAT)
            m = (UMat*)obj;
        இல்லையெனில் {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        எனில் (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            திரும்பு;
        }

        எனில்(fixedType())
        {
            எனில்(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            இல்லையெனில்
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        எனில்(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            க்கு(முழு j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        திரும்பு;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

வெறுமை _OutputArray::release() மாறிலி
{
    CV_Assert(!fixedSize());

    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        ((Mat*)obj)->release();
        திரும்பு;
    }

    எனில்( k == UMAT )
    {
        ((UMat*)obj)->release();
        திரும்பு;
    }

    எனில்( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில்( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில்( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    எனில்( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    எனில்( k == NONE )
        திரும்பு;

    எனில்( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        எனில் (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } இல்லையெனில் { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        நிறுத்து

        முழு esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        திரும்பு;
    }

    எனில்( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        க்கு( முழு i = 0; i < sz.height; i++ )
            v[i].release();
        திரும்பு;
    }

    எனில்( k == STD_BOOL_VECTOR )
    {
        ((std::vector<தருக்க>*)obj)->clear();
        திரும்பு;
    }

    எனில்( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        திரும்பு;
    }
    எனில் (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        திரும்பு;
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

வெறுமை _OutputArray::clear() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        திரும்பு;
    }

    release();
}

தருக்க _OutputArray::needed() மாறிலி
{
    திரும்பு kind() != NONE;
}

Mat& _OutputArray::getMatRef(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில்( i < 0 )
    {
        CV_Assert( k == MAT );
        திரும்பு *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    எனில்( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (முழு)v.size() );
        திரும்பு v[i];
    }
    இல்லையெனில்
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        திரும்பு v[i];
    }
}

UMat& _OutputArray::getUMatRef(முழு i) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில்( i < 0 )
    {
        CV_Assert( k == UMAT );
        திரும்பு *(UMat*)obj;
    }
    இல்லையெனில்
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (முழு)v.size() );
        திரும்பு v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    திரும்பு *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    திரும்பு *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    திரும்பு *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    திரும்பு *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    திரும்பு *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    திரும்பு *(cuda::HostMem*)obj;
}

வெறுமை _OutputArray::setTo(மாறிலி உள்ளீடு_Array_2& arr, மாறிலி உள்ளீடு_Array_2 & mask) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == NONE )
        ;
    இல்லையெனில் எனில் (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    இல்லையெனில் எனில்( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    இல்லையெனில் எனில்( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), உள்ளீடு_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<இரட்டை, 4>(value.ptr<இரட்டை>())), mask);
#இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    இல்லையெனில்
        CV_Error(Error::StsNotImplemented, "");
}

வெறுமை _OutputArray::setZero() மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();

    எனில்( k == NONE )
        ;
    இல்லையெனில் எனில் (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    இல்லையெனில்
    {
        setTo(Scalar::all(0), noArray());
    }
}

வெறுமை _OutputArray::assign(மாறிலி UMat& u) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    இல்லையெனில் எனில் (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    இல்லையெனில் எனில் (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    இல்லையெனில்
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


வெறுமை _OutputArray::assign(மாறிலி Mat& m) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    இல்லையெனில் எனில் (k == MAT)
    {
        *(Mat*)obj = m;
    }
    இல்லையெனில் எனில் (k == MATX)
    {
        m.copyTo(getMat());
    }
    இல்லையெனில்
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


வெறுமை _OutputArray::move(UMat& u) மாறிலி
{
    எனில் (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        திரும்பு;
    }
    முழு k = kind();
    எனில் (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    இல்லையெனில் எனில் (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    இல்லையெனில் எனில் (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    இல்லையெனில்
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


வெறுமை _OutputArray::move(Mat& m) மாறிலி
{
    எனில் (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        திரும்பு;
    }
    முழு k = kind();
    எனில் (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    இல்லையெனில் எனில் (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    இல்லையெனில் எனில் (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    இல்லையெனில்
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


வெறுமை _OutputArray::assign(மாறிலி std::vector<UMat>& v) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        க்கு (size_t i = 0; i < v.size(); i++)
        {
            மாறிலி UMat& m = v[i];
            UMat& this_m = this_v[i];
            எனில் (this_m.u != வெற்று && this_m.u == m.u)
                தொடர்; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    இல்லையெனில் எனில் (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        க்கு (size_t i = 0; i < v.size(); i++)
        {
            மாறிலி UMat& m = v[i];
            Mat& this_m = this_v[i];
            எனில் (this_m.u != வெற்று && this_m.u == m.u)
                தொடர்; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    இல்லையெனில்
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


வெறுமை _OutputArray::assign(மாறிலி std::vector<Mat>& v) மாறிலி
{
    உள்ளீடு_Array_2::KindFlag k = kind();
    எனில் (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        க்கு (size_t i = 0; i < v.size(); i++)
        {
            மாறிலி Mat& m = v[i];
            UMat& this_m = this_v[i];
            எனில் (this_m.u != வெற்று && this_m.u == m.u)
                தொடர்; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    இல்லையெனில் எனில் (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        க்கு (size_t i = 0; i < v.size(); i++)
        {
            மாறிலி Mat& m = v[i];
            Mat& this_m = this_v[i];
            எனில் (this_m.u != வெற்று && this_m.u == m.u)
                தொடர்; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    இல்லையெனில்
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


நிலையான உள்ளீடு_Output_Array_2 _none;
உள்ளீடு_Output_Array noArray() { திரும்பு _none; }

} // cv::
