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

#وصف_رد STD_ويڪٽر_SWITCH
#وصف_ڪريو STD_ويڪٽر_SWITCH(esz, func) \
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

Mat داخلا_Array_2::getMat_(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    إذا( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع *m;
        أرجع m->صف(i);
    }

    إذا( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        إذا( i < 0 )
            أرجع m->getMat(accessFlags);
        أرجع m->getMat(accessFlags).صف(i);
    }

    إذا (k == MATX)
    {
        CV_Assert( i < 0 );
        أرجع Mat(sz, flags, obj);
    }

    إذا( k == STD_BOOL_ويڪٽر )
    {
        CV_Assert( i < 0 );
        صحيح t = CV_8U;
        ثابت std::vector<منطقي>& v = *(ثابت std::vector<منطقي>*)obj;
        صحيح j, n = (صحيح)v.size();
        إذا( n == 0 )
            أرجع Mat();
        Mat m(1, &n, t);
        uchar* dst = m.معلومات;
        لكل( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        أرجع m;
    }

    إذا( k == NONE )
        أرجع Mat();

    إذا( k == STD_ويڪٽر || k == STD_ويڪٽر_ويڪٽر )
    {
        صحيح t = CV_MAT_TYPE(flags);
        صحيح esz = CV_ELEM_سائيز(t);

    #وصف_رد GET_MAT
    #وصف_ڪريو GET_MAT(T) \
        { \
            std::vector<T>* v; \
            إذا (k == STD_ويڪٽر_ويڪٽر) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } وإلا { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            صحيح v_sz = صحيح(v->size()); \
            أرجع v->empty() ? Mat() : Mat(1, &v_sz, t, v->معلومات()); \
        }

        STD_ويڪٽر_SWITCH(esz, GET_MAT)
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        أرجع v[i];
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        CV_Assert( 0 <= i && i < sz.اوچائي );

        أرجع v[i];
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        أرجع v[i].getMat(accessFlags);
    }

    إذا( k == OPENGL_بفر )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    إذا( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    إذا( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    إذا( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;

        أرجع cuda_mem->create_Mat_سرو();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat داخلا_Array_2::getUMat(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    إذا( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        إذا( i < 0 )
            أرجع *m;
        أرجع m->صف(i);
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        أرجع v[i];
    }

    إذا( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع m->getUMat(accessFlags);
        أرجع m->صف(i).getUMat(accessFlags);
    }

    أرجع getMat(i).getUMat(accessFlags);
}

فراغ داخلا_Array_2::get_Mat_ويڪٽر(std::vector<Mat>& mv) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    إذا( k == MAT )
    {
        ثابت Mat& m = *(ثابت Mat*)obj;
        صحيح n = (صحيح)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        لكل( صحيح i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.کانن_ڳڻپ, m.type(), (فراغ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (فراغ*)m.ptr(i), &m.قدم[1]);
        أرجع;
    }

    إذا (k == MATX)
    {
        size_t n = sz.اوچائي, esz = CV_ELEM_سائيز(flags);
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.ويڪر, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.ويڪر*i);
        أرجع;
    }

    إذا( k == STD_ويڪٽر )
    {
        ثابت std::vector<uchar>& v = *(ثابت std::vector<uchar>*)obj;

        size_t n = ڪل(), esz = CV_ELEM_سائيز(flags);
        صحيح t = CV_MAT_اونهائي(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (فراغ*)(v.معلومات() + esz*i));
        أرجع;
    }

    إذا( k == NONE )
    {
        mv.clear();
        أرجع;
    }

    إذا( k == STD_ويڪٽر_ويڪٽر )
    {
        صحيح typ = CV_MAT_TYPE(flags);
        صحيح esz = CV_ELEM_سائيز(typ);

        #وصف_رد GET_VEC_VEC_MAT_VEC
        #وصف_ڪريو GET_VEC_VEC_MAT_VEC(T) \
        { \
            ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            لكل (size_t i = 0; i < n; i++) { \
                ثابت std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)وڌ_کان_وڌ_عدد); \
                صحيح ni = (صحيح)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (فراغ*)vi.معلومات()) : Mat(); \
            } \
        } \
        اكسر

        STD_ويڪٽر_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        أرجع;
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        أرجع;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.اوچائي;
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        أرجع;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        أرجع;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ داخلا_Array_2::get_UMat_ويڪٽر(std::vector<UMat>& umv) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    AccessFlag accessFlags = flags & ACCESS_نقاب;

    إذا( k == NONE )
    {
        umv.clear();
        أرجع;
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        أرجع;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        size_t n = sz.اوچائي;
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        أرجع;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& v = *(ثابت std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        أرجع;
    }

    إذا( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        أرجع;
    }
    إذا( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        أرجع;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat داخلا_Array_2::getGpuMat() ثابت
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا (k == CUDA_GPU_MAT)
    {
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        أرجع *d_mat;
    }

    إذا (k == CUDA_HOST_MEM)
    {
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        أرجع cuda_mem->create_Gpu_Mat_سرو();
    }

    إذا (k == OPENGL_بفر)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    إذا (k == NONE)
        أرجع cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#وإلا
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
}
فراغ داخلا_Array_2::get_Gpu_Mat_ويڪٽر(std::vector<cuda::GpuMat>& gpumv) ثابت
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
}
فراغ داخلا_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ثابت
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#وإلا
    CV_اڻ_استعمال_ٿيل(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
}
cuda::GpuMatND داخلا_Array_2::getGpuMatND() ثابت
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND* d_mat = (ثابت cuda::GpuMatND*)obj;
        أرجع *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#وإلا
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
}
ogl::بفر_2 داخلا_Array_2::get_OGl_بفر() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    CV_Assert(k == OPENGL_بفر);

    ثابت ogl::بفر_2* gl_buf = (ثابت ogl::بفر_2*)obj;
    أرجع *gl_buf;
}

صحيح داخلا_Array_2::صفون_2(صحيح i) ثابت
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        أرجع (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#جيڪڏهن_پورو

    أرجع سائيز_3(i).اوچائي;
}

صحيح داخلا_Array_2::کانن_ڳڻپ_2(صحيح i) ثابت
{
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        أرجع (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#جيڪڏهن_پورو

    أرجع سائيز_3(i).ويڪر;
}

سائيز داخلا_Array_2::سائيز_3(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat* m = (ثابت Mat*)obj;
        CV_Assert(m->dims <= 2);
        أرجع سائيز(m->کانن_ڳڻپ, m->صفون);
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat* m = (ثابت UMat*)obj;
        CV_Assert(m->dims <= 2);
        أرجع سائيز(m->کانن_ڳڻپ, m->صفون);
    }

    إذا (k == MATX)
    {
        CV_Assert( i < 0 );
        أرجع sz;
    }

    إذا( k == NONE )
        أرجع سائيز();

    إذا( k == STD_ويڪٽر || k == STD_ويڪٽر_ويڪٽر || k == STD_BOOL_ويڪٽر )
    {
        size_t n = ڪل(i);
        CV_Assert(n <= (size_t)وڌ_کان_وڌ_عدد);
        أرجع سائيز((صحيح)n, 1);
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        إذا( i < 0 )
            أرجع vv.empty() ? سائيز() : سائيز((صحيح)vv.size(), 1);
        CV_Assert( i < (صحيح)vv.size() );

        أرجع vv[i].size();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع sz.اوچائي==0 ? سائيز() : سائيز(sz.اوچائي, 1);
        CV_Assert( i < sz.اوچائي );

        أرجع vv[i].size();
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        إذا (i < 0)
            أرجع vv.empty() ? سائيز() : سائيز((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        أرجع vv[i].size();
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        إذا (i < 0)
            أرجع vv.empty() ? سائيز() : سائيز((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        إذا (m.dims == 0)
            أرجع سائيز();
        إذا (m.dims == 1)
            أرجع سائيز(1, m.size[0]);
        أرجع سائيز(m.size[1], m.size[0]);
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        إذا( i < 0 )
            أرجع vv.empty() ? سائيز() : سائيز((صحيح)vv.size(), 1);
        CV_Assert( i < (صحيح)vv.size() );

        أرجع vv[i].size();
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        إذا (i < 0)
            أرجع vv.empty() ? سائيز() : سائيز((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        إذا (m.dims == 0)
            أرجع سائيز();
        إذا (m.dims == 1)
            أرجع سائيز(1, m.size[0]);
        أرجع سائيز(m.size[1], m.size[0]);
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا( k == OPENGL_بفر )
    {
        CV_Assert( i < 0 );
        ثابت ogl::بفر_2* buf = (ثابت ogl::بفر_2*)obj;
        أرجع buf->size();
    }

    إذا( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        أرجع d_mat->size();
    }

    إذا( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        أرجع cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحيح داخلا_Array_2::sizend(صحيح* arrsz, صحيح i) ثابت
{
    صحيح j, d = 0;
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == NONE )
        ;
    وإلا إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا( k == STD_ويڪٽر_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i < (صحيح)vv.size() );
        ثابت Mat& m = vv[i];
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.اوچائي );
        ثابت Mat& m = vv[i];
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا( k == STD_ويڪٽر_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( i < (صحيح)vv.size() );
        ثابت UMat& m = vv[i];
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا (((k == STD_ويڪٽر || k == STD_BOOL_ويڪٽر || k == STD_ويڪٽر_ويڪٽر) && i < 0) ||
             (k == STD_ويڪٽر_ويڪٽر && i >= 0))
    {
        size_t n = ڪل(i);
        d = 1;
        إذا(arrsz)
        {
            arrsz[0] = صحيح(n);
        }
    }
    وإلا
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        سائيز sz2d = size(i);
        d = 2;
        إذا(arrsz)
        {
            arrsz[0] = sz2d.اوچائي;
            arrsz[1] = sz2d.ويڪر;
        }
    }

    أرجع d;
}

منطقي داخلا_Array_2::empty(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (i >= 0) {
        إذا (k == STD_ويڪٽر_MAT) {
            خودڪار mv = reinterpret_cast<ثابت std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            أرجع mv->at(i).empty();
        }
        وإلا إذا (k == STD_ويڪٽر_UMAT) {
            خودڪار umv = reinterpret_cast<ثابت std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            أرجع umv->at(i).empty();
        }
        وإلا إذا (k == STD_ويڪٽر || k == STD_BOOL_ويڪٽر || k == STD_ويڪٽر_ويڪٽر) {
            size_t n = ڪل(i);
            أرجع n == 0;
        } وإلا {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    أرجع empty();
}

MatShape داخلا_Array_2::shape(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    MatShape shape;

    إذا( k == NONE )
        ;
    وإلا إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat& m = *(ثابت Mat*)obj;
        shape = m.size;
    }
    وإلا إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat& m = *(ثابت UMat*)obj;
        shape = m.size;
    }
    وإلا إذا( k == STD_ويڪٽر_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    وإلا إذا( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.اوچائي );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    وإلا إذا( k == STD_ويڪٽر_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت UMat& m = vv[i];
        shape = m.size;
    }
    وإلا
    {
        صحيح sizes[CV_MAX_DIM];
        صحيح dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    أرجع shape;
}

منطقي داخلا_Array_2::same_سائيز(ثابت داخلا_Array_2& arr) ثابت
{
    داخلا_Array_2::قسم_Flag k1 = قسم(), k2 = arr.قسم();
    سائيز sz1;

    إذا( k1 == MAT )
    {
        ثابت Mat* m = ((ثابت Mat*)obj);
        إذا( k2 == MAT )
            أرجع m->size == ((ثابت Mat*)arr.obj)->size;
        إذا( k2 == UMAT )
            أرجع m->size == ((ثابت UMat*)arr.obj)->size;
        إذا( m->dims > 2 )
            أرجع خطأ;
        sz1 = m->size();
    }
    وإلا إذا( k1 == UMAT )
    {
        ثابت UMat* m = ((ثابت UMat*)obj);
        إذا( k2 == MAT )
            أرجع m->size == ((ثابت Mat*)arr.obj)->size;
        إذا( k2 == UMAT )
            أرجع m->size == ((ثابت UMat*)arr.obj)->size;
        إذا( m->dims > 2 )
            أرجع خطأ;
        sz1 = m->size();
    }
    وإلا إذا ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        أرجع ((ثابت cuda::GpuMatND*)obj)->size == ((ثابت cuda::GpuMatND*)arr.obj)->size;
    }
    وإلا
        sz1 = size();

    إذا( arr.dims() > 2 )
        أرجع خطأ;
    أرجع sz1 == arr.size();
}

صحيح داخلا_Array_2::dims_(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت Mat*)obj)->dims;
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت UMat*)obj)->dims;
    }

    إذا (k == MATX)
    {
        CV_Assert( i < 0 );
        أرجع 2;
    }

    إذا( k == STD_ويڪٽر || k == STD_BOOL_ويڪٽر )
    {
        CV_Assert( i < 0 );
        أرجع 1;
    }

    إذا( k == NONE )
        أرجع 0;

    إذا( k == STD_ويڪٽر_ويڪٽر )
    {
        إذا( i < 0 )
            أرجع 1;
        CV_Assert( size_t(i) < ڪل() );
        أرجع 1;
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        إذا( i < 0 )
            أرجع 1;
        CV_Assert( i < (صحيح)vv.size() );

        أرجع vv[i].dims;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع 1;
        CV_Assert( i < sz.اوچائي );

        أرجع vv[i].dims;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        إذا( i < 0 )
            أرجع 1;
        CV_Assert( i < (صحيح)vv.size() );

        أرجع vv[i].dims;
    }

    إذا( k == OPENGL_بفر )
    {
        CV_Assert( i < 0 );
        أرجع 2;
    }

    إذا( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        أرجع 2;
    }

    إذا( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت cuda::GpuMatND*)obj)->dims;
    }

    إذا( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        أرجع 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

size_t داخلا_Array_2::ڪل(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت Mat*)obj)->ڪل();
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت UMat*)obj)->ڪل();
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        إذا( i < 0 )
            أرجع vv.size();

        CV_Assert( i < (صحيح)vv.size() );
        أرجع vv[i].ڪل();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع sz.اوچائي;

        CV_Assert( i < sz.اوچائي );
        أرجع vv[i].ڪل();
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        إذا( i < 0 )
            أرجع vv.size();

        CV_Assert( i < (صحيح)vv.size() );
        أرجع vv[i].ڪل();
    }

    إذا( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        size_t res = 0;
        ثابت cuda::GpuMatND& _gpuMatND = *((ثابت cuda::GpuMatND*)obj);
        إذا (_gpuMatND.dims > 0)
        {
            res = 1;
            لكل(صحيح d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            أرجع res;
        }
    }

    إذا (k == STD_ويڪٽر || k == STD_ويڪٽر_ويڪٽر)
    {
        CV_Assert(i < 0 || k == STD_ويڪٽر_ويڪٽر);
        صحيح esz = CV_ELEM_سائيز(flags);

        #وصف_رد GET_VEC_سائيز
        #وصف_ڪريو GET_VEC_سائيز(T) \
            إذا (k == STD_ويڪٽر_ويڪٽر) { \
                ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                إذا (i < 0) \
                    أرجع n; \
                CV_Assert((size_t)i < n); \
                أرجع vv->at(i).size(); \
            } وإلا \
                أرجع ((ثابت std::vector<T >*)obj)->size()

        STD_ويڪٽر_SWITCH(esz, GET_VEC_سائيز)
    }

    أرجع size(i).area();
}

