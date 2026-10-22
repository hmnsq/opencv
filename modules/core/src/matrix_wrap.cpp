// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"
#अन्तर्भाव "opencv2/core/mat.hpp"

नामाकाश cv {

प्रकारनिर्देश Vec<पूर्णाङ्क, 5> Vec5i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 7> Vec7i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 9> Vec9i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 10> Vec10i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 11> Vec11i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 12> Vec12i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 13> Vec13i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 14> Vec14i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 15> Vec15i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 16> Vec16i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 32> Vec32i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 64> Vec64i;
प्रकारनिर्देश Vec<पूर्णाङ्क, 128> Vec128i;

#undef STD_VECTOR_SWITCH
#define STD_VECTOR_SWITCH(esz, func) \
    विकल्प( esz ) \
    { \
    पक्ष 1: func(uchar); \
    पक्ष 2: func(Vec2b); \
    पक्ष 3: func(Vec3b); \
    पक्ष 4: func(पूर्णाङ्क); \
    पक्ष 6: func(Vec3s); \
    पक्ष 8: func(Vec2i); \
    पक्ष 12: func(Vec3i); \
    पक्ष 16: func(Vec4i); \
    पक्ष 20: func(Vec5i); \
    पक्ष 24: func(Vec6i); \
    पक्ष 28: func(Vec7i); \
    पक्ष 32: func(Vec8i); \
    पक्ष 36: func(Vec9i); \
    पक्ष 40: func(Vec10i); \
    पक्ष 44: func(Vec11i); \
    पक्ष 48: func(Vec12i); \
    पक्ष 52: func(Vec13i); \
    पक्ष 56: func(Vec14i); \
    पक्ष 60: func(Vec15i); \
    पक्ष 64: func(Vec16i); \
    पक्ष 128: func(Vec32i); \
    पक्ष 256: func(Vec64i); \
    पक्ष 512: func(Vec128i); \
    स्वभाव: \
        CV_Error_(cv::Error::StsBadArg, ("Vectors of vectors with element size %d are not supported. Please, modify STD_VECTOR_SWITCH() macro\n", esz)); \
    }

/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

Mat आगत_Array_2::getMat_(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    यदि( k == MAT )
    {
        अचर Mat* m = (अचर Mat*)obj;
        यदि( i < 0 )
            फर्काऊ *m;
        फर्काऊ m->row(i);
    }

    यदि( k == UMAT )
    {
        अचर UMat* m = (अचर UMat*)obj;
        यदि( i < 0 )
            फर्काऊ m->getMat(accessFlags);
        फर्काऊ m->getMat(accessFlags).row(i);
    }

    यदि (k == MATX)
    {
        CV_Assert( i < 0 );
        फर्काऊ Mat(sz, flags, obj);
    }

    यदि( k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        पूर्णाङ्क t = CV_8U;
        अचर std::vector<तार्किक>& v = *(अचर std::vector<तार्किक>*)obj;
        पूर्णाङ्क j, n = (पूर्णाङ्क)v.size();
        यदि( n == 0 )
            फर्काऊ Mat();
        Mat m(1, &n, t);
        uchar* dst = m.data;
        लागि( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        फर्काऊ m;
    }

    यदि( k == NONE )
        फर्काऊ Mat();

    यदि( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        पूर्णाङ्क t = CV_MAT_TYPE(flags);
        पूर्णाङ्क esz = CV_ELEM_SIZE(t);

    #undef GET_MAT
    #define GET_MAT(T) \
        { \
            std::vector<T>* v; \
            यदि (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = \
                    reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                CV_Assert(माप_प्रकार(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            पूर्णाङ्क v_sz = पूर्णाङ्क(v->size()); \
            फर्काऊ v->empty() ? Mat() : Mat(1, &v_sz, t, v->data()); \
        }

        STD_VECTOR_SWITCH(esz, GET_MAT)
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& v = *(अचर std::vector<Mat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णाङ्क)v.size() );

        फर्काऊ v[i];
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* v = (अचर Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );

        फर्काऊ v[i];
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& v = *(अचर std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णाङ्क)v.size() );

        फर्काऊ v[i].getMat(accessFlags);
    }

    यदि( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
    }

    यदि( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call download method for cuda::GpuMatND object");
    }

    यदि( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );

        अचर cuda::HostMem* cuda_mem = (अचर cuda::HostMem*)obj;

        फर्काऊ cuda_mem->createMatHeader();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

UMat आगत_Array_2::getUMat(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    यदि( k == UMAT )
    {
        अचर UMat* m = (अचर UMat*)obj;
        यदि( i < 0 )
            फर्काऊ *m;
        फर्काऊ m->row(i);
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& v = *(अचर std::vector<UMat>*)obj;
        CV_Assert( 0 <= i && i < (पूर्णाङ्क)v.size() );

        फर्काऊ v[i];
    }

    यदि( k == MAT )
    {
        अचर Mat* m = (अचर Mat*)obj;
        यदि( i < 0 )
            फर्काऊ m->getUMat(accessFlags);
        फर्काऊ m->row(i).getUMat(accessFlags);
    }

    फर्काऊ getMat(i).getUMat(accessFlags);
}

शून्य आगत_Array_2::getMatVector(std::vector<Mat>& mv) अचर
{
    आगत_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    यदि( k == MAT )
    {
        अचर Mat& m = *(अचर Mat*)obj;
        पूर्णाङ्क n = (पूर्णाङ्क)m.size[0];
        mv.resize(n);
        CV_Assert(m.dims >= 2);

        लागि( पूर्णाङ्क i = 0; i < n; i++ )
            mv[i] = m.dims <= 2 ? Mat(1, m.cols, m.type(), (शून्य*)m.ptr(i)) :
                Mat(m.dims-1, &m.size[1], m.type(), (शून्य*)m.ptr(i), &m.step[1]);
        फर्काऊ;
    }

    यदि (k == MATX)
    {
        माप_प्रकार n = sz.height, esz = CV_ELEM_SIZE(flags);
        mv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = Mat(1, sz.width, CV_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        फर्काऊ;
    }

    यदि( k == STD_VECTOR )
    {
        अचर std::vector<uchar>& v = *(अचर std::vector<uchar>*)obj;

        माप_प्रकार n = total(), esz = CV_ELEM_SIZE(flags);
        पूर्णाङ्क t = CV_MAT_DEPTH(flags), cn = CV_MAT_CN(flags);
        mv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = Mat(1, cn, t, (शून्य*)(v.data() + esz*i));
        फर्काऊ;
    }

    यदि( k == NONE )
    {
        mv.clear();
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_VECTOR )
    {
        पूर्णाङ्क typ = CV_MAT_TYPE(flags);
        पूर्णाङ्क esz = CV_ELEM_SIZE(typ);

        #undef GET_VEC_VEC_MAT_VEC
        #define GET_VEC_VEC_MAT_VEC(T) \
        { \
            अचर std::vector<std::vector<T > >* vv = (अचर std::vector<std::vector<T > >*)obj; \
            माप_प्रकार n = vv->size(); \
            mv.resize(n); \
            लागि (माप_प्रकार i = 0; i < n; i++) { \
                अचर std::vector<T >& vi = vv->at(i); \
                CV_Assert(vi.size() <= (माप_प्रकार)पूर्णांक_उच्च); \
                पूर्णाङ्क ni = (पूर्णाङ्क)vi.size(); \
                mv[i] = ni > 0 ? Mat(1, &ni, typ, (शून्य*)vi.data()) : Mat(); \
            } \
        } \
        रोक

        STD_VECTOR_SWITCH(esz, GET_VEC_VEC_MAT_VEC)
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& v = *(अचर std::vector<Mat>*)obj;
        माप_प्रकार n = v.size();
        mv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i];
        फर्काऊ;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* v = (अचर Mat*)obj;
        माप_प्रकार n = sz.height;
        mv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i];
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& v = *(अचर std::vector<UMat>*)obj;
        माप_प्रकार n = v.size();
        mv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        फर्काऊ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य आगत_Array_2::getUMatVector(std::vector<UMat>& umv) अचर
{
    आगत_Array_2::KindFlag k = kind();
    AccessFlag accessFlags = flags & ACCESS_MASK;

    यदि( k == NONE )
    {
        umv.clear();
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& v = *(अचर std::vector<Mat>*)obj;
        माप_प्रकार n = v.size();
        umv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        फर्काऊ;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* v = (अचर Mat*)obj;
        माप_प्रकार n = sz.height;
        umv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& v = *(अचर std::vector<UMat>*)obj;
        माप_प्रकार n = v.size();
        umv.resize(n);

        लागि( माप_प्रकार i = 0; i < n; i++ )
            umv[i] = v[i];
        फर्काऊ;
    }

    यदि( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        फर्काऊ;
    }
    यदि( k == MAT )
    {
        Mat& v = *(Mat*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        फर्काऊ;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat आगत_Array_2::getGpuMat() अचर
{
#ifdef HAVE_CUDA
    आगत_Array_2::KindFlag k = kind();

    यदि (k == CUDA_GPU_MAT)
    {
        अचर cuda::GpuMat* d_mat = (अचर cuda::GpuMat*)obj;
        फर्काऊ *d_mat;
    }

    यदि (k == CUDA_HOST_MEM)
    {
        अचर cuda::HostMem* cuda_mem = (अचर cuda::HostMem*)obj;
        फर्काऊ cuda_mem->createGpuMatHeader();
    }

    यदि (k == OPENGL_BUFFER)
    {
        CV_Error(cv::Error::StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
    }

    यदि (k == NONE)
        फर्काऊ cuda::GpuMat();

    CV_Error(cv::Error::StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
शून्य आगत_Array_2::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) अचर
{
#ifdef HAVE_CUDA
    आगत_Array_2::KindFlag k = kind();
    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
#अन्यथा
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
शून्य आगत_Array_2::getGpuMatNDVector(std::vector<cuda::GpuMatND>& gpumv) अचर
{
#ifdef HAVE_CUDA
    आगत_Array_2::KindFlag k = kind();
    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        gpumv = *(std::vector<cuda::GpuMatND>*)obj;
    }
#अन्यथा
    CV_UNUSED(gpumv);
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
cuda::GpuMatND आगत_Array_2::getGpuMatND() अचर
{
#ifdef HAVE_CUDA
    आगत_Array_2::KindFlag k = kind();

    यदि (k == CUDA_GPU_MATND)
    {
        अचर cuda::GpuMatND* d_mat = (अचर cuda::GpuMatND*)obj;
        फर्काऊ *d_mat;
    }

    CV_Error(cv::Error::StsNotImplemented, "getGpuMatND is available only for cuda::GpuMatND");
#अन्यथा
    CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
}
ogl::Buffer आगत_Array_2::getOGlBuffer() अचर
{
    आगत_Array_2::KindFlag k = kind();

    CV_Assert(k == OPENGL_BUFFER);

    अचर ogl::Buffer* gl_buf = (अचर ogl::Buffer*)obj;
    फर्काऊ *gl_buf;
}

पूर्णाङ्क आगत_Array_2::rows_(पूर्णाङ्क i) अचर
{
#ifdef HAVE_CUDA
    आगत_Array_2::KindFlag k = kind();
    यदि (k == CUDA_GPU_MATND)
    {
        अचर cuda::GpuMatND& _gpuMatND = *(अचर cuda::GpuMatND*)obj;
        फर्काऊ (_gpuMatND.dims < 1) ? 0 : _gpuMatND.size[0];
    }
#endif

    फर्काऊ size_(i).height;
}

पूर्णाङ्क आगत_Array_2::cols_(पूर्णाङ्क i) अचर
{
#ifdef HAVE_CUDA
    आगत_Array_2::KindFlag k = kind();
    यदि (k == CUDA_GPU_MATND)
    {
        अचर cuda::GpuMatND& _gpuMatND = *(अचर cuda::GpuMatND*)obj;
        फर्काऊ (_gpuMatND.dims < 2) ? 0 : _gpuMatND.size[1];
    }
#endif

    फर्काऊ size_(i).width;
}

Size आगत_Array_2::size_(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        अचर Mat* m = (अचर Mat*)obj;
        CV_Assert(m->dims <= 2);
        फर्काऊ Size(m->cols, m->rows);
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        अचर UMat* m = (अचर UMat*)obj;
        CV_Assert(m->dims <= 2);
        फर्काऊ Size(m->cols, m->rows);
    }

    यदि (k == MATX)
    {
        CV_Assert( i < 0 );
        फर्काऊ sz;
    }

    यदि( k == NONE )
        फर्काऊ Size();

    यदि( k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
    {
        माप_प्रकार n = total(i);
        CV_Assert(n <= (माप_प्रकार)पूर्णांक_उच्च);
        फर्काऊ Size((पूर्णाङ्क)n, 1);
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        यदि( i < 0 )
            फर्काऊ vv.empty() ? Size() : Size((पूर्णाङ्क)vv.size(), 1);
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        फर्काऊ vv[i].size();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        यदि( i < 0 )
            फर्काऊ sz.height==0 ? Size() : Size(sz.height, 1);
        CV_Assert( i < sz.height );

        फर्काऊ vv[i].size();
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        अचर std::vector<cuda::GpuMat>& vv = *(अचर std::vector<cuda::GpuMat>*)obj;
        यदि (i < 0)
            फर्काऊ vv.empty() ? Size() : Size((पूर्णाङ्क)vv.size(), 1);
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        फर्काऊ vv[i].size();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        अचर std::vector<cuda::GpuMatND>& vv = *(अचर std::vector<cuda::GpuMatND>*)obj;
        यदि (i < 0)
            फर्काऊ vv.empty() ? Size() : Size((पूर्णाङ्क)vv.size(), 1);
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        अचर cuda::GpuMatND& m = vv[i];
        यदि (m.dims == 0)
            फर्काऊ Size();
        यदि (m.dims == 1)
            फर्काऊ Size(1, m.size[0]);
        फर्काऊ Size(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        यदि( i < 0 )
            फर्काऊ vv.empty() ? Size() : Size((पूर्णाङ्क)vv.size(), 1);
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        फर्काऊ vv[i].size();
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        अचर std::vector<cuda::GpuMatND>& vv = *(अचर std::vector<cuda::GpuMatND>*)obj;
        यदि (i < 0)
            फर्काऊ vv.empty() ? Size() : Size((पूर्णाङ्क)vv.size(), 1);
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        अचर cuda::GpuMatND& m = vv[i];
        यदि (m.dims == 0)
            फर्काऊ Size();
        यदि (m.dims == 1)
            फर्काऊ Size(1, m.size[0]);
        फर्काऊ Size(m.size[1], m.size[0]);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        अचर ogl::Buffer* buf = (अचर ogl::Buffer*)obj;
        फर्काऊ buf->size();
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        अचर cuda::GpuMat* d_mat = (अचर cuda::GpuMat*)obj;
        फर्काऊ d_mat->size();
    }

    यदि( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        अचर cuda::HostMem* cuda_mem = (अचर cuda::HostMem*)obj;
        फर्काऊ cuda_mem->size();
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णाङ्क आगत_Array_2::sizend(पूर्णाङ्क* arrsz, पूर्णाङ्क i) अचर
{
    पूर्णाङ्क j, d = 0;
    आगत_Array_2::KindFlag k = kind();

    यदि( k == NONE )
        ;
    अन्यथा यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        अचर Mat& m = *(अचर Mat*)obj;
        d = m.dims;
        यदि(arrsz)
            लागि(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        अचर UMat& m = *(अचर UMat*)obj;
        d = m.dims;
        यदि(arrsz)
            लागि(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == STD_VECTOR_MAT && i >= 0 )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        अचर Mat& m = vv[i];
        d = m.dims;
        यदि(arrsz)
            लागि(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == STD_ARRAY_MAT && i >= 0 )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        CV_Assert( i < sz.height );
        अचर Mat& m = vv[i];
        d = m.dims;
        यदि(arrsz)
            लागि(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि( k == STD_VECTOR_UMAT && i >= 0 )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        अचर UMat& m = vv[i];
        d = m.dims;
        यदि(arrsz)
            लागि(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    अन्यथा यदि (((k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) && i < 0) ||
             (k == STD_VECTOR_VECTOR && i >= 0))
    {
        माप_प्रकार n = total(i);
        d = 1;
        यदि(arrsz)
        {
            arrsz[0] = पूर्णाङ्क(n);
        }
    }
    अन्यथा
    {
        CV_CheckLE(dims(i), 2, "Not supported");
        Size sz2d = size(i);
        d = 2;
        यदि(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    फर्काऊ d;
}

तार्किक आगत_Array_2::empty(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि (i >= 0) {
        यदि (k == STD_VECTOR_MAT) {
            स्वतः mv = reinterpret_cast<अचर std::vector<Mat>*>(obj);
            CV_Assert((माप_प्रकार)i < mv->size());
            फर्काऊ mv->at(i).empty();
        }
        अन्यथा यदि (k == STD_VECTOR_UMAT) {
            स्वतः umv = reinterpret_cast<अचर std::vector<UMat>*>(obj);
            CV_Assert((माप_प्रकार)i < umv->size());
            फर्काऊ umv->at(i).empty();
        }
        अन्यथा यदि (k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR) {
            माप_प्रकार n = total(i);
            फर्काऊ n == 0;
        } अन्यथा {
            CV_Error(Error::StsNotImplemented, "");
        }
    }
    फर्काऊ empty();
}

MatShape आगत_Array_2::shape(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();
    MatShape shape;

    यदि( k == NONE )
        ;
    अन्यथा यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        अचर Mat& m = *(अचर Mat*)obj;
        shape = m.size;
    }
    अन्यथा यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        अचर UMat& m = *(अचर UMat*)obj;
        shape = m.size;
    }
    अन्यथा यदि( k == STD_VECTOR_MAT && i >= 0 )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        CV_Assert( (माप_प्रकार)i < vv.size() );
        अचर Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा यदि( k == STD_ARRAY_MAT && i >= 0 )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        CV_Assert( i < sz.height );
        अचर Mat& m = vv[i];
        shape = m.size;
    }
    अन्यथा यदि( k == STD_VECTOR_UMAT && i >= 0 )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        CV_Assert( (माप_प्रकार)i < vv.size() );
        अचर UMat& m = vv[i];
        shape = m.size;
    }
    अन्यथा
    {
        पूर्णाङ्क sizes[CV_MAX_DIM];
        पूर्णाङ्क dims = sizend(sizes, i);
        shape = MatShape(dims, sizes);
    }

    फर्काऊ shape;
}

तार्किक आगत_Array_2::sameSize(अचर आगत_Array_2& arr) अचर
{
    आगत_Array_2::KindFlag k1 = kind(), k2 = arr.kind();
    Size sz1;

    यदि( k1 == MAT )
    {
        अचर Mat* m = ((अचर Mat*)obj);
        यदि( k2 == MAT )
            फर्काऊ m->size == ((अचर Mat*)arr.obj)->size;
        यदि( k2 == UMAT )
            फर्काऊ m->size == ((अचर UMat*)arr.obj)->size;
        यदि( m->dims > 2 )
            फर्काऊ असत्य;
        sz1 = m->size();
    }
    अन्यथा यदि( k1 == UMAT )
    {
        अचर UMat* m = ((अचर UMat*)obj);
        यदि( k2 == MAT )
            फर्काऊ m->size == ((अचर Mat*)arr.obj)->size;
        यदि( k2 == UMAT )
            फर्काऊ m->size == ((अचर UMat*)arr.obj)->size;
        यदि( m->dims > 2 )
            फर्काऊ असत्य;
        sz1 = m->size();
    }
    अन्यथा यदि ( (k1 == CUDA_GPU_MATND) && (k2 == CUDA_GPU_MATND))
    {
        फर्काऊ ((अचर cuda::GpuMatND*)obj)->size == ((अचर cuda::GpuMatND*)arr.obj)->size;
    }
    अन्यथा
        sz1 = size();

    यदि( arr.dims() > 2 )
        फर्काऊ असत्य;
    फर्काऊ sz1 == arr.size();
}

पूर्णाङ्क आगत_Array_2::dims_(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर Mat*)obj)->dims;
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर UMat*)obj)->dims;
    }

    यदि (k == MATX)
    {
        CV_Assert( i < 0 );
        फर्काऊ 2;
    }

    यदि( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CV_Assert( i < 0 );
        फर्काऊ 1;
    }

    यदि( k == NONE )
        फर्काऊ 0;

    यदि( k == STD_VECTOR_VECTOR )
    {
        यदि( i < 0 )
            फर्काऊ 1;
        CV_Assert( माप_प्रकार(i) < total() );
        फर्काऊ 1;
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        यदि( i < 0 )
            फर्काऊ 1;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        फर्काऊ vv[i].dims;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        यदि( i < 0 )
            फर्काऊ 1;
        CV_Assert( i < sz.height );

        फर्काऊ vv[i].dims;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        यदि( i < 0 )
            फर्काऊ 1;
        CV_Assert( i < (पूर्णाङ्क)vv.size() );

        फर्काऊ vv[i].dims;
    }

    यदि( k == OPENGL_BUFFER )
    {
        CV_Assert( i < 0 );
        फर्काऊ 2;
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ 2;
    }

    यदि( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर cuda::GpuMatND*)obj)->dims;
    }

    यदि( k == CUDA_HOST_MEM )
    {
        CV_Assert( i < 0 );
        फर्काऊ 2;
    }

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

माप_प्रकार आगत_Array_2::total(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर Mat*)obj)->total();
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर UMat*)obj)->total();
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        यदि( i < 0 )
            फर्काऊ vv.size();

        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        फर्काऊ vv[i].total();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        यदि( i < 0 )
            फर्काऊ sz.height;

        CV_Assert( i < sz.height );
        फर्काऊ vv[i].total();
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        यदि( i < 0 )
            फर्काऊ vv.size();

        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        फर्काऊ vv[i].total();
    }

    यदि( k == CUDA_GPU_MATND )
    {
        CV_Assert( i < 0 );
        माप_प्रकार res = 0;
        अचर cuda::GpuMatND& _gpuMatND = *((अचर cuda::GpuMatND*)obj);
        यदि (_gpuMatND.dims > 0)
        {
            res = 1;
            लागि(पूर्णाङ्क d = 0 ; d<_gpuMatND.dims ; ++d)
                res *= _gpuMatND.size[d];
            फर्काऊ res;
        }
    }

    यदि (k == STD_VECTOR || k == STD_VECTOR_VECTOR)
    {
        CV_Assert(i < 0 || k == STD_VECTOR_VECTOR);
        पूर्णाङ्क esz = CV_ELEM_SIZE(flags);

        #undef GET_VEC_SIZE
        #define GET_VEC_SIZE(T) \
            यदि (k == STD_VECTOR_VECTOR) { \
                अचर std::vector<std::vector<T > >* vv = (अचर std::vector<std::vector<T > >*)obj; \
                माप_प्रकार n = vv->size(); \
                यदि (i < 0) \
                    फर्काऊ n; \
                CV_Assert((माप_प्रकार)i < n); \
                फर्काऊ vv->at(i).size(); \
            } अन्यथा \
                फर्काऊ ((अचर std::vector<T >*)obj)->size()

        STD_VECTOR_SWITCH(esz, GET_VEC_SIZE)
    }

    फर्काऊ size(i).area();
}

पूर्णाङ्क आगत_Array_2::type_(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
        फर्काऊ ((अचर Mat*)obj)->type();

    यदि( k == UMAT )
        फर्काऊ ((अचर UMat*)obj)->type();

    यदि( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        फर्काऊ CV_MAT_TYPE(flags);

    यदि( k == NONE )
        फर्काऊ -1;

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        यदि( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            फर्काऊ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        फर्काऊ vv[i >= 0 ? i : 0].type();
    }

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        यदि( vv.empty() )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            फर्काऊ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < (पूर्णाङ्क)vv.size() );
        फर्काऊ vv[i >= 0 ? i : 0].type();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        यदि( sz.height == 0 )
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            फर्काऊ CV_MAT_TYPE(flags);
        }
        CV_Assert( i < sz.height );
        फर्काऊ vv[i >= 0 ? i : 0].type();
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        अचर std::vector<cuda::GpuMat>& vv = *(अचर std::vector<cuda::GpuMat>*)obj;
        यदि (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            फर्काऊ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        फर्काऊ vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
#ifdef HAVE_CUDA
        अचर std::vector<cuda::GpuMatND>& vv = *(अचर std::vector<cuda::GpuMatND>*)obj;
        यदि (vv.empty())
        {
            CV_Assert((flags & FIXED_TYPE) != 0);
            फर्काऊ CV_MAT_TYPE(flags);
        }
        CV_Assert(i < (पूर्णाङ्क)vv.size());
        फर्काऊ vv[i >= 0 ? i : 0].type();
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि( k == OPENGL_BUFFER )
        फर्काऊ ((अचर ogl::Buffer*)obj)->type();

    यदि( k == CUDA_GPU_MAT )
        फर्काऊ ((अचर cuda::GpuMat*)obj)->type();

    यदि( k == CUDA_GPU_MATND )
        फर्काऊ ((अचर cuda::GpuMatND*)obj)->type();

    यदि( k == CUDA_HOST_MEM )
        फर्काऊ ((अचर cuda::HostMem*)obj)->type();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

पूर्णाङ्क आगत_Array_2::depth_(पूर्णाङ्क i) अचर
{
    फर्काऊ CV_MAT_DEPTH(type(i));
}

तार्किक आगत_Array_2::empty_() अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
        फर्काऊ ((अचर Mat*)obj)->empty();

    यदि( k == UMAT )
        फर्काऊ ((अचर UMat*)obj)->empty();

    यदि (k == MATX)
        फर्काऊ असत्य;

    यदि( k == STD_VECTOR || k == STD_BOOL_VECTOR || k == STD_VECTOR_VECTOR )
    {
        माप_प्रकार n = total(-1);
        फर्काऊ n == 0;
    }

    यदि( k == NONE )
        फर्काऊ सत्य;

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        फर्काऊ vv.empty();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        फर्काऊ sz.height == 0;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        फर्काऊ vv.empty();
    }

    यदि( k == OPENGL_BUFFER )
        फर्काऊ ((अचर ogl::Buffer*)obj)->empty();

    यदि( k == CUDA_GPU_MAT )
        फर्काऊ ((अचर cuda::GpuMat*)obj)->empty();

    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        अचर std::vector<cuda::GpuMat>& vv = *(अचर std::vector<cuda::GpuMat>*)obj;
        फर्काऊ vv.empty();
    }

    यदि( k == CUDA_GPU_MATND )
        फर्काऊ ((अचर cuda::GpuMatND*)obj)->empty();

    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        अचर std::vector<cuda::GpuMatND>& vv = *(अचर std::vector<cuda::GpuMatND>*)obj;
        फर्काऊ vv.empty();
    }

    यदि( k == CUDA_HOST_MEM )
        फर्काऊ ((अचर cuda::HostMem*)obj)->empty();

    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

तार्किक आगत_Array_2::isContinuous(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
        फर्काऊ i < 0 ? ((अचर Mat*)obj)->isContinuous() : सत्य;

    यदि( k == UMAT )
        फर्काऊ i < 0 ? ((अचर UMat*)obj)->isContinuous() : सत्य;

    यदि( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        फर्काऊ सत्य;

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].isContinuous();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        फर्काऊ vv[i].isContinuous();
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].isContinuous();
    }

    यदि( k == CUDA_GPU_MAT )
      फर्काऊ i < 0 ? ((अचर cuda::GpuMat*)obj)->isContinuous() : सत्य;

    यदि( k == CUDA_GPU_MATND )
        फर्काऊ i < 0 ? ((अचर cuda::GpuMatND*)obj)->isContinuous() : सत्य;

    CV_Error(cv::Error::StsNotImplemented, "Unknown/unsupported array type");
}

