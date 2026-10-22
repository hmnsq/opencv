// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"
#অন্তর্ভাব "opencv2/core/mat.hpp"

নামাকাশ cv {

প্রকারলেখ Vec<পূর্ণসংখ্যা, 5> Vec5i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 7> Vec7i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 9> Vec9i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 10> Vec10i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 11> Vec11i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 12> Vec12i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 13> Vec13i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 14> Vec14i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 15> Vec15i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 16> Vec16i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 32> Vec32i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 64> Vec64i;
প্রকারলেখ Vec<পূর্ণসংখ্যা, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    নির্বাচন( esz ) \
    { \
    ক্ষেত্র 1: func(uchar); \
    ক্ষেত্র 2: func(Vec2b); \
    ক্ষেত্র 3: func(Vec3b); \
    ক্ষেত্র 4: func(পূর্ণসংখ্যা); \
    ক্ষেত্র 6: func(Vec3s); \
    ক্ষেত্র 8: func(Vec2i); \
    ক্ষেত্র 12: func(Vec3i); \
    ক্ষেত্র 16: func(Vec4i); \
    ক্ষেত্র 20: func(Vec5i); \
    ক্ষেত্র 24: func(Vec6i); \
    ক্ষেত্র 28: func(Vec7i); \
    ক্ষেত্র 32: func(Vec8i); \
    ক্ষেত্র 36: func(Vec9i); \
    ক্ষেত্র 40: func(Vec10i); \
    ক্ষেত্র 44: func(Vec11i); \
    ক্ষেত্র 48: func(Vec12i); \
    ক্ষেত্র 52: func(Vec13i); \
    ক্ষেত্র 56: func(Vec14i); \
    ক্ষেত্র 60: func(Vec15i); \
    ক্ষেত্র 64: func(Vec16i); \
    ক্ষেত্র 128: func(Vec32i); \
    ক্ষেত্র 256: func(Vec64i); \
    ক্ষেত্র 512: func(Vec128i); \
    পূর্বনির্ধারিত: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat প্রবেশ_Array_2::getMat_(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == MAT )
    {
        ধ্রুবক Mat* m = (ধ্রুবক Mat*)obj;
        যদি( i < 0 )
            ফেরত *m;
        ফেরত m->row(i);
    }

    যদি( k == UMAT )
    {
        ধ্রুবক UMat* m = (ধ্রুবক UMat*)obj;
        যদি( i < 0 )
            ফেরত m->getMat(accessFlags);
        ফেরত m->getMat(accessFlags).row(i);
    }

    যদি (k == MATX)
    {
        CV_Assert( i < 0 );
        ফেরত Mat(sz, flags, obj);
    }

    যদি( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        পূর্ণসংখ্যা t = CV_8U;
        ধ্রুবক std::vector<যৌক্তিক>& v = *(ধ্রুবক std::vector<যৌক্তিক>*)obj;
        পূর্ণসংখ্যা j, n = (পূর্ণসংখ্যা)v.size();
        যদি( n == 0 )
            ফেরত Mat();
        Mat m(1, &n, t);
        uchar* dst = m.তথ্য;
        জন্য( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        ফেরত m;
    }

    যদি( k == NONE )
        ফেরত Mat();

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        পূর্ণসংখ্যা t = CV_MAT_TYPE(flags);
        পূর্ণসংখ্যা esz = CV_ELEM_আকার(t);

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
            পূর্ণসংখ্যা v_sz = পূর্ণসংখ্যা(v->size()); \
            ফেরত v->empty() ? Mat() : Mat(1, &v_sz, t, v->তথ্য()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& v = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (পূর্ণসংখ্যা)v.size() );

        ফেরত v[i];
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* v = (ধ্রুবক Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        ফেরত v[i];
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& v = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (পূর্ণসংখ্যা)v.size() );

        ফেরত v[i].getMat(accessFlags);
    }

    যদি( k == OPENGL_বাফার )
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

        ধ্রুবক cuda::HostMem* cuda_mem = (ধ্রুবক cuda::HostMem*)obj;

        ফেরত cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat প্রবেশ_Array_2::getUMat(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == UMAT )
    {
        ধ্রুবক UMat* m = (ধ্রুবক UMat*)obj;
        যদি( i < 0 )
            ফেরত *m;
        ফেরত m->row(i);
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& v = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (পূর্ণসংখ্যা)v.size() );

        ফেরত v[i];
    }

    যদি( k == MAT )
    {
        ধ্রুবক Mat* m = (ধ্রুবক Mat*)obj;
        যদি( i < 0 )
            ফেরত m->getUMat(accessFlags);
        ফেরত m->row(i).getUMat(accessFlags);
    }

    ফেরত getMat(i).getUMat(accessFlags);
}

