// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"
#समावेश "opencv2/core/mat.hpp"

namespace cv {

प्रकार Vec<पूर्णांक, 5> Vec5i;
प्रकार Vec<पूर्णांक, 7> Vec7i;
प्रकार Vec<पूर्णांक, 9> Vec9i;
प्रकार Vec<पूर्णांक, 10> Vec10i;
प्रकार Vec<पूर्णांक, 11> Vec11i;
प्रकार Vec<पूर्णांक, 12> Vec12i;
प्रकार Vec<पूर्णांक, 13> Vec13i;
प्रकार Vec<पूर्णांक, 14> Vec14i;
प्रकार Vec<पूर्णांक, 15> Vec15i;
प्रकार Vec<पूर्णांक, 16> Vec16i;
प्रकार Vec<पूर्णांक, 32> Vec32i;
प्रकार Vec<पूर्णांक, 64> Vec64i;
प्रकार Vec<पूर्णांक, 128> Vec128i;

#अघोषित STD_VECTOR_SWITCH
#घोषणा STD_VECTOR_SWITCH(esz, func) \
    चयन( esz ) \
    { \
    हाल 1: func(uchar); \
    हाल 2: func(Vec2b); \
    हाल 3: func(Vec3b); \
    हाल 4: func(पूर्णांक); \
    हाल 6: func(Vec3s); \
    हाल 8: func(Vec2i); \
    हाल 12: func(Vec3i); \
    हाल 16: func(Vec4i); \
    हाल 20: func(Vec5i); \
    हाल 24: func(Vec6i); \
    हाल 28: func(Vec7i); \
    हाल 32: func(Vec8i); \
    हाल 36: func(Vec9i); \
    हाल 40: func(Vec10i); \
    हाल 44: func(Vec11i); \
    हाल 48: func(Vec12i); \
    हाल 52: func(Vec13i); \
    हाल 56: func(Vec14i); \
    हाल 60: func(Vec15i); \
    हाल 64: func(Vec16i); \
    हाल 128: func(Vec32i); \
    हाल 256: func(Vec64i); \
    हाल 512: func(Vec128i); \
    शेष: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat निवेश_Array_2::getMat_(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    अगर( k == MAT )
    {
        स्थिर Mat* m = (स्थिर Mat*)obj;
        अगर( i < 0 )
            वापस *m;
        वापस m->row(i);
    }

    अगर( k == UMAT )
    {
        स्थिर UMat* m = (स्थिर UMat*)obj;
        अगर( i < 0 )
            वापस m->getMat(accessFlags);
        वापस m->getMat(accessFlags).row(i);
    }

    अगर (k == MATX)
    {
        CV_Assert( i < 0 );
        वापस Mat(sz, flags, obj);
    }

    अगर( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        पूर्णांक t = CV_8U;
        स्थिर std::vector<bool>& v = *(स्थिर std::vector<bool>*)obj;
        पूर्णांक j, n = (पूर्णांक)v.size();
        अगर( n == 0 )
            वापस Mat();
        Mat m(1, &n, t);
        uchar* dst = m.तथ्य;
        क्रम( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        वापस m;
    }

    अगर( k == NONE )
        वापस Mat();

    अगर( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        पूर्णांक t = CV_MAT_TYPE(flags);
        पूर्णांक esz = CV_ELEM_आकार(t);

    #अघोषित GET_MAT
    #घोषणा GET_MAT(T) \
        { \
            std::vector<T>* v; \
            अगर (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(माप_प्रकार(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            पूर्णांक v_sz = पूर्णांक(v->size()); \
            वापस v->empty() ? Mat() : Mat(1, &v_sz, t, v->तथ्य()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& v = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णांक)v.size() );

        वापस v[i];
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* v = (स्थिर Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        वापस v[i];
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& v = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णांक)v.size() );

        वापस v[i].getMat(accessFlags);
    }

    अगर( k == OPENGL_बफर )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    अगर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    अगर( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    अगर( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        स्थिर cuda::HostMem* cuda_mem = (स्थिर cuda::HostMem*)obj;

        वापस cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat निवेश_Array_2::getUMat(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    अगर( k == UMAT )
    {
        स्थिर UMat* m = (स्थिर UMat*)obj;
        अगर( i < 0 )
            वापस *m;
        वापस m->row(i);
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& v = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णांक)v.size() );

        वापस v[i];
    }

    अगर( k == MAT )
    {
        स्थिर Mat* m = (स्थिर Mat*)obj;
        अगर( i < 0 )
            वापस m->getUMat(accessFlags);
        वापस m->row(i).getUMat(accessFlags);
    }

    वापस getMat(i).getUMat(accessFlags);
}

व्योम निवेश_Array_2::getMatVector(std::vector<Mat>& mv) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    अगर( k == MAT )
    {
        स्थिर Mat& m = *(स्थिर Mat*)obj;
        पूर्णांक n = (पूर्णांक)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        क्रम( पूर्णांक i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (व्योम*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (व्योम*)m.ptr(i), &m.step[1]);
        वापस;
    }

    अगर (k == MATX)
    {
        माप_प्रकार n = sz.height, esz = CV_ELEM_आकार(flags);
        mv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        वापस;
    }

    अगर( k == STD_VECTOR )
    {
        स्थिर std::vector<uchar>& v = *(स्थिर std::vector<uchar>*)obj;

        माप_प्रकार n = योग(), esz = CV_ELEM_आकार(flags);
        पूर्णांक t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (व्योम*)(v.तथ्य() + esz*i));
        वापस;
    }

    अगर( k == NONE )
    {
        mv.सफा();
        वापस;
    }

    अगर( k == STD_VECTOR_VECTOR )
    {
        पूर्णांक typ = CV_MAT_TYPE(flags);
        पूर्णांक esz = CV_ELEM_आकार(typ);

        #अघोषित GET_VEC_VEC_MAT_VEC
        #घोषणा GET_VEC_VEC_MAT_VEC(T) \
        { \
            स्थिर std::vector<std::vector<T > >* vv = (स्थिर std::vector<std::vector<T > >*)obj; \
            माप_प्रकार n = vv->size(); \
            mv.resize(n); \
            क्रम (माप_प्रकार i = 0; i < n; i++) { \
                स्थिर std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (माप_प्रकार)पूर्णांक_उच्च); \
                पूर्णांक ni = (पूर्णांक)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (व्योम*)vi.तथ्य()) : Mat(); \
            } \
        } \
        अवरोध

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        वापस;
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& v = *(स्थिर std::vector<Mat>*)obj;
        माप_प्रकार n = v.size();
        mv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i];
        वापस;
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* v = (स्थिर Mat*)obj;
        माप_प्रकार n = sz.height;
        mv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i];
        वापस;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& v = *(स्थिर std::vector<UMat>*)obj;
        माप_प्रकार n = v.size();
        mv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        वापस;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

व्योम निवेश_Array_2::getUMatVector(std::vector<UMat>& umv) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    अगर( k == NONE )
    {
        umv.सफा();
        वापस;
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& v = *(स्थिर std::vector<Mat>*)obj;
        माप_प्रकार n = v.size();
        umv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        वापस;
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* v = (स्थिर Mat*)obj;
        माप_प्रकार n = sz.height;
        umv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        वापस;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& v = *(स्थिर std::vector<UMat>*)obj;
        माप_प्रकार n = v.size();
        umv.resize(n);

        क्रम( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i];
        वापस;
    }

    अगर( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        वापस;
    }
    अगर( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        वापस;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat निवेश_Array_2::getGpuMat() स्थिर
{
#अगर_घोषित HAVE_CUDA
    निवेश_Array_2::KindFlag k = kind();

    अगर (k == CUDA_GPU_MAT)
    {
        स्थिर cuda::GpuMat* d_mat = (स्थिर cuda::GpuMat*)obj;
        वापस *d_mat;
    }

    अगर (k == CUDA_HOST_MEM)
    {
        स्थिर cuda::HostMem* cuda_mem = (स्थिर cuda::HostMem*)obj;
        वापस cuda_mem->createGpuMatHeader();
    }

    अगर (k == OPENGL_बफर)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    अगर (k == NONE)
        वापस cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
}
व्योम निवेश_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) स्थिर
{
#अगर_घोषित HAVE_CUDA
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#अन्यथा
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
}
व्योम निवेश_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) स्थिर
{
#अगर_घोषित HAVE_CUDA
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#अन्यथा
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
}
cuda::GpuMatND निवेश_Array_2::getGpuMatND() स्थिर
{
#अगर_घोषित HAVE_CUDA
    निवेश_Array_2::KindFlag k = kind();

    अगर (k == CUDA_GPU_MATND)
    {
        स्थिर cuda::GpuMatND* d_mat = (स्थिर cuda::GpuMatND*)obj;
        वापस *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
}
ogl::बफर_2 निवेश_Array_2::get_OGl_बफर() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_बफर);

    स्थिर ogl::बफर_2* gl_buf = (स्थिर ogl::बफर_2*)obj;
    वापस *gl_buf;
}

पूर्णांक निवेश_Array_2::rows_(पूर्णांक i) स्थिर
{
#अगर_घोषित HAVE_CUDA
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == CUDA_GPU_MATND)
    {
        स्थिर cuda::GpuMatND& _gpuMatND = *(स्थिर cuda::GpuMatND*)obj;
        वापस (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#पूर्ण_अगर

    वापस आकार_3(i).height;
}

पूर्णांक निवेश_Array_2::cols_(पूर्णांक i) स्थिर
{
#अगर_घोषित HAVE_CUDA
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == CUDA_GPU_MATND)
    {
        स्थिर cuda::GpuMatND& _gpuMatND = *(स्थिर cuda::GpuMatND*)obj;
        वापस (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#पूर्ण_अगर

    वापस आकार_3(i).width;
}

आकार निवेश_Array_2::आकार_3(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिर Mat* m = (स्थिर Mat*)obj;
        CV_Assert(m->dims <= 2);
        वापस आकार(m->cols, m->rows);
    }

    अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        स्थिर UMat* m = (स्थिर UMat*)obj;
        CV_Assert(m->dims <= 2);
        वापस आकार(m->cols, m->rows);
    }

    अगर (k == MATX)
    {
        CV_Assert( i < 0 );
        वापस sz;
    }

    अगर( k == NONE )
        वापस आकार();

    अगर( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        माप_प्रकार n = योग(i);
        CV_Assert(n <= (माप_प्रकार)पूर्णांक_उच्च);
        वापस आकार((पूर्णांक)n, 1);
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        अगर( i < 0 )
            वापस vv.empty() ? आकार() : आकार((पूर्णांक)vv.size(), 1);
        CV_Assert( i < (पूर्णांक)vv.size() );

        वापस vv[i].size();
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        अगर( i < 0 )
            वापस sz.height==0 ? आकार() : आकार(sz.height, 1);
        CV_Assert( i < sz.height );

        वापस vv[i].size();
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#अगर_घोषित HAVE_CUDA
        स्थिर std::vector<cuda::GpuMat>& vv = *(स्थिर std::vector<cuda::GpuMat>*)obj;
        अगर (i < 0)
            वापस vv.empty() ? आकार() : आकार((पूर्णांक)vv.size(), 1);
        CV_Assert(i < (पूर्णांक)vv.size());
        वापस vv[i].size();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#अगर_घोषित HAVE_CUDA
        स्थिर std::vector<cuda::GpuMatND>& vv = *(स्थिर std::vector<cuda::GpuMatND>*)obj;
        अगर (i < 0)
            वापस vv.empty() ? आकार() : आकार((पूर्णांक)vv.size(), 1);
        CV_Assert(i < (पूर्णांक)vv.size());
        स्थिर cuda::GpuMatND& m = vv[i];
        अगर (m.dims == 0)
            वापस आकार();
        अगर (m.dims == 1)
            वापस आकार(1, m.size[0]);
        वापस आकार(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        अगर( i < 0 )
            वापस vv.empty() ? आकार() : आकार((पूर्णांक)vv.size(), 1);
        CV_Assert( i < (पूर्णांक)vv.size() );

        वापस vv[i].size();
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#अगर_घोषित HAVE_CUDA
        स्थिर std::vector<cuda::GpuMatND>& vv = *(स्थिर std::vector<cuda::GpuMatND>*)obj;
        अगर (i < 0)
            वापस vv.empty() ? आकार() : आकार((पूर्णांक)vv.size(), 1);
        CV_Assert(i < (पूर्णांक)vv.size());
        स्थिर cuda::GpuMatND& m = vv[i];
        अगर (m.dims == 0)
            वापस आकार();
        अगर (m.dims == 1)
            वापस आकार(1, m.size[0]);
        वापस आकार(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर( k == OPENGL_बफर )
    {
        CV_Assert( i < 0 );
        स्थिर ogl::बफर_2* buf = (स्थिर ogl::बफर_2*)obj;
        वापस buf->size();
    }

    अगर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        स्थिर cuda::GpuMat* d_mat = (स्थिर cuda::GpuMat*)obj;
        वापस d_mat->size();
    }

    अगर( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        स्थिर cuda::HostMem* cuda_mem = (स्थिर cuda::HostMem*)obj;
        वापस cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णांक निवेश_Array_2::sizend(पूर्णांक* arrsz, पूर्णांक i) स्थिर
{
    पूर्णांक j, d = 0;
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == NONE )
        ;
    अन्यथा अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिर Mat& m = *(स्थिर Mat*)obj;
        d = m.dims;
        अगर(arrsz)
            क्रम(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        स्थिर UMat& m = *(स्थिर UMat*)obj;
        d = m.dims;
        अगर(arrsz)
            क्रम(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा अगर( k == STD_VECTOR_MAT && i >= 0 )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णांक)vv.size() );
        स्थिर Mat& m = vv[i];
        d = m.dims;
        अगर(arrsz)
            क्रम(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा अगर( k == STD_ARRAY_MAT && i >= 0 )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        CV_Assert( i < sz.height );
        स्थिर Mat& m = vv[i];
        d = m.dims;
        अगर(arrsz)
            क्रम(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा अगर( k == STD_VECTOR_UMAT && i >= 0 )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णांक)vv.size() );
        स्थिर UMat& m = vv[i];
        d = m.dims;
        अगर(arrsz)
            क्रम(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा अगर (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        माप_प्रकार n = योग(i);
        d = 1;
        अगर(arrsz)
        {
            arrsz[0] = पूर्णांक(n);
        }
    }
    अन्यथा
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        आकार sz2d = size(i);
        d = 2;
        अगर(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    वापस d;
}

bool निवेश_Array_2::empty(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर (i >= 0) {
        अगर (k == STD_VECTOR_MAT) {
            स्वतः mv = reinterpret_cast<स्थिर std::vector<Mat>*>(obj);
            CV_Assert((माप_प्रकार)i < mv->size());
            वापस mv->at(i).empty();
        }
        अन्यथा अगर (k == STD_VECTOR_UMAT) {
            स्वतः umv = reinterpret_cast<स्थिर std::vector<UMat>*>(obj);
            CV_Assert((माप_प्रकार)i < umv->size());
            वापस umv->at(i).empty();
        }
        अन्यथा अगर (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            माप_प्रकार n = योग(i);
            वापस n == 0;
        } अन्यथा {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    वापस empty();
}

MatShape निवेश_Array_2::shape(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    MatShape shape;

    अगर( k == NONE )
        ;
    अन्यथा अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिर Mat& m = *(स्थिर Mat*)obj;
        shape = m.size;
    }
    अन्यथा अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        स्थिर UMat& m = *(स्थिर UMat*)obj;
        shape = m.size;
    }
    अन्यथा अगर( k == STD_VECTOR_MAT && i >= 0 )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert( (माप_प्रकार)i < vv.size() );
        स्थिर Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा अगर( k == STD_ARRAY_MAT && i >= 0 )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        CV_Assert( i < sz.height );
        स्थिर Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा अगर( k == STD_VECTOR_UMAT && i >= 0 )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert( (माप_प्रकार)i < vv.size() );
        स्थिर UMat& m = vv[i];
        shape = m.size;
    }
    अन्यथा
    {
        पूर्णांक sizes[CV_MAX_DIM];
        पूर्णांक dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    वापस shape;
}

bool निवेश_Array_2::same_आकार(स्थिर निवेश_Array_2& arr) स्थिर
{
    निवेश_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    आकार sz1;

    अगर( k1 == MAT )
    {
        स्थिर Mat* m = ((स्थिर Mat*)obj);
        अगर( k2 == MAT )
            वापस m->size == ((स्थिर Mat*)arr.obj)->size;
        अगर( k2 == UMAT )
            वापस m->size == ((स्थिर UMat*)arr.obj)->size;
        अगर( m->dims > 2 )
            वापस false;
        sz1 = m->size();
    }
    अन्यथा अगर( k1 == UMAT )
    {
        स्थिर UMat* m = ((स्थिर UMat*)obj);
        अगर( k2 == MAT )
            वापस m->size == ((स्थिर Mat*)arr.obj)->size;
        अगर( k2 == UMAT )
            वापस m->size == ((स्थिर UMat*)arr.obj)->size;
        अगर( m->dims > 2 )
            वापस false;
        sz1 = m->size();
    }
    अन्यथा अगर ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        वापस ((स्थिर cuda::GpuMatND*)obj)->size == ((स्थिर cuda::GpuMatND*)arr.obj)->size;
    }
    अन्यथा
        sz1 = size();

    अगर( arr.dims() > 2 )
        वापस false;
    वापस sz1 == arr.size();
}

पूर्णांक निवेश_Array_2::dims_(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर Mat*)obj)->dims;
    }

    अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर UMat*)obj)->dims;
    }

    अगर (k == MATX)
    {
        CV_Assert( i < 0 );
        वापस 2;
    }

    अगर( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        वापस 1;
    }

    अगर( k == NONE )
        वापस 0;

    अगर( k == STD_VECTOR_VECTOR )
    {
        अगर( i < 0 )
            वापस 1;
        CV_Assert( माप_प्रकार(i) < योग() );
        वापस 1;
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        अगर( i < 0 )
            वापस 1;
        CV_Assert( i < (पूर्णांक)vv.size() );

        वापस vv[i].dims;
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        अगर( i < 0 )
            वापस 1;
        CV_Assert( i < sz.height );

        वापस vv[i].dims;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        अगर( i < 0 )
            वापस 1;
        CV_Assert( i < (पूर्णांक)vv.size() );

        वापस vv[i].dims;
    }

    अगर( k == OPENGL_बफर )
    {
        CV_Assert( i < 0 );
        वापस 2;
    }

    अगर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        वापस 2;
    }

    अगर( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर cuda::GpuMatND*)obj)->dims;
    }

    अगर( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        वापस 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

माप_प्रकार निवेश_Array_2::योग(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर Mat*)obj)->योग();
    }

    अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर UMat*)obj)->योग();
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        अगर( i < 0 )
            वापस vv.size();

        CV_Assert( i < (पूर्णांक)vv.size() );
        वापस vv[i].योग();
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        अगर( i < 0 )
            वापस sz.height;

        CV_Assert( i < sz.height );
        वापस vv[i].योग();
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        अगर( i < 0 )
            वापस vv.size();

        CV_Assert( i < (पूर्णांक)vv.size() );
        वापस vv[i].योग();
    }

    अगर( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        माप_प्रकार res = 0;
        स्थिर cuda::GpuMatND& _gpuMatND = *((स्थिर cuda::GpuMatND*)obj);
        अगर (_gpuMatND.dims > 0)
        {
            res = 1;
            क्रम(पूर्णांक d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            वापस res;
        }
    }

    अगर (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        पूर्णांक esz = CV_ELEM_आकार(flags);

        #अघोषित GET_VEC_आकार
        #घोषणा GET_VEC_आकार(T) \
            अगर (k == STD_VECTOR_VECTOR) { \
                स्थिर std::vector<std::vector<T > >* vv = (स्थिर std::vector<std::vector<T > >*)obj; \
                माप_प्रकार n = vv->size(); \
                अगर (i < 0) \
                    वापस n; \
                CV_Assert((माप_प्रकार)i < n); \
                वापस vv->at(i).size(); \
            } अन्यथा \
                वापस ((स्थिर std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_आकार)
    }

    वापस size(i).area();
}

पूर्णांक निवेश_Array_2::type_(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
        वापस ((स्थिर Mat*)obj)->type();

    अगर( k == UMAT )
        वापस ((स्थिर UMat*)obj)->type();

    अगर( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        वापस CV_MAT_TYPE(flags);

    अगर( k == NONE )
        वापस -1;

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        अगर( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            वापस CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णांक)vv.size() );
        वापस vv[i >= 0 ? i : 0].type();
    }

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        अगर( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            वापस CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णांक)vv.size() );
        वापस vv[i >= 0 ? i : 0].type();
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        अगर( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            वापस CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        वापस vv[i >= 0 ? i : 0].type();
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#अगर_घोषित HAVE_CUDA
        स्थिर std::vector<cuda::GpuMat>& vv = *(स्थिर std::vector<cuda::GpuMat>*)obj;
        अगर (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            वापस CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णांक)vv.size());
        वापस vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#अगर_घोषित HAVE_CUDA
        स्थिर std::vector<cuda::GpuMatND>& vv = *(स्थिर std::vector<cuda::GpuMatND>*)obj;
        अगर (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            वापस CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णांक)vv.size());
        वापस vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर( k == OPENGL_बफर )
        वापस ((स्थिर ogl::बफर_2*)obj)->type();

    अगर( k == CUDA_GPU_MAT )
        वापस ((स्थिर cuda::GpuMat*)obj)->type();

    अगर( k == CUDA_GPU_MATND )
        वापस ((स्थिर cuda::GpuMatND*)obj)->type();

    अगर( k == CUDA_HOST_MEM )
        वापस ((स्थिर cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णांक निवेश_Array_2::depth_(पूर्णांक i) स्थिर
{
    वापस CV_MAT_DEPTH(type(i));
}

bool निवेश_Array_2::empty_() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
        वापस ((स्थिर Mat*)obj)->empty();

    अगर( k == UMAT )
        वापस ((स्थिर UMat*)obj)->empty();

    अगर (k == MATX)
        वापस false;

    अगर( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        माप_प्रकार n = योग(-1);
        वापस n == 0;
    }

    अगर( k == NONE )
        वापस true;

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        वापस vv.empty();
    }

    अगर( k == STD_ARRAY_MAT )
    {
        वापस sz.height == 0;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        वापस vv.empty();
    }

    अगर( k == OPENGL_बफर )
        वापस ((स्थिर ogl::बफर_2*)obj)->empty();

    अगर( k == CUDA_GPU_MAT )
        वापस ((स्थिर cuda::GpuMat*)obj)->empty();

    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        स्थिर std::vector<cuda::GpuMat>& vv = *(स्थिर std::vector<cuda::GpuMat>*)obj;
        वापस vv.empty();
    }

    अगर( k == CUDA_GPU_MATND )
        वापस ((स्थिर cuda::GpuMatND*)obj)->empty();

    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        स्थिर std::vector<cuda::GpuMatND>& vv = *(स्थिर std::vector<cuda::GpuMatND>*)obj;
        वापस vv.empty();
    }

    अगर( k == CUDA_HOST_MEM )
        वापस ((स्थिर cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool निवेश_Array_2::isContinuous(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
        वापस i < 0 ? ((स्थिर Mat*)obj)->isContinuous() : true;

    अगर( k == UMAT )
        वापस i < 0 ? ((स्थिर UMat*)obj)->isContinuous() : true;

    अगर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        वापस true;

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].isContinuous();
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        वापस vv[i].isContinuous();
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].isContinuous();
    }

    अगर( k == CUDA_GPU_MAT )
      वापस i < 0 ? ((स्थिर cuda::GpuMat*)obj)->isContinuous() : true;

    अगर( k == CUDA_GPU_MATND )
        वापस i < 0 ? ((स्थिर cuda::GpuMatND*)obj)->isContinuous() : true;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

