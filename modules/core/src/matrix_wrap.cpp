// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/mat.hpp"

ନାମାକାଶ cv {

ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 5> Vec5i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 7> Vec7i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 9> Vec9i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 10> Vec10i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 11> Vec11i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 12> Vec12i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 13> Vec13i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 14> Vec14i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 15> Vec15i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 16> Vec16i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 32> Vec32i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 64> Vec64i;
ପ୍ରକାରବ୍ୟାଖ୍ୟା Vec<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    ବାଛ( esz ) \
    { \
    ସ୍ଥିତି 1: func(uchar); \
    ସ୍ଥିତି 2: func(Vec2b); \
    ସ୍ଥିତି 3: func(Vec3b); \
    ସ୍ଥିତି 4: func(ପୂର୍ଣ୍ଣସଂଖ୍ୟା); \
    ସ୍ଥିତି 6: func(Vec3s); \
    ସ୍ଥିତି 8: func(Vec2i); \
    ସ୍ଥିତି 12: func(Vec3i); \
    ସ୍ଥିତି 16: func(Vec4i); \
    ସ୍ଥିତି 20: func(Vec5i); \
    ସ୍ଥିତି 24: func(Vec6i); \
    ସ୍ଥିତି 28: func(Vec7i); \
    ସ୍ଥିତି 32: func(Vec8i); \
    ସ୍ଥିତି 36: func(Vec9i); \
    ସ୍ଥିତି 40: func(Vec10i); \
    ସ୍ଥିତି 44: func(Vec11i); \
    ସ୍ଥିତି 48: func(Vec12i); \
    ସ୍ଥିତି 52: func(Vec13i); \
    ସ୍ଥିତି 56: func(Vec14i); \
    ସ୍ଥିତି 60: func(Vec15i); \
    ସ୍ଥିତି 64: func(Vec16i); \
    ସ୍ଥିତି 128: func(Vec32i); \
    ସ୍ଥିତି 256: func(Vec64i); \
    ସ୍ଥିତି 512: func(Vec128i); \
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat ନିବେଶ_Array_2::getMat_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ଯଦି( k == MAT )
    {
        ଅଚଳ Mat* m = (ଅଚଳ Mat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ *m;
        ଫେରନ୍ତୁ m->row(i);
    }

    ଯଦି( k == UMAT )
    {
        ଅଚଳ UMat* m = (ଅଚଳ UMat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ m->getMat(accessFlags);
        ଫେରନ୍ତୁ m->getMat(accessFlags).row(i);
    }

    ଯଦି (k == MATX)
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ Mat(sz, flags, obj);
    }

    ଯଦି( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = CV_8U;
        ଅଚଳ std::vector<ତାର୍କିକ>& v = *(ଅଚଳ std::vector<ତାର୍କିକ>*)obj;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size();
        ଯଦି( n == 0 )
            ଫେରନ୍ତୁ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        ପାଇଁ( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ଫେରନ୍ତୁ m;
    }

    ଯଦି( k == NONE )
        ଫେରନ୍ତୁ Mat();

    ଯଦି( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = CV_MAT_TYPE(flags);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            ଯଦି (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ଅନ୍ୟଥା { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା v_sz = ପୂର୍ଣ୍ଣସଂଖ୍ୟା(v->size()); \
            ଫେରନ୍ତୁ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& v = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );

        ଫେରନ୍ତୁ v[i];
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* v = (ଅଚଳ Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ଫେରନ୍ତୁ v[i];
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& v = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );

        ଫେରନ୍ତୁ v[i].getMat(accessFlags);
    }

    ଯଦି( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    ଯଦି( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    ଯଦି( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    ଯଦି( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ଅଚଳ cuda::HostMem* cuda_mem = (ଅଚଳ cuda::HostMem*)obj;

        ଫେରନ୍ତୁ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat ନିବେଶ_Array_2::getUMat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ଯଦି( k == UMAT )
    {
        ଅଚଳ UMat* m = (ଅଚଳ UMat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ *m;
        ଫେରନ୍ତୁ m->row(i);
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& v = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );

        ଫେରନ୍ତୁ v[i];
    }

    ଯଦି( k == MAT )
    {
        ଅଚଳ Mat* m = (ଅଚଳ Mat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ m->getUMat(accessFlags);
        ଫେରନ୍ତୁ m->row(i).getUMat(accessFlags);
    }

    ଫେରନ୍ତୁ getMat(i).getUMat(accessFlags);
}

ଶୂନ୍ୟ ନିବେଶ_Array_2::getMatVector(std::vector<Mat>& mv) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ଯଦି( k == MAT )
    {
        ଅଚଳ Mat& m = *(ଅଚଳ Mat*)obj;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (ଶୂନ୍ୟ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (ଶୂନ୍ୟ*)m.ptr(i), &m.step[1]);
        ଫେରନ୍ତୁ;
    }

    ଯଦି (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR )
    {
        ଅଚଳ std::vector<uchar>& v = *(ଅଚଳ std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (ଶୂନ୍ୟ*)(v.data() + esz*i));
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == NONE )
    {
        mv.clear();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_VECTOR )
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା typ = CV_MAT_TYPE(flags);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            ଅଚଳ std::vector<std::vector<T > >* vv = (ଅଚଳ std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            ପାଇଁ (size_t i = 0; i < n; i++) { \
                ଅଚଳ std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା ni = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (ଶୂନ୍ୟ*)vi.data()) : Mat(); \
            } \
        } \
        ଭାଙ୍ଗ

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& v = *(ଅଚଳ std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* v = (ଅଚଳ Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& v = *(ଅଚଳ std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ଫେରନ୍ତୁ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ଶୂନ୍ୟ ନିବେଶ_Array_2::getUMatVector(std::vector<UMat>& umv) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    ଯଦି( k == NONE )
    {
        umv.clear();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& v = *(ଅଚଳ std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* v = (ଅଚଳ Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& v = *(ଅଚଳ std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        ପାଇଁ( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ଫେରନ୍ତୁ;
    }
    ଯଦି( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ଫେରନ୍ତୁ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat ନିବେଶ_Array_2::getGpuMat() ଅଚଳ
{
#ifdef HAVE_CUDA
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି (k == CUDA_GPU_MAT)
    {
        ଅଚଳ cuda::GpuMat* d_mat = (ଅଚଳ cuda::GpuMat*)obj;
        ଫେରନ୍ତୁ *d_mat;
    }

    ଯଦି (k == CUDA_HOST_MEM)
    {
        ଅଚଳ cuda::HostMem* cuda_mem = (ଅଚଳ cuda::HostMem*)obj;
        ଫେରନ୍ତୁ cuda_mem->createGpuMatHeader();
    }

    ଯଦି (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    ଯଦି (k == NONE)
        ଫେରନ୍ତୁ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#ଅନ୍ୟଥା
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ଶୂନ୍ୟ ନିବେଶ_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) ଅଚଳ
{
#ifdef HAVE_CUDA
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ଶୂନ୍ୟ ନିବେଶ_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ଅଚଳ
{
#ifdef HAVE_CUDA
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#ଅନ୍ୟଥା
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND ନିବେଶ_Array_2::getGpuMatND() ଅଚଳ
{
#ifdef HAVE_CUDA
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି (k == CUDA_GPU_MATND)
    {
        ଅଚଳ cuda::GpuMatND* d_mat = (ଅଚଳ cuda::GpuMatND*)obj;
        ଫେରନ୍ତୁ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#ଅନ୍ୟଥା
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer ନିବେଶ_Array_2::getOGlBuffer() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    ଅଚଳ ogl::Buffer* gl_buf = (ଅଚଳ ogl::Buffer*)obj;
    ଫେରନ୍ତୁ *gl_buf;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_Array_2::rows_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
#ifdef HAVE_CUDA
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == CUDA_GPU_MATND)
    {
        ଅଚଳ cuda::GpuMatND& _gpuMatND = *(ଅଚଳ cuda::GpuMatND*)obj;
        ଫେରନ୍ତୁ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ଫେରନ୍ତୁ size_(i).height;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_Array_2::cols_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
#ifdef HAVE_CUDA
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == CUDA_GPU_MATND)
    {
        ଅଚଳ cuda::GpuMatND& _gpuMatND = *(ଅଚଳ cuda::GpuMatND*)obj;
        ଫେରନ୍ତୁ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ଫେରନ୍ତୁ size_(i).width;
}

Size ନିବେଶ_Array_2::size_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ Mat* m = (ଅଚଳ Mat*)obj;
        CV_Assert(m->dims <= 2);
        ଫେରନ୍ତୁ Size(m->cols, m->rows);
    }

    ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ UMat* m = (ଅଚଳ UMat*)obj;
        CV_Assert(m->dims <= 2);
        ଫେରନ୍ତୁ Size(m->cols, m->rows);
    }

    ଯଦି (k == MATX)
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ sz;
    }

    ଯଦି( k == NONE )
        ଫେରନ୍ତୁ Size();

    ଯଦି( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ଫେରନ୍ତୁ Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)n, 1);
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ vv.empty() ? Size() : Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size(), 1);
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );

        ଫେରନ୍ତୁ vv[i].size();
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        ଫେରନ୍ତୁ vv[i].size();
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ଅଚଳ std::vector<cuda::GpuMat>& vv = *(ଅଚଳ std::vector<cuda::GpuMat>*)obj;
        ଯଦି (i < 0)
            ଫେରନ୍ତୁ vv.empty() ? Size() : Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size(), 1);
        CV_Assert(i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size());
        ଫେରନ୍ତୁ vv[i].size();
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ଅଚଳ std::vector<cuda::GpuMatND>& vv = *(ଅଚଳ std::vector<cuda::GpuMatND>*)obj;
        ଯଦି (i < 0)
            ଫେରନ୍ତୁ vv.empty() ? Size() : Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size(), 1);
        CV_Assert(i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size());
        ଅଚଳ cuda::GpuMatND& m = vv[i];
        ଯଦି (m.dims == 0)
            ଫେରନ୍ତୁ Size();
        ଯଦି (m.dims == 1)
            ଫେରନ୍ତୁ Size(1, m.size[0]);
        ଫେରନ୍ତୁ Size(m.size[1], m.size[0]);
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ vv.empty() ? Size() : Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size(), 1);
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );

        ଫେରନ୍ତୁ vv[i].size();
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ଅଚଳ std::vector<cuda::GpuMatND>& vv = *(ଅଚଳ std::vector<cuda::GpuMatND>*)obj;
        ଯଦି (i < 0)
            ଫେରନ୍ତୁ vv.empty() ? Size() : Size((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size(), 1);
        CV_Assert(i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size());
        ଅଚଳ cuda::GpuMatND& m = vv[i];
        ଯଦି (m.dims == 0)
            ଫେରନ୍ତୁ Size();
        ଯଦି (m.dims == 1)
            ଫେରନ୍ତୁ Size(1, m.size[0]);
        ଫେରନ୍ତୁ Size(m.size[1], m.size[0]);
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ଅଚଳ ogl::Buffer* buf = (ଅଚଳ ogl::Buffer*)obj;
        ଫେରନ୍ତୁ buf->size();
    }

    ଯଦି( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ cuda::GpuMat* d_mat = (ଅଚଳ cuda::GpuMat*)obj;
        ଫେରନ୍ତୁ d_mat->size();
    }

    ଯଦି( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ଅଚଳ cuda::HostMem* cuda_mem = (ଅଚଳ cuda::HostMem*)obj;
        ଫେରନ୍ତୁ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_Array_2::sizend(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* arrsz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j, d = 0;
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == NONE )
        ;
    ଅନ୍ୟଥା ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ Mat& m = *(ଅଚଳ Mat*)obj;
        d = m.dims;
        ଯଦି(arrsz)
            ପାଇଁ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ଅନ୍ୟଥା ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ UMat& m = *(ଅଚଳ UMat*)obj;
        d = m.dims;
        ଯଦି(arrsz)
            ପାଇଁ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ଅନ୍ୟଥା ଯଦି( k == STD_VECTOR_MAT && i >= 0 )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଅଚଳ Mat& m = vv[i];
        d = m.dims;
        ଯଦି(arrsz)
            ପାଇଁ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ଅନ୍ୟଥା ଯଦି( k == STD_ARRAY_MAT && i >= 0 )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        CV_Assert( i < sz.height );
        ଅଚଳ Mat& m = vv[i];
        d = m.dims;
        ଯଦି(arrsz)
            ପାଇଁ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ଅନ୍ୟଥା ଯଦି( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଅଚଳ UMat& m = vv[i];
        d = m.dims;
        ଯଦି(arrsz)
            ପାଇଁ(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    ଅନ୍ୟଥା ଯଦି (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        ଯଦି(arrsz)
        {
            arrsz[0] = ପୂର୍ଣ୍ଣସଂଖ୍ୟା(n);
        }
    }
    ଅନ୍ୟଥା
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        ଯଦି(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ଫେରନ୍ତୁ d;
}

ତାର୍କିକ ନିବେଶ_Array_2::empty(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (i >= 0) {
        ଯଦି (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<ଅଚଳ std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ଫେରନ୍ତୁ mv->at(i).empty();
        }
        ଅନ୍ୟଥା ଯଦି (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<ଅଚଳ std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ଫେରନ୍ତୁ umv->at(i).empty();
        }
        ଅନ୍ୟଥା ଯଦି (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            ଫେରନ୍ତୁ n == 0;
        } ଅନ୍ୟଥା {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ଫେରନ୍ତୁ empty();
}

MatShape ନିବେଶ_Array_2::shape(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    MatShape shape;

    ଯଦି( k == NONE )
        ;
    ଅନ୍ୟଥା ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ Mat& m = *(ଅଚଳ Mat*)obj;
        shape = m.size;
    }
    ଅନ୍ୟଥା ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ UMat& m = *(ଅଚଳ UMat*)obj;
        shape = m.size;
    }
    ଅନ୍ୟଥା ଯଦି( k == STD_VECTOR_MAT && i >= 0 )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ଅଚଳ Mat& m = vv[i];
        shape = m.size;
    }
    ଅନ୍ୟଥା ଯଦି( k == STD_ARRAY_MAT && i >= 0 )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        CV_Assert( i < sz.height );
        ଅଚଳ Mat& m = vv[i];
        shape = m.size;
    }
    ଅନ୍ୟଥା ଯଦି( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ଅଚଳ UMat& m = vv[i];
        shape = m.size;
    }
    ଅନ୍ୟଥା
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[CV_MAX_DIM];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ଫେରନ୍ତୁ shape;
}

ତାର୍କିକ ନିବେଶ_Array_2::sameSize(ଅଚଳ ନିବେଶ_Array_2& arr) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    ଯଦି( k1 == MAT )
    {
        ଅଚଳ Mat* m = ((ଅଚଳ Mat*)obj);
        ଯଦି( k2 == MAT )
            ଫେରନ୍ତୁ m->size == ((ଅଚଳ Mat*)arr.obj)->size;
        ଯଦି( k2 == UMAT )
            ଫେରନ୍ତୁ m->size == ((ଅଚଳ UMat*)arr.obj)->size;
        ଯଦି( m->dims > 2 )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        sz1 = m->size();
    }
    ଅନ୍ୟଥା ଯଦି( k1 == UMAT )
    {
        ଅଚଳ UMat* m = ((ଅଚଳ UMat*)obj);
        ଯଦି( k2 == MAT )
            ଫେରନ୍ତୁ m->size == ((ଅଚଳ Mat*)arr.obj)->size;
        ଯଦି( k2 == UMAT )
            ଫେରନ୍ତୁ m->size == ((ଅଚଳ UMat*)arr.obj)->size;
        ଯଦି( m->dims > 2 )
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        sz1 = m->size();
    }
    ଅନ୍ୟଥା ଯଦି ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMatND*)obj)->size == ((ଅଚଳ cuda::GpuMatND*)arr.obj)->size;
    }
    ଅନ୍ୟଥା
        sz1 = size();

    ଯଦି( arr.dims() > 2 )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ sz1 == arr.size();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_Array_2::dims_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ Mat*)obj)->dims;
    }

    ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ UMat*)obj)->dims;
    }

    ଯଦି (k == MATX)
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ 2;
    }

    ଯଦି( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି( k == NONE )
        ଫେରନ୍ତୁ 0;

    ଯଦି( k == STD_VECTOR_VECTOR )
    {
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ 1;
        CV_Assert( size_t(i) < total() );
        ଫେରନ୍ତୁ 1;
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ 1;
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );

        ଫେରନ୍ତୁ vv[i].dims;
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ 1;
        CV_Assert( i < sz.height );

        ଫେରନ୍ତୁ vv[i].dims;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ 1;
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );

        ଫେରନ୍ତୁ vv[i].dims;
    }

    ଯଦି( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ 2;
    }

    ଯଦି( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ 2;
    }

    ଯଦି( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMatND*)obj)->dims;
    }

    ଯଦି( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t ନିବେଶ_Array_2::total(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ Mat*)obj)->total();
    }

    ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ UMat*)obj)->total();
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ vv.size();

        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଫେରନ୍ତୁ vv[i].total();
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ sz.height;

        CV_Assert( i < sz.height );
        ଫେରନ୍ତୁ vv[i].total();
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        ଯଦି( i < 0 )
            ଫେରନ୍ତୁ vv.size();

        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଫେରନ୍ତୁ vv[i].total();
    }

    ଯଦି( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ଅଚଳ cuda::GpuMatND& _gpuMatND = *((ଅଚଳ cuda::GpuMatND*)obj);
        ଯଦି (_gpuMatND.dims > 0)
        {
            res = 1;
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ଫେରନ୍ତୁ res;
        }
    }

    ଯଦି (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            ଯଦି (k == STD_VECTOR_VECTOR) { \
                ଅଚଳ std::vector<std::vector<T > >* vv = (ଅଚଳ std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                ଯଦି (i < 0) \
                    ଫେରନ୍ତୁ n; \
                CV_Assert((size_t)i < n); \
                ଫେରନ୍ତୁ vv->at(i).size(); \
            } ଅନ୍ୟଥା \
                ଫେରନ୍ତୁ ((ଅଚଳ std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    ଫେରନ୍ତୁ size(i).area();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_Array_2::type_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
        ଫେରନ୍ତୁ ((ଅଚଳ Mat*)obj)->type();

    ଯଦି( k == UMAT )
        ଫେରନ୍ତୁ ((ଅଚଳ UMat*)obj)->type();

    ଯଦି( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ଫେରନ୍ତୁ CV_MAT_TYPE(flags);

    ଯଦି( k == NONE )
        ଫେରନ୍ତୁ -1;

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        ଯଦି( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ଫେରନ୍ତୁ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଫେରନ୍ତୁ vv[i >= 0 ? i : 0].type();
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        ଯଦି( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ଫେରନ୍ତୁ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଫେରନ୍ତୁ vv[i >= 0 ? i : 0].type();
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        ଯଦି( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ଫେରନ୍ତୁ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ଫେରନ୍ତୁ vv[i >= 0 ? i : 0].type();
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ଅଚଳ std::vector<cuda::GpuMat>& vv = *(ଅଚଳ std::vector<cuda::GpuMat>*)obj;
        ଯଦି (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ଫେରନ୍ତୁ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size());
        ଫେରନ୍ତୁ vv[i >= 0 ? i : 0].type();
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ଅଚଳ std::vector<cuda::GpuMatND>& vv = *(ଅଚଳ std::vector<cuda::GpuMatND>*)obj;
        ଯଦି (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ଫେରନ୍ତୁ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size());
        ଫେରନ୍ତୁ vv[i >= 0 ? i : 0].type();
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି( k == OPENGL_BUFFER )
        ଫେରନ୍ତୁ ((ଅଚଳ ogl::Buffer*)obj)->type();

    ଯଦି( k == CUDA_GPU_MAT )
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMat*)obj)->type();

    ଯଦି( k == CUDA_GPU_MATND )
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMatND*)obj)->type();

    ଯଦି( k == CUDA_HOST_MEM )
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ନିବେଶ_Array_2::depth_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ଫେରନ୍ତୁ CV_MAT_DEPTH(type(i));
}

ତାର୍କିକ ନିବେଶ_Array_2::empty_() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
        ଫେରନ୍ତୁ ((ଅଚଳ Mat*)obj)->empty();

    ଯଦି( k == UMAT )
        ଫେରନ୍ତୁ ((ଅଚଳ UMat*)obj)->empty();

    ଯଦି (k == MATX)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        ଫେରନ୍ତୁ n == 0;
    }

    ଯଦି( k == NONE )
        ଫେରନ୍ତୁ ସତ୍ୟ;

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        ଫେରନ୍ତୁ vv.empty();
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଫେରନ୍ତୁ sz.height == 0;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        ଫେରନ୍ତୁ vv.empty();
    }

    ଯଦି( k == OPENGL_BUFFER )
        ଫେରନ୍ତୁ ((ଅଚଳ ogl::Buffer*)obj)->empty();

    ଯଦି( k == CUDA_GPU_MAT )
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMat*)obj)->empty();

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ଅଚଳ std::vector<cuda::GpuMat>& vv = *(ଅଚଳ std::vector<cuda::GpuMat>*)obj;
        ଫେରନ୍ତୁ vv.empty();
    }

    ଯଦି( k == CUDA_GPU_MATND )
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMatND*)obj)->empty();

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ଅଚଳ std::vector<cuda::GpuMatND>& vv = *(ଅଚଳ std::vector<cuda::GpuMatND>*)obj;
        ଫେରନ୍ତୁ vv.empty();
    }

    ଯଦି( k == CUDA_HOST_MEM )
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ତାର୍କିକ ନିବେଶ_Array_2::isContinuous(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
        ଫେରନ୍ତୁ i < 0 ? ((ଅଚଳ Mat*)obj)->isContinuous() : ସତ୍ୟ;

    ଯଦି( k == UMAT )
        ଫେରନ୍ତୁ i < 0 ? ((ଅଚଳ UMat*)obj)->isContinuous() : ସତ୍ୟ;

    ଯଦି( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ଫେରନ୍ତୁ ସତ୍ୟ;

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].isContinuous();
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ଫେରନ୍ତୁ vv[i].isContinuous();
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].isContinuous();
    }

    ଯଦି( k == CUDA_GPU_MAT )
      ଫେରନ୍ତୁ i < 0 ? ((ଅଚଳ cuda::GpuMat*)obj)->isContinuous() : ସତ୍ୟ;

    ଯଦି( k == CUDA_GPU_MATND )
        ଫେରନ୍ତୁ i < 0 ? ((ଅଚଳ cuda::GpuMatND*)obj)->isContinuous() : ସତ୍ୟ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