صحيح داخلا_Array_2::type_(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
        أرجع ((ثابت Mat*)obj)->type();

    إذا( k == UMAT )
        أرجع ((ثابت UMat*)obj)->type();

    إذا( k == MATX || k == STD_ويڪٽر || k == STD_ويڪٽر_ويڪٽر || k == STD_BOOL_ويڪٽر )
        أرجع CV_MAT_TYPE(flags);

    إذا( k == NONE )
        أرجع -1;

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        إذا( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            أرجع CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحيح)vv.size() );
        أرجع vv[i >= 0 ? i : 0].type();
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        إذا( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            أرجع CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (صحيح)vv.size() );
        أرجع vv[i >= 0 ? i : 0].type();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        إذا( sz.اوچائي == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            أرجع CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.اوچائي );
        أرجع vv[i >= 0 ? i : 0].type();
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        إذا (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            أرجع CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحيح)vv.size());
        أرجع vv[i >= 0 ? i : 0].type();
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        إذا (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            أرجع CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (صحيح)vv.size());
        أرجع vv[i >= 0 ? i : 0].type();
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا( k == OPENGL_بفر )
        أرجع ((ثابت ogl::بفر_2*)obj)->type();

    إذا( k == CUDA_GPU_MAT )
        أرجع ((ثابت cuda::GpuMat*)obj)->type();

    إذا( k == CUDA_GPU_MATND )
        أرجع ((ثابت cuda::GpuMatND*)obj)->type();

    إذا( k == CUDA_HOST_MEM )
        أرجع ((ثابت cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

صحيح داخلا_Array_2::اونهائي_4(صحيح i) ثابت
{
    أرجع CV_MAT_اونهائي(type(i));
}

منطقي داخلا_Array_2::خالي_جايون() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
        أرجع ((ثابت Mat*)obj)->empty();

    إذا( k == UMAT )
        أرجع ((ثابت UMat*)obj)->empty();

    إذا (k == MATX)
        أرجع خطأ;

    إذا( k == STD_ويڪٽر || k == STD_BOOL_ويڪٽر || k == STD_ويڪٽر_ويڪٽر )
    {
        size_t n = ڪل(-1);
        أرجع n == 0;
    }

    إذا( k == NONE )
        أرجع سچ;

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        أرجع vv.empty();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        أرجع sz.اوچائي == 0;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        أرجع vv.empty();
    }

    إذا( k == OPENGL_بفر )
        أرجع ((ثابت ogl::بفر_2*)obj)->empty();

    إذا( k == CUDA_GPU_MAT )
        أرجع ((ثابت cuda::GpuMat*)obj)->empty();

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        أرجع vv.empty();
    }

    إذا( k == CUDA_GPU_MATND )
        أرجع ((ثابت cuda::GpuMatND*)obj)->empty();

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        أرجع vv.empty();
    }

    إذا( k == CUDA_HOST_MEM )
        أرجع ((ثابت cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقي داخلا_Array_2::isContinuous(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
        أرجع i < 0 ? ((ثابت Mat*)obj)->isContinuous() : سچ;

    إذا( k == UMAT )
        أرجع i < 0 ? ((ثابت UMat*)obj)->isContinuous() : سچ;

    إذا( k == MATX || k == STD_ويڪٽر ||
        k == NONE || k == STD_ويڪٽر_ويڪٽر || k == STD_BOOL_ويڪٽر )
        أرجع سچ;

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isContinuous();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.اوچائي);
        أرجع vv[i].isContinuous();
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isContinuous();
    }

    إذا( k == CUDA_GPU_MAT )
      أرجع i < 0 ? ((ثابت cuda::GpuMat*)obj)->isContinuous() : سچ;

    إذا( k == CUDA_GPU_MATND )
        أرجع i < 0 ? ((ثابت cuda::GpuMatND*)obj)->isContinuous() : سچ;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقي داخلا_Array_2::isSubmatrix(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
        أرجع i < 0 ? ((ثابت Mat*)obj)->isSubmatrix() : خطأ;

    إذا( k == UMAT )
        أرجع i < 0 ? ((ثابت UMat*)obj)->isSubmatrix() : خطأ;

    إذا( k == MATX || k == STD_ويڪٽر ||
        k == NONE || k == STD_ويڪٽر_ويڪٽر || k == STD_BOOL_ويڪٽر )
        أرجع خطأ;

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isSubmatrix();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.اوچائي);
        أرجع vv[i].isSubmatrix();
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isSubmatrix();
    }

    إذا( k == CUDA_GPU_MATND )
    {
        أرجع ((ثابت cuda::GpuMatND*)obj)->isSubmatrix();
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t داخلا_Array_2::offset(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat * ثابت m = ((ثابت Mat*)obj);
        أرجع (size_t)(m->ptr() - m->datastart);
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت UMat*)obj)->offset;
    }

    إذا( k == MATX || k == STD_ويڪٽر ||
        k == NONE || k == STD_ويڪٽر_ويڪٽر || k == STD_BOOL_ويڪٽر )
        أرجع 0;

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحيح)vv.size() );

        أرجع (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.اوچائي );
        أرجع (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].offset;
    }

    إذا( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        ثابت cuda::GpuMat * ثابت m = ((ثابت cuda::GpuMat*)obj);
        أرجع (size_t)(m->معلومات - m->datastart);
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع (size_t)(vv[i].معلومات - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t داخلا_Array_2::قدم(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت Mat*)obj)->قدم;
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت UMat*)obj)->قدم;
    }

    إذا( k == MATX || k == STD_ويڪٽر ||
        k == NONE || k == STD_ويڪٽر_ويڪٽر || k == STD_BOOL_ويڪٽر )
        أرجع 0;

    إذا( k == STD_ويڪٽر_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحيح)vv.size() );
        أرجع vv[i].قدم;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.اوچائي );
        أرجع vv[i].قدم;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].قدم;
    }

    إذا( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت cuda::GpuMat*)obj)->قدم;
    }
    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].قدم;
    }
    إذا( k == CUDA_GPU_MATND )
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        أرجع _gpuMatND.قدم[i];
    }

    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ثابت cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        أرجع _gpuMatND.قدم[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

فراغ داخلا_Array_2::copy_منزل(ثابت _OutputArray& arr) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == NONE )
        arr.release();
    وإلا إذا( k == MAT || k == MATX || k == STD_ويڪٽر || k == STD_BOOL_ويڪٽر )
    {
        Mat m = getMat();
        m.copy_منزل(arr);
    }
    وإلا إذا( k == UMAT )
        ((UMat*)obj)->copy_منزل(arr);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    وإلا إذا (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_منزل(arr);
#جيڪڏهن_پورو
    وإلا
        CV_Error(Error::StsNotImplemented, "");
}