तार्किक आगत_Array_2::isSubmatrix(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
        फर्काऊ i < 0 ? ((अचर Mat*)obj)->isSubmatrix() : असत्य;

    यदि( k == UMAT )
        फर्काऊ i < 0 ? ((अचर UMat*)obj)->isSubmatrix() : असत्य;

    यदि( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        फर्काऊ असत्य;

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].isSubmatrix();
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        CV_Assert(i >= 0 && i < sz.height);
        फर्काऊ vv[i].isSubmatrix();
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].isSubmatrix();
    }

    यदि( k == CUDA_GPU_MATND )
    {
        फर्काऊ ((अचर cuda::GpuMatND*)obj)->isSubmatrix();
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        अचर std::vector<cuda::GpuMatND>& vv = *(अचर std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].isSubmatrix();
    }

    CV_Error(cv::Error::StsNotImplemented, "");
}

माप_प्रकार आगत_Array_2::offset(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        अचर Mat * अचर m = ((अचर Mat*)obj);
        फर्काऊ (माप_प्रकार)(m->ptr() - m->datastart);
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर UMat*)obj)->offset;
    }

    यदि( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        फर्काऊ 0;

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णाङ्क)vv.size() );

        फर्काऊ (माप_प्रकार)(vv[i].ptr() - vv[i].datastart);
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        फर्काऊ (माप_प्रकार)(vv[i].ptr() - vv[i].datastart);
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].offset;
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        अचर cuda::GpuMat * अचर m = ((अचर cuda::GpuMat*)obj);
        फर्काऊ (माप_प्रकार)(m->data - m->datastart);
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        अचर std::vector<cuda::GpuMat>& vv = *(अचर std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ (माप_प्रकार)(vv[i].data - vv[i].datastart);
    }

    CV_Error(Error::StsNotImplemented, "");
}