শূন্য প্রবেশ_Array_2::getMatVector(std::vector<Mat>& mv) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == MAT )
    {
        ধ্রুবক Mat& m = *(ধ্রুবক Mat*)obj;
        পূর্ণসংখ্যা n = (পূর্ণসংখ্যা)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        জন্য( পূর্ণসংখ্যা i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (শূন্য*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (শূন্য*)m.ptr(i), &m.step[1]);
        ফেরত;
    }

    যদি (k == MATX)
    {
        size_t n = sz.height, esz = CV_ELEM_আকার(flags);
        mv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        ফেরত;
    }

    যদি( k == STD_VECTOR )
    {
        ধ্রুবক std::vector<uchar>& v = *(ধ্রুবক std::vector<uchar>*)obj;

        size_t n = যোগ(), esz = CV_ELEM_আকার(flags);
        পূর্ণসংখ্যা t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (শূন্য*)(v.তথ্য() + esz*i));
        ফেরত;
    }

    যদি( k == NONE )
    {
        mv.clear();
        ফেরত;
    }

    যদি( k == STD_VECTOR_VECTOR )
    {
        পূর্ণসংখ্যা typ = CV_MAT_TYPE(flags);
        পূর্ণসংখ্যা esz = CV_ELEM_আকার(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            ধ্রুবক std::vector<std::vector<T > >* vv = (ধ্রুবক std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            জন্য (size_t i = 0; i < n; i++) { \
                ধ্রুবক std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)INT_MAX); \
                পূর্ণসংখ্যা ni = (পূর্ণসংখ্যা)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (শূন্য*)vi.তথ্য()) : Mat(); \
            } \
        } \
        বিরতি

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        ফেরত;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& v = *(ধ্রুবক std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ফেরত;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* v = (ধ্রুবক Mat*)obj;
        size_t n = sz.height;
        mv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        ফেরত;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& v = *(ধ্রুবক std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        ফেরত;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

শূন্য প্রবেশ_Array_2::getUMatVector(std::vector<UMat>& umv) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    যদি( k == NONE )
    {
        umv.clear();
        ফেরত;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& v = *(ধ্রুবক std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ফেরত;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* v = (ধ্রুবক Mat*)obj;
        size_t n = sz.height;
        umv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        ফেরত;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& v = *(ধ্রুবক std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        জন্য( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        ফেরত;
    }

    যদি( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        ফেরত;
    }
    যদি( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        ফেরত;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat প্রবেশ_Array_2::getGpuMat() ধ্রুবক
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি (k == CUDA_GPU_MAT)
    {
        ধ্রুবক cuda::GpuMat* d_mat = (ধ্রুবক cuda::GpuMat*)obj;
        ফেরত *d_mat;
    }

    যদি (k == CUDA_HOST_MEM)
    {
        ধ্রুবক cuda::HostMem* cuda_mem = (ধ্রুবক cuda::HostMem*)obj;
        ফেরত cuda_mem->createGpuMatHeader();
    }

    যদি (k == OPENGL_বাফার)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    যদি (k == NONE)
        ফেরত cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#অন্যথা
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
শূন্য প্রবেশ_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) ধ্রুবক
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
শূন্য প্রবেশ_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ধ্রুবক
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
cuda::GpuMatND প্রবেশ_Array_2::getGpuMatND() ধ্রুবক
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি (k == CUDA_GPU_MATND)
    {
        ধ্রুবক cuda::GpuMatND* d_mat = (ধ্রুবক cuda::GpuMatND*)obj;
        ফেরত *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#অন্যথা
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::বাফার_2 প্রবেশ_Array_2::get_OGl_বাফার() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_বাফার);

    ধ্রুবক ogl::বাফার_2* gl_buf = (ধ্রুবক ogl::বাফার_2*)obj;
    ফেরত *gl_buf;
}

পূর্ণসংখ্যা প্রবেশ_Array_2::rows_(পূর্ণসংখ্যা i) ধ্রুবক
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == CUDA_GPU_MATND)
    {
        ধ্রুবক cuda::GpuMatND& _gpuMatND = *(ধ্রুবক cuda::GpuMatND*)obj;
        ফেরত (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    ফেরত আকার_4(i).height;
}

পূর্ণসংখ্যা প্রবেশ_Array_2::cols_(পূর্ণসংখ্যা i) ধ্রুবক
{
#ifdef HAVE_CUDA
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == CUDA_GPU_MATND)
    {
        ধ্রুবক cuda::GpuMatND& _gpuMatND = *(ধ্রুবক cuda::GpuMatND*)obj;
        ফেরত (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    ফেরত আকার_4(i).width;
}

আকার_2 প্রবেশ_Array_2::আকার_4(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক Mat* m = (ধ্রুবক Mat*)obj;
        CV_Assert(m->dims <= 2);
        ফেরত আকার_2(m->cols, m->rows);
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক UMat* m = (ধ্রুবক UMat*)obj;
        CV_Assert(m->dims <= 2);
        ফেরত আকার_2(m->cols, m->rows);
    }

    যদি (k == MATX)
    {
        CV_Assert( i < 0 );
        ফেরত sz;
    }

    যদি( k == NONE )
        ফেরত আকার_2();

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        size_t n = যোগ(i);
        CV_Assert(n <= (size_t)INT_MAX);
        ফেরত আকার_2((পূর্ণসংখ্যা)n, 1);
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        যদি( i < 0 )
            ফেরত vv.empty() ? আকার_2() : আকার_2((পূর্ণসংখ্যা)vv.size(), 1);
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );

        ফেরত vv[i].size();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        যদি( i < 0 )
            ফেরত sz.height==0 ? আকার_2() : আকার_2(sz.height, 1);
        CV_Assert( i < sz.height );

        ফেরত vv[i].size();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ধ্রুবক std::vector<cuda::GpuMat>& vv = *(ধ্রুবক std::vector<cuda::GpuMat>*)obj;
        যদি (i < 0)
            ফেরত vv.empty() ? আকার_2() : আকার_2((পূর্ণসংখ্যা)vv.size(), 1);
        CV_Assert(i < (পূর্ণসংখ্যা)vv.size());
        ফেরত vv[i].size();
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ধ্রুবক std::vector<cuda::GpuMatND>& vv = *(ধ্রুবক std::vector<cuda::GpuMatND>*)obj;
        যদি (i < 0)
            ফেরত vv.empty() ? আকার_2() : আকার_2((পূর্ণসংখ্যা)vv.size(), 1);
        CV_Assert(i < (পূর্ণসংখ্যা)vv.size());
        ধ্রুবক cuda::GpuMatND& m = vv[i];
        যদি (m.dims == 0)
            ফেরত আকার_2();
        যদি (m.dims == 1)
            ফেরত আকার_2(1, m.size[0]);
        ফেরত আকার_2(m.size[1], m.size[0]);
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        যদি( i < 0 )
            ফেরত vv.empty() ? আকার_2() : আকার_2((পূর্ণসংখ্যা)vv.size(), 1);
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );

        ফেরত vv[i].size();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ধ্রুবক std::vector<cuda::GpuMatND>& vv = *(ধ্রুবক std::vector<cuda::GpuMatND>*)obj;
        যদি (i < 0)
            ফেরত vv.empty() ? আকার_2() : আকার_2((পূর্ণসংখ্যা)vv.size(), 1);
        CV_Assert(i < (পূর্ণসংখ্যা)vv.size());
        ধ্রুবক cuda::GpuMatND& m = vv[i];
        যদি (m.dims == 0)
            ফেরত আকার_2();
        যদি (m.dims == 1)
            ফেরত আকার_2(1, m.size[0]);
        ফেরত আকার_2(m.size[1], m.size[0]);
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == OPENGL_বাফার )
    {
        CV_Assert( i < 0 );
        ধ্রুবক ogl::বাফার_2* buf = (ধ্রুবক ogl::বাফার_2*)obj;
        ফেরত buf->size();
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক cuda::GpuMat* d_mat = (ধ্রুবক cuda::GpuMat*)obj;
        ফেরত d_mat->size();
    }

    যদি( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ধ্রুবক cuda::HostMem* cuda_mem = (ধ্রুবক cuda::HostMem*)obj;
        ফেরত cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

পূর্ণসংখ্যা প্রবেশ_Array_2::sizend(পূর্ণসংখ্যা* arrsz, পূর্ণসংখ্যা i) ধ্রুবক
{
    পূর্ণসংখ্যা j, d = 0;
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == NONE )
        ;
    অন্যথা যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক Mat& m = *(ধ্রুবক Mat*)obj;
        d = m.dims;
        যদি(arrsz)
            জন্য(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক UMat& m = *(ধ্রুবক UMat*)obj;
        d = m.dims;
        যদি(arrsz)
            জন্য(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == STD_VECTOR_MAT && i >= 0 )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );
        ধ্রুবক Mat& m = vv[i];
        d = m.dims;
        যদি(arrsz)
            জন্য(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == STD_ARRAY_MAT && i >= 0 )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        CV_Assert( i < sz.height );
        ধ্রুবক Mat& m = vv[i];
        d = m.dims;
        যদি(arrsz)
            জন্য(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );
        ধ্রুবক UMat& m = vv[i];
        d = m.dims;
        যদি(arrsz)
            জন্য(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    অন্যথা যদি (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        size_t n = যোগ(i);
        d = 1;
        যদি(arrsz)
        {
            arrsz[0] = পূর্ণসংখ্যা(n);
        }
    }
    অন্যথা
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        আকার_2 sz2d = size(i);
        d = 2;
        যদি(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    ফেরত d;
}

যৌক্তিক প্রবেশ_Array_2::empty(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (i >= 0) {
        যদি (k == STD_VECTOR_MAT) {
            auto mv = reinterpret_cast<ধ্রুবক std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            ফেরত mv->at(i).empty();
        }
        অন্যথা যদি (k == STD_VECTOR_UMAT) {
            auto umv = reinterpret_cast<ধ্রুবক std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            ফেরত umv->at(i).empty();
        }
        অন্যথা যদি (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            size_t n = যোগ(i);
            ফেরত n == 0;
        } অন্যথা {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    ফেরত empty();
}

MatShape প্রবেশ_Array_2::shape(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    MatShape shape;

    যদি( k == NONE )
        ;
    অন্যথা যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক Mat& m = *(ধ্রুবক Mat*)obj;
        shape = m.size;
    }
    অন্যথা যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক UMat& m = *(ধ্রুবক UMat*)obj;
        shape = m.size;
    }
    অন্যথা যদি( k == STD_VECTOR_MAT && i >= 0 )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ধ্রুবক Mat& m = vv[i];
        shape = m.size;
    }
    অন্যথা যদি( k == STD_ARRAY_MAT && i >= 0 )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        CV_Assert( i < sz.height );
        ধ্রুবক Mat& m = vv[i];
        shape = m.size;
    }
    অন্যথা যদি( k == STD_VECTOR_UMAT && i >= 0 )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ধ্রুবক UMat& m = vv[i];
        shape = m.size;
    }
    অন্যথা
    {
        পূর্ণসংখ্যা sizes[CV_MAX_DIM];
        পূর্ণসংখ্যা dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    ফেরত shape;
}

যৌক্তিক প্রবেশ_Array_2::same_আকার(ধ্রুবক প্রবেশ_Array_2& arr) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    আকার_2 sz1;

    যদি( k1 == MAT )
    {
        ধ্রুবক Mat* m = ((ধ্রুবক Mat*)obj);
        যদি( k2 == MAT )
            ফেরত m->size == ((ধ্রুবক Mat*)arr.obj)->size;
        যদি( k2 == UMAT )
            ফেরত m->size == ((ধ্রুবক UMat*)arr.obj)->size;
        যদি( m->dims > 2 )
            ফেরত মিথ্যা;
        sz1 = m->size();
    }
    অন্যথা যদি( k1 == UMAT )
    {
        ধ্রুবক UMat* m = ((ধ্রুবক UMat*)obj);
        যদি( k2 == MAT )
            ফেরত m->size == ((ধ্রুবক Mat*)arr.obj)->size;
        যদি( k2 == UMAT )
            ফেরত m->size == ((ধ্রুবক UMat*)arr.obj)->size;
        যদি( m->dims > 2 )
            ফেরত মিথ্যা;
        sz1 = m->size();
    }
    অন্যথা যদি ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        ফেরত ((ধ্রুবক cuda::GpuMatND*)obj)->size == ((ধ্রুবক cuda::GpuMatND*)arr.obj)->size;
    }
    অন্যথা
        sz1 = size();

    যদি( arr.dims() > 2 )
        ফেরত মিথ্যা;
    ফেরত sz1 == arr.size();
}