فراغ داخلا_Array_2::copy_منزل(ثابت _OutputArray& arr, ثابت داخلا_Array_2 & نقاب) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == NONE )
        arr.release();
    وإلا إذا( k == MAT || k == MATX || k == STD_ويڪٽر || k == STD_BOOL_ويڪٽر )
    {
        Mat m = getMat();
        m.copy_منزل(arr, نقاب);
    }
    وإلا إذا( k == UMAT )
        ((UMat*)obj)->copy_منزل(arr, نقاب);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
    وإلا إذا (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_منزل(arr, نقاب);
#جيڪڏهن_پورو
    وإلا
        CV_Error(Error::StsNotImplemented, "");
}

منطقي _OutputArray::fixed_سائيز() ثابت
{
    أرجع (flags & FIXED_سائيز) == FIXED_سائيز;
}

منطقي _OutputArray::fixedType() ثابت
{
    أرجع (flags & FIXED_TYPE) == FIXED_TYPE;
}

فراغ _OutputArray::create_(سائيز _sz, صحيح mtype, صحيح i, منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا( k == MAT && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((Mat*)obj)->size.عامل()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        أرجع;
    }
    إذا( k == UMAT && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((UMat*)obj)->size.عامل()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        أرجع;
    }
    إذا( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    إذا( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.اوچائي) && (((cuda::GpuMatND*)obj)->size[1] == _sz.ويڪر)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        cuda::GpuMatND::سائيز_Array sizes({_sz.اوچائي, _sz.ويڪر});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    إذا( k == OPENGL_بفر && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((ogl::بفر_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::بفر_2*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_OPENGL
        ((ogl::بفر_2*)obj)->create(_sz, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#جيڪڏهن_پورو
    }
    إذا( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    صحيح sizes[] = {_sz.اوچائي, _sz.ويڪر};
    create(2, sizes, mtype, i, allowTransposed, fixed_اونهائي_نقاب);
}

فراغ _OutputArray::create_(صحيح صفون_3, صحيح کانن_ڳڻپ_3, صحيح mtype, صحيح i, منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا( k == MAT && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((Mat*)obj)->size.عامل()() == سائيز(کانن_ڳڻپ_3, صفون_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(صفون_3, کانن_ڳڻپ_3, mtype);
        أرجع;
    }
    إذا( k == UMAT && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((UMat*)obj)->size.عامل()() == سائيز(کانن_ڳڻپ_3, صفون_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(صفون_3, کانن_ڳڻپ_3, mtype);
        أرجع;
    }
    إذا( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((cuda::GpuMat*)obj)->size() == سائيز(کانن_ڳڻپ_3, صفون_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(صفون_3, کانن_ڳڻپ_3, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    إذا( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == صفون_3) && (((cuda::GpuMatND*)obj)->size[1] == کانن_ڳڻپ_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        cuda::GpuMatND::سائيز_Array sizes({صفون_3, کانن_ڳڻپ_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    إذا( k == OPENGL_بفر && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((ogl::بفر_2*)obj)->size() == سائيز(کانن_ڳڻپ_3, صفون_3));
        CV_Assert(!fixedType() || ((ogl::بفر_2*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_OPENGL
        ((ogl::بفر_2*)obj)->create(صفون_3, کانن_ڳڻپ_3, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#جيڪڏهن_پورو
    }
    إذا( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
        CV_Assert(!fixed_سائيز() || ((cuda::HostMem*)obj)->size() == سائيز(کانن_ڳڻپ_3, صفون_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::HostMem*)obj)->create(صفون_3, کانن_ڳڻپ_3, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    صحيح sizes[] = {صفون_3, کانن_ڳڻپ_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_اونهائي_نقاب);
}

فراغ _OutputArray::create(صحيح d, ثابت صحيح* sizes, صحيح mtype, صحيح i,
                          منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    صحيح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    داخلا_Array_2::قسم_Flag k = قسم();
    mtype = CV_MAT_TYPE(mtype);

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_سائيز()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.صفون == size0 && m.کانن_ڳڻپ == size1) ||
            (allowTransposed && m.صفون == size1 && m.کانن_ڳڻپ == size0 && m.isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_اونهائي(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m.type();
            وإلا
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_سائيز())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_سائيز()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.صفون == size0 && m.کانن_ڳڻپ == size1) ||
            (allowTransposed && m.صفون == size1 && m.کانن_ڳڻپ == size0 && m.isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m.type();
            وإلا
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_سائيز())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا( k == MATX )
    {
        CV_Assert( i < 0 );
        صحيح type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_اونهائي_نقاب) != 0) );
        CV_CheckLE(d, 2, "");
        سائيز requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        إذا (sz.ويڪر == 1 || sz.اوچائي == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            صحيح ڪل_1d = std::max(sz.ويڪر, sz.اوچائي);
            CV_Check(requested_size, std::max(requested_size.ويڪر, requested_size.اوچائي) == ڪل_1d, "");
        }
        وإلا
        {
            إذا (!allowTransposed)
            {
                CV_CheckEQ(requested_size, sz, "");
            }
            وإلا
            {
                CV_Check(requested_size,
                        (requested_size == sz || (requested_size.اوچائي == sz.ويڪر && requested_size.ويڪر == sz.اوچائي)),
                        "");
            }
        }
        أرجع;
    }

    إذا( k == STD_ويڪٽر || k == STD_ويڪٽر_ويڪٽر )
    {
        CV_Assert(!fixed_سائيز());
        CV_Assert(k == STD_ويڪٽر_ويڪٽر || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        صحيح esz = CV_ELEM_سائيز(flags);

        إذا( k == STD_ويڪٽر || (k == STD_ويڪٽر_ويڪٽر && i >= 0) )
        {
            صحيح type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_اونهائي_نقاب) != 0) );
        }

    #وصف_رد RESIZE_VEC
    #وصف_ڪريو RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            إذا (k == STD_ويڪٽر_ويڪٽر) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                إذا (i < 0) { \
                    CV_Assert(!fixed_سائيز() || len == vv->size()); \
                    vv->resize(len); \
                    أرجع; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } وإلا { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_سائيز() || len == v->size()); \
            v->resize(len); \
        } \
        اكسر

        STD_ويڪٽر_SWITCH(esz, RESIZE_VEC)
        أرجع;
    }

    إذا( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        إذا( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_سائيز() || len == len0);
            v.resize(len);
            إذا( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    إذا( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_نقاب) | _type;
                }
            }
            أرجع;
        }

        CV_Assert( i < (صحيح)v.size() );
        Mat& m = v[i];

        إذا( allowTransposed )
        {
            إذا( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_سائيز());
                m.release();
            }

            إذا( d == 2 && m.dims == 2 && m.معلومات &&
                m.type() == mtype && m.صفون == sizes[1] && m.کانن_ڳڻپ == sizes[0] )
                أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m.type();
            وإلا
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        إذا(fixed_سائيز())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        إذا( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.اوچائي;

            CV_Assert(len == len0);
            إذا( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    إذا( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_نقاب) | _type;
                }
            }
            أرجع;
        }

        CV_Assert( i < sz.اوچائي );
        Mat& m = v[i];

        إذا( allowTransposed )
        {
            إذا( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_سائيز());
                m.release();
            }

            إذا( d == 2 && m.dims == 2 && m.معلومات &&
                m.type() == mtype && m.صفون == sizes[1] && m.کانن_ڳڻپ == sizes[0] )
                أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m.type();
            وإلا
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        إذا(fixed_سائيز())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        إذا( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_سائيز() || len == len0);
            v.resize(len);
            إذا( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    إذا( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_نقاب) | _type;
                }
            }
            أرجع;
        }

        CV_Assert( i < (صحيح)v.size() );
        UMat& m = v[i];

        إذا( allowTransposed )
        {
            إذا( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_سائيز());
                m.release();
            }

            إذا( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.صفون == sizes[1] && m.کانن_ڳڻپ == sizes[0] )
                أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m.type();
            وإلا
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        إذا(fixed_سائيز())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_اونهائي_نقاب);
        أرجع;
    }

    إذا( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_اونهائي_نقاب == 0 )
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        cuda::GpuMatND::سائيز_Array سائيز_Array_2 = cuda::GpuMatND::سائيز_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(سائيز_Array_2, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _OutputArray::create(ثابت MatShape& shape, صحيح mtype, صحيح i,
                          منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    إذا (shape.dims < 0) {
        release();
    } وإلا {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_اونهائي_نقاب);
    }
}

Mat _OutputArray::reinterpret(صحيح mtype) ثابت
{
    mtype = CV_MAT_TYPE(mtype);
    أرجع getMat().reinterpret(mtype);
}

فراغ _OutputArray::create_Same_سائيز(ثابت داخلا_Array_2& arr, صحيح mtype) ثابت
{
    create(arr.shape(), mtype);
}

فراغ _OutputArray::fit(سائيز _sz, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    صحيح sizes[] = {_sz.اوچائي, _sz.ويڪر};
    fit(2, sizes, mtype, i, allowTransposed, fixed_اونهائي_نقاب);
}

فراغ _OutputArray::fit(صحيح صفون, صحيح کانن_ڳڻپ, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    صحيح sizes[] = {صفون, کانن_ڳڻپ};
    fit(2, sizes, mtype, i, allowTransposed, fixed_اونهائي_نقاب);
}

فراغ _OutputArray::fit(ثابت MatShape& shape, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    إذا (shape.dims < 0) {
        release();
        أرجع;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_اونهائي_نقاب);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا ((k == MAT && i < 0) || (k == STD_ويڪٽر_MAT && i >= 0)) {
        Mat_سائيز& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } وإلا إذا ((k == UMAT && i < 0) || (k == STD_ويڪٽر_UMAT && i >= 0)) {
        Mat_سائيز& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

فراغ _OutputArray::fit_Same_سائيز(ثابت داخلا_Array_2& arr, صحيح mtype) ثابت
{
    fit(arr.shape(), mtype);
}

فراغ _OutputArray::fit(صحيح d, ثابت صحيح* sizes, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::اونهائي_نقاب fixed_اونهائي_نقاب) ثابت
{
    صحيح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    داخلا_Array_2::قسم_Flag k = قسم();
    mtype = CV_MAT_TYPE(mtype);

    إذا( (k == MAT && i < 0) || (k == STD_ويڪٽر_MAT && i >= 0) )
    {
        Mat* m;
        إذا (k == MAT)
            m = (Mat*)obj;
        وإلا {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_سائيز()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->صفون == size0 && m->کانن_ڳڻپ == size1) ||
             (allowTransposed && m->صفون == size1 && m->کانن_ڳڻپ == size0 && m->isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_اونهائي(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m->type();
            وإلا
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_سائيز())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        أرجع;
    }

    إذا( (k == UMAT && i < 0) || (k == STD_ويڪٽر_UMAT && i >= 0) )
    {
        UMat* m;
        إذا (k == UMAT)
            m = (UMat*)obj;
        وإلا {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_سائيز()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->صفون == size0 && m->کانن_ڳڻپ == size1) ||
             (allowTransposed && m->صفون == size1 && m->کانن_ڳڻپ == size0 && m->isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_اونهائي(flags)) & fixed_اونهائي_نقاب) != 0 )
                mtype = m->type();
            وإلا
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_سائيز())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        أرجع;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_اونهائي_نقاب);
}

فراغ _OutputArray::release() ثابت
{
    CV_Assert(!fixed_سائيز());

    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        ((Mat*)obj)->release();
        أرجع;
    }

    إذا( k == UMAT )
    {
        ((UMat*)obj)->release();
        أرجع;
    }

    إذا( k == CUDA_GPU_MAT )
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا( k == CUDA_GPU_MATND )
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا( k == CUDA_HOST_MEM )
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }

    إذا( k == OPENGL_بفر )
    {
#جيڪڏهن_وصف_ٿيل HAVE_OPENGL
        ((ogl::بفر_2*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#جيڪڏهن_پورو
    }

    إذا( k == NONE )
        أرجع;

    إذا( k == STD_ويڪٽر || k == STD_ويڪٽر_ويڪٽر )
    {
    #وصف_رد CLEAR_VEC
    #وصف_ڪريو CLEAR_VEC(T) \
        إذا (k == STD_ويڪٽر_ويڪٽر) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } وإلا { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        اكسر

        صحيح esz = CV_ELEM_سائيز(flags);
        STD_ويڪٽر_SWITCH(esz, CLEAR_VEC);
        أرجع;
    }

    إذا( k == STD_ويڪٽر_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        أرجع;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        لكل( صحيح i = 0; i < sz.اوچائي; i++ )
            v[i].release();
        أرجع;
    }

    إذا( k == STD_BOOL_ويڪٽر )
    {
        ((std::vector<منطقي>*)obj)->clear();
        أرجع;
    }

    إذا( k == STD_ويڪٽر_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        أرجع;
    }
    إذا (k == STD_ويڪٽر_CUDA_GPU_MAT)
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _OutputArray::clear() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == MAT )
    {
        CV_Assert(!fixed_سائيز());
        ((Mat*)obj)->resize(0);
        أرجع;
    }

    release();
}

منطقي _OutputArray::needed() ثابت
{
    أرجع قسم() != NONE;
}

Mat& _OutputArray::getMatRef(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا( i < 0 )
    {
        CV_Assert( k == MAT );
        أرجع *(Mat*)obj;
    }

    CV_Assert( k == STD_ويڪٽر_MAT || k == STD_ARRAY_MAT );

    إذا( k == STD_ويڪٽر_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (صحيح)v.size() );
        أرجع v[i];
    }
    وإلا
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.اوچائي );
        أرجع v[i];
    }
}

UMat& _OutputArray::getUMatRef(صحيح i) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا( i < 0 )
    {
        CV_Assert( k == UMAT );
        أرجع *(UMat*)obj;
    }
    وإلا
    {
        CV_Assert( k == STD_ويڪٽر_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (صحيح)v.size() );
        أرجع v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == CUDA_GPU_MAT );
    أرجع *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    CV_Assert(k == STD_ويڪٽر_CUDA_GPU_MAT);
    أرجع *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == CUDA_GPU_MATND );
    أرجع *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    CV_Assert(k == STD_ويڪٽر_CUDA_GPU_MAT_ND);
    أرجع *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::بفر_2& _OutputArray::get_OGl_بفر_Ref() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == OPENGL_بفر );
    أرجع *(ogl::بفر_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    CV_Assert( k == CUDA_HOST_MEM );
    أرجع *(cuda::HostMem*)obj;
}