माप_प्रकार आगत_Array_2::step(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर Mat*)obj)->step;
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर UMat*)obj)->step;
    }

    यदि( k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        फर्काऊ 0;

    यदि( k == STD_VECTOR_MAT )
    {
        अचर std::vector<Mat>& vv = *(अचर std::vector<Mat>*)obj;
        CV_Assert( i >= 0 && i < (पूर्णाङ्क)vv.size() );
        फर्काऊ vv[i].step;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        अचर Mat* vv = (अचर Mat*)obj;
        CV_Assert( i >= 0 && i < sz.height );
        फर्काऊ vv[i].step;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        अचर std::vector<UMat>& vv = *(अचर std::vector<UMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].step;
    }

    यदि( k == CUDA_GPU_MAT )
    {
        CV_Assert( i < 0 );
        फर्काऊ ((अचर cuda::GpuMat*)obj)->step;
    }
    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        अचर std::vector<cuda::GpuMat>& vv = *(अचर std::vector<cuda::GpuMat>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        फर्काऊ vv[i].step;
    }
    यदि( k == CUDA_GPU_MATND )
    {
        अचर cuda::GpuMatND& _gpuMatND = *(अचर cuda::GpuMatND*)obj;
        CV_Assert( i >= 0 && i < _gpuMatND.dims );
        फर्काऊ _gpuMatND.step[i];
    }

    यदि (k == STD_VECTOR_CUDA_GPU_MAT_ND)
    {
        अचर std::vector<cuda::GpuMatND>& vv = *(अचर std::vector<cuda::GpuMatND>*)obj;
        CV_Assert(i >= 0 && (माप_प्रकार)i < vv.size());
        अचर cuda::GpuMatND& _gpuMatND = vv[i];
        CV_Assert( _gpuMatND.dims > 0 );
        फर्काऊ _gpuMatND.step[0];
    }

    CV_Error(Error::StsNotImplemented, "");
}

