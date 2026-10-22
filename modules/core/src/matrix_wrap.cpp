// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

प्रकारव्याख्या Vec<पूर्णांक, 5> Vec5i;
प्रकारव्याख्या Vec<पूर्णांक, 7> Vec7i;
प्रकारव्याख्या Vec<पूर्णांक, 9> Vec9i;
प्रकारव्याख्या Vec<पूर्णांक, 10> Vec10i;
प्रकारव्याख्या Vec<पूर्णांक, 11> Vec11i;
प्रकारव्याख्या Vec<पूर्णांक, 12> Vec12i;
प्रकारव्याख्या Vec<पूर्णांक, 13> Vec13i;
प्रकारव्याख्या Vec<पूर्णांक, 14> Vec14i;
प्रकारव्याख्या Vec<पूर्णांक, 15> Vec15i;
प्रकारव्याख्या Vec<पूर्णांक, 16> Vec16i;
प्रकारव्याख्या Vec<पूर्णांक, 32> Vec32i;
प्रकारव्याख्या Vec<पूर्णांक, 64> Vec64i;
प्रकारव्याख्या Vec<पूर्णांक, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    निवड( esz ) \
    { \
    स्थिती 1: func(uchar); \
    स्थिती 2: func(Vec2b); \
    स्थिती 3: func(Vec3b); \
    स्थिती 4: func(पूर्णांक); \
    स्थिती 6: func(Vec3s); \
    स्थिती 8: func(Vec2i); \
    स्थिती 12: func(Vec3i); \
    स्थिती 16: func(Vec4i); \
    स्थिती 20: func(Vec5i); \
    स्थिती 24: func(Vec6i); \
    स्थिती 28: func(Vec7i); \
    स्थिती 32: func(Vec8i); \
    स्थिती 36: func(Vec9i); \
    स्थिती 40: func(Vec10i); \
    स्थिती 44: func(Vec11i); \
    स्थिती 48: func(Vec12i); \
    स्थिती 52: func(Vec13i); \
    स्थिती 56: func(Vec14i); \
    स्थिती 60: func(Vec15i); \
    स्थिती 64: func(Vec16i); \
    स्थिती 128: func(Vec32i); \
    स्थिती 256: func(Vec64i); \
    स्थिती 512: func(Vec128i); \
    पूर्वनिर्धारित: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat आदान_Array_2::getMat_(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    जर( k == MAT )
    {
        स्थिरांक Mat* m = (स्थिरांक Mat*)obj;
        जर( i < 0 )
            परत *m;
        परत m->row(i);
    }

    जर( k == UMAT )
    {
        स्थिरांक UMat* m = (स्थिरांक UMat*)obj;
        जर( i < 0 )
            परत m->getMat(accessFlags);
        परत m->getMat(accessFlags).row(i);
    }

    जर (k == MATX)
    {
        CV_Assert( i < 0 );
        परत Mat(sz, flags, obj);
    }

    जर( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        पूर्णांक t = CV_8U;
        स्थिरांक std::vector<तार्किक>& v = *(स्थिरांक std::vector<तार्किक>*)obj;
        पूर्णांक j, n = (पूर्णांक)v.size();
        जर( n == 0 )
            परत Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        साठी( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        परत m;
    }

    जर( k == NONE )
        परत Mat();

    जर( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        पूर्णांक t = CV_MAT_TYPE(flags);
        पूर्णांक esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            जर (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(माप_प्रकार(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            पूर्णांक v_sz = पूर्णांक(v->size()); \
            परत v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& v = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णांक)v.size() );

        परत v[i];
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* v = (स्थिरांक Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        परत v[i];
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& v = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णांक)v.size() );

        परत v[i].getMat(accessFlags);
    }

    जर( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    जर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    जर( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    जर( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        स्थिरांक cuda::HostMem* cuda_mem = (स्थिरांक cuda::HostMem*)obj;

        परत cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat आदान_Array_2::getUMat(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    जर( k == UMAT )
    {
        स्थिरांक UMat* m = (स्थिरांक UMat*)obj;
        जर( i < 0 )
            परत *m;
        परत m->row(i);
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& v = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णांक)v.size() );

        परत v[i];
    }

    जर( k == MAT )
    {
        स्थिरांक Mat* m = (स्थिरांक Mat*)obj;
        जर( i < 0 )
            परत m->getUMat(accessFlags);
        परत m->row(i).getUMat(accessFlags);
    }

    परत getMat(i).getUMat(accessFlags);
}

शून्य आदान_Array_2::getMatVector(std::vector<Mat>& mv) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    जर( k == MAT )
    {
        स्थिरांक Mat& m = *(स्थिरांक Mat*)obj;
        पूर्णांक n = (पूर्णांक)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        साठी( पूर्णांक i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (शून्य*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (शून्य*)m.ptr(i), &m.step[1]);
        परत;
    }

    जर (k == MATX)
    {
        माप_प्रकार n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        परत;
    }

    जर( k == STD_VECTOR )
    {
        स्थिरांक std::vector<uchar>& v = *(स्थिरांक std::vector<uchar>*)obj;

        माप_प्रकार n = total(), esz = CV_ELEM_SIZE(flags);
        पूर्णांक t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (शून्य*)(v.data() + esz*i));
        परत;
    }

    जर( k == NONE )
    {
        mv.clear();
        परत;
    }

    जर( k == STD_VECTOR_VECTOR )
    {
        पूर्णांक typ = CV_MAT_TYPE(flags);
        पूर्णांक esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            स्थिरांक std::vector<std::vector<T > >* vv = (स्थिरांक std::vector<std::vector<T > >*)obj; \
            माप_प्रकार n = vv->size(); \
            mv.resize(n); \
            साठी (माप_प्रकार i = 0; i < n; i++) { \
                स्थिरांक std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (माप_प्रकार)पूर्णांक_उच्च); \
                पूर्णांक ni = (पूर्णांक)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (शून्य*)vi.data()) : Mat(); \
            } \
        } \
        थांबा

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        परत;
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& v = *(स्थिरांक std::vector<Mat>*)obj;
        माप_प्रकार n = v.size();
        mv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i];
        परत;
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* v = (स्थिरांक Mat*)obj;
        माप_प्रकार n = sz.height;
        mv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i];
        परत;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& v = *(स्थिरांक std::vector<UMat>*)obj;
        माप_प्रकार n = v.size();
        mv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        परत;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य आदान_Array_2::getUMatVector(std::vector<UMat>& umv) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    जर( k == NONE )
    {
        umv.clear();
        परत;
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& v = *(स्थिरांक std::vector<Mat>*)obj;
        माप_प्रकार n = v.size();
        umv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        परत;
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* v = (स्थिरांक Mat*)obj;
        माप_प्रकार n = sz.height;
        umv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        परत;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& v = *(स्थिरांक std::vector<UMat>*)obj;
        माप_प्रकार n = v.size();
        umv.resize(n);

        साठी( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i];
        परत;
    }

    जर( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        परत;
    }
    जर( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        परत;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat आदान_Array_2::getGpuMat() स्थिरांक
{
#ifdef HAVE_CUDA
    आदान_Array_2::KindFlag k = kind();

    जर (k == CUDA_GPU_MAT)
    {
        स्थिरांक cuda::GpuMat* d_mat = (स्थिरांक cuda::GpuMat*)obj;
        परत *d_mat;
    }

    जर (k == CUDA_HOST_MEM)
    {
        स्थिरांक cuda::HostMem* cuda_mem = (स्थिरांक cuda::HostMem*)obj;
        परत cuda_mem->createGpuMatHeader();
    }

    जर (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    जर (k == NONE)
        परत cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
शून्य आदान_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) स्थिरांक
{
#ifdef HAVE_CUDA
    आदान_Array_2::KindFlag k = kind();
    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#अन्यथा
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
शून्य आदान_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) स्थिरांक
{
#ifdef HAVE_CUDA
    आदान_Array_2::KindFlag k = kind();
    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#अन्यथा
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND आदान_Array_2::getGpuMatND() स्थिरांक
{
#ifdef HAVE_CUDA
    आदान_Array_2::KindFlag k = kind();

    जर (k == CUDA_GPU_MATND)
    {
        स्थिरांक cuda::GpuMatND* d_mat = (स्थिरांक cuda::GpuMatND*)obj;
        परत *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer आदान_Array_2::getOGlBuffer() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    स्थिरांक ogl::Buffer* gl_buf = (स्थिरांक ogl::Buffer*)obj;
    परत *gl_buf;
}

पूर्णांक आदान_Array_2::rows_(पूर्णांक i) स्थिरांक
{
#ifdef HAVE_CUDA
    आदान_Array_2::KindFlag k = kind();
    जर (k == CUDA_GPU_MATND)
    {
        स्थिरांक cuda::GpuMatND& _gpuMatND = *(स्थिरांक cuda::GpuMatND*)obj;
        परत (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    परत size_(i).height;
}

पूर्णांक आदान_Array_2::cols_(पूर्णांक i) स्थिरांक
{
#ifdef HAVE_CUDA
    आदान_Array_2::KindFlag k = kind();
    जर (k == CUDA_GPU_MATND)
    {
        स्थिरांक cuda::GpuMatND& _gpuMatND = *(स्थिरांक cuda::GpuMatND*)obj;
        परत (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    परत size_(i).width;
}

Size आदान_Array_2::size_(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक Mat* m = (स्थिरांक Mat*)obj;
        CV_Assert(m->dims <= 2);
        परत Size(m->cols, m->rows);
    }

    जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक UMat* m = (स्थिरांक UMat*)obj;
        CV_Assert(m->dims <= 2);
        परत Size(m->cols, m->rows);
    }

    जर (k == MATX)
    {
        CV_Assert( i < 0 );
        परत sz;
    }

    जर( k == NONE )
        परत Size();

    जर( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        माप_प्रकार n = total(i);
        CV_Assert(n <= (माप_प्रकार)पूर्णांक_उच्च);
        परत Size((पूर्णांक)n, 1);
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        जर( i < 0 )
            परत vv.empty() ? Size() : Size((पूर्णांक)vv.size(), 1);
        CV_Assert( i < (पूर्णांक)vv.size() );

        परत vv[i].size();
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        जर( i < 0 )
            परत sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        परत vv[i].size();
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        स्थिरांक std::vector<cuda::GpuMat>& vv = *(स्थिरांक std::vector<cuda::GpuMat>*)obj;
        जर (i < 0)
            परत vv.empty() ? Size() : Size((पूर्णांक)vv.size(), 1);
        CV_Assert(i < (पूर्णांक)vv.size());
        परत vv[i].size();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        स्थिरांक std::vector<cuda::GpuMatND>& vv = *(स्थिरांक std::vector<cuda::GpuMatND>*)obj;
        जर (i < 0)
            परत vv.empty() ? Size() : Size((पूर्णांक)vv.size(), 1);
        CV_Assert(i < (पूर्णांक)vv.size());
        स्थिरांक cuda::GpuMatND& m = vv[i];
        जर (m.dims == 0)
            परत Size();
        जर (m.dims == 1)
            परत Size(1, m.size[0]);
        परत Size(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        जर( i < 0 )
            परत vv.empty() ? Size() : Size((पूर्णांक)vv.size(), 1);
        CV_Assert( i < (पूर्णांक)vv.size() );

        परत vv[i].size();
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        स्थिरांक std::vector<cuda::GpuMatND>& vv = *(स्थिरांक std::vector<cuda::GpuMatND>*)obj;
        जर (i < 0)
            परत vv.empty() ? Size() : Size((पूर्णांक)vv.size(), 1);
        CV_Assert(i < (पूर्णांक)vv.size());
        स्थिरांक cuda::GpuMatND& m = vv[i];
        जर (m.dims == 0)
            परत Size();
        जर (m.dims == 1)
            परत Size(1, m.size[0]);
        परत Size(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        स्थिरांक ogl::Buffer* buf = (स्थिरांक ogl::Buffer*)obj;
        परत buf->size();
    }

    जर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक cuda::GpuMat* d_mat = (स्थिरांक cuda::GpuMat*)obj;
        परत d_mat->size();
    }

    जर( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        स्थिरांक cuda::HostMem* cuda_mem = (स्थिरांक cuda::HostMem*)obj;
        परत cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णांक आदान_Array_2::sizend(पूर्णांक* arrsz, पूर्णांक i) स्थिरांक
{
    पूर्णांक j, d = 0;
    आदान_Array_2::KindFlag k = kind();

    जर( k == NONE )
        ;
    अन्यथा जर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक Mat& m = *(स्थिरांक Mat*)obj;
        d = m.dims;
        जर(arrsz)
            साठी(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक UMat& m = *(स्थिरांक UMat*)obj;
        d = m.dims;
        जर(arrsz)
            साठी(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा जर( k == STD_VECTOR_MAT && i >= 0 )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णांक)vv.size() );
        स्थिरांक Mat& m = vv[i];
        d = m.dims;
        जर(arrsz)
            साठी(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा जर( k == STD_ARRAY_MAT && i >= 0 )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        CV_Assert( i < sz.height );
        स्थिरांक Mat& m = vv[i];
        d = m.dims;
        जर(arrsz)
            साठी(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा जर( k == STD_VECTOR_UMAT && i >= 0 )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णांक)vv.size() );
        स्थिरांक UMat& m = vv[i];
        d = m.dims;
        जर(arrsz)
            साठी(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा जर (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        माप_प्रकार n = total(i);
        d = 1;
        जर(arrsz)
        {
            arrsz[0] = पूर्णांक(n);
        }
    }
    अन्यथा
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        जर(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    परत d;
}

तार्किक आदान_Array_2::empty(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर (i >= 0) {
        जर (k == STD_VECTOR_MAT) {
            स्वतः mv = reinterpret_cast<स्थिरांक std::vector<Mat>*>(obj);
            CV_Assert((माप_प्रकार)i < mv->size());
            परत mv->at(i).empty();
        }
        अन्यथा जर (k == STD_VECTOR_UMAT) {
            स्वतः umv = reinterpret_cast<स्थिरांक std::vector<UMat>*>(obj);
            CV_Assert((माप_प्रकार)i < umv->size());
            परत umv->at(i).empty();
        }
        अन्यथा जर (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            माप_प्रकार n = total(i);
            परत n == 0;
        } अन्यथा {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    परत empty();
}

MatShape आदान_Array_2::shape(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    MatShape shape;

    जर( k == NONE )
        ;
    अन्यथा जर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक Mat& m = *(स्थिरांक Mat*)obj;
        shape = m.size;
    }
    अन्यथा जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक UMat& m = *(स्थिरांक UMat*)obj;
        shape = m.size;
    }
    अन्यथा जर( k == STD_VECTOR_MAT && i >= 0 )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert( (माप_प्रकार)i < vv.size() );
        स्थिरांक Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा जर( k == STD_ARRAY_MAT && i >= 0 )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        CV_Assert( i < sz.height );
        स्थिरांक Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा जर( k == STD_VECTOR_UMAT && i >= 0 )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert( (माप_प्रकार)i < vv.size() );
        स्थिरांक UMat& m = vv[i];
        shape = m.size;
    }
    अन्यथा
    {
        पूर्णांक sizes[CV_MAX_DIM];
        पूर्णांक dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    परत shape;
}

तार्किक आदान_Array_2::sameSize(स्थिरांक आदान_Array_2& arr) स्थिरांक
{
    आदान_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    जर( k1 == MAT )
    {
        स्थिरांक Mat* m = ((स्थिरांक Mat*)obj);
        जर( k2 == MAT )
            परत m->size == ((स्थिरांक Mat*)arr.obj)->size;
        जर( k2 == UMAT )
            परत m->size == ((स्थिरांक UMat*)arr.obj)->size;
        जर( m->dims > 2 )
            परत असत्य;
        sz1 = m->size();
    }
    अन्यथा जर( k1 == UMAT )
    {
        स्थिरांक UMat* m = ((स्थिरांक UMat*)obj);
        जर( k2 == MAT )
            परत m->size == ((स्थिरांक Mat*)arr.obj)->size;
        जर( k2 == UMAT )
            परत m->size == ((स्थिरांक UMat*)arr.obj)->size;
        जर( m->dims > 2 )
            परत असत्य;
        sz1 = m->size();
    }
    अन्यथा जर ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        परत ((स्थिरांक cuda::GpuMatND*)obj)->size == ((स्थिरांक cuda::GpuMatND*)arr.obj)->size;
    }
    अन्यथा
        sz1 = size();

    जर( arr.dims() > 2 )
        परत असत्य;
    परत sz1 == arr.size();
}

पूर्णांक आदान_Array_2::dims_(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक Mat*)obj)->dims;
    }

    जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक UMat*)obj)->dims;
    }

    जर (k == MATX)
    {
        CV_Assert( i < 0 );
        परत 2;
    }

    जर( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        परत 1;
    }

    जर( k == NONE )
        परत 0;

    जर( k == STD_VECTOR_VECTOR )
    {
        जर( i < 0 )
            परत 1;
        CV_Assert( माप_प्रकार(i) < total() );
        परत 1;
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        जर( i < 0 )
            परत 1;
        CV_Assert( i < (पूर्णांक)vv.size() );

        परत vv[i].dims;
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        जर( i < 0 )
            परत 1;
        CV_Assert( i < sz.height );

        परत vv[i].dims;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        जर( i < 0 )
            परत 1;
        CV_Assert( i < (पूर्णांक)vv.size() );

        परत vv[i].dims;
    }

    जर( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        परत 2;
    }

    जर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        परत 2;
    }

    जर( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक cuda::GpuMatND*)obj)->dims;
    }

    जर( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        परत 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

माप_प्रकार आदान_Array_2::total(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक Mat*)obj)->total();
    }

    जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक UMat*)obj)->total();
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        जर( i < 0 )
            परत vv.size();

        CV_Assert( i < (पूर्णांक)vv.size() );
        परत vv[i].total();
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        जर( i < 0 )
            परत sz.height;

        CV_Assert( i < sz.height );
        परत vv[i].total();
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        जर( i < 0 )
            परत vv.size();

        CV_Assert( i < (पूर्णांक)vv.size() );
        परत vv[i].total();
    }

    जर( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        माप_प्रकार res = 0;
        स्थिरांक cuda::GpuMatND& _gpuMatND = *((स्थिरांक cuda::GpuMatND*)obj);
        जर (_gpuMatND.dims > 0)
        {
            res = 1;
            साठी(पूर्णांक d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            परत res;
        }
    }

    जर (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        पूर्णांक esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            जर (k == STD_VECTOR_VECTOR) { \
                स्थिरांक std::vector<std::vector<T > >* vv = (स्थिरांक std::vector<std::vector<T > >*)obj; \
                माप_प्रकार n = vv->size(); \
                जर (i < 0) \
                    परत n; \
                CV_Assert((माप_प्रकार)i < n); \
                परत vv->at(i).size(); \
            } अन्यथा \
                परत ((स्थिरांक std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    परत size(i).area();
}

पूर्णांक आदान_Array_2::type_(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
        परत ((स्थिरांक Mat*)obj)->type();

    जर( k == UMAT )
        परत ((स्थिरांक UMat*)obj)->type();

    जर( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        परत CV_MAT_TYPE(flags);

    जर( k == NONE )
        परत -1;

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        जर( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            परत CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णांक)vv.size() );
        परत vv[i >= 0 ? i : 0].type();
    }

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        जर( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            परत CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णांक)vv.size() );
        परत vv[i >= 0 ? i : 0].type();
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        जर( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            परत CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        परत vv[i >= 0 ? i : 0].type();
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        स्थिरांक std::vector<cuda::GpuMat>& vv = *(स्थिरांक std::vector<cuda::GpuMat>*)obj;
        जर (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            परत CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णांक)vv.size());
        परत vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        स्थिरांक std::vector<cuda::GpuMatND>& vv = *(स्थिरांक std::vector<cuda::GpuMatND>*)obj;
        जर (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            परत CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णांक)vv.size());
        परत vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर( k == OPENGL_BUFFER )
        परत ((स्थिरांक ogl::Buffer*)obj)->type();

    जर( k == CUDA_GPU_MAT )
        परत ((स्थिरांक cuda::GpuMat*)obj)->type();

    जर( k == CUDA_GPU_MATND )
        परत ((स्थिरांक cuda::GpuMatND*)obj)->type();

    जर( k == CUDA_HOST_MEM )
        परत ((स्थिरांक cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णांक आदान_Array_2::depth_(पूर्णांक i) स्थिरांक
{
    परत CV_MAT_DEPTH(type(i));
}

तार्किक आदान_Array_2::empty_() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
        परत ((स्थिरांक Mat*)obj)->empty();

    जर( k == UMAT )
        परत ((स्थिरांक UMat*)obj)->empty();

    जर (k == MATX)
        परत असत्य;

    जर( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        माप_प्रकार n = total(-1);
        परत n == 0;
    }

    जर( k == NONE )
        परत सत्य;

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        परत vv.empty();
    }

    जर( k == STD_ARRAY_MAT )
    {
        परत sz.height == 0;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        परत vv.empty();
    }

    जर( k == OPENGL_BUFFER )
        परत ((स्थिरांक ogl::Buffer*)obj)->empty();

    जर( k == CUDA_GPU_MAT )
        परत ((स्थिरांक cuda::GpuMat*)obj)->empty();

    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        स्थिरांक std::vector<cuda::GpuMat>& vv = *(स्थिरांक std::vector<cuda::GpuMat>*)obj;
        परत vv.empty();
    }

    जर( k == CUDA_GPU_MATND )
        परत ((स्थिरांक cuda::GpuMatND*)obj)->empty();

    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        स्थिरांक std::vector<cuda::GpuMatND>& vv = *(स्थिरांक std::vector<cuda::GpuMatND>*)obj;
        परत vv.empty();
    }

    जर( k == CUDA_HOST_MEM )
        परत ((स्थिरांक cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

तार्किक आदान_Array_2::isContinuous(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
        परत i < 0 ? ((स्थिरांक Mat*)obj)->isContinuous() : सत्य;

    जर( k == UMAT )
        परत i < 0 ? ((स्थिरांक UMat*)obj)->isContinuous() : सत्य;

    जर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        परत सत्य;

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].isContinuous();
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        परत vv[i].isContinuous();
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].isContinuous();
    }

    जर( k == CUDA_GPU_MAT )
      परत i < 0 ? ((स्थिरांक cuda::GpuMat*)obj)->isContinuous() : सत्य;

    जर( k == CUDA_GPU_MATND )
        परत i < 0 ? ((स्थिरांक cuda::GpuMatND*)obj)->isContinuous() : सत्य;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

