// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/mat.hpp"

নামাকাশ cv {

প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 5> Vec5i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 7> Vec7i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 9> Vec9i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 10> Vec10i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 11> Vec11i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 12> Vec12i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 13> Vec13i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 14> Vec14i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 15> Vec15i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 16> Vec16i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 32> Vec32i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 64> Vec64i;
প্রকারনির্দেশ Vec<পূর্ণাঙ্ক, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    বিকল্প( esz ) \
    { \
    পক্ষ 1: func(uchar); \
    পক্ষ 2: func(Vec2b); \
    পক্ষ 3: func(Vec3b); \
    পক্ষ 4: func(পূর্ণাঙ্ক); \
    পক্ষ 6: func(Vec3s); \
    পক্ষ 8: func(Vec2i); \
    পক্ষ 12: func(Vec3i); \
    পক্ষ 16: func(Vec4i); \
    পক্ষ 20: func(Vec5i); \
    পক্ষ 24: func(Vec6i); \
    পক্ষ 28: func(Vec7i); \
    পক্ষ 32: func(Vec8i); \
    পক্ষ 36: func(Vec9i); \
    পক্ষ 40: func(Vec10i); \
    পক্ষ 44: func(Vec11i); \
    পক্ষ 48: func(Vec12i); \
    পক্ষ 52: func(Vec13i); \
    পক্ষ 56: func(Vec14i); \
    পক্ষ 60: func(Vec15i); \
    পক্ষ 64: func(Vec16i); \
    পক্ষ 128: func(Vec32i); \
    পক্ষ 256: func(Vec64i); \
    পক্ষ 512: func(Vec128i); \
    স্বভাব: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat প্রবেশ_Array_2::getMat_(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == MAT )
    {
        ধ্রুব Mat* m = (ধ্রুব Mat*)obj;
        যদি( i < 0 )
            প্রত্যাগম *m;
        প্রত্যাগম m->row(i);
    }

    যদি( k == UMAT )
    {
        ধ্রুব UMat* m = (ধ্রুব UMat*)obj;
        যদি( i < 0 )
            প্রত্যাগম m->getMat(accessFlags);
        প্রত্যাগম m->getMat(accessFlags).row(i);
    }

    যদি (k == MATX)
    {
        CV_Assert( i < 0 );
        প্রত্যাগম Mat(sz, flags, obj);
    }

    যদি( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        পূর্ণাঙ্ক t = CV_8U;
        ধ্রুব std::vector<তার্কিক>& v = *(ধ্রুব std::vector<তার্কিক>*)obj;
        পূর্ণাঙ্ক j, n = (পূর্ণাঙ্ক)v.size();
        যদি( n == 0 )
            প্রত্যাগম Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        প্রতি( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        প্রত্যাগম m;
    }

    যদি( k == NONE )
        প্রত্যাগম Mat();

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        পূর্ণাঙ্ক t = CV_MAT_TYPE(flags);
        পূর্ণাঙ্ক esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            যদি (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } অন্যথা { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            পূর্ণাঙ্ক v_sz = পূর্ণাঙ্ক(v->size()); \
            প্রত্যাগম v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& v = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (পূর্ণাঙ্ক)v.size() );

        প্রত্যাগম v[i];
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* v = (ধ্রুব Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        প্রত্যাগম v[i];
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& v = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (পূর্ণাঙ্ক)v.size() );

        প্রত্যাগম v[i].getMat(accessFlags);
    }

    যদি( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    যদি( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    যদি( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ধ্রুব cuda::HostMem* cuda_mem = (ধ্রুব cuda::HostMem*)obj;

        প্রত্যাগম cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat প্রবেশ_Array_2::getUMat(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == UMAT )
    {
        ধ্রুব UMat* m = (ধ্রুব UMat*)obj;
        যদি( i < 0 )
            প্রত্যাগম *m;
        প্রত্যাগম m->row(i);
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& v = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (পূর্ণাঙ্ক)v.size() );

        প্রত্যাগম v[i];
    }

    যদি( k == MAT )
    {
        ধ্রুব Mat* m = (ধ্রুব Mat*)obj;
        যদি( i < 0 )
            প্রত্যাগম m->getUMat(accessFlags);
        প্রত্যাগম m->row(i).getUMat(accessFlags);
    }

    প্রত্যাগম getMat(i).getUMat(accessFlags);
}

শূন্য প্রবেশ_Array_2::getMatVector(std::vector<Mat>& mv) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == MAT )
    {
        ধ্রুব Mat& m = *(ধ্রুব Mat*)obj;
        পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        প্রতি( পূর্ণাঙ্ক i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (শূন্য*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (শূন্য*)m.ptr(i), &m.step[1]);
        প্রত্যাগম;
    }

    যদি (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR )
    {
        ধ্রুব std::vector<uchar>& v = *(ধ্রুব std::vector<uchar>*)obj;

        size_t n = total(), esz = CV_ELEM_SIZE(flags);
        পূর্ণাঙ্ক t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (শূন্য*)(v.data() + esz*i));
        প্রত্যাগম;
    }

    যদি( k == NONE )
    {
        mv.clear();
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_VECTOR )
    {
        পূর্ণাঙ্ক typ = CV_MAT_TYPE(flags);
        পূর্ণাঙ্ক esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            ধ্রুব std::vector<std::vector<T > >* vv = (ধ্রুব std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            প্রতি (size_t i = 0; i < n; i++) { \
                ধ্রুব std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                পূর্ণাঙ্ক ni = (পূর্ণাঙ্ক)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (শূন্য*)vi.data()) : Mat(); \
            } \
        } \
        ভঙ্গ

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& v = *(ধ্রুব std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        প্রত্যাগম;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* v = (ধ্রুব Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& v = *(ধ্রুব std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        প্রত্যাগম;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

শূন্য প্রবেশ_Array_2::getUMatVector(std::vector<UMat>& umv) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == NONE )
    {
        umv.clear();
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& v = *(ধ্রুব std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        প্রত্যাগম;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* v = (ধ্রুব Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& v = *(ধ্রুব std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        প্রতি( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        প্রত্যাগম;
    }

    যদি( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        প্রত্যাগম;
    }
    যদি( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        প্রত্যাগম;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat প্রবেশ_Array_2::getGpuMat() ধ্রুব
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি (k == CUDA_GPU_MAT)
    {
        ধ্রুব cuda::GpuMat* d_mat = (ধ্রুব cuda::GpuMat*)obj;
        প্রত্যাগম *d_mat;
    }

    যদি (k == CUDA_HOST_MEM)
    {
        ধ্রুব cuda::HostMem* cuda_mem = (ধ্রুব cuda::HostMem*)obj;
        প্রত্যাগম cuda_mem->createGpuMatHeader();
    }

    যদি (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    যদি (k == NONE)
        প্রত্যাগম cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#অন্যথা
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
শূন্য প্রবেশ_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) ধ্রুব
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#অন্যথা
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
শূন্য প্রবেশ_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ধ্রুব
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#অন্যথা
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND প্রবেশ_Array_2::getGpuMatND() ধ্রুব
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি (k == CUDA_GPU_MATND)
    {
        ধ্রুব cuda::GpuMatND* d_mat = (ধ্রুব cuda::GpuMatND*)obj;
        প্রত্যাগম *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#অন্যথা
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer প্রবেশ_Array_2::getOGlBuffer() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    ধ্রুব ogl::Buffer* gl_buf = (ধ্রুব ogl::Buffer*)obj;
    প্রত্যাগম *gl_buf;
}

পূর্ণাঙ্ক প্রবেশ_Array_2::rows_(পূর্ণাঙ্ক i) ধ্রুব
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == CUDA_GPU_MATND)
    {
        ধ্রুব cuda::GpuMatND& _gpuMatND = *(ধ্রুব cuda::GpuMatND*)obj;
        প্রত্যাগম (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    প্রত্যাগম size_(i).height;
}

পূর্ণাঙ্ক প্রবেশ_Array_2::cols_(পূর্ণাঙ্ক i) ধ্রুব
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == CUDA_GPU_MATND)
    {
        ধ্রুব cuda::GpuMatND& _gpuMatND = *(ধ্রুব cuda::GpuMatND*)obj;
        প্রত্যাগম (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    প্রত্যাগম size_(i).width;
}

Size প্রবেশ_Array_2::size_(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব Mat* m = (ধ্রুব Mat*)obj;
        CV_Assert(m->dims <= 2);
        প্রত্যাগম Size(m->cols, m->rows);
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব UMat* m = (ধ্রুব UMat*)obj;
        CV_Assert(m->dims <= 2);
        প্রত্যাগম Size(m->cols, m->rows);
    }

    যদি (k == MATX)
    {
        CV_Assert( i < 0 );
        প্রত্যাগম sz;
    }

    যদি( k == NONE )
        প্রত্যাগম Size();

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = total(i);
        CV_Assert(n <= (size_t)INT_MAX);
        প্রত্যাগম Size((পূর্ণাঙ্ক)n, 1);
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        যদি( i < 0 )
            প্রত্যাগম vv.empty() ? Size() : Size((পূর্ণাঙ্ক)vv.size(), 1);
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );

        প্রত্যাগম vv[i].size();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        যদি( i < 0 )
            প্রত্যাগম sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        প্রত্যাগম vv[i].size();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ধ্রুব std::vector<cuda::GpuMat>& vv = *(ধ্রুব std::vector<cuda::GpuMat>*)obj;
        যদি (i < 0)
            প্রত্যাগম vv.empty() ? Size() : Size((পূর্ণাঙ্ক)vv.size(), 1);
        CV_Assert(i < (পূর্ণাঙ্ক)vv.size());
        প্রত্যাগম vv[i].size();
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ধ্রুব std::vector<cuda::GpuMatND>& vv = *(ধ্রুব std::vector<cuda::GpuMatND>*)obj;
        যদি (i < 0)
            প্রত্যাগম vv.empty() ? Size() : Size((পূর্ণাঙ্ক)vv.size(), 1);
        CV_Assert(i < (পূর্ণাঙ্ক)vv.size());
        ধ্রুব cuda::GpuMatND& m = vv[i];
        যদি (m.dims == 0)
            প্রত্যাগম Size();
        যদি (m.dims == 1)
            প্রত্যাগম Size(1, m.size[0]);
        প্রত্যাগম Size(m.size[1], m.size[0]);
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        যদি( i < 0 )
            প্রত্যাগম vv.empty() ? Size() : Size((পূর্ণাঙ্ক)vv.size(), 1);
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );

        প্রত্যাগম vv[i].size();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ধ্রুব std::vector<cuda::GpuMatND>& vv = *(ধ্রুব std::vector<cuda::GpuMatND>*)obj;
        যদি (i < 0)
            প্রত্যাগম vv.empty() ? Size() : Size((পূর্ণাঙ্ক)vv.size(), 1);
        CV_Assert(i < (পূর্ণাঙ্ক)vv.size());
        ধ্রুব cuda::GpuMatND& m = vv[i];
        যদি (m.dims == 0)
            প্রত্যাগম Size();
        যদি (m.dims == 1)
            প্রত্যাগম Size(1, m.size[0]);
        প্রত্যাগম Size(m.size[1], m.size[0]);
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        ধ্রুব ogl::Buffer* buf = (ধ্রুব ogl::Buffer*)obj;
        প্রত্যাগম buf->size();
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব cuda::GpuMat* d_mat = (ধ্রুব cuda::GpuMat*)obj;
        প্রত্যাগম d_mat->size();
    }

    যদি( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ধ্রুব cuda::HostMem* cuda_mem = (ধ্রুব cuda::HostMem*)obj;
        প্রত্যাগম cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

পূর্ণাঙ্ক প্রবেশ_Array_2::sizend(পূর্ণাঙ্ক* arrsz, পূর্ণাঙ্ক i) ধ্রুব
{
    পূর্ণাঙ্ক j, d = 0;
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == NONE )
        ;
    অন্যথা যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব Mat& m = *(ধ্রুব Mat*)obj;
        d = m.dims;
        যদি(arrsz)
            প্রতি(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব UMat& m = *(ধ্রুব UMat*)obj;
        d = m.dims;
        যদি(arrsz)
            প্রতি(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == STD_VECTOR_MAT && i >= 0 )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );
        ধ্রুব Mat& m = vv[i];
        d = m.dims;
        যদি(arrsz)
            প্রতি(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == STD_ARRAY_MAT && i >= 0 )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        CV_Assert( i < sz.height );
        ধ্রুব Mat& m = vv[i];
        d = m.dims;
        যদি(arrsz)
            প্রতি(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );
        ধ্রুব UMat& m = vv[i];
        d = m.dims;
        যদি(arrsz)
            প্রতি(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = total(i);
        d = 1;
        যদি(arrsz)
        {
            arrsz[0] = পূর্ণাঙ্ক(n);
        }
    }
    অন্যথা
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        যদি(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    প্রত্যাগম d;
}

তার্কিক প্রবেশ_Array_2::empty(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (i >= 0) {
        যদি (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<ধ্রুব std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            প্রত্যাগম mv->at(i).empty();
        }
        অন্যথা যদি (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<ধ্রুব std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            প্রত্যাগম umv->at(i).empty();
        }
        অন্যথা যদি (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = total(i);
            প্রত্যাগম n == 0;
        } অন্যথা {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    প্রত্যাগম empty();
}

MatShape প্রবেশ_Array_2::shape(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    MatShape shape;

    যদি( k == NONE )
        ;
    অন্যথা যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব Mat& m = *(ধ্রুব Mat*)obj;
        shape = m.size;
    }
    অন্যথা যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব UMat& m = *(ধ্রুব UMat*)obj;
        shape = m.size;
    }
    অন্যথা যদি( k == STD_VECTOR_MAT && i >= 0 )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ধ্রুব Mat& m = vv[i];
        shape = m.size;
    }
    অন্যথা যদি( k == STD_ARRAY_MAT && i >= 0 )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        CV_Assert( i < sz.height );
        ধ্রুব Mat& m = vv[i];
        shape = m.size;
    }
    অন্যথা যদি( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ধ্রুব UMat& m = vv[i];
        shape = m.size;
    }
    অন্যথা
    {
        পূর্ণাঙ্ক sizes[CV_MAX_DIM];
        পূর্ণাঙ্ক dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    প্রত্যাগম shape;
}

তার্কিক প্রবেশ_Array_2::sameSize(ধ্রুব প্রবেশ_Array_2& arr) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    যদি( k1 == MAT )
    {
        ধ্রুব Mat* m = ((ধ্রুব Mat*)obj);
        যদি( k2 == MAT )
            প্রত্যাগম m->size == ((ধ্রুব Mat*)arr.obj)->size;
        যদি( k2 == UMAT )
            প্রত্যাগম m->size == ((ধ্রুব UMat*)arr.obj)->size;
        যদি( m->dims > 2 )
            প্রত্যাগম অসত্য;
        sz1 = m->size();
    }
    অন্যথা যদি( k1 == UMAT )
    {
        ধ্রুব UMat* m = ((ধ্রুব UMat*)obj);
        যদি( k2 == MAT )
            প্রত্যাগম m->size == ((ধ্রুব Mat*)arr.obj)->size;
        যদি( k2 == UMAT )
            প্রত্যাগম m->size == ((ধ্রুব UMat*)arr.obj)->size;
        যদি( m->dims > 2 )
            প্রত্যাগম অসত্য;
        sz1 = m->size();
    }
    অন্যথা যদি ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        প্রত্যাগম ((ধ্রুব cuda::GpuMatND*)obj)->size == ((ধ্রুব cuda::GpuMatND*)arr.obj)->size;
    }
    অন্যথা
        sz1 = size();

    যদি( arr.dims() > 2 )
        প্রত্যাগম অসত্য;
    প্রত্যাগম sz1 == arr.size();
}

পূর্ণাঙ্ক প্রবেশ_Array_2::dims_(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব Mat*)obj)->dims;
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব UMat*)obj)->dims;
    }

    যদি (k == MATX)
    {
        CV_Assert( i < 0 );
        প্রত্যাগম 2;
    }

    যদি( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম 1;
    }

    যদি( k == NONE )
        প্রত্যাগম 0;

    যদি( k == STD_VECTOR_VECTOR )
    {
        যদি( i < 0 )
            প্রত্যাগম 1;
        CV_Assert( size_t(i) < total() );
        প্রত্যাগম 1;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        যদি( i < 0 )
            প্রত্যাগম 1;
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );

        প্রত্যাগম vv[i].dims;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        যদি( i < 0 )
            প্রত্যাগম 1;
        CV_Assert( i < sz.height );

        প্রত্যাগম vv[i].dims;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        যদি( i < 0 )
            প্রত্যাগম 1;
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );

        প্রত্যাগম vv[i].dims;
    }

    যদি( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম 2;
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম 2;
    }

    যদি( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব cuda::GpuMatND*)obj)->dims;
    }

    যদি( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t প্রবেশ_Array_2::total(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব Mat*)obj)->total();
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব UMat*)obj)->total();
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        যদি( i < 0 )
            প্রত্যাগম vv.size();

        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );
        প্রত্যাগম vv[i].total();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        যদি( i < 0 )
            প্রত্যাগম sz.height;

        CV_Assert( i < sz.height );
        প্রত্যাগম vv[i].total();
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        যদি( i < 0 )
            প্রত্যাগম vv.size();

        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );
        প্রত্যাগম vv[i].total();
    }

    যদি( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ধ্রুব cuda::GpuMatND& _gpuMatND = *((ধ্রুব cuda::GpuMatND*)obj);
        যদি (_gpuMatND.dims > 0)
        {
            res = 1;
            প্রতি(পূর্ণাঙ্ক d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            প্রত্যাগম res;
        }
    }

    যদি (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        পূর্ণাঙ্ক esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            যদি (k == STD_VECTOR_VECTOR) { \
                ধ্রুব std::vector<std::vector<T > >* vv = (ধ্রুব std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                যদি (i < 0) \
                    প্রত্যাগম n; \
                CV_Assert((size_t)i < n); \
                প্রত্যাগম vv->at(i).size(); \
            } অন্যথা \
                প্রত্যাগম ((ধ্রুব std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    প্রত্যাগম size(i).area();
}

পূর্ণাঙ্ক প্রবেশ_Array_2::type_(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        প্রত্যাগম ((ধ্রুব Mat*)obj)->type();

    যদি( k == UMAT )
        প্রত্যাগম ((ধ্রুব UMat*)obj)->type();

    যদি( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        প্রত্যাগম CV_MAT_TYPE(flags);

    যদি( k == NONE )
        প্রত্যাগম -1;

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        যদি( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            প্রত্যাগম CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );
        প্রত্যাগম vv[i >= 0 ? i : 0].type();
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        যদি( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            প্রত্যাগম CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (পূর্ণাঙ্ক)vv.size() );
        প্রত্যাগম vv[i >= 0 ? i : 0].type();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        যদি( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            প্রত্যাগম CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        প্রত্যাগম vv[i >= 0 ? i : 0].type();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ধ্রুব std::vector<cuda::GpuMat>& vv = *(ধ্রুব std::vector<cuda::GpuMat>*)obj;
        যদি (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            প্রত্যাগম CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (পূর্ণাঙ্ক)vv.size());
        প্রত্যাগম vv[i >= 0 ? i : 0].type();
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ধ্রুব std::vector<cuda::GpuMatND>& vv = *(ধ্রুব std::vector<cuda::GpuMatND>*)obj;
        যদি (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            প্রত্যাগম CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (পূর্ণাঙ্ক)vv.size());
        প্রত্যাগম vv[i >= 0 ? i : 0].type();
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == OPENGL_BUFFER )
        প্রত্যাগম ((ধ্রুব ogl::Buffer*)obj)->type();

    যদি( k == CUDA_GPU_MAT )
        প্রত্যাগম ((ধ্রুব cuda::GpuMat*)obj)->type();

    যদি( k == CUDA_GPU_MATND )
        প্রত্যাগম ((ধ্রুব cuda::GpuMatND*)obj)->type();

    যদি( k == CUDA_HOST_MEM )
        প্রত্যাগম ((ধ্রুব cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

পূর্ণাঙ্ক প্রবেশ_Array_2::depth_(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রত্যাগম CV_MAT_DEPTH(type(i));
}

তার্কিক প্রবেশ_Array_2::empty_() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        প্রত্যাগম ((ধ্রুব Mat*)obj)->empty();

    যদি( k == UMAT )
        প্রত্যাগম ((ধ্রুব UMat*)obj)->empty();

    যদি (k == MATX)
        প্রত্যাগম অসত্য;

    যদি( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = total(-1);
        প্রত্যাগম n == 0;
    }

    যদি( k == NONE )
        প্রত্যাগম সত্য;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        প্রত্যাগম vv.empty();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        প্রত্যাগম sz.height == 0;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        প্রত্যাগম vv.empty();
    }

    যদি( k == OPENGL_BUFFER )
        প্রত্যাগম ((ধ্রুব ogl::Buffer*)obj)->empty();

    যদি( k == CUDA_GPU_MAT )
        প্রত্যাগম ((ধ্রুব cuda::GpuMat*)obj)->empty();

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ধ্রুব std::vector<cuda::GpuMat>& vv = *(ধ্রুব std::vector<cuda::GpuMat>*)obj;
        প্রত্যাগম vv.empty();
    }

    যদি( k == CUDA_GPU_MATND )
        প্রত্যাগম ((ধ্রুব cuda::GpuMatND*)obj)->empty();

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ধ্রুব std::vector<cuda::GpuMatND>& vv = *(ধ্রুব std::vector<cuda::GpuMatND>*)obj;
        প্রত্যাগম vv.empty();
    }

    যদি( k == CUDA_HOST_MEM )
        প্রত্যাগম ((ধ্রুব cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

তার্কিক প্রবেশ_Array_2::isContinuous(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        প্রত্যাগম i < 0 ? ((ধ্রুব Mat*)obj)->isContinuous() : সত্য;

    যদি( k == UMAT )
        প্রত্যাগম i < 0 ? ((ধ্রুব UMat*)obj)->isContinuous() : সত্য;

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        প্রত্যাগম সত্য;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].isContinuous();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        প্রত্যাগম vv[i].isContinuous();
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].isContinuous();
    }

    যদি( k == CUDA_GPU_MAT )
      প্রত্যাগম i < 0 ? ((ধ্রুব cuda::GpuMat*)obj)->isContinuous() : সত্য;

    যদি( k == CUDA_GPU_MATND )
        প্রত্যাগম i < 0 ? ((ধ্রুব cuda::GpuMatND*)obj)->isContinuous() : সত্য;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

তার্কিক প্রবেশ_Array_2::isSubmatrix(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        প্রত্যাগম i < 0 ? ((ধ্রুব Mat*)obj)->isSubmatrix() : অসত্য;

    যদি( k == UMAT )
        প্রত্যাগম i < 0 ? ((ধ্রুব UMat*)obj)->isSubmatrix() : অসত্য;

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        প্রত্যাগম অসত্য;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].isSubmatrix();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        প্রত্যাগম vv[i].isSubmatrix();
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].isSubmatrix();
    }

    যদি( k == CUDA_GPU_MATND )
    {
        প্রত্যাগম ((ধ্রুব cuda::GpuMatND*)obj)->isSubmatrix();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ধ্রুব std::vector<cuda::GpuMatND>& vv = *(ধ্রুব std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t প্রবেশ_Array_2::offset(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব Mat * ধ্রুব m = ((ধ্রুব Mat*)obj);
        প্রত্যাগম (size_t)(m->ptr() - m->datastart);
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব UMat*)obj)->offset;
    }

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        প্রত্যাগম 0;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (পূর্ণাঙ্ক)vv.size() );

        প্রত্যাগম (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        প্রত্যাগম (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].offset;
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুব cuda::GpuMat * ধ্রুব m = ((ধ্রুব cuda::GpuMat*)obj);
        প্রত্যাগম (size_t)(m->data - m->datastart);
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ধ্রুব std::vector<cuda::GpuMat>& vv = *(ধ্রুব std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম (size_t)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t প্রবেশ_Array_2::step(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব Mat*)obj)->step;
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব UMat*)obj)->step;
    }

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        প্রত্যাগম 0;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুব std::vector<Mat>& vv = *(ধ্রুব std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (পূর্ণাঙ্ক)vv.size() );
        প্রত্যাগম vv[i].step;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুব Mat* vv = (ধ্রুব Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        প্রত্যাগম vv[i].step;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুব std::vector<UMat>& vv = *(ধ্রুব std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].step;
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        প্রত্যাগম ((ধ্রুব cuda::GpuMat*)obj)->step;
    }
    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ধ্রুব std::vector<cuda::GpuMat>& vv = *(ধ্রুব std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        প্রত্যাগম vv[i].step;
    }
    যদি( k == CUDA_GPU_MATND )
    {
        ধ্রুব cuda::GpuMatND& _gpuMatND = *(ধ্রুব cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        প্রত্যাগম _gpuMatND.step[i];
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ধ্রুব std::vector<cuda::GpuMatND>& vv = *(ধ্রুব std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ধ্রুব cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        প্রত্যাগম _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

শূন্য প্রবেশ_Array_2::copyTo(ধ্রুব _OutputArray& arr) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == NONE )
        arr.release();
    অন্যথা যদি( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    অন্যথা যদি( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    অন্যথা যদি (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    অন্যথা
        CV_Error(Error::StsNotImplemented, "");
}

শূন্য প্রবেশ_Array_2::copyTo(ধ্রুব _OutputArray& arr, ধ্রুব প্রবেশ_Array_2 & mask) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == NONE )
        arr.release();
    অন্যথা যদি( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    অন্যথা যদি( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    অন্যথা যদি (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    অন্যথা
        CV_Error(Error::StsNotImplemented, "");
}

তার্কিক _OutputArray::fixedSize() ধ্রুব
{
    প্রত্যাগম (flags & FIXED_SIZE) == FIXED_SIZE;
}

তার্কিক _OutputArray::fixedType() ধ্রুব
{
    প্রত্যাগম (flags & FIXED_TYPE) == FIXED_TYPE;
}

শূন্য _OutputArray::create_(Size _sz, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i, তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        প্রত্যাগম;
    }
    যদি( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        প্রত্যাগম;
    }
    যদি( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    যদি( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    পূর্ণাঙ্ক sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::create_(পূর্ণাঙ্ক _rows, পূর্ণাঙ্ক _cols, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i, তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        প্রত্যাগম;
    }
    যদি( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        প্রত্যাগম;
    }
    যদি( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    যদি( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    পূর্ণাঙ্ক sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::create(পূর্ণাঙ্ক d, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i,
                          তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    পূর্ণাঙ্ক size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    প্রবেশ_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        প্রত্যাগম;
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        প্রত্যাগম;
    }

    যদি( k == MATX )
    {
        CV_Assert( i < 0 );
        পূর্ণাঙ্ক type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        যদি (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            পূর্ণাঙ্ক total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        অন্যথা
        {
            যদি (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            অন্যথা
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.height == sz.width && requested_size.width == sz.height)),
                        "");
            }
        }
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        পূর্ণাঙ্ক esz = CV_ELEM_SIZE(flags);

        যদি( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            পূর্ণাঙ্ক type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            যদি (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                যদি (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    প্রত্যাগম; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } অন্যথা { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        ভঙ্গ

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        প্রত্যাগম;
    }

    যদি( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    যদি( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        যদি( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            যদি( fixedType() )
            {
                পূর্ণাঙ্ক _type = CV_MAT_TYPE(flags);
                প্রতি( size_t j = len0; j < len; j++ )
                {
                    যদি( v[j].type() == _type )
                        অনুবর্ত;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            প্রত্যাগম;
        }

        CV_Assert( i < (পূর্ণাঙ্ক)v.size() );
        Mat& m = v[i];

        যদি( allowTransposed )
        {
            যদি( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            যদি( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        যদি(fixedSize())
        {
            CV_Assert(m.dims == d);
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        প্রত্যাগম;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        যদি( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            যদি( fixedType() )
            {
                পূর্ণাঙ্ক _type = CV_MAT_TYPE(flags);
                প্রতি( size_t j = len0; j < len; j++ )
                {
                    যদি( v[j].type() == _type )
                        অনুবর্ত;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            প্রত্যাগম;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        যদি( allowTransposed )
        {
            যদি( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            যদি( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        যদি(fixedSize())
        {
            CV_Assert(m.dims == d);
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        যদি( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            যদি( fixedType() )
            {
                পূর্ণাঙ্ক _type = CV_MAT_TYPE(flags);
                প্রতি( size_t j = len0; j < len; j++ )
                {
                    যদি( v[j].type() == _type )
                        অনুবর্ত;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            প্রত্যাগম;
        }

        CV_Assert( i < (পূর্ণাঙ্ক)v.size() );
        UMat& m = v[i];

        যদি( allowTransposed )
        {
            যদি( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            যদি( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        যদি(fixedSize())
        {
            CV_Assert(m.dims == d);
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        প্রত্যাগম;
    }

    যদি ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        প্রত্যাগম;
    }

    যদি( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

শূন্য _OutputArray::create(ধ্রুব MatShape& shape, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i,
                          তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    যদি (shape.dims < 0) {
        release();
    } অন্যথা {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(পূর্ণাঙ্ক mtype) ধ্রুব
{
    mtype = CV_MAT_TYPE(mtype);
    প্রত্যাগম getMat().reinterpret(mtype);
}

শূন্য _OutputArray::createSameSize(ধ্রুব প্রবেশ_Array_2& arr, পূর্ণাঙ্ক mtype) ধ্রুব
{
    create(arr.shape(), mtype);
}

শূন্য _OutputArray::fit(Size _sz, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i,
                       তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    পূর্ণাঙ্ক sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::fit(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i,
                       তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    পূর্ণাঙ্ক sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::fit(ধ্রুব MatShape& shape, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i,
                       তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    যদি (shape.dims < 0) {
        release();
        প্রত্যাগম;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } অন্যথা যদি ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

শূন্য _OutputArray::fitSameSize(ধ্রুব প্রবেশ_Array_2& arr, পূর্ণাঙ্ক mtype) ধ্রুব
{
    fit(arr.shape(), mtype);
}

শূন্য _OutputArray::fit(পূর্ণাঙ্ক d, ধ্রুব পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক mtype, পূর্ণাঙ্ক i,
                       তার্কিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুব
{
    পূর্ণাঙ্ক size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    প্রবেশ_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    যদি( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        যদি (k == MAT)
            m = (Mat*)obj;
        অন্যথা {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            অন্যথা
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        প্রত্যাগম;
    }

    যদি( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        যদি (k == UMAT)
            m = (UMat*)obj;
        অন্যথা {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            প্রত্যাগম;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            অন্যথা
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            প্রতি(পূর্ণাঙ্ক j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        প্রত্যাগম;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::release() ধ্রুব
{
    CV_Assert(!fixedSize());

    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        ((Mat*)obj)->release();
        প্রত্যাগম;
    }

    যদি( k == UMAT )
    {
        ((UMat*)obj)->release();
        প্রত্যাগম;
    }

    যদি( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    যদি( k == NONE )
        প্রত্যাগম;

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        যদি (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } অন্যথা { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        ভঙ্গ

        পূর্ণাঙ্ক esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        প্রত্যাগম;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        প্রতি( পূর্ণাঙ্ক i = 0; i < sz.height; i++ )
            v[i].release();
        প্রত্যাগম;
    }

    যদি( k == STD_BOOL_VECTOR )
    {
        ((std::vector<তার্কিক>*)obj)->clear();
        প্রত্যাগম;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        প্রত্যাগম;
    }
    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        প্রত্যাগম;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

শূন্য _OutputArray::clear() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        প্রত্যাগম;
    }

    release();
}

তার্কিক _OutputArray::needed() ধ্রুব
{
    প্রত্যাগম kind() != NONE;
}

Mat& _OutputArray::getMatRef(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( i < 0 )
    {
        CV_Assert( k == MAT );
        প্রত্যাগম *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    যদি( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (পূর্ণাঙ্ক)v.size() );
        প্রত্যাগম v[i];
    }
    অন্যথা
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        প্রত্যাগম v[i];
    }
}

UMat& _OutputArray::getUMatRef(পূর্ণাঙ্ক i) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( i < 0 )
    {
        CV_Assert( k == UMAT );
        প্রত্যাগম *(UMat*)obj;
    }
    অন্যথা
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (পূর্ণাঙ্ক)v.size() );
        প্রত্যাগম v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    প্রত্যাগম *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    প্রত্যাগম *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    প্রত্যাগম *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    প্রত্যাগম *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    প্রত্যাগম *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    প্রত্যাগম *(cuda::HostMem*)obj;
}

শূন্য _OutputArray::setTo(ধ্রুব প্রবেশ_Array_2& arr, ধ্রুব প্রবেশ_Array_2 & mask) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == NONE )
        ;
    অন্যথা যদি (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    অন্যথা যদি( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    অন্যথা যদি( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), প্রবেশ_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<দ্বিগুণ, 4>(value.ptr<দ্বিগুণ>())), mask);
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    অন্যথা
        CV_Error(Error::StsNotImplemented, "");
}

শূন্য _OutputArray::setZero() ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == NONE )
        ;
    অন্যথা যদি (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    অন্যথা
    {
        setTo(Scalar::all(0), noArray());
    }
}

শূন্য _OutputArray::assign(ধ্রুব UMat& u) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    অন্যথা যদি (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    অন্যথা যদি (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


শূন্য _OutputArray::assign(ধ্রুব Mat& m) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    অন্যথা যদি (k == MAT)
    {
        *(Mat*)obj = m;
    }
    অন্যথা যদি (k == MATX)
    {
        m.copyTo(getMat());
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


শূন্য _OutputArray::move(UMat& u) ধ্রুব
{
    যদি (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        প্রত্যাগম;
    }
    পূর্ণাঙ্ক k = kind();
    যদি (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    অন্যথা যদি (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    অন্যথা যদি (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


শূন্য _OutputArray::move(Mat& m) ধ্রুব
{
    যদি (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        প্রত্যাগম;
    }
    পূর্ণাঙ্ক k = kind();
    যদি (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    অন্যথা যদি (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    অন্যথা যদি (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


শূন্য _OutputArray::assign(ধ্রুব std::vector<UMat>& v) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        প্রতি (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুব UMat& m = v[i];
            UMat& this_m = this_v[i];
            যদি (this_m.u != রিক্ত && this_m.u == m.u)
                অনুবর্ত; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা যদি (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        প্রতি (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুব UMat& m = v[i];
            Mat& this_m = this_v[i];
            যদি (this_m.u != রিক্ত && this_m.u == m.u)
                অনুবর্ত; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


শূন্য _OutputArray::assign(ধ্রুব std::vector<Mat>& v) ধ্রুব
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        প্রতি (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুব Mat& m = v[i];
            UMat& this_m = this_v[i];
            যদি (this_m.u != রিক্ত && this_m.u == m.u)
                অনুবর্ত; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা যদি (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        প্রতি (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুব Mat& m = v[i];
            Mat& this_m = this_v[i];
            যদি (this_m.u != রিক্ত && this_m.u == m.u)
                অনুবর্ত; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


স্থির প্রবেশ_Output_Array_2 _none;
প্রবেশ_Output_Array noArray() { প্রত্যাগম _none; }

} // cv::
