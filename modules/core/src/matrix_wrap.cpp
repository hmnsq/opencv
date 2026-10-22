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

#تعریف_لغوه STD_ویکټور_SWITCH
#تعریف_کړه STD_ویکټور_SWITCH(esz, func) \
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

Mat ننوت_Array_2::getMat_(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    AccessFlag accessFlags = flags & ACCESS_ماسک;

    إذا( k == MAT )
    {
        ثابت Mat* m = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع *m;
        أرجع m->لیکه(i);
    }

    إذا( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        إذا( i < 0 )
            أرجع m->getMat(accessFlags);
        أرجع m->getMat(accessFlags).لیکه(i);
    }

    إذا (k == MATX)
    {
        CV_Assert( i < 0 );
        أرجع Mat(sz, flags, obj);
    }

    إذا( k == STD_BOOL_ویکټور )
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

    إذا( k == STD_ویکټور || k == STD_ویکټور_ویکټور )
    {
        صحيح t = CV_MAT_TYPE(flags);
        صحيح esz = CV_ELEM_اندازه(t);

    #تعریف_لغوه GET_MAT
    #تعریف_کړه GET_MAT(T) \
        { \
            std::vector<T>* v; \
            إذا (k == STD_ویکټور_ویکټور) { \
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

        STD_ویکټور_SWITCH(esz, GET_MAT)
    }

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& v = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (صحيح)v.size() );

        أرجع v[i];
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* v = (ثابت Mat*)obj;
        CV_Assert( 0 <= i && i < sz.لوړوالی );

        أرجع v[i];
    }

    إذا( k == STD_ویکټور_UMAT )
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

        أرجع cuda_mem->create_Mat_سرلیک();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat ننوت_Array_2::getUMat(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    AccessFlag accessFlags = flags & ACCESS_ماسک;

    إذا( k == UMAT )
    {
        ثابت UMat* m = (ثابت UMat*)obj;
        إذا( i < 0 )
            أرجع *m;
        أرجع m->لیکه(i);
    }

    إذا( k == STD_ویکټور_UMAT )
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
        أرجع m->لیکه(i).getUMat(accessFlags);
    }

    أرجع getMat(i).getUMat(accessFlags);
}

فراغ ننوت_Array_2::get_Mat_ویکټور(std::vector<Mat>& mv) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    AccessFlag accessFlags = flags & ACCESS_ماسک;

    إذا( k == MAT )
    {
        ثابت Mat& m = *(ثابت Mat*)obj;
        صحيح n = (صحيح)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        لكل( صحيح i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.ستون_شمېر, m.type(), (فراغ*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (فراغ*)m.ptr(i), &m.ګام[1]);
        أرجع;
    }

    إذا (k == MATX)
    {
        size_t n = sz.لوړوالی, esz = CV_ELEM_اندازه(flags);
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.پلنوالی, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.پلنوالی*i);
        أرجع;
    }

    إذا( k == STD_ویکټور )
    {
        ثابت std::vector<uchar>& v = *(ثابت std::vector<uchar>*)obj;

        size_t n = ټول(), esz = CV_ELEM_اندازه(flags);
        صحيح t = CV_MAT_ژوروالی(flags), cn = CV_MAT_CN(flags);
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

    إذا( k == STD_ویکټور_ویکټور )
    {
        صحيح typ = CV_MAT_TYPE(flags);
        صحيح esz = CV_ELEM_اندازه(typ);

        #تعریف_لغوه GET_VEC_VEC_MAT_VEC
        #تعریف_کړه GET_VEC_VEC_MAT_VEC(T) \
        { \
            ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
            size_t n = vv->size(); \
            mv.resize(n); \
            لكل (size_t i = 0; i < n; i++) { \
                ثابت std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (size_t)لوی_عدد); \
                صحيح ni = (صحيح)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (فراغ*)vi.معلومات()) : Mat(); \
            } \
        } \
        اكسر

        STD_ویکټور_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        أرجع;
    }

    إذا( k == STD_ویکټور_MAT )
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
        size_t n = sz.لوړوالی;
        mv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        أرجع;
    }

    إذا( k == STD_ویکټور_UMAT )
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

فراغ ننوت_Array_2::get_UMat_ویکټور(std::vector<UMat>& umv) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    AccessFlag accessFlags = flags & ACCESS_ماسک;

    إذا( k == NONE )
    {
        umv.clear();
        أرجع;
    }

    إذا( k == STD_ویکټور_MAT )
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
        size_t n = sz.لوړوالی;
        umv.resize(n);

        لكل( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        أرجع;
    }

    إذا( k == STD_ویکټور_UMAT )
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