शून्य आगत_Array_2::copyTo(अचर _OutputArray& arr) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == NONE )
        arr.release();
    अन्यथा यदि( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr);
    }
    अन्यथा यदि( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
#ifdef HAVE_CUDA
    अन्यथा यदि (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr);
#endif
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

शून्य आगत_Array_2::copyTo(अचर _OutputArray& arr, अचर आगत_Array_2 & mask) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == NONE )
        arr.release();
    अन्यथा यदि( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        Mat m = getMat();
        m.copyTo(arr, mask);
    }
    अन्यथा यदि( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
#ifdef HAVE_CUDA
    अन्यथा यदि (k == CUDA_GPU_MAT)
        ((cuda::GpuMat*)obj)->copyTo(arr, mask);
#endif
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

तार्किक _OutputArray::fixedSize() अचर
{
    फर्काऊ (flags & FIXED_SIZE) == FIXED_SIZE;
}

तार्किक _OutputArray::fixedType() अचर
{
    फर्काऊ (flags & FIXED_TYPE) == FIXED_TYPE;
}

शून्य _OutputArray::create_(Size _sz, पूर्णाङ्क mtype, पूर्णाङ्क i, तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.चालक()() == _sz);
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_sz, mtype);
        फर्काऊ;
    }
    यदि( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.चालक()() == _sz);
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        फर्काऊ;
    }
    यदि( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    यदि( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _sz.height) && (((cuda::GpuMatND*)obj)->size[1] == _sz.width)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_sz.height, _sz.width});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    यदि( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    यदि( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    पूर्णाङ्क sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::create_(पूर्णाङ्क _rows, पूर्णाङ्क _cols, पूर्णाङ्क mtype, पूर्णाङ्क i, तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((Mat*)obj)->size.चालक()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((Mat*)obj)->type() == mtype);
        ((Mat*)obj)->create(_rows, _cols, mtype);
        फर्काऊ;
    }
    यदि( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((UMat*)obj)->size.चालक()() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((UMat*)obj)->type() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        फर्काऊ;
    }
    यदि( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::GpuMat*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    यदि( k == CUDA_GPU_MATND && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((((cuda::GpuMatND*)obj)->dims == 2) && (((cuda::GpuMatND*)obj)->size[0] == _rows) && (((cuda::GpuMatND*)obj)->size[1] == _cols)));
        CV_Assert(!fixedType() || ((cuda::GpuMatND*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizes({_rows, _cols});
        ((cuda::GpuMatND*)obj)->create(sizes, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    यदि( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((ogl::Buffer*)obj)->type() == mtype);
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }
    यदि( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CV_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == Size(_cols, _rows));
        CV_Assert(!fixedType() || ((cuda::HostMem*)obj)->type() == mtype);
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    पूर्णाङ्क sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::create(पूर्णाङ्क d, अचर पूर्णाङ्क* sizes, पूर्णाङ्क mtype, पूर्णाङ्क i,
                          तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    पूर्णाङ्क size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    आगत_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    यदि( k == MAT )
    {
        CV_Assert( i < 0 );
        Mat& m = *(Mat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        फर्काऊ;
    }

    यदि( k == UMAT )
    {
        CV_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        CV_Assert(!(m.empty() && fixedType() && fixedSize()) && "Can't reallocate empty UMat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m.empty() && d <= 2 && m.dims <= 2 &&
            m.type() == mtype &&
            ((m.rows == size0 && m.cols == size1) ||
            (allowTransposed && m.rows == size1 && m.cols == size0 && m.isContinuous())))
        {
            फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_CheckTypeEQ(m.type(), CV_MAT_TYPE(mtype), "Can't reallocate UMat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixedSize())
        {
            CV_CheckEQ(m.dims, d, "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m.size[j], sizes[j], "Can't reallocate UMat with locked size (probably due to misused 'const' modifier)");
        }
        m.create(d, sizes, mtype);
        फर्काऊ;
    }

    यदि( k == MATX )
    {
        CV_Assert( i < 0 );
        पूर्णाङ्क type0 = CV_MAT_TYPE(flags);
        CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CV_CheckLE(d, 2, "");
        Size requested_size(d == 2 ? sizes[1] : 1, d >= 1 ? sizes[0] : 1);
        यदि (sz.width == 1 || sz.height == 1)
        {
            // NB: 1D arrays assume allowTransposed=true (see #4159)
            पूर्णाङ्क total_1d = std::max(sz.width, sz.height);
            CV_Check(requested_size, std::max(requested_size.width, requested_size.height) == total_1d, "");
        }
        अन्यथा
        {
            यदि (!allowTransposed)
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
        फर्काऊ;
    }

    यदि( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CV_Assert(!fixedSize());
        CV_Assert(k == STD_VECTOR_VECTOR || i < 0);
        CV_Assert( d <= 2 && (size0 == 1 || size1 == 1 || size0*size1 == 0) );
        माप_प्रकार len = size0*size1 > 0 ? size0 + size1 - 1 : 0;
        पूर्णाङ्क esz = CV_ELEM_SIZE(flags);

        यदि( k == STD_VECTOR || (k == STD_VECTOR_VECTOR && i >= 0) )
        {
            पूर्णाङ्क type0 = CV_MAT_TYPE(flags);
            CV_Assert( mtype == type0 || (CV_MAT_CN(mtype) == CV_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );
        }

    #undef RESIZE_VEC
    #define RESIZE_VEC(T) \
        { \
            std::vector<T>* v; \
            यदि (k == STD_VECTOR_VECTOR) { \
                std::vector<std::vector<T> >* vv = reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
                यदि (i < 0) { \
                    CV_Assert(!fixedSize() || len == vv->size()); \
                    vv->resize(len); \
                    फर्काऊ; \
                } \
                CV_Assert(माप_प्रकार(i) < vv->size()); \
                v = &vv->at(i); \
            } अन्यथा { \
                v = reinterpret_cast<std::vector<T>*>(obj); \
            } \
            CV_Assert(!fixedSize() || len == v->size()); \
            v->resize(len); \
        } \
        रोक

        STD_VECTOR_SWITCH(esz, RESIZE_VEC)
        फर्काऊ;
    }

    यदि( k == NONE )
    {
        CV_Error(cv::Error::StsNullPtr, "create() called for the missing output array" );
    }

    यदि( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;

        यदि( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            यदि( fixedType() )
            {
                पूर्णाङ्क _type = CV_MAT_TYPE(flags);
                लागि( माप_प्रकार j = len0; j < len; j++ )
                {
                    यदि( v[j].type() == _type )
                        जारी;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            फर्काऊ;
        }

        CV_Assert( i < (पूर्णाङ्क)v.size() );
        Mat& m = v[i];

        यदि( allowTransposed )
        {
            यदि( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            यदि( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        यदि(fixedSize())
        {
            CV_Assert(m.dims == d);
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        फर्काऊ;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;

        यदि( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = sz.height;

            CV_Assert(len == len0);
            यदि( fixedType() )
            {
                पूर्णाङ्क _type = CV_MAT_TYPE(flags);
                लागि( माप_प्रकार j = len0; j < len; j++ )
                {
                    यदि( v[j].type() == _type )
                        जारी;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            फर्काऊ;
        }

        CV_Assert( i < sz.height );
        Mat& m = v[i];

        यदि( allowTransposed )
        {
            यदि( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            यदि( d == 2 && m.dims == 2 && m.data &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }

        यदि(fixedSize())
        {
            CV_Assert(m.dims == d);
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        यदि( i < 0 )
        {
            CV_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            माप_प्रकार len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CV_Assert(!fixedSize() || len == len0);
            v.resize(len);
            यदि( fixedType() )
            {
                पूर्णाङ्क _type = CV_MAT_TYPE(flags);
                लागि( माप_प्रकार j = len0; j < len; j++ )
                {
                    यदि( v[j].type() == _type )
                        जारी;
                    CV_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CV_MAT_TYPE_MASK) | _type;
                }
            }
            फर्काऊ;
        }

        CV_Assert( i < (पूर्णाङ्क)v.size() );
        UMat& m = v[i];

        यदि( allowTransposed )
        {
            यदि( !m.isContinuous() )
            {
                CV_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            यदि( d == 2 && m.dims == 2 && m.u &&
                m.type() == mtype && m.rows == sizes[1] && m.cols == sizes[0] )
                फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m.channels() && ((1 << CV_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m.type();
            अन्यथा
                CV_Assert(CV_MAT_TYPE(mtype) == m.type());
        }
        यदि(fixedSize())
        {
            CV_Assert(m.dims == d);
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_Assert(m.size[j] == sizes[j]);
        }

        m.create(d, sizes, mtype);
        फर्काऊ;
    }

    यदि ((k == CUDA_GPU_MAT || k == CUDA_HOST_MEM) && d <= 2 &&
        i < 0 && !allowTransposed && fixedDepthMask == 0)
    {
        create((d < 2 ? 1 : sizes[0]), (d < 1 ? 1 : sizes[d > 1]),
                mtype, i, allowTransposed, fixedDepthMask);
        फर्काऊ;
    }

    यदि( k == CUDA_GPU_MATND && d > 0 && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
#ifdef HAVE_CUDA
        cuda::GpuMatND::SizeArray sizeArray = cuda::GpuMatND::SizeArray(sizes, sizes+d);
        ((cuda::GpuMatND*)obj)->create(sizeArray, mtype);
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }


    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य _OutputArray::create(अचर MatShape& shape, पूर्णाङ्क mtype, पूर्णाङ्क i,
                          तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    यदि (shape.dims < 0) {
        release();
    } अन्यथा {
        create(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);
    }
}

Mat _OutputArray::reinterpret(पूर्णाङ्क mtype) अचर
{
    mtype = CV_MAT_TYPE(mtype);
    फर्काऊ getMat().reinterpret(mtype);
}

शून्य _OutputArray::createSameSize(अचर आगत_Array_2& arr, पूर्णाङ्क mtype) अचर
{
    create(arr.shape(), mtype);
}

शून्य _OutputArray::fit(Size _sz, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    पूर्णाङ्क sizes[] = {_sz.height, _sz.width};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::fit(पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    पूर्णाङ्क sizes[] = {rows, cols};
    fit(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::fit(अचर MatShape& shape, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    यदि (shape.dims < 0) {
        release();
        फर्काऊ;
    }
    fit(shape.dims, shape.p, mtype, i, allowTransposed, fixedDepthMask);

    // fit(dims, sizes) resets the layout, so carry it over the same way Mat::fit(const MatShape&) does
    आगत_Array_2::KindFlag k = kind();
    यदि ((k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0)) {
        MatSize& msize = getMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    } अन्यथा यदि ((k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0)) {
        MatSize& msize = getUMatRef(i).size;
        msize.layout = shape.layout;
        msize.C = shape.C;
    }
}

शून्य _OutputArray::fitSameSize(अचर आगत_Array_2& arr, पूर्णाङ्क mtype) अचर
{
    fit(arr.shape(), mtype);
}

शून्य _OutputArray::fit(पूर्णाङ्क d, अचर पूर्णाङ्क* sizes, पूर्णाङ्क mtype, पूर्णाङ्क i,
                       तार्किक allowTransposed, _OutputArray::DepthMask fixedDepthMask) अचर
{
    पूर्णाङ्क size0 = d > 0 ? sizes[0] : 1, size1 = d > 1 ? sizes[1] : 1;
    आगत_Array_2::KindFlag k = kind();
    mtype = CV_MAT_TYPE(mtype);

    यदि( (k == MAT && i < 0) || (k == STD_VECTOR_MAT && i >= 0) )
    {
        Mat* m;
        यदि (k == MAT)
            m = (Mat*)obj;
        अन्यथा {
            std::vector<Mat>& v = *(std::vector<Mat>*)obj;
            CV_Assert((माप_प्रकार)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        फर्काऊ;
    }

    यदि( (k == UMAT && i < 0) || (k == STD_VECTOR_UMAT && i >= 0) )
    {
        UMat* m;
        यदि (k == UMAT)
            m = (UMat*)obj;
        अन्यथा {
            std::vector<UMat>& v = *(std::vector<UMat>*)obj;
            CV_Assert((माप_प्रकार)i < v.size());
            m = &v[i];
        }
        CV_Assert(!(m->empty() && fixedType() && fixedSize()) && "Can't reallocate empty Mat with locked layout (probably due to misused 'const' modifier)");
        यदि (!m->empty() && d <= 2 && m->dims <= 2 &&
            m->type() == mtype &&
            ((m->rows == size0 && m->cols == size1) ||
             (allowTransposed && m->rows == size1 && m->cols == size0 && m->isContinuous())))
        {
            फर्काऊ;
        }

        यदि(fixedType())
        {
            यदि(CV_MAT_CN(mtype) == m->channels() && ((1 << CV_MAT_DEPTH(flags)) & fixedDepthMask) != 0 )
                mtype = m->type();
            अन्यथा
                CV_CheckTypeEQ(m->type(), CV_MAT_TYPE(mtype), "Can't reallocate Mat with locked type (probably due to misused 'const' modifier)");
        }
        यदि(fixedSize())
        {
            CV_CheckEQ(m->dims, d, "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
            लागि(पूर्णाङ्क j = 0; j < d; ++j)
                CV_CheckEQ(m->size[j], sizes[j], "Can't reallocate Mat with locked size (probably due to misused 'const' modifier)");
        }
        m->fit(d, sizes, mtype);
        फर्काऊ;
    }

    create(d, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

शून्य _OutputArray::release() अचर
{
    CV_Assert(!fixedSize());

    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        ((Mat*)obj)->release();
        फर्काऊ;
    }

    यदि( k == UMAT )
    {
        ((UMat*)obj)->release();
        फर्काऊ;
    }

    यदि( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMat*)obj)->release();
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि( k == CUDA_GPU_MATND )
    {
#ifdef HAVE_CUDA
        ((cuda::GpuMatND*)obj)->release();
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि( k == CUDA_HOST_MEM )
    {
#ifdef HAVE_CUDA
        ((cuda::HostMem*)obj)->release();
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }

    यदि( k == OPENGL_BUFFER )
    {
#ifdef HAVE_OPENGL
        ((ogl::Buffer*)obj)->release();
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "OpenGL support is not enabled in this OpenCV build (missing HAVE_OPENGL)");
#endif
    }

    यदि( k == NONE )
        फर्काऊ;

    यदि( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
    #undef CLEAR_VEC
    #define CLEAR_VEC(T) \
        यदि (k == STD_VECTOR_VECTOR) { \
            std::vector<std::vector<T> >* vv = \
                reinterpret_cast<std::vector<std::vector<T> >*>(obj); \
            vv->clear(); \
        } अन्यथा { \
            std::vector<T>* v = reinterpret_cast<std::vector<T>*>(obj); \
            v->clear(); \
        } \
        रोक

        पूर्णाङ्क esz = CV_ELEM_SIZE(flags);
        STD_VECTOR_SWITCH(esz, CLEAR_VEC);
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_MAT )
    {
        ((std::vector<Mat>*)obj)->clear();
        फर्काऊ;
    }

    यदि( k == STD_ARRAY_MAT )
    {
        Mat* v = (Mat*)obj;
        लागि( पूर्णाङ्क i = 0; i < sz.height; i++ )
            v[i].release();
        फर्काऊ;
    }

    यदि( k == STD_BOOL_VECTOR )
    {
        ((std::vector<तार्किक>*)obj)->clear();
        फर्काऊ;
    }

    यदि( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        फर्काऊ;
    }
    यदि (k == STD_VECTOR_CUDA_GPU_MAT)
    {
#ifdef HAVE_CUDA
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        फर्काऊ;
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    CV_Error(Error::StsNotImplemented, "Unknown/unsupported array type");
}

शून्य _OutputArray::clear() अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == MAT )
    {
        CV_Assert(!fixedSize());
        ((Mat*)obj)->resize(0);
        फर्काऊ;
    }

    release();
}

तार्किक _OutputArray::needed() अचर
{
    फर्काऊ kind() != NONE;
}

Mat& _OutputArray::getMatRef(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि( i < 0 )
    {
        CV_Assert( k == MAT );
        फर्काऊ *(Mat*)obj;
    }

    CV_Assert( k == STD_VECTOR_MAT || k == STD_ARRAY_MAT );

    यदि( k == STD_VECTOR_MAT )
    {
        std::vector<Mat>& v = *(std::vector<Mat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)v.size() );
        फर्काऊ v[i];
    }
    अन्यथा
    {
        Mat* v = (Mat*)obj;
        CV_Assert( 0 <= i && i < sz.height );
        फर्काऊ v[i];
    }
}

UMat& _OutputArray::getUMatRef(पूर्णाङ्क i) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि( i < 0 )
    {
        CV_Assert( k == UMAT );
        फर्काऊ *(UMat*)obj;
    }
    अन्यथा
    {
        CV_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CV_Assert( i < (पूर्णाङ्क)v.size() );
        फर्काऊ v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() अचर
{
    आगत_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MAT );
    फर्काऊ *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() अचर
{
    आगत_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    फर्काऊ *(std::vector<cuda::GpuMat>*)obj;
}
cuda::GpuMatND& _OutputArray::getGpuMatNDRef() अचर
{
    आगत_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_GPU_MATND );
    फर्काऊ *(cuda::GpuMatND*)obj;
}

std::vector<cuda::GpuMatND>& _OutputArray::getGpuMatNDVecRef() अचर
{
    आगत_Array_2::KindFlag k = kind();
    CV_Assert(k == STD_VECTOR_CUDA_GPU_MAT_ND);
    फर्काऊ *(std::vector<cuda::GpuMatND>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() अचर
{
    आगत_Array_2::KindFlag k = kind();
    CV_Assert( k == OPENGL_BUFFER );
    फर्काऊ *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() अचर
{
    आगत_Array_2::KindFlag k = kind();
    CV_Assert( k == CUDA_HOST_MEM );
    फर्काऊ *(cuda::HostMem*)obj;
}

शून्य _OutputArray::setTo(अचर आगत_Array_2& arr, अचर आगत_Array_2 & mask) अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == NONE )
        ;
    अन्यथा यदि (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setTo(arr, mask);
    }
    अन्यथा यदि( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    अन्यथा यदि( k == CUDA_GPU_MAT )
    {
#ifdef HAVE_CUDA
        Mat value = arr.getMat();
        CV_Assert( checkScalar(value, type(), arr.kind(), आगत_Array_2::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<द्विगुण, 4>(value.ptr<द्विगुण>())), mask);
#अन्यथा
        CV_Error(Error::StsNotImplemented, "CUDA support is not enabled in this OpenCV build (missing HAVE_CUDA)");
#endif
    }
    अन्यथा
        CV_Error(Error::StsNotImplemented, "");
}

शून्य _OutputArray::setZero() अचर
{
    आगत_Array_2::KindFlag k = kind();

    यदि( k == NONE )
        ;
    अन्यथा यदि (k == MAT || k == MATX || k == STD_VECTOR)
    {
        Mat m = getMat();
        m.setZero();
    }
    अन्यथा
    {
        setTo(Scalar::all(0), noArray());
    }
}

शून्य _OutputArray::assign(अचर UMat& u) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    अन्यथा यदि (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
    }
    अन्यथा यदि (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(अचर Mat& m) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    अन्यथा यदि (k == MAT)
    {
        *(Mat*)obj = m;
    }
    अन्यथा यदि (k == MATX)
    {
        m.copyTo(getMat());
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::move(UMat& u) अचर
{
    यदि (fixedSize())
    {
        // TODO Performance warning
        assign(u);
        फर्काऊ;
    }
    पूर्णाङ्क k = kind();
    यदि (k == UMAT)
    {
        *(UMat*)obj = std::move(u);
    }
    अन्यथा यदि (k == MAT)
    {
        u.copyTo(*(Mat*)obj); // TODO check u.getMat()
        u.release();
    }
    अन्यथा यदि (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u.getMat()
        u.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::move(Mat& m) अचर
{
    यदि (fixedSize())
    {
        // TODO Performance warning
        assign(m);
        फर्काऊ;
    }
    पूर्णाङ्क k = kind();
    यदि (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
        m.release();
    }
    अन्यथा यदि (k == MAT)
    {
        *(Mat*)obj = std::move(m);
    }
    अन्यथा यदि (k == MATX)
    {
        m.copyTo(getMat());
        m.release();
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(अचर std::vector<UMat>& v) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        लागि (माप_प्रकार i = 0; i < v.size(); i++)
        {
            अचर UMat& m = v[i];
            UMat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा यदि (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        लागि (माप_प्रकार i = 0; i < v.size(); i++)
        {
            अचर UMat& m = v[i];
            Mat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


शून्य _OutputArray::assign(अचर std::vector<Mat>& v) अचर
{
    आगत_Array_2::KindFlag k = kind();
    यदि (k == STD_VECTOR_UMAT)
    {
        std::vector<UMat>& this_v = *(std::vector<UMat>*)obj;
        CV_Assert(this_v.size() == v.size());

        लागि (माप_प्रकार i = 0; i < v.size(); i++)
        {
            अचर Mat& m = v[i];
            UMat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा यदि (k == STD_VECTOR_MAT)
    {
        std::vector<Mat>& this_v = *(std::vector<Mat>*)obj;
        CV_Assert(this_v.size() == v.size());

        लागि (माप_प्रकार i = 0; i < v.size(); i++)
        {
            अचर Mat& m = v[i];
            Mat& this_m = this_v[i];
            यदि (this_m.u != रिक्त && this_m.u == m.u)
                जारी; // same object (see dnn::Layer::forward_fallback)
            m.copyTo(this_m);
        }
    }
    अन्यथा
    {
        CV_Error(Error::StsNotImplemented, "");
    }
}


स्थिर आगत_Output_Array_2 _none;
आगत_Output_Array noArray() { फर्काऊ _none; }

} // cv::