فراغ _OutputArray::سيٽ_منزل(ثابت داخلا_Array_2& arr, ثابت داخلا_Array_2 & نقاب) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == NONE )
        ;
    وإلا إذا (k == MAT || k == MATX || k == STD_ويڪٽر)
    {
        Mat m = getMat();
        m.سيٽ_منزل(arr, نقاب);
    }
    وإلا إذا( k == UMAT )
        ((UMat*)obj)->سيٽ_منزل(arr, نقاب);
    وإلا إذا( k == CUDA_GPU_MAT )
    {
#جيڪڏهن_وصف_ٿيل HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.قسم(), داخلا_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->سيٽ_منزل(Scalar(Vec<مزدوج, 4>(value.ptr<مزدوج>())), نقاب);
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#جيڪڏهن_پورو
    }
    وإلا
        CV_Error(Error::StsNotImplemented, "");
}

فراغ _OutputArray::سيٽ_Zero() ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();

    إذا( k == NONE )
        ;
    وإلا إذا (k == MAT || k == MATX || k == STD_ويڪٽر)
    {
        Mat m = getMat();
        m.سيٽ_Zero();
    }
    وإلا
    {
        سيٽ_منزل(Scalar::all(0), noArray());
    }
}

فراغ _OutputArray::assign(ثابت UMat& u) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    وإلا إذا (k == MAT)
    {
        u.copy_منزل(*(Mat*)obj); // TODO check u.getMat()
    }
    وإلا إذا (k == MATX)
    {
        u.copy_منزل(getMat()); // TODO check u.getMat()
    }
    وإلا
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::assign(ثابت Mat& m) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == UMAT)
    {
        m.copy_منزل(*(UMat*)obj); // TODO check m.getUMat()
    }
    وإلا إذا (k == MAT)
    {
        *(Mat*)obj = m;
    }
    وإلا إذا (k == MATX)
    {
        m.copy_منزل(getMat());
    }
    وإلا
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::move(UMat& u) ثابت
{
    إذا (fixed_سائيز())
    {
        // TODO Performance warning
        assign(u);
        أرجع;
    }
    صحيح k = قسم();
    إذا (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    وإلا إذا (k == MAT)
    {
        u.copy_منزل(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    وإلا إذا (k == MATX)
    {
        u.copy_منزل(getMat()); // TODO check u.getMat()
        u.release();
    }
    وإلا
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::move(Mat& m) ثابت
{
    إذا (fixed_سائيز())
    {
        // TODO Performance warning
        assign(m);
        أرجع;
    }
    صحيح k = قسم();
    إذا (k == UMAT)
    {
        m.copy_منزل(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    وإلا إذا (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    وإلا إذا (k == MATX)
    {
        m.copy_منزل(getMat());
        m.release();
    }
    وإلا
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::assign(ثابت std::vector<UMat>& v) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == STD_ويڪٽر_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            UMat& this_m = this_v[i];
            إذا (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copy_منزل(this_m);
        }
    }
    وإلا إذا (k == STD_ويڪٽر_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت UMat& m = v[i];
            Mat& this_m = this_v[i];
            إذا (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copy_منزل(this_m);
        }
    }
    وإلا
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


فراغ _OutputArray::assign(ثابت std::vector<Mat>& v) ثابت
{
    داخلا_Array_2::قسم_Flag k = قسم();
    إذا (k == STD_ويڪٽر_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            UMat& this_m = this_v[i];
            إذا (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copy_منزل(this_m);
        }
    }
    وإلا إذا (k == STD_ويڪٽر_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        لكل (size_t i = 0; i < v.size(); i++)
        {
            ثابت Mat& m = v[i];
            Mat& this_m = this_v[i];
            إذا (this_m.u != عدم && this_m.u == m.u)
                استمر; // same object (see dnn::Layer::forward_fallback)
            m.copy_منزل(this_m);
        }
    }
    وإلا
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


ساكن داخلا_Output_Array_2 _none;
داخلا_Output_Array noArray() { أرجع _none; }

} // cv::