ତାର୍କିକ ନିବେଶ_Array_2::isSubmatrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
        ଫେରନ୍ତୁ i < 0 ? ((ଅଚଳ Mat*)obj)->isSubmatrix() : ମିଥ୍ୟା;

    ଯଦି( k == UMAT )
        ଫେରନ୍ତୁ i < 0 ? ((ଅଚଳ UMat*)obj)->isSubmatrix() : ମିଥ୍ୟା;

    ଯଦି( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].isSubmatrix();
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ଫେରନ୍ତୁ vv[i].isSubmatrix();
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].isSubmatrix();
    }

    ଯଦି( k == CUDA_GPU_MATND )
    {
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMatND*)obj)->isSubmatrix();
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ଅଚଳ std::vector<cuda::GpuMatND>& vv = *(ଅଚଳ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t ନିବେଶ_Array_2::offset(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ Mat * ଅଚଳ m = ((ଅଚଳ Mat*)obj);
        ଫେରନ୍ତୁ (size_t)(m->ptr() - m->datastart);
    }

    ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ UMat*)obj)->offset;
    }

    ଯଦି( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ଫେରନ୍ତୁ 0;

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );

        ଫେରନ୍ତୁ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ଫେରନ୍ତୁ (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].offset;
    }

    ଯଦି( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ଅଚଳ cuda::GpuMat * ଅଚଳ m = ((ଅଚଳ cuda::GpuMat*)obj);
        ଫେରନ୍ତୁ (size_t)(m->data - m->datastart);
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ଅଚଳ std::vector<cuda::GpuMat>& vv = *(ଅଚଳ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t ନିବେଶ_Array_2::step(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ Mat*)obj)->step;
    }

    ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ UMat*)obj)->step;
    }

    ଯଦି( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ଫେରନ୍ତୁ 0;

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ଅଚଳ std::vector<Mat>& vv = *(ଅଚଳ std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)vv.size() );
        ଫେରନ୍ତୁ vv[i].step;
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        ଅଚଳ Mat* vv = (ଅଚଳ Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ଫେରନ୍ତୁ vv[i].step;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ଅଚଳ std::vector<UMat>& vv = *(ଅଚଳ std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].step;
    }

    ଯଦି( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ଫେରନ୍ତୁ ((ଅଚଳ cuda::GpuMat*)obj)->step;
    }
    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ଅଚଳ std::vector<cuda::GpuMat>& vv = *(ଅଚଳ std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଫେରନ୍ତୁ vv[i].step;
    }
    ଯଦି( k == CUDA_GPU_MATND )
    {
        ଅଚଳ cuda::GpuMatND& _gpuMatND = *(ଅଚଳ cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ଫେରନ୍ତୁ _gpuMatND.step[i];
    }

    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ଅଚଳ std::vector<cuda::GpuMatND>& vv = *(ଅଚଳ std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ଅଚଳ cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ଫେରନ୍ତୁ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

ଶୂନ୍ୟ ନିବେଶ_Array_2::copyTo(ଅଚଳ _OutputArray& arr) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == NONE )
        arr.release();
    ଅନ୍ୟଥା ଯଦି( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    ଅନ୍ୟଥା ଯଦି( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    ଅନ୍ୟଥା ଯଦି (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "");
}

ଶୂନ୍ୟ ନିବେଶ_Array_2::copyTo(ଅଚଳ _OutputArray& arr, ଅଚଳ ନିବେଶ_Array_2 & mask) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == NONE )
        arr.release();
    ଅନ୍ୟଥା ଯଦି( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    ଅନ୍ୟଥା ଯଦି( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    ଅନ୍ୟଥା ଯଦି (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "");
}

ତାର୍କିକ _OutputArray::fixedSize() ଅଚଳ
{
    ଫେରନ୍ତୁ (flags & FIXED_SIZE) == FIXED_SIZE;
}

ତାର୍କିକ _OutputArray::fixedType() ଅଚଳ
{
    ଫେରନ୍ତୁ (flags & FIXED_TYPE) == FIXED_TYPE;
}

ଶୂନ୍ୟ _OutputArray::create_(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ଫେରନ୍ତୁ;
    }
    ଯଦି( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ଫେରନ୍ତୁ;
    }
    ଯଦି( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ଯଦି( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ଯଦି( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ଯଦି( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ଶୂନ୍ୟ _OutputArray::create_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ଫେରନ୍ତୁ;
    }
    ଯଦି( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ଫେରନ୍ତୁ;
    }
    ଯଦି( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ଯଦି( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ଯଦି( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    ଯଦି( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ଶୂନ୍ୟ _OutputArray::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                          ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ନିବେଶ_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ଯଦି( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ଯଦି (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ଅନ୍ୟଥା
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ଯଦି(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        ଯଦି (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ଅନ୍ୟଥା
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        ଯଦି(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == MATX )
    {
        CV_Assert( i < 0 );
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        ଯଦି (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        ଅନ୍ୟଥା
        {
            ଯଦି (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            ଅନ୍ୟଥା
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = CV_ELEM_SIZE(flags);

        ଯଦି( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            ଯଦି (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                ଯଦି (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    ଫେରନ୍ତୁ; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } ଅନ୍ୟଥା { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        ଭାଙ୍ଗ

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        ଯଦି( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ଯଦି( fixedType() )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type = CV_MAT_TYPE(flags);
                ପାଇଁ( size_t j = len0; j < len; j++ )
                {
                    ଯଦି( v[j].type() == _type )
                        ଜାରି;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ଫେରନ୍ତୁ;
        }

        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );
        Mat& m = v[i];

        ଯଦି( allowTransposed )
        {
            ଯଦି( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ଯଦି( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ଅନ୍ୟଥା
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ଯଦି(fixedSize())
        {
            CV_Assert(m.dims == d);
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        ଯଦି( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            ଯଦି( fixedType() )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type = CV_MAT_TYPE(flags);
                ପାଇଁ( size_t j = len0; j < len; j++ )
                {
                    ଯଦି( v[j].type() == _type )
                        ଜାରି;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ଫେରନ୍ତୁ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        ଯଦି( allowTransposed )
        {
            ଯଦି( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ଯଦି( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ଅନ୍ୟଥା
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        ଯଦି(fixedSize())
        {
            CV_Assert(m.dims == d);
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        ଯଦି( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            ଯଦି( fixedType() )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା _type = CV_MAT_TYPE(flags);
                ପାଇଁ( size_t j = len0; j < len; j++ )
                {
                    ଯଦି( v[j].type() == _type )
                        ଜାରି;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ଫେରନ୍ତୁ;
        }

        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );
        UMat& m = v[i];

        ଯଦି( allowTransposed )
        {
            ଯଦି( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            ଯଦି( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            ଅନ୍ୟଥା
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        ଯଦି(fixedSize())
        {
            CV_Assert(m.dims == d);
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    ଯଦି ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ଶୂନ୍ୟ _OutputArray::create(ଅଚଳ MatShape& shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                          ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ଯଦି (shape.dims < 0) {
        release();
    } ଅନ୍ୟଥା {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype) ଅଚଳ
{
    mtype = CV_MAT_TYPE(mtype);
    ଫେରନ୍ତୁ getMat().reinterpret(mtype);
}

ଶୂନ୍ୟ _OutputArray::createSameSize(ଅଚଳ ନିବେଶ_Array_2& arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype) ଅଚଳ
{
    create(arr.shape(), mtype);
}

ଶୂନ୍ୟ _OutputArray::fit(Size _sz, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                       ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ଶୂନ୍ୟ _OutputArray::fit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                       ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ଶୂନ୍ୟ _OutputArray::fit(ଅଚଳ MatShape& shape, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                       ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ଯଦି (shape.dims < 0) {
        release();
        ଫେରନ୍ତୁ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } ଅନ୍ୟଥା ଯଦି ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

ଶୂନ୍ୟ _OutputArray::fitSameSize(ଅଚଳ ନିବେଶ_Array_2& arr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype) ଅଚଳ
{
    fit(arr.shape(), mtype);
}

ଶୂନ୍ୟ _OutputArray::fit(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mtype, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                       ତାର୍କିକ allowTransposed, _OutputArray::DepthMask fixedDepthMask) ଅଚଳ
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ନିବେଶ_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    ଯଦି( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        ଯଦି (k == MAT)
            m = (Mat*)obj;
        ଅନ୍ୟଥା {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ଯଦି (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ଅନ୍ୟଥା
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ଯଦି(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        ଯଦି (k == UMAT)
            m = (UMat*)obj;
        ଅନ୍ୟଥା {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        ଯଦି (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ଫେରନ୍ତୁ;
        }

        ଯଦି(fixedType())
        {
            ଯଦି(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            ଅନ୍ୟଥା
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        ଯଦି(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            ପାଇଁ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ଫେରନ୍ତୁ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

ଶୂନ୍ୟ _OutputArray::release() ଅଚଳ
{
    CV_Assert(!fixedSize());

    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        ((Mat*)obj)->release();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == UMAT )
    {
        ((UMat*)obj)->release();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    ଯଦି( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    ଯଦି( k == NONE )
        ଫେରନ୍ତୁ;

    ଯଦି( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        ଯଦି (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } ଅନ୍ୟଥା { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        ଭାଙ୍ଗ

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sz.height; i++ )
            v[i].release();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_BOOL_VECTOR )
    {
        ((std::vector<ତାର୍କିକ>*)obj)->clear();
        ଫେରନ୍ତୁ;
    }

    ଯଦି( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ଫେରନ୍ତୁ;
    }
    ଯଦି (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ଫେରନ୍ତୁ;
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

ଶୂନ୍ୟ _OutputArray::clear() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        ଫେରନ୍ତୁ;
    }

    release();
}

ତାର୍କିକ _OutputArray::needed() ଅଚଳ
{
    ଫେରନ୍ତୁ kind() != NONE;
}

Mat& _OutputArray::getMatRef(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି( i < 0 )
    {
        CV_Assert( k == MAT );
        ଫେରନ୍ତୁ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    ଯଦି( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );
        ଫେରନ୍ତୁ v[i];
    }
    ଅନ୍ୟଥା
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ଫେରନ୍ତୁ v[i];
    }
}

UMat& _OutputArray::getUMatRef(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି( i < 0 )
    {
        CV_Assert( k == UMAT );
        ଫେରନ୍ତୁ *(UMat*)obj;
    }
    ଅନ୍ୟଥା
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)v.size() );
        ଫେରନ୍ତୁ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ଫେରନ୍ତୁ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ଫେରନ୍ତୁ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ଫେରନ୍ତୁ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ଫେରନ୍ତୁ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    ଫେରନ୍ତୁ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ଫେରନ୍ତୁ *(cuda::HostMem*)obj;
}

ଶୂନ୍ୟ _OutputArray::setTo(ଅଚଳ ନିବେଶ_Array_2& arr, ଅଚଳ ନିବେଶ_Array_2 & mask) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == NONE )
        ;
    ଅନ୍ୟଥା ଯଦି (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    ଅନ୍ୟଥା ଯଦି( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    ଅନ୍ୟଥା ଯଦି( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), ନିବେଶ_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<ଦ୍ୱିଗୁଣ, 4>(value.ptr<ଦ୍ୱିଗୁଣ>())), mask);
#ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    ଅନ୍ୟଥା
        CV_Error(Error::StsNotImplemented, "");
}

ଶୂନ୍ୟ _OutputArray::setZero() ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();

    ଯଦି( k == NONE )
        ;
    ଅନ୍ୟଥା ଯଦି (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    ଅନ୍ୟଥା
    {
        setTo(Scalar::all(0), noArray());
    }
}

ଶୂନ୍ୟ _OutputArray::assign(ଅଚଳ UMat& u) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    ଅନ୍ୟଥା ଯଦି (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    ଅନ୍ୟଥା ଯଦି (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    ଅନ୍ୟଥା
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ଶୂନ୍ୟ _OutputArray::assign(ଅଚଳ Mat& m) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    ଅନ୍ୟଥା ଯଦି (k == MAT)
    {
        *(Mat*)obj = m;
    }
    ଅନ୍ୟଥା ଯଦି (k == MATX)
    {
        m.copyTo(getMat());
    }
    ଅନ୍ୟଥା
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ଶୂନ୍ୟ _OutputArray::move(UMat& u) ଅଚଳ
{
    ଯଦି (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        ଫେରନ୍ତୁ;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = kind();
    ଯଦି (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    ଅନ୍ୟଥା ଯଦି (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    ଅନ୍ୟଥା ଯଦି (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    ଅନ୍ୟଥା
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ଶୂନ୍ୟ _OutputArray::move(Mat& m) ଅଚଳ
{
    ଯଦି (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        ଫେରନ୍ତୁ;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = kind();
    ଯଦି (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    ଅନ୍ୟଥା ଯଦି (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    ଅନ୍ୟଥା ଯଦି (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    ଅନ୍ୟଥା
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ଶୂନ୍ୟ _OutputArray::assign(ଅଚଳ std::vector<UMat>& v) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ପାଇଁ (size_t i = 0; i < v.size(); i++)
        {
            ଅଚଳ UMat& m = v[i];
            UMat& this_m = this_v[i];
            ଯଦି (this_m.u != ଖାଲି && this_m.u == m.u)
                ଜାରି; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ଅନ୍ୟଥା ଯଦି (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ପାଇଁ (size_t i = 0; i < v.size(); i++)
        {
            ଅଚଳ UMat& m = v[i];
            Mat& this_m = this_v[i];
            ଯଦି (this_m.u != ଖାଲି && this_m.u == m.u)
                ଜାରି; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ଅନ୍ୟଥା
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ଶୂନ୍ୟ _OutputArray::assign(ଅଚଳ std::vector<Mat>& v) ଅଚଳ
{
    ନିବେଶ_Array_2::KindFlag k = kind();
    ଯଦି (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ପାଇଁ (size_t i = 0; i < v.size(); i++)
        {
            ଅଚଳ Mat& m = v[i];
            UMat& this_m = this_v[i];
            ଯଦି (this_m.u != ଖାଲି && this_m.u == m.u)
                ଜାରି; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ଅନ୍ୟଥା ଯଦି (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        ପାଇଁ (size_t i = 0; i < v.size(); i++)
        {
            ଅଚଳ Mat& m = v[i];
            Mat& this_m = this_v[i];
            ଯଦି (this_m.u != ଖାଲି && this_m.u == m.u)
                ଜାରି; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    ଅନ୍ୟଥା
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ସ୍ଥିର ନିବେଶ_Output_Array_2 _none;
ନିବେଶ_Output_Array noArray() { ଫେରନ୍ତୁ _none; }

} // cv::