bool निवेश_Array_2::isSubmatrix(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
        वापस i < 0 ? ((स्थिर Mat*)obj)->isSubmatrix() : false;

    अगर( k == UMAT )
        वापस i < 0 ? ((स्थिर UMat*)obj)->isSubmatrix() : false;

    अगर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        वापस false;

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].isSubmatrix();
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        वापस vv[i].isSubmatrix();
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].isSubmatrix();
    }

    अगर( k == CUDA_GPU_MATND )
    {
        वापस ((स्थिर cuda::GpuMatND*)obj)->isSubmatrix();
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        स्थिर std::vector<cuda::GpuMatND>& vv = *(स्थिर std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

माप_प्रकार निवेश_Array_2::offset(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        स्थिर Mat * स्थिर m = ((स्थिर Mat*)obj);
        वापस (माप_प्रकार)(m->ptr() - m->datastart);
    }

    अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर UMat*)obj)->offset;
    }

    अगर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        वापस 0;

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णांक)vv.size() );

        वापस (माप_प्रकार)(vv[i].ptr() - vv[i].datastart);
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        वापस (माप_प्रकार)(vv[i].ptr() - vv[i].datastart);
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].offset;
    }

    अगर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        स्थिर cuda::GpuMat * स्थिर m = ((स्थिर cuda::GpuMat*)obj);
        वापस (माप_प्रकार)(m->तथ्य - m->datastart);
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        स्थिर std::vector<cuda::GpuMat>& vv = *(स्थिर std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस (माप_प्रकार)(vv[i].तथ्य - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

माप_प्रकार निवेश_Array_2::step(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर Mat*)obj)->step;
    }

    अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर UMat*)obj)->step;
    }

    अगर( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        वापस 0;

    अगर( k == STD_VECTOR_MAT )
    {
        स्थिर std::vector<Mat>& vv = *(स्थिर std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णांक)vv.size() );
        वापस vv[i].step;
    }

    अगर( k == STD_ARRAY_MAT )
    {
        स्थिर Mat* vv = (स्थिर Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        वापस vv[i].step;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        स्थिर std::vector<UMat>& vv = *(स्थिर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].step;
    }

    अगर( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        वापस ((स्थिर cuda::GpuMat*)obj)->step;
    }
    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        स्थिर std::vector<cuda::GpuMat>& vv = *(स्थिर std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        वापस vv[i].step;
    }
    अगर( k == CUDA_GPU_MATND )
    {
        स्थिर cuda::GpuMatND& _gpuMatND = *(स्थिर cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        वापस _gpuMatND.step[i];
    }

    अगर (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        स्थिर std::vector<cuda::GpuMatND>& vv = *(स्थिर std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        स्थिर cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        वापस _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

व्योम निवेश_Array_2::copyTo(स्थिर _OutputArray& arr) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == NONE )
        arr.release();
    अन्यथा अगर( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    अन्यथा अगर( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#अगर_घोषित HAVE_CUDA
    अन्यथा अगर (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#पूर्ण_अगर
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

व्योम निवेश_Array_2::copyTo(स्थिर _OutputArray& arr, स्थिर निवेश_Array_2 & mask) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == NONE )
        arr.release();
    अन्यथा अगर( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    अन्यथा अगर( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#अगर_घोषित HAVE_CUDA
    अन्यथा अगर (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#पूर्ण_अगर
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

bool _OutputArray::fixed_आकार() स्थिर
{
    वापस (flags & FIXED_आकार) == FIXED_आकार;
}

bool _OutputArray::fixedType() स्थिर
{
    वापस (flags & FIXED_TYPE) == FIXED_TYPE;
}

व्योम _OutputArray::create_(आकार _sz, पूर्णांक mtype, पूर्णांक i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((Mat*)obj)->size.चालक()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        वापस;
    }
    अगर( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((UMat*)obj)->size.चालक()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        वापस;
    }
    अगर( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#अगर_घोषित HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    अगर( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#अगर_घोषित HAVE_CUDA
        cuda::GpuMatND::आकार_Array sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    अगर( k == OPENGL_बफर && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((ogl::बफर_2*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::बफर_2*)obj)->type() == mtype);
#अगर_घोषित HAVE_OPENGL
        ((ogl::बफर_2*)obj)->create(_sz, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#पूर्ण_अगर
    }
    अगर( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#अगर_घोषित HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    पूर्णांक sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

व्योम _OutputArray::create_(पूर्णांक _rows, पूर्णांक _cols, पूर्णांक mtype, पूर्णांक i, bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((Mat*)obj)->size.चालक()() == आकार(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        वापस;
    }
    अगर( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((UMat*)obj)->size.चालक()() == आकार(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        वापस;
    }
    अगर( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::GpuMat*)obj)->size() == आकार(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#अगर_घोषित HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    अगर( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#अगर_घोषित HAVE_CUDA
        cuda::GpuMatND::आकार_Array sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    अगर( k == OPENGL_बफर && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((ogl::बफर_2*)obj)->size() == आकार(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::बफर_2*)obj)->type() == mtype);
#अगर_घोषित HAVE_OPENGL
        ((ogl::बफर_2*)obj)->create(_rows, _cols, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#पूर्ण_अगर
    }
    अगर( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixed_आकार() || ((cuda::HostMem*)obj)->size() == आकार(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#अगर_घोषित HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    पूर्णांक sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

व्योम _OutputArray::create(पूर्णांक d, स्थिर पूर्णांक* sizes, पूर्णांक mtype, पूर्णांक i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    पूर्णांक size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    निवेश_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    अगर( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        अगर (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        अगर(fixed_आकार())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        वापस;
    }

    अगर( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        अगर (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        अगर(fixed_आकार())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        वापस;
    }

    अगर( k == MATX )
    {
        CV_Assert( i < 0 );
        पूर्णांक type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        आकार requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        अगर (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            पूर्णांक योग_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == योग_1d, "");
        }
        अन्यथा
        {
            अगर (!allowTransposed)
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
        वापस;
    }

    अगर( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixed_आकार());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        माप_प्रकार len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        पूर्णांक esz = CV_ELEM_आकार(flags);

        अगर( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            पूर्णांक type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #अघोषित RESIZE_VEC
    #घोषणा RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            अगर (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                अगर (i < 0) { \
                    CV_Assert(!fixed_आकार() || len == vv->size()); \
                    vv->resize(len); \
                    वापस; \
                } \
                CV_Assert(माप_प्रकार(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixed_आकार() || len == v->size()); \
            v->resize(len); \
        } \
        अवरोध

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        वापस;
    }

    अगर( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    अगर( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        अगर( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_आकार() || len == len0);
            v.resize(len);
            अगर( fixedType() )
            {
                पूर्णांक _type = CV_MAT_TYPE(flags);
                क्रम( माप_प्रकार j = len0; j < len; j++ )
                {
                    अगर( v[j].type() == _type )
                        जारी;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            वापस;
        }

        CV_Assert( i < (पूर्णांक)v.size() );
        Mat& m = v[i];

        अगर( allowTransposed )
        {
            अगर( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_आकार());
                m.release();
            }

            अगर( d == 2 && m.dims == 2 && m.तथ्य &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        अगर(fixed_आकार())
        {
            CV_Assert(m.dims == d);
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        वापस;
    }

    अगर( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        अगर( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            अगर( fixedType() )
            {
                पूर्णांक _type = CV_MAT_TYPE(flags);
                क्रम( माप_प्रकार j = len0; j < len; j++ )
                {
                    अगर( v[j].type() == _type )
                        जारी;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            वापस;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        अगर( allowTransposed )
        {
            अगर( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_आकार());
                m.release();
            }

            अगर( d == 2 && m.dims == 2 && m.तथ्य &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        अगर(fixed_आकार())
        {
            CV_Assert(m.dims == d);
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        वापस;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        अगर( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixed_आकार() || len == len0);
            v.resize(len);
            अगर( fixedType() )
            {
                पूर्णांक _type = CV_MAT_TYPE(flags);
                क्रम( माप_प्रकार j = len0; j < len; j++ )
                {
                    अगर( v[j].type() == _type )
                        जारी;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            वापस;
        }

        CV_Assert( i < (पूर्णांक)v.size() );
        UMat& m = v[i];

        अगर( allowTransposed )
        {
            अगर( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixed_आकार());
                m.release();
            }

            अगर( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        अगर(fixed_आकार())
        {
            CV_Assert(m.dims == d);
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        वापस;
    }

    अगर ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        वापस;
    }

    अगर( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#अगर_घोषित HAVE_CUDA
        cuda::GpuMatND::आकार_Array आकार_Array_2 = cuda::GpuMatND::आकार_Array(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(आकार_Array_2, mtype);
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

व्योम _OutputArray::create(स्थिर MatShape& shape, पूर्णांक mtype, पूर्णांक i,
                          bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    अगर (shape.dims < 0) {
        release();
    } अन्यथा {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(पूर्णांक mtype) स्थिर
{
    mtype = CV_MAT_TYPE(mtype);
    वापस getMat().reinterpret(mtype);
}

व्योम _OutputArray::create_Same_आकार(स्थिर निवेश_Array_2& arr, पूर्णांक mtype) स्थिर
{
    create(arr.shape(), mtype);
}

व्योम _OutputArray::fit(आकार _sz, पूर्णांक mtype, पूर्णांक i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    पूर्णांक sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

व्योम _OutputArray::fit(पूर्णांक rows, पूर्णांक cols, पूर्णांक mtype, पूर्णांक i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    पूर्णांक sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

व्योम _OutputArray::fit(स्थिर MatShape& shape, पूर्णांक mtype, पूर्णांक i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    अगर (shape.dims < 0) {
        release();
        वापस;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    निवेश_Array_2::KindFlag k = kind();
    अगर ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        Mat_आकार& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } अन्यथा अगर ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        Mat_आकार& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

व्योम _OutputArray::fit_Same_आकार(स्थिर निवेश_Array_2& arr, पूर्णांक mtype) स्थिर
{
    fit(arr.shape(), mtype);
}

व्योम _OutputArray::fit(पूर्णांक d, स्थिर पूर्णांक* sizes, पूर्णांक mtype, पूर्णांक i,
                       bool allowTransposed, _OutputArray::DepthMask fixedDepthMask) स्थिर
{
    पूर्णांक size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    निवेश_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    अगर( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        अगर (k == MAT)
            m = (Mat*)obj;
        अन्यथा {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((माप_प्रकार)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        अगर (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        अगर(fixed_आकार())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        वापस;
    }

    अगर( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        अगर (k == UMAT)
            m = (UMat*)obj;
        अन्यथा {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((माप_प्रकार)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixed_आकार()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        अगर (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            वापस;
        }

        अगर(fixedType())
        {
            अगर(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        अगर(fixed_आकार())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            क्रम(पूर्णांक j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        वापस;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

व्योम _OutputArray::release() स्थिर
{
    CV_Assert(!fixed_आकार());

    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        ((Mat*)obj)->release();
        वापस;
    }

    अगर( k == UMAT )
    {
        ((UMat*)obj)->release();
        वापस;
    }

    अगर( k == CUDA_GPU_MAT )
    {
#अगर_घोषित HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर( k == CUDA_GPU_MATND )
    {
#अगर_घोषित HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर( k == CUDA_HOST_MEM )
    {
#अगर_घोषित HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }

    अगर( k == OPENGL_बफर )
    {
#अगर_घोषित HAVE_OPENGL
        ((ogl::बफर_2*)obj)->release();
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#पूर्ण_अगर
    }

    अगर( k == NONE )
        वापस;

    अगर( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #अघोषित CLEAR_VEC
    #घोषणा CLEAR_VEC(T) \
        अगर (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->सफा(); \
        } अन्यथा { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->सफा(); \
        } \
        अवरोध

        पूर्णांक esz = CV_ELEM_आकार(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        वापस;
    }

    अगर( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->सफा();
        वापस;
    }

    अगर( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        क्रम( पूर्णांक i = 0; i < sz.height; i++ )
            v[i].release();
        वापस;
    }

    अगर( k == STD_BOOL_VECTOR )
    {
        ((std::vector<bool>*)obj)->सफा();
        वापस;
    }

    अगर( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->सफा();
        वापस;
    }
    अगर (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#अगर_घोषित HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->सफा();
        वापस;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

व्योम _OutputArray::सफा() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == MAT )
    {
        CV_Assert(!fixed_आकार());
        ((Mat*)obj)->resize(0);
        वापस;
    }

    release();
}

bool _OutputArray::needed() स्थिर
{
    वापस kind() != NONE;
}

Mat& _OutputArray::getMatRef(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर( i < 0 )
    {
        CV_Assert( k == MAT );
        वापस *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    अगर( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णांक)v.size() );
        वापस v[i];
    }
    अन्यथा
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        वापस v[i];
    }
}

UMat& _OutputArray::getUMatRef(पूर्णांक i) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर( i < 0 )
    {
        CV_Assert( k == UMAT );
        वापस *(UMat*)obj;
    }
    अन्यथा
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णांक)v.size() );
        वापस v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    वापस *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    वापस *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    वापस *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    वापस *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::बफर_2& _OutputArray::get_OGl_बफर_Ref() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_बफर );
    वापस *(ogl::बफर_2*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    वापस *(cuda::HostMem*)obj;
}

व्योम _OutputArray::setTo(स्थिर निवेश_Array_2& arr, स्थिर निवेश_Array_2 & mask) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == NONE )
        ;
    अन्यथा अगर (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    अन्यथा अगर( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    अन्यथा अगर( k == CUDA_GPU_MAT )
    {
#अगर_घोषित HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), निवेश_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<द्विगुन, 4>(value.ptr<द्विगुन>())), mask);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#पूर्ण_अगर
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

व्योम _OutputArray::setZero() स्थिर
{
    निवेश_Array_2::KindFlag k = kind();

    अगर( k == NONE )
        ;
    अन्यथा अगर (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    अन्यथा
    {
        setTo(Scalar::all(0), noArray());
    }
}

व्योम _OutputArray::assign(स्थिर UMat& u) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    अन्यथा अगर (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    अन्यथा अगर (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


व्योम _OutputArray::assign(स्थिर Mat& m) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    अन्यथा अगर (k == MAT)
    {
        *(Mat*)obj = m;
    }
    अन्यथा अगर (k == MATX)
    {
        m.copyTo(getMat());
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


व्योम _OutputArray::move(UMat& u) स्थिर
{
    अगर (fixed_आकार())
    {
        // TODO Performance warning
        assign(u);
        वापस;
    }
    पूर्णांक k = kind();
    अगर (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    अन्यथा अगर (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    अन्यथा अगर (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


व्योम _OutputArray::move(Mat& m) स्थिर
{
    अगर (fixed_आकार())
    {
        // TODO Performance warning
        assign(m);
        वापस;
    }
    पूर्णांक k = kind();
    अगर (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    अन्यथा अगर (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    अन्यथा अगर (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


व्योम _OutputArray::assign(स्थिर std::vector<UMat>& v) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        क्रम (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिर UMat& m = v[i];
            UMat& this_m = this_v[i];
            अगर (this_m.u != शून्य && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा अगर (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        क्रम (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिर UMat& m = v[i];
            Mat& this_m = this_v[i];
            अगर (this_m.u != शून्य && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


व्योम _OutputArray::assign(स्थिर std::vector<Mat>& v) स्थिर
{
    निवेश_Array_2::KindFlag k = kind();
    अगर (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        क्रम (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिर Mat& m = v[i];
            UMat& this_m = this_v[i];
            अगर (this_m.u != शून्य && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा अगर (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        क्रम (माप_प्रकार i = 0; i < v.size(); i++)
        {
            स्थिर Mat& m = v[i];
            Mat& this_m = this_v[i];
            अगर (this_m.u != शून्य && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


अटल निवेश_Output_Array_2 _none;
निवेश_Output_Array noArray() { वापस _none; }

} // cv::