तार्किक आदान_Array_2::isSubmatrix(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
        परत i < 0 ? ((स्थिरांक Mat*)obj)->isSubmatrix() : असत्य;

    जर( k == UMAT )
        परत i < 0 ? ((स्थिरांक UMat*)obj)->isSubmatrix() : असत्य;

    जर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        परत असत्य;

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].isSubmatrix();
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        परत vv[i].isSubmatrix();
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].isSubmatrix();
    }

    जर( k == CUDA_GPU_MATND )
    {
        परत ((स्थिरांक cuda::GpuMatND*)obj)->isSubmatrix();
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        स्थिरांक std::vector<cuda::GpuMatND>& vv = *(स्थिरांक std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

माप_प्रकार आदान_Array_2::offset(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक Mat * स्थिरांक m = ((स्थिरांक Mat*)obj);
        परत (माप_प्रकार)(m->ptr() - m->datastart);
    }

    जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक UMat*)obj)->offset;
    }

    जर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        परत 0;

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णांक)vv.size() );

        परत (माप_प्रकार)(vv[i].ptr() - vv[i].datastart);
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        परत (माप_प्रकार)(vv[i].ptr() - vv[i].datastart);
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].offset;
    }

    जर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        स्थिरांक cuda::GpuMat * स्थिरांक m = ((स्थिरांक cuda::GpuMat*)obj);
        परत (माप_प्रकार)(m->data - m->datastart);
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        स्थिरांक std::vector<cuda::GpuMat>& vv = *(स्थिरांक std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत (माप_प्रकार)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

माप_प्रकार आदान_Array_2::step(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक Mat*)obj)->step;
    }

    जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक UMat*)obj)->step;
    }

    जर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        परत 0;

    जर( k == STD_VECTOR_MAT )
    {
        स्थिरांक std::vector<Mat>& vv = *(स्थिरांक std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णांक)vv.size() );
        परत vv[i].step;
    }

    जर( k == STD_ARRAY_MAT )
    {
        स्थिरांक Mat* vv = (स्थिरांक Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        परत vv[i].step;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        स्थिरांक std::vector<UMat>& vv = *(स्थिरांक std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].step;
    }

    जर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        परत ((स्थिरांक cuda::GpuMat*)obj)->step;
    }
    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        स्थिरांक std::vector<cuda::GpuMat>& vv = *(स्थिरांक std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        परत vv[i].step;
    }
    जर( k == CUDA_GPU_MATND )
    {
        स्थिरांक cuda::GpuMatND& _gpuMatND = *(स्थिरांक cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        परत _gpuMatND.step[i];
    }

    जर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        स्थिरांक std::vector<cuda::GpuMatND>& vv = *(स्थिरांक std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        स्थिरांक cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        परत _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

शून्य आदान_Array_2::copyTo(स्थिरांक _OutputArray& arr) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == NONE )
        arr.release();
    अन्यथा जर( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    अन्यथा जर( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    अन्यथा जर (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

शून्य आदान_Array_2::copyTo(स्थिरांक _OutputArray& arr, स्थिरांक आदान_Array_2 & mask) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == NONE )
        arr.release();
    अन्यथा जर( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    अन्यथा जर( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    अन्यथा जर (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

तार्किक _OutputArray::fixedSize() स्थिरांक
{
    परत (flags & FIXED_SIZE) == FIXED_SIZE;
}

तार्किक _OutputArray::fixedType() स्थिरांक
{
    परत (flags & FIXED_TYPE) == FIXED_TYPE;
}

शून्य _OutputArray::create_(Size _sz, पूर्णांक mtype, पूर्णांक i, तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.चालक()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        परत;
    }
    जर( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.चालक()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        परत;
    }
    जर( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    जर( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    जर( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    जर( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    पूर्णांक sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::create_(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक mtype, पूर्णांक i, तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.चालक()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        परत;
    }
    जर( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.चालक()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        परत;
    }
    जर( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    जर( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    जर( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    जर( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    पूर्णांक sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::create(पूर्णांक d, स्थिरांक पूर्णांक* sizes, पूर्णांक mtype, पूर्णांक i,
                          तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    पूर्णांक size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    आदान_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    जर( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        जर (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        जर(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        परत;
    }

    जर( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        जर (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        जर(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        परत;
    }

    जर( k == MATX )
    {
        CV_Assert( i < 0 );
        पूर्णांक type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        जर (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            पूर्णांक total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        अन्यथा
        {
            जर (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            अन्यथा
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        परत;
    }

    जर( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        माप_प्रकार len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        पूर्णांक esz = CV_ELEM_SIZE(flags);

        जर( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            पूर्णांक type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            जर (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                जर (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    परत; \
                } \
                CV_Assert(माप_प्रकार(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        थांबा

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        परत;
    }

    जर( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    जर( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        जर( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            जर( fixedType() )
            {
                पूर्णांक _type = CV_MAT_TYPE(flags);
                साठी( माप_प्रकार j = len0; j < len; j++ )
                {
                    जर( v[j].type() == _type )
                        सुरूठेवा;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            परत;
        }

        CV_Assert( i < (पूर्णांक)v.size() );
        Mat& m = v[i];

        जर( allowTransposed )
        {
            जर( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            जर( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        जर(fixedSize())
        {
            CV_Assert(m.dims == d);
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        परत;
    }

    जर( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        जर( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            जर( fixedType() )
            {
                पूर्णांक _type = CV_MAT_TYPE(flags);
                साठी( माप_प्रकार j = len0; j < len; j++ )
                {
                    जर( v[j].type() == _type )
                        सुरूठेवा;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            परत;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        जर( allowTransposed )
        {
            जर( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            जर( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        जर(fixedSize())
        {
            CV_Assert(m.dims == d);
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        परत;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        जर( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            जर( fixedType() )
            {
                पूर्णांक _type = CV_MAT_TYPE(flags);
                साठी( माप_प्रकार j = len0; j < len; j++ )
                {
                    जर( v[j].type() == _type )
                        सुरूठेवा;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            परत;
        }

        CV_Assert( i < (पूर्णांक)v.size() );
        UMat& m = v[i];

        जर( allowTransposed )
        {
            जर( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            जर( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        जर(fixedSize())
        {
            CV_Assert(m.dims == d);
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        परत;
    }

    जर ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        परत;
    }

    जर( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य _OutputArray::create(स्थिरांक MatShape& shape, पूर्णांक mtype, पूर्णांक i,
                          तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    जर (shape.dims < 0) {
        release();
    } अन्यथा {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(पूर्णांक mtype) स्थिरांक
{
    mtype = CV_MAT_TYPE(mtype);
    परत getMat().reinterpret(mtype);
}

शून्य _OutputArray::createSameSize(स्थिरांक आदान_Array_2& arr, पूर्णांक mtype) स्थिरांक
{
    create(arr.shape(), mtype);
}

शून्य _OutputArray::fit(Size _sz, पूर्णांक mtype, पूर्णांक i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    पूर्णांक sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::fit(पूर्णांक rows, पूर्णांक cols, पूर्णांक mtype, पूर्णांक i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    पूर्णांक sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::fit(स्थिरांक MatShape& shape, पूर्णांक mtype, पूर्णांक i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    जर (shape.dims < 0) {
        release();
        परत;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    आदान_Array_2::KindFlag k = kind();
    जर ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } अन्यथा जर ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

शून्य _OutputArray::fitSameSize(स्थिरांक आदान_Array_2& arr, पूर्णांक mtype) स्थिरांक
{
    fit(arr.shape(), mtype);
}

शून्य _OutputArray::fit(पूर्णांक d, स्थिरांक पूर्णांक* sizes, पूर्णांक mtype, पूर्णांक i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिरांक
{
    पूर्णांक size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    आदान_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    जर( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        जर (k == MAT)
            m = (Mat*)obj;
        अन्यथा {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((माप_प्रकार)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        जर (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        जर(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        परत;
    }

    जर( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        जर (k == UMAT)
            m = (UMat*)obj;
        अन्यथा {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((माप_प्रकार)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        जर (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            परत;
        }

        जर(fixedType())
        {
            जर(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        जर(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            साठी(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        परत;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::release() स्थिरांक
{
    CV_Assert(!fixedSize());

    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        ((Mat*)obj)->release();
        परत;
    }

    जर( k == UMAT )
    {
        ((UMat*)obj)->release();
        परत;
    }

    जर( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    जर( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    जर( k == NONE )
        परत;

    जर( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        जर (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } अन्यथा { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        थांबा

        पूर्णांक esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        परत;
    }

    जर( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        परत;
    }

    जर( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        साठी( पूर्णांक i = 0; i < sz.height; i++ )
            v[i].release();
        परत;
    }

    जर( k == STD_BOOL_VECTOR )
    {
        ((std::vector<तार्किक>*)obj)->clear();
        परत;
    }

    जर( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        परत;
    }
    जर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        परत;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य _OutputArray::clear() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        परत;
    }

    release();
}

तार्किक _OutputArray::needed() स्थिरांक
{
    परत kind() != NONE;
}

Mat& _OutputArray::getMatRef(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर( i < 0 )
    {
        CV_Assert( k == MAT );
        परत *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    जर( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णांक)v.size() );
        परत v[i];
    }
    अन्यथा
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        परत v[i];
    }
}

UMat& _OutputArray::getUMatRef(पूर्णांक i) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर( i < 0 )
    {
        CV_Assert( k == UMAT );
        परत *(UMat*)obj;
    }
    अन्यथा
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णांक)v.size() );
        परत v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    परत *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    परत *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    परत *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    परत *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    परत *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    परत *(cuda::HostMem*)obj;
}

शून्य _OutputArray::setTo(स्थिरांक आदान_Array_2& arr, स्थिरांक आदान_Array_2 & mask) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == NONE )
        ;
    अन्यथा जर (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    अन्यथा जर( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    अन्यथा जर( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), आदान_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<द्विगुण, 4>(value.ptr<द्विगुण>())), mask);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

शून्य _OutputArray::setZero() स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();

    जर( k == NONE )
        ;
    अन्यथा जर (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    अन्यथा
    {
        setTo(Scalar::all(0), noArray());
    }
}

शून्य _OutputArray::assign(स्थिरांक UMat& u) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    अन्यथा जर (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    अन्यथा जर (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(स्थिरांक Mat& m) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    अन्यथा जर (k == MAT)
    {
        *(Mat*)obj = m;
    }
    अन्यथा जर (k == MATX)
    {
        m.copyTo(getMat());
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::move(UMat& u) स्थिरांक
{
    जर (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        परत;
    }
    पूर्णांक k = kind();
    जर (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    अन्यथा जर (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    अन्यथा जर (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::move(Mat& m) स्थिरांक
{
    जर (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        परत;
    }
    पूर्णांक k = kind();
    जर (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    अन्यथा जर (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    अन्यथा जर (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(स्थिरांक std::vector<UMat>& v) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        साठी (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिरांक UMat& m = v[i];
            UMat& this_m = this_v[i];
            जर (this_m.u != रिक्त && this_m.u == m.u)
                सुरूठेवा; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा जर (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        साठी (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिरांक UMat& m = v[i];
            Mat& this_m = this_v[i];
            जर (this_m.u != रिक्त && this_m.u == m.u)
                सुरूठेवा; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(स्थिरांक std::vector<Mat>& v) स्थिरांक
{
    आदान_Array_2::KindFlag k = kind();
    जर (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        साठी (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिरांक Mat& m = v[i];
            UMat& this_m = this_v[i];
            जर (this_m.u != रिक्त && this_m.u == m.u)
                सुरूठेवा; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा जर (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        साठी (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिरांक Mat& m = v[i];
            Mat& this_m = this_v[i];
            जर (this_m.u != रिक्त && this_m.u == m.u)
                सुरूठेवा; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


स्थिर आदान_Output_Array_2 _none;
आदान_Output_Array noArray() { परत _none; }

} // cv::