cuda::GpuMat ننوت_Array_2::getGpuMat() ثابت
{
#که_تعریف_شوی HAVE_CUDA
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا (k == CUDA_GPU_MAT)
    {
        ثابت cuda::GpuMat* d_mat = (ثابت cuda::GpuMat*)obj;
        أرجع *d_mat;
    }

    إذا (k == CUDA_HOST_MEM)
    {
        ثابت cuda::HostMem* cuda_mem = (ثابت cuda::HostMem*)obj;
        أرجع cuda_mem->create_Gpu_Mat_سرلیک();
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
#د_که_پای
}
فراغ ننوت_Array_2::get_Gpu_Mat_ویکټور(std::vector<cuda::GpuMat>& gpumv) ثابت
{
#که_تعریف_شوی HAVE_CUDA
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#وإلا
    CV_ناکارول_شوی(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
}
فراغ ننوت_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) ثابت
{
#که_تعریف_شوی HAVE_CUDA
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#وإلا
    CV_ناکارول_شوی(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
}
cuda::GpuMatND ننوت_Array_2::getGpuMatND() ثابت
{
#که_تعریف_شوی HAVE_CUDA
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND* d_mat = (ثابت cuda::GpuMatND*)obj;
        أرجع *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#وإلا
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
}
ogl::بفر_2 ننوت_Array_2::get_OGl_بفر() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    CV_Assert(k == OPENGL_بفر);

    ثابت ogl::بفر_2* gl_buf = (ثابت ogl::بفر_2*)obj;
    أرجع *gl_buf;
}

صحيح ننوت_Array_2::لیکې_2(صحيح i) ثابت
{
#که_تعریف_شوی HAVE_CUDA
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        أرجع (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#د_که_پای

    أرجع اندازه_3(i).لوړوالی;
}

صحيح ننوت_Array_2::ستون_شمېر_2(صحيح i) ثابت
{
#که_تعریف_شوی HAVE_CUDA
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (k == CUDA_GPU_MATND)
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        أرجع (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#د_که_پای

    أرجع اندازه_3(i).پلنوالی;
}

اندازه ننوت_Array_2::اندازه_3(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        ثابت Mat* m = (ثابت Mat*)obj;
        CV_Assert(m->dims <= 2);
        أرجع اندازه(m->ستون_شمېر, m->لیکې);
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        ثابت UMat* m = (ثابت UMat*)obj;
        CV_Assert(m->dims <= 2);
        أرجع اندازه(m->ستون_شمېر, m->لیکې);
    }

    إذا (k == MATX)
    {
        CV_Assert( i < 0 );
        أرجع sz;
    }

    إذا( k == NONE )
        أرجع اندازه();

    إذا( k == STD_ویکټور || k == STD_ویکټور_ویکټور || k == STD_BOOL_ویکټور )
    {
        size_t n = ټول(i);
        CV_Assert(n <= (size_t)لوی_عدد);
        أرجع اندازه((صحيح)n, 1);
    }

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        إذا( i < 0 )
            أرجع vv.empty() ? اندازه() : اندازه((صحيح)vv.size(), 1);
        CV_Assert( i < (صحيح)vv.size() );

        أرجع vv[i].size();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع sz.لوړوالی==0 ? اندازه() : اندازه(sz.لوړوالی, 1);
        CV_Assert( i < sz.لوړوالی );

        أرجع vv[i].size();
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
#که_تعریف_شوی HAVE_CUDA
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        إذا (i < 0)
            أرجع vv.empty() ? اندازه() : اندازه((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        أرجع vv[i].size();
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
#که_تعریف_شوی HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        إذا (i < 0)
            أرجع vv.empty() ? اندازه() : اندازه((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        إذا (m.dims == 0)
            أرجع اندازه();
        إذا (m.dims == 1)
            أرجع اندازه(1, m.size[0]);
        أرجع اندازه(m.size[1], m.size[0]);
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        إذا( i < 0 )
            أرجع vv.empty() ? اندازه() : اندازه((صحيح)vv.size(), 1);
        CV_Assert( i < (صحيح)vv.size() );

        أرجع vv[i].size();
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
#که_تعریف_شوی HAVE_CUDA
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        إذا (i < 0)
            أرجع vv.empty() ? اندازه() : اندازه((صحيح)vv.size(), 1);
        CV_Assert(i < (صحيح)vv.size());
        ثابت cuda::GpuMatND& m = vv[i];
        إذا (m.dims == 0)
            أرجع اندازه();
        إذا (m.dims == 1)
            أرجع اندازه(1, m.size[0]);
        أرجع اندازه(m.size[1], m.size[0]);
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
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

صحيح ننوت_Array_2::sizend(صحيح* arrsz, صحيح i) ثابت
{
    صحيح j, d = 0;
    ننوت_Array_2::ډول_Flag k = ډول();

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
    وإلا إذا( k == STD_ویکټور_MAT && i >= 0 )
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
        CV_Assert( i < sz.لوړوالی );
        ثابت Mat& m = vv[i];
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا( k == STD_ویکټور_UMAT && i >= 0 )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert( i < (صحيح)vv.size() );
        ثابت UMat& m = vv[i];
        d = m.dims;
        إذا(arrsz)
            لكل(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    وإلا إذا (((k == STD_ویکټور || k == STD_BOOL_ویکټور || k == STD_ویکټور_ویکټور) && i < 0) ||
             (k == STD_ویکټور_ویکټور && i >= 0))
    {
        size_t n = ټول(i);
        d = 1;
        إذا(arrsz)
        {
            arrsz[0] = صحيح(n);
        }
    }
    وإلا
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        اندازه sz2d = size(i);
        d = 2;
        إذا(arrsz)
        {
            arrsz[0] = sz2d.لوړوالی;
            arrsz[1] = sz2d.پلنوالی;
        }
    }

    أرجع d;
}

منطقي ننوت_Array_2::empty(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (i >= 0) {
        إذا (k == STD_ویکټور_MAT) {
            خپلکاری mv = reinterpret_cast<ثابت std::vector<Mat>*>(obj);
            CV_Assert((size_t)i < mv->size());
            أرجع mv->at(i).empty();
        }
        وإلا إذا (k == STD_ویکټور_UMAT) {
            خپلکاری umv = reinterpret_cast<ثابت std::vector<UMat>*>(obj);
            CV_Assert((size_t)i < umv->size());
            أرجع umv->at(i).empty();
        }
        وإلا إذا (k == STD_ویکټور || k == STD_BOOL_ویکټور || k == STD_ویکټور_ویکټور) {
            size_t n = ټول(i);
            أرجع n == 0;
        } وإلا {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    أرجع empty();
}

MatShape ننوت_Array_2::shape(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
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
    وإلا إذا( k == STD_ویکټور_MAT && i >= 0 )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( (size_t)i < vv.size() );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    وإلا إذا( k == STD_ARRAY_MAT && i >= 0 )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i < sz.لوړوالی );
        ثابت Mat& m = vv[i];
        shape = m.size;
    }
    وإلا إذا( k == STD_ویکټور_UMAT && i >= 0 )
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

منطقي ننوت_Array_2::same_اندازه(ثابت ننوت_Array_2& arr) ثابت
{
    ننوت_Array_2::ډول_Flag k1 = ډول(), k2 = arr.ډول();
    اندازه sz1;

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

صحيح ننوت_Array_2::dims_(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

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

    إذا( k == STD_ویکټور || k == STD_BOOL_ویکټور )
    {
        CV_Assert( i < 0 );
        أرجع 1;
    }

    إذا( k == NONE )
        أرجع 0;

    إذا( k == STD_ویکټور_ویکټور )
    {
        إذا( i < 0 )
            أرجع 1;
        CV_Assert( size_t(i) < ټول() );
        أرجع 1;
    }

    إذا( k == STD_ویکټور_MAT )
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
        CV_Assert( i < sz.لوړوالی );

        أرجع vv[i].dims;
    }

    إذا( k == STD_ویکټور_UMAT )
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

size_t ننوت_Array_2::ټول(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت Mat*)obj)->ټول();
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت UMat*)obj)->ټول();
    }

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        إذا( i < 0 )
            أرجع vv.size();

        CV_Assert( i < (صحيح)vv.size() );
        أرجع vv[i].ټول();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        إذا( i < 0 )
            أرجع sz.لوړوالی;

        CV_Assert( i < sz.لوړوالی );
        أرجع vv[i].ټول();
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        إذا( i < 0 )
            أرجع vv.size();

        CV_Assert( i < (صحيح)vv.size() );
        أرجع vv[i].ټول();
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

    إذا (k == STD_ویکټور || k == STD_ویکټور_ویکټور)
    {
        CV_Assert(i < 0 || k == STD_ویکټور_ویکټور);
        صحيح esz = CV_ELEM_اندازه(flags);

        #تعریف_لغوه GET_VEC_اندازه
        #تعریف_کړه GET_VEC_اندازه(T) \
            إذا (k == STD_ویکټور_ویکټور) { \
                ثابت std::vector<std::vector<T > >* vv = (ثابت std::vector<std::vector<T > >*)obj; \
                size_t n = vv->size(); \
                إذا (i < 0) \
                    أرجع n; \
                CV_Assert((size_t)i < n); \
                أرجع vv->at(i).size(); \
            } وإلا \
                أرجع ((ثابت std::vector<T >*)obj)->size()

        STD_ویکټور_SWITCH(esz, GET_VEC_اندازه)
    }

    أرجع size(i).area();
}

صحيح ننوت_Array_2::type_(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
        أرجع ((ثابت Mat*)obj)->type();

    إذا( k == UMAT )
        أرجع ((ثابت UMat*)obj)->type();

    إذا( k == MATX || k == STD_ویکټور || k == STD_ویکټور_ویکټور || k == STD_BOOL_ویکټور )
        أرجع CV_MAT_TYPE(flags);

    إذا( k == NONE )
        أرجع -1;

    إذا( k == STD_ویکټور_UMAT )
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

    إذا( k == STD_ویکټور_MAT )
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
        إذا( sz.لوړوالی == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            أرجع CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.لوړوالی );
        أرجع vv[i >= 0 ? i : 0].type();
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
#که_تعریف_شوی HAVE_CUDA
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
#د_که_پای
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
#که_تعریف_شوی HAVE_CUDA
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
#د_که_پای
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

صحيح ننوت_Array_2::ژوروالی_4(صحيح i) ثابت
{
    أرجع CV_MAT_ژوروالی(type(i));
}

منطقي ننوت_Array_2::تشه() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
        أرجع ((ثابت Mat*)obj)->empty();

    إذا( k == UMAT )
        أرجع ((ثابت UMat*)obj)->empty();

    إذا (k == MATX)
        أرجع خطأ;

    إذا( k == STD_ویکټور || k == STD_BOOL_ویکټور || k == STD_ویکټور_ویکټور )
    {
        size_t n = ټول(-1);
        أرجع n == 0;
    }

    إذا( k == NONE )
        أرجع ریښتیا;

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        أرجع vv.empty();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        أرجع sz.لوړوالی == 0;
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        أرجع vv.empty();
    }

    إذا( k == OPENGL_بفر )
        أرجع ((ثابت ogl::بفر_2*)obj)->empty();

    إذا( k == CUDA_GPU_MAT )
        أرجع ((ثابت cuda::GpuMat*)obj)->empty();

    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        أرجع vv.empty();
    }

    إذا( k == CUDA_GPU_MATND )
        أرجع ((ثابت cuda::GpuMatND*)obj)->empty();

    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        أرجع vv.empty();
    }

    إذا( k == CUDA_HOST_MEM )
        أرجع ((ثابت cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقي ننوت_Array_2::isContinuous(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
        أرجع i < 0 ? ((ثابت Mat*)obj)->isContinuous() : ریښتیا;

    إذا( k == UMAT )
        أرجع i < 0 ? ((ثابت UMat*)obj)->isContinuous() : ریښتیا;

    إذا( k == MATX || k == STD_ویکټور ||
        k == NONE || k == STD_ویکټور_ویکټور || k == STD_BOOL_ویکټور )
        أرجع ریښتیا;

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isContinuous();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.لوړوالی);
        أرجع vv[i].isContinuous();
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isContinuous();
    }

    إذا( k == CUDA_GPU_MAT )
      أرجع i < 0 ? ((ثابت cuda::GpuMat*)obj)->isContinuous() : ریښتیا;

    إذا( k == CUDA_GPU_MATND )
        أرجع i < 0 ? ((ثابت cuda::GpuMatND*)obj)->isContinuous() : ریښتیا;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

منطقي ننوت_Array_2::isSubmatrix(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
        أرجع i < 0 ? ((ثابت Mat*)obj)->isSubmatrix() : خطأ;

    إذا( k == UMAT )
        أرجع i < 0 ? ((ثابت UMat*)obj)->isSubmatrix() : خطأ;

    إذا( k == MATX || k == STD_ویکټور ||
        k == NONE || k == STD_ویکټور_ویکټور || k == STD_BOOL_ویکټور )
        أرجع خطأ;

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isSubmatrix();
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert(i >= 0 && i < sz.لوړوالی);
        أرجع vv[i].isSubmatrix();
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isSubmatrix();
    }

    إذا( k == CUDA_GPU_MATND )
    {
        أرجع ((ثابت cuda::GpuMatND*)obj)->isSubmatrix();
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

size_t ننوت_Array_2::offset(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

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

    إذا( k == MATX || k == STD_ویکټور ||
        k == NONE || k == STD_ویکټور_ویکټور || k == STD_BOOL_ویکټور )
        أرجع 0;

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحيح)vv.size() );

        أرجع (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.لوړوالی );
        أرجع (size_t)(vv[i].ptr() - vv[i].datastart);
    }

    إذا( k == STD_ویکټور_UMAT )
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

    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع (size_t)(vv[i].معلومات - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

size_t ننوت_Array_2::ګام(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت Mat*)obj)->ګام;
    }

    إذا( k == UMAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت UMat*)obj)->ګام;
    }

    إذا( k == MATX || k == STD_ویکټور ||
        k == NONE || k == STD_ویکټور_ویکټور || k == STD_BOOL_ویکټور )
        أرجع 0;

    إذا( k == STD_ویکټور_MAT )
    {
        ثابت std::vector<Mat>& vv = *(ثابت std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (صحيح)vv.size() );
        أرجع vv[i].ګام;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        ثابت Mat* vv = (ثابت Mat*)obj;
        CV_Assert( i >= 0 && i < sz.لوړوالی );
        أرجع vv[i].ګام;
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ثابت std::vector<UMat>& vv = *(ثابت std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].ګام;
    }

    إذا( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        أرجع ((ثابت cuda::GpuMat*)obj)->ګام;
    }
    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
        ثابت std::vector<cuda::GpuMat>& vv = *(ثابت std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        أرجع vv[i].ګام;
    }
    إذا( k == CUDA_GPU_MATND )
    {
        ثابت cuda::GpuMatND& _gpuMatND = *(ثابت cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        أرجع _gpuMatND.ګام[i];
    }

    إذا (k == STD_ویکټور_CUDA_GPU_MAT_ND)
    {
        ثابت std::vector<cuda::GpuMatND>& vv = *(ثابت std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (size_t)i < vv.size());
        ثابت cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        أرجع _gpuMatND.ګام[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

فراغ ننوت_Array_2::copy_منزل(ثابت _OutputArray& arr) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == NONE )
        arr.release();
    وإلا إذا( k == MAT || k == MATX || k == STD_ویکټور || k == STD_BOOL_ویکټور )
    {
        Mat m = getMat();
        m.copy_منزل(arr);
    }
    وإلا إذا( k == UMAT )
        ((UMat*)obj)->copy_منزل(arr);
#که_تعریف_شوی HAVE_CUDA
    وإلا إذا (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_منزل(arr);
#د_که_پای
    وإلا
        CV_Error(Error::StsNotImplemented, "");
}

فراغ ننوت_Array_2::copy_منزل(ثابت _OutputArray& arr, ثابت ننوت_Array_2 & ماسک) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == NONE )
        arr.release();
    وإلا إذا( k == MAT || k == MATX || k == STD_ویکټور || k == STD_BOOL_ویکټور )
    {
        Mat m = getMat();
        m.copy_منزل(arr, ماسک);
    }
    وإلا إذا( k == UMAT )
        ((UMat*)obj)->copy_منزل(arr, ماسک);
#که_تعریف_شوی HAVE_CUDA
    وإلا إذا (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copy_منزل(arr, ماسک);
#د_که_پای
    وإلا
        CV_Error(Error::StsNotImplemented, "");
}

منطقي _OutputArray::fixed_اندازه() ثابت
{
    أرجع (flags & FIXED_اندازه) == FIXED_اندازه;
}

منطقي _OutputArray::fixedType() ثابت
{
    أرجع (flags & FIXED_TYPE) == FIXED_TYPE;
}

فراغ _OutputArray::create_(اندازه _sz, صحيح mtype, صحيح i, منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا( k == MAT && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((Mat*)obj)->size.چلوونکی()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        أرجع;
    }
    إذا( k == UMAT && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((UMat*)obj)->size.چلوونکی()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        أرجع;
    }
    إذا( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    إذا( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.لوړوالی) && (((cuda::GpuMatND*)obj)->size[1] == _sz.پلنوالی)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_CUDA
        cuda::GpuMatND::اندازه_Array sizes({_sz.لوړوالی, _sz.پلنوالی});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    إذا( k == OPENGL_بفر && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((ogl::بفر_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::بفر_2*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_OPENGL
        ((ogl::بفر_2*)obj)->create(_sz, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#د_که_پای
    }
    إذا( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    صحيح sizes[] = {_sz.لوړوالی, _sz.پلنوالی};
    create(2, sizes, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
}

فراغ _OutputArray::create_(صحيح لیکې_3, صحيح ستون_شمېر_3, صحيح mtype, صحيح i, منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا( k == MAT && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((Mat*)obj)->size.چلوونکی()() == اندازه(ستون_شمېر_3, لیکې_3));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(لیکې_3, ستون_شمېر_3, mtype);
        أرجع;
    }
    إذا( k == UMAT && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((UMat*)obj)->size.چلوونکی()() == اندازه(ستون_شمېر_3, لیکې_3));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(لیکې_3, ستون_شمېر_3, mtype);
        أرجع;
    }
    إذا( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((cuda::GpuMat*)obj)->size() == اندازه(ستون_شمېر_3, لیکې_3));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(لیکې_3, ستون_شمېر_3, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    إذا( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == لیکې_3) && (((cuda::GpuMatND*)obj)->size[1] == ستون_شمېر_3)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_CUDA
        cuda::GpuMatND::اندازه_Array sizes({لیکې_3, ستون_شمېر_3});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    إذا( k == OPENGL_بفر && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((ogl::بفر_2*)obj)->size() == اندازه(ستون_شمېر_3, لیکې_3));
        CV_Assert(!fixedType() || ((ogl::بفر_2*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_OPENGL
        ((ogl::بفر_2*)obj)->create(لیکې_3, ستون_شمېر_3, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#د_که_پای
    }
    إذا( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
        CV_Assert(!fixed_اندازه() || ((cuda::HostMem*)obj)->size() == اندازه(ستون_شمېر_3, لیکې_3));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#که_تعریف_شوی HAVE_CUDA
        ((cuda::HostMem*)obj)->create(لیکې_3, ستون_شمېر_3, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    صحيح sizes[] = {لیکې_3, ستون_شمېر_3};
    create(2, sizes, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
}

فراغ _OutputArray::create(صحيح d, ثابت صحيح* sizes, صحيح mtype, صحيح i,
                          منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    صحيح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ننوت_Array_2::ډول_Flag k = ډول();
    mtype = CV_MAT_TYPE(mtype);

    إذا( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_اندازه()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.لیکې == size0 && m.ستون_شمېر == size1) ||
            (allowTransposed && m.لیکې == size1 && m.ستون_شمېر == size0 && m.isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_ژوروالی(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m.type();
            وإلا
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_اندازه())
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
        CV_Assert(!(m.empty() && fixedType() && fixed_اندازه()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.لیکې == size0 && m.ستون_شمېر == size1) ||
            (allowTransposed && m.لیکې == size1 && m.ستون_شمېر == size0 && m.isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m.type();
            وإلا
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_اندازه())
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
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixed_ژوروالی_ماسک) != 0) );
        CV_CheckLE(d, 2, "");
        اندازه requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        إذا (sz.پلنوالی == 1 || sz.لوړوالی == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            صحيح ټول_1d = std::max(sz.پلنوالی, sz.لوړوالی);
            CV_Check(requested_size, std::max(requested_size.پلنوالی, requested_size.لوړوالی) == ټول_1d, "");
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
                        (requested_size == sz || (requested_size.لوړوالی == sz.پلنوالی && requested_size.پلنوالی == sz.لوړوالی)),
                        "");
            }
        }
        أرجع;
    }

    إذا( k == STD_ویکټور || k == STD_ویکټور_ویکټور )
    {
        CV_Assert(!fixed_اندازه());
        CV_Assert(k == STD_ویکټور_ویکټور || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        size_t len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        صحيح esz = CV_ELEM_اندازه(flags);

        إذا( k == STD_ویکټور || (k == STD_ویکټور_ویکټور && i >= 0) )
        {
            صحيح type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixed_ژوروالی_ماسک) != 0) );
        }

    #تعریف_لغوه RESIZE_VEC
    #تعریف_کړه RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            إذا (k == STD_ویکټور_ویکټور) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                إذا (i < 0) { \
                    CV_Assert(!fixed_اندازه() || len == vv->size()); \
                    vv->resize(len); \
                    أرجع; \
                } \
                CV_Assert(size_t(i) < vv->size()); \
                v = &vv->at(i); \
            } وإلا { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_اندازه() || len == v->size()); \
            v->resize(len); \
        } \
        اكسر

        STD_ویکټور_SWITCH(esz, RESIZE_VEC)
        أرجع;
    }

    إذا( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    إذا( k == STD_ویکټور_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        إذا( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_اندازه() || len == len0);
            v.resize(len);
            إذا( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    إذا( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ماسک) | _type;
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
                CV_Assert(!fixedType() && !fixed_اندازه());
                m.release();
            }

            إذا( d == 2 && m.dims == 2 && m.معلومات &&
                m.type() == mtype && m.لیکې == sizes[1] && m.ستون_شمېر == sizes[0] )
                أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m.type();
            وإلا
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        إذا(fixed_اندازه())
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
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.لوړوالی;

            CV_Assert(len == len0);
            إذا( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    إذا( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ماسک) | _type;
                }
            }
            أرجع;
        }

        CV_Assert( i < sz.لوړوالی );
        Mat& m = v[i];

        إذا( allowTransposed )
        {
            إذا( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_اندازه());
                m.release();
            }

            إذا( d == 2 && m.dims == 2 && m.معلومات &&
                m.type() == mtype && m.لیکې == sizes[1] && m.ستون_شمېر == sizes[0] )
                أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m.type();
            وإلا
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        إذا(fixed_اندازه())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        إذا( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_اندازه() || len == len0);
            v.resize(len);
            إذا( fixedType() )
            {
                صحيح _type = CV_MAT_TYPE(flags);
                لكل( size_t j = len0; j < len; j++ )
                {
                    إذا( v[j].type() == _type )
                        استمر;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_ماسک) | _type;
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
                CV_Assert(!fixedType() && !fixed_اندازه());
                m.release();
            }

            إذا( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.لیکې == sizes[1] && m.ستون_شمېر == sizes[0] )
                أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m.type();
            وإلا
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        إذا(fixed_اندازه())
        {
            CV_Assert(m.dims == d);
            لكل(صحيح j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        أرجع;
    }

    إذا ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
        أرجع;
    }

    إذا( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixed_ژوروالی_ماسک == 0 )
    {
#که_تعریف_شوی HAVE_CUDA
        cuda::GpuMatND::اندازه_Array اندازه_Array_2 = cuda::GpuMatND::اندازه_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(اندازه_Array_2, mtype);
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _OutputArray::create(ثابت MatShape& shape, صحيح mtype, صحيح i,
                          منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    إذا (shape.dims < 0) {
        release();
    } وإلا {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
    }
}

Mat _OutputArray::reinterpret(صحيح mtype) ثابت
{
    mtype = CV_MAT_TYPE(mtype);
    أرجع getMat().reinterpret(mtype);
}

فراغ _OutputArray::create_Same_اندازه(ثابت ننوت_Array_2& arr, صحيح mtype) ثابت
{
    create(arr.shape(), mtype);
}

فراغ _OutputArray::fit(اندازه _sz, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    صحيح sizes[] = {_sz.لوړوالی, _sz.پلنوالی};
    fit(2, sizes, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
}

فراغ _OutputArray::fit(صحيح لیکې, صحيح ستون_شمېر, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    صحيح sizes[] = {لیکې, ستون_شمېر};
    fit(2, sizes, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
}

فراغ _OutputArray::fit(ثابت MatShape& shape, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    إذا (shape.dims < 0) {
        release();
        أرجع;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا ((k == MAT && i < 0) || (k == STD_ویکټور_MAT && i >= 0)) {
        Mat_اندازه& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } وإلا إذا ((k == UMAT && i < 0) || (k == STD_ویکټور_UMAT && i >= 0)) {
        Mat_اندازه& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

فراغ _OutputArray::fit_Same_اندازه(ثابت ننوت_Array_2& arr, صحيح mtype) ثابت
{
    fit(arr.shape(), mtype);
}

فراغ _OutputArray::fit(صحيح d, ثابت صحيح* sizes, صحيح mtype, صحيح i,
                       منطقي allowTransposed, _OutputArray::ژوروالی_ماسک fixed_ژوروالی_ماسک) ثابت
{
    صحيح size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    ننوت_Array_2::ډول_Flag k = ډول();
    mtype = CV_MAT_TYPE(mtype);

    إذا( (k == MAT && i < 0) || (k == STD_ویکټور_MAT && i >= 0) )
    {
        Mat* m;
        إذا (k == MAT)
            m = (Mat*)obj;
        وإلا {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_اندازه()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->لیکې == size0 && m->ستون_شمېر == size1) ||
             (allowTransposed && m->لیکې == size1 && m->ستون_شمېر == size0 && m->isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_ژوروالی(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m->type();
            وإلا
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_اندازه())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        أرجع;
    }

    إذا( (k == UMAT && i < 0) || (k == STD_ویکټور_UMAT && i >= 0) )
    {
        UMat* m;
        إذا (k == UMAT)
            m = (UMat*)obj;
        وإلا {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((size_t)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_اندازه()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        إذا (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->لیکې == size0 && m->ستون_شمېر == size1) ||
             (allowTransposed && m->لیکې == size1 && m->ستون_شمېر == size0 && m->isContinuous())))
        {
            أرجع;
        }

        إذا(fixedType())
        {
            إذا(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_ژوروالی(flags)) & fixed_ژوروالی_ماسک) != 0 )
                mtype = m->type();
            وإلا
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        إذا(fixed_اندازه())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            لكل(صحيح j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        أرجع;
    }

    create(d, sizes, mtype, i, allowTransposed, fixed_ژوروالی_ماسک);
}

فراغ _OutputArray::release() ثابت
{
    CV_Assert(!fixed_اندازه());

    ننوت_Array_2::ډول_Flag k = ډول();

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
#که_تعریف_شوی HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }

    إذا( k == CUDA_GPU_MATND )
    {
#که_تعریف_شوی HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }

    إذا( k == CUDA_HOST_MEM )
    {
#که_تعریف_شوی HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }

    إذا( k == OPENGL_بفر )
    {
#که_تعریف_شوی HAVE_OPENGL
        ((ogl::بفر_2*)obj)->release();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#د_که_پای
    }

    إذا( k == NONE )
        أرجع;

    إذا( k == STD_ویکټور || k == STD_ویکټور_ویکټور )
    {
    #تعریف_لغوه CLEAR_VEC
    #تعریف_کړه CLEAR_VEC(T) \
        إذا (k == STD_ویکټور_ویکټور) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } وإلا { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        اكسر

        صحيح esz = CV_ELEM_اندازه(flags);
        STD_ویکټور_SWITCH(esz, CLEAR_VEC);
        أرجع;
    }

    إذا( k == STD_ویکټور_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        أرجع;
    }

    إذا( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        لكل( صحيح i = 0; i < sz.لوړوالی; i++ )
            v[i].release();
        أرجع;
    }

    إذا( k == STD_BOOL_ویکټور )
    {
        ((std::vector<منطقي>*)obj)->clear();
        أرجع;
    }

    إذا( k == STD_ویکټور_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        أرجع;
    }
    إذا (k == STD_ویکټور_CUDA_GPU_MAT)
    {
#که_تعریف_شوی HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        أرجع;
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

فراغ _OutputArray::clear() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == MAT )
    {
        CV_Assert(!fixed_اندازه());
        ((Mat*)obj)->resize(0);
        أرجع;
    }

    release();
}

منطقي _OutputArray::needed() ثابت
{
    أرجع ډول() != NONE;
}

Mat& _OutputArray::getMatRef(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا( i < 0 )
    {
        CV_Assert( k == MAT );
        أرجع *(Mat*)obj;
    }

    CV_Assert( k == STD_ویکټور_MAT || k == STD_ARRAY_MAT );

    إذا( k == STD_ویکټور_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (صحيح)v.size() );
        أرجع v[i];
    }
    وإلا
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.لوړوالی );
        أرجع v[i];
    }
}

UMat& _OutputArray::getUMatRef(صحيح i) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا( i < 0 )
    {
        CV_Assert( k == UMAT );
        أرجع *(UMat*)obj;
    }
    وإلا
    {
        CV_Assert( k == STD_ویکټور_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (صحيح)v.size() );
        أرجع v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    CV_Assert( k == CUDA_GPU_MAT );
    أرجع *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    CV_Assert(k == STD_ویکټور_CUDA_GPU_MAT);
    أرجع *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    CV_Assert( k == CUDA_GPU_MATND );
    أرجع *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    CV_Assert(k == STD_ویکټور_CUDA_GPU_MAT_ND);
    أرجع *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::بفر_2& _OutputArray::get_OGl_بفر_Ref() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    CV_Assert( k == OPENGL_بفر );
    أرجع *(ogl::بفر_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
    CV_Assert( k == CUDA_HOST_MEM );
    أرجع *(cuda::HostMem*)obj;
}

فراغ _OutputArray::سېټ_منزل(ثابت ننوت_Array_2& arr, ثابت ننوت_Array_2 & ماسک) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == NONE )
        ;
    وإلا إذا (k == MAT || k == MATX || k == STD_ویکټور)
    {
        Mat m = getMat();
        m.سېټ_منزل(arr, ماسک);
    }
    وإلا إذا( k == UMAT )
        ((UMat*)obj)->سېټ_منزل(arr, ماسک);
    وإلا إذا( k == CUDA_GPU_MAT )
    {
#که_تعریف_شوی HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.ډول(), ننوت_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->سېټ_منزل(Scalar(Vec<مزدوج, 4>(value.ptr<مزدوج>())), ماسک);
#وإلا
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#د_که_پای
    }
    وإلا
        CV_Error(Error::StsNotImplemented, "");
}

فراغ _OutputArray::سېټ_Zero() ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();

    إذا( k == NONE )
        ;
    وإلا إذا (k == MAT || k == MATX || k == STD_ویکټور)
    {
        Mat m = getMat();
        m.سېټ_Zero();
    }
    وإلا
    {
        سېټ_منزل(Scalar::all(0), noArray());
    }
}

فراغ _OutputArray::assign(ثابت UMat& u) ثابت
{
    ننوت_Array_2::ډول_Flag k = ډول();
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
    ننوت_Array_2::ډول_Flag k = ډول();
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
    إذا (fixed_اندازه())
    {
        // TODO Performance warning
        assign(u);
        أرجع;
    }
    صحيح k = ډول();
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
    إذا (fixed_اندازه())
    {
        // TODO Performance warning
        assign(m);
        أرجع;
    }
    صحيح k = ډول();
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
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (k == STD_ویکټور_UMAT)
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
    وإلا إذا (k == STD_ویکټور_MAT)
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
    ننوت_Array_2::ډول_Flag k = ډول();
    إذا (k == STD_ویکټور_UMAT)
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
    وإلا إذا (k == STD_ویکټور_MAT)
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


ساكن ننوت_Output_Array_2 _none;
ننوت_Output_Array noArray() { أرجع _none; }

} // cv::