পূর্ণসংখ্যা প্রবেশ_Array_2::dims_(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক Mat*)obj)->dims;
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক UMat*)obj)->dims;
    }

    যদি (k == MATX)
    {
        CV_Assert( i < 0 );
        ফেরত 2;
    }

    যদি( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        ফেরত 1;
    }

    যদি( k == NONE )
        ফেরত 0;

    যদি( k == STD_VECTOR_VECTOR )
    {
        যদি( i < 0 )
            ফেরত 1;
        CV_Assert( size_t(i) < যোগ() );
        ফেরত 1;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        যদি( i < 0 )
            ফেরত 1;
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );

        ফেরত vv[i].dims;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        যদি( i < 0 )
            ফেরত 1;
        CV_Assert( i < sz.height );

        ফেরত vv[i].dims;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        যদি( i < 0 )
            ফেরত 1;
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );

        ফেরত vv[i].dims;
    }

    যদি( k == OPENGL_বাফার )
    {
        CV_Assert( i < 0 );
        ফেরত 2;
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ফেরত 2;
    }

    যদি( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক cuda::GpuMatND*)obj)->dims;
    }

    যদি( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ফেরত 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t প্রবেশ_Array_2::যোগ(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক Mat*)obj)->যোগ();
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক UMat*)obj)->যোগ();
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        যদি( i < 0 )
            ফেরত vv.size();

        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );
        ফেরত vv[i].যোগ();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        যদি( i < 0 )
            ফেরত sz.height;

        CV_Assert( i < sz.height );
        ফেরত vv[i].যোগ();
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        যদি( i < 0 )
            ফেরত vv.size();

        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );
        ফেরত vv[i].যোগ();
    }

    যদি( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ধ্রুবক cuda::GpuMatND& _gpuMatND = *((ধ্রুবক cuda::GpuMatND*)obj);
        যদি (_gpuMatND.dims > 0)
        {
            res = 1;
            জন্য(পূর্ণসংখ্যা d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            ফেরত res;
        }
    }

    যদি (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        পূর্ণসংখ্যা esz = CV_ELEM_আকার(flags);

        #undef GET_VEC_আকার
        #define GET_VEC_আকার(T) \
            যদি (k == STD_VECTOR_VECTOR) { \
                ধ্রুবক std::vector<std::vector<T > >* vv = (ধ্রুবক std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                যদি (i < 0) \
                    ফেরত n; \
                CV_Assert((size_t)i < n); \
                ফেরত vv->at(i).size(); \
            } অন্যথা \
                ফেরত ((ধ্রুবক std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_আকার)
    }

    ফেরত size(i).area();
}

পূর্ণসংখ্যা প্রবেশ_Array_2::type_(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        ফেরত ((ধ্রুবক Mat*)obj)->type();

    যদি( k == UMAT )
        ফেরত ((ধ্রুবক UMat*)obj)->type();

    যদি( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ফেরত CV_MAT_TYPE(flags);

    যদি( k == NONE )
        ফেরত -1;

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        যদি( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ফেরত CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );
        ফেরত vv[i >= 0 ? i : 0].type();
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        যদি( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ফেরত CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (পূর্ণসংখ্যা)vv.size() );
        ফেরত vv[i >= 0 ? i : 0].type();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        যদি( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ফেরত CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        ফেরত vv[i >= 0 ? i : 0].type();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ধ্রুবক std::vector<cuda::GpuMat>& vv = *(ধ্রুবক std::vector<cuda::GpuMat>*)obj;
        যদি (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ফেরত CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (পূর্ণসংখ্যা)vv.size());
        ফেরত vv[i >= 0 ? i : 0].type();
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        ধ্রুবক std::vector<cuda::GpuMatND>& vv = *(ধ্রুবক std::vector<cuda::GpuMatND>*)obj;
        যদি (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            ফেরত CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (পূর্ণসংখ্যা)vv.size());
        ফেরত vv[i >= 0 ? i : 0].type();
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == OPENGL_বাফার )
        ফেরত ((ধ্রুবক ogl::বাফার_2*)obj)->type();

    যদি( k == CUDA_GPU_MAT )
        ফেরত ((ধ্রুবক cuda::GpuMat*)obj)->type();

    যদি( k == CUDA_GPU_MATND )
        ফেরত ((ধ্রুবক cuda::GpuMatND*)obj)->type();

    যদি( k == CUDA_HOST_MEM )
        ফেরত ((ধ্রুবক cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

পূর্ণসংখ্যা প্রবেশ_Array_2::depth_(পূর্ণসংখ্যা i) ধ্রুবক
{
    ফেরত CV_MAT_DEPTH(type(i));
}

যৌক্তিক প্রবেশ_Array_2::empty_() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        ফেরত ((ধ্রুবক Mat*)obj)->empty();

    যদি( k == UMAT )
        ফেরত ((ধ্রুবক UMat*)obj)->empty();

    যদি (k == MATX)
        ফেরত মিথ্যা;

    যদি( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        size_t n = যোগ(-1);
        ফেরত n == 0;
    }

    যদি( k == NONE )
        ফেরত সত্য;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        ফেরত vv.empty();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ফেরত sz.height == 0;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        ফেরত vv.empty();
    }

    যদি( k == OPENGL_বাফার )
        ফেরত ((ধ্রুবক ogl::বাফার_2*)obj)->empty();

    যদি( k == CUDA_GPU_MAT )
        ফেরত ((ধ্রুবক cuda::GpuMat*)obj)->empty();

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ধ্রুবক std::vector<cuda::GpuMat>& vv = *(ধ্রুবক std::vector<cuda::GpuMat>*)obj;
        ফেরত vv.empty();
    }

    যদি( k == CUDA_GPU_MATND )
        ফেরত ((ধ্রুবক cuda::GpuMatND*)obj)->empty();

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ধ্রুবক std::vector<cuda::GpuMatND>& vv = *(ধ্রুবক std::vector<cuda::GpuMatND>*)obj;
        ফেরত vv.empty();
    }

    যদি( k == CUDA_HOST_MEM )
        ফেরত ((ধ্রুবক cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

যৌক্তিক প্রবেশ_Array_2::isContinuous(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        ফেরত i < 0 ? ((ধ্রুবক Mat*)obj)->isContinuous() : সত্য;

    যদি( k == UMAT )
        ফেরত i < 0 ? ((ধ্রুবক UMat*)obj)->isContinuous() : সত্য;

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ফেরত সত্য;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].isContinuous();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ফেরত vv[i].isContinuous();
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].isContinuous();
    }

    যদি( k == CUDA_GPU_MAT )
      ফেরত i < 0 ? ((ধ্রুবক cuda::GpuMat*)obj)->isContinuous() : সত্য;

    যদি( k == CUDA_GPU_MATND )
        ফেরত i < 0 ? ((ধ্রুবক cuda::GpuMatND*)obj)->isContinuous() : সত্য;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

যৌক্তিক প্রবেশ_Array_2::isSubmatrix(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
        ফেরত i < 0 ? ((ধ্রুবক Mat*)obj)->isSubmatrix() : মিথ্যা;

    যদি( k == UMAT )
        ফেরত i < 0 ? ((ধ্রুবক UMat*)obj)->isSubmatrix() : মিথ্যা;

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ফেরত মিথ্যা;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].isSubmatrix();
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        ফেরত vv[i].isSubmatrix();
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].isSubmatrix();
    }

    যদি( k == CUDA_GPU_MATND )
    {
        ফেরত ((ধ্রুবক cuda::GpuMatND*)obj)->isSubmatrix();
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ধ্রুবক std::vector<cuda::GpuMatND>& vv = *(ধ্রুবক std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t প্রবেশ_Array_2::offset(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক Mat * ধ্রুবক m = ((ধ্রুবক Mat*)obj);
        ফেরত (size_t)(m->ptr() - m->datastart);
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক UMat*)obj)->offset;
    }

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ফেরত 0;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (পূর্ণসংখ্যা)vv.size() );

        ফেরত (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ফেরত (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].offset;
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ধ্রুবক cuda::GpuMat * ধ্রুবক m = ((ধ্রুবক cuda::GpuMat*)obj);
        ফেরত (size_t)(m->তথ্য - m->datastart);
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ধ্রুবক std::vector<cuda::GpuMat>& vv = *(ধ্রুবক std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত (size_t)(vv[i].তথ্য - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t প্রবেশ_Array_2::step(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক Mat*)obj)->step;
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক UMat*)obj)->step;
    }

    যদি( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        ফেরত 0;

    যদি( k == STD_VECTOR_MAT )
    {
        ধ্রুবক std::vector<Mat>& vv = *(ধ্রুবক std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (পূর্ণসংখ্যা)vv.size() );
        ফেরত vv[i].step;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        ধ্রুবক Mat* vv = (ধ্রুবক Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        ফেরত vv[i].step;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ধ্রুবক std::vector<UMat>& vv = *(ধ্রুবক std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].step;
    }

    যদি( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ফেরত ((ধ্রুবক cuda::GpuMat*)obj)->step;
    }
    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ধ্রুবক std::vector<cuda::GpuMat>& vv = *(ধ্রুবক std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ফেরত vv[i].step;
    }
    যদি( k == CUDA_GPU_MATND )
    {
        ধ্রুবক cuda::GpuMatND& _gpuMatND = *(ধ্রুবক cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        ফেরত _gpuMatND.step[i];
    }

    যদি (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        ধ্রুবক std::vector<cuda::GpuMatND>& vv = *(ধ্রুবক std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ধ্রুবক cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        ফেরত _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

শূন্য প্রবেশ_Array_2::copyTo(ধ্রুবক _OutputArray& arr) ধ্রুবক
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

শূন্য প্রবেশ_Array_2::copyTo(ধ্রুবক _OutputArray& arr, ধ্রুবক প্রবেশ_Array_2 & mask) ধ্রুবক
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

যৌক্তিক _OutputArray::fixed_আকার() ধ্রুবক
{
    ফেরত (flags & FIXED_আকার) == FIXED_আকার;
}

যৌক্তিক _OutputArray::fixedType() ধ্রুবক
{
    ফেরত (flags & FIXED_TYPE) == FIXED_TYPE;
}

শূন্য _OutputArray::create_(আকার_2 _sz, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i, যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((Mat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        ফেরত;
    }
    যদি( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((UMat*)obj)->size.operator()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        ফেরত;
    }
    যদি( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::আকার_Array sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == OPENGL_বাফার && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((ogl::বাফার_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::বাফার_2*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::বাফার_2*)obj)->create(_sz, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    যদি( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    পূর্ণসংখ্যা sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::create_(পূর্ণসংখ্যা _rows, পূর্ণসংখ্যা _cols, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i, যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((Mat*)obj)->size.operator()() == আকার_2(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        ফেরত;
    }
    যদি( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((UMat*)obj)->size.operator()() == আকার_2(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        ফেরত;
    }
    যদি( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((cuda::GpuMat*)obj)->size() == আকার_2(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::আকার_Array sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    যদি( k == OPENGL_বাফার && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((ogl::বাফার_2*)obj)->size() == আকার_2(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::বাফার_2*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::বাফার_2*)obj)->create(_rows, _cols, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    যদি( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_আকার() || ((cuda::HostMem*)obj)->size() == আকার_2(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    পূর্ণসংখ্যা sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::create(পূর্ণসংখ্যা d, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i,
                          যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    পূর্ণসংখ্যা size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    প্রবেশ_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    যদি( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_আকার()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixed_আকার())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ফেরত;
    }

    যদি( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_আকার()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixed_আকার())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        ফেরত;
    }

    যদি( k == MATX )
    {
        CV_Assert( i < 0 );
        পূর্ণসংখ্যা type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        আকার_2 requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        যদি (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            পূর্ণসংখ্যা যোগ_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == যোগ_1d, "");
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
        ফেরত;
    }

    যদি( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixed_আকার());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        পূর্ণসংখ্যা esz = CV_ELEM_আকার(flags);

        যদি( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            পূর্ণসংখ্যা type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            যদি (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                যদি (i < 0) { \
                    CV_Assert(!fixed_আকার() || len == vv->size()); \
                    vv->resize(len); \
                    ফেরত; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } অন্যথা { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_আকার() || len == v->size()); \
            v->resize(len); \
        } \
        বিরতি

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        ফেরত;
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

            CV_Assert(!fixed_আকার() || len == len0);
            v.resize(len);
            যদি( fixedType() )
            {
                পূর্ণসংখ্যা _type = CV_MAT_TYPE(flags);
                জন্য( size_t j = len0; j < len; j++ )
                {
                    যদি( v[j].type() == _type )
                        চালু;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ফেরত;
        }

        CV_Assert( i < (পূর্ণসংখ্যা)v.size() );
        Mat& m = v[i];

        যদি( allowTransposed )
        {
            যদি( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_আকার());
                m.release();
            }

            যদি( d == 2 && m.dims == 2 && m.তথ্য &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        যদি(fixed_আকার())
        {
            CV_Assert(m.dims == d);
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ফেরত;
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
                পূর্ণসংখ্যা _type = CV_MAT_TYPE(flags);
                জন্য( size_t j = len0; j < len; j++ )
                {
                    যদি( v[j].type() == _type )
                        চালু;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ফেরত;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        যদি( allowTransposed )
        {
            যদি( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_আকার());
                m.release();
            }

            যদি( d == 2 && m.dims == 2 && m.তথ্য &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        যদি(fixed_আকার())
        {
            CV_Assert(m.dims == d);
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ফেরত;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        যদি( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_আকার() || len == len0);
            v.resize(len);
            যদি( fixedType() )
            {
                পূর্ণসংখ্যা _type = CV_MAT_TYPE(flags);
                জন্য( size_t j = len0; j < len; j++ )
                {
                    যদি( v[j].type() == _type )
                        চালু;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            ফেরত;
        }

        CV_Assert( i < (পূর্ণসংখ্যা)v.size() );
        UMat& m = v[i];

        যদি( allowTransposed )
        {
            যদি( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_আকার());
                m.release();
            }

            যদি( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            অন্যথা
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        যদি(fixed_আকার())
        {
            CV_Assert(m.dims == d);
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        ফেরত;
    }

    যদি ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        ফেরত;
    }

    যদি( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::আকার_Array আকার_Array_2 = cuda::GpuMatND::আকার_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(আকার_Array_2, mtype);
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

শূন্য _OutputArray::create(ধ্রুবক MatShape& shape, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i,
                          যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    যদি (shape.dims < 0) {
        release();
    } অন্যথা {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(পূর্ণসংখ্যা mtype) ধ্রুবক
{
    mtype = CV_MAT_TYPE(mtype);
    ফেরত getMat().reinterpret(mtype);
}

শূন্য _OutputArray::create_Same_আকার(ধ্রুবক প্রবেশ_Array_2& arr, পূর্ণসংখ্যা mtype) ধ্রুবক
{
    create(arr.shape(), mtype);
}

শূন্য _OutputArray::fit(আকার_2 _sz, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i,
                       যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    পূর্ণসংখ্যা sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::fit(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i,
                       যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    পূর্ণসংখ্যা sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::fit(ধ্রুবক MatShape& shape, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i,
                       যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    যদি (shape.dims < 0) {
        release();
        ফেরত;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        Mat_আকার& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } অন্যথা যদি ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        Mat_আকার& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

শূন্য _OutputArray::fit_Same_আকার(ধ্রুবক প্রবেশ_Array_2& arr, পূর্ণসংখ্যা mtype) ধ্রুবক
{
    fit(arr.shape(), mtype);
}

শূন্য _OutputArray::fit(পূর্ণসংখ্যা d, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা mtype, পূর্ণসংখ্যা i,
                       যৌক্তিক allowTransposed, _OutputArray::DepthMask fixedDepthMask) ধ্রুবক
{
    পূর্ণসংখ্যা size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
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
        CV_Assert(!(m->empty() && fixedType() && fixed_আকার()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            অন্যথা
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixed_আকার())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ফেরত;
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
        CV_Assert(!(m->empty() && fixedType() && fixed_আকার()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        যদি (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            ফেরত;
        }

        যদি(fixedType())
        {
            যদি(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            অন্যথা
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        যদি(fixed_আকার())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            জন্য(পূর্ণসংখ্যা j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        ফেরত;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

শূন্য _OutputArray::release() ধ্রুবক
{
    CV_Assert(!fixed_আকার());

    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        ((Mat*)obj)->release();
        ফেরত;
    }

    যদি( k == UMAT )
    {
        ((UMat*)obj)->release();
        ফেরত;
    }

    যদি( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    যদি( k == OPENGL_বাফার )
    {
#ifdef HAVE_OPENGL
        ((ogl::বাফার_2*)obj)->release();
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    যদি( k == NONE )
        ফেরত;

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
        বিরতি

        পূর্ণসংখ্যা esz = CV_ELEM_আকার(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        ফেরত;
    }

    যদি( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        ফেরত;
    }

    যদি( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        জন্য( পূর্ণসংখ্যা i = 0; i < sz.height; i++ )
            v[i].release();
        ফেরত;
    }

    যদি( k == STD_BOOL_VECTOR )
    {
        ((std::vector<যৌক্তিক>*)obj)->clear();
        ফেরত;
    }

    যদি( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        ফেরত;
    }
    যদি (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        ফেরত;
#অন্যথা
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

শূন্য _OutputArray::clear() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();

    যদি( k == MAT )
    {
        CV_Assert(!fixed_আকার());
        ((Mat*)obj)->resize(0);
        ফেরত;
    }

    release();
}

যৌক্তিক _OutputArray::needed() ধ্রুবক
{
    ফেরত kind() != NONE;
}

Mat& _OutputArray::getMatRef(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( i < 0 )
    {
        CV_Assert( k == MAT );
        ফেরত *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    যদি( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (পূর্ণসংখ্যা)v.size() );
        ফেরত v[i];
    }
    অন্যথা
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        ফেরত v[i];
    }
}

UMat& _OutputArray::getUMatRef(পূর্ণসংখ্যা i) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি( i < 0 )
    {
        CV_Assert( k == UMAT );
        ফেরত *(UMat*)obj;
    }
    অন্যথা
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (পূর্ণসংখ্যা)v.size() );
        ফেরত v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    ফেরত *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    ফেরত *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    ফেরত *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    ফেরত *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::বাফার_2& _OutputArray::get_OGl_বাফার_Ref() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_বাফার );
    ফেরত *(ogl::বাফার_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    ফেরত *(cuda::HostMem*)obj;
}

শূন্য _OutputArray::setTo(ধ্রুবক প্রবেশ_Array_2& arr, ধ্রুবক প্রবেশ_Array_2 & mask) ধ্রুবক
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

শূন্য _OutputArray::setZero() ধ্রুবক
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

শূন্য _OutputArray::assign(ধ্রুবক UMat& u) ধ্রুবক
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


শূন্য _OutputArray::assign(ধ্রুবক Mat& m) ধ্রুবক
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


শূন্য _OutputArray::move(UMat& u) ধ্রুবক
{
    যদি (fixed_আকার())
    {
        // TODO Performance warning
        assign(u);
        ফেরত;
    }
    পূর্ণসংখ্যা k = kind();
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


শূন্য _OutputArray::move(Mat& m) ধ্রুবক
{
    যদি (fixed_আকার())
    {
        // TODO Performance warning
        assign(m);
        ফেরত;
    }
    পূর্ণসংখ্যা k = kind();
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


শূন্য _OutputArray::assign(ধ্রুবক std::vector<UMat>& v) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        জন্য (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুবক UMat& m = v[i];
            UMat& this_m = this_v[i];
            যদি (this_m.u != ফাঁকা && this_m.u == m.u)
                চালু; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা যদি (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        জন্য (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুবক UMat& m = v[i];
            Mat& this_m = this_v[i];
            যদি (this_m.u != ফাঁকা && this_m.u == m.u)
                চালু; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


শূন্য _OutputArray::assign(ধ্রুবক std::vector<Mat>& v) ধ্রুবক
{
    প্রবেশ_Array_2::KindFlag k = kind();
    যদি (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        জন্য (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুবক Mat& m = v[i];
            UMat& this_m = this_v[i];
            যদি (this_m.u != ফাঁকা && this_m.u == m.u)
                চালু; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা যদি (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        জন্য (size_t i = 0; i < v.size(); i++)
        {
            ধ্রুবক Mat& m = v[i];
            Mat& this_m = this_v[i];
            যদি (this_m.u != ফাঁকা && this_m.u == m.u)
                চালু; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    অন্যথা
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


স্থির প্রবেশ_Output_Array_2 _none;
প্রবেশ_Output_Array noArray() { ফেরত _none; }

} // cv::
